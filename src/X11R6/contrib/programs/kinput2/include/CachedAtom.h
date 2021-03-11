/* $Id: CachedAtom.h,v 1.4 1991/09/16 05:12:16 ishisone Rel $ */
/*
 * Copyright (c) 1991  Software Research Associates, Inc.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Software Research Associates not be
 * used in advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.  Software Research
 * Associates makes no representations about the suitability of this software
 * for any purpose.  It is provided "as is" without express or implied
 * warranty.
 *
 * Author:  Makoto Ishisone, Software Research Associates, Inc., Japan
 */

#ifndef _CachedAtom_h
#define _CachedAtom_h

/*
 * Atom CachedInternAtom(Display *dpy, String name, Bool only_if_exists)
 * String CachedGetAtomName(Display *dpy, Atom atom)
 *	$B$=$l$>$l(B XInternAtom(), XGetAtomName() $B$H$[$\F1$8!#(B
 *	$B%-%c%C%7%s%0$r9T$J$&$H$3$m$,0c$&!#(B(R5 $B$N(B XInternAtom() $B$O(B
 *	$B%-%c%C%7%e$9$k$,(B XGetAtomName() $B$O$7$J$$(B)
 *	$B$^$?!"(BCachedGetAtomName() $B$,JV$9J8;zNs$O6&M-$5$l$F$$$k$N$G(B
 *	free() $B$7$F$O$J$i$J$$!#(B
 */

extern Atom CachedInternAtom(
#if NeedFunctionPrototypes
	Display *	/* dpy */,
	String		/* name */,
	Bool		/* only_if_exists */
#endif
);

extern String CachedGetAtomName(
#if NeedFunctionPrototypes
	Display *	/* dpy */,
	Atom		/* atom */
#endif
);

#endif
