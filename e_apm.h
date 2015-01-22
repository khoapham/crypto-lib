#ifndef HAVE_E_APM_H
#define HAVE_E_APM_H
#include <stdint.h>
#include <stdbool.h>

#ifndef APM
#define APM 39
#endif

#ifndef SOL_APM
#define SOL_APM 280
#endif

/* Socket options */
#define APM_SET_KEY			1
#define APM_SET_IV			2
#define APM_SET_OP			3

/* Operations */
#define APM_OP_DECRYPT			0
#define APM_OP_ENCRYPT			1


#define HAVE___ATTRIBUTE__ 1

#if HAVE___ATTRIBUTE__
#  define __UNUSED__ __attribute__((unused))
#  define __U__      __attribute__((unused))
#else
#  define __UNUSED__
#  define __U__
#endif	/* HAVE___ATTRIBUTE__ */

struct NID_store
{
	size_t len;
	int *data;
};
bool NID_store_contains(struct NID_store *store, int nid);

extern struct NID_store ciphers_available;
extern struct NID_store ciphers_used;

extern struct NID_store digests_available;
extern struct NID_store digests_used;

struct apm_iv {
        __u32   ivlen;
        __u8    iv[0];
};

#ifdef DEBUG
#define TRACE(...) fprintf(stderr,__VA_ARGS__)
#else
#define TRACE(...)
#endif

#endif
