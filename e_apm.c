#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <memory.h>
#include <openssl/aes.h>
#include <openssl/engine.h>
#include <sys/socket.h>
#include <linux/if_alg.h>
#include <unistd.h>
#include <sys/param.h>
#include <ctype.h>
#include <stdbool.h>

#include "e_apm.h"
#include "ciphers.h"
#include "digests.h"


#define DYNAMIC_ENGINE
#define APM_ENGINE_ID	"apm"
#define APM_ENGINE_NAME	"use APM for XGENE crypto engine"

bool NID_store_contains(struct NID_store *store, int nid)
{
	TRACE("%s %p %i (%s) (%i)\n", __PRETTY_FUNCTION__, store, nid, OBJ_nid2sn(nid), (int)store->len);
	size_t i=0;
	for( i=0;i<store->len;i++ )
	{
		TRACE("%s ...\n", OBJ_nid2sn(store->data[i]));
		if( store->data[i] == nid )
			return true;
	}
	return false;
}

bool NID_store_add(struct NID_store *store, int nid)
{
	TRACE("%s %p %i (%s) (%i)\n", __PRETTY_FUNCTION__, store, nid, OBJ_nid2sn(nid), (int)store->len);
	int *r = malloc((store->len+1) * sizeof(int));
	memcpy(r, store->data, store->len * sizeof(int));
	free(store->data);
	store->data = r;
	store->data[store->len] = nid;
	store->len += 1;
	return true;
}

static int CIPHER_to_nid(const EVP_CIPHER *c)
{
	return EVP_CIPHER_nid(c);
}

static int MD_to_nid(const EVP_MD *d)
{
	return EVP_MD_type(d);
}

static bool NID_store_from_string(struct NID_store *store, struct NID_store *available, const char *names,
								  const void *(*by_name)(const char *),
								  int (*to_nid)(const void *))
{
	char *str, *r;
	char *c = NULL;
	r = str = strdup(names);
	while( (c = strtok_r(r, " ", &r)) != NULL )
	{
		const void *ec = by_name(c);
		if( ec == NULL )
		{
			/* the cipher/digest is unknown */
			TRACE("unknown %s\n", c);
			return false;
		}
		int nid = to_nid(ec);
		if( NID_store_contains(available, nid) == false )
			/* we do not support the cipher */
			return false;

		if( NID_store_add(store, nid) == false)
			return false;
	}
	return true;
}

int digest_nids[] = {
	NID_md4,
	NID_md5,
	NID_sha1,
	NID_sha224,
	NID_sha256,
	NID_sha512,
};

struct NID_store digests_available =
{
	.len = sizeof(digest_nids)/sizeof(digest_nids[0]),
	.data = digest_nids,
};

struct NID_store digests_used =
{
	.len = 0,
};

int cipher_nids[] = {
	NID_des_cbc,
	NID_des_ede3_cbc,
	NID_aes_128_cbc,
	NID_aes_192_cbc,
	NID_aes_256_cbc,
};

struct NID_store ciphers_available =
{
	.len = sizeof(cipher_nids)/sizeof(cipher_nids[0]),
	.data = cipher_nids,
};

struct NID_store ciphers_used =
{
	.len = 0,
};

int apm_init(ENGINE * engine __U__)
{
	TRACE("%s\n", __PRETTY_FUNCTION__);
	printf("Initializing the APM lib\n");
//	int sock;
//	if((sock = socket(APM, SOCK_SEQPACKET, 0)) == -1)
//		return 0;
//	close(sock);
	return 1;
}

int apm_finish(ENGINE * engine __U__)
{
	return 1;
}
/* The definitions for control commands specific to this engine */
#define APM_CMD_CIPHERS	ENGINE_CMD_BASE
#define APM_CMD_DIGESTS	(ENGINE_CMD_BASE + 1)

static const ENGINE_CMD_DEFN apm_cmd_defns[] = {
	{APM_CMD_CIPHERS,"CIPHERS","which ciphers to run",ENGINE_CMD_FLAG_STRING},
	{APM_CMD_DIGESTS,"DIGESTS","which digests to run",ENGINE_CMD_FLAG_STRING},
	{0, NULL, NULL, 0}
};

static int apm_ctrl(ENGINE *e, int cmd, long i __U__, void *p, void (*f)() __U__)
{
	TRACE("%s\n", __PRETTY_FUNCTION__);
	OpenSSL_add_all_algorithms();
	switch( cmd )
	{
	case APM_CMD_CIPHERS:
		if( p == NULL )
			return 1;
		if( NID_store_from_string(&ciphers_used, &ciphers_available, p, (void *)EVP_get_cipherbyname, (void *)CIPHER_to_nid) == false )
			return 0;
		ENGINE_unregister_ciphers(e);
		ENGINE_register_ciphers(e);
		return 1;
	case APM_CMD_DIGESTS:
		if( p == NULL )
			return 1;
		if( NID_store_from_string(&digests_used, &digests_available, p, (void *)EVP_get_digestbyname, (void *)MD_to_nid) == false )
			return 0;
		ENGINE_unregister_digests(e);
		ENGINE_register_digests(e);
		return 1;

	default:
		break;
	}
	return 0;
}

static int apm_bind_helper(ENGINE * e)
{
	TRACE("%s\n", __PRETTY_FUNCTION__);
	if( !ENGINE_set_id(e, APM_ENGINE_ID) ||
		!ENGINE_set_init_function(e, apm_init) ||
		!ENGINE_set_finish_function(e, apm_finish) ||
		!ENGINE_set_name(e, APM_ENGINE_NAME) ||
		!ENGINE_set_ciphers (e, apm_list_ciphers) ||
		!ENGINE_set_digests (e, apm_list_digests) ||
		!ENGINE_set_ctrl_function(e, apm_ctrl) ||
		!ENGINE_set_cmd_defns(e, apm_cmd_defns))
		return 0;
	return 1;
}

ENGINE *ENGINE_apm(void)
{
	TRACE("%s\n", __PRETTY_FUNCTION__);
	ENGINE *eng = ENGINE_new();
	if( !eng )
		return NULL;

	if( !apm_bind_helper(eng) )
	{
		ENGINE_free(eng);
		return NULL;
	}
	return eng;
}

static int apm_bind_fn(ENGINE *e, const char *id)
{
	TRACE("%s\n", __PRETTY_FUNCTION__);
	if( id && (strcmp(id, APM_ENGINE_ID) != 0) )
		return 0;

	if( !apm_bind_helper(e) )
		return 0;

	return 1;
}

IMPLEMENT_DYNAMIC_CHECK_FN()
IMPLEMENT_DYNAMIC_BIND_FN(apm_bind_fn)

