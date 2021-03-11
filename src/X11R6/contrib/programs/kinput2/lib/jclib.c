/*
 *	jclib -- $B$+$J4A;zJQ49MQ%i%$%V%i%j(B (Wnn Version4.0 $BBP1~HG(B)
 *		version 5.2
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
 *		ishisone@sra.co.jp
 */

/*
 * Portability issue:
 *
 *	+ define SYSV, SVR4 or USG if you don't have bcopy() or bzero().
 *
 *	  if you define USG (which should be defined if your OS is based
 *	  on System V Rel 2) or SYSV (in case of System V Rel 3),
 *	  memchr() is used for bzero(), and my own version of bcopy()
 *	  is used in order to handle overlapping regions.
 *
 *	  if you define SVR4 (yes, System V Rel4), memmove() is used for
 *	  bcopy(), and memchr() is used for bzero().
 *
 *	+ jclib assumes bcopy() can handle overlapping data blocks.
 *	  If your bcopy() can't, you should define OVERLAP_BCOPY,
 *	  which force to use my own bcopy() rather than the one
 *	  in libc.a.
 */

/*
 * $B35MW(B
 *
 * jclib $B$O(B Wnn $B$NF|K\8lF~NO%i%$%V%i%j(B jslib $B$N>e$K:n$i$l$?(B
 * ($BHf3SE*(B)$B9b%l%Y%k$N%i%$%V%i%j$G$"$k!#$+$J4A;zJQ49ItJ,$@$1$,(B
 * $BMQ0U$5$l$F$$$k$N$G!"$=$NB><-=q$K4X$9$kA`:n$J$I$O(B
 * $BD>@\(B jslib $B$r8F$S=P$9$3$H$K$J$k!#(B
 *
 * jclib $B$O$+$J%P%C%U%!$H%G%#%9%W%l%$%P%C%U%!$H$$$&#2$D$N%P%C%U%!$r;}$D!#(B
 * $B$+$J%P%C%U%!$K$OFI$_J8;zNs$,F~$j!"%G%#%9%W%l%$%P%C%U%!$K$OJQ497k2L(B
 * ($BI=<(J8;zNs(B)$B$,F~$k!#$+$J%P%C%U%!$H8@$&8F$SJ}$O$"$^$j@53N$G$O$J$$!#(B
 * Wnn Version 4 $B$G$O4A;z$+$JJQ49$b$G$-$k$+$i$G$"$k!#(B
 *
 * $B%I%C%H$H%+%l%s%HJ8@a$H$$$&35G0$r;}$A!"J8;z$NA^F~(B / $B:o=|$O%I%C%H$N0LCV$K(B
 * $BBP$7$F9T$J$o$l!"JQ49$=$NB>$NA`:n$O%+%l%s%HJ8@a$KBP$7$F9T$J$o$l$k!#(B
 * Wnn Version 4 $B$G$OBgJ8@a$H>.J8@a$H$$$&#2<oN`$NJ8@a$N35G0$,$"$k$,!"(B
 * $B$=$l$KBP1~$7$F(B jclib $B$b$3$N#2<oN`$r07$&$3$H$,$G$-$k!#(B
 *
 * $B$3$N%i%$%V%i%j$O<!$N$h$&$J5!G=$rDs6!$9$k!#(B
 *	$B!&$+$J%P%C%U%!$X$NJ8;z$NA^F~(B / $B:o=|(B
 *	$B!&$+$J4A;zJQ49(B / $B:FJQ49(B / $BL5JQ49(B
 *	$B!&$R$i$,$J"N%+%?%+%JJQ49(B
 *	$B!&3NDj(B
 *	$B!&J8@a$N3HBg(B / $B=L>.(B
 *	$B!&%+%l%s%HJ8@a(B / $B%I%C%H$N0\F0(B
 *	$B!&<!8uJd(B/$BA08uJdCV$-49$((B
 *	$B!&8uJd<h$j=P$7(B / $BA*Br(B
 *	$B!&%P%C%U%!$N%/%j%"(B
 *
 * $BJ8;z%3!<%I$H$7$F$O(B Wnn $B$HF1$8$/(B EUC $BFbIt%3!<%I(B (process code) $B$r;HMQ$9$k!#(B
 */

/*
 * Wnn Version 4 $BBP1~$K$"$?$C$F(B
 *
 * jclib $B$O$b$H$b$H(B Wnn Version 3 $B$N(B libjd $B$N>e$K:n$i$l$?%i%$%V%i%j$G$"$k!#(B
 * $B$3$l$r$r(B ver4 $BBP1~$K$9$k$K$"$?$C$F!"$$$/$D$+$NJ}?K$rN)$F$?!#(B
 *
 * 1. $B$+$J%P%C%U%!$H%G%#%9%W%l%$%P%C%U%!$NFs$D$NJ8;z%P%C%U%!$r;}$A!"(B
 * $B$+$J%P%C%U%!$K$OFI$_!"%G%#%9%W%l%$%P%C%U%!$K$OJQ497k2L$,F~$k$H$+(B
 * $BMM!9$JA`:n$O%+%l%s%HJ8@a$H8F$P$l$kJ8@a$KBP$7$F9T$J$o$l$k$H$+$$$C$?(B
 * $B4pK\E*$J%3%s%;%W%H$OJQ$($J$$!#(B
 *
 * 2. $B@N$N%i%$%V%i%j$r;H$C$?%"%W%j%1!<%7%g%s$,?7$7$$%i%$%V%i%j$K(B
 * $B0\9T$7$d$9$$$h$&$K!"%U%!%s%/%7%g%s%$%s%?!<%U%'%$%9$b$G$-$k$@$1(B
 * $B;w$?$b$N$K$9$k!#(B
 *
 * 3. 1,2 $B$NJ}?K$r$G$-$k$@$1<i$j$D$D!"(Bversion4.0 $B$GF3F~$5$l$?<!$N$h$&$J(B
 * $B5!G=$r%5%]!<%H$9$k!#(B
 *	$B!&4D6-(B
 *	$B!&%^%k%A%P%C%U%!(B ($BJQ49%P%C%U%!$,J#?t;}$F$k$h$&$K$9$k(B)
 *	$B!&BgJ8@a(B / $B>.J8@a(B ($BBgJ8@a$r4pK\$H$9$k$,!">.J8@a$b07$($k$h$&$K$9$k(B)
 *
 * 4. 1 $B$+$i(B 3 $B$^$G$NJ}?K$K=>$$$D$D!"$G$-$k$@$15^$$$G:n$k(B :-)
 */

/*
 * $B%a%b(B
 *
 * ver 0.0	89/07/21
 *	$B$H$j$"$($::n$j$O$8$a$k(B
 * ver 0.1	89/08/02
 *	$BH>J,$/$i$$$+$1$?(B
 *	$B<!8uJd4XO"$,$^$@$G$-$F$$$J$$(B
 * ver 0.2	89/08/04
 *	jcInsertChar() / jcDeleteChar() $B$r:n@.(B
 * ver 0.3	89/08/07
 *	$B0l1~$G$-$?(B
 *	$B$^$@$$$/$D$+5?LdE@$,$"$k$1$l$I(B
 * ver 0.4	89/08/08
 *	$B:#;H$C$?$h%S%C%H$N07$$$r;D$7$F!"$[$\$G$-$?$N$G$O$J$$$+$H(B
 *	$B;W$o$l$k(B
 *	$B:Y$+$$%P%0$r$+$J$j=$@5(B
 * ver 0.5	89/08/09
 *	$BN)LZ$5$s(B@KABA $B$K<ALd$7$?=j!":#;H$C$?$h%S%C%H$rMn$9$N$b(B
 *	$B%/%i%$%"%s%HB&$N@UG$$G$"$k$3$H$,$o$+$k(B
 *	$B$3$l$X$NBP1~(B
 *	$B$D$$$G$K%G!<%?9=B$$N@bL@$rDI2C(B
 *	$B%U%!%$%k$N%5%$%:$,(B 80KB $B$r1[$($F$7$^$C$?(B
 *	$B%3%a%s%H$r$H$l$P$+$J$j>.$5$/$J$k$s$@$1$I(B
 * ver 0.6	89/08/22
 *	jcDeleteChar() $B$rA4LLE*$K=q$-D>$9(B
 *	$B$3$l$G0l1~@5$7$/F0:n$9$k$h$&$K$J$C$?(B
 *	jcInsertChar() $B$G:G8e$N(B clauseInfo $B$N@_Dj$,4V0c$C$F$$$?$N$G(B
 *	$B$=$l$r=$@5(B
 *	jcPrintDetail() $B$K4JC1$J(B clauseInfo $B%G!<%?$N(B consistency check $B$r(B
 *	$BF~$l$k(B
 * ver 0.7	89/08/26
 *	jcExpand() $B$N%P%0=$@5(B
 *	$B>.J8@a$NC1J8@aJQ49$r>/$7=$@5(B
 * ver 0.8	89/08/30
 *	changecinfo() $B$G(B conv $B%U%i%0$r%;%C%H$9$k$N$rK:$l$F$$$?(B
 *	moveKBuf()/moveDBuf()/moveCInfo() $B$r>/$7=$@5(B
 *	SYSV $B$,(B define $B$5$l$F$$$l$P(B bcopy()/bzero() $B$NBe$o$j$K(B
 *	memcpy()/memset() $B$r;H$&$h$&$K=$@5(B
 * ver 0.9	89/09/22
 *	setLCandData() $B$G<!8uJd%P%C%U%!$N8uJd?t$K%+%l%s%HBgJ8@a$N(B
 *	$BJ,$r2C$($k$N$rK:$l$F$$$?(B
 * ver 0.10	89/10/16
 *	wnn-4.0.1 $B$G(B commonheader.h -> commonhd.h $B$K$J$C$?$N$G(B
 *	$B$=$l$N=$@5(B
 * ver 0.11	89/10/18
 *	USG $B$,(B define $B$5$l$F$$$F$b(B memcpy()/memset() $B$r;H$&$h$&$K=$@5(B
 * ver 0.12	89/10/19
 *	resizeBuffer() $B$G%I%C%H$N:F@_Dj$rK:$l$F$$$k$H$$$&=EBg$J%P%0$r=$@5(B
 * ver 4.0	89/10/27
 *	$B%P!<%8%g%sHV9f$r=$@5$7$F(B 4.0 $B$K$9$k!#(B
 * --- kinput $B$r(B R4 $B$K(B contribute ---
 * ver 4.1	90/06/04
 *	$B%/%i%$%"%s%HB&$K$"$k<-=q!&IQEY%U%!%$%k$N%;!<%V$,$G$-$J$$$H$$$&(B
 *	$B=EBg$J%P%0$r=$@5(B
 * ver 4.2	90/06/15
 *	$B<-=q$,EPO?2DG=$+$I$&$+$NH=Dj$,4V0c$C$F$$$F!"5UJQ492DG=<-=q$N(B
 *	$B%;!<%V$,$G$-$J$$$H$$$&$^$?$^$?=EBg$J%P%0$r=$@5(B
 *	$B:#$N$H$3$m(B kinput/wterm $B$H$bC18lEPO?5!G=$,$D$$$F$J$$$N$G(B
 *	$B<B32$O$J$+$C$?$,(B
 * ver 4.3	91/08/15
 *	$BJ8;z%G!<%?7?$H$7$F(B wchar_t $B$G$O$J$/!"(Bwchar $B$r;H$&$h$&$K$9$k(B
 *	$B:G=*E*$K$O(B Wnn $B$N<!4|%P!<%8%g%s$N7?$K9g$o$;$k$D$b$j(B
 * ver 4.4	91/09/18
 *	SYSV $B$^$?$O(B USG $B$,Dj5A$5$l$F$$$k>l9g$K$O<+F0E*$K(B OVERLAP_BCOPY
 *	$B$bDj5A$9$k$h$&$K$7$?(B
 *	SVR4 $B$,Dj5A$5$l$F$$$k>l9g$K$O(B bcopy $B$NBe$o$j$K(B memmove() $B$r;HMQ(B
 *	$B$9$k$h$&$K$7$?(B
 * ver 4.5	91/09/23
 *	DEBUG $B$r(B DEBUG_JCLIB $B$KJQ99(B
 * ver 5.0	91/10/01
 *	kinput2 $B%j%j!<%98~$1$K%P!<%8%g%sHV9f$r=$@5$7$F(B 5.0 $B$K$9$k!#(B
 * --- kinput2 $B$r(B R5 $B$K(B contribute ---
 * ver 5.1	92/02/07
 *	John Yates $B$5$s(B (yates@bldrsoft.com) $B$+$i(B getLCandDataLen() $B$G(B
 *	$BJ8;z?t$r?t$(4V0c$($F$$$?$N$r;XE&$5$l$?$N$G$=$l$N=$@5(B
 * ver 5.2	92/12/24
 *	jcInsertChar() $B$G%G!<%?$N=i4|2=$r$7$F$$$J$+$C$?ItJ,$,$"$C$?(B
 *	$B$N$G=$@5(B ($BCM$,BeF~$5$l$k$^$G;HMQ$5$l$J$$$N$G%P%0$G$O$J$$$N$@$,(B
 *	$B$A$g$C$H5$;}$A$o$k$$$N$G(B)
 */

