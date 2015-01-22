/* MD5.H - header file for MD5C.C
 */

/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
 rights reserved.

 License to copy and use this software is granted provided that it
 is identified as the "RSA Data Security, Inc. MD5 Message-Digest
 Algorithm" in all material mentioning or referencing this software
 or this function.

 License is also granted to make and use derivative works provided
 that such works are identified as "derived from the RSA Data
 Security, Inc. MD5 Message-Digest Algorithm" in all material
 mentioning or referencing the derived work.

 RSA Data Security, Inc. makes no representations concerning either
 the merchantability of this software or the suitability of this
 software for any particular purpose. It is provided "as is"
 without express or implied warranty of any kind.
 These notices must be retained in any copies of any part of this
 documentation and/or software.
 */
#include <stdint.h>

/* MD5 context. */
typedef struct MD5Context{
	uint32_t state[4]; /* state (ABCD) */
	uint32_t count[2]; /* number of bits, modulo 2^64 (lsb first) */
	unsigned char buffer[64]; /* input buffer */
} MD5Context;

int MD5Init(MD5Context *);
int MD5Update(MD5Context *, unsigned char *, unsigned int);
int MD5Final(unsigned char[16], MD5Context *);
int MD5Export(unsigned char[16], MD5Context *);
