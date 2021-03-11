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
static char rcs_id[] = "@(#) 102.1 $Id: kctrl.c,v 7.32 1994/04/21 02:39:40 kon Exp $";
#endif /* lint */

#include "canna.h"

#include <errno.h>
#include <sys/types.h>
#include <canna/mfdef.h>

#define DEFAULT_COLUMN_WIDTH	70

extern char *CANNA_initfilename;
extern char saveapname[];

static int insertEmptySlots pro((uiContext));
static int callCallback pro((uiContext, int));

static
doInitializeFunctions(d)
uiContext d;
{
  BYTE *p;
  int res = 0;
  wcKanjiStatus ks, *pks;
  extern BYTE *initfunc;
  wchar_t xxxx[10];

  d->ch = 0;
  d->buffer_return = xxxx;
  d->n_buffer = sizeof(xxxx) / sizeof(wchar_t);
  d->nbytes = 0;

  if (initfunc) {
    pks = d->kanji_status_return;
    d->kanji_status_return = &ks;
    for (p = initfunc ; *p ; p++) {
      res = _doFunc(d, *p);
    }
    res = _afterDoFunc(d, res);
    d->kanji_status_return = pks;
  }
  return res;
}

/* uiContext �ν���� */

initRomeStruct(d, flg)
     uiContext	d;
     int	flg;
{
  extern KanjiModeRec alpha_mode, empty_mode;
  extern KanjiModeRec kzhr_mode, kzkt_mode, kzal_mode;
  extern KanjiModeRec khkt_mode, khal_mode;
  yomiContext yc;
  extern defaultContext, defaultBushuContext;

  bzero(d, sizeof(uiContextRec));

  if (insertEmptySlots(d) < 0) {
    return -1;
  }

  /* ����ƥ����Ȥ�ǽ�˥ǥ�ץꥱ���Ȥ��Ƥ��������ä��ʤ� */
  d->contextCache = -1;

  /* ����⡼��(����ե��٥åȥ⡼��)������� */
  d->majorMode = d->minorMode = CANNA_MODE_AlphaMode;
  yc = (yomiContext)d->modec;
  if (flg) {
    yc->minorMode = CANNA_MODE_ChikujiYomiMode;
    yc->generalFlags |= CANNA_YOMI_CHIKUJI_MODE;
  }
  alphaMode(d);

  /* ��� func ��¹Ԥ��� */

  (void)doInitializeFunctions(d);
  return 0;
}

static void
freeModec(modec)
mode_context modec;
{
  coreContext cc;
  union {
    coreContext c;
    yomiContext y;
    ichiranContext i;
    forichiranContext f;
    mountContext m;
    tourokuContext t;
  } gc;

  cc = (coreContext)modec;
  while (cc) {
    switch (cc->id) {
    case CORE_CONTEXT:
      gc.c = cc;
      cc = (coreContext)gc.c->next;
      freeCoreContext(gc.c);
      break;
    case YOMI_CONTEXT:
      gc.y = (yomiContext)cc;
      cc = (coreContext)gc.y->next;
      freeYomiContext(gc.y);
      break;
    case ICHIRAN_CONTEXT:
      gc.i = (ichiranContext)cc;
      cc = (coreContext)gc.i->next;
      freeIchiranContext(gc.i);
      break;
    case FORICHIRAN_CONTEXT:
      gc.f = (forichiranContext)cc;
      cc = (coreContext)gc.f->next;
      freeForIchiranContext(gc.f);
      break;
    case MOUNT_CONTEXT:
      gc.m = (mountContext)cc;
      cc = (coreContext)gc.m->next;
      freeIchiranContext(gc.i);
      break;
    case TOUROKU_CONTEXT:
      gc.t = (tourokuContext)cc;
      cc = (coreContext)gc.t->next;
      free(gc.t);
      break;
    default:
      break;
    }
  }
}

static void
freeCallbacks(cb)
struct callback *cb;
{
  struct callback *nextcb;

  for (; cb ; cb = nextcb) {
    nextcb = cb->next;
    free(cb);
  }
}

void
freeRomeStruct(d)
uiContext d;
{
  freeModec(d->modec);
  if (d->cb) {
    freeCallbacks(d->cb);
  }
  if (d->contextCache >= 0) {
    if (RkwCloseContext(d->contextCache) < 0) {
      if (errno == EPIPE) {
	jrKanjiPipeError();
      }
    }
  }
  freeAllMenuInfo(d->minfo);
  if (d->selinfo) {
    selectinfo *p, *q;

    for (p = d->selinfo ; p ; p = q) {
      q = p->next;
      free((char *)p);
    }
  }
  free(d);
}

static
insertEmptySlots(d)
uiContext d;
{
  extern KanjiModeRec	empty_mode;
  struct callback	*pushCallback();
  yomiContext		yc;

  if (pushCallback(d, NULL, NO_CALLBACK, NO_CALLBACK,
		   NO_CALLBACK, NO_CALLBACK) == (struct callback *)0)
    return NoMoreMemory();

  yc = newYomiContext(0, 0, /* ��̤ϳ�Ǽ���ʤ� */
		      CANNA_NOTHING_RESTRICTED,
		      !CANNA_YOMI_CHGMODE_INHIBITTED,
		      !CANNA_YOMI_END_IF_KAKUTEI,
		      CANNA_YOMI_INHIBIT_NONE);
  if (yc == (yomiContext)0) {
    popCallback(d);
    return NoMoreMemory();
  }
  yc->majorMode = yc->minorMode = CANNA_MODE_HenkanMode;
  d->majorMode = d->minorMode = CANNA_MODE_HenkanMode;
  d->modec = (mode_context)yc;

  d->current_mode = yc->curMode = yc->myEmptyMode = &empty_mode;
  yc->romdic = romajidic;
  d->ncolumns = DEFAULT_COLUMN_WIDTH;
  d->minfo = (menuinfo *)0;
  d->selinfo = (selectinfo *)0;
  d->prevMenu = (menustruct *)0;
  return 0;
}

/* 

  display �� window ���Ȥ� ����ƥ�����ID ��ºݤΥ���ƥ����Ȥ��б���
  ���뤿��Υϥå���ơ��֥�

  display �� window �������륭���ΤȤ��������ȡ������˥���ƥ�����
  �����äƤ����Ψ���⤤���⤷���äƤ��ʤ��Ȥ⡢�ݥ��󥿥�������򤿤ɤä�
  �Ԥ��Ȥ��Ĥ��ϥ���ƥ����Ȥ�������˰㤤�ʤ���

 */

#define HASHTABLESIZE 96

static struct bukRec {
  unsigned int data1, data2;
  uiContext context;
  struct bukRec *next;
} *conHash[HASHTABLESIZE];

/* �ϥå���ơ��֥��Ĵ�٤ƥ���ƥ����Ȥ����뤫�ɤ���Ĵ�٤�ؿ� */

static
countContext()
{
  struct bukRec *hash;

  int i, c;
  for(i = 0, c = 0; i < HASHTABLESIZE; i++) {
    for(hash = conHash[i] ; hash && hash->context ;hash = hash->next){
      c++;
    }
  }
#ifdef DEBUG
  fprintf(stderr, "���=%d\n", c);
#endif
  if(c) {
    return 0;
  }
  else {
    return 1;
  }
}