/*
 * $B%U%!%s%/%7%g%s(B
 *
 * jcConvBuf *jcCreateBuffer(struct wnn_env *env, int nclause, int buffersize)
 *	$B;XDj$5$l$?4D6-$r;H$C$FJQ49$N%P%C%U%!$r:n@.$9$k!#%P%C%U%!$O(B
 *	$BJ#?t:n$k$3$H$,$G$-$k!#0l$D$N%P%C%U%!$G$OF1;~$KJ#?t$NJ8$r(B
 *	$BJQ49$9$k$3$H$O$G$-$J$$$N$G!"J#?t$NJ8$rJB9T$7$FJQ49$7$?$$>l9g$K$O(B
 *	$B4v$D$+$N%P%C%U%!$rMQ0U$7$J$/$F$O$J$i$J$$!#(B
 *	$B4D6-$N@_Dj$^$G$rM=$a$d$C$F$*$/I,MW$,$"$k!#$D$^$j%5!<%P$H$N@\B3!"(B
 *	$B4D6-$N@8@.!"<-=q$N@_Dj$J$I$O(B jclib $B$N<iHwHO0O$G$O$J$$!#(B
 *	$B0z?t$N(B nclause $B$H(B buffersize $B$G!"$=$l$>$l=i4|2=;~$K%"%m%1!<%H$9$k(B
 *	$BJ8@a>pJs$*$h$S$+$J!&%G%#%9%W%l%$%P%C%U%!$NBg$-$5$,;XDj$G$-$k!#(B
 *	$B$?$@$7$3$l$i$O!"%5%$%:$,B-$j$J$/$J$l$PI,MW$K1~$8$F<+F0E*$K(B
 *	$BA}$d$5$l$k$?$a!"$3$3$K;XDj$7$?0J>e$N?t$NJ8@a$d!"J8;zNs$,JQ49$G$-$J$$(B
 *	$B$o$1$G$O$J$$!#$=$l$>$l(B 0 $B$^$?$OIi$NCM$r;XDj$9$k$H!"%G%U%)%k%H$N(B
 *	$B%5%$%:$G%"%m%1!<%H$5$l$k!#=>$C$FDL>o$O(B nclause/buffersize $B$H$b(B
 *	0 $B$r;XDj$7$F$*$1$P$h$$!#(B
 *	$B%j%?!<%s%P%j%e!<$H$7$F%P%C%U%!$rJV$9!#%(%i!<$N;~$K$O(B NULL $B$,(B
 *	$BJV$5$l$k!#(B
 *
 * int jcDestroyBuffer(jcConvBuf *buf, int savedic)
 *	$B%P%C%U%!$N;HMQ$r=*N;$9$k!#4D6-$r>C$7$?$j!"%5!<%P$H$N@\B3$r@Z$C$?$j(B
 *	$B$9$k$3$H$O(B jclib $B$N<iHwHO0O30$G$"$k!#(B
 *	$B0z?t(B savedic $B$,(B 0 $B$G$J$1$l$P!"4D6-Cf$G;HMQ$5$l$F$$$kA4$F$N<-=q$r(B
 *	$B%;!<%V$9$k!#(B
 *
 * int jcClear(jcConvBuf *buf)
 *	$B%P%C%U%!$r%/%j%"$9$k!#?7$?$KJQ49$r;O$a$k:]$K$O:G=i$K$3$N(B
 *	$B%U%!%s%/%7%g%s$r8F$P$J$1$l$P$J$i$J$$!#(B
 *
 * int jcInsertChar(jcConvBuf *buf, int c)
 *	$B%I%C%H$K#1J8;zA^F~$9$k!#(B
 *	$B%+%l%s%HJ8@a$,4{$KJQ49$5$l$F$$$l$PL5JQ49$N>uBV$KLa$k!#(B
 *	$B%+%l%s%HJ8@a$OBgJ8@a$G$"$k!#(B
 *
 * int jcDeleteChar(jcConvBuf *buf, int prev)
 *	$B%I%C%H$NA0Kt$O8e$m$N#1J8;z$r:o=|$9$k!#(B
 *	$B%+%l%s%HJ8@a$,4{$KJQ49$5$l$F$$$l$PL5JQ49$N>uBV$KLa$k!#(B
 *	$B%+%l%s%HJ8@a$OBgJ8@a$G$"$k!#(B
 *
 * int jcConvert(jcConvBuf *buf, int small, int tan, int jump)
 *	$B%+%l%s%HJ8@a$+$i8e$m$rJQ49$9$k!#(B
 *	$B0z?t(B tan $B$,(B 0 $B$J$iO"J8@aJQ49!"$=$&$G$J$1$l$P%+%l%s%HJ8@a$r(B
 *	$BC1J8@aJQ49$7!"$=$N$"$H$rO"J8@aJQ49$9$k!#(B
 *	$B0z?t(B small $B$,(B 0 $B$G$J$1$l$P>.J8@a$,!"$=$&$G$J$1$l$PBgJ8@a$,(B
 *	$B%+%l%s%HJ8@a$H$7$F;H$o$l$k!#(B
 *	$B0z?t(B jump $B$G!"JQ498e$N%+%l%s%HJ8@a$N0LCV$,7h$^$k!#(Bjump $B$,(B
 *	0 $B$J$i%+%l%s%HJ8@a$N0LCV$OJQ49$7$F$bJQ$o$i$J$$(B ($B$?$@$7(B
 *	$B%+%l%s%HJ8@a$H$7$FBgJ8@a$r;XDj$7$?>l9g!"JQ498e$N%+%l%s%H(B
 *	$B>.J8@a$O%+%l%s%HBgJ8@a$N:G=i$N>.J8@a$K$J$k(B) $B$,!"(B0 $B$G$J$1$l$P(B
 *	$B:G8e$NJ8@a$N<!(B ($B6uJ8@a(B) $B$K0\F0$9$k!#C`<!JQ49$7$F$$$/$h$&$J(B
 *	$B%"%W%j%1!<%7%g%s$G$O$3$l$r(B 1 $B$K$9$k$H$h$$$@$m$&!#(B
 *
 * int jcUnconvert(jcConvBuf *buf)
 *	$B%+%l%s%HBgJ8@a$rL5JQ49$N>uBV$KLa$9!#(B
 *	$B%+%l%s%HBgJ8@a$,$$$/$D$+$N>.J8@a$+$i$G$-$F$$$?>l9g!"$3$l$i$N(B
 *	$B>.J8@a$O$^$H$a$i$l!"0l$D$NL5JQ49>uBV$NJ8@a$K$J$k!#(B
 *	$B%+%l%s%H>.J8@a$rL5JQ49$KLa$95!G=$OMQ0U$7$J$$!#$J$<$+$H$$$&$H!"(B
 *	$BBgJ8@a$NCf$N(B 1 $B>.J8@a$N$_$,L5JQ49$K$J$C$F$7$^$&$H!"$=$NJ8@a$K(B
 *	$B4X$7$F(B jcMove() $B$G0\F0$r9T$J$C$?;~!"$I$&0\F0$9$l$P$h$$$N$+(B
 *	$B$h$/$o$+$i$J$$!"$D$^$j0\F0$N%;%^%s%F%#%/%9$,ITL@3N$K$J$C$F$7$^$&(B
 *	$B$+$i$G$"$k!#(B
 *
 * int jcKana(jcConvBuf *buf, int small, int kind)
 *	$B%+%l%s%HJ8@a$r$+$J$K$9$k!#(B
 *	$B0z?t(B kind $B$,!"(BJC_HIRAGANA $B$J$i$R$i$,$J!"(BJC_KATAKANA $B$J$i%+%?%+%J$K(B
 *	$BJQ$o$k!#J8@a$NJQ49>uBV$OJQ2=$7$J$$!#$D$^$jJQ49$5$l$F$$$l$P(B
 *	$BJQ49>uBV$N$^$^!"L$JQ49$N>uBV$J$iL$JQ49$N$^$^$G$"$k!#(B
 *	$B0z?t(B small $B$,(B 0 $B$G$J$1$l$P%+%l%s%H>.J8@a$,!"$=$&$G$J$1$l$P(B
 *	$B%+%l%s%HBgJ8@a$,JQ$o$k!#(B
 *	$B%+%l%s%HBgJ8@a$r$+$J$K$9$k>l9g!"$=$NCf$N>.J8@a$O0l$D$K$^$H$a$i$l$k!#(B
 *
 * int jcFix(jcConvBuf *buf)
 *	$B8=:_!"%P%C%U%!$K$O$$$C$F$$$kJQ49J8;zNs$r3NDj$5$;$k!#(B
 *
 * int jcExpand(jcConvBuf *buf, int small, int convf)
 *	$B%+%l%s%HJ8@a$ND9$5$r#1J8;z?-$P$9!#0z?t(B convf $B$,(B 0 $B$G$J$1$l$P(B
 *	$B?-$P$7$?$"$H:FJQ49$9$k!#(B
 *	$B0z?t(B small $B$,(B 0 $B$G$J$1$l$P>.J8@a$,!"$=$&$G$J$1$l$PBgJ8@a$,(B
 *	$B%+%l%s%HJ8@a$H$7$F;H$o$l$k!#(B
 *
 * int jcShrink(jcConvBuf *buf, int small, int convf)
 *	$B%+%l%s%HJ8@a$ND9$5$r#1J8;z=L$a$k!#0z?t(B convf $B$,(B 0 $B$G$J$1$l$P(B
 *	$B=L$a$?$"$H:FJQ49$9$k!#(B
 *	$B0z?t(B small $B$,(B 0 $B$G$J$1$l$P>.J8@a$,!"$=$&$G$J$1$l$PBgJ8@a$,(B
 *	$B%+%l%s%HJ8@a$H$7$F;H$o$l$k!#(B
 *
 * int jcNext(jcConvBuf *buf, int small, int prev)
 *	$B%+%l%s%HJ8@a$r<!8uJdKt$OA08uJd$GCV$-49$($k!#(B
 *	$B0z?t(B small $B$,(B 0 $B$G$J$1$l$P>.J8@a$,!"$=$&$G$J$1$l$PBgJ8@a$,(B
 *	$B%+%l%s%HJ8@a$H$7$F;H$o$l$k!#(B
 *
 * int jcCandidateInfo(jcConvBuf *buf, int small, int *ncandp, int *curcandp)
 *	$B<!8uJd$N>pJs$rJV$9!#(B
 *	$B<!8uJd0lMw$r=P$9$?$a$K$O:G=i$K$3$N4X?t$r8F$V$H$h$$!#(B
 *
 * int jcGetCandidate(jcConvBuf *buf, int n, wchar *candstr)
 *	$B;XDj$5$l$?8uJdHV9f$NJ8;zNs$rJV$9!#%+%l%s%H8uJdHV9f$O$3$NHV9f$K(B
 *	$BJQ$o$k!#%G%#%9%W%l%$%P%C%U%!$OJQ2=$7$J$$!#(B
 *	$B@N$N(B jclib $B$O<!8uJd$,MQ0U$5$l$F$$$J$1$l$PMQ0U$7$?$,!"$3$N%P!<%8%g%s(B
 *	$B$G$O%(%i!<$K$J$k!#(BjcNext $B$d(B jcCandidateInfo $B$r@h$K8F$s$G$*$+$J$1$l$P(B
 *	$B$J$i$J$$!#(B
 *
 * int jcSelect(jcConvBuf *buf, int n)
 *	$B;XDj$5$l$?HV9f$N8uJd$G%G%#%9%W%l%$%P%C%U%!$rCV$-49$($k!#(B
 *	$B%+%l%s%H8uJdHV9f$O$3$NHV9f$KJQ$o$k!#(B
 *
 * int jcDotOffset(jcConvBuf *buf)
 *	$BBgJ8@a$N@hF,$+$i$N%I%C%H$N%*%U%;%C%H$rJV$9!#(B
 *	$BNc$($P(B 0 $B$J$i%I%C%H$,%+%l%s%HJ8@a$N@hF,$K$"$k$3$H$K$J$k!#(B
 *
 * int jcIsConverted(jcConvBuf *buf, int cl)
 *	$B;XDj$5$l$?J8@a$,JQ49$5$l$F$$$k$+$I$&$+$rJV$9(B
 *	0 $B$J$iL5JQ49>uBV(B
 *	1 $B$J$iJQ49>uBV(B
 *	-1 $B$J$i(B $B%(%i!<(B
 *
 * int jcMove(jcConvBuf *buf, int small, int dir)
 *	$B%I%C%H!&%+%l%s%HJ8@a$r0\F0$9$k!#(B
 *	$B%+%l%s%HJ8@a$,JQ49:Q$_$G$"$l$PJ8@a0\F0$7!"$=$&$G$J$1$l$P(B
 *	$B%I%C%H$N$_$,0\F0$9$k!#(B
 *	$BJ8@a0\F0;~$K!"0z?t(B small $B$,(B 0 $B$G$J$1$l$P>.J8@aC10L$G0\F0$7!"(B
 *	$B$=$&$G$J$1$l$PBgJ8@aC10L$K0\F0$9$k!#(B
 *
 * int jcTop(jcConvBuf *buf)
 *	$B%I%C%H!&%+%l%s%HJ8@a$rJ8$N@hF,$K0\F0$9$k!#%+%l%s%H>.J8@a!&(B
 *	$B%+%l%s%HBgJ8@a$H$b$K0\F0$9$k!#(B
 *
 * int jcBottom(jcConvBuf *buf)
 *	$B%I%C%H!&%+%l%s%HJ8@a$rJ8$N:G8e$K0\F0$9$k!#%+%l%s%H>.J8@a!&(B
 *	$B%+%l%s%HBgJ8@a$H$b$K0\F0$9$k!#(B
 *	$B$b$7!":G8e$NJ8@a$,L5JQ49>uBV$G$"$l$P%+%l%s%HJ8@a$O$=$NJ8@a$K$J$j!"(B
 *	$B%I%C%H$O$=$NJ8@a$N:G8e$KMh$k!#$=$&$G$J$1$l$P%+%l%s%HJ8@a$O(B
 *	$B:G8e$NJ8@a$N<!(B ($B$D$^$j6u$NJ8@a(B) $B$KMh$k!#(B
 *
 * int jcChangeClause(jcConvBuf *buf, wchar *str)
 *	$B%+%l%s%HBgJ8@a$r;XDj$5$l$?J8;zNs$GF~$l49$($k!#(B
 *	$B%G%#%9%W%l%$%P%C%U%!$@$1$G$O$J$/!"$+$J%P%C%U%!$NFbMF$b(B
 *	$BCV$-49$o$k!#J8@a$OL5JQ49>uBV$K$J$k!#(B
 *
 * int jcSaveDic(jcConvBuf *buf)
 *	$B;HMQCf$N4D6-$G;H$o$l$F$$$kA4$F$N<-=qJB$S$KIQEY%U%!%$%k$r(B
 *	$B%;!<%V$9$k!#(B
 *	$B$3$N%U%!%s%/%7%g%s$O>o$K(B 0 $B$rJV$9!#K\Ev$K%;!<%V$5$l$?$+$N(B
 *	$B%A%'%C%/$O$7$J$$!#(B
 *
 * $B$3$l$i$N%U%!%s%/%7%g%s$OFC$K=q$+$l$F$$$J$1$l$P@.8y$N>l9g$K$O(B 0,
 * $B%(%i!<$N>l9g$K$O(B -1 $B$rJV$9!#(B
 */

/*
 * $B%0%m!<%P%kJQ?t(B
 *
 * jclib $B$G;H$o$l$k%0%m!<%P%kJQ?t$O(B jcErrno $B$?$@0l$D$G$"$k!#(B
 *
 * extern int jcErrno
 *	$B%(%i!<$N:]$K!"%(%i!<%3!<%I$,BeF~$5$l$k!#%(%i!<%3!<%I$O(B jclib.h $B$G(B
 *	$BDj5A$5$l$F$$$k!#(B
 */

/*
 * $B%G!<%?9=B$(B
 *
 * jclib $B$N;}$D%G!<%?$G!"%"%W%j%1!<%7%g%s$+$iD>@\%"%/%;%9$7$F$h$$$N$O(B
 * $BJQ49%P%C%U%!(B jcConvBuf $B7?$N(B public member $B$H=q$+$l$?ItJ,$N$_$G$"$k!#(B
 * $BD>@\%"%/%;%9$7$F$h$$$H$$$C$F$b!"CM$r;2>H$9$k$@$1$G!"CM$rJQ99$9$k$3$H$O(B
 * $B5v$5$l$J$$!#%"%W%j%1!<%7%g%s$,>!<j$KCM$rJQ99$7$?>l9g$N(B jclib $B$NF0:n$O(B
 * $BJ]>Z$5$l$J$$!#(B
 *
 * <$BJQ49%P%C%U%!(B>
 *
 * jcConvBuf $B7?$O(B jclib.h $B$G<!$N$h$&$KDj5A$5$l$F$$$k!#(B
 *
 * typedef struct {
 *    /-* public member *-/
 *	int		nClause;	$BJ8@a?t(B
 *	int		curClause;	$B%+%l%s%HJ8@aHV9f(B
 *	int		curLCStart;	$B%+%l%s%HBgJ8@a3+;OJ8@aHV9f(B
 *	int		curLCEnd;	$B%+%l%s%HBgJ8@a=*N;J8@aHV9f(B
 *	wchar		*kanaBuf;	$B$+$J%P%C%U%!(B
 *	wchar		*kanaEnd;
 *	wchar		*displayBuf;	$B%G%#%9%W%l%$%P%C%U%!(B
 *	wchar		*displayEnd;
 *	jcClause	*clauseInfo;	$BJ8@a>pJs(B
 *	struct wnn_env	*env;
 *    /-* private member *-/
 *	[ $B>JN,(B ]
 * } jcConvBuf;
 *
 * nClause $B$O8=:_$NJ8@a?t$rI=$9!#$3$l$O>.J8@a$N?t$G$"$k!#(B
 * curClause $B$O%+%l%s%H>.J8@a$NHV9f$G$"$k!#(B
 * curLCStart $B$H(B curLCEnd $B$O%+%l%s%HBgJ8@a$NHO0O$r<($9!#(BcurLCStart $B$+$i(B
 * curLCEnd-1 $B$NHO0O$NJ8@a$,%+%l%s%HBgJ8@a$G$"$k!#$D$^$j!"(BcurLCEnd $B$O(B
 * $B<!$NBgJ8@a$N@hF,$NHV9f$G$"$k!#(B
 *
 * kanaBuf $B$H(B displayBuf $B$,$+$J%P%C%U%!$H%G%#%9%W%l%$%P%C%U%!$G$"$k!#(B
 * jcInsertChar() $BEy$r;H$C$FF~NO$5$l$?FI$_$O$+$J%P%C%U%!$H%G%#%9%W%l%$(B
 * $B%P%C%U%!$KF~$k!#$3$l$rJQ49$9$k$H!"%G%#%9%W%l%$%P%C%U%!$NJ}$@$1$,(B
 * $B4A;z$NJ8;zNs$K$J$k!#(B
 * kanaEnd $B$*$h$S(B displayEnd $B$O$=$l$>$l$N%P%C%U%!$KF~$l$i$l$?J8;zNs$N:G8e(B
 * $B$NJ8;z$N<!$r;X$7$F$$$k!#$+$J%P%C%U%!!&%G%#%9%W%l%$%P%C%U%!$O$I$A$i$b(B
 * NULL $B%?!<%_%M!<%H$5$l$J$$!#(B
 *
 * clauseInfo $B$OJ8@a>pJs$NF~$C$?G[Ns$G$"$k!#$3$l$O$"$H$G@bL@$9$k!#(B
 *
 * env $B$O$3$NJQ49%P%C%U%!$N;HMQ$9$k4D6-$G$"$k!#(B
 *
 * <$BJ8@a>pJs(B>
 *
 * $B3FJ8@a$N>pJs$O(B clauseInfo $B$H$$$&L>A0$N(B jcClause $B7?$NG[Ns$KF~$C$F$$$k!#(B
 * jcClause $B7?$O(B jclib.h $B$G<!$N$h$&$KDj5A$5$l$F$$$k!#(B
 *
 * typedef struct {
 *	wchar	*kanap;		$BFI$_J8;zNs(B ($B$+$J%P%C%U%!$NCf$r;X$9(B)
 *	wchar	*fzkp;		$BIUB08l$NFI$_J8;zNs(B ($B$+$J%P%C%U%!$NCf$r;X$9(B)
 *	wchar	*dispp;		$BI=<(J8;zNs(B ($B%G%#%9%W%l%$%P%C%U%!$NCf$r;X$9(B)
 *	int	dicno;		$B<-=qHV9f(B
 *	int	entry;		$B%(%s%H%j(B
 *	int	hinshi;		$BIJ;lHV9f(B
 *	int	kangovect;	$BA0C<$N@\B3%Y%/%H%k(B
 *	char	conv;		$BJQ49:Q$_$+(B
 *				0: $BL$JQ49(B 1: $BJQ49:Q(B -1: jclib$B$G5?;wJQ49(B
 *	char	ltop;		$BBgJ8@a$N@hF,$+(B?
 *	char	imabit;		$B:#;H$C$?$h%S%C%H(B
 * } jcClause;
 *
 * kanap, fzkp $B$O$+$J%P%C%U%!>e$N!"$=$NJ8@a$NFI$_$N;O$^$j$H!"IUB08l$N(B
 * $B;O$^$j$N0LCV$r<($9%]%$%s%?$G$"$k!#$^$?!"(Bdispp $B$O!"%G%#%9%W%l%$%P%C%U%!(B
 * $B>e$G!"$=$NJ8@a$N;O$^$j$N0LCV$r<($9!#=>$C$F!"(Bn $BHV$NJ8@a$O!"(B
 *	$B$h$_(B:	clauseInfo[n].kanap $B$+$i(B clauseInfo[n+1].kanap $B$NA0$^$G(B
 *	$BIUB08l(B:	clauseInfo[n].fzkp $B$+$i(B clauseInfo[n+1].kanap $B$NA0$^$G(B
 *	    ($BIUB08l$,$J$1$l$P(B clauseInfo[n].fzkp == clauseInfo[n+1].kanap $B$K(B
 *	     $B$J$k(B)
 *	$B4A;z(B:	clauseInfo[n].dispp $B$+$i(B clauseInfo[n+1].dispp $B$NA0$^$G(B
 * $B$H$J$k!#$3$N$h$&$K(B n $BHVL\$NJ8@a$NHO0O$r<($9$N$K(B n+1 $BHVL\$N(B clauseInfo $B$,(B
 * $BI,MW$J$?$a!"(BclauseInfo $B$NG[Ns$NMWAG$O>o$K@hF,$+$iJ8@a?t(B+1$B8D$,M-8z$G$"$k!#(B
 *
 * dicno, entry, hinshi, kangovect $B$K$D$$$F$O(B Wnn $B$N%I%-%e%a%s%H$r;2>H$N$3$H!#(B
 *
 * conv $B$O$=$NJ8@a$NJQ49>uBV$rI=$9!#(B0 $B$J$iL$JQ49>uBV!"(B1 $B$J$iJQ49>uBV!"(B
 * -1 $B$J$i(B jcKana() $B$K$h$C$F5?;wJQ49$5$l$?$3$H$r<($9!#(B
 *
 * ltop $B$,(B 0 $B$G$J$1$l$P$=$NJ8@a$,BgJ8@a$N@hF,$G$"$k$3$H$r<($9!#(Bimabit $B$O(B
 * $B$=$NJ8@a$N448l$N:#;H$C$?$h%S%C%H$,F~$C$F$$$k!#(B
 *
 * kanap, dispp $BEy$G!"(Bn $BHVL\$NJ8@a$NHO0O$r<($9$N$K(B n+1 $BHVL\$NJ8@a>pJs$,(B
 * $BI,MW$J$?$a!"(BclauseInfo $B$NG[Ns$NMWAG$O>o$K@hF,$+$iJ8@a?t(B+1$B8D$,M-8z$G$"$k!#(B
 * $BJ8@a?t(B+1 $B8DL\$NJ8@a>pJs(B (clauseInfo[nClause]) $B$O(B
 *	kanap, dispp: $B$=$l$>$l(B kanaEnd, displayEnd $B$KEy$7$$(B
 *	conv: 0 ($BL$JQ49>uBV(B)
 *	ltop: 1
 *	$B;D$j$N%a%s%P$OITDj(B
 * $B$G$"$k!#(B
 *
 * $BJ8@a>pJs$N(B kanap, fzkp, dispp $B$rNc$r;H$C$F<($7$F$*$/!#(B
 *
 * $BNcJ8(B: $B$3$l$O%G!<%?9=B$$r<($9$?$a$NNcJ8$G$9(B ($BJ8@a?t(B 6)
 *
 * kanap:   $B#0(B    $B#1(B    $B#2(B        $B#3(B    $B#4(B    $B#5(B          $B#6(B(=kanaEnd)
 *	    $B"-(B    $B"-(B    $B"-(B        $B"-(B    $B"-(B    $B"-(B          $B"-(B
 * kanaBuf: $B$3$l$O$G!<$?$3$&$>$&$r$7$a$9$?$a$N$l$$$V$s$G$9(B
 *		$B",(B      $B",(B      $B",(B    $B",(B    $B",(B        $B",(B
 * fzkp:	$B#0(B      $B#1(B      $B#2(B    $B#3(B    $B#4(B        $B#5(B
 *
 * dispp:      $B#0(B    $B#1(B    $B#2(B    $B#3(B  $B#4(B    $B#5(B      $B#6(B(=displayEnd)
 *	       $B"-(B    $B"-(B    $B"-(B    $B"-(B  $B"-(B    $B"-(B      $B"-(B
 * displayBuf: $B$3$l$O%G!<%?9=B$$r<($9$?$a$NNcJ8$G$9(B
 */

#ifndef lint
static char	*rcsid = "$Id: jclib.c,v 5.2 1992/12/24 04:43:05 ishisone Rel $";
#endif

#ifdef DEBUG_JCLIB
#include	<stdio.h>
#endif
#include	"commonhd.h"
#include	"jslib.h"
#include	"jclib.h"

#ifdef CHECK_PROTOTYPE
#include	"js.c.p"
#endif

#ifndef NULL
#define NULL	0
#endif

#define CHECKFIXED(buf)	\
	{ if ((buf)->fixed) { jcErrno = JE_ALREADYFIXED; return -1; } }
#define Free(p)		{if (p) free((char *)(p));}
#define DotSet(buf)	(buf)->dot = (buf)->clauseInfo[(buf)->curLCStart].kanap

#define KANABEG	0xa4a1	/* '$B$!(B' */
#define KANAEND	0xa4f3	/* '$B$s(B' */
#define KATAOFFSET	0x100	/* $B%+%?%+%J$H$R$i$,$J$N%3!<%I!&%*%U%;%C%H(B */

/* $B%G%U%)%k%H$N%P%C%U%!%5%$%:(B */
#define DEF_BUFFERSIZE	100	/* 100 $BJ8;z(B */
#define DEF_CLAUSESIZE	20	/* 20 $BJ8@a(B */
#define DEF_CANDSIZE	1024	/* 1K $B%P%$%H(B */
#define DEF_RESETSIZE	10	/* 10 $BC18l(B */

/* buf->candKind $B$NCM(B */
#define CAND_SMALL	0	/* $B>.J8@a8uJd(B */
#define CAND_LARGE	1	/* $BBgJ8@a8uJd(B */

#define MAXFZK	LENGTHBUNSETSU

