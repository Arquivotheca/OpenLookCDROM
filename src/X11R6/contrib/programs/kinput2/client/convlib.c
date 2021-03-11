/*
 *	convlib2.c -- X11 $B%D!<%k%-%C%HJQ49F~NOMQ%i%$%V%i%j(B
 *
 *		ishisone@sra.co.jp
 */

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

/*
 * --$B%$%s%?!<%U%'%$%9(B--
 *
 * $BMQ0U$5$l$F$$$k%U%!%s%/%7%g%s$O<!$N(B4$B$D(B ($B$=$N$&$A(B1$B$D$O(B backward
 * compatibility $B$N$?$a(B) $B$@$1$G$"$k!#;DG0$J$,$i(B X11R5 $B$G:N$jF~$l$i$l$kI8(B
 * $B=`F~NO%$%s%?!<%U%'%$%9(B XIM $B$K$O=>$C$F$$$J$$!#$7$+$7(B Xt $B%D!<%k%-%C%H$r(B
 * $BMxMQ$9$k%W%m%0%i%`$J$i(B XIM $B$h$j$b$O$k$+$K4JC1$KAH$_9~$`$3$H$,$G$-$k$@(B
 * $B$m$&!#(B
 *
 * int _beginConversionWithAttributes(
 *	Widget w,		$BJQ49F~NO$r$7$?$$(B widget
 *	Atom catom,		$B%;%l%/%7%g%s%"%H%`(B eg "_JAPANESE_CONVERSION"
 *	Atom tatom,		$BJQ49%F%-%9%H%?%$%W(B eg "COMPOUND_TEXT"
 *	void (*inputproc)(),	$BJQ49%F%-%9%HF~NO%3!<%k%P%C%/4X?t(B
 *	void (*startendproc)(),	$BJQ493+;O(B / $B=*N;(B / $B%"%\!<%H(B $B%3!<%k%P%C%/4X?t(B
 *	XtPointer client_data,	$B%3!<%k%P%C%/4X?t$KEO$5$l$k%G!<%?(B
 *	ArgList attrs,		$BJQ49B0@-%j%9%H(B
 *	Cardinal nattrs		$BB0@-%j%9%H$N9`L\?t(B
 *	)
 *
 *	$BJQ49$r3+;O$9$k!#$b$C$H@53N$K$$$&$H!"0z?t(B catom $B$K;XDj$5$l$?%;%l(B
 *	$B%/%7%g%s%"%H%`$N%*!<%J!<(B ($B$3$l$,JQ49%U%m%s%H%(%s%I!"Nc$($P(B
 *	kinput2 $B$G$"$k(B) $B$rC5$7!"JQ49$N%j%/%(%9%H$rAw$k!#F1;~$K0z?t(B attrs
 *	$B$G;XDj$5$l$kJQ49B0@-(B ($BNc$($P%+!<%=%k$N0LCV(B) $B$r%U%m%s%H%(%s%I$KDL(B
 *	$BCN$9$k!#JQ49B0@-$K$D$$$F$O8e$GJL$K@bL@$9$k!#%U%m%s%H%(%s%I$,B8:_(B
 *	$B$7$J$$;~$K$O(B -1 $B$r!"$=$l0J30$O(B 0 $B$rJV$9!#(B
 *
 *	startendproc $B$OJQ49$N>uBV$NJQ2=$r%"%W%j%1!<%7%g%s$KCN$i$;$k$?$a(B
 *	$B$N%3!<%k%P%C%/$G$"$k!#$3$N%U%!%s%/%7%g%s$O<!$N$h$&$J7A<0$G8F$S=P(B
 *	$B$5$l$k!#(B
 *
 *	(*startendproc)(Widget w, Atom catom, int state,
 *			XtPointer client_data, Window convwin)
 *
 *	w, catom, client_data $B$O(B _beginConversionWithAttributes() $B$G;XDj(B
 *	$B$7$?$b$N$HF1$8$b$N$,EO$5$l$k!#(Bstate $B$K$O(B3$B<oN`$"$j!"$=$l$>$l(B
 *		 0: $B%U%m%s%H%(%s%I$,JQ49%j%/%(%9%H$r<u$1IU$1$?(B
 *		-1: $B%U%m%s%H%(%s%I$,JQ49%j%/%(%9%H$r5qH]$7$?(B
 *		 1: $BJQ49$,=*N;$7$?(B
 *	$B$H$$$&$3$H$rI=$9!#(Bstate $B$,(B 0 $B$N;~!"(Bconvwin $B$K$O<B:]$KJQ49=hM}(B
 *	$B$,9T$J$o$l$k%&%#%s%I%&$N(B ID $B$,F~$k!#$3$l$O(B eventCaptureMethod 
 *	$B$r(B "none" $B$K$7$F%/%i%$%"%s%H$KMh$?%-!<%$%Y%s%H$r%U%m%s%H%(%s%I(B
 *	$B$K%U%)%o!<%I$9$k;~$J$I$K;HMQ$5$l$k!#(B
 *
 *	$B:F$S(B _beginConversionWithAttributes() $B$N0z?t$N@bL@$KLa$C$F(Btatom 
 *	$B$O%U%m%s%H%(%s%I$+$iAw$i$l$F$/$kJ8;zNs$N%(%s%3!<%G%#%s%0$N;XDj$G(B
 *	$B$"$k!#(B
 *	kinput2 $B%W%m%H%3%k$G$O!"%U%m%s%H%(%s%I$KBP$7$F(B COMPOUND_TEXT$B%(%s(B
 *	$B%3!<%G%#%s%0$r%5%]!<%H$9$k$3$H$7$+5a$a$F$$$J$$$N$G!"B>$N%(%s%3!<(B
 *	$B%G%#%s%0$r%5%]!<%H$9$k$+$I$&$+$O%U%m%s%H%(%s%I$N%$%s%W%j%a%s%F!<(B
 *	$B%7%g%s$K0MB8$9$k!#=>$C$F(B COMPOUNT_TEXT $B0J30$N%(%s%3!<%G%#%s%0$r(B
 *	$B;XDj$9$k$H!"<B:]$KAw$i$l$F$/$kJ8;zNs$N%(%s%3!<%G%#%s%0$,;XDj$7$?(B
 *	$B$b$N$H0[$J$k$3$H$b$"$jF@$k$N$GCm0U$9$k$3$H!#(B
 *
 *	$B3NDjJ8;zNs$,JQ49%U%m%s%H%(%s%I$+$iAw$i$l$F$/$k$H(B inputproc $B$K;X(B
 *	$BDj$5$l$?%3!<%k%P%C%/%U%!%s%/%7%g%s$,<!$N$h$&$J7A<0$G8F$P$l$k!#(B
 *
 *	(*inputproc)(Widget w, Atom catom,
 *			Atom proptype, int propformat,
 *			unsigned long propsize, unsigned char *propvalue,
 *			XtPointer client_data)
 *
 *	w $B$H(B catom$B!"$=$l$K(B client_data $B$O(B 
 *	_beginConversionWithAttributes() $B$G;XDj$7$?$b$N$G$"$k!#(B
 *
 *	proptype $B$OJ8;zNs$N%(%s%3!<%G%#%s%0!"(Bpropformat $B$O(B1$BJ8;z$N%S%C%H(B
 *	$BD9!"(Bpropsize $B$,D9$5!"$=$7$F(B propvalue $B$K<B:]$NJ8;zNs%G!<%?$,F~$C(B
 *	$B$F$$$k!#<B$OJ8;zNs$O%U%m%s%H%(%s%I$+$i(B X $B$N%&%#%s%I%&%W%m%Q%F%#(B
 *	$B$H$7$FEO$5$l$F$*$j!"$3$l$i$N%Q%i%a!<%?$O$=$N%W%m%Q%F%#$N%Q%i%a!<(B
 *	$B%?$=$N$b$N$G$"$k!#=>$C$F(B XGetWindowProperty() $B$N@bL@$r;2>H$9$k$H(B
 *	$B3F%Q%i%a!<%?$N0UL#$,$O$C$-$j$o$+$k$@$m$&!#(B
 *
 * void _changeConversionAttributes(
 *	Widget	w,
 *	Atom catom,		$B%;%l%/%7%g%s%"%H%`(B eg "_JAPANESE_CONVERSION"
 *	ArgList attrs,		$BJQ49B0@-%j%9%H(B
 *	Cardinal nattrs		$BB0@-%j%9%H$N9`L\?t(B
 *	)
 *
 *	$BJQ49Cf$KJQ49B0@-$rJQ2=$5$;$k!#Nc$($P%+!<%=%k0LCV$,JQ$o$C$?$H$-$K(B
 *	$B$3$N%U%!%s%/%7%g%s$G$=$l$r%U%m%s%H%(%s%I$KCN$i$;$k$3$H$,$G$-$k!#(B
 *
 * void _endConversion(
 *	Widget w,
 *	Atom catom,		$B%;%l%/%7%g%s%"%H%`(B eg "_JAPANESE_CONVERSION"
 *	Boolean throwaway	$B$3$N8eMh$?JQ497k2L$r<u$1$H$k$+$I$&$+(B
 *	)
 *
 *	$BJQ49$r=*N;$5$;$k!#DL>o!"JQ49$N=*N;$OJQ49%U%m%s%H%(%s%I$,9T$J$&$N(B
 *	$B$GFC$K%"%W%j%1!<%7%g%s$+$i$3$N%U%!%s%/%7%g%s$r;H$C$F6/@)E*$K=*N;(B
 *	$B$5$;$kI,MW$O$J$$!#(B
 *	$B0z?t(B throwaway $B$,(B True $B$@$H$3$N%U%!%s%/%7%g%s$r<B9T$7$?8e$K%U%m(B
 *	$B%s%H%(%s%I$+$iAw$i$l$?J8;zNs$rL5;k$9$k!#(B
 *
 * void _beginConversion(	-- provided for backward compatibility
 *	Widgete w,		$BJQ49F~NO$r$7$?$$(B widget
 *	Atom catom,		$B%;%l%/%7%g%s%"%H%`(B eg "_JAPANESE_CONVERSION"
 *	Atom tatom,		$BJQ49%F%-%9%H%?%$%W(B eg "COMPOUND_TEXT"
 *	void (*inputproc)(),	$BJQ49%F%-%9%HF~NO%3!<%k%P%C%/4X?t(B
 *	XtPointer client_data	$B%3!<%k%P%C%/4X?t$KEO$5$l$k%G!<%?(B
 *	)
 *
 *	$B$3$l$O(B backward compatibility $B$N$?$a$KMQ0U$5$l$?%U%!%s%/%7%g%s$G(B
 *	$B$"$k!#$3$N%U%!%s%/%7%g%s$G$OJQ49B0@-$N;XDj$,0l@Z$G$-$J$$$7!"JQ49(B
 *	$B>uBV$NJQ2=$bCN$k$3$H$,$G$-$J$$$N$G!"$G$-$k$@$1(B
 *	_beginConversionWithAttributes() $B$rMQ$$$k$N$,K>$^$7$$!#(B
 *
 *
 * --$B%;%l%/%7%g%s%"%H%`(B--
 *
 * _beginConversionWithAttributes() $B$J$I$K;XDj$9$k%;%l%/%7%g%s%"%H%`$O(B
 * $BF~NO$7$?$$8@8l$K$h$C$F0[$J$j!"<!$N$h$&$JL>A0$K$J$C$F$$$k!#(B
 *	"_<$B8@8lL>(B>_CONVERSION"
 * $BNc$($PF|K\8l$N>l9g$O(B "_JAPANESE_CONVERSION" $B$G$"$k!#(B
 *
 *
 * --$BJQ49B0@-%j%9%H(B--
 *
 * $BJQ49B0@-$O(B XtSetValues() $B$J$I$G;HMQ$5$l$k%j%9%H$HF1$87A(B (ArgList) $B$G$"$k!#(B
 * $BB0@-$H$7$F;XDj$G$-$k$N$O<!$N9`L\$G$"$k!#4pK\E*$K$3$l$i$NB0@-$O(B XIM $B$N(B
 * $B;EMM$G$NDj5A$r$=$N$^$^:NMQ$7$F$$$k$N$G!"5?LdE@$,$"$l$P(B XIM $B$N%I%-%e%a%s%H$r(B
 * $B;2>H$7$F$[$7$$!#(B
 *
 * "inputStyle" : $BCM(B String
 *	$BF~NO%9%?%$%k$r;XDj$9$k!#CM$O(B
 *		"root" (root window style):	$BJL%&%#%s%I%&$K$h$kJQ49(B
 *		"off" (off-the-spot style):	$B;XDj$7$?JQ49NN0hFb$G$NJQ49(B
 *		"over" (over-the-spot style):	$B$=$N>lJQ49(B
 *	$B$N$I$l$+$NJ8;zNs$r;XDj$9$k!#(B
 *
 * "focusWindow" : $BCM(B Window
 *	$BJQ49$r9T$J$&%&%#%s%I%&$r;XDj$9$k!#$3$l$,;XDj$5$l$?$J$+$C$?>l9g$K(B
 *	$B$O(B _beginConversionWithAttributes() $B$G;XDj$7$?(B Widget $B$N%&%#%s%I(B
 *	$B%&$,;H$o$l$k$N$GDL>o$O;XDj$7$J$/$F$h$$!#(B
 *
 * "spotX", "spotY" : $BCM(B Position
 *	$B%9%]%C%H%m%1!<%7%g%s$r;XDj$9$k!#$3$l$OF~NO%9%?%$%k$,(B 
 *	over-the-spot $B$N;~$N$_M-8z$G$"$k!#J8;z$r=q$-;O$a$k0LCV$r;XDj$9$k(B
 *	$B$,!"(BspotY $B$O%Y!<%9%i%$%s$N0LCV$G$"$k$3$H$KCm0U!#(B
 *	spotX$B!"(BspotY $B$N$&$AJRJ}$@$1;XDj$7$F$bL58z!#(B
 *
 * "foreground", "background" : $BCM(B Pixel
 *	$BA07J?'!"GX7J?'$r;XDj$9$k!#$3$l$bJRJ}$@$1;XDj$7$F$bL58z!#(B
 *
 * "eventCaptureMethod" : $BCM(B String
 *	$B%U%m%s%H%(%s%I$,$I$N$h$&$KF~NO%$%Y%s%H$r$H$k$+$r;XDj$9$k!#(B
 *		"none"		$B2?$b$7$J$$(B
 *		"inputOnly"	InputOnly $B%&%#%s%I%&$K$h$k(B
 *		"focusSelect"	$B%U%)!<%+%9%&%#%s%I%&$N%$%Y%s%H$rD>@\(B
 *				$B%;%l%/%H$9$k(B
 *	$B$G$"$k!#(B
 *
 *	"$B2?$b$7$J$$(B" $B$r;XDj$7$?>l9g!"%"%W%j%1!<%7%g%s$OJQ49Cf$K%"%W%j%1!<(B
 *	$B%7%g%s$KMh$?%$%Y%s%H$r%U%m%s%H%(%s%I$KEO$7$F(B (XSendEvent() $B$r;H(B
 *	$B$&(B) $B$d$i$J$1$l$P$J$i$J$$!#$3$N%$%Y%s%H$N%U%)%o!<%I:n6H$O$3$N%i%$(B
 *	$B%V%i%j$G$O%5%]!<%H$7$F$$$J$$!#=>$C$F(B "$B2?$b$7$J$$(B" $B$r;XDj$9$k$3$H(B
 *	$B$O$"$^$j$*4+$a$7$J$$!#(B
 *
 *	"$B%U%)!<%+%9%&%#%s%I%&$N%$%Y%s%H$rD>@\%;%l%/%H$9$k(B" $B>l9g!"%"%W%j(B
 *	$B%1!<%7%g%s$OJQ49Cf$O(B sendevent $B%U%i%0$NN)$C$F$$$J$$%-!<%$%Y%s%H(B
 *	$B$rL5;k$7$J$/$F$O$J$i$J$$!#(Bsendevent $B%U%i%0$NN)$C$?%-!<%$%Y%s%H$O(B
 *	$B%U%m%s%H%(%s%I$+$iLa$5$l$?%$%Y%s%H$G$"$k2DG=@-$,$"$j!"$3$l$OL5;k(B
 *	$B$7$J$/$F$bNI$$$,!"EvA3$N$3$H$J$,$i%;%-%e%j%F%#$K$O5$$r$D$1$J$/$F(B
 *	$B$O$J$i$J$$!#(B
 *
 *	"InputOnly $B%&%#%s%I%&$K$h$k(B" $B$r;XDj$9$k$H%U%m%s%H%(%s%I$O%/%i%$(B
 *	$B%"%s%H$N%&%#%s%I%&(B ($B$3$l$O%U%)!<%+%9%&%#%s%I%&$G$O$J$/!"(B
 *	_beginConversionWithAttributes() $B$G;XDj$7$?(B Widget $B$N%&%#%s%I%&(B
 *	$B$G$"$k(B) $B$NA0$KF)L@$J%&%#%s%I%&$r:n$j!"$=$3$KMh$?%$%Y%s%H$r2#<h$j(B
 *	$B$9$k!#$3$N>l9g%$%Y%s%H$O%U%m%s%H%(%s%I$,>!<j$K$H$C$F$7$^$&$N$G%"(B
 *	$B%W%j%1!<%7%g%s$O%$%Y%s%H$K4X$7$F2?$b9M$($J$/$F$h$$!#=>$C$FJ}K!$H(B
 *	$B$7$F$O$3$l$,0lHV4JC1$G$"$k!#$7$+$7$J$,$iNc$($P(B click-to-type $B7A(B
 *	$B<0$N%&%#%s%I%&%^%M!<%8%c$r;H$C$?$j$7$F%-!<F~NO$N%U%)!<%+%9$,@_Dj(B
 *	$B$5$l$F$$$k>l9g$K$O$&$^$/$$$+$J$$!#(B
 *
 * "lineSpacing" : $BCM(B int
 *	$B9T4V3V$r;XDj$9$k!#%Y!<%9%i%$%s4V$N5wN%$r;XDj$9$k!#(B
 *
 * "clientArea" : $BCM(B XRectangle $B$X$N%]%$%s%?(B
 *	$BJQ49%F%-%9%H$NI=<($K;HMQ$9$kNN0h$r;XDj$9$k!#(B
 *
 * "statusArea" : $BCM(B XRectangle $B$X$N%]%$%s%?(B
 *	$B%b!<%II=<($K;HMQ$9$kNN0h$r;XDj$9$k!#(B
 *
 * "cursor" : $BCM(B Cursor
 *	$B;HMQ$9$k%+!<%=%k(B ($B%^%&%9%+!<%=%k$M(B) $B$r;XDj$9$k!#(B
 *
 * "fonts" : $BCM(B NULL $B%?!<%_%M!<%H$5$l$?(B XFontStruct * $B$NG[Ns(B
 *	$B;HMQ$9$k%U%)%s%H$r;XDj$9$k!#=gHV$O$I$&$G$b$h$$!#%U%m%s%H%(%s%IB&(B
 *	$B$GH=CG$9$k!#$?$@$7(B XLFD $B$K=>$o$J$$%U%)%s%H$r;XDj$5$l$k$H!"%U%m%s(B
 *	$B%H%(%s%I$G$=$N%-%c%i%/%?%;%C%H$,$o$+$i$:!"$=$N%U%)%s%H$,;H$o$l$J(B
 *	$B$$$3$H$,$"$k!#(B
 *
 * $B>e5-$NB0@-$N$&$A!"(BinputStyle $B$H(B eventCaptureMethod $B$OJQ49ESCf$G(B ($B$D$^$j(B
 * _changeConversionAttributes() $B$G(B) $BJQ99$9$k$3$H$O$G$-$J$$!#(B
 */

