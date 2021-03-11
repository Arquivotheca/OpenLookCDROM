/*
 *	jilib -- $B$+$J4A;zJQ49MQ%i%$%V%i%j(B (Wnn Version4.0 $BBP1~HG(B)
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
 *	If you use SysVR2, you should define NO_MKDIRSYS in order to
 *	use /bin/mkdir for making directories rather than mkdir()
 *	system call.
 */

/*
 * $B35MW(B
 *
 * jilib $B$O(B jslib, jclib $B$N=i4|2=$r9T$&$?$a$N%i%$%V%i%j$G$"$k!#(B
 * $B<g$JL\E*$O4D6-=i4|2=%U%!%$%k(B (wnnevnrc) $B$rFI$s$G4D6-$N@_Dj$r(B
 * $B9T$J$&$3$H$G$"$k!#(B
 * jllib $B$r;H$&$D$b$j$J$i!"F1MM$N(B ($B$H$$$&$+!"$b$C$H9bEY$J(B) $B5!G=$O(B
 * jllib $B$KF~$C$F$$$k$N$G!"$3$N%i%$%V%i%j$r;H$&I,MW$O$J$$$@$m$&!#(B
 */

/*
 * Wnn Version 4 $BBP1~$K$"$?$C$F(B
 *
 * jilib $B$O$b$H$b$H(B Wnn Version 3 $B$N(B jclib $B$H6&$K;H$&$h$&$K:n$i$l$?(B
 * $B%i%$%V%i%j$G$"$k!#(Bjclib $B$r(B Wnn ver4 $BBP1~$K$9$k$K$"$?$C$F!"(Bjilib $B$b(B
 * $BBgI}$K=q$-49$($?!#(B
 * $B=>Mh$N$b$N$H$N8_49@-$O9M$($i$l$F$$$J$$!#(B
 */

/*
 * $B%a%b(B
 *
 * ver 0.0	89/08/10
 *	$B$H$j$"$($::n$j$O$8$a$k(B
 * ver 0.1	89/08/14
 *	$B0l1~$G$-$?(B
 * ver 0.2	89/08/22
 *	SYSV $B$@$C$?$i(B index/rindex $B$r(B strchr/strrchr $B$K$9$k(B
 * ver 0.3	89/08/26
 *	$B%5!<%P!"$*$h$S4D6-$,6&M-$5$l$k$h$&$K$9$k(B
 *	jiCreateEnv() $B$G(B js_dead_env_flg $B$NCM$,%j%9%H%"$5$l$J$$>l9g$,$"$C$?(B
 *	$B$N$G$=$l$r=$@5(B
 * ver 0.4	89/08/29
 *	$B%(%i!<$H%3%s%U%!!<%`$N%3!<%k%P%C%/$K(B client_data $B$N0z?t$rDI2C(B
 * ver 0.5	89/09/01
 *	$B%5!<%P$H4D6-$r(B linked list $B$G;}$D$h$&$KJQ99(B
 * ver 0.6	89/09/08
 *	Wnn-V4 $B$N?7$7$$%P!<%8%g%s$,Mh$?$i%i%$%V%i%j$N%$%s%?!<%U%'%$%9$,(B
 *	$B0c$C$F$$$?$N$G$=$l$K9g$o$;$F=$@5(B
 *	$B!&(Bjs_env_exist() $B$,DI2C$5$l$F$$$?$N$G(B envExist() $B$r$=$l$r;H$&$h$&$K(B
 *	  $B=$@5(B
 *	$B!&(Bjs_dic_add() $B$K(B rev $B0z?t!"(Bjs_dic_file_create(_client)() $B$K(B
 *	  type $B0z?t$,DI2C$5$l!"$D$$$G$K(B wnnenvrc $B$N(B setdic $B$K(B
 *	  $B$+$J4A;zJQ49(B / $B4A;z$+$JJQ49(B $B$N%U%i%0$,$D$$$?$N$G$=$l$K9g$o$;$F(B
 *	  $B=$@5(B
 * ver 0.7	89/10/16
 *	@WNN_DIC_DIR $B$H(B @ENV $B$NE83+$,DI2C$5$l$F$$$k$N$K5$$E$+$:!"(B
 *	$B%$%s%W%j%a%s%H$7$F$$$J$+$C$?$N$r=$@5(B
 *	$B$5$i$K!"(B@$B$NE83+$K$b$H$b$H%P%0$,$"$k$3$H$,H=L@$7$?$N$G$=$l$r=$@5(B
 * ver 0.8	89/10/16
 *	wnn-4.0.1 $B$G(B commonheader.h -> commonhd.h $B$K$J$C$?$N$G(B
 *	$B$=$l$N=$@5(B
 * ver 4.0	89/10/27
 *	$B%P!<%8%g%sHV9f$N=$@5(B
 * ver 4.1	89/11/02
 *	SysV $B$G$&$^$/%3%s%Q%$%k$G$-$J$$$N$r=$@5(B
 * --- kinput $B$r(B R4 $B$K(B contribute ---
 * ver 4.2	89/12/12
 *	jiOpenServer() $B$G!"(Bserver $B$K(B NULL $B$^$?$O(B "" $B$r;XDj$7!"4D6-JQ?t(B
 *	JSERVER $B$,Dj5A$5$l$F$$$J$$;~$K%3%"%@%s%W$9$k%P%0$r=$@5(B
 * ver 4.3	90/02/20
 *	$BIQEY%U%!%$%k$,<-=q$H%^%C%A$7$J$+$C$?;~$K!"C1$K%(%i!<$K$9$k(B
 *	$BBe$o$j$KIQEY%U%!%$%k$r:n$jD>$9$h$&$KJQ99(B
 * ver 4.4	90/03/26
 *	$B%G%#%l%/%H%j$r:n$k$N$K(B /bin/mkdir $B$r(B system() $B$G5/F0$9$k;~!"(B
 *	($BIaDL$O(B mkdir() $B%7%9%F%`%3!<%k$r;H$&$N$G(B SYSVR2 $B$N;~$@$1$@$,(B)
 *	SIGCHLD (SIGCLD) $B$N%O%s%I%i$,Dj5A$5$l$F$$$k$H$=$N%j%?!<%sCM$,(B
 *	$B$*$+$7$/$J$C$F%(%i!<$K$J$k$3$H$,$"$k$H$$$&%P%0$r=$@5(B
 * ver 4.5	90/03/26
 *	4.4 $B$N=$@5$G(B signal.h $B$r(B include $B$9$k$N$rK:$l$F$$$?(B
 * ver 4.6	90/04/02
 *	wnnenvrc $B%U%!%$%k$N(B setdic $B9T$N=hM}$,4V0c$C$F$$$?!#:G8e$N0z?t$r(B
 *	$BI>2A$7$J$$$H$$$&%P%0$r=$@5(B ($B5UJQ49$N;XDj$,8z$+$J$+$C$?(B)
 * ver 4.7	91/09/18
 *	bcopy $B$NBe$o$j$K(B mybcopy $B$r;H$&$h$&$KJQ99!#IaDL$N(B bcopy $B$h$j$O(B
 *	$BCY$$$,%Q%U%)!<%^%s%9$K$OA4$/4X78$N$J$$$H$3$m$J$N$G!#(B
 *	SVR4 $B$NDj5A$rDI2C(B
 *	$B$b$&(B mkdir() $B$,$J$$%^%7%s$J$s$F$[$H$s$I$J$$$@$m$&$+$i(B mkdir() $B$,(B
 *	$B$J$$>l9g$K$O(B explicit $B$K(B NO_MKDIRSYS $B$r(B define $B$7$F$b$i$&$h$&$K$9$k(B
 * ver 4.8	91/09/23
 *	DEBUG $B$r(B DEBUG_JILIB $B$KJQ99(B
 * ver 4.9	91/09/23
 *	unuseServer() $B$H(B deleteEnv() $B$NLa$jCM$,ITDj$K$J$k$3$H$,$"$k$H$$$&(B
 *	$B%P%0$r=$@5(B
 * ver 4.10	91/09/26
 *	X11R5 $B$K$D$$$F$/$k(B Wnn $B$@$H(B ENVRCFILE $B$,(B "/wnnenvrc" $B$K$J$C$F(B
 *	$B$$$F!"%G%#%l%/%H%jItJ,$O(B LIBDIR $B$KF~$k$h$&$K$J$C$F$$$k$N$G(B
 *	$B$=$l$K9g$o$;$F=$@5(B
 * ver 4.11	91/09/30
 *	4.10 $B$N=$@5$,40A4$K4V0c$C$F$$$k$3$H$,$o$+$C$?$N$G$=$l$N=$@5(B
 *	$B7k6I!"(Bcommonhd.h $B$N(B JSERVER_VERSION $B$r8+$F%P!<%8%g%s$r%A%'%C%/$9$k(B
 *	$B$h$&$K$9$k(B
 * ver 5.0	91/10/01
 *	kinput2 $B%j%j!<%98~$1$K%P!<%8%g%sHV9f$r=$@5$7$F(B 5.0 $B$K$9$k!#(B
 * --- kinput2 $B$r(B R5 $B$K(B contribute ---
 * ver 5.1	92/01/27
 *	Wnn 4.1 $B$G$O(B wnnenvrc $B$N%G%U%)%k%H$,(B LIBDIR/ja_JP/wnnenvrc $B$K(B
 *	$B$J$C$?$N$K9g$o$;$F=>Mh$N(B LIBDIR/wnnenvrc $B$HN>J}8+$k$h$&$K=$@5(B
 * ver 5.2	94/06/06
 *	error() $B$N0z?t$N8+D>$7!#K\Mh$O(B varargs $B$r;H$&$Y$-$@$,!"$H$j$"$($:(B
 *	$BLdBj$r>/$J$/$9$k$?$a!"2DJQ8D$N0z?t$N7?$r(B int $B$+$i(B char * $B$K=$@5(B
 */

