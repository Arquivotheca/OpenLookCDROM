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
  echostr2Gline      ���ξ���֤����Ȥ�����Υ����ɥ饤����֤�
  echostrClear       ���ξ줬�����ä����褦�ʥ꥿�����ͤ���
  checkGLineLen      �����ɥ饤���ɽ��������뤫�ɤ����Υ����å�
  NothingChanged     �����Ѳ����ʤ����Ȥ򼨤��꥿�����ͤ���
  NothingForGLine    �����ɥ饤��˴ؤ��Ƥϲ����Ѳ����ʤ�
  NothingChangedWithBeep
                     NothingChange �򤷤Ƥ���˥ӡ��ײ����Ĥ餹
  NothingForGLineWithBeep
                     NothingForGLine �򤷤Ƥ���˥ӡ��ײ����Ĥ餹
  ujisncpy           EUC ʸ���� n �Х��ȥ��ԡ�����
  makeGLineMessage   ������ʸ�����GLine��ɽ������褦�ʥ꥿�����ͤ���
  makeGLineMessageFromString
  		     ������eucʸ�����GLine��ɽ������褦�ʥ꥿�����ͤ���
  copyYomiBuffers    ¾���ɤߥ���ƥ����Ȥ�����ɤߥǡ����Υ��ԡ�
  setWStrings	     ʸ������ν������Ԥ�
  NoMoreMemory       ���꤬�ʤ����饨�顼����Ȥ������顼�ͤ��֤�
  GLineNGReturn      ���顼��å������򥬥��ɥ饤��˰ܤ�
  GLineNGReturnFI    �����⡼�ɤ�ȴ���� GLineNGReturn �򤹤롣
  GLineNGReturnTK    ��Ͽ�⡼�ɤ�ȴ���� GLineNGReturn �򤹤롣
  WStrlen            �磻�ɥ���饯��ʸ�����Ĺ������� (cf. strlen)
  WStrcat            �磻�ɥ���饯��ʸ�����ä��롣(cf. strcat)
  WStrcpy            �磻�ɥ���饯��ʸ����򥳥ԡ����롣(cf. strcpy)
  WStrncpy           �磻�ɥ���饯��ʸ������ʸ�����ԡ����롣(cf. strncpy)
  WStrcmp	     �磻�ɥ���饯��ʸ�������Ӥ��롣(cf. strcmp)
  WStrncmp	     �磻�ɥ���饯��ʸ������ʸ����Ӥ��롣(cf. strncmp)
  WToupper	     �磻�ɥ���饯���αѻ���ʸ������ʸ�����Ѵ����롣
  WWhatGPlain	     �磻�ɥ���饯����ʸ����°���륰��ե��å��ץ졼����֤�
  WIsG0              G0�Υ磻�ɥ���饯��ʸ������
  WIsG1              G1�Υ磻�ɥ���饯��ʸ������
  WIsG2              G2�Υ磻�ɥ���饯��ʸ������
  WIsG3              G3�Υ磻�ɥ���饯��ʸ������
  WGetLeft           G1��G3�κ�¦(�����͡�)
  WGetRight          G1��G3�α�¦(�����͡�)
  MBstowcs           EUC ��磻�ɥ���饯��ʸ������Ѵ�
  CNvW2E             �磻�ɥ���饯���� EUC ���Ѵ�(�����å��դ�)
  WCstombs           �磻�ɥ���饯���� EUC ���Ѵ�
  WSfree	     WString�ǳ��ݤ����ΰ��������
  WString            EUC ��磻�ɤ��Ѵ����� malloc �ޤǤ����֤�(free ����)
  WStringOpen        �嵭�ؿ��ν��������
  WStringClose       �嵭�ؿ��ν�λ����

 */

static char sccs_id[] = "@(#) 102.1 $Id: util.c,v 1.6 1993/03/01 08:08:45 kon Exp $";

#define _WCHAR16 /* ��������ʤ���Τ򥳥�ѥ��뤹����ϤϤ����Ƥ� */
                 /* ������ưŪ���ڤ��ؤ�����Ϥ� */

