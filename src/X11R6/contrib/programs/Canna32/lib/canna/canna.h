/* Copyright 1992 NEC Corporation, Tokyo, Japan.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without
 * fee, provided that the above copyright notice appear in all copies
 * and that both that copyright notice and this permission notice
 * appear in supporting documentation, and that the name of NEC
 * Corporation not be used in advertising or publicity pertaining to
 * distribution of the software without specific, written prior
 * permission.  NEC Corporation makes no representations about the
 * suitability of this software for any purpose.  It is provided "as
 * is" without express or implied warranty.
 *
 * NEC CORPORATION DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN 
 * NO EVENT SHALL NEC CORPORATION BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF 
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR 
 * OTHER TORTUOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR 
 * PERFORMANCE OF THIS SOFTWARE. 
 */

/*
 * @(#) 102.1 $Id: canna.h,v 7.50 1994/04/21 07:00:05 kon Exp $
 */

#ifndef _CANNA_H_
#define _CANNA_H_

#include "cannaconf.h"

#include "widedef.h"

#include <stdio.h>

#if __STDC__
#include <stdlib.h>
#define pro(x) x
#else
extern char *malloc(), *realloc(), *calloc();
extern void free();
#define pro(x) ()
#endif

#if defined(USG) || defined(SYSV) || defined(SVR4)
#include <string.h>
# ifndef index
# define index strchr
# endif
#else
#include <strings.h>
#endif

#include <canna/RK.h>
#include <canna/jrkanji.h>

#ifdef BIGPOINTER
#define POINTERINT long long
#else
#define POINTERINT long
#endif

#define	WCHARSIZE	(sizeof(wchar_t))

#ifdef HAVE_WCHAR_OPERATION

#ifndef JAPANESE_LOCALE
#define JAPANESE_LOCALE "japan"
#endif

#define MBstowcs mbstowcs
#define WCstombs wcstombs

#else

#define MBstowcs CANNA_mbstowcs
#define WCstombs CANNA_wcstombs

#endif

#include "sglobal.h"

#define XLookupKanji2			 IROHA_G300_XLookupKanji2
#define XKanjiControl2			 IROHA_G301_XKanjiControl2
#define XwcLookupKanji2			 IROHA_G425_XwcLookupKanji2
#define XwcKanjiControl2		 IROHA_G426_XwcKanjiControl2
#define FirstTime			 CANNA_G271_FirstTime

#define STROKE_LIMIT 500 /* ?????????????????????? */

#if defined(SYSV) || defined(SVR4) || __STDC__
# if defined(SYSV) || defined(SVR4)
#  include <memory.h>
# endif
# define bzero(buf, size) memset((char *)(buf), 0x00, (size))
# define bcopy(src, dst, size) memcpy((char *)(dst), (char *)(src), (size))
#endif

typedef unsigned char BYTE;

/*
 * CANNALIBDIR  -- ????????????????????????????????????????????????
 *                 ????????????????????????????????
 */

#ifndef CANNALIBDIR
#define CANNALIBDIR "/usr/lib/canna"
#endif

/* flags ?????? */
#define CANNA_KANJIMODE_TABLE_SHARED	01
#define CANNA_KANJIMODE_EMPTY_MODE	02

/* func ?????????? */
#define KEY_CALL  0
#define KEY_CHECK 1
#define KEY_SET   2

extern int searchfunc();
extern BYTE default_kmap[];

/* menuitem -- ???????????????????????????????????? */

typedef struct _menuitem {
  int flag; /* ???????? */
  union {
    struct _menustruct *menu_next; /* ???????????????????? */
    int fnum;    /* ???????? */
    char *misc;  /* ??????(lisp ??????????????) */
  } u;
} menuitem;

#define MENU_SUSPEND 0 /* ??????????????????(lisp ??????????) */
#define MENU_MENU    1 /* ???????? */
#define MENU_FUNC    2 /* ???????? */

/* menustruct -- ?????????????????????? */

typedef struct _menustruct {
  int     nentries; /* ?????????????????? */
  wchar_t **titles; /* ?????????????????????? */
  wchar_t *titledata; /* ?????????????????????? */
  menuitem *body;   /* ??????????????(????) */
  int     modeid;   /* ???????????????????? */
  struct _menustruct *prev; /* ???????????????????????????? */
} menustruct;