/* �ϥå��奭������ؿ�(�����ø�) */

static unsigned int
makeKey(data1, data2)
unsigned int data1, data2;
{
  unsigned int key;

  key = data1 % HASHTABLESIZE;
  key += data2 % HASHTABLESIZE;
  key %= HASHTABLESIZE;
  return key;
}

/* 

  keyToContext -- Display �� Window ���Ȥʤɤ��饳��ƥ����Ȥ���Ф�����

  display �� window ���Ȥ�����ƥ����Ȥ���äƤ���Ф��Υ���ƥ�����
  ���֤���

  ���äƤ��ʤ��ΤǤ���С�NULL ���֤���

  */

uiContext 
keyToContext(data1, data2)
unsigned int data1, data2;
{
  unsigned int key;
  struct bukRec *p;

  key = makeKey(data1, data2);
  for (p = conHash[key] ; p ; p = p->next) {
    if (p->data1 == data1 && p->data2 == data2) {
      /* ����㤢����ƥ����Ȥ����Ĥ���ޤ����� */
      return p->context;
    }
  }
  return (uiContext)0; /* ���Ĥ���ޤ���Ǥ����� */
}


/* internContext -- �ϥå���ơ��֥����Ͽ���� 

  ���ΤȤ������ˡ�display �� window ���Ȥ�¸�ߤ���ΤǤ���С�
  ������ˤĤʤ��äƤ��륳��ƥ����Ȥ�ե꡼����Τ���ա���

*/

struct bukRec *
internContext(data1, data2, context)
unsigned int data1, data2;
uiContext context;
{
  unsigned int key;
  struct bukRec *p, **pp;

  key = makeKey(data1, data2);
  for (pp = &conHash[key]; (p = *pp) != (struct bukRec *)0; pp = &(p->next)) {
    if (p->data1 == data1 && p->data2 == data2) {
      freeRomeStruct(p->context);
      p->context = context;
      return p;
    }
  }
  p = *pp = (struct bukRec *)malloc(sizeof(struct bukRec));
  if (p) {
    p->data1 = data1;
    p->data2 = data2;
    p->context = context;
    p->next = (struct bukRec *)0;
  }
  return p;
}


/* rmContext -- �ϥå���ơ��֥뤫��������

*/

void
rmContext(data1, data2)
unsigned int data1, data2;
{
  unsigned int key;
  struct bukRec *p, *q, **pp;

  key = makeKey(data1, data2);
  pp = &conHash[key];
  for (p = *pp ; p ; p = q) {
    q = p->next;
    if (p->data1 == data1 && p->data2 == data2) {
      *pp = q;
      free(p);
    }
    else {
      pp = &(p->next);
    }
  }
}

/* cfuncdef

  freeBukRecs() -- �ݥ���Ȥ���Ƥ�����ΥХ��åȤΥե꡼

  �Х��åȤˤ�äƥݥ���Ȥ���Ƥ���ǡ����򤹤٤ƥե꡼���롣
  �ե꡼���оݤˤ� uiContext ��ޤޤ�롣

*/

static void
freeBukRecs(p)
struct bukRec *p;
{
  struct bukRec *nextp;

  if (p) { /* reconfirm that p points some structure */
    freeRomeStruct(p->context);
    nextp = p->next;
    if (nextp) {
      freeBukRecs(nextp);
    }
    free(p);
  }
}

/* cfuncdef

  clearHashTable() -- �ϥå���ơ��֥�����Ƥ򤹤٤ƥե꡼���롣

*/

static void
clearHashTable()
{
  int i;
  struct bukRec *p;

  for (i = 0 ; i < HASHTABLESIZE ; i++) {
    p = conHash[i];
    conHash[i] = 0;
    if (p) {
      freeBukRecs(p);
    }
  }
}

#define NWARNINGMESG 64
static char *WarningMesg[NWARNINGMESG + 1]; /* +1 �ϺǸ�� NULL �ݥ��󥿤�ʬ */
static int nWarningMesg = 0;

static void
initWarningMesg()
{
  int i;

  for (i = 0 ; i < nWarningMesg ; i++) {
    free(WarningMesg[i]);
    WarningMesg[i] = (char *)0;
  }
  nWarningMesg = 0;
}

void
addWarningMesg(s)
char *s;
{
  int n;
  char *work;

  if (nWarningMesg < NWARNINGMESG) {
    n = strlen(s);
    work = (char *)malloc(n + 1);
    if (work) {
      strcpy(work, s);
      WarningMesg[nWarningMesg++] = work;
    }
  }
}

static int
KC_keyconvCallback(d, arg)
uiContext d;
char *arg;
/* ARGSUSED */
{
  extern void (*keyconvCallback)();

  if (arg) {
    keyconvCallback = (void (*)())arg;
  }
  else {
    keyconvCallback = (void (*)())0;
  }
  return 0;
}

static
KC_initialize(d, arg)
     uiContext d;
     char *arg;
     /* ARGSUSED */
{
  extern FirstTime;

  if (FirstTime) {

#ifdef ENGINE_SWITCH
    RkSetServerName((char *)0);
#endif

    if (WCinit() < 0) {
      /* locale �Ķ����Խ�ʬ�����ܸ����Ϥ��Ǥ��ʤ� */
      jrKanjiError =
	"The locale database is insufficient for Japanese input system.";
      if (arg) *(char ***)arg = (char **)0;
      return -1;
    }

    debug_message("KC_INITIALIZE �������ˤ��뤾\n",0,0,0);

    if (initExtMenu() < 0) {
      jrKanjiError = "Insufficient memory.";
      if (arg) *(char ***)arg = (char **)0;
      return -1;
    }

    /* �磻�ɥ���饯���ѥ�������ν���� */

    WStringOpen();

    initBushuTable();

    initGyouTable();

    initHinshiTable();

    initUlKigoTable();

    initUlKeisenTable();

    initOnoffTable();

    initKigoTable();

    initHinshiMessage();

    /* �������˥󥰥�å������ν���� */
    initWarningMesg();

    /* �⡼��̾�ν���� */
    initModeNames();

    /* �����ơ��֥�ν���� */
    initKeyTables();

    /* ������ե�������ɤ߹��� */
    parse();

    /* keyconvCallback �� parse ������פʤΤǥ��ꥢ���� */
    KC_keyconvCallback(d, (char *)0);

    /* �����ط�ʸ����ν���� */
    initIchiran();

    /* ���޻������Ѵ��ν���� */
    RomkanaInit();

    /* ���ʴ����Ѵ��ν���� */
    KanjiInit();
    /* �����Ǥ⥨�顼��̵�뤷�ޤ��������ˤʤ�ʤ��Ƥ⤤������ */

    if (arg) {
      *(char ***)arg = nWarningMesg ? WarningMesg : (char **)0;
    }
    FirstTime = 0;
    return 0;
  }
  else {
    /* ����Initialize�򤷤Ƥ�����ˤϤ⤦��å�����������ʤ����Ȥˤ��� */
    if (arg) {
      *(char ***)arg = (char **)0;
    }
    return -1;
  }
}

