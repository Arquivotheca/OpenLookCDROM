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
static char rcs_id[] = "@(#) 102.1 $Id: bushu.c,v 5.15 1994/03/10 02:20:13 kon Exp $";
#endif /* lint */

#include	<errno.h>
#include "canna.h"

extern int  errno;

extern wchar_t *WString();
extern HexkeySelect;

extern int makeGlineStatus(), uuslQuitCatch();
extern int uiUtilIchiranTooSmall(), uuslIchiranQuitCatch();
static int bushuHenkan(), makeBushuIchiranQuit();
static int vBushuExitCatch(), bushuQuitCatch();


#define	BUSHU_SZ	150

static
char *bushu_schar[] = { "��", "Ш", "��", "��", "��", "��",
			 
			 "���ʤ�äȤ���", "��", "��", "ұ", "��ҹ��", "е",

			 "��", "�͡��Ρʤˤ�٤��", "��", "��", "Ȭ", "ѹ",
			 
			 "��", "��", "׮", "��ʤ�������)", "��", "��",

			 "��", "��", "��ʤ���������)", "�ȡʤ���Τؤ��",

			 "��", "���ʤ����ȡ�", "��", "���ʤ��󤺤���", "׵",

			 "��", "����ñ�ʤġ�", "��ʤ���ˤ礦��", "��", "��",

			 "��", "��ʤƤؤ��", "��", "��", "��", "ͼ",

			 "��", "˻�ʤ�ä���٤��", "��", "��", "��",

			 "�����ʤ����ؤ��", "��", "��", "��", "��", "��",

			 "��", "��", "��", "��", "ھ", "��",

			 "��", "��", "���ʤ�ä���", "��", "��", "��",

			 "��", "��", "��", "��", "��", "���ʤ��᤹�ؤ��", "��",

			 "��", "Ω", "��", "��", "�", "��",

			 "�ˡʤ�ޤ������", "��", "��", "��", "��", "Ϸ",

			 "��", "��", "��ʤ����ؤ��", "��", "��", "��",

			 "�ݡʤ����������", "��", "�סʤȤ餫�����", "��",

			 "��", "��", "��", "��", "��", "��",

			 "��", "��", "­��ɥ", "�", "��",

			 "��", "��", "��", "��", "��", "��", "��", "ë",

			 "��", "��", "��", "Ʀ", "��", "�", "��", "��",

			 "��", "��", "�", "��", "��", "��", "��", "��",

			 "��", "��", "��", "��", "��", "��", "��", "��",

			 "�", "��", "��", "��", "Ļ", "��", "��", "ɡ",

			 "��", "����", "����¾"
			 };

static
char *bushu_skey[] =  { "����", "��", "�����Ф�", "���夦", "�դ�", "������",
			  "��äȤ�", "��", "����", "��", "���ޤ�", "�ʤ�", "��",
			  
			  "�Ҥ�", "��", "�Ĥ���", "�Ϥ�", "��", "��",
			  
			  "��", "����", "��������", "���Τ�", "�����", "���礦",
			  
			  "��", "����", "�����", "��", "������", "����餤",
			  
			  "��", "����", "���㤯", "��", "����", "����",
			  
			  "����", "��", "��", "�Ϥ�", "��", "���",
			  
			  "�椦", "���", "��ä���", "����", "������", "����",
			  
			  "����", "����", "��", "�����ޤ�", "��", "������",
			  
			  "����", "�Ĥ�", "�Ĥ�", "�ˤ�", "�Τ֤�", "��",
			  
			  "�ۤ�", "�ۤ�", "��ĤƤ�", "��ޤ�", "����", "����",
			  
			  "����", "����", "�����", "����", "���᤹", "��",
			  
			  "����", "��", "����", "�Τ�", "��", "�Ϥ�", "��",

			  "��ޤ�", "���", "����", "����", "����", "����",

			  "����", "����", "�����", "����", "����", "����",

			  "����", "��", "�Ȥ�", "�ˤ�", "�ˤ�", "�Ϥ�", "�ҤĤ�",

			  "�դ�", "�դ�", "�ߤ�", "�ष", "����", "����",

			  "���Τ�", "����", "����", "���餤", "�����", "����",

			  "����", "����", "����", "����", "�Ĥ�", "�Τ���",

			  "�Ф�", "�ޤ�", "��", "�स��", "����", "���餺",

			  "����", "���", "�դ�Ȥ�", "�ڡ���", "����", "����",

			  "����", "����", "����", "���礯", "�ʤᤷ", "���",

			  "����", "����", "����", "������", "�Ȥ�", "�ۤ�",

			  "����", "����", "�Ȥ�", "����", "����", "�Ϥ�",

			  "��", "������", "���Τ�"
			  };