extern char	*malloc();
extern char	*realloc();
extern void	free();
#ifdef SVR4
extern char	*memmove();
extern char	*memset();
#define bcopy(p, q, l)	memmove(q, p, l)
#define bzero(p, l)	memset(p, 0, l)
#else
#if defined(SYSV) || defined(USG)
#define OVERLAP_BCOPY
extern char	*memset();
#define bzero(p, l)	memset(p, 0, l)
#else
extern int	bcopy();
extern int	bzero();
#endif
#endif

/*
 * $B<!8uJd%P%C%U%!$N%G!<%?%?%$%W(B ($B%i%$%V%i%j$N30$+$i$O8+$($J$$$h$&$K$9$k(B)
 * buf->candBuf $B$N;X$9@h$O!">.J8@a8uJd$N;~$O(B jcSCand $B$NG[Ns$G!"(B $BBgJ8@a8uJd$N(B
 * $B;~$K$O(B jcLCand $B$NG[Ns$G$"$k(B
 */
/* $B>.J8@a8uJd(B */
typedef struct {
	wchar	*kanji;		/* $BI=<(J8;zNs(B */
	int	kanalen;	/* $B$+$J$ND9$5(B */
	int	fzkoffset;
	int	dicno;
	int	entry;
	int	hinshi;
	int	kangovect;
	int	status;
#define	IMA_BIT		1	/* $B:#;H$C$?$h%S%C%H(B */
#define CONNECT_PREV	2	/* $BA0J8@a$K@\B3$G$-$k$+(B? */
#define CONNECT_NEXT	4	/* $B8e$m$NJ8@a$K@\B3$G$-$k$+(B? */
} jcSCand;

/* $BBgJ8@a8uJd(B */
typedef struct {
	int	nscand;		/* $B4^$^$l$k>.J8@a$N?t(B */
	jcSCand	*scand;		/* $B>.J8@a$NG[Ns(B */
} jcLCand;

/*
 * $B%j%;%C%H%P%C%U%!(B ($B:#;H$C$?$h%S%C%H$rMn$9BP>]$N%(%s%H%j$r$N%G!<%?%?%$%W(B
 * ($B%i%$%V%i%j$N30$+$i$O8+$($J$$$h$&$K$9$k(B)
 * buf->resetBuf $B$N;X$9@h$O!"(BjcEntry $B$NG[Ns$G$"$k(B
 */
typedef struct {
	int	dicno;
	int	entry;
} jcEntry;

#ifdef __STDC__
/* $B%U%!%s%/%7%g%s%W%m%H%?%$%W@k8@(B */
static int wstrlen(wchar *);
static void setcinfo(jcClause *, struct wnn_sho_bunsetsu *,
		     wchar *, wchar *);
static void changecinfo(jcClause *, jcSCand *, wchar *, wchar *);
static int wstrlen(wchar *);
static void getFVec(jcClause *, int *, wchar *, int *);
static void getBVec(jcClause *, int *, int *, int *);
static void moveKBuf(jcConvBuf *, int, int);
static void moveDBuf(jcConvBuf *, int, int);
static void moveCInfo(jcConvBuf *, int, int);
static int resizeBuffer(jcConvBuf *, int);
static int resizeCInfo(jcConvBuf *, int);
static int resizeCandBuf(jcConvBuf *, int);
static void setCurClause(jcConvBuf *, int);
static int renConvert(jcConvBuf *, int);
static int tanConvert(jcConvBuf *, int);
static int doKanrenConvert(jcConvBuf *, int);
static int doKantanDConvert(jcConvBuf *, int, int);
static int doKantanSConvert(jcConvBuf *, int);
static int unconvert(jcConvBuf *, int, int);
static int expandOrShrink(jcConvBuf *, int, int, int);
static int getKanjiLenDbun(struct wnn_dai_bunsetsu *, int);
static int getKanjiLenSbun(struct wnn_sho_bunsetsu *, int);
static int getSCandidates(jcConvBuf *);
static int findSCand(jcSCand *, jcSCand *, struct wnn_sho_bunsetsu *);
static int setSCandData(jcConvBuf *, int, struct wnn_sho_bunsetsu *, int);
static int getSCandDataLen(jcClause *, int, struct wnn_sho_bunsetsu *);
static int getLCandidates(jcConvBuf *);
static int findLCand(jcLCand *, jcLCand *, struct wnn_dai_bunsetsu *);
static int setLCandData(jcConvBuf *, int, struct wnn_dai_bunsetsu *);
static int getLCandDataLen(jcClause *, jcClause *, int,
			   struct wnn_dai_bunsetsu *);
static int changeCand(jcConvBuf *, int);
static int setupCandBuf(jcConvBuf *, int);
static void checkAndResetCandidates(jcConvBuf *, int, int);
static void addResetClause(jcConvBuf *, int, int);
static void addResetCandidate(jcConvBuf *, int);
static void addResetEntry(jcConvBuf *, int, int);
static void saveDicAll(jcConvBuf *);
#else
static int wstrlen();
static void setcinfo();
static void changecinfo();
static int wstrlen();
static void getFVec();
static void getBVec();
static void moveKBuf();
static void moveDBuf();
static void moveCInfo();
static int resizeBuffer();
static int resizeCInfo();
static int resizeCandBuf();
static void setCurClause();
static int renConvert();
static int tanConvert();
static int doKanrenConvert();
static int doKantanDConvert();
static int doKantanSConvert();
static int unconvert();
static int expandOrShrink();
static int getKanjiLenDbun();
static int getKanjiLenSbun();
static int getSCandidates();
static int findSCand();
static int setSCandData();
static int getSCandDataLen();
static int getLCandidates();
static int findLCand();
static int setLCandData();
static int getLCandDataLen();
static int changeCand();
static int setupCandBuf();
static void checkAndResetCandidates();
static void addResetClause();
static void addResetCandidate();
static void addResetEntry();
static void saveDicAll();
#endif

/* $B%(%i!<HV9f(B */
int	jcErrno;

static struct wnn_ret_buf	jsbuf = { 0, NULL };

/*
 *	portability $B$N$?$a$N%U%!%s%/%7%g%s(B
 */

#ifdef OVERLAP_BCOPY
#undef bcopy
static
bcopy(from, to, n)
register char *from;
register char *to;
register int n;
{
	if (n <= 0 || from == to) return;

	if (from < to) {
		from += n;
		to += n;
		while (n-- > 0)
			*--to = *--from;
	} else {
		while (n-- > 0)
			*to++ = *from++;
	}
}
#endif

/*
 *	jclib $BFbIt$G;H$o$l$k%U%!%s%/%7%g%s(B
 */

/* wstrlen -- wchar $B7?J8;zNs$N(B strlen */
static int
wstrlen(s)
wchar *s;
{
	int	n = 0;

	while (*s++)
		n++;
	return n;
}

/* setcinfo -- clauesInfo $B$K>.J8@a$N>pJs$r%;%C%H$9$k(B */
static void
setcinfo(clp, sbun, kanap, dispp)
jcClause *clp;
struct wnn_sho_bunsetsu *sbun;
wchar *kanap;
wchar *dispp;
{
	/* ltop $B%U%i%0$O$H$j$"$($:(B 0 $B$K$7$F$*$/(B */
	clp->conv = 1;
	clp->dicno = sbun->dic_no;
	clp->entry = sbun->entry;
	clp->hinshi = sbun->hinsi;
	clp->kangovect = sbun->kangovect;
	clp->ltop = 0;
	clp->kanap = kanap + sbun->start;
	clp->fzkp = kanap + sbun->jiriend + 1;
	clp->dispp = dispp;
	clp->imabit = sbun->ima;
}

/* changecinfo -- clauseInfo $B$N>pJs$r;XDj$5$l$?8uJd$N$b$N$GCV$-49$($k(B */
static void
changecinfo(clp, candp, kanap, dispp)
jcClause *clp;
jcSCand *candp;
wchar *kanap;
wchar *dispp;
{
	clp->kanap = kanap;
	clp->fzkp = clp->kanap + candp->fzkoffset;
	clp->dispp = dispp;
	clp->dicno = candp->dicno;
	clp->entry = candp->entry;
	clp->hinshi = candp->hinshi;
	clp->kangovect = candp->kangovect;
	clp->conv = 1;
	clp->imabit = (candp->status & IMA_BIT) != 0;
	clp->ltop = (candp->status & CONNECT_PREV) == 0;
}

/* getFVec -- $B>.J8@a$N!"A0$H$N@\B3$rD4$Y$F%Q%i%a!<%?$r7h$a$k(B */
static void
getFVec(clp, hinship, fzk, connp)
jcClause *clp;
int *hinship;
wchar *fzk;
int *connp;
{
	wchar	*p, *q, *r;

	if (clp->ltop) {
		/* $BBgJ8@a@hF,(B */
		*hinship = WNN_BUN_SENTOU;
		fzk[0] = 0;
	} else if ((clp - 1)->conv != 1) {
		/* $BA0$NJ8@a$OJQ49$5$l$F$$$J$$!"$"$k$$$O$+$JJQ49$5$l$F$$$k(B */
		*hinship = WNN_ALL_HINSI;
		fzk[0] = 0;
	} else {
		*hinship = (clp - 1)->hinshi;
		if (connp) *connp |= CONNECT_PREV;
		/* $BA0$NJ8@a$NIUB08lJ8;zNs$r%3%T!<$9$k(B */
		p = fzk;
		q = (clp - 1)->fzkp;
		r = clp->kanap;
		while (q < r) {
			*p++ = *q++;
		}
		/* NULL $B%?!<%_%M!<%H$5$;$J$1$l$P$J$i$J$$(B */
		*p = 0;
	}
}

/* getBVec -- $B>.J8@a$N!"8e$m$H$N@\B3$rD4$Y$F%Q%i%a!<%?$r7h$a$k(B */
static void
getBVec(clp, vecp, vec1p, connp)
jcClause *clp;
int *vecp;
int *vec1p;
int *connp;
{
#if WNN_VECT_KANTAN != WNN_VECT_KANZEN
	/* $BF1$8(B getBVec() $B$rC1J8@aJQ49$HA48uJd<h$j=P$7$K;H$&$3$H$,$G$-$J$$(B */
	!! ERROR !!
#else
	if ((++clp)->ltop || clp->conv != 1) {
		*vecp = WNN_VECT_KANZEN;
		*vec1p = WNN_VECT_NO;
	} else {
		*vecp = clp->kangovect;
		*vec1p = WNN_VECT_KANZEN;
		if (connp) *connp |= CONNECT_NEXT;
	}
#endif
}

static int
getKanjiLenDbun(dbun, ndbun)
struct wnn_dai_bunsetsu *dbun;
int ndbun;
{
	struct wnn_sho_bunsetsu	*sbun;
	int	nsbun;
	int	nc = 0;

	while (ndbun-- > 0) {
		sbun = dbun->sbn;
		nsbun = dbun->sbncnt;
		while (nsbun-- > 0) {
			nc += wstrlen(sbun->kanji) + wstrlen(sbun->fuzoku);
			sbun++;
		}
		dbun++;
	}
	return nc;
}

static int
getKanjiLenSbun(sbun, nsbun)
struct wnn_sho_bunsetsu *sbun;
int nsbun;
{
	int	nc = 0;

	while (nsbun-- > 0) {
		nc += wstrlen(sbun->kanji) + wstrlen(sbun->fuzoku);
		sbun++;
	}

	return nc;
}

/* moveKBuf -- $B$+$J%P%C%U%!$N;XDj$5$l$?J8@a$N@hF,$+$i$"$H$rF0$+$9(B */
static void
moveKBuf(buf, cl, move)
jcConvBuf *buf;
int cl;
int move;
{
	jcClause	*clp = buf->clauseInfo + cl;
	jcClause	*clpend;
	int		movelen;

	if (move == 0) return;

	if ((movelen = buf->kanaEnd - clp->kanap) > 0) {
		/* $B$+$J%P%C%U%!$NFbMF$rF0$+$9(B */
		(void)bcopy((char *)clp->kanap, (char *)(clp->kanap + move),
			    movelen * sizeof(wchar));
	}

	/* $B$+$J%P%C%U%!$NJQ99$K9g$o$;$F(B clauseInfo $B$r%"%C%W%G!<%H$9$k(B */
	clpend = buf->clauseInfo + buf->nClause;
	while (clp <= clpend) {
		clp->kanap += move;
		clp++->fzkp += move;
	}

	/* kanaEnd $B$N%"%C%W%G!<%H(B */
	buf->kanaEnd += move;
}

/* moveDBuf -- $B%G%#%9%W%l%$%P%C%U%!$N;XDj$5$l$?J8@a$N@hF,$+$i$"$H$rF0$+$9(B */
static void
moveDBuf(buf, cl, move)
jcConvBuf *buf;
int cl;
int move;
{
	jcClause	*clp = buf->clauseInfo + cl;
	jcClause	*clpend;
	int		movelen;

	if (move == 0) return;

	if ((movelen = buf->displayEnd - clp->dispp) > 0) {
		/* $B%G%#%9%W%l%$%P%C%U%!$NFbMF$rF0$+$9(B */
		(void)bcopy((char *)clp->dispp, (char *)(clp->dispp + move),
			    movelen * sizeof(wchar));
	}

	/* $B%G%#%9%W%l%$%P%C%U%!$NJQ99$K9g$o$;$F(B clauseInfo $B$r(B
	 * $B%"%C%W%G!<%H$9$k(B
	 */
	clpend = buf->clauseInfo + buf->nClause;
	while (clp <= clpend) {
		clp++->dispp += move;
	}

	/* displayEnd $B$N%"%C%W%G!<%H(B */
	buf->displayEnd += move;
}

/* moveCInfo -- ClauseInfo $B$N;XDj$5$l$?J8@a$N@hF,$+$i$"$H$rF0$+$9(B */
static void
moveCInfo(buf, cl, move)
jcConvBuf *buf;
int cl;
int move;
{
	jcClause	*clp = buf->clauseInfo + cl;
	int		len;

	/* move $B$K@5$N?t$r;XDj$9$l$PJ8@a$NA^F~!"Ii$J$iJ8@a$N:o=|$K$J$k(B */

	if (move == 0) return;

	if ((len = buf->nClause + 1 - cl) > 0) {
		(void)bcopy((char *)clp, (char *)(clp + move),
			    len * sizeof(jcClause));
	}
	buf->nClause += move;
	if (buf->candClause >= 0) {
		buf->candClause += move;
		buf->candClauseEnd += move;
	}
}

/* resizeBuffer -- $B$+$J(B/$B%G%#%9%W%l%$%P%C%U%!$NBg$-$5$rJQ$($k(B */
static int
resizeBuffer(buf, len)
jcConvBuf *buf;
int len;
{
	wchar	*kbufold, *dbufold;
	wchar	*kbufnew, *dbufnew;
	int	allocsize;
	jcClause	*clp, *clpend;

	kbufold = buf->kanaBuf;
	dbufold = buf->displayBuf;

	/* realloc $B$9$k(B */
	allocsize = (len + 1) * sizeof(wchar);
	kbufnew = (wchar *)realloc((char *)kbufold, allocsize);
	dbufnew = (wchar *)realloc((char *)dbufold, allocsize);

	if (kbufnew == NULL || dbufnew == NULL) {
		Free(kbufnew);
		Free(dbufnew);
		jcErrno = JE_NOCORE;
		return -1;
	}

	buf->bufferSize = len;

	if (kbufnew == kbufold && dbufnew == dbufold) {
		/* $B%]%$%s%?$OA0$HJQ$o$C$F$$$J$$(B */
		return 0;
	}

	/* $B3F<o%]%$%s%?$r$D$1JQ$($k(B */

	buf->kanaBuf = kbufnew;
	buf->kanaEnd = kbufnew + (buf->kanaEnd - kbufold);
	buf->displayBuf = dbufnew;
	buf->displayEnd = dbufnew + (buf->displayEnd - dbufold);

	buf->dot = kbufnew + (buf->dot - kbufold);

	clp = buf->clauseInfo;
	clpend = clp + buf->nClause;
	while (clp <= clpend) {
		clp->kanap = kbufnew + (clp->kanap - kbufold);
		clp->fzkp = kbufnew + (clp->fzkp - kbufold);
		clp->dispp = dbufnew + (clp->dispp - dbufold);
		clp++;
	}

	return 0;
}

/* resizeCInfo -- clauseInfo $B%P%C%U%!$NBg$-$5$rJQ$($k(B */
static int
resizeCInfo(buf, size)
jcConvBuf *buf;
int size;
{
	jcClause	*cinfonew;

	/* realloc $B$9$k(B */
	cinfonew = (jcClause *)realloc((char *)buf->clauseInfo,
				       (size + 1) * sizeof(jcClause));
	if (cinfonew == NULL) {
		jcErrno = JE_NOCORE;
		return -1;
	}

	buf->clauseSize = size;
	buf->clauseInfo = cinfonew;
	return 0;
}

/* resizeCandBuf -- $B<!8uJd%P%C%U%!$NBg$-$5$rJQ$($k(B */
static int
resizeCandBuf(buf, nbytes)
jcConvBuf *buf;
int nbytes;
{
	char	*p;

	if ((p = realloc((char *)buf->candBuf, nbytes)) == NULL) {
		jcErrno = JE_NOCORE;
		return -1;
	}

	buf->candSize = nbytes;
	buf->candBuf = p;
	return 0;
}

/* setCurClause -- $B%+%l%s%HJ8@a$r@_Dj$9$k(B */
static void
setCurClause(buf, cl)
jcConvBuf *buf;
int cl;		/* $B%+%l%s%H>.J8@aHV9f(B */
{
	jcClause	*clp = buf->clauseInfo;
	int		i;

	/* $B%+%l%s%H>.J8@a(B */
	buf->curClause = cl;

	/* $B%+%l%s%HBgJ8@a3+;OJ8@a(B */
	for (i = cl; i > 0 && !clp[i].ltop; i--)
		;
	buf->curLCStart = i;

	/* $B%+%l%s%HBgJ8@a=*N;J8@a(B ($B$N<!(B) */
	for (i = cl + 1; i <= buf->nClause && !clp[i].ltop; i++)
		;
	buf->curLCEnd = i;
}

/* renConvert -- $B%+%l%s%HJ8@a$+$i8e$m$rO"J8@aJQ49$9$k(B */
static int
renConvert(buf, small)
jcConvBuf *buf;
int small;
{
	/* $BO"J8@aJQ49$9$k(B */
	if (doKanrenConvert(buf,
			    small ? buf->curClause : buf->curLCStart) < 0) {
		return -1;
	}

	/*
	 * $B%+%l%s%HJ8@a$N@_Dj(B
	 * small $B$,(B 0 $B$J$i!"(B
	 *	$B%+%l%s%HBgJ8@a$N@hF,$O(B buf->curLCStart $B$GJQ$o$i$:(B
	 *	$B%+%l%s%HBgJ8@a=*$j$O(B ltop $B%U%i%0$r%5!<%A$7$FC5$9(B
	 *	$B%+%l%s%H>.J8@a$O%+%l%s%HBgJ8@a@hF,$K0\F0(B
	 * small $B$,(B 0 $B$G$J$$$J$i!"(B
	 *	$B%+%l%s%H>.J8@a$O(B buf->curClause $B$GJQ$o$i$:(B
	 *	$B%+%l%s%HBgJ8@a$N@hF,$*$h$S=*$j$O!"%+%l%s%H>.J8@a$N(B
	 *	$BA08e$r(B ltop $B%U%i%0$r%5!<%A$7$FC5$9(B
	 */
	setCurClause(buf, small ? buf->curClause : buf->curLCStart);

	/* $B%I%C%H$N@_Dj(B */
	DotSet(buf);

	return 0;
}

