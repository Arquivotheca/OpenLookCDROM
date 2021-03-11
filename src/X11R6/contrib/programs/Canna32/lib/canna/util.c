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

/* filedef

  util.c -- �桼�ƥ���ƥ��ؿ��򽸤᤿��

  �ʲ��δؿ������롣(�ɲä����ͤϤ����Ƚ񤤤Ȥ��Ƥ�)

  GlineClear         �����ɥ饤�󤬾ä����褦�ʥ꥿�����ͤ���
  Gline2echostr      �����ɥ饤����֤����Ȥ�����Τ򤽤ξ���֤�
  echostrClear       ���ξ줬�����ä����褦�ʥ꥿�����ͤ���
  checkGLineLen      �����ɥ饤���ɽ��������뤫�ɤ����Υ����å�
  NothingChanged     �����Ѳ����ʤ����Ȥ򼨤��꥿�����ͤ���
  NothingForGLine    �����ɥ饤��˴ؤ��Ƥϲ����Ѳ����ʤ�
  NothingChangedWithBeep
                     NothingChange �򤷤Ƥ���˥ӡ��ײ����Ĥ餹
  NothingForGLineWithBeep
                     NothingForGLine �򤷤Ƥ���˥ӡ��ײ����Ĥ餹
  Beep               �ӡ��ײ���ʤ餹��
  makeGLineMessage   ������ʸ�����GLine��ɽ������褦�ʥ꥿�����ͤ���
  makeGLineMessageFromString
  		     ������eucʸ�����GLine��ɽ������褦�ʥ꥿�����ͤ���
  setWStrings	     ʸ������ν������Ԥ�
  NoMoreMemory       ���꤬�ʤ����饨�顼����Ȥ������顼�ͤ��֤�
  GLineNGReturn      ���顼��å������򥬥��ɥ饤��˰ܤ�
  GLineNGReturnFI    �����⡼�ɤ�ȴ���� GLineNGReturn �򤹤롣
  GLineNGReturnTK    ��Ͽ�⡼�ɤ�ȴ���� GLineNGReturn �򤹤롣
  WStrlen            �磻�ɥ���饯��ʸ�����Ĺ������� (cf. strlen)
  WStrcat            �磻�ɥ���饯��ʸ�����ä��롣(cf. strcat)
  WStrcpy            �磻�ɥ���饯��ʸ����򥳥ԡ����롣(cf. strcpy)
  WStrncpy           �磻�ɥ���饯��ʸ������ʸ�����ԡ����롣(cf. strncpy)
  WStraddbcpy        �磻�ɥ���饯��ʸ��������ʸ�������֡��Хå�����å���
                     �����˥Хå�����å��������ʤ��饳�ԡ����롣
  WStrcmp	     �磻�ɥ���饯��ʸ�������Ӥ��롣(cf. strcmp)
  WStrncmp	     �磻�ɥ���饯��ʸ������ʸ����Ӥ��롣(cf. strncmp)
  WWhatGPlain	     �磻�ɥ���饯����ʸ����°���륰��ե��å��ץ졼����֤�
  WIsG0              G0�Υ磻�ɥ���饯��ʸ������
  WIsG1              G1�Υ磻�ɥ���饯��ʸ������
  WIsG2              G2�Υ磻�ɥ���饯��ʸ������
  WIsG3              G3�Υ磻�ɥ���饯��ʸ������
  CANNA_mbstowcs     EUC ��磻�ɥ���饯��ʸ������Ѵ�
  CNvW2E             �磻�ɥ���饯���� EUC ���Ѵ�(�����å��դ�)
  CANNA_wcstombs     �磻�ɥ���饯���� EUC ���Ѵ�
  WSfree	     WString�ǳ��ݤ����ΰ��������
  WString            EUC ��磻�ɤ��Ѵ����� malloc �ޤǤ����֤�(free ����)
  WStringOpen        �嵭�ؿ��ν��������
  WStringClose       �嵭�ؿ��ν�λ����
  WToupper           ������ʸ������ʸ���ˤ���
  key2wchar          �����ܡ������Ϥ�磻�ɥ���饯���ˤ��롣
  US2WS              Ushort �� wchar_t ���Ѵ����롣
  WS2US              wchar_t �� Ushort ���Ѵ����롣
  confirmContext     yc->context ���Ȥ����Τ���ǧ���� 
  makeRkError        Rk �δؿ��ǥ��顼���Ǥ��Ȥ��ν����򤹤롣

 */

#if !defined(lint) && !defined(__CODECENTER__)
static char rcs_id[] = "@(#) 102.1 $Id: util.c,v 5.18 1994/05/10 07:49:26 kon Exp $";
#endif	/* lint */

#include "canna.h"
#include <errno.h>

#ifdef luna88k
extern int errno;
#endif

#ifdef HAVE_WCHAR_OPERATION
#include <locale.h>
#if __STDC__ || defined(SVR4)
#include <limits.h>
#endif
#endif

#ifndef MB_LEN_MAX
#define MB_LEN_MAX 5 /* 5 ��Ԥ��ʤ������Ȥϻפ� */
#endif

/* arraydef

  tmpbuf -- ����äȲ��˻Ȥ���Хåե�

 */

/*
 * Gline �򥯥ꥢ����
 *
 * ������	uiContext
 * �����	�ʤ�
 */
GlineClear(d)
uiContext d;
{
  d->kanji_status_return->info |= KanjiGLineInfo;
  d->kanji_status_return->gline.line = (wchar_t *)NULL;
  d->kanji_status_return->gline.length = 0;
  d->kanji_status_return->gline.revPos = 0;
  d->kanji_status_return->gline.revLen = 0;
}

/* cfuncdef

  Gline2echostr -- �����Ԥ����Ƥ򤽤ξ�˰�ư

 */

static
Gline2echostr(d)
uiContext d;
{
  d->kanji_status_return->echoStr =
    d->kanji_status_return->gline.line;
  d->kanji_status_return->length =
    d->kanji_status_return->gline.length;
  d->kanji_status_return->revPos =
    d->kanji_status_return->gline.revPos;
  d->kanji_status_return->revLen =
    d->kanji_status_return->gline.revLen;
  GlineClear(d);
}

