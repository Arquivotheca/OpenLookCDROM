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
static	char	rcs_id[] = "@(#) 102.1 $Id: henkan.c,v 5.50 1994/04/18 12:09:58 kon Exp $";
#endif /* lint */

#include	"canna.h"

#include	<errno.h>
#include	<fcntl.h>
#ifdef MEASURE_TIME
#include <sys/types.h>
#include <sys/times.h>
#endif

extern int defaultBushuContext;
extern int errno, keepCursorPosition, kojin, ChikujiContinue, RenbunContinue;
extern int Gakushu, InhibitHankakuKana, CursorWrap, ReverseWidely;
extern int yomiInfoLevel, BunsetsuKugiri, stayAfterValidate;
extern int ckverbose, kakuteiIfEndOfBunsetsu;
extern int doKatakanaGakushu, doHiraganaGakushu;
extern int defaultContext, kouho_threshold;
extern struct dicname *RengoGakushu, *KatakanaGakushu, *HiraganaGakushu;
extern KanjiModeRec cy_mode, cb_mode, yomi_mode, tankouho_mode, empty_mode;
extern int strokelimit;
extern char saveapname[];

#define DICERRORMESGLEN 78

static int doYomiHenkan();

static char dictmp[DICERRORMESGLEN];
static char *mountErrorMessage = "��ޥ���ȤǤ��ޤ���Ǥ���";

static int
kanakanError(d)
uiContext d;
{
  return makeRkError(d, "���ʴ����Ѵ��˼��Ԥ��ޤ���");
}

static void
dicMesg(s, d)
char *s, *d;
{
  if (ckverbose == CANNA_FULL_VERBOSE) {
    char buf[128];
    sprintf(buf, "\"%s\"", d);
    printf("%14s %-20s ����ꤷ�Ƥ��ޤ���\n", s, buf);
  }
}

#ifndef ENGINE_SWITCH
#define CANNA_SERVER_NAME_LEN 128
static char iroha_server_name[CANNA_SERVER_NAME_LEN] = {0, 0};

RkSetServerName(s)
char *s;
{
  if (s)
    (void)strncpy(iroha_server_name, s, CANNA_SERVER_NAME_LEN);
  else
    iroha_server_name[0] = '\0';
  return 0;
}

char *
RkGetServerHost()
{
  if (iroha_server_name[0]) {
    return iroha_server_name;
  }
  else {
    return (char *)0;
  }
}
#endif /* !ENGINE_SWITCH */

static
RkwInitError()
{
  if (errno == EPIPE) {
    jrKanjiError = "���ʴ����Ѵ������Ф��̿��Ǥ��ޤ���Ǥ���";
  }
  else {
    jrKanjiError = "���ʴ����Ѵ�����ν�����˼��Ԥ��ޤ���";
  }
  addWarningMesg(jrKanjiError);
  RkwFinalize();
}

static void
mountError(dic)
char *dic;
{
  int mnterrlen;
  if (DICERRORMESGLEN < 
      (unsigned)(strlen(dic) + (mnterrlen = strlen(mountErrorMessage)) + 1)) {
    (void)strncpy(dictmp, dic, DICERRORMESGLEN - mnterrlen - 3/* ... */ - 1);
    (void)strcpy(dictmp + DICERRORMESGLEN - mnterrlen - 3 - 1, "...");
    strcpy(dictmp + DICERRORMESGLEN - mnterrlen - 1, mountErrorMessage);
  }
  else {
    sprintf(dictmp, "%s%s", dic, mountErrorMessage);
  }
  jrKanjiError = dictmp;
  addWarningMesg(dictmp);
}

/*
 * ���ʴ����Ѵ��Τ���ν������
 *
 * ��RkwInitialize��Ƥ�ǡ�defaultContext ���������
 * ��defaultBushuContext ���������
 * ������Υ������ѥ������ꤹ��
 * �������ƥ༭�������Ѽ��񡢥桼�������ޥ���Ȥ���
 *
 * ������	�ʤ�
 * �����	0:�ޤ���� -1:�Ȥ��Ȥ�����
 */
KanjiInit()
{
  char *ptr, *getenv(), *kodmesg = ""/* ����μ�����Υ�å����� */;
  int con;
  static int mountnottry = 1; /* �ޥ���Ƚ�����ԤäƤ��뤫�ɤ��� */
  struct dicname *stp;
  extern struct dicname *kanjidicnames;
  extern FirstTime;

#ifdef DEBUG
  if (iroha_debug) {
    fprintf(stderr,"\n�����Ф���³���� strokelimit = %d (default:%d)\n",
	    strokelimit, STROKE_LIMIT);
  }
#endif
  /* Ϣʸ��饤�֥����������� */

  if (!(ptr = RkGetServerHost()) &&
      !(ptr = getenv("IROHADICDIR"))) {
    ptr = DICHOME;
  }
  if ((defaultContext = RkwInitialize(ptr)) == -1) {
    RkwInitError();
    return -1;
  }

  if (defaultContext != -1) {
    if((defaultBushuContext = RkwCreateContext()) == -1) {
      jrKanjiError = "�����ѤΥ���ƥ����Ȥ�����Ǥ��ޤ���Ǥ���";
      addWarningMesg(jrKanjiError);
      defaultContext = -1;
      RkwFinalize();
      return -1;
    }
  } else {
    defaultBushuContext = -1;
  }

  debug_message("�ǥե���ȥ���ƥ�����(%d), ���󥳥�ƥ�����(%d)\n",
		defaultContext, defaultBushuContext, 0);

  if (defaultContext != -1) {
    if((ptr = getenv("IROHADICPATH")) == (char *)NULL) {
      if((RkwSetDicPath(defaultContext, "iroha")) == -1) {
	jrKanjiError = "����Υǥ��쥯�ȥ������Ǥ��ޤ���Ǥ���";
	RkwFinalize();
	return(NG);
      }
      if((RkwSetDicPath(defaultBushuContext, "iroha")) == -1) {
	jrKanjiError = "����Υǥ��쥯�ȥ������Ǥ��ޤ���Ǥ���";
	RkwFinalize();
	return(NG);
      }
    } else {
      if((RkwSetDicPath(defaultContext, ptr)) == -1) {
	jrKanjiError = "����Υǥ��쥯�ȥ������Ǥ��ޤ���Ǥ���";
	RkwFinalize();
	return(NG);
      }
      if((RkwSetDicPath(defaultBushuContext, ptr)) == -1) {
	jrKanjiError = "����Υǥ��쥯�ȥ������Ǥ��ޤ���Ǥ���";
	RkwFinalize();
	return(NG);
      }
    }

    if (saveapname[0]) {
      RkwSetAppName(defaultContext, saveapname);
    }

    if (!FirstTime && !mountnottry) { /* KC_INITIALIZE �ǸƤӽФ���Ƥ��ʤ��ơ�
					 ���˥ޥ���Ƚ�����ԤäƤ����� */
      /* ʸˡ����Υޥ���� */
      for (stp = kanjidicnames; stp ; stp = stp->next) {
	if (stp->dictype == DIC_GRAMMAR) {
	  if (stp->dicflag == DIC_MOUNTED) {
	    if (RkwMountDic(defaultContext, stp->name,
			    kojin ? PL_ALLOW : PL_INHIBIT) == -1) {
	      stp->dicflag = DIC_MOUNT_FAILED;
	      mountError(stp->name);
	    }
	    else {
	      stp->dicflag = DIC_MOUNTED;
	      dicMesg("ʸˡ����", stp->name);
	    }
	  }
	}
      }
      /* �����ƥ༭��Υޥ���� */
      for (stp = kanjidicnames ; stp ; stp = stp->next) {
	if (stp->dictype != DIC_GRAMMAR) {
	  if (stp->dicflag == DIC_MOUNTED) {
	    if (stp->dictype == DIC_BUSHU) {
	      con = defaultBushuContext;
	    }
	    else {
	      con = defaultContext;
	    }
	    if (RkwMountDic(con, stp->name, kojin ? PL_ALLOW : PL_INHIBIT)
		== -1) {
#ifdef DEBUG
	    if (iroha_debug) {
	      fprintf(stderr, "saveddicname = %s\n", stp->name);
	    }
#endif
	      stp->dicflag = DIC_MOUNT_FAILED;
	      mountError(stp->name);
	    }
	    dicMesg("saveddicname�μ���", stp->name);
	  }
	}
      }
    }
    else { /* KC_INITIALIZE ����ƤӽФ���Ƥ����硣
	      �ޤ��ϡ��ޥ���Ƚ�����ԤäƤ��ʤ���� */
#ifdef DEBUG
      if (iroha_debug) {
	fprintf(stderr,
		"�����.canna���̤�˥ޥ���Ȥ���\n");
      }
#endif

      mountnottry = 0; /* �ޥ���Ƚ�����Ԥ��Τ� mountnottry = 0 �ˤ��� */
      /* ʸˡ����Υޥ���� */
      for (stp = kanjidicnames; stp ; stp = stp->next) {
	if (stp->dictype == DIC_GRAMMAR) {
	  if (RkwMountDic(defaultContext, stp->name,
			  kojin ? PL_ALLOW : PL_INHIBIT) == -1) {
	    stp->dicflag = DIC_MOUNT_FAILED;
	    mountError(stp->name);
	  }
	  else {
	    stp->dicflag = DIC_MOUNTED;
	    dicMesg("ʸˡ����", stp->name);
	  }
	}
      }

      /* �����ƥ༭��Υޥ���� */
      for (stp = kanjidicnames ; stp ; stp = stp->next) {
	if (stp->dictype != DIC_GRAMMAR) {
	  con = defaultContext;
	  if (stp->dictype == DIC_PLAIN) {
	    kodmesg = "�����ƥ༭��";
	  }
	  else if (stp->dictype == DIC_USER) {
	    /* �桼������Υޥ���� */	    
	    kodmesg = "ñ����Ͽ�Ѽ���";
	  }
	  else if (stp->dictype == DIC_RENGO) {
	    /* Ϣ�켭��Υޥ���� */
	    RengoGakushu = stp;
	    kodmesg = "Ϣ�켭��";
	  }
	  else if (stp->dictype == DIC_KATAKANA) {
	    KatakanaGakushu = stp;
	    kodmesg = "Ϣ�켭��";
	  }
	  else if (stp->dictype == DIC_RENGO) {
	    HiraganaGakushu = stp;
	    kodmesg = "Ϣ�켭��";
	  }
	  else if (stp->dictype == DIC_BUSHU) {
	    kodmesg = "���󼭽�";
	    con = defaultBushuContext;
	  }
	  if (RkwMountDic(con, stp->name,
			  kojin ? PL_ALLOW : PL_INHIBIT) == -1) {
	    stp->dicflag = DIC_MOUNT_FAILED;
	    if (stp->dictype != DIC_USER || strcmp(stp->name, "user")) {
  	      mountError(stp->name);
	    }
	  }
	  else {
	    stp->dicflag = DIC_MOUNTED;
	    dicMesg(kodmesg, stp->name);
	  }
	}
      }
    }
    return(0);
  }
  return -1;
}