#define	BUSHU_CNT	(sizeof(bushu_schar)/sizeof(char *))

static wchar_t *bushu_char[BUSHU_CNT];
static wchar_t *bushu_key[BUSHU_CNT];

int
initBushuTable()
{
  setWStrings(bushu_char, bushu_schar, BUSHU_CNT);
  setWStrings(bushu_key, bushu_skey, BUSHU_CNT);
}


/*
 * �������Υ������Ѥ�ʸ�������
 *
 * ������	RomeStruct
 * �����	���ｪλ�� 0
 */
static int
makeBushuEchoStr(d)
uiContext d;
{
  ichiranContext ic = (ichiranContext)d->modec;

  d->kanji_status_return->echoStr = ic->allkouho[*(ic->curIkouho)];
  d->kanji_status_return->length = 1;
  d->kanji_status_return->revPos = 0;
  d->kanji_status_return->revLen = 1;

  return(0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * forichiranContext�Ѵؿ�                                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * forichiranContext �ν����
 */
static
clearForIchiranContext(p)
forichiranContext p;
{
  p->id = FORICHIRAN_CONTEXT;
  p->curIkouho = 0;
  p->allkouho = 0;

  return(0);
}
  
static forichiranContext
newForIchiranContext()
{
  forichiranContext fcxt;

  if((fcxt = 
      (forichiranContext)malloc(sizeof(forichiranContextRec))) == NULL) {
    jrKanjiError = "malloc (newForIchiranContext) �Ǥ��ޤ���Ǥ���";
    return(0);
  }
  clearForIchiranContext(fcxt);

  return fcxt;
}

getForIchiranContext(d)
uiContext d;
{
  forichiranContext fc;
  int retval = 0;

  if(pushCallback(d, d->modec, 0, 0, 0, 0) == 0) {
    jrKanjiError = "malloc (pushCallback) �Ǥ��ޤ���Ǥ���";
    return(NG);
  }
  
  if((fc = newForIchiranContext()) == NULL) {
    popCallback(d);
    return(NG);
  }
  fc->next = d->modec;
  d->modec = (mode_context)fc;

  fc->prevMode = d->current_mode;
  fc->majorMode = d->majorMode;

  return(retval);
}

void
popForIchiranMode(d)
uiContext d;
{
  forichiranContext fc = (forichiranContext)d->modec;

  d->modec = fc->next;
  d->current_mode = fc->prevMode;
  freeForIchiranContext(fc);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * ����⡼������                                                            *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static
vBushuMode(d, major_mode)
uiContext d;
int major_mode;
{
  forichiranContext fc;
  ichiranContext ic;
  unsigned char inhibit = 0;
  int retval = 0;

  d->status = 0;

  if((retval = getForIchiranContext(d)) == NG) {
    killmenu(d);
    return(GLineNGReturn(d));
  }

  fc = (forichiranContext)d->modec;

  /* selectOne ��Ƥ֤���ν��� */
  fc->allkouho = bushu_char;
  fc->curIkouho = 0;
  if( !HexkeySelect )
    inhibit |= ((unsigned char)NUMBERING | (unsigned char)CHARINSERT);
  else
    inhibit |= (unsigned char)CHARINSERT;

  if((retval = selectOne(d, fc->allkouho, &fc->curIkouho, BUSHU_SZ,
		 BANGOMAX, inhibit, 0, WITH_LIST_CALLBACK,
		 0, vBushuExitCatch,
		 bushuQuitCatch, uiUtilIchiranTooSmall)) == NG) {
    killmenu(d);
    return(GLineNGReturnFI(d));
  }

  ic = (ichiranContext)d->modec;
  ic->majorMode = major_mode;
  ic->minorMode = CANNA_MODE_BushuMode;
  currentModeInfo(d);

  *(ic->curIkouho) = d->curbushu;

  /* ��������Ԥ������Ƹ���������Ф��ʤ� */
  if(ic->tooSmall) {
    d->status = AUX_CALLBACK;
    killmenu(d);
    return(retval);
  }

  if ( !(ic->flags & ICHIRAN_ALLOW_CALLBACK) ) {
    makeGlineStatus(d);
  }
  /* d->status = ICHIRAN_EVERYTIME; */

  return(retval);
}

static
vBushuIchiranQuitCatch(d, retval, env)
     uiContext d;
     int retval;
     mode_context env;
     /* ARGSUSED */
{
  popCallback(d); /* ������ݥå� */

  if (((forichiranContext)env)->allkouho != (wchar_t **)bushu_char) {
    /* bushu_char �� static ����������� free ���ƤϤ����ʤ���
       ���������ΤäƤʤ󤫱����ʤ� */
    freeGetIchiranList(((forichiranContext)env)->allkouho);
  }
  popForIchiranMode(d);
  popCallback(d);

  return(vBushuMode(d, CANNA_MODE_BushuMode));
}

static
vBushuExitCatch(d, retval, env)
     uiContext d;
     int retval;
     mode_context env;
     /* ARGSUSED */
{
  forichiranContext fc;
  int cur, res;

  popCallback(d); /* ������ݥå� */

  fc = (forichiranContext)d->modec;
  cur = fc->curIkouho;

  popForIchiranMode(d);
  popCallback(d);

  res = bushuHenkan(d, 1, 1, cur, vBushuIchiranQuitCatch);
  if (res < 0) {
    makeYomiReturnStruct(d);
    return 0;
  }
  return res;
}

BushuMode(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  if (yc->generalFlags & CANNA_YOMI_CHGMODE_INHIBITTED) {
    killmenu(d);
    return NothingChangedWithBeep(d);
  }    

  return(vBushuMode(d, CANNA_MODE_BushuMode));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * ����⡼�����Ϥΰ���ɽ��                                                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static
bushuEveryTimeCatch(d, retval, env)
     uiContext d;
     int retval;
     mode_context env;
     /* ARGSUSED */
{
  makeBushuEchoStr(d);

  return(retval);
}

static
bushuExitCatch(d, retval, env)
uiContext d;
int retval;
mode_context env;
{
  yomiContext yc;

  popCallback(d); /* ������ݥå� */

  if (((forichiranContext)env)->allkouho != bushu_char) {
    /* bushu_char �� static ����������� free ���ƤϤ����ʤ���
       ���������ΤäƤʤ󤫱����ʤ� */
    freeGetIchiranList(((forichiranContext)env)->allkouho);
  }
  popForIchiranMode(d);
  popCallback(d);
  yc = (yomiContext)d->modec;
  if (yc->savedFlags & CANNA_YOMI_MODE_SAVED) {
    restoreFlags(yc);
  }
  retval = YomiExit(d, retval);
  killmenu(d);
  currentModeInfo(d);

  return retval;
}

static
bushuQuitCatch(d, retval, env)
     uiContext d;
     int retval;
     mode_context env;
     /* ARGSUSED */
{
  popCallback(d); /* ������ݥå� */

  if (((forichiranContext)env)->allkouho != (wchar_t **)bushu_char) {
    /* bushu_char �� static ����������� free ���ƤϤ����ʤ���
       ���������ΤäƤʤ󤫱����ʤ� */
    freeGetIchiranList(((forichiranContext)env)->allkouho);
  }
  popForIchiranMode(d);
  popCallback(d);
  currentModeInfo(d);
  GlineClear(d);

  return prevMenuIfExist(d);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * ����Ȥ��Ƥ��Ѵ��ΰ���ɽ��                                                *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static
convBushuQuitCatch(d, retval, env)
uiContext d;
int retval;
mode_context env;
{
  popCallback(d); /* ������ݥå� */

  if (((forichiranContext)env)->allkouho != (wchar_t **)bushu_char) {
    /* bushu_char �� static ����������� free ���ƤϤ����ʤ���
       ���������ΤäƤʤ󤫱����ʤ� */
    freeGetIchiranList(((forichiranContext)env)->allkouho);
  }
  popForIchiranMode(d);
  popCallback(d);

  makeYomiReturnStruct(d);
  currentModeInfo(d);

  return(retval);
}

/*
 * �ɤߤ�����Ȥ����Ѵ�����
 *
 * ������	uiContext
 * �����	���ｪλ�� 0	�۾ｪλ�� -1
 */
ConvertAsBushu(d)
uiContext	d;
{
  yomiContext yc = (yomiContext)d->modec;
  int res;

  d->status = 0; /* clear status */
  
  if (yc->henkanInhibition & CANNA_YOMI_INHIBIT_ASBUSHU ||
      yc->right || yc->left) {
    return NothingChangedWithBeep(d);
  }

  if (yc->generalFlags & CANNA_YOMI_CHIKUJI_MODE) {
    if (!(yc->status & CHIKUJI_OVERWRAP) && yc->nbunsetsu) {
      moveToChikujiTanMode(d);
      return TanKouhoIchiran(d);
    }
    else if (yc->nbunsetsu) {
      return NothingChanged(d);
    }
  }

  d->nbytes = yc->kEndp;
  WStrncpy(d->buffer_return, yc->kana_buffer, d->nbytes);

  /* 0 �ϡ�ConvertAsBushu ����ƤФ줿���Ȥ򼨤� */
  res = bushuHenkan(d, 0, 1, 0, convBushuQuitCatch);
  if (res < 0) {
    makeYomiReturnStruct(d);
    return 0;
  }
  return res;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * ������                                                                    *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * �ɤߤ����󼭽񤫤������Ѵ�����
 */
static
bushuBgnBun(st, yomi, length)
RkStat *st;
wchar_t *yomi;
int length;
{
  int nbunsetsu;
  extern defaultBushuContext;

  /* Ϣʸ���Ѵ��򳫻Ϥ��� *//* ����ˤ������Τ߼��Ф� */
  if ((defaultBushuContext == -1)) {
    if (KanjiInit() == -1 || defaultBushuContext == -1) {
      jrKanjiError = "���ʴ����Ѵ������Ф��̿��Ǥ��ޤ���";
      return(NG);
    }
  }

  if((nbunsetsu = RkwBgnBun(defaultBushuContext, yomi, length, 0)) == -1) {
    if(errno == EPIPE)
      jrKanjiPipeError();
    jrKanjiError = "���ʴ����Ѵ��˼��Ԥ��ޤ���";
    return(NG);
  }
  
  if(RkwGetStat(defaultBushuContext, st) == -1) {
    if(errno == EPIPE)
      jrKanjiPipeError();
    jrKanjiError = "���ƥ���������Ф��ޤ���Ǥ���";
    return(NG);
  }

  return(nbunsetsu);
}

/*
 * �ɤߤ�Ⱦ�������ղä��Ƹ�������Ԥ�ɽ������
 *
 * ������	uiContext
 *		flag	ConvertAsBushu����ƤФ줿 0
 *			BushuYomiHenkan����ƤФ줿 1
 * �����	���ｪλ�� 0	�۾ｪλ�� -1
 *
 *
 * �����������Ϥޤ� getForIchiranContext ���ƤФ�Ƥ��ʤ���ΤȤ���
 */

static
bushuHenkan(d, flag, ext, cur, quitfunc)
uiContext	d;
int             flag, cur;
int             (*quitfunc)();
{
  forichiranContext fc;
  ichiranContext ic;
  unsigned char inhibit = 0;
  wchar_t *yomi, **allBushuCands;
  RkStat	st;
  int nelem, currentkouho, nbunsetsu, length, retval = 0;
  extern defaultBushuContext;
  
  wchar_t **getIchiranList();

  if(flag) {
    yomi = (wchar_t *)bushu_key[cur];
    length = WStrlen(yomi);
    d->curbushu = (short)cur;
  } else {
    d->nbytes = RomajiFlushYomi(d, d->buffer_return, d->n_buffer);
    yomi = d->buffer_return;
    length = d->nbytes;
  }

  if((nbunsetsu = bushuBgnBun(&st, yomi, length)) == NG) {
    killmenu(d);
    (void)GLineNGReturn(d);
    return -1;
  }

  if((nbunsetsu != 1) || (st.klen > 1) || (st.maxcand == 0)) {
    /* ����Ȥ��Ƥθ��䤬�ʤ� */

    d->kanji_status_return->length = -1;

    makeBushuIchiranQuit(d, flag);
    currentModeInfo(d);

    killmenu(d);
    if(flag) {
      makeGLineMessageFromString(d, "��������θ���Ϥ���ޤ���");
    } else {
      return(NothingChangedWithBeep(d));
    }
    return(0);
  }

  /* ��������Ԥ�ɽ������ */
  /* 0 �ϡ������ȸ��� + 0 �򥫥��ȸ���ˤ��뤳�Ȥ򼨤� */

  if((allBushuCands
      = getIchiranList(defaultBushuContext, &nelem, &currentkouho)) == 0) {
    killmenu(d);
    (void)GLineNGReturn(d);
    return -1;
  }

  /* �����Ѵ��ϳؽ����ʤ��� */
  if(RkwEndBun(defaultBushuContext, 0) == -1) { /* 0:�ؽ����ʤ� */
    if(errno == EPIPE)
      jrKanjiPipeError();
    jrKanjiError = "���ʴ����Ѵ��ν�λ�˼��Ԥ��ޤ���";
    freeGetIchiranList(allBushuCands);
    killmenu(d);
    (void)GLineNGReturn(d);
    return -1;
  }

  if(getForIchiranContext(d) == NG) {
    freeGetIchiranList(allBushuCands);
    killmenu(d);
    (void)GLineNGReturn(d);
    return -1;
  }

  fc = (forichiranContext)d->modec;
  fc->allkouho = allBushuCands;

  if( !HexkeySelect )
    inhibit |= (unsigned char)NUMBERING;
  fc->curIkouho = currentkouho;	/* ���ߤΥ����ȸ����ֹ����¸���� */
  currentkouho = 0;	/* �����ȸ��䤫�鲿���ܤ򥫥��ȸ���Ȥ��뤫 */

  if((retval = selectOne(d, fc->allkouho, &fc->curIkouho, nelem, BANGOMAX,
			 inhibit, currentkouho, WITH_LIST_CALLBACK,
			 bushuEveryTimeCatch, bushuExitCatch,
			 quitfunc, uiUtilIchiranTooSmall)) == NG) {
    freeGetIchiranList(allBushuCands);
    killmenu(d);
    (void)GLineNGReturnFI(d);
    return -1;
  }

  ic = (ichiranContext)d->modec;

  if(!flag) { /* convertAsBushu */
    ic->majorMode = ic->minorMode = CANNA_MODE_BushuMode;
  } else {
    if(ext) {
      ic->majorMode = ic->minorMode = CANNA_MODE_BushuMode;
    } else {
      ic->majorMode = CANNA_MODE_ExtendMode;
      ic->minorMode = CANNA_MODE_BushuMode;
    }
  }
  currentModeInfo(d);

  /* ��������Ԥ������Ƹ���������Ф��ʤ� */
  if(ic->tooSmall) {
    d->status = AUX_CALLBACK;
    killmenu(d);
    return(retval);
  }

  if ( !(ic->flags & ICHIRAN_ALLOW_CALLBACK) ) {
    makeGlineStatus(d);
  }
  /* d->status = EVERYTIME_CALLBACK; */

  return(retval);
}

/*
 * ����Ԥ�õ������⡼�ɤ���ȴ�����ɤߤ��ʤ��⡼�ɤ˰ܹԤ���
 *
 * ������	uiContext
 *		flag	ConvertAsBushu����ƤФ줿 0
 *			BushuYomiHenkan����ƤФ줿 1
 * �����	���ｪλ�� 0	�۾ｪλ�� -1
 */
static
makeBushuIchiranQuit(d, flag)
uiContext	d;
int              flag;
{
  extern defaultBushuContext;

  /* �����Ѵ��ϳؽ����ʤ��� */
  if(RkwEndBun(defaultBushuContext, 0) == -1) { /* 0:�ؽ����ʤ� */
    if(errno == EPIPE)
      jrKanjiPipeError();
    jrKanjiError = "���ʴ����Ѵ��ν�λ�˼��Ԥ��ޤ���";
    return(NG);
  }

  if(flag) {
    /* kanji_status_return �򥯥ꥢ���� */
    d->kanji_status_return->length  = 0;
    d->kanji_status_return->revLen  = 0;
    
/*
    d->status = QUIT_CALLBACK;
*/
  } else {
    makeYomiReturnStruct(d);
  }
  GlineClear(d);
  
  return(0);
}

