/* $Id: ConvProto.h,v 1.5 1991/10/24 04:12:41 ishisone Rel $ */
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

#ifndef _ConvProto_h
#define _ConvProto_h

/* $BJQ49%5!<%P$N%W%m%U%!%$%k$,F~$k%W%m%Q%F%#L>(B */
#define CONVERSION_PROFILE	"_CONVERSION_PROFILE"

/* $BJQ49%5!<%P$N%W%m%U%!%$%k$N%W%m%Q%F%#$HJQ49B0@-$N%W%m%Q%F%#$N%?%$%W(B */
#define CONVERSION_ATTRIBUTE_TYPE	"_CONVERSION_ATTRIBUTE_TYPE"

/* $B%W%m%H%3%k%P!<%8%g%sL>(B */
#define PROTOCOL_VERSION	"PROTOCOL-2.0"

#define CONV_ATTR(code,len)	((unsigned long)((code)<<16)+(len))

#define UPPER16U(data)		(((data)>>16)&0xffff)
#define UPPER16S(data)		((short)(((data)>>16)&0xffff))
#define LOWER16U(data)		((data)&0xffff)
#define LOWER16S(data)		((short)((data)&0xffff))

#define CODE_OF_ATTR(head)	UPPER16U(head)
#define LENGTH_OF_ATTR(head)	LOWER16U(head)

/*
 * Conversion Profile Codes
 */

#define CONVPROF_PROTOCOL_VERSION	1
#define CONVPROF_SUPPORTED_STYLES	2
#define CONVPROF_SUPPORTED_EXTENSIONS	3
#define CONVPROF_EXTENSION_DATA		4

/*
 * Standard Conversion Attribute Codes (0-255)
 */

/* 0-127: can be specified at any time (startup and during conversion) */
#define CONVATTR_NONE			0
#define CONVATTR_INDIRECT		1
#define CONVATTR_FOCUS_WINDOW		2
#define CONVATTR_SPOT_LOCATION		3
#define CONVATTR_CLIENT_AREA		4
#define CONVATTR_STATUS_AREA		5
#define CONVATTR_COLORMAP		6
#define CONVATTR_COLOR			7
#define CONVATTR_BACKGROUND_PIXMAP	8
#define CONVATTR_LINE_SPACING		9
#define CONVATTR_FONT_ATOMS		10
#define CONVATTR_CURSOR			11

/* 128-255: can be specified only at startup time */
#define CONVATTR_INPUT_STYLE		128
#define CONVATTR_EVENT_CAPTURE_METHOD	129
#define CONVATTR_USE_EXTENSION		255

/* argument for CONVATTR_INPUT_STYLE and CONVPROP_SUPPORTED_STYLES */
#define CONVARG_ROOTWINDOW		1L
#define CONVARG_OFFTHESPOT		2L
#define CONVARG_OVERTHESPOT		4L

/* argument for CONVATTR_EVENT_CAPTURE_METHOD */
#define CONVARG_NONE			0L
#define CONVARG_CREATE_INPUTONLY	1L
#define CONVARG_SELECT_FOCUS_WINDOW	2L

/*
 * $B%W%m%U%!%$%k%G!<%?(B / $BJQ49B0@-%G!<%?$NI=8=J}K!(B
 *
 * $BJQ49%5!<%P$NFC@-$rI=$9%W%m%U%!%$%k%G!<%?$H!"JQ49$K4X$9$kB0@-$r;XDj$9(B
 * $B$kJQ49B0@-%G!<%?$O6&DL$N%U%)!<%^%C%H$rMQ$$$k!#(B
 *
 * $B8D!9$N%G!<%?$O(B 32bit$BCM$NG[Ns$GI=8=$5$l$k!#:G=i$N(B 1$B%o!<%I$O%X%C%@$G!"(B
 * $B$=$l$K(B 0$B%o!<%I0J>e$N%G!<%?$,B3$/!#%X%C%@$N>e0L(B 16bit $B$O$=$N%W%m%U%!%$(B
 * $B%k(B / $BJQ49B0@-$N%3!<%I$rI=$7!"2<0L(B 16 bit $B$OB3$/%G!<%?$N%o!<%I?t(B 
 * (32bit $BC10L(B) $B$rI=$9!#(B
 *
 *	+----------------+----------------+
 *	|  Code (16bit)  | Length (16bit) |
 *	+----------------+----------------+
 *	|              Data0              |
 *	+---------------------------------+
 *	|              .....              |
 *	+---------------------------------+
 *	|              DataN              |
 *	+---------------------------------+
 *
 * $B<B:]$N%W%m%U%!%$%k%G!<%?$dJQ49B0@-%G!<%?$O$3$N%G!<%?$,$$$/$D$+O"B3$7(B
 * $B$?$b$N$G$"$k!#(B
 */