/*
 * ���ʴ����Ѵ��Τ���θ����
 *
 * �������ƥ༭�������Ѽ��񡢥桼������򥢥�ޥ���Ȥ���
 * ��RkwFinalize��Ƥ�
 *
 * ������	�ʤ�
 * �����	�ʤ�
 */
KanjiFin()
{
  struct dicname *dp, *np;
  int con;

  for (dp = kanjidicnames ; dp ;) {
    if (dp->dictype == DIC_BUSHU) {
      con = defaultBushuContext;
    }
    else {
      con = defaultContext;
    }
    if (dp->dicflag == DIC_MOUNTED) {
      if (RkwUnmountDic(con, dp->name) == -1) {
	char buf[256];
	sprintf(buf, "%s �򥢥�ޥ���ȤǤ��ޤ���Ǥ�����", dp->name);
	addWarningMesg(buf);
      }
    }
    np = dp->next;
    free(dp->name);
    free((char *)dp);
    dp = np;
  }
  kanjidicnames = (struct dicname *)0;
	  
  /* Ϣʸ��饤�֥���λ������ */
  RkwFinalize();

  return(0);
}

static tanContext
newTanContext(majo, mino)
int majo, mino;
{
  tanContext tan;

  tan = (tanContext)malloc(sizeof(tanContextRec));
  if (tan) {
    bzero(tan, sizeof(tanContextRec));
    tan->id = TAN_CONTEXT;
    tan->majorMode = majo;
    tan->minorMode = mino;
    tan->left = tan->right = (tanContext)0;
    tan->next = (mode_context)0;
    tan->curMode = &tankouho_mode;
  }
  return tan;
}

void
freeTanContext(tan)
tanContext tan;
{
  if (tan->kanji) free((char *)tan->kanji);
  if (tan->yomi) free((char *)tan->yomi);
  if (tan->roma) free((char *)tan->roma);
  if (tan->kAttr) free((char *)tan->kAttr);
  if (tan->rAttr) free((char *)tan->rAttr);
  free((char *)tan);
}

static wchar_t *
DUpwstr(w, l)
wchar_t *w;
int l;
{
  wchar_t *res;

  res = (wchar_t *)malloc((l + 1) * sizeof(wchar_t));
  if (res) {
    WStrncpy(res, w, l);
    res[l] = (wchar_t)0;
  }
  return res;
}

static BYTE *
DUpattr(a, l)
BYTE *a;
int l;
{
  BYTE *res;

  res = (BYTE *)malloc((l + 1) * sizeof(BYTE));
  if (res) {
    bcopy(a, res, (l + 1) * sizeof(BYTE));
  }
  return res;
}

static void
copyYomiinfo2Tan(yc, tan)
yomiContext yc;
tanContext tan;
{
  tan->next = yc->next;
  tan->prevMode = yc->prevMode;
  tan->generalFlags = yc->generalFlags;
  tan->savedFlags = yc->savedFlags;

  tan->romdic = yc->romdic;
  tan->myMinorMode = yc->myMinorMode;
  tan->myEmptyMode = yc->myEmptyMode;
  tan->savedMinorMode = yc->savedMinorMode;
  tan->allowedChars = yc->allowedChars;
  tan->henkanInhibition = yc->henkanInhibition;
}

static void
copyTaninfo2Yomi(tan, yc)
tanContext tan;
yomiContext yc;
{
  /* next �� prevMode �ϴ�������Ѥ� */
  yc->generalFlags = tan->generalFlags;
  yc->savedFlags = tan->savedFlags;

  yc->romdic = tan->romdic;
  yc->myMinorMode = tan->myMinorMode;
  yc->myEmptyMode = tan->myEmptyMode;
  yc->savedMinorMode = tan->savedMinorMode;
  yc->allowedChars = tan->allowedChars;
  yc->henkanInhibition = tan->henkanInhibition;
}

extern yomiContext dupYomiContext pro((yomiContext));

