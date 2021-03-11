/*
 *	cconv.c -- $BF~NOJ8;zJQ49%i%$%V%i%j(B (for X11)
 *		ver 10.2
 */

/*
 * Copyright (C) 1988  Software Research Associates, Inc.
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
 * $B35MW(B
 *
 * $BF~NOJ8;zJQ49$O!"$+$J4A;zJQ49$NA0CJ3,$G$"$j!"$=$NL\E*$O%-!<%\!<%I$+$i(B
 * $BF~NO$5$l$?%-!<%3!<%I$r2r<a$9$k$3$H$G$"$k!#(B
 * $B=>$C$F!"Bg$-$/J,$1$k$H<!$N(B2$B$D$N5!G=$r;}$D$3$H$K$J$k!#(B
 *
 * 1. $B%m!<%^;z$+$JJQ49$J$IF~NO$5$l$?J8;z$rJL$NJ8;z$KJQ$($k5!G=(B
 * 2. $B%-!<$NF~NO$K$h$j!"$+$J4A;zJQ49$G;H$o$l$k!VJQ49!W!"!V3NDj!W$J$I$N(B
 *    $B3F<o5!G=$r8F$S=P$95!G=(B
 *
 * $B$3$N(B cconv $B%i%$%V%i%j$O(B X Window version 11 $B$N$b$H$G;H$o$l$k$3$H$r(B
 * $BA[Dj$7$F$$$k!#(B
 */

/*
 * $B%a%b(B
 *
 * version 6.0	88/06/05
 *	$B$@$$$?$$$G$-$?(B
 * version 6.1	88/06/06
 *	$B%3%a%s%HF~$l(B
 * version 6.2	88/06/07
 *	$B$3$l$@$1F~$l$l$P%^%K%e%"%k=q$+$J$/$F$bBg>fIW$@$m$&(B
 * version 6.4	88/09/05
 *	$B2CF#;a(B@$BEl9)Bg$N0U8+$K$h$j(B redo $B5!G=$rF~$l$k(B
 *	$B$D$$$G$K(B static function $B$N@k8@$r$D$1$k(B
 *	$B$$$/$D$+$N4X?t$r(B external $B$+$i(B static $B$KJQ$($k(B
 * version 6.5	88/09/07
 *	function $B$,J#?t=q$1$k$h$&$K$7$?(B
 *	$B$=$N$?$a%G!<%?9=B$$rJQ99$9$k(B
 * version 6.6	88/10/07
 *	$B$J$s$H(B readRuleFile() $B$G%*!<%W%s$7$?%U%!%$%k$r(B
 *	$BJD$8$k$N$rK:$l$F$$$?(B -- $B$"!<$O$:$+$7$$(B
 *	$B=>$C$F(B wterm $B$G2?2s$b(B KanjiConvert $B$N(B on/off $B$r7+$jJV$9$H(B
 *	$B=i4|2=$K<:GT$9$k(B
 * version 6.7	88/12/19
 *	wstrlen() $B$O$3$N%i%$%V%i%j$GDj5A$9$k$Y$-$b$N$G$O$J$$$N$G30$9(B
 * version 6.8	89/07/21
 *	$B1F;3;a(B@$B>>2<EE4o$+$i%-!<%3!<%II=5-$,$&$^$/F0$+$J$$$H$NO"Mm$r<u$1$k(B
 *	getKey() $B$G(B XKEY $BI=5-$N>l9g!"(B#0x$B#1#6?J?t(B $B$NFI$_$H$j$K%P%0$,(B
 *	$B$"$k$3$H$,H=L@(B
 * version 7.0	89/08/16
 *	jclib $B$N(B Wnn version4 $BBP1~$K$h$j!"JQ49%P%C%U%!$,J#?t;}$F$k$h$&$K(B
 *	$B$J$C$?(B
 *	$B$=$l$K9g$o$;$F!"(Bcconv $B$b%P%C%U%!$,J#?t;}$F$k$h$&$K$7$J$1$l$P(B
 *	$B$J$i$J$$(B
 *	$B7k6IH>J,0J>e=q$-49$($F$7$^$C$?$h$&$@(B
 * version 7.1	89/08/17
 *	$B%P%0%U%#%C%/%9(B
 *	$B%b!<%IJQ99DLCNMQ$N%3!<%k%P%C%/$rDI2C$9$k(B
 * version 7.2	89/08/23
 *	$B%U%!%s%/%7%g%s%F!<%V%k$O!"$d$O$j(B ccParseRule() $B$G$O$J$/!"(B
 *	ccCreateBuf() $B$G;XDj$9$k$Y$-$b$N$J$N$G!"$=$N$h$&$KJQ99$9$k(B
 * version 7.3 89/08/25
 *	lint $B$G7Y9p$,=P$J$$$h$&$K=$@5(B
 *	dumpAllRules() $B$G$A$c$s$HJQ49%k!<%k$,%@%s%W$5$l$k$h$&$K=$@5(B
 * version 7.4 89/08/26
 *	ccParseRule() $B$G!"JQ49Dj5A%U%!%$%k$,A0$HF1$8$G$"$l$P(B
 *	$B%k!<%k$r6&MQ$9$k$h$&$KJQ99(B
 *	ccParseRule/ccFreeRule $B$G$N(B memory leak $B$N=$@5(B
 * version 7.5 89/09/26
 *	$BJQ49Dj5A%U%!%$%k$G(B include $B$,;H$($k$h$&$K=$@5(B
 * version 7.6 89/09/27
 *	include $B$N@bL@$r=q$/$N$rK:$l$F$$$?$N$G$=$l$rDI2C(B
 * version 7.7 89/10/26
 *	getKey() $B$G(B #$B?t;z(B $B7?$NI=5-$r$7$?;~$K$^$@%P%0$,$"$C$?$N$G=$@5(B
 *	$B$=$b$=$b$3$NJ}<0$NI=5-$G$O%^%9%/$,;XDj$G$-$J$$$H$$$&:,K\E*$J(B
 *	$BLdBj$,$"$k$,!"$^$"$3$l$O$$$$$3$H$K$7$F!#(B
 * version 8.0 89/10/27
 *	R4 contrib $B8~$1$N(B Release $B$K$"$?$C$F%P!<%8%g%sHV9f$r=$@5(B
 * version 8.1 89/12/25
 *	include $B%U%!%$%k$,$J$+$C$?;~$K%3%"%@%s%W$9$k%P%0$r=$@5(B
 * version 8.2 90/02/15
 *	X11R4 keyboard group support $BDI2C(B
 * version 8.3 90/02/16
 *	keyboard group support $B$N7k2L!"Nc$($P(B
 *		"" shift-Right "" foo
 *		"" Right       "" bar
 *	$B$H$$$&%k!<%k$,$"$C$?;~!"(Bshift-Right $B$r2!$7$?$K$b$+$+$o$i$:(B
 *	$B$7$?$N%k!<%k$K%^%C%A$7$F$7$^$&$H$$$&%P%0$r=$@5(B
 *	$BDj5A%U%!%$%k(B (include $B%U%!%$%k$b4^$`(B) $B$N%5!<%A%Q%9$H$7$F(B
 *	$B4D6-JQ?t(B CC_DEF_PATH $B$r;H$&$h$&$K=$@5(B
 * version 8.4 90/04/17
 *	keyboard group support $B$N%3!<%I$N%1%"%l%9%_%9$r=$@5(B
 *	$BJQ?tL>(B ks1 $B$H(B ks2 $B$r4V0c$($?(B
 * version 8.5 90/05/31
 *	keyboard group support $B$NIT6q9g$r=$@5(B
 *	$BL1ED;a(B@$B%"%9%F%C%/$+$i$N%l%]!<%H(B
 * version 8.6 91/03/20
 *	$B$I$N%k!<%k$K$b%^%C%A$7$J$+$C$?;~$K$O%G%U%)%k%H%"%/%7%g%s$,8F$P$l$k$,!"(B
 *	$B$3$N=hM}$G(B
 *		+ $B0z?t$,4V0c$C$F$$$?(B
 *		+ XLookupString() $B$N%j%?!<%s%P%j%e!<$,(B 0 $B$N$H$-(B ($B$D$^$j(B
 *		  $B%-!<%$%Y%s%H$KBP1~$9$kJ8;zNs$,$J$+$C$?;~(B) $B$K%G%U%)%k%H(B
 *		  $B%"%/%7%g%s$,8F$P$l$J$+$C$?(B
 *	$B$H$$$&LdBj$r=$@5(B
 *	$B%G%U%)%k%H%"%/%7%g%s$,(B NULL $B$N;~!"$I$N%k!<%k$K$b%^%C%A$7$J$1$l$P(B
 *	ccConvchar() $B$,(B -1 $B$rJV$9$h$&$K=$@5(B
 * version 8.7 91/03/25
 *	$BA0$N%P!<%8%g%s$N=$@5$K$h$j!"C1$K%7%U%H%-!<$d%3%s%H%m!<%k%-!<$r(B
 *	$B2!$7$?$@$1$G%G%U%)%k%H%"%/%7%g%s$,8F$P$l$k$h$&$K$J$C$?$,!"$3$l$O(B
 *	$B%f!<%6$N4|BT$9$kF0:n$G$O$J$$$h$&$J5$$,$9$k$N$G!"(BXLookupString() $B$N(B
 *	$B%j%?!<%s%P%j%e!<$,(B 0 $B$N;~$K$O%G%U%)%k%H%"%/%7%g%s$r8F$P$J$$$h$&$K(B
 *	$B:F=$@5(B
 *	$B$^$?!"%G%U%)%k%H%"%/%7%g%s$,(B NULL $B$G$+$D%k!<%k$K%^%C%A$7$J$$;~$K(B
 *	ccConvchar() $B$,(B -1 $B$rJV$9$h$&$K$7$?$,!"%G%U%)%k%H%"%/%7%g%s$NCM$K(B
 *	$B$h$C$F%j%?!<%s%P%j%e!<$,JQ$o$k$N$OJQ$@$7!"(B-1 $B$rJV$9$N$O%(%i!<$,(B
 *	$B5/$3$C$?$_$?$$$J$N$G!"7k6I%G%U%)%k%H%"%/%7%g%s$,(B NULL $B$G$"$k$+(B
 *	$B$I$&$+$K4X$o$i$:%^%C%A$7$?;~$O(B 1$B!"%^%C%A$7$J$$;~$O(B 0 $B$rJV$9(B
 *	$B$h$&$K=$@5(B
 *	$B$3$NJQ99$K$h$j(B ccConvchar() $B$N%j%?!<%s%P%j%e!<$,(B 0 $B$+$I$&$+%A%'%C%/(B
 *	$B$7$F$$$?%"%W%j%1!<%7%g%s$OF0$+$J$/$J$k$,!":#$^$G(B ccConvchar() $B$N(B
 *	$B%j%?!<%s%P%j%e!<$ODj5A$7$F$$$J$+$C$?$N$G$3$l$O$=$N%"%W%j%1!<%7%g%s$N(B
 *	$B@UG$(B
 * version 9.0 91/08/15
 *	$BJ8;z$N%G!<%?7?$H$7$F:#$^$G$:$C$H(B wchar_t $B$r;H$C$F$$$?$,!"(Bwchar_t $B$N(B
 *	$BDj5A$,5!<o$K$h$C$F0[$J$k$?$a!"(Bwchar $B$H$$$&7?$KJQ99$9$k!#K\Ev$O(B
 *	Wnn $B$N<!4|%P!<%8%g%s$K9g$o$;$?$$$N$G8e$G:F$SJQ99$9$k2DG=@-$,$"$k(B
 * version 9.1 91/09/18
 *	Files $B9=B$BN$N(B index $B$H$$$&L>A0$N%U%#!<%k%I$,!"(BSystemV $B7O$N%^%7%s(B
 *	$B$@$H(B strchr $B$KJQ$o$C$F$7$^$&$N$G(B findex $B$H$$$&L>A0$KJQ99(B
 * version 9.2 91/09/23
 *	DEBUG $B$,Dj5A$5$l$F$$$k$HJQ49%k!<%k$,%@%s%W$5$l$F$7$^$&$N$O(B kinput2
 *	$B$G;HMQ$9$k>l9g:$$k$N$G!"(BDEBUG $B$r(B DEBUG_CCONV $B$KJQ99(B
 * version 10.0 91/10/01
 *	R5 contrib $B8~$1$N%j%j!<%9$K$"$?$C$F%P!<%8%g%sHV9f$r=$@5(B
 * version 10.1 92/06/05
 *	Display $B9=B$BN$N(B mode_switch $B$r;2>H$7$F$$$?$,!"(BDisplay $B9=B$BN$O(B
 *	$BK\Mh(B opaque $B$G$"$j!"Cf$N%a%s%P!<$rD>@\%"%/%;%9$9$k$N$O$^$:$$$N$G(B
 *	mode_switch $B$NCM$r<+J,$G7W;;$9$k$h$&$KJQ99(B
 * version 10.2 94/04/21
 *	$B4X?t(B eproc $B$O(B varargs.h/stdarg.h $B$r;H$o$:$K=q$+$l$F$$$?$,!"(B
 *	$B0\?"@-$KLdBj$,$"$k(B (int $B$H(B long $B$N%5%$%:$,0[$J$k%^%7%s$J$I(B)
 *	$B$N$G=$@5(B
 */

/*
 * $B;HMQK!(B
 *
 * $B;HMQK!$O3d9g4JC1$G$"$k!#:G=i$KJQ49%k!<%kDj5A%U%!%$%k$rFI$_9~$`$?$a$K(B
 * ccParseRule() $B$r%3!<%k$9$k!#(B
 * $B<!$KJQ49%P%C%U%!$r:n$k$?$a$K(B ccCreateBuf() $B$r8F$S!"%P%C%U%!$r:n$k!#(B
 * $B$3$N;~$K!";HMQ$9$k%k!<%k$d<o!9$N%3!<%k%P%C%/4X?t$r;XDj$9$k!#(B
 * $BJQ49%k!<%k$HJQ49%P%C%U%!$OJ#?t;}$D$3$H$,$G$-$k!#(B
 * $B$^$?!"A0$N%P!<%8%g%s$H$N8_49@-$N$?$a$K!"JQ49%k!<%k$NFI$_9~$_$H(B
 * $BJQ49%P%C%U%!$N:n@.$rF1;~$K9T$J$&!"(BccInit() $B$H$$$&4X?t$bMQ0U$5$l$F$$$k!#(B
 *
 * $B$"$H$O%-!<%W%l%9$N%$%Y%s%H$,Mh$?$i(B ccConvchar() $B$r8F$V$@$1$G$"$k!#(B
 * $B$3$N%U%!%s%/%7%g%s$,JQ49$r9T$J$$!"E,Ev$J%3!<%k%P%C%/%k!<%A%s$r8F$V!#(B
 *
 * $B8e$OI,MW$K1~$8$FMQ0U$5$l$F$$$k$=$NB>$N%U%!%s%/%7%g%s$r8F$Y$P$h$$!#(B
 */