/*
 * $B%W%m%U%!%$%k%G!<%?(B
 *
 * $B%W%m%U%!%$%k%G!<%?MQ$N%3!<%I$O<!$N(B 4$B<oN`$,Dj5A$5$l$F$$$k!#JQ49B0@-(B
 * $B%G!<%?$H0[$J$j!"%W%i%$%Y!<%HMQ$N%3!<%INN0h$J$I$OMQ0U$5$l$F$$$J$$!#(B
 *
 * Protocol Version
 *	code: 1
 *	data-length: 1
 *	data[0]:
 *		CARD32: protocol version atom ("PROTOCOL-2.0")
 *
 *	$B%G!<%?$OJQ49%5!<%P$N%W%m%H%3%k%P!<%8%g%s$rI=$9%"%H%`$G$"$k!#$3$3(B
 *	$B$GDj5A$5$l$F$$$k%W%m%H%3%k$N%P!<%8%g%s$O(B "PROTOCOL-2.0" $B$G$"$k!#(B
 *
 * Supported Styles
 *	code: 2
 *	data-length: 1
 *	data[0]:
 *		CARD32: input styles
 *
 *	$B%G!<%?$OJQ49%5!<%P$,%5%]!<%H$9$kF~NO%9%?%$%k$rI=$9!#%5%]!<%H$9$k(B
 *	$BF~NO%9%?%$%k$NCM$N(B bitwise-or $B$G$"$k!#(B
 *
 * Supported Extensions
 *	code: 3
 *	data-length: N
 *	data[0]:
 *		CARD32: extension atom 1 (Atom)
 *	...
 *	data[N-1]:
 *		CARD32: extension atom N (Atom)
 *
 *	$B%G!<%?$OJQ49%5!<%P$,%5%]!<%H$9$k3HD%$rI=$9%"%H%`$N%j%9%H$G$"$k!#(B
 *
 * Extension Data
 *	code: 4
 *	data-length: N
 *	data[0]:
 *		CARD32: extension atom (Atom)
 *	data[1] - data[N-1]:
 *		extension specific data
 *
 *	$B%G!<%?$O3HD%FH<+$KDj5A$7$?%W%m%U%!%$%k%G!<%?$G$"$k!#I8=`%W%m%H%3(B
 *	$B%k$H$7$F$O%G!<%?$N@hF,$K3HD%%"%H%`(B ($B$3$l$O(BSupported Extensions 
 *	$B$K;XDj$5$l$?$b$N$G$J$1$l$P$J$i$J$$(B)$B$r$D$1$k$3$H$r5,Dj$9$k$@$1$G!"(B
 *	$B$=$N8e$N%G!<%?$K4X$7$F$O0l@Z5,Dj$7$J$$!#(B
 *
 * $B%/%i%$%"%s%HB&$NL5MQ$N:.Mp$rKI$0$?$a!"(BProtocol Version $B$H(B Supported
 * Styles$B$N9`L\$OI,$:$J$1$l$P$J$i$J$$!#$^$?!"(BExtension Data $B0J30$O%W%m%U%!(B
 * $B%$%k%G!<%?$NCf$KF1$8%3!<%I$N%G!<%?$,J#?t$"$C$F$O$J$i$J$$!#(B
 */

