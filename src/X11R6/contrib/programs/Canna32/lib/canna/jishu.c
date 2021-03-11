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
static char rcs_id[] = "@(#) 102.1 $Id: jishu.c,v 5.8 1994/03/10 02:25:57 kon Exp $";
#endif /* lint */

#include "canna.h"
#include <ctype.h>

extern int WToupper();
static int setInhibitInformation(), JishuNextJishu(), JishuPreviousJishu();
static int jishuAdjustRome(), myjishuAdjustRome(), JishuZenkaku();
static int JishuHankaku();

/* yc->jishu_kc          ����ʸ���狼
 * d->jishu_rEndp
 * d->jishu_kEndp
 * ����� 
 *               ������sh|
 * C-            ������sh|
 * C-            ������s|h
 * C-            ������|sh
 * C-            ����|��sh
 *
 *               ������sh|
 * C-            aishish|
 * C-            ��������|
 * C-            �������|h
 * C-            ������|sh
 * C-            �����|��sh
 * C-            aish|��sh
 * C-            ����sh|��sh
 * C-            ����sh|sh
 * C-            ����s|��sh
 * C-            ����|��sh
 * C-
 * 
 */

#define	INHIBIT_HANKATA	01
#define	INHIBIT_KANA	02
#define INHIBIT_ALPHA	04

void
enterJishuMode(d, yc)
uiContext d;
yomiContext yc;
{
  extern KanjiModeRec jishu_mode;
  int pos;

  yc->jishu_kc = JISHU_HIRA;/* ���ϤҤ餬�ʥ⡼�ɤǤ� */
  yc->jishu_case = 0; /* ��ʸ���١����Υ⡼�ɤǤ� */
  setInhibitInformation(yc);
  if (yc->cmark < yc->cStartp) {
    yc->cmark = yc->cStartp;
  }
  if (yc->kCurs == yc->cmark) {
    yc->jishu_kEndp = yc->kEndp;
    yc->jishu_rEndp = yc->rEndp;
  }
  else if (yc->kCurs < yc->cmark) {
    int rpos;

    yc->jishu_kEndp = yc->cmark;
    yc->cmark = yc->kCurs;
    yc->kRStartp = yc->kCurs = yc->jishu_kEndp;
    kPos2rPos(yc, 0, yc->kCurs, 0, &rpos);
    yc->jishu_rEndp = yc->rStartp = yc->rCurs = rpos;
  }
  else {
    yc->jishu_kEndp = yc->kCurs;
    yc->jishu_rEndp = yc->rCurs;
  }
/*  yc->majorMode = d->majorMode; */
  kPos2rPos(yc, 0, (int)yc->cmark, 0, &pos);
  yc->rmark = (short)pos;
  d->current_mode = yc->curMode = &jishu_mode;
}

void
leaveJishuMode(d, yc)
uiContext d;
yomiContext yc;
{
  extern KanjiModeRec yomi_mode, cy_mode;

  yc->jishu_kEndp = 0;
  if (yc->generalFlags & CANNA_YOMI_CHIKUJI_MODE) {
    d->current_mode = yc->curMode = &cy_mode;
  }
  else {
    d->current_mode = yc->curMode = &yomi_mode;
  }
  yc->minorMode = getBaseMode(yc);
  currentModeInfo(d);
}

static
setInhibitInformation(yc)
yomiContext yc;
{
  int i;
  extern InhibitHankakuKana;

  yc->inhibition = InhibitHankakuKana ? INHIBIT_HANKATA : 0;
  for (i = 0 ; i < yc->kEndp ; i++) {
    if ( !(yc->kAttr[i] & HENKANSUMI) && WIsG0(yc->kana_buffer[i]) ) {
      yc->inhibition |= INHIBIT_KANA;
      break;
    }
  }
  for (i = 0 ; i < yc->rEndp ; i++) {
    if (!WIsG0(yc->romaji_buffer[i])) {
      yc->inhibition |= INHIBIT_ALPHA;
    }
  }
}