echostrClear(d)
uiContext d;
{
  d->kanji_status_return->echoStr = (wchar_t *)NULL;
  d->kanji_status_return->length =
    d->kanji_status_return->revPos = d->kanji_status_return->revLen = 0;
}

/* 
 * ʸ���󤫤饳��������ü����ؿ�
 */

static
colwidth(s, len)
wchar_t *s;
int     len;
{
  int ret = 0;
  wchar_t *es = s + len;

  for (; s < es ; s++) {
    switch (WWhatGPlain(*s)) {
    case 0:
    case 2:
      ret ++;
      break;
    case 1:
    case 3:
      ret += 2;
      break;
    }
  }
  return ret;
}


/* cfuncdef

  checkGLineLen -- �����Ԥ�ɽ���Ǥ���Ĺ����ۤ��Ƥ��뤫������å�

  Ĺ�����ۤ��Ƥ����顢����������ʬ��ɽ�������褦�ˤ��롣

 */

checkGLineLen(d)
uiContext d;
{
  if (d->kanji_status_return->info & KanjiGLineInfo) {
    if (colwidth(d->kanji_status_return->gline.line,
		 d->kanji_status_return->gline.length) > d->ncolumns) {
      Gline2echostr(d);
    }
  }
}

/* cfuncdef

  NothingChanged -- �ɤߤˤĤ��Ƥϲ����Ѥ��ʤ��褦�ˤ���

 */

NothingChanged(d)
uiContext d;
{
  d->kanji_status_return->length = -1; /* �Ѥ��ʤ��� */
  d->kanji_status_return->revPos 
    = d->kanji_status_return->revLen = 0;
  d->kanji_status_return->info = 0;
  return 0;
}

NothingForGLine(d)
uiContext d;
{
  d->kanji_status_return->length = -1; /* �Ѥ��ʤ��� */
  d->kanji_status_return->revPos 
    = d->kanji_status_return->revLen = 0;
  return 0;
}

void
Beep()
{
  extern int (*jrBeepFunc) pro((void));

  if (jrBeepFunc) {
    jrBeepFunc();
  }
}

NothingChangedWithBeep(d)
uiContext d;
{
  Beep();
  return NothingChanged(d);
}

NothingForGLineWithBeep(d)
uiContext d;
{
  Beep();
  return NothingForGLine(d);
}

#ifdef SOMEONE_USE_THIS
/* ï��ȤäƤ��ʤ��ߤ����� */
Insertable(ch)
unsigned char ch;
{
  if ((0x20 <= ch && ch <= 0x7f) || (0xa0 <= ch && ch <= 0xff)) {
    return 1;
  }
  else {
    return 0;
  }
}
#endif /* SOMEONE_USE_THIS */

/*
  extractSimpleYomiString -- yomiContext ���ɤ���ʬ��������Ф�

  ����
     yc  -- yomiContext
     s   -- ���Ф���Υ��ɥ쥹
     e   -- ������ۤ��Ƽ��Ф��ƤϤʤ�ʤ����ȸ������ɥ쥹
     sr  -- ȿž�ΰ�γ��ϰ��֤��֤����ɥ쥹
     er  -- ȿž�ΰ�ν�λ���֤��֤����ɥ쥹

 */

static int
extractSimpleYomiString(yc, s, e, sr, er)
yomiContext yc;
wchar_t *s, *e, **sr, **er;
{
  extern int ReverseWidely;
  int len = yc->kEndp - yc->cStartp;

  if (yc->jishu_kEndp) {
    return extractJishuString(yc, s, e, sr, er);
  }

  if (s + len < e) {
    WStrncpy(s, yc->kana_buffer + yc->cStartp, len);
  }
  else {
    WStrncpy(s, yc->kana_buffer + yc->cStartp, e - s);
    len = e - s;
  }
  if (ReverseWidely) {
    *sr = s;
    *er = s + yc->kCurs - yc->cStartp;
  }
  else if (yc->kCurs == yc->kEndp && !yc->right) {
    *sr = *er = s + yc->kCurs - yc->cStartp;
  }
  else {
    *sr = s + yc->kCurs - yc->cStartp;
    *er = *sr + 1;
  }
  return len;
}

/*
  extractKanjiString -- yomiContext �δ����������Ф�

  ����
     yc  -- yomiContext
     s   -- ���Ф���Υ��ɥ쥹
     e   -- ������ۤ��Ƽ��Ф��ƤϤʤ�ʤ����ȸ������ɥ쥹
     b   -- ʸ����ڤ�򤹤뤫�ɤ���
     sr  -- ȿž�ΰ�γ��ϰ��֤��֤����ɥ쥹
     er  -- ȿž�ΰ�ν�λ���֤��֤����ɥ쥹
 */

static int
extractKanjiString(yc, s, e, b, sr, er)
yomiContext yc;
wchar_t *s, *e, **sr, **er;
int b;
{
  wchar_t *ss = s;
  int i, len, nbun;

  nbun = yc->bunlen ? yc->curbun : yc->nbunsetsu;

  for (i = 0 ; i < nbun ; i++) {
    if (i && b && s < e) {
      *s++ = (wchar_t)' ';
    }
    RkwGoTo(yc->context, i);
    len = RkwGetKanji(yc->context, s, e - s);
    if (len < 0) {
      if (errno == EPIPE) {
	jrKanjiPipeError();
      }
      jrKanjiError = "�����ȸ������Ф��ޤ���Ǥ���";
    }
    else {
      if (i == yc->curbun) {
	*sr = s; *er = s + len;
      }
      s += len;
    }
  }

  if (yc->bunlen) {
    if (i && b && s < e) {
      *s++ = (wchar_t)' ';
    }
    len = yc->kEndp - yc->kanjilen;
    if (e - s < len) {
      len = e - s;
    }
    WStrncpy(s, yc->kana_buffer + yc->kanjilen, len);
    if (b) {
      *er = (*sr = s + yc->bunlen) +
	((yc->kanjilen + yc->bunlen == yc->kEndp) ? 0 : 1);
    }
    else {
      *sr = s; *er = s + yc->bunlen;
    }
    s += len;
  }

  if (s < e) {
    *s = (wchar_t)'\0';
  }

  RkwGoTo(yc->context, yc->curbun);
  return s - ss;
}

