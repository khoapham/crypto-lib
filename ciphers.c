#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/if_alg.h>

#include <openssl/aes.h>
#include <openssl/engine.h>

#include "e_apm.h"
#include "ciphers.h"
#include "aes.h"
#include "des.h"

#include "cryptouio/xgene_sec_uio.h"
#include "cryptouio/xgene_sec_alg.h"
#include "cryptouio/xgene_sec_sa.h"
#include "cryptouio/xgene_sec_port.h"

#define APM_AES_256_CBC         2
#define MAX_ACTIVE_OPS          256

int max_outstanding_ops = 2; //128;
void *buf[MAX_ACTIVE_OPS];

struct xgene_sec_uio_ctx *uioctx;
struct xgene_sec_session_ctx *sess;

void *data_buf;

static struct {
        int     id;
        int     nid;
        int     ivmax;
        int     keylen;
} ciphers[] = {
                {APM_AES_256_CBC,               NID_aes_256_cbc,        16,     16,  },
                { 0,                            NID_undef,              0,       0, },
};

int apm_CIPHER_init_key(EVP_CIPHER_CTX *ctx, const struct sockaddr_alg *sa, const unsigned char *key, const unsigned char *iv __U__, int enc __U__)
{
	TRACE("%s %p\n", __PRETTY_FUNCTION__, ctx);
//	int keylen = EVP_CIPHER_CTX_key_length(ctx);
//	struct apm_cipher_data *acd = CIPHER_DATA(ctx);
//
//	acd->op = -1;
//
//	if( ctx->encrypt )
//		acd->type = APM_OP_ENCRYPT;
//	else
//		acd->type = APM_OP_DECRYPT;
//
//	if((acd->tfmfd = socket(APM, SOCK_SEQPACKET, 0)) == -1)
//	{
//		TRACE("socket");
//		return 0;
//	}
//
//	if( bind(acd->tfmfd, (struct sockaddr *)sa, sizeof(struct sockaddr_alg)) == -1 )
//	{
//		TRACE("bind");
//		return 0;
//	}
//
//	if (setsockopt(acd->tfmfd, SOL_APM, APM_SET_KEY, key, keylen) == -1)
//	{
//		TRACE("setsockopt");
//		return 0;
//	}

        int uio_dev_id = 0;
        int rc;
        int i=0;

        uioctx = (struct xgene_sec_uio_ctx*)malloc(sizeof(struct xgene_sec_uio_ctx));

        uioctx->uioinfo.uio_num = uio_dev_id;
        rc = xgene_sec_uio_init(uioctx);

        sess = (struct xgene_sec_session_ctx *)malloc(sizeof(struct xgene_sec_session_ctx));
        if (!sess)
                return NULL;
        memset(sess, 0, sizeof(struct xgene_sec_session_ctx/**sess*/));

        if (!key && !ctx) {
                return NULL;
        }

        if (xgene_sec_session_init(uioctx, sess, MAX_OUTSTANDING_TKN) < 0){
                free (sess);
                return NULL;
        }

        xgene_sec_setkey_aes(sess, (unsigned char *)key, (unsigned int)ctx->key_len, SA_CRYPTO_MODE_AES_3DES_CBC, enc);

        data_buf = cryptouio_alloc_buffer (uioctx);
        memset (data_buf, 0, sizeof (data_buf));

	return 1;
}

int apm_CIPHER_cleanup_key(EVP_CIPHER_CTX *ctx)
{
	TRACE("%s %p\n", __PRETTY_FUNCTION__, ctx);
//	struct apm_cipher_data *acd = CIPHER_DATA(ctx);
//	if( acd->tfmfd != -1 )
//		close(acd->tfmfd);
//	if( acd->op != -1 )
//		close(acd->op);
        xgene_sec_session_deinit(uioctx, sess);
	return 1;
}