/* tanConvert -- $B%+%l%s%HJ8@a$rC1J8@aJQ49$9$k(B */
static int
tanConvert(buf, small)
jcConvBuf *buf;
int small;
{
	/*
	 * $BC1J8@aJQ49$N>l9g!"4pK\E*$K(B 2 $BCJ3,$N=hM}$r9T$J$&$3$H$K$J$k(B
	 * $B$^$:!"%+%l%s%HJ8@a$rC1J8@aJQ49(B
	 * $B<!$K!"$=$N$"$H$rO"J8@aJQ49(B
	 */

	if (small) {
		/* $B$^$:C1J8@aJQ49$9$k(B */
		if (doKantanSConvert(buf, buf->curClause) < 0)
			return -1;

		/* $B%+%l%s%HJ8@a$N@_Dj(B
		 *	$B%+%l%s%H>.J8@a$O(B buf->curClause $B$GJQ$o$i$:(B
		 *	$B%+%l%s%HBgJ8@a$N@hF,$H:G8e$O%+%l%s%H>.J8@a$N(B
		 *	$BA08e$K(B ltop $B%U%i%0$r%5!<%A$7$FC5$9(B
		 */
		setCurClause(buf, buf->curClause);
		/* $B%I%C%H$N@_Dj(B */
		DotSet(buf);

		/* $BO"J8@aJQ49(B */
		if (buf->curClause + 1 < buf->nClause &&
		    buf->clauseInfo[buf->curClause + 1].conv == 0) {
			/* $B>.J8@a$NC1J8@aJQ49%b!<%I$G!"<!$NJ8@a$,(B
			 * $BL5JQ49$@$C$?>l9g!"(Bltop $B%U%i%0$r(B 0 $B$K$7$F(B
			 * $BA0$H@\B3$G$-$k$h$&$K$9$k(B
			 */
			buf->clauseInfo[buf->curClause + 1].ltop = 0;
		}
		if (doKanrenConvert(buf, buf->curClause + 1) < 0)
			return -1;

		/* $B$b$&0lEY%+%l%s%HJ8@a$N@_Dj(B
		 * $BO"J8@aJQ49$N7k2L$K$h$C$F$O%+%l%s%HBgJ8@a$N:G8e$,(B
		 * $B0\F0$9$k$3$H$,$"$k(B
		 */
		setCurClause(buf, buf->curClause);

		/* $B%I%C%H$O0\F0$7$J$$$N$G:F@_Dj$7$J$/$F$h$$(B */
	} else {
		/* $B$^$:C1J8@aJQ49$9$k(B */
		if (doKantanDConvert(buf, buf->curLCStart, buf->curLCEnd) < 0)
			return -1;

		/* $B%+%l%s%HJ8@a$N@_Dj(B
		 *	$B%+%l%s%HBgJ8@a$N@hF,$O(B buf->curLCStart $B$GJQ$o$i$:(B
		 *	$B%+%l%s%HBgJ8@a=*$j$O(B ltop $B%U%i%0$r%5!<%A$7$FC5$9(B
		 *	$B%+%l%s%H>.J8@a$O%+%l%s%HBgJ8@a@hF,$K0\F0(B
		 */
		setCurClause(buf, buf->curLCStart);
		DotSet(buf);

		/* $BO"J8@aJQ49(B */
		if (doKanrenConvert(buf, buf->curLCEnd) < 0)
			return -1;
		/* $B$3$A$i$O(B small $B$N;~$H0c$C$FO"J8@aJQ49$N7k2L%+%l%s%HJ8@a$,(B
		 * $B0\F0$9$k$3$H$O$J$$(B
		 */
	}

	return 0;
}

/* doKanrenConvert -- $B;XDj$5$l$?J8@a$+$i8e$m$rO"J8@aJQ49$9$k(B */
static int
doKanrenConvert(buf, cl)
jcConvBuf *buf;
int cl;
{
	jcClause	*clp;
	int	hinshi;
	wchar	*kanap, *dispp;
	wchar	*wp;
	wchar	fzk[MAXFZK];
	struct wnn_dai_bunsetsu	*dbun;
	struct wnn_sho_bunsetsu	*sbun;
	int	ndbun, nsbun;
	int	len;
	int	i, j;

	/*
	 * $B;XDj$5$l$?J8@a$+$i8e$m$rO"J8@aJQ49$9$k(B
	 * $B%+%l%s%HJ8@a$N:F@_Dj$J$I$O$7$J$$(B
	 */

	if (cl >= buf->nClause) {
		/* $B;XDj$5$l$?J8@a$O$J$$(B
		 * $B%(%i!<$K$O$7$J$$(B
		 * $B6u$NJ8@a$rJQ49$7$h$&$H$7$?;~$K!"$=$l$r;vA0$K%A%'%C%/$7$F(B
		 * $B%(%i!<$K$9$k$N$O>e0L$N4X?t$N@UG$$G$"$k(B
		 */
		return 0;
	}

	clp = buf->clauseInfo + cl;

	/* $BA0$NJ8@a$H$N@\B3$rD4$Y$k(B */
	getFVec(clp, &hinshi, fzk, NULL);

	*(buf->kanaEnd) = 0;	/* NULL $B%?!<%_%M!<%H$5$;$F$*$/(B */
	/* $BO"J8@aJQ49$9$k(B */
	ndbun = js_kanren(buf->env, clp->kanap, hinshi, fzk,
			  WNN_VECT_KANREN, WNN_VECT_NO, WNN_VECT_BUNSETSU,
			  &jsbuf);

	if (ndbun < 0) {
		jcErrno = JE_WNNERROR;
		return -1;
	}

	dbun = (struct wnn_dai_bunsetsu *)jsbuf.buf;

	/* $B$H$j$"$($:>.J8@a$N?t$r?t$($k(B */
	nsbun = 0;
	for (i = 0; i < ndbun; i++) {
		nsbun += dbun[i].sbncnt;
	}

	/* clauseInfo $B$N%5%$%:$N%A%'%C%/(B */
	if (cl + nsbun > buf->clauseSize) {
		if (resizeCInfo(buf, cl + nsbun) < 0)
			return -1;
	}

	/* $B<!$KJQ49J8;zNs$ND9$5$N%A%'%C%/(B */
	clp = buf->clauseInfo + cl;
	len = (clp->dispp - buf->displayBuf) + getKanjiLenDbun(dbun, ndbun);
	if (len > buf->bufferSize) {
		if (resizeBuffer(buf, len) < 0)
			return -1;
	}

	buf->nClause = cl + nsbun;

	/* $B$G$O(B clauseInfo $B$KJQ497k2L$rF~$l$F$$$/(B */
	clp = buf->clauseInfo + cl;
	kanap = clp->kanap;
	dispp = clp->dispp;
	for (i = 0; i < ndbun; i++) {
		int	connstate = dbun->sbn->status;

		sbun = dbun->sbn;
		nsbun = dbun->sbncnt;

		for (j = 0; j < nsbun; j++) {
			setcinfo(clp, sbun, kanap, dispp);

			/* ltop $B$N@_Dj(B */
			clp->ltop = (j == 0 && connstate != WNN_CONNECT);

			/* $B%G%#%9%W%l%$%P%C%U%!$X$NJQ49J8;zNs$N%3%T!<(B */
			/* $B<+N)8lItJ,(B */
			wp = sbun->kanji;
			while (*dispp++ = *wp++)
				;
			dispp--;
			/* $BIUB08lItJ,(B */
			wp = sbun->fuzoku;
			while (*dispp++ = *wp++)
				;
			dispp--;

			sbun++;
			clp++;
		}
		dbun++;
	}

	/* $B:G8e$N(B clauseInfo $B$N@_Dj(B */
	clp->kanap = buf->kanaEnd;
	clp->dispp = buf->displayEnd = dispp;
	clp->conv = 0;
	clp->ltop = 1;

	return 0;
}

/* doKantanDConvert -- $B;XDj$5$l$?HO0O$NJ8@a$rBgJ8@a$H$7$FC1J8@aJQ49$9$k(B */
static int
doKantanDConvert(buf, cls, cle)
jcConvBuf *buf;
int cls;
int cle;
{
	jcClause	*clps, *clpe;
	int	len, diff, newlen;
	int	cldiff, nclausenew;
	int	hinshi;
	wchar	*kanap, *dispp;
	wchar	*wp;
	wchar	fzk[MAXFZK];
	wchar	savechar;
	wchar	*savep;
	struct wnn_dai_bunsetsu	*dbun;
	struct wnn_sho_bunsetsu	*sbun;
	int	ndbun, nsbun;
	int	i;

	/*
	 * $B;XDj$5$l$?HO0O$NJ8@a$rBgJ8@a$H$7$FC1J8@aJQ49$9$k(B
	 * $B%+%l%s%HJ8@a$N:F@_Dj$J$I$O$7$J$$(B
	 */

	clps = buf->clauseInfo + cls;
	clpe = buf->clauseInfo + cle;
	/* $BA0$NJ8@a$H$N@\B3$rD4$Y$k(B */
	getFVec(clps, &hinshi, fzk, NULL);

	/* $BFI$_$r(B NULL $B%?!<%_%M!<%H$9$k(B
	 * $BC1$K(B 0 $B$rF~$l$k$H<!$NJ8@a$,2u$l$k$N$G!"$=$NA0$K%;!<%V$7$F$*$/(B
	 */
	savep = clpe->kanap;
	savechar = *savep;
	*savep = 0;

	/* $BC1J8@aJQ49$9$k(B */
	ndbun = js_kantan_dai(buf->env, clps->kanap, hinshi, fzk,
			      WNN_VECT_KANTAN, WNN_VECT_NO, &jsbuf);
	/* $B$9$+$5$:%;!<%V$7$F$"$C$?J8;z$r$b$H$KLa$9(B */
	*savep = savechar;
	if (ndbun < 0) {
		jcErrno = JE_WNNERROR;
		return -1;
	}

	dbun = (struct wnn_dai_bunsetsu *)jsbuf.buf;
	sbun = dbun->sbn;
	nsbun = dbun->sbncnt;

	cldiff = nsbun - (cle - cls);
	nclausenew = buf->nClause + cldiff;
	/* clauseInfo $B$N%5%$%:$N%A%'%C%/(B */
	if (nclausenew > buf->clauseSize) {
		if (resizeCInfo(buf, nclausenew) < 0)
			return -1;
	}

	/* $BJQ49J8;zNs$ND9$5$N%A%'%C%/(B */
	len = getKanjiLenDbun(dbun, 1);
	diff = len - (clpe->dispp - clps->dispp);
	newlen = (buf->displayEnd - buf->displayBuf) + diff;
	if (newlen > buf->bufferSize) {
		if (resizeBuffer(buf, newlen) < 0)
			return -1;
	}

	/* $BJ8@a$rA^F~$9$k$N$G!"%G%#%9%W%l%$%P%C%U%!$NFbMF$r0\F0$5$;$k(B */
	/* $B$I$&$;$"$H$+$iO"J8@aJQ49$9$k$+$i$$$$$G$O$J$$$+$H$$$&9M$(J}$b$"$k$,!"(B
	 * $B$I$3$G%(%i!<$,5/$3$C$F$b0l1~$N(B consistency $B$,J]$?$l$k$h$&$K(B
	 * $B$9$k$H$$$&$N$,L\I8$G$"$k(B
	 */
	moveDBuf(buf, cle, diff);

	/* clauseInfo $B$rF0$+$9(B ($BF1;~$K(B nClause $B$b%"%C%W%G!<%H$5$l$k(B) */
	moveCInfo(buf, cle, cldiff);

	/* $B$G$O(B clauseInfo $B$KJQ497k2L$rF~$l$k(B */
	clps = buf->clauseInfo + cls;
	kanap = clps->kanap;
	dispp = clps->dispp;
	for (i = 0; i < nsbun; i++) {
		setcinfo(clps, sbun, kanap, dispp);

		clps->ltop = i == 0 && sbun->status != WNN_CONNECT;

		/* $B%G%#%9%W%l%$%P%C%U%!$X$NJQ49J8;zNs$N%3%T!<(B */
		/* $B<+N)8lItJ,(B */
		wp = sbun->kanji;
		while (*dispp++ = *wp++)
			;
		dispp--;
		/* $BIUB08lItJ,(B */
		wp = sbun->fuzoku;
		while (*dispp++ = *wp++)
			;
		dispp--;

		clps++;
		sbun++;
	}

	/* $B<!$N(B clauseInfo $B$N@_Dj(B */
	clps->ltop = 1;

	return 0;
}

/* doKantanSConvert -- $B;XDj$5$l$?J8@a$r>.J8@a$H$7$FC1J8@aJQ49$9$k(B */
static int
doKantanSConvert(buf, cl)
jcConvBuf *buf;
int cl;
{
	jcClause	*clp;
	int	len, newlen, diff;
	int	hinshi;
	wchar	*dispp;
	wchar	fzk[MAXFZK];
	wchar	*wp;
	wchar	savechar;
	wchar	*savep;
	int	vec, vec1;
	struct wnn_sho_bunsetsu	*sbun;
	int	nsbun;

	/*
	 * $B;XDj$5$l$?J8@a$r>.J8@a$H$7$FC1J8@aJQ49$9$k(B
	 * $B%+%l%s%HJ8@a$N:F@_Dj$J$I$O$7$J$$(B
	 */

	clp = buf->clauseInfo + cl;

	/* $BA0$NJ8@a$H$N@\B3$rD4$Y$k(B */
	getFVec(clp, &hinshi, fzk, NULL);
	/* $B8e$m$N>.J8@a$H$N@\B3$rD4$Y$k(B */
	getBVec(clp, &vec, &vec1, NULL);

	/* $BFI$_$r(B NULL $B%?!<%_%M!<%H$9$k(B
	 * $BC1$K(B 0 $B$rF~$l$k$H<!$NJ8@a$,2u$l$k$N$G!"$=$NA0$K%;!<%V$7$F$*$/(B
	 */
	savep = (clp + 1)->kanap;
	savechar = *savep;
	*savep = 0;

	/* $BC1J8@aJQ49$9$k(B */
	nsbun = js_kantan_sho(buf->env, clp->kanap, hinshi, fzk,
			      vec, vec1, &jsbuf);
	/* $B$9$+$5$:%;!<%V$7$F$"$C$?J8;z$r$b$H$KLa$9(B */
	*savep = savechar;
	if (nsbun < 0) {
		jcErrno = JE_WNNERROR;
		return -1;
	}

	sbun = (struct wnn_sho_bunsetsu *)jsbuf.buf;

	/* $BJQ49J8;zNs$ND9$5$N%A%'%C%/(B */
	clp = buf->clauseInfo + cl;
	len = getKanjiLenSbun(sbun, 1);
	diff = len - ((clp + 1)->dispp - clp->dispp);
	newlen = (buf->displayEnd - buf->displayBuf) + diff;
	if (newlen > buf->bufferSize) {
		if (resizeBuffer(buf, newlen) < 0)
			return -1;
	}

	/* $BJ8@a$rA^F~$9$k$N$G!"%G%#%9%W%l%$%P%C%U%!$NFbMF$r0\F0$5$;$k(B */
	/* $B$I$&$;$"$H$+$iO"J8@aJQ49$9$k$+$i$$$$$G$O$J$$$+$H$$$&9M$(J}$b$"$k$,!"(B
	 * $B$I$3$G%(%i!<$,5/$3$C$F$b0l1~$N(B consistency $B$,J]$?$l$k$h$&$K(B
	 * $B$9$k$H$$$&$N$,L\I8$G$"$k(B
	 */
	moveDBuf(buf, cl + 1, diff);

	/* $B$G$O(B clauseInfo $B$KJQ497k2L$rF~$l$k(B */
	clp = buf->clauseInfo + cl;
	dispp = clp->dispp;

	setcinfo(clp, sbun, clp->kanap, dispp);
	clp->ltop = sbun->status != WNN_CONNECT;

	/* $B%G%#%9%W%l%$%P%C%U%!$X$NJQ49J8;zNs$N%3%T!<(B */
	/* $B<+N)8lItJ,(B */
	wp = sbun->kanji;
	while (*dispp++ = *wp++)
		;
	dispp--;
	/* $BIUB08lItJ,(B */
	wp = sbun->fuzoku;
	while (*dispp++ = *wp++)
		;

	/* $B<!$N(B clauseInfo $B$N@_Dj(B */
	(++clp)->ltop = sbun->status_bkwd == WNN_NOT_CONNECT_BK ||
	    vec == WNN_VECT_KANTAN;

	return 0;
}

/* unconvert -- $B;XDj$5$l$?HO0O$NJ8@a$r0l$D$NL5JQ49$NJ8@a$K$9$k(B */
static int
unconvert(buf, start, end)
jcConvBuf *buf;
int start;
int end;
{
	jcClause	*clps, *clpe;
	int	diff, len;

	if (end <= start)
		return 0;

	if (start >= buf->nClause)
		return 0;

	clps = buf->clauseInfo + start;
	clpe = buf->clauseInfo + end;

	/*
	 * $B%G%#%9%W%l%$%P%C%U%!$NFbMF$r%+%J%P%C%U%!$NFbMF$GCV$-49$($k(B
	 * $B!D$H$$$C$F$b<B:]$NF0:n$O$=$l$[$I4JC1$G$O$J$$(B
	 *
	 * $B!&$^$:!"CV$-49$($?7k2L!"%G%#%9%W%l%$%P%C%U%!$,$"$U$l$J$$$+D4$Y!"(B
	 *   $B$"$U$l$k$h$&$J$i%P%C%U%!$N%5%$%:$rBg$-$/$9$k(B
	 * $B!&%G%#%9%W%l%$%P%C%U%!$K!"$+$J%P%C%U%!$+$i%G!<%?$r0\$9(B
	 * $B!&(BclauseInfo $B$r=q$-49$($F!"(Bstart $B$+$i(B end-1 $B$^$G$NJ8@a$r(B
	 *   $B0l$D$NL5JQ49$NJ8@a$K$^$H$a$k(B
	 * $B!&$b$A$m$s(B nClause $B$bJQ$($k(B
	 * $B!&(Bstart+1 $B$+$i:G8e$^$G$NJ8@a$N(B clauseInfo $B$N(B dispp $B$r(B
	 *   $B%G%#%9%W%l%$%P%C%U%!$N$:$l$K1~$8$FD4@0$9$k(B
	 *
	 * $B$=$NB>$K<!$N$3$H$b9T$J$&I,MW$,$"$k$,!"$3$N4X?t$G$O$d$i$J$$(B
	 * $B>e0L$N4X?t$G@_Dj$9$k$3$H(B
	 * $B!&BgJ8@a%U%i%0(B (ltop) $B$N@_Dj(B
	 * $B!&%+%l%s%HJ8@a!"$*$h$S<!8uJdJ8@a$N0\F0(B
	 *   $B<!8uJdJ8@a$,L5JQ49$NJ8@a$K$J$C$F$7$^$C$?;~$N=hM}(B
	 * $B!&%I%C%H$N0\F0(B
	 */

	/* $BFI$_$ND9$5$H4A;z$ND9$5$N:9$rD4$Y$k(B */
	diff = (clpe->kanap - clps->kanap) - (clpe->dispp - clps->dispp);
	/* $BCV$-49$($?>l9g$N%G%#%9%W%l%$%P%C%U%!$ND9$5(B */
	len = (buf->displayEnd - buf->displayBuf) + diff;
	/* $B%P%C%U%!$N%5%$%:$,B-$j$J$1$l$P%5%$%:$rBg$-$/$9$k(B */
	if (len > buf->bufferSize) {
		if (resizeBuffer(buf, len) < 0) {
			/* $B%5%$%:$,JQ$($i$l$J$+$C$?(B */
			return -1;
		}
	}

	/* $BCV$-49$((B */
	/* $B$^$:8e$m$NItJ,$rF0$+$7$F$+$i(B */
	moveDBuf(buf, end, diff);
	/* $BFI$_$rF~$l$k(B */
	(void)bcopy((char *)clps->kanap, (char *)clps->dispp,
		    (clpe->kanap - clps->kanap) * sizeof(wchar));

	/*
	 * start $B$+$i(B end $B$^$G$NJ8@a$r0l$D$K$^$H$a$k(B
	 */

	/* $BL5JQ49>uBV$K$J$C$?J8@a$N(B clauseInfo $B$N@_Dj(B */
	clps->conv = 0;

	/* end $B$+$i$"$H$N(B clauseInfo $B$r(B'$B$D$a$k(B' */
	moveCInfo(buf, end, start + 1 - end);

	return 0;
}