#ifndef lint
static char	*rcsid = "$Id: convlib2.c,v 1.12 1994/06/03 09:13:19 ishisone Rel $";
#endif

#include <X11/StringDefs.h>
#include <X11/Intrinsic.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include "ConvProto.h"

typedef struct {
    Display	*display;
    Atom	profileAtom;	/* "_CONVERSION_PROFILE" */
    Atom	typeAtom;	/* "_CONVERSION_ATTRIBUTE_TYPE" */
    Atom	versionAtom;	/* "PROTOCOL-2.0" */
    Atom	reqAtom;	/* "CONVERSION_REQUEST" */
    Atom	notifyAtom;	/* "CONVERSION_NOTIFY" */
    Atom	endAtom;	/* "CONVERSION_END" */
    Atom	endReqAtom;	/* "CONVERSION_END_REQUEST" */
    Atom	attrAtom;	/* "CONVERSION_ATTRIBUTE" */
    Atom	attrNotifyAtom;	/* "CONVERSION_ATTRIBUTE_NOTIFY" */
} ConversionAtoms;

typedef struct {
    Atom	convatom;
    Window	convowner;
    Atom	property;
    void	(*inputproc)();
    void	(*startendproc)();
    XtPointer	closure;
} ConversionContext;

static XContext	convertPrivContext;