static void
freeKeysup()
{
  int i;
  extern keySupplement keysup[];
  extern int nkeysup;

  for (i = 0 ; i < nkeysup ; i++) {
    if (keysup[i].cand) {
      free((char *)keysup[i].cand);
      keysup[i].cand = (wchar_t **)0;
    }
    if (keysup[i].fullword) {
      free((char *)keysup[i].fullword);
      keysup[i].fullword = (wchar_t *)0;
    }
  }
  nkeysup = 0;
}

extern void restoreBindings();
extern  InhibitHankakuKana;
extern int nothermodes;

static
freeBuffer()
{
  if(CANNA_initfilename) {
    free(CANNA_initfilename);
  }
  CANNA_initfilename = NULL;
}

static
freeExtra()
{
  extern extraFunc *extrafuncp;
  extraFunc *p, *q;

  for (p = extrafuncp ; p ; p = q) {
    q = p->next;
    switch (p->keyword) {
      case EXTRA_FUNC_DEFMODE:
        if (p->u.modeptr->romdic_owner &&
	    p->u.modeptr->romdic != NULL) {
	  RkwCloseRoma(p->u.modeptr->romdic);
	}
        free((char *)p->u.modeptr->emode);
	if (p->u.modeptr->romaji_table) {
	  free((char *)p->u.modeptr->romaji_table);
	}
        free((char *)p->u.modeptr);
        break;
      case EXTRA_FUNC_DEFSELECTION:
        free((char *)p->u.kigoptr->kigo_str);
        free((char *)p->u.kigoptr->kigo_data);
        free((char *)p->u.kigoptr);
        break;
      case EXTRA_FUNC_DEFMENU:
        freeMenu(p->u.menuptr);
        break;
    }
    free((char *)p);
  }
  extrafuncp = (extraFunc *)0;
}

static
KC_finalize(d, arg)
     uiContext d;
     char *arg;
     /* ARGSUSED */
{
  extern FirstTime;
  int res;
  
  /* �������˥󥰥�å������ν���� */
  initWarningMesg();
  if (arg) {
    *(char ***)arg = 0;
  }

  if (FirstTime) {
    jrKanjiError = "������⤵��Ƥ��ʤ��Τˡؽ����٤ȸ����ޤ���";
    return -1;
  }
  else {
    FirstTime = 1;

    /* ���ʴ����Ѵ��ν�λ */
    res = KanjiFin();

    /* ���޻������Ѵ��ν�λ */
    RomkanaFin();

    /* free all uiContexts and hash tables here */

    /* �����ޥåץơ��֥�Υ��ꥢ */
    restoreDefaultKeymaps();

    /* �⡼��ʸ����Υե꡼ */
    resetModeNames();

    /* uiContext �θ����Τ���Υϥå���ơ��֥�򥯥ꥢ
       uiContext ����˥ե꡼���� */
    clearHashTable();

    /* ���������ä� */
    freeKeysup();

    /* �����ȥ������ޥ�������Ƥ������Ȥ��᤹�� */
    restoreBindings();

    /* ��ĥ��ǽ��initfileɽ���ѤΥХåե���������� */
    freeBuffer();

    /* �磻�ɥ���饯���Ѽ�ư��������ν�λ���� */
    WStringClose();

    /* ������̾�ݻ��ѥ���γ��� */
    /* RkSetServerName((char *)0); ���ƤϤ����ʤ��ΤǤϡ� */

    /* ��˥塼��Ϣ�Υ���γ��� */
    finExtMenu();

    /* �ǥե���Ȱʳ��Υ⡼���ѥ���γ��� */
    freeExtra();

    /* ���󥸥�Υ����� */
#ifdef ENGINE_SWITCH
    close_engine();
#endif

    if (arg) {
      *(char ***)arg = nWarningMesg ? WarningMesg : (char **)0;
    }
    return res;
  }
}

static
KC_setWidth(d, arg)
uiContext d;
caddr_t arg;
{
  d->ncolumns = (int)(POINTERINT)arg;
  return 0;
}

static
KC_setBunsetsuKugiri(d, arg)
     uiContext d;
     caddr_t arg;
     /* ARGSUSED */
{
  extern BunsetsuKugiri;

  BunsetsuKugiri = (int)(POINTERINT)arg;
  return 0;
}

#define CHANGEBUFSIZE 1024

static long gflags[] = {
  0,
  CANNA_YOMI_BASE_HANKAKU,
  CANNA_YOMI_KATAKANA,
  CANNA_YOMI_KATAKANA | CANNA_YOMI_HANKAKU | CANNA_YOMI_BASE_HANKAKU,
  CANNA_YOMI_ROMAJI | CANNA_YOMI_ZENKAKU,
  CANNA_YOMI_ROMAJI | CANNA_YOMI_BASE_HANKAKU,
  CANNA_YOMI_KAKUTEI,
  CANNA_YOMI_BASE_HANKAKU | CANNA_YOMI_KAKUTEI,
  CANNA_YOMI_KATAKANA | CANNA_YOMI_KAKUTEI,
  CANNA_YOMI_KATAKANA | CANNA_YOMI_HANKAKU | CANNA_YOMI_BASE_HANKAKU |
    CANNA_YOMI_KAKUTEI,
  CANNA_YOMI_ROMAJI | CANNA_YOMI_ZENKAKU | CANNA_YOMI_KAKUTEI,
  CANNA_YOMI_ROMAJI | CANNA_YOMI_BASE_HANKAKU | CANNA_YOMI_KAKUTEI,
};

