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
static char rcs_id[] = "@(#) 102.1 $Id: empty.c,v 5.30 1994/04/12 12:55:24 kon Exp $";
#endif /* lint */

#include "canna.h"
#include "patchlevel.h"

extern KanjiModeRec yomi_mode, cy_mode;

/* EmptySelfInsert -- ��ʬ���Ȥ����ʸ����Ȥ����֤��ؿ���
 * 
 */

static
inEmptySelfInsert(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;
  int res = 0;

  d->kanji_status_return->info |= KanjiThroughInfo | KanjiEmptyInfo;
  if (!(yc->generalFlags & CANNA_YOMI_END_IF_KAKUTEI)) {
    res = d->nbytes;
  }
  /* else { ����ǡ����������ԤäƤ���ͤˤϳ���ǡ������Ϥ��ʤ� } */

  return res;
}

static
EmptySelfInsert(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;
  int res = inEmptySelfInsert(d);

/* ñ����Ͽ�ΤȤ��� yomi mode �γ��ꥭ���� empty mode �Ǥϳ��ꥭ���Ǥ�
   ���ä��ꤹ��ȡ����Υ����β����ǻ��Ǥ��ޤä��ꤹ��Τεߺѡ�yomi
   mode �ξ�� yomi mode ����äƤ���Τ�ñ����Ͽ�λ����餤�����ȸ�
   �����Ȥ�Ƚ�Ǥκ����ˤ��Ƥ��롣�����Ϥ���ʤ��Ȥ�ꤿ���ʤ��� */

  if (yc->next && yc->next->id == YOMI_CONTEXT &&
      yomi_mode.keytbl[d->buffer_return[0]] == CANNA_FN_Kakutei) {
    d->status = EXIT_CALLBACK;
    if (d->cb->func[EXIT_CALLBACK] != NO_CALLBACK) {
      d->kanji_status_return->info &= ~KanjiThroughInfo; /* �Ż����� */
      popYomiMode(d);
    }
  }
  return res;
}

/* EmptyYomiInsert -- ���⡼�ɤ˰ܹԤ����ɤߤ����Ϥ���ؿ�
 *
 */

static
EmptyYomiInsert(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  d->current_mode = (yc->generalFlags & CANNA_YOMI_CHIKUJI_MODE) ?
    &cy_mode : &yomi_mode;
  RomajiClearYomi(d);
  return YomiInsert(d); /* ������Хå��Υ����å��� YomiInsert �Ǥ���� */
}

/* EmptyQuotedInsert -- ���ΰ�����ɤΤ褦��ʸ���Ǥ⥹�롼���̤��ؿ���
 *
 */

/* 
  Empty �⡼�ɤǤ� quotedInset �� ^Q �Τ褦��ʸ������� Emacs �ʤɤ���
  ���̤äƤ��ޤ��Хޥåפ��֤��Ƥ��ޤ��Τǡ����ʴ����Ѵ������ǲ�����
  ����ʤ�Ƥ��Ȥ�ɬ�פʤ��ΤǤϤʤ��Τ��ʤ���
 */

static
EmptyQuotedInsert(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  d->current_mode = (yc->generalFlags & CANNA_YOMI_CHIKUJI_MODE) ?
    &cy_mode : &yomi_mode;
  return YomiQuotedInsert(d);
}

/* 
  AlphaSelfInsert -- ��ʬ���Ȥ����ʸ����Ȥ����֤��ؿ���
 */

static
AlphaSelfInsert(d)
uiContext d;
{
  unsigned kanap = (unsigned)(unsigned char)d->ch;

  d->kanji_status_return->length = 0;
  d->kanji_status_return->info |= KanjiEmptyInfo;
  if ( d->nbytes != 1 || kanap < 0xa0 || 0xdf < kanap ) {
    d->kanji_status_return->info |= KanjiThroughInfo;
    return d->nbytes;
  }
  else { /* ��̾�������Ϥξ�� */
    if (d->n_buffer > 1) {
      return 1;
    }
    else {
      return 0;
    }
  }
}