/*
 * $BJQ49$N;EAH$_(B
 *
 * $B$3$NF~NOJ8;zJQ49$N;EAH$_$rM}2r$9$k$?$a$N%-!<%o!<%I$O<!$N(B3$B$D$G$"$k!#(B
 *	$B!&%b!<%I(B
 *	$B!&JQ49%k!<%k(B
 *	$B!&%3%s%F%-%9%H(B
 *
 * $B!&%b!<%I(B
 *	$BF~NOJ8;zJQ49$K$O!V%b!<%I!W$,$"$k!#$3$l$ONc$($P!"(B
 *	$B!V%m!<%^;z2>L>JQ49$r$7$F$+$J$rF~NO$9$k%b!<%I!W$H$+(B
 *	$B!V%"%k%U%!%Y%C%H$rF~NO$9$k%b!<%I!W$H$$$C$?$b$N$G$"$k!#(B
 *	$B%b!<%I$4$H$K0[$J$kJQ49%k!<%k$,@_Dj$G$-$k!#EvA3%b!<%I$N@ZBX$($b(B
 *	$BJQ49%k!<%k$H$7$F5-=R$G$-$k!#(B
 * 
 * $B!&JQ49%k!<%k(B
 *	$BJQ49%k!<%k$O(B4$B$D$N9`L\$+$i9=@.$5$l$k!#(B
 *		$BF~NO%-!<(B
 *		$B%3%s%F%-%9%HJ8;zNs(B
 *		$BJQ497k2LJ8;zNs(B
 *		$B%U%!%s%/%7%g%s(B
 *	$B$3$N$&$A!"F~NO%-!<$H%3%s%F%-%9%HJ8;zNs$,%k!<%k$N%^%C%A%s%0$K;H$o$l$k!#(B
 * 
 *	$BF~NO%-!<$O%-!<%3!<%I$r;XDj$9$k$b$N$G!"(BX $B$N%-!<%$%Y%s%H%3!<%I$G(B
 *	$B;XDj$9$kJ}K!$H!"$=$l$r(B XLookupString $B$7$?7k2L$N%-%c%i%/%?%3!<%I$G(B
 *	$B;XDj$9$kJ}K!$H$N(B2$BDL$j$,$"$k!#(B
 *
 *	$B%3%s%F%-%9%HJ8;zNs$O!"$=$l$^$G$K$I$N$h$&$JJ8;z$,F~NO$5$l$?$+$r(B
 *	$B;XDj$9$k$b$N$G$"$k!#(B
 *
 *	$BJQ497k2LJ8;zNs$O$=$NL>$NDL$j!"JQ497k2L$N;XDj$G!"%^%C%A$7$?(B
 *	$B%3%s%F%-%9%H$,$3$NJQ497k2L$KCV$-$+$o$k!#(B
 *
 *	$B%U%!%s%/%7%g%s$O%k!<%k$,%^%C%A$7$?$H$-$K<B9T$5$l$k%U%!%s%/%7%g%s(B 
 *	("$BJQ49(B" $B$H$+(B "$B3NDj(B" $B$H$+(B) $B$N;XDj$G$"$k!#%U%!%s%/%7%g%s$OJ#?t(B
 *	$B;XDj$9$k$3$H$b$G$-$k!#(B
 * 
 *	$BNc$r$"$2$k$H!"(B
 *	$B!V(B"ky" $B$HF~$C$F$$$k$H$3$m$G(B 'a' $B$H$$$&%-!<$,2!$5$l$?$i(B
 *	"$B$-$c$C(B!" $B$HJQ49$7!"%Y%k$rLD$i$9!W(B
 *	$B$H$$$&%k!<%k(B ($B$"$s$^$j0UL#$J$$$1$I(B) $B$G!"(B
 *		'a' $B$,F~NO%-!<!"(B
 *		"ky" $B$,%3%s%F%-%9%H!"(B
 *		"$B$-$c$C(B!" $B$,JQ497k2L!"(B
 *		<$B%Y%k$rLD$i$9(B> $B$,%U%!%s%/%7%g%s(B
 *	$B$H$$$&$3$H$K$J$k!#(B
 *
 * $B!&%3%s%F%-%9%H(B
 *	$B0J>e$N$h$&$K!"%3%s%F%-%9%H$H$$$&$N$O$=$l$^$G$K$I$N$h$&$JJ8;z$,(B
 *	$BF~NO$5$l$F$$$J$1$l$P$J$i$J$$$+$r;XDj$9$k$b$N$G$"$k!#(B
 *	$B$=$l$HF1;~$K$=$NJ8;z$rJQ497k2L$HCV$-JQ$($k$3$H$r;XDj$9$k$b$N(B
 *	$B$G$b$"$k!#(B
 *
 *	$B%3%s%F%-%9%H$O6uJ8;zNs$G$b$h$$!#$=$N$H$-$K$O$=$l$^$G$K$I$N$h$&$J(B
 *	$BJ8;z$,F~NO$5$l$?$+$K$+$+$o$i$:!"%^%C%A%s%0$,5/$3$k!#(B
 *
 *	$B%3%s%F%-%9%H$OH>3Q$N>l9gBgJ8;z>.J8;z$r6hJL$7$J$$!#(B
 *	$B$I$A$i$K$b%^%C%A$9$k!#(B
 *
 * $B!&JQ49$N<B:](B
 *	$B$5$F!"$I$N$h$&$K$7$FJQ49$,9T$J$o$l$k$N$+$r4JC1$K=q$$$F$_$k!#(B
 *
 *	$B%-!<%W%l%9%$%Y%s%H$r0z?t$K$7$F(B ccConvchar() $B$,8F$P$l$k!#(B
 *	$B$9$k$H!"$^$:$O%$%Y%s%H%3!<%I$G%+%l%s%H%b!<%I$GDj5A$5$l$F$$$k(B
 *	$BJQ49%k!<%k$NCf$+$i%^%C%A$9$k$b$N$rC5$9!#C5:w$O%k!<%k$N5-=R=g$K(B
 *	$B9T$J$o$l!":G=i$K%^%C%A$7$?$b$N$,A*$P$l$k!#(B
 *	$B$b$7%^%C%A$9$k%k!<%k$,$J$1$l$P!":#EY$O%$%Y%s%H%3!<%I$r(B
 *	XLookupString() $B$GJQ49$7$?%-%c%i%/%?%3!<%I$G!"%^%C%A$9$k$b$N$rC5$9!#(B
 *
 *	$B$=$l$G$b%^%C%A$9$k%k!<%k$,8+$D$+$i$J$+$C$?$H$-$K$O!"%G%U%)%k%H(B
 *	$B%"%/%7%g%s$N%3!<%k%P%C%/%k!<%A%s$,8F$P$l$k!#(B
 *
 *	$B%^%C%A$9$k%k!<%k$,8+$D$+$l$P!"JQ497k2LJ8;zNs$X$NCV$-49$($r9T$J$&!#(B
 *	$B$D$^$j!"$^$:%k!<%k$K5-=R$5$l$F$$$k%3%s%F%-%9%HJ8;zNs$NJ8;z?tJ,$@$1(B
 *	1$BJ8;z:o=|MQ$N%3!<%k%P%C%/%k!<%A%s$,8F$P$l!"$=$N8eJQ497k2LJ8;zNs$,(B
 *	1$BJ8;zF~NOMQ$N%3!<%k%P%C%/%k!<%A%s$r8F$s$GF~NO$5$l$k!#(B
 *	$B$=$N%k!<%k$K%U%!%s%/%7%g%s$,;XDj$5$l$F$$$l$P%G%#%9%Q%C%AMQ$N(B
 *	$B%3!<%k%P%C%/%k!<%A%s$,8F$S=P$5$l$k!#(B
 *
 *	$B%U%!%s%/%7%g%s$,B>$N%b!<%I$X$N@ZBX$($@$C$?>l9g$K$O>/$7J#;($G$"$k!#(B
 *	$B$=$N$H$-$K$O!"$^$:(B EXITMODE $B$H$$$&5?;w%-!<$r0z?t$K$7$F(B ccConvchar()
 *	$B$,8F$P$l$k!#$3$l$O$"$k%b!<%I$+$iH4$1$k:]$K2?$+=hM}$r$7$?$$!"(B
 *	$B$H$$$&$H$-$N$?$a$K$"$k!#$=$N8e%+%l%s%H%b!<%I$r?7$7$$%b!<%I$K$7$F!"(B
 *	$B:#EY$O(B ENTERMODE $B$H$$$&5?;w%-!<$r0z?t$K$7$F(B ccConvchar $B$,8F$P$l$k!#(B
 *	$B$3$l$K$h$C$F$"$k%b!<%I$KF~$C$?$H$-$N=hM}$r$9$k$3$H$,$G$-$k!#(B
 *	$B$=$N8e!"%3%s%F%-%9%H$,%/%j%"$5$l!":G8e$K!"%b!<%IJQ99DLCNMQ$N(B
 *	$B%3!<%k%P%C%/4X?t$,;XDj$5$l$F$$$l$P$=$l$,8F$P$l$k!#(B
 */