static int
doTanBubunMuhenkan(d, yc)
uiContext d;
yomiContext yc;
{
  int cur = yc->curbun, i, len, ylen = 0, rlen = 0;
  int scuryomi, ecuryomi, scurroma, ecurroma;
  wchar_t xxx[ROMEBUFSIZE];
  tanContext tan, prevLeft = yc->left;
  BYTE *p, *q, *r;
  extern void trimYomi();

  yc->kouhoCount = 0;
  scuryomi = ecuryomi = scurroma = ecurroma = 0;

  jrKanjiError = "���꤬­��ޤ���";
  for (i = 0 ; i < yc->nbunsetsu ; i++) {
    tan = (tanContext)0;
    if (i == cur ||
	(tan = newTanContext(yc->majorMode, CANNA_MODE_TankouhoMode))) {
      if (tan) {
	copyYomiinfo2Tan(yc, tan);
      }
      RkwGoTo(yc->context, i);
      len = RkwGetKanji(yc->context, xxx, ROMEBUFSIZE);
      if (len >= 0) {
	if (!tan || (tan->kanji = DUpwstr(xxx, len))) {
	  len = RkwGetYomi(yc->context, xxx, ROMEBUFSIZE);
	  if (len >= 0) {
	    if (!tan || (tan->yomi = DUpwstr(xxx, len))) {
	      if (!tan || (tan->kAttr = DUpattr(yc->kAttr + ylen, len))) {
		r = yc->rAttr + rlen;
		for (p = yc->kAttr + ylen, q = p + len ; p < q ; p++) {
		  if (*p & SENTOU) {
		    r++;
		    while (!(*r & SENTOU)) {
		      r++;
		    }
		  }
		}
		if (i == cur) {
		  scuryomi = ylen;
		  ecuryomi = ylen + len;
		}
		ylen += len;
		len = r - yc->rAttr - rlen; /* ���޻���Ĺ�� */
		if (!tan ||
		    (tan->roma = DUpwstr(yc->romaji_buffer + rlen, len))) {
		  if (!tan || (tan->rAttr = DUpattr(yc->rAttr + rlen, len))) {
		    if (i == cur) {
		      scurroma = rlen;
		      ecurroma = rlen + len;
		    }
		    rlen += len;
		    if (tan) {
		      if (i != cur) {
			/* �Ȥꤢ�������ˤĤʤ��� */
			tan->right = (tanContext)yc;
			tan->left = yc->left;
			if (yc->left) {
			  yc->left->right = tan;
			}
			yc->left = tan;
		      }
#ifdef DEBUG
		      {
			char yyy[ROMEBUFSIZE];
			WCstombs(yyy, tan->kanji, ROMEBUFSIZE);
			printf("%s/", yyy);
			WCstombs(yyy, tan->yomi, ROMEBUFSIZE);
			printf("%s/", yyy);
			WCstombs(yyy, tan->roma, ROMEBUFSIZE);
			printf("%s\n", yyy);
		      }
#endif
		    }
		    continue;
		  }
		  if (tan) free((char *)tan->roma);
		}
		if (tan) free((char *)tan->kAttr);
	      }
	      if (tan) free((char *)tan->yomi);
	    }
	  }
	  else {
	    makeRkError(d, "���ʴ����Ѵ������Ф��̿��Ǥ��ޤ���");
	  }
	  if (tan) free((char *)tan->kanji);
	}
      }
      else {
	makeRkError(d, "���ʴ����Ѵ������Ф��̿��Ǥ��ޤ���");
      }
    }
    /* ���顼�����򤹤� */
    while ((tan = yc->left) != prevLeft) {
      yc->left = tan->left;
      freeTanContext(tan);
    }
    return -1;
  }

  if (chikujip(yc) && chikujiyomiremain(yc)) {
    int rpos;
    yomiContext lyc = dupYomiContext(yc);

    if (!lyc) { /* ���顼�����򤹤� */
      while ((tan = yc->left) != prevLeft) {
	yc->left = tan->left;
	freeTanContext(tan);
      }
      return -1;
    }

    if (yc->right) { /* �ʤ��Ϥ� */
      yc->right->left = (tanContext)lyc;
    }
    lyc->right = yc->right;
    yc->right = (tanContext)lyc;
    lyc->left = (tanContext)yc;

    kPos2rPos(lyc, 0, yc->cStartp, (int *)0, &rpos);
    d->modec = (mode_context)lyc;
    moveToChikujiYomiMode(d);
    trimYomi(d, yc->cStartp, yc->kEndp, rpos, yc->rEndp);
    d->modec = (mode_context)yc;
    yc->status = lyc->status;
    lyc->cStartp = lyc->cRStartp = lyc->ys = lyc->ye = 0;
  }

  if (cur + 1 < yc->nbunsetsu) { /* yc ���Ǹ夸��ʤ���� */
    int n = yc->nbunsetsu - cur - 1;
    tan = yc->left;
    tan->right = yc->right;
    if (yc->right) {
      yc->right->left = tan;
    }
    for (i = 1 ; i < n ; i++) { /* yomi �� right �����٤� tan �������� */
      tan = tan->left;
    }
    if (tan->left) {
      tan->left->right = (tanContext)yc;
    }
    yc->left = tan->left;
    tan->left = (tanContext)yc;
    yc->right = tan;
  }
  RkwGoTo(yc->context, cur);
  if (RkwEndBun(yc->context, Gakushu ? 1 : 0) == -1) {
    jrKanjiError = "���ʴ����Ѵ��ν�λ�˼��Ԥ��ޤ���";
    if (errno == EPIPE) {
      jrKanjiPipeError();
    }
  }

  trimYomi(d, scuryomi, ecuryomi, scurroma, ecurroma);

  yc->cRStartp = yc->rCurs = yc->rStartp = 0;
  yc->cStartp = yc->kCurs = yc->kRStartp =
    yc->ys = yc->ye = 0;
  yc->status &= CHIKUJI_NULL_STATUS;
  /* �ʤ���༡�Ǥʤ��ʤ� */
  if (chikujip(yc)) {
    yc->generalFlags &= ~CANNA_YOMI_CHIKUJI_MODE;
    yc->generalFlags |= CANNA_YOMI_BASE_CHIKUJI;
  }

  d->current_mode = yc->curMode = &yomi_mode;
  yc->minorMode = getBaseMode(yc);

  /* ����̵�Ѵ��ˤ��� */
  yc->nbunsetsu = 0;

  /* ñ������֤����ɤߤ����Ȥ��ˤ�̵����mark����Ƭ���᤹ */
  yc->cmark = yc->pmark = 0;

  abandonContext(d, yc);
  return 0;
}

yomiContext
newFilledYomiContext(next, prev)
mode_context next;
KanjiMode prev;
{
  yomiContext yc;

  yc = newYomiContext(0, 0, /* ��̤ϳ�Ǽ���ʤ� */
		      CANNA_NOTHING_RESTRICTED,
		      !CANNA_YOMI_CHGMODE_INHIBITTED,
		      !CANNA_YOMI_END_IF_KAKUTEI,
		      CANNA_YOMI_INHIBIT_NONE);
  if (yc) {
    yc->majorMode = yc->minorMode = CANNA_MODE_HenkanMode;
    yc->curMode = &yomi_mode;
    yc->myEmptyMode = &empty_mode;
    yc->romdic = romajidic;
    yc->next = next;
    yc->prevMode = prev;
  }
  return yc;
}

#ifdef DO_MERGE
static
yomiContext
mergeYomiContext(yc)
yomiContext yc;
{
  yomiContext res, a, b;

  res = yc;
  while (res->left && res->left->id == YOMI_CONTEXT) {
    res = (yomiContext)res->left;
  }
  for (a = (yomiContext)res->right ; a && a->id == YOMI_CONTEXT ; a = b) {
    b = (yomiContext)a->right;
    appendYomi2Yomi(a, res);
    if (yc == a) {
      res->kCurs = res->kRStartp = res->kEndp;
      res->rCurs = res->rStartp = res->rEndp;
      res->cmark = res->kCurs;
    }
    res->right = a->right;
    if (res->right) {
      res->right->left = (tanContext)res;
    }
    freeYomiContext(a);
  }
  return res;
}
#endif

/*
  tanContext �� yomiContext �ˤ��ơ��ɤ����Ͼ��֤ˤ���

   0          ����
   otherwise  �����餷���ɤߥ���ƥ����Ȥ��֤�

 */

static yomiContext
tanbunUnconvert(d, tan)
uiContext d;
tanContext tan;
{
  yomiContext yc;
  extern int chikuji;

  yc = newFilledYomiContext(tan->next, tan->prevMode);
  if (yc) {
    extern KanjiModeRec yomi_mode, empty_mode;

    appendTan2Yomi(tan, yc);
    copyTaninfo2Yomi(tan, yc);
    yc->right = tan->right;
    yc->left = tan->left;
    if (yc->myMinorMode) {
      yc->minorMode = yc->myMinorMode;
    }

    if (chikujip(yc)) { /* �༡�ˤϤ��ʤ� */
      yc->generalFlags &= ~CANNA_YOMI_CHIKUJI_MODE;
      yc->generalFlags |= CANNA_YOMI_BASE_CHIKUJI;
    }

    if (yc->left) {
      yc->left->right = (tanContext)yc;
    }
    if (yc->right) {
      yc->right->left = (tanContext)yc;
    }
    freeTanContext(tan);
#ifdef DO_MERGE /* ������Ƥ��ʤ� */
    yc = mergeYomiContext(yc);
#endif
    d->current_mode = yc->curMode;
    d->modec = (mode_context)yc;
    return yc;
  }
  jrKanjiError = "���꤬­��ޤ���";
  return (yomiContext)0;
}