extractJishuString(yc, s, e, sr, er)
yomiContext yc;
wchar_t *s, *e, **sr, **er;
{
  wchar_t xxxx[1024], yyyy[1024];
  wchar_t *ss = s;
  int jishulen, len, revlen;

  if (s + yc->cmark - yc->cStartp < e) {
    WStrncpy(s, yc->kana_buffer + yc->cStartp, yc->cmark - yc->cStartp);
    s += yc->cmark - yc->cStartp;
  }
  else {
    WStrncpy(s, yc->kana_buffer + yc->cStartp, e - s);
    s = e;
  }

  if ((yc->jishu_kc == JISHU_ZEN_KATA ||
       yc->jishu_kc == JISHU_HAN_KATA ||
       yc->jishu_kc == JISHU_HIRA)) {
    int i, j, m, n, t, r;
    wchar_t *p = yyyy;
    for (i = yc->cmark ; i < yc->jishu_kEndp ;) {
      if (yc->kAttr[i] & STAYROMAJI) {
	j = i++;
	while (i < yc->jishu_kEndp && (yc->kAttr[i] & STAYROMAJI)) {
	  i++;
	}
	t = r = 0;
	while (j < i) {
	  int s = t;
	  WStrncpy(xxxx + t, yc->kana_buffer + j, i - j);
	  RkwMapPhonogram(yc->romdic, p, 1024 - (p - yyyy), 
			  xxxx, s + i - j, xxxx[0],
			  RK_FLUSH | RK_SOKON, &n, &m, &t, &r);
	  /* RK_SOKON ���դ���Τϵ켭���� */
	  p += m;
	  j += n - s;
	  WStrncpy(xxxx, p, t);
	}
      }
      else {
	*p++ = yc->kana_buffer[i++];
      }
    }
    jishulen = p - yyyy;
  }

  switch (yc->jishu_kc)
    {
    case JISHU_ZEN_KATA: /* ���ѥ������ʤ��Ѵ����� */
      len = RkwCvtZen(xxxx, 1024, yyyy, jishulen);
      revlen = RkwCvtKana(s, e - s, xxxx, len);
      break;

    case JISHU_HAN_KATA: /* Ⱦ�ѥ������ʤ��Ѵ����� */
      len = RkwCvtKana(xxxx, 1024, yyyy, jishulen);
      revlen = RkwCvtHan(s, e - s, xxxx, len);
      break;

    case JISHU_HIRA: /* �Ҥ餬�ʤ��Ѵ����� */
      len = RkwCvtZen(xxxx, 1024, yyyy, jishulen);
      revlen = RkwCvtHira(s, e - s, xxxx, len);
      break;

    case JISHU_ZEN_ALPHA: /* ���ѱѿ����Ѵ����� */
      if (yc->jishu_case == CANNA_JISHU_UPPER) {
	int i;
	wchar_t *p = yc->romaji_buffer;

	for (i = yc->rmark ; i < yc->jishu_rEndp ; i++)
	  xxxx[i - yc->rmark] = WToupper(p[i]);
	xxxx[yc->jishu_rEndp - yc->rmark] = 0;
	revlen = RkwCvtZen(s, e - s, xxxx, yc->jishu_rEndp - yc->rmark);
      } else if (yc->jishu_case == CANNA_JISHU_CAPITALIZE) {
	WStrncpy(xxxx, yc->romaji_buffer + yc->rmark,
		 yc->jishu_rEndp - yc->rmark);
	*xxxx = WToupper(*xxxx);
	xxxx[yc->jishu_rEndp - yc->rmark] = 0;
	revlen = RkwCvtZen(s, e - s, xxxx, yc->jishu_rEndp - yc->rmark);
      } else {
	revlen = RkwCvtZen(s, e - s, yc->romaji_buffer + yc->rmark,
			   yc->jishu_rEndp - yc->rmark);
      }
      break;

    case JISHU_HAN_ALPHA: /* Ⱦ�ѱѿ����Ѵ����� */
      revlen = yc->jishu_rEndp - yc->rmark;
      if (yc->jishu_case == CANNA_JISHU_UPPER) {
	int i;
	wchar_t *p = yc->romaji_buffer + yc->rmark;

	for (i = 0 ; i < revlen && s < e ; i++) {
	  *s++ = WToupper(p[i]);
	}
	s -= revlen;
      } else if (yc->jishu_case == CANNA_JISHU_CAPITALIZE) {
	if (s + revlen < e) {
	  WStrncpy(s, yc->romaji_buffer + yc->rmark, revlen);
	}
	else {
	  WStrncpy(s, yc->romaji_buffer + yc->rmark, e - s);
	  revlen = e - s;
	}
	*s = WToupper(yc->romaji_buffer[yc->rmark]);
      }
      else if (s + revlen < e) {
	WStrncpy(s, yc->romaji_buffer + yc->rmark, revlen);
      }
      else {
	WStrncpy(s, yc->romaji_buffer + yc->rmark, e - s);
	revlen = e - s;
      }
      break;

    default:/* �ɤ�Ǥ�ʤ��ä����Ѵ�����ʤ��Τǲ��⤷�ʤ� */
      break;
    }

  *sr = s;
  s += revlen;
  *er = s;

/* ʸ�����Ѵ����ʤ���ʬ���դ��ä��� */
  switch (yc->jishu_kc)
    {
    case JISHU_HIRA: /* �Ҥ餬�ʤʤ� */
    case JISHU_ZEN_KATA: /* ���ѥ������ʤʤ� */
    case JISHU_HAN_KATA: /* Ⱦ�ѥ������ʤʤ� */
      /* ���ʥХåե�����ʸ�������Ф� */
      if (s + yc->kEndp - yc->jishu_kEndp < e) {
	WStrncpy(s, yc->kana_buffer + yc->jishu_kEndp, 
		 yc->kEndp - yc->jishu_kEndp);
	s += yc->kEndp - yc->jishu_kEndp;
      }
      else {
	WStrncpy(s, yc->kana_buffer + yc->jishu_kEndp, e - s);
	s = e;
      }
      break;

    case JISHU_ZEN_ALPHA: /* ���ѱѿ��ʤ� */
    case JISHU_HAN_ALPHA: /* Ⱦ�ѱѿ��ʤ� */
      len = RkwCvtRoma(romajidic, s, e - s,
		       yc->romaji_buffer + yc->jishu_rEndp,
		       yc->rEndp - yc->jishu_rEndp,
		       RK_FLUSH | RK_SOKON | RK_XFER);
      s += len;
      break;
    default:/* �ɤ�Ǥ�ʤ��ä��鲿�⤷�ʤ� */
      break;
    }

  if (s < e) {
    *s = (wchar_t)0;
  }
  return s - ss;
}

