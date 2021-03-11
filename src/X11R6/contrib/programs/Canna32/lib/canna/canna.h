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

#define STROKE_LIMIT 500 /* ���ȥ�������³���ڤ� */

#if defined(SYSV) || defined(SVR4) || __STDC__
# if defined(SYSV) || defined(SVR4)
#  include <memory.h>
# endif
# define bzero(buf, size) memset((char *)(buf), 0x00, (size))
# define bcopy(src, dst, size) memcpy((char *)(dst), (char *)(src), (size))
#endif

typedef unsigned char BYTE;

/*
 * CANNALIBDIR  -- �����ƥ�Υ������ޥ����ե��������޻������Ѵ�
 *                 �ơ��֥뤬�֤����ǥ��쥯�ȥꡣ
 */

#ifndef CANNALIBDIR
#define CANNALIBDIR "/usr/lib/canna"
#endif

/* flags �ξ��� */
#define CANNA_KANJIMODE_TABLE_SHARED	01
#define CANNA_KANJIMODE_EMPTY_MODE	02

/* func ���軰���� */
#define KEY_CALL  0
#define KEY_CHECK 1
#define KEY_SET   2

extern int searchfunc();
extern BYTE default_kmap[];

/* menuitem -- ��˥塼ɽ���ι��ܤ��������ơ��֥� */

typedef struct _menuitem {
  int flag; /* ���򸫤� */
  union {
    struct _menustruct *menu_next; /* ��˥塼�ؤΥݥ��� */
    int fnum;    /* ��ǽ�ֹ� */
    char *misc;  /* ����¾(lisp �Υ���ܥ�ʤ�) */
  } u;
} menuitem;

#define MENU_SUSPEND 0 /* �ޤ���ޤäƤ��ʤ�(lisp �Υ���ܥ�) */
#define MENU_MENU    1 /* ��˥塼 */
#define MENU_FUNC    2 /* ��ǽ�ֹ� */

/* menustruct -- ��˥塼�򤷤��빽¤�� */

typedef struct _menustruct {
  int     nentries; /* ��˥塼�ι��ܤο� */
  wchar_t **titles; /* ��˥塼�θ��Ф��ꥹ�� */
  wchar_t *titledata; /* ��Υꥹ�Ȥμ���ʸ���� */
  menuitem *body;   /* ��˥塼�����(����) */
  int     modeid;   /* ��˥塼�Υ⡼���ֹ� */
  struct _menustruct *prev; /* ������Υ�˥塼�ؤΥݥ��� */
} menustruct;

typedef struct _menuinfo {
  menustruct *mstruct; /* �ɤΥ�˥塼�� */
  int        curnum;   /* ���ʤ������򤵤줿�ֹ�Ϥ���Ǥ��� */
  struct _menuinfo *next;
} menuinfo;

/* defselection ��������줿����ط��ΰ�����ȤäƤ�����¤�� */

typedef struct {
  wchar_t	**kigo_data;	/* ����ɽ���γ����Ǥ����� */
  wchar_t	*kigo_str;	/* ����ɽ���������Ǥ���������� */
  int		kigo_size;	/* ���Ǥο� */
  int		kigo_mode;	/* ���ΤȤ��Υ⡼�� */
} kigoIchiran;

typedef struct _selectinfo {
  kigoIchiran	*ichiran;	/* �ɤΰ����� */
  int		curnum;		/* �������򤵤줿�ֹ� */
  struct _selectinfo *next;
} selectinfo;

/*
 * glineinfo -- �������ɽ���Τ��������������Ǽ���Ƥ�������ι�¤�Ρ�
 * ���줾��Υ��Фϰʲ��ΰ�̣����ġ�
 *
 * glkosu -- ���ιԤˤ������ο�
 * glhead -- ���ιԤ���Ƭ���䤬��kouhoinfo�β����ܤ�(0���������)
 * gllen  -- ���ιԤ�ɽ�����뤿���ʸ�����Ĺ��
 * gldata -- ���ιԤ�ɽ�����뤿���ʸ����ؤΥݥ���
 */

