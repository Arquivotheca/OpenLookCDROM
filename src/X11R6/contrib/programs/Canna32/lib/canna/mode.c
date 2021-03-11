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
static char rcs_id[] = "@(#) 102.1 $Id: mode.c,v 7.12 1994/03/13 09:13:21 kon Exp $";
#endif /* lint */

#include "canna.h"
#include <canna/mfdef.h>

extern int howToReturnModeInfo;
static wchar_t numMode[2];
static wchar_t *bad = (wchar_t *)0;
struct ModeNameRecs ModeNames[CANNA_MODE_MAX_IMAGINARY_MODE];

static
char *
_sModeNames[CANNA_MODE_MAX_IMAGINARY_MODE] = {
  "      ",		/* AlphaMode */
  "[ �� ]",	/* EmptyMode */
  "[����]",	/* KigoMode */
  "[���]",	/* YomiMode (�⡼��ʸ����ɽ���ˤϻȤ�ʤ�) */
  "[����]",	/* JishuMode */
  "[����]",	/* TanKouhoMode */
  "[����]",	/* IchiranMode */
  "[����]",	/* YesNoMode */
  NULL,	/* OnOffMode */
  "[ʸ��]",    /* AdjustBunsetsuMode */
  "[�༡]",	/* ChikujiYomiMode    �༡�λ����ɤ���ʬ */
  "[�༡]",    /* ChikujiHenkanMode  �༡�λ����Ѵ�����ʬ */

  /* Imaginary Mode */

  "[ �� ]",	/* HenkanNyuryokuMode */
  "[����]",	/* ZenHiraHenkanMode */
  "[Ⱦ��]",	/* HanHiraHenkanMode */
  "[����]",	/* ZenKataHenkanMode */
  "[Ⱦ��]",	/* HanKataHenkanMode */
  "[����]",	/* ZenAlphaHenkanMode */
  "[Ⱦ��]",	/* HanAlphaHenkanMode */
  "<����>",	/* ZenHiraKakuteiMode */
  "<Ⱦ��>",	/* HanHiraKakuteiMode */
  "<����>",	/* ZenKataKakuteiMode */
  "<Ⱦ��>",	/* HanKataKakuteiMode */
  "<����>",	/* ZenAlphaKakuteiMode */
  "<Ⱦ��>",	/* HanAlphaKakuteiMode */
  "[16��]",	/* HexMode */
  "[����]",	/* BushuMode */
  "[��ĥ]",	/* ExtendMode */
  "[ �� ]",	/* RussianMode */
  "[ �� ]",	/* GreekMode */
  "[����]",	/* LineMode */
  "[�ѹ�]",	/* ChangingServerMode */
  "[�Ѵ�]",	/* HenkanMethodMode */
  "[���]",	/* DeleteDicMode */
  "[��Ͽ]",	/* TourokuMode */
  "[�ʻ�]",	/* TourokuHinshiMode */
  "[����]",	/* TourokuDicMode */
  "[ �� ]",	/* QuotedInsertMode */
  "[�Խ�]",	/* BubunMuhenkanMode */
  "[����]",  /* MountDicMode */
  };

static wchar_t * _ModeNames[CANNA_MODE_MAX_IMAGINARY_MODE];

extern extraFunc *FindExtraFunc();
#define findExtraMode(mnum) \
 FindExtraFunc((mnum) - CANNA_MODE_MAX_IMAGINARY_MODE + CANNA_FN_MAX_FUNC)

newmode *
findExtraKanjiMode(mnum)
int mnum;
{
  extern extraFunc *extrafuncp;
  extraFunc *extrafunc;
  register int fnum =
    mnum - CANNA_MODE_MAX_IMAGINARY_MODE + CANNA_FN_MAX_FUNC;

  for (extrafunc = extrafuncp; extrafunc; extrafunc = extrafunc->next) {
    if (extrafunc->fnum == fnum) {
      switch (extrafunc->keyword) {
      case EXTRA_FUNC_DEFMODE:
	return extrafunc->u.modeptr;
      default:
	return (newmode *)0;
      }
    }
  }
  return (newmode *)0;
}