/*
 * ccdef $B%U%!%$%k$N=q$-J}(B
 *
 * $BF~NOJ8;zJQ49Dj5A%U%!%$%k(B (ccdef $B%U%!%$%k$H>JN,$9$k(B) $B$N=q$-J}$r4JC1$K5-$9!#(B
 *
 * ccdef $B%U%!%$%k$O<!$N(B3$B$D$N%Q!<%H$+$i9=@.$5$l$k!#(B
 * $B$3$l$i$O$3$N=g$KJB$s$G$$$J$1$l$P$J$i$J$$!#(B
 *
 *	<$B%b!<%I@k8@(B>
 *	<$B=i4|%b!<%I@k8@(B>
 *	<$B3F%b!<%I$NJQ49%k!<%k5-=R(B>
 *
 * <$B%b!<%I@k8@(B> $B$O;HMQ$9$k%b!<%IL>$r@k8@$9$k$b$N$G!"%U%)!<%^%C%H$O<!$NDL$j!#(B
 *	defmode Mode1 Mode2 Mode3...
 * $B;HMQ$9$k%b!<%I$O$9$Y$F$3$3$G@k8@$7$F$*$+$J$/$F$O$J$i$J$$!#(B
 *
 * <$B=i4|%b!<%I@k8@(B> $B$O(B cconv $B$,=i4|2=$5$l$?$H$-$N%b!<%I$r;XDj$9$k$b$N$G$"$k!#(B
 * $B%U%)!<%^%C%H$O<!$NDL$j!#(B
 *	initialmode Mode3
 *
 *
 * <$B3F%b!<%I$NJQ49%k!<%k5-=R(B> $B$,<B:]$NJQ49%k!<%k$r5-=R$9$kItJ,$G$"$k!#(B
 * 1$B$D$N%b!<%I$KBP$9$k5-=R7A<0$O<!$NDL$j!#(B
 *
 *	mode <$B%b!<%IL>(B> "<$B%W%m%s%W%H(B>" [fallthrough <$B%b!<%IL>(B>]
 *		"<$B%3%s%F%-%9%H(B>" <$B%-!<(B> "<$BJQ497k2L(B>" [<$B%U%!%s%/%7%g%s(B>...]
 *			:
 *			:
 *	endmode
 *
 * <$B%b!<%IL>(B> $B$O(B <$B%b!<%I@k8@(B> $B$G@k8@$7$?%b!<%IL>$G$"$k!#(B
 *
 * <$B%W%m%s%W%H(B> $B$O%b!<%I$rI=$9J8;zNs$G$"$k!#$3$l$O%+%l%s%H%b!<%I$rI=<($9$k(B
 * $B:]$K;HMQ$5$l$k$b$N$G!"4A;z$G$b$h$$!#D9$5$O<+M3$G$"$k$,!"M>$jD9$$$H(B
 * $B$9$Y$FI=<($5$l$k$+$I$&$+5?Ld$G$"$k!#(B
 * $B$=$N8e$N!"(B[ ] $B$K$/$k$^$l$?ItJ,$O>JN,2DG=$J$3$H$r<($9!#(B
 * $B$b$7$"$l$P!"$3$N%b!<%I$G%^%C%A$9$k%k!<%k$,$J$+$C$?$H$-$K$O(B fallthrough $B$G(B
 * $B;XDj$5$l$k%b!<%I$N%k!<%k$,0z$-B3$$$FC5$5$l$k!#(B
 *
 * mode $B$H(B endmode $B$K$O$5$^$l$?ItJ,$,8D!9$NJQ49%k!<%k$G$"$k!#(B
 * <$B%3%s%F%-%9%H(B> $B$,%3%s%F%-%9%HJ8;zNs$G$"$k!#$3$3$G$O(B '^' $B$H(B '\' $B$,FC<l(B
 * $B%-%c%i%/%?$H$7$F;HMQ$G$-$k!#(B"^C" $B$H=q$1$P$=$l$O(B $B%3%s%H%m!<%k(B-C $B$rI=$9!#(B
 * $B%P%C%/%9%i%C%7%e(B '\' $B$O<!$NJ8;z$NFC<lJ8;z$H$7$F$N0UL#$r$J$/$9!#(B
 * '^' $B<+?H$r;H$$$?$1$l$P(B "\^" $B$H%(%9%1!<%W$9$k!#F1MM$K(B '\' $B<+?H$r;H$$$?$1$l$P(B
 * "\\" $B$H=E$M$l$P$h$$!#(B
 *
 * <$B%-!<(B> $B$OF~NO$5$l$?%-!<$G!"(BX $B$N%-!<%$%Y%s%H!"%-%c%i%/%?%3!<%I!"(B
 * $B%a%?%-%c%i%/%?!"5?;w%-!<$N$$$:$l$+$G;XDj$9$k!#(B
 *
 * X $B$N%-!<%$%Y%s%H$O!"4pK\E*$K!"(B
 *	$B%b%G%#%U%!%$%"(B-Keysym$BL>(B
 * $B$GI=$9!#Nc$($P!"(B
 *	Tab
 *	control-a
 *	shift-control-space
 * $B$J$I$G$"$k!#(B
 *
 * $B%-%c%i%/%?%3!<%I$r=q$/$K$O!"%7%s%0%k%/%)!<%H$r;HMQ$9$k!#Nc$($P(B
 *	'a'
 *	'^['
 *	'\''
 * $B$J$I$G$"$k!#$o$+$k$H;W$&$,!"(B2$BHVL\$O(B ESCAPE$B!"(B3$BHVL\$O%7%s%0%k%/%)!<%H(B
 * $B$=$N$b$N$r<($9!#(B
 *
 * $B%a%?%-%c%i%/%?$O<!$N(B10$B<oN`$,;HMQ$G$-$k!#(B
 *  $B%-!<%$%Y%s%H$G$b%-%c%i%/%?%3!<%I$G$b%^%C%A$9$k$b$N(B
 *	@any		- $B2?$K$G$b%^%C%A$9$k(B
 *  $B%-!<%$%Y%s%H$K%^%C%A$9$k$b$N(B
 *	@raw		- $B%-!<%$%Y%s%H$J$i2?$K$G$b%^%C%A(B
 *	@func		- $B%U%!%s%/%7%g%s%-!<$K%^%C%A(B
 *	@cursor		- $B%+!<%=%k%-!<$K%^%C%A(B
 *	@keypad		- $B%F%s%-!<%Q%C%I$N%-!<$K%^%C%A(B
 *	@modifier	- $B%b%G%#%U%!%$%"(B (shift, control $B$J$I(B) $B$K%^%C%A(B
 *	@non-ascii	- $B%-!<%$%Y%s%H$N$&$A!"(BXLookupString() $B$N7k2L$,(B
 *			  $BD9$5(B0$B!"$D$^$jBP1~$9$k%"%9%-!<J8;z$,$J$$$b$N$K%^%C%A(B
 *  $B%-%c%i%/%?%3!<%I$K%^%C%A$9$k$b$N(B
 *	@ascii		- $B%-%c%i%/%?%3!<%I$J$i2?$K$G$b%^%C%A(B
 *	@printable	- $B0u;z2DG=J8;z(B (0x20 $B!e(B c $B!e(B 0x7e) $B$K%^%C%A(B
 *	@control	- $B%3%s%H%m!<%kJ8;z(B (0x00 $B!e(B c $B!e(B 0x1f) $B$K%^%C%A(B
 *
 * $B5?;w%-!<$OK\Ev$N%-!<F~NO$G$O$J$$!#$3$l$K$O(B2$B<oN`$"$k!#(B
 *	ENTERMODE	- $B$"$k%b!<%I$KF~$C$?$H$-$K2>A[E*$KF~NO$5$l$k%-!<(B
 *	EXITMODE	- $B$"$k%b!<%I$+$iH4$1$k$H$-$K2>A[E*$KF~NO$5$l$k%-!<(B
 *
 * <$BJQ497k2L(B> $B$OJQ497k2L$r;XDj$9$k!#$3$3$G$O(B '&'$B!"(B'/'$B!"(B'^'$B!"(B'\' $B$N(B4$B<oN`$N(B
 * $BFC<lJ8;z$,;HMQ$G$-$k!#(B'^' $B$H(B '\' $B$K$D$$$F$O(B <$B%3%s%F%-%9%H(B> $B$HF1$8$G$"$k!#(B
 * '&' $B$O(B $B%^%C%A$7$?(B <$B%-!<(B> $B$HCV$-49$($i$l$k!#(B<$B%-!<(B> $B$,%$%Y%s%H$N>l9g$K$O(B
 * $B$=$N%$%Y%s%H$r(B XLookupString() $B$7$?7k2L$NJ8;zNs$GCV$-49$o$k!#(B
 * '/' $B$,=P$F$/$k$H!"$=$3$G%3%s%F%-%9%H$O%/%j%"$5$l$k!#(B
 *
 * <$B%U%!%s%/%7%g%s(B> $B$O$J$/$F$b$h$$!#$"$l$PJQ49$N8e!"%U%!%s%/%7%g%s$,(B
 * $B<B9T$5$l$k!#%U%!%s%/%7%g%s$rJ#?t;XDj$9$k$3$H$b$G$-!"$=$N>l9g$K$O(B
 * $B;XDj$5$l$?=g=x$G<B9T$5$l$k!#%U%!%s%/%7%g%s$N6h@Z$j$O6uGrJ8;z$G$"$k!#(B
 *	func1 func2 func3
 * $B$N$h$&$K;XDj$9$k!#(B
 * $B%U%!%s%/%7%g%s$H$7$F%+%l%s%H%b!<%I$NJQ99$r;XDj$9$k$3$H$b$G$-$k!#(B
 *	goto <$B%b!<%IL>(B>
 * $B$G%+%l%s%H%b!<%I$,JQ$o$k!#$3$N%b!<%IL>$H$7$F(B "PREV" $B$,;HMQ$G$-$k!#(B
 * $B$3$l$O0l$DA0$N%b!<%I$rI=$9!#(B
 * $B$^$?!":FJQ49$r;XDj$9$k$3$H$b$G$-$k!#(B<$B%U%!%s%/%7%g%s(B>$B$H$7$F(B
 *	redo
 * $B$H=q$/$H!"$b$&0lEY%^%C%A$9$k%k!<%k$r:G=i$+$iC5$7$K$$$/!#$?$@$7$=$NA0$K(B
 * <$B%3%s%F%-%9%H(B> $B$,(B <$BJQ497k2L(B> $B$KCV$-49$($i$l$F$$$k$3$H$KCm0U!#$3$N5!G=$r(B
 * $B;H$&;~$K$OL58B%k!<%W$K4Y$i$J$$$h$&$KCm0U$7$F%k!<%k$r=q$+$J$1$l$P$J$i$J$$!#(B
 * $B0l1~0BA4:v$H$7$F(B redo $B$,(B MAXREDO (=20) $B2s8F$P$l$k$H<:GT$9$k$h$&$K(B
 * $B$J$C$F$$$k!#(B
 * $BCm0U(B: goto $B$H(B redo $B$N$"$H$K;XDj$5$l$?%U%!%s%/%7%g%s$O<B9T$5$l$J$$!#(B
 * $B$D$^$j!"(B
 *	func1 goto XX func2
 * $B$@$H!":G=i$K(B func1 $B$,<B9T$5$l!"<!$K%+%l%s%H%b!<%I$,(B XX $B$KJQ99$5$l$k$,(B
 * func2 $B$O<B9T$5$l$J$$!#(B
 *
 * $B:G8e$K=EMW$JCm0U$r0l$D!#(Bccdef $B%U%!%$%k$O(B EUC $B%3!<%I$G=q$+$l$F$$$J$1$l$P(B
 * $B$J$i$J$$!#(B
 *
 * $B$J$*!"=q$-K:$l$?$,(B '#' $B$G;O$^$k9T$O%3%a%s%H9T$G$"$k!#(B
 *
 * $B$^$?!"B>$N%U%!%$%k$r(B include $BJ8$r;H$C$F%$%s%/%k!<%I$9$k$3$H$,$G$-$k!#(B
 * $B=q<0$O(B
 *	include	$B%U%!%$%kL>(B
 * $B$G$"$k!#%U%!%$%kL>Cf$K6uGrJ8;z$,4^$^$l$k;~$K$O(B ' $B$+(B " $B$G0O$a$P$h$$!#(B
 * $B%U%!%$%kL>$,AjBP%Q%9L>$N;~$K$O$^$:%+%l%s%H%G%#%l%/%H%j$,C5$5$l!"$J$1$l$P(B
 * $B4D6-JQ?t(B CC_DEF_PATH $B$,Dj5A$5$l$F$$$l$P$=$l$K;XDj$5$l$?%G%#%l%/%H%j(B
 * (':' $B$G6h@Z$C$FJ#?t;XDj$9$k$3$H$,$G$-$k(B) $B$N2<$,C5$5$l!"$=$l$G$b$J$1$l$P(B
 * CCDEFPATH (/usr/lib/X11/ccdef/) $B$N2<$,C5$5$l$k!#(B
 */

/*
 * $B%$%s%?!<%U%'%$%9!&%U%!%s%/%7%g%s(B 
 *
 * - $BJQ49%k!<%kFI$_9~$_(B -
 *
 * ccRule ccParseRule(char *deffile, void (*errprint)())
 *	$BF~NOJ8;zJQ49%U%!%$%k$rFI$_9~$`!#(B
 *	deffile $B$G;XDj$5$l$k%U%!%$%k$+$iJQ49%k!<%k$rFI$_9~$`!#$b$7$b(B
 *	deffile $B$,(B NULL $B$N;~$O!"4D6-JQ?t(B CC_DEF $B$NCM$,;HMQ$5$l$k!#(B
 *	deffile $B$,AjBP%Q%9L>$N;~$K$O$^$:%+%l%s%H%G%#%l%/%H%j$N2<$,C5$5$l!"(B
 *	$B$J$1$l$P!"4D6-JQ?t(B CC_DEF_PATH $B$G;XDj$5$l$?%G%#%l%/%H%j$N2<$,(B
 *	$BC5$5$l$k!#(BCC_DEF_PATH $B$K$O4D6-JQ?t(B PATH $B$J$I$HF1$8$/J#?t$N(B
 *	$B%G%#%l%/%H%j$r(B ':' $B$G6h@Z$C$F;XDj$9$k$3$H$,$G$-$k!#(B
 *	$B$J$1$l$P(B CCDEFPATH (/usr/lib/X11/ccdef) $B$N2<$,C5$5$l$k!#(B
 *	$BF1$8JQ49%U%!%$%k$KBP$7$FJ#?t2s(B ccParseRule() $B$r9T$J$C$F$b!"(B
 *	$B<B:]$KFI$_9~$^$l$k$N$O:G=i$N0l2s$@$1$G$"$k!#(B
 *	errprint $B$O%(%i!<I=<(MQ$N%U%!%s%/%7%g%s$G$"$k$,!"(B NULL $B$N>l9g$K$O(B
 *	stderr $B$X(B fprintf() $B$r;HMQ$7$F=PNO$5$l$k!#(B
 *	$B@.8y;~$K$OJQ49%k!<%k$r!"<:GT;~$K$O(B NULL $B$rJV$9!#(B
 *
 * void ccFreeRule(ccRule rule)
 *	$B;HMQ$7$J$/$J$C$?JQ49%k!<%k$G;H$o$l$F$$$?NN0h$r2rJ|$9$k!#(B
 *
 *
 * - $BJQ49%P%C%U%!:n@.(B -
 *
 * ccBuf ccCreateBuf(ccRule rule, int contextsize,
 *		     char *functable[], int functablesize,
 *		     void (*default_action)(), void (*insert_char)(),
 *		     void (*delete_char)(), void (*function_dispatch)(),
 *		     void (*mode_notify)(), caddr_t client_data);
 *	$BJQ49%P%C%U%!$r:n$j!"$=$l$rJV$9!#(B
 *	rule $B$G!"$I$NJQ49%k!<%k$r;HMQ$9$k$+;XDj$9$k!#(B
 *	contextsize $B$K$O%3%s%F%-%9%H$r2?J8;zJ];}$9$k$+$r;XDj$9$k!#(B
 *	$B$h$[$IFC<l$J%k!<%k$,$J$$8B$j!"IaDL$O?tJ8;z$GB-$j$k$O$:$G$"$k!#(B
 *	functable $B$O%U%!%s%/%7%g%sL>$N0lMwI=!"(Bfunctablesize $B$O$=$N%5%$%:(B
 *	$B$G$"$k!#(B
 *	default_action $B$O!"F~NO$5$l$?%-!<$,$I$N%k!<%k$K$b%^%C%A$7$J$+$C$?(B
 *	$B$H$-$K8F$P$l$k%U%!%s%/%7%g%s$G$"$k!#(BNULL $B$r;XDj$9$k$H!"%^%C%A(B
 *	$B$7$J$+$C$?$H$-$K$O2?$b5/$3$i$J$$!#(B
 *	insert_char, delete_char $B$O$=$l$>$l(B 1$BJ8;zA^F~(B / 1$BJ8;z:o=|MQ$N(B
 *	$B%U%!%s%/%7%g%s$G$"$k!#(B
 *	function_dispatch $B$K$O%U%!%s%/%7%g%s%3!<%k$N$?$a$N%G%#%9%Q%C%A(B
 *	$B%k!<%A%s$r;XDj$9$k!#(B
 *	mode_notify $B$O!"%b!<%I$,JQ$o$C$?;~$K8F$S=P$5$l$k%U%!%s%/%7%g%s$G$"$k!#(B
 *	$B%3!<%k%P%C%/$NI,MW$,$J$$$H$-$O(B NULL $B$r;XDj$7$F$*$1$P$h$$!#(B
 *	client_data $B$O!"%3!<%k%P%C%/;~$K0z?t$H$7$FEO$5$l$k%G!<%?$G$"$k!#(B
 *	$B@.8y;~$K$O%P%C%U%!$r!"%(%i!<$N;~$K$O(B NULL $B$,JV$5$l$k!#(B
 *
 * void ccDestroyBuf(ccBuf buf)
 *	$B;HMQ$7$J$/$J$C$?JQ49%P%C%U%!$G;H$o$l$F$$$?NN0h$r2rJ|$9$k!#(B
 *
 *
 * - $BJQ49(B -
 *
 * int ccConvchar(ccBuf buf, XKeyPressedEvent *event)
 *	X11 $B$N%-!<%\!<%I$N%W%l%9%$%Y%s%H$r<u$1<h$j!"JQ49$r9T$J$&!#(B
 *	$B9T$J$C$?7k2L!"J8;z$NA^F~!&:o=|$O(B ccCreateBuf() $B$G;XDj$5$l$?(B
 *	insert_char, delete_char $B$,8F$S=P$5$l!"%U%!%s%/%7%g%s$K$D$$$F$O(B
 *	$BF1$8$/(B ccCreateBuf() $B$G;XDj$5$l$k(B function_dispatch $B$,%G%#%9%Q%C%A$N(B
 *	$B$?$a$K8F$P$l$k!#(B
 *	$B$I$N%k!<%k$K$b%^%C%A$7$J$1$l$P!"(Bdefault_action $B$,8F$P$l$k!#(B
 *	$B$I$N%k!<%k$K$b%^%C%A$7$J$+$C$?;~$K$O(B 0$B!"%^%C%A$7$?;~$K$O(B 1 $B$,(B
 *	$BJV$5$l$k!#(B
 *
 *
 * - $B%+%l%s%H%b!<%I(B -
 *
 * int ccGetMode(ccBuf buf)
 *	$B%+%l%s%H%b!<%IHV9f$,JV$5$l$k!#(B
 *
 * wchar *ccGetModePrompt(ccBuf buf)
 *	$B%+%l%s%H%b!<%IL>$,(B EUC$B%W%m%;%9%3!<%I$GJV$5$l$k!#(B
 *
 *
 * - $B%P%C%U%!$N;HMQ$7$F$$$k%k!<%k(B -
 *
 * ccRule ccGetRule(ccBuf buf)
 *	$B;XDj$5$l$?JQ49%P%C%U%!$,;HMQ$7$F$$$kJQ49%k!<%k$rJV$9!#(B
 *
 *
 * - $B%3%s%F%-%9%H(B -
 *
 * $B%3%s%F%-%9%H$X$NJ8;z$NDI2C$d:o=|$O!"%k!<%k$K%^%C%A$7$?$H$-$K$O(B
 * $B<+F0E*$K9T$J$o$l$k!#$^$?!"%+%l%s%H%b!<%I$NJQ99$K$H$b$J$C$F(B
 * $B%3%s%F%-%9%H$O<+F0E*$K%/%j%"$5$l$k!#(B
 * $B=>$C$FIaDL$O%f!<%6$,$3$l$i$N%U%!%s%/%7%g%s$r8F$VI,MW$O$J$$!#(B
 * $B$?$@$7!"%^%C%A$7$J$+$C$?J8;z$r(B default_action $B$,A^F~$9$k$H$+!"(B
 * $B%P%C%/%9%Z!<%9$N=hM}$r9T$J$&!"$J$I$N>l9g$K$O%f!<%6$,(B explicit $B$K(B
 * $B8F$VI,MW$,$"$k!#(B
 *
 * void ccContextAppend(ccBuf buf, int c)
 *	$B%3%s%F%-%9%H$N:G8e$KJ8;z(B c (EUC $B%W%m%;%9%3!<%I(B)$B$rIU$12C$($k!#(B
 *
 * void ccContextDelete(ccBuf buf)
 *	$B%3%s%F%-%9%H$N:G8e$N(B1$BJ8;z$r:o=|$9$k!#(B
 *
 * void ccContextClear(ccBuf buf)
 *	$B%3%s%F%-%9%H$r%/%j%"$9$k!#(B
 *
 * void ccContextSet(ccBuf buf, wchar *cstr)
 *	$B%3%s%F%-%9%HJ8;zNs$r%;%C%H$9$k!#J8;zNs$O(B null $B%?!<%_%M!<%H$5$l$?(B
 *	EUC $B%W%m%;%9%3!<%I$G$J$1$l$P$J$i$J$$!#(B
 *	$BJ8;zNs$ND9$5$,(B ccCreateBuf() $B$G;XDj$5$l$?(B contextsize $B$h$j(B
 *	$BD9$$$H$-$K$O!":G8e$N(B contextsize $BJ8;z$,%;%C%H$5$l$k!#(B
 *
 * void ccContextGet(ccBuf buf, wchar *cstr)
 *	$B8=:_$N%3%s%F%-%9%HJ8;zNs$rJV$9!#J8;zNs$O(B null $B%?!<%_%M!<%H$5$l$?(B
 *	EUC $B%W%m%;%9%3!<%I$G$"$k!#(B
 */