/*
 * $B%U%!%s%/%7%g%s(B
 *
 * jilib $B$,Ds6!$9$k%U%!%s%/%7%g%s$O<!$N(B4$B$D$G$"$k!#(B
 *
 * WNN_JSERVER_ID *jiOpenServer(char *servername, int timeout)
 *	servername $B$G;XDj$5$l$?%5!<%P$H@\B3$9$k!#(Btimeout $B$G(B
 *	$BICC10L$G@\B3$N%?%$%`%"%&%H$r;XDj$9$k$3$H$,$G$-$k!#(B
 *	$B@\B3$G$-$?>l9g$O%5!<%P$N(B ID $B$,!"$G$-$J$+$C$?;~$K$O(B NULL $B$,JV$5$l$k!#(B
 *
 *	$B$[$H$s$I(B js_open() $B$HF1$8$@$,!"(Bservername $B$,(B NULL $B$"$k$$$O(B
 *	$B6uJ8;zNs(B ("") $B$G$"$C$?>l9g$K$O4D6-JQ?t(B JSERVER $B$NCM$r;HMQ$9$k(B
 *	$B$H$3$m!"$*$h$S$9$G$KF1$8%5!<%P$K@\B3$5$l$F$$$l$P$=$l$r6&M-(B
 *	$B$9$k$H$3$m$,(B js_open() $B$H0[$J$k!#$3$N$?$a!"(BjiOpenServer() $B$r(B
 *	$B;HMQ$7$F:n$C$?@\B3$r@Z$k$?$a$K$O!"(BjiCloseServer() $B$rMQ$$$J$/$F$O(B
 *	$B$J$i$J$$!#(B
 *
 * int jiCloseServer(WNN_JSERVER_ID *server)
 *	$B;XDj$5$l$?%5!<%P$H$N@\B3$r@Z$k!#$H$O$$$C$F$b%5!<%P;q8;$O(B
 *	$B6&M-$5$l$F$$$k$N$G!"%j%U%!%l%s%9%+%&%s%H$,(B 0 $B$K$J$i$J$$8B$j(B
 *	$B<B:]$K(B js_close() $B$,8F$P$l$k$3$H$O$J$$!#(B
 *
 *	$B;XDj$5$l$?%5!<%P$,(B jiOpenServer() $B$K$h$C$F@\B3$5$l$?$b$N$G$O(B
 *	$B$J$$>l9g!"$*$h$S(B js_close() $B$,<:GT$7$?;~$K$O(B -1 $B$,!"$=$&$G(B
 *	$B$J$$;~$K$O(B 0 $B$,JV$5$l$k!#(B
 *
 * WNN_ENV *jiCreateEnv(WNN_JSERVER_ID *server, char *envname,
 *			int override, char *wnnrcfile,
 *			void (*errmsgfunc)(), int (*confirmfunc)(),
 *			caddr_t client_data)
 *	server $B$G;XDj$5$l$?%5!<%P$K!"(Benvname $B$H$$$&L>A0$N4D6-$r:n$k!#(B
 *	envname $B$,6uJ8;zNs$N;~$O!"%G%U%)%k%H$N4D6-L>$H$7$F%f!<%6L>$,(B
 *	$B;H$o$l$k!#(B(envname $B$,(B NULL $B$N;~$O!"4D6-L>$H$7$F$=$N$^$^(B NULL $B$,(B
 *	$B%5!<%P$KEO$5$l!"(Bjs_connect() $B$N%^%K%e%"%k$K$"$k$h$&$KB>$H6&M-(B
 *	$B$5$l$k$3$H$NL5$$4D6-$,:n$i$l$k(B)
 *
 *	jiOpenServer() $B$HF1$8$h$&$K!"F1$8%5!<%P$GF1$84D6-L>$N4D6-$O(B
 *	$B6&M-$5$l$k!#$=$N$?$a!"$3$N%U%!%s%/%7%g%s$r;H$C$F:n$C$?4D6-$r(B
 *	$B>C$9$K$O!"(BjiDeleteEnv() $B$r;HMQ$7$J$/$F$O$J$i$J$$!#(B
 *
 *	wnnrcfile $B$K$O4D6-=i4|2=%U%!%$%kL>$r;XDj$9$k!#(BjiCreateEnv() $B$O(B
 *	$B$=$NFbMF$K$7$?$,$C$F<-=q$dIUB08l%U%!%$%k$J$I$N@_Dj$r$9$k!#(B
 *	$B$b$7(B wnnrcfile $B$,(B NULL $B$J$i$P=i4|2=$N=hM}$O9T$J$o$J$$!#$^$?!"(B
 *	wnnrcfile $B$,6uJ8;zNs$G$"$C$?>l9g$K$O!"$^$:4D6-JQ?t(B WNNENVRC $B$N(B
 *	$BCM$,;HMQ$5$l$k!#$=$l$b$J$1$l$P%G%U%)%k%H$N(B /usr/local/lib/wnn/wnnenvrc
 *	$B$,;HMQ$5$l$k!#(B
 *
 *	$B%U%i%0(B override $B$,(B 0 $B$N;~$K$O!";XDj$5$l$?4D6-$,$9$G$KB8:_$7$F(B
 *	$B$$$l$P=i4|2=$N=hM}$r9T$J$o$J$$!#(B
 *
 *	errmsgfunc $B$*$h$S(B confirmfunc $B$O$H$b$K%3!<%k%P%C%/4X?t$G$"$k!#(B
 *	errmsgfunc $B$O%(%i!<$d7Y9p$N%a%C%;!<%8$rI=<($9$k;~$K<!$N7A<0$G8F$P$l$k!#(B
 *		(*errmsgfunc)(int type, char *message, caddr_t client_data);
 *	type $B$O%a%C%;!<%8$N%?%$%W$G!"(BTYPE_WARNING $B$H(B TYPE_ERROR $B$N$I$A$i$+(B
 *	$B$G$"$k!#(BTYPE_WARNING $B$OC1$J$k7Y9p$G!"=hM}$OB3$1$i$l$k!#$=$l$KBP$7(B
 *	TYPE_ERROR $B$N>l9g!"$=$N%(%i!<$,5/$-$?;~E@$G=hM}$OCf;_$5$l$k!#(B
 *	message $B$OI=<($5$l$k$Y$-%a%C%;!<%8$G$"$k!#$3$NJ8;zNs$N:G8e$K(B
 *	$B2~9T%3!<%I$OF~$C$F$$$J$$!#(B
 *	client_data $B$K$O(B jiCreateEnv() $B$N0z?t$G;XDj$5$l$?CM$,$=$N$^$^(B
 *	$BEO$5$l$k!#(B
 *	errmsgfunc $B$,(B NULL $B$N;~$O!"%a%C%;!<%8$OI8=`%(%i!<=PNO$K=q$+$l$k!#(B
 *
 *	confirmfunc $B$O!";XDj$5$l$?<-=q$,$J$+$C$?;~$K!"%f!<%6$KBP$7$F(B
 *	$B?7$?$K:n@.$9$k$+$I$&$+$r$?$:$M$k$?$a$K<!$N7A<0$G8F$S=P$5$l$k!#(B
 *		(*confirmfunc)(int type, char *file, caddr_t client_data);
 *	type $B$O:n$i$l$k%U%!%$%k$N7A<0$G!"(BTYPE_DIC $B$J$i<-=q!"(BTYPE_HINDO $B$J$i(B
 *	$BIQEY%U%!%$%k$G$"$k!#(Bfile $B$O%Q%9L>$G$"$k!#(B
 *	confirmfunc $B$O(B 0 $B$^$?$O(B 1 $B$NCM$rJV$5$J$/$F$O$J$i$J$$!#JV$5$l$?(B
 *	$BCM$,(B 0 $B$J$i%U%!%$%k$O:n$i$l$:!"$=$N<-=q$O;H$o$l$J$$!#(B1 $B$J$i$P(B
 *	$B%U%!%$%k$O?7$?$K:n$i$l$k!#(B
 *	client_data $B$K$O(B jiCreateEnv() $B$N0z?t$G;XDj$5$l$?CM$,$=$N$^$^(B
 *	$BEO$5$l$k!#(B
 *	confirmfunc $B$,(B NULL $B$N;~$O!">o$K?7$?$K%U%!%$%k$,:n$i$l$k!#(B
 *
 *	jiCreateEnv() $B$O%(%i!<$N;~$K$O(B NULL $B$r!"$=$&$G$J$1$l$P(B $B4D6-$N(B ID $B$r(B
 *	$BJV$9!#(B
 *
 * int jiDeleteEnv(WNN_ENV *env)
 *	$B;XDj$5$l$?4D6-$r>C$9!#4D6-$b%5!<%P$HF1$8$/6&M-$5$l$k$N$G(B
 *	$B%j%U%!%l%s%9%+%&%s%H$,(B 0 $B$K$J$i$J$$8B$j<B:]$K(B js_disconnect()
 *	$B$G%5!<%PFb$N4D6-$,>C5n$5$l$k$3$H$O$J$$!#(B
 *
 *	$B;XDj$5$l$?4D6-$,(B jiCreateEnv() $B$K$h$C$F:n$i$l$?$b$N$G$O(B
 *	$B$J$$>l9g!"$*$h$S(B js_disconnect() $B$,<:GT$7$?;~$K$O(B -1 $B$,!"$=$&$G(B
 *	$B$J$$;~$K$O(B 0 $B$,JV$5$l$k!#(B
 */