typedef struct _menuinfo {
  menustruct *mstruct; /* ?????????????? */
  int        curnum;   /* ?????????????????????????????????? */
  struct _menuinfo *next;
} menuinfo;

/* defselection ???????????????????????????????????????????? */

typedef struct {
  wchar_t	**kigo_data;	/* ?????????????????????? */
  wchar_t	*kigo_str;	/* ???????????????????????????? */
  int		kigo_size;	/* ???????? */
  int		kigo_mode;	/* ???????????????? */
} kigoIchiran;

typedef struct _selectinfo {
  kigoIchiran	*ichiran;	/* ?????????? */
  int		curnum;		/* ?????????????????? */
  struct _selectinfo *next;
} selectinfo;

/*
 * glineinfo -- ????????????????????????????????????????????????????????
 * ????????????????????????????????????
 *
 * glkosu -- ????????????????????
 * glhead -- ????????????????????kouhoinfo??????????(0??????????)
 * gllen  -- ??????????????????????????????????
 * gldata -- ????????????????????????????????????????
 */

typedef struct {
  int glkosu;
  int glhead;
  int gllen;
  wchar_t *gldata;
} glineinfo;

/*
 * kouhoinfo -- ??????????????????????????????????????????????????
 * ????????????????????????????????????
 *
 * khretsu -- ????????????????
 * khpoint -- ??????????????????????????
 * khdata -- ????????????????????????????
 */

typedef struct {
  int khretsu;
  int khpoint;
  wchar_t *khdata;
} kouhoinfo;

#define ROMEBUFSIZE 	1024
#define	BANGOSIZE	2	/* ???????????????????????????????? */
#define	BANGOMAX   	9	/* ?????????????????????? */

#define	KIGOBANGOMAX   	16	/* ?????????????????????? */
#define GOBISUU		9

#define	ON		1
#define	OFF		0

#define	NG		-1

#define NO_CALLBACK	0
#define NCALLBACK	4

#define	JISHU_HIRA	0
#define JISHU_ZEN_KATA	1
#define JISHU_HAN_KATA	2
#define JISHU_ZEN_ALPHA	3
#define JISHU_HAN_ALPHA	4
#define MAX_JISHU	5

#define  SENTOU        0x01
#define  HENKANSUMI    0x02
#define  SUPKEY        0x04
#define  GAIRAIGO      0x08
#define  STAYROMAJI    0x10

/* identifier for each context structures */
#define CORE_CONTEXT       ((BYTE)0)
#define YOMI_CONTEXT       ((BYTE)1)
#define ICHIRAN_CONTEXT    ((BYTE)2)
#define FORICHIRAN_CONTEXT ((BYTE)3)
#define MOUNT_CONTEXT      ((BYTE)4)
#define TOUROKU_CONTEXT    ((BYTE)5)
#define TAN_CONTEXT	   ((BYTE)6)

typedef struct _coreContextRec {
  BYTE id;
  BYTE majorMode, minorMode;
  struct _kanjiMode *prevMode; /* ?????????????? */
  struct _coreContextRec *next;
} coreContextRec, *coreContext;

typedef coreContext mode_context;