static int
expandOrShrink(buf, small, expand, convf)
jcConvBuf *buf;
int small;
int expand;
int convf;
{
	jcClause	*clp, *clpe;
	int	start, end;

	start = small ? buf->curClause : buf->curLCStart;
	end = small ? start + 1 : buf->curLCEnd;

	clp = buf->clauseInfo + start;
	clpe = buf->clauseInfo + end;

	/*
	 * $B?-$S=L$_$G$-$k$+$N%A%'%C%/(B
	 */
	if (expand) {
		/*
		 * $B%+%l%s%HJ8@a$,:G8e$NJ8@a$N;~$K$O(B
		 * $B$b$&9-$2$i$l$J$$(B
		 */
		if (end >= buf->nClause) {
			jcErrno = JE_CANTEXPAND;
			return -1;
		}
	} else {
		if (buf->curClause == buf->nClause ||
		    clpe->kanap - clp->kanap <= 1) {
			/* $B%+%l%s%HJ8@a$,6u$+!"$"$k$$$OD9$5$,#10J2<(B */
			jcErrno = JE_CANTSHRINK;
			return -1;
		}
	}

	/* $B%+%l%s%HJ8@a$*$h$S$=$N<!$NC18l$r%j%;%C%H%j%9%H$KF~$l$k(B */
	addResetClause(buf, start, (end >= buf->nClause) ? end : end + 1);

	/* $B8uJdJ8@a$,%+%l%s%HJ8@a$N8e$m$K$"$l$PL58z$K$9$k(B */
	checkAndResetCandidates(buf, start, buf->nClause);

	/* $B%+%l%s%HJ8@a$,JQ49$5$l$F$$$l$P!"$H$j$"$($:L5JQ49$K$9$k(B */
	if (clp->conv) {
		if (unconvert(buf, start, end) < 0)
			return -1;
	}

	/* $B%+%l%s%HJ8@a$N:F@_Dj(B */
	if (small) {
		buf->curClause = start;
		buf->curLCEnd = start + 1;
	} else {
		buf->curClause = buf->curLCStart = start;
		buf->curLCEnd = start + 1;
	}
	DotSet(buf);
	buf->clauseInfo[start + 1].ltop = 1;

	/* $B$5$i$K%+%l%s%HJ8@a$N8e$m$rL5JQ49$K$9$k(B
	 * $B$3$N;~$K$O$9$G$K(B start $B$+$i(B end $B$^$G$NJ8@a$,0l$D$N(B
	 * $BL5JQ49$NJ8@a$K$^$H$a$i$l$F$$$k$3$H$KCm0U(B
	 */
	if (unconvert(buf, start + 1, buf->nClause) < 0)
		return -1;

	if (small) {
		if (start + 1 < buf->nClause)
			buf->clauseInfo[start + 1].ltop = 0;
		buf->curLCEnd = buf->nClause;
	}
	clp = buf->clauseInfo + start;
	/*
	 * $B%+%l%s%HJ8@a$ND9$5$r#1J8;z?-$S=L$_$5$;$k(B
	 */
	if (expand) {
		/* $B%+%l%s%HJ8@a$ND9$5$r?-$P$9(B */
		(++clp)->kanap++;
		clp->dispp++;
		/* $B9-$2$?7k2L!"8e$m$NJ8@a$,$J$/$J$k$3$H$,$"$k(B */
		if (clp->kanap == buf->kanaEnd) {
			buf->nClause--;
			clp->ltop = 1;
		}
	} else {
		/* $B%+%l%s%HJ8@a$ND9$5$r=L$a$k(B */
		(++clp)->kanap--;
		clp->dispp--;
		/* $B%+%l%s%HJ8@a$,:G8e$NJ8@a$@$C$?>l9g$K$O!"#1J8@aA}$($k(B */
		if (start == buf->nClause - 1) {
			if (buf->nClause >= buf->clauseSize) {
				if (resizeCInfo(buf, buf->nClause + 1) < 0) {
					/* $BJQ99$7$?=j$r85$KLa$9(B */
					clp = buf->clauseInfo + start + 1;
					clp->kanap++;
					clp->dispp--;
					return -1;
				}
			}

			/* $B>e$G(B clauseInfo $B$,(B realloc $B$5$l$?$+$b$7$l$J$$$N$G(B
			 * clp $B$r:F$S@_Dj(B
			 */
			clp = buf->clauseInfo + buf->nClause;

			/* $B?7$7$/$G$-$?J8@a$N(B clauseInfo $B$N@_Dj(B
			 * kanap, dispp $B$O$9$G$K>e$G%;%C%H$5$l$F$$$k(B
			 */
			clp->ltop = small ? 0 : 1;
			clp->conv = 0;

			/* $B:G8e$NJ8@a$N<!$N(B ($B=*C<$N(B) clauseInfo $B$N@_Dj(B */
			(++clp)->kanap = buf->kanaEnd;
			clp->dispp = buf->displayEnd;
			clp->conv = 0;

			buf->nClause++;
		}
	}

	if (convf) {
		return tanConvert(buf, small);
	}
	return 0;
}

/* getSCandidates -- $B%+%l%s%H>.J8@a$NA48uJd$r<h$j=P$9(B */
static int
getSCandidates(buf)
jcConvBuf *buf;
{
	wchar	savechar;
	wchar	*savep;
	int	hinshi;
	wchar	fzk[MAXFZK];
	int	vec, vec1;
	int	ncand;
	jcClause	*clp;
	int	conn = 0;

	clp = buf->clauseInfo + buf->curClause;

	/* $BA0$H$N@\B3$rD4$Y$k(B */
	getFVec(clp, &hinshi, fzk, &conn);
	/* $B8e$m$H$N@\B3$rD4$Y$k(B */
	getBVec(clp, &vec, &vec1, &conn);

	/* $BFI$_$r(B NULL $B%?!<%_%M!<%H$7$F$*$/(B */
	savep = (clp + 1)->kanap;
	savechar = *savep;
	*savep = 0;

	/* $BA48uJd$r<h$j=P$9(B */
	ncand = js_kanzen_sho(buf->env, clp->kanap, hinshi, fzk,
			  vec, vec1, &jsbuf);
	/* $B$9$+$5$:FI$_$r$b$H$KLa$9(B */
	*savep = savechar;
	if (ncand < 0) {
		jcErrno = JE_WNNERROR;
		return -1;
	}

	if (setSCandData(buf, ncand, (struct wnn_sho_bunsetsu *)jsbuf.buf,
			 conn) < 0)
		return -1;

	/* $B8uJdJ8@a$N>pJs$rF~$l$k(B */
	buf->curCand = 0;
	buf->candKind = CAND_SMALL;
	buf->candClause = buf->curClause;
	buf->candClauseEnd = buf->curClause + 1;	/* $BG0$N$?$a(B */

	return 0;
}

/* getLCandidates -- $B%+%l%s%HBgJ8@a$NA48uJd$r<h$j=P$9(B */
static int
getLCandidates(buf)
jcConvBuf *buf;
{
	wchar	savechar;
	wchar	*savep;
	int	ncand;
	jcClause	*clps, *clpe;

	clps = buf->clauseInfo + buf->curLCStart;
	clpe = buf->clauseInfo + buf->curLCEnd;

	/* $BFI$_$r(B NULL $B%?!<%_%M!<%H$7$F$*$/(B */
	savep = clpe->kanap;
	savechar = *savep;
	*savep = 0;

	/* $BA48uJd$r<h$j=P$9(B */
	ncand = js_kanzen_dai(buf->env, clps->kanap, WNN_BUN_SENTOU, NULL,
			      WNN_VECT_KANZEN, WNN_VECT_NO, &jsbuf);
	/* $B$9$+$5$:FI$_$r$b$H$KLa$9(B */
	*savep = savechar;
	if (ncand < 0) {
		jcErrno = JE_WNNERROR;
		return -1;
	}

	if (setLCandData(buf, ncand, (struct wnn_dai_bunsetsu *)jsbuf.buf) < 0)
		return -1;

	/* $B8uJdJ8@a$N>pJs$rF~$l$F$*$/(B */
	buf->curCand = 0;
	buf->candKind = CAND_LARGE;
	buf->candClause = buf->curLCStart;
	buf->candClauseEnd = buf->curLCEnd;

	return 0;
}

/* findSCand -- $BF1$88uJd$,$J$$$+$I$&$+D4$Y$k(B ($B>.J8@a8uJd(B) */
static int
findSCand(scps, scpe, sbun)
jcSCand *scps;
jcSCand *scpe;
struct wnn_sho_bunsetsu *sbun;
{
	int	kangovect = sbun->kangovect;
	int	hinshi = sbun->hinsi;
	wchar	c = sbun->kanji[0];
	wchar	*p, *q;

	while (scps < scpe) {
		/* $BIJ;l$H!"A0$X$N@\B3%Y%/%H%k$H!"@hF,$N#1J8;z$,F1$8$+$I$&$+$G(B
		 * $B$U$k$$$K$+$1$k(B
		 * jllib $B$N%=!<%9$r8+$k$H(B kangovect $B$NHf3S$O%3%a%s%H%"%&%H(B
		 * $B$5$l$F$$$k$,!"$J$<$@$m$&(B?
		 */
		if (scps->hinshi == hinshi &&
		    scps->kangovect == kangovect &&
		    scps->kanji[0] == c) {
			/* $B;zLL$,F1$8$+D4$Y$k(B */
			p = scps->kanji;
			q = sbun->kanji;
			while (*q) {
				if (*p++ != *q++) goto next;
			}
			q = sbun->fuzoku;
			while (*q) {
				if (*p++ != *q++) goto next;
			}
			if (*p) goto next;

			/* $BF1$8(B */
			return 1;
		}
	next:
		scps++;
	}
	return 0;
}

/* findSCand -- $BF1$88uJd$,$J$$$+$I$&$+D4$Y$k(B ($BBgJ8@a8uJd(B) */
static int
findLCand(lcps, lcpe, dbun)
jcLCand *lcps;
jcLCand *lcpe;
struct wnn_dai_bunsetsu *dbun;
{
	struct wnn_sho_bunsetsu	*sbun;
	jcSCand	*scps, *scpe;
	int	nsbun = dbun->sbncnt;
	wchar	*p, *q;

	while (lcps < lcpe) {
		/* $B$H$j$"$($:>.J8@a?t$G$U$k$$$K$+$1$k(B */
		if (lcps->nscand != nsbun) goto next;

		scps = lcps->scand;
		scpe = scps + nsbun;
		sbun = dbun->sbn;
		while (scps < scpe) {
			/* $BIJ;l$H!"A0$X$N@\B3%Y%/%H%k$H!"@hF,$N#1J8;z$,(B
			 * $BF1$8$+$I$&$+$G$U$k$$$K$+$1$k(B
			 * jllib $B$N%=!<%9$r8+$k$H(B kangovect $B$NHf3S$O(B
			 * $B%3%a%s%H%"%&%H$5$l$F$$$k$,!"$J$<$@$m$&(B?
			 */
			if (scps->hinshi == sbun->hinsi &&
			    scps->kangovect == sbun->kangovect &&
			    scps->kanji[0] == sbun->kanji[0]) {
				/* $B;zLL$,F1$8$+D4$Y$k(B */
				p = scps->kanji;
				q = sbun->kanji;
				while (*q) {
					if (*p++ != *q++) goto next;
				}
				q = sbun->fuzoku;
				while (*q) {
					if (*p++ != *q++) goto next;
				}
				if (*p) goto next;
				
				scps++;
				sbun++;
			} else {
				goto next;	/* $B0c$&(B */
			}
		}
		/* $BF1$8$@$C$?(B */
		return 1;

	next:
		lcps++;
	}
	return 0;
}

/* setSCandData -- $B<h$j=P$7$?%+%l%s%H>.J8@a$NA48uJd$r3JG<$9$k(B */
static int
setSCandData(buf, ncand, sbun, conn)
jcConvBuf *buf;
int ncand;
struct wnn_sho_bunsetsu *sbun;
int conn;
{
	jcClause	*clp;
	jcSCand *scandp, *scp;
	wchar	*sp, *spend;
	wchar *wp;
	int	bytesneeded;
	int	i;

	clp = buf->clauseInfo + buf->curClause;

	/* buf->candBuf $B$KI,MW$JBg$-$5$rD4$Y$k(B */
	bytesneeded = getSCandDataLen(clp, ncand, sbun);
	if (bytesneeded > buf->candSize &&
	    resizeCandBuf(buf, bytesneeded) < 0) {
		return -1;
	}

	scandp = scp = (jcSCand *)buf->candBuf;
	wp = (wchar *)(scandp + (ncand + 1));

	/* $B$^$:!"<!8uJd$N:G=i$K!"8=:_$N>.J8@a$NFbMF$rF~$l$k(B */
	scp->kanji = wp;
	scp->kanalen = (clp + 1)->kanap - clp->kanap;
	scp->fzkoffset = clp->fzkp - clp->kanap;
	scp->dicno = clp->dicno;
	scp->entry = clp->entry;
	scp->hinshi = clp->hinshi;
	scp->kangovect = clp->kangovect;
	scp->status = clp->imabit ? IMA_BIT : 0;
	if (!clp->ltop) scp->status |= CONNECT_PREV;
	if (!(clp + 1)->ltop) scp->status |= CONNECT_NEXT;
	scp++;
	sp = clp->dispp;
	spend = (clp + 1)->dispp;
	while (sp < spend)
		*wp++ = *sp++;
	*wp++ = 0;

	/* $B<!$KA48uJd$N>pJs$rF~$l$k(B */
	for (i = 0; i < ncand; i++, sbun++) {
		if (findSCand(scandp, scp, sbun)) {
			continue;
		}
		scp->kanji = wp;
		scp->kanalen = sbun->end - sbun->start + 1;
		scp->fzkoffset = sbun->jiriend - sbun->start + 1;
		scp->dicno = sbun->dic_no;
		scp->entry = sbun->entry;
		scp->hinshi = sbun->hinsi;
		scp->kangovect = sbun->kangovect;
		scp->status = sbun->ima ? IMA_BIT : 0;
		/* $B>.J8@a$NA48uJd<h$j=P$7$N;~$K$O!"A08e$N@\B3$r8+$k(B
		 * $BI,MW$,$"$k(B
		 */
		if ((conn & CONNECT_PREV) && sbun->status == WNN_CONNECT) {
			scp->status |= CONNECT_PREV;
		}
		if ((conn & CONNECT_NEXT) &&
		    sbun->status_bkwd == WNN_CONNECT_BK) {
			scp->status |= CONNECT_NEXT;
		}
		sp = sbun->kanji;
		while (*wp++ = *sp++)
			;
		wp--;
		sp = sbun->fuzoku;
		while (*wp++ = *sp++)
			;
		scp++;
	}

	/* $B<B:]$K(B candBuf $B$KF~$l$i$l$?8uJd$N?t(B */
	buf->nCand = scp - scandp;

	return 0;
}

/* setLCandData -- $B<h$j=P$7$?%+%l%s%HBgJ8@a$NA48uJd$r3JG<$9$k(B */
static int
setLCandData(buf, ncand, dbun)
jcConvBuf *buf;
int ncand;
struct wnn_dai_bunsetsu *dbun;
{
	jcClause	*clp, *clps, *clpe;
	jcLCand *lcandp, *lcp;
	jcSCand *scandp, *scp;
	struct wnn_sho_bunsetsu *sbun;
	wchar	*sp, *spend;
	wchar *wp;
	int	nscand;
	int	bytesneeded;
	int	i, j;

	clps = buf->clauseInfo + buf->curLCStart;
	clpe = buf->clauseInfo + buf->curLCEnd;

	/* buf->candBuf $B$KI,MW$JBg$-$5$rD4$Y$k(B */
	bytesneeded = getLCandDataLen(clps, clpe, ncand, dbun);
	if (bytesneeded > buf->candSize &&
	    resizeCandBuf(buf, bytesneeded) < 0) {
		return -1;
	}

	/* $B>.J8@a$N?t$r?t$($k(B */
	nscand = 0;
	for (i = 0; i < ncand; i++) {
		nscand += dbun[i].sbncnt;
	}
	nscand += buf->curLCEnd - buf->curLCStart;
	lcandp = lcp = (jcLCand *)buf->candBuf;
	scandp = scp = (jcSCand *)(lcandp + (ncand + 1));
	wp = (wchar *)(scandp + nscand);

	/* $B$^$:!"<!8uJd$N:G=i$K!"8=:_$NBgJ8@a$NFbMF$rF~$l$k(B */
	lcp->nscand = buf->curLCEnd - buf->curLCStart;
	lcp++->scand = scp;
	clp = clps;
	while (clp < clpe) {
		scp->kanji = wp;
		scp->kanalen = (clp + 1)->kanap - clp->kanap;
		scp->fzkoffset = clp->fzkp - clp->kanap;
		scp->dicno = clp->dicno;
		scp->entry = clp->entry;
		scp->hinshi = clp->hinshi;
		scp->kangovect = clp->kangovect;
		scp->status = clp->imabit ? IMA_BIT : 0;
		if (clp != clps) scp->status |= CONNECT_PREV;
		if (clp != clpe - 1) scp->status |= CONNECT_NEXT;
		sp = clp->dispp;
		spend = (clp + 1)->dispp;
		while (sp < spend)
			*wp++ = *sp++;
		*wp++ = 0;
		scp++;
		clp++;
	}

	/* $B<!$KA48uJd$N>pJs$rF~$l$k(B */
	for (i = 0; i < ncand; i++, dbun++) {
		if (findLCand(lcandp, lcp, dbun)) {
			continue;
		}
		sbun = dbun->sbn;
		lcp->nscand = dbun->sbncnt;
		lcp->scand = scp;
		for (j = 0; j < dbun->sbncnt; j++) {
			scp->kanji = wp;
			scp->kanalen = sbun->end - sbun->start + 1;
			scp->fzkoffset = sbun->jiriend - sbun->start + 1;
			scp->dicno = sbun->dic_no;
			scp->entry = sbun->entry;
			scp->hinshi = sbun->hinsi;
			scp->kangovect = sbun->kangovect;
			scp->status = sbun->ima ? IMA_BIT : 0;
			/* $BA08e$NJ8@a$H$N@\B3>pJs$rF~$l$k(B
			 * $BBgJ8@a$NC1J8@aJQ49$J$N$G!"(B
			 * $B:G=i$N>.J8@a0J30$OA0$K$D$J$,$k$7!"(B
			 * $B:G8e$N>.J8@a0J30$O8e$m$K$D$J$,$k(B
			 */
			if (j != 0) {
				scp->status |= CONNECT_PREV;
			}
			if (j != dbun->sbncnt - 1) {
				scp->status |= CONNECT_NEXT;
			}
			sp = sbun->kanji;
			while (*wp++ = *sp++)
				;
			wp--;
			sp = sbun->fuzoku;
			while (*wp++ = *sp++)
				;
			scp++;
			sbun++;
		}
		lcp++;
	}

	/* $B<B:]$K(B candBuf $B$KF~$l$i$l$?8uJd$N?t(B */
	buf->nCand = lcp - lcandp;

	return 0;
}

/* getSCandDataLen -- $BA48uJd$N>pJs$rF~$l$k$N$KI,MW$J%a%b%j$NNL$rD4$Y$k(B ($B>.J8@a(B) */
static int
getSCandDataLen(clp, ncand, sbun)
jcClause *clp;
int ncand;
struct wnn_sho_bunsetsu *sbun;
{
	int	nbyte = 0;
	int	i;

	/* $B8=:_I=<($5$l$F$$$kJ8@a$bA48uJd$K4^$a$k$N$G!"(B
	 * ncand + 1 $B8D$V$sI,MW(B
	 */
	nbyte = sizeof(jcSCand) * (ncand + 1);

	nbyte += (((clp + 1)->dispp - clp->dispp) + 1) * sizeof(wchar);
	for (i = 0; i < ncand; i++, sbun++) {
		nbyte += (wstrlen(sbun->kanji) + wstrlen(sbun->fuzoku) + 1) *
		    sizeof(wchar);
	}

	return nbyte;
}

/* getLCandDataLen -- $BA48uJd$N>pJs$rF~$l$k$N$KI,MW$J%a%b%j$NNL$rD4$Y$k(B ($BBgJ8@a(B) */
static int
getLCandDataLen(clps, clpe, ncand, dbun)
jcClause *clps;
jcClause *clpe;
int ncand;
struct wnn_dai_bunsetsu *dbun;
{
	int	ndbun;
	int	nsbun;
	int	nchar;
	int	i;

	/* $BBgJ8@a$N?t(B */
	ndbun = ncand;

	/* $B>.J8@a$N?t(B */
	nsbun = 0;
	for (i = 0; i < ncand; i++) {
		nsbun += dbun[i].sbncnt;
	}

