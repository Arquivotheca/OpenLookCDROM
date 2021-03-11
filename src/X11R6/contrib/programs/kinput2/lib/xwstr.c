/*
 *	xwstr.c
 */

/*
 * Copyright (c) 1989  Software Research Associates, Inc.
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
 *		ishisone@sra.co.jp
 */

/*
 * wide character string $B$rMQ$$$F%F%-%9%H$rI=<($9$k$?$a$N%i%$%V%i%j(B
 *
 * wide character $B$NFCD9$r@8$+$7!":GBgF1;~$K(B 4 $B<oN`$N%U%)%s%H$r(B
 * $B07$&$3$H$,$G$-$k!#(B
 *
 *	GSet	bit $BI=8=(B		UJIS $B$G$N3d$jEv$F(B
 *	----------------------------------------------
 *	G0	0xxxxxxx0xxxxxxx	ASCII
 *	G1	1xxxxxxx1xxxxxxx	$B4A;z(B
 *	G2	0xxxxxxx1xxxxxxx	$BH>3Q$+$J(B
 *	G3	1xxxxxxx0xxxxxxx	$B30;z(B
 *
 * XWSGC XWSSetGCSet(GC gc0, GC gc1, GC gc2, GC gc3)
 *	$B;XDj$5$l$?(B 4$B$D$N(B GC $B$rAH$_9g$o$;$F(B XWSGC $B$r:n$k!#(B
 *	G0, G1, G2, G3 $B$NJ8;z$KBP$7$F$=$l$>$l(B gc0, gc1, gc2, gc3 $B$,(B
 *	$B;H$o$l$k!#0z?t(B gc? $B$K$O(B NULL $B$r;XDj$7$F$b$h$$!#$=$N>l9g$K$O(B
 *	$BBP1~$9$k%-%c%i%/%?%;%C%H$NJ8;z$O=q$+$l$J$$!#(B
 *	$B$J$*(B Xt $B$NCf$G;HMQ$9$k;~$K$O!"(Bxtwstr.c $B$NCf$N(B XtWSGetGCSet()
 *	$B$r;H$&J}$,$h$$$@$m$&!#(B
 *
 * void XWSSetMapping(XWSGC gcset, int g0map, int g1map, int g2map, int g3map)
 *	G0, G1, G2, G3 $B$NJ8;z$rIA2h$9$k;~$K%U%)%s%H$N(B GL / GR $B$N$I$A$i$r(B
 *	$B;HMQ$9$k$+$r@_Dj$9$k!#(B0 $B$r;XDj$9$k$H(B GL $B$,;HMQ$5$l!"(B1 $B$r;XDj$9$k$H(B
 *	GR $B$,;HMQ$5$l$k!#(B-1 $B$,;XDj$5$l$?>l9g$K$OA0$N@_Dj$,$=$N$^$^;H$o$l$k!#(B
 *	$B%G%U%)%k%H$N@_Dj(B ($B$D$^$j(B XWSSetGCSet() $B$^$?$O(B XtWSGetGCSet() $B$G(B
 *	gcset $B$,:n$i$l$?;~(B) $B$O$9$Y$F(B GL $B$r;HMQ$9$k$h$&$K$J$C$F$$$k!#(B
 *
 * int XWSDrawString(Display *d, Drawable w, XWSGC gcset,
 *		      int x, int y, wchar *wstr, int len)
 * int XWSDrawImageString(Display *d, Drawable w, XWSGC gcset,
 *		   int x, int y, wchar *wstr, int len)
 *	$B$=$l$>$l!"(BXlib $B$N(B XDrawString() / XDrawImageString() $B$K(B
 *	$BBP1~$9$k4X?t!#;XDj$5$l$?0LCV$K;XDj$5$l$?(B GC $B%;%C%H$rMQ$$$F(B
 *	$B%F%-%9%H$r=q$/!#0z?t(B len $B$OJ8;zNs(B wstr $B$NJ8;z?t$G$"$k!#(B
 *	$B%P%$%H?t$G$O$J$$$3$H$KCm0U!#(B
 *	$B$J$*!"(BXDrawString() $B$J$I$H$O0[$J$j!"%j%?!<%s%P%j%e!<$H$7$F(B
 *	$B=q$$$?J8;z$NI}$rJV$9!#(B
 *
 * int XWSTextWidth(XWSGC gcset, wchar *wstr, int len)
 * void XWSTextExtents(XWSGC gcset, wchar *wstr, int len,
 *			int *ascent, int *descent, XCharStruct *overall)
 *	$B$=$l$>$l!"(BXlib $B$N(B XTextWidth() / XTextExtents() $B$KBP1~$9$k(B
 *	$B4X?t!#;XDj$5$l$?J8;zNs$NI}!&Bg$-$5$rJV$9!#(B
 *
 * void XWSFontHeight(XWSGC gcset, wchar *wstr, int len,
 *			int *ascent, int *descent)
 *	$B0z?t(B wstr $B$G;XDj$5$l$?J8;z$KBP1~$9$k%U%)%s%H$N(B ascent/descent $B$N(B
 *	$B:GBgCM$rJV$9!#(Bwstr $B$K(B NULL $B$r;XDj$9$k$H!"(BXWSGC $B$K%;%C%H$5$l$?(B
 *	$B$9$Y$F$N%U%)%s%H$N(B ascent/descent $B$N:GBgCM$rJV$9!#(B
 */