typedef struct {
  int glkosu;
  int glhead;
  int gllen;
  wchar_t *gldata;
} glineinfo;

/*
 * kouhoinfo -- ��������Τ��������������Ǽ���Ƥ�������ι�¤��
 * ���줾��Υ��Фϰʲ��ΰ�̣����ġ�
 *
 * khretsu -- ���θ��䤬�����
 * khpoint -- ���θ���ιԤΤʤ��Ǥΰ���
 * khdata -- ���θ����ʸ����ؤΥݥ���
 */

typedef struct {
  int khretsu;
  int khpoint;
  wchar_t *khdata;
} kouhoinfo;

#define ROMEBUFSIZE 	1024
#define	BANGOSIZE	2	/* �������γƸ�����ֹ�Υ����� */
#define	BANGOMAX   	9	/* ���������κ������� */

#define	KIGOBANGOMAX   	16	/* ���������κ������� */
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
  struct _kanjiMode *prevMode; /* �������Υ⡼�� */
  struct _coreContextRec *next;
} coreContextRec, *coreContext;

typedef coreContext mode_context;

typedef struct  _yomiContextRec {
  /* core �����Ʊ������ */
  BYTE id;
  BYTE majorMode, minorMode;
  struct _kanjiMode *prevMode;	/* �������Υ⡼�� */
  mode_context    next;

  struct _kanjiMode *curMode;
  struct _tanContextRec	 *left, *right;

  /* ���޻������Ѵ��ط� */
  struct RkRxDic *romdic;	/* ���޻������Ѵ����� */
  wchar_t   romaji_buffer[ROMEBUFSIZE];
  /* ���޻��Хåե��� rStartp, rEndp �Σ��ĤΥ���ǥå����ˤ�äƴ�������
   * �롣rStartp �ϥ��ʤ��Ѵ��Ǥ��ʤ��ä����޻��κǽ��ʸ���ؤΥ���ǥ�
   * �����Ǥ��ꡢrEndp �Ͽ����˥��޻������Ϥ�����ˡ���Ǽ���٤� 
   * romaji_buffer ��Υ���ǥå����Ǥ��롣���������Ϥ������޻��ϡ�
   * romaji_buffer + rEndp �����˳�Ǽ���졢���Υ��޻��򥫥ʤ��Ѵ���
   * ����ϡ�romaji_buffer + rStartp ���� rEndp - rStartp �Х��Ȥ�ʸ����
   * �оݤȤʤ롣 */
  int		  rEndp, rStartp, rCurs; /* ���޻��Хåե��Υݥ��� */
  wchar_t         kana_buffer[ROMEBUFSIZE];
  BYTE            rAttr[ROMEBUFSIZE], kAttr[ROMEBUFSIZE];
  int		  kEndp; /* ���ʥХåե��κǸ�򲡤���ݥ��� */
  int             kRStartp, kCurs;

  /* ����¾�Υ��ץ���� */
  BYTE            myMinorMode;  /* yomiContext ��ͭ�Υޥ��ʥ⡼�� */
  struct _kanjiMode *myEmptyMode;		/* empty �⡼�ɤϤɤ줫 */
  long		  generalFlags;		/* see below */
  long		  savedFlags;		/* ��Υե饰�ΰ����Υ����� */
  BYTE		  savedMinorMode;	/* �ޥ��ʥ⡼�ɤΥ����� */
  BYTE		  allowedChars;		/* see jrkanji.h */
  BYTE		  henkanInhibition;	/* see below */
  int             cursup;		/* ���ʤ����ɤλ��˻Ȥ� */
#define SUSPCHARBIAS 100
  int             n_susp_chars;

/* from henkanContext */
  /* ���ʴ����Ѵ��ط� */
  int            context;
  int		 kouhoCount;	/* ���� henkanNext ��Ϣ³���Ʋ����줿�� */
  wchar_t        echo_buffer[ROMEBUFSIZE];
  wchar_t        **allkouho; /* RkGetKanjiList��������ʸ���������ˤ���
				�ȤäƤ����Ȥ��� */
  int            curbun;     /* ������ʸ�� */
  int		 curIkouho;  /* �����ȸ��� */
  int            nbunsetsu;  /* ʸ��ο� */

/* ifdef MEASURE_TIME */
  long		 proctime;   /* ��������(�Ѵ��Ƿ�¬����) */
  long		 rktime;     /* ��������(RK�ˤ��������) */
/* endif MEASURE_TIME */
/* end of from henkanContext */

/* �༡����ƥ����Ȥ��� */
  int		 ye, ys, status;
/* �༡����ƥ����Ȥ���(�����ޤ�) */
  int		 cStartp, cRStartp; /* �༡���ɤߤȤ��ƻĤäƤ�����ʬ */

/* ���拾��ƥ����Ȥ��� */
  BYTE           inhibition;
  BYTE           jishu_kc, jishu_case;
  int            jishu_kEndp, jishu_rEndp;
  short          rmark;
/* ���拾��ƥ����Ȥ���(�����ޤ�) */

/* adjustContext ���� */
  int kanjilen, bunlen;           /* ������ʬ��ʸ���Ĺ�� */
/* adjustContext ����(�����ޤ�) */
  struct _kanjiMode *tanMode; /* ñ����ΤȤ��Υ⡼�� */
  int tanMinorMode;     /*        ��            */

  /* ������ѿ� */
  int		  last_rule;		/* ����Υ����Ѵ��˻Ȥ�줿�롼�� */
  wchar_t	  *retbuf, *retbufp;
  int		  retbufsize;
  short           pmark, cmark; /* �������Υޡ����Ⱥ��Υޡ��� */
  BYTE            englishtype;  /* �Ѹ쥿����(�ʲ��򸫤�) */
} yomiContextRec, *yomiContext;