static
KC_changeMode(d, arg)
uiContext d;
wcKanjiStatusWithValue *arg;
{
  coreContext cc;
  yomiContext yc;

  d->buffer_return = arg->buffer;
  d->n_buffer = arg->n_buffer;
  d->kanji_status_return = arg->ks;

  bzero(d->kanji_status_return, sizeof(wcKanjiStatus));

  d->nbytes = escapeToBasicStat(d, CANNA_FN_Quit);
  cc = (coreContext)d->modec;
  d->kanji_status_return->info &= ~(KanjiThroughInfo | KanjiEmptyInfo);

  if (cc->majorMode == CANNA_MODE_AlphaMode) {
    /* ���⡼�ɤ��ä���ȴ���롣
       �١����å��⡼�ɤϦ��⡼�ɤ��Ѵ��⡼�ɤ��餤�����ʤ��Ȼפ��� */
    if (arg->val == CANNA_MODE_AlphaMode) {
      return 0;
    }
    else {
      cc = (coreContext)cc->next; /* ���Υ���ƥ����� */
      yc = (yomiContext)cc;

      if (yc->generalFlags & CANNA_YOMI_CHGMODE_INHIBITTED) {
	Beep();
	arg->val = 0;
	return 0;
      }

      doFunc(d, CANNA_FN_JapaneseMode);
    }
  }
  else {
    yc = (yomiContext)cc;

    if (yc->generalFlags & CANNA_YOMI_CHGMODE_INHIBITTED) {
      Beep();
      arg->val = 0;
      return 0;
    }
  }

  switch (arg->val) {
  case CANNA_MODE_AlphaMode:
    arg->val = doFunc(d, CANNA_FN_AlphaMode);
    return 0;

  case CANNA_MODE_HenkanMode:
    arg->val = doFunc(d, CANNA_FN_HenkanNyuryokuMode);
    return 0;

  case CANNA_MODE_HexMode:
    arg->val = doFunc(d, CANNA_FN_HexMode);
    return 0;

  case CANNA_MODE_BushuMode:
    arg->val = doFunc(d, CANNA_FN_BushuMode);
    return 0;

  case CANNA_MODE_KigoMode:
    arg->val = doFunc(d, CANNA_FN_KigouMode);
    return 0;

  case CANNA_MODE_TourokuMode:
    arg->val = doFunc(d, CANNA_FN_ExtendMode);
    return 0;

  case CANNA_MODE_HanKataHenkanMode:
  case CANNA_MODE_HanKataKakuteiMode:
    if (InhibitHankakuKana) {
      Beep();
      arg->val = 0;
      return 0;
    }
  case CANNA_MODE_ZenHiraHenkanMode:
  case CANNA_MODE_HanHiraHenkanMode:
  case CANNA_MODE_ZenKataHenkanMode:
  case CANNA_MODE_ZenAlphaHenkanMode:
  case CANNA_MODE_HanAlphaHenkanMode:
  case CANNA_MODE_ZenHiraKakuteiMode:
  case CANNA_MODE_HanHiraKakuteiMode:
  case CANNA_MODE_ZenKataKakuteiMode:
  case CANNA_MODE_ZenAlphaKakuteiMode:
  case CANNA_MODE_HanAlphaKakuteiMode:
    yc->generalFlags &= ~(CANNA_YOMI_ATTRFUNCS | CANNA_YOMI_BASE_HANKAKU);
    yc->generalFlags |= gflags[arg->val - CANNA_MODE_ZenHiraHenkanMode];
    EmptyBaseModeInfo(d, yc);
    arg->val = 0;
    return 0;
  default:
    return(-1);
  }
  /* NOTREACHED */
}

static
baseModeP(d)
uiContext d;
{
  extern KanjiModeRec alpha_mode, empty_mode;

  return (d->current_mode == &alpha_mode) ||
    (d->current_mode == &empty_mode
     && ((yomiContext)(d->modec))->next == (mode_context)0);
}

/*

  ����Ū�ʾ��֤ˤ�ɤ롣���ʤ�����ɤߤ����äƤ������Ѵ���ξ��֤���ȴ
  ���롣������ȴ���뤫���裲�����ǻ��ꤹ�롣ȴ�����Ȥ��Ƥ�

  ��QUIT (C-g) ��ȴ����
  ������ (Return) ��ȴ����

  �����롣

*/

escapeToBasicStat(d, how)
uiContext d;
int how;
{
  int len = 0, totallen = 0;
  wchar_t *p = d->buffer_return;
  int totalinfo = 0;
  int maxcount = 32;

  do {
    if(d->kanji_status_return) {
      d->kanji_status_return->length = 0;
      totalinfo |= (d->kanji_status_return->info & KanjiModeInfo);
    }
    else {
      return -1;
    }
    d->kanji_status_return->info = 0;
    d->nbytes = 0; /* �����ͤ�����ʸ��Ĺ�Ȥ��ƻȤ���礬����Τǥ��ꥢ���� */
    len = doFunc(d, how);
    d->buffer_return += len;
    d->n_buffer -= len;
    totallen += len;
    maxcount--;
  } while (maxcount > 0 && !baseModeP(d));
  d->kanji_status_return->info |= KanjiGLineInfo | totalinfo;
  d->kanji_status_return->gline.length = 0;
  d->kanji_status_return->gline.revPos = 0;
  d->kanji_status_return->gline.revLen = 0;
  d->buffer_return = p;
  return totallen;
}

static
KC_setUFunc(d, arg)
     uiContext d;
     caddr_t arg;
     /* ARGSUSED */
{
  extern howToBehaveInCaseOfUndefKey;

  howToBehaveInCaseOfUndefKey = (int)(POINTERINT)arg;
  return 0;
}

static
KC_setModeInfoStyle(d, arg)
     uiContext d;
     caddr_t arg;
     /* ARGSUSED */
{
  int	tmpval;
  extern howToReturnModeInfo;

  if ((tmpval = (int)(POINTERINT)arg) < 0 || tmpval > MaxModeInfoStyle)
    return(-1);
  howToReturnModeInfo = (int)(POINTERINT)arg;
  return 0;
}

static
KC_setHexInputStyle(d, arg)
     uiContext d;
     caddr_t arg;
     /* ARGSUSED */
{
  extern hexCharacterDefiningStyle;

  hexCharacterDefiningStyle = (int)(POINTERINT)arg;
  return 0;
}

static
KC_inhibitHankakuKana(d, arg)
     uiContext d;
     caddr_t arg;
     /* ARGSUSED */
{
  extern InhibitHankakuKana;

  InhibitHankakuKana = (int)(POINTERINT)arg;
  return 0;
}

static
popTourokuWithGLineClear(d, retval, env)
     uiContext d;
     int retval;
     mode_context env;
     /* ARGSUSED */
{
  tourokuContext tc;

  popCallback(d); /* �ɤߤ� pop */

  tc = (tourokuContext)d->modec;
  if (tc->udic) {
    free(tc->udic);
  }
  popTourokuMode(d);
  popCallback(d);
  GlineClear(d);
  currentModeInfo(d);
  return 0;
}


static
KC_defineKanji(d, arg)
uiContext d;
wcKanjiStatusWithValue *arg;
{
  d->buffer_return = arg->buffer;
  d->n_buffer = arg->n_buffer;
  d->kanji_status_return = arg->ks;

  if(arg->ks->length > 0 && arg->ks->echoStr && arg->ks->echoStr[0]) {
    wchar_t xxxx[ROMEBUFSIZE];

    WStrncpy(xxxx, arg->ks->echoStr, arg->ks->length);
    xxxx[arg->ks->length] = (wchar_t)0;
    
    bzero(d->kanji_status_return, sizeof(wcKanjiStatus));

    d->nbytes = escapeToBasicStat(d, CANNA_FN_Quit);
    d->kanji_status_return->info &= ~(KanjiThroughInfo | KanjiEmptyInfo);
    dicTourokuControl(d, xxxx, popTourokuWithGLineClear);
    arg->val = d->nbytes;
  } else {
    d->nbytes = escapeToBasicStat(d, CANNA_FN_Quit);
    d->kanji_status_return->info &= ~(KanjiThroughInfo | KanjiEmptyInfo);
    arg->val = dicTourokuControl(d, 0, popTourokuWithGLineClear);
  }
  arg->val = callCallback(d, arg->val);

  return 0;
}


/* cfuncdef

  RK ����ƥ����Ȥ�̵���ˤ��롣
  flag �����ʳ��ʤ� RkwClose() ��Ԥ���

 */

static void
closeRK(cxp, flag)
int *cxp;
int flag;
{
  if (flag && *cxp >= 0) {
    RkwCloseContext(*cxp);
  }
  *cxp = -1;
}

/* cfuncdef

   closeRKContextInUIContext -- uiContext ��� RK ����ƥ����Ȥ� close ���롣

 */