/*
  extractYomiString -- yomiContext ��ʸ������Ф�

  ����
     yc  -- yomiContext
     s   -- ���Ф���Υ��ɥ쥹
     e   -- ������ۤ��Ƽ��Ф��ƤϤʤ�ʤ����ȸ������ɥ쥹
     b   -- ʸ����ڤ�򤹤뤫�ɤ���
     sr  -- ȿž�ΰ�γ��ϰ��֤��֤����ɥ쥹
     er  -- ȿž�ΰ�ν�λ���֤��֤����ɥ쥹
 */

static int
extractYomiString(yc, s, e, b, sr, er)
yomiContext yc;
wchar_t *s, *e, **sr, **er;
int b;
{
  int autoconvert = yc->generalFlags & CANNA_YOMI_CHIKUJI_MODE, len;
  wchar_t *ss = s;
  extern int ReverseWidely;

  if (autoconvert) {
    len = extractKanjiString(yc, s, e, b, sr, er);
    s += len;
    if (yc->kEndp - yc->cStartp > 0) {
      wchar_t *ssr, *eer;

      if (b && len && s < e) {
	*s++ = (wchar_t)' ';
      }
      len = extractSimpleYomiString(yc, s, e, &ssr, &eer);
      s += len;
      if (!(yc->status & CHIKUJI_ON_BUNSETSU) &&
	  (!yc->nbunsetsu || (yc->status & CHIKUJI_OVERWRAP))) {
	*sr = ssr; *er = eer;
      }
    }
  }
  else if (yc->nbunsetsu) { /* ñ����⡼�� */
    len = extractKanjiString(yc, s, e, b, sr, er);
    s += len;
  }
  else {
    len = extractSimpleYomiString(yc, s, e, sr, er);
    s += len;
  }
  if (s < e) {
    *s = (wchar_t)'\0';
  }
  return s - ss;
}

static
extractString(str, s, e)
wchar_t *str, *s, *e;
{
  int len;

  len = WStrlen(str);
  if (s + len < e) {
    WStrcpy(s, str);
    return len;
  }
  else {
    WStrncpy(s, str, e - s);
    return e - s;
  }
}

/*
  extractTanString -- tanContext ��ʸ������Ф�

  ����
     tan -- tanContext
     s   -- ���Ф���Υ��ɥ쥹
     e   -- ������ۤ��Ƽ��Ф��ƤϤʤ�ʤ����ȸ������ɥ쥹
 */

int
extractTanString(tan, s, e)
tanContext tan;
wchar_t *s, *e;
{
  return extractString(tan->kanji, s, e);
}

/*
  extractTanYomi -- tanContext ��ʸ������Ф�

  ����
     tan -- tanContext
     s   -- ���Ф���Υ��ɥ쥹
     e   -- ������ۤ��Ƽ��Ф��ƤϤʤ�ʤ����ȸ������ɥ쥹
 */

int
extractTanYomi(tan, s, e)
tanContext tan;
wchar_t *s, *e;
{
  return extractString(tan->yomi, s, e);
}

/*
  extractTanRomaji -- tanContext ��ʸ������Ф�

  ����
     tan -- tanContext
     s   -- ���Ф���Υ��ɥ쥹
     e   -- ������ۤ��Ƽ��Ф��ƤϤʤ�ʤ����ȸ������ɥ쥹
 */

int
extractTanRomaji(tan, s, e)
tanContext tan;
wchar_t *s, *e;
{
  return extractString(tan->roma, s, e);
}

void
makeKanjiStatusReturn(d, yc)
uiContext d;
yomiContext yc;
{
  extern int BunsetsuKugiri, ReverseWidely;
  int len;
  wchar_t *s = d->genbuf, *e = s + ROMEBUFSIZE, *sr, *er, *sk, *ek;
  tanContext tan = (tanContext)yc;

  /* �ǽ���Ѵ�����Ƥ�����ʬ����Ф� */
  while (tan->left) {
    tan = tan->left;
  }

  while (tan) {
    switch (tan->id) {
    case TAN_CONTEXT:
      len = extractTanString(tan, s, e);
      sk = s; ek = s + len;
      break;
    case YOMI_CONTEXT:
      len = extractYomiString((yomiContext)tan, s, e,
			      BunsetsuKugiri, &sk, &ek);
      break;
    default:
      break;
    }

    if ((mode_context)tan == (mode_context)yc) {
      sr = sk;
      er = ek;
    }
    s += len;
    tan = tan->right;
    if (BunsetsuKugiri && tan && s < e) {
      *s++ = (wchar_t)' ';
    }
  }
  
  if (s < e) {
    *s = (wchar_t)'\0';
  }

  d->kanji_status_return->length = s - d->genbuf;;
  d->kanji_status_return->echoStr = d->genbuf;
  d->kanji_status_return->revPos = sr - d->genbuf;
  d->kanji_status_return->revLen = er - sr;
}

#define MESSBUFSIZE 256

/*
 * ��С����ʤ��Υ�å������򥬥��ɥ饤���ɽ������
 * �������Ϥ����ä��Ȥ��˾ä���褦�˥ե饰�����ꤹ��
 */
void
makeGLineMessage(d, msg, sz)
uiContext d;
wchar_t *msg;
int sz;
{
  static wchar_t messbuf[MESSBUFSIZE];
  int len = sz < MESSBUFSIZE ? sz : MESSBUFSIZE - 1;

  WStrncpy(messbuf, msg, len);
  messbuf[len] = (wchar_t)0;
  d->kanji_status_return->gline.line = messbuf;
  d->kanji_status_return->gline.length = len;
  d->kanji_status_return->gline.revPos = 0;
  d->kanji_status_return->gline.revLen = 0;
  d->kanji_status_return->info |= KanjiGLineInfo;

  d->flags &= ~PCG_RECOGNIZED;
  d->flags |= PLEASE_CLEAR_GLINE;
  checkGLineLen(d);
}