/* for generalFlags */
#define CANNA_YOMI_MODE_SAVED		0x01L /* savedFlags �ˤ����Ȥ��ʤ� */

#define CANNA_YOMI_BREAK_ROMAN		0x01L
#define CANNA_YOMI_CHIKUJI_MODE		0x02L
#define CANNA_YOMI_CHGMODE_INHIBITTED	0x04L
#define CANNA_YOMI_END_IF_KAKUTEI	0x08L
#define CANNA_YOMI_DELETE_DONT_QUIT	0x10L

#define CANNA_YOMI_IGNORE_USERSYMBOLS	0x20L
#define CANNA_YOMI_IGNORE_HENKANKEY	0x40L

#define CANNA_YOMI_BASE_CHIKUJI		0x80L /* �����༡ */

/* for generalFlags also used in savedFlags */

/* �ʲ��� ATTRFUNCS �˥ޥ��������ӥåȤ� defmode ��°���Ȥ��ƻȤ��� */
#define CANNA_YOMI_KAKUTEI		0x0100L
#define CANNA_YOMI_HENKAN		0x0200L
#define CANNA_YOMI_ZENKAKU		0x0400L
#define CANNA_YOMI_HANKAKU		0x0800L /* �ºݤ�Ⱦ�� */
#define CANNA_YOMI_HIRAGANA		0x1000L
#define CANNA_YOMI_KATAKANA		0x2000L
#define CANNA_YOMI_ROMAJI		0x4000L
#define CANNA_YOMI_JISHUFUNCS		0x7c00L
#define CANNA_YOMI_ATTRFUNCS		0x7f00L

#define CANNA_YOMI_BASE_HANKAKU		0x8000L /* ����Ⱦ�� */

/* kind of allowed input keys */
#define CANNA_YOMI_INHIBIT_NONE		0
#define CANNA_YOMI_INHIBIT_HENKAN	1
#define CANNA_YOMI_INHIBIT_JISHU	2
#define CANNA_YOMI_INHIBIT_ASHEX	4
#define CANNA_YOMI_INHIBIT_ASBUSHU	8
#define CANNA_YOMI_INHIBIT_ALL		15

/* ��������Τ���Υե饰 */
#define NUMBERING 			1
#define CHARINSERT			2

#define CANNA_JISHU_UPPER		1
#define CANNA_JISHU_CAPITALIZE		2