typedef struct  _yomiContextRec {
  /* core ?????????????? */
  BYTE id;
  BYTE majorMode, minorMode;
  struct _kanjiMode *prevMode;	/* ?????????????? */
  mode_context    next;

  struct _kanjiMode *curMode;
  struct _tanContextRec	 *left, *right;

  /* ???????????????????? */
  struct RkRxDic *romdic;	/* ???????????????????? */
  wchar_t   romaji_buffer[ROMEBUFSIZE];
  /* ?????????????????? rStartp, rEndp ????????????????????????????????????
   * ????rStartp ????????????????????????????????????????????????????????
   * ????????????rEndp ?????????????????????????????????????????? 
   * romaji_buffer ????????????????????????????????????????????????????
   * romaji_buffer + rEndp ????????????????????????????????????????????
   * ????????romaji_buffer + rStartp ???? rEndp - rStartp ??????????????
   * ???????????? */
  int		  rEndp, rStartp, rCurs; /* ?????????????????????????? */
  wchar_t         kana_buffer[ROMEBUFSIZE];
  BYTE            rAttr[ROMEBUFSIZE], kAttr[ROMEBUFSIZE];
  int		  kEndp; /* ?????????????????????????????????? */
  int             kRStartp, kCurs;

  /* ?????????????????? */
  BYTE            myMinorMode;  /* yomiContext ?????????????????? */
  struct _kanjiMode *myEmptyMode;		/* empty ?????????????? */
  long		  generalFlags;		/* see below */
  long		  savedFlags;		/* ???????????????????????? */
  BYTE		  savedMinorMode;	/* ???????????????????? */
  BYTE		  allowedChars;		/* see jrkanji.h */
  BYTE		  henkanInhibition;	/* see below */
  int             cursup;		/* ?????????????????????? */
#define SUSPCHARBIAS 100
  int             n_susp_chars;

/* from henkanContext */
  /* ???????????????? */
  int            context;
  int		 kouhoCount;	/* ???? henkanNext ???????????????????? */
  wchar_t        echo_buffer[ROMEBUFSIZE];
  wchar_t        **allkouho; /* RkGetKanjiList????????????????????????????
				???????????????? */
  int            curbun;     /* ???????????? */
  int		 curIkouho;  /* ???????????? */
  int            nbunsetsu;  /* ???????? */

/* ifdef MEASURE_TIME */
  long		 proctime;   /* ????????(??????????????) */
  long		 rktime;     /* ????????(RK????????????) */
/* endif MEASURE_TIME */
/* end of from henkanContext */

/* ???????????????????? */
  int		 ye, ys, status;
/* ????????????????????(????????) */
  int		 cStartp, cRStartp; /* ?????????????????????????????? */

/* ???????????????????? */
  BYTE           inhibition;
  BYTE           jishu_kc, jishu_case;
  int            jishu_kEndp, jishu_rEndp;
  short          rmark;
/* ????????????????????(????????) */

/* adjustContext ???? */
  int kanjilen, bunlen;           /* ???????????????????? */
/* adjustContext ????(????????) */
  struct _kanjiMode *tanMode; /* ???????????????????? */
  int tanMinorMode;     /*        ??            */

  /* ?????????? */
  int		  last_rule;		/* ???????????????????????????????? */
  wchar_t	  *retbuf, *retbufp;
  int		  retbufsize;
  short           pmark, cmark; /* ?????????????????????????? */
  BYTE            englishtype;  /* ??????????(??????????) */
} yomiContextRec, *yomiContext;

/* for generalFlags */
#define CANNA_YOMI_MODE_SAVED		0x01L /* savedFlags ???????????????? */

#define CANNA_YOMI_BREAK_ROMAN		0x01L
#define CANNA_YOMI_CHIKUJI_MODE		0x02L
#define CANNA_YOMI_CHGMODE_INHIBITTED	0x04L
#define CANNA_YOMI_END_IF_KAKUTEI	0x08L
#define CANNA_YOMI_DELETE_DONT_QUIT	0x10L

#define CANNA_YOMI_IGNORE_USERSYMBOLS	0x20L
#define CANNA_YOMI_IGNORE_HENKANKEY	0x40L

#define CANNA_YOMI_BASE_CHIKUJI		0x80L /* ???????? */

/* for generalFlags also used in savedFlags */

/* ?????? ATTRFUNCS ?????????????????????? defmode ???????????????????? */
#define CANNA_YOMI_KAKUTEI		0x0100L
#define CANNA_YOMI_HENKAN		0x0200L
#define CANNA_YOMI_ZENKAKU		0x0400L
#define CANNA_YOMI_HANKAKU		0x0800L /* ?????????? */
#define CANNA_YOMI_HIRAGANA		0x1000L
#define CANNA_YOMI_KATAKANA		0x2000L
#define CANNA_YOMI_ROMAJI		0x4000L
#define CANNA_YOMI_JISHUFUNCS		0x7c00L
#define CANNA_YOMI_ATTRFUNCS		0x7f00L

#define CANNA_YOMI_BASE_HANKAKU		0x8000L /* ???????? */

/* kind of allowed input keys */
#define CANNA_YOMI_INHIBIT_NONE		0
#define CANNA_YOMI_INHIBIT_HENKAN	1
#define CANNA_YOMI_INHIBIT_JISHU	2
#define CANNA_YOMI_INHIBIT_ASHEX	4
#define CANNA_YOMI_INHIBIT_ASBUSHU	8
#define CANNA_YOMI_INHIBIT_ALL		15

/* ?????????????????????? */
#define NUMBERING 			1
#define CHARINSERT			2