#ifndef lint
static char	*rcsid = "$Id: jilib.c,v 5.2 1994/06/06 05:29:28 ishisone Rel $";
#endif

#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include "commonhd.h"
#include "config.h"
#include "jslib.h"
#include "jilib.h"

#ifdef CHECK_PROTOTYPE
#include "js.c.p"
#endif

#ifdef DEBUG_JILIB
#define PRINT(a)	printf a
#else
#define PRINT(a)
#endif

#if defined(SYSV) || defined(USG) || defined(SVR4)
#define index	strchr
#define rindex	strrchr
#endif

/* used by access(2) */
#ifndef R_OK
#define R_OK	4
#endif

#if defined(USG) && !defined(SYSV)
#define NO_MKDIRSYS
#endif

#ifdef NO_MKDIRSYS
#include <signal.h>	/* for SIGCLD */
#endif

#define ERROR_LOAD	-2
#define NO_LOAD		-1

#define MAXARG	20

#define CONFIRM			1
#define CONFIRM1		2
#define CREATE_WITHOUT_CONFIRM	3
#define NO_CREATE		4

typedef struct _servrec {
	struct _servrec	*next;
	WNN_JSERVER_ID	*server;
	int		refcnt;
} ServerRec;

static ServerRec	*servers;

typedef struct _envrec {
	struct _envrec	*next;
	WNN_ENV		*env;
	char		envname[WNN_ENVNAME_LEN + 1];
	int		refcnt;
} EnvRec;

static EnvRec		*environs;

typedef struct {
	void	(*errorcall)();
	int	(*confirmcall)();
	caddr_t	client_data;
} CallbackRec;

#ifdef __STDC__
static WNN_JSERVER_ID *useServer(char *, int);
static void addServer(WNN_JSERVER_ID *);
static int unuseServer(WNN_JSERVER_ID *);
static WNN_ENV *findEnv(WNN_JSERVER_ID *, char *);
static char *getEnvName(WNN_ENV *);
static void addEnv(char *, WNN_ENV *);
static int deleteEnv(WNN_ENV *);
static int envExist(WNN_JSERVER_ID *, char *);
static int fileExist(WNN_ENV *, char *);
static int makeDir(WNN_ENV *, char *);
static int mkdirproc(WNN_ENV *, int, char *);
static int createDicFile(WNN_ENV *, char *, char *, char *);
static int createHindoFile(WNN_ENV *, char *, int, char *);
static int removeFile(WNN_ENV *, char *, int, char *);
static char *getdicpasswd(char *);
static int fileLoad(WNN_ENV *, CallbackRec *, int,
		    char *, char *, char *, int, int *);