static int
TbBubunMuhenkan(d)
uiContext d;
{
  tanContext tan = (tanContext)d->modec;
  yomiContext yc;

  yc = tanbunUnconvert(d, tan);
  if (yc) {
    currentModeInfo(d);
    makeKanjiStatusReturn(d, yc);
    return 0;
  }
  makeGLineMessageFromString(d, jrKanjiError);
  return NothingChangedWithBeep(d);
}

/*
  TanBubunMuhenkan -- �Ѵ����ʸ�����ʸ�����ʬ�䤹�롣

    ���κݡ��ɤߤ���޻���ʬ�䤹��
 */

int
TanBubunMuhenkan(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  if (yc->id != YOMI_CONTEXT) {
    return TbBubunMuhenkan(d);
  }

  if (!yc->right && !yc->left && yc->nbunsetsu == 1) {
    return TanMuhenkan(d);
  }

  if (doTanBubunMuhenkan(d, yc) < 0) {
    makeGLineMessageFromString(d, jrKanjiError);
    return TanMuhenkan(d);
  }
  makeYomiReturnStruct(d);
  currentModeInfo(d);
  return 0;
}

int
prepareHenkanMode(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  if (confirmContext(d, yc) < 0) {
    return 0;
  }
  d->current_mode = yc->curMode = &tankouho_mode;

  return 1;
}

doHenkan(d, len, kanji)
uiContext d;
int len;
wchar_t *kanji;
{
  /* ��ߤ�������Ѵ����� */
  if(doYomiHenkan(d, len, kanji) == NG) {
    return -1;
  }

  /* kanji_status_return���� */
  makeKanjiStatusReturn(d, (yomiContext)d->modec);
  return 0;
}


/*
 * ���ʴ����Ѵ���Ԥ�
 * ��d->yomi_buffer�ˤ�ߤ���Ф���RkwBgnBun��Ƥ�Ǥ��ʴ����Ѵ��򳫻Ϥ���
 * ��������ʸ�����Ƭʸ��ˤ��ơ�������ʸ�������
 *
 * ������	uiContext
 *		len       len �����ꤵ��Ƥ�����ʸ��Ĺ�򤽤�Ĺ���ˤ��롣
 *		kanji	  kanji �����ꤵ��Ƥ�����ñʸ���Ѵ����ơ�
 *			  �����ȸ���� kanji �Ǽ����줿����˹�碌�롣
 * �����	���ｪλ�� 0	�۾ｪλ�� -1
 */
static
doYomiHenkan(d, len, kanji)
uiContext	d;
int len;
wchar_t *kanji;
{
  unsigned int mode;
  yomiContext yc = (yomiContext)d->modec;
  extern defaultContext;

#ifdef DEBUG
  if (iroha_debug) {
/*    printf("yomi     => "); Wprintf(hc->yomi_buffer); putchar('\n');*/
    printf("yomi len => %d\n", hc->yomilen);
  }
#endif

  /* Ϣʸ���Ѵ��򳫻Ϥ��� *//* ����ˤʤ� �������� �Ҥ餬�� ���ղä��� */
  mode = 0;
  mode = (RK_XFER<<RK_XFERBITS) | RK_KFER;
  if (kanji) {
    mode |= RK_HENKANMODE(RK_TANBUN |
			  RK_MAKE_WORD |
			  RK_MAKE_EISUUJI |
			  RK_MAKE_KANSUUJI) << (2 * RK_XFERBITS);
  }
  
  if (confirmContext(d, yc) < 0) {
    return NG;
  }

#ifdef MEASURE_TIME
  {
    struct tms timebuf;
    long RkTime, times();

    RkTime = times(&timebuf);
#endif /* MEASURE_TIME */

    if ((yc->nbunsetsu =
	 RkwBgnBun(yc->context, yc->kana_buffer, yc->kEndp, mode)) == -1) {
      yc->nbunsetsu = 0;
      return kanakanError(d);
    }
    
    if (len > 0 && (yc->nbunsetsu = RkwResize(yc->context, len)) == -1) {
      RkwEndBun(yc->context, 0);
      yc->nbunsetsu = 0;
      return kanakanError(d);
    }

    if (kanji) {
      /* kanji �����ꤵ��Ƥ����顢Ʊ�����䤬�Ǥ�ޤ� RkwNext �򤹤� */
      int i, n;

      n = RkwGetKanjiList(yc->context, d->genbuf, ROMEBUFSIZE);
      if (n < 0) {
	return kanakanError(d);
      }
      for (i = 0 ; i < n ; i++) {
	RkwXfer(yc->context, i);
	len = RkwGetKanji(yc->context, d->genbuf, ROMEBUFSIZE);
	if (len < 0) {
	  return kanakanError(d);
	}
	d->genbuf[len] = (wchar_t)'\0';
	if (!WStrcmp(kanji, d->genbuf)) {
	  break;
	}
      }
      if (i == n) {
	RkwXfer(yc->context, 0);
      }
    }

#ifdef MEASURE_TIME
    yc->rktime = times(&timebuf);
    yc->rktime -= RkTime;
  }
#endif /* MEASURE_TIME */

  /* ������ʸ�����Ƭʸ�� */
  yc->curbun = 0;

  return(0);
}

int
TanNop(d)
uiContext	d;
{
  yomiContext yc = (yomiContext)d->modec;

  /* currentModeInfo �ǥ⡼�ɾ���ɬ���֤�褦�˥��ߡ��Υ⡼�ɤ�����Ƥ��� */
  d->majorMode = d->minorMode = CANNA_MODE_AlphaMode;
  currentModeInfo(d);

  makeKanjiStatusReturn(d, yc);
  return 0;
}

static int
doGoTo(d, yc)
uiContext d;
yomiContext yc;
{
  if (RkwGoTo(yc->context, yc->curbun) == -1) {
    return makeRkError(d, "ʸ��ΰ�ư�˼��Ԥ��ޤ���");
  }
  yc->status |= CHIKUJI_OVERWRAP;

  /* kanji_status_return���� */
  makeKanjiStatusReturn(d, yc);
  return 0;
}

/*
 * ��ʸ��˰�ư����
 *
 * ������	uiContext
 * �����	���ｪλ�� 0	�۾ｪλ�� -1
 */
int
TanForwardBunsetsu(d)
uiContext	d;
{
  yomiContext yc = (yomiContext)d->modec;

  if (yc->id != YOMI_CONTEXT) {
    return TbForward(d);
  }

  yc->kouhoCount = 0;
  if (yc->curbun + 1 < yc->nbunsetsu) {
    yc->curbun++;
  }
  else if (yc->cStartp && yc->cStartp < yc->kEndp) { /* �༡���ɤߤ����ˤ��� */
    yc->kRStartp = yc->kCurs = yc->cStartp;
    yc->rStartp = yc->rCurs = yc->cRStartp;
    moveToChikujiYomiMode(d);
  }
  else if (yc->right) {
    return TbForward(d);
  }
  else if (kakuteiIfEndOfBunsetsu) {
    d->nbytes = TanKakutei(d);
    d->kanji_status_return->length = 0;
    return d->nbytes;
  }
  else if (!CursorWrap) {
    return NothingForGLine(d);
  }
  else if (yc->left) {
    return TbBeginningOfLine(d);
  }
  else {
    yc->curbun = 0;
  }

  /* ������ʸ��򣱤ı��˰ܤ� */
  /* ������ʸ�᤬�Ǳ����ä��顢
     �Ǻ��򥫥���ʸ��ˤ���   */
  return doGoTo(d, yc);
}

/*
 * ��ʸ��˰�ư����
 *
 * ������	uiContext
 * �����	���ｪλ�� 0	�۾ｪλ�� -1
 */
int
TanBackwardBunsetsu(d)
uiContext	d;
{
  yomiContext yc = (yomiContext)d->modec;

  if (yc->id != YOMI_CONTEXT) {
    return TbBackward(d);
  }

  yc->kouhoCount = 0;
  if (yc->curbun) {
    yc->curbun--;
  }
  else if (yc->left) {
    return TbBackward(d);
  }
  else if (!CursorWrap) {
    return NothingForGLine(d);
  }
  else if (yc->right) {
    return TbEndOfLine(d);
  }
  else if (yc->cStartp && yc->cStartp < yc->kEndp) { /* �༡���ɤߤ����ˤ��� */
    yc->kCurs = yc->kRStartp = yc->kEndp;
    yc->rCurs = yc->rStartp = yc->rEndp;
    moveToChikujiYomiMode(d);
  }
  else {
    yc->curbun = yc->nbunsetsu - 1;
  }

  return doGoTo(d, yc);
}