int apm_CIPHER_do_cipher(EVP_CIPHER_CTX *ctx, unsigned char *out_arg, const unsigned char *in_arg, size_t nbytes)
{
	TRACE("%s %p\n", __PRETTY_FUNCTION__, ctx);
//	struct apm_cipher_data *acd = CIPHER_DATA(ctx);
//	int block_size = EVP_CIPHER_CTX_block_size(ctx);
//	struct msghdr msg = {.msg_name = NULL};
//	struct cmsghdr *cmsg;
//	struct apm_iv *ivm;
//	struct iovec iov;
//	char buf[CMSG_SPACE(sizeof(acd->type)) + CMSG_SPACE(offsetof(struct apm_iv, iv) + block_size)];
//	ssize_t len;
//	unsigned char save_iv[block_size];
//
//	memset(buf, 0, sizeof(buf));
//
//	msg.msg_control = buf;
//	msg.msg_controllen = 0;
//	msg.msg_controllen = sizeof(buf);
//	if( acd->op == -1 )
//	{
//		if((acd->op = accept(acd->tfmfd, NULL, 0)) == -1)
//			return 0;
//	}
//	/* set operation type encrypt|decrypt */
//	cmsg = CMSG_FIRSTHDR(&msg);
//	cmsg->cmsg_level = SOL_APM;
//	cmsg->cmsg_type = APM_SET_OP;
//	cmsg->cmsg_len = CMSG_LEN(4);
//	memcpy(CMSG_DATA(cmsg),&acd->type, 4);
//
//	/* set IV - or update if it was set before */
//	if(!ctx->encrypt)
//		memcpy(save_iv, in_arg + nbytes - block_size, block_size);
//
//	cmsg = CMSG_NXTHDR(&msg, cmsg);
//	cmsg->cmsg_level = SOL_APM;
//	cmsg->cmsg_type = APM_SET_IV;
//	cmsg->cmsg_len = CMSG_LEN(offsetof(struct apm_iv, iv) + block_size);
//	ivm = (void*)CMSG_DATA(cmsg);
//	ivm->ivlen = block_size;
//	memcpy(ivm->iv, ctx->iv, block_size);
//
//	msg.msg_iov = &iov;
//	msg.msg_iovlen = 1;
//
//	unsigned int todo = nbytes;
//	unsigned int done = 0;
//	while( todo-done > 0 )
//	{
//		iov.iov_base = (void *)(in_arg + done);
//		iov.iov_len = todo-done;
//
//		if((len = sendmsg(acd->op, &msg, 0)) == -1)
//			return 0;
//
//		if (read(acd->op, out_arg+done, len) != len)
//			return 0;
//
//		/* do not update IV for following chunks */
//		msg.msg_controllen = 0;
//		done += len;
//	}
//
//	/* copy IV for next iteration */
//	if(ctx->encrypt)
//		memcpy(ctx->iv, out_arg + done - block_size, block_size);
//	else
//		memcpy(ctx->iv, save_iv, block_size);
        int i=0;
        int rc;
        int command = 0;

        if (ctx->encrypt) {
                printf("INL: %d\n",nbytes);
                xgene_sec_dump("DATA BUFFER IN: ",in_arg, nbytes);
                memcpy (data_buf, in_arg /*input*/, nbytes);
                xgene_sec_dump("DATA BUFFER: ",data_buf, nbytes);
                rc = xgene_sec_encrypt(uioctx, sess, data_buf /*in*/, nbytes, ctx->iv, 16, NULL);
                printf("RC = %d\n",rc);
                if (rc == -ENOMEM) {
                        printf("Out of slot\n");
                        return -1;
                }
                if (rc != -EINPROGRESS) {
                        printf("Operation failed error %d\n", rc);
                        return -1;
                }
                usleep(1000000);	//???
                memcpy(out_arg, data_buf, nbytes);
                xgene_sec_dump("DATA BUFFER OUT: ",data_buf, nbytes);
                xgene_sec_dump("OUT : ", out_arg, nbytes);

        }
        else {
                printf("INL: %d\n",nbytes);
                memcpy (data_buf, in_arg, nbytes);
                rc = xgene_sec_decrypt(uioctx, sess, data_buf/*in*/, nbytes, ctx->iv, 16, NULL);
                if (rc == -ENOMEM)
                        return rc;
                if (rc != -EINPROGRESS) {
                        printf("Operation failed error %d\n", rc);
                        return -1;
                }
                usleep(10000);		//???
                memcpy(out_arg, data_buf, nbytes);
                xgene_sec_dump("DATA BUFFER OUT: ",data_buf, nbytes);
                xgene_sec_dump("OUT : ", out_arg, nbytes);


        }

        xgene_sec_queue2hw_flush(uioctx, command);

        while (i<10) {

                i++;
        }

//        xgene_sec_queue_process(uioctx, max_outstanding_ops, submit_completed /*NULL*/);

	return 1;
}

int apm_list_ciphers(ENGINE *e __U__, const EVP_CIPHER **cipher, const int **nids, int nid)
{
	TRACE("%s\n", __PRETTY_FUNCTION__);
	if( !cipher )
	{
		*nids = ciphers_used.data;
		return ciphers_used.len;
	}

	if( NID_store_contains(&ciphers_used, nid) == false )
		return 0;

	switch( nid )
	{
#define CASE(name)\
case NID_##name:\
	*cipher = &apm_##name;\
	break;
	CASE(aes_128_cbc);
	CASE(aes_192_cbc);
	CASE(aes_256_cbc);
	CASE(des_cbc);
	CASE(des_ede3_cbc);
#undef CASE
	default:
		*cipher = NULL;
	}
	TRACE("cipher %p\n", *cipher);
	return(*cipher != 0);
}

/**
 * DES
 */
DECLARE_CIPHER(des, cbc(des))
DECLARE_DES_EVP(des, 8)

DECLARE_CIPHER(des_ede3, cbc(des3_ede))
DECLARE_DES_EVP(des_ede3, 24)

/**
 * AES
 */
DECLARE_CIPHER(aes, cbc(aes))
#define EVP_CIPHER_block_size_CBC	AES_BLOCK_SIZE
DECLARE_AES_EVP(128,cbc,CBC);
DECLARE_AES_EVP(192,cbc,CBC);
DECLARE_AES_EVP(256,cbc,CBC);
#undef EVP_CIPHER_block_size_CBC