static
closeRKContextInUIContext(d, flag)
uiContext d;
int flag; /* ���ʳ��ʤ饯�����⤹�롣 */
{
  coreContext cc;

  closeRK(&(d->contextCache), flag);
  for (cc = (coreContext)d->modec ; cc ; cc = (coreContext)cc->next) {
    if (cc->id == YOMI_CONTEXT) {
      closeRK(&(((yomiContext)cc)->context), flag);
    }
  }
}

/* cfuncdef

  closeRKContextInMemory() -- ���٤Ƥ� RK ����ƥ����ȤΥ�����

  �Х��åȤˤ�äƥݥ���Ȥ���Ƥ���ǡ���������Ƥ� RK ����ƥ����Ȥ�
  ���������롣

*/

static void
closeRKContextInMemory(p, flag)
struct bukRec *p;
int flag;
{

  while (p) { /* reconfirm that p points some structure */
    closeRKContextInUIContext(p->context, flag);
    p = p->next;
  }
}

/* cfuncdef

  makeContextToBeClosed() -- �ϥå���ơ��֥���Υ���ƥ����Ȥ�̵���ˤ���

*/

void
makeAllContextToBeClosed(flag)
int flag;
{
  int i;
  struct bukRec *p;

  for (i = 0 ; i < HASHTABLESIZE ; i++) {
    p = conHash[i];
    if (p) {
      closeRKContextInMemory(p, flag);
    }
  }
}

static
KC_kakutei(d, arg)
uiContext d;
wcKanjiStatusWithValue *arg;
{
  d->buffer_return = arg->buffer;
  d->n_buffer = arg->n_buffer;
  d->kanji_status_return = arg->ks;

  bzero(d->kanji_status_return, sizeof(wcKanjiStatus));

  d->nbytes = escapeToBasicStat(d, CANNA_FN_Kakutei);
  if ( !baseModeP(d) ) {
    d->nbytes = escapeToBasicStat(d, CANNA_FN_Quit);
  }
  d->kanji_status_return->info &= ~KanjiThroughInfo;
  arg->val = d->nbytes;
  return d->nbytes;
}

static
KC_kill(d, arg)
uiContext d;
wcKanjiStatusWithValue *arg;
{
  d->buffer_return = arg->buffer;
  d->n_buffer = arg->n_buffer;
  d->kanji_status_return = arg->ks;

  bzero(d->kanji_status_return, sizeof(wcKanjiStatus));

  d->nbytes = escapeToBasicStat(d, CANNA_FN_Quit);
  d->kanji_status_return->info &= ~KanjiThroughInfo;
  arg->val = d->nbytes;
  return d->nbytes;
}

static
KC_modekeys(d, arg)
uiContext d;
unsigned char *arg;
{
  int n = 0;
  int i;
  extern KanjiModeRec alpha_mode;
  int func;

  for (i = 0 ; i < 256 ; i++) {
    func = alpha_mode.keytbl[i];
    if (func != CANNA_FN_SelfInsert &&
	func != CANNA_FN_FunctionalInsert &&
	func != CANNA_FN_Undefined &&
	func != CANNA_FN_FuncSequence &&
	func != CANNA_FN_UseOtherKeymap	&&
	alpha_mode.func(d, &alpha_mode, KEY_CHECK, 0/*dummy*/, func)) {
      arg[n++] = i;
    }
  }
  return n;
}

static
KC_queryMode(d, arg)
uiContext d;
wchar_t *arg;
{
  return queryMode(d, arg);
}

static
KC_queryConnection(d, arg)
     uiContext d;
     unsigned char *arg;
     /* ARGSUSED */
{
  extern defaultContext;

  if (defaultContext != -1) {
    return 1;
  }
  else {
    return 0;
  }
}

static
KC_setServerName(d, arg)
     uiContext d;
     unsigned char *arg;
     /* ARGSUSED */
{
  return RkSetServerName((char *)arg);
}

static
KC_parse(d, arg)
     uiContext d;
     char **arg;
     /* ARGSUSED */
{
  initWarningMesg();

  parse_string(*arg);

  *(char ***)arg = nWarningMesg ? WarningMesg : (char **)0;

  return nWarningMesg;
}

int yomiInfoLevel = 0;

static
KC_yomiInfo(d, arg)
     uiContext d;
     int arg;
     /* ARGSUSED */
{
  yomiInfoLevel = arg;
  return 0;
}

static
KC_storeYomi(d, arg)
uiContext d;
wcKanjiStatusWithValue *arg;
{
  extern KanjiModeRec yomi_mode, cy_mode;
  coreContext cc;
  wchar_t buf[2048];
  wchar_t *p, *q;
  int len = 0;

  p = arg->ks->echoStr;
  q = arg->ks->mode;
  if (p) {
    WStrcpy(buf, p);
    p = buf;
    len = WStrlen(buf);
  }
  if (q) {
    WStrcpy(buf + len + 1, q);
    q = buf + len + 1;
  }
  KC_kill(d, arg);
  cc = (coreContext)d->modec;
  if (cc->majorMode == CANNA_MODE_AlphaMode) {
    doFunc(d, CANNA_FN_JapaneseMode);
  }
  d->kanji_status_return = arg->ks;
  d->kanji_status_return->info &= ~(KanjiThroughInfo | KanjiEmptyInfo);
  RomajiStoreYomi(d, p, q);
  if (p && *p) {
    d->current_mode =
      (((yomiContext)d->modec)->generalFlags & CANNA_YOMI_CHIKUJI_MODE)
	? &cy_mode : &yomi_mode;
  }
  makeYomiReturnStruct(d);
  arg->val = 0;
  return 0;
}

char *initFileSpecified = (char *)0;

static
KC_setInitFileName(d, arg)
     uiContext d;
     char *arg;
     /* ARGSUSED */
{
  int len;

  if (initFileSpecified) { /* �����Τ�Τ�ե꡼���� */
    free(initFileSpecified);
  }

  if ( arg && *arg ) {
    len = strlen(arg);
    initFileSpecified = malloc(len + 1);
    if (initFileSpecified) {
      strcpy(initFileSpecified, arg);
    }
    else {
      return -1;
    }
  }
  else {
    initFileSpecified = (char *)0;
  }
  return 0;
}

static
KC_do(d, arg)
uiContext d;
wcKanjiStatusWithValue *arg;
{
  d->buffer_return = arg->buffer;
  d->n_buffer = arg->n_buffer;
  d->kanji_status_return = arg->ks;
  d->ch = *(d->buffer_return);
  d->nbytes = 1;

  bzero(d->kanji_status_return, sizeof(wcKanjiStatus));

  arg->val = doFunc(d, arg->val);
  return 0;
}

/*

  �ȥåץ�٥�ˤϤʤ��⡼�ɤ��Ф��Ʋ��餫�κ�Ȥ�
  ���������Ȥ��˸ƤӽФ��ؿ���fnum == 0 �� d->ch �򸫤롣

  '91.12.28 ���ߤ� uldefine.c ���餷���ƤФ�Ƥ��餺 modec ���ͤ�
  yomi_mode �������äƤ��ʤ���

  ���δؿ��Ϥ�����ǸƤִؿ��ˤ�äƥ⡼�ɤ��ץå��夵�줿���ʤ�������
  ���뤳�Ȥ�����ʤ��ʤäƤ��ޤ��Τǥ⡼���ѹ���ȼ����ǽ�θƽФ�����
  Ū�˶ػߤ��뤳�ȤȤ��롣

 */