/*
 * $BJQ49B0@-%G!<%?(B
 *
 * $BB0@-%3!<%I$N$&$A!"(B0 $B$+$i(B 255 $B$^$G$OI8=`%W%m%H%3%k$,;HMQ$9$k$b$N$G!"8=(B
 * $B:_B0@-$,3d$j?6$i$l$F$$$J$$$+$i$H$$$C$F>!<j$K;HMQ$7$F$O$J$i$J$$!#$=$N(B
 * $B$h$&$JL\E*$N$?$aB0@-%3!<%I(B 256 $B$+$i(B 65535 $B$,%W%i%$%Y!<%H%3!<%I3HD%NN(B
 * $B0h$H$7$FMQ0U$5$l$F$$$k!#$?$@$7$3$NNN0h$N;HMQ$KEv$?$C$F$O$"$i$+$8$a$=(B
 * $B$N3HD%%3!<%I$r;HMQ$9$k$3$H$r(B Use Extension ($B2<5-;2>H(B) $B$rMQ$$$F$"$i$+(B
 * $B$8$a@k8@$9$kI,MW$,$"$k!#(B
 *
 * $BB0@-%G!<%?$N;XDjJ}K!$K$O!"JQ493+;O;~$K;XDj$9$kJ}K!$H!"JQ49Cf$K;XDj$9(B
 * $B$kJ}K!$N(B 2$BDL$j$,$"$k$,!"B0@-%3!<%I$K$h$C$F$OJQ493+;O;~$K$7$+;XDj$G$-(B
 * $B$J$$$b$N$,$"$k!#$=$3$G!"(B0-255 $B$NI8=`%3!<%I$N$&$A!"(B0 $B$+$i(B 127 $B$^$G$OJQ(B
 * $B493+;O;~$G$bJQ49Cf$G$b;XDj$G$-$k$b$N!"(B128 $B$+$i(B 255 $B$^$G$OJQ493+;O;~$K(B
 * $B$7$+;XDj$G$-$J$$$b$N!"$KJ,$1$F$"$k!#3HD%%3!<%I$K$D$$$F$OFC$K$3$N$h$&(B
 * $B$J6hJL$ODj$a$J$$!#(B
 *
 * $B$3$N%W%m%H%3%k$GDj5A$5$l$kB0@-%3!<%I$O<!$NDL$j$G$"$k!#(B
 *
 * -- $BJQ493+;O;~$K$b!"JQ49ESCf$K$b;XDj$G$-$k$b$N(B --
 *
 * No Operation:
 *	code: 0
 *	data-length: N (could be 0)
 *	data: anything
 *
 *	$B2?$b$7$J$$!#%W%m%Q%F%#$N$"$kItJ,$r%9%-%C%W$5$;$k$N$KJXMx!#(B
 *	
 * Indirect Attribute:
 *	code: 1
 *	data-length: 1
 *	data[0]:
 *		CARD32: property name (Atom)
 *
 *	$B;XDj$5$l$?%W%m%Q%F%#$K=>$C$FB0@-$r@_Dj$9$k!#(BCONVERSION_ATTRIBUTE
 * 	$B%$%Y%s%H$GJ#?t$NB0@-%G!<%?$r@_Dj$7$?$$;~$d!"%$%Y%s%H$KB0@-%G!<%?$,(B
 *	$BF~$j@Z$i$J$$;~$K;HMQ$9$k!#(B
 *
 * Focus Window:
 *	code: 2
 *	data-length: 1
 *	data[0]:
 *		CARD32: focus window (Window)
 *
 *	$B%U%)!<%+%9%&%#%s%I%&$r;XDj$9$k!#(B
 *
 * Spot Location:
 *	data-length: 1
 *	data[0]:
 *		INT16(upper 16bit): X
 *		INT16(lower 16bit): Y
 *
 *	$B%9%]%C%H%m%1!<%7%g%s$r;XDj$9$k!#%Y!<%9%i%$%s$N3+;OE@$G;XDj$9$k!#(B
 *
 * Client Area:
 *	data-length: 2
 *	data[0]:
 *		INT16(upper 16bit): X
 *		INT16(lower 16bit): Y
 *	data[1]:
 *		CARD16(upper 16bit): Width
 *		CARD16(lower 16bit): Height
 *
 *	$BJQ49%F%-%9%HI=<(NN0h$r;XDj$9$k!#(B
 *
 * Status Area:
 *	data-length: 2
 *	data[0]:
 *		INT16(upper 16bit): X
 *		INT16(lower 16bit): Y
 *	data[1]:
 *		CARD16(upper 16bit): Width
 *		CARD16(lower 16bit): Height
 *
 *	$B%9%F!<%?%9I=<(NN0h$r;XDj$9$k!#(B
 *
 * Colormap:
 *	data-length: 1
 *	data[0]:
 *		CARD32: colormap (XID)
 *
 *	$B%+%i!<%^%C%W(B ID $B$r;XDj$9$k!#(B
 *
 * Color:
 *	data-length: 2
 *	data[0]:
 *		CARD32: foreground pixel
 *	data[1]:
 *		CARD32: background pixel
 *
 *	$B%U%)%"%0%i%&%s%I$H%P%C%/%0%i%&%s%I$N%T%/%;%kCM$r;XDj$9$k!#(B
 *
 * Background Pixmap:
 *	data-length: 1
 *	data[0]:
 *		CARD32: background pixmap (Pixmap)
 *
 *	$B%P%C%/%0%i%&%s%I$N(B Pixmap ID $B$r;XDj$9$k!#(B
 *
 * Line Spacing:
 *	data-length: 1
 *	data[0]:
 *		CARD32: line spacing
 *
 *	$B9T4V$r;XDj$9$k!#%Y!<%9%i%$%s4V$N5wN%$G;XDj$9$k!#(B
 *
 * Font Atoms:
 *	data-length: N (>0)
 *	data[0]:
 *		CARD32: font atom 1 (Atom)
 *	...
 *	data[N-1]:
 *		CARD32: font atom N (Atom)
 *
 *	$B;HMQ$9$k%U%)%s%H$N(B "FONT" $B%"%H%`$N%j%9%H$r;XDj$9$k!#(B
 *
 * Cursor:
 *	data-length: 1
 *	data[0]:
 *		CARD32: cursor (Cursor)
 *
 *	$B%+!<%=%k(B ID $B$r;XDj$9$k!#(B
 *
 * -- $BJQ493+;O;~$N$_;XDj$G$-$k$b$N(B --
 *
 * Input Style:
 *	data-length: 1
 *	data[0]:
 *		CARD32: input style
 *
 *	$BF~NOJ}K!$r;XDj$9$k!#(B
 *	$B%G%U%)%k%H$O(B Root Window Style $B$G$"$k!#(B
 *
 * Event Capture Method:
 *	data-length: 1
 *	data[0]:
 *		CARD32: event capture method
 *
 *	$B%/%i%$%"%s%H%&%#%s%I%&$+$i$N%$%Y%s%H$N<hF@J}K!$r;XDj$9$k!#%G%U%)(B
 *	$B%k%H$O%/%i%$%"%s%H%&%#%s%I%&$NA0$K(B InputOnly $B%&%#%s%I%&$r:n$C$F(B
 *	$B$=$N%-!<%$%Y%s%H$r%;%l%/%H$9$k$H$$$&$b$N$G$"$k!#B>$NJ}K!$H$7$F$O!"(B
 *	$B%U%)!<%+%9%&%#%s%I%&(B ($B%U%)!<%+%9%&%#%s%I%&$,;XDj$5$l$F$$$J$1$l$P(B
 *	$B%/%i%$%"%s%H%&%#%s%I%&(B) $B$N%-!<%$%Y%s%H$rD>@\%;%l%/%H$9$k(B ($B$3$N>l(B
 *	$B9g!"JQ49Cf$O%/%i%$%"%s%H$O%-!<%$%Y%s%H$rL5;k$7$J$/$F$O$J$i$J$$(B) 
 *	$B$b$N$H!"2?$b$7$J$$!"$D$^$jJQ49Cf$N%/%i%$%"%s%H$O%-!<%$%Y%s%H$r%U(B
 *	$B%m%s%H%(%s%I$K(B SendEvent $B$7$J$/$F$O$J$i$J$$!"$H$$$&J}K!$,$"$k!#(B
 *
 * Use Extension:
 *	data-length: N
 *	data[0]:
 *		CARD32: extension atom 1 (Atom)
 *	...
 *	data[N-1]:
 *		CARD32: extension atom N (Atom)
 *
 *	$B$3$NB0@-@_Dj$G;HMQ$5$l$k3HD%$r;XDj$9$k!#$3$3$G;XDj$9$k3HD%$O%5!<(B
 *	$B%P$,%5%]!<%H$7$F$$$k$b$N!"$D$^$j%W%m%U%!%$%k%G!<%?Cf$N(BSupported
 *	Extensions $B$K=q$+$l$?3HD%$G$J$1$l$P$J$i$J$$!#(B
 */

#endif