#include "canna.h"

#if __STDC__
#include <stdlib.h>
#else
extern char *malloc(), *realloc(), *calloc();
extern void free();
#endif

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
 ujisncpy -- ���Х���ʸ����������ڤ�ʤ��褦�˥��ԡ�����ؿ�

  �ºݤ˥��ԡ�����Х��ȿ��� n �ǻ��ꤵ�줿�Х��ȿ������뤤�Ϥ�����
  ���������ͤˤʤ롣�ºݤ˥��ԡ������Х��ȿ����֤���

  SS3 �ˤ��б����Ƥ��ʤ���
 */

int
ujisncpy(dest, src, n)
unsigned char *dest, *src;
int n;
{
  int i = 0;
  unsigned char c;

  while (i < n) {
    c = src[i];
    if ( ! (c & 0x80) ) { /* ASCII �ξ�� */
      dest[i++] = c;
    }
    else if (i + 1 < n) { /* ���������ʤξ�硢�����⥳�ԡ��Ǥ����� */
      dest[i++] = c;
      dest[i] = src[i];
      i++;
    }
    else { /* �����ǣ��Х��Ȥ������ԡ��Ǥ��ʤ���� */
      return i; /* ���ԡ������줺��n ��꣱�������ͤǽ���ä��� */
    }
  }
  return i; /* n �Х��ȥ��ԡ������줿 */
}

setWStrings(ws, s, sz)
wchar_t **ws;
unsigned char **s;
int sz;
{
  int f = sz;
  wchar_t *WString();

  for (; (f && sz) || (!f && *s); ws++, s++, sz--)
    *ws = WString(*s);
}


copyAttribute(dest, src, n)
     BYTE	*dest;
     BYTE	*src;
     int n;
{
  for (; n; n--)
    *dest++ = *src++;
}

/*
 * �磻�ɥ���饯�����ڥ졼�����
 *
 */

