/* misc.h -- general utilities */
#ifndef __ORIHIME_LIBOS_MISC_H__
#define __ORIHIME_LIBOS_MISC_H__

#ifdef __ORIHIME_LIBOS_INTERNAL

#define ALIGNDOWN(x, a) ((x) / (a) * (a))
#define ALIGNUP(x, a) ((((x) - 1) / (a) + 1) * (a))

#endif	/* __ORIHIME_LIBOS_INTERNAL */

#endif	/* __ORIHIME_LIBOS_MISC_H__ */