#define CANNA_JISHU_UPPER		1
#define CANNA_JISHU_CAPITALIZE		2

/* englishtype */
#define CANNA_ENG_KANA			0 /* ???????????? */
#define CANNA_ENG_ENG1			1
#define CANNA_ENG_ENG2			2 /* ?????????????????????? */
#define CANNA_ENG_NO			3

/* yc->status ????????(??????) */

#define	CHIKUJI_ON_BUNSETSU		0x0001 /* ???????????? */
#define	CHIKUJI_OVERWRAP		0x0002 /* ?????????????????? */
#define	CHIKUJI_NULL_STATUS	        0 /* ???????????? */

/* yc ??????????????????(??????) */

#define adjustp(yc) (0< (yc)->bunlen)
#define jishup(yc) (0 < (yc)->jishu_kEndp)
#define chikujip(yc) ((yc)->generalFlags & CANNA_YOMI_CHIKUJI_MODE)
#define henkanp(yc) (0 < (yc)->nbunsetsu)

#define chikujiyomiremain(yc) ((yc)->cStartp < (yc)->kEndp)

typedef struct _ichiranContextRec {
  BYTE id;
  BYTE majorMode, minorMode;
  struct _kanjiMode *prevMode;	/* ?????????????? */
  mode_context    next;

  int            svIkouho;   /* ????????????????????????????(??????????) */
  int            *curIkouho; /* ???????????? */
  int            nIkouho;    /* ????????(??????????) */
  int		 tooSmall;   /* ?????????????????????????????????????????? */
  int            curIchar;   /* ??????????????????????????????????????
    							?????????????? */
  BYTE           inhibit;
  BYTE           flags;	     /* ?????????? */
  wchar_t        **allkouho; /* RkGetKanjiList????????????????????????????
				???????????????? */
  wchar_t        *glinebufp; /* ??????????????????????????????????????
				?????????????? */
  kouhoinfo      *kouhoifp;  /* ??????????????????????????????????????
				???????????? */
  glineinfo      *glineifp;  /* ??????????????????????????????????????
				???????????? */
} ichiranContextRec, *ichiranContext;

/* ???????????? */
#define ICHIRAN_ALLOW_CALLBACK 1 /* ???????????????????????? */
#define ICHIRAN_STAY_LONG    0x02 /* ???????????? */
#define ICHIRAN_NEXT_EXIT    0x04 /* ???? quit ???????? */


typedef struct _foirchiranContextRec {
  BYTE id;
  BYTE majorMode, minorMode;
  struct _kanjiMode *prevMode;	/* ?????????????? */
  mode_context    next;

  int            curIkouho;  /* ???????????? */
  wchar_t        **allkouho; /* RkGetKanjiList????????????????????????????
				???????????????? */
  menustruct     *table;  /* ?????????????????????? */
  int            *prevcurp;  /* ???????????????? */
} forichiranContextRec, *forichiranContext;

typedef struct _mountContextRec {
  BYTE id;
  BYTE majorMode, minorMode;
  struct _kanjiMode *prevMode;	/* ?????????????? */
  mode_context    next;

  BYTE            *mountOldStatus; /* ???????????????????????????? */
  BYTE            *mountNewStatus; /* ???????????????????????????? */
  char            **mountList;   /* ???????????????????????? */
  int            curIkouho;     /* ???????????? */
} mountContextRec, *mountContext;

typedef struct _tourokuContextRec {
  BYTE id;
  BYTE majorMode, minorMode;
  struct _kanjiMode *prevMode;	/* ?????????????? */
  mode_context    next;

  wchar_t        genbuf[ROMEBUFSIZE];
  wchar_t        qbuf[ROMEBUFSIZE];
  wchar_t        tango_buffer[ROMEBUFSIZE];
  int            tango_len;  /* ???????????????????????????? */
  wchar_t        yomi_buffer[ROMEBUFSIZE];
  int            yomi_len;   /* ???????????????????????????? */
  int            curHinshi;  /* ?????????? */
  int            workDic;    /* ???????????? */
  struct dicname *newDic;    /* ???????????? */
  wchar_t	 hcode[16];  /* ?????????????? */
  int		 katsuyou;   /* ?????????????????????? */
  wchar_t        **udic;     /* ?????????????????? (??????) */
  int            delContext; /* ?????????????????????????????????? */
} tourokuContextRec, *tourokuContext;