#if __STDC__
/* function prototype */
static void callStart(Widget, ConversionContext *, Window);
static void callFail(Widget, ConversionContext *);
static void callEnd(Widget, ConversionContext *);
static long getInputStyle(String);
static long getCaptureMethod(String);
static ConversionAtoms *getAtoms(Widget);
static ConversionContext *getConversionContext(Widget);
static void recvConvAck(Widget, XtPointer, XEvent *, Boolean *);
static void getConv(Widget, XtPointer, XEvent *, Boolean *);
static Boolean setConvAttrProp(Widget, ArgList, Cardinal, Atom);
static int makeAttrData(Widget, ArgList, Cardinal, unsigned long **);
static Boolean checkProtocols(Display *, Window, ConversionAtoms *);
#else
static void callStart();
static void callFail();
static void callEnd();
static long getInputStyle();
static long getCaptureMethod();
static ConversionAtoms *getAtoms();
static ConversionContext *getConversionContext();
static void recvConvAck();
static void getConv();
static Boolean setConvAttrProp();
static int makeAttrData();
static Boolean checkProtocols();
#endif

static void
callStart(w, context, convwin)
Widget w;
ConversionContext *context;
Window convwin;
{
    if (context->startendproc != NULL) {
	(*context->startendproc)(w, context->convatom,
				 0, context->closure, convwin);
    }
}