static
inhibittedJishu(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  return (((yc->jishu_kc == JISHU_ZEN_KATA ||
	    yc->jishu_kc == JISHU_HAN_KATA) &&
	   (yc->inhibition & INHIBIT_KANA)) ||
	  ((yc->jishu_kc == JISHU_ZEN_ALPHA ||
	    yc->jishu_kc == JISHU_HAN_ALPHA) &&
	   (yc->inhibition & INHIBIT_ALPHA)) ||
	  ((yc->jishu_kc == JISHU_HAN_KATA) && 
	   (yc->inhibition & INHIBIT_HANKATA))
	  );
}

static
void
nextJishu(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  do {
    yc->jishu_kc = (BYTE)(((int)yc->jishu_kc + 1) % MAX_JISHU);
  } while (inhibittedJishu(d));
}

static
void
previousJishu(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  do {
    yc->jishu_kc = (unsigned char)
      (((int)yc->jishu_kc + MAX_JISHU - 1) % MAX_JISHU);
  } while (inhibittedJishu(d));
}
	    

static
JishuNextJishu(d) /* ����⡼�ɤλ��˽���ʸ�����Ѵ��򤹤� */
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

/* ���Ф���ʸ������Ѵ����� */
  nextJishu(d);
  if (yc->jishu_kc == JISHU_HIRA) {
    if (yc->jishu_kEndp == yc->kEndp && yc->jishu_rEndp == yc->rEndp) {
      leaveJishuMode(d, yc);
    }
  }
  makeKanjiStatusReturn(d, yc);
  return 0;
}

static
JishuPreviousJishu(d) /* ����⡼�ɤλ��˵ղ��ʸ�����Ѵ��򤹤� */
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

/* ���Ф���ʸ������Ѵ����� */
  previousJishu(d);
  if (yc->jishu_kc == JISHU_HIRA) {
    if (yc->jishu_kEndp == yc->kEndp && yc->jishu_rEndp == yc->rEndp) {
      leaveJishuMode(d, yc);
    }
  }
  makeKanjiStatusReturn(d, yc);
  return 0;
}