/*
 * $B%3!<%k%P%C%/%U%!%s%/%7%g%s$H$=$N0z?t(B
 *
 *	$B%G%#%9%Q%C%A%U%!%s%/%7%g%s(B:
 *	    function_dispatch(int func, unsigned char *str, int nbytes,
 *			      caddr_t client_data)
 *		$B%^%C%A$7$?%k!<%k$K%U%!%s%/%7%g%s$,=q$+$l$F$$$?$H$-$K(B
 *		$B8F$S=P$5$l$k!#(B
 *		$B0z?t(B func $B$O!"%U%!%s%/%7%g%sHV9f$G$"$k!#$3$l$O(B
 *		ccParseRule() $B$N0z?t$H$7$FEO$5$l$?!"(Bfunctable[] $B$N(B
 *		$B%$%s%G%C%/%9$G$"$k!#(Bstr, nbytes $B$O$3$N(B
 *		$B%U%!%s%/%7%g%s$r0z$-5/$3$7$?%-!<$NI=$9J8;zNs!#(B
 *		client_data $B$O(B ccCreateBuf() $B$N;~$K;XDj$5$l$?%G!<%?$G$"$k!#(B
 *
 *	$B%G%U%)%k%H%"%/%7%g%sMQ%U%!%s%/%7%g%s(B:
 *	    default_action(unsigned char *str, int nbytes, caddr_t client_data)
 *		$B2!$5$l$?%-!<$KBP$7$F%^%C%A$9$k%k!<%k$,$J$+$C$?$H$-$K(B
 *		$B8F$S=P$5$l$k!#(B
 *		str, nbytes $B$O%-!<$NI=$9J8;zNs!#(B
 *		client_data $B$O(B ccCreateBuf() $B$N;~$K;XDj$5$l$?%G!<%?!#(B
 *
 *	$BJ8;zA^F~%U%!%s%/%7%g%s(B:
 *	    insert_char(wchar c, caddr_t client_data)
 *		$BJQ497k2L$NJ8;z$r%"%W%j%1!<%7%g%s$KEO$9$N$K;HMQ$5$l$k!#(B
 *		c $B$O(B EUC $B%W%m%;%9%3!<%I$G$"$k!#$3$N%U%!%s%/%7%g%s$O(B
 *		1$BJ8;z$4$H$K8F$P$l$k!#(B
 *		client_data $B$O(B ccCreateBuf() $B$N;~$K;XDj$5$l$?%G!<%?!#(B
 *
 *	$BJ8;z:o=|%U%!%s%/%7%g%s(B:
 *	    delete_char(caddr_t client_data)
 *		$BJQ497k2L$K$7$?$,$C$F:o=|$9$kJ8;z$,$"$l$P$=$NJ8;z?tJ,(B
 *		$B$3$N%U%!%s%/%7%g%s$,8F$P$l$k!#(B
 *		client_data $B$O(B ccCreateBuf() $B$N;~$K;XDj$5$l$?%G!<%?!#(B
 *
 *	$B%b!<%IJQ99%U%!%s%/%7%g%s(B:
 *	    mode_notify(int newmode, int oldmode, caddr_t client_data)
 *		$B%+%l%s%H%b!<%I$,JQ99$5$l$?;~$K$3$N%U%!%s%/%7%g%s$,(B
 *		$B8F$P$l$k!#(Bnewmode $B$,?7$7$$%+%l%s%H%b!<%I!"(Boldmode $B$,(B
 *		$BA0$N%+%l%s%H%b!<%IHV9f$G$"$k!#(B
 *
 *	$B%(%i!<I=<(%U%!%s%/%7%g%s(B:
 *	    error_handler(char *errstr)
 *		ccParseRule() $BCf$G!"%(%i!<$,H/@8$7$?;~$K8F$S=P$5$l$k!#(B
 */

#ifndef lint
static char	*rcsid = "$Id: cconv.c,v 10.2 1994/04/21 07:52:46 ishisone Rel $";
#endif

#include	<stdio.h>
#include	<X11/Xlib.h>
#include	<X11/keysym.h>
#include	<X11/Xutil.h>
#include	<X11/Xos.h>
#include	"cconv.h"

#define uchar	unsigned char
#define ushort	unsigned short
#define ulong	unsigned long

#define Malloc(size)		malloc((unsigned int)(size))
#define Realloc(p, size)	realloc((char *)(p), (unsigned int)(size))
#define Free(p)			{ if (p) (void)free((char *)(p)); }

#define Strcmp(s1, s2)		strcmp((char *)(s1), (char *)(s2))
#define Strncmp(s1, s2, n)	strncmp((char *)(s1), (char *)(s2), n)
#define Strcpy(s1, s2)		strcpy((char *)(s1), (char *)(s2))
#define Strcat(s1, s2)		strcat((char *)(s1), (char *)(s2))
#define Strlen(s)		strlen((char *)(s))

#define EPROC2(efunc, format, a) { \
	char tmp[1024]; \
	(void)sprintf(tmp, format, a); \
	eproc(efunc, tmp); }

/* $B%-!<%3!<%I$N%(%s%3!<%IJ}K!(B */
#define RAWKEY	(1L<<31)
#define PSEUDO	(1L<<30)
#define METAC	(1L<<29)
#define META_ASCII	1	/* any ascii character (not useful) */
#define META_CONTROL	2	/* any control character */
#define META_RAW	3	/* any key event (not useful) */
#define META_ANY	4	/* anything (character or event) */
#define META_FUNC	5	/* any function key event */
#define META_CURSOR	6	/* any cursor key event */
#define META_KEYPAD	7	/* any keypad key event */
#define META_MODIFIER	8	/* any modifier key event */
#define META_PRINTABLE	9	/* any printable character */
#define META_NONASCII	10	/* key event that has no ascii interpretation */

/* pseudo key code */
#define ENTERMODE	(ulong)(PSEUDO|1)
#define EXITMODE	(ulong)(PSEUDO|2)

/* function code */
#define ENDFUNC		0xffff
#define MODECHANGE	0x8000
#define REDO		0x7fff
#define PREVMODE	(MODECHANGE|0x1000)

#define MAXREDO		20

/* key encoding */
#define ccEncodeKey(ev) ((ulong)XLookupKeysym(ev, 0) | (ulong)RAWKEY)
#define ccEncodeMask(ev) ((int)((ev)->state & 0xff))

#define ccEncodeChar(c)	((ulong)(c))

#define NOMODE		0xffff