static void
callFail(w, context)
Widget w;
ConversionContext *context;
{
    if (context->startendproc != NULL) {
	(*context->startendproc)(w, context->convatom,
				 -1, context->closure, None);
    }
}

static void
callEnd(w, context)
Widget w;
ConversionContext *context;
{
    if (context->startendproc != NULL) {
	(*context->startendproc)(w, context->convatom,
				 1, context->closure, None);
    }
}

static long
getInputStyle(s)
String s;
{
    String p;
    char buf[64];

    (void)strcpy(buf, s);
    for (p = buf; *p != '\0'; p++) {
	if ('A' <= *p && *p <= 'Z') *p += 'a' - 'A';
    }
    if (!strcmp(buf, "over")) return CONVARG_OVERTHESPOT;
    if (!strcmp(buf, "off")) return CONVARG_OFFTHESPOT;
    return CONVARG_ROOTWINDOW;
}

static long
getCaptureMethod(s)
String s;
{
    String p;
    char buf[64];

    (void)strcpy(buf, s);
    for (p = buf; *p != '\0'; p++) {
	if ('A' <= *p && *p <= 'Z') *p += 'a' - 'A';
    }
    if (!strcmp(buf, "none")) return CONVARG_NONE;
    if (!strcmp(buf, "focusselect")) return CONVARG_SELECT_FOCUS_WINDOW;
    return CONVARG_CREATE_INPUTONLY;
}