static
JishuShrink(d) /* ʸ�����Ѵ��ΰ��̤�� */
uiContext d;
{  
  yomiContext yc = (yomiContext)d->modec;

  /* ��Υݥ��󥿤��᤹ */
  switch (yc->jishu_kc)
    {
    case JISHU_ZEN_ALPHA:
    case JISHU_HAN_ALPHA: /* ���ѱѿ�����Ⱦ�ѱѿ����ʤ� */
      myjishuAdjustRome(d);
      yc->jishu_rEndp--; /* ������޻��Хåե�����ǥå������᤹ */
      if (yc->rAttr[yc->jishu_rEndp] & SENTOU) {
	                       /* ���޻������Ѵ���Ƭ�ե饰�Хåե���
				* Ω�äƤ�����
			        */
	for (--yc->jishu_kEndp ; 
	     yc->jishu_kEndp > 0 && !(yc->kAttr[yc->jishu_kEndp] & SENTOU) ;) {
	  --yc->jishu_kEndp;
	}
	                       /* �����Ѵ������ե饰�Хåե�����Ƭ��
				* Ω�äƤ����ޤ�
				* ���狼�ʥХåե�����ǥå�����
				* �᤹
			        */
      }
      break;
    case JISHU_HIRA:
    case JISHU_ZEN_KATA:
    case JISHU_HAN_KATA: /* �Ҥ餬�ʤ����ѥ������ʤ�Ⱦ�ѥ������ʤʤ� */
      jishuAdjustRome(d);
      yc->jishu_kEndp--; /* ���狼�ʥХåե�����ǥå�����ʸ��ʬ�᤹ */
      if (yc->kAttr[yc->jishu_kEndp] & SENTOU) {
                               /* �����Ѵ������ե饰�Хåե���
				* Ω�äƤ�����
			        */
	for (--yc->jishu_rEndp ; 
	     yc->jishu_rEndp > 0 && !(yc->rAttr[yc->jishu_rEndp] & SENTOU) ;) {
	  --yc->jishu_rEndp;
	}
	                       /* ���޻������Ѵ���Ƭ�ե饰�Хåե���
				* Ω�äƤ����ޤ�
				* ������޻��Хåե�����ǥå�����
				* �᤹
			        */
      }
      break;
    }

  if(yc->jishu_rEndp <= yc->rmark) {/* �������������Хåե�����ǥå�����
				     * ����Ĺ�����᤹
				     */
    yc->jishu_kEndp = yc->kEndp;
    yc->jishu_rEndp = yc->rEndp;
  }
  makeKanjiStatusReturn(d, yc);
  return 0;
}

static
JishuNop(d)
uiContext d;
{
  /* currentModeInfo �ǥ⡼�ɾ���ɬ���֤�褦�˥��ߡ��Υ⡼�ɤ�����Ƥ��� */
  d->majorMode = d->minorMode = CANNA_MODE_AlphaMode;
  currentModeInfo(d);

  makeKanjiStatusReturn(d, (yomiContext)d->modec);
  return 0;
}

static
JishuExtend(d) /* ʸ�����Ѵ��ΰ�򿭤Ф� */
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  /* ��Υݥ��󥿤����䤹 */
  switch (yc->jishu_kc) {
    case JISHU_ZEN_ALPHA:
    case JISHU_HAN_ALPHA: /* ���ѱѿ�����Ⱦ�ѱѿ����ʤ� */
      myjishuAdjustRome(d);

      if(yc->jishu_rEndp >= yc->rEndp && yc->jishu_kEndp >= yc->kEndp ) {
                                    /* �������������Хåե�����ǥå�����
				     * ���������᤹
				     */
	yc->jishu_rEndp = yc->rmark;
	yc->jishu_kEndp = yc->cmark;
      }

      if (yc->rAttr[yc->jishu_rEndp] & SENTOU) {
	                       /* ���޻������Ѵ���Ƭ�ե饰�Хåե���
				* Ω�äƤ�����
			        */

	for (yc->jishu_kEndp++ ; 
	     yc->jishu_kEndp > 0 && !(yc->kAttr[yc->jishu_kEndp] & SENTOU) ;) {
	  yc->jishu_kEndp++;
	}
	                       /* �����Ѵ������ե饰�Хåե�����Ƭ��
				* Ω�äƤ����ޤ�
				* ���狼�ʥХåե�����ǥå��������䤹
			        */
      }
      yc->jishu_rEndp++; /* ������޻��Хåե�����ǥå��������䤹 */
      break;
    case JISHU_HIRA:
    case JISHU_ZEN_KATA:
    case JISHU_HAN_KATA: /* �Ҥ餬�ʤ����ѥ������ʤ�Ⱦ�ѥ������ʤʤ� */
      jishuAdjustRome(d);

      if(yc->jishu_rEndp >= yc->rEndp && yc->jishu_kEndp >= yc->kEndp ) {
                                    /* �������������Хåե�����ǥå�����
				     * ���������᤹
				     */
	yc->jishu_rEndp = yc->rmark;
	yc->jishu_kEndp = yc->cmark;
      }

      if (yc->kAttr[yc->jishu_kEndp] & SENTOU) {
                               /* �����Ѵ������ե饰�Хåե���
				* Ω�äƤ�����
			        */
	for (yc->jishu_rEndp++ ; 
	     yc->jishu_rEndp > 0 && !(yc->rAttr[yc->jishu_rEndp] & SENTOU) ;) {
	  yc->jishu_rEndp++;
	}
	                       /* ���޻������Ѵ���Ƭ�ե饰�Хåե���
				* Ω�äƤ����ޤ�
				* ������޻��Хåե�����ǥå��������䤹
			        */
      }
      yc->jishu_kEndp++; /* ���狼�ʥХåե�����ǥå�����ʸ��ʬ���䤹 */
      break;
    }
  makeKanjiStatusReturn(d, yc);
  return 0;
}

