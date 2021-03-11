/* $Id: ConvDisp.h,v 1.8 1991/03/29 15:38:14 ishisone Rel $ */
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

#ifndef _ConvDisplay_h
#define _ConvDisplay_h

#include <X11/Object.h>
#include "ICtypes.h"

/*
  ConvDisplay new resources:

  name		class		type		default			access
  ----------------------------------------------------------------------------
  foreground	Foreground	Pixel		DefaultForeground	CSG
  background	Background	Pixel		DefaultBackground	CSG
  cursorBitmap	CursorBitmap	Bitmap		*1			CSG
  hotX		HotX		Position	3			CSG
  hotY		HotY		Position	2			CSG

  note:	*1) default is a small caret

*/

#define XtNcursorBitmap "cursorBitmap"
#define XtCCursorBitmap "CursorBitmap"
#define XtNhotX "hotX"
#define XtNhotY "hotY"
#define XtCHotX "HotX"
#define XtCHotY "HotY"

typedef struct _ConvDisplayClassRec	*ConvDisplayObjectClass;
typedef struct _ConvDisplayRec		*ConvDisplayObject;

extern WidgetClass	convDisplayObjectClass;

/*
 * public interface functions
 */

/*
 * int CDStringWidth(Widget object, ICString *str, int start, int end)
 *	$B;XDj$5$l$?J8;zNs$N(B start $BJ8;zL\$+$i(B end - 1 $BJ8;zL\$^$G$N(B
 *	$BI}(B ($BC10L$O%T%/%;%k(B) $B$rJV$9(B
 *	end $B$,Ii$N;~$OJ8;zNs$N:G8e$^$G$rI=$9(B
 */
extern int CDStringWidth(
#if NeedFunctionPrototypes
	Widget		/* object */,
	ICString *	/* str */,
	int		/* start */,
	int		/* end */
#endif
);

/*
 * int CDLineHeight(Widget object, Position *ascentp)
 *	$B9T$N9b$5(B ($BC10L$O%T%/%;%k(B) $B$rJV$9(B
 *	ascentp $B$,(B NULL $B$G$J$$;~$K$O(B ascent $B$,JV$5$l$k(B
 */
extern int CDLineHeight(
#if NeedFunctionPrototypes
	Widget		/* object */,
	Position *	/* ascentp */
#endif
);

/*
 * void CDDrawString(Widget object, Widget canvas, ICString *str,
 *		      int start, int end, int x, int y)
 *	canvas $B$G;XDj$5$l$k(B widget $B$N:BI8(B (x, y) $B$+$i!";XDj$5$l$?(B
 *	$BJ8;zNs$N(B start $BJ8;zL\$+$i(B end - 1 $BJ8;zL\$^$G$r=q$/(B
 *	end $B$,Ii$N;~$OJ8;zNs$N:G8e$^$G$rI=$9(B
 *	$B$?$@$7!"(B(x, y) $B$OJ8;zNs$N:8>e$N:BI8$G$"$k(B
 */
extern void CDDrawString(
#if NeedFunctionPrototypes
	Widget		/* object */,
	Widget		/* canvas */,
	ICString *	/* str */,
	int		/* start */,
	int		/* end */,
	int		/* x */,
	int		/* y */
#endif
);

/*
 * int CDMaxChar(Widget object, ICString *str, int start, int width)
 *	$B;XDj$5$l$?J8;zNs$N(B start $BJ8;zL\$+$i!"%T%/%;%kI}(B width $B$K(B
 *	$BF~$kJ8;z?t$rJV$9(B
 */
extern int CDMaxChar(
#if NeedFunctionPrototypes
	Widget		/* object */,
	ICString *	/* str */,
	int		/* start */,
	int		/* end */
#endif
);

/*
 * void CDDrawCursor(Widget object, Widget canvas, int x, int y, int on)
 *	canvas $B$G;XDj$5$l$k(B widget $B$N:BI8(B (x, y) $B$K%$%s%5!<%H%+!<%=%k$r(B
 *	$BI=<($9$k(B
 *	on $B$,(B False $B$N;~$K$O%+!<%=%k$r>C$9(B
 *	$B;XDj$9$k(B y $B:BI8$K$O%U%)%s%H$N%Y!<%9%i%$%s$r;XDj$9$k(B
 */
extern void CDDrawCursor(
#if NeedFunctionPrototypes
	Widget		/* object */,
	Widget		/* canvas */,
	int		/* x */,
	int		/* y */,
	int		/* on */			 
#endif
);

/*
 * void CDGetCursorBounds(Widget object, XRectangle *bounds)
 *	$BI=<($5$l$k%+!<%=%k$NI=<(NN0h$r(B bounds $B$KJV$9(B
 *	$B:BI8(B (x, y) $B$K%+!<%=%k$r=q$$$?;~$NI=<(NN0h$O(B
 *		(x + bounds->x, y + bounds->y): $B:8>e:BI8(B
 *		(bounds->width, bounds->height): $BNN0h$NBg$-$5(B
 *	$B$K$J$k(B
 */
extern void CDGetCursorBounds(
#if NeedFunctionPrototypes
	Widget		/* object */,
	XRectangle *	/* bounds */
#endif
);


/*
 * void CDSetFonts(Widget object, XFontStruct **fontset, Cardinal num_fonts)
 *		(note: fontset is an array of (XFontStruct *).)
 *	$BIA2h$K(B fontset $B$G;XDj$5$l$k%U%)%s%H$r;HMQ$9$k$h$&$K@_Dj$9$k(B
 *	$B%*%V%8%'%/%H$O(B fontset $B$NCf$+$i<+J,$N;HMQ$9$k%-%c%i%/%?%;%C%H$N(B
 *	$B%U%)%s%H$rA*$s$G@_Dj$9$k(B
 *	$B$b$7!"I,MW$J%-%c%i%/%?%;%C%H$N%U%)%s%H$,$J$1$l$PE,Ev$J%U%)%s%H$r(B
 *	$B@_Dj$9$k(B
 *	$B$3$l$OB>$N%-%c%i%/%?%;%C%H$N%U%)%s%H$K9g$o$;$FA*$V$N$,$h$$$H(B
 *	$B;W$o$l$k$,!"%G%U%)%k%H%U%)%s%H$G4V$K9g$o$;$F$b$h$$(B
 *	$B$b$7(B num_fonts $B$,(B 0 $B$G$"$l$P3F%*%V%8%'%/%H$N%G%U%)%k%H%U%)%s%H$r(B
 *	$B;HMQ$9$k$h$&$K@_Dj$5$l$k(B
 */
extern void CDSetFonts(
#if NeedFunctionPrototypes
	Widget		/* object */,
	XFontStruct **	/* fontset */,
	Cardinal	/* num_fonts */
#endif
);

/*
 * void CDSetBlockCursor(Widget object, XRectangle *shape)
 *	shape $B$G;XDj$5$l$k%V%m%C%/%+!<%=%k$r@_Dj$9$k(B
 *	shape $B$N;XDj$O(B CDGetCursorBounds() $B$N(B bounds $B$HF1$8$G$"$k(B
 */
extern void CDSetBlockCursor(
#if NeedFunctionPrototypes
	Widget		/* object */,
	XRectangle *	/* shape */
#endif
);

#endif /* _ConvDisplay_h */
