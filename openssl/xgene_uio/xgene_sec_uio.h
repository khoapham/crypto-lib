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
#ifndef __XGENE_SEC_UIO_H__
#define __XGENE_SEC_UIO_H__

#include "xgene_sec_qm.h"
#include "list.h"
#include "cdal_api_crypto_sk.h"

/*
 * Parameters for tuning
 *
 * SA is assumed to be 512 bytes. If you know that you never exceed an lower
 * value, changing this will allow for more session token/SA cache.
 * SA is assumed to be 512 bytes. If you know that you never exceed an lower
 * value, changing this will allow for more session token/SA cache.
 * Buffer is assumed to be 16K bytes. If you know that you never exceed an
 * lower value, you can change this and give you more buffer. Buffer comes
 * from kernel memory.
 */
#define SA_MAX_SIZE			512
#define TKN_MAX_SIZE		512
#define BUFFER_CHUNK_SIZE	(16 * 1024)
#define SHM_SIZE			(2 * BUFFER_CHUNK_SIZE)
#define SHM_CHUNK_SIZE		128

#define UIO_MAX_NAME_SIZE		64
#define UIO_MAX_NUM			255

#define UIO_INVALID_SIZE		-1
#define UIO_INVALID_ADDR		(~0)

#define UIO_MMAP_NOT_DONE		0
#define UIO_MMAP_OK			1
#define UIO_MMAP_FAILED			2

#define MMAP_QMFCSR_INDEX		0
#define MMAP_WQ_DESC_INDEX		1
#define MMAP_CQ_DESC_INDEX		2
#define MMAP_TKNSA_INDEX		3
#define MMAP_BUFFER_INDEX		4

/* This should be identical to the define in include/linux/uio_driver.h */
#define MAX_UIO_MAPS			5

struct uio_map_t {
	u64 addr;
	u64 size;
	int mmap_result;
	void *vaddr;
};

struct uio_info_t {
	int uio_num;
	int fd;
	struct uio_map_t maps[MAX_UIO_MAPS];
	unsigned long event_count;
	char name[UIO_MAX_NAME_SIZE];
	char version[UIO_MAX_NAME_SIZE];
};

struct xgene_sec_uio_ctx {
	struct uio_info_t uioinfo;
	struct qm_ctx qm_ctx;
	struct list_head buffer_head;
	struct list_head sa_head;
	struct list_head tkn_head;
	struct list_head shm_head;
};

int xgene_sec_uio_init(struct xgene_sec_uio_ctx *uio_ctx);
void xgene_sec_uio_deinit(struct xgene_sec_uio_ctx *uio_ctx);

static inline void *cryptouio_get_qmcmd_base(struct uio_info_t *info, int qid)
{
	return info->maps[MMAP_QMFCSR_INDEX].vaddr + (qid << 6) + 0x2c;
}

static inline void *cryptouio_get_wq_base(struct uio_info_t *info)
{
	return info->maps[MMAP_WQ_DESC_INDEX].vaddr;
}

static inline void *cryptouio_get_cq_base(struct uio_info_t *info)
{
	return info->maps[MMAP_CQ_DESC_INDEX].vaddr;
}

static inline u64 cryptouio_sa_xlate_phy(struct uio_info_t *info, void *vaddr)
{
	return (ulong) vaddr - (ulong) info->maps[MMAP_TKNSA_INDEX].vaddr +
	    info->maps[MMAP_TKNSA_INDEX].addr;
}

static inline u32 cryptouio_get_sa_size(struct uio_info_t *info)
{
	return 4 * MAX_SLOT * SA_MAX_SIZE;
}

static inline void *cryptouio_get_tkn_base(struct uio_info_t *info)
{
	return info->maps[MMAP_TKNSA_INDEX].vaddr;
}

static inline u64 cryptouio_tkn_xlate_phy(struct uio_info_t *info, void *vaddr)
{
	return (ulong) vaddr - (ulong) info->maps[MMAP_TKNSA_INDEX].vaddr +
	    info->maps[MMAP_TKNSA_INDEX].addr;
}

static inline void *cryptouio_tkn_xlate_va(struct uio_info_t *info, u64 paddr)
{
	return paddr - info->maps[MMAP_TKNSA_INDEX].addr +
	    info->maps[MMAP_TKNSA_INDEX].vaddr;
}

static inline u32 cryptouio_get_tkn_size(struct uio_info_t *info)
{
	return info->maps[MMAP_TKNSA_INDEX].size - cryptouio_get_sa_size(info);
}

static inline void *cryptouio_get_sa_base(struct uio_info_t *info)
{
	return info->maps[MMAP_TKNSA_INDEX].vaddr +
	    cryptouio_get_tkn_size(info);
}

static inline void *cryptouio_get_buffer_base(struct uio_info_t *info)
{
	return info->maps[MMAP_BUFFER_INDEX].vaddr;
}

static inline u32 cryptouio_get_buffer_size(struct uio_info_t *info)
{
	return (info->maps[MMAP_TKNSA_INDEX].size / BUFFER_CHUNK_SIZE) *
	    BUFFER_CHUNK_SIZE;
}

static inline u64 cryptouio_buffer_xlate_phy(struct uio_info_t *info, void *buf)
{
	return (ulong) buf - (ulong) info->maps[MMAP_BUFFER_INDEX].vaddr +
	    info->maps[MMAP_BUFFER_INDEX].addr;
}

static inline u64 cryptouio_shm_xlate_phy(struct uio_info_t *info, void *buf)
{
	return (ulong) buf - (ulong) info->maps[MMAP_BUFFER_INDEX].vaddr +
	    info->maps[MMAP_BUFFER_INDEX].addr;
}

void *cryptouio_alloc_shm(struct xgene_sec_uio_ctx *uio_ctx);
void cryptouio_free_shm(struct xgene_sec_uio_ctx *uio_ctx, void *buffer);
void *cryptouio_alloc_buffer(struct xgene_sec_uio_ctx *uio_ctx);
void cryptouio_free_buffer(struct xgene_sec_uio_ctx *uio_ctx, void *buffer);
void *cryptouio_alloc_tkn(struct xgene_sec_uio_ctx *uio_ctx);
void cryptouio_free_tkn(struct xgene_sec_uio_ctx *uio_ctx, void *tkn);
void *cryptouio_alloc_sa(struct xgene_sec_uio_ctx *uio_ctx);
void cryptouio_free_sa(struct xgene_sec_uio_ctx *uio_ctx, void *sa);

#endif
