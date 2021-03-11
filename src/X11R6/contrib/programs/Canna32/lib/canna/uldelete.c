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
static char rcs_id[] = "@(#) 102.1 $Id: uldelete.c,v 7.17 1994/03/01 14:14:05 kon Exp $";
#endif

#include	<errno.h>
#include 	"canna.h"

extern int errno;

extern HexkeySelect;

extern uiUtilIchiranTooSmall();
extern void freeAndPopTouroku();

static int dicSakujoYomi(), dicSakujoEndBun(), dicSakujoTango(), dicSakujoDo();

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * ñ�����μ������                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static
uuSDicExitCatch(d, retval, env)
uiContext d;
int retval;
mode_context env;
/* ARGSUSED */
{
  forichiranContext fc;
  int cur;
  tourokuContext tc;

  d->nbytes = 0;

  popCallback(d); /* ������ pop */

  fc = (forichiranContext)d->modec;
  cur = fc->curIkouho;

  popForIchiranMode(d);
  popCallback(d);

  tc = (tourokuContext)d->modec;
  tc->workDic = cur;  

  return(dicSakujoYomi(d));
}

static
uuSDicQuitCatch(d, retval, env)
uiContext d;
int retval;
mode_context env;
/* ARGSUSED */
{
  popCallback(d); /* ������ pop */

  popForIchiranMode(d);
  popCallback(d);

  freeAndPopTouroku(d);

  return prevMenuIfExist(d);
}