typedef struct _tanContextRec {
  BYTE id;
  BYTE majorMode, minorMode;
  struct _kanjiMode *prevMode;	/* ?????????????? */
  mode_context    next;

  struct _kanjiMode *curMode;
  struct _tanContextRec	 *left, *right;

  struct RkRxDic *romdic;	/* ???????????????????? */
  BYTE            myMinorMode;  /* yomiContext ?????????????????? */
  struct _kanjiMode *myEmptyMode;		/* empty ?????????????? */
  long generalFlags, savedFlags; /* yomiContext ???????? */
  BYTE		  savedMinorMode;	/* ???????????????????? */
  BYTE		  allowedChars;		/* see jrkanji.h */
  BYTE		  henkanInhibition;	/* see below */

  wchar_t *kanji, *yomi, *roma;
  BYTE *kAttr, *rAttr;
} tanContextRec, *tanContext;

struct moreTodo {
  BYTE          todo; /* ???????????????????? */
  BYTE          fnum; /* ???????????????????????????????????????????? */
  char		ch;   /* ???? */
};

/* ???????????????????????????????? */

struct ModeNameRecs {
  int           alloc;
  wchar_t       *name;
};

/* ?????????????????????????????????????????? */

#define DEFAULTINDEXSEPARATOR     '.'

/* 

  uiContext ????????????????????????????????????????????????????????
  XLookupKanjiString ????????????????????????????????????????????????
  ??????????????????????????????????????????????????????????????????
  ??????????????????????????????????????????????????????????????????
  ????????????????????????????????????????????????????
 
  ????????????????????????????????????????????????????????????
 
 */

typedef struct _uiContext {

  /* XLookupKanjiString?????????? */
  wchar_t        *buffer_return;
  int            n_buffer;
  wcKanjiStatus    *kanji_status_return;

  /* XLookupKanjiString?????????????????????????? */
  int		 nbytes;

  /* ?????????? */
  char ch;

  /* ???????????????????? */
  int		 contextCache;	 /* ?????????????????????????? */
  struct _kanjiMode *current_mode;
  BYTE		 majorMode, minorMode;	 /* ?????????? */

  short		 curkigo;	 /* ????????????(????????) */
  char           currussia;	 /* ????????????(??????????) */
  char           curgreek;	 /* ????????????(????????????) */
  char           curkeisen;	 /* ????????????(????) */
  short          curbushu;       /* ?????????????? */
  int            ncolumns;	 /* ???????????????????????????????????????? */
  wchar_t        genbuf[ROMEBUFSIZE];	/* ???????????? */
  short          strokecounter;  /* ????????????????????????
				    ???????????????????????????? */

  /* ?????????????????????? */
  char           *client_data;   /* ???????????????????????? */
  void           (*list_func) pro((char *, int, wchar_t **, int, int *));
                 /* ?????????????????????? */
  /* ?????? */
  char		 flags;		 /* ?????????? */
  char		 status;	 /* ??????????????????????????????????
				    ??????????????????
				     ????????
				     ??????????
				     ??????????
				     ????????
				    ????????(????????) */

  /* ???????????????????? */
  struct callback *cb;

  /* ?????????????????????????????????? */
  struct moreTodo more;

  /* ???????????????? */
  menustruct *prevMenu;

  /* ???????????????????????????????????????????????????????? */
  menuinfo *minfo;

  /* ???????????????????????????????????????????????????? */
  selectinfo *selinfo;

  /* ?????????????????????????? */
  mode_context   modec;		/* ???????????????????? */
} uiContextRec, *uiContext;

/* uiContext ?? flags ?????????????? */
#define PLEASE_CLEAR_GLINE	1	/* GLine ?????????? */
#define PCG_RECOGNIZED		2	/* GLine ???????????????? */
#define MULTI_SEQUENCE_EXECUTED	4	/* ???????????????????????????????? */

#define EVERYTIME_CALLBACK	0
#define EXIT_CALLBACK		1
#define QUIT_CALLBACK		2
#define AUX_CALLBACK		3

/* 
 * ???????????????????????????????????????????? 
 * ????????????????????????????????????????????????????????????
 */

struct funccfunc {
  BYTE funcid;
  int (*cfunc) pro((struct _uiContext *));
};

typedef struct _kanjiMode {
  int (*func) pro((struct _uiContext *, struct _kanjiMode *, int, int, int));
  BYTE *keytbl;
  int flags;			/* ???????? */
  struct funccfunc *ftbl;
} *KanjiMode, KanjiModeRec;