_do_func_slightly(d, fnum, mode_c, c_mode)
uiContext d;
int fnum;
mode_context mode_c;
KanjiMode c_mode;
{
  uiContextRec e;
  wcKanjiStatus ks;
  long gfback;
  BYTE inhback;
  int retval;
  yomiContext yc = (yomiContext)0;

  bzero(&e, sizeof(uiContextRec));
  e.buffer_return = e.genbuf;
  e.n_buffer = ROMEBUFSIZE;
  e.kanji_status_return = &ks;

  e.nbytes = d->nbytes;
  e.ch     = d->ch;

  e.status = 0; /* �⡼�ɤˤĤ���"������"�Υ��ơ�����������ͤȤ��� */
  e.more.todo = 0;
  e.modec = mode_c;
  e.current_mode = c_mode;
  e.cb = (struct callback *)0;

  if (((coreContext)mode_c)->id == YOMI_CONTEXT) {
    yc = (yomiContext)mode_c;
    gfback = yc->generalFlags;
    inhback = yc->henkanInhibition;
    yc->generalFlags |= CANNA_YOMI_CHGMODE_INHIBITTED;
    yc->henkanInhibition |= CANNA_YOMI_INHIBIT_ALL;
  }

  retval = c_mode->func(&e, c_mode, KEY_CALL, (int)e.ch, fnum);

  if (yc) {
    yc->generalFlags = gfback;
    yc->henkanInhibition = inhback;
  }

  return retval;
}

static
callCallback(d, res)  /* ������Хå���Ƥ� */
uiContext d;
int res;
{
  struct callback *cbp;

  for (cbp = d->cb; cbp ;) {
    int index;
    int (*callbackfunc) pro((uiContext, int, mode_context));

    index = d->status;
    d->status = 0; /* Callback ���ʤ��Ƥ� EXIT��QUIT��AUX �ϥ��ꥢ���� */
    callbackfunc = cbp->func[index];
    if (callbackfunc) {
      d->kanji_status_return->info &= ~KanjiEmptyInfo;
      if (index) { /* everytime �ʳ� */
	res = (*callbackfunc)(d, res, cbp->env);
	cbp = d->cb; /* ������Хå��ؿ��ˤ�ꥳ����Хå���
			�ƤӽФ����Τ�ٱ礹�뤿������ľ�� */
	/* �����ǥ�����Хå��ؿ���ݥåץ��åפ��褦���ɤ����ͤ��ɤ��� */
	continue;
      }
      else {
	res = (*callbackfunc)(d, res, cbp->env);
      }
    }
    cbp = cbp->next;
  }
  return res;
}

_doFunc(d, fnum)
uiContext d;
int fnum;
{
  int res = 0, tmpres, ginfo = 0;
  int reallyThrough = 1;
  wchar_t *prevEcho, *prevGEcho;
  int prevEchoLen = -1, prevRevPos, prevRevLen;
  int prevGEchoLen, prevGRevPos, prevGRevLen;

  d->status = 0; /* �⡼�ɤˤĤ���"������"�Υ��ơ�����������ͤȤ��� */
  d->more.todo = 0;
  tmpres = d->current_mode->func(d, d->current_mode,
				 KEY_CALL, (int)(unsigned char)d->ch, fnum);

  if (d->flags & MULTI_SEQUENCE_EXECUTED) {
    d->flags &= ~MULTI_SEQUENCE_EXECUTED;
    return tmpres;
  }

  /* ������Хå���¹Ԥ��� */
  res = tmpres = callCallback(d, tmpres);

  if (d->kanji_status_return->length >= 0) {
    prevEcho    = d->kanji_status_return->echoStr;
    prevEchoLen = d->kanji_status_return->length;
    prevRevPos  = d->kanji_status_return->revPos;
    prevRevLen  = d->kanji_status_return->revLen;
  }
  if (d->kanji_status_return->info & KanjiGLineInfo) {
    ginfo = 1;
    prevGEcho    = d->kanji_status_return->gline.line;
    prevGEchoLen = d->kanji_status_return->gline.length;
    prevGRevPos  = d->kanji_status_return->gline.revPos;
    prevGRevLen  = d->kanji_status_return->gline.revLen;
  }

  /* moreToDo ��¹Ԥ��ʤ��Ƥ� */
  while (d->more.todo) {
    if (!(d->kanji_status_return->info & KanjiThroughInfo)) {
      reallyThrough = 0;
    }
    d->kanji_status_return->info &= ~(KanjiThroughInfo | KanjiEmptyInfo);
    d->more.todo = 0;
    d->ch = d->more.ch;	/* moreTodo �� more.ch �Ϥ���ʤ��ΤǤϡ� */
    d->nbytes = 1;
    d->buffer_return += tmpres;
    d->n_buffer -= tmpres;

    {
      int check;
      /* �����ܰʹߤ˰ʲ��Υǡ����������Ƥ����礬����Τ�����ľ���� */
      d->buffer_return[0] = key2wchar((int)(unsigned char)d->ch, &check);
      if (!check) {
	d->nbytes = 0;
      }
    }

    tmpres = _doFunc(d, d->more.fnum);

    if (tmpres >= 0) {
      res += tmpres;
      if (d->kanji_status_return->length >= 0) {
	prevEcho    = d->kanji_status_return->echoStr;
	prevEchoLen = d->kanji_status_return->length;
	prevRevPos  = d->kanji_status_return->revPos;
	prevRevLen  = d->kanji_status_return->revLen;
      }
      if (d->kanji_status_return->info & KanjiGLineInfo) {
	ginfo = 1;
	prevGEcho    = d->kanji_status_return->gline.line;
	prevGEchoLen = d->kanji_status_return->gline.length;
	prevGRevPos  = d->kanji_status_return->gline.revPos;
	prevGRevLen  = d->kanji_status_return->gline.revLen;
      }
    }
  }
  if (!reallyThrough) {
    d->kanji_status_return->info &= ~KanjiThroughInfo;
  }

  d->kanji_status_return->length  = prevEchoLen;
  if (prevEchoLen >= 0) {
    d->kanji_status_return->echoStr = prevEcho;
    d->kanji_status_return->revPos  = prevRevPos;
    d->kanji_status_return->revLen  = prevRevLen;
  }
  if (ginfo) {
    d->kanji_status_return->gline.line    = prevGEcho;
    d->kanji_status_return->gline.length  = prevGEchoLen;
    d->kanji_status_return->gline.revPos  = prevGRevPos;
    d->kanji_status_return->gline.revLen  = prevGRevLen;
    d->kanji_status_return->info |= KanjiGLineInfo;
  }

  return res;
}