void
makeGLineMessageFromString(d, msg)
uiContext d;
char  *msg;
{
  int len;

  len = MBstowcs(d->genbuf, msg, ROMEBUFSIZE);
  makeGLineMessage(d, d->genbuf, len);
}

setWStrings(ws, s, sz)
wchar_t **ws;
char **s;
int sz;
{
  int f = sz;
  wchar_t *WString();

  for (; (f && sz) || (!f && *s); ws++, s++, sz--)
    *ws = WString(*s);
  return(0);
}

#ifdef DEBUG
dbg_msg(fmt, x, y, z)
char *fmt;
int x, y, z;
{
  if (iroha_debug) {
    fprintf(stderr, fmt, x, y, z);
  }
}

checkModec(d)
uiContext d;
{
  coreContext c;
  struct callback *cb;
  int depth = 0, cbDepth = 0;
  int callbacks = 0;

  for (c = (coreContext)d->modec ; c ; c = (coreContext)c->next)
    depth++;
  for (cb = d->cb ; cb ; cb = cb->next) {
    int i;

    cbDepth++;
    for (i = 0 ; i < 4 ; i++) {
      callbacks <<= 1;
      if (cb->func[i]) {
	callbacks++;
      }
    }
  }
  if (depth != cbDepth) {
    fprintf(stderr, "�����������������������㤦������������������\n");
  }
  debug_message("�� ����: d->modec:%d d->cb:%d callbacks:0x%08x ", 
		depth, cbDepth, callbacks);
  debug_message("EXIT_CALLBACK = 0x%x\n", d->cb->func[EXIT_CALLBACK],0,0);
  {
    extern KanjiModeRec yomi_mode;
    if (d->current_mode == &yomi_mode) {
      yomiContext yc = (yomiContext)d->modec;
      if (yc->kana_buffer[yc->kEndp]) {
	fprintf(stderr, "���������� ���ʥХåե��˥��ߤ����äƤ��뤾��\n");
      }
    }
  }
}

static char pbufstr[] = " o|do?b%";

showRomeStruct(dpy, win)
unsigned int dpy, win;
{
  char buf[1024];
  uiContext d, keyToContext();
  extern defaultContext;
  static int n = 0;
  int i;
  
  n++;
  fprintf(stderr, "\n�ڥǥХ���å�����(%d)��\n", n);
  d = keyToContext((unsigned int)dpy, (unsigned int)win);
  fprintf(stderr, "buffer(0x%x), bytes(%d)\n",
	  d->buffer_return, d->n_buffer);
  fprintf(stderr, "nbytes(%d), ch(0x%x)\n", d->nbytes, d->ch);
  fprintf(stderr, "�⡼��: %d\n", ((coreContext)d->modec)->minorMode);

  /* ����ƥ����� */
  fprintf(stderr, "����ƥ�����(%d)\n", d->contextCache);
  fprintf(stderr, "�ǥե���ȥ���ƥ�����(%d)\n", defaultContext);

  /* ���޻����ʴ�Ϣ */
  if (((coreContext)d->modec)->id == YOMI_CONTEXT) {
    yomiContext yc = (yomiContext)d->modec;

    fprintf(stderr, "r:       Start(%d), Cursor(%d), End(%d)\n",
	    yc->rStartp, yc->rCurs, yc->rEndp);
    fprintf(stderr, "k: ̤�Ѵ�Start(%d), Cursor(%d), End(%d)\n",
	    yc->kRStartp, yc->kCurs, yc->kEndp);
    WStrncpy(buf, yc->romaji_buffer, yc->rEndp);
    buf[yc->rEndp] = '\0';
    fprintf(stderr, "romaji_buffer(%s)\n", buf);
    fprintf(stderr, "romaji_attrib(");
    for (i = 0 ; i <= yc->rEndp ; i++) {
      fprintf(stderr, "%1x", yc->rAttr[i]);
    }
    fprintf(stderr, ")\n");
    fprintf(stderr, "romaji_pointr(");
    for (i = 0 ; i <= yc->rEndp ; i++) {
      int n = 0;
      if (i == yc->rStartp)
	n |= 1;
      if (i == yc->rCurs)
	n |= 2;
      if (i == yc->rEndp)
	n |= 4;
      fprintf(stderr, "%c", pbufstr[n]);
    }
    fprintf(stderr, ")\n");
    WStrncpy(buf, yc->kana_buffer, yc->kEndp);
    buf[yc->kEndp] = '\0';
    fprintf(stderr, "kana_buffer(%s)\n", buf);
    fprintf(stderr, "kana_attrib(");
    for (i = 0 ; i <= yc->kEndp ; i++) {
      fprintf(stderr, "%1x", yc->kAttr[i]);
    }
    fprintf(stderr, ")\n");
    fprintf(stderr, "kana_pointr(");
    for (i = 0 ; i <= yc->kEndp ; i++) {
      int n = 0;
      if (i == yc->kRStartp)
	n |= 1;
      if (i == yc->kCurs)
	n |= 2;
      if (i == yc->kEndp)
	n |= 4;
      fprintf(stderr, "%c", pbufstr[n]);
    }
    fprintf(stderr, ")\n");
    fprintf(stderr, "\n");
  }
/*  RkPrintDic(0, "kon"); */
}
#endif /* DEBUG */

extern char *jrKanjiError;

NoMoreMemory()
{
  jrKanjiError = "���꤬��­���Ƥ��ޤ���";
  return NG;
}

GLineNGReturn(d)
uiContext d;
{
  int len;
  len = MBstowcs(d->genbuf, jrKanjiError, ROMEBUFSIZE);
  makeGLineMessage(d, d->genbuf, len);
  currentModeInfo(d);

  return(0);
}