#ifndef lint
static char	*rcsid = "$Id: xwstr.c,v 2.8 1994/05/30 10:27:42 ishisone Rel $";
#endif

#include <X11/Xlib.h>
#if defined(XlibSpecificationRelease) && XlibSpecificationRelease > 4
#include <X11/Xfuncs.h>
#endif
#include "WStr.h"
#include "XWStr.h"

#ifdef __STDC__
#include <stdlib.h>
#else
extern char *malloc();
#endif

#define G0MASK	0x0000
#define G1MASK	0x8080
#define G2MASK	0x0080
#define G3MASK	0x8000

#define IS2B(f)	(((f)->max_byte1 > 0) || ((f)->max_char_or_byte2 > 255))
#define MIN(a, b)	((a) > (b) ? (b) : (a))
#define MAX(a, b)	((a) > (b) ? (a) : (b))

#ifndef NULL
#define NULL	0
#endif

#define bufsize	256

#ifdef __STDC__
/* static function prototype */
static int flushstr(Display *, Drawable,  FontEnt *, int, int,
		    XChar2b *, XChar2b *, int);
static int wsdrawstring(Display *, Drawable, XWSGC, int, int,
			wchar *, int, int);
#else
static int flushstr();
static int wsdrawstring();
#endif

XWSGC
XWSSetGCSet(dpy, gc0, gc1, gc2, gc3)
Display *dpy;
GC gc0;
GC gc1;
GC gc2;
GC gc3;
{
	XWSGC	gcset;
	int	i;

	gcset = (XWSGC)malloc(sizeof(XWSGCSet));
	if (gcset == NULL)
		return (XWSGC)NULL;

	gcset->fe[0].gc = gc0;
	gcset->fe[1].gc = gc1;
	gcset->fe[2].gc = gc2;
	gcset->fe[3].gc = gc3;

	for (i = 0; i < 4; i++) {
		if (gcset->fe[i].gc == NULL) {
			gcset->fe[i].font = NULL;
		} else {
			XFontStruct	*font;
			gcset->fe[i].font = font = XQueryFont(dpy, XGContextFromGC(gcset->fe[i].gc));
			gcset->fe[i].flag = FONTQUERY;
			if (IS2B(font))
				gcset->fe[i].flag = TWOB;
		}
	}

	return gcset;
}

void
XWSSetMapping(gcset, g0map, g1map, g2map, g3map)
XWSGC gcset;
int g0map;
int g1map;
int g2map;
int g3map;
{
	int	map[4];
	int	i;

	map[0] = g0map; map[1] = g1map; map[2] = g2map; map[3] = g3map;
	for (i = 0; i < 4; i++) {
		if (map[i] < 0)
			continue;
		else if (map[i] == 0)
			gcset->fe[i].flag &= ~GRMAPPING;
		else
			gcset->fe[i].flag |= GRMAPPING;
	}
}

