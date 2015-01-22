/*
 * AppliedMicro X-Gene User Space Driver
 *
 * Copyright (c) 2014, Applied Micro Circuits Corporation
 * All Rights Reserved.
 *
 * Author: Loc Ho <lho@apm.com>
 *
 * THIS WORK CONTAINS PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF
 * AppliedMicro AND IS SUBJECT TO THE TERMS OF NON-DISCLOSURE AGREEMENT
 * BETWEEN AppliedMicro AND THE COMPANY USING THIS FILE.
 *
 */
#ifndef __XGENE_SEC_ALG_H__
#define __XGENE_SEC_ALG_H__

#include "xgene_sec_uio.h"

/*
 * Parameter for tuning
 *
 * This is the reserved SA/token for this session for performance reason.
 * This allows the token to be pre-prepare. Please note that SA and token
 * come from kernel memory. If you run out, you will need to increase the
 * kernel SA/tkn memory allocation.
 */
#define MAX_OUTSTANDING_TKN		256
struct xgene_sec_session_ctx {
	struct sec_sa_item *sa[MAX_OUTSTANDING_TKN];
	struct sec_tkn_ctx *tkn[MAX_OUTSTANDING_TKN];
	int tkn_total;
	int tkn_next;
};

int xgene_sec_session_init(struct xgene_sec_uio_ctx *uio_ctx,
			   struct xgene_sec_session_ctx *sess,
			   int total_outstanding);
int xgene_sec_session_deinit(struct xgene_sec_uio_ctx *uio_ctx,
			     struct xgene_sec_session_ctx *sess);

int xgene_sec_setkey_aes(struct xgene_sec_session_ctx *session,
			 const unsigned char *key, unsigned int keylen,
			 unsigned char cm, int encrypt);
int xgene_sec_setkey_3des(struct xgene_sec_session_ctx *session,
			  const unsigned char *key, unsigned int keylen,
			  unsigned char cm, int encrypt);
int xgene_sec_encrypt(struct xgene_sec_uio_ctx *uio_ctx,
		      struct xgene_sec_session_ctx *session,
		      void *buffer, int len, void *iv, int iv_len, void *ctx);
int xgene_sec_decrypt(struct xgene_sec_uio_ctx *uio_ctx,
		      struct xgene_sec_session_ctx *session,
		      void *buffer, int len, void *iv, int iv_len, void *ctx);

int xgene_sec_hash_hmac_setkey(struct xgene_sec_session_ctx *session,
			       const unsigned char *key,
			       unsigned int keylen, unsigned char ha);
int xgene_sec_hash_digest(struct xgene_sec_uio_ctx *uio_ctx,
			  struct xgene_sec_session_ctx *session, void *buffer,
			  int len, void *icv, void *ctx);

int xgene_sec_setkey_cbc_hmac_authenc(struct xgene_sec_session_ctx *session, const unsigned char *enckey,	/* Key for Cipher */
				      unsigned int enckeylen, const unsigned char *mackey,	/* Key for Authenc */
				      unsigned int mackeylen,
				      unsigned char ca,
				      unsigned char ha, int encrypt);
int xgene_sec_encrypt_cbc_authenc(struct xgene_sec_uio_ctx *uio_ctx,
				  struct xgene_sec_session_ctx *session,
				  cdal_sym_key_op_data_t * op_data,
				  int iv_len, int icvsize, void *ctx);
int xgene_sec_decrypt_cbc_authenc(struct xgene_sec_uio_ctx *uio_ctx,
				  struct xgene_sec_session_ctx *session,
				  cdal_sym_key_op_data_t * op_data,
				  int iv_len, int icvsize, void *ctx);

int xgene_sec_setkey_aes_gcm(struct xgene_sec_session_ctx *session,
			     const unsigned char *key,
			     unsigned int keylen, int encrypt);
int xgene_sec_encrypt_aes_gcm(struct xgene_sec_uio_ctx *uio_ctx,
			      struct xgene_sec_session_ctx *session,
			      cdal_sym_key_op_data_t * op_data,
			      int iv_len, int icvsize, void *ctx);
int xgene_sec_decrypt_aes_gcm(struct xgene_sec_uio_ctx *uio_ctx,
			      struct xgene_sec_session_ctx *session,
			      cdal_sym_key_op_data_t * op_data,
			      int iv_len, int icvsize, void *ctx);
#endif