GLineNGReturnFI(d)
uiContext d;
{
  popForIchiranMode(d);
  popCallback(d);
  GLineNGReturn(d);
  return(0);
}

GLineNGReturnTK(d)
uiContext d;
{
  popTourokuMode(d);
  popCallback(d);
  GLineNGReturn(d);
  return(0);
}

#ifdef USE_COPY_ATTRIBUTE
copyAttribute(dest, src, n)
     BYTE	*dest;
     BYTE	*src;
     int	n;
{
  if (dest > src && dest < src + n) {
    dest += n;
    src += n;
    while (n-- > 0) {
      *--dest = *--src;
    }
  }
  else {
    while (n-- > 0) {
      *dest++ = *src++;
    }
  }
}
#endif

#ifdef DEBUG_ALLOC
int fail_malloc = 0;

#undef malloc

char *
debug_malloc(n)
int n;
{
  if (fail_malloc)
    return (char *)0;
  else
    return malloc(n);
}
#endif /* DEBUG_ALLOC */

/*
 * �磻�ɥ���饯�����ڥ졼�����
 *
 */

static int wchar_type; /* �磻�ɥ���饯���Υ�����(���򸫤�) */

#define CANNA_WCTYPE_16 0  /* 16�ӥå�ɽ�� */
#define CANNA_WCTYPE_32 1  /* 32�ӥå�ɽ�� */
#define CANNA_WCTYPE_OT 99 /* ����¾��ɽ�� */

/*
 WCinit() -- �磻�ɥ���饯���Ȥ��Ƥɤ줬�Ȥ��Ƥ��뤫���ǧ����

        ���δؿ����ƤӽФ����ޤ��� setlocale ���ʤ���Ƥ��ʤ���Фʤ�ʤ�
 */

#define TYPE16A 0x0000a4a2
#define TYPE32A 0x30001222

int
WCinit()
{
#ifdef HAVE_WCHAR_OPERATION
  extern int locale_insufficient;
  wchar_t wc[24];
  char *a = "��"; /* 0xa4a2 */

  locale_insufficient = 0;
  if (mbstowcs(wc, a, sizeof(wc) / sizeof(wchar_t)) != 1) {
    /* ¿ʬ setlocale ���ʤ���Ƥ��ʤ� */
    setlocale(LC_CTYPE, "");
    if (mbstowcs(wc, a, sizeof(wc) / sizeof(wchar_t)) != 1) {
      setlocale(LC_CTYPE, JAPANESE_LOCALE);
      if (mbstowcs(wc, a, sizeof(wc) / sizeof(wchar_t)) != 1) {
	locale_insufficient = 1;
	return -1;
      }
    }
  }
  switch (wc[0]) {
  case TYPE16A:
    wchar_type = CANNA_WCTYPE_16;
    break;
  case TYPE32A:
    wchar_type = CANNA_WCTYPE_32;
    break;
  default:
    wchar_type = CANNA_WCTYPE_OT;
    break;
  }
#else /* !HAVE_WCHAR_OPERATION */
# ifdef WCHAR16

  wchar_type = CANNA_WCTYPE_16;

# else /* !WCHAR16 */

  if (sizeof(wchar_t) == 2) {
    /* NOTREACHED */
    wchar_type = CANNA_WCTYPE_16;
  }
  else {
    /* NOTREACHED */
    wchar_type = CANNA_WCTYPE_32;
  }

# endif /* !WCHAR16 */
#endif /* !HAVE_WCHAR_OPERATION */

  return 0;
}


int
WStrlen(ws)
wchar_t *ws;
{
  int res = 0;
  while (*ws++) {
    res++;
  }
  return res;
}

wchar_t *
WStrcpy(ws1, ws2)
wchar_t *ws1, *ws2;
{
  wchar_t *ws;
  int cnt, len;

  ws = ws2;
  while (*ws) {
    ws++;
  }
  len = cnt = ws - ws2;
  if (ws2 < ws1 && ws1 < ws /* == ws2 + cnt */) {
    while (cnt--) {
      ws1[cnt] = ws2[cnt];
    }
  }
  else {
    ws = ws1;
    while (*ws2) {
      *ws++ = *ws2++;
    }
  }
  ws1[len] = (wchar_t)0;
  return ws1;
}

wchar_t *
WStrncpy(ws1, ws2, cnt)
wchar_t *ws1, *ws2;
int cnt;
{
  wchar_t *ws;

  if  (ws2 == (wchar_t *)0)
    return((wchar_t *)0);
  if (ws2 < ws1 && ws1 < ws2 + cnt) {
    while (cnt--) {
      ws1[cnt] = ws2[cnt];
    }
  }
  else {
    int i = 0;
    ws = ws1;
    while (i++ < cnt && *ws2) {
      *ws++ = *ws2++;
    }
  }
  return ws1;
}

wchar_t *
WStraddbcpy(ws1, ws2, cnt)
wchar_t	*ws1, *ws2;
int cnt;
{
  wchar_t *strp = ws1, *endp = ws1 + cnt - 1;

  while (*ws2 != (wchar_t)'\0' && ws1 < endp) {
    if (*ws2 == (wchar_t)' ' || *ws2 == (wchar_t)'\t' || *ws2 == (wchar_t)'\\')
      *ws1++ = (wchar_t)'\\';
    *ws1++ = *ws2++;
  }
  if (ws1 == endp) {
    ws1--;
  }
  *ws1 = (wchar_t)'\0';
  return(strp);
}

wchar_t *
WStrcat(ws1, ws2)
wchar_t *ws1, *ws2;
{
  wchar_t *ws;

  ws = ws1;
  while (*ws) {
    ws++;
  }
  WStrcpy(ws, ws2);
  return ws1;
}

int
WStrcmp(w1, w2)
wchar_t *w1, *w2;
{
  while (*w1 && *w1 == *w2) {
    w1++;
    w2++;
  }
  return(*w1 - *w2);
}

int
WStrncmp(w1, w2, n)
wchar_t *w1, *w2;
int n;
{
  if (n == 0) return(0);
  while (--n && *w1 && *w1 == *w2) {
    w1++;
    w2++;
  }
  return *w1 - *w2;
}

