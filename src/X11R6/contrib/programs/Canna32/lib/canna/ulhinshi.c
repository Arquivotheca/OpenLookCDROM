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
static char rcs_id[] = "@(#) 102.1 $Id: ulhinshi.c,v 5.19 1994/03/10 02:30:25 kon Exp $";
#endif

#include <errno.h>
#include "canna.h"

extern int  errno;

extern int grammaticalQuestion;

extern HexkeySelect;

extern int uiUtilIchiranTooSmall();
extern int uuTTangoQuitCatch();
extern void freeAndPopTouroku();

static int tourokuYes(), tourokuNo(), makeDoushi(), uuTDicExitCatch();
static int uuTDicQuitCatch(), tangoTouroku();

static char *e_message[] = {
  /*0*/"����˺٤����ʻ�ʬ���Τ���μ���򤷤Ƥ��ɤ��Ǥ���?(y/n)",
  /*1*/"�ɤߤȸ���� ���߷������Ϥ��Ƥ���������",
  /*2*/"�ɤߤȸ���� ���Ѥ��㤤�ޤ������Ϥ��ʤ����Ƥ���������",
  /*3*/"�ɤߤȸ���� ���߷������Ϥ��Ƥ�����������) �ᤤ",
  /*4*/"�ɤߤȸ���� ���߷������Ϥ��Ƥ�����������) �Ť���",
  /*5*/"��",
  /*6*/"�ʡפ��������Ǥ�����(y/n)",
  /*7*/"�ʤ��פ��������Ǥ�����(y/n)",
  /*8*/"�פ��������Ǥ�����(y/n)",
  /*9*/"����פ��������Ǥ�����(y/n)",
  /*10*/"�ȡפ��������Ǥ�����(y/n)",
  /*11*/"�������פ��������Ǥ�����(y/n)",
  /*12*/"������פ��������Ǥ�����(y/n)",
  /*13*/"���ʴ����Ѵ������Ф��̿��Ǥ��ޤ���",
  /*14*/"ñ����Ͽ�Ǥ��ޤ���Ǥ���",
  /*15*/"��",
  /*16*/"��",
  /*17*/"��",
  /*18*/"�ˤ���Ͽ���ޤ���",
};

#define message_num (sizeof(e_message) / sizeof(char *))
static wchar_t *message[message_num];

static char sgyouA[] = "���������ʤФޤ��";
static char sgyouI[] = "���������ˤӤߤꤤ";
static char sgyouU[] = "�������Ĥ̤֤�뤦";

static wchar_t *gyouA;
static wchar_t *gyouI;
static wchar_t *gyouU;

/* ���ƤΥ�å�������"unsigned char"����"wchar_t"���Ѵ����� */
int
initHinshiMessage()
{
  int i;

  for(i = 0; i < message_num; i++) {
    message[i] = WString(e_message[i]);
    if(!message[i]) {
      return(-1);
    }
  }
  return 0;
}

/* WSprintf(to_buf, x1, x2, from_buf)
   :WSprintf(to_buf,"x1%sx2",from_buf);
 */
static void
WSprintf(to_buf, x1, x2, from_buf)
wchar_t *to_buf, *x1, *x2, *from_buf;
{
    WStrcpy(to_buf, x1);
    WStrcat(to_buf, from_buf);
    WStrcat(to_buf, x2);
}

static void
EWStrcat(buf, xxxx)
wchar_t *buf;
char *xxxx;
{
  wchar_t x[1024];

  MBstowcs(x, xxxx, 1024);
  WStrcat(buf, x);
}

static void
EWStrcpy(buf, xxxx)
wchar_t *buf;
char *xxxx;
{
  wchar_t x[1024];
  int len;

  len = MBstowcs(x, xxxx, 1024);
  WStrncpy(buf, x, len);
  buf[len] = 0;
}

static int
EWStrcmp(buf, xxxx)
wchar_t *buf;
char *xxxx;
{
  wchar_t x[1024];

  MBstowcs(x, xxxx, 1024);
  return(WStrncmp(buf, x, WStrlen(x)));
}

static int
EWStrncmp(buf, xxxx, len)
wchar_t *buf;
char *xxxx;
int len;
/* ARGSUSED */
{
  wchar_t x[1024];

  MBstowcs(x, xxxx, 1024);
  return(WStrncmp(buf, x, WStrlen(x)));
}