/* englishtype */
#define CANNA_ENG_KANA			0 /* �̾����뤳�� */
#define CANNA_ENG_ENG1			1
#define CANNA_ENG_ENG2			2 /* ξü�˶������äƤ��� */
#define CANNA_ENG_NO			3

/* yc->status �Υե饰(�༡��) */

#define	CHIKUJI_ON_BUNSETSU		0x0001 /* ʸ���ˤ��� */
#define	CHIKUJI_OVERWRAP		0x0002 /* ʸ�ᤫ���ɤ߾��֡� */
#define	CHIKUJI_NULL_STATUS	        0 /* ��Τ�ä��� */

/* yc ��Ȥ��⡼�ɤζ���(ͥ���) */

#define adjustp(yc) (0< (yc)->bunlen)
#define jishup(yc) (0 < (yc)->jishu_kEndp)
#define chikujip(yc) ((yc)->generalFlags & CANNA_YOMI_CHIKUJI_MODE)
#define henkanp(yc) (0 < (yc)->nbunsetsu)

#define chikujiyomiremain(yc) ((yc)->cStartp < (yc)->kEndp)

typedef struct _ichiranContextRec {
  BYTE id;
  BYTE majorMode, minorMode;
  struct _kanjiMode *prevMode;	/* �������Υ⡼�� */
  mode_context    next;

  int            svIkouho;   /* �����ȸ�������ȤäƤ���(����ɽ����) */
  int            *curIkouho; /* �����ȸ��� */
  int            nIkouho;    /* ����ο�(����ɽ����) */
  int		 tooSmall;   /* �������������Ƹ���������Ф��ʤ���ե饰 */
  int            curIchar;   /* ̤����ʸ���󤢤��ñ����Ͽ��ñ�����Ϥ�
    							��Ƭʸ���ΰ��� */
  BYTE           inhibit;
  BYTE           flags;	     /* ���򸫤Ƥ� */
  wchar_t        **allkouho; /* RkGetKanjiList��������ʸ���������ˤ���
				�ȤäƤ����Ȥ��� */
  wchar_t        *glinebufp; /* ��������Τ����Ԥ�ɽ�����뤿���ʸ��
				��ؤΥݥ��� */
  kouhoinfo      *kouhoifp;  /* ��������ط��ξ�����Ǽ���Ƥ�����¤��
				�ؤΥݥ��� */
  glineinfo      *glineifp;  /* ��������ط��ξ�����Ǽ���Ƥ�����¤��
				�ؤΥݥ��� */
} ichiranContextRec, *ichiranContext;

/* �ե饰�ΰ�̣ */
#define ICHIRAN_ALLOW_CALLBACK 1 /* ������Хå��򤷤Ƥ��ɤ� */
#define ICHIRAN_STAY_LONG    0x02 /* ���֤�ȴ���� */
#define ICHIRAN_NEXT_EXIT    0x04 /* ���� quit ��ȴ���� */


typedef struct _foirchiranContextRec {
  BYTE id;
  BYTE majorMode, minorMode;
  struct _kanjiMode *prevMode;	/* �������Υ⡼�� */
  mode_context    next;

  int            curIkouho;  /* �����ȸ��� */
  wchar_t        **allkouho; /* RkGetKanjiList��������ʸ���������ˤ���
				�ȤäƤ����Ȥ��� */
  menustruct     *table;  /* ʸ����ȴؿ��Υơ��֥� */
  int            *prevcurp;  /* ���Υ����ȸ��� */
} forichiranContextRec, *forichiranContext;

typedef struct _mountContextRec {
  BYTE id;
  BYTE majorMode, minorMode;
  struct _kanjiMode *prevMode;	/* �������Υ⡼�� */
  mode_context    next;

  BYTE            *mountOldStatus; /* �ޥ���Ȥ���Ƥ��뤫���ʤ��� */
  BYTE            *mountNewStatus; /* �ޥ���Ȥ���Ƥ��뤫���ʤ��� */
  char            **mountList;   /* �ޥ���Ȳ�ǽ�ʼ���ΰ��� */
  int            curIkouho;     /* �����ȸ��� */
} mountContextRec, *mountContext;

