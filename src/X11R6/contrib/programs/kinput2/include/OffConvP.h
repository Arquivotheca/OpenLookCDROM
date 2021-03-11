/* $Id: OffConvP.h,v 1.6 1991/09/11 07:44:00 ishisone Rel $ */
/*
 * Copyright (c) 1990  Software Research Associates, Inc.
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

#ifndef _OffTheSpotConversionP_h
#define _OffTheSpotConversionP_h

#include "ConvCtrlP.h"
#include "OffConv.h"

typedef struct {
    Position	x, y;
} DisplayLocation;

typedef struct _fragment_ {
    unsigned short from;	/* $BI=<(3+;OJ8;z0LCV(B */
    unsigned short nchars;	/* $BI=<(J8;z?t(B */
    XRectangle region;		/* $BI=<(NN0h(B */
    struct _fragment_ *next;	/* $B<!$N%U%i%0%a%s%H(B */
} DisplayFragment;

typedef struct {
    ICString seg;		/* $B%;%0%a%s%H(B */
    short redrawpos;		/* $B=q$-D>$7$,I,MW$JJ8;z$N3+;O0LCV(B
				 * (-1 $B$J$i=q$-D>$9I,MW$J$7(B)
				 */
    DisplayFragment *fragments;	/* $BI=<(%U%i%0%a%s%H(B */
} DisplaySegment;


/*
 *	off-the-spot conversion control widget data structure
 */

typedef struct {
    int empty;
} OffTheSpotConversionClassPart;

typedef struct _OffTheSpotConversionClassRec {
    CoreClassPart	core_class;
    CompositeClassPart	composite_class;
    ShellClassPart	shell_class;
    WMShellClassPart	wm_shell_class;
    VendorShellClassPart	vendor_shell_class;
    TransientShellClassPart	transient_shell_class;
    ConversionControlClassPart	conversionControl_class;
    OffTheSpotConversionClassPart offTheSpotConversion_class;
} OffTheSpotConversionClassRec;

extern OffTheSpotConversionClassRec offTheSpotConversionClassRec;

typedef struct {
    /* resources */
    Dimension	leftmargin;
    Dimension	rightmargin;
    /* private state */
    Widget	displayobj;
    Widget	selectionshell;
    Widget	selectionwidget;
    Widget	formwidget;
    Widget	modewidget;
    Widget	modeshell;
    Widget	modedisplayobj;
    Widget	canvaswidget;
    DisplayLocation	cursorlocation;
    Boolean	cursorvisible;
    DisplaySegment	*dispsegments;
    Cardinal	numsegments;
    Cardinal	dispsegmentsize;
    ICString	*candlist;
    Cardinal	numcands;
    Boolean	selectionpoppedup;
    Dimension	lineheight;
    Position	ascent;
    Widget	auxshell;
    Widget	auxwidget;
    Boolean	auxpoppedup;
} OffTheSpotConversionPart;

typedef struct _OffTheSpotConversionRec {
    CorePart		core;
    CompositePart	composite;
    ShellPart		shell;
    WMShellPart		wm;
    VendorShellPart	vendor;
    TransientShellPart	transient;	
    ConversionControlPart	ccontrol;
    OffTheSpotConversionPart	offthespot;
} OffTheSpotConversionRec;


/*
 *	separate conversion control widget data structure
 */

typedef struct {
    int empty;
} SeparateConversionClassPart;

typedef struct _SeparateConversionClassRec {
    CoreClassPart	core_class;
    CompositeClassPart	composite_class;
    ShellClassPart	shell_class;
    WMShellClassPart	wm_shell_class;
    VendorShellClassPart	vendor_shell_class;
    TransientShellClassPart	transient_shell_class;
    ConversionControlClassPart	conversionControl_class;
    OffTheSpotConversionClassPart offTheSpotConversion_class;
    SeparateConversionClassPart separateConversion_class;
} SeparateConversionClassRec;

extern SeparateConversionClassRec separateConversionClassRec;

typedef struct {
    Widget	formwidget;
} SeparateConversionPart;

typedef struct _SeparateConversionRec {
    CorePart		core;
    CompositePart	composite;
    ShellPart		shell;
    WMShellPart		wm;
    VendorShellPart	vendor;
    TransientShellPart	transient;	
    ConversionControlPart	ccontrol;
    OffTheSpotConversionPart	offthespot;
    SeparateConversionPart	separate;
} SeparateConversionRec;

#endif
