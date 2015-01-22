struct apm_cipher_data
{
	int tfmfd;
	int op;
	uint32_t type;
};
#define CIPHER_DATA(ctx) ((struct apm_cipher_data*)(ctx->cipher_data))


#define DECLARE_CIPHER_INIT_KEY(alias, name)\
static int apm_##alias##_init_key (EVP_CIPHER_CTX *ctx,	\
	const unsigned char *key,   							\
	const unsigned char *iv __U__,  						\
	int enc __U__)  										\
{															\
	static struct sockaddr_alg sa = {  							\
		.salg_family = APM,  							\
		.salg_type = "skcipher",							\
		.salg_name = #name,  							\
	};														\
	return apm_CIPHER_init_key(ctx, &sa, key, iv, enc);	\
}

#define DEFINE_CIPHER_INIT_KEY(alias, name)\
static int apm_##alias##_init_key (EVP_CIPHER_CTX *ctx,	\
	const unsigned char *key,   							\
	const unsigned char *iv __U__,  						\
	int enc __U__);

#define DECLARE_CIPHER_CLEANUP_KEY(name)					\
static int apm_##name##_cleanup_key(EVP_CIPHER_CTX *ctx)	\
{   														\
	return apm_CIPHER_cleanup_key(ctx);					\
}

#define DEFINE_CIPHER_CLEANUP_KEY(name)						\
int apm_##name##_cleanup_key(EVP_CIPHER_CTX *ctx);


#define DECLARE_CIPHER_DO_CIPHER(name)						\
static int apm_##name##_do_cipher(EVP_CIPHER_CTX *ctx, 	\
unsigned char *out_arg, 									\
const unsigned char *in_arg, size_t nbytes)					\
{   														\
	return apm_CIPHER_do_cipher(ctx, out_arg, in_arg, nbytes);\
}

#define DEFINE_CIPHER_DO_CIPHER(name)						\
int apm_##name##_do_cipher(EVP_CIPHER_CTX *ctx);


#define DECLARE_CIPHER(name, param)\
DECLARE_CIPHER_INIT_KEY(name, param)\
DECLARE_CIPHER_CLEANUP_KEY(name)\
DECLARE_CIPHER_DO_CIPHER(name)

int apm_CIPHER_init_key(EVP_CIPHER_CTX *ctx, const struct sockaddr_alg *sa, const unsigned char *key, const unsigned char *iv __U__, int enc __U__);
int apm_CIPHER_cleanup_key(EVP_CIPHER_CTX *ctx);
int apm_CIPHER_do_cipher(EVP_CIPHER_CTX *ctx, unsigned char *out_arg, const unsigned char *in_arg, size_t nbytes);

int apm_list_ciphers(ENGINE *e, const EVP_CIPHER **cipher, const int **nids, int nid);
