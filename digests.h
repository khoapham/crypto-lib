struct apm_digest_data
{
	int tfmfd;
	int opfd;
};

#define DIGEST_DATA(ctx) ((struct apm_digest_data*)(ctx->md_data))

#define DECLARE_DIGEST_INIT(name)\
int apm_##name##_init(EVP_MD_CTX *ctx)					\
{															\
	static struct sockaddr_alg sa = {  						\
		.salg_family = APM,  							\
		.salg_type = "hash",								\
		.salg_name = #name,  							\
	};														\
	return apm_DIGEST_init(ctx, &sa);					\
}

#define DEFINE_DIGEST_INIT(name)\
int apm_##name##_init_key (EVP_MD_CTX *ctx);

#define DECLARE_DIGEST_UPDATE(name) 						\
int apm_##name##_update(EVP_MD_CTX *ctx, 				\
						   const void *data, size_t length) \
{   														\
	return apm_DIGEST_update(ctx, data, length); 		\
}

#define DEFINE_DIGEST_UPDATE(name) \
int apm_##name##_update(EVP_MD_CTX *ctx, const void *data, size_t length);


#define DECLARE_DIGEST_FINAL(name, digest_len) \
int apm_##name##_final(EVP_MD_CTX *ctx, unsigned char *md)\
{   														 \
	return apm_DIGEST_final(ctx, md, digest_len##_DIGEST_LENGTH);   			 \
}

#define DEFINE_DIGEST_FINAL(name) \
int apm_##name##_final(EVP_MD_CTX *ctx, unsigned char *md);

#define DECLARE_DIGEST_COPY(name) \
int apm_##name##_copy(EVP_MD_CTX *_to,const EVP_MD_CTX *_from) \
{   															  \
	return apm_DIGEST_copy(_to, _from);  					  \
}
#define DEFINE_DIGEST_COPY(name) \
int apm_##name##_copy(EVP_MD_CTX *_to,const EVP_MD_CTX *_from);

#define DECLARE_DIGEST_CLEANUP(name) \
int apm_##name##_cleanup(EVP_MD_CTX *ctx)  \
{   										\
	return apm_DIGEST_cleanup(ctx);  	\
}

#define DEFINE_DIGEST_CLEANUP(name) \
int apm_##name##_cleanup(EVP_MD_CTX *ctx);


#define DECLARE_DIGEST(name, digest_len)\
DECLARE_DIGEST_INIT(name)\
DECLARE_DIGEST_UPDATE(name)\
DECLARE_DIGEST_FINAL(name, digest_len)\
DECLARE_DIGEST_COPY(name)\
DECLARE_DIGEST_CLEANUP(name)

#define DEFINE_DIGEST(name)\
DEFINE_DIGEST_INIT(name)\
DEFINE_DIGEST_UPDATE(name)\
DEFINE_DIGEST_FINAL(name)\
DEFINE_DIGEST_COPY(name)\
DEFINE_DIGEST_CLEANUP(name)


#define	DECLARE_MD(digest, md, block)\
const EVP_MD apm_##digest##_md = {\
	.type = NID_##digest,\
	.pkey_type = NID_##digest##WithRSAEncryption,\
	.md_size = md##_DIGEST_LENGTH,\
	.flags = 0,\
	.init = apm_##digest##_init,\
	.update = apm_##digest##_update,\
	.final = apm_##digest##_final,\
	.copy = apm_##digest##_copy,\
	.cleanup = apm_##digest##_cleanup,\
	.sign = (evp_sign_method *)RSA_sign,\
	.verify = (evp_verify_method *)RSA_verify,\
	.required_pkey_type =\
	{\
		EVP_PKEY_RSA,\
		EVP_PKEY_RSA2,\
		0,\
		0\
	},\
	.block_size = block##_CBLOCK,\
	.ctx_size = sizeof(struct apm_digest_data),\
	.md_ctrl = NULL\
};

#define DEFINE_MD(digest)\
extern const EVP_MD apm_##digest##_md;

DEFINE_DIGEST(md4)
DEFINE_MD(md4)

DEFINE_DIGEST(md5)
DEFINE_MD(md5)

DEFINE_DIGEST(sha1)
DEFINE_MD(sha1)

DEFINE_DIGEST(sha224)
DEFINE_MD(sha224)

DEFINE_DIGEST(sha256)
DEFINE_MD(sha256)

DEFINE_DIGEST(sha512)
DEFINE_MD(sha512)

int apm_list_digests(ENGINE *e, const EVP_MD **digest, const int **nids, int nid);