/* WWhatGPlain -- �ɤΥ���ե��å��ץ졼���ʸ������

   �����:
     0 : G0 ASCII
     1 : G1 ����(JISX0208)
     2 : G2 Ⱦ�ѥ�������(JISX0201)
     3 : G3 ����(������� JISX0212)
 */

int
WWhatGPlain(wc)
wchar_t wc;
{
  static char plain[4] = {0, 2, 3, 1};

  switch (wchar_type) {
  case CANNA_WCTYPE_16:
    switch (((unsigned long)wc) & 0x8080) {
    case 0x0000:
      return 0;
    case 0x8080:
      return 1;
    case 0x0080:
      return 2;
    case 0x8000:
      return 3;
    }
    break;
  case CANNA_WCTYPE_32:
    return plain[(((unsigned long)wc) >> 28) & 3];
  default:
    return 0; /* �ɤ����褦 */
  }
  /* NOTREACHED */
}

int
WIsG0(wc)
wchar_t wc;
{
  return (WWhatGPlain(wc) == 0);
}

int
WIsG1(wc)
wchar_t wc;
{
  return (WWhatGPlain(wc) == 1);
}

int
WIsG2(wc)
wchar_t wc;
{
  return (WWhatGPlain(wc) == 2);
}

int
WIsG3(wc)
wchar_t wc;
{
  return (WWhatGPlain(wc) == 3);
}

#ifndef HAVE_WCHAR_OPERATION
int
CANNA_mbstowcs(dest, src, destlen)
wchar_t *dest;
char *src;
int destlen;
{
  register int i, j;
  register unsigned ec;

  if (wchar_type == CANNA_WCTYPE_16) {
    for (i = 0, j = 0 ;
	 (ec = (unsigned)(unsigned char)src[i]) != 0 && j < destlen ; i++) {
      if (ec & 0x80) {
	switch (ec) {
	case 0x8e: /* SS2 */
	  dest[j++] = (wchar_t)(0x80 | ((unsigned)src[++i] & 0x7f));
	  break;
	case 0x8f: /* SS3 */
	  dest[j++] = (wchar_t)(0x8000
				| (((unsigned)src[i + 1] & 0x7f) << 8)
				| ((unsigned)src[i + 2] & 0x7f));
	  i += 2;
	  break;
	default:
	  dest[j++] = (wchar_t)(0x8080 | (((unsigned)src[i] & 0x7f) << 8)
				| ((unsigned)src[i + 1] & 0x7f));
	  i++;
	  break;
	}
      }
      else {
	dest[j++] = (wchar_t)ec;
      }
    }
    if (j < destlen)
      dest[j] = (wchar_t)0;
    return j;
  }
  else if (wchar_type == CANNA_WCTYPE_32) {
    for (i = 0, j = 0 ;
	 (ec = (unsigned)(unsigned char)src[i]) != 0 && j < destlen ; i++) {
      if (ec & 0x80) {
	switch (ec) {
	case 0x8e: /* SS2 */
	  dest[j++] = (wchar_t)(0x10000000L | ((unsigned)src[++i] & 0x7f));
	  break;
	case 0x8f: /* SS3 */
	  dest[j++] = (wchar_t)(0x20000000L
				| (((unsigned)src[i + 1] & 0x7f) << 7)
				| ((unsigned)src[i + 2] & 0x7f));
	  i += 2;
	  break;
	default:
	  dest[j++] = (wchar_t)(0x30000000L | (((unsigned)src[i] & 0x7f) << 7)
				| ((unsigned)src[i + 1] & 0x7f));
	  i++;
	  break;
	}
      }
      else {
	dest[j++] = (wchar_t)ec;
      }
    }
    if (j < destlen)
      dest[j] = (wchar_t)0;
    return j;
  }
  else {
    return 0;
  }
}
#endif /* HAVE_WCHAR_OPERATION */

int
CNvW2E(src, srclen, dest, destlen)
wchar_t *src;
char *dest;
int srclen, destlen;
{
  register int i, j;

#ifdef HAVE_WCHAR_OPERATION
  register char *p, *ep;

  for (p = dest, ep = dest + destlen, i = 0 ;
       i < srclen && p + MB_LEN_MAX < ep ; i++) {
    j = wctomb(p, src[i]);
    if (j < 0) {
      return 0; /* �Τ� -1 ���ä�����ï������å��ʤ��Τǰ����Τ��� 0 �ˤ��� */
    }
    p += j;
  }
  if (p < ep) {
    *p = (unsigned char)'\0';
  }
  return p - dest;
#else /* !HAVE_WCHAR_OPERATION */
  switch (wchar_type) {
  case CANNA_WCTYPE_16:
    for (i = 0, j = 0 ; i < srclen && j + 2 < destlen ; i++) {
      wchar_t wc = src[i];
      switch (wc & 0x8080) {
      case 0:
	/* ASCII */
	dest[j++] = (char)((unsigned)wc & 0x7f);
	break;
      case 0x0080:
	/* Ⱦ�ѥ��� */
	dest[j++] = (char)0x8e; /* SS2 */
	dest[j++] = (char)(((unsigned)wc & 0x7f) | 0x80);
	break;
      case 0x8000:
	/* ���� */
	dest[j++] = (char)0x8f; /* SS3 */
	dest[j++] = (char)((((unsigned)wc & 0x7f00) >> 8) | 0x80);
	dest[j++] = (char)(((unsigned)wc & 0x7f) | 0x80);
	break;
      case 0x8080:
	/* ���� */
	dest[j++] = (char)((((unsigned)wc & 0x7f00) >> 8) | 0x80);
	dest[j++] = (char)(((unsigned)wc & 0x7f) | 0x80);
	break;
      }
    }
    dest[j] = (char)0;
    return j;
#ifndef WCHAR16
  case CANNA_WCTYPE_32:
  default:
    for (i = 0, j = 0 ; i < srclen && j + 2 < destlen ; i++) {
      wchar_t wc = src[i];
      switch (wc >> 28) {
      case 0:
	/* ASCII */
	dest[j++] = (char)((unsigned)wc & 0x7f);
	break;
      case 1:
	/* Ⱦ�ѥ��� */
	dest[j++] = (char)0x8e; /* SS2 */
	dest[j++] = (char)(((unsigned)wc & 0x7f) | 0x80);
	break;
      case 2:
	/* ���� */
	dest[j++] = 0x8f; /* SS3 */
	dest[j++] = (char)((((unsigned)wc & 0x3f80) >> 7) | 0x80);
	dest[j++] = (char)(((unsigned)wc & 0x7f) | 0x80);
	break;
      case 3:
	/* ���� */
	dest[j++] = (char)((((unsigned)wc & 0x3f80) >> 7) | 0x80);
	dest[j++] = (char)(((unsigned)wc & 0x7f) | 0x80);
	break;
      }
    }
    dest[j] = (char)0;
    return j;
#endif /* WCHAR16 */
  }
#endif /* !HAVE_WCHAR_OPERATION */
}