	/* $B4A;z$ND9$5(B */
	nchar = 0;
	for (i = 0; i < ncand; i++) {
		int	j = dbun[i].sbncnt;
		struct wnn_sho_bunsetsu *sbun = dbun[i].sbn;

		while (j-- > 0) {
			nchar += wstrlen(sbun->kanji) +
				wstrlen(sbun->fuzoku) + 1;
			sbun++;
		}
	}

	/* $B8=:_I=<($5$l$F$$$kJ8@a$bA48uJd$K4^$a$k(B */
	ndbun += 1;
	nsbun += clpe - clps;
	nchar += (clpe->dispp - clps->dispp) + (clpe - clps);

	return ndbun * sizeof(jcLCand) + nsbun * sizeof(jcSCand) +
	    nchar * sizeof(wchar);
}

/* changeCand -- $B%+%l%s%HJ8@a$r;XDj$5$l$?HV9f$N8uJd$GCV$-49$($k(B */
static int
changeCand(buf, n)
jcConvBuf *buf;
int n;
{
	int	buflen, oldlen, newlen;
	int	oldclen, newclen;
	wchar	*p;
	wchar	*kanap, *dispp;

	if (buf->candKind == CAND_SMALL) {
		jcSCand	*candp = ((jcSCand *)buf->candBuf) + n;
		jcClause	*clp = buf->clauseInfo + buf->curClause;
		/* $B%+%l%s%H>.J8@a$rJQ$($k(B */
		/* $B%G%#%9%W%l%$%P%C%U%!$N%5%$%:$N%A%'%C%/(B */
		newlen = wstrlen(candp->kanji);
		oldlen = (clp + 1)->dispp - clp->dispp;
		buflen = (buf->displayEnd - buf->displayBuf) + newlen - oldlen;
		if (buflen > buf->bufferSize &&
		    resizeBuffer(buf, buflen) < 0) {
			return -1;
		}
		/* $B%G%#%9%W%l%$%P%C%U%!$rF0$+$9(B */
		moveDBuf(buf, buf->curClause + 1, newlen - oldlen);
		/* $B8uJdJ8;zNs$r%3%T!<$9$k(B */
		(void)bcopy((char *)candp->kanji, (char *)clp->dispp,
			    newlen * sizeof(wchar));
		/* clauseInfo $B$N%"%C%W%G!<%H(B */
		changecinfo(clp, candp, clp->kanap, clp->dispp);
		/* $B8e$m$NJ8@a$N(B ltop $B$N@_Dj(B */
		(clp + 1)->ltop = !(candp->status & CONNECT_NEXT);
		/* $B%+%l%s%HBgJ8@a$N:F@_Dj(B */
		setCurClause(buf, buf->curClause);
	} else {
		jcLCand	*candp = ((jcLCand *)buf->candBuf) + n;
		jcSCand	*scandp;
		jcClause	*clps = buf->clauseInfo + buf->curLCStart;
		jcClause	*clpe = buf->clauseInfo + buf->curLCEnd;
		int	i;

		/* $B%+%l%s%HBgJ8@a$rJQ$($k(B */

		/* $B%G%#%9%W%l%$%P%C%U%!$N%5%$%:$N%A%'%C%/(B */
		newlen = 0;
		scandp = candp->scand;
		for (i = 0; i < candp->nscand; i++) {
			newlen += wstrlen(scandp++->kanji);
		}
		oldlen = clpe->dispp - clps->dispp;
		buflen = (buf->displayEnd - buf->displayBuf) + newlen - oldlen;
		if (buflen > buf->bufferSize &&
		    resizeBuffer(buf, buflen) < 0) {
			return -1;
		}

		/* caluseInfo $B$N%5%$%:$N%A%'%C%/(B */
		oldclen = buf->curLCEnd - buf->curLCStart;
		newclen = candp->nscand;
		if (buf->nClause + newclen - oldclen > buf->clauseSize &&
		    resizeCInfo(buf, buf->nClause + newclen - oldclen) < 0) {
			return -1;
		}

		/* $B%G%#%9%W%l%$%P%C%U%!$rF0$+$9(B */
		moveDBuf(buf, buf->curLCEnd, newlen - oldlen);

		/* clauseInfo $B$rF0$+$9(B */
		moveCInfo(buf, buf->curLCEnd, newclen - oldclen);

		/* $B8uJdJ8;zNs$r%3%T!<$7!"F1;~$K(B clauseInfo $B$r(B
		 * $B%"%C%W%G!<%H$9$k(B
		 */
		clps = buf->clauseInfo + buf->curLCStart;
		scandp = candp->scand;
		kanap = clps->kanap;
		dispp = clps->dispp;
		for (i = 0; i < candp->nscand; i++) {
			changecinfo(clps, scandp, kanap, dispp);
			clps++->ltop = (i == 0);
			kanap += scandp->kanalen;
			p = scandp++->kanji;
			while (*p)
				*dispp++ = *p++;
		}

		/* $B%+%l%s%HJ8@a$N%;%C%H(B */
		setCurClause(buf, buf->curLCStart);

		/* $B8uJdJ8@a$N%;%C%H(B
		 * $B$3$NF~$l49$($K$h$C$F!"%+%l%s%HBgJ8@a$N=*$j$,(B
		 * $B0\F0$9$k$3$H$,$"$k$N$G!"$3$3$G8uJdJ8@a$r(B
		 * $B:F@_Dj$9$k(B
		 */
		buf->candClause = buf->curLCStart;
		buf->candClauseEnd = buf->curLCEnd;
	}
	return 0;
}

/* setupCandBuf -- $B<!8uJd%P%C%U%!$K<!8uJd$N>pJs$rMQ0U$9$k(B
 *		   $B4{$K%;%C%H$5$l$F$$$l$P2?$b$7$J$$(B
 */
static int
setupCandBuf(buf, small)
jcConvBuf *buf;
int small;
{
	if (small) {
		if (buf->candKind != CAND_SMALL ||
		    buf->curClause != buf->candClause) {
			/* $B%+%l%s%H>.J8@a$N8uJd$r<h$j=P$9(B */
			buf->candClause = -1;
			if (getSCandidates(buf) < 0)
				return -1;
		}
	} else {
		if (buf->candKind != CAND_LARGE ||
		    buf->curLCStart != buf->candClause ||
		    buf->curLCEnd != buf->candClauseEnd) {
			/* $B%+%l%s%HBgJ8@a$N8uJd$r<h$j=P$9(B */
			buf->candClause = -1;
			if (getLCandidates(buf) < 0)
				return -1;
		}
	}

	return 0;
}

/* checkAndResetCandidates -- $B<!8uJd%P%C%U%!$NFbMF$,M-8z$+$I$&$+%A%'%C%/$7$F!"(B
 *			      $BI,MW$J=hM}$r9T$J$&(B
 */
static void
checkAndResetCandidates(buf, cls, cle)
jcConvBuf *buf;
int cls;
int cle;
{
	/* $BJ8@aHV9f(B cls $B$+$i(B cle - 1 $B$^$G$NJ8@a$,JQ99$5$l$k(B
	 * $B<!8uJd%P%C%U%!$K$O$$$C$F$$$k8uJdJ8@a$,$3$NCf$K4^$^$l$F$$$l$P(B
	 * $B<!8uJd%P%C%U%!$NFbMF$rL58z$K$7$J$/$F$O$J$i$J$$(B
	 *
	 * $B$I$N$h$&$J>l9g$+$H$$$&$H!"(B
	 * 1. buf->candKind $B$,(B CAND_SMALL $B$G!"(B
	 *	cls <= buf->candClause < cle
	 * 2. buf->candKind $B$,(B CAND_LARGE $B$G!"(B
	 *	buf->candClause < cle $B$+$D(B cls < buf->candClauseEnd 
	 */
	if (buf->candKind == CAND_SMALL)
		buf->candClauseEnd = buf->candClause + 1;
	if (buf->candClause < cle && cls < buf->candClauseEnd) {
		/* $BL58z$K$9$k(B */
		buf->candClause = buf->candClauseEnd = -1;
	}
}

/* addResetClause -- $B;XDj$5$l$?J8@a$NC18l$r%j%;%C%H%(%s%H%j$N%j%9%H$KF~$l$k(B */
static void
addResetClause(buf, cls, cle)
jcConvBuf *buf;
int cls;
int cle;
{
	jcClause	*clp;

	for (clp = buf->clauseInfo + cls; cls < cle; cls++, clp++) {
		if (clp->conv == 1 && clp->imabit) {
			addResetEntry(buf, clp->dicno, clp->entry);
		}
	}
}

/* addResetCandidate -- $B;XDj$5$l$?HV9f0J30$N8uJd$r%j%;%C%H%j%9%H$KF~$l$k(B */
static void
addResetCandidate(buf, n)
jcConvBuf *buf;
int n;
{
	int	i, j;
	int	ncand = buf->nCand;

	if (buf->candClause < 0) return;

	if (buf->candKind == CAND_SMALL) {
		jcSCand	*scp = (jcSCand *)buf->candBuf;
		for (i = 0; i < ncand; i++, scp++) {
			if (i != n && scp->status & IMA_BIT)
				addResetEntry(buf, scp->dicno, scp->entry);
		}
	} else {	/* CAND_LARGE */
		jcLCand	*lcp = (jcLCand *)buf->candBuf;
		for (i = 0; i < ncand; i++, lcp++) {
			jcSCand	*scp;
			if (i == n) continue;
			for (j = 0, scp = lcp->scand; j < lcp->nscand;
			     j++, scp++) {
				if (scp->status & IMA_BIT)
					addResetEntry(buf, scp->dicno,
						      scp->entry);
			}
		}
	}
}

/* addResetEntry -- $B:#;H$C$?$h%S%C%H$rMn$9BP>]$N%(%s%H%j$r%j%9%H$KIU$12C$($k(B */
static void
addResetEntry(buf, dicno, entry)
jcConvBuf *buf;
int dicno;
int entry;
{
	jcEntry	*ep;
	int	i;

	/*
	 * $B$3$N%U%!%s%/%7%g%s$r8F$V$N$O!"(B
	 *
	 * 1. jcNext() $B$,8F$P$l$?;~$K!"%+%l%s%HJ8@a$KF~$C$F$$$?8uJd(B
	 * 2. jcSelect() $B$,8F$P$l$?;~$K!"A*$P$l$J$+$C$?8uJd(B
	 * 3. jcExpand()/jcShrink() $B$,8F$P$l$?;~$K!"(B
	 *    $B%+%l%s%HJ8@a$H$=$N<!$NJ8@a$KF~$C$F$$$?8uJd(B
	 *
	 * $B$N#3<oN`$K$9$k(B
	 * 3. $B$O$A$g$C$H$d$j2a$.(B (jllib $B$O$b$C$H8-$$$3$H$r$7$F$$$k(B)
	 * $B$@$,!"$^$"Mn$5$J$$$h$j$OMn$9J}$,$$$$$N$G$O$J$$$+$H$$$&$3$H$G(B
	 */

	/* $B$9$G$K%j%9%H$K$O$$$C$F$$$J$$$+%A%'%C%/$9$k(B */
	ep = (jcEntry *)buf->resetBuf;
	i = buf->nReset;
	while (i-- > 0) {
		if (ep->entry == entry && ep->dicno == dicno) {
			/* $BF~$C$F$$$?(B */
			return;
		}
		ep++;
	}

	/* $B%P%C%U%!$NBg$-$5$N%A%'%C%/(B */
	if (buf->nReset >= buf->resetSize) {
		char	*p;
		p = realloc(buf->resetBuf,
			    (buf->resetSize + 10) * sizeof(jcEntry));
		if (p == NULL) return;
		buf->resetSize += 10;
		buf->resetBuf = p;
	}
	ep = (jcEntry *)buf->resetBuf + buf->nReset++;
	ep->dicno = dicno;
	ep->entry = entry;
}

/* saveDicAll -- $B4D6-$G;HMQ$5$l$F$$$k<-=q$r%;!<%V$9$k(B */
static void
saveDicAll(buf)
jcConvBuf *buf;
{
	int	ndic, i;
	WNN_DIC_INFO	*diclist;
	struct wnn_env	*env = buf->env;
	char	hname[256];
	int	hlen;

	if ((ndic = js_dic_list(env, &jsbuf)) < 0)
		return;

	/*
	 * $B%/%i%$%"%s%HB&$N%U%!%$%k$r%;!<%V$9$k;~$K$O!"$=$N%[%9%H$K$"$k(B
	 * $B%U%!%$%k$+$I$&$+$r(B hostname $B$G%A%'%C%/$9$k(B
	 */
	(void)gethostname(hname, sizeof(hname));
	hname[sizeof(hname) - 2] = '\0';
	(void)strcat(hname, "!");
	hlen = strlen(hname);

	diclist = (WNN_DIC_INFO *)jsbuf.buf;
	for (i = 0; i < ndic; i++) {
		/* $BEPO?2DG=7A<0$G$J$$<-=q$r%;!<%V$9$kI,MW$O$J$$$@$m$&(B */
		if (diclist->rw == WNN_DIC_RW && diclist->body >= 0) {
			/* $B<-=qK\BN$N%;!<%V(B */
			if (diclist->localf) {
				(void)js_file_write(env, diclist->body,
						    diclist->fname);
			} else if (!strncmp(diclist->fname, hname, hlen)) {
				(void)js_file_receive(env, diclist->body,
						      NULL);
			}
		}
		if (diclist->hindo >= 0) {
			/* $BIQEY%U%!%$%k(B */
			if (diclist->hlocalf) {
				(void)js_file_write(env, diclist->hindo,
						    diclist->hfname);
			} else if (!strncmp(diclist->hfname, hname, hlen)) {
				(void)js_file_receive(env, diclist->hindo,
						      NULL);
			}
		}
		diclist++;
	}
}

/*
 *	$B$3$3$+$i(B Public $B$J%U%!%s%/%7%g%s(B
 */

/* jcCreateBuf -- $BJQ49%P%C%U%!$N:n@.(B */
jcConvBuf *
jcCreateBuffer(env, nclause, buffersize)
struct wnn_env *env;
int nclause;
int buffersize;
{
	jcConvBuf	*buf;

	/* $B$^$:(B jcConvBuf $B$N3NJ](B */
	if ((buf = (jcConvBuf *)malloc(sizeof(jcConvBuf))) == NULL) {
		jcErrno = JE_NOCORE;
		return NULL;
	}
	(void)bzero((char *)buf, sizeof(jcConvBuf));
	buf->env = env;

	/* $B<!$K3F<o%P%C%U%!$N3NJ](B */

	/* $B$^$:!"$+$J%P%C%U%!$H%G%#%9%W%l%$%P%C%U%!(B */
	buf->bufferSize = (buffersize <= 0) ? DEF_BUFFERSIZE : buffersize;
	/* $B%P%C%U%!$N:G8e$r(B NULL $B%?!<%_%M!<%H$9$k$3$H$,$"$k$N$G!"(B
	 * 1$BJ8;zJ8Bg$-$/$7$F$*$/(B
	 */
	buf->kanaBuf = (wchar *)malloc((buf->bufferSize + 1) *
					 sizeof(wchar));
	buf->displayBuf = (wchar *)malloc((buf->bufferSize + 1) *
					    sizeof(wchar));

	/* $B<!$K(B clauseInfo $B%P%C%U%!(B */
	buf->clauseSize = (nclause <= 0) ? DEF_CLAUSESIZE : nclause;
	/* clauseInfo $B%P%C%U%!$O(B nclause + 1 $B8D%"%m%1!<%H$9$k(B
	 * $B$J$<$+$H$$$&$H(B clauseinfo $B$O%G%j%_%?$H$7$FMWAG$r(B
	 * 1$B8D;H$&$N$G(B nclause $B8D$NJ8@a$r07$&$?$a$K$O(B nclause + 1 $B8D$N(B
	 * $BBg$-$5$r;}$?$J$1$l$P$J$i$J$$$+$i$G$"$k(B
	 */
	buf->clauseInfo = (jcClause *)malloc((buf->clauseSize + 1)
					     * sizeof(jcClause));

	/* $B<!8uJd%P%C%U%!(B */
	buf->candSize = DEF_CANDSIZE;
	buf->candBuf = (char *)malloc(buf->candSize);

	/* $B%j%;%C%H%P%C%U%!(B */
	buf->resetSize = DEF_RESETSIZE;
	buf->resetBuf = (char *)malloc(buf->resetSize * sizeof(jcEntry));

	if (buf->kanaBuf == NULL || buf->displayBuf == NULL ||
	    buf->clauseInfo == NULL || buf->candBuf == NULL ||
	    buf->resetBuf == NULL) {
		/* malloc() $B$G$-$J$+$C$?(B */
		Free(buf->kanaBuf);
		Free(buf->displayBuf);
		Free(buf->clauseInfo);
		Free(buf->candBuf);
		Free(buf->resetBuf);
		Free(buf);
		jcErrno = JE_NOCORE;
		return NULL;
	}

	(void)jcClear(buf);
	return buf;
}

/* jcDestroyBuffer -- $BJQ49%P%C%U%!$N>C5n(B */
int
jcDestroyBuffer(buf, savedic)
jcConvBuf *buf;
int savedic;
{
	/* $B%"%m%1!<%H$7$?%a%b%j$N2rJ|(B */
	Free(buf->kanaBuf);
	Free(buf->displayBuf);
	Free(buf->clauseInfo);
	Free(buf->candBuf);
	Free(buf->resetBuf);

	/* savedic $B$,(B 0 $B$G$J$1$l$P!"4D6-$K%m!<%I$5$l$F$$$kA4$F$N%U%!%$%k$r(B
	 * save $B$9$k(B
	 */
	if (savedic)
		saveDicAll(buf);

	Free(buf);

	return 0;
}

/* jcClear -- jclib $B$N%/%j%"(B $B?7$?$JJQ49$r;O$a$kKh$K8F$P$J$1$l$P$J$i$J$$(B */
int
jcClear(buf)
jcConvBuf *buf;
{
	/* $B=i4|CM$N@_Dj(B */
	buf->nClause = buf->curClause = buf->curLCStart = 0;
	buf->curLCEnd = 1;
	buf->kanaEnd = buf->kanaBuf;
	buf->displayEnd = buf->displayBuf;
	buf->clauseInfo[0].kanap = buf->kanaBuf;
	buf->clauseInfo[0].fzkp = buf->kanaBuf;
	buf->clauseInfo[0].dispp = buf->displayBuf;
	buf->clauseInfo[0].conv = 0;
	buf->clauseInfo[0].ltop = 1;
	buf->dot = buf->kanaBuf;
	buf->fixed = 0;
	buf->candClause = buf->candClauseEnd = -1;
	buf->nReset = 0;
	jcErrno = JE_NOERROR;

	return 0;
}

/* jcConvert -- $B%+%l%s%HJ8@a0J9_$r$+$J4A;zJQ49$9$k(B */
int
jcConvert(buf, small, tan, jump)
jcConvBuf *buf;
int small;
int tan;
int jump;
{
	int	ret;

	CHECKFIXED(buf);

	if (buf->curClause == buf->nClause) {
		/* $B%+%l%s%HJ8@a$,:G8e$NJ8@a$G$7$+$b6u(B */
		jcErrno = JE_CLAUSEEMPTY;
		return -1;
	}

	/* $B$b$7<!8uJd%P%C%U%!$NFbMF$,%+%l%s%HJ8@a0J9_$@$HL50UL#$K$J$k(B */
	checkAndResetCandidates(buf,
				small ? buf->curClause : buf->curLCStart,
				buf->nClause);

	if (tan) {
		ret = tanConvert(buf, small);
	} else {
		ret = renConvert(buf, small);
	}

	if (ret < 0)
		return ret;

	if (jump) {
		/* $B%I%C%H$H%+%l%s%HJ8@a$rJ8$N:G8e$K0\F0$5$;$k(B */
		buf->curClause = buf->curLCStart = buf->nClause;
		buf->curLCEnd = buf->nClause + 1;
		buf->dot = buf->kanaEnd;
	}
	return 0;
}

