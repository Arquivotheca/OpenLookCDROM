/* $Id: OverConvP.h,v 1.16 1992/09/18 00:34:07 ishisone Rel $ */
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

#ifndef _OverTheSpotConversionP_h
#define _OverTheSpotConversionP_h

#include "ConvCtrlP.h"
#include "OverConv.h"

#define XtRModeLocation "ModeLocation"

typedef struct _canvasrec_ {
    Widget	canvas;
    Position	x, y;
    Position	maxx, maxy;	/* for canvas reconfiguration */
    Boolean	shouldpopup;
    Boolean	poppedup;
    struct _canvasrec_	*next;
} TextCanvas;

typedef struct {
    TextCanvas	*canvas;
    Position	x, y;
} DisplayLocation;

typedef struct _fragment_ {
    unsigned short from;	/* $BI=<(3+;OJ8;z0LCV(B */
    unsigned short nchars;	/* $BI=<(J8;z?t(B */
    TextCanvas *canvas;		/* $BI=<($9$k%-%c%s%P%9(B ($B?F(B widget $B$H$O8B$i$J$$(B) */
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
 *	over-the-spot conversion control widget data structure
 */

typedef struct {
    int empty;
} OverTheSpotConversionClassPart;

typedef struct _OverTheSpotConversionClassRec {
    CoreClassPart	core_class;
    CompositeClassPart	composite_class;
    ShellClassPart	shell_class;
    WMShellClassPart	wm_shell_class;
    VendorShellClassPart	vendor_shell_class;
    TransientShellClassPart	transient_shell_class;
    ConversionControlClassPart	conversionControl_class;
    OverTheSpotConversionClassPart overTheSpotConversion_class;
} OverTheSpotConversionClassRec;

extern OverTheSpotConversionClassRec overTheSpotConversionClassRec;

typedef struct {
    /* resources */
    Position	spotx, spoty;
    Boolean	spotforwarding;
    Dimension	linespacing;
    XRectangle	clientarea;
    ModeLocation	modelocation;
    Boolean	shrinkwindow;
    Boolean	ignorestatusarea;
    Boolean	borderforeground;	/* force the border color to be same
					 * as foreground color
					 */
    /* private state */
    Atom	wm_state;	/* WM_STATE atom */
    Widget	displayobj;
    Widget	selectionshell;
    Widget	selectionwidget;
    Widget	selectiondisplayobj;
    Widget	auxshell;
    Widget	auxwidget;
    Widget	auxdisplayobj;
    Widget	modeshell;
    Widget	modewidget;
    Widget	modedisplayobj;
    Boolean	modelocationspecified;
    Widget	modeshell_fix, modewidget_fix, modedisplayobj_fix;
    Widget	modeshell_float, modewidget_float, modedisplayobj_float;
    Colormap	colormap;
    Pixel	background;
    Cursor	canvascursor;
    Position	focusoffsetx;
    Position	focusoffsety;
    TextCanvas	*canvaslist;
    TextCanvas	*overflowcanvas;
    Position	overflowoffset;
    DisplayLocation	cursorlocation;
    Boolean	cursorvisible;
    DisplaySegment	*dispsegments;
    Cardinal	numsegments;
    Cardinal	dispsegmentsize;
    ICString	*candlist;
    Cardinal	numcands;
    Boolean	selectionpoppedup;
    Boolean	auxpoppedup;
    Position	ascent;
    Dimension	lineheight;
    Dimension	saved_bw;
} OverTheSpotConversionPart;

typedef struct _OverTheSpotConversionRec {
    CorePart		core;
    CompositePart	composite;
    ShellPart		shell;
    WMShellPart		wm;
    VendorShellPart	vendor;
    TransientShellPart	transient;	
    ConversionControlPart	ccontrol;
    OverTheSpotConversionPart	overthespot;
} OverTheSpotConversionRec;

#endif
