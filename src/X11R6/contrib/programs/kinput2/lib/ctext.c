/*
 * ctext.c -- Compound Text <-> Japanese Wide Character String converter
 */

/******************************************************************************

$B!&;X<((B (designation)
		1byte			multi-byte
		94char      96char	94char       96char
	-------------------------------------------------------
	G0  :	ESC ( F	 |  -none-	ESC $ ( F  |  -none-
	G1  :	ESC ) F  |  ESC - F	ESC $ ) F  |  ESC $ - F

$B!&=*C<J8;z(B F
	1byte
	    94chars
		B	ASCII
		I	JIS KANA
		J	JIS-ROMAN
	    96chars
		A	8859/1 right half
		B	8859/2 right half
		C	8859/3 right half
		D	8859/4 right half
		F	8859/7 right half
		G	8859/6 right half
		H	8859/8 right half
		M	8859/9 (DIS) right half
	multi-byte
	    94chars ^ 2
		A	GB Hanzi
		B	JIS Kanji 1983
		C	KS Hangul/Hanja

-------------------------------------------------------------------------------
COMPOUND_TEXT $B$N;EMM(B (Comopund Text Encoding Version 1 -- MIT X Consortium Standard)
$B!&(BG0 G1 $B$N$_$r;HMQ$9$k!#(BG2 G3 $B$O;HMQ$7$J$$!#(B
$B!&(BG0 $B$,(B GL$B!"(BG1 $B$,(B GR $B$K8F$S=P$5$l$F$*$j!"$=$l$rJQ99$9$k$3$H$O$G$-$J$$!#(B
  $B$D$^$j!"(BLocking Shift $B$*$h$S(B Single Shift $B$O;HMQ$7$J$$!#(B
$B!&=i4|@_Dj$H$7$F(B ISO Latin-1 $B$,(B G0/G1 $B$K;X<($5$l$F$$$k!#(B
$B!&%^%k%A%P%$%H$NJ8;z$r(B G0 $B$K;X<($9$k$N$K!"(BESC-$-F $B$O;HMQ$7$J$$!#(B
  ESC-$-(-F $B$r;HMQ$9$k!#(B
$B!&;HMQ$G$-$k=*C<J8;z$O!">e$K=q$+$l$?DL$j!#(B
$B!&(BC0 $B$G;HMQ$G$-$kJ8;z$O!"(BNL TAB ESC $B$N$_$H$9$k!#(B
$B!&(BC1 $B$G;HMQ$G$-$kJ8;z$O(B CSI $B$N$_$H$9$k!#(B
$B!&%F%-%9%H$NIA2hJ}8~$N%7!<%1%s%9$,4^$^$l$k!#(B
	$B:8$+$i1&(B
	$B1&$+$i:8(B
	$B85$NJ}8~$KLa$k(B
******************************************************************************/

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

#ifndef lint
static char *rcsid = "$Id: ctext.c,v 2.3 1993/02/02 02:38:41 ishisone Rel $";
#endif

typedef unsigned short	wchar;

#define NULL	0

#define CS96	0x100	/* 96chars CS */
#define MBCS	0x200	/* Multibyte CS */

/* convJWStoCT -- Japanese Wide Character String -> COMPOUND_TEXT */
int
convJWStoCT(wstr, xstr, jisroman)
register wchar *wstr;
register unsigned char *xstr;
int jisroman;	/* true $B$J$i$P(B G0 $B$N%-%c%i%/%?%;%C%H$H$7$F(B JIS ROMAN $B$r!"(B
		 * false $B$J$i$P(B ASCII $B$r;HMQ$9$k(B
		 */