#ifndef HAVE_WCHAR_OPERATION
int
CANNA_wcstombs(dest, src, destlen)
char *dest;
wchar_t *src;
int destlen;
{
  return CNvW2E(src, WStrlen(src), dest, destlen);
}
#endif

/* cfuncdef

  WString -- EUC����磻�ɥ���饯���ؤΥޥåԥ󥰤���� malloc

  WString �ϰ�����ʸ�����磻�ɥ���饯�����Ѵ���������ʸ���󤬼��ޤ�
  �����Υ���� malloc ��������ʸ�����Ǽ���֤���

  ���ѼԤϤ��δؿ��������ݥ��󥿤� free ����ɬ�פϤ��ޤ�ʤ���

  ���ʤ�������δؿ�����������ϸ�� WStringClose ��ƤӽФ����Ȥ���
  free ����롣

  ������������ʤΤǤ��δؿ������ˤ˸ƤӽФ��ƤϤ����ʤ������ޤ�EUC��
  �������Ǥ��Ƥ���ʸ����ʤɤ�α���٤��Ǥ��롣

  ���ε�ǽ��Ȥ��ͤϺǽ�� WStringOpen ��ƤӽФ��ʤ���Фʤ�ʤ�����
  �桼�����󥿥ե������饤�֥��Ǥϥ����ƥब��ưŪ���ɤ�Ǥ�����
  �Ǥ���ɬ�פϤʤ���

 */ 

static wchar_t **wsmemories = NULL;
static int nwsmemories = 0;

#define WSBLOCKSIZE 128

int
WStringOpen()
{
  return 0;
}

wchar_t *
WString(s)
char *s;
{
  int i, len;
  wchar_t *temp, **wm;

  if (wsmemories == (wchar_t **)NULL) {
    nwsmemories = WSBLOCKSIZE;
    if (!(wsmemories = (wchar_t **)calloc(nwsmemories, sizeof(wchar_t *))))
      return((wchar_t *)0) ;
    /* calloc ���줿����ϥ��ꥢ����Ƥ��� */
  }

  for (i = 0 ; i < nwsmemories && wsmemories[i] ;) {
    i++;
  }

  if (i == nwsmemories) { /* �Ȥ��ڤä��Τ����䤹 */
    if (!(wm = (wchar_t **)realloc(wsmemories,
				 (nwsmemories + WSBLOCKSIZE) 
				 * sizeof(wchar_t *))))
      return((wchar_t *)0);
    wsmemories = wm;
    for (; i < nwsmemories + WSBLOCKSIZE ; i++)
      wsmemories[i] = (wchar_t *)0;
    i = nwsmemories;
    nwsmemories += WSBLOCKSIZE;
  }

  /* �Ȥꤢ�����礭���ȤäƤ����ơ����Υ������򸫤����٤Υ�������
     ľ�����֤� */

  len = strlen(s);
  if (!(temp = (wchar_t *)malloc((len + 1) * WCHARSIZE)))
    return((wchar_t *)0);
  len = MBstowcs(temp, s, len + 1);
  if (!(wsmemories[i] = (wchar_t *)malloc((len + 1) * WCHARSIZE))) {
    free(temp);
    return((wchar_t *) 0);
  }
  WStrncpy(wsmemories[i], temp, len);
  wsmemories[i][len] = (wchar_t)0;
  free(temp);
  return(wsmemories[i]);
}

int
WStringClose()
{
  int i;

  for (i = 0 ; i < nwsmemories ; i++)
    if (wsmemories[i])
      free(wsmemories[i]);
  free(wsmemories);
  wsmemories = (wchar_t **)0;
  nwsmemories = 0;
}

WSfree(s)
     wchar_t *s;
{
  int	i;
  wchar_t **t;

  for (t = wsmemories, i = nwsmemories; s != *t && i;) {
    t++;
    i--;
  }
  if (s != *t)
    return(-1);
  free(*t);
  *t = (wchar_t *) 0;
  return(0);
}