static
jishuAdjustRome(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  while (!(yc->rAttr[yc->jishu_rEndp] & SENTOU)) {
    ++yc->jishu_rEndp;
  }
}

static
myjishuAdjustRome(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  while (!(yc->kAttr[yc->jishu_kEndp] & SENTOU)
	 && !(yc->jishu_kEndp == yc->kEndp)) {
    ++yc->jishu_kEndp;
  }
}

static
JishuZenkaku(d) /* �����Ѵ� */
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

/* ���Ф���ʸ������Ѵ����� */
  switch(yc->jishu_kc)
    {
    case JISHU_HIRA: /* �Ҥ餬�ʤʤ鲿�⤷�ʤ� */
      break;
      
    case JISHU_HAN_ALPHA: /* Ⱦ�ѱѿ��ʤ����ѱѿ����Ѵ����� */
      yc->jishu_kc = JISHU_ZEN_ALPHA;
      break;
      
    case JISHU_ZEN_ALPHA: /* ���ѱѿ��ʤ鲿�⤷�ʤ� */
      break;
      
    case JISHU_HAN_KATA: /* Ⱦ�ѥ������ʤʤ����ѥ������ʤ��Ѵ����� */
      yc->jishu_kc = JISHU_ZEN_KATA;
      break;
      
    case JISHU_ZEN_KATA: /* ���ѥ������ʤʤ鲿�⤷�ʤ� */
      break;
      
    default: /* �ɤ�Ǥ�ʤ��ä����Ѵ�����ʤ��Τǲ��⤷�ʤ� */
      break;
    }

  makeKanjiStatusReturn(d, yc);
  return 0;
}

static
JishuHankaku(d) /* Ⱦ���Ѵ� */
     uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;
  extern InhibitHankakuKana;
  
  /* ���Ф���ʸ������Ѵ����� */
  switch(yc->jishu_kc)
    {
    case JISHU_HIRA: /* �Ҥ餬�ʤʤ�Ⱦ�ѥ������ʤ��Ѵ����� */
      yc->jishu_kc = JISHU_HAN_KATA;
      break;
      
    case JISHU_ZEN_KATA: /* ���ѥ������ʤʤ�Ⱦ�ѥ������ʤ��Ѵ����� */
      if (InhibitHankakuKana) {
	return NothingChangedWithBeep(d);
      }
      yc->jishu_kc = JISHU_HAN_KATA;
      break;
      
    case JISHU_HAN_KATA: /* Ⱦ�ѥ������ʤʤ鲿�⤷�ʤ� */
      break;
      
    case JISHU_ZEN_ALPHA: /* ���ѱѿ��ʤ�Ⱦ�ѱѿ����Ѵ����� */
      yc->jishu_kc = JISHU_HAN_ALPHA;
      break;
      
    case JISHU_HAN_ALPHA: /* Ⱦ�ѱѿ��ʤ鲿�⤷�ʤ� */
      break;
      
    default: /* �ɤ�Ǥ�ʤ��ä����Ѵ�����ʤ��Τǲ��⤷�ʤ� */
      break;
    }

  makeKanjiStatusReturn(d, yc);
  return 0;
}

static
exitJishuAndDoSomething(d, fnum)
uiContext d;
int fnum;
{
  exitJishu(d);
  d->more.todo = 1;
  d->more.ch = d->ch;
  d->more.fnum = fnum;
  makeYomiReturnStruct(d);
  currentModeInfo(d);
  return d->nbytes = 0;
}

static
JishuYomiInsert(d)
uiContext d;
{
  return exitJishuAndDoSomething(d, 0);
}