int
XWSDrawString(d, w, gcset, x, y, wstr, len)
Display *d;
Drawable w;
XWSGC gcset;
int x;
int y;
wchar *wstr;
int len;
{
	return wsdrawstring(d, w, gcset, x, y, wstr, len, 0);
}

int
XWSDrawImageString(d, w, gcset, x, y, wstr, len)
Display *d;
Drawable w;
XWSGC gcset;
int x;
int y;
wchar *wstr;
int len;
{
	return wsdrawstring(d, w, gcset, x, y, wstr, len, 1);
}

void
XWSFontHeight(gcset, wstr, len, ascent, descent)
XWSGC gcset;
wchar *wstr;
int len;
int *ascent;
int *descent;
{
	FontEnt	*fep = &(gcset->fe[0]);
	int	i;
	int	asc = 0;
	int	dsc = 0;

	if (wstr) {
		while (len-- > 0) {
			i = GSET(*wstr++);
			if (fep[i].font) {
				if ((fep[i].font)->ascent > asc)
					asc = (fep[i].font)->ascent;
				if ((fep[i].font)->descent > dsc)
					dsc = (fep[i].font)->descent;
			}
		}
	} else {
		for (i = 0; i < 4; i++) {
			if (fep[i].font) {
				if ((fep[i].font)->ascent > asc)
					asc = (fep[i].font)->ascent;
				if ((fep[i].font)->descent > dsc)
					dsc = (fep[i].font)->descent;
			}
		}
	}
	*ascent = asc;
	*descent = dsc;
}

int
XWSTextWidth(gcset, wstr, len)
XWSGC gcset;
wchar *wstr;
int len;
{
	XChar2b			buf[bufsize];
	XChar2b			*cp;
	wchar			*wstr1 = wstr + len;
	XChar2b			*cpend = buf + bufsize;
	int			c;
	int			width = 0;
	int			gmask, gset;
	FontEnt			*fe;
	int			is2b;
	int			grmap;

	while (wstr < wstr1) {
		gmask = *wstr & 0x8080;

		switch (gmask) {
		case G0MASK:
			gset = 0;
			break;
		case G1MASK:
			gset = 1;
			break;
		case G2MASK:
			gset = 2;
			break;
		case G3MASK:
			gset = 3;
			break;
		}

		fe = &gcset->fe[gset];
		is2b = fe->flag & TWOB;
		grmap = (fe->flag & GRMAPPING) ? 0x80 : 0;
		cp = buf;

		if (fe->font == NULL) {
			while (wstr < wstr1 && (*wstr & 0x8080) == gmask)
				wstr++;
			continue;
		}

		while (wstr < wstr1 && ((c = *wstr) & 0x8080) == gmask) {
			if (cp >= cpend - 1) {
				/* flush */
				width += XTextWidth16(fe->font, buf, cp - buf);
				cp = buf;
			}
			if (is2b)
				cp->byte1 = ((c >> 8) & 0x7f) | grmap;
			else
				cp->byte1 = 0;
			cp->byte2 = (c & 0x7f) | grmap;
			cp++;
			wstr++;
		}

		if (cp == buf)
			continue;

		/* flush */
		width += XTextWidth16(fe->font, buf, cp - buf);
	}

	return width;
}