static char *expandDicPath(char *, WNN_ENV *);
static void expandTop(char *);
static void expandUSR(char *, WNN_ENV *);
static char *strinc(char *, char *);
static void error();
static int doconfirm(CallbackRec *, int, char *);
static int readEnvFile(WNN_ENV *, char *, CallbackRec *, int *);
static int procInclude(WNN_ENV *, CallbackRec *, int *, int, char **);
static int procSetDic(WNN_ENV *, CallbackRec *, int *, int, char **);
static int procSetFuzokugo(WNN_ENV *, CallbackRec *, int, char **);
static int procSetParam(WNN_ENV *, CallbackRec *, int, char **);
#else
static WNN_JSERVER_ID *useServer();
static void addServer();
static int unuseServer();
static WNN_ENV *findEnv();
static char *getEnvName();
static void addEnv();
static int deleteEnv();
static int envExist();
static int fileExist();
static int makeDir();
static int mkdirproc();
static int createDicFile();
static int createHindoFile();
static char *getdicpasswd();
static int fileLoad();
static char *expandDicPath();
static void expandTop();
static void expandUSR();
static char *strinc();
static void error();
static int doconfirm();
static int readEnvFile();
static int procInclude();
static int procSetDic();
static int procSetFuzokugo();
static int procSetParam();
#endif

extern char	*index();
extern char	*rindex();
extern char	*getenv();
extern char	*malloc();
extern char	*realloc();
extern void	free();
extern char	*strcpy();
extern char	*strncpy();
extern char	*strcat();
extern int	strcmp();
extern int	strncmp();

/* mybcopy -- my own version of bcopy() which can handle overlapped regions */
static
mybcopy(from, to, n)
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

/* useServer -- $B;XDj$5$l$?L>A0$N%5!<%P$r;HMQ$G$-$k$h$&$K$9$k(B */
static WNN_JSERVER_ID *
useServer(servername, timeout)
char *servername;
int timeout;
{
	ServerRec	*srp;
	WNN_JSERVER_ID	*server;

	for (srp = servers; srp; srp = srp->next) {
		if (!strcmp(servername, srp->server->js_name)) {
			srp->refcnt++;
			return srp->server;
		}
	}

	PRINT(("useServer(): do js_open()\n"));
	if ((server = js_open(servername, timeout)) != NULL) {
		addServer(server);
	}

	return server;
}

/* addServer -- $B;XDj$5$l$?%5!<%P$r%j%9%H$KEPO?$9$k(B */
static void
addServer(server)
WNN_JSERVER_ID *server;
{
	ServerRec	*srp;

	if ((srp = (ServerRec *)malloc(sizeof(ServerRec))) != NULL) {
		srp->next = servers;
		servers = srp;
		srp->server = server;
		srp->refcnt = 1;
	}
}

/* unuseServer -- $B;H$o$l$J$/$J$C$?%5!<%P$N=hM}$r$9$k(B */
static int
unuseServer(server)
WNN_JSERVER_ID *server;
{
	ServerRec	*srp, *srp0;
	int		ret = 0;

	for (srp = servers, srp0 = NULL; srp; srp0 = srp, srp = srp->next) {
		if (server == srp->server) {
			if (--srp->refcnt <= 0) {
				PRINT(("unuseServer(): do js_close()\n"));
				ret = js_close(server);
				if (srp0 == NULL) {
					servers = srp->next;
				} else {
					srp0->next = srp->next;
				}
				(void)free((char *)srp);
			}
			return ret;
		}
	}
	return -1;
}

/* findEnv -- $B;XDj$5$l$?L>A0$N4D6-$,$"$k$+!"%j%9%H$rD4$Y$k(B */
static WNN_ENV *
findEnv(server, envname)
WNN_JSERVER_ID *server;
char *envname;
{
	EnvRec	*envp;

	if (envname == NULL) return NULL;

	for (envp = environs; envp; envp = envp->next) {
		if (server == envp->env->js_id &&
		    !strcmp(envname, envp->envname)) {
			envp->refcnt++;
			return envp->env;
		}
	}

	return NULL;
}

/* getEnvName -- $B4D6-L>$rJV$9(B */
static char *
getEnvName(env)
WNN_ENV *env;
{
	EnvRec	*envp;

	if (env == NULL) return NULL;

	for (envp = environs; envp; envp = envp->next) {
		if (env == envp->env)
			return envp->envname;
	}

	return NULL;
}

/* addEnv -- $B;XDj$5$l$?4D6-$r%j%9%H$KEPO?$9$k(B */
static void
addEnv(envname, env)
char *envname;
WNN_ENV *env;
{
	EnvRec	*envp;

	if ((envp = (EnvRec *)malloc(sizeof(EnvRec))) != NULL) {
		envp->next = environs;
		environs = envp;
		envp->env = env;
		if (envname != NULL) {
			(void)strncpy(envp->envname, envname, WNN_ENVNAME_LEN);
		} else {
			envp->envname[0] = '\0';
		}
		envp->refcnt = 1;
	}
}

/* deleteEnv -- $B;H$o$l$J$/$J$C$?4D6-$N=hM}$r$9$k(B */
static int
deleteEnv(env)
WNN_ENV *env;
{
	EnvRec	*envp, *envp0;
	int	ret = 0;

	for (envp0 = NULL, envp = environs; envp; envp0 = envp, envp = envp->next) {
		if (env == envp->env) {
			if (--envp->refcnt <= 0) {
				PRINT(("deleteEnv(): do js_disconnect()\n"));
				ret = js_disconnect(env);
				if (envp0 == NULL) {
					environs = envp->next;
				} else {
					envp0->next = envp->next;
				}
				(void)free((char *)envp);
			}
			return ret;
		}
	}
	return -1;
}

/* envExist -- $B;XDj$5$l$?L>A0$N4D6-$,$"$k$+$I$&$+D4$Y$k(B */
static int
envExist(server, envname)
WNN_JSERVER_ID *server;
char *envname;
{
	return js_env_exist(server, envname);
}

/* fileExist -- $B;XDj$5$l$?L>A0$N%U%!%$%k$,$"$k$+$I$&$+D4$Y$k(B */
static int
fileExist(env, file)
WNN_ENV *env;
char *file;
{
	int	ret;

	if (*file == C_LOCAL) {
		ret = access(file + 1, R_OK);
	} else {
		ret = js_access(env, file, R_OK);
	}

	return ret >= 0;
}

/* makeDir -- $B;XDj$5$l$?%G%#%l%/%H%j$r:n$k(B */
static int
makeDir(env, dir)
WNN_ENV *env;
char *dir;
{
	char	c;
	char	*p, *q;
	int	local = 0;

	if (*dir == C_LOCAL) {
		local++;
		dir++;
	}

	/* $B$9$G$K$"$k$+$I$&$+D4$Y$k(B */
	if (local) {
		if (access(dir, R_OK) >= 0)
			return 0;
	} else {
		if (js_access(env, dir, R_OK) >= 0)
			return 0;
		else if (wnn_errorno == WNN_JSERVER_DEAD)
			return -1;
	}

	/* $B>e$+$i:n$C$F$$$/(B */
	/* $B$5$9$,$K(B / $B$O:n$kI,MW$,$J$$$H;W$o$l$k(B */
	q = (*dir == '/') ? dir + 1 : dir;

	while (p = index(q, '/')) {
		c = *p; *p = '\0';
		if (mkdirproc(env, local, dir) < 0)
			return -1;
		*p = c;
		q = p + 1;
	}
	if (*q != '\0' && mkdirproc(env, local, dir) < 0)
		return -1;

	return 0;
}