static
AlphaNop(d)
uiContext d;
{
  /* currentModeInfo �ǥ⡼�ɾ���ɬ���֤�褦�˥��ߡ��Υ⡼�ɤ�����Ƥ��� */
  d->majorMode = d->minorMode = CANNA_MODE_KigoMode;
  currentModeInfo(d);
  return 0;
}

static
EmptyQuit(d)
uiContext d;
{
  int res;

  res = inEmptySelfInsert(d);
  d->status = QUIT_CALLBACK;
  if (d->cb->func[QUIT_CALLBACK] != NO_CALLBACK) {
    d->kanji_status_return->info &= ~KanjiThroughInfo; /* �Ż����� */
    popYomiMode(d);
  }
  return res;
}

static
EmptyKakutei(d)
uiContext d;
{
  int res;

  res = inEmptySelfInsert(d);
  d->status = EXIT_CALLBACK;
  if (d->cb->func[EXIT_CALLBACK] != NO_CALLBACK) {
    d->kanji_status_return->info &= ~KanjiThroughInfo; /* �Ż����� */
    popYomiMode(d);
  }
  return res;
}

static
EmptyDeletePrevious(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  if (yc->generalFlags & CANNA_YOMI_DELETE_DONT_QUIT) {
    /* Delete �� QUIT ���ʤ��ΤǤ���С�selfInsert */
    return inEmptySelfInsert(d);
  }
  else {
    return EmptyQuit(d);
  }
}

extraFunc *
FindExtraFunc(fnum)
int fnum;
{
  extern extraFunc *extrafuncp;
  extraFunc *extrafunc;

  for (extrafunc = extrafuncp; extrafunc; extrafunc = extrafunc->next) {
    if (extrafunc->fnum == fnum) {
      return extrafunc;
    }
  }
  return (extraFunc *)0;
}

static
UserMode(d, estruct)
uiContext d;
extraFunc *estruct;
{
  newmode *nmode = estruct->u.modeptr;
  yomiContext yc = (yomiContext)d->modec;
  int modeid
    = estruct->fnum - CANNA_FN_MAX_FUNC + CANNA_MODE_MAX_IMAGINARY_MODE;

  if (yc->generalFlags & CANNA_YOMI_CHGMODE_INHIBITTED) {
    return NothingChangedWithBeep(d);
  }

  yc->generalFlags &= ~CANNA_YOMI_ATTRFUNCS;
  yc->generalFlags |= nmode->flags;
  if (yc->generalFlags & CANNA_YOMI_END_IF_KAKUTEI) {
    /* ����ǽ����褦�ʥ⡼�ɤ��ä������⡼�ɤˤʤ�ʤ� */
    yc->generalFlags &= ~CANNA_YOMI_KAKUTEI;
  }
  yc->romdic = nmode->romdic;
  d->current_mode = yc->myEmptyMode = nmode->emode;

  yc->majorMode = yc->minorMode = yc->myMinorMode = (BYTE)modeid;

  currentModeInfo(d);

  d->kanji_status_return->length = 0;
  return 0;
}

static
UserSelect(d, estruct)
uiContext d;
extraFunc *estruct;
{
  int curkigo = 0, *posp = (int *)0;
  kigoIchiran *kigop = (kigoIchiran *)0;
  extern int uuKigoGeneralExitCatch(), uuKigoMake();
  selectinfo *selinfo = (selectinfo *)0, *info;

  info = d->selinfo;
  while (info) {
    if (info->ichiran == estruct->u.kigoptr) {
      selinfo = info;
      break;
    }
    info = info->next;
  }

  if (!selinfo) {
    selinfo = (selectinfo *)malloc(sizeof(selectinfo));
    if (selinfo) {
      selinfo->ichiran = estruct->u.kigoptr;
      selinfo->curnum = 0;
      selinfo->next = d->selinfo;
      d->selinfo = selinfo;
    }
  }

  if (selinfo) {
    curkigo = selinfo->curnum;
    posp = &selinfo->curnum;
  }

  kigop = estruct->u.kigoptr;
  if (!kigop) {
    return NothingChangedWithBeep(d);
  }
  return uuKigoMake(d, kigop->kigo_data, kigop->kigo_size, 
                    curkigo, kigop->kigo_mode, uuKigoGeneralExitCatch, posp);
}
  
