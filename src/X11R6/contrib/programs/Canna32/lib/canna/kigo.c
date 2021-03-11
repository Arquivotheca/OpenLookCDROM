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

#if !defined(lint) && !defined(__CODECENTER__)
static	char	rcs_id[] = "@(#) 102.1 $Id: kigo.c,v 5.11 1994/03/04 12:13:09 kon Exp $";
#endif /* lint */

#include	"canna.h"

extern SelectDirect;

#define BYTE1		84	/* JIS������ɽ�����Х��Ȥο� */
#define BYTE2		94	/* JIS������ɽ������Х��Ȥο� */
#define KIGOSU		(((BYTE1 - 1) * BYTE2) + 4)
    				/* �������� */

#define KIGOSIZE	1	/* ��������ʸ���� */
#define KIGOCOLS	2	/* �������Υ����� */
#define KIGOSPACE	2	/* ����δ֤ζ���ʸ���Υ����� */
#define KIGOWIDTH	(KIGOCOLS + KIGOSPACE)
					/* bangomax��׻����뤿��ο� */

#define NKAKKOCHARS	1	/* JIS������ɽ���ѳ�̤�ʸ���� */
#define KAKKOCOLS       2       /* Ʊ������ */
#define NKCODECHARS	4	/* JIS������ɽ�����Τ�Τ�ʸ���� */
#define KCODECOLS       4       /* Ʊ������ */
/* JIS������ɽ�����Τ�ʸ���� */
#define NKCODEALLCHARS	(NKAKKOCHARS + NKAKKOCHARS + NKCODECHARS)
/* Ʊ������ */
#define KCODEALLCOLS    (KAKKOCOLS + KAKKOCOLS + KCODECOLS)

static int kigo_curIkouho;

int
initKigoTable()
{
}

/* cfunc ichiranContext
 *
 * ichiranContext
 *
 */
static
clearKigoContext(p)
ichiranContext p;
{
  p->id = ICHIRAN_CONTEXT;
  p->svIkouho = 0;
  p->curIkouho = 0;
  p->nIkouho = 0;
  p->tooSmall = 0;
  p->curIchar = 0;
  p->allkouho = 0;
  p->glinebufp = 0;
  p->kouhoifp = (kouhoinfo *)0;
  p->glineifp = (glineinfo *)0;
  p->flags = (unsigned char)0;
}

static ichiranContext
newKigoContext()
{
  ichiranContext kcxt;

  if((kcxt = (ichiranContext)malloc(sizeof(ichiranContextRec))) == NULL) {
    jrKanjiError = "malloc (newKigoContext) �Ǥ��ޤ���Ǥ���";
    return(0);
  }
  clearKigoContext(kcxt);

  return kcxt;
}


#ifdef	SOMEONE_USES_THIS
static void
freeKigoContext(kc)
ichiranContext kc;
{
  free(kc);
}
#endif	/* SOMEONE_USES_THIS */

/*
 * ��������Ԥ���
 */
static
getKigoContext(d,
	  everyTimeCallback, exitCallback, quitCallback, auxCallback)
uiContext d;
int (*everyTimeCallback)(), (*exitCallback)();
int (*quitCallback)(), (*auxCallback)();
{
  extern KanjiModeRec kigo_mode;
  ichiranContext kc;
  int retval = 0;

  if(pushCallback(d, d->modec,
	everyTimeCallback, exitCallback, quitCallback, auxCallback) == 0) {
    jrKanjiError = "malloc (pushCallback) �Ǥ��ޤ���Ǥ���";
    return(NG);
  }
  
  if((kc = newKigoContext()) == NULL) {
    popCallback(d);
    return(NG);
  }
  kc->next = d->modec;
  d->modec = (mode_context)kc;

  kc->prevMode = d->current_mode;
  d->current_mode = &kigo_mode;

  return(retval);
}

static void
popKigoMode(d)
uiContext d;
{
  ichiranContext kc = (ichiranContext)d->modec;

  d->modec = kc->next;
  d->current_mode = kc->prevMode;
  freeIchiranContext(kc);
}

/*
 * ��������Ԥ˴ؤ��빽¤�Τ����Ƥ򹹿�����
 *
 * �������ȸ���ˤ�ä� kouhoinfo �� glineinfo �����������Ԥ���
 * �������ȸ���Υ����ɤ򥭥�饯�����Ѵ�����
 *
 * ������	RomeStruct
 * �����	�ʤ�
 */
