/*
 * Copyright 1993 Network Computing Devices, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name Network Computing Devices, Inc. not be
 * used in advertising or publicity pertaining to distribution of this 
 * software without specific, written prior permission.
 * 
 * THIS SOFTWARE IS PROVIDED 'AS-IS'.  NETWORK COMPUTING DEVICES, INC.,
 * DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING WITHOUT
 * LIMITATION ALL IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, OR NONINFRINGEMENT.  IN NO EVENT SHALL NETWORK
 * COMPUTING DEVICES, INC., BE LIABLE FOR ANY DAMAGES WHATSOEVER, INCLUDING
 * SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES, INCLUDING LOSS OF USE, DATA,
 * OR PROFITS, EVEN IF ADVISED OF THE POSSIBILITY THEREOF, AND REGARDLESS OF
 * WHETHER IN AN ACTION IN CONTRACT, TORT OR NEGLIGENCE, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * $NCDId: @(#)Amd.h,v 1.4 1993/10/13 06:29:21 mellon Exp $
 */

/* Portions derived from */
/***********************************************************
Copyright 1987 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

/* Bogus!!   Amd.h and Xmd.h serve the same function for two different
   protocols, but some files include both, so we must check to see if
   Xmd.h has been included and then claim that it has been.   This means
   that Xmd.h and Amd.h need to be kept in sync manually, which is A
   Bad Thing.   This should be fixed. */
#ifndef XMD_H
#define XMD_H 1
#ifndef AMD_H
#define AMD_H 1
/* $XConsortium: Xmd.h,v 1.36 90/03/14 12:15:16 jim Exp $ */
/*
 *  Xmd.h: MACHINE DEPENDENT DECLARATIONS.
 */

/*
 * Special per-machine configuration flags.
 */
#ifdef CRAY
#define WORD64				/* 64-bit architecture */
#define UNSIGNEDBITFIELDS		/* bit fields do not honor sign */
#endif


/*
 * Stuff to handle large architecture machines; the constants were generated
 * on a 32-bit machine and must coorespond to the protocol.
 */
#ifdef WORD64
#define MUSTCOPY
#endif /* WORD64 */


/*
 * Definition of macro used to set constants for size of network structures;
 * machines with preprocessors that can't handle all of the sz_ symbols
 * can define this macro to be sizeof(x) if and only if their compiler doesn't
 * pad out structures (esp. the xTextElt structure which contains only two 
 * one-byte fields).  Network structures should always define sz_symbols.
 *
 * The sz_ prefix is used instead of something more descriptive so that the
 * symbols are no more than 32 characters long (which causes problems for some
 * compilers and preprocessors).
 */
#if defined(__STDC__) && !defined(UNIXCPP)
#define SIZEOF(x) sz_##x
#else
#define SIZEOF(x) sz_/**/x
#endif /* if ANSI C compiler else not */



/*
 * ibm pcc doesn't understand pragmas.
 */
#if defined(ibm032) && defined(__HIGHC__)
pragma on(pointers_compatible);
pragma off(char_default_unsigned);
#endif


/*
 * Bitfield suffixes for the protocol structure elements, if you
 * need them.  Note that bitfields are not guarranteed to be signed
 * (or even unsigned) according to ANSI C.
 */
#ifdef WORD64
#define B32 :32
#define B16 :16
#else
#define B32
#define B16
#endif

#ifndef __alpha
typedef long		INT32;
typedef unsigned long	CARD32;
typedef unsigned long	BITS32;
#else /* __alpha */
typedef int		INT32;
typedef unsigned int	CARD32;
typedef unsigned int	BITS32;
#endif /* __alpha */

typedef short          INT16;
typedef char           INT8;

typedef unsigned short CARD16;
typedef unsigned char  CARD8;

typedef unsigned short		BITS16;
typedef unsigned char		BYTE;

typedef unsigned char            BOOL;


/*
 * definitions for sign-extending bitfields on 64-bit architectures
 */
#if defined(WORD64) && defined(UNSIGNEDBITFIELDS)
#define cvtINT8toInt(val)   (((val) & 0x00000080) ? ((val) | 0xffffffffffffff00) : (val))
#define cvtINT16toInt(val)  (((val) & 0x00008000) ? ((val) | 0xffffffffffff0000) : (val))
#define cvtINT32toInt(val)  (((val) & 0x80000000) ? ((val) | 0xffffffff00000000) : (val))
#define cvtINT8toShort(val)  cvtINT8toInt(val)
#define cvtINT16toShort(val) cvtINT16toInt(val)
#define cvtINT32toShort(val) cvtINT32toInt(val)
#define cvtINT8toLong(val)  cvtINT8toInt(val)
#define cvtINT16toLong(val) cvtINT16toInt(val)
#define cvtINT32toLong(val) cvtINT32toInt(val)
#else
#define cvtINT8toInt(val) (val)
#define cvtINT16toInt(val) (val)
#define cvtINT32toInt(val) (val)
#define cvtINT8toShort(val) (val)
#define cvtINT16toShort(val) (val)
#define cvtINT32toShort(val) (val)
#define cvtINT8toLong(val) (val)
#define cvtINT16toLong(val) (val)
#define cvtINT32toLong(val) (val)
#endif /* WORD64 and UNSIGNEDBITFIELDS */



#ifdef MUSTCOPY
/*
 * This macro must not cast or else pointers will get aligned and be wrong
 */
#define NEXTPTR(p,t)  (((char *) p) + SIZEOF(t))
#else /* else not MUSTCOPY, this is used for 32-bit machines */
/*
 * this version should leave result of type (t *), but that should only be 
 * used when not in MUSTCOPY
 */  
#define NEXTPTR(p,t) (((t *)(p)) + 1)
#endif /* MUSTCOPY - used machines whose C structs don't line up with proto */

#endif /* AMD_H */
#endif /* XMD_H */