static int
mkdirproc(env, local, dir)
WNN_ENV *env;
int local;
char *dir;
{
	if (local) {
		/* $B$^$:$O$"$k$+$I$&$+D4$Y$k(B */
		if (access(dir, R_OK) >= 0)
			return 0;
#ifdef NO_MKDIRSYS
		{
		char	buf[1024];	/*enough?*/
		int	(*ohandle)() = signal(SIGCLD, SIG_DFL);
		int	ret;
		(void)sprintf(buf, "/bin/mkdir %s", dir);
		ret = system(buf);
		(void)signal(SIGCLD, ohandle);
		return (ret != 0) ? -1 : 0;
		}
#else
		return mkdir(dir, 0777);
#endif
	} else {
		if (js_access(env, dir, R_OK) >= 0)
			return 0;
		return js_mkdir(env, dir);
	}
}

static int
createDicFile(env, file, dpasswd, hpasswd)
WNN_ENV *env;
char *file;
char *dpasswd;
char *hpasswd;
{
	PRINT(("createDicFile(env, %s, %s, %s)\n", file,
	       dpasswd ? dpasswd : "-", hpasswd ? hpasswd : "-"));

	if (*file == C_LOCAL) {
		return js_dic_file_create_client(env, file + 1, WNN_REV_DICT,
						 NULL, dpasswd, hpasswd);
	} else {
		return js_dic_file_create(env, file, WNN_REV_DICT,
					  NULL, dpasswd, hpasswd);
	}
}

static int
createHindoFile(env, file, dicfid, hpasswd)
WNN_ENV *env;
char *file;
int dicfid;
char *hpasswd;
{
	PRINT(("createHindoFile(env, %s, %d, %s)\n", file, dicfid,
	       hpasswd ? hpasswd : "-"));

	if (*file == C_LOCAL) {
		return js_hindo_file_create_client(env, dicfid, file + 1,
						   NULL, hpasswd);
	} else {
		return js_hindo_file_create(env, dicfid, file, NULL, hpasswd);
	}
}

static int
removeFile(env, file, fid, passwd)
WNN_ENV *env;
char *file;
int fid;
char *passwd;
{
	if (fid >= 0) (void)js_file_discard(env, fid);

	if (*file == C_LOCAL) {
		return js_file_remove_client(env->js_id, file + 1, passwd);
	} else {
		return js_file_remove(env->js_id, file, passwd);
	}
}

/* getdicpasswd -- $B;XDj$5$l$?%U%!%$%k$+$i%Q%9%o!<%I$r<h$j=P$9(B */
static char *
getdicpasswd(file)
char *file;	/* $B%U%!%$%kL>(B $B$?$@$7%Q%9%o!<%I$,>e=q$-$5$l$k(B */
{
	FILE	*fp = fopen(file, "r");
	int	len;

	PRINT(("getdicpasswd(%s)\n", file));
	/* file$BL>$N0z?t$K%Q%9%o!<%I$r>e=q$-$9$k(B
	 * $B1x$$$1$I!"$^$"$$$$$+(B
	 */
	if (fp == NULL) return NULL;
	if (fgets(file, 256, fp) == NULL) {
		(void)fclose(fp);
		return NULL;
	}

	len = strlen(file);
	if (file[len - 1] == '\n') file[len - 1] = '\0';

	(void)fclose(fp);
	PRINT(("getdicpasswd(): password=%s\n", file));
	return file;
}

/* fileLoad -- $B%U%!%$%k$r%m!<%I$9$k(B $B$b$7I,MW$,$"$l$P?7$?$K:n@.$9$k(B */
static int
fileLoad(env, cbrec, type, file, dpasswd, hpasswd,
	 dicfid, confirmp)
WNN_ENV *env;
CallbackRec *cbrec;
int type;
char *file;
char *dpasswd;
char *hpasswd;
int dicfid;
int *confirmp;
{
	int	fid;
	char	*filep;

	if (!fileExist(env, file)) {
		switch (*confirmp) {
		case NO_CREATE:
			error(cbrec, TYPE_WARNING, "setdic: %s doesn't exist",
			      file);
			return NO_LOAD;
		case CONFIRM:
			if (!doconfirm(cbrec, type, file)) {
				return NO_LOAD;
			}
			break;
		case CONFIRM1:
			if (!doconfirm(cbrec, type, file)) {
				*confirmp = NO_CREATE;
				return NO_LOAD;
			}
			*confirmp = CREATE_WITHOUT_CONFIRM;
			break;
		}
		/* $B%U%!%$%k$r:n$k(B */
		if (filep = rindex(file, '/')) {
			char	c = *filep;
			/* $B%G%#%l%/%H%j$r:n$k(B */
			*filep = '\0';
			if (makeDir(env, file) < 0) {
				error(cbrec, TYPE_ERROR,
				      "setdic: can't make directory %s",
				      file);
				return ERROR_LOAD;
			}
			*filep++ = c;
		}

		if (type == TYPE_DIC) {
			if (createDicFile(env, file, dpasswd, hpasswd) < 0) {
				error(cbrec, TYPE_ERROR,
				      "setdic: can't create dic (%s)",
				      file);
				return ERROR_LOAD;
			}
		} else {
			if (createHindoFile(env, file, dicfid, hpasswd) < 0) {
				error(cbrec, TYPE_ERROR,
				      "setdic: can't create hindo (%s)",
				      file);
				return ERROR_LOAD;
			}
		}
	}

	if (*file == C_LOCAL) {
		fid = js_file_send(env, file + 1);
	} else {
		fid = js_file_read(env, file);
	}

	if (fid < 0) {
		error(cbrec, TYPE_ERROR, "setdic: can't load %s", file);
		return ERROR_LOAD;
	}

	return fid;
}

/* expandDicPath -- $B<-=q$N%Q%9L>$NE83+$r$9$k(B */
static char *
expandDicPath(path, env)
char *path;
WNN_ENV *env;
{
	/* $B;O$a$K@hF,$rE83+$7$F(B */
	expandTop(path);
	/* $B$"$H$O(B @USR $B$rE83+$9$k(B */
	expandUSR(path, env);
	return path;
}