static
makeKigoGlineStatus(d)
uiContext	d;
{
  ichiranContext kc = (ichiranContext)d->modec;
  wchar_t *gptr;
  char xxx[3];
  char *yyy = xxx;
  int  i, b1, b2;

  gptr = kc->glineifp->gldata + NKAKKOCHARS;
  
  /* �����ȵ����JIS�����ɤ�����Ԥ���Υ��å��������� */
  WCstombs(xxx, kc->kouhoifp[*(kc->curIkouho)].khdata, 3);

  for(i=0; i<2; i++, yyy++) {
    b1 = (((unsigned long)*yyy & 0x7f) >> 4);
    b2 = (*yyy & 0x0f);
    *gptr++ = b1 + ((b1 > 0x09) ? ('a' - 10) : '0');
    *gptr++ = b2 + ((b2 > 0x09) ? ('a' - 10) : '0');
  }

  d->kanji_status_return->info |= KanjiGLineInfo;
  d->kanji_status_return->gline.line = kc->glineifp->gldata;
  d->kanji_status_return->gline.length = kc->glineifp->gllen;
  d->kanji_status_return->gline.revPos =
    kc->kouhoifp[*(kc->curIkouho)].khpoint;
  d->kanji_status_return->gline.revLen = KIGOSIZE;

}

/* ��������Ѥ�glineinfo��kouhoinfo����
 *
 * ��glineinfo��
 *    int glkosu   : int glhead     : int gllen  : wchar_t *gldata
 *    ���Ԥθ���� : ��Ƭ���椬     : ���Ԥ�Ĺ�� : ��������Ԥ�ʸ����
 *                 : �����ܤε��椫 :
 * -------------------------------------------------------------------------
 * 0 | 6           : 0              : 24         : ������������������������
 *
 *  ��kouhoinfo��
 *    int khretsu  : int khpoint  : wchar_t *khdata
 *    ̤����       : �Ԥ���Ƭ���� : �����ʸ��
 *                 : ���Х����ܤ� :
 * -------------------------------------------------------------------------
 * 0 | 0           : 0            : ��
 * 1 | 0           : 4            : ��
 * 2 | 0           : 8            : ��
 *          :               :
 *
 * ������	headkouho	�����ȵ������Ԥ���Ƭ����ΰ���
 *					(2121���鲿���ܤ�(2121�ϣ�����))
 *		uiContext
 * �����	���ｪλ�� 0
 */