_afterDoFunc(d, retval)
uiContext d;
int retval;
{
  int res = retval;
  wcKanjiStatus   *kanji_status_return = d->kanji_status_return;

  /* GLine ��ä��ȸ����Τʤ�ä��ޤ��礦 */
  if (d->flags & PLEASE_CLEAR_GLINE) {
    if (d->flags & PCG_RECOGNIZED) { /* �����������ʤ� */
      if (res >= 0 &&	kanji_status_return->length >= 0) {
	d->flags &= ~(PLEASE_CLEAR_GLINE | PCG_RECOGNIZED);
	   /* ��������ܤ�̤����ޤ��� */
	if (!(kanji_status_return->info & KanjiGLineInfo)) {
	  GlineClear(d);
	}
      }
    }
    else {
      d->flags |= PCG_RECOGNIZED;
    }
  }
  return res;
}

/* cfuncdef

  doFunc -- _doFunc ���ɤ�ǡ������ ClearGLine �����䡢������Хå���
            �����򤹤롣

 */

doFunc(d, fnum)
uiContext d;
int fnum;
{
  return _afterDoFunc(d, _doFunc(d, fnum));
}

static
KC_getContext(d, arg)
     uiContext d;
     int arg;
     /* ARGSUSED */
{
  extern defaultContext, defaultBushuContext;

  switch (arg)
    {
    case 0:
      return RkwDuplicateContext(defaultContext);
    case 1:
      return RkwDuplicateContext(defaultBushuContext);
    default:
      return(-1);
    }
  /* NOTREACHED */
}

static
KC_closeUIContext(d, arg)
uiContext d;
wcKanjiStatusWithValue *arg;
{
  extern struct ModeNameRecs ModeNames[];
  int ret;

  d->buffer_return = arg->buffer;
  d->n_buffer = arg->n_buffer;
  d->kanji_status_return = arg->ks;

  bzero(d->kanji_status_return, sizeof(wcKanjiStatus));

  if ((d->nbytes = escapeToBasicStat(d, CANNA_FN_Quit)) < 0)
    return -1;
  d->kanji_status_return->info &= ~KanjiThroughInfo;
  arg->val = d->nbytes;
  freeRomeStruct(d);

  ret = countContext();
#ifdef DEBUG
  fprintf(stderr, "ret=%d\n", ret);
#endif
  return ret;
}

static yomiContext
getYomiContext(d)
uiContext d;
{
  coreContext cc = (coreContext)d->modec;
  yomiContext yc;

  switch (cc->id) {
  case YOMI_CONTEXT:
    yc = (yomiContext)cc;
    break;
  default:
    if (cc->minorMode == CANNA_MODE_AlphaMode) {
      yc = (yomiContext)(cc->next);
    }
    else {
      yc = (yomiContext)0;
    }
    break;
  }
  return yc;
}

static
KC_inhibitChangeMode(d, arg)
uiContext d;
int arg;
{
  yomiContext yc;

  yc = getYomiContext(d);
  if (yc) {
    if (arg) {
      yc->generalFlags |= CANNA_YOMI_CHGMODE_INHIBITTED;
    }
    else {
      yc->generalFlags &= ~CANNA_YOMI_CHGMODE_INHIBITTED;
    }
    return 0;
  }
  else {
    return -1;
  }  
}

static
KC_letterRestriction(d, arg)
uiContext d;
int arg;
{
  yomiContext yc;

  yc = getYomiContext(d);
  if (yc) {
    yc->allowedChars = arg;
    return 0;
  }
  else {
    return -1;
  }
}

static
countColumns(str)
wchar_t *str;
{
  int len = 0;
  wchar_t *p;

  if (str) {
    for (p = str ; *p ; p++) {
      switch (WWhatGPlain(*p)) {
      case 0:
      case 2:
	len += 1;
	break;
      case 1:
      case 3:
	len += 2;
	break;
      }
    }
  }
  return len;
}

static
KC_queryMaxModeStr(d, arg)
     uiContext d;
     int arg;
     /* ARGSUSED */
{
  int i, maxcolumns = 0, ncols;
  extern struct ModeNameRecs ModeNames[];
  extern extraFunc *extrafuncp;
  extraFunc *ep;

  for (i = 0 ; i < CANNA_MODE_MAX_IMAGINARY_MODE ; i++) {
    ncols = countColumns(ModeNames[i].name);
    if (ncols > maxcolumns) {
      maxcolumns = ncols;
    }
  }
  for (ep = extrafuncp ; ep ; ep = ep->next) {
    ncols = countColumns(ep->display_name);
    if (ncols > maxcolumns) {
      maxcolumns = ncols;
    }
  }
  return maxcolumns;
}

static int
KC_setListCallback(d, arg)
uiContext d;
jrListCallbackStruct *arg;
{
  extern int iListCB;

  if (iListCB) {
    d->client_data = (char *)0;
    d->list_func = (void (*)())0;
    return -1;
  }
  if (arg->callback_func) {
    d->client_data = arg->client_data;
    d->list_func = arg->callback_func;
  }
  else {
    d->client_data = (char *)0;
    d->list_func = (void (*)())0;
  }
  return 0;
}

static int
KC_setVerbose(d, arg)
     uiContext d;
     int arg;
     /* ARGSUSED */
{
  extern int ckverbose;

  ckverbose = arg;
  return 0;
}

/* kanjiInitialize ���ʴ����Ѵ��ν���� KC_INITIALIZE�������Ǥ��롣 */

int 
kanjiInitialize(mes)
char ***mes;
{
  return KC_initialize(0, (char *)mes);
}

/* kanjiFinalize KC_FINALIZE�������Ǥ��롣 */

int 
kanjiFinalize(mes)
char ***mes;
{
  return KC_finalize(0, (char *)mes);
}

/* createKanjiContext ����ƥ����Ȥ���������ΤǤ��롣 */

static unsigned char context_table[100] = "";

int
createKanjiContext()
{
  int i;

  for (i = 0; i < 100; i++) {
    if (!context_table[i]) {
      context_table[i] = 1;
      return i;
    }
  }
  return -1; /* ����ƥ����Ȥ�����Ǥ��ʤ��ä��� */
}

/* wcCloseKanjiContext ����ƥ����Ȥ򥯥��������Ρ� */

int
wcCloseKanjiContext(context,ksva)
#if __STDC__
const int context;
#else
int context;
#endif
wcKanjiStatusWithValue *ksva;
{
  context_table[context] = 0;
  return  XwcKanjiControl2(0, context, KC_CLOSEUICONTEXT, (BYTE *)ksva);
}

/* jrCloseKanjiContext ����ƥ����Ȥ򥯥��������Ρ� */

int
jrCloseKanjiContext(context,ksva)
#if __STDC__
const int context;
#else
int context;
#endif
jrKanjiStatusWithValue *ksva;
{
  context_table[context] = 0;
  return  XKanjiControl2(0, context, KC_CLOSEUICONTEXT, (BYTE *)ksva);
}