typedef struct _tourokuContextRec {
  BYTE id;
  BYTE majorMode, minorMode;
  struct _kanjiMode *prevMode;	/* �������Υ⡼�� */
  mode_context    next;

  wchar_t        genbuf[ROMEBUFSIZE];
  wchar_t        qbuf[ROMEBUFSIZE];
  wchar_t        tango_buffer[ROMEBUFSIZE];
  int            tango_len;  /* ñ����Ͽ��ñ���ʸ�����Ĺ�� */
  wchar_t        yomi_buffer[ROMEBUFSIZE];
  int            yomi_len;   /* ñ����Ͽ���ɤߤ�ʸ�����Ĺ�� */
  int            curHinshi;  /* �ʻ������ */
  int            workDic;    /* ����Ѥμ��� */
  struct dicname *newDic;    /* �ɲä��뼭�� */
  wchar_t	 hcode[16];  /* ñ����Ͽ���ʻ� */
  int		 katsuyou;   /* ñ����Ͽ��ư��γ��ѷ� */
  wchar_t        **udic;     /* ñ����Ͽ�Ǥ��뼭�� (����̾) */
  int            delContext; /* ñ�����ǣ��Ĥμ����ޥ���Ȥ��� */
} tourokuContextRec, *tourokuContext;

typedef struct _tanContextRec {
  BYTE id;
  BYTE majorMode, minorMode;
  struct _kanjiMode *prevMode;	/* �������Υ⡼�� */
  mode_context    next;

  struct _kanjiMode *curMode;
  struct _tanContextRec	 *left, *right;

  struct RkRxDic *romdic;	/* ���޻������Ѵ����� */
  BYTE            myMinorMode;  /* yomiContext ��ͭ�Υޥ��ʥ⡼�� */
  struct _kanjiMode *myEmptyMode;		/* empty �⡼�ɤϤɤ줫 */
  long generalFlags, savedFlags; /* yomiContext �Υ��ԡ� */
  BYTE		  savedMinorMode;	/* �ޥ��ʥ⡼�ɤΥ����� */
  BYTE		  allowedChars;		/* see jrkanji.h */
  BYTE		  henkanInhibition;	/* see below */

  wchar_t *kanji, *yomi, *roma;
  BYTE *kAttr, *rAttr;
} tanContextRec, *tanContext;

struct moreTodo {
  BYTE          todo; /* ��äȤ���Ρ��򼨤� */
  BYTE          fnum; /* �ؿ��ֹ档���ʤ鼡��ʸ���Ǽ�����뤳�Ȥ򤹤� */
  char		ch;   /* ʸ�� */
};

/* �⡼��̾���Ǽ����ǡ����η���� */

struct ModeNameRecs {
  int           alloc;
  wchar_t       *name;
};

/* �������ֹ�Υ��ѥ졼�����Υǥե���Ȥ���� */

#define DEFAULTINDEXSEPARATOR     '.'

/* 

  uiContext �ϥ��޻������Ѵ������ʴ����Ѵ��˻Ȥ��빽¤�ΤǤ��롣
  XLookupKanjiString �ʤɤˤ���Ѵ��ϡ�������ɥ���ʬΥ���줿ʣ������
  �ϥݡ��Ȥ��б����Ƥ���Τǡ�������Υ��޻��ξ���䡢���ʴ����Ѵ�
  ���ͻҤʤɤ򤽤줾��Υ�����ɥ����ʬΥ�����ݻ����Ƥ����ʤ���Ф�
  ��ʤ������ι�¤�ΤϤ��Τ���˻Ȥ��빽¤�ΤǤ��롣
 
  ��¤�ΤΥ��Ф��ɤΤ褦�ʤ�Τ����뤫�ϡ�����򻲾Ȥ��뤳��
 
 */