static
makeKigoInfo(d, headkouho)
uiContext	d;
int		headkouho;
{
  ichiranContext kc = (ichiranContext)d->modec;
  wchar_t *gptr;
  int  i, b1, b2, lnko, cn;
  int  byte1hex = 0xa1;
  int  byte2hex = 0xa1;
  char xxx[3];

  b2 = headkouho % BYTE2;	/* JIS������ɽ��(�ؼ�)�ΰ��� (��-1) */
  b1 = headkouho / BYTE2;	/* JIS������ɽ��(�ټ�)�ΰ��� (��-1) */

  xxx[2] = '\0';

#ifdef DEBUG
  if (iroha_debug) {
    printf("kigoinfo = bangomax %d, b1 %d, b2 %d\n", kc->nIkouho, b1, b2);
    printf("kigoinfo = headkouho %d, curIkouho %d\n",
	   headkouho, *(kc->curIkouho));
  }
#endif

  /* ��������Ѥ�glineinfo��kouhoinfo���� */
  gptr = kc->glinebufp;

  kc->glineifp->glhead = headkouho;
  kc->glineifp->gldata = gptr;

  /* JIS�����ɤ�ɽ���ΰ���������˺�� */
  MBstowcs(gptr, "��", 1);
  for(i=0, gptr++; i<NKCODECHARS; i++)
    *gptr++ = ' ';
  MBstowcs(gptr++, "��", 1);

  for(cn=NKCODEALLCHARS, lnko=0;
      b1<BYTE1 && lnko<kc->nIkouho && (headkouho+lnko)<KIGOSU ; b1++) {
    for(; b2<BYTE2 && lnko<kc->nIkouho && (headkouho+lnko)<KIGOSU; b2++, lnko++) {
      
      /* ���ڤ�ˤʤ����򥳥ԡ����� */
      if(lnko != 0) {
	MBstowcs(gptr++, "��", 1); 
	cn ++;
      }

      kc->kouhoifp[lnko].khpoint = cn;
      kc->kouhoifp[lnko].khdata = gptr;
      
      /* ����򥳥ԡ����� */
      *xxx = (char)byte1hex + b1;
      *(xxx + 1) = (char)byte2hex + b2;
      MBstowcs(gptr++, xxx, 1);
      cn ++;
    }
    b2 = 0;
  }
  *gptr = (wchar_t)0;
  kc->glineifp->glkosu = lnko;
  kc->glineifp->gllen = WStrlen(kc->glineifp->gldata);

  return(0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * �������                                                                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static
kigoIchiranExitCatch(d, retval, env)
     uiContext d;
     int retval;
     mode_context env;
     /* ARGSUSED */
{
  popCallback(d);
  retval = YomiExit(d, retval);
  currentModeInfo(d);

  killmenu(d);

  return(retval);
}

static
kigoIchiranQuitCatch(d, retval, env)
     uiContext d;
     int retval;
     mode_context env;
     /* ARGSUSED */
{
  popCallback(d);
  currentModeInfo(d);

  return prevMenuIfExist(d);
}

KigoIchiran(d)
uiContext	d;
{
  yomiContext yc = (yomiContext)d->modec;

  if (yc->generalFlags & CANNA_YOMI_CHGMODE_INHIBITTED) {
    return NothingChangedWithBeep(d);
  }    
  
  if(makeKigoIchiran(d, CANNA_MODE_KigoMode) == NG)
    return(GLineNGReturn(d));
  else
    return(0);
}

/*
 * ��������Ԥ�ɽ������
 *
 * ������	uiContext
 * �����	���ｪλ�� 0	�۾ｪλ�� -1
 */
makeKigoIchiran(d, major_mode)
uiContext	d;
int             major_mode;
{
  ichiranContext kc;
  int            headkouho;
  extern int quickly_escape;

  if(d->ncolumns < (KCODEALLCOLS + KIGOCOLS)) {
    NothingChangedWithBeep(d);
    jrKanjiError = "��������Ѥ����������Τǵ�������Ǥ��ޤ���";
    return(NG);
  }

  if(getKigoContext(d, 0, kigoIchiranExitCatch, kigoIchiranQuitCatch, 0) == NG)
    return(NG);

  kc = (ichiranContext)d->modec;
  kc->majorMode = major_mode;
  kc->minorMode = CANNA_MODE_KigoMode;
  kc->flags |= quickly_escape ? 0 : ICHIRAN_STAY_LONG;

  currentModeInfo(d);

  /* ���絭��ɽ�����Υ��å� */
  /* ���������� "��JIS ��" ʬ�򺹤������Ʒ׻����� */
  if((kc->nIkouho =
      (((d->ncolumns - KCODEALLCOLS - KIGOCOLS) / KIGOWIDTH) + 1))
                                                  > KIGOBANGOMAX) {
    kc->nIkouho = KIGOBANGOMAX;
  }

  kc->curIkouho = &kigo_curIkouho;

  if(allocIchiranBuf(d) == NG) { /* ��������⡼�� */
    popKigoMode(d);
    popCallback(d);
    return(NG);
  }

  /* �����ȸ���Τ��뵭������Ԥ���Ƭ����ȡ�
     ��������Υ����ȸ���ΰ��֤���� */
  if(d->curkigo) {		/* a1a1���鲿���ܤε��椫 */
    headkouho = (d->curkigo / kc->nIkouho) * kc->nIkouho;
    *(kc->curIkouho) = d->curkigo % kc->nIkouho;
  } else {
    d->curkigo = 0;
    headkouho = 0;
    *(kc->curIkouho) = 0;
  }

  /* glineinfo��kouhoinfo���� */
  makeKigoInfo(d, headkouho);

  /* kanji_status_return���� */
  makeKigoGlineStatus(d);

  return(0);
}

static
KigoNop(d)
uiContext	d;
{
  /* currentModeInfo �ǥ⡼�ɾ���ɬ���֤�褦�˥��ߡ��Υ⡼�ɤ�����Ƥ��� */
  d->majorMode = d->minorMode = CANNA_MODE_AlphaMode;
  currentModeInfo(d);

  makeKigoGlineStatus(d);
  return 0;
}

/*
 * �����������μ��ε���˰�ư����
 *
 * ������	uiContext
 * �����	���ｪλ�� 0
 */
static
KigoForwardKouho(d)
uiContext	d;
{
  ichiranContext kc = (ichiranContext)d->modec;
  int  headkouho;

  /* ���ε���ˤ��� */
  ++*(kc->curIkouho);
  
  /* ����ɽ���κǸ�ε�����ä��顢���ΰ����Ԥ���Ƭ����򥫥��ȵ���Ȥ��� */
  if((*(kc->curIkouho) >= kc->nIkouho) ||
     (kc->glineifp->glhead + *(kc->curIkouho) >= KIGOSU)) {
    headkouho  = kc->glineifp->glhead + kc->nIkouho;
    if(headkouho >= KIGOSU)
      headkouho = 0;
    *(kc->curIkouho) = 0;
    makeKigoInfo(d, headkouho);
  }

  /* kanji_status_retusrn ���� */
  makeKigoGlineStatus(d);
  /* d->status = EVERYTIME_CALLBACK; */

  return(0);
}

/*
 * ���������������ε���˰�ư����
 *
 * ������	uiContext
 * �����	���ｪλ�� 0
 */
static
KigoBackwardKouho(d)
uiContext	d;
{
  ichiranContext kc = (ichiranContext)d->modec;
  int  headkouho;

  /* ���ε���ˤ��� */
  --*(kc->curIkouho);

  /* ����ɽ������Ƭ�ε�����ä��顢���ΰ����Ԥκǽ�����򥫥��ȵ���Ȥ��� */
  if(*(kc->curIkouho) < 0) {
    headkouho  = kc->glineifp->glhead - kc->nIkouho;
    if(headkouho < 0)
      headkouho = ((KIGOSU - 1) / kc->nIkouho) * kc->nIkouho;
    makeKigoInfo(d, headkouho);
    *(kc->curIkouho) = kc->glineifp->glkosu - 1;
  }

  /* kanji_status_retusrn ���� */
  makeKigoGlineStatus(d);
  /* d->status = EVERYTIME_CALLBACK; */

  return(0);
}

/*
 * �����������˰�ư����
 *
 * ������	uiContext
 * �����	���ｪλ�� 0
 */
static
KigoPreviousKouhoretsu(d)
uiContext	d;
{
  ichiranContext kc = (ichiranContext)d->modec;
  int headkouho;

  /** ��������ˤ��� **/
  headkouho  = kc->glineifp->glhead - kc->nIkouho;
  if(headkouho < 0)
    headkouho = ((KIGOSU -1) / kc->nIkouho) * kc->nIkouho;
  makeKigoInfo(d, headkouho);

  /* *(kc->curIkouho) �������ȵ�������ε��������礭���ʤäƤ��ޤä���
     �Ǳ�����򥫥��ȸ���Ȥ��� */
  if(*(kc->curIkouho) >= kc->glineifp->glkosu)
    *(kc->curIkouho) = kc->glineifp->glkosu - 1;

  /* kanji_status_retusrn ���� */
  makeKigoGlineStatus(d);
  /* d->status = EVERYTIME_CALLBACK; */

  return(0);
}

/*
 * �����������˰�ư����
 *
 * ������	uiContext
 * �����	���ｪλ�� 0
 */
static
KigoNextKouhoretsu(d)
uiContext	d;
{
  ichiranContext kc = (ichiranContext)d->modec;
  int headkouho;

  /** ��������ˤ��� **/
  headkouho = kc->glineifp->glhead + kc->nIkouho;
  if(headkouho >= KIGOSU)
    headkouho = 0;
  makeKigoInfo(d, headkouho);

  /* *(kc->curIkouho) �������ȵ�������ε��������礭���ʤäƤ��ޤä���
     �Ǳ�����򥫥��ȸ���Ȥ��� */
  if(*(kc->curIkouho) >= kc->glineifp->glkosu)
    *(kc->curIkouho) = kc->glineifp->glkosu - 1;

  /* kanji_status_retusrn ���� */
  makeKigoGlineStatus(d);
  /* d->status = EVERYTIME_CALLBACK; */

  return(0);
}

/*
 * ��������������Ƭ�ε���˰�ư����
 *
 * ������	uiContext
 * �����	���ｪλ�� 0
 */
static
KigoBeginningOfKouho(d)
uiContext	d;
{
  ichiranContext kc = (ichiranContext)d->modec;

  /* ���������Ƭ����򥫥��ȸ���ˤ��� */
  *(kc->curIkouho) = 0;

  /* kanji_status_retusrn ���� */
  makeKigoGlineStatus(d);
  /* d->status = EVERYTIME_CALLBACK; */

  return(0);
}

/*
 * �����������κǱ��ε���˰�ư����
 *
 * ������	uiContext
 * �����	���ｪλ�� 0
 */
static
KigoEndOfKouho(d)
uiContext	d;
{
  ichiranContext kc = (ichiranContext)d->modec;

  /** ������κǱ�����򥫥��ȸ���ˤ��� **/
  *(kc->curIkouho) = kc->glineifp->glkosu - 1;

  /* kanji_status_retusrn ���� */
  makeKigoGlineStatus(d);
  /* d->status = EVERYTIME_CALLBACK; */

  return(0);
}

/*
 * ����������椫�����򤵤줿�������ꤹ��
 *
 * �����˵��������������������ꤷ�����椬�����ȸ���Ȥʤ�褦�ˡ�
 *   ���ꤷ������򥻡��֤��Ƥ���
 *
 * ������	uiContext
 * �����	���ｪλ�� 0
 */
static
KigoKakutei(d)
uiContext	d;
{
  ichiranContext kc = (ichiranContext)d->modec;

  /* �����ȵ���򥻡��֤��� */
  d->curkigo = kc->glineifp->glhead + *(kc->curIkouho);

  /* ���������ȥ�󥰤����ʸ����Ȥ��� */
  if (d->n_buffer >= KIGOSIZE) {
    d->nbytes = KIGOSIZE;
    WStrncpy(d->buffer_return, kc->kouhoifp[*(kc->curIkouho)].khdata, 
	    d->nbytes);
    d->buffer_return[KIGOSIZE] = (wchar_t)0;
  }
  else {
    d->nbytes = 0;
  }

  if (kc->flags & ICHIRAN_STAY_LONG) {
    kc->flags |= ICHIRAN_NEXT_EXIT;
    d->status = EVERYTIME_CALLBACK;
  }
  else {
    freeIchiranBuf(kc);
    popKigoMode(d);
    GlineClear(d);

    d->status = EXIT_CALLBACK;
  }

  return(d->nbytes);
}

#ifdef	SOMEONE_USES_THIS
/*
 * ���������������Ϥ��줿�ֹ�ε���˰�ư����  ��̤���ѡ�
 *
 * ������	uiContext
 * �����	���ｪλ�� 0
 */
static
KigoBangoKouho(d)
uiContext	d;
{
  ichiranContext kc = (ichiranContext)d->modec;
  int num;

  /* ���ϥǡ����� ������ ����� ���� */
  if(((0x30 <= d->ch) && (d->ch <= 0x39))
     || ((0x61 <= d->ch) && (d->ch <= 0x66))) {
    if((0x30 <= d->ch) && (d->ch <= 0x39))
      num = (int)(d->ch & 0x0f);
    else if((0x61 <= d->ch) && (d->ch <= 0x66))
      num = (int)(d->ch - 0x57);
  } 
  else {
    /* ���Ϥ��줿�ֹ������������ޤ��� */
    return NothingChangedWithBeep(d);
  }
  /* ���ϥǡ����� ����Ԥ����¸�ߤ�������� */
  if(num >= kc->glineifp->glkosu) {
    /* ���Ϥ��줿�ֹ������������ޤ��� */
    return NothingChangedWithBeep(d);
  }

  /* ���������Ƭ��������� */
  *(kc->curIkouho) = num;

  /* SelectDirect �Υ������ޥ����ν��� */
  if(SelectDirect) /* ON */ {

    return(KigoKakutei(d));
  } else           /* OFF */ {
    /* kanji_status_retusrn ���� */
    makeKigoGlineStatus(d);

    return(0);
  }
}
#endif	/* SOMEONE_USES_THIS */

/*
 * ��������Ԥ�õ��
 *
 * ������	uiContext
 * �����	���ｪλ�� 0
 */
static
KigoQuit(d)
uiContext	d;
{
  ichiranContext kc = (ichiranContext)d->modec;
  BYTE fl = kc->flags;

  freeIchiranBuf(kc);
  popKigoMode(d);
  /* gline �򥯥ꥢ���� */
  GlineClear(d);
  d->status = (fl & ICHIRAN_NEXT_EXIT) ? EXIT_CALLBACK : QUIT_CALLBACK;
  return 0;
}

#include	"kigomap.c"