static
UserMenu(d, estruct)
uiContext d;
extraFunc *estruct;
{
  return showmenu(d, estruct->u.menuptr);
}

/* �ǥե���Ȱʳ��Υ⡼�ɻ��ѻ��˸ƤӽФ��ؿ����ڤ�ʬ���� */

static
ProcExtraFunc(d, fnum)
uiContext d;
int fnum;
{
  extraFunc *extrafunc;

  extrafunc = FindExtraFunc(fnum);
  if (extrafunc) {
    switch (extrafunc->keyword) {
      case EXTRA_FUNC_DEFMODE:
        return UserMode(d, extrafunc);
      case EXTRA_FUNC_DEFSELECTION:
        return UserSelect(d, extrafunc);
      case EXTRA_FUNC_DEFMENU:
        return UserMenu(d, extrafunc);
      default:
        break;
    }
  }
  return NothingChangedWithBeep(d);
}

getBaseMode(yc)
yomiContext yc;
{
  int res;
  long fl = yc->generalFlags;

  if (yc->myMinorMode) {
    return yc->myMinorMode;
  }
  else if (fl & CANNA_YOMI_ROMAJI) {
    res = CANNA_MODE_ZenAlphaHenkanMode;
  }
  else if (fl & CANNA_YOMI_KATAKANA) {
    res = CANNA_MODE_ZenKataHenkanMode;
  }
  else {
    res = CANNA_MODE_ZenHiraHenkanMode;
  }
  if (fl & CANNA_YOMI_BASE_HANKAKU) {
    res++;
  }
  if (fl & CANNA_YOMI_KAKUTEI) {
    res += CANNA_MODE_ZenHiraKakuteiMode - CANNA_MODE_ZenHiraHenkanMode;
  }
  if (res == CANNA_MODE_ZenHiraHenkanMode) {
    if (chikujip(yc)) {
      res = CANNA_MODE_ChikujiYomiMode;
    }
    else {
      res = CANNA_MODE_HenkanMode;
    }
  }
  return res;
}

/* �١���ʸ�����ڤ��ؤ� */

EmptyBaseModeInfo(d, yc)
uiContext d;
yomiContext yc;
{
  coreContext cc = (coreContext)d->modec;

  cc->minorMode = getBaseMode(yc);
  currentModeInfo(d);
}

extern InhibitHankakuKana;

EmptyBaseHira(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  if (yc->generalFlags & CANNA_YOMI_CHGMODE_INHIBITTED) {
    return NothingChangedWithBeep(d);
  }
  yc->generalFlags &= ~(CANNA_YOMI_KATAKANA | CANNA_YOMI_HANKAKU |
			CANNA_YOMI_ROMAJI | CANNA_YOMI_ZENKAKU);
  EmptyBaseModeInfo(d, yc);
  return 0;
}

EmptyBaseKata(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  if ((yc->generalFlags & CANNA_YOMI_CHGMODE_INHIBITTED)
      || (InhibitHankakuKana
	  && (yc->generalFlags & CANNA_YOMI_BASE_HANKAKU))) {
    return NothingChangedWithBeep(d);
  }
  yc->generalFlags &= ~(CANNA_YOMI_ROMAJI | CANNA_YOMI_ZENKAKU);
  yc->generalFlags |= CANNA_YOMI_KATAKANA |
    ((yc->generalFlags & CANNA_YOMI_BASE_HANKAKU) ? CANNA_YOMI_HANKAKU : 0);
  EmptyBaseModeInfo(d, yc);
  return 0;
}

EmptyBaseEisu(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  if (yc->generalFlags & CANNA_YOMI_CHGMODE_INHIBITTED) {
    return NothingChangedWithBeep(d);
  }
/*  yc->generalFlags &= ~CANNA_YOMI_ATTRFUNCS; ���ꥢ����Τ�� */
  yc->generalFlags |= CANNA_YOMI_ROMAJI |
    ((yc->generalFlags & CANNA_YOMI_BASE_HANKAKU) ? 0 : CANNA_YOMI_ZENKAKU);
  EmptyBaseModeInfo(d, yc);
  return 0;
}

