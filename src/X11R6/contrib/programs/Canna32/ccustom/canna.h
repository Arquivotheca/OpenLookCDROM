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
 * @(#) 102.1 $Id: canna.h,v 1.11 1994/05/31 13:23:15 kon Exp $
 * $Revision: 1.11 $
 * $Author: kon $
 */

#ifndef _CANNA_H_
#define _CANNA_H_

#include "widedef.h"

#include <stdio.h>

#define	WCHARSIZE	sizeof(wchar_t)

/* #include "sglobal.h" ����ϥ�꡼�����ˤϥ����Ȥ�Ϥ������� */
#define necKanjiError jrKanjiError

/*#include "jrkanji.h"*/

#if defined(SYSV) || defined(SVR4) || __STDC__
# if defined(SYSV) || defined(SVR4)
#  include <memory.h>
# endif
# define bzero(buf, size) memset((char *)(buf), 0x00, (size))
# define bcopy(src, dst, size) memcpy((char *)(dst), (char *)(src), (size))
#endif

typedef unsigned char BYTE;

/*
 * LIBDIR  -- �����ƥ�Υ������ޥ����ե��������޻������Ѵ��ơ��֥뤬
 *            �֤����ǥ��쥯�ȥꡣ
 */

#ifndef CANNALIBDIR
#define CANNALIBDIR "/usr/lib/canna"
#endif

#define XKanjiStatus          jrKanjiStatus
#define XKanjiStatusWithValue jrKanjiStatusWithValue

/* 
 * ���ʴ����Ѵ��Τ�����͡��ʥ����ޥåץơ��֥� 
 * �����ޥåץơ��֥�Ͻ����ؿ��ؤΥݥ��󥿤�����ȤʤäƤ��롣
 */

typedef struct {
  int (*func)();
  unsigned char *keytbl;
  int flags;			/* ���򸫤� */
  int (**ftbl)();
} *KanjiMode, KanjiModeRec;

struct funccfunc {
  unsigned char funcid;
  int (*cfunc)();
};

/* flags �ξ��� */
#define CANNA_KANJIMODE_TABLE_SHARED	01
#define CANNA_KANJIMODE_EMPTY_MODE	02

/* func ���軰���� */
#define KEY_CALL  0
#define KEY_CHECK 1
#define KEY_SET   2

extern BYTE default_kmap[];

#define CANNA_FULL_VERBOSE 2

/* menuitem -- ��˥塼ɽ���ι��ܤ��������ơ��֥� */

typedef struct _menuitem {
  wchar_t *title;
  int (*func)();
  struct _menuitem *menu_next;
  int *prev_kouho;
  int minorMode;
} menuitem;

#define NEXT_MENU (int (*)())0 /* menuitem �� func �ե�����ɤ����� */

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
#define	BANGOSIZE	2	/* �������γƸ�����ֹ�ΥХ��ȿ� */
#define	REVSIZE		2	/* ���������ֹ�Υ�С����ΥХ��ȿ� */
#define	BANGOMAX   	9	/* ���������κ������� */
#define KIGOSIZE	1	/* �������ΥХ��ȿ� */
#define GL_KUHAKUSIZE	1	/* �����ֹ�ζ���ʸ���ΥХ��ȿ� */
#define KG_KUHAKUSIZE	1	/* ����δ֤ζ���ʸ���ΥХ��ȿ� */
#define KIGOCOL		(KIGOSIZE + KG_KUHAKUSIZE)
					/* bangomax��׻����뤿��ο� */
#define	KIGOBANGOMAX   	16	/* ���������κ������� */
#define HINSHISUU	14
#define HINSHIBUF	256
#define GOBISUU		9

#define	ON		1
#define	OFF		-1

#define	NG		-1

#define NO_CALLBACK	0
#define NCALLBACK	4

#define	JISHU_HIRA	0
#define JISHU_ZEN_KATA	1
#define JISHU_HAN_KATA	2
#define JISHU_ZEN_ALPHA	3
#define JISHU_HAN_ALPHA	4
#define MAX_JISHU	5

#define  SENTOU        1
#define  HENKANSUMI    2
#define  SUPKEY        4
#define  WARIKOMIMOJI  8
#define  SHIRIKIRE    16
#define  ATAMAKIRE    32

typedef char *mode_context;

struct callback {
  int (*func[NCALLBACK])();
  mode_context    env;
  struct callback *next;
};

/* identifier for each context structures */
#define CORE_CONTEXT       (char)0
#define YOMI_CONTEXT       (char)1
#define JISHU_CONTEXT      (char)2
#define HENKAN_CONTEXT     (char)3
#define ICHIRAN_CONTEXT    (char)4
#define FORICHIRAN_CONTEXT (char)5
#define MOUNT_CONTEXT      (char)6
#define TOUROKU_CONTEXT    (char)7
#define ADJUST_CONTEXT     (char)8
#define CHIKUJI_CONTEXT    (char)9

typedef struct _coreContextRec {
  char id;
  char majorMode, minorMode;
  struct callback c;
  KanjiMode	  prevMode; /* �������Υ⡼�� */
  mode_context    next;
} coreContextRec, *coreContext;

typedef struct  _yomiContextRec {
  /* core �����Ʊ������ */
  char id;
  char majorMode, minorMode;
  struct callback c;
  KanjiMode	  prevMode;	/* �������Υ⡼�� */
  mode_context    next;

  /* ���޻������Ѵ��ط� */
  struct RkwRxDic *romdic;	/* ���޻������Ѵ����� */
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
  KanjiMode       myEmptyMode;		/* empty �⡼�ɤϤɤ줫 */
  long		  generalFlags;		/* see below */
  char		  allowedChars;		/* see jrkanji.h */
  char		  henkanInhibition;	/* see below */
  int		  (*henkanCallback)();	/* �Ѵ���Ԥ����ˤ����ѿ��˥��ɥ쥹
             �����ꤵ��Ƥ���ʤ�Ф��Υ��ɥ쥹�δؿ����̾�δؿ��������
             �Ƥ֡����ꤵ��Ƥ��ʤ������̾�δؿ���Ƥ֡�
               �����Ѵ��ʤɤ��Ѵ������������줿�����ü�ʽ������Ԥ���Τ�
             �б����뤿����դ�����
	       �Ѵ�������Хå��ϰ����Ȥ��Ƥɤ��Ѵ��ؿ����ƤФ줿�Τ��򼨤�
	     �ֹ��ȼ����*/
  int             cursup;		/* ���ʤ����ɤλ��˻Ȥ� */
#define SUSPCHARBIAS 100
  int             n_susp_chars;

  /* ������ѿ� */
  wchar_t	  *retbuf, *retbufp;
  int		  retbufsize;
} yomiContextRec, *yomiContext;

/* for generalFlags */
#define CANNA_YOMI_BREAK_ROMAN		0x1L
#define CANNA_YOMI_CHGMODE_INHIBITTED	0x2L
#define CANNA_YOMI_END_IF_KAKUTEI	0x4L
#define CANNA_YOMI_DELETE_DONT_QUIT	0x8L

#define CANNA_YOMI_KAKUTEI		0x10L
#define CANNA_YOMI_HENKAN		0x20L
#define CANNA_YOMI_ZENKAKU		0x40L
#define CANNA_YOMI_HANKAKU		0x80L
#define CANNA_YOMI_HIRAGANA		0x100L
#define CANNA_YOMI_KATAKANA		0x200L
#define CANNA_YOMI_ROMAJI		0x400L
#define CANNA_YOMI_ATTRFUNCS		0x7f0L

#define CANNA_YOMI_BASE_HANKAKU		0x800L

#define CANNA_YOMI_IGNORE_USERSYMBOLS	0x1000L

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

typedef struct _henkanContextRec {
  char id;
  char majorMode, minorMode;
  struct callback c;
  KanjiMode	  prevMode;	/* �������Υ⡼�� */
  mode_context    next;

  /* ���ʴ����Ѵ��ط� */
  int            context;
  int            check;
  int		 kouhoCount;	/* ���� henkanNext ��Ϣ³���Ʋ����줿�� */
  wchar_t        yomi_buffer[ROMEBUFSIZE];
  wchar_t        echo_buffer[ROMEBUFSIZE];
  wchar_t        **allkouho; /* RkGetKanjiList��������ʸ���������ˤ���
				�ȤäƤ����Ȥ��� */
  int            yomilen;    /* �ɤߤ�Ĺ�����ɤ߼��Ȥ� kana_buffer ������
			        ���� */
  int            curbun;     /* ������ʸ�� */
  int		 curIkouho;  /* �����ȸ��� */
  int            nbunsetsu;  /* ʸ��ο� */
#define MAXNBUNSETSU	256
  int            kugiri[MAXNBUNSETSU]; /* ʸ��ʬ����Ԥ�����ʸ�᤯��
					  ��ξ��� */
  int		 *kanaKugiri, *romajiKugiri, *jishubun;
  yomiContext    ycx;
/* ifdef MEASURE_TIME */
  long		 proctime;   /* ��������(�Ѵ��Ƿ�¬����) */
  long		 rktime;     /* ��������(RK�ˤ��������) */
/* endif MEASURE_TIME */
} henkanContextRec, *henkanContext;

typedef struct _jishuContextRec {
  char id;
  char majorMode, minorMode;
  struct callback c;
  KanjiMode	  prevMode;	/* ���Υ⡼�� */
  mode_context    next;

  unsigned char	 inhibition;
  wchar_t        jishu_buffer[ROMEBUFSIZE];
  int            jishu_len, jishu_kEndp, jishu_rEndp;
  unsigned char  kc, jishu_case;
  yomiContext    ycx;
  henkanContext    hcx;
} jishuContextRec, *jishuContext;

typedef struct _ichiranContextRec {
  char id;
  char majorMode, minorMode;
  struct callback c;
  KanjiMode	  prevMode;	/* �������Υ⡼�� */
  mode_context    next;

  int            svIkouho;   /* �����ȸ�������ȤäƤ���(����ɽ����) */
  int            *curIkouho; /* �����ȸ��� */
  int            nIkouho;    /* ����ο�(����ɽ����) */
  int		 tooSmall;   /* �������������Ƹ���������Ф��ʤ���ե饰 */
  int            curIchar;   /* ̤����ʸ���󤢤��ñ����Ͽ��ñ�����Ϥ�
    							��Ƭʸ���ΰ��� */
  unsigned char  inhibit;
  unsigned char  flags;	     /* ���򸫤Ƥ� */
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


typedef struct _foirchiranContextRec {
  char id;
  char majorMode, minorMode;
  struct callback c;
  KanjiMode	  prevMode;	/* �������Υ⡼�� */
  mode_context    next;

  int            curIkouho;  /* �����ȸ��� */
  wchar_t        **allkouho; /* RkGetKanjiList��������ʸ���������ˤ���
				�ȤäƤ����Ȥ��� */
  menuitem       *table;  /* ʸ����ȴؿ��Υơ��֥� */
  int            *prevcurp;  /* ���Υ����ȸ��� */
  int            (*prevfunc)();  /* ���δؿ� */
  int            (*curfunc)();  /* �����ȴؿ� */
} forichiranContextRec, *forichiranContext;

typedef struct _mountContextRec {
  char id;
  char majorMode, minorMode;
  struct callback c;
  KanjiMode	  prevMode;	/* �������Υ⡼�� */
  mode_context    next;

  unsigned char        *mountOldStatus; /* �ޥ���Ȥ���Ƥ��뤫���ʤ��� */
  unsigned char        *mountNewStatus; /* �ޥ���Ȥ���Ƥ��뤫���ʤ��� */
  unsigned char        **mountList;   /* �ޥ���Ȳ�ǽ�ʼ���ΰ��� */
  int            curIkouho;     /* �����ȸ��� */
} mountContextRec, *mountContext;

typedef struct _tourokuContextRec {
  char id;
  char majorMode, minorMode;
  struct callback c;
  KanjiMode	  prevMode;	/* �������Υ⡼�� */
  mode_context    next;

  wchar_t        genbuf[ROMEBUFSIZE];
  wchar_t        qbuf[ROMEBUFSIZE];
  wchar_t        tango_buffer[ROMEBUFSIZE];
  int            tango_len;  /* ñ����Ͽ��ñ���ʸ�����Ĺ�� */
  wchar_t        yomi_buffer[ROMEBUFSIZE];
  int            yomi_len;   /* ñ����Ͽ���ɤߤ�ʸ�����Ĺ�� */
  int            curHinshi;  /* �ʻ������ */
  wchar_t	 hcode[16];   /* ñ����Ͽ���ʻ� */
  int		 katsuyou;   /* ñ����Ͽ��ư��γ��ѷ� */
  wchar_t        **udic;     /* ñ����Ͽ�Ǥ��뼭�� (����̾) */
  int            delContext; /* ñ�����ǣ��Ĥμ����ޥ���Ȥ��� */
} tourokuContextRec, *tourokuContext;

typedef struct _adjustContextRec {
  char id;
  char majorMode, minorMode;
  struct callback c;
  KanjiMode	  prevMode;	/* �������Υ⡼�� */
  mode_context    next;

  yomiContext    ycx;
  henkanContext  hcx;

  wchar_t        genbuf[ROMEBUFSIZE];
  int            kanjilen, strlen, bunlen; /* ������ʬ�����Ρ�ʸ���Ĺ�� */
} adjustContextRec, *adjustContext;

typedef struct _chikujiContextRec {
  char id;
  char majorMode, minorMode;
  struct callback c;
  KanjiMode	  prevMode;
  mode_context    next;

  yomiContext    yc;
  henkanContext  hc;
  wchar_t        echo_buffer[ROMEBUFSIZE];
  int		 ye, ys;
  int		 status;
} chikujiContextRec, *chikujiContext;

#define	CHIKUJI_ON_BUNSETSU		0x0001
#define	CHIKUJI_YOMI_CHANGED		0x0002
#define	CHIKUJI_RENBUNSETSU		0x0004
#define	CHIKUJI_DOING_HENKAN		0x0008
#define	CHIKUJI_OVERWRAP		0x0010
#define	CHIKUJI_HENKAN_INHIBIT		0x0020
#define CHIKUJI_HENKAN_FAILED		0x0040
#define	CHIKUJI_NULL_STATUS		0x0000

struct moreTodo {
  char          todo; /* ��äȤ���Ρ��򼨤� */
  char          fnum; /* �ؿ��ֹ档���ʤ鼡��ʸ���Ǽ�����뤳�Ȥ򤹤� */
  unsigned char ch;   /* ʸ�� */
};

/* �⡼��̾���Ǽ����ǡ����η���� */

struct ModeNameRecs {
  int           alloc;
  wchar_t       *name;
};


/*
 * ���޻������Ѵ��ơ��֥�ϣ��Ĥ�����ɤ��Ǥ��礦��ʣ����ɬ�פʤΤ�
 * ����� RomeStruct �Υ��ФȤ�������Ƥ���ɬ�פ⤢��ޤ��礦��...��
 * �λ��Ϥ��λ��ǹͤ��ޤ��礦��
 */
     
extern struct RkwRxDic *romajidic, *RkwOpenRoma();

/*
 * �����̾��������Ƥ����ѿ�
 */

#define MAX_DICS 16

extern char *kanjidicname[];
extern int  nkanjidics;

extern char *userdicname[];
extern int  nuserdics;
extern char userdicstatus[];

extern char *bushudicname[];
extern int nbushudics;

extern char *localdicname[];
extern int nlocaldics;

/*
 * ���顼�Υ�å�����������Ƥ����ѿ�
 */

extern char *necKanjiError;

/*
 * �ǥХ�ʸ��ɽ�����뤫�ɤ����Υե饰
 */

extern iroha_debug;

/*
 * 16�ʥ��������Ϥ�����Ԥ�ɽ�����뤫�ɤ�����Ĵ�٤��
 */

#define hexGLine(plen) (d->ncolumns /2 >= plen + 4)

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
extern int fail_malloc;
#define malloc(n) debug_malloc(n)
#endif /* DEBUG_MALLOC */

#ifndef	_UTIL_FUNCTIONS_DEF_

#define	_UTIL_FUNCTIONS_DEF_

extern makeGLineMessage();
extern makeGLineMessageFromStrings();
extern setWStrings();
extern WStrlen();
extern wchar_t *WStrcat();
extern wchar_t *WStrcpy();
extern wchar_t *WStrncpy();
extern WStrncmp();
extern MBstowcs();
extern wchar_t *WString();

#endif	/* _UTIL_FUNCTIONS_DEF_ */

/*
 * �������⡼�ɤ�������빽¤��
 */

typedef struct {
  wchar_t         *display_name; /* �⡼��ɽ��̾ */
  unsigned char   *romaji_table; /* ���޻������Ѵ��ơ��֥�(EUC) */
  struct RkwRxDic *romdic;	 /* ���޻�����¤�� */
  long            romdic_owner;  /* ���޻������ʬ��Open������ */
  long            flags;	 /* flags for yomiContext->generalFlags */
  KanjiMode       emode;	 /* current_mode �����빽¤�� */
} newmode;

#define MAX_OTHER_MODES 16

/* ���޻������Ѵ�����­���륭����ʸ�����Ѵ��ơ��֥� */

typedef struct {
  wchar_t	key;		/* ���� */
  int		groupid;	/* ���롼��id */
  int           ncand;          /* ����ο� */
  wchar_t       **cand;         /* ��������� */
  wchar_t	*fullword;	/* ������ (����1@����2@...����n@@) */
} keySupplement;

#define MAX_KEY_SUP 64

#ifndef	DEBUG_CHIKUJI
#define	debugging(a)
#define debuggingf(a, b)
#define debuggingw(a, b, c)
#endif	/* DEBUG_CHIKUJI */

#define HEX_USUAL     0
#define HEX_IMMEDIATE 1

#define CANNA_HALF_VERBOSE 1

#endif /* !_CANNA_H_ */