struct callback {
  int (*func[NCALLBACK]) pro((struct _uiContext *, int, mode_context));
  mode_context    env;
  struct callback *next;
};

/* ???????????????????? */
     
extern struct RkRxDic *romajidic;
extern struct RkRxDic *RkwOpenRoma pro((unsigned char *));

/*
 * ??????????????????????????
 */

struct dicname {
  struct dicname *next;
  char *name;
  int dictype;
  unsigned long dicflag;
};

/* dictype ???????????????????????? */
#define DIC_PLAIN 0
#define DIC_USER  1
#define DIC_BUSHU 2
#define DIC_GRAMMAR 3
#define DIC_RENGO 4
#define DIC_KATAKANA 5
#define DIC_HIRAGANA 6

/* dicflag ???????????????????????? */
#define DIC_NOT_MOUNTED  0
#define DIC_MOUNTED      1
#define DIC_MOUNT_FAILED 2

extern struct dicname *kanjidicnames;

/*
 * ??????????????????????????????????
 */

extern char *jrKanjiError;

/*
 * ??????????????????????????????????
 */

extern iroha_debug;

/*
 * ??????????????????????????????????
 */

#define IrohaFunctionKey(key) \
  ((0x80 <= (int)(unsigned char)(key) &&  \
    (int)(unsigned char)(key) <= 0x8b) || \
   (0x90 <= (int)(unsigned char)(key) &&  \
    (int)(unsigned char)(key) <= 0x9b) || \
   (0xe0 <= (int)(unsigned char)(key) &&  \
    (int)(unsigned char)(key) <= 0xff) )

/* selectOne ???????????????????????????????????????? */

#define WITHOUT_LIST_CALLBACK 0
#define WITH_LIST_CALLBACK    1

/*
 * Rk ????????????????????????????????????????
 */

#ifdef DEBUG
#include "traceRK.h"
#endif /* DEBUG */

/*
 * ??????????????????????????????
 */

#ifdef DEBUG
#define debug_message(fmt, x, y, z)	dbg_msg(fmt, x, y, z)
#else /* !DEBUG */
#define debug_message(fmt, x, y, z)
#endif /* !DEBUG */

/*
 * malloc ????????
 */

#ifdef DEBUG_ALLOC
extern char *debug_malloc pro((int));
extern int fail_malloc;
#define malloc(n) debug_malloc(n)
#endif /* DEBUG_MALLOC */

/*
 * ????????????????????????????
 */

typedef struct {
  char           *romaji_table; /* ??????????????????????????(EUC) */
  struct RkRxDic *romdic;	 /* ?????????????????? */
  int             romdic_owner;  /* ????????????????????Open?????? */
  long            flags;	 /* flags for yomiContext->generalFlags */
  KanjiMode       emode;	 /* current_mode ???????????? */
} newmode;

/* ?????????????????????????????????????????????????? */

typedef struct {
  wchar_t	key;		/* ???? */
  int		groupid;	/* ????????id */
  int           ncand;          /* ???????? */
  wchar_t       **cand;         /* ?????????? */
  wchar_t	*fullword;	/* ?????? (????1@????2@...????n@@) */
} keySupplement;

#define MAX_KEY_SUP 64

#define HEX_USUAL     0
#define HEX_IMMEDIATE 1

#define ModeInfoStyleIsString		0
#define ModeInfoStyleIsNumeric		1
#define ModeInfoStyleIsExtendedNumeric	2
#define MaxModeInfoStyle		ModeInfoStyleIsExtendedNumeric

#define killmenu(d) ((d)->prevMenu = (menustruct *)0)
#define	defineEnd(d) killmenu(d)
#define	deleteEnd(d) killmenu(d)

/* defmode??defselection??defmenu ?????????? */

typedef struct _extra_func {
  int  		fnum;		/* ???????? */
  int		keyword;	/* ?????????????????????????????????? */
  wchar_t	*display_name;	/* ???????????? */
  union {
    newmode 	*modeptr;	/* defmode ???????????????? */
    kigoIchiran	*kigoptr;	/* defselection ???????????????? */
    menustruct	*menuptr;	/* defmenu ???????????????? */
  } u;
  struct _extra_func *next;
} extraFunc;