/*
 * ������򥫥��ȸ���ˤ���
 *
 * ������	uiContext
 * �����	���ｪλ�� 0	�۾ｪλ�� -1
 */

static
tanNextKouho(d, yc)
uiContext	d;
yomiContext   yc;
{
#ifdef MEASURE_TIME
  struct tms timebuf;
  long proctime, times();

  proctime = times(&timebuf);
#endif /* MEASURE_TIME */

  /* ���θ���򥫥��ȸ���Ȥ��� */
  if (RkwNext(yc->context) == -1) {
    makeRkError(d, "�����ȸ������Ф��ޤ���Ǥ���");
    return TanMuhenkan(d);
  }

#ifdef MEASURE_TIME
  yc->rktime = times(&timebuf);
  yc->rktime -= proctime;
#endif /* MEASURE_TIME */

  /* kanji_status_return���� */
  makeKanjiStatusReturn(d, yc);

#ifdef MEASURE_TIME
  yc->proctime = times(&timebuf);
  yc->proctime -= proctime;
#endif /* MEASURE_TIME */

  return(0);
}

/*
  tanbunHenkan -- �Ѵ����롣

    �ߤ��ϡ�kanji �Ǽ����줿�����Ʊ�����䤬�Ф�ޤ� RkwNext �򤹤뤳��
    �Ǥ��롣����������Ȥ⸡�Ф��ʤ���Фʤ�ޤ���
 */

static int
tanbunHenkan(d, yc, kanji)
uiContext d;
yomiContext yc;
wchar_t *kanji;
{
  if (!prepareHenkanMode(d)) {
    makeGLineMessageFromString(d, jrKanjiError);
    makeYomiReturnStruct(d);
    return 0;
  }
  yc->minorMode = CANNA_MODE_TankouhoMode;
  yc->kouhoCount = 1;
  if (doHenkan(d, 0, kanji) < 0) {
    makeGLineMessageFromString(d, jrKanjiError);
    makeYomiReturnStruct(d);
    return 0;
  }
  if (kouho_threshold > 0 && yc->kouhoCount >= kouho_threshold) {
    return tanKouhoIchiran(d, 0);
  }
  
  currentModeInfo(d);
  makeKanjiStatusReturn(d, yc);
  return 0;
}

/*
  enterTanHenkanMode -- tanContext �� yomiContext �ˤ����Ѵ��ν����򤹤�

 */

static int
enterTanHenkanMode(d, fnum)
uiContext d;
{
  tanContext tan = (tanContext)d->modec;
  yomiContext yc;
  wchar_t *prevkanji;

  prevkanji = tan->kanji;
  tan->kanji = (wchar_t *)0;

  yc = tanbunUnconvert(d, tan);
  if (yc) {
    tanbunHenkan(d, yc, prevkanji);
    free((char *)prevkanji);

    /*������
      ñ����⡼�ɤη��ˤ���
      */

    d->more.todo = 1;
    d->more.ch = d->ch;
    d->more.fnum = fnum;
    return 0;
  }
  else { /* ��ťե꡼�򤷤ʤ����ᶯĴŪ�� else ��� */
    free((char *)prevkanji);
  }
  makeGLineMessageFromString(d, jrKanjiError);
  return NothingChangedWithBeep(d);
}

/*
 * ��������Ԥ�ɽ������
 *
 * ���������ɽ���Τ���Υǡ�����ơ��֥�˺�������
 * ���������ɽ���Ԥ������Ȥ��ϡ�������ɽ�����ʤ��Ǽ�����򤽤ξ��ɽ������
 *
 * ������	uiContext
 * �����	���ｪλ�� 0	�۾ｪλ�� -1
 */

TanKouhoIchiran(d)
uiContext d;
{
  if (d->modec->id != YOMI_CONTEXT) {
    return enterTanHenkanMode(d, CANNA_FN_KouhoIchiran);
  }
  return tanKouhoIchiran(d, 1);
}

TanNextKouho(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  if (yc->id != YOMI_CONTEXT) {
    return enterTanHenkanMode(d, CANNA_FN_Next);
  }
  yc->status |= CHIKUJI_OVERWRAP;
  yc->kouhoCount = 0;
  return tanNextKouho(d, yc);
}

/*

  TanHenkan -- ���������å�����ʳ��� TanNextKouho �Ȥۤ�Ʊ��

 */
static int
TanHenkan(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  if (yc->id != YOMI_CONTEXT) {
    return enterTanHenkanMode(d, CANNA_FN_Henkan);
  }

  if (kouho_threshold && ++yc->kouhoCount >= kouho_threshold) {
    return TanKouhoIchiran(d);
  }
  else {
    return tanNextKouho(d, yc);
  }
}

/*
 * ������򥫥��ȸ���ˤ���
 *
 * ������	uiContext
 * �����	���ｪλ�� 0	�۾ｪλ�� -1
 */
TanPreviousKouho(d)
uiContext	d;
{
  yomiContext yc = (yomiContext)d->modec;

  if (yc->id != YOMI_CONTEXT) {
    return enterTanHenkanMode(d, CANNA_FN_Prev);
  }

  yc->status |= CHIKUJI_OVERWRAP;
  yc->kouhoCount = 0;
  /* ���θ���򥫥��ȸ���Ȥ��� */
  if (RkwPrev(yc->context) == -1) {
    makeRkError(d, "�����ȸ������Ф��ޤ���Ǥ���");
    return TanMuhenkan(d);
  }

  /* kanji_status_return���� */
  makeKanjiStatusReturn(d, yc);

  return 0;
}

/*
  tanJishuHenkan -- �����ʸ����������Ѵ�����
 */

static int tanJishuHenkan pro((uiContext, int));

static int
tanJishuHenkan(d, fn)
uiContext d;
int fn;
{
  d->nbytes = TanBubunMuhenkan(d);
  d->more.todo = 1;
  d->more.ch = d->ch;
  d->more.fnum = fn;
  return d->nbytes;
}

TanHiragana(d)
uiContext	d;
{
  return tanJishuHenkan(d, CANNA_FN_Hiragana);
}

TanKatakana(d)
uiContext	d;
{
  return tanJishuHenkan(d, CANNA_FN_Katakana);
}

TanRomaji(d)
uiContext	d;
{
  return tanJishuHenkan(d, CANNA_FN_Romaji);
}

TanUpper(d)
uiContext	d;
{
  return tanJishuHenkan(d, CANNA_FN_ToUpper);
}

TanCapitalize(d)
uiContext	d;
{
  return tanJishuHenkan(d, CANNA_FN_Capitalize);
}

TanZenkaku(d)
uiContext d;
{
  return tanJishuHenkan(d, CANNA_FN_Zenkaku);
}

TanHankaku(d)
uiContext d;
{
  return tanJishuHenkan(d, CANNA_FN_Hankaku);
}

static int
gotoBunsetsu(yc, n)
yomiContext yc;
int n;
{
  /* ������ʸ����ư���� */
  if (RkwGoTo(yc->context, n) == -1) {
    if (errno == EPIPE) {
      jrKanjiPipeError();
    }
    jrKanjiError = "ʸ��ΰ�ư�˼��Ԥ��ޤ���";
    return NG;
  }
  yc->curbun = n;
  return 0;
}

/*
 * �Ǻ�ʸ��˰�ư����
 *
 * ������	uiContext
 * �����	���ｪλ�� 0	�۾ｪλ�� -1
 */
int
TanBeginningOfBunsetsu(d)
uiContext	d;
{
  yomiContext yc = (yomiContext)d->modec;

  if (yc->id != YOMI_CONTEXT || yc->left) {
    return TbBeginningOfLine(d);
  }
  yc->kouhoCount = 0;
  if (gotoBunsetsu(yc, 0) < 0) {
    return NG;
  }
  makeKanjiStatusReturn(d, yc);
  return 0;
}

/*
 * �Ǳ�ʸ��˰�ư����
 *
 * ������	uiContext
 * �����	���ｪλ�� 0	�۾ｪλ�� -1
 */