extern nothermodes;

static wchar_t *
modestr(mid)
int mid;
{
  if (mid < CANNA_MODE_MAX_IMAGINARY_MODE) {
      return(ModeNames[mid].name);
  }
  else if (mid - CANNA_MODE_MAX_IMAGINARY_MODE < nothermodes) {
    extraFunc *ep = findExtraMode(mid);
    if (ep) {
      return ep->display_name;
    }
  }
  return (wchar_t *)0;
}
    
currentModeInfo(d)
uiContext d;
{
  coreContext cc = (coreContext)d->modec;

  if (d->current_mode->flags & CANNA_KANJIMODE_EMPTY_MODE) {
    d->kanji_status_return->info |= KanjiEmptyInfo;
  }

  if (howToReturnModeInfo == ModeInfoStyleIsString) {
    wchar_t *modename, *gmodename;
    if (cc->minorMode != d->minorMode) {
      modename = modestr(cc->minorMode);
      gmodename = modestr(d->minorMode);
      if (modename && (gmodename == (wchar_t *)NULL ||
		       WStrcmp(modename, gmodename))) {
	d->majorMode = cc->majorMode;
	d->minorMode = cc->minorMode;
	d->kanji_status_return->mode = modename;
	d->kanji_status_return->info |= KanjiModeInfo;
      }
    }
  }
  else {
    if (cc->majorMode != d->majorMode) {
      d->majorMode = cc->majorMode;
      d->minorMode = cc->minorMode;
      numMode[0] = (wchar_t)('@' + cc->majorMode);
      numMode[1] = (wchar_t) 0;
      d->kanji_status_return->info |= KanjiModeInfo;
      d->kanji_status_return->mode = numMode;
    }
  }
}
     
/* ���Υե�����ˤϥ⡼���ѹ��˴ؤ��������̤����äƤ��롣�⡼�ɤ���
 * ���Ȥϡ����޻������Ѵ���ɽ�̤˸����⡼�ɤ��ѹ������ǤϤʤ�����
 * ���ɤߤ��ʤ����֤��顢�ɤߤ�������֤˰ܤ���ˤ��������Τ�ؤ���
 */

initModeNames()
{
  int i;
  
  for (i = 0 ; i < CANNA_MODE_MAX_IMAGINARY_MODE ; i++) {
    ModeNames[i].alloc = 0;
    ModeNames[i].name = _ModeNames[i] = 
      _sModeNames[i] ? WString(_sModeNames[i]) : 0;
  }
  if (!bad) {
    bad = WString("���꤬­��ޤ���");
  }
}

resetModeNames()
{
  int i;

  for (i = 0 ; i < CANNA_MODE_MAX_IMAGINARY_MODE ; i++) {
    if (ModeNames[i].alloc && ModeNames[i].name) {
      ModeNames[i].alloc = 0;
      WSfree(ModeNames[i].name);
    }
    ModeNames[i].name = _ModeNames[i];
  }
}

static
japaneseMode(d)
uiContext d;
{
  coreContext cc = (coreContext)d->modec;

  d->current_mode = cc->prevMode;
  d->modec = cc->next;
  freeCoreContext(cc);
  d->status = EXIT_CALLBACK;
}

/* cfuncdef

  JapaneseMode(d) -- �⡼�ɤ����ܸ����ϥ⡼�ɤ��Ѥ��롣

  ���� ���δؿ��Ϧ��⡼�ɤǤ����Ƥ�ǤϤ����ʤ���

 */

JapaneseMode(d)
uiContext d;
{
  coreContext cc = (coreContext)d->modec;
  yomiContext yc = (yomiContext)cc->next;

  if (yc->generalFlags & CANNA_YOMI_CHGMODE_INHIBITTED) {
    return NothingChangedWithBeep(d);
  }

  japaneseMode(d);
  d->kanji_status_return->length = 0;
  return 0;
}