int
ToggleChikuji(d, flg)
     uiContext d;
     int flg;
{
  yomiContext	yc = (yomiContext)d->modec;
  extern KanjiModeRec empty_mode;
  extern chikuji;

  if ((yc->generalFlags & CANNA_YOMI_CHIKUJI_MODE) &&
      yc->context != -1) {
    RkwEndBun(yc->context, 0);
    abandonContext(d, yc);
  }
  (void)escapeToBasicStat(d, CANNA_FN_Kakutei);
  d->kanji_status_return->info &= ~KanjiThroughInfo;

  if (flg) {
    /* �༡��ư���Ѥ��� */
    yc->generalFlags |= CANNA_YOMI_CHIKUJI_MODE;
    yc->majorMode = CANNA_MODE_HenkanMode;
    chikuji = 1;
  }
  else {
    /* Ϣʸ����Ѥ��� */
    yc->generalFlags &= ~CANNA_YOMI_CHIKUJI_MODE;
    yc->majorMode = CANNA_MODE_HenkanMode;
    chikuji = 0;
  }
  yc->minorMode = getBaseMode(yc);
  d->majorMode = d->minorMode = CANNA_MODE_AlphaMode; /* ���ߡ� */
  currentModeInfo(d);
  return 0;
}

static int
KC_lispInteraction(d, arg)
uiContext d;
int arg;
/* ARGSUSED */
{
  clisp_main();
  return 0;
}

/*
 * �����ФȤ���³���ڤ�
 */
static int
KC_disconnectServer(d, arg)
uiContext d;
int arg;
/* ARGSUSED */
{

#ifdef DEBUG
  fprintf(stderr,"�����ФȤ���³���ڤ�  ");
#endif
  jrKanjiPipeError();
}

static int
KC_setAppName(d, arg)
uiContext d;
unsigned char *arg;
/* ARGSUSED */
{
  extern int defaultContext;

  strncpy(saveapname, (char *)arg, CANNA_MAXAPPNAME);
  RkwSetAppName(defaultContext, saveapname);

  return(0);
}

static int
KC_debugmode(d, arg)
uiContext d;
int arg;
/* ARGSUSED */
{
  extern int iroha_debug;

  iroha_debug = arg;
  return 0;
}

static void
debug_yomibuf(yc)
yomiContext yc;
/* ARGSUSED */
{
#ifdef DEBUG
  char kana[1024], roma[1024], ka[1024], ya[1024], *kanap, *romap, *kap, *yap;
  int len, i, j, k, maxcol, columns, tmp;
  wchar_t xxx[1024];

#define MANYSPACES "                                                  "

  kanap = kana; romap = roma; kap = ka; yap = ya;

  for (i = 0, j = 0 ; i < yc->kEndp || j < yc->rEndp ;) {
    maxcol = 0;

    if (i < yc->kEndp) {
      k = i + 1;
      columns = 0;
      tmp = 
	(WIsG0(yc->kana_buffer[i]) || WIsG2(yc->kana_buffer[i])) ? 1 : 2;
      if (i == yc->kRStartp && i != yc->kCurs) {
	*kanap++ = '\'';
	*kap++ = '\'';
	columns++;
      }
      if (yc->kAttr[i] & HENKANSUMI) {
	*kap++ = ' ';
      }
      else {
	*kap++ = 'x';
      }
      if (tmp > 1) {
	*kap++ = ' ';
      }
      columns += tmp;
      while (!(yc->kAttr[k] & SENTOU)) {
	tmp = (WIsG0(yc->kana_buffer[k]) || WIsG2(yc->kana_buffer[k])) ? 1 : 2;
	columns += tmp;
	if (yc->kAttr[k] & HENKANSUMI) {
	  *kap++ = ' ';
	}
	else {
	  *kap++ = 'x';
	}
	if (tmp > 1) {
	  *kap++ = ' ';
	}
	k++;
      }
      WStrncpy(xxx, yc->kana_buffer + i, k - i);
      xxx[k - i] = (wchar_t)0;
      sprintf(kanap, "%ws ", xxx);
      *kap++ = ' ';
      len = strlen(kanap);
      if (columns > maxcol) {
	maxcol = columns;
      }
      else {
	strncpy(kanap + len, MANYSPACES, maxcol - columns);
	strncpy(kap, MANYSPACES, maxcol - columns);
	kap += maxcol - columns;
	len += maxcol - columns;
      }
      kanap += len;
      i = k;
    }

    if (j < yc->rEndp) {
      k = j + 1;
      columns = 
	(WIsG0(yc->romaji_buffer[j]) || WIsG2(yc->romaji_buffer[j])) ? 1 : 2;
      if (j == yc->rStartp && j != yc->rCurs) {
	*romap++ = '\'';
	columns++;
      }
      while (!(yc->rAttr[k] & SENTOU)) {
	columns += 
	  (WIsG0(yc->romaji_buffer[k]) || WIsG2(yc->romaji_buffer[k])) ? 1 : 2;
	k++;
      }
      WStrncpy(xxx, yc->romaji_buffer + j, k - j);
      xxx[k - j] = (wchar_t)0;
      sprintf(romap, "%ws ", xxx);
      len = strlen(romap);
      if (columns > maxcol) {
	strncpy(kanap, MANYSPACES, columns - maxcol);
	kanap += columns - maxcol;
	strncpy(kap, MANYSPACES, columns - maxcol);
	kap += columns - maxcol;
	maxcol = columns;
      }
      else {
	strncpy(romap + len, MANYSPACES, maxcol - columns);
	len += maxcol - columns;
      }
      romap += len;
      j = k;
    }
  }
  *kap = *kanap = *romap = '\0';
  printf("%s\n", roma);
  printf("%s\n", kana);
  printf("%s\n", ka);
#endif /* DEBUG */
}

static int
KC_debugyomi(d, arg)
uiContext d;
int arg;
/* ARGSUSED */
{
  if (((coreContext)(d)->modec)->id == YOMI_CONTEXT) {
    debug_yomibuf((yomiContext)d->modec);
  }
  return 0;
}

static int
KC_queryPhono(d, arg)
uiContext d;
char *arg;
/* ARGSUSED */
{
  extern struct RkRxDic *romajidic;
  struct RkRxDic **foo = (struct RkRxDic **)arg;

  *foo = romajidic;
  return 0;
}

/* KanjiControl�θġ�������ؿ��ؤΥݥ��� */

static int (*kctlfunc[MAX_KC_REQUEST])() = {
  KC_initialize,
  KC_finalize,
  KC_changeMode,
  KC_setWidth,
  KC_setUFunc,
  KC_setBunsetsuKugiri,
  KC_setModeInfoStyle,
  KC_setHexInputStyle,
  KC_inhibitHankakuKana,
  KC_defineKanji,
  KC_kakutei,
  KC_kill,
  KC_modekeys,
  KC_queryMode,
  KC_queryConnection,
  KC_setServerName,
  KC_parse,
  KC_yomiInfo,
  KC_storeYomi,
  KC_setInitFileName,
  KC_do,
  KC_getContext,
  KC_closeUIContext,
  KC_inhibitChangeMode,
  KC_letterRestriction,
  KC_queryMaxModeStr,
  KC_setListCallback,
  KC_setVerbose,
  KC_lispInteraction,
  KC_disconnectServer,
  KC_setAppName,
  KC_debugmode,
  KC_debugyomi,
  KC_keyconvCallback,
  KC_queryPhono,
};

kanjiControl(request, d, arg)
int request;
uiContext d;
caddr_t arg;
{
  return kctlfunc[request](d, arg);
}