#define EXTRA_FUNC_DEFMODE	1
#define EXTRA_FUNC_DEFSELECTION	2
#define EXTRA_FUNC_DEFMENU	3

#define tanbunMode(d, tan) /* tanContext ?????????????????? */ \
  { extern KanjiModeRec tankouho_mode; (d)->current_mode = &tankouho_mode; \
    (d)->modec = (mode_context)(tan); currentModeInfo(d); }

#define freeForIchiranContext(fc) free((char *)fc)
#define freeIchiranContext(ic) free((char *)ic)
#define freeYomiContext(yc) free((char *)yc)
#define freeCoreContext(cc) free((char *)cc)

#ifndef DICHOME
#define DICHOME "/usr/lib/canna/dic"
#endif

#define DEFAULT_CANNA_SERVER_NAME "cannaserver"

#ifndef	_UTIL_FUNCTIONS_DEF_

#define	_UTIL_FUNCTIONS_DEF_

/* ?????????????????????????????????????????????????????????? */
extern RkwGoto pro((char *, int));

extern void makeGLineMessage pro((uiContext, wchar_t *, int));
extern void makeGLineMessageFromStrings pro((uiContext, char *));
extern newmode *findExtraKanjiMode pro((int));
extern setWStrings pro((wchar_t **, char **, int));
extern WStrlen pro((wchar_t *));
extern wchar_t *WStrcat pro((wchar_t *, wchar_t *));
extern wchar_t *WStrcpy pro((wchar_t *, wchar_t *));
extern wchar_t *WStrncpy pro((wchar_t *, wchar_t *, int));
extern WStrncmp pro((wchar_t *, wchar_t *, int));
extern wchar_t *WString pro((char *));
extern prevMenuIfExist pro((uiContext));
extern showmenu pro((uiContext, menustruct *));
extern yomiContext
  newYomiContext pro((wchar_t *, int, int, int, int, int)),
  GetKanjiString pro((uiContext, wchar_t *, int, int, int, int, int,
		      int (*)(uiContext, int, mode_context),
		      int (*)(uiContext, int, mode_context),
		      int (*)(uiContext, int, mode_context)));
extern void restoreFlags pro((yomiContext));
extern void kPos2rPos pro((yomiContext, int, int, int *, int *));
extern void makeKanjiStatusReturn pro((uiContext, yomiContext));
extern wchar_t key2wchar pro((int, int *));
extern struct bukRec *internContext
  pro((unsigned int, unsigned int, uiContext));
extern void freeRomeStruct pro((uiContext));
extern void rmContext pro((unsigned int, unsigned int));
extern struct callback *pushCallback
  pro((uiContext, mode_context,
       int (*)(uiContext, int, mode_context),
       int (*)(uiContext, int, mode_context),
       int (*)(uiContext, int, mode_context),
       int (*)(uiContext, int, mode_context)));
extern void popCallback pro((uiContext));
extern void makeYomiReturnStruct pro((uiContext));
extern void moveToChikujiTanMode pro((uiContext));
extern void moveToChikujiYomiMode pro((uiContext));
extern void makeGLineMessageFromString pro((uiContext, char *));
extern void addWarningMesg pro((char *));
extern int prepareHenkanMode pro((uiContext));
extern void makeAllContextToBeClosed pro((int));
extern void Beep pro((void));
extern void freeAllMenuInfo pro((menuinfo *));
extern void freeMenu pro((menustruct *));
extern void restoreDefaultKeymaps pro((void));
extern void finExtMenu pro((void));
extern void popTourokuMode pro((uiContext));
extern void freeIchiranBuf pro((ichiranContext));
extern char *RkGetServerName pro((void));
extern void popForIchiranMode pro((uiContext));
extern void clisp_main pro((void));
extern void clisp_fin pro((void));
extern void popYomiMode pro((uiContext));
extern void freeTanContext pro((tanContext));
extern void enterJishuMode pro((uiContext, yomiContext));
extern void leaveJishuMode pro((uiContext, yomiContext));
extern void finishTanKakutei pro((uiContext));
extern void removeKana pro((uiContext, yomiContext, int, int));
extern void clearHenkanContext pro((yomiContext));
extern void doMuhenkan pro((uiContext, yomiContext));
extern void removeCurrentBunsetsu pro((uiContext, tanContext));

#endif /* _UTIL_FUNCTIONS_DEF_ */

#endif /* !_CANNA_H_ */
