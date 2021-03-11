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
static	char	rcs_id[] = "@(#) 102.1 $Id: onoff.c,v 5.4 1994/03/01 14:06:59 kon Exp $";
#endif /* lint */

#include	<errno.h>
#include	"canna.h"

#define ICHISIZE 9

extern int kCount;

static int makeOnOffIchiran();

static wchar_t *black;
static wchar_t *white;
static wchar_t *space;

int
initOnoffTable()
{
  black = WString("��");
  white = WString("��");
  space = WString("��");
}

static void
popOnOffMode(d)
uiContext d;
{
  ichiranContext oc = (ichiranContext)d->modec;

  d->modec = oc->next;
  d->current_mode = oc->prevMode;
  freeIchiranContext(oc);
}

/*
 * ��������Ԥ���
 */
selectOnOff(d, buf, ck, nelem, bangomax, currentkouho, status,
	  everyTimeCallback, exitCallback, quitCallback, auxCallback)
uiContext d;
wchar_t **buf;
int *ck;
int nelem, bangomax;
int currentkouho;
unsigned char *status;
int (*everyTimeCallback)(), (*exitCallback)();
int (*quitCallback)(), (*auxCallback)();
{
  extern KanjiModeRec onoff_mode;
  ichiranContext oc;
  int retval = 0;
  ichiranContext newIchiranContext();

  if(pushCallback(d, d->modec,
	everyTimeCallback, exitCallback, quitCallback, auxCallback) == 0) {
    jrKanjiError = "malloc (pushCallback) �Ǥ��ޤ���Ǥ���";
    return(NG);
  }
  
  if((oc = (ichiranContext)newIchiranContext()) == NULL) {
    popCallback(d);
    return(NG);
  }
  oc->next = d->modec;
  d->modec = (mode_context)oc;

  oc->prevMode = d->current_mode;
  d->current_mode = &onoff_mode;

  oc->allkouho = buf;
  oc->curIkouho = ck;

  if((retval = makeOnOffIchiran(d, nelem, bangomax,
			currentkouho, status))   == NG) {
    popOnOffMode(d);
    popCallback(d);
    return(NG);
  }
  return(retval);
}

/*
 * ��������Ԥ�ɽ���ѤΥǡ�����ơ��֥�˺�������
 *
 * ��glineinfo �� kouhoinfo���������
 *
 * ������	uiContext
 * �����	���ｪλ�� 0	�۾ｪλ�� -1
 */