/* Wide Character string wstr $B$r(B COMPOUND_TEXT xstr $B$KJQ49$7!"(B
 * $BJQ498e$N%P%$%H?t$rJV$9(B($B:G8e$N(B null byte $B$O4^$^$J$$(B)$B!#$b$7(B xstr $B$,(B
 * NULL $B$J$i$PJQ49$O$;$:!"J8;z?t$N$_$rJV$9!#(B
 */
{
	register int	c;
	register int	g0, g1;
	register int	n = 0;
	int		g0cs;
	
	g0cs = jisroman ? 'J' : 'B';

	g0 = 'B';
	g1 = CS96|'A';
	
	/*
	 * G0, G1 $B$O<!$N$h$&$K;H$$J,$1$k(B
	 *  G0: ASCII / JIS-ROMAN
	 *  G1: $B4A;z(B / $B$+$J(B
	 */
	
	while (c = *wstr++) {
		switch (c & 0x8080) {
		case 0:		/* ASCII or C0 or DEL */
			if (c < ' ' || c == 0x7f) {
				/* C0 or DEL */
				if (c == '\t' || c == '\n') {
					if (xstr) *xstr++ = c;
					n++;
				}
				break;
			}
			if (g0 != g0cs) {
				if (xstr) {
					*xstr++ = '\033';
					*xstr++ = '(';
					*xstr++ = g0cs;
				}
				n += 3;
				g0 = g0cs;
			}
			if (xstr) *xstr++ = c & 0x7f;
			n++;
			break;
		case 0x80:	/* $B$+$J(B or C1 */
			if (0x80 <= c && c <= 0x9f) break;
			if (g1 != 'I') {
				if (xstr) {
					*xstr++ = '\033';
					*xstr++ = ')';
					*xstr++ = 'I';
				}
				n += 3;
				g1 = 'I';
			}
			if (xstr) *xstr++ = c & 0xff;
			n++;
			break;
		case 0x8080:	/* $B4A;z(B */
			if (g1 != (MBCS|'B')) {
				if (xstr) {
					*xstr++ = '\033';
					*xstr++ = '$';
					*xstr++ = ')';
					*xstr++ = 'B';
				}
				n += 4;
				g1 = MBCS|'B';
			}
			if (xstr) {
				*xstr++ = (c >> 8) & 0xff;
				*xstr++ = c & 0xff;
			}
			n += 2;
			break;
		default:
			/* $BL5;k$9$k(B */
			break;
		}
	}
	if (xstr) *xstr = '\0';
	return n;
}

static unsigned char *
getesc(str, len)
unsigned char *str;
int len;
{
	register int	c;

	/* $B%(%9%1!<%W%7!<%1%s%9$N!"%(%9%1!<%W$KB3$/(B
	 * $BCf4VJ8;z$H=*C<J8;z$rD4$Y$k(B
	 */
	/* $BCf4VJ8;z$O(B 02/00 $B$+$i(B 02/15 $B$^$G(B */
	while (len > 0) {
		c = *str;
		if (c < 0x20 || 0x2f < c)
			break;
		len--, str++;
	}
	/* $B=*C<J8;z$O(B 03/00 $B$+$i(B 07/14 $B$^$G(B */
	if (--len < 0 || (c = *str++) < 0x30 || 0x7e < c)
		return (unsigned char *)NULL;

	return str;
}

static unsigned char *
getcsi(str, len)
unsigned char *str;
int len;
{
	register int	c;

	/* CSI $B%7!<%1%s%9$N!"(BCSI $B$KB3$/(B
	 * $B%Q%i%a%?J8;z!&Cf4VJ8;z$H=*C<J8;z$rD4$Y$k(B
	 */
	/* $B%Q%i%a%?$O(B 03/00 $B$+$i(B 03/15 $B$^$G(B */
	while (len > 0) {
		c = *str;
		if (c < 0x30 || 0x3f < c)
			break;
		len--, str++;
	}
	/* $BCf4VJ8;z$O(B 02/00 $B$+$i(B 02/15 $B$^$G(B */
	while (len > 0) {
		c = *str;
		if (c < 0x20 || 0x2f < c)
			break;
		len--, str++;
	}
	/* $B=*C<J8;z$O(B 04/00 $B$+$i(B 07/14 $B$^$G(B */
	if (--len < 0 || (c = *str++) < 0x40 || 0x7e < c)
		return (unsigned char *)NULL;

	return str;
}

