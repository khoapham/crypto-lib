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
#ifndef __XGENE_SEC_PORT_H__
#define __XGENE_SEC_PORT_H__

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "list.h"

#if defined(__ARMEB__) || defined(__ARMEL__)
typedef unsigned long long u64;
#else
typedef unsigned long u64;
#endif
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

#if defined(__AARCH64EB__) || defined(__ARMEB__)
#define cpu_to_be32(x)	(x)
#define cpu_to_be16(x)	(x)
#define cpu_to_le32(x)	__builtin_bswap32(x)
#define cpu_to_le16(x)	__builtin_bswap16(x)
#define le32_to_cpu(x)	__builtin_bswap32(x)
#else
#define cpu_to_be32(x)	__builtin_bswap32(x)
#define cpu_to_be16(x)	__builtin_bswap16(x)
#define cpu_to_le32(x)	(x)
#define cpu_to_le16(x)	(x)
#define le32_to_cpu(x)	(x)
#endif

static inline void writel(u32 val, volatile void *addr)
{
	val = cpu_to_le32(val);
#if defined(__ARMEB__) || defined(__ARMEL__)
	asm volatile ("str %1, %0":"+Qo" (*(volatile u32 *)addr)
		      :"r"(val));
#else
	asm volatile ("str %w0, [%1]"::"r" (val), "r"(addr));
#endif
}

#define APM_SEC_DEBUG
#define APM_SEC_TXDEBUG
#define APM_SEC_RXDEBUG
#define APM_SEC_SATKNDEBUG
#define APM_SEC_QMDEBUG

/*
#undef APM_SEC_DEBUG
#undef APM_SEC_TXDEBUG
#undef APM_SEC_RXDEBUG
#undef APM_SEC_SATKNDEBUG
#undef APM_SEC_QMDEBUG
*/
/* Debugging Macro */
#define APMSEC_HDR			"XGSEC: "

#if !defined(APM_SEC_DEBUG)
# define APMSEC_LOG(fmt, ...)
#else
# define APMSEC_LOG(fmt, ...)		\
	do { \
		printf(APMSEC_HDR fmt, ##__VA_ARGS__); \
	} while(0);
#endif

#if !defined(APM_SEC_TXDEBUG)
# define APMSEC_TXLOG(fmt, ...)
# define APMSEC_TXDUMP(hdr, d, l)
#else
# define APMSEC_TXLOG(fmt, ...)		\
	do { \
		if (verbose) \
			printf(APMSEC_HDR fmt, ##__VA_ARGS__); \
	} while(0);
# define APMSEC_TXDUMP(hdr, d, l)	\
		if (verbose) \
			xgene_sec_dump(hdr, d, l)
#endif

#if !defined(APM_SEC_RXDEBUG)
# define APMSEC_RXLOG(fmt, ...)
# define APMSEC_RXDUMP(hdr, d, l)
#else
# define APMSEC_RXLOG(fmt, ...)		\
	do { \
		if (verbose) \
			printf(APMSEC_HDR fmt, ##__VA_ARGS__); \
	} while(0);
# define APMSEC_RXDUMP(hdr, d, l) 	\
		if (verbose) \
			xgene_sec_dump(hdr, d, l)
#endif

#if !defined(APM_SEC_SATKNDEBUG)
# define APMSEC_SATKNLOG(fmt, ...)
# define APMSEC_SADUMP(s, l)
# define APMSEC_TKNDUMP(t)
#else
# define APMSEC_SATKNLOG(fmt, ...)	\
	do { \
		if (verbose) \
			printf(APMSEC_HDR fmt, ##__VA_ARGS__); \
	} while(0);
# define APMSEC_SADUMP(s, l)		\
		if (verbose) 		\
	 		sec_sa_dump((s), (l))
# define APMSEC_TKNDUMP(t)		\
		if (verbose) \
			sec_tkn_dump((t))
#endif

#if !defined(APM_SEC_QMDEBUG)
# define APMSEC_QMSGDUMP(hdr, d, l)
#else
# define APMSEC_QMSGDUMP(hdr, d, l)	\
		if (verbose) \
			xgene_sec_dump(hdr, d, l)
#endif

extern int verbose;
void xgene_sec_dump(char *hdr, void *buf, int len);

#endif