int
initGyouTable()
{
  gyouA = WString(sgyouA);
  gyouI = WString(sgyouI);
  gyouU = WString(sgyouU);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * ñ����Ͽ���ʻ����� ��Yes/No ���� Quit��                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static
uuTHinshiYNQuitCatch(d, retval, env)
uiContext d;
int retval;
mode_context env;
/* ARGSUSED */
{
  popCallback(d);
  
  return(dicTourokuHinshi(d));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * ñ����Ͽ���ʻ����� ��Yes/No �裲�ʳ� ���̥�����Хå���                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static
uuTHinshi2YesCatch(d, retval, env)
uiContext d;
int retval;
mode_context env;
/* ARGSUSED */
{
  tourokuContext tc;

  popCallback(d); /* yesNo ��ݥå� */

  tourokuYes(d);   /* �ʻ줬��ޤ�� tc->hcode �˥��åȤ��� */

  tc = (tourokuContext)d->modec;

  if (!tc->qbuf[0]) {
    if (tc->hcode[0]) {
      /* �ʻ줬��ޤä��Τǡ���Ͽ����桼������λ����Ԥ� */
      return(dicTourokuDictionary(d, uuTDicExitCatch, uuTDicQuitCatch));
    }
  }
  return(retval);
}

static
uuTHinshi2NoCatch(d, retval, env)
uiContext d;
int retval;
mode_context env;
/* ARGSUSED */
{
  tourokuContext tc;

  popCallback(d); /* yesNo ��ݥå� */

  tourokuNo(d);   /* �ʻ줬��ޤ�� tc->hcode �˥��åȤ��� */

  tc = (tourokuContext)d->modec;

  if (!tc->qbuf[0]) {
    if (tc->hcode[0]) {
      /* �ʻ줬��ޤä��Τǡ���Ͽ����桼������λ����Ԥ� */
      return(dicTourokuDictionary(d, uuTDicExitCatch, uuTDicQuitCatch));
    }
  }

  return(retval);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * ñ����Ͽ���ʻ����� ��Yes/No �裱�ʳ� ������Хå���                       *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static
uuTHinshi1YesCatch(d, retval, env)
uiContext d;
int retval;
mode_context env;
/* ARGSUSED */
{
  tourokuContext tc;
  coreContext ync;
  
  popCallback(d); /* yesNo ��ݥå� */

  tourokuYes(d);   /* �ʻ줬��ޤ�� tc->hcode �˥��åȤ��� */

  tc = (tourokuContext)d->modec;

  if(tc->qbuf[0]) {
    /* ���䤹�� */
    makeGLineMessage(d, tc->qbuf, WStrlen(tc->qbuf));
    if((retval = getYesNoContext(d,
		 0, uuTHinshi2YesCatch,
		 uuTHinshiYNQuitCatch, uuTHinshi2NoCatch)) == NG) {
      defineEnd(d);
      return(GLineNGReturnTK(d));
    }
    ync = (coreContext)d->modec;
    ync->majorMode = CANNA_MODE_ExtendMode;
    ync->minorMode = CANNA_MODE_TourokuHinshiMode;
  } else if(tc->hcode[0]) {
    /* �ʻ줬��ޤä��Τǡ���Ͽ����桼������λ����Ԥ� */
    return(dicTourokuDictionary(d, uuTDicExitCatch, uuTDicQuitCatch));
  }

  return(retval);
}

static
uuTHinshi1NoCatch(d, retval, env)
uiContext d;
int retval;
mode_context env;
/* ARGSUSED */
{
  tourokuContext tc;
  coreContext ync;

  popCallback(d); /* yesNo ��ݥå� */

  tourokuNo(d);   /* �ʻ줬��ޤ�� tc->hcode �˥��åȤ��� */

  tc = (tourokuContext)d->modec;

  if(tc->qbuf[0]) {
    /* ���䤹�� */
    makeGLineMessage(d, tc->qbuf, WStrlen(tc->qbuf));
    if((retval = getYesNoContext(d,
		 0, uuTHinshi2YesCatch,
		 uuTHinshiYNQuitCatch, uuTHinshi2NoCatch)) == NG) {
      defineEnd(d); 
      return(GLineNGReturnTK(d));
    }
    ync = (coreContext)d->modec;
    ync->majorMode = CANNA_MODE_ExtendMode;
    ync->minorMode = CANNA_MODE_TourokuHinshiMode;
  } else if(tc->hcode[0]) {
    /* �ʻ줬��ޤä��Τǡ���Ͽ����桼������λ����Ԥ� */
    return(dicTourokuDictionary(d, uuTDicExitCatch, uuTDicQuitCatch));
  }

  return(retval);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * ñ����Ͽ���ʻ�ʬ�����롩                                                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static
uuTHinshiQYesCatch(d, retval, env)
uiContext d;
int retval;
mode_context env;
/* ARGSUSED */
{
  tourokuContext tc;
  coreContext ync;

  popCallback(d); /* yesNo ��ݥå� */

  tc = (tourokuContext)d->modec;

  makeGLineMessage(d, tc->qbuf, WStrlen(tc->qbuf)); /* ���� */
  if((retval = getYesNoContext(d,
	 0, uuTHinshi1YesCatch,
	 uuTHinshiYNQuitCatch, uuTHinshi1NoCatch)) == NG) {
    defineEnd(d);
    return(GLineNGReturnTK(d));
  }
  ync = (coreContext)d->modec;
  ync->majorMode = CANNA_MODE_ExtendMode;
  ync->minorMode = CANNA_MODE_TourokuHinshiMode;

  return(retval);
}

static
uuTHinshiQNoCatch(d, retval, env)
uiContext d;
int retval;
mode_context env;
/* ARGSUSED */
{
  popCallback(d); /* yesNo ��ݥå� */

  return(dicTourokuDictionary(d, uuTDicExitCatch, uuTDicQuitCatch));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * ñ����Ͽ���ʻ�����                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static int makeHinshi();

dicTourokuHinshiDelivery(d)
uiContext	d;
{
  tourokuContext tc = (tourokuContext)d->modec;
  coreContext ync;
  int retval = 0;

  makeHinshi(d); /* �ʻ졢���顼��å�����������򥻥åȤ��Ƥ��� */

#ifdef DEBUG
  if(iroha_debug) {
    printf("tc->genbuf=%s, tc->qbuf=%s, tc->hcode=%s\n", tc->genbuf, tc->qbuf,
	   tc->hcode);
  }
#endif
  if(tc->genbuf[0]) {
    /* ���Ϥ��줿�ǡ����˸�꤬���ä��Τǡ�
       ��å�������ɽ�������ɤ����Ϥ���� */
    clearYomi(d);
    return(dicTourokuTango(d, uuTTangoQuitCatch));
  } else if(tc->qbuf[0] && grammaticalQuestion) {
    /* �٤����ʻ�ʬ���Τ���μ���򤹤� */
    WStrcpy(d->genbuf, message[0]);
    if((retval = getYesNoContext(d,
		 0, uuTHinshiQYesCatch,
		 uuTHinshiYNQuitCatch, uuTHinshiQNoCatch)) == NG) {
      defineEnd(d);
      return(GLineNGReturnTK(d));
    }
    makeGLineMessage(d, d->genbuf, WStrlen(d->genbuf));
    ync = (coreContext)d->modec;
    ync->majorMode = CANNA_MODE_ExtendMode;
    ync->minorMode = CANNA_MODE_TourokuHinshiMode;
    return(retval);
  } else if(tc->hcode[0]) {
    /* �ʻ줬��ޤä��Τǡ���Ͽ����桼������λ����Ԥ� */
    return(dicTourokuDictionary(d, uuTDicExitCatch, uuTDicQuitCatch));
  }
  return 0;
}

/*
 * ���򤵤줿�ʻ줫�鼡��ư���Ԥ�
 * 
 * tc->hcode	�ʻ�
 * tc->qbuf	����
 * tc->genbuf	���顼
 */
static int
makeHinshi(d)
uiContext	d;
{
  tourokuContext tc = (tourokuContext)d->modec;
  int tlen, ylen, yomi_katsuyou;
  wchar_t tmpbuf[256];

  tc->hcode[0] = 0;
  tc->qbuf[0] = 0;
  tc->genbuf[0] = 0;

  tlen = tc->tango_len;
  ylen = tc->yomi_len;

  switch(tc->curHinshi) {
  case 0:  /* ��̾ */
    EWStrcpy(tc->hcode, "#JN");
    break;

  case 1:  /* ��̾ */
    EWStrcpy(tc->hcode, "#CN");
    break;
    
  case 2:  /* ���Ρ����̾ */
    EWStrcpy(tc->hcode, "#KK");
    break;

  case 3:  /* ̾�� �� */

  case 4:  /* ����̾�� �� */
    if(tc->curHinshi == 3)
      EWStrcpy(tc->hcode, "#T35"); /* �ܺ٤��ʻ��ɬ�פȤ��ʤ���� */
    else
      EWStrcpy(tc->hcode, "#T30"); /* �ܺ٤��ʻ��ɬ�פȤ��ʤ���� */
    tc->katsuyou = 0;
    WSprintf(tc->qbuf, message[5], message[6], tc->tango_buffer);
    break;

  case 5:  /* ñ���� */
    EWStrcpy(tc->hcode, "#KJ");
    break;

  case 6:  /* ư�� �� */

    /* ���Ϥ����߷����� */
    tc->katsuyou = 0;
    while (tc->katsuyou<GOBISUU &&
	   tc->tango_buffer[tlen - 1] != gyouU[tc->katsuyou]) {
      tc->katsuyou++;
    }
    yomi_katsuyou = 0;
    while (yomi_katsuyou<GOBISUU &&
	   tc->yomi_buffer[ylen - 1] != gyouU[yomi_katsuyou]) {
      yomi_katsuyou++;
    }
    if((tc->katsuyou == GOBISUU) || (yomi_katsuyou == GOBISUU)){
      WStrcpy(tc->genbuf, message[1]);
      return(0);
    }
    if(tc->katsuyou != yomi_katsuyou){
      WStrcpy(tc->genbuf, message[2]);
      return(0);
    }

    /* �㳰���� */
    if(!(EWStrcmp(tc->tango_buffer, "����"))) {
      /* �����ѳʳ��� */
      EWStrcpy(tc->hcode, "#KX");
      return(0);
    }
    if(tlen >= 3 &&
       !(EWStrcmp(tc->tango_buffer+tlen-3, "�󤺤�"))) {
      /* �󥶹��ѳʳ��� */
      EWStrcpy(tc->hcode, "#NZX");
      return(0);
    }
    if(tlen >= 2 &&
       !(EWStrcmp(tc->tango_buffer+tlen-2, "����"))) {
      /* �����ѳʳ��� */
      EWStrcpy(tc->hcode, "#ZX");
      return(0);
    }
    if(!(EWStrcmp(tc->tango_buffer, "����"))) {
      /* �����ѳʳ��� */
      EWStrcpy(tc->hcode, "#SX");
      return(0);
    }

    makeDoushi(d); /* �ܺ٤��ʻ��ɬ�פȤ��ʤ���� */
    /* ̤������Ĥ��� */
    WStrncpy(tmpbuf, tc->tango_buffer, tlen-1);  
    tmpbuf[tlen - 1] = gyouA[tc->katsuyou];
    tmpbuf[tlen] = (wchar_t)0;
    WSprintf(tc->qbuf, message[5], message[7], tmpbuf);
    break;

  case 7:  /* ���ƻ� �� */
    tc->katsuyou = 1;
    if(tlen >= 1 && ylen >= 1 &&
       ((EWStrncmp(tc->tango_buffer+tlen-1, "��", 1) != 0) ||
	(EWStrncmp(tc->yomi_buffer+ylen-1, "��", 1) != 0))) {
      WStrcpy(tc->genbuf, message[3]);
      return(0);
    }

    EWStrcpy(tc->hcode, "#KY"); /* �ܺ٤��ʻ��ɬ�פȤ��ʤ���� */
    WStrncpy(tmpbuf, tc->tango_buffer, tlen-1);  
    tmpbuf[tlen-1] = 0;
    WSprintf(tc->qbuf, message[5], message[8], tmpbuf);
    break;

  case 8:  /* ����ư�� �� */
    tc->katsuyou = 1;
    if(tlen >= 1 && ylen >= 1 &&
       ((EWStrncmp(tc->tango_buffer+tlen-1, "��", 1)) ||
	(EWStrncmp(tc->yomi_buffer+ylen-1, "��", 1)))) {
      WStrcpy(tc->genbuf, message[4]);
      return(0);
    }
    EWStrcpy(tc->hcode, "#T05"); /* �ܺ٤��ʻ��ɬ�פȤ��ʤ���� */
    WStrncpy(tmpbuf, tc->tango_buffer, tlen-1);  
    tmpbuf[tlen-1] = 0;  
    WSprintf(tc->qbuf, message[5], message[9], tmpbuf);
    break;

  case 9:  /* ���� ��*/
    EWStrcpy(tc->hcode, "#F14"); /* �ܺ٤��ʻ��ɬ�פȤ��ʤ���� */
    tc->katsuyou = 0;
    WSprintf(tc->qbuf, message[5], message[9], tc->tango_buffer);
    break;

  case 10: /* ���� */
    EWStrcpy(tc->hcode, "#NN");
    break;

  case 11: /* ��³�졦��ư�� */
    EWStrcpy(tc->hcode, "#CJ");
    break;

  case 12: /* Ϣ�λ� */
    EWStrcpy(tc->hcode, "#RT");
    break;

  case 13: /* ����¾�θ�ͭ̾�� */
    EWStrcpy(tc->hcode, "#KK");
    break;

  case 14:  /* ���� ���� */

  case 15:  /* ���� ���� */
    WSprintf(tc->qbuf, message[5], message[10], tc->tango_buffer);
    break;

  case 16:  /* ư�� ���� */
    WStrncpy(tmpbuf, tc->tango_buffer, tlen-1);
    tmpbuf[tlen - 1] = gyouI[tc->katsuyou];
    tmpbuf[tlen] = (wchar_t)'\0';
    WSprintf(tc->qbuf, message[5], message[11], tmpbuf);
    break;

  case 17:  /* ư�� ���� */
    /* �岼���ʳ��Ѥ��� */
    WStrncpy(tmpbuf, tc->tango_buffer, tlen-1);
    tmpbuf[tlen-1] = (wchar_t)'\0';
    WSprintf(tc->qbuf, message[5], message[11], tmpbuf);
    break;

  case 18:  /* ����ư�� ���� */

  case 19:  /* ����ư�� ���� */
    WStrncpy(tmpbuf, tc->tango_buffer, tlen-1);  
    tmpbuf[tlen-1] = 0;
    WSprintf(tc->qbuf, message[5], message[12], tmpbuf);
    break;
  }

  return(0);
}

static
tourokuYes(d)
uiContext	d;
{
  tourokuContext tc = (tourokuContext)d->modec;

  tc->hcode[0] = 0;
  tc->qbuf[0] = 0;
  tc->genbuf[0] = 0;

  switch(tc->curHinshi) {
  case 3:  /* ̾�� */
    EWStrcpy(tc->hcode, "#T15");   /* ���������� */
    break;

  case 4:  /* ����̾�� */
    EWStrcpy(tc->hcode, "#T10");          /* �¿����ⵤ */
    break;

  case 6:  /* ư�� */
    tc->curHinshi = 16;
    makeHinshi(d);
    break;

  case 7:  /* ���ƻ� */
    EWStrcpy(tc->hcode, "#KYT");          /* ������ */
    break;

  case 8:  /* ����ư�� */
    tc->curHinshi = 18;
    makeHinshi(d);
    break;

  case 9:  /* ���� ��*/
    tc->curHinshi = 14;
    makeHinshi(d);
    break;

  case 14:  /* ���� ���� */
    EWStrcpy(tc->hcode, "#F04");          /* �դä��� */
    break;

  case 15:  /* ���� ���� */
    EWStrcpy(tc->hcode, "#F06");          /* ���� */
    break;

  case 16:  /* ư�� ���� */
    makeDoushi(d);
    EWStrcat( tc->hcode, "r" );
    break;

  case 17:  /* ư�� ���� */
    EWStrcpy(tc->hcode, "#KSr");          /* ������ */
    break;

  case 18:  /* ����ư�� ���� */
    EWStrcpy(tc->hcode, "#T10");          /* �ؿ��� */
    break;

  case 19:  /* ����ư�� ���� */
    EWStrcpy(tc->hcode, "#T15");          /* �ճ�������ǽ�� */
    break;
  }

  return(0);
}

static
tourokuNo(d)
uiContext	d;
{
  tourokuContext tc = (tourokuContext)d->modec;
  wchar_t test[1024];

  tc->hcode[0] = 0;
  tc->qbuf[0] = 0;
  tc->genbuf[0] = 0;

  switch( tc->curHinshi ) {
  case 3:  /* ̾�� */
    EWStrcpy(tc->hcode, "#T35");   /* ������ */
    EWStrcpy(test, "#T35");   /* ������ */
    break;

  case 4:  /* ����̾�� */
    EWStrcpy(tc->hcode, "#T30");          /* ���ϡ����� */
    break;

  case 6:  /* ư�� */
    tc->curHinshi = 17;
    makeHinshi(d);
    break;

  case 7:  /* ���ƻ� */
    EWStrcpy(tc->hcode, "#KY");           /* ���������ᤤ */
    break;

  case 8:  /* ����ư�� */
    tc->curHinshi = 19;
    makeHinshi(d);
    break;

  case 9:  /* ���� ��*/
    tc->curHinshi = 15;
    makeHinshi(d);
    break;

  case 14:  /* ���� ���� */
    EWStrcpy(tc->hcode, "#F12");          /* ���ä� */
    break;

  case 15:  /* ���� ���� */
    EWStrcpy(tc->hcode, "#F14");          /* ˰���ޤ� */
    break;

  case 16:  /* ư�� ���� */
    makeDoushi(d);
    break;

  case 17:  /* ư�� ���� */
    EWStrcpy(tc->hcode, "#KS");           /* �ߤ�� */
    break;

  case 18:  /* ����ư�� ���� */
    EWStrcpy(tc->hcode, "#T13");          /* ¿���Ƥ� */
    break;

  case 19:  /* ����ư�� ���� */
    EWStrcpy(tc->hcode, "#T18");          /* ���������Ť��� */
    break;
  }
  return(0);
}

static
makeDoushi(d)
uiContext	d;
{
  tourokuContext tc = (tourokuContext)d->modec;

    switch(tc->katsuyou){
    case  0:
      EWStrcpy( tc->hcode, "#K5" );     /* �֤� */
      break;
    case  1:
      EWStrcpy( tc->hcode, "#G5" );     /* �Ĥ� */
      break;
    case  2:
      EWStrcpy( tc->hcode, "#S5" );     /* �֤� */
      break;
    case  3:
      EWStrcpy( tc->hcode, "#T5" );     /* ��� */
      break;
    case  4:
      EWStrcpy( tc->hcode, "#N5" );     /* ��� */
      break;
    case  5:
      EWStrcpy( tc->hcode, "#B5" );     /* ž�� */
      break;
    case  6:
      EWStrcpy( tc->hcode, "#M5" );     /* ���� */
      break;
    case  7:
      EWStrcpy( tc->hcode, "#R5" );     /* ��ĥ�� */
      break;
    case  8:
      EWStrcpy( tc->hcode, "#W5" );     /* ���� */
      break;
    }
}    

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * ����ΰ���                                                                *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static
uuTDicExitCatch(d, retval, env)
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

  return(tangoTouroku(d));
}

static
uuTDicQuitCatch(d, retval, env)
uiContext d;
int retval;
mode_context env;
/* ARGSUSED */
{
  popCallback(d); /* ������ pop */

  popForIchiranMode(d);
  popCallback(d);

  return(dicTourokuHinshi(d));
}

dicTourokuDictionary(d, exitfunc, quitfunc)
uiContext d;
int (*exitfunc)();
int (*quitfunc)();
{
  tourokuContext tc = (tourokuContext)d->modec;
  forichiranContext fc;
  ichiranContext ic;
  wchar_t **work;
  unsigned char inhibit = 0;
  int retval, upnelem = 0;

  retval = d->nbytes = 0;
  d->status = 0;

  for(work = tc->udic; *work; work++)
    upnelem++;

  if((retval = getForIchiranContext(d)) == NG) {
    if(tc->udic)
      free(tc->udic);
    defineEnd(d);
    return(GLineNGReturnTK(d));
  }
  fc = (forichiranContext)d->modec;

  /* selectOne ��Ƥ֤���ν��� */

  fc->allkouho = tc->udic;

  fc->curIkouho = 0;
  if( !HexkeySelect )
    inhibit |= ((unsigned char)NUMBERING | (unsigned char)CHARINSERT); 
  else
    inhibit |= (unsigned char)CHARINSERT;

   if((retval = selectOne(d, fc->allkouho, &fc->curIkouho, upnelem,
		 BANGOMAX, inhibit, 0, WITHOUT_LIST_CALLBACK,
		 0, exitfunc, quitfunc, uiUtilIchiranTooSmall)) == NG) {
    if(fc->allkouho)
      free(fc->allkouho);
    popForIchiranMode(d);
    popCallback(d);
    defineEnd(d);
    return(GLineNGReturnTK(d));
  }

  ic = (ichiranContext)d->modec;
  ic->majorMode = CANNA_MODE_ExtendMode;
  ic->minorMode = CANNA_MODE_TourokuDicMode;
  currentModeInfo(d);

  /* ��������Ԥ������Ƹ���������Ф��ʤ� */
  if(ic->tooSmall) {
    d->status = AUX_CALLBACK;
    return(retval);
  }

  makeGlineStatus(d);
  /* d->status = ICHIRAN_EVERYTIME; */

  return(retval);
}

/*
 * ñ����Ͽ��Ԥ�
 */
static
tangoTouroku(d)
uiContext	d;
{
  tourokuContext tc = (tourokuContext)d->modec;
  wchar_t ktmpbuf[256];
  wchar_t ttmpbuf[256];
  wchar_t line[ROMEBUFSIZE];
  wchar_t xxxx[1024];
  char dicname[1024];
  extern defaultContext;
  int linecnt;
  wchar_t *WStraddbcpy();

  defineEnd(d);
  if(tc->katsuyou || (EWStrncmp(tc->hcode, "#K5", 3) == 0)) {
    WStrncpy(ttmpbuf, tc->tango_buffer, tc->tango_len - 1);
    ttmpbuf[tc->tango_len - 1] = (wchar_t)0;
    WStrncpy(ktmpbuf, tc->yomi_buffer, tc->yomi_len - 1);
    ktmpbuf[tc->yomi_len - 1] = 0;
  } else {
    WStrcpy(ttmpbuf, tc->tango_buffer);
    WStrcpy(ktmpbuf, tc->yomi_buffer);
  }

  /* ����񤭹����Ѥΰ�Ԥ��� */
  WStraddbcpy(line, ktmpbuf, ROMEBUFSIZE);
  EWStrcat(line, " ");
  WStrcat(line, tc->hcode);
  EWStrcat(line, " ");
  linecnt = WStrlen(line);
  WStraddbcpy(line + linecnt, ttmpbuf, ROMEBUFSIZE - linecnt);

  if(defaultContext == -1) {
    if((KanjiInit() < 0) || (defaultContext == -1)) {
      jrKanjiError = (char *)e_message[13];
      freeAndPopTouroku(d);
      return(GLineNGReturn(d));
    }
  }
  /* �������Ͽ���� */
  WCstombs(dicname, tc->udic[tc->workDic], 1024);

  if(RkwDefineDic(defaultContext, dicname, line) != 0) {
    if(errno == EPIPE)
      jrKanjiPipeError();
    WStrcpy(d->genbuf, message[14]);
  } else {
    extern int auto_sync;

    if (auto_sync) {
      RkwSync(defaultContext, dicname);
    }
    /* ��Ͽ�δ�λ��ɽ������ */
    WSprintf(d->genbuf, message[15], message[16], tc->tango_buffer);
    WSprintf(xxxx, message[17], message[18], tc->yomi_buffer);
    WStrcat(d->genbuf, xxxx);
  }
  makeGLineMessage(d, d->genbuf, WStrlen(d->genbuf));

  freeAndPopTouroku(d);
  currentModeInfo(d);

  return(0); /* ñ����Ͽ��λ */
}
