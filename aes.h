#define	DECLARE_AES_EVP(ksize,lmode,umode)                  \
const EVP_CIPHER apm_aes_##ksize##_##lmode = {    		\
	.nid = NID_aes_##ksize##_##lmode,                       \
	.block_size = EVP_CIPHER_block_size_##umode,            \
	.key_len = AES_KEY_SIZE_##ksize,                        \
	.iv_len = AES_BLOCK_SIZE,                               \
	.flags = 0 | EVP_CIPH_##umode##_MODE,                   \
	.init = apm_aes_init_key,                            \
	.do_cipher = apm_aes_do_cipher,                      \
	.cleanup = apm_aes_cleanup_key,                      \
	.ctx_size = sizeof(struct apm_cipher_data),          \
	.set_asn1_parameters = EVP_CIPHER_set_asn1_iv,          \
	.get_asn1_parameters = EVP_CIPHER_get_asn1_iv,          \
	.ctrl = NULL,                                           \
	.app_data = NULL                                        \
};

#define	DEFINE_AES_EVP(ksize,lmode,umode)                  \
extern const EVP_CIPHER apm_aes_##ksize##_##lmode;

#define AES_KEY_SIZE_128        16
#define AES_KEY_SIZE_192        24
#define AES_KEY_SIZE_256        32

DEFINE_AES_EVP(128,cbc,CBC);
DEFINE_AES_EVP(192,cbc,CBC);
DEFINE_AES_EVP(256,cbc,CBC);