EmptyBaseZen(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  if (yc->generalFlags & CANNA_YOMI_CHGMODE_INHIBITTED) {
    return NothingChangedWithBeep(d);
  }
  yc->generalFlags &= ~CANNA_YOMI_BASE_HANKAKU;
  if (yc->generalFlags & CANNA_YOMI_ROMAJI) {
    yc->generalFlags |= CANNA_YOMI_ZENKAKU;
  }
  /* ���� ���޻��⡼�ɤǤ��ĥ������ʥ⡼�ɤλ�������
          (���ξ��ɽ�̾�ϥ��޻��⡼��) */
  if (yc->generalFlags & CANNA_YOMI_KATAKANA) {
    yc->generalFlags &= ~CANNA_YOMI_HANKAKU;
  }
  EmptyBaseModeInfo(d, yc);
  return 0;
}

EmptyBaseHan(d)
uiContext d;
{
  extern InhibitHankakuKana;
  yomiContext yc = (yomiContext)d->modec;

  if ((yc->generalFlags & CANNA_YOMI_CHGMODE_INHIBITTED) ||
      /* �⡼���ѹ����ػߤ���Ƥ���Ȥ� */
      (InhibitHankakuKana &&
       (yc->generalFlags & CANNA_YOMI_KATAKANA) &&
       !(yc->generalFlags & CANNA_YOMI_ROMAJI))) {
    /* Ⱦ�ѥ��ʤ��ػߤ���Ƥ���Τ�Ⱦ�ѥ��ʤˤ��������ʤȤ� */
    return NothingChangedWithBeep(d);
  }
  yc->generalFlags |= CANNA_YOMI_BASE_HANKAKU;
  if (yc->generalFlags & CANNA_YOMI_ROMAJI) {
    yc->generalFlags &= ~CANNA_YOMI_ZENKAKU;
  }
  /* ���� ���޻��⡼�ɤǤ��ĥ������ʥ⡼�ɤλ�������
          (���ξ��ɽ�̾�ϥ��޻��⡼��) */
  if (yc->generalFlags & CANNA_YOMI_KATAKANA) {
    if (!InhibitHankakuKana) {
      yc->generalFlags |= CANNA_YOMI_HANKAKU;
    }
  }
  EmptyBaseModeInfo(d, yc);
  return 0;
}

EmptyBaseKana(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  if ((yc->generalFlags & CANNA_YOMI_CHGMODE_INHIBITTED) ||
      /* �⡼���ѹ����ػߤ���Ƥ����� */
      (!InhibitHankakuKana && (yc->generalFlags & CANNA_YOMI_KATAKANA) &&
       (yc->generalFlags & CANNA_YOMI_BASE_HANKAKU))) {
    /* Ⱦ�ѥ��ʤ��ػߤ���Ƥ���Τ�Ⱦ�ѥ��ʤˤʤäƤ��ޤ������ʾ�� */
    return NothingChangedWithBeep(d);
  }
  yc->generalFlags &= ~(CANNA_YOMI_ROMAJI | CANNA_YOMI_ZENKAKU);

  if ((yc->generalFlags & CANNA_YOMI_BASE_HANKAKU) &&
      (yc->generalFlags & CANNA_YOMI_KATAKANA)) {
    yc->generalFlags |= CANNA_YOMI_HANKAKU;
  }
  EmptyBaseModeInfo(d, yc);
  return 0;
}

EmptyBaseKakutei(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  if (yc->generalFlags & CANNA_YOMI_CHGMODE_INHIBITTED) {
    return NothingChangedWithBeep(d);
  }
  yc->generalFlags |= CANNA_YOMI_KAKUTEI;

  EmptyBaseModeInfo(d, yc);
  return 0;
}

EmptyBaseHenkan(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  if (yc->generalFlags & CANNA_YOMI_CHGMODE_INHIBITTED) {
    return NothingChangedWithBeep(d);
  }
  yc->generalFlags &= ~CANNA_YOMI_KAKUTEI;

  EmptyBaseModeInfo(d, yc);
  return 0;
}

