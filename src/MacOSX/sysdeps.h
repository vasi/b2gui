/*
 *	sysdeps.h - System dependent definitions for Mac OS X.
 *				Based on Unix version
 *
 *	$Id: sysdeps.h,v 1.11 2011/03/11 16:47:48 asvitkine Exp $
 *
 *	Basilisk II (C) 1997-2008 Christian Bauer
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef SYSDEPS_H
#define SYSDEPS_H

#ifndef __STDC__
#error "Your compiler is not ANSI. Get a real one."
#endif

#include "config.h"
#include "user_strings_unix.h"

#ifndef STDC_HEADERS
#error "You don't have ANSI C header files."
#endif

#ifdef HAVE_UNISTD_H
# include <sys/types.h>
# include <unistd.h>
#endif

#include <netinet/in.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef HAVE_PTHREADS
# include <pthread.h>
#endif

#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif

#ifdef TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#if defined(__MACH__)
#include <mach/clock.h>
#endif

/* Symbol to distinguish Nigel's Aqua port from a normal Darwin X11 build */
/* (this sysdeps.h file is currently specific to the Mac OS X Aqua port)  */
#define AQUA 1

/* Header which defines OS X version for selecting APIs */
#ifdef AVAILABILITYMACROS
# include <AvailabilityMacros.h>
#endif


#ifdef ENABLE_NATIVE_M68K

/* Mac and host address space are the same */
#define REAL_ADDRESSING 1

/* Using 68k natively */
#define EMULATED_68K 0

/* Mac ROM is not write protected */
#define ROM_IS_WRITE_PROTECTED 0
#define USE_SCRATCHMEM_SUBTERFUGE 1

#else

/* Mac and host address space are distinct */
#ifndef REAL_ADDRESSING
#define REAL_ADDRESSING 0
#endif

/* Using 68k emulator */
#define EMULATED_68K 1

/* The m68k emulator uses a prefetch buffer ? */
#define USE_PREFETCH_BUFFER 0

/* Mac ROM is write protected when banked memory is used */
#if REAL_ADDRESSING || DIRECT_ADDRESSING
# define ROM_IS_WRITE_PROTECTED 0
# define USE_SCRATCHMEM_SUBTERFUGE 1
#else
# define ROM_IS_WRITE_PROTECTED 1
#endif

#endif

/* Direct Addressing requires Video on SEGV signals */
#if DIRECT_ADDRESSING && !ENABLE_VOSF
# undef  ENABLE_VOSF
# define ENABLE_VOSF 1
#endif

/* ExtFS is supported */
#define SUPPORTS_EXTFS 1

/* BSD socket API supported */
#define SUPPORTS_UDP_TUNNEL 1


/* Data types */
typedef unsigned char uint8;
typedef signed char int8;
#if SIZEOF_SHORT == 2
typedef unsigned short uint16;
typedef short int16;
#elif SIZEOF_INT == 2
typedef unsigned int uint16;
typedef int int16;
#else
#error "No 2 byte type, you lose."
#endif
#if SIZEOF_INT == 4
typedef unsigned int uint32;
typedef int int32;
#elif SIZEOF_LONG == 4
typedef unsigned long uint32;
typedef long int32;
#else
#error "No 4 byte type, you lose."
#endif
#if SIZEOF_LONG == 8
typedef unsigned long uint64;
typedef long int64;
#define VAL64(a) (a ## l)
#define UVAL64(a) (a ## ul)
#elif SIZEOF_LONG_LONG == 8
typedef unsigned long long uint64;
typedef long long int64;
#define VAL64(a) (a ## LL)
#define UVAL64(a) (a ## uLL)
#else
#error "No 8 byte type, you lose."
#endif
#if SIZEOF_VOID_P == 4
typedef uint32 uintptr;
typedef int32 intptr;
#elif SIZEOF_VOID_P == 8
typedef uint64 uintptr;
typedef int64 intptr;
#else
#error "Unsupported size of pointer"
#endif

#ifndef HAVE_LOFF_T
typedef off_t loff_t;
#endif
#ifndef HAVE_CADDR_T
typedef char * caddr_t;
#endif

/* Time data type for Time Manager emulation */
#ifdef HAVE_CLOCK_GETTIME
typedef struct timespec tm_time_t;
#elif defined(__MACH__)
typedef mach_timespec_t tm_time_t;
#else
typedef struct timeval tm_time_t;
#endif

/* Define codes for all the float formats that we know of.
 * Though we only handle IEEE format.  */