static
makeOnOffIchiran(d, nelem, bangomax, currentkouho, status)
uiContext d;
int nelem, bangomax;
int currentkouho;
unsigned char *status;
{
  ichiranContext oc = (ichiranContext)d->modec;
  wchar_t **kkptr, *kptr, *gptr, *svgptr;
  int ko, lnko, cn = 0, svcn, line = 0, dn = 0, svdn;

  oc->nIkouho = nelem;	/* ����ο� */

  /* �����ȸ���򥻥åȤ��� */
  oc->svIkouho = *(oc->curIkouho);
  *(oc->curIkouho) += currentkouho;
  if(*(oc->curIkouho) >= oc->nIkouho)
    oc->svIkouho = *(oc->curIkouho) = 0;

  if(allocIchiranBuf(d) == NG)
    return(NG);

  if(d->ncolumns < 1) {
    oc->tooSmall = 1;
    return(0);
  }

  /* glineinfo��kouhoinfo���� */
  /* 
   ��glineinfo��
      int glkosu   : int glhead     : int gllen  : wchar_t *gldata
      ���Ԥθ���� : ��Ƭ���䤬     : ���Ԥ�Ĺ�� : ��������Ԥ�ʸ����
                   : �����ܤθ��䤫 :
   -------------------------------------------------------------------------
   0 | 6           : 0              : 24         : �����������ʣ�����������
   1 | 4           : 6              : 16         : ���ã�����������

    ��kouhoinfo��
      int khretsu  : int khpoint  : wchar_t *khdata
      �ʤ����ܤ�   : �Ԥ���Ƭ���� : �����ʸ����
      ������䤫   : ���Х����ܤ� :
   -------------------------------------------------------------------------
   0 | 0           : 0            : ��
   1 | 0           : 4            : ��
             :                :             :
   7 | 1           : 0            : ��
   8 | 1           : 4            : ��
  */

  kkptr = oc->allkouho;
  kptr = *(oc->allkouho);
  gptr = oc->glinebufp;

  /* line -- �����ܤ�
     ko   -- ���Τ���Ƭ���鲿���ܤθ��䤫
     lnko -- �����Ƭ���鲿���ܤθ��䤫
     cn   -- �����Ƭ���鲿�Х����ܤ� */

  for(line=0, ko=0; ko<oc->nIkouho; line++) {
    oc->glineifp[line].gldata = gptr; /* ����Ԥ�ɽ�����뤿���ʸ���� */
    oc->glineifp[line].glhead = ko;   /* ���ιԤ���Ƭ����ϡ����ΤǤ�ko���� */

    oc->tooSmall = 1;
    for(lnko = cn = dn = 0;
	dn<d->ncolumns - (kCount ? ICHISIZE + 1: 0) &&
	lnko<bangomax && ko<oc->nIkouho ; lnko++, ko++) {
      oc->tooSmall = 0;
      kptr = kkptr[ko];
      oc->kouhoifp[ko].khretsu = line; /* �����ܤ�¸�ߤ��뤫��Ͽ */
      oc->kouhoifp[ko].khpoint = cn + (lnko ? 1 : 0);
      oc->kouhoifp[ko].khdata = kptr;  /* ����ʸ����ؤΥݥ��� */
      svgptr = gptr;
      svcn = cn;
      svdn = dn;

      /* �������򥳥ԡ����� */
      if(lnko) {
	WStrncpy(gptr++, space, WStrlen(space));
	cn++; dn += 2;
      }
      if(status[ko] == 1)
	WStrncpy(gptr, black, WStrlen(black));
      else
	WStrncpy(gptr, white, WStrlen(white));	 
      cn ++; gptr++; dn +=2;
      /* ����򥳥ԡ����� */
      for(; *kptr && dn<d->ncolumns - (kCount ? ICHISIZE + 1: 0);
	  gptr++, kptr++, cn++) {
	if (((*gptr = *kptr) & 0x8080) == 0x8080) dn++;
        dn++;
      }

      /* ���������Ϥߤ����Ƥ��ޤ������ˤʤä��Τǣ����᤹ */
      if ((dn >= d->ncolumns - (kCount ? ICHISIZE + 1: 0)) && *kptr) {
	if (lnko) {
	  gptr = svgptr;
	  cn = svcn;
	  dn = svdn;
	}
	else {
	  oc->tooSmall = 1;
	}
	break;
      }
    }
    if (oc->tooSmall) {
      return 0;
    }
    if (kCount) {
      for (;dn < d->ncolumns - 1; dn++) {
	*gptr++ = ' ';
      }
    }
    /* ���Խ���� */
    *gptr++ = (wchar_t)0;
    oc->glineifp[line].glkosu = lnko;
    oc->glineifp[line].gllen = WStrlen(oc->glineifp[line].gldata);
  }
  /* �Ǹ��NULL������� */
  oc->kouhoifp[ko].khretsu = 0;
  oc->kouhoifp[ko].khpoint = 0;
  oc->kouhoifp[ko].khdata  = (wchar_t *)NULL;
  oc->glineifp[line].glkosu  = 0;
  oc->glineifp[line].glhead  = 0;
  oc->glineifp[line].gllen   = 0;
  oc->glineifp[line].gldata  = (wchar_t *)NULL;

#ifdef DEBUG
  if (iroha_debug) {
    int i;
    for(i=0; oc->glineifp[i].glkosu; i++)
      printf("%d: %s\n", i, oc->glineifp[i].gldata);
  }
#endif

  return(0);
}

/*
 * �����ȸ���򸽺ߤ�ȿ�Фˤ���(ON��OFF, OFF��ON)
 *
 * ������	uiContext
 * �����	���ｪλ�� 0	�۾ｪλ�� -1
 */
static
OnOffSelect(d)
uiContext d;
{
  ichiranContext oc = (ichiranContext)d->modec;
  mountContext mc = (mountContext)oc->next;
  int point, retval = 0;
  wchar_t *gline;

  /* mountNewStatus ���ѹ����� (1��0, 0��1) */
  if(mc->mountNewStatus[*(oc->curIkouho)])
    mc->mountNewStatus[*(oc->curIkouho)] = 0;
  else
    mc->mountNewStatus[*(oc->curIkouho)] = 1;

  /* gline�ѤΥǡ�����񤭴����� (������, ������) */
  gline = oc->glineifp[oc->kouhoifp[*(oc->curIkouho)].khretsu].gldata;
  point = oc->kouhoifp[*(oc->curIkouho)].khpoint;

    *(gline+point) = ((mc->mountNewStatus[*(oc->curIkouho)]) ? *black : *white);
  makeGlineStatus(d);
  /* d->status = EVERYTIME_CALLBACK; */

  return(retval);
}

/*
 * status �򤽤Τޤ��֤���OnOff�⡼�ɤ�POP���� (EXIT_CALLBACK)
 *
 * ������	uiContext
 * �����	���ｪλ�� 0	�۾ｪλ�� -1
 */
static
OnOffKakutei(d)
uiContext d;
{
  ichiranContext oc = (ichiranContext)d->modec;
  int retval = 0;
/* ����ʤ��ΤǤ� unsigned char *kakuteiStrings;*/

  /* �������ɽ�����ѤΥ��ꥢ��ե꡼���� */
  freeIchiranBuf(oc);

  popOnOffMode(d);

#ifdef DEBUG
  if(iroha_debug) {
    mountContext mc = (mountContext)d->modec;
    int i;

    printf("<��mount>\n");
    for(i= 0; mc->mountList[i]; i++)
      printf("[%s][%x][%x]\n", mc->mountList[i],
	     mc->mountOldStatus[i], mc->mountNewStatus[i]);
    printf("\n");
  }
#endif

  /* gline �򥯥ꥢ���� */
  GlineClear(d);

  d->status = EXIT_CALLBACK;

  return(retval);
}

#include	"onoffmap.c"