/* jcUnconvert -- $B%+%l%s%HBgJ8@a$rL5JQ49$N>uBV$KLa$9(B */
int
jcUnconvert(buf)
jcConvBuf *buf;
{
	jcClause	*clp = buf->clauseInfo + buf->curClause;

	CHECKFIXED(buf);

	if (buf->curClause == buf->nClause) {
		/* $B%+%l%s%HJ8@a$,:G8e$NJ8@a$G$7$+$b6u(B */
		jcErrno = JE_CLAUSEEMPTY;
		return -1;
	}

	if (!clp->conv) {
		/* $B%+%l%s%HJ8@a$OJQ49$5$l$F$$$J$$(B */
		/* $BL5JQ49$NJ8@a$O(B jclib $BFbIt$G$O>o$KBgJ8@a$H$7$F(B
		 * $B07$o$l$k$N$G!"%+%l%s%H>.J8@a$NJQ49>uBV$r8+$F!"(B
		 * $B$=$l$,JQ49>uBV$J$i%+%l%s%HBgJ8@aFb$N(B
		 * $BA4$F$N>.J8@a$OJQ49>uBV!"$=$&$G$J$1$l$PL5JQ49>uBV!"(B
		 * $B$K$J$k(B
		 */
		jcErrno = JE_NOTCONVERTED;
		return -1;
	}

	checkAndResetCandidates(buf, buf->curLCStart, buf->curLCEnd);

	if (unconvert(buf, buf->curLCStart, buf->curLCEnd) < 0)
		return -1;

	/* $BBgJ8@a$N@_Dj(B */
	clp = buf->clauseInfo + buf->curLCStart;
	clp->ltop = 1;
	(++clp)->ltop = 1;

	/* $B%+%l%s%HJ8@a$N:F@_Dj(B */
	buf->curClause = buf->curLCStart;
	buf->curLCEnd = buf->curLCStart + 1;

	/* $B%I%C%H$N@_Dj(B */
	DotSet(buf);

	return 0;
}

/* jcExpand -- $B%+%l%s%HJ8@a$r#1J8;z9-$2$k(B */
int
jcExpand(buf, small, convf)
jcConvBuf *buf;
int small;
int convf;
{
	CHECKFIXED(buf);

	return expandOrShrink(buf, small, 1, convf);
}

/* jcShrink -- $B%+%l%s%HJ8@a$r#1J8;z=L$a$k(B */
int
jcShrink(buf, small, convf)
jcConvBuf *buf;
int small;
int convf;
{
	CHECKFIXED(buf);

	return expandOrShrink(buf, small, 0, convf);
}

/* jcKana -- $B%+%l%s%HJ8@a$r$+$J$K$9$k(B */
int
jcKana(buf, small, kind)
jcConvBuf *buf;
int small;
int kind;
{
	jcClause	*clp;
	wchar		*kanap, *kanaendp, *dispp;
	int		start, end;
	int		conv;
	int		c;

	CHECKFIXED(buf);

	/* $BJ8@aHV9f$N%A%'%C%/(B */
	if (buf->curClause >= buf->nClause) {
		/* $B%+%l%s%HJ8@a$,:G8e$NJ8@a$G$7$+$b6u$@$C$?>l9g(B
		 * $B$3$N>l9g%(%i!<$K$7$F$b$h$$$,(B...
		 */
		return 0;
	}

	/*
	 * $B%+%l%s%HJ8@a$,JQ49$5$l$F$$$l$P$$$C$?$sL5JQ49$K$9$k(B
	 */

	/* $B$"$H$GJQ49>uBV$r$b$H$KLa$9$?$a!"JQ49>uBV$r%;!<%V$7$F$*$/(B */
	conv = buf->clauseInfo[buf->curClause].conv;

	if (small) {
		start = buf->curClause;
		end = start + 1;
	} else {
		start = buf->curLCStart;
		end = buf->curLCEnd;
	}

	/* $B<!8uJd%P%C%U%!$NFbMF$N%A%'%C%/(B */
	checkAndResetCandidates(buf, start, end);

	if (unconvert(buf, start, end) < 0) {
		return -1;
	}

	/* small $B$,(B 0$B!"$D$^$j%+%l%s%HJ8@a$H$7$FBgJ8@a$rA*Br$7$?>l9g!"(B
	 * $B$=$NCf$N>.J8@a$O0l$D$K$^$H$a$i$l$k$N$G!"(BcurClause $B$H(B
	 * curLCEnd $B$rJQ$($kI,MW$,$"$k(B
	 */
	if (!small) {
		buf->curClause = buf->curLCStart;
		buf->curLCEnd = buf->curLCStart + 1;
	}

	/* $B$+$JJQ49$9$k(B */
	/* $B%G%#%9%W%l%$%P%C%U%!$@$1$G$O$J$/!"$+$J%P%C%U%!$bJQ49$9$k(B */
	/* $B$3$l$K$O$5$7$?$kM}M3$O$J$$$,!"$^$"!"(BVer3 $BHG$N(B jclib $B$,(B
	 * $B$=$&$@$C$?$N$G!D(B
	 */
	clp = buf->clauseInfo + buf->curClause;
	kanap = clp->kanap;
	kanaendp = (clp + 1)->kanap;
	dispp = clp->dispp;

	if (kind == JC_HIRAGANA) {	/* $B%+%?%+%J"*$R$i$,$J(B */
		/* $B%+%?%+%J$r$R$i$,$J$KJQ49$9$k:]$K$O$R$i$,$J$K$J$$;z(B
		 * "$B%t%u%v(B" $B$,$"$k$N$G$$$-$*$$$GJQ49$7$F$7$^$o$J$$$h$&$K(B
		 * $B5$$rIU$1$J$1$l$P$J$i$J$$(B
		 * ($B$^$"<B:]$O5$$r$D$1$k$H$$$&$[$I$N$b$N$G$O$J$$$,(B)
		 */
		while (kanap < kanaendp) {
			c = *kanap;
			if ((KANABEG + KATAOFFSET) <= c &&
					c <= (KANAEND + KATAOFFSET)) {
				*kanap = *dispp = c - KATAOFFSET;
			}
			kanap++, dispp++;
		}
	} else {	/* $B$R$i$,$J"*%+%?%+%J(B */
		while (kanap < kanaendp) {
			c = *kanap;
			if (KANABEG <= c && c <= KANAEND) {
				*kanap = *dispp = c + KATAOFFSET;
			}
			kanap++, dispp++;
		}
	}

	/*
	 * $BJQ49>uBV$r$b$H$KLa$7$F$*$/(B
	 */

	/* $B$H$O$$$C$F$b4{$KJQ49$5$l$?J8@a$N>l9g!"$3$l$NIQEY>pJs$r(B
	 * $B%5!<%P$KAw$k$H$^$:$$$N$G!"$"$H$G$+$JJQ49$7$?$3$H$,$o$+$k$h$&$K(B
	 * jcClause.conv $B$O(B -1 $B$K%;%C%H$9$k(B
	 */
	if (conv) {
		clp->conv = -1;
		clp->hinshi = WNN_ALL_HINSI;
		clp->fzkp = (clp + 1)->kanap;
	} else {
		clp->conv = 0;
	}
	return 0;
}

/* jcFix -- $B3NDj$9$k(B */
int
jcFix(buf)
jcConvBuf *buf;
{
	struct wnn_env	*env = buf->env;
	jcEntry		*ep;
	jcClause	*clp;
	int		i;

	if (buf->fixed) {
		/* $B4{$K3NDj$5$l$F$$$k(B
		 * $B%(%i!<$K$7$F$b$h$$$,!D(B
		 */
		return 0;
	}

	/* $BIQEY>pJs$r%;%C%H$9$k(B */

	/* $B$^$:$O:#;H$C$?$h%S%C%H$rMn$9(B */
	ep = (jcEntry *)buf->resetBuf;
	for (i = 0; i < buf->nReset; i++, ep++) {
		if (ep->dicno >= 0) {
			(void)js_hindo_set(env, ep->dicno, ep->entry,
					   WNN_IMA_OFF, WNN_HINDO_NOP);
		}
	}
	buf->nReset = 0;

	/* $B<!$K!"IQEY$r>e$2!":#;H$C$?$h%S%C%H$rN)$F$k(B */
	clp = buf->clauseInfo;
	for (i = 0; i < buf->nClause; i++) {
		/* $BJQ49%U%i%0(B clp->conv $B$O(B -1, 0, 1 $B$N#3<oN`$NCM$r$H$k(B
		 * $B$3$N$&$A!"(B-1 $B$O(B jclib $B$K$h$k5?;wJ8@a!"(B0 $B$OL$JQ49$N(B
		 * $BJ8@a$J$N$G!"$3$l$i$K4X$7$F$OL5;k$9$k(B
		 */
		if (clp->conv == 1) {
			(void)js_hindo_set(buf->env, clp->dicno, clp->entry,
					   WNN_IMA_ON, WNN_HINDO_INC);
		}
		clp++;
	}

	/* $B3NDj%U%i%0$rN)$F$k(B */
	buf->fixed = 1;

	return 0;
}

/* jcNext -- $B%+%l%s%HJ8@a$r<!8uJd(B/$BA08uJd$GCV$-49$($k(B */
int
jcNext(buf, small, prev)
jcConvBuf *buf;
int small;
int prev;
{
	int	curcand;

	CHECKFIXED(buf);

	if (!buf->clauseInfo[buf->curClause].conv) {
		/* $B$^$@JQ49$5$l$F$$$J$$(B */
		jcErrno = JE_NOTCONVERTED;
		return -1;
	}

	/* $B:#;H$o$l$F$$$kC18l$r%j%;%C%H%P%C%U%!$K2C$($k(B */
	if (small) {
		addResetClause(buf, buf->curClause, buf->curClause + 1);
	} else {
		addResetClause(buf, buf->curLCStart, buf->curLCEnd);
	}

	/* $B<!8uJd$N>pJs$r<!8uJd%P%C%U%!$KMQ0U$9$k(B */
	if (setupCandBuf(buf, small) < 0)
		return -1;

	if (buf->nCand == 1) {
		/* $B<!8uJd$,$J$$(B */
		jcErrno = JE_NOCANDIDATE;
		return -1;
	}

	if (prev) {
		curcand = buf->curCand - 1;
		if (curcand < 0)
			curcand = buf->nCand - 1;
	} else {
		curcand = buf->curCand + 1;
		if (curcand >= buf->nCand)
			curcand = 0;
	}

	/* $B%G%#%9%W%l%$%P%C%U%!$rJQ99(B */
	if (changeCand(buf, curcand) < 0)
		return -1;

	buf->curCand = curcand;

	return 0;
}

/* jcCandidateInfo -- $B<!8uJd$N?t$H8=:_$N8uJdHV9f$rD4$Y$k(B
 *		      $B$b$7<!8uJd$,$^$@%P%C%U%!$KF~$C$F$$$J$1$l$PMQ0U$9$k(B
 */
int
jcCandidateInfo(buf, small, ncandp, curcandp)
jcConvBuf *buf;
int small;
int *ncandp;
int *curcandp;
{
	CHECKFIXED(buf);

	if (!buf->clauseInfo[buf->curClause].conv) {
		/* $B$^$@JQ49$5$l$F$$$J$$(B */
		jcErrno = JE_NOTCONVERTED;
		return -1;
	}

	/* $B<!8uJd$N>pJs$r<!8uJd%P%C%U%!$KMQ0U$9$k(B */
	if (setupCandBuf(buf, small) < 0)
		return -1;

	*ncandp = buf->nCand;
	*curcandp = buf->curCand;

	return 0;
}

/* jcGetCandidate -- $B;XDj$5$l$?HV9f$N8uJd$r<h$j=P$9(B */
int
jcGetCandidate(buf, n, candstr)
jcConvBuf *buf;
int n;
wchar *candstr;
{
	int	ns;
	wchar	*p;
	jcLCand *lcandp;
	jcSCand *scandp;

	CHECKFIXED(buf);

	/* $BJ8@a$N%A%'%C%/(B */
	if (buf->candClause < 0) {
		jcErrno = JE_NOCANDIDATE;
		return -1;
	}

	/* $B8uJdHV9f$N%A%'%C%/(B */
	if (n < 0 || n >= buf->nCand) {
		jcErrno = JE_NOSUCHCANDIDATE;
		return -1;
	}

	/* $BJ8;zNs$r%3%T!<(B */
	if (buf->candKind == CAND_SMALL) {
		scandp = (jcSCand *)buf->candBuf;
		p = scandp[n].kanji;
		while (*candstr++ = *p++)
			;
	} else {
		lcandp = (jcLCand *)buf->candBuf;
		scandp = lcandp[n].scand;
		ns = lcandp[n].nscand;
		while (ns-- > 0) {
			p = scandp++->kanji;
			while (*candstr++ = *p++)
				;
			candstr--;
		}
	}

	return 0;
}

/* jcSelect -- $B%G%#%9%W%l%$%P%C%U%!$r;XDj$5$l$?8uJd$HCV$-49$($k(B */
int
jcSelect(buf, n)
jcConvBuf *buf;
int n;
{
	CHECKFIXED(buf);

	/* $BJ8@a$N%A%'%C%/(B */
	if (buf->candClause < 0) {
		jcErrno = JE_NOCANDIDATE;
		return -1;
	}

	/* $B8uJdHV9f$N%A%'%C%/(B */
	if (n < 0 || n >= buf->nCand) {
		jcErrno = JE_NOSUCHCANDIDATE;
		return -1;
	}

	/* $B;XDj$5$l$?8uJd0J30$N$b$N$r%j%;%C%H%j%9%H$KF~$l$k(B */
	addResetCandidate(buf, n);

	/* $B%G%#%9%W%l%$%P%C%U%!$rJQ99(B */
	if (changeCand(buf, n) < 0)
		return -1;

	/* CurCand $B$NJQ99(B */
	buf->curCand = n;

	return 0;
}

/* jcDotOffset -- $BBgJ8@a$N@hF,$+$i$N%I%C%H$N%*%U%;%C%H$rJV$9(B */
int
jcDotOffset(buf)
jcConvBuf *buf;
{
	return buf->dot - buf->clauseInfo[buf->curLCStart].kanap;
}

/* jcIsConverted -- $B;XDj$5$l$?J8@a$,JQ49$5$l$F$$$k$+$I$&$+$rJV$9(B */
int
jcIsConverted(buf, cl)
jcConvBuf *buf;
int cl;
{
	if (cl < 0 || cl > buf->nClause) {
		/* cl == jcNClause $B$N$H$-$r%(%i!<$K$7$F$b$$$$$N$@$1$l$I(B
		 * $B%+%l%s%HJ8@a$,(B jcNClause $B$N$H$-$,$"$k$N$G(B
		 * $B%(%i!<$H$O$7$J$$$3$H$K$7$?(B
		 */
		return -1;
	}
	return (buf->clauseInfo[cl].conv != 0);
}

/* jcMove -- $B%I%C%H!&%+%l%s%HJ8@a$r0\F0$9$k(B */
int
jcMove(buf, small, dir)
jcConvBuf *buf;
int small;
int dir;
{
	jcClause	*clp = buf->clauseInfo + buf->curClause;
	int		i;

	if (!clp->conv) {
		/* $B%+%l%s%HJ8@a$,JQ49$5$l$F$$$J$$$N$G!"%I%C%H$N0\F0$K$J$k(B */
		if (dir == JC_FORWARD) {
			if (buf->curClause == buf->nClause) {
				/* $B$9$G$K0lHV:G8e$K$$$k(B */
				jcErrno = JE_CANTMOVE;
				return -1;
			} else if (buf->dot == (clp + 1)->kanap) {
				/* $B%I%C%H$,%+%l%s%HJ8@a$N:G8e$K$"$k$N$G(B
				 * $BJ8@a0\F0$9$k(B
				 */
				goto clausemove;
			} else {
				buf->dot++;
			}
		} else {
			if (buf->dot == clp->kanap) {
				/* $B%I%C%H$,%+%l%s%HJ8@a$N@hF,$K$"$k$N$G(B
				 * $BJ8@a0\F0$9$k(B
				 */
				goto clausemove;
			} else
				buf->dot--;
		}
		return 0;
	}

clausemove:	/* $BJ8@a0\F0(B */
	clp = buf->clauseInfo;

	if (small) {
		/* $B>.J8@aC10L$N0\F0(B */
		if (dir == JC_FORWARD) {
			if (buf->curClause == buf->nClause) {
				jcErrno = JE_CANTMOVE;
				return -1;
			}
			buf->curClause++;
			if (buf->curClause >= buf->curLCEnd) {
				/* $BBgJ8@a$b0\F0$9$k(B */
				buf->curLCStart = buf->curLCEnd;
				for (i = buf->curLCStart + 1;
				     i <= buf->nClause && !clp[i].ltop; i++)
					;
				buf->curLCEnd = i;
			}
		} else {	/* JC_BACKWARD */
			if (buf->curClause == 0) {
				jcErrno = JE_CANTMOVE;
				return -1;
			}
			buf->curClause--;
			if (buf->curClause < buf->curLCStart) {
				/* $BBgJ8@a$b0\F0$9$k(B */
				buf->curLCEnd = buf->curLCStart;
				for (i = buf->curClause; !clp[i].ltop; i--)
					;
				buf->curLCStart = i;
			}
		}
	} else {
		/* $BBgJ8@aC10L$N0\F0(B */
		if (dir == JC_FORWARD) {
			if (buf->curLCStart == buf->nClause) {
				jcErrno = JE_CANTMOVE;
				return -1;
			}
			buf->curLCStart = buf->curClause = buf->curLCEnd;
			for (i = buf->curLCStart + 1;
			     i <= buf->nClause && !clp[i].ltop; i++)
				;
			buf->curLCEnd = i;
		} else {
			if (buf->curLCStart == 0) {
				jcErrno = JE_CANTMOVE;
				return -1;
			}
			buf->curLCEnd = buf->curLCStart;
			for (i = buf->curLCEnd - 1; !clp[i].ltop; i--)
				;
			buf->curLCStart = buf->curClause = i;
		}
	}

	/* $BJ8@a0\F0$7$?$i%I%C%H$O$=$NJ8@a$N@hF,$K0\F0$9$k(B */
	buf->dot = clp[buf->curClause].kanap;

	return 0;
}

/* jcTop -- $B%I%C%H!&%+%l%s%HJ8@a$rJ8$N@hF,$K0\F0$9$k(B */
int
jcTop(buf)
jcConvBuf *buf;
{
	/* $B%+%l%s%HJ8@a$r(B 0 $B$K$7$F%I%C%H$r@hF,$K;}$C$F$/$k(B */
	setCurClause(buf, 0);
	buf->dot = buf->kanaBuf;

	return 0;
}

/* jcBottom -- $B%I%C%H!&%+%l%s%HJ8@a$rJ8$N:G8e$K0\F0$9$k(B */
int
jcBottom(buf)
jcConvBuf *buf;
{
	/*
	 * Ver3 $BBP1~$N(B jclib $B$G$O!"%+%l%s%HJ8@a$r(B jcNClause $B$K$7$F(B
	 * $B%I%C%H$r:G8e$K;}$C$F$/$k$@$1$@$C$?(B
	 * $B$3$l$@$H!":G8e$NJ8@a$K$+$J$rF~$l$F$$$F!"%+!<%=%k$rF0$+$7$F(B
	 * jcBottom() $B$G85$KLa$C$F:F$S$+$J$rF~$l$k$H!"JL$NJ8@a$K(B
	 * $B$J$C$F$7$^$&(B
	 * $B$=$3$G!":G8e$NJ8@a$,L5JQ49>uBV$N;~$K$O!"%+%l%s%HJ8@a$O(B
	 * buf->nClause $B$G$O$J$/!"(Bbuf->nClause - 1 $B$K$9$k$3$H$K$9$k(B
	 */
	if (buf->nClause > 0 && !buf->clauseInfo[buf->nClause - 1].conv) {
		buf->curClause = buf->curLCStart = buf->nClause - 1;
		buf->curLCEnd = buf->nClause;
	} else {
		buf->curClause = buf->curLCStart = buf->nClause;
		buf->curLCEnd = buf->nClause + 1;
	}
	buf->dot = buf->kanaEnd;
	return 0;
}