/* 
 generalReplace -- ���ʥХåե��ˤ���޻��Хåե��ˤ�Ȥ����ִ��롼����

  �����ִ��롼�����ʸ����Υ������ִ���Ԥ�����Υ饤�֥��롼��
  ��Ǥ��롣������ʸ������ݻ����Ƥ������Ȥߤϼ��Τ褦�ˤʤäƤ���
  ��ΤȤ��롣

    ��ʸ�����ѤΥХåե�
    ��ʸ����°���ѤΥХåե�
    ����������(����ǥå���(�ݥ��󥿤ǤϤʤ�))
    ��ʸ����ν�����ؤ�����ǥå���
    ��ɬ�����������꺸��¸�ߤ��륤��ǥå���(̤�Ѵ�ʸ���ؤΥ���ǥ�
      �����˻Ȥä��ꤹ��)

  �嵭�˼������Хåե���Υ���������������λ��ꤵ�줿Ĺ����ʸ����
  ���̤˻��ꤵ���ʸ������֤�����������򤹤롣

  �ȡ�����ΥХ��ȿ����Ѳ��������ʸ����ν�����ؤ�����ǥå�����
  �ͤ��Ѳ������롣�ޤ������������������ʬ���Ф���ʸ������ִ���Ԥ���
  ��ˤϥ�������ݥ��������ͤ��Ѳ������롣����������Ѳ���������̡�
  ̤�Ѵ�ʸ�����ؤΥ���ǥå������⾮�����ʤä����ˤϡ�̤�Ѵ�ʸ����
  �ؤΥ���ǥå������ͤ򥫡�������ͤ˹�碌�ƾ��������롣

  ���δؿ��κǽ������ˤϿ�������������ʸ�����°���˴ؤ���ҥ�Ȥ����� 
  �Ǥ��롣���������������ʸ����γ�ʸ�����Ф��ơ��ҥ�Ȥ�Ϳ����줿��
  ���Ȥ�°���ͤȤ��Ƴ�Ǽ����롣

  �ڰ�����
     buf      �Хåե��ؤΥݥ���
     attr     °���Хåե��ؤΥݥ���
     startp   �Хåե���̤����ʸ����ʤɤؤΥ���ǥå��������Ƥ�����
              ���ؤΥݥ���
     cursor   ����������֤����Ƥ����ѿ��ؤΥݥ���
     endp     ʸ����κǽ����֤�ؤ������Ƥ����ѿ��ؤΥݥ���

     bytes    ���Х����ִ����뤫����ο������ꤵ���ȥ��������������
              ʬ�� |bytes| ʬ��ʸ�����ִ����оݤȤʤꡢ���ο�������
              �����ȥ�������θ�����ʬ�� bytes ʬ��ʸ�����оݤ�
              �ʤ롣
     rplastr  �������֤�ʸ����ؤΥݥ���
     len      �������֤�ʸ�����Ĺ��
     attrmask �������֤�ʸ�����°���Υҥ��

  �ºݤˤϤ��δؿ���ľ�ܤ˻Ȥ鷺�ˡ�bytes, rplastr, len, attrmask ����
  ��Ϳ��������Ǥ���ޥ���kanaReplace, romajiReplace ��Ȥ��Τ��ɤ���
*/

generalReplace(buf, attr, startp, cursor, endp, bytes, rplastr, len, attrmask)
wchar_t *buf, *rplastr;
BYTE *attr;
int *startp, *cursor, *endp,  bytes, len, attrmask; 
{ 
  int idou, begin, end, i; 
  int cursorMove;

  if (bytes > 0) {
    cursorMove = 0;
    begin = *cursor;
    end = *endp;
  }
  else {
    bytes = -bytes;
    cursorMove = 1;
    begin = *cursor - bytes;
    end = *endp;
  }

  idou = len - bytes;

  moveStrings(buf, attr, begin + bytes, end, idou);
  *endp += idou;
  if (cursorMove) {
    *cursor += idou;
    if (*cursor < *startp)
      *startp = *cursor;
  }

  WStrncpy(buf + begin, rplastr, len);
  for (i = 0 ; i < len ; i++) {
    attr[begin + i] = attrmask;
  }
/*  if (len)
    attr[begin] |= attrmask; */
}

WToupper(w)
wchar_t w;
{
  if ('a' <= w && w <= 'z')
    return((wchar_t) (w - 'a' + 'A'));
  else
    return(w);
}

/*
  ������ wchar ��ʸ�����Ѵ����롣

  ����:
      key         ���Ϥ��줿����
      check       wchar_t ���Ѵ����줿���ɤ������Ǽ���뤿����ѿ��Υ��ɥ쥹
  ����:
      �ؿ�������  �Ѵ����줿 wchar_t ��ʸ��
      check       ���ޤ��Ѵ��Ǥ������ɤ���
  ���:
      check ��ɬ��ͭ�����ѿ��Υ��ɥ쥹��ݥ���Ȥ��뤳�ȡ�
      check �Υݥ�������ͭ������ key2wchar �Ǥϥ����å����ʤ���
 */

wchar_t
key2wchar(key, check)
int key;
int *check;
{
  *check = 1; /* Success as default */
  if (161 <= key && key <= 223) { /* �������ʤ��ϰϤ��ä��� */
    char xxxx[4];
    wchar_t yyyy[4];
    int nchars;

    xxxx[0] = (char)0x8e; /* SS2 */
    xxxx[1] = (char)key;
    xxxx[2] = '\0';
    nchars = MBstowcs(yyyy, xxxx, 4);
    if (nchars != 1) {
      *check = 0;
      return 0; /* ���顼 */
    }
    return yyyy[0];
  }
  else {
    return (wchar_t)key;
  }
}

int
confirmContext(d, yc)
uiContext d;
yomiContext yc;
{
  extern defaultContext;

  if (yc->context < 0) {
    if (d->contextCache >= 0) {
      yc->context = d->contextCache;
      d->contextCache = -1;
    }
    else {
      if (defaultContext == -1) {
	if (KanjiInit() < 0 || defaultContext == -1) {
	  jrKanjiError = "���ʴ����Ѵ������Ф��̿��Ǥ��ޤ���";
	  return -1;
	}
      }
      yc->context = RkwDuplicateContext(defaultContext);
      if (yc->context < 0) {
	if (errno == EPIPE) {
	  jrKanjiPipeError();
	}
	jrKanjiError = "���ʴ����Ѵ��˼��Ԥ��ޤ���";
	return -1;
      }
    }
  }
  return yc->context;
}

int
abandonContext(d, yc)
uiContext d;
yomiContext yc;
{
  extern defaultContext;

  if (yc->context >= 0) {
    if (d->contextCache >= 0) {
      RkwCloseContext(yc->context);
    }
    else {
      d->contextCache = yc->context;
    }
    yc->context = -1;
  }
  return 0;
}

int
makeRkError(d, str)
uiContext d;
char *str;
{
  extern errno;

  if (errno == EPIPE) {
    jrKanjiPipeError();
  }
  jrKanjiError = str;
  makeGLineMessageFromString(d, jrKanjiError);
  return -1;
}