static ConversionAtoms *
getAtoms(w)
Widget	w;
{
    int i;
    Display *disp = XtDisplay(w);
    ConversionAtoms *cap;
    static ConversionAtoms *convatomp;
    static Cardinal ndisp = 0;
#define nalloc	2

    /*
     * $B%"%H%`$O%G%#%9%W%l%$$4$H$K0c$&$N$G!"(B
     * $B%G%#%9%W%l%$$4$H$K:n$i$J$/$F$O$J$i$J$$(B
     */

    /* $B$9$G$K%"%H%`$,:n$i$l$F$$$k$+$I$&$+D4$Y$k(B */
    cap = convatomp;
    for (i = 0; i < ndisp; i++, cap++) {
	if (cap->display == disp) return cap;
    }

    /*
     * $B$^$@:n$i$l$F$$$J$$$N$G?7$7$/:n$k(B
     */
    if (ndisp == 0) {
	/* $B:G=i$J$N$G(B Context $B$bF1;~$K:n$k(B */
	convertPrivContext = XUniqueContext();
	convatomp = (ConversionAtoms *)
	  XtMalloc(sizeof(ConversionAtoms) * nalloc);
	cap = convatomp;
    } else if (ndisp % nalloc == 0) {
	/* $B%5%$%:$rA}$d$9(B */
	convatomp = (ConversionAtoms *)
	  XtRealloc((char *)convatomp,
		    sizeof(ConversionAtoms) * (ndisp + nalloc));
	cap = convatomp + ndisp;
    } else {
	cap = convatomp + ndisp;
    }

    /* $B%G%#%9%W%l%$$NEPO?(B */
    cap->display = disp;

    /* Atom $B$N:n@.(B */
    cap->profileAtom = XInternAtom(disp, CONVERSION_PROFILE, False);
    cap->typeAtom = XInternAtom(disp, CONVERSION_ATTRIBUTE_TYPE, False);
    cap->versionAtom = XInternAtom(disp, PROTOCOL_VERSION, False);
    cap->reqAtom = XInternAtom(disp, "CONVERSION_REQUEST", False);
    cap->notifyAtom = XInternAtom(disp, "CONVERSION_NOTIFY", False);
    cap->endAtom = XInternAtom(disp, "CONVERSION_END", False);
    cap->endReqAtom = XInternAtom(disp, "CONVERSION_END_REQUEST", False);
    cap->attrAtom = XInternAtom(disp, "CONVERSION_ATTRIBUTE", False);
    cap->attrNotifyAtom = XInternAtom(disp, "CONVERSION_ATTRIBUTE_NOTIFY", False);

    ndisp++;

    return cap;
}

static ConversionContext *
getConversionContext(w)
Widget	w;
{
    ConversionContext *context;

    if (XFindContext(XtDisplay(w), XtWindow(w),
		     convertPrivContext, (caddr_t *)&context)) {
	/* error -- $BB?J,%3%s%F%-%9%H$,8+$D$+$i$J$+$C$?$?$a(B */
	return NULL;
    } else {
	return context;
    }
}

/* ARGSUSED */
static void
recvConvAck(w, closure, ev, junk)
Widget	w;
XtPointer	closure;
XEvent	*ev;
Boolean	*junk;	/* NOTUSED */
{
    XClientMessageEvent *cev = &(ev->xclient);
    ConversionAtoms *cap;
    ConversionContext *context;

    if (ev->type != ClientMessage) return;

    cap = getAtoms(w);
    context = getConversionContext(w);

    /* $B@5$7$$%$%Y%s%H$+$I$&$+%A%'%C%/$9$k(B */
    if (cev->window != XtWindow(w) ||
	cev->message_type != cap->notifyAtom ||
	cev->data.l[0] != context->convatom) {
	return;
    }

    /*
     * $B$3$N%O%s%I%i$O$b$&MQ:Q$_$J$N$G30$9(B
     */
    XtRemoveEventHandler(w, NoEventMask, True, recvConvAck, closure);

    if (cev->data.l[2] == None) {
	XtWarning("selection request failed");
	XDeleteContext(XtDisplay(w), XtWindow(w), convertPrivContext);
	callFail(w, context);
	XtFree((char *)context);
	return;
    }

    callStart(w, context, (Window)cev->data.l[3]);

    /*
     * PropertyNotify $B$H(B CONVERSION_END $BMQ$N%$%Y%s%H%O%s%I%i$r(B
     * $BEPO?$9$k(B
     */
    XtAddEventHandler(w, PropertyChangeMask, True, getConv, closure);

    /* $B%W%m%Q%F%#L>$r%9%H%"$9$k(B */
    context->property = cev->data.l[2];
}

/* ARGSUSED */
static void
getConv(w, closure, ev, junk)
Widget	w;
XtPointer	closure;
XEvent	*ev;
Boolean	*junk;	/* NOTUSED */
{
    ConversionAtoms *cap;
    ConversionContext *context;

    /* PropertyNotify $B$H(B ClientMessage $B0J30$OL5;k$9$k(B */
    if (ev->type != PropertyNotify && ev->type != ClientMessage) return;

    cap = getAtoms(w);
    context = getConversionContext(w);

    if (ev->type == ClientMessage) {
	XClientMessageEvent *cev = &(ev->xclient);

	/*
	 * $BK\Ev$KF~NO=*N;$N%$%Y%s%H$+$I$&$+%A%'%C%/$9$k(B
	 */
	if (cev->message_type == cap->endAtom &&
	    cev->format == 32 &&
	    cev->data.l[0] == context->convatom) {
	    /* $B%&%#%s%I%&$N%3%s%F%-%9%H$r:o=|$9$k(B */
	    XDeleteContext(XtDisplay(w), XtWindow(w),
			   convertPrivContext);
	    /* $B%$%Y%s%H%O%s%I%i$r30$9(B */
	    XtRemoveEventHandler(w, PropertyChangeMask, True,
				 getConv, closure);
	    callEnd(w, context);
	    XtFree((char *)context);
	}
    } else {			/* PropertyNotify */
	XPropertyEvent *pev = &(ev->xproperty);
	Atom proptype;
	int propformat;
	unsigned long propsize, rest;
	unsigned char *propvalue;

	if (context->property == None) return;

	/* $B@5$7$$%$%Y%s%H$+$I$&$+$N%A%'%C%/(B */
	if (pev->window != XtWindow(w) ||
	    pev->atom != context->property ||
	    pev->state != PropertyNewValue) {
	    return;
	}

	/* $B$b$7%3!<%k%P%C%/4X?t(B context->inputproc $B$,(B
	 * NULL $B$J$i$P%W%m%Q%F%#$r:o=|$9$k$@$1(B
	 */
	if (context->inputproc == NULL) {
	    XDeleteProperty(XtDisplay(w), XtWindow(w), context->property);
	    return;
	}

	/* $B%W%m%Q%F%#$+$iJQ49J8;zNs$r<h$j=P$9(B */
	XGetWindowProperty(XtDisplay(w), XtWindow(w),
			   context->property,
			   0L, 100000L, True, AnyPropertyType,
			   &proptype, &propformat, &propsize, &rest,
			   &propvalue);

	/* $B%W%m%Q%F%#$N%?%$%W!&%U%)!<%^%C%H$N%A%'%C%/(B */
	if (proptype == None) {
	    /* $B%W%m%Q%F%#$,B8:_$7$J$+$C$?(B
	     * $B$3$l$OO"B3$7$F2?2s$b%W%m%Q%F%#$K%G!<%?$,(B
	     * $BF~$l$i$l$?;~!"0l2s$N(B GetWindowProperty $B$G(B
	     * $BJ#?t$N%G!<%?$r$H$C$F$7$^$C$?$"$H$K5/$-$k(B
	     * $B=>$C$F$3$l$O%(%i!<$G$O$J$$(B
	     */
	    return;
	}

	/* $B%3!<%k%P%C%/$r8F$V(B */
	(*context->inputproc)(w, context->convatom,
			      proptype, propformat,
			      propsize, propvalue,
			      context->closure);
    }
}