int
TanEndOfBunsetsu(d)
uiContext	d;
{
  yomiContext yc = (yomiContext)d->modec;

  if (yc->id != YOMI_CONTEXT || yc->right) {
    return TbEndOfLine(d);
  }

  yc->kouhoCount = 0;
  if (yc->cStartp && yc->cStartp < yc->kEndp) {
    yc->kRStartp = yc->kCurs = yc->kEndp;
    yc->rStartp = yc->rCurs = yc->rEndp;
    moveToChikujiYomiMode(d);
  }
  if (gotoBunsetsu(yc, yc->nbunsetsu - 1) < 0) {
    return NG;
  }
  yc->status |= CHIKUJI_OVERWRAP;
  makeKanjiStatusReturn(d, yc);
  return 0;
}

int
tanMuhenkan(d, kCurs)
uiContext d;
int kCurs;
{
  extern KanjiModeRec yomi_mode;
  yomiContext yc = (yomiContext)d->modec;
  int autoconvert = (yc->generalFlags & CANNA_YOMI_CHIKUJI_MODE);

  if (RkwEndBun(yc->context, 0) == -1) {
    if (errno == EPIPE) {
      jrKanjiPipeError();
    }
  }

  if (autoconvert) {
    yc->status &= CHIKUJI_NULL_STATUS;
    d->current_mode = yc->curMode = &cy_mode;
    yc->ys = yc->ye = yc->cStartp = yc->cRStartp = 0;
    yc->rCurs = yc->rStartp = yc->rEndp;
    yc->kCurs = yc->kRStartp = yc->kEndp;
    clearHenkanContext(yc);
  }
  else {
    d->current_mode = yc->curMode = &yomi_mode;
  }
  yc->minorMode = getBaseMode(yc);

  if (kCurs >= 0) {
    int rpos;

    kPos2rPos(yc, 0, kCurs, (int *)0, &rpos);
    yc->kCurs = yc->kRStartp = kCurs;
    yc->rCurs = yc->rStartp = rpos;
  }

  /* ����̵�Ѵ��ˤ��� */
  yc->nbunsetsu = 0;

  /* ñ������֤����ɤߤ����Ȥ��ˤ�̵����mark����Ƭ���᤹ */
  yc->cmark = yc->pmark = 0;

  abandonContext(d, yc);

  return 0;
}

/*
 * ���Ƥ�ʸ����ɤߤ��ᤷ��YomiInputMode �����
 *
 * ������	uiContext
 * �����	���ｪλ�� 0	�۾ｪλ�� -1
 */

TanMuhenkan(d)
uiContext	d;
{
  yomiContext yc = (yomiContext)d->modec, newyc;
  tanContext tan;

  if (yc->id != YOMI_CONTEXT || yc->left || yc->right) {
    tan = (tanContext)yc;
    while (tan->left) {
      tan = tan->left;
    }
    if (tan->id == YOMI_CONTEXT) {
      newyc = (yomiContext)tan;
    }
    else {
      newyc = newFilledYomiContext(yc->next, yc->prevMode);
      if (newyc) {
	extern int chikuji;

	tan->left = (tanContext)newyc;
	newyc->right = tan;
	newyc->generalFlags = tan->generalFlags;
	newyc->savedFlags = tan->savedFlags;
	if (chikujip(newyc)) {
	  newyc->curMode = &cy_mode;
	}
	newyc->minorMode = getBaseMode(newyc);
      }
      else {
	jrKanjiError = "���꤬­��ޤ���";
	makeGLineMessageFromString(d, jrKanjiError);
	return NothingChangedWithBeep(d);
      }
    }
    d->modec = (mode_context)newyc;
    d->current_mode = newyc->curMode;

    doMuhenkan(d, newyc);

    if (newyc->generalFlags &
	(CANNA_YOMI_CHIKUJI_MODE | CANNA_YOMI_BASE_CHIKUJI)) {
      /* �ֿ����༡���ä��פΤǤ���С��༡�⡼�ɤ��᤹ */
      newyc->generalFlags |= CANNA_YOMI_CHIKUJI_MODE;
      newyc->generalFlags &= ~CANNA_YOMI_BASE_CHIKUJI;
      newyc->minorMode = getBaseMode(newyc);
      d->current_mode = newyc->curMode = &cy_mode;
    }

    makeYomiReturnStruct(d);
    currentModeInfo(d);
    return 0;
  }

  if (yc->generalFlags & 
      (CANNA_YOMI_CHIKUJI_MODE | CANNA_YOMI_BASE_CHIKUJI)) {
    /* �ֿ����༡���ä��פΤǤ���С��༡�⡼�ɤ��᤹ */
    yc->generalFlags |= CANNA_YOMI_CHIKUJI_MODE;
    yc->generalFlags &= ~CANNA_YOMI_BASE_CHIKUJI;
    /* �̥륹�ơ��������᤹ */
    yc->status &= CHIKUJI_NULL_STATUS;
  }

  tanMuhenkan(d, -1);
  makeYomiReturnStruct(d);
  currentModeInfo(d);
  return 0;
}

int
TanDeletePrevious(d)
uiContext	d;
{
  yomiContext yc = (yomiContext)d->modec;
  wchar_t tmpbuf[ROMEBUFSIZE];
  int i, j, l = -1;
  extern int BackspaceBehavesAsQuit;

  if (yc->id != YOMI_CONTEXT) {
    return TanMuhenkan(d);
  }

  if ((yc->generalFlags & CANNA_YOMI_CHIKUJI_MODE) &&
      !BackspaceBehavesAsQuit) {
    return ChikujiTanDeletePrevious(d);
  }
  else {
    if (keepCursorPosition) {
      for (i = 0, l = 0; i <= yc->curbun; i++) {
	if (RkwGoTo(yc->context, i) == -1
	    || (j = RkwGetYomi(yc->context, tmpbuf, ROMEBUFSIZE)) == -1) {
	  l = -1;
	  break;
	}
	l += j;
      }
    }
    yc->status &= CHIKUJI_NULL_STATUS;
    tanMuhenkan(d, l);
    makeYomiReturnStruct(d);
    currentModeInfo(d);
    return 0;
  }
}

#if 0
/*
  doTanKakutei -- ���ꤵ����ư��򤹤�

  retval 0 -- ����̵�����ꤷ����
         1 -- ���ꤷ����ʤ��ʤä���
        -1 -- ���顼��
 */

static
doTanKakutei(d, yc)
uiContext	d;
yomiContext yc;
{
  if ((yc->generalFlags & CANNA_YOMI_CHIKUJI_MODE) &&
      (yc->cStartp < yc->kEndp)) {
    (void)RomajiFlushYomi(d, (wchar_t *)0, 0);
  }

  return 0;
}
#endif /* 0 */