static void
expandTop(path)
char *path;
{
	char	namebuf[256];
	char	tmp[256];
	char	*name;
	char	*p;
	char	*svpath = path;
	struct passwd	*pwp;
	int	local = 0;

	if (*path == C_LOCAL) {
		local = 1;
		path++;
	}

	p = NULL;

	switch (*path) {
	case '@':
		path++;
		if (!strncmp(path, "HOME", 4)) {
			if ((p = getenv("HOME")) == NULL)
				p = "";
			path += 4;
		} else if (!strncmp(path, "LIBDIR", 6)) {
			p = LIBDIR;
			path += 6;
		} else if (!strncmp(path, "WNN_DIC_DIR", 11)) {
			if ((p = getenv("HOME")) == NULL) {
				p = "";
			} else {
				(void)strcpy(tmp, p);
				(void)strcat(tmp, "/");
				if ((p = getenv("WNN_DIC_DIR")) == NULL) {
					(void)strcat(tmp, "Wnn");
				} else {
					(void)strcat(tmp, p);
				}
				p = tmp;
			}
			path += 11;
		}
		break;
	case '~':
		if (*++path == '/') {
			pwp = getpwuid(getuid());
		} else {
			char	*cp;
			char	c;

			cp = path;
			while (*cp && *cp != '/')
				cp++;
			c = *cp;
			*cp = '\0';
			pwp = getpwnam(path);
			*cp = c;
			path = cp;
		}
		if (pwp == NULL)
			p = "";
		else
			p = pwp->pw_dir;
		break;
	}

	if (p != NULL) {
		if (strlen(path) + strlen(p) + local > 255) {
			return;
		}
		name = namebuf;
		if (local) *name++ = C_LOCAL;
		(void)strcpy(name, p);
		(void)strcat(name, path);
		(void)strcpy(svpath, namebuf);
	}
}

static void
expandUSR(path, env)
char *path;
WNN_ENV *env;
{
	static char	myname[10];
	register char	*t;
	int		mynamelen;
	char		*envname;
	int		envnamelen;

	/* @USR $B$NE83+$r$9$k(B */
	if (myname[0] == '\0') {
		(void)strcpy(myname, getpwuid(getuid())->pw_name);
	}
	mynamelen = strlen(myname);
	t = path;
	while (t = strinc(t, "@USR")) {
		if (strlen(path) + mynamelen - 4 > 255) return;

		(void)mybcopy(t + 4, t + mynamelen, strlen(t + 4) + 1);
		(void)mybcopy(myname, t, mynamelen);
	}

	/* @ENV $B$NE83+$r$9$k(B */
	envname = NULL;
	t = path;
	while (t = strinc(t, "@ENV")) {
		if (envname == NULL) {
			envname = getEnvName(env);
			if (envname == NULL) envname = "";
			envnamelen = strlen(envname);
		}
		if (strlen(path) + envnamelen - 4 > 255) return;

		(void)mybcopy(t + 4, t + envnamelen, strlen(t + 4) + 1);
		(void)mybcopy(envname, t, envnamelen);
	}
}

static char *
strinc(s1, s2)
char *s1;
char *s2;
{
	int	c = *s2;
	int	len = strlen(s2);

	while (s1 = index(s1, c)) {
		if (!strncmp(s1, s2, len))
			return s1;
		s1++;
	}
	return NULL;
}

/* parseLine -- $B%3%^%s%I9T$N%Q!<%8%s%0(B */
static int
parseLine(line, argv, argsize)
char *line;
char **argv;
int argsize;
{
	int	argc = 0;

	while (argc < argsize) {
		/* $B6uGr$N%9%-%C%W(B */
		while (*line <= ' ') {
			if (*line == '\0' || *line == '\n')
				return argc;
			line++;
		}
		argv[argc++] = line++;
		while (*line++ > ' ')
			;
		line--;
		if (*line == '\0' || *line == '\n') {
			*line = '\0';
			return argc;
		}
		*line++ = '\0';
	}
	return argc;
}


/* VARARGS3 */
static void
error(cbrec, level, s, a, b)
CallbackRec *cbrec;
int level;
char *s;
char *a, *b;
{
	char	tmp[1024];
	void (*callback)() = cbrec->errorcall;

	(void)sprintf(tmp, s, a, b);

	if (callback) {
		(*callback)(level, tmp, cbrec->client_data);
	} else {
		fprintf(stderr, "%s: %s\n",
			level == TYPE_ERROR ? "Error" : "Warning", tmp);
	}
}

static int
doconfirm(cbrec, type, file)
CallbackRec *cbrec;
int type;
char *file;
{
	int	(*callback)() = cbrec->confirmcall;

	return (callback ? (*callback)(type, file, cbrec->client_data) : 1);
}

/*
 *	$B%Q%V%j%C%/$J%U%!%s%/%7%g%s(B
 */

/* jiOpenServer -- $B%5!<%P$r%*!<%W%s$9$k(B */
WNN_JSERVER_ID *
jiOpenServer(servername, timeout)
char *servername;
int timeout;
{
	extern char	*getenv();

	/*
	 * servername $B$,(B NULL $B$^$?$O6uJ8;zNs$@$C$?>l9g$O!"(B
	 * $B4D6-JQ?t(B 'JSERVER' $B$r;HMQ$9$k(B
	 */
	if (servername == NULL || *servername == '\0') {
		servername = getenv(WNN_JSERVER_ENV);
	}
	PRINT(("jiOpenServer(): servername=%s\n",
	       servername ? servername : "<NULL>"));
	if (servername == NULL) servername = "unix";	/* unix domain */
	return useServer(servername, timeout);
}

/* jiCloseServer -- $B%5!<%P$r%/%m!<%:$9$k(B */
int
jiCloseServer(server)
WNN_JSERVER_ID *server;
{
	PRINT(("jiCloseServer(): servername=%s\n",
	       server->js_name));
	return unuseServer(server);
}