/* jcInsertChar -- $B%I%C%H$N0LCV$K0lJ8;zA^F~$9$k(B */
int
jcInsertChar(buf, c)
jcConvBuf *buf;
int c;
{
	jcClause	*clp;
	wchar	*dot, *dispdot;
	int	ksizenew, dsizenew;

	CHECKFIXED(buf);

	/* $B8uJdJ8@a$,%+%l%s%HJ8@a$K$"$l$PL58z$K$9$k(B */
	checkAndResetCandidates(buf, buf->curLCStart, buf->curLCEnd);

	/*
	 * $B!&%+%l%s%HJ8@aHV9f$,(B buf->nClause $B$G$"$k>l9g(B
	 *	- $B$3$l$O%I%C%H$,:G8e$NJ8@a$N<!$K$"$k$H$$$&$3$H$J$N$G(B
	 *	  $B?7$7$$J8@a$r:n$k(B
	 * $B!&JQ49:Q$_$NJ8@a$N>l9g(B
	 *	- $BL5JQ49$N>uBV$KLa$7$F$+$iA^F~(B
	 * $B!&$=$NB>(B
	 *	- $BC1$KA^F~$9$l$P$h$$(B
	 */
	clp = buf->clauseInfo + buf->curLCStart;
	if (buf->curLCStart == buf->nClause) {
		/* $B?7$?$KJ8@a$r:n$k(B */
		/* clauseInfo $B$N%5%$%:$N%A%'%C%/(B */
		if (buf->nClause >= buf->clauseSize &&
		    resizeCInfo(buf, buf->nClause + 1) < 0) {
			return -1;
		}
		/* buf->nClause $B$N%"%C%W%G!<%H$H(B clauseInfo $B$N@_Dj(B */
		clp = buf->clauseInfo + ++(buf->nClause);
		clp->conv = 0;
		clp->ltop = 1;
		clp->kanap = buf->kanaEnd;
		clp->fzkp = buf->kanaEnd;
		clp->dispp = buf->displayEnd;
	} else if (clp->conv) {
		/* $BL5JQ49>uBV$K$9$k(B */
		if (unconvert(buf, buf->curLCStart, buf->curLCEnd) < 0)
			return -1;
		buf->curClause = buf->curLCStart;
		buf->curLCEnd = buf->curLCStart + 1;
		DotSet(buf);
	}

	clp = buf->clauseInfo + buf->curLCStart;

	/* $B%P%C%U%!$NBg$-$5$N%A%'%C%/(B */
	ksizenew = (buf->kanaEnd - buf->kanaBuf) + 1;
	dsizenew = (buf->displayEnd - buf->displayBuf) + 1;
	if ((ksizenew > buf->bufferSize || dsizenew > buf->bufferSize) &&
	    resizeBuffer(buf, ksizenew > dsizenew ? ksizenew : dsizenew) < 0) {
		    return -1;
	}

	/* $B$+$J%P%C%U%!$r%"%C%W%G!<%H(B */
	dot = buf->dot;
	/* $B%+%l%s%HJ8@a$N8e$m$r0lJ8;z$:$i$9(B */
	moveKBuf(buf, buf->curLCStart + 1, 1);
	/* $B%+%l%s%HJ8@aFb$N%I%C%H0J9_$r0lJ8;z$:$i$9(B */
	(void)bcopy((char *)dot, (char *)(dot + 1),
		    ((clp + 1)->kanap - dot) * sizeof(wchar));
	/* $BA^F~(B */
	*dot = c;

	/* $B%G%#%9%W%l%$%P%C%U%!$r%"%C%W%G!<%H(B */
	dispdot = clp->dispp + (dot - clp->kanap);
	/* $B%+%l%s%HJ8@a$N8e$m$r0lJ8;z$:$i$9(B */
	moveDBuf(buf, buf->curLCStart + 1, 1);
	/* $B%+%l%s%HJ8@aFb$N%I%C%H0J9_$r0lJ8;z$:$i$9(B */
	(void)bcopy((char *)dispdot, (char *)(dispdot + 1),
	      ((clp + 1)->dispp - dispdot) * sizeof(wchar));
	/* $BA^F~(B */
	*dispdot = c;

	/* $B%I%C%H$r99?7(B */
	buf->dot++;

	return 0;
}

/* jcDeleteChar -- $B%I%C%H$NA0$^$?$O8e$m$N0lJ8;z$r:o=|$9$k(B */
int
jcDeleteChar(buf, prev)
jcConvBuf *buf;
int prev;
{
	jcClause	*clp;
	wchar		*dot, *dispdot;

	CHECKFIXED(buf);

	clp = buf->clauseInfo;
	if (buf->nClause == 0) {
		/* $BJ8@a?t$,(B 0$B!"$D$^$j2?$bF~$C$F$$$J$$;~(B:
		 *	- $B%(%i!<(B
		 */
		jcErrno = JE_CANTDELETE;
		return -1;
	} else if (buf->curClause >= buf->nClause) {
		/* $B%+%l%s%HJ8@a$,:G8e$NJ8@a$N<!$K$"$k;~(B:
		 *	- prev $B$G$"$l$P!"A0$NJ8@a$N:G8e$NJ8;z$r:o=|(B
		 *	  $B%+%l%s%HJ8@a$OA0$NJ8@a$K0\F0$9$k(B
		 *	  $BI,MW$J$i$PA0$NJ8@a$rL5JQ49$KLa$7$F$+$i:o=|$9$k(B
		 *	  $BA0$NJ8@a$,$J$$$3$H$O$"$jF@$J$$(B
		 *	- !prev $B$J$i$P%(%i!<(B
		 */
		if (!prev) {
			jcErrno = JE_CANTDELETE;
			return -1;
		}
		(void)jcMove(buf, 0, JC_BACKWARD);
	} else if (clp[buf->curLCStart].conv) {
		/* $B%+%l%s%HJ8@a$,JQ49$5$l$F$$$k;~(B:
		 *	- prev $B$G$"$l$PA0$NJ8@a$N:G8e$NJ8;z$r:o=|(B
		 *	  $B%+%l%s%HJ8@a$OA0$NJ8@a$K0\F0$9$k(B
		 *	  $BI,MW$J$i$PA0$NJ8@a$rL5JQ49$KLa$7$F$+$i:o=|$9$k(B
		 *	  $B%+%l%s%HJ8@a$,@hF,$J$i$P%(%i!<(B
		 *	- !prev $B$J$i%+%l%s%HJ8@a$rL5JQ49$KLa$7$F!"J8@a$N(B
		 *	  $B:G=i$NJ8;z$r:o=|(B
		 */
		if (prev) {
			if (buf->curLCStart == 0) {
				jcErrno = JE_CANTDELETE;
				return -1;
			}
			(void)jcMove(buf, 0, JC_BACKWARD);
		}
	} else {
		/* $B%+%l%s%HJ8@a$,JQ49$5$l$F$$$J$$;~(B:
		 *	- prev $B$G$"$l$P%I%C%H$NA0$NJ8;z$r:o=|(B
		 *	  $B$?$@$7%I%C%H$,J8@a$N@hF,$K$"$l$PA0$NJ8@a$N(B
		 *	  $B:G8e$NJ8;z$r:o=|(B
		 *	  $B$=$N;~$K$O%+%l%s%HJ8@a$OA0$NJ8@a$K0\F0$9$k(B
		 *	  $BI,MW$J$i$PA0$NJ8@a$rL5JQ49$KLa$7$F$+$i:o=|$9$k(B
		 *	  $B%+%l%s%HJ8@a$,@hF,$J$i$P%(%i!<(B
		 *	- !prev $B$J$i%I%C%H$N<!$NJ8;z$r:o=|(B
		 *	  $B%I%C%H$,J8@a$N:G8e$NJ8;z$N<!$K$"$l$P%(%i!<(B
		 */
		if (prev) {
			if (buf->dot == clp[buf->curLCStart].kanap) {
				if (buf->curLCStart == 0) {
					jcErrno = JE_CANTDELETE;
					return -1;
				}
				(void)jcMove(buf, 0, JC_BACKWARD);
			}
		} else {
			if (buf->dot == clp[buf->curLCEnd].kanap) {
				jcErrno = JE_CANTDELETE;
				return -1;
			}
		}
	}

	/* $B8uJdJ8@a$,%+%l%s%HJ8@a$K$"$l$PL58z$K$9$k(B */
	checkAndResetCandidates(buf, buf->curLCStart, buf->curLCEnd);

	if (buf->clauseInfo[buf->curLCStart].conv) {
		/* $B%+%l%s%HJ8@a$,JQ49:Q$_$G$"$l$PL5JQ49$KLa$9(B */
		if (jcUnconvert(buf) < 0)
			return -1;
		/* prev $B$G$"$l$PJ8@a$N:G8e$NJ8;z!"$=$&$G$J$1$l$PJ8@a$N(B
		 * $B@hF,$NJ8;z$r:o=|$9$k(B
		 */
		if (prev) {
			buf->dot = buf->clauseInfo[buf->curLCEnd].kanap - 1;
		} else {
			buf->dot = buf->clauseInfo[buf->curLCStart].kanap;
		}
	} else {
		/* prev $B$J$i%I%C%H$r#1J8;zLa$7$F$*$/(B
		 * $B$3$&$9$l$P%I%C%H$N8e$m$NJ8;z$r:o=|$9$k$3$H$K$J$k(B
		 * $B:o=|$7=*$o$C$?$H$-$K%I%C%H$rF0$+$9I,MW$b$J$$(B
		 */
		if (prev)
			buf->dot--;
	}

	clp = buf->clauseInfo + buf->curLCStart;

	/* $B$+$J%P%C%U%!$r%"%C%W%G!<%H(B */
	dot = buf->dot;
	/* $B%+%l%s%HJ8@aFb$N%I%C%H0J9_$r0lJ8;z$:$i$9(B */
	(void)bcopy((char *)(dot + 1), (char *)dot,
		    ((clp + 1)->kanap - (dot + 1)) * sizeof(wchar));
	/* $B%+%l%s%HJ8@a$N8e$m$r0lJ8;z$:$i$9(B */
	moveKBuf(buf, buf->curLCEnd, -1);

	/* $B%G%#%9%W%l%$%P%C%U%!$r%"%C%W%G!<%H(B */
	dispdot = clp->dispp + (dot - clp->kanap);
	/* $B%+%l%s%HJ8@aFb$N%I%C%H0J9_$r0lJ8;z$:$i$9(B */
	(void)bcopy((char *)(dispdot + 1), (char *)dispdot,
		   ((clp + 1)->dispp - (dispdot + 1)) * sizeof(wchar));
	/* $B%+%l%s%HJ8@a$N8e$m$r0lJ8;z$:$i$9(B */
	moveDBuf(buf, buf->curLCEnd, -1);

	/* $B%+%l%s%HJ8@a$ND9$5$,#1$@$C$?>l9g$K$OJ8@a$,#18:$k$3$H$K$J$k(B */
	if (clp->kanap == (clp + 1)->kanap) {
		/* $BJ8@a$,$J$/$J$C$F$7$^$C$?(B */
		moveCInfo(buf, buf->curLCEnd, -1);
		setCurClause(buf, buf->curLCStart);
		DotSet(buf);
	}

	return 0;
}

/* jcChangeClause -- $B%+%l%s%HBgJ8@a$r;XDj$5$l$?J8;zNs$GCV$-49$($k(B */
int
jcChangeClause(buf, str)
jcConvBuf *buf;
wchar *str;
{
	jcClause	*clps, *clpe;
	wchar	*p;
	int	newlen;
	int	oklen, odlen;
	int	ksize, dsize;

	CHECKFIXED(buf);

	checkAndResetCandidates(buf, buf->curLCStart, buf->curLCEnd);

	clps = buf->clauseInfo + buf->curLCStart;
	clpe = buf->clauseInfo + buf->curLCEnd;

	newlen = 0;
	p = str;
	while (*p++)
		newlen++;

	/* $B$+$J%P%C%U%!$H%G%#%9%W%l%$%P%C%U%!$N%5%$%:$rD4$Y$F!"(B
	 * $BF~$i$J$+$C$?$iBg$-$/$9$k(B
	 */
	if (buf->curLCStart < buf->nClause) {
		oklen = clpe->kanap - clps->kanap;
		odlen = clpe->dispp - clps->dispp;
	} else {
		oklen = odlen = 0;
	}
	ksize = (buf->kanaEnd - buf->kanaBuf) + newlen - oklen;
	dsize = (buf->displayEnd - buf->displayBuf) + newlen - odlen;
	if (ksize > buf->bufferSize || dsize > buf->bufferSize) {
		if (resizeBuffer(buf, ksize > dsize ? ksize : dsize) < 0)
			return -1;
	}

	/* curLCStart $B$,(B nClause $B$KEy$7$$;~$@$1!"?7$?$KJ8@a$,:n$i$l$k(B */
	if (buf->curLCStart == buf->nClause) {
		/* clauseInfo $B$NBg$-$5$rD4$Y$k(B*/
		if (buf->nClause + 1 > buf->clauseSize) {
			if (resizeCInfo(buf, buf->nClause + 1) < 0)
				return -1;
		}
		/* $B?7$?$K$G$-$?(B clauseInfo $B$K$O!"(BnClause $BHVL\(B
		 * ($B$D$^$j:G8e$N(B clauseInfo) $B$NFbMF$r%3%T!<$7$F$*$/(B
		 */
		clpe = buf->clauseInfo + buf->nClause + 1;
		*clpe = *(clpe - 1);

		buf->nClause++;
	}

	clps = buf->clauseInfo + buf->curLCStart;
	clpe = buf->clauseInfo + buf->curLCEnd;

	/* $B$+$J%P%C%U%!$NJQ99(B */
	/* $B$^$:$O8e$m$r0\F0$5$;$k(B */
	moveKBuf(buf, buf->curLCEnd, newlen - oklen);
	/* str $B$r%3%T!<(B */
	(void)bcopy((char *)str, (char *)clps->kanap,
		    newlen * sizeof(wchar));
	/* $B%G%#%9%W%l%$%P%C%U%!$NJQ99(B */
	/* $B$^$:$O8e$m$r0\F0$5$;$k(B */
	moveDBuf(buf, buf->curLCEnd, newlen - odlen);
	/* str $B$r%3%T!<(B */
	(void)bcopy((char *)str, (char *)clps->dispp,
		    newlen * sizeof(wchar));

	/* clauseInfo $B$NJQ99(B */
	/* $B$^$:$O8e$m$r0\F0$5$;$k(B */
	if (clpe > clps + 1) {
		(void)bcopy((char *)clpe, (char *)(clps + 1),
			    (buf->nClause + 1 - buf->curLCEnd) *
			    sizeof(jcClause));
	}
	clps->conv = 0;
	clps->ltop = 1;
	(clps + 1)->ltop = 1;

	return 0;
}

/* jcSaveDic -- $B<-=q!&IQEY%U%!%$%k$r%;!<%V$9$k(B */
int
jcSaveDic(buf)
jcConvBuf *buf;
{
	saveDicAll(buf);
	return 0;
}

#ifdef DEBUG_JCLIB

#include "WStr.h"

#define UL	"\033[4m"	/* $B%"%s%@!<%i%$%s(B */
#define MD	"\033[1m"	/* xterm $B%\!<%k%I(B */
#define SO	"\033[7m"	/* $B%9%?%s%I%"%&%H(B */
#define ST	"\033[m"	/* $B%N!<%^%k(B */

static void
wputstr(ss, se)
register wchar	*ss, *se;
{
	wchar	wline[256];
	wchar	*p;
	unsigned char	line[256];

	p = wline;
	while (ss < se)
		*p++ = *ss++;
	*p = 0;
	(void)convJWStoSJIS(wline, line);
	fputs(line, stdout);
}

void
jcPrintKanaBuf(buf)
jcConvBuf *buf;
{
	int	i;

	for (i = 0; i < buf->nClause; i++) {
		wputstr(buf->clauseInfo[i].kanap,
			buf->clauseInfo[i + 1].kanap);
		if (i < buf->nClause - 1) {
			putchar('-');
		}
	}
	putchar('\n');
}

void
jcPrintDisplayBuf(buf)
jcConvBuf *buf;
{
	int	i;
	jcClause	*clp = buf->clauseInfo;

	for (i = 0; i < buf->nClause; i++) {
		if (buf->curLCStart <= i && i < buf->curLCEnd) {
			fputs(MD, stdout);
		}
		if (i == buf->curClause) {
			fputs(SO, stdout);
		}
		if (!clp->conv) {
			fputs(UL, stdout);
		}
		wputstr(clp->dispp, (clp + 1)->dispp);
		fputs(ST, stdout);
		if (i < buf->nClause - 1) {
			if ((clp + 1)->ltop) {
				putchar('|');
			} else {
				putchar('-');
			}
		}
		clp++;
	}
	putchar('\n');
}

void
jcPrintDetail(buf)
jcConvBuf *buf;
{
	int	i;
	jcClause	*clp = buf->clauseInfo;
	int	jlen;

	printf("NofClauses: %d, CurClause: %d, CurLClause: %d-%d\n",
	       buf->nClause, buf->curClause, buf->curLCStart, buf->curLCEnd);
	if (buf->candClause >= 0) {
		if (buf->candKind == CAND_SMALL) {
			printf("CandClause: %d, ", buf->candClause);
		} else {
			printf("CandClause: %d-%d, ",
			       buf->candClause, buf->candClauseEnd);
		}
		printf("NofCandidates: %d\n", buf->nCand);
	}
	for (i = 0; i < buf->nClause; i++) {
		printf("cl[%d]: %c ", i, clp->ltop ? '*' : ' ');
		if (clp->conv > 0) {
			printf("<converted> dic=%d,entry=%d,hinshi=%d ",
			       clp->dicno, clp->entry, clp->hinshi);
			if (clp->imabit) putchar('*');
			printf("kanji: ");
			jlen = ((clp + 1)->dispp - clp->dispp) -
			    ((clp + 1)->kanap - clp->fzkp);
			wputstr(clp->dispp, clp->dispp + jlen);
			if (clp->fzkp != (clp + 1)->kanap) {
				putchar('-');
				wputstr(clp->dispp + jlen, (clp + 1)->dispp);
			}
		} else if (clp->conv < 0) {
			printf("<pseudo> kanji: ");
			wputstr(clp->dispp, (clp + 1)->dispp);
		} else {
			printf("<unconverted> kanji: ");
			wputstr(clp->dispp, (clp + 1)->dispp);
		}
		putchar('\n');
		clp++;
	}
	printf("cl[%d]: %c (last)\n", i, clp->ltop ? '*' : ' ');
	/* consistency check */
	if (clp->conv || clp->kanap != buf->kanaEnd || !clp->ltop) {
		printf("inconsistency found in last cinfo! - ");
		if (clp->conv) printf("<conv flag> ");
		if (clp->kanap != buf->kanaEnd) printf("<kanap> ");
		if (!clp->ltop) printf("<ltop flag>");
		putchar('\n');
	}
}

void
jcPrintCandBuf(buf)
jcConvBuf *buf;
{
	int	i;

	if (buf->candClause < 0) {
		printf("not ready\n");
		return;
	}

	if (buf->candKind == CAND_SMALL) {
		jcSCand	*scp;

		printf("<SMALL> CandClause: %d NofCandidates: %d CurCand: %d\n",
		       buf->candClause, buf->nCand, buf->curCand);
		scp = (jcSCand *)buf->candBuf;
		for (i = 0; i < buf->nCand; i++) {
			printf("%ccand[%d]%c: ",
			       i == buf->curCand ? '+' : ' ', i,
			       scp->status & IMA_BIT ? '*' : ' ');
			printf("dic=%d,entry=%d,hinshi=%d kanji: ",
			       scp->dicno, scp->entry, scp->hinshi);
			if (scp->status & CONNECT_PREV) putchar('<');
			wputstr(scp->kanji, scp->kanji + wstrlen(scp->kanji));
			if (scp->status & CONNECT_NEXT) putchar('>');
			putchar('\n');
			scp++;
		}
	} else {
		jcLCand	*lcp;
		jcSCand	*scp;
		int	j;

		printf("<LARGE> CandClause: %d-%d NofCandidates: %d CurCand: %d\n",
		       buf->candClause, buf->candClauseEnd,
		       buf->nCand, buf->curCand);
		lcp = (jcLCand *)buf->candBuf;
		for (i = 0; i < buf->nCand; i++) {
			printf("%ccand[%d](%d): ",
			       i == buf->curCand ? '+' : ' ', i, lcp->nscand);
			scp = lcp->scand;
			for (j = lcp->nscand; j > 0; j--) {
				wputstr(scp->kanji,
					scp->kanji + wstrlen(scp->kanji));
				if (j > 1) putchar('-');
				scp++;
			}
			putchar('\n');
			lcp++;
		}
	}
}
#endif
