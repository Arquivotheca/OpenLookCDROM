/* $Id: CandPanel.h,v 1.4 1991/03/22 18:13:39 ishisone Rel $ */
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

#ifndef _CandidatePanel_h
#define _CandidatePanel_h

#include "ICtypes.h"

/*
  CandidatePanel new resources:

  name			class		type		default		access
  ----------------------------------------------------------------------------
  foreground		Foreground	Pixel		default		CSG
  horizontalSpacing	Spacing		Dimension	6		CSG
  verticalSpacing	Spacing		Dimension	4		CSG
  list			List		Pointer		NULL		CSG
  numStrings		NumStrings	Int		0		CSG
  defaultWidth		DefaultWidth	Dimension	400		CSG
  currentItem		CurrentItem	Int		0		CSG
  cursor		Cursor		Cursor		parent		CSG
  callback		Callback	Callback	--

  CandidatePanel widget $B$K$O<B:]$KJ8;zNs$rI=<($9$k$?$a$N(B widget $B$r;R(B widget
  $B$H$7$FM?$($kI,MW$,$"$k!#$3$N(B widget $B$N%/%i%9$O(B ConvDisplayObject $B%/%i%9$N(B
  $B%5%V%/%i%9$G$J$1$l$P$J$i$J$$!#(B
*/

#define XtCSpacing "Spacing"
#define XtNhorizontalSpacing "horizontalSpacing"
#define XtNverticalSpacing "verticalSpacing"

#define XtCList "List"
#define XtNlist "list"
#define XtCNumStrings "NumStrings"
#define XtNnumStrings "numStrings"

#define XtCDefaultWidth "DefaultWidth"
#define XtNdefaultWidth "defaultWidth"

#define XtCCurrentItem "CurrentItem"
#define XtNcurrentItem "currentItem"

#define XtNcursor "cursor"

typedef struct _CandidatePanelClassRec*	CandidatePanelWidgetClass;
typedef struct _CandidatePanelRec*		CandidatePanelWidget;

extern WidgetClass candidatePanelWidgetClass;

/*
 * Public Interface Functions
 */

/*
 * void CPanelSetList(Widget w, ICString *list, int nstrings,
 *		      int current, int resize)
 *	$B%j%9%H$r?7$?$K%;%C%H$9$k(B
 *	current $B$K$O%+%l%s%H%"%$%F%`$NHV9f$r;XDj$9$k(B
 *	resize $B$,(B True $B$J$i$P@_Dj$5$l$?%j%9%H$K9g$o$;$F%j%5%$%:$r9T$J$&(B
 *	list $B$,(B NULL $B$J$i$P%j%9%H$OJQ99$5$l$J$$$,!"%5%$%:$N7W;;$r$d$jD>$9$N$G(B
 *	$B;R(B widget (ConvDisplayObject) $B$N%U%)%s%H$J$I$N%j%=!<%9$rJQ$($?;~$K$O(B
 *		CPanelSetList(w, NULL, 0, 0, 0);
 *	$B$r<B9T$9$k$H$h$$(B
 */
extern void CPanelSetList(
#if NeedFunctionPrototypes
	Widget		/* w */,
	ICString *	/* list (or NULL) */,
	int		/* nstrings */,
	int		/* current */,
	int		/* resize */
#endif
);

/*
 * void CPanelSetCurrent(Widget w, int current)
 *	$B%+%l%s%H%"%$%F%`$r(B idx $B$G;XDj$7$?HV9f$N$b$N$K%;%C%H$9$k(B
 */
extern void CPanelSetCurrent(
#if NeedFunctionPrototypes
	Widget		/* w */,
	int		/* current */
#endif
);

/*
 * void CPanelMoveCurrent(Widget w, int dir)
 *	$B%+%l%s%H%"%$%F%`$r(B dir $B$G;XDj$7$?J}8~$N%"%$%F%`$KJQ$($k(B
 *	dir $B$NCM$O(B ICtypes.h $B$KDj5A$5$l$F$$$k(B
 */
extern void CPanelMoveCurrent(
#if NeedFunctionPrototypes
	Widget		/* w */,
	int		/* dir */
#endif
);

#endif