/* jiCreateEnv -- $B;XDj$5$l$?4D6-$r:n$k(B */
WNN_ENV *
jiCreateEnv(server, envname, override, wnnrcfile, errmsgfunc, confirmfunc, client_data)
WNN_JSERVER_ID *server;
char *envname;
int override;
char *wnnrcfile;
void (*errmsgfunc)();
int (*confirmfunc)();
caddr_t client_data;
{
	int		noInitialize = 0;
	WNN_ENV		*env;
	int		confirm;
	int		saveflag;
	CallbackRec	callbackrec;
	char		buf[256];

	callbackrec.errorcall = errmsgfunc;
	callbackrec.confirmcall = confirmfunc;
	callbackrec.client_data = client_data;

	/*
	 * envname $B$,6uJ8;zNs(B (i.e. *envname == '\0') $B$@$C$?>l9g$K$O(B
	 * $B%f!<%6L>$r;H$&(B ($B$3$NJU$O(B uum $B$K9g$o$;$F$"$k(B)
	 * envname $B$,(B NULL $B$N>l9g$K$O(B NULL $B$N$^$^(B
	 */
	if (envname != NULL && *envname == '\0') {
		envname = getpwuid(getuid())->pw_name;
	}
	PRINT(("jiCreateEnv(): envname=%s\n", envname ? envname : "<NULL>"));

	/* $B$9$G$K:n$i$l$F$$$k$+$I$&$+D4$Y$k(B */
	env = findEnv(server, envname);
	if (env != NULL && !override) {
		return env;
	}

	/* $B$H$j$"$($:(B jserver $B$,;`$s$G$$$k$+D4$Y$k(B */
	if (server->js_dead) {
		error(&callbackrec, TYPE_ERROR, "JSERVER is DEAD");
		wnn_errorno = WNN_JSERVER_DEAD;
		return NULL;
	}

	/* jserver $B$,;`$s$@;~$N=hM}(B
	 * longjmp $B$GLa$C$F$/$k$H$3$m$r@_Dj$9$k(B
	 * $B$3$&$7$F$*$/$3$H$G!"0J9_(B jserver $B$,;`$s$@$+$I$&$+$N%A%'%C%/$,(B
	 * $BITMW$K$J$k(B
	 * $B$?$@$7!">e0L$N4X?t$G$9$G$K@_Dj$5$l$F$$$k$H$-$K$O$=$l$KG$$;$k(B
	 */
	if ((saveflag = server->js_dead_env_flg) == 0) {
		server->js_dead_env_flg = 1;
		if (setjmp(server->js_dead_env)) {
			error(&callbackrec, TYPE_ERROR, "JSERVER is DEAD");
			server->js_dead_env_flg = 0;
			wnn_errorno = WNN_JSERVER_DEAD;
			return NULL;
		}
	}

	if (!override && envExist(server, envname) == 1) {
		/* $B$9$G$K;XDj$5$l$?L>A0$N4D6-$,B8:_$9$k(B */
		PRINT(("jcCreateEnv(): %s already exists\n", envname));
		noInitialize = 1;
	}

	/* $B4D6-$r:n$k(B */
	if ((env = js_connect(server, envname)) == NULL) {
		error(&callbackrec, TYPE_ERROR, "can't conenct to environment");
		server->js_dead_env_flg = saveflag;
		return NULL;
	}

	/*
	 * wnnrcfile $B$,6uJ8;zNs(B (i.e. *wnnrcfile == '\0') $B$@$C$?>l9g$K$O(B
	 * $B$^$:4D6-JQ?t(B 'WNNENVRC' $B$r;H$&(B
	 * $B$=$l$b$J$1$l$P%G%U%)%k%H$N(B /usr/local/lib/wnn/wnnenvrc $B$r(B
	 * $B;H$&(B
	 * wnnrcfile $B$,(B NULL $B$N>l9g$K$O=i4|2=$O9T$J$o$J$$(B
	 */

	if (noInitialize || wnnrcfile == NULL) {
		/* $B4D6-%j%9%H$KF~$l$F$*$/(B */
		addEnv(envname, env);
		server->js_dead_env_flg = saveflag;
		return env;
	}

	if (*wnnrcfile == '\0') {
		if ((wnnrcfile = getenv("WNNENVRC")) == NULL ||
		    access(wnnrcfile, R_OK) < 0) {
			/* use default file defined in 'config.h' */
#if JSERVER_VERSION > 0x4030
			(void)strcpy(buf, LIBDIR);
			(void)strcat(buf, "/ja_JP");
			(void)strcat(buf, ENVRCFILE);
			if (access(buf, R_OK) < 0) {
				(void)strcpy(buf, LIBDIR);
				(void)strcat(buf, ENVRCFILE);
			}
#else
			(void)strcpy(buf, ENVRCFILE);
#endif
			wnnrcfile = buf;
		}
	}
	PRINT(("jiCreateEnv(): wnnrcfile=%s\n", wnnrcfile));

	/* $B=i4|2=$9$k(B */
	confirm = CONFIRM;
	if (readEnvFile(env, wnnrcfile, &callbackrec, &confirm) < 0) {
		(void)js_disconnect(env);
		server->js_dead_env_flg = saveflag;
		return NULL;
	}

	/* $B4D6-%j%9%H$KF~$l$F$*$/(B */
	addEnv(envname, env);

	/* $B85$KLa$7$F$*$/(B */
	server->js_dead_env_flg = saveflag;

	return env;
}

/* jiDeleteEnv -- $B4D6-$r>C$9(B */
int
jiDeleteEnv(env)
WNN_ENV *env;
{
	PRINT(("jiDeleteEnv()\n"));
	return deleteEnv(env);
}

/*
 *	$B4D6-@_Dj$N%U%!%s%/%7%g%s(B
 */

static int
readEnvFile(env, file, cbrec, confirmp)
WNN_ENV *env;
char *file;
CallbackRec *cbrec;
int *confirmp;
{
	char	line[512];
	FILE	*fp;
	FILE	*fopen();
	int	argc;
	char	*argv[MAXARG];
	int	abort;

	if ((fp = fopen(file, "r")) == NULL) {
		error(cbrec, TYPE_ERROR, "can't open %s", file);
		return -1;
	}

	while (fgets(line, sizeof(line), fp)) {
		if (*line == '\0' || *line == '\n' || *line == ';')
			continue;
		PRINT(("readEnvFile(): doing line %s", line));

		/* parsing */
		argc = parseLine(line, argv, MAXARG);
		if (argc < 1)
			continue;

		abort = 0;
		if (!strcmp(argv[0], "include")) {
			abort = procInclude(env, cbrec, confirmp, argc, argv);
		} else if (!strcmp(argv[0], "setdic")) {
			abort = procSetDic(env, cbrec, confirmp, argc, argv);
		} else if (!strcmp(argv[0], "setfuzokugo")) {
			abort = procSetFuzokugo(env, cbrec, argc, argv);
		} else if (!strcmp(argv[0], "setparam")) {
			abort = procSetParam(env, cbrec, argc, argv);
		} else if (!strcmp(argv[0], "confirm")) {
			*confirmp = CONFIRM;
		} else if (!strcmp(argv[0], "confirm1")) {
			*confirmp = CONFIRM1;
		} else if (!strcmp(argv[0], "create_without_confirm")) {
			*confirmp = CREATE_WITHOUT_CONFIRM;
		} else if (!strcmp(argv[0], "no_create")) {
			*confirmp = NO_CREATE;
		}
		if (abort) {
			(void)fclose(fp);
			return -1;
		}
	}
	(void)fclose(fp);
	return 0;
}

/*
 * $B%3%^%s%I=hM}%k!<%A%s(B
 *
 *	procInclude()		include $B$N=hM}(B
 *	procSetDic()		setdic $B$N=hM}(B
 *	procSetFuzokugo()	setfuzokugo $B$N=hM}(B
 *	procSetParam()		setparam $B$N=hM}(B
 */

static int
procInclude(env, cbrec, confirmp, ac, av)
WNN_ENV *env;
CallbackRec *cbrec;
int *confirmp;
int ac;
char **av;
{
	char	tmp[256];

	if (ac < 2) {
		error(cbrec, TYPE_ERROR, "include: filename expected");
		return -1;
	}

	(void)strcpy(tmp, av[1]);
	expandTop(tmp);
	return readEnvFile(env, tmp, cbrec, confirmp);
}