#define MATCHED_CHAR	0x8080	/* $B$3$l$O(B EUC $B$H$7$F$O(B illegal $B$J%3!<%I$J$N$G(B
				 * $BFCJL$JL\E*$KMQ$$$k(B */
#define CCLEAR_CHAR	0x8081	/* $B>e$KF1$8(B */

#define CANONIC(c)	(((c) >= 'A' && (c) <= 'Z') ? c += ('a' - 'A') : c)

/* $B%G!<%?9=B$(B */
/* $BJQ49%k!<%k(B */
typedef struct convdesc {
	ulong	key;		/* $BF~NO%-!<(B */
	ushort	mask;		/* modifier mask (of X KeyEvent) */
	ushort	context;	/* context$BJ8;zNs(B ($B5U=g$K3JG<$5$l$k(B) */
	ushort	result;		/* $BJQ497k2LJ8;zNs(B */
	ushort	function;	/* function $B%Y%/%?$N%$%s%G%C%/%9(B */
} ConvDesc;

/* $B%b!<%I$4$H$N%F!<%V%k(B */
typedef struct {
	char		*name;		/* $B%b!<%IL>(B */
	int		nrule;		/* $B%k!<%k$N?t(B */
	ConvDesc	*cdbuf;		/* $B%k!<%k(B */
	wchar		*prompt;	/* $B%W%m%s%W%HJ8;zNs(B */
	ushort		fallthrough;
} ModeTable;

/* ccParseRule() $B$GFbItI=8=$KJQ49$5$l$?%k!<%k(B */
typedef struct _ccRule {
	char		*rulefile;	/* $BDj5A%U%!%$%kL>(B */
	ccRule		next;		/* $B%k!<%k%j%9%H$N<!$NMWAG(B */
	int		refcnt;		/* $B;2>H?t(B */
	int		nmode;		/* $B%b!<%I$N?t(B */
	int		initialmode;	/* $B=i4|%b!<%I(B */
	ModeTable	*modes;		/* $B%b!<%I%F!<%V%k(B */
	wchar		*strbuf;	/* $B%9%H%j%s%0%P%C%U%!(B */
	ushort		*funcbuf;	/* $B%U%!%s%/%7%g%s%P%C%U%!(B */
	int		nfunc;		/* $B%U%!%s%/%7%g%s$N?t(B */
	char		*funcnamebuf;	/* $B%U%!%s%/%7%g%sL>$N%P%C%U%!(B */
	void		(*errorfunc)();	/* $B%(%i!<%3!<%k%P%C%/(B */
} ccRuleRec;

static ccRule	ccrules;	/* $BJQ49%k!<%k$N%j%9%H(B */

/* ccCreateBuf() $B$G:n$i$l$kJQ49%P%C%U%!(B */
typedef struct _ccBuf {
	ccRule	rule;			/* $BJQ49%k!<%k(B */
	short	*functbl;
	void	(*defaultaction)();	/* callback functions */
	void	(*insertchar)();
	void	(*deletechar)();
	void	(*executefunction)();
	void	(*modenotify)();
	caddr_t	client_data;
	int	currentmode;		/* $B8=:_$N%b!<%I(B */
	int	previousmode;		/* $B0l$DA0$N%b!<%I(B */
	wchar	*context;
	wchar	*contextend;
	int	contextsize;
} ccBufRec;

#define STRSIZE		200
#define MORESTRSIZE	100
typedef struct {
	wchar	*strbuf;
	wchar	*strend;
	wchar	*strp;
} _strbufRec;

#define	CDSIZE		10
#define	MORECDSIZE	30
typedef struct {
	ConvDesc	*cdbuf;		/* ConvDesc $B%"%l%$(B */
	ConvDesc	*cdend;
	ConvDesc	*cdp;
} _cdbufRec;

#define FUNCSIZE	20
#define MOREFUNCSIZE	20
typedef struct {
	ushort	*funcbuf;	/* $B%U%!%s%/%7%g%s%Y%/%?(B */
	ushort	*funcend;
	ushort	*funcp;
} _funcbufRec;

#define MAXFUNC			1024	/* $B%U%!%s%/%7%g%s$N<oN`$N>e8B(B */
#define FUNCNAMESIZE		100
#define MOREFUNCNAMESIZE	50
typedef struct {
	int	nfunc;
	ushort	funcnames[MAXFUNC];
	char	*funcnamebuf;
	char	*funcnameend;
	char	*funcnamep;
	int	funcsize;
} _funcnameRec;

/* $BDj5A%U%!%$%k(B (for include directive) */
#define MAXINC		10
typedef struct {
	int	findex;
	FILE	*fp[MAXINC];
} Files;

extern char	*malloc();
extern char	*realloc();

#ifdef __STDC__
static int wstrlen(wchar *);
static wchar *wrev(wchar *);
static void eproc(void (*)(), char *);
static wchar *promptsave(wchar *);
static int parseLine(uchar *, uchar **, int);
static FILE *openfile(char *);
static int doinclude(uchar *, Files *, void (*)());
static uchar *getline(uchar *, int, Files *, void (*)());
static int readRuleFile(ccRule, char *);
static int registMode(ccRule, int, uchar **);
static int newMode(ccRule, Files *, _strbufRec *, _funcbufRec *,
		   _funcnameRec *, int, uchar **);
static int getDesc(ccRule, uchar *, _funcbufRec *, _funcnameRec *,
		   ulong *, int *, wchar *, wchar *, int *);
static int getMode(ccRule, uchar *);
static uchar *getQuote(uchar *, wchar *, int);
static int getKey(uchar *, ulong *, int *);
static int getmask(uchar *);
static char *strinc(char *, char *);
static ulong getcode(uchar *);
static int getFunc(ccRule, _funcbufRec *, _funcnameRec *, int, uchar **);
static ccRule findRule(char *);
static void addRule(ccRule);
static void deleteRule(ccRule);
static int wstrsave(_strbufRec *, wchar *);
static int wstralloc(_strbufRec *, int);
static void wstradjust(_strbufRec *);
static ConvDesc *cdalloc(_cdbufRec *);
static void cdadjust(_cdbufRec *);
static int funcalloc(_funcbufRec *, int);
static void funcadjust(_funcbufRec *);
static int funcsearch(_funcnameRec *, char *);
static void funcnameadjust(_funcnameRec *);
static int convchar(ccBuf, ulong, int, char *, int);
static int cconvert(ccBuf, int, ulong, int, int *, char *, int);
static int metamatch(ulong, ulong, int);
static int contextMatch(ccBuf, wchar *);
static void substStr(ccBuf, wchar *, wchar *, char *, int);
#else
static int wstrlen();
static wchar *wrev();
static void eproc();
static wchar *promptsave();
static int parseLine();
static int readRuleFile();
static int registMode();
static int newMode();
static int getDesc();
static int getMode();
static uchar *getQuote();
static int getKey();
static int getmask();
static char *strinc();
static ulong getcode();
static int getFunc();
static int wstrsave();
static int wstralloc();
static void wstradjust();
static ConvDesc *cdalloc();
static void cdadjust();
static int funcalloc();
static void funcadjust();
static int funcsearch();
static void funcnameadjust();
static int convchar();
static int cconvert();
static int metamatch();
static int contextMatch();
static void substStr();
#endif

/*
 *	Private Functions
 */

static int wstrlen(str)
wchar *str;
{
	int	len = 0;

	while (*str++)
		len++;
	return len;
}

static wchar *wrev(s)
wchar *s;
{
	wchar	*str = s;
	wchar	*end = str;
	int	c;

	while (*end++)
		;
	end -= 2;
	while (str < end) {
		c = *str;
		*str++ = *end;
		*end-- = c;
	}
	return s;
}

static void eproc(efunc, msg)
void (*efunc)();
char *msg;
{
	if (efunc == NULL) {
		/* stderr $B$K%W%j%s%H(B */
		(void)fprintf(stderr, "%s\n", msg);
	} else {
		/* $B%(%i!<%O%s%I%i$r8F$V(B */
		(*efunc)(msg);
	}
}

static wchar *promptsave(str)
wchar *str;
{
	int	len = (wstrlen(str) + 1) * sizeof(wchar);
	wchar	*p = (wchar *)Malloc(len);

	if (p != NULL) {
		wchar	*q = p;
		while (*q++ = *str++)
			;
	}
	return p;
}

static int parseLine(line, argv, argvsize)
uchar *line;
uchar **argv;
int argvsize;
{
	int	c, qc;
	int	argc;
	int	state;
#define IN_WORD		1	/* $B%o!<%I$NCf(B */
#define IN_QUOTE	2	/* $B%o!<%I$NCf$G$5$i$K%/%)!<%H$NCf(B */

	qc = 0;		/* not necessary, but for lint */
	argc = 0;
	state = 0;	/* $B%o!<%I$N30(B */
	while (c = *line) {
		/* $B%/%)!<%H$5$l$F$$$J$$6uGrJ8;z$O!"%o!<%I$r=*$i$;$k(B */
		if (state != IN_QUOTE && (c == ' ' || c == '\t' || c == '\n')) {
			/* NULL $B%?!<%_%M!<%H$5$;$k(B */
			*line++ = '\0';
			state = 0;
			continue;
		}
		/* $B%o!<%I$N30$N6uGr0J30$NJ8;z$O!"%o!<%I$N;O$^$j$K$J$k(B */
		if (state == 0) {
			if (argc >= argvsize)
				return argc;
			argv[argc++] = line;
			state = IN_WORD;
		}
		/* $B%P%C%/%9%i%C%7%e$O!"<!$NJ8;z$r%9%-%C%W$5$;$k(B */
		if (c == '\\') {
			/* $B$H$O$$$C$F$b<!$,(B Nul $BJ8;z$J$iOC$OJL(B */
			if (*++line == '\0') {
				*--line = '\0';
				break;
			}
		} else if (state == IN_QUOTE) {
			/* $B%/%)!<%H$O;O$^$j$HF1$8J8;z$G=*$k(B */
			if (c == qc)
				state = IN_WORD;
		} else if (c == '\'' || c == '"') {
			/* $B%/%)!<%H$N30$K%/%)!<%HJ8;z$,$"$l$P%/%)!<%H$N;O$^$j(B */
			state = IN_QUOTE;
			qc = c;
		}
		line++;
	}

	/* $B:G8e$N%/%)!<%H$,JD$8$F$$$J$$$+$b$7$l$J$$(B */
	return state != IN_QUOTE ? argc : argc - 1;
}

/* openfile -- $BDj5A%U%!%$%k$r%*!<%W%s$9$k(B */
static FILE *openfile(file)
char *file;
{
	FILE	*fp;
	char	filename[1024];
	FILE	*fopen();
	char	*getenv();

	if ((fp = fopen(file, "r")) == NULL && *file != '/') {
		char	*p, *q;
		/* $B4D6-JQ?t(B CC_DEF_PATH $B$,$"$l$P$=$N%Q%9$N2<$r(B
		 * $B%5!<%A$9$k(B
		 */
		if ((p = getenv("CC_DEF_PATH")) != NULL) {
			while (*p != '\0') {
				q = filename;
				while (*p != '\0' && *p != ':')
					*q++ = *p++;
				if (*p == ':') p++;
				if (q == filename) continue;
				*q++ = '/';
				*q = '\0';
				(void)Strcat(filename, file);
				if ((fp = fopen(filename, "r")) != NULL)
					return fp;
			}
		}
		/* $B%G%U%)%k%H$N%5!<%A%Q%9(B CCDEFPATH(/usr/lib/X11/ccdef) $B$N(B
		 * $B2<$r%5!<%A$9$k(B
		 */
		(void)Strcpy(filename, CCDEFPATH);
		(void)Strcat(filename, file);
		fp = fopen(filename, "r");
	}
	return fp;
}

/* doinclude -- include $B9T$N=hM}$r$9$k(B */
static int doinclude(line, files, efunc)
uchar *line;
Files *files;
void (*efunc)();
{
	int	argc;
	uchar	*argv[2];
	char	*name;

	argc = parseLine(line, argv, 2);
	if (files->findex > MAXINC - 2) {
		eproc(efunc, "include nesting too deep");
		return -1;
	}
	if (argc < 2) {
		eproc(efunc, "missing include filename");
		return -1;
	}
	name = (char *)argv[1];
	if (*name == '\'' || *name == '"') {
		name++;
		name[strlen(name) - 1] = '\0';
	}
	if ((files->fp[++files->findex] = openfile(name)) == NULL) {
		EPROC2(efunc, "can't open %s", name);
		--files->findex;
		return -1;
	}
	return 0;
}

/* getline -- 1$B9TFI$_9~$`(B ($B$=$N:](B include $B$N=hM}$r9T$J$&(B) */
static uchar *getline(line, linesize, files, efunc)
uchar *line;
int linesize;
Files *files;
void (*efunc)();
{
 redo:
	if (fgets((char *)line, linesize, files->fp[files->findex])) {
		register uchar	*p = line;
		while (*p == ' ' || *p == '\t')
			p++;
		if (!Strncmp(p, "include", 7)) {
			if (doinclude(p, files, efunc) < 0) {
				return NULL;
			} else {
				goto redo;
			}
		}
		return line;
	} else if (files->findex > 0) {
		(void)fclose(files->fp[files->findex]);
		files->findex--;
		goto redo;
	}
	(void)fclose(files->fp[files->findex]);
	return NULL;
}

/* readRuleFile -- $BJQ49%k!<%kDj5A%U%!%$%k$rFI$_9~$`(B */
static int readRuleFile(rule, file)
ccRule rule;
char *file;
{
	FILE		*fp;
	int		moderegistered;
	uchar	line[256], tmp[256];
	uchar	*argv[20];
	int		argc;
	_strbufRec	strrec;
	_funcbufRec	funcrec;
	_funcnameRec	fnrec;
	Files		files;
	void		(*efunc)() = rule->errorfunc;

	if ((fp = openfile(file)) == NULL) {
		EPROC2(efunc, "can't open file %s", file);
		return -1;
	}
	files.findex = 0;
	files.fp[0] = fp;

	moderegistered = 0;

	strrec.strbuf = NULL;
	funcrec.funcbuf = NULL;
	fnrec.nfunc = 0;
	fnrec.funcnamebuf = NULL;

	while (getline(line, sizeof(line), &files, efunc)) {
		(void)Strcpy(tmp, line);
		if ((argc = parseLine(tmp, argv, 20)) == 0)
			continue;

		/* '#' $B$G;O$^$k9T$O%3%a%s%H(B */
		if (*line == '\0' || *line == '\n' || *line == '#') {
			continue;
		} else if (!moderegistered && argc > 1 &&
			   !Strcmp(argv[0], "defmode")) {
			/* $B%b!<%IDj5A9T(B */
			if (registMode(rule, argc, argv) < 0) {
				(void)fclose(fp);
				return -1;
			}
			moderegistered++;
		} else if (!Strcmp(argv[0], "mode") && argc > 2) {
			/* $B$"$k%b!<%I$KBP$9$kJQ49Dj5A(B */
			if (!moderegistered) {
				eproc(efunc, "'mode' before 'defmode'");
				(void)fclose(fp);
				return -1;
			}
			if (newMode(rule, &files, &strrec, &funcrec, &fnrec,
				    argc, argv) < 0)
				return -1;
		} else if (!Strcmp(argv[0], "initialmode") &&
			   argc > 1) {
			if (!moderegistered) {
				eproc(efunc, "'initialmode' before 'defmode'");
				(void)fclose(fp);
				return -1;
			}
			rule->initialmode = getMode(rule, argv[1]);
		} else {
			EPROC2(efunc, "syntax error - %s", line);
		}
	}
	(void)fclose(fp);

	wstradjust(&strrec);
	funcadjust(&funcrec);
	funcnameadjust(&fnrec);

	rule->strbuf = strrec.strbuf;
	rule->funcbuf = funcrec.funcbuf;
	rule->funcnamebuf = fnrec.funcnamebuf;
	rule->nfunc = fnrec.nfunc;
#ifdef DEBUG_CCONV
	dumpAllRules(rule);
#endif

	return 0;
}

static int registMode(rule, ac, av)
ccRule rule;
int ac;
uchar **av;
{
	int		nmode;
	ModeTable	*modes;
	int		i;

	ac--, av++;

	nmode = ac;
	modes = (ModeTable *)Malloc(nmode * sizeof(ModeTable));
	if (modes == NULL) {
		eproc(rule->errorfunc, "can't alloc memory");
		return -1;
	}
	rule->modes = modes;

	for (i = 0; i < nmode; i++) {
		if ((modes[i].name = Malloc(Strlen(av[i]) + 1)) == NULL) {
			eproc(rule->errorfunc, "can't alloc memory");
			Free(modes);
			return -1;
		}
		(void)Strcpy(modes[i].name, av[i]);
		modes[i].nrule = 0;
		modes[i].cdbuf = NULL;
		modes[i].prompt = NULL;
		modes[i].fallthrough = NOMODE;
	}

	rule->nmode = nmode;
	return 0;
}

/* newMode -- $B$"$k%b!<%I$K$D$$$F$N%k!<%k$rFI$_9~$`(B */
static int newMode(rule, files, srec, frec, fnrec, ac, av)
ccRule rule;
Files *files;
_strbufRec *srec;
_funcbufRec *frec;
_funcnameRec *fnrec;
int ac;
uchar **av;
{
	uchar	line[256];
	int		mode;
	ulong		inkey;
	int		modmask;
	wchar		prompt[30], context[100], result[100];
	int		func;
	int		ndesc = 0;
	ModeTable	*mp;
	ConvDesc	*cdp;
	_cdbufRec	cdbuf;
	void		(*efunc)() = rule->errorfunc;

	/* $B%U%)!<%^%C%H$O(B
	 * "mode <$B%b!<%IL>(B> <"$B%W%m%s%W%HJ8;zNs(B"> [fallthrough <$B%b!<%IL>(B>]
	 */
	/* $B%b!<%I$N%A%'%C%/(B */
	if ((mode = getMode(rule, av[1])) < 0) {
		EPROC2(efunc, "illegal modename: %s", av[1]);
		return -1;	/* No Such Mode */
	}
	mp = &rule->modes[mode];

	if (getQuote(av[2], prompt, 0) == NULL) {
		EPROC2(efunc, "illegal prompt: %s", av[2]);
		return -1;
	}
	mp->prompt = promptsave(prompt);
	mp->nrule = 0;
	
	if (ac > 4 && !Strcmp(av[3], "fallthrough")) {
		mp->fallthrough = getMode(rule, av[4]);
	} else {
		mp->fallthrough = NOMODE;
	}

	cdbuf.cdbuf = NULL;

	/* $B%k!<%k$rFI$s$G%9%H%"$9$k(B */
	while (getline(line, sizeof(line), files, efunc)) {
		/* '#' $B$G;O$^$k9T$O%3%a%s%H(B */
		if (*line == '\0' || *line == '\n' || *line == '#')
			continue;
		if (!Strncmp(line, "endmode", 6))
			break;
		if (getDesc(rule, line, frec, fnrec, &inkey, &modmask,
			    context, result, &func)) {
			if ((cdp = cdalloc(&cdbuf)) == NULL) {
				return -1;
			}

			/* $B%k!<%k$N%9%H%"(B */
			cdp->key = inkey;
			cdp->mask = modmask;
			cdp->context = *context ? wstrsave(srec, wrev(context)) : 0;
			cdp->result = *result ? wstrsave(srec, result) : 0;
			cdp->function = func;
			ndesc++;
		} else
			EPROC2(efunc, "illegal description - %s", line);
	}

	/* $B%k!<%k$,#1$D$b$J$1$l$P%(%i!<$K$9$Y$-$@$m$&(B */
	/* $B$H;W$C$?$,Nc$($P(B ASCII $B%b!<%I$N$H$-$K$O%k!<%k$,#1$D$b$J$$$3$H$,(B
	 * $B$"$j$&$k(B
	 */

	cdadjust(&cdbuf);

	mp->nrule = ndesc;
	mp->cdbuf = cdbuf.cdbuf;

	return 0;
}

static int getDesc(rule, line, frec, fnrec, keyp, maskp, context, result, funcp)
ccRule rule;
uchar *line;
_funcbufRec *frec;
_funcnameRec *fnrec;
ulong *keyp;
int *maskp;
wchar *context;
wchar *result;
int *funcp;
{
	uchar	tmp[256];
	uchar	*av[20];
	int		ac;
	void		(*efunc)() = rule->errorfunc;

	/* valid description format is:
		"context"	key	"result"	[function...]
	*/

	(void)Strcpy(tmp, line);
	ac = parseLine(tmp, av, 20);
	if (ac < 3) {
		EPROC2(efunc, "syntax error - %s", line);
		return 0;
	}

	/* context $B$NFI$_9~$_(B */
	if (getQuote(av[0], context, 0) == NULL)
		return 0;

	/* $B%-!<%3!<%I$rFI$_9~$s$G(B */
	if (getKey(av[1], keyp, maskp) < 0) {
		EPROC2(efunc, "no such key (%s)", av[1]);
		return 0;
	}

	/* result $B$rFI$_9~$s$G(B */
	if (getQuote(av[2], result, 1) == NULL)
		return 0;

	/* $B%U%!%s%/%7%g%s$N5-=R$,$"$l$P$=$l$rFI$_9~$`(B */
	/* $B$b$7AjEv$9$k%U%!%s%/%7%g%s$,$J$/$F$b%(%i!<$K$7$J$$(B */
	if (ac > 3) {
		*funcp =  getFunc(rule, frec, fnrec, ac - 3, &av[3]);
	} else {
		*funcp = 0;
	}

	return 1;
}

static int getMode(rule, str)
ccRule rule;
uchar *str;
{
	ModeTable	*modes = rule->modes;
	int	i;

	for (i = 0; i < rule->nmode; i++) {
		if (!Strcmp(str, modes[i].name))
			return i;
	}
	EPROC2(rule->errorfunc, "undefined mode %s", str);
	return -1;
}

/* getQuote -- $B%/%)!<%F!<%7%g%s5-9f$G0O$^$l$?J8;zNs$rFI$s$G(B wchar $B$K$9$k(B */
static uchar *getQuote(line, str, metaf)
uchar	*line;
wchar		*str;
int		metaf;	/* '&' $B$H(B '/' $B$r%a%?!&%-%c%i%/%?$H$9$k$+$I$&$+(B */
{
	int	c;
	int	quote;		/* quote flag */
	int	qc = *line++;	/* quote character */
#define SS2	0x8e
#define SS3	0x8f

	if (qc != '\'' && qc != '"')
		return((uchar *)NULL);
	
	quote = 0;
	while ((c = *line++) && c != qc) {
		if (c == '\\' && !quote) {
			quote = 1;
			continue;
		}
			
		if (c == '^' && !quote) {
			if (c = *line++)
				*str++ = c - '@';
			else
				break;
		} else if (metaf && c == '&' && !quote)
			*str++ = MATCHED_CHAR;
		else if (metaf && c == '/' && !quote)
			*str++ = CCLEAR_CHAR;
		else if (c < 0x80)
			*str++ = c;
		else if (c == SS2)
			*str++ = *line++ | 0x80;
		else if (c == SS3) {
			c = *line++;
			*str++ = (c << 8) | (*line++ & 0x7f) | 0x8000;
		} else {
			*str++ = (c << 8) | *line++ | 0x8080;
		}
	}
	*str = 0;

	return((c == qc) ? line : (uchar *)NULL);
}

/* getKey -- $B%-!<%3!<%I$rFI$`(B */
static int getKey(line, keyp, maskp)
uchar *line;
ulong *keyp;
int *maskp;
{
	/*
	 * $B%-!<%3!<%I$N5-=RK!$O#2DL$j(B
	 * 1. ASCII $BI=5-(B
	 *	'a'
	 *	'^H'
	 *	'\033'
	 *	'\xff'
	 *
	 * 2. XKEY $BI=5-(B
	 *	#124
	 *	#0132
	 *	#0x58
	 *	shift-A
	 *	shift-control-meta-HENKAN
	 *
	 * pseudo code
	 *	ENTERMODE
	 *	EXITMODE
	 *
	 * wild character
	 *	@ascii
	 *	@control
	 *	@raw
	 *	@any
	 */

	int	key = 0;

	*maskp = 0;

	if (*line == '\'') {	/* $B%7%s%0%k%/%)!<%H$G;O$^$k$N$G(B ASCII $BI=5-(B */
		if (*++line == '\\') {
			/* '\'' $B$N>l9g(B($B%7%s%0%k%/%)!<%H<+?H(B)$B!"(B
			 * '\\' $B$N>l9g(B($B%P%C%/%9%i%C%7%e(B)
			 * '\033' $B$N$h$&$J#8?JI=5-$N>l9g$H(B
			 * '\x27' $B$N$h$&$J#1#6?JI=5-$N>l9g$,$"$k(B
			 */
			if (*++line == '\'')	/* '\'' */
				key = '\'';
			else if (*line == '\\')	/* '\\' */
				key = '\\';
			else if (*line == 'x')	/* hexadecimal */
				(void)sscanf((char *)++line, "%x", &key);
			else			/* octal */
				(void)sscanf((char *)line, "%o", &key);
			key &= 0xff;
		} else if (*line == '^') {
			/* '^' ($B%+%l%C%H<+?H(B) $B$^$?$O%3%s%H%m!<%k%3!<%I(B */
			if (*++line == '\'')
				key = '^';
			else
				key = *line - '@';
		} else {
			key = *line;
		}
		*keyp = key;
	} else if (*line == '#') {	/* event code */
		if (*++line == '0') {	/* octal or hexadecimal */
			if (*(line + 1) == 'x')	/* hexadecimal */
				(void)sscanf((char *)line + 2, "%x", &key);
			else			/* octal */
				(void)sscanf((char *)line, "%o", &key);
		} else {
			key = atoi((char *)line);
		}
		*keyp = (ulong)key | (ulong)RAWKEY;
	} else if (!Strcmp(line, "ENTERMODE")) {
		*keyp = ENTERMODE;
	} else if (!Strcmp(line, "EXITMODE")) {
		*keyp = EXITMODE;
	} else if (*line == '@') {
		/* $B%o%$%k%I!&%-%c%i%/%?(B */
		line++;
		if (!Strcmp(line, "ascii"))
			key = META_ASCII;
		else if (!Strcmp(line, "printable"))
			key = META_PRINTABLE;
		else if (!Strcmp(line, "control"))
			key = META_CONTROL;
		else if (!Strcmp(line, "raw"))
			key = META_RAW;
		else if (!Strcmp(line, "any"))
			key = META_ANY;
		else if (!Strcmp(line, "func"))
			key = META_FUNC;
		else if (!Strcmp(line, "cursor"))
			key = META_CURSOR;
		else if (!Strcmp(line, "keypad"))
			key = META_KEYPAD;
		else if (!Strcmp(line, "modifier"))
			key = META_MODIFIER;
		else if (!Strcmp(line, "non-ascii"))
			key = META_NONASCII;
		else
			return -1;
		*keyp = (ulong)key | (ulong)METAC;
	} else {
		if ((key = getcode(line)) == 0)
			return -1;
		*keyp = (ulong)key | (ulong)RAWKEY;
		*maskp = getmask(line);
	}
	return 0;
}

/* getmask -- $B%b%G%#%U%!%$%"!&%^%9%/$rJV$9(B */
static int getmask(s)
uchar *s;
{
	int	mask = 0;
	char	buf[256];
	uchar	*p;

	if ((p = (uchar *)rindex((char *)s, '-')) == NULL)
		return 0;
	(void)strncpy(buf, (char *)s, p - s);
	buf[p - s] = '\0';
	
	if (strinc(buf, "shift"))
		mask |= ShiftMask;
	if (strinc(buf, "control"))
		mask |= ControlMask;
	if (strinc(buf, "lock"))
		mask |= LockMask;
	if (strinc(buf, "mod1"))
		mask |= Mod1Mask;
	if (strinc(buf, "mod2"))
		mask |= Mod2Mask;
	if (strinc(buf, "mod3"))
		mask |= Mod3Mask;
	if (strinc(buf, "mod4"))
		mask |= Mod4Mask;
	if (strinc(buf, "mod5"))
		mask |= Mod5Mask;
	return mask;
}

static char *strinc(s, k)
char *s;
char *k;
{
	register int	len = Strlen(k);

	while (s = index(s, *k))
		if (!Strncmp(s, k, len))
			return s;
		else
			s++;
	return NULL;
}


/* getcode -- KeySym $B$rJV$9(B */
static ulong getcode(s)
uchar *s;
{
	register uchar	*t;
	KeySym			keysym;
	KeySym			XStringToKeysym();

	if ((t = (uchar *)rindex((char *)s, '-')) == NULL)
		t = s;
	else
		t++;
	keysym = XStringToKeysym((char *)t);
	if (keysym == NoSymbol)
		return (ulong)0;
	else
		return (ulong)keysym;
}

static int getFunc(rule, frec, fnrec, n, args)
ccRule rule;
_funcbufRec *frec;
_funcnameRec *fnrec;
int	n;
uchar	**args;
{
	int	i, j;
	uchar	*func;
	uchar	*arg;
	ushort	*fp;
	int	findex;
	void	(*efunc)() = rule->errorfunc;

	findex = funcalloc(frec, n + 1);
	fp = frec->funcbuf + findex;

	j = 0;
	while (n > 0) {
		func = *args++;
		if (!Strcmp(func, "goto")) {
			/* $B%b!<%I$NJQ99(B */
			if (n < 2)
			    break;	/* $B%b!<%IL>$,=q$$$F$J$$(B */
			arg = *args++;
			--n;
			if (!Strcmp(arg, "PREV")) {
				fp[j++] = PREVMODE;
				break;
			}
			if ((i = getMode(rule, arg)) < 0) {
				break;
			}
			fp[j++] = MODECHANGE | i;
			break;
		} else if (!Strcmp(func, "redo")) {
			fp[j++] = REDO;
			break;
		} else {
			/* $B%U%!%s%/%7%g%sI=$N8!:w(B */
			int	fnum;
			if ((fnum = funcsearch(fnrec, (char *)func)) < 0) {
				EPROC2(efunc, "too many functions (> %d)", MAXFUNC);
			} else {
				fp[j++] = fnum;
			}
		}
		--n;
	}
	fp[j++] = ENDFUNC;
	frec->funcp = fp + j;	/* kludge */
	return findex;
}

static ccRule
findRule(rulefile)
char *rulefile;
{
	ccRule	rule = ccrules;

	while (rule) {
		if (rule->rulefile && !strcmp(rulefile, rule->rulefile)) {
			return rule;
		}
		rule = rule->next;
	}
	return NULL;
}

static void
addRule(rule)
ccRule rule;
{
	rule->refcnt = 1;
	rule->next = ccrules;
	ccrules = rule;
}

static void
deleteRule(rule)
ccRule	rule;
{
	ccRule	rp = ccrules;
	ccRule	rp0 = NULL;

	while (rp) {
		if (rule == rp) {
			if (rp0 == NULL) {
				ccrules = rp->next;
			} else {
				rp0->next = rp->next;
			}
			return;
		}
		rp0 = rp;
		rp = rp->next;
	}
}

static int wstrsave(srec, str)
_strbufRec *srec;
wchar *str;
{
	int	len = wstrlen(str);
	int	pos;
	wchar	*p;

	pos = wstralloc(srec, len + 1);
	if (pos > 0) {
		p = srec->strbuf + pos;
		while (len-- > 0) {
			*p++ = *str++;
		}
		*p = 0;
	}
	return pos;
}

/* $B%P%C%U%!%"%m%1!<%H%U%!%s%/%7%g%s(B */

static int wstralloc(srec, len)
_strbufRec *srec;
int len;
{
	int	ret;

	if (srec->strbuf == NULL) {
		/* allocate srec->strbuf */
		srec->strbuf = (wchar *)Malloc(STRSIZE * sizeof(wchar));
		if (srec->strbuf == NULL)
			return 0;
		srec->strend = srec->strbuf + STRSIZE;
		srec->strp = srec->strbuf;
		*srec->strp++ = 0;	/* dummy */
	}
	if (srec->strp + len > srec->strend) {
		/* allocate more memory */
		int	size = (srec->strp + len) - srec->strend;
		int	offset = srec->strp - srec->strbuf;
		wchar	*wp;

		if (size < MORESTRSIZE)
			size = MORESTRSIZE;
		size += srec->strend - srec->strbuf;
		wp = (wchar *)Realloc(srec->strbuf, size * sizeof(wchar));
		if (wp == NULL)
			return 0;
		srec->strp = wp + offset;
		srec->strbuf = wp;
		srec->strend = wp + size;
	}

	ret = srec->strp - srec->strbuf;
	srec->strp += len;

	return ret;
}

static void wstradjust(srec)
_strbufRec *srec;
{
	int	size = srec->strp - srec->strbuf;
	wchar	*wp;

	if (size == 0) return;
	wp = (wchar *)Realloc(srec->strbuf, size * sizeof(wchar));
	if (wp != NULL) {
		srec->strbuf = wp;
		srec->strp = srec->strend = wp + size;
	}
}

static ConvDesc *cdalloc(crec)
_cdbufRec *crec;
{
	ConvDesc	*ret;

	if (crec->cdbuf == NULL) {
		crec->cdbuf = (ConvDesc *)Malloc(CDSIZE * sizeof(ConvDesc));
		if (crec->cdbuf == NULL)
			return NULL;
		crec->cdend = crec->cdbuf + CDSIZE;
		crec->cdp = crec->cdbuf;
	}
	if (crec->cdp >= crec->cdend) {
		int	size = crec->cdend - crec->cdbuf + MORECDSIZE;
		int	offset = crec->cdp - crec->cdbuf;
		ConvDesc	*cdp;

		cdp = (ConvDesc *)Realloc(crec->cdbuf, size * sizeof(ConvDesc));
		if (cdp == NULL) {
			return NULL;
		}
		crec->cdp = cdp + offset;
		crec->cdbuf = cdp;
		crec->cdend = cdp + size;
	}

	ret = crec->cdp++;
	return ret;
}

static void cdadjust(crec)
_cdbufRec *crec;
{
	int		size = crec->cdp - crec->cdbuf;
	ConvDesc	*cdp;

	if (size == 0) return;
	cdp = (ConvDesc *)Realloc(crec->cdbuf, size * sizeof(ConvDesc));
	if (cdp != NULL) {
		crec->cdbuf = cdp;
		crec->cdp = crec->cdend = cdp + size;
	}
}

static int funcalloc(frec, n)
_funcbufRec *frec;
int n;
{
	int	ret;

	if (frec->funcbuf == NULL) {
		/* allocate funcbuf */
		frec->funcbuf = (ushort *)Malloc(FUNCSIZE * sizeof(ushort));
		if (frec->funcbuf == NULL)
			return 0;
		frec->funcend = frec->funcbuf + FUNCSIZE;
		frec->funcp = frec->funcbuf;
		*(frec->funcp)++ = ENDFUNC;	/* dummy */
	}
	if (frec->funcp + n > frec->funcend) {
		/* allocate more memory */
		int	size = (frec->funcp + n) - frec->funcend;
		int	offset = frec->funcp - frec->funcbuf;
		ushort	*up;

		if (size < MOREFUNCSIZE)
			size = MOREFUNCSIZE;
		size += frec->funcend - frec->funcbuf;
		up = (ushort *)Realloc(frec->funcbuf, size * sizeof(ushort));
		if (up == NULL)
			return 0;
		frec->funcp = up + offset;
		frec->funcbuf = up;
		frec->funcend = up + size;
	}

	ret = frec->funcp - frec->funcbuf;
	frec->funcp += n;

	return ret;
}

static void funcadjust(frec)
_funcbufRec *frec;
{
	int	size = frec->funcp - frec->funcbuf;
	ushort	*fp;

	if (size == 0) return;
	fp = (ushort *)Realloc(frec->funcbuf, size * sizeof(ushort));
	if (fp != NULL) {
		frec->funcbuf = fp;
		frec->funcp = frec->funcend = fp + size;
	}
}

static int funcsearch(fnrec, funcname)
_funcnameRec *fnrec;
char *funcname;
{
	int	nfunc = fnrec->nfunc;
	ushort	*fnames = fnrec->funcnames;
	char	*fnbuf = fnrec->funcnamebuf;
	int	i;
	int	len;

	for (i = 0; i < nfunc; i++) {
		if (!strcmp(funcname, fnbuf + *fnames++))
			return i;
	}

	if (nfunc >= MAXFUNC)
		return -1;

	len = strlen(funcname) + 1;

	/* add new function */
	if (fnrec->funcnamebuf == NULL) {
		/* allocate funcnamebuf */
		if ((fnrec->funcnamebuf = Malloc(FUNCNAMESIZE)) == NULL)
			return -1;
		fnrec->funcnameend = fnrec->funcnamebuf + FUNCNAMESIZE;
		fnrec->funcnamep = fnrec->funcnamebuf;
	}
	if (fnrec->funcnamep + len > fnrec->funcnameend) {
		/* allocate more memory */
		int	size = (fnrec->funcnamep + len) - fnrec->funcnameend;
		int	offset = fnrec->funcnamep - fnrec->funcnamebuf;
		char	*cp;

		if (size < MOREFUNCNAMESIZE)
			size = MOREFUNCNAMESIZE;
		size += fnrec->funcnameend - fnrec->funcnamebuf;
		if ((cp = Realloc(fnrec->funcnamebuf, size)) == NULL)
			return 0;
		fnrec->funcnamep = cp + offset;
		fnrec->funcnamebuf = cp;
		fnrec->funcnameend = cp + size;
	}

	(void)strcpy(fnrec->funcnamep, funcname);
	fnrec->funcnames[nfunc] = fnrec->funcnamep - fnrec->funcnamebuf;
	fnrec->funcnamep += len;

	return fnrec->nfunc++;
}

static void funcnameadjust(fnrec)
_funcnameRec *fnrec;
{
	int	size = fnrec->funcnamep - fnrec->funcnamebuf;
	char	*cp;

	if (size == 0) return;
	if (cp = Realloc(fnrec->funcnamebuf, size)) {
		fnrec->funcnamebuf = cp;
		fnrec->funcnamep = fnrec->funcnameend = cp + size;
	}
}

static int convchar(buf, key, mask, str, len)
ccBuf buf;
ulong key;	/* keysym (RAWKEY) or ascii code */
int mask;	/* modifier mask */
char *str;	/* ascii interpretation */
int len;	/* length of str */
{
	int	r;
	int	func;
	int	redocount = 0;
	ushort	*fp;
	ccRule	rule = buf->rule;

 redo:
	/* $B$^$:$O8=:_$N%b!<%I$N%k!<%k$GJQ49$7$F$_$k(B */
	r = cconvert(buf, buf->currentmode, key, mask, &func, str, len);
	if (r < 0) {
		/* $B%^%C%A$9$k%k!<%k$,8+$D$+$i$J$+$C$?$N$G(B
		 * fallthrough $B$G;XDj$5$l$k%b!<%I$N%k!<%k$rC5$9(B
		 */
		int	tmpmode = rule->modes[buf->currentmode].fallthrough;

		while (tmpmode != NOMODE) {
			r = cconvert(buf, tmpmode, key, mask, &func, str, len);
			if (r >= 0)	/* $B%^%C%A$7$?(B */
				break;
			tmpmode = rule->modes[tmpmode].fallthrough;
		}
	}

	if (r < 0)
		return -1;	/* $B$I$l$K$b%^%C%A$7$J$+$C$?(B */

	if (func == 0)
		return 0;	/* $B2?$b$7$J$$(B */

	fp = rule->funcbuf + func;
	while ((func = *fp++) != ENDFUNC) {
		if (func == REDO) {
			if (redocount++ > MAXREDO)
			    return -1;	/* $B$?$V$sL58B%k!<%W(B */
			else
			    goto redo;	/* redo -- $B$b$&0lEY(B */
		} else if (func & MODECHANGE) {	/* $B%+%l%s%H%b!<%I$NJQ99(B */
			int	tmpmode = buf->currentmode;
			
			/* pseudo-key $B$NF~NO(B */
			(void)convchar(buf, EXITMODE, 0, (char *)NULL, 0);
			
			if (func == PREVMODE) {
				buf->currentmode = buf->previousmode;
			} else {
				buf->currentmode = func & ~MODECHANGE;
			}
			buf->previousmode = tmpmode;
			
			/* pseudo-key $B$NF~NO(B */
			(void)convchar(buf, ENTERMODE, 0, (char *)NULL, 0);
			
			/* $B%b!<%I$,JQ$o$C$?;~$K$O%3%s%F%-%9%H$r%/%j%"$9$k(B */
			ccContextClear(buf);

			/* $B%b!<%IJQ99%3!<%k%P%C%/$,$"$l$P8F$S=P$9(B */
			if (buf->modenotify) {
				(*buf->modenotify)(buf->currentmode,
						  buf->previousmode,
						  buf->client_data);
			}
			break;
		} else {
			int	truefunc = buf->functbl[func];
			/* $B%G%#%9%Q%C%A!&%k!<%A%s$r8F$V(B */
			if (truefunc >= 0) {
				(*buf->executefunction)(truefunc, str, len,
							buf->client_data);
			}
		}
	}
	return 0;
}

static int cconvert(buf, mode, inkey, mask, func, str, len)
ccBuf buf;
int mode;	/* current mode */
ulong inkey;	/* input key (raw/mapped) */
int mask;
int *func;	/* function */
char *str;
int len;
{
	ccRule		rule = buf->rule;
	ConvDesc	*entry;
	ulong		key;
	int		n;
	ModeTable	*modep;

	if (mode < 0 || mode >= rule->nmode)
		return -1;	/* No Such Mode */

	modep = &rule->modes[mode];

	if ((n = modep->nrule) <= 0)
		return -1;	/* No Rules */

	for (entry = modep->cdbuf; --n >= 0; entry++) {
		key = entry->key;
		if (key & (ulong)METAC) {
			/* $B%o%$%k%I%+!<%IJ8;z$N%^%C%A%s%0(B */
			if (!metamatch(key, inkey, len == 0))
				continue;
		} else if (key & (ulong)RAWKEY && mask != entry->mask) {
			continue;
		} else if (key != inkey) {
			continue;
		}

		/* $B%-!<$,%^%C%A$7$?(B */
		if (contextMatch(buf, rule->strbuf + entry->context)) {
			substStr(buf, rule->strbuf + entry->context,
				 rule->strbuf + entry->result, str, len);
			*func = entry->function;
			return 0;
		}
	}

	return -1;	/* No Match */
}

static int metamatch(rkey, inkey, nonascii)
ulong rkey;
ulong inkey;
int nonascii;
{
	int	type = (int)(rkey & 0xff);

	switch (type) {
	case META_ASCII:
		return !(inkey & (ulong)(RAWKEY|PSEUDO));
	case META_PRINTABLE:
		return (0x20 <= inkey && inkey < 0x7f);
	case META_CONTROL:
		return inkey < 0x20;
	case META_RAW:
		return inkey & (ulong)RAWKEY;
	case META_ANY:
		return 1;
	case META_FUNC:
		return (inkey & (ulong)RAWKEY) && IsFunctionKey(inkey & 0xffff);
	case META_CURSOR:
		return (inkey & (ulong)RAWKEY) && IsCursorKey(inkey & 0xffff);
	case META_KEYPAD:
		return (inkey & (ulong)RAWKEY) && IsKeypadKey(inkey & 0xffff);
	case META_MODIFIER:
		return (inkey & (ulong)RAWKEY) && IsModifierKey(inkey & 0xffff);
	case META_NONASCII:
		return (inkey & (ulong)RAWKEY) && nonascii;
	default:
		return 0;
	}
	/* NOTREACHED */
}

static int contextMatch(buf, context)
ccBuf buf;
wchar *context;
{
	wchar	*c0 = buf->contextend;
	wchar	*c1 = buf->context;

	if (context == 0 || *context == 0)
		return 1;	/* $BL5>r7o%^%C%A(B */
	if (c0 - c1 < wstrlen(context))	/* $BD9$5$N%A%'%C%/(B */
		return 0;	/* matching fail */
	c0--;
	while (*context) {
		if (CANONIC(*c0) == CANONIC(*context))
			c0--, context++;
		else
			return 0;	/* fail */
	}
	return 1;
}

static void substStr(buf, context, result, str, len)
ccBuf buf;
wchar *context;
wchar *result;
char *str;
int len;
{
	register int	c;
	int		nbytes;
	uchar	*bufp;

	/* $B%3%s%F%-%9%H$NJ,$r>C$9(B */
	while (*context++) {
		ccContextDelete(buf);
		(*buf->deletechar)(buf->client_data);
	}

	while (c = *result++) {
		if (c == MATCHED_CHAR) {
			nbytes = len;
			bufp = (uchar *)str;
			while (nbytes-- > 0) {
				c = *bufp++;
				/* bufp $B$NCf?H$O(B ASCII $B$+(B $B%+%J$J$N$G(B
				 * EUC $B%W%m%;%9%3!<%I$X$NJQ49$r$o$6$o$6(B
				 * $B$d$kI,MW$O$J$$(B
				 */
				ccContextAppend(buf, c);
				(*buf->insertchar)(c, buf->client_data);
			}
			continue;
		} else if (c == CCLEAR_CHAR) {
			/* $B%3%s%F%-%9%H$r%/%j%"$9$k(B */
			ccContextClear(buf);
			continue;
		}
		ccContextAppend(buf, c);
		(*buf->insertchar)(c, buf->client_data);
	}
}

/* getModeSwitchMask -- mode-switch $B$N%b%G%#%U%!%$%"%^%9%/$rD4$Y$k(B */
int getModeSwitchMask(dpy)
Display *dpy;
{
	KeyCode	modeswkey;
	struct modesw {
		Display		*dpy;
		int		mode_switch;
		struct modesw	*next;
	} *msp;
	static struct modesw	*modeswlist;

	for (msp = modeswlist; msp != NULL; msp = msp->next) {
		if (dpy == msp->dpy) return msp->mode_switch;
	}
	msp = (struct modesw *)Malloc(sizeof(struct modesw));
	msp->dpy = dpy;
	msp->next = modeswlist;
	modeswlist = msp;

	msp->mode_switch = 0;

	if ((modeswkey = XKeysymToKeycode(dpy, XK_Mode_switch)) != 0) {
		XModifierKeymap	*map = XGetModifierMapping(dpy);
		int	keypermod = map->max_keypermod;
		int	modbit;
		int	i;

		for (modbit = 3; modbit < 8; modbit++) {
			for (i = 0; i < keypermod; i++) {
				if (map->modifiermap[keypermod * modbit + i] == modeswkey) {
					msp->mode_switch = 1 << modbit;
					goto found;
				}
			}
		}
	found:
		XFreeModifiermap(map);
	}

	return msp->mode_switch;
}

/*
 *	Public Functions
 */

/* ccParseRule -- $BJQ49Dj5A%U%!%$%k$rFI$_9~$`(B */
ccRule ccParseRule(deffile, errprint)
char *deffile;		/* $BF~NOJ8;zJQ49Dj5A%U%!%$%k(B */
void (*errprint)();	/* $B%(%i!<%a%C%;!<%8I=<(MQ%3!<%k%P%C%/(B */
{
	ccRule		rule;
	extern char	*getenv();

	/* $BJQ49Dj5A%U%!%$%k$NFI$_9~$_(B */
	if (deffile == NULL) {
		/* $B4D6-JQ?t(B CC_DEF $B$rD4$Y$k(B */
		if ((deffile = getenv("CC_DEF")) == NULL) {
			return (ccRule)NULL;
		}
	}

	if (rule = findRule(deffile)) {
		/* same rule found */
		rule->refcnt++;
		return rule;
	}

	/* ccRuleRec $B$N%"%m%1!<%H(B */
	if ((rule = (ccRule)Malloc(sizeof(ccRuleRec))) == NULL) {
		return (ccRule)NULL;
	}

	rule->errorfunc = errprint;
	rule->rulefile = Malloc(Strlen(deffile) + 1);
	if (rule->rulefile) {
		(void)Strcpy(rule->rulefile, deffile);
	}

	if (readRuleFile(rule, deffile) < 0) {
		Free(rule);
		return (ccRule)NULL;
	}

	addRule(rule);

	return rule;
}

/* ccCreateBuf -- $BJQ49%P%C%U%!$r:n$k(B */
ccBuf ccCreateBuf(rule, csize, functable, nfunc,
		  def_action, insert, delete, execute, modenotify, data)
ccRule rule;		/* $B;HMQ$9$kF~NOJ8;zJQ49%k!<%k(B */
int csize;		/* context size ($BJ8;z?t(B) */
char *functable[];	/* $B%U%!%s%/%7%g%s!&%F!<%V%k(B */
int nfunc;		/* functable $B$N%(%s%H%j?t(B */
void (*def_action)();	/* $B%G%U%)%k%H!&%"%/%7%g%s!&%3!<%k%P%C%/(B */
void (*insert)();	/* $BJ8;zF~NO%3!<%k%P%C%/(B */
void (*delete)();	/* $BJ8;z:o=|%3!<%k%P%C%/(B */
void (*execute)();	/* $B%U%!%s%/%7%g%s<B9T%3!<%k%P%C%/(B */
void (*modenotify)();	/* $B%b!<%IJQ99DLCN%3!<%k%P%C%/(B */
caddr_t data;		/* callback $B%G!<%?(B */
{
	ccBuf	buf;
	char	*funcnamep;
	short	*functblp;
	int	i, j;

	/* ccBuf $B$N%"%m%1!<%H(B */
	if ((buf = (ccBuf)Malloc(sizeof(ccBufRec))) == NULL) {
		return (ccBuf)NULL;
	}

	buf->rule = rule;

	/* context $B%P%C%U%!$N%"%m%1!<%H(B */
	if (csize <= 0) csize = 1;
	buf->context = (wchar *)Malloc(csize * sizeof(wchar));
	if (buf->context == NULL) {
		Free(buf);
		return (ccBuf)NULL;
	}
	buf->contextend = buf->context;
	buf->contextsize = csize;

	/* function $B%3%s%P!<%H%F!<%V%k$N%"%m%1!<%H(B */
	buf->functbl = (short *)Malloc(rule->nfunc * sizeof(short));
	if (buf->functbl == NULL) {
		Free(buf->context);
		Free(buf);
		return (ccBuf)NULL;
	}
	/* ccRule $B$KF~$C$F$$$k%U%!%s%/%7%g%sI=$H!"0z?t$GM?$($i$l$?(B
	 * $B%U%!%s%/%7%g%sI=$+$i!"(BccRule $BFbIt$N%U%!%s%/%7%g%sHV9f$H(B
	 * $B:#M?$($i$l$?%U%!%s%/%7%g%sHV9f$H$NBP1~I=$r:n$k(B
	 */
	funcnamep = rule->funcnamebuf;
	functblp = buf->functbl;
	for (i = rule->nfunc, functblp = buf->functbl; i > 0; i--, functblp++) {
		for (j = 0; j < nfunc; j++) {
			if (!strcmp(functable[j], funcnamep)) {
				*functblp = j;
				break;
			}
		}
		if (j >= nfunc) *functblp = -1;

		while (*funcnamep++)
			;
	}

	buf->defaultaction = def_action;
	buf->insertchar = insert;
	buf->deletechar = delete;
	buf->executefunction = execute;
	buf->modenotify = modenotify;
	buf->client_data = data;

	/* $B%+%l%s%H%b!<%I$N@_Dj(B */
	buf->previousmode = buf->currentmode = rule->initialmode;

	return buf;
}

/* ccFreeRule -- $B%k!<%k$r<N$F$k(B */
void ccFreeRule(rule)
ccRule rule;
{
	ModeTable	*modep;
	int		i;

	if (rule == NULL) return;
	if (--rule->refcnt > 0) return;

	deleteRule(rule);

	for (modep = rule->modes, i = 0; i < rule->nmode; modep++, i++) {
		Free(modep->name);
		Free(modep->cdbuf);
		Free(modep->prompt);
	}
	Free(rule->rulefile);
	Free(rule->modes);
	Free(rule->strbuf);
	Free(rule->funcbuf);
	Free(rule->funcnamebuf);
	Free(rule);
}

/* ccDestroyBuf -- $BJQ49%P%C%U%!$r<N$F$k(B */
void ccDestroyBuf(buf)
ccBuf buf;
{
	if (buf == NULL) return;
	Free(buf->context);
	Free(buf->functbl);
	Free(buf);
}

/* ccConvchar -- $BJQ49MQ%U%!%s%/%7%g%s(B */
int ccConvchar(buf, event)
ccBuf buf;
XKeyPressedEvent *event;	/* $B%-!<%$%Y%s%H(B */
{
	int	r;
	char	str[256];
	char	*p;
	int	nbytes;
	int	n;
	KeySym	ks1, ks2;
	int	mask;

	/* $B$H$j$"$($:(B LookupString $B$7$F$*$/(B */
	nbytes = XLookupString(event, str, sizeof(str), &ks2, 0);

	/* $B$^$:$O%$%Y%s%H%3!<%I$GJQ49$7$F$_$k(B */
	r = -1;
	mask = ccEncodeMask(event);

	/* $B$^$:$O%b%G%#%U%!%$%"$r0l:]9MN8$7$J$$(B KeySym $B$r(B
	 * $B%3!<%I$H$7$FJQ49$7$F$_$k(B
	 */
	ks1 = (ulong)XLookupKeysym(event, 0);
	if (ks1 != NoSymbol) {
		r = convchar(buf, (ulong)ks1 | (ulong)RAWKEY,
			     mask, str, nbytes);
	}

	/* $B>e$,%^%C%A$7$J$1$l$P!"(BShift, Lock, ModeSwitch $B$r(B
	 * $B9MN8$7$?(B KeySym ($B$D$^$j(B XLookupString() $B$,JV$9(B KeySym) $B$,(B
	 * NoSymbol $B$G$J$1$l$P$=$N%3!<%I$GJQ49$7$F$_$k(B
	 */
	if (r < 0 && ks2 != NoSymbol) {
		int	msw = getModeSwitchMask(event->display);

		if (mask & (ShiftMask | LockMask | msw)) {
			mask &= ~(ShiftMask | LockMask | msw);
			r = convchar(buf, (ulong)ks2 | (ulong)RAWKEY,
				     mask, str, nbytes);
		}
	}

	if (r < 0) {
		int	match = 0;

		if (nbytes == 0) return 0;

		/* ASCII $BI=5-$GJQ49$7$F$_$k(B */
		p = str;
		n = nbytes;
		while (n-- > 0) {
			r = convchar(buf, ccEncodeChar(*p), 0, p, 1);
			if (r >= 0) match = 1;
			p++;
		}
		if (!match) {
			/* default action $B$N%U%!%s%/%7%g%s$r$h$V(B */
			if (buf->defaultaction != NULL) {
				(void)(*buf->defaultaction)(str, nbytes, buf->client_data);
			}
			return 0;
		}
	}
	return 1;
}

/* ccGetMode -- $B8=:_$N%b!<%IHV9f$rJV$9(B */
int ccGetMode(buf)
ccBuf buf;
{
	return buf->currentmode;
}

/* ccGetModePrompt -- $B8=:_$N%b!<%I$N%W%m%s%W%HJ8;zNs$rJV$9(B */
wchar *ccGetModePrompt(buf)
ccBuf buf;
{
	return buf->rule->modes[buf->currentmode].prompt;
}

/* ccGetRule -- $BJQ49%P%C%U%!$G;H$o$l$F$$$kJQ49%k!<%k$rJV$9(B */
ccRule ccGetRule(buf)
ccBuf buf;
{
	return buf->rule;
}

/* ccContextAppend -- $B%3%s%F%-%9%H$K(B1$BJ8;z2C$($k(B */
void ccContextAppend(buf, c)
ccBuf buf;
int c;
{
	wchar	*p;

	/* $B%3%s%F%-%9%HJ8;zNs$KA^F~(B */
	if (buf->contextend - buf->context < buf->contextsize) {
		*buf->contextend++ = c;
	} else {
		/* $B%3%s%F%-%9%HJ8;zNs$,0lGU$J$N$G(B
		 * $B@hF,$N#1J8;z$r<N$F$F$D$a$k(B
		 */
		p = buf->context + 1;
		while (p < buf->contextend) {
			*(p - 1) = *p;
			p++;
		}
		/* $B$"$$$?=j$KA^F~(B */
		*--p = c;
	}
}

/* ccContextDelete -- $B%3%s%F%-%9%H$r(B1$BJ8;z:o=|$9$k(B */
void ccContextDelete(buf)
ccBuf buf;
{
	if (buf->contextend > buf->context)
		buf->contextend--;
}

/* ccContextClear -- $B%3%s%F%-%9%H$r%/%j%"$9$k(B */
void ccContextClear(buf)
ccBuf buf;
{
	/* $B%b!<%I$,JQ$o$C$?;~$K$O<+F0E*$K%/%j%"$5$l$k$,$=$l0J30$K(B
	 * $B8=:_$N%3%s%F%-%9%H$r6/@)E*$K%/%j%"$7$?$$>l9g$KMQ$$$k(B
	 */
	buf->contextend = buf->context;
}

/* ccContextSet -- $B%3%s%F%-%9%H$r%;%C%H$9$k(B */
void ccContextSet(buf, cstr)
ccBuf buf;
wchar *cstr;
{
	int	len = wstrlen(cstr);
	wchar	*p = buf->context;

	if (len > buf->contextsize) {
		cstr += len - buf->contextsize;
		len = buf->contextsize;
	}
	while (len-- > 0) {
		*p++ = *cstr++;
	}
}

/* ccContextGet -- $B8=:_$N%3%s%F%-%9%H$rJV$9(B */
void ccContextGet(buf, cstr)
ccBuf buf;
wchar	*cstr;
{
	register wchar	*wp = buf->context;

	while (wp < buf->contextend)
		*cstr++ = *wp++;
	*cstr = 0;
}


/*
 *	Obsolete Functions
 */

/* ccInit -- $BJQ49%k!<%k$rFI$_9~$s$G%P%C%U%!$r:n$k(B */
ccBuf ccInit(deffile, contextsize, defactfunc, insertfunc, deletefunc, dofunc,
	     errprint, functable, functablesize)
char *deffile;
int contextsize;
void (*defactfunc)();
void (*insertfunc)();
void (*deletefunc)();
void (*dofunc)();
void (*errprint)();
char *functable[];
int functablesize;
{
	ccRule	rule;

	if ((rule = ccParseRule(deffile, errprint)) == NULL) {
		return (ccBuf)NULL;
	}

	return ccCreateBuf(rule, contextsize, functable, functablesize,
			   defactfunc, insertfunc, deletefunc, dofunc,
			   (void (*)())NULL, (caddr_t)NULL);
}

/* ccTerminate -- $BF~NOJ8;zJQ49$r=*N;$9$k(B */
void ccTerminate(buf)
ccBuf buf;
{
	ccFreeRule(buf->rule);
	ccDestroyBuf(buf);
}


#ifdef DEBUG_CCONV
/*
 *	Debug Functions
 */

static void putws(s)
wchar *s;
{
	unsigned char	line[256];

	(void)convJWStoSJIS(s, line);
	fputs(line, stdout);
}

static void puteuc(s)
uchar *s;
{
	wchar	tmp[256];

	(void)convEUCtoJWS(s, tmp);
	putws(tmp);
}

void dumpRules(rule, mode)
ccRule rule;
int mode;
{
	int	nkey;
	ModeTable	*modep;
	ConvDesc	*cdp;
	wchar		*strbuf = rule->strbuf;
	ushort		*funcbuf = rule->funcbuf;
	char		**funcnames;
	wchar		*p, *q;
	wchar		restmp[256];
	ushort		*funcp;
	int	i, j;

	funcnames = (char **)__builtin_alloca(rule->nfunc * sizeof(char *));
{	char	*cp, **fnp;
	cp = rule->funcnamebuf;
	fnp = funcnames;
	for (i = 0; i < rule->nfunc; i++) {
		*fnp++ = cp;
		while (*cp++)
			;
	}
}

	if (mode < 0 || mode >= rule->nmode) {
		printf("No such mode %d\n", mode);
		return;
	}
	modep = &rule->modes[mode];

	printf("mode: %s (%d) prompt: ", modep->name, mode);
	putws(modep->prompt);
	if (modep->fallthrough != NOMODE) {
		printf(" fallthrough: %d", modep->fallthrough);
	}
	putchar('\n');
	cdp = modep->cdbuf;
	for (i = 0; i < modep->nrule; i++) {
		printf("rule[%d]: \"", i);
		putws(strbuf + cdp->context);
		printf("\"\t");
		if (cdp->key & RAWKEY) {
			ulong	key = cdp->key & ~RAWKEY;
			int	mask = cdp->mask;
			char	*keysymname;
			if (mask & ShiftMask) printf("shift-");
			if (mask & ControlMask) printf("control-");
			if (mask & LockMask) printf("lock-");
			if (mask & Mod1Mask) printf("mod1-");
			if (mask & Mod2Mask) printf("mod2-");
			if (mask & Mod3Mask) printf("mod3-");
			if (mask & Mod4Mask) printf("mod4-");
			if (mask & Mod5Mask) printf("mod5-");
			keysymname = XKeysymToString((KeySym)key);
			printf(keysymname ? keysymname : "<illegal keysym>");
		} else if (cdp->key & METAC) {
			switch (cdp->key & ~METAC) {
			case META_ASCII:	printf("@ascii"); break;
			case META_CONTROL:	printf("@control"); break;
			case META_RAW:		printf("@raw"); break;
			case META_ANY:		printf("@any"); break;
			case META_FUNC:		printf("@func"); break;
			case META_CURSOR:	printf("@cursor"); break;
			case META_KEYPAD:	printf("@keypad"); break;
			case META_MODIFIER:	printf("@modifier"); break;
			case META_NONASCII:	printf("@non-ascii"); break;
			case META_PRINTABLE:	printf("@printable"); break;
			default:		printf("<illegal meta>");
			}
		} else if (cdp->key & PSEUDO) {
			switch (cdp->key) {
			case ENTERMODE:	printf("ENTERMODE"); break;
			case EXITMODE:	printf("EXITMODE"); break;
			default:	printf("<illegal pseudo>");
			}
		} else {
			putchar('\'');
			if (cdp->key >= 0x80) {
				printf("\\x%x", cdp->key);
			} else if (cdp->key < 0x20) {
				putchar('^');
				putchar(cdp->key + '@');
			} else if (cdp->key == 0x7f) {
				printf("^?");
			} else {
				putchar(cdp->key);
			}
			putchar('\'');
		}
		printf("\t\"");
		p = restmp;
		q = strbuf + cdp->result;
		while (*q) {
			if (*q == MATCHED_CHAR) {
				*p++ = '&';
			} else if (*q == CCLEAR_CHAR) {
				*p++ = '/';
			} else {
				*p++ = *q;
			}
			q++;
		}
		*p = 0;
		putws(restmp);
		printf("\"\t");
		funcp = funcbuf + cdp->function;
		while (*funcp != ENDFUNC) {
			if (*funcp == REDO) {
				printf("redo ");
			} else if (*funcp == PREVMODE) {
				printf("goto prev ");
			} else if (*funcp & MODECHANGE) {
				int	gotomode = *funcp & ~MODECHANGE;
				if (gotomode < 0 || gotomode >= rule->nmode) {
					printf("<illegal goto>");
				} else {
					printf("goto %s ",
					       rule->modes[gotomode].name);
				}
			} else {
				if (*funcp >= rule->nfunc) {
					printf("<illegal function> ");
				} else {
					printf("%s ", funcnames[*funcp]);
				}
			}
			funcp++;
		}
		putchar('\n');
		cdp++;
	}
	putchar('\n');
}

void dumpAllRules(rule)
ccRule rule;
{
	int	i;

	printf("** RULE DUMP **\n");
	printf("number of modes: %d  initialmode: %s (%d)\n\n",
	       rule->nmode,
	       rule->modes[rule->initialmode].name,
	       rule->initialmode);
	for (i = 0; i < rule->nmode; i++) {
		dumpRules(rule, i);
	}
	fflush(stdout);
}
#endif