typedef struct _uiContext {

  /* XLookupKanjiString�Υѥ�᥿ */
  wchar_t        *buffer_return;
  int            n_buffer;
  wcKanjiStatus    *kanji_status_return;

  /* XLookupKanjiString������ͤǤ���ʸ�����Ĺ�� */
  int		 nbytes;

  /* ����饯�� */
  char ch;

  /* ���ߥ����Х�ǡ��� */
  int		 contextCache;	 /* �Ѵ�����ƥ����ȥ���å��� */
  struct _kanjiMode *current_mode;
  BYTE		 majorMode, minorMode;	 /* ľ���Τ�� */

  short		 curkigo;	 /* �����ȵ���(��������) */
  char           currussia;	 /* �����ȵ���(����ʸ��) */
  char           curgreek;	 /* �����ȵ���(���ꥷ��ʸ��) */
  char           curkeisen;	 /* �����ȵ���(����) */
  short          curbushu;       /* ����������̾ */
  int            ncolumns;	 /* ��ԤΥ���������������λ����Ѥ����� */
  wchar_t        genbuf[ROMEBUFSIZE];	/* ���ѥХåե� */
  short          strokecounter;  /* �������ȥ����Υ�����
				    ���޻��⡼�ɤǥ��ꥢ����� */

  /* �ꥹ�ȥ�����Хå���Ϣ */
  char           *client_data;   /* ���ץꥱ��������ѥǡ��� */
  void           (*list_func) pro((char *, int, wchar_t **, int, int *));
                 /* �ꥹ�ȥ�����Хå��ؿ� */
  /* ����¾ */
  char		 flags;		 /* ���򸫤Ƥ� */
  char		 status;	 /* �ɤΤ褦�ʾ��֤��֤ä��Τ��򼨤���
				    ���Υ⡼�ɤȤ��ơ�
				     ��������
				     ��������λ
				     ����������
				     ������¾
				    �����롣(���򸫤�) */

  /* ������Хå��������� */
  struct callback *cb;

  /* ��äȤ��뤳�Ȥ������Ȥ�����¤�� */
  struct moreTodo more;

  /* �����åȥ������� */
  menustruct *prevMenu;

  /* �ƥ�˥塼�����Ф줿�ֹ��Ͽ���Ƥ�����¤�ΤؤΥݥ��� */
  menuinfo *minfo;

  /* �ư��������Ф줿�ֹ��Ͽ���Ƥ�����¤�ΤؤΥݥ��� */
  selectinfo *selinfo;

  /* ���֥���ƥ����ȤؤΥ�� */
  mode_context   modec;		/* ���������ˤĤʤ�ͽ�� */
} uiContextRec, *uiContext;

/* uiContext �� flags �ΥӥåȤΰ�̣ */
#define PLEASE_CLEAR_GLINE	1	/* GLine ��ä��Ƥ� */
#define PCG_RECOGNIZED		2	/* GLine �򼡤Ͼä��ޤ��� */
#define MULTI_SEQUENCE_EXECUTED	4	/* ���ä��ޥ���������󥹤��Ԥ�줿 */

#define EVERYTIME_CALLBACK	0
#define EXIT_CALLBACK		1
#define QUIT_CALLBACK		2
#define AUX_CALLBACK		3

/* 
 * ���ʴ����Ѵ��Τ�����͡��ʥ����ޥåץơ��֥� 
 * �����ޥåץơ��֥�Ͻ����ؿ��ؤΥݥ��󥿤�����ȤʤäƤ��롣
 */

struct funccfunc {
  BYTE funcid;
  int (*cfunc) pro((struct _uiContext *));
};

typedef struct _kanjiMode {
  int (*func) pro((struct _uiContext *, struct _kanjiMode *, int, int, int));
  BYTE *keytbl;
  int flags;			/* ���򸫤� */
  struct funccfunc *ftbl;
} *KanjiMode, KanjiModeRec;

struct callback {
  int (*func[NCALLBACK]) pro((struct _uiContext *, int, mode_context));
  mode_context    env;
  struct callback *next;
};

/* ���޻������Ѵ����� */
     
extern struct RkRxDic *romajidic;
extern struct RkRxDic *RkwOpenRoma pro((unsigned char *));

/*
 * �����̾��������Ƥ����ѿ�
 */

struct dicname {
  struct dicname *next;
  char *name;
  int dictype;
  unsigned long dicflag;
};