static int
procSetDic(env, cbrec, confirmp, ac, av)
WNN_ENV *env;
CallbackRec *cbrec;
int *confirmp;
int ac;
char **av;
{
	char	dicfilebuf[256], hindofilebuf[256];
	char	dicpasswdbuf[256], hindopasswdbuf[256];
	char	*dicfilename, *hindofilename;
	int	prio, dicro, hindoro, rev;
	int	dicid, hindoid;
	char	*dicpasswd, *hindopasswd;

	if (ac < 2) {
		error(cbrec, TYPE_ERROR, "setdic: dic filename expected");
		return -1;
	}
	
	(void)strcpy(dicfilebuf, av[1]);
	dicfilename = expandDicPath(dicfilebuf, env);

	/* $B%G%U%)%k%H$NCM(B */
	hindofilename = NULL;
	prio = 5;
	dicro = hindoro = 0;
	rev = 0;
	dicpasswd = hindopasswd = NULL;

	if (ac >= 3 && strcmp(av[2], "-")) {
		(void)strcpy(hindofilebuf, av[2]);
		hindofilename = expandDicPath(hindofilebuf, env);
	}
	if (ac >= 4) {
		prio = atoi(av[3]);
	}
	if (ac >= 5) {
		dicro = atoi(av[4]);
	}
	if (ac >= 6) {
		hindoro = atoi(av[5]);
	}
	if (ac >= 7 && strcmp(av[6], "-")) {
		(void)strcpy(dicpasswdbuf, av[6]);
		dicpasswd = getdicpasswd(expandDicPath(dicpasswdbuf, env));
		/* $B%Q%9%o!<%I$,FI$a$J$/$F$b!"%(%i!<$K$O$7$J$$(B */
		if (dicpasswd == NULL) {
			error(cbrec, TYPE_WARNING,
			      "can't read password for %s", dicfilename);
		}
	}
	if (hindofilename && ac >= 8 && strcmp(av[7], "-")) {
		(void)strcpy(dicpasswdbuf, av[7]);
		hindopasswd = getdicpasswd(expandDicPath(hindopasswdbuf, env));
		/* $B%Q%9%o!<%I$,FI$a$J$/$F$b!"%(%i!<$K$O$7$J$$(B */
		if (hindopasswd == NULL) {
			error(cbrec, TYPE_WARNING,
			      "can't read password for %s", hindofilename);
		}
	}
	if (ac >= 9) {
		rev = atoi(av[8]);
	}

	dicid = hindoid = -1;

	if ((dicid = fileLoad(env, cbrec, TYPE_DIC, dicfilename,
			      dicpasswd, hindopasswd, -1, confirmp)) == ERROR_LOAD) {
		return -1;
	} else if (dicid == NO_LOAD) {
		return 0;
	}

 re_create:
	if (hindofilename) {
		/* $BIQEY%U%!%$%k$,;XDj$5$l$F$$$k;~!"(B
		 * confirm $B$5$l$J$$$?$a$KIQEY%U%!%$%k$,:n$i$l$J$+$C$?$J$i$P(B
		 * $BC1$K<-=q$r;H$o$J$$$@$1(B
		 * confirm $B$5$l$?$K$b$+$+$o$i$::n$i$l$J$+$C$?$J$i$P(B
		 * $B%(%i!<$G%"%\!<%H$9$k(B
		 */
		hindoid = fileLoad(env, cbrec, TYPE_HINDO,
				   hindofilename, NULL, hindopasswd,
				   dicid, confirmp);
		if (hindoid == ERROR_LOAD) {
			(void)js_file_discard(env, dicid);
			return -1;
		} else if (hindoid == NO_LOAD) {
			return 0;
		}
	}

	if (js_dic_add(env, dicid, hindoid, rev, prio, dicro, hindoro,
		       dicpasswd, hindopasswd) < 0) {
		/*
		 * $B$b$7!"<-=q$HIQEY%U%!%$%k$,%^%C%A$7$J$+$C$?$H$-$K$O!"(B
		 * $B$$$-$J$j%(%i!<$K$;$:!"IQEY%U%!%$%k$r:n$jD>$9$+$I$&$+(B
		 * $B?R$M$k$h$&$K$9$k(B
		 */
		if (wnn_errorno == WNN_HINDO_NO_MATCH) {
			int	retry = 0;

			error(cbrec, TYPE_WARNING,
			      "setdic: hindo (%s) doesn't match with dic (%s)",
			      hindofilename, dicfilename);

			switch (*confirmp) {
			case NO_CREATE:
				break;
			case CONFIRM:
				if (doconfirm(cbrec, TYPE_HINDO,
					      hindofilename)) {
					retry = 1;
				}
				break;
			case CONFIRM1:
				if (doconfirm(cbrec, TYPE_HINDO,
					      hindofilename)) {
					*confirmp = CREATE_WITHOUT_CONFIRM;
					retry = 1;
				} else {
					*confirmp = NO_CREATE;
				}
				break;
			default:	/* CREATE_WITHOUT_CONFIRM */
				retry = 1;
				break;
			}

			/* $B0lEYIQEY%U%!%$%k$r>C$7$F!"$b$&0lEY%m!<%I$9$k(B */
			if (retry) {
				if (removeFile(env, hindofilename,
					       hindoid, hindopasswd) < 0) {
					error(cbrec, TYPE_ERROR,
					      "setdic: can't remove hindo (%s)",
					      hindofilename);
				} else {
					goto re_create;
				}
			}
		}
		error(cbrec, TYPE_ERROR, "setdic: can't add dic (%s)",
		      dicfilename);
		return -1;
	}

	return 0;
}

static int
procSetFuzokugo(env, cbrec, ac, av)
WNN_ENV *env;
CallbackRec *cbrec;
int ac;
char **av;
{
	char	filebuf[256];
	int	fid;

	if (ac < 2) {
		error(cbrec, TYPE_ERROR, "setfuzokugo: filename expected");
		return -1;
	}
	
	(void)strcpy(filebuf, av[1]);
	(void)expandDicPath(filebuf, env);

	if (*filebuf == C_LOCAL) {
		fid = js_file_send(env, filebuf + 1);
	} else {
		fid = js_file_read(env, filebuf);
	}
	if (fid < 0) {
		error(cbrec, TYPE_ERROR, "setfuzokugo: can't load %s", filebuf);
		return -1;
	}

	if (js_fuzokugo_set(env, fid) < 0) {
		error(cbrec, TYPE_ERROR, "setfuzokugo: can't set fuzokugo-file");
		return -1;
	}

	return 0;
}

static int
procSetParam(env, cbrec, ac, av)
WNN_ENV *env;
CallbackRec *cbrec;
int ac;
char **av;
{
	struct wnn_param	param;

	if (ac < 2) {
		error(cbrec, TYPE_ERROR, "setparam: no parameter");
		return -1;
	}

	/* $B;XDj$5$l$?%Q%i%a!<%?$N?t$,>/$J$$$H$-$O!";XDj$5$l$F$J$$(B
	 * $B%Q%i%a!<%?$NCM$O8=:_$N%Q%i%a!<%?$HF1$8$K$9$k(B
	 */
	if (ac < 18 && js_param_get(env, &param) < 0) {
		error(cbrec, TYPE_ERROR,
		      "setparam: can't get default value");
		return -1;
	}

	/* $B%Q%i%a!<%?$NCM$NBeF~(B */
	switch (ac) {
	case 18: param.p15 = atoi(av[17]);
	case 17: param.p14 = atoi(av[16]);
	case 16: param.p13 = atoi(av[15]);
	case 15: param.p12 = atoi(av[14]);
	case 14: param.p11 = atoi(av[13]);
	case 13: param.p10 = atoi(av[12]);
	case 12: param.p9 = atoi(av[11]);
	case 11: param.p8 = atoi(av[10]);
	case 10: param.p7 = atoi(av[9]);
	case  9: param.p6 = atoi(av[8]);
	case  8: param.p5 = atoi(av[7]);
	case  7: param.p4 = atoi(av[6]);
	case  6: param.p3 = atoi(av[5]);
	case  5: param.p2 = atoi(av[4]);
	case  4: param.p1 = atoi(av[3]);
	case  3: param.nsho = atoi(av[2]);
	case  2: param.n = atoi(av[1]);
	}

	if (js_param_set(env, &param) < 0) {
		error(cbrec, TYPE_ERROR, "setparam: can't set parameters");
		return -1;
	}

	return 0;
}