/* convCTtoJWS -- COMPOUND_TEXT -> Japanese Wide Character String */
int
convCTtoJWS(xstr, len, wstr)
register unsigned char *xstr;
int len;
wchar *wstr;
/* COMPOUND_TEXT xstr $B$r(B Wide Character string wstr $B$KJQ49$7!"(B
 * $BJQ498e$NJ8;z?t$rJV$9(B($B:G8e$N(B null $BJ8;z$O4^$^$J$$(B)$B!#$b$7(B wstr $B$,(B
 * NULL $B$J$i$PJQ49$O$;$:!"J8;z?t$N$_$rJV$9!#(B
 */
{
	register int	c;
	int	nskip;
	int	n = 0;
	int	g0, g1, gs;
	unsigned char	*xstr1;

	/*
	 * Compound Text $BCf$K$O(B null octet $B$,4^$^$l$k2DG=@-$,$"$k(B
	 * $B$=$3$GJ8;zNs$ND9$5(B len $B$r0z?t$G;XDj$G$-$k$h$&$K$7$F$"$k$N$@$,!"(B
	 * 0 $B$"$k$$$OIi$N;~$K$O(B (null octet $B$O$J$$$b$N$H$7$F(B) strlen() $B$G(B
	 * $BD9$5$rD4$Y$k(B
	 */
	if (len <= 0) {
		len = strlen((char *)xstr);
	}

	/* $B=i4|>uBV$O!"(BISO 8859/1 $B$,(B G0/G1 $B$KF~$C$F$$$k(B */
	g0 = 'B';	/* ASCII -> G0 */
	g1 = CS96|'A';	/* Latin/1 right hand part -> G1 */

	while (len-- > 0) {
		switch (c = *xstr++) {
		case '\n':	/* NEWLINE */
		case '\t':	/* TAB */
			if (wstr) *wstr++ = c;
			n++;
			break;
		case 0x9b:	/* CSI */
			/*
			 * CSI $B$N0lHL7A$O(B
			 *	CSI {P} {I} F
			 * $B%Q%i%a%?(B P $B$O(B 03/00 $B$+$i(B 03/15$B!"(B
			 * $BCf4VJ8;z(B I $B$O(B 02/00 $B$+$i(B 02/15$B!"(B
			 * $B=*C<J8;z(B F $B$O(B 04/00 $B$+$i(B 07/14 $B$NHO0O(B
			 */
			/*
			 * $B8=:_Dj5A$5$l$F$$$k$N$O(B directionality $B$@$1$G!"(B
			 * $B$=$l$O(B
			 *	CSI-1-]		begin left-to-right text
			 *	CSI-2-]		begin right-to-left text
			 *	CSI-]		end of string
			 * $B$G$"$k(B
			 * $B$,$H$j$"$($::#$O$3$l$rL5;k$9$k$N$G!"(BCSI $B$N(B
			 * $B%7!<%1%s%9$O$9$Y$FL5;k!"$H$$$&$3$H$K$J$k(B
			 */
			xstr1 = getcsi(xstr, len);
			if (xstr1 == NULL)
				return -1;
			len -= xstr1 - xstr;
			xstr = xstr1;
			break;
		case '\033':	/* ESC */
			/*
			 * $B%(%9%1!<%W%7!<%1%s%9$N0lHL7A$O(B
			 *	ESC {I} F
			 * $BCf4VJ8;z(B I $B$O(B 02/00 $B$+$i(B 02/15 $B$G!"(B
			 * $B=*C<J8;z(B F $B$O(B 03/00 $B$+$i(B 07/14 $B$NHO0O(B
			 */
			/*
			 * $B8=:_Dj5A$5$l$F$$$k$N$O!"(B
			 *   $B%9%?%s%@!<%I%-%c%i%/%?%;%C%H(B
			 *	ESC-(-F
			 *	ESC-$-(-F
			 *	ESC-)-F
			 *	ESC---F
			 *	ESC-$-)-F
			 *   $B%N%s%9%?%s%@!<%I%-%c%i%/%?%;%C%H(B
			 *	ESC-%-/-[0123]
			 * $B%9%?%s%@!<%I$J%-%c%i%/%?%;%C%H$O@5$7$/2r<a(B
			 * $B$7$J$/$F$O$J$i$J$$$7!"%N%s%9%?%s%@!<%I$J$b$N$O(B
			 * $BL5;k$9$k$1$l$I$b%G!<%?$r%9%-%C%W$9$kI,MW$,$"$k(B
			 */
			xstr1 = getesc(xstr, len);
			if (xstr1 == NULL)
				return -1;
			len -= xstr1 - xstr;
			switch (xstr1 - xstr) {
			case 2:		/* ESC - I - F */
				switch (*xstr++) {
				case '(':	/* 94chars CS -> G0 */
					g0 = *xstr;
					break;
				case ')':	/* 94chars CS -> G1 */
					g1 = *xstr;
					break;
				case '-':	/* 96chars CS -> G1 */
					g1 = *xstr | CS96;
					break;
				default:	/* ignore */
					break;
				}
				break;
			case 3:		/* ESC - I - I - F */
				switch (*xstr++) {
				case '$':
					switch (*xstr++) {
					case '(':	/* 94chars MBCS -> G0 */
						g0 = *xstr | MBCS;
						break;
					case ')':	/* 94chars MBCS -> G1 */
						g1 = *xstr | MBCS;
						break;
					case '-':	/* 96chars MBCS -> G1 */
						g1 = *xstr | CS96 | MBCS;
						break;
					default:	/* ignore */
						break;
					}
					break;
				case '%':
					if (*xstr++ != '/') {
						/* unknown sequence */
						break;
					}
					/*
					 * $B%W%i%$%Y!<%H%(%s%3!<%G%#%s%0(B
					 * $B40A4$KL5;k$9$k(B
					 * $B$?$@$7$=$N$"$H$KB3$/%G!<%?$r(B
					 * $B%9%-%C%W$9$kI,MW$,$"$k(B
					 *	ESC-%-/-F-M-L
					 */
					len -= 2;
					if (len < 0)
						return -1;
					nskip = (*xstr1 & 0x7f) * 128 +
					    (*(xstr1 + 1) & 0x7f);
					if ((len -= nskip) < 0)
						return -1;
					xstr1 += nskip + 2;
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
			xstr = xstr1;
			break;
		default:
			if (!(c & 0x60)) {
				/*
				 * NL/TAB/ESC/CSI $B0J30$N(B C0 or C1
				 * $B$3$l$OL@$i$+$K%(%i!<(B
				 */
				return -1;
			}
			gs = (c & 0x80) ? g1 : g0;
			c &= 0x7f;
			if (gs & MBCS) {
				switch (gs & 0x70) {
				case 0x70:	/* 4byte/char */
					if (--len < 0) return -1;
					c = (c << 8) | (*xstr++ & 0x7f);
				case 0x60:	/* 3byte/char */
					if (--len < 0) return -1;
					c = (c << 8) | (*xstr++ & 0x7f);
				case 0x50:	/* 2byte/char */
				case 0x40:	/* 2byte/char */
					if (--len < 0) return -1;
					c = (c << 8) | (*xstr++ & 0x7f);
					break;
				default:
					return -1;
				}
			}
			if (wstr) {
				switch (gs) {
				case 'B':
				case 'J':
					*wstr++ = c;
					n++;
					break;
				case 'I':
					*wstr++ = 0x80 | c;
					n++;
					break;
				case MBCS|'B':
					*wstr++ = 0x8080 | c;
					n++;
					break;
				}
			} else {
				switch (gs) {
				case 'B':
				case 'J':
				case 'I':
					n++;
					break;
				case MBCS|'B':
					n++;
					break;
				}
			}
			break;
		}
	}
	if (wstr) *wstr = 0;
	return n;
}