void
XWSTextExtents(gcset, wstr, len, ascent, descent, overall)
XWSGC gcset;
wchar *wstr;
int len;
int *ascent;
int *descent;
XCharStruct *overall;
{
	XChar2b			buf[bufsize];
	XChar2b			*cp;
	wchar			*wstr1 = wstr + len;
	XChar2b			*cpend = buf + bufsize;
	int			c;
	int			gmask, gset;
	FontEnt			*fe;
	int			is2b;
	int			grmap;
	int			dir, as, ds;
	XCharStruct		oa;

	*ascent = *descent = 0;
	(void)bzero(overall, sizeof(XCharStruct));

	while (wstr < wstr1) {
		gmask = *wstr & 0x8080;

		switch (gmask) {
		case G0MASK:
			gset = 0;
			break;
		case G1MASK:
			gset = 1;
			break;
		case G2MASK:
			gset = 2;
			break;
		case G3MASK:
			gset = 3;
			break;
		}

		fe = &gcset->fe[gset];
		is2b = fe->flag & TWOB;
		grmap = (fe->flag & GRMAPPING) ? 0x80 : 0;
		cp = buf;

		if (fe->font == NULL) {
			while (wstr < wstr1 && (*wstr & 0x8080) == gmask)
				wstr++;
			continue;
		}

		while (wstr < wstr1 && ((c = *wstr) & 0x8080) == gmask) {
			if (cp >= cpend - 1) {
				/* flush */
				XTextExtents16(fe->font, buf, cp - buf,
					       &dir, &as, &ds, &oa);
				cp = buf;
				*ascent = MAX(*ascent, as);
				*descent = MAX(*descent, ds);
				overall->lbearing = MIN(overall->lbearing,
							overall->width + oa.lbearing);
				overall->rbearing = MAX(overall->rbearing,
							overall->width + oa.rbearing);
				overall->width += oa.width;
				overall->ascent = MAX(overall->ascent, oa.ascent);
				overall->descent = MAX(overall->descent, oa.descent);
			}
			if (is2b)
				cp->byte1 = ((c >> 8) & 0x7f) | grmap;
			else
				cp->byte1 = 0;
			cp->byte2 = (c & 0x7f) | grmap;
			cp++;
			wstr++;
		}

		if (cp == buf)
			continue;

		/* flush */
		XTextExtents16(fe->font, buf, cp - buf, &dir, &as, &ds, &oa);
		*ascent = MAX(*ascent, as);
		*descent = MAX(*descent, ds);
		overall->lbearing = MIN(overall->lbearing,
					overall->width + oa.lbearing);
		overall->rbearing = MAX(overall->rbearing,
					overall->width + oa.rbearing);
		overall->width += oa.width;
		overall->ascent = MAX(overall->ascent, oa.ascent);
		overall->descent = MAX(overall->descent, oa.descent);
	}
}


/*
 * private functions
 */

static int
wsdrawstring(d, w, gcset, x, y, wstr, len, image)
Display *d;
Drawable w;
XWSGC gcset;
int x;
int y;
wchar *wstr;
int len;
int image;
{
	XChar2b			buf[bufsize];
	XChar2b			*cp;
	wchar			*wstr1 = wstr + len;
	XChar2b			*cpend = buf + bufsize;
	int			c;
	int			sx = x;
	int			gmask, gset;
	FontEnt			*fe;
	int			is2b;
	int			grmap;

	while (wstr < wstr1) {
		gmask = *wstr & 0x8080;

		switch (gmask) {
		case G0MASK:
			gset = 0;
			break;
		case G1MASK:
			gset = 1;
			break;
		case G2MASK:
			gset = 2;
			break;
		case G3MASK:
			gset = 3;
			break;
		}

		fe = &gcset->fe[gset];
		is2b = fe->flag & TWOB;
		grmap = (fe->flag & GRMAPPING) ? 0x80 : 0;
		cp = buf;

		if (fe->gc == NULL) {
			while (wstr < wstr1 && (*wstr & 0x8080) == gmask)
				wstr++;
			continue;
		}
		while (wstr < wstr1 && ((c = *wstr) & 0x8080) == gmask) {
			if (cp >= cpend - 1) {
				/* flush */
				x += flushstr(d, w, fe, x, y, buf, cp, image);
				cp = buf;
			}
			if (is2b)
				cp->byte1 = ((c >> 8) & 0x7f) | grmap;
			else
				cp->byte1 = 0;
			cp->byte2 = (c & 0x7f) | grmap;
			cp++;
			wstr++;
		}
		/* flush */
		x += flushstr(d, w, fe, x, y, buf, cp, image);
		cp = buf;
	}

	return x - sx;
}

static int
flushstr(d, w, fe, x, y, cp0, cp1, image)
Display *d;
Drawable w;
FontEnt *fe;
int x;
int y;
XChar2b *cp0;
XChar2b *cp1;
int image;
{
	if (cp0 >= cp1 || fe->gc == NULL)
		return 0;

	if (image)
		XDrawImageString16(d, w, fe->gc, x, y, cp0, cp1 - cp0);
	else
		XDrawString16(d, w, fe->gc, x, y, cp0, cp1 - cp0);
	return XTextWidth16(fe->font, cp0, cp1 - cp0);
}
