/* $Id: jclib.h,v 5.0 1991/10/01 07:58:04 ishisone Rel $ */

/*
 *	jclib.h -- jclib $BMQ%X%C%@%U%!%$%k(B (Wnn Version4 $BBP1~HG(B)
 *		version 5.0
 *		ishisone@sra.co.jp
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
 */

#ifndef _jclib_h
#define _jclib_h

#include	"jslib.h"

#ifndef WCHAR_DEFINED
#define WCHAR_DEFINED
#undef wchar
typedef unsigned short wchar;
#endif


/* $BDj?t(B */
#define JC_FORWARD	1
#define JC_BACKWARD	0
#define JC_NEXT		0
#define JC_PREV		1
#define JC_HIRAGANA	0
#define JC_KATAKANA	1

/* $B%(%i!<HV9f(B */
#define JE_NOERROR		0
#define JE_WNNERROR		1	/* jlib $B$N%(%i!<(B */
#define JE_NOCORE		2	/* $B%a%b%j$,3NJ]$G$-$J$$(B */
#define JE_NOTCONVERTED		3	/* $BBP>]J8@a$,$^$@JQ49$5$l$F$$$J$$(B */
#define JE_CANTDELETE		4	/* $B%P%C%U%!$N@hF,$NA0!"$"$k$$$O(B
					 * $B:G8e$N<!$NJ8;z$r:o=|$7$h$&$H$7$?(B */
#define JE_NOSUCHCLAUSE		5	/* $B;XDj$5$l$?HV9f$NJ8@a$,B8:_$7$J$$(B */
#define JE_CANTSHRINK		6	/* 1 $BJ8;z$NJ8@a$r=L$a$h$&$H$7$?(B */
#define JE_CANTEXPAND		7	/* $B:G8e$NJ8@a$r?-$P$=$&$H$7$?(B */
#define JE_NOCANDIDATE		8	/* $B<!8uJd$,$J$$(B */
#define JE_NOSUCHCANDIDATE	9	/* $B;XDj$5$l$?HV9f$N8uJd$,B8:_$7$J$$(B */
#define JE_CANTMOVE		10	/* $B%P%C%U%!$N@hF,$NA0!"$"$k$$$O(B
					 * $B:G8e$N<!$K0\F0$7$h$&$H$7$?(B */
#define JE_CLAUSEEMPTY		11	/* $B6u$NJ8$rJQ49$7$h$&$H$7$?(B */
#define JE_ALREADYFIXED		12	/* $B$9$G$K3NDj$5$l$F$$$kJ8$KBP$7$F(B
					 * $BA`:n$r9T$J$C$?(B */

/* $B%(%i!<HV9f(B */
extern int	jcErrno;	/* $B%(%i!<HV9f(B */

/* $B%G!<%?%?%$%W(B */
typedef struct {
	wchar	*kanap;		/* $BFI$_J8;zNs(B */
	wchar	*fzkp;		/* $BIUB08l$NFI$_J8;zNs(B */
	wchar	*dispp;		/* $BI=<(J8;zNs(B */
	int	dicno;
	int	entry;
	int	hinshi;
	int	kangovect;
	char	conv;		/* $BJQ49:Q$_$+(B */
				/* 0: $BL$JQ49(B 1: $BJQ49:Q(B -1: jclib$B$G5?;wJQ49(B */
	char	ltop;		/* $BBgJ8@a$N@hF,$+(B? */
	char	imabit;		/* $B:#;H$C$?$h%S%C%H(B */
} jcClause;

typedef struct {
    /* public member */
	int		nClause;	/* $BJ8@a?t(B */
	int		curClause;	/* $B%+%l%s%HJ8@aHV9f(B */
	int		curLCStart;	/* $B%+%l%s%HBgJ8@a3+;OJ8@aHV9f(B */
	int		curLCEnd;	/* $B%+%l%s%HBgJ8@a=*N;J8@aHV9f(B */
	wchar		*kanaBuf;	/* $B$+$J%P%C%U%!(B */
	wchar		*kanaEnd;
	wchar		*displayBuf;	/* $B%G%#%9%W%l%$%P%C%U%!(B */
	wchar		*displayEnd;
	jcClause	*clauseInfo;	/* $BJ8@a>pJs(B */
	struct wnn_env	*env;
    /* private member */
	int		fixed;		/* $B3NDj$5$l$?$+$I$&$+(B */
	wchar		*dot;		/* $B%I%C%H$N0LCV(B */
	int		candKind;	/* $BBgJ8@a$N8uJd$+>.J8@a$N8uJd$+$rI=$9%U%i%0(B */
	int		candClause;	/* $B<!8uJd$NJ8@aHV9f(B */
	int		candClauseEnd;	/* $BBgJ8@a$N<!8uJd$N;~!"=*N;J8@aHV9f(B */
	int		nCand;		/* $B<!8uJd$N?t(B */
	int		curCand;	/* $B%+%l%s%H$N<!8uJdHV9f(B */
	char		*candBuf;	/* opaque */
	int		nReset;		/* $B:#;H$C$?$h%S%C%H$rMn$9$Y$-(B
					 * $B%(%s%H%j$N?t(B */
	char		*resetBuf;	/* opaque */
	int		bufferSize;	/* kanaBuf/displayBuf $B$NBg$-$5(B */
	int		clauseSize;	/* clauseInfo $B$NBg$-$5(B */
	int		candSize;	/* candBuf $B$NBg$-$5(B */
	int		resetSize;	/* resetBuf $B$NBg$-$5(B */
} jcConvBuf;

#ifdef __STDC__
extern jcConvBuf *jcCreateBuffer(struct wnn_env *env,
				 int nclause, int buffersize);
extern int jcDestroyBuffer(jcConvBuf *buf, int savedic);
extern int jcClear(jcConvBuf *buf);
extern int jcInsertChar(jcConvBuf *buf, int c);
extern int jcDeleteChar(jcConvBuf *buf, int prev);
extern int jcConvert(jcConvBuf *buf, int small, int tan, int jump);
extern int jcUnconvert(jcConvBuf *buf);
extern int jcExpand(jcConvBuf *buf, int small, int convf);
extern int jcShrink(jcConvBuf *buf, int small, int convf);
extern int jcKana(jcConvBuf *buf, int small, int kind);
extern int jcFix(jcConvBuf *buf);
extern int jcNext(jcConvBuf *buf, int small, int prev);
extern int jcCandidateInfo(jcConvBuf *buf, int small,
			   int *ncandp, int *curcandp);
extern int jcGetCandidate(jcConvBuf *buf, int n, wchar *candstr);
extern int jcSelect(jcConvBuf *buf, int n);
extern int jcDotOffset(jcConvBuf *buf);
extern int jcIsConverted(jcConvBuf *buf, int cl);
extern int jcMove(jcConvBuf *buf, int small, int dir);
extern int jcTop(jcConvBuf *buf);
extern int jcBottom(jcConvBuf *buf);
extern int jcChangeClause(jcConvBuf *buf, wchar *str);
extern int jcSaveDic(jcConvBuf *buf);
#else
extern jcConvBuf *jcCreateBuffer();
extern int jcDestroyBuffer();
extern int jcClear();
extern int jcInsertChar();
extern int jcDeleteChar();
extern int jcConvert();
extern int jcUnconvert();
extern int jcExpand();
extern int jcShrink();
extern int jcKana();
extern int jcFix();
extern int jcNext();
extern int jcCandidateInfo();
extern int jcGetCandidate();
extern int jcSelect();
extern int jcDotOffset();
extern int jcIsConverted();
extern int jcMove();
extern int jcTop();
extern int jcBottom();
extern int jcChangeClause();
extern int jcSaveDic();
#endif

#endif /* _jclib_h */