void
finishTanKakutei(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;
  int autoconvert = yc->generalFlags & CANNA_YOMI_CHIKUJI_MODE;

#ifdef DO_RENGO_LEARNING
#define RENGOBUFSIZE 256

  if (RengoGakushu && hc->nbunsetsu > 1) { /* Ϣ��ؽ��򤷤褦���ʤ� */
    RkLex  lex[2][RENGOBUFSIZE];
    wchar_t yomi[2][RENGOBUFSIZE];
    wchar_t kanji[2][RENGOBUFSIZE];
    wchar_t word[1024], *w;
    unsigned char xxxx[ROMEBUFSIZE];
    int    nword[2], wlen;

    *(w = word) = (wchar_t) '\0';
    wlen = 1024;

    RkwGoTo(hc->context, 0);
    nword[0] = RkwGetLex(hc->context, lex[0], RENGOBUFSIZE);
    yomi[0][0] =
      (wchar_t) '\0'; /* yomi[current][0]�ο����� �� RkwGetYomi������ */

    for (i = 1 ; i < hc->nbunsetsu ; i++) {
      int current, previous, mighter;

      current = i % 2;
      previous = 1 - current;

      nword[current] = 0;
      if ( !nword[previous] ) {
	nword[previous] = RkwGetLex(hc->context, lex[previous], RENGOBUFSIZE);
      }
      RkwRight(hc->context);

      if (nword[previous] == 1) {
	nword[current] = RkwGetLex(hc->context, lex[current], RENGOBUFSIZE);
	yomi[current][0] = (wchar_t) '\0';
	if (((lex[previous][0].ylen <= 3 && lex[previous][0].klen == 1) ||
	     (lex[current][0].ylen <= 3 && lex[current][0].klen == 1)) &&
	    (lex[current][0].rownum < R_K5 ||
	     R_NZX < lex[current][0].rownum)) {
	  if ( !yomi[previous][0] ) {
	    RkwLeft(hc->context);
	    RkwGetYomi(hc->context, yomi[previous], RENGOBUFSIZE);
	    RkwGetKanji(hc->context, kanji[previous], RENGOBUFSIZE);
	    RkwRight(hc->context);
	  }
	  RkwGetYomi(hc->context, yomi[current], RENGOBUFSIZE);
	  RkwGetKanji(hc->context, kanji[current], RENGOBUFSIZE);

	  WStrncpy(yomi[previous] + lex[previous][0].ylen,
		   yomi[current], lex[current][0].ylen);
	  yomi[previous][lex[previous][0].ylen + lex[current][0].ylen] =
	    (wchar_t) '\0';

	  WStrncpy(kanji[previous] + lex[previous][0].klen,
		   kanji[current], lex[current][0].klen);
	  kanji[previous][lex[previous][0].klen + lex[current][0].klen] =
	    (wchar_t) '\0';

#ifdef NAGASADEBUNPOUWOKIMEYOU
	  if (lex[previous][0].klen >= lex[current][0].klen) {
	    /* ���δ�����Ĺ��       >=    ���δ�����Ĺ�� */
	    mighter = previous;
	  }
	  else {
	    mighter = current;
	  }
#else /* !NAGASADEBUNPOUWOKIMEYOU */
	  mighter = current;
#endif /* !NAGASADEBUNPOUWOKIMEYOU */
	  WStrcpy(w, yomi[previous]);
	  printf(xxxx, " #%d#%d ", lex[mighter][0].rownum,
		 lex[mighter][0].colnum);
	  MBstowcs(w + WStrlen(w), xxxx, wlen - WStrlen(w));
	  WStrcat(w, kanji[previous]);
	  wlen -= (WStrlen(w) + 1); w += WStrlen(w) + 1; *w = (wchar_t) '\0';
	}
      }
    }
  }
#endif /* DO_RENGO_LEARNING */

  if (RkwEndBun(yc->context, Gakushu ? 1 : 0) == -1) {
    if (errno == EPIPE) {
      jrKanjiPipeError();
    }
  }

#ifdef DO_RENGO_LEARNING
  if (RengoGakushu && yc->nbunsetsu > 1) { /* Ϣ��ؽ��򤷤褦���ʤ� */
    for (w = word ; *w ; w += WStrlen(w) + 1) {
      RkwDefineDic(yc->context, RengoGakushu, w);
    }
  }
#endif /* DO_RENGO_LEARNING */

  if (autoconvert) {
    yc->status &= CHIKUJI_NULL_STATUS;
    yc->ys = yc->ye = yc->cStartp = yc->cRStartp = 0;
    clearHenkanContext(yc);
    yc->kEndp = yc->rEndp = yc->kCurs = yc->rCurs =
      yc->cStartp = yc->cRStartp = 
	yc->rStartp = yc->kRStartp = 0;
    yc->kAttr[0] = yc->rAttr[0] = SENTOU;
    yc->kana_buffer[0] = yc->romaji_buffer[0] = 0;
/*    d->kanji_status_return->info |= KanjiEmptyInfo; ¿ʬ�פ�ʤ��Τ�.. */
    d->current_mode = yc->curMode = yc->myEmptyMode;
  }
  yc->minorMode = getBaseMode(yc);
  
  /* ñ������֤����ɤߤ����Ȥ��ˤ�̵����mark����Ƭ���᤹ */
  yc->nbunsetsu = 0;
  yc->cmark = yc->pmark = 0;

  abandonContext(d, yc);

  if (yc->savedFlags & CANNA_YOMI_MODE_SAVED) {
    restoreFlags(yc);
  }
}

TanKakutei(d)
uiContext d;
{
  return YomiKakutei(d);
}

/*
 * �����������ꤵ�������޻��򥤥󥵡��Ȥ���
 *
 * ������	uiContext
 * �����	���ｪλ�� 0	�۾ｪλ�� -1
 */

static int
TanKakuteiYomiInsert(d)
uiContext d;
{
  yomiContext yc = (yomiContext)d->modec;

  if (yc->id != YOMI_CONTEXT) {
    return enterTanHenkanMode(d, CANNA_FN_FunctionalInsert);
  }

  if ((yc->generalFlags & CANNA_YOMI_CHIKUJI_MODE) && ChikujiContinue) {
    yc->minorMode = CANNA_MODE_ChikujiTanMode;
    d->current_mode = yc->curMode = &cb_mode;
    d->nbytes = 0;
    currentModeInfo(d);
    yc->status &= ~CHIKUJI_OVERWRAP;
    if (yc->kCurs != yc->kEndp) {
      yc->rStartp = yc->rCurs = yc->rEndp;
      yc->kRStartp = yc->kCurs = yc->kEndp;
    }
    yc->ys = yc->ye = yc->cStartp;
    return YomiInsert(d);
  }
  else if (RenbunContinue) {
    extern nKouhoBunsetsu;
    
    d->nbytes = 0;
    yc->curbun = yc->nbunsetsu;
    if (doTanBubunMuhenkan(d, yc) < 0) {
      makeGLineMessageFromString(d, jrKanjiError);
      return NothingChangedWithBeep(d);
    }
    if (nKouhoBunsetsu) {
      (void)cutOffLeftSide(d, yc, nKouhoBunsetsu);
    }
  }
  else {
    d->nbytes = YomiKakutei(d);
  }
  d->more.todo = 1;
  d->more.ch = d->ch;
  d->more.fnum = 0;    /* ��� ch �Ǽ����������򤻤� */
  return d->nbytes;
}


/* cfuncdef

  pos �ǻ��ꤵ�줿ʸ�ᤪ��Ӥ�������ʸ��λ����Ѵ������
  ���ꥢ���롣
*/

static int
doTbResize(d, yc, n)
uiContext d;
yomiContext yc;
int n;
{
  int len;

  if (doTanBubunMuhenkan(d, yc) < 0) {
    makeGLineMessageFromString(d, jrKanjiError);
    return NothingChangedWithBeep(d);
  }
  len = yc->kEndp;
  doMuhenkan(d, yc); /* yc ���鱦��ߤ��̵�Ѵ��ˤ��� yc �ˤĤʤ��� */
  if (!prepareHenkanMode(d)) {
    makeGLineMessageFromString(d, jrKanjiError);
    makeYomiReturnStruct(d);
    currentModeInfo(d);
    return 0;
  }
  yc->minorMode = CANNA_MODE_TankouhoMode;
  yc->kouhoCount = 0;
  if (doHenkan(d, len + n, (wchar_t *)0) < 0) {
    makeGLineMessageFromString(d, jrKanjiError);
    makeYomiReturnStruct(d);
    currentModeInfo(d);
    return 0;
  }
  currentModeInfo(d);
  makeKanjiStatusReturn(d, yc);
  return 0;
}

/*
 * ʸ��򿭤Ф�
 *
 * ������	uiContext
 * �����	���ｪλ�� 0	�۾ｪλ�� -1
 */
static int
TanExtendBunsetsu(d)
uiContext	d;
{
  yomiContext yc = (yomiContext)d->modec;

  if (yc->id != YOMI_CONTEXT) {
    return enterTanHenkanMode(d, CANNA_FN_Extend);
  }

  d->nbytes = 0;
  yc->kouhoCount = 0;
  if (yc->right) {
    return doTbResize(d, yc, 1);
  }
  if ((yc->nbunsetsu = RkwEnlarge(yc->context)) <= 0) {
    makeRkError(d, "ʸ��γ���˼��Ԥ��ޤ���");
    return TanMuhenkan(d);
  }
  makeKanjiStatusReturn(d, yc);
  return(d->nbytes);
}

/*
 * ʸ���̤��
 *
 * ������	uiContext
 * �����	���ｪλ�� 0	�۾ｪλ�� -1
 */
static int
TanShrinkBunsetsu(d)
uiContext	d;
{
  yomiContext yc = (yomiContext)d->modec;

  if (yc->id != YOMI_CONTEXT) {
    return enterTanHenkanMode(d, CANNA_FN_Shrink);
  }

  d->nbytes = 0;
  yc->kouhoCount = 0;

  if (yc->right) {
    return doTbResize(d, yc, -1);
  }

  /* ʸ���̤�� */
  if ((yc->nbunsetsu = RkwShorten(yc->context)) <= 0) {
    makeRkError(d, "ʸ��ν̾��˼��Ԥ��ޤ���");
    return TanMuhenkan(d);
  }
  makeKanjiStatusReturn(d, yc);
  
  return(d->nbytes);
}