AlphaMode(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  if (yc->generalFlags & CANNA_YOMI_CHGMODE_INHIBITTED) {
    return NothingChangedWithBeep(d);
  }
  else {
    alphaMode(d);
    currentModeInfo(d);
    d->kanji_status_return->length = 0;
    return 0;
  }
}

HenkanNyuryokuMode(d)
uiContext d;
{
  extern KanjiModeRec empty_mode;
  yomiContext yc = (yomiContext)d->modec;

  if (yc->generalFlags & CANNA_YOMI_CHGMODE_INHIBITTED) {
    return NothingChangedWithBeep(d);
  }

  yc->generalFlags &= (~CANNA_YOMI_ATTRFUNCS & ~CANNA_YOMI_IGNORE_USERSYMBOLS &
		       ~CANNA_YOMI_BASE_HANKAKU);
  d->current_mode = yc->myEmptyMode = &empty_mode;
  yc->majorMode = yc->minorMode = CANNA_MODE_EmptyMode;
  yc->myMinorMode = 0; /* 0 �� AlphaMode �ȽŤʤ뤬����Ϥʤ� */
  yc->romdic = romajidic;
  EmptyBaseModeInfo(d, yc);

  if(yc->rCurs)
    return RomajiFlushYomi(d, 0, 0); /* ����ϻ���Ū */

  d->kanji_status_return->length = 0;
  return 0;
}

queryMode(d, arg)
uiContext d;
wchar_t *arg;
{
  coreContext cc = (coreContext)d->modec;
  wchar_t *mode_str = (wchar_t *)0;
  extraFunc *ep;

  switch (howToReturnModeInfo) {
  case ModeInfoStyleIsString:
    if (d->minorMode < (BYTE)CANNA_MODE_MAX_IMAGINARY_MODE) {
      mode_str = ModeNames[d->minorMode].name;
    }
    else if (d->minorMode <
	     (BYTE)(CANNA_MODE_MAX_IMAGINARY_MODE + nothermodes)) {
      ep = findExtraMode(d->minorMode);
      if (ep) {
	mode_str = ep->display_name;
      }
    }
    if (!mode_str) {
      int ii;
      for (ii = 0; ii < 4; ii++, arg++) {
	*arg = (wchar_t)'\0';
      }
    }
    else {
      WStrcpy(arg, mode_str);
    }
    break;
  case ModeInfoStyleIsExtendedNumeric:
    arg[1] = (wchar_t)('@' + (int)cc->minorMode);
  case ModeInfoStyleIsNumeric:
    arg[0] = (wchar_t)('@' + (int)cc->majorMode);
    break;
  default:
    return(-1);
    /* NOTREACHED */
    break;
  }
  return 0;
}

/* 
 *   ����⡼�ɤ��Ф��ƥ⡼��ɽ��ʸ�������ꤹ�롣
 *
 */

changeModeName(modeid, str)
int modeid;
char *str;
{
  extraFunc *ep;

  if (modeid == CANNA_MODE_HenkanNyuryokuMode)
    modeid = CANNA_MODE_EmptyMode;

  if (modeid >= 0) {
    if (modeid < CANNA_MODE_MAX_IMAGINARY_MODE) {
      if (ModeNames[modeid].alloc && ModeNames[modeid].name) {
	WSfree(ModeNames[modeid].name);
      }
      if (str) {
	ModeNames[modeid].alloc = 1;
	ModeNames[modeid].name = WString(str);
      }
      else {
	ModeNames[modeid].alloc = 0;
	ModeNames[modeid].name = (wchar_t *)0;
      }
    }
    else if (modeid < (CANNA_MODE_MAX_IMAGINARY_MODE + nothermodes)) {
      ep = findExtraMode(modeid);
      if (ep) {
	if (ep->display_name) {
	  WSfree(ep->display_name);
	}
	if (str) {
	  ep->display_name = WString(str);
	}
	else {
	  ep->display_name = (wchar_t *)0;
	}
      }
      else {
	return -1;
      }
    }
    return 0;
  }
  return -1;
}