static Boolean
setConvAttrProp(w, attrs, nattrs, prop)
Widget w;
ArgList attrs;
Cardinal nattrs;
Atom prop;
{
    unsigned long *data;
    int len;

    if ((len = makeAttrData(w, attrs, nattrs, &data)) > 0) {
	XChangeProperty(XtDisplay(w), XtWindow(w),
			prop, prop, 32,
			PropModeReplace, (unsigned char *)data, len);
	XtFree((char *)data);
	return True;
    }
    return False;
}

static int
makeAttrData(w, args, nargs, datap)
Widget w;
ArgList args;
Cardinal nargs;
unsigned long **datap;
{
    unsigned long *data;
    Cardinal len;
    Boolean spotx_specified = False, spoty_specified = False;
    Boolean fore_specified = False, back_specified = False;
    Pixel savedfg, savedbg;
    Position savedx, savedy;

#define ALLOC(n) \
    data = (unsigned long *)XtRealloc((char *)data, \
				      sizeof(unsigned long)*(len+(n)))

    data = NULL;
    len = 0;
    while (nargs-- > 0) {
	if (!strcmp(args->name, "spotX")) {
	    savedx = (Position)args->value;
	    spotx_specified = True;
	} else if (!strcmp(args->name, "spotY")) {
	    savedy = (Position)args->value;
	    spoty_specified = True;
	} else if (!strcmp(args->name, "foreground")) {
	    savedfg = (Pixel)args->value;
	    fore_specified = True;
	} else if (!strcmp(args->name, "background")) {
	    savedbg = (Pixel)args->value;
	    back_specified = True;
	} else if (!strcmp(args->name, "focusWindow")) {
	    Window win = (Window)args->value;
	    ALLOC(2);
	    data[len] = CONV_ATTR(CONVATTR_FOCUS_WINDOW, 1);
	    data[len + 1] = (unsigned long)win;
	    len += 2;
	} else if (!strcmp(args->name, "inputStyle")) {
	    long style = getInputStyle((String)args->value);
	    ALLOC(2);
	    data[len] = CONV_ATTR(CONVATTR_INPUT_STYLE, 1);
	    data[len + 1] = style;
	    len += 2;
	} else if (!strcmp(args->name, "eventCaptureMethod")) {
	    long method = getCaptureMethod((String)args->value);
	    ALLOC(2);
	    data[len] = CONV_ATTR(CONVATTR_EVENT_CAPTURE_METHOD, 1);
	    data[len + 1] = method;
	    len += 2;
	} else if (!strcmp(args->name, "lineSpacing")) {
	    int spacing = (int)args->value;
	    ALLOC(2);
	    data[len] = CONV_ATTR(CONVATTR_LINE_SPACING, 1);
	    data[len + 1] = spacing;
	    len += 2;
	} else if (!strcmp(args->name, "clientArea")) {
	    XRectangle *rectp = (XRectangle *)args->value;
	    ALLOC(3);
	    data[len] = CONV_ATTR(CONVATTR_CLIENT_AREA, 2);
	    data[len + 1] = (rectp->x << 16) | (rectp->y & 0xffff);
	    data[len + 2] = (rectp->width << 16) | (rectp->height & 0xffff);
	    len += 3;
	} else if (!strcmp(args->name, "statusArea")) {
	    XRectangle *rectp = (XRectangle *)args->value;
	    ALLOC(3);
	    data[len] = CONV_ATTR(CONVATTR_STATUS_AREA, 2);
	    data[len + 1] = (rectp->x << 16) | (rectp->y & 0xffff);
	    data[len + 2] = (rectp->width << 16) | (rectp->height & 0xffff);
	    len += 3;
	} else if (!strcmp(args->name, "cursor")) {
	    Cursor cursor = (Cursor)args->value;
	    ALLOC(2);
	    data[len] = CONV_ATTR(CONVATTR_CURSOR, 1);
	    data[len + 1] = cursor;
	    len += 2;
	} else if (!strcmp(args->name, "fonts")) {
	    XFontStruct **fontp = (XFontStruct **)args->value;
	    int nfonts, nrealfonts;
	    int i;

	    for (nfonts = 0; fontp[nfonts] != NULL; nfonts++)
	        ;
	    ALLOC(nfonts + 1);
	    nrealfonts = 0;
	    for (i = 0; i < nfonts; i++) {
		unsigned long atom;
		if (XGetFontProperty(fontp[i], XA_FONT, &atom)) {
		    data[len + ++nrealfonts] = atom;
		}
	    }
	    data[len] = CONV_ATTR(CONVATTR_FONT_ATOMS, nrealfonts);
	    len += nrealfonts + 1;
	} else {
	    String params[1];
	    Cardinal num_params;

	    params[0] = args->name;
	    XtAppWarningMsg(XtWidgetToApplicationContext(w),
			    "conversionError", "invalidResource",
			    "ConversionLibraryError",
			    "_beginConversionWithAttributes: unknown resource %s",
			    params, &num_params);
	}
	args++;
    }
    if (spotx_specified && spoty_specified) {
	ALLOC(2);
	data[len] = CONV_ATTR(CONVATTR_SPOT_LOCATION, 1);
	data[len + 1] = (savedx << 16) | (savedy & 0xffff);
	len += 2;
    }
    if (fore_specified && back_specified) {
	ALLOC(3);
	data[len] = CONV_ATTR(CONVATTR_COLOR, 2);
	data[len + 1] = savedfg;
	data[len + 2] = savedbg;
	len += 3;
    }
    *datap = data;
    return len;
#undef ALLOC
}