#define UNKNOWN_FLOAT_FORMAT 0
#define IEEE_FLOAT_FORMAT 1
#define VAX_FLOAT_FORMAT 2
#define IBM_FLOAT_FORMAT 3
#define C4X_FLOAT_FORMAT 4

/* UAE CPU data types */
#define uae_s8 int8
#define uae_u8 uint8
#define uae_s16 int16
#define uae_u16 uint16
#define uae_s32 int32
#define uae_u32 uint32
#define uae_s64 int64
#define uae_u64 uint64
typedef uae_u32 uaecptr;

/* Alignment restrictions */
#if defined(__i386__) || defined(__powerpc__) || defined(__m68k__) || defined(__x86_64__)
# define CPU_CAN_ACCESS_UNALIGNED
#endif

/* Timing functions */
extern uint64 GetTicks_usec(void);
extern void Delay_usec(uint32 usec);

#ifdef HAVE_PTHREADS
/* Centralized pthread attribute setup */
void Set_pthread_attr(pthread_attr_t *attr, int priority);
#endif

/* UAE CPU defines */
#ifdef WORDS_BIGENDIAN

#ifdef CPU_CAN_ACCESS_UNALIGNED

/* Big-endian CPUs which can do unaligned accesses */
static inline uae_u32 do_get_mem_long(uae_u32 *a) {return *a;}
static inline uae_u32 do_get_mem_word(uae_u16 *a) {return *a;}
static inline void do_put_mem_long(uae_u32 *a, uae_u32 v) {*a = v;}
static inline void do_put_mem_word(uae_u16 *a, uae_u32 v) {*a = v;}

#else /* CPU_CAN_ACCESS_UNALIGNED */

#ifdef sgi
/* The SGI MIPSPro compilers can do unaligned accesses given enough hints.
 * They will automatically inline these routines. */
#ifdef __cplusplus
extern "C" { /* only the C compiler does unaligned accesses */
#endif
extern uae_u32 do_get_mem_long(uae_u32 *a);
extern uae_u32 do_get_mem_word(uae_u16 *a);
extern void do_put_mem_long(uae_u32 *a, uae_u32 v);
extern void do_put_mem_word(uae_u16 *a, uae_u32 v);
#ifdef __cplusplus
}
#endif

#else /* sgi */

/* Big-endian CPUs which can not do unaligned accesses (this is not the most efficient way to do this...) */
static inline uae_u32 do_get_mem_long(uae_u32 *a) {uint8 *b = (uint8 *)a; return (b[0] << 24) | (b[1] << 16) | (b[2] << 8) | b[3];}
static inline uae_u32 do_get_mem_word(uae_u16 *a) {uint8 *b = (uint8 *)a; return (b[0] << 8) | b[1];}
static inline void do_put_mem_long(uae_u32 *a, uae_u32 v) {uint8 *b = (uint8 *)a; b[0] = v >> 24; b[1] = v >> 16; b[2] = v >> 8; b[3] = v;}
static inline void do_put_mem_word(uae_u16 *a, uae_u32 v) {uint8 *b = (uint8 *)a; b[0] = v >> 8; b[1] = v;}
#endif /* sgi */

#endif /* CPU_CAN_ACCESS_UNALIGNED */

#else /* WORDS_BIGENDIAN */

#if defined(__i386__) || defined(__x86_64__)

/* Intel x86 */
#define X86_PPRO_OPT
static inline uae_u32 do_get_mem_long(uae_u32 *a) {uint32 retval; __asm__ ("bswap %0" : "=r" (retval) : "0" (*a) : "cc"); return retval;}
#ifdef X86_PPRO_OPT
static inline uae_u32 do_get_mem_word(uae_u16 *a) {uint32 retval; __asm__ ("movzwl %w1,%k0\n\tshll $16,%k0\n\tbswapl %k0\n" : "=&r" (retval) : "m" (*a) : "cc"); return retval;}
#else
static inline uae_u32 do_get_mem_word(uae_u16 *a) {uint32 retval; __asm__ ("xorl %k0,%k0\n\tmovw %w1,%w0\n\trolw $8,%w0" : "=&r" (retval) : "m" (*a) : "cc"); return retval;}
#endif
#define HAVE_GET_WORD_UNSWAPPED
#define do_get_mem_word_unswapped(a) ((uae_u32)*((uae_u16 *)(a)))
static inline void do_put_mem_long(uae_u32 *a, uae_u32 v) {__asm__ ("bswap %0" : "=r" (v) : "0" (v) : "cc"); *a = v;}
#ifdef X86_PPRO_OPT
static inline void do_put_mem_word(uae_u16 *a, uae_u32 v) {__asm__ ("bswapl %0" : "=&r" (v) : "0" (v << 16) : "cc"); *a = v;}
#else
static inline void do_put_mem_word(uae_u16 *a, uae_u32 v) {__asm__ ("rolw $8,%0" : "=r" (v) : "0" (v) : "cc"); *a = v;}
#endif
#define HAVE_OPTIMIZED_BYTESWAP_32
/* bswap doesn't affect condition codes */
static inline uae_u32 do_byteswap_32(uae_u32 v) {__asm__ ("bswap %0" : "=r" (v) : "0" (v)); return v;}
#define HAVE_OPTIMIZED_BYTESWAP_16
#ifdef X86_PPRO_OPT
static inline uae_u32 do_byteswap_16(uae_u32 v) {__asm__ ("bswapl %0" : "=&r" (v) : "0" (v << 16) : "cc"); return v;}
#else
static inline uae_u32 do_byteswap_16(uae_u32 v) {__asm__ ("rolw $8,%0" : "=r" (v) : "0" (v) : "cc"); return v;}
#endif

