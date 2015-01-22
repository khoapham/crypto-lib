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
#ifndef __XGENE_QMTM_H__
#define __XGENE_QMTM_H__

/* QMTM messaging structures */
/*
 * @struct  xgene_qmtm_msg16
 * @brief   This structure represents 16 byte QMTM message format
 */
struct xgene_qmtm_msg16 {
#if defined(__AARCH64EB__) || defined(__ARMEB__)
	/* memory word 0 (bit 31:0) */
	u32 UserInfo;

	/* memory word 1 (bit 63:32) */
	u32 HL:1;
	u32 LErr:3;
	u32 RType:4;
	u32 IN:1;
	u32 Rv:1;
	u32 HB:1;
	u32 PB:1;
	u32 LL:1;
	u32 NV:1;
	u32 LEI:2;
	u32 ELErr:2;
	u32 Rv2:2;
	u32 FPQNum:12;

	/* memory word 2 (bit 95:64) */
	u32 DataAddrL;		/* split 10/32 */

	/* memory word 3 (bit 127:96) */
	u32 C:1;
	u32 BufDataLen:15;
	u32 Rv6:6;
	u32 DataAddrH:10;	/* split 10/32 */
#else
	/* memory word 0 (bit 31:0) */
	u32 UserInfo;

	/* memory word 1 (bit 63:32) */
	u32 FPQNum:12;
	u32 Rv2:2;
	u32 ELErr:2;
	u32 LEI:2;
	u32 NV:1;
	u32 LL:1;
	u32 PB:1;
	u32 HB:1;
	u32 Rv:1;
	u32 IN:1;
	u32 RType:4;
	u32 LErr:3;
	u32 HL:1;

	/* memory word 2 (bit 95:64) */
	u32 DataAddrL;		/* split 10/32 */

	/* memory word 3 (bit 127:96) */
	u32 DataAddrH:10;	/* split 10/32 */
	u32 Rv6:6;
	u32 BufDataLen:15;
	u32 C:1;
#endif
} __attribute__ ((packed));

/*
 * @struct  xgene_qmtm_msg_up16
 * @brief   This structure represents upper 16 byte portion of 32 byte of
 *          QMTM message format
 */
struct xgene_qmtm_msg_up16 {
#if defined(__AARCH64EB__) || defined(__ARMEB__)
	/* memory word 4 (bit 159:128) */
	u32 H0Info_msbL;	/* split 16/32 */

	/* memory word 5 (bit 191:160) */
	u32 HR:1;
	u32 Rv0:1;
	u32 DR:1;
	u32 Rv1:1;
	u32 TotDataLengthLinkListLSBs:12;
	u16 H0Info_msbH;	/* split 16/32 */

	/* memory word 6 (bit 223:192) */
	u32 H0Info_lsbL;	/* split 16/32 */

	/* memory word 7 (bit 255:224) */
	u32 H0FPSel:4;
	u32 H0Enq_Num:12;
	u16 H0Info_lsbH;	/* split 16/32 */
#else
	/* memory word 4 (bit 159:128) */
	u32 H0Info_msbL;	/* split 16/32 */

	/* memory word 5 (bit 191:160) */
	u16 H0Info_msbH;	/* split 16/32 */
	u32 TotDataLengthLinkListLSBs:12;
	u32 Rv1:1;
	u32 DR:1;
	u32 Rv0:1;
	u32 HR:1;

	/* memory word 6 (bit 223:192) */
	u32 H0Info_lsbL;	/* split 16/32 */

	/* memory word 7 (bit 255:224) */
	u16 H0Info_lsbH;	/* split 16/32 */
	u32 H0Enq_Num:12;
	u32 H0FPSel:4;
#endif
} __attribute__ ((packed));

/*
 * @struct  xgene_qmtm_msg_ext8
 * @brief   This structure represents 8 byte portion of QMTM extended (64B)
 *          message format
 */
struct xgene_qmtm_msg_ext8 {
#if defined(__AARCH64EB__) || defined(__ARMEB__)
	u32 NxtDataAddrL;
	u32 Rv1:1;
	u32 NxtBufDataLength:15;
	u32 NxtFPQNum:4;
	u32 Rv2:2;
	u32 NxtDataAddrH:10;
#else
	u32 NxtDataAddrL;
	u32 NxtDataAddrH:10;
	u32 Rv2:2;
	u32 NxtFPQNum:4;
	u32 NxtBufDataLength:15;
	u32 Rv1:1;
#endif
} __attribute__ ((packed));

/*
 * @struct  xgene_qmtm_msg_ll8
 * @brief   This structure represents 8 byte Link list portion of
 *          QMTM extended (64B) message format
 */
struct xgene_qmtm_msg_ll8 {
#if defined(__AARCH64EB__) || defined(__ARMEB__)
	u32 NxtDataPtrL;
	u8 TotDataLengthLinkListMSBs;
	u8 NxtLinkListength;
	u32 NxtFPQNum:4;
	u32 Rv2:2;
	u32 NxtDataPtrH:10;
#else
	u32 NxtDataPtrL;
	u32 NxtDataPtrH:10;
	u32 Rv2:2;
	u32 NxtFPQNum:4;
	u8 NxtLinkListength;
	u8 TotDataLengthLinkListMSBs;
#endif
} __attribute__ ((packed));

/*
 * @struct  xgene_qmtm_msg32
 * @brief   This structure represents 32 byte QMTM message format
 */
struct xgene_qmtm_msg32 {
	struct xgene_qmtm_msg16 msg16;
	struct xgene_qmtm_msg_up16 msgup16;
} __attribute__ ((packed));

/*
 * @struct  xgene_qmtm_msg_ext32
 * @brief   This structure represents 32 byte of QMTM extended (64B)
 *          message format
 */
struct xgene_qmtm_msg_ext32 {
	struct xgene_qmtm_msg_ext8 msg8_2;
	struct xgene_qmtm_msg_ext8 msg8_1;
	union {
		struct xgene_qmtm_msg_ext8 msg8_4;
		struct xgene_qmtm_msg_ll8 msg8_ll;
	};
	struct xgene_qmtm_msg_ext8 msg8_3;
} __attribute__ ((packed));

/*
 * @struct  xgene_qmtm_msg64
 * @brief   This structure represents 64 byte QMTM message format
 */
struct xgene_qmtm_msg64 {
	struct xgene_qmtm_msg32 msg32_1;
	struct xgene_qmtm_msg_ext32 msg32_2;
} __attribute__ ((packed));

#if defined(__AARCH64EB__) || defined(__ARMEB__)
#define xgene_qmtm_msg_le32(word, words)	\
	do { \
		unsigned int w; \
		for (w = 0; w < words; w++) \
			*(word + w) = cpu_to_le32(*(word + w)); \
	} while (0)
#else
#define xgene_qmtm_msg_le32(word, words)	\
	do {} while (0)
#endif

/* Empty Slot Soft Signature */
#define EMPTY_SLOT_INDEX 		7
#define EMPTY_SLOT       		0x22222222

/* Destination QM, 2 MSb in work queue, dstqid */
#define QMTM_QUEUE_ID(qm, qid)		(((u16)(qm) << 10) | qid)

/* Number of messages prefetched */
#define NUM_MSGS_IN_BUF_RD(s)		((s) & 0x000001ff)

/**
 * @brief   Encode data length to BufDataLen assuming maximum buffer size
 * @param   len Data length
 * @return  Encoded value of buffer data length field
 */
static inline u16 xgene_qmtm_encode_datalen(u32 len)
{
	return len & 0x3FFF;
}

#endif /* __XGENE_QMTM_H__ */