dicSakujo(d)
uiContext d;
{
  wchar_t **up, **getUserDicName();
  tourokuContext tc;

  d->status = 0;
  /* �桼������ǥޥ���Ȥ���Ƥ����Τ��äƤ��� */
  if((up = getUserDicName(d)) == 0) {
    deleteEnd(d);
    return(GLineNGReturn(d));
  }

  if (getTourokuContext(d) == NG) {
    if(up)
      free(up);
    deleteEnd(d);
    return(GLineNGReturn(d));
  }
  tc = (tourokuContext)d->modec;

  tc->udic = up;
  if(!*up) {
    makeGLineMessageFromString(d, "ñ����Ͽ�Ѽ��񤬻��ꤵ��Ƥ��ޤ���");
    
    freeAndPopTouroku(d);
    deleteEnd(d);
    currentModeInfo(d);
    return(0);
  }
  return(dicTourokuDictionary(d, uuSDicExitCatch, uuSDicQuitCatch));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * ñ�������ɤߤ�����                                                      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static
uuSYomiEveryTimeCatch(d, retval, env)
uiContext d;
int retval;
mode_context env;
/* ARGSUSED */
{
  int len, echoLen, revPos;
  wchar_t tmpbuf[ROMEBUFSIZE];

  retval = 0;
  if((echoLen = d->kanji_status_return->length) < 0)
    return(retval);

  if (echoLen == 0) {
    d->kanji_status_return->revPos = 0;
    d->kanji_status_return->revLen = 0;
  }

  /* ��ꤢ���� echoStr �� d->genbuf ���⤷��ʤ��Τ� copy ���Ƥ��� */
  WStrncpy(tmpbuf, d->kanji_status_return->echoStr, echoLen);

  revPos = MBstowcs(d->genbuf, "�ɤ�?[", ROMEBUFSIZE);
  WStrncpy(d->genbuf + revPos, tmpbuf, echoLen);
  *(d->genbuf + revPos + echoLen) = (wchar_t) ']';
  len = revPos + echoLen + 1;
  *(d->genbuf + len) = (wchar_t) '\0';
  d->kanji_status_return->gline.line = d->genbuf;
  d->kanji_status_return->gline.length = len;
  if (d->kanji_status_return->revLen) {
    d->kanji_status_return->gline.revPos =
      d->kanji_status_return->revPos + revPos;
    d->kanji_status_return->gline.revLen = d->kanji_status_return->revLen;
  }
  else { /* ȿž�ΰ褬�ʤ���� */
    d->kanji_status_return->gline.revPos = len - 1;
    d->kanji_status_return->gline.revLen = 1;
  }
  d->kanji_status_return->info &= ~(KanjiThroughInfo | KanjiEmptyInfo);
  d->kanji_status_return->info |= KanjiGLineInfo;
  echostrClear(d);
  checkGLineLen(d);

  return retval;
}

static
uuSYomiExitCatch(d, retval, env)
uiContext d;
int retval;
mode_context env;
/* ARGSUSED */
{
  tourokuContext tc;

  popCallback(d); /* �ɤߤ� pop */

  tc = (tourokuContext)d->modec;

  WStrncpy(tc->yomi_buffer, d->buffer_return, retval);
  tc->yomi_buffer[retval] = (wchar_t)'\0';
  tc->yomi_len = WStrlen(tc->yomi_buffer);

  return(dicSakujoTango(d));
}

static
uuSYomiQuitCatch(d, retval, env)
uiContext d;
int retval;
mode_context env;
/* ARGSUSED */
{
  popCallback(d); /* �ɤߤ� pop */

  clearYomi(d);

  freeAndPopTouroku(d);

  return(dicSakujo(d));
}

static
dicSakujoYomi(d)
uiContext d;
{
  yomiContext yc;

  d->status = 0;

  yc = GetKanjiString(d, 0, 0,
	      CANNA_NOTHING_RESTRICTED,
	      CANNA_YOMI_CHGMODE_INHIBITTED,
	      CANNA_YOMI_END_IF_KAKUTEI,
	      (CANNA_YOMI_INHIBIT_HENKAN | CANNA_YOMI_INHIBIT_ASHEX |
	      CANNA_YOMI_INHIBIT_ASBUSHU),
	      uuSYomiEveryTimeCatch, uuSYomiExitCatch,
	      uuSYomiQuitCatch);
  if (yc == (yomiContext)0) {
    deleteEnd(d);
    return NoMoreMemory();
  }
  yc->majorMode = CANNA_MODE_ExtendMode;
  yc->minorMode = CANNA_MODE_DeleteDicMode;
  currentModeInfo(d);

  return(0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * ñ������ñ�������                                                      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static
CloseDeleteContext(tc)
tourokuContext tc;
{
  if(tc->delContext >= 0) {
    if (RkwCloseContext(tc->delContext) < 0) {
      if (errno == EPIPE) {
	jrKanjiPipeError();
      }
    }
  }
#ifdef DEBUG
  else
    printf("ERROR: delContext < 0\n");
#endif
}

static
uuSTangoExitCatch(d, retval, env)
uiContext d;
int retval;
mode_context env;
/* ARGSUSED */
{
  forichiranContext fc;
  tourokuContext tc;
  RkLex lex[5];
  int cur;
  char tmpbuf[64];

  popCallback(d); /* ������ pop */

  fc = (forichiranContext)d->modec;
  cur = fc->curIkouho;

  freeGetIchiranList(fc->allkouho);

  popForIchiranMode(d);
  popCallback(d);

  tc = (tourokuContext)d->modec;
  WStrcpy(tc->tango_buffer, d->buffer_return);
  tc->tango_buffer[d->nbytes] = 0;
  tc->tango_len = d->nbytes;

  if(RkwXfer(tc->delContext, cur) == NG) {
    if(errno == EPIPE)
      jrKanjiPipeError();
    jrKanjiError = "�����ȸ������Ф��ޤ���Ǥ���";
  }

  if(RkwGetLex(tc->delContext, lex, 5) <= 0) {
    if(errno == EPIPE)
      jrKanjiPipeError();
    jrKanjiError = "�����Ǿ������Ф��ޤ���Ǥ���";
  }
  sprintf((char *)tmpbuf, "#%d#%d",  lex[0].rownum, lex[0].colnum);
  MBstowcs(tc->hcode, tmpbuf, 16);

  if(dicSakujoEndBun(d) == NG) {
    if (tc->udic) free(tc->udic);
    CloseDeleteContext(tc);
    deleteEnd(d);
    return(GLineNGReturnTK(d));
  }
  d->nbytes = 0;

  return(dicSakujoDo(d));
}

static
uuSTangoQuitCatch(d, retval, env)
uiContext d;
int retval;
mode_context env;
/* ARGSUSED */
{
  forichiranContext fc;
  tourokuContext tc;

  popCallback(d); /* ������ pop */

  fc = (forichiranContext)d->modec;
  freeGetIchiranList(fc->allkouho);

  popForIchiranMode(d);
  popCallback(d);
  dicSakujoEndBun(d);

  tc = (tourokuContext)d->modec;
  CloseDeleteContext(tc);
  clearYomi(d);
  return(dicSakujoYomi(d));
}

/*
 * �ɤߤ���ꤵ�줿���񤫤��Ѵ�����
 */
static
dicSakujoBgnBun(d, st)
uiContext d;
RkStat *st;
{
  tourokuContext tc = (tourokuContext)d->modec;
  int nbunsetsu;
  char dicname[1024];

  if(!tc) {
#ifndef DEBUG
    printf("tc = NULL\n");
#endif
  }
  if(!tc->udic) {
#ifndef DEBUG
    printf("tc->udic = NULL\n");
#endif
  }

  if((tc->delContext = RkwCreateContext())== NG) {
    if (errno == EPIPE) {
      jrKanjiPipeError();
    }
    jrKanjiError = "ñ�����ѤΥ���ƥ����Ȥ�����Ǥ��ޤ���";
    return(NG);
  }

  WCstombs(dicname, tc->udic[tc->workDic], 1024);/*������core dump����*/
  if(RkwMountDic(tc->delContext, dicname, 0) == NG) {
    if (errno == EPIPE) {
      jrKanjiPipeError();
    }
    jrKanjiError = "ñ�����Ѥμ����ޥ���ȤǤ��ޤ���Ǥ���";
    CloseDeleteContext(tc);
    return(NG);
  }

  if((nbunsetsu = RkwBgnBun(tc->delContext, tc->yomi_buffer, tc->yomi_len, 0))
	== -1) {
    if(errno == EPIPE)
      jrKanjiPipeError();
    jrKanjiError = "���ʴ����Ѵ��˼��Ԥ��ޤ���";
    CloseDeleteContext(tc);
    return(NG);
  }
  
  if(RkwGetStat(tc->delContext, st) == -1) {
    RkwEndBun(tc->delContext, 0); /* 0:�ؽ����ʤ� */
    if(errno == EPIPE)
      jrKanjiPipeError();
    jrKanjiError = "���ƥ���������Ф��ޤ���Ǥ���";
    CloseDeleteContext(tc);
    return(NG);
  }

  return(nbunsetsu);
}

static
dicSakujoEndBun(d)
uiContext d;
{
  tourokuContext tc = (tourokuContext)d->modec;

  if(RkwEndBun(tc->delContext, 0) == -1) {	/* 0:�ؽ����ʤ� */
    if(errno == EPIPE)
      jrKanjiPipeError();
    jrKanjiError = "���ʴ����Ѵ��ν�λ�˼��Ԥ��ޤ���";
    return(NG);
  }

  return(0);
}

static
dicSakujoTango(d)
uiContext d;
{
  tourokuContext tc = (tourokuContext)d->modec;
  forichiranContext fc;
  ichiranContext ic;
  wchar_t **allDelCands, **getIchiranList();
  unsigned char inhibit = 0;
  int nbunsetsu, nelem, currentkouho, retval = 0;
  RkStat	st;

  if(tc->yomi_len < 1) {
    makeGLineMessageFromString(d, "�ɤߤ����Ϥ��Ƥ�������");

    return dicSakujoYomi(d);
  }

  if((nbunsetsu = dicSakujoBgnBun(d, &st)) == NG) {
    if (tc->udic) free(tc->udic);
    deleteEnd(d);
    return(GLineNGReturnTK(d));
  }
  if((nbunsetsu != 1) || (st.maxcand == 0)) {
    /* ���䤬�ʤ� */
    if(dicSakujoEndBun(d) == NG) {
      deleteEnd(d);
      return(GLineNGReturnTK(d));
    }

    makeGLineMessageFromString(d, "�����ɤߤ���Ͽ���줿ñ���¸�ߤ��ޤ���");
    freeAndPopTouroku(d);
    deleteEnd(d);
    currentModeInfo(d);
    return(0);
  }

  /* ���٤Ƥθ������Ф� */
  if((allDelCands = 
      getIchiranList(tc->delContext, &nelem, &currentkouho)) == 0) {
    if(tc->udic)
      free(tc->udic);
    dicSakujoEndBun(d);
    CloseDeleteContext(tc);
    deleteEnd(d);
    return(GLineNGReturnTK(d));
  }

  if(getForIchiranContext(d) == NG) {
    if(tc->udic)
      free(tc->udic);
    freeGetIchiranList(allDelCands);
    dicSakujoEndBun(d);
    CloseDeleteContext(tc);
    deleteEnd(d);
    return(GLineNGReturnTK(d));
  }

  fc = (forichiranContext)d->modec;
  fc->allkouho = allDelCands;

  if( !HexkeySelect )
    inhibit |= ((unsigned char)NUMBERING | (unsigned char)CHARINSERT);
  else
    inhibit |= (unsigned char)CHARINSERT;

  fc->curIkouho = currentkouho;	/* ���ߤΥ����ȸ����ֹ����¸���� */
  currentkouho = 0;	/* �����ȸ��䤫�鲿���ܤ򥫥��ȸ���Ȥ��뤫 */

  /* ��������˰ܹԤ��� */
  if((retval = selectOne(d, fc->allkouho, &fc->curIkouho, nelem, BANGOMAX,
               inhibit, currentkouho, WITHOUT_LIST_CALLBACK,
	       0, uuSTangoExitCatch,
	       uuSTangoQuitCatch, uiUtilIchiranTooSmall)) == NG) {
    if (tc->udic) free(tc->udic);
    freeGetIchiranList(fc->allkouho);
    CloseDeleteContext(tc);
    deleteEnd(d);
    return(GLineNGReturnTK(d));
  }

  ic = (ichiranContext)d->modec;
  if(ic->tooSmall) {
    d->status = AUX_CALLBACK;
    return(retval);
  }

  makeGlineStatus(d);
  /* d->status = EVERYTIME_CALLBACK; */

  return(retval);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * ñ����                                                                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static
uuSDeleteYesCatch(d, retval, env)
uiContext d;
int retval;
mode_context env;
/* ARGSUSED */
{
  tourokuContext tc;
  char dicname[1024];
  extern defaultContext;
  wchar_t xx1[1024];
  wchar_t *WStraddbcpy();
  int bufcnt;

  deleteEnd(d);
  popCallback(d); /* yesNo ��ݥå� */

  tc = (tourokuContext)d->modec;

  MBstowcs(xx1, " ", 1024);
  WStraddbcpy(d->genbuf, tc->yomi_buffer, ROMEBUFSIZE);
  WStrcat(d->genbuf, xx1);
  WStrcat(d->genbuf, tc->hcode);
  WStrcat(d->genbuf, xx1);
  bufcnt = WStrlen(d->genbuf);
  WStraddbcpy(d->genbuf + bufcnt, tc->tango_buffer, 
                                               ROMEBUFSIZE - bufcnt);

  if(defaultContext == -1) {
    if((KanjiInit() < 0) || (defaultContext == -1)) {
      jrKanjiError = "���ʴ����Ѵ������Ф��̿��Ǥ��ޤ���";
      freeAndPopTouroku(d);
      return(GLineNGReturn(d));
    }
  }
  /* ���񤫤�ñ��������� */
  WCstombs(dicname, tc->udic[tc->workDic], 1024);

  if(RkwDeleteDic(defaultContext, dicname, d->genbuf) == NG) {
    if(errno == EPIPE)
      jrKanjiPipeError();
    MBstowcs(d->genbuf, "ñ�����Ǥ��ޤ���Ǥ���", 512);
  } else {
    extern int auto_sync;
    int l;

    if (auto_sync) {
      RkwSync(defaultContext, dicname);
    }
    /* ����δ�λ��ɽ������ */
    l = MBstowcs(d->genbuf, "��", ROMEBUFSIZE);
    WStrcpy(d->genbuf + l, tc->tango_buffer);
    l += WStrlen(tc->tango_buffer);
    l += MBstowcs(d->genbuf + l, "��(", ROMEBUFSIZE - l);
    WStrcpy(d->genbuf + l, tc->yomi_buffer);
    l += WStrlen(tc->yomi_buffer);
    l += MBstowcs(d->genbuf + l, ")�������ޤ���", ROMEBUFSIZE - l);
  }

  CloseDeleteContext(tc);

  makeGLineMessage(d, d->genbuf, WStrlen(d->genbuf));

  freeAndPopTouroku(d);

  currentModeInfo(d);

  return(0);
}

static
uuSDeleteQuitCatch(d, retval, env)
uiContext d;
int retval;
mode_context env;
/* ARGSUSED */
{
  tourokuContext tc = (tourokuContext)env;

  popCallback(d); /* yesNo ��ݥå� */

  CloseDeleteContext(tc);
  freeAndPopTouroku(d);

  return prevMenuIfExist(d);
}

static
uuSDeleteNoCatch(d, retval, env)
uiContext d;
int retval;
mode_context env;
{
  tourokuContext tc = (tourokuContext)env;

  popCallback(d); /* yesNo ��ݥå� */

  CloseDeleteContext(tc);

  freeAndPopTouroku(d);
  deleteEnd(d);
  currentModeInfo(d);

  GlineClear(d);

  return(retval);
}

static
dicSakujoDo(d)
uiContext d;
{
  tourokuContext tc = (tourokuContext)d->modec;
  int l;

  l = MBstowcs(d->genbuf, "��", ROMEBUFSIZE);
  WStrcpy(d->genbuf + l, tc->tango_buffer);
  l += WStrlen(tc->tango_buffer);
  l += MBstowcs(d->genbuf + l, "��(", ROMEBUFSIZE - l);
  WStrcpy(d->genbuf + l, tc->yomi_buffer);
  l += WStrlen(tc->yomi_buffer);
  l += MBstowcs(d->genbuf + l, ")���?(y/n)", ROMEBUFSIZE - l);
  if(getYesNoContext(d,
	     0, uuSDeleteYesCatch,
	     uuSDeleteQuitCatch, uuSDeleteNoCatch) == NG) {
    if(tc->udic)
      free(tc->udic);
    CloseDeleteContext(tc);
    deleteEnd(d);
    return(GLineNGReturnTK(d));
  }
  makeGLineMessage(d, d->genbuf, WStrlen(d->genbuf));

  return(0);
}