#elif defined(CPU_CAN_ACCESS_UNALIGNED)

/* Other little-endian CPUs which can do unaligned accesses */
static inline uae_u32 do_get_mem_long(uae_u32 *a) {uint32 x = *a; return (x >> 24) | (x >> 8) & 0xff00 | (x << 8) & 0xff0000 | (x << 24);}
static inline uae_u32 do_get_mem_word(uae_u16 *a) {uint16 x = *a; return (x >> 8) | (x << 8);}
static inline void do_put_mem_long(uae_u32 *a, uae_u32 v) {*a = (v >> 24) | (v >> 8) & 0xff00 | (v << 8) & 0xff0000 | (v << 24);}
static inline void do_put_mem_word(uae_u16 *a, uae_u32 v) {*a = (v >> 8) | (v << 8);}

#else /* CPU_CAN_ACCESS_UNALIGNED */

/* Other little-endian CPUs which can not do unaligned accesses (this needs optimization) */
static inline uae_u32 do_get_mem_long(uae_u32 *a) {uint8 *b = (uint8 *)a; return (b[0] << 24) | (b[1] << 16) | (b[2] << 8) | b[3];}
static inline uae_u32 do_get_mem_word(uae_u16 *a) {uint8 *b = (uint8 *)a; return (b[0] << 8) | b[1];}
static inline void do_put_mem_long(uae_u32 *a, uae_u32 v) {uint8 *b = (uint8 *)a; b[0] = v >> 24; b[1] = v >> 16; b[2] = v >> 8; b[3] = v;}
static inline void do_put_mem_word(uae_u16 *a, uae_u32 v) {uint8 *b = (uint8 *)a; b[0] = v >> 8; b[1] = v;}

#endif /* CPU_CAN_ACCESS_UNALIGNED */

#endif /* WORDS_BIGENDIAN */

#ifndef HAVE_OPTIMIZED_BYTESWAP_32
static inline uae_u32 do_byteswap_32(uae_u32 v)
	{ return (((v >> 24) & 0xff) | ((v >> 8) & 0xff00) | ((v & 0xff) << 24) | ((v & 0xff00) << 8)); }
#endif

#ifndef HAVE_OPTIMIZED_BYTESWAP_16
static inline uae_u32 do_byteswap_16(uae_u32 v)
	{ return (((v >> 8) & 0xff) | ((v & 0xff) << 8)); }
#endif

#define do_get_mem_byte(a) ((uae_u32)*((uae_u8 *)(a)))
#define do_put_mem_byte(a, v) (*(uae_u8 *)(a) = (v))

#define call_mem_get_func(func, addr) ((*func)(addr))
#define call_mem_put_func(func, addr, v) ((*func)(addr, v))
#define __inline__ inline
#define CPU_EMU_SIZE 0
#undef NO_INLINE_MEMORY_ACCESS
#undef MD_HAVE_MEM_1_FUNCS
#define ENUMDECL typedef enum
#define ENUMNAME(name) name
#define write_log printf

#if defined(X86_ASSEMBLY) || defined(X86_64_ASSEMBLY)
#define ASM_SYM_FOR_FUNC(a) __asm__(a)
#else
#define ASM_SYM_FOR_FUNC(a)
#endif

#ifndef REGPARAM
# define REGPARAM
#endif
#define REGPARAM2

#endif
