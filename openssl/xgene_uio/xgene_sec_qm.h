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
#ifndef __XGENE_SEC_QM_H__
#define __XGENE_SEC_QM_H__

#include "xgene_sec_tkn.h"
#include "cdal_api_crypto_sk.h"

/*
 * Parameter for tuning
 *
 * The queue size is 64K. Each slot is 32 bytes. This leaves 2K slots.
 * Set to 1024 in case we need 64 bytes messages.
 */
#define MAX_SLOT		1024

struct xgene_sec_uio_ctx;
struct xgene_qmtm_msg32;

struct qm_queue {
	struct xgene_qmtm_msg32 *msg32;
	u32 *command;
	int qhead;
	int qtail;
	int count;
	int queue_id;
};

struct qm_ctx {
	struct qm_queue wq;
	struct qm_queue cq;
};

static inline u64 xgene_sec_encode2hwaddr(u64 paddr)
{
	return paddr >> 4;
}

static inline u64 xgene_sec_decode2hwaddr(u64 paddr)
{
	return paddr << 4;
}

int xgene_sec_qm_init(struct xgene_sec_uio_ctx *uio_ctx);
int xgene_sec_queue2hw(struct xgene_sec_uio_ctx *uio_ctx,
		       struct sec_tkn_ctx *tkn, void *src, int src_len,
		       void *dst);
int xgene_sec_queue2hw_authenc(struct xgene_sec_uio_ctx *uio_ctx,
			       struct sec_tkn_ctx *tkn,
			       cdal_sym_key_op_data_t * op_data, int iv_len,
			       int icvsize, int enc);
int xgene_sec_queue2hw_gcm(struct xgene_sec_uio_ctx *uio_ctx,
			   struct sec_tkn_ctx *tkn,
			   cdal_sym_key_op_data_t * op_data, int iv_len,
			   int icvsize, int enc);
void xgene_sec_queue2hw_flush(struct xgene_sec_uio_ctx *uio_ctx, u32 command);
int xgene_sec_queue_process(struct xgene_sec_uio_ctx *ctx, int budget,
			    void (*cb) (void *, int status));

#endif