#define BUNPOU_DISPLAY

#ifdef BUNPOU_DISPLAY
/*
 * ʸˡ�����ץ��Ȥ���
 *
 * ������	uiContext
 * �����	���ｪλ�� 0	�۾ｪλ�� -1
 */
TanPrintBunpou(d)
uiContext	d;
{
  yomiContext yc = (yomiContext)d->modec;
  static wchar_t mesg[512];

  if (yc->id != YOMI_CONTEXT) {
    return enterTanHenkanMode(d, CANNA_FN_ConvertAsHex);
  }

#ifdef notdef
#ifdef DO_GETYOMI
  if (RkwGetYomi(yc->context, buf, 256) == -1) {
    if (errno == EPIPE) {
      jrKanjiPipeError();
      TanMuhenkan(d);
    }
    fprintf(stderr, "�����ȸ�����ɤߤ���Ф��ޤ���Ǥ�����\n");
  }
  Wfprintf(stderr, "%s\n", buf);
#endif /* DO_GETYOMI */

  if(RkwGetKanji(yc->context, buf, 256) == -1) {
    if(errno == EPIPE) {
      jrKanjiPipeError();
    }
    jrKanjiError = "�����ȸ������Ф��ޤ���Ǥ���";
    return NG;
  }
#endif

  if (RkwGetHinshi(yc->context, mesg, sizeof(mesg) / sizeof(wchar_t)) < 0) {
    jrKanjiError = "�ʻ�������Ф��ޤ���Ǥ���";
    makeGLineMessageFromString(d, jrKanjiError);
    makeKanjiStatusReturn(d, yc);
    return 0;
  }

  makeKanjiStatusReturn(d, yc);
  d->kanji_status_return->info |= KanjiGLineInfo;
  d->kanji_status_return->gline.line = mesg;
  d->kanji_status_return->gline.length = WStrlen(mesg);
  d->kanji_status_return->gline.revPos = 0;
  d->kanji_status_return->gline.revLen = 0;
  d->flags |= PLEASE_CLEAR_GLINE;
  return 0;
}
#endif /* BUNPOU_DISPLAY */

#ifdef MEASURE_TIME
static
TanPrintTime(d)
uiContext	d;
{
  unsgined char tmpbuf[1024];
  static wchar_t buf[256];
  yomiContext yc = (yomiContext)d->modec;

  ycc->kouhoCount = 0;
  sprintf(tmpbuf, "�Ѵ����� %d [ms]������ UI ���� %d [ms]",
	   (yc->proctime) * 50 / 3,
	   (yc->proctime - yc->rktime) * 50 / 3);
  MBstowcs(buf, tmpbuf, 1024);
  d->kanji_status_return->info |= KanjiGLineInfo;
  d->kanji_status_return->gline.line = buf;
  d->kanji_status_return->gline.length = WStrlen(buf);
  d->kanji_status_return->gline.revPos = 0;
  d->kanji_status_return->gline.revLen = 0;
  d->kanji_status_return->length = -1;
  d->flags |= PLEASE_CLEAR_GLINE;
  return 0;
}
#endif /* MEASURE_TIME */

jrKanjiPipeError()
{
  extern defaultContext, defaultBushuContext;

  defaultContext = -1;
  defaultBushuContext = -1;

  makeAllContextToBeClosed(0);

  RkwFinalize();
#ifdef DEBUG
  if (iroha_debug) {
    fprintf(stderr, "��³���ڤ줿\n");
  }
#endif
}

/* cfuncdef

  TanBunsetsuMode -- ñ����⡼�ɤ���ʸ�῭�Ф��̤�⡼�ɤذܹԤ���

 */

static
TanBunsetsuMode(d)
uiContext	d;
{
  yomiContext yc = (yomiContext)d->modec;

  if (yc->id != YOMI_CONTEXT) {
    return enterTanHenkanMode(d, CANNA_FN_AdjustBunsetsu);
  }
  if (yc->right) {
    doTbResize(d, yc, 0);
    yc = (yomiContext)d->modec;
  }
  if (enterAdjustMode(d, yc) < 0) {
    return TanMuhenkan(d);
  }
  makeKanjiStatusReturn(d, yc);
  currentModeInfo(d);
  return 0;
}

static void
chikujiSetCursor(d, forw)
uiContext d;
int forw;
{
  yomiContext yc = (yomiContext)d->modec;

  if (forw) { /* ���ֺ��عԤ� */
    if (yc->nbunsetsu) { /* ʸ�᤬���롩 */
      gotoBunsetsu(yc, 0);
      moveToChikujiTanMode(d);
    }
    else {
      yc->kRStartp = yc->kCurs = yc->cStartp;
      yc->rStartp = yc->rCurs = yc->cRStartp;
      moveToChikujiYomiMode(d);
    }
  }
  else { /* ���ֱ��عԤ� */
    if (yc->cStartp < yc->kEndp) { /* �ɤߤ����롩 */
      yc->kRStartp = yc->kCurs = yc->kEndp;
      yc->rStartp = yc->rCurs = yc->rEndp;
      moveToChikujiYomiMode(d);
    }
    else {
      gotoBunsetsu(yc, yc->nbunsetsu - 1);
      moveToChikujiTanMode(d);
    }
  }
}


void
setMode(d, tan, forw)
uiContext d;
tanContext tan;
int forw;
{
  yomiContext yc = (yomiContext)tan;

  d->current_mode = yc->curMode;
  currentModeInfo(d);
  if (tan->id == YOMI_CONTEXT) {
    if (yc->generalFlags & CANNA_YOMI_CHIKUJI_MODE) {
      chikujiSetCursor(d, forw);
    }
    else if (yc->nbunsetsu) {
      if (forw) {
	gotoBunsetsu(yc, 0);
      }
      else {
	gotoBunsetsu(yc, yc->nbunsetsu - 1);
      }
    }
    else /* �ɤߥ⡼�� */ if (forw) {
      yc->kCurs = yc->kRStartp = yc->cStartp;
      yc->rCurs = yc->rStartp = yc->cRStartp;
    }
    else {
      yc->kCurs = yc->kRStartp = yc->kEndp;
      yc->rCurs = yc->rStartp = yc->rEndp;
    }
  }
}

int
TbForward(d)
uiContext d;
{
  tanContext tan = (tanContext)d->modec;

  if (tan->right) {
    d->modec = (mode_context)tan->right;
    tan = (tanContext)d->modec;
  }
  else if (CursorWrap && tan->left) {
    while (tan->left) {
      tan = tan->left;
    }
    d->modec = (mode_context)tan;
  }
  else {
    return NothingChanged(d);
  }
  setMode(d, tan, 1);
  makeKanjiStatusReturn(d, (yomiContext)d->modec);
  return 0;
}

int
TbBackward(d)
uiContext d;
{
  tanContext tan = (tanContext)d->modec;

  if (tan->left) {
    d->modec = (mode_context)tan->left;
    tan = (tanContext)d->modec;
  }
  else if (CursorWrap && tan->right) {
    while (tan->right) {
      tan = tan->right;
    }
    d->modec = (mode_context)tan;
  }
  else {
    return NothingChanged(d);
  }
  setMode(d, tan, 0);
  makeKanjiStatusReturn(d, (yomiContext)d->modec);
  return 0;
}

int
TbBeginningOfLine(d)
uiContext d;
{
  tanContext tan = (tanContext)d->modec;

  while (tan->left) {
    tan = tan->left;
  }
  d->modec = (mode_context)tan;
  setMode(d, tan, 1);
  makeKanjiStatusReturn(d, (yomiContext)d->modec);
  return 0;
}

int
TbEndOfLine(d)
uiContext d;
{
  tanContext tan = (tanContext)d->modec;

  while (tan->right) {
    tan = tan->right;
  }
  d->modec = (mode_context)tan;
  setMode(d, tan, 0);
  makeKanjiStatusReturn(d, (yomiContext)d->modec);
  return 0;
}

#include	"tanmap.c"