static Boolean
checkProtocols(dpy, window, cap)
Display *dpy;
Window window;
ConversionAtoms *cap;
{
    Atom type;
    int format;
    unsigned long nitems;
    unsigned long bytesafter;
    unsigned long *data, *saveddata;
    int err;
    Boolean ret;

    data = NULL;
    err = XGetWindowProperty(dpy, window, cap->profileAtom,
			     0L, 100L, False,
			     cap->typeAtom,
			     &type, &format, &nitems,
			     &bytesafter, (unsigned char **)&data);
    if (err) return False;
    if (format != 32 || type != cap->typeAtom) {
	if (data != NULL) XtFree((char *)data);
	return False;
    }

    ret = False;
    saveddata = data;
    while (nitems > 0) {
	int code = CODE_OF_ATTR(*data);
	int len = LENGTH_OF_ATTR(*data);

	data++;
	nitems--;
	if (nitems < len) break;

	switch (code) {
	case CONVPROF_PROTOCOL_VERSION:
	    if (*data == cap->versionAtom) ret = True;
	    break;
	case CONVPROF_SUPPORTED_STYLES:
	    break;	/* XXX for now */
	default:
	    break;
	}
	data += len;
	nitems -= len;
    }
    XtFree((char *)saveddata);

    return ret;
}


/*
 *	public functions
 */

int
_beginConversionWithAttributes(w, catom, tatom, inputproc, startendproc, client_data, attrs, nattrs)
Widget w;
Atom catom;		/* Selection Atom e.g. JAPANESE_CONVERSION */
Atom tatom;		/* Property Type Atom e.g. COMPOUND_TEXT */
void (*inputproc)();	/* conversion text callback function */
void (*startendproc)();	/* conversion start/end callback function */
XtPointer client_data;	/* client_data passed to callback function */
ArgList attrs;		/* attribute data */
Cardinal nattrs;	/* number of attr args */
{
    Window owner;
    XEvent event;
    ConversionAtoms *cap;
    ConversionContext *context;
    Boolean anyattr;

    cap = getAtoms(w);

    /* $BJQ49%5!<%P$rC5$9(B */
    if ((owner = XGetSelectionOwner(XtDisplay(w), catom)) == None) {
	/* $B$J$$(B
	 * $B$b$7$bJQ49Cf$@$C$?$iJQ49$rCf;_$9$k(B
	 */
	XtWarning("Conversion Server not found");
	if ((context = getConversionContext(w)) != NULL) {
	    /* $B%$%Y%s%H%O%s%I%i$r30$9(B */
	    XtRemoveEventHandler(w, NoEventMask, True, recvConvAck,
				 (XtPointer)NULL);
	    XtRemoveEventHandler(w, PropertyChangeMask, True, getConv,
				 (XtPointer)NULL);
	    /* $B%&%#%s%I%&$N%3%s%F%-%9%H$r:o=|$9$k(B */
	    XDeleteContext(XtDisplay(w), XtWindow(w), convertPrivContext);
	    callEnd(w, context);
	    XtFree((char *)context);
	}
	return -1;
    }

    /*
     * $B:#$9$G$KJQ49Cf$+$I$&$+D4$Y$k(B
     * $BJQ49Cf$J$i2?$b$;$:$K%j%?!<%s$9$k!D$o$1$K$O$$$+$J$$(B
     * $B$J$<$+$H$$$&$H!"JQ49%5!<%P$,2?$i$+$N;v>p$GESCf$G;`$s$@>l9g(B
     * CONVERSION_END $B$,%/%i%$%"%s%H$KMh$J$$$3$H$,$"$k$+$i$G$"$k(B
     * $B$=$3$G!"JQ49Cf$N>l9g$G$b(B SelectionOwner $B$rC5$7$F!"$=$l$,(B
     * $B:G=i$K(B _beginConversion() $B$,8F$P$l$?;~$H(B WindowID $B$,F1$8$+(B
     * $B$I$&$+3NG'$9$k(B
     * $BK\Ev$O(B SelectionOwner $B$K$J$C$?;~4V$b%A%'%C%/$7$?$$$N$@$,(B
     * ICCCM $B$K=R$Y$i$l$F$$$k$h$&$K!"(BGetSelectionOwner $B$G$O(B
     * $B$=$l$,$o$+$i$J$$$N$G$"$-$i$a$k(B
     */
    if ((context = getConversionContext(w)) != NULL) {
	Window curOwner;
	curOwner = (catom == context->convatom) ? owner :
	  XGetSelectionOwner(XtDisplay(w), context->convatom);
	if (curOwner == context->convowner) {
	    /* $B2?$b$;$:$K%j%?!<%s(B */
	    return 0;
	}
	/* SelectionOwner $B$,JQ$o$C$F$$$k(B
	 * $B$3$l$OESCf$GJQ49%5!<%P$,%/%i%C%7%e$7$?$K0c$$$J$$(B
	 * $B$H$$$&$3$H$G(B CONVERSION_END $B$,Mh$?;~$HF1$8$h$&$J(B
	 * $B=hM}$r$9$k(B
	 */
	/* $B%$%Y%s%H%O%s%I%i$r30$9(B
	 * CONVERSION_NOTIFY $B$N%$%Y%s%H$,Mh$k$^$G$O(B
	 * recvConvAck() $B$,%O%s%I%i$G!"$=$N8e$O(B
	 * getConv() $B$,%O%s%I%i$G$"$k(B
	 */
	XtRemoveEventHandler(w, NoEventMask, True, recvConvAck,
			     (XtPointer)NULL);
	XtRemoveEventHandler(w, PropertyChangeMask, True, getConv,
			     (XtPointer)NULL);
	/* $B%&%#%s%I%&$N%3%s%F%-%9%H$r:o=|$9$k(B */
	XDeleteContext(XtDisplay(w), XtWindow(w), convertPrivContext);
	callEnd(w, context);
	XtFree((char *)context);
    }

    /*
     * $B%5!<%P$+$i$N(B CONVERSION_NOTIFY $BMQ$N%$%Y%s%H%O%s%I%i$r(B
     * $BEPO?$9$k(B
     */
    XtAddEventHandler(w, NoEventMask, True, recvConvAck, (XtPointer)NULL);

    /*
     * $B%3%s%F%-%9%H$r$D$/$C$FI,MW$J>pJs$rEPO?$9$k(B
     */
    context = XtNew(ConversionContext);
    context->convatom = catom;
    context->convowner = owner;
    context->property = None;	/* $B$3$l$O(B CONVERSION_NOTIFY $B$,Mh$?;~$K(B
				 * $B@5$7$/@_Dj$5$l$k(B */
    context->inputproc = inputproc;
    context->startendproc = startendproc;
    context->closure = client_data;
    XSaveContext(XtDisplay(w), XtWindow(w),
		 convertPrivContext, (caddr_t)context);

    /*
     * $BJQ49B0@-%j%9%H$,;XDj$5$l$F$$$l$P%W%m%Q%F%#$K$=$l$rEPO?$9$k(B
     */
    if (nattrs != 0 && attrs != NULL &&
	checkProtocols(XtDisplay(w), owner, cap)) {
	anyattr = setConvAttrProp(w, attrs, nattrs, cap->attrAtom);
    }

    /*
     * ClientMessage $B%$%Y%s%H$r;H$C$FF|K\8lF~NO$r%j%/%(%9%H$9$k(B
     */
    event.xclient.type = ClientMessage;
    event.xclient.window = owner;
    event.xclient.message_type = cap->reqAtom;
    event.xclient.format = 32;
    event.xclient.data.l[0] = catom;
    event.xclient.data.l[1] = XtWindow(w);
    event.xclient.data.l[2] = tatom;
    /* $B7k2L$r%9%H%"$9$k%W%m%Q%F%#L>$O!"B?8@8l$rF1;~$K;HMQ$9$k$3$H$r(B
     * $B9M$($F!"(Bselection atom $B$r;HMQ$9$k$3$H$K$9$k(B
     */
    event.xclient.data.l[3] = catom;
    event.xclient.data.l[4] = anyattr ? cap->attrAtom : None;
    XSendEvent(XtDisplay(w), owner, False, NoEventMask, &event);

    return 0;
}