static int
renbunInit(d)
uiContext d;
{
  d->status = 0;
  killmenu(d);
  if (ToggleChikuji(d, 0) == -1) {
    jrKanjiError = "Ϣʸ���Ѵ������ؤ��뤳�Ȥ��Ǥ��ޤ���";
    makeGLineMessageFromString(d, jrKanjiError);
    currentModeInfo(d);
    return(-1);
  }
  else {
    makeGLineMessageFromString(d, "Ϣʸ���Ѵ������ؤ��ޤ���");
    currentModeInfo(d);
    return 0;
  }
}

static int
showVersion(d)
uiContext d;
{
  int retval = 0;
  char s[512];
  extern CannaVersion;

  d->status = 0;
  killmenu(d);

  sprintf(s, "���ܸ����ϥ����ƥ�ؤ���ʡ�Version %d.%d",
	  CannaVersion / 1000, CannaVersion % 1000);
  strcat(s, CANNA_PATCH_LEVEL);
  makeGLineMessageFromString(d, s);
  currentModeInfo(d);

  return (retval);
}

static int
showServer(d)
uiContext d;
{
  int retval = 0;
  char s[512];
  extern defaultContext;

  d->status = 0;
  killmenu(d);

  if(defaultContext == -1) {
    sprintf(s, "���ʴ����Ѵ������ФȤ���³���ڤ�Ƥ��ޤ�");
  }
  else {
    sprintf(s, "%s �Τ��ʴ����Ѵ������Ф���³���Ƥ��ޤ�", RkGetServerName());
  }
  makeGLineMessageFromString(d, s);
  currentModeInfo(d);

  return (retval);
}

static int
showGakushu(d)
uiContext d;
{
  int retval = 0;
  extern Gakushu;

  d->status = 0;
  killmenu(d);
  
  if (Gakushu == 1) {
    makeGLineMessageFromString(d, "�ؽ����ϣΤξ��֤Ǥ�");
  }
  else {
    makeGLineMessageFromString(d, "�ؽ����ϣƣƤξ��֤Ǥ�");
  }
    currentModeInfo(d);

  return (retval);
}

static int
showInitFile(d)
uiContext d;
{
  int retval = 0;
  char s[512];
  extern char *CANNA_initfilename;

  d->status = 0;
  killmenu(d);

  if (CANNA_initfilename && strlen(CANNA_initfilename)) {
    sprintf(s, "������ե������ %s ����Ѥ��Ƥ��ޤ�", CANNA_initfilename);
    makeGLineMessageFromString(d, s);
  }
  else {
    sprintf(s, "�������ޥ����ե���������ꤵ��Ƥ��ޤ���");
    makeGLineMessageFromString(d, s);
  }
    currentModeInfo(d);

  return (retval);
}

static int
showRomkanaFile(d)
uiContext d;
{
  int retval = 0;
  char s[512];
  extern char *RomkanaTable;

  d->status = 0;
  killmenu(d);
  
  if (RomkanaTable && romajidic) {
    sprintf(s, "���޻������Ѵ��ơ��֥�� %s ����Ѥ��Ƥ��ޤ�",
	    RomkanaTable);
    makeGLineMessageFromString(d, s);
  }
  else {
    sprintf(s, "���޻������Ѵ��ơ��֥�ϻ��Ѥ���Ƥ��ޤ���");
    makeGLineMessageFromString(d, s);
  }
    currentModeInfo(d);

  return (retval);
}

static int
dicSync(d)
uiContext d;
{
  int retval = 0;
  char s[512];
  extern defaultContext;

  d->status = 0;
  killmenu(d);

  retval = RkwSync(defaultContext, "");
  sprintf(s, "����� Sync ����%s",
	  retval < 0 ? "�˼��Ԥ��ޤ���" : "��Ԥ��ޤ���");
  makeGLineMessageFromString(d, s);
  currentModeInfo(d);

  return 0;
}

#include "emptymap.c"
#include "alphamap.c"