wchar_t
WToupper(w)
wchar_t w;
{
  if (WIsG0(w)) {
    if ('a' <= w && w <= 'z')
      return((wchar_t) (w - 'a' + 'A'));
  } else
    return(w);
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

  for (ws = ws2, cnt = 0 ; *ws ; ws++, cnt++) ;
  len = cnt;
  if (ws2 < ws1 && ws1 < ws2 + cnt) {
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

  if  (ws2 == (wchar_t *) NULL)
    return;
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
WStrcat(ws1, ws2)
wchar_t *ws1, *ws2;
{
  wchar_t *ws;

  for (ws = ws1; *ws; ws++);
  WStrcpy(ws, ws2);
  return ws1;
}

int
WStrcmp(w1, w2)
wchar_t *w1, *w2;
{
  for (; *w1 && *w1 == *w2; w1++, w2++);
  return(*w1 - *w2);
}

int
WStrncmp(w1, w2, n)
wchar_t *w1, *w2;
int n;
{
  if (n == 0) return(0);
  for (; --n && *w1 && *w1 == *w2; w1++, w2++);
  return(*w1 - *w2);
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
#ifdef _WCHAR16
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
#else /* !_WCHAR16 */
  static char plain[4] = {0, 2, 3, 1};

  return plain[(((unsigned long)wc) >> 28) & 3];
#endif /* !_WCHAR16 */
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

/* �ʲ��Σ��Ĥδؿ��ϣ��Х��Ȥޤ�Ŭ�Ѳ� */

int
WGetLeft(wc)
wchar_t wc;
{
  if (WIsG0(wc) || WIsG2(wc))
    return 0;
  else {
#ifdef _WCHAR16
    return ((((unsigned long)wc) >> 8) & 0x7f);
#else /* !_WCHAR16 */
    return ((((unsigned long)wc) >> 7) & 0x7f);
#endif /* !_WCHAR16 */
  }
}

int
WGetRight(wc)
wchar_t wc;
{
  if (WIsG0(wc) || WIsG2(wc))
    return 0;
  else {
    return (((unsigned long)wc) & 0x7f);
  }
}

int
MBstowcs(dest, src, destlen)
wchar_t *dest;
unsigned char *src;
int destlen;
{
  register int i, j;
  register unsigned char ec;

  for (i = 0, j = 0 ; (ec = src[i]) && j < destlen ; i++) {
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

int
CNvW2E(src, srclen, dest, destlen)
wchar_t *src;
unsigned char *dest;
int srclen, destlen;
{
  register int i, j;
  register wchar_t wc;

  for (i = 0, j = 0 ; i < srclen && j + 2 < destlen ; i++) {
    wc = src[i];
    switch (wc & 0x8080) {
    case 0:
      /* ASCII */
      dest[j++] = (unsigned char)((unsigned)wc & 0x7f);
      break;
    case 0x0080:
      /* Ⱦ�ѥ��� */
      dest[j++] = 0x8e; /* SS2 */
      dest[j++] = (unsigned char)(((unsigned)wc & 0x7f) | 0x80);
      break;
    case 0x8000:
      /* ���� */
      dest[j++] = 0x8f; /* SS3 */
      dest[j++] = (unsigned char)((((unsigned)wc & 0x7f00) >> 8) | 0x80);
      dest[j++] = (unsigned char)(((unsigned)wc & 0x7f) | 0x80);
      break;
    case 0x8080:
      /* ���� */
      dest[j++] = (unsigned char)((((unsigned)wc & 0x7f00) >> 8) | 0x80);
      dest[j++] = (unsigned char)(((unsigned)wc & 0x7f) | 0x80);
      break;
    }
  }
  dest[j] = (unsigned char)0;
  return j;
}

int
WCstombs(dest, src, destlen)
unsigned char *dest;
wchar_t *src;
int destlen;
{
  return CNvW2E(src, WStrlen(src), dest, destlen);
}

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
unsigned char *s;
{
  int i, len;
  wchar_t *temp;

  if (wsmemories == (wchar_t **)NULL) {
    nwsmemories = WSBLOCKSIZE;
    wsmemories = (wchar_t **)calloc(nwsmemories, sizeof(wchar_t *));
    /* calloc ���줿����ϥ��ꥢ����Ƥ��� */
  }

  for (i = 0 ; i < nwsmemories && wsmemories[i] ; i++);

  if (i == nwsmemories) { /* �Ȥ��ڤä��Τ����䤹 */
    wsmemories = (wchar_t **)realloc(wsmemories,
				     (nwsmemories + WSBLOCKSIZE) 
				     * sizeof(wchar_t *));
    for (; i < nwsmemories + WSBLOCKSIZE ; i++) {
      wsmemories[i] = (wchar_t *)0;
    }
    i = nwsmemories;
    nwsmemories += WSBLOCKSIZE;
  }

  /* �Ȥꤢ�����礭���ȤäƤ����ơ����Υ������򸫤����٤Υ�������
     ľ�����֤� */

  len = strlen((char *)s);
  temp = (wchar_t *)malloc((len + 1) * WCHARSIZE);
  len = MBstowcs(temp, s, len);
  wsmemories[i] = (wchar_t *)malloc((len + 1) * WCHARSIZE);
  WStrncpy(wsmemories[i], temp, len);
  free(temp);
  wsmemories[i][len] = (wchar_t)0;
  return wsmemories[i];
}

int
WStringClose()
{
  int i;

  for (i = 0 ; i < nwsmemories ; i++) {
    if (wsmemories[i]) {
      free(wsmemories[i]);
    }
  }
  free(wsmemories);
  wsmemories = (wchar_t **)0;
  nwsmemories = 0;
}

WSfree(s)
wchar_t *s;
{
  int	i;
  wchar_t **t;
  for (t = wsmemories, i = nwsmemories; s != *t && i; t++, i--)
    ;
  free(*t);
  *t = (wchar_t *) 0;
}