/* this is provided for backward compatibility */
void
_beginConversion(w, catom, tatom, inputproc, client_data)
Widget	w;
Atom	catom;			/* Selection Atom e.g. JAPANESE_CONVERSION */
Atom	tatom;			/* Property Type Atom e.g. COMPOUND_TEXT */
void	(*inputproc)();		/* conversion text callback function */
XtPointer client_data;		/* client_data passed to callback function */
{
    (void)_beginConversionWithAttributes(w, catom, tatom, inputproc,
				   (void (*)())NULL, client_data,
				   (ArgList)NULL, 0);
}

void
_changeConversionAttributes(w, catom, attrs, nattrs)
Widget	w;
Atom	catom;			/* Selection Atom e.g. JAPANESE_CONVERSION */
ArgList	attrs;			/* attribute data */
Cardinal nattrs;		/* number of attr args */
{
    XEvent event;
    ConversionAtoms *cap;
    ConversionContext *context;
    unsigned long *data;
    int len;

    if (attrs == NULL || nattrs == 0) return;

    cap = getAtoms(w);
    context = getConversionContext(w);

    if (context == NULL || (catom != None && catom != context->convatom)) {
	return;
    }

    if (XGetSelectionOwner(XtDisplay(w), context->convatom) !=
	context->convowner) {
	/* $BJQ49%5!<%P$,0[$J$k!"$"$k$$$O$J$$(B */
	XtRemoveEventHandler(w, NoEventMask, True, recvConvAck,
			     (XtPointer)NULL);
	XtRemoveEventHandler(w, PropertyChangeMask, True, getConv,
			     (XtPointer)NULL);
	/* $B%&%#%s%I%&$N%3%s%F%-%9%H$r:o=|$9$k(B */
	XDeleteContext(XtDisplay(w), XtWindow(w), convertPrivContext);
	callEnd(w, context);
	XtFree((char *)context);
	return;
    }

    data = NULL;
    if ((len = makeAttrData(w, attrs, nattrs, &data)) == 0) return;

    event.xclient.type = ClientMessage;
    event.xclient.window = context->convowner;
    event.xclient.message_type = cap->attrNotifyAtom;
    event.xclient.format = 32;
    event.xclient.data.l[0] = context->convatom;
    event.xclient.data.l[1] = XtWindow(w);
    if (len <= 3 && len == LENGTH_OF_ATTR(data[0]) + 1) {
	int i;
	/* $B%$%Y%s%H$NCf$K<}$^$k(B */
	for (i = 0; i < len; i++) {
	    event.xclient.data.l[2 + i] = data[i];
	}
    } else {
	XChangeProperty(XtDisplay(w), XtWindow(w),
			cap->attrAtom, cap->attrAtom, 32,
			PropModeReplace, (unsigned char *)data, len);
	event.xclient.data.l[2] = CONV_ATTR(CONVATTR_INDIRECT, 1);
	event.xclient.data.l[3] = cap->attrAtom;
    }

    XSendEvent(XtDisplay(w), context->convowner, False, NoEventMask, &event);

    if (data != NULL) XtFree((char *)data);
}

void
_endConversion(w, catom, throwaway)
Widget	w;
Atom	catom;		/* Selection Atom */
Boolean	throwaway;
{
    XEvent event;
    ConversionAtoms *cap;
    ConversionContext *context;

    cap = getAtoms(w);
    context = getConversionContext(w);

    if (context == NULL || (catom != None && catom != context->convatom)) {
	return;
    }

    if (XGetSelectionOwner(XtDisplay(w), context->convatom) !=
	context->convowner) {
	/* $BJQ49%5!<%P$,0[$J$k!"$"$k$$$O$J$$(B */
	XtRemoveEventHandler(w, NoEventMask, True, recvConvAck,
			     (XtPointer)NULL);
	XtRemoveEventHandler(w, PropertyChangeMask, True, getConv,
			     (XtPointer)NULL);
	/* $B%&%#%s%I%&$N%3%s%F%-%9%H$r:o=|$9$k(B */
	XDeleteContext(XtDisplay(w), XtWindow(w), convertPrivContext);
	/* $B%3!<%k%P%C%/$r8F$V(B */
	callEnd(w, context);
	XtFree((char *)context);
	return;
    }

    if (throwaway) context->inputproc = NULL;

    event.xclient.type = ClientMessage;
    event.xclient.window = context->convowner;
    event.xclient.message_type = cap->endReqAtom;
    event.xclient.format = 32;
    event.xclient.data.l[0] = context->convatom;
    event.xclient.data.l[1] = XtWindow(w);

    XSendEvent(XtDisplay(w), context->convowner, False, NoEventMask, &event);
}