static
JishuQuit(d)
uiContext d;
{
  leaveJishuMode(d, (yomiContext)d->modec);
  makeKanjiStatusReturn(d, (yomiContext)d->modec);
  return 0;
}

/* ��ʸ���ˤ���ؿ� */

static
JishuToUpper(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  if (!(yc->inhibition & INHIBIT_ALPHA)) { /* ̵��������ʸ�����Ѵ����� */
    if (yc->jishu_kc == JISHU_HIRA || yc->jishu_kc == JISHU_ZEN_KATA) {
      yc->jishu_kc = JISHU_ZEN_ALPHA;
    }
    else if (yc->jishu_kc == JISHU_HAN_KATA) {
      yc->jishu_kc = JISHU_HAN_ALPHA;
    }
  }

  if (yc->jishu_kc == JISHU_ZEN_ALPHA || yc->jishu_kc == JISHU_HAN_ALPHA) {
    yc->jishu_case = CANNA_JISHU_UPPER;
    makeKanjiStatusReturn(d, yc);
    return 0;
  }
  else {
    /* ���Ȳ����Ѥ��ޤ��� */
    d->kanji_status_return->length = -1;
    return 0;
  }
}

static
JishuCapitalize(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  if (!(yc->inhibition & INHIBIT_ALPHA)) { /* ̵��������ʸ�����Ѵ����� */
    if (yc->jishu_kc == JISHU_HIRA || yc->jishu_kc == JISHU_ZEN_KATA) {
      yc->jishu_kc = JISHU_ZEN_ALPHA;
    }
    else if (yc->jishu_kc == JISHU_HAN_KATA) {
      yc->jishu_kc = JISHU_HAN_ALPHA;
    }
  }

  if (yc->jishu_kc == JISHU_ZEN_ALPHA || yc->jishu_kc == JISHU_HAN_ALPHA) {
    yc->jishu_case = CANNA_JISHU_CAPITALIZE;
    makeKanjiStatusReturn(d, yc);
    return 0;
  }
  else {
    /* ���Ȳ����Ѥ��ޤ��� */
    d->kanji_status_return->length = -1;
    return 0;
  }
}

static
JishuToLower(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  if (!(yc->inhibition & INHIBIT_ALPHA)) { /* ̵��������ʸ�����Ѵ����� */
    if (yc->jishu_kc == JISHU_HIRA || yc->jishu_kc == JISHU_ZEN_KATA) {
      yc->jishu_kc = JISHU_ZEN_ALPHA;
    }
    else if (yc->jishu_kc == JISHU_HAN_KATA) {
      yc->jishu_kc = JISHU_HAN_ALPHA;
    }
  }

  if (yc->jishu_kc == JISHU_ZEN_ALPHA || yc->jishu_kc == JISHU_HAN_ALPHA) {
    yc->jishu_case = 0;
    makeKanjiStatusReturn(d, yc);
    return 0;
  }
  else {
    /* ���Ȳ����Ѥ��ޤ��� */
    d->kanji_status_return->length = -1;
    return 0;
  }
}

static
JishuHiragana(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  yc->jishu_kc = JISHU_HIRA;
  makeKanjiStatusReturn(d, yc);
  return 0;
}

static
JishuKatakana(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  yc->jishu_kc = JISHU_ZEN_KATA;
  makeKanjiStatusReturn(d, yc);
  return 0;
}

static
JishuRomaji(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  if (yc->inhibition == INHIBIT_ALPHA) {
    return NothingChangedWithBeep(d);
  }
  yc->jishu_kc = JISHU_ZEN_ALPHA;
  makeKanjiStatusReturn(d, yc);
  return 0;
}

/*
 * ���ʴ����Ѵ���Ԥ�(�Ѵ����������Ʋ����줿)��TanKouhoMode�˰ܹԤ���
 *
 * ������	uiContext
 * �����	���ｪλ�� 0	�۾ｪλ�� -1
 */

static
JishuKanjiHenkan(d)
uiContext	d;
{
  return exitJishuAndDoSomething(d, CANNA_FN_Henkan);
}

static
JishuKanjiHenkanOInsert(d)
uiContext	d;
{
  return exitJishuAndDoSomething(d, CANNA_FN_HenkanOrInsert);
}

static
JishuKanjiHenkanONothing(d)
uiContext	d;
{
  return exitJishuAndDoSomething(d, CANNA_FN_HenkanOrNothing);
}

#include "jishumap.c"