/* dictype �ˤϰʲ��Τ����줫������ */
#define DIC_PLAIN 0
#define DIC_USER  1
#define DIC_BUSHU 2
#define DIC_GRAMMAR 3
#define DIC_RENGO 4
#define DIC_KATAKANA 5
#define DIC_HIRAGANA 6

/* dicflag �ˤϰʲ��Τ����줫������ */
#define DIC_NOT_MOUNTED  0
#define DIC_MOUNTED      1
#define DIC_MOUNT_FAILED 2

extern struct dicname *kanjidicnames;

/*
 * ���顼�Υ�å�����������Ƥ����ѿ�
 */

extern char *jrKanjiError;

/*
 * �ǥХ�ʸ��ɽ�����뤫�ɤ����Υե饰
 */

extern iroha_debug;

/*
 * �����������󥹤�ȯ������褦�ʥ���
 */

#define IrohaFunctionKey(key) \
  ((0x80 <= (int)(unsigned char)(key) &&  \
    (int)(unsigned char)(key) <= 0x8b) || \
   (0x90 <= (int)(unsigned char)(key) &&  \
    (int)(unsigned char)(key) <= 0x9b) || \
   (0xe0 <= (int)(unsigned char)(key) &&  \
    (int)(unsigned char)(key) <= 0xff) )

/* selectOne �ǥ�����Хå���ȼ�����ɤ�����ɽ���ޥ��� */

#define WITHOUT_LIST_CALLBACK 0
#define WITH_LIST_CALLBACK    1

/*
 * Rk �ؿ���ȥ졼�����뤿���̾���ν񤭴�����
 */

#ifdef DEBUG
#include "traceRK.h"
#endif /* DEBUG */

/*
 * �ǥХ���å����������ѤΥޥ���
 */

#ifdef DEBUG
#define debug_message(fmt, x, y, z)	dbg_msg(fmt, x, y, z)
#else /* !DEBUG */
#define debug_message(fmt, x, y, z)
#endif /* !DEBUG */

/*
 * malloc �ΥǥХ�
 */

#ifdef DEBUG_ALLOC
extern char *debug_malloc pro((int));
extern int fail_malloc;
#define malloc(n) debug_malloc(n)
#endif /* DEBUG_MALLOC */

/*
 * �������⡼�ɤ�������빽¤��
 */

typedef struct {
  char           *romaji_table; /* ���޻������Ѵ��ơ��֥�̾(EUC) */
  struct RkRxDic *romdic;	 /* ���޻�����¤�� */
  int             romdic_owner;  /* ���޻������ʬ��Open������ */
  long            flags;	 /* flags for yomiContext->generalFlags */
  KanjiMode       emode;	 /* current_mode �����빽¤�� */
} newmode;

/* ���޻������Ѵ�����­���륭����ʸ�����Ѵ��ơ��֥� */

typedef struct {
  wchar_t	key;		/* ���� */
  int		groupid;	/* ���롼��id */
  int           ncand;          /* ����ο� */
  wchar_t       **cand;         /* ��������� */
  wchar_t	*fullword;	/* ������ (����1@����2@...����n@@) */
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

/* defmode��defselection��defmenu �Ѥι�¤�� */

typedef struct _extra_func {
  int  		fnum;		/* �ؿ��ֹ� */
  int		keyword;	/* �������⡼�ɤ�������줿������� */
  wchar_t	*display_name;	/* �⡼��ɽ��̾ */
  union {
    newmode 	*modeptr;	/* defmode ���б����빽¤�� */
    kigoIchiran	*kigoptr;	/* defselection ���б����빽¤�� */
    menustruct	*menuptr;	/* defmenu ���б����빽¤�� */
  } u;
  struct _extra_func *next;
} extraFunc;

#define EXTRA_FUNC_DEFMODE	1
#define EXTRA_FUNC_DEFSELECTION	2
#define EXTRA_FUNC_DEFMENU	3

#define tanbunMode(d, tan) /* tanContext ��Ϣ�⡼�ɤؤΰܹ� */ \
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

/* �褯���ڥ�ߥ�����Τǥ���ѥ�����ˤҤä�����褦������� */
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
