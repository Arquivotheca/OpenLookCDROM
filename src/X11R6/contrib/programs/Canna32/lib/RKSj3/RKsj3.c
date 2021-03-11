/* Copyright 1993 NEC Corporation, Tokyo, Japan.
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

/*
 * RKsj3.c
 *	RK ��Ϣʸ���Ѵ��饤�֥����б�������
 *
 *	1993.6: kon
 */

#ifndef lint
static char rcsid[]="$Id: RKsj3.c,v 1.29 1994/01/17 05:18:24 kon Exp $";
#endif

#include <canna/widedef.h>

#ifdef ENGINE_SWITCH
#include "RKrename.h"
#endif

#include <canna/RK.h>

#ifdef HAVE_WCHAR_OPERATION
#include <locale.h>
#endif

#include "sj3lib.h"

#ifndef RK_DEFAULT_SJ3SERV
#define RK_DEFAULT_SJ3SERV	"unix"
#endif

#ifndef YES
#define YES 1
#endif
#ifndef NO
#define NO 0
#endif

#if __STDC__ || defined(SYSV) || defined(SVR4)
#define bcopy(x, y, z) memcpy(y, x, z)
#endif

#define NSTEP 16 /* ����ƥ����Ȥ����䤹���Υ��ƥå� */

struct candidate { /* �������������Ƥ����Ȥ��� */
  wchar_t *kanji;
  int len, artificial;
  struct studyrec dcid;
};

static void
freeCandidate(c, n)
struct candidate *c;
int n;
{
  int i;

  for (i = 0 ; i < n ; i++) {
    if (c[i].kanji) {
      free(c[i].kanji);
    }
  }
  free(c);
}

struct clause { /* ʸ��ξ��������Ƥ����Ȥ��� */
  int gotall; /* ����������Ѥߤ� */
  wchar_t *yomi;
  int iniyomilen, yomilen;
  int numcand, curcand;
  struct candidate *cand;
};

static void
freeClause(c, n)
struct clause *c;
int n;
{
  int i;

  for (i = 0 ; i < n ; i++) {
    if (c[i].cand) {
      freeCandidate(c[i].cand, c[i].numcand);
    }
  }
  free(c);
}

struct context { /* ����ƥ����� */
  int      vacant; /* �����Ƥ��뤫 */
  wchar_t *yomi;
  int      len;
  int      numbun, curbun;
  struct clause *bun;
};

/*
static void
freeContext(c)
struct context *c;
{
  if (c->yomi) {
    free(c->yomi);
  }
  if (c->numbun > 0 && c->bun) {
    freeClause(c->bun, c->numbun);
  }
  free(c);
}
*/

static struct context **contexts; /* ���ƤΥ���ƥ����ȤϤ������黲�Ȳ� */
static int ncontexts, mcontext;   /* ���ƤΥ���ƥ����Ȥο��ʤ�.. */

/* �ޥ������

  validContext(cn)  cn ��ɽ����륳��ƥ����Ȥ� Create ���줿�����ʤ�Τ�
  henkanChuu(con)   con �����äƤ��륳��ƥ����ȥݥ��󥿤��Ѵ���Τ�Τ�

 */
#define validContext(cn) ((0 <= (cn) && (cn) < mcontext && contexts[cn] && \
  !contexts[cn]->vacant) ? contexts[cn] : (struct context *)0)
#define henkanChuu(con) ((con)->numbun)

static int wchar_type; /* wchar_t ������ɽ�����ɿ魯�뤷���� */

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
  unsigned char *a = (unsigned char *)"��"; /* 0xa4a2 */
  wchar_t wc[24];

#ifdef HAVE_WCHAR_OPERATION
  if (mbstowcs(wc, a, sizeof(wc) / sizeof(wchar_t)) != 1) {
    /* ¿ʬ setlocale ���ʤ���Ƥ��ʤ� */
    (void)setlocale(LC_CTYPE, "");
    if (mbstowcs(wc, a, sizeof(wc) / sizeof(wchar_t)) != 1) {
      (void)setlocale(LC_CTYPE, JAPANESE_LOCALE);
      if (mbstowcs(wc, a, sizeof(wc) / sizeof(wchar_t)) != 1) {
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
# ifdef _WCHAR16

  wchar_type = CANNA_WCTYPE_16;

# else /* !_WCHAR16 */

  if (sizeof(wchar_t) == 2) {
    wchar_type = CANNA_WCTYPE_16;
  }
  else {
    wchar_type = CANNA_WCTYPE_32;
  }

# endif /* !_WCHAR16 */
#endif /* !HAVE_WCHAR_OPERATION */

  return 0;
}

#define G1MASK 0x03000000L
#define G2MASK 0x01000000L
#define G3MASK 0x02000000L

/*
 ��̤ΥХ��ȿ����֤�
 */

static int
WS2SS(s, n, d, m)
wchar_t *s;
unsigned char *d;
int m, n;
{
  unsigned hi, lo, pl;
  wchar_t ch;
  int mm = m;

  while (n-- > 0 && m > 1) {
    ch = *s++;
    if (wchar_type == CANNA_WCTYPE_16) {
      hi = ((unsigned)ch >> 8) & 0x7f;
      lo = (unsigned)ch & 0x7f;
      pl = (((unsigned)ch & 0x8000) >> 14) | (((unsigned)ch & 0x80) >> 7);
    }
    else {
      hi = ((unsigned)ch >> 7) & 0x7f;
      lo = (unsigned)ch & 0x7f;
      pl = (unsigned)ch >> 28;
    }
    switch (pl) {
    case 0:
      *d++ = lo; m--;
      break;
    case 3:
      lo += (hi & 1) ? 0x1f : 0x7d;
      if (lo > 0x7f) lo++;
      hi = (hi - 0x21) / 2 + 0x81;
      if (hi > 0x9f) hi += 0x40;
      *d++ = hi; *d++ = lo; m -= 2;
      break;
    case 1:
      *d++ = (lo | 0x80); m--;
      break;
    case 2: /* can not translate to SJIS */
      *d++ = '#'; m--;
      break;
    }
  }
  return mm - m;
}

/*
 byte �����֤�
 */

static int
SS2WS(sj, n, d, m)
unsigned char *sj;
wchar_t *d;
int m, n;
{
  unsigned char	*s = sj;
  unsigned char *S = sj + n;
  wchar_t *e = d;
  unsigned	hi, lo;
  int 		count = 0;
  
  while (m-- > 0 && s < S) {
    hi = (unsigned)*s++;
    if ( hi <= 0x7f ) { /* ascii */
      *e++ = hi;
    }
    else if ( 0xa0 <= hi && hi <= 0xdf ) { /* hankaku katakana */
      if (wchar_type == CANNA_WCTYPE_16) {
	*e++ = hi;
      }
      else {
	*e++ = (G2MASK | (hi & 0x7f));
      }
    }
    else {
      hi -= (hi <= 0x9f) ? 0x80 : 0xc0;
      hi = 2 * hi + 0x20;
      if ((lo = *s++) <= 0x9e) { /* kisuu ku */
	hi--;
	if (0x80 <= lo) lo--;
	lo -= (0x40 - 0x21);
      } else { /* guusuu ku */
	lo -= (0x9f - 0x21);
      }
      if (wchar_type == CANNA_WCTYPE_16) {
	*e++ = 0x8080 | (hi << 8) | lo;
      }
      else {
	*e++ = G1MASK | ((hi & 0x7f) << 7) | (lo & 0x7f);
      }
    }
    count++;
  }
  return count;
}

static int
SJlen(sj, n)
unsigned char *sj;
int n;
{
  unsigned char	*s = sj;
  unsigned char *S = sj + n;
  unsigned	hi;
  int 		count = 0;
  
  while (s < S) {
    hi = (unsigned)*s++;
    if ( hi <= 0x7f ) /* ascii */
      ;
    else if ( 0xa0 <= hi && hi <= 0xdf ) /* hankaku katakana */
      ;
    else
      s++;
    count++;
  }
  return count;
}

static void
WSncpy(d, s, n)
wchar_t *d, *s;
int n;
{
  while (n--) {
    *d++ = *s++;
  }
}

static int
WSncmp(d, s, n)
wchar_t *d, *s;
int n;
{
  while (n--) {
    if (*d != *s) {
      return *d - *s;
    }
    d++; s++;
  }
  return 0;
}

static int
WSlen(s)
wchar_t *s;
{
  wchar_t *e = s;

  while (*e++);

  return e - s;
}

#define SJ3SERV_AT "sj3serv@"

static char sj3serv_name[256] = SJ3SERV_AT;
static char sj3user_name[256] = "";

static char *
FindLogname()
{
  char *username = (char *)0, *getenv(), *getlogin();

  if ((username = getlogin()) == (char *)0) {
    if ((username = getenv("LOGNAME")) == (char *)0) {	
      username = getenv("USER");
    }
  }
  return username;
}

static int disconnected = YES, dicerror = NO;

/* ����� */
int RkwInitialize(home)
char *home; /* ����ǥ��쥯�ȥ� */
{
  extern char *getenv();
  char *p;

  if (!disconnected) { /* ���٤� initialize ���ʤ� */
    return -1;
  }

  (void)WCinit();
  if( home && home[0] != '/' ) {
    (void)strcpy(sj3serv_name + strlen(SJ3SERV_AT), home);
  }
  else if (p = getenv("SJ3SERV")) {
    (void)strcpy(sj3serv_name + strlen(SJ3SERV_AT), p);
  }
  else {
    (void)strcpy(sj3serv_name + strlen(SJ3SERV_AT), RK_DEFAULT_SJ3SERV);
  }
  if (p = FindLogname()) {
    (void)strcpy(sj3user_name, p);
  }

  disconnected = sj3_open(sj3serv_name + strlen(SJ3SERV_AT), sj3user_name);

  dicerror = (disconnected & (SJ3_CANNOT_OPEN_MDICT |
			      SJ3_CANNOT_OPEN_UDICT |
			      SJ3_CANNOT_OPEN_STUDY |
			      SJ3_CANNOT_MAKE_UDIR  |
			      SJ3_CANNOT_MAKE_UDICT |
			      SJ3_CANNOT_MAKE_STUDY));

  if (disconnected & (SJ3_SERVER_DEAD | SJ3_CONNECT_ERROR)) {
    return ( -1 );
  }
  disconnected = NO;

  return RkwCreateContext();
}


/* ��λ���� */
void RkwFinalize()
{
  int i;
  struct context *con;

  /* free all contexts */
  for (i = 0 ; i < mcontext ; i++) {
    if (con = validContext(i)) {
      if (henkanChuu(con)) {
	(void)RkwEndBun(i, 0);
      }
    }
    if (contexts[i]) {
      free(contexts[i]);
    }
  }
  free(contexts);
  ncontexts = mcontext = 0;
  sj3_close();
  disconnected = YES;
}


/* ����ե�����Υ����ץ�

  SJ3 �ϼ����ͳ�˻��Ĥ��Ȥ��Ǥ��ʤ��Τǰʲ��Τ褦�˼�갷����

  (1) sj3_open �Ǽ����Ϣ�Υ��顼���ФƤ�������ƤΥޥ�����׵���Ф�
      ���顼���֤���

  (2) sj3_open �Ǽ����Ϣ�Υ��顼��̵��������ƤΥޥ�����׵���Ф���
      �������פ��֤���

 */

int RkwMountDic(ctx, name, mode)
int ctx;
char *name;
int mode;
/*ARGSUSED*/
{
  if (dicerror) {
    return -1;
  }
  else {
    return 0;
  }
}

static unsigned char sbuf[1024], skbuf[1024];
static int maxbuf = 1024;
static struct bunsetu sbun[1024];
static wchar_t wcbuf[1024];
static int wcsize = 1024;
static struct douon dou[1024];

/* renbun -- Ϣʸ���Ѵ��롼����

  RkwBgnBun ��¾��RkwResize �ʤɤǤ⤳�δؿ���ƤӽФ���

  yomi ��ľ���������Ѥ�����offset ��­�����Ȥ����黲�Ȥ��롣

 */

static int
renbun(con, offset, n, nbun, bun_return)
struct context *con;
int n, offset, nbun;
struct clause **bun_return;
{
  int len, res, totalyomilen = offset, i, j;
  struct clause *wbun;

  len = WS2SS(con->yomi + offset, n, sbuf, maxbuf);
  sbuf[len] = (unsigned char)0;
  if ((res = sj3_getkan(sbuf, sbun, skbuf, maxbuf)) > 0) {
    if (wbun =
	(struct clause *)malloc((nbun + res) * sizeof(struct clause))) {
      for (i = 0 ; i < res ; i++) {
	wbun[i + nbun].gotall = NO;
	wbun[i + nbun].curcand = 0;
	wbun[i + nbun].numcand = 1;
	if (wbun[i + nbun].cand =
	    (struct candidate *)malloc(sizeof(struct candidate))) {
	  n = SS2WS(sbun[i].deststr, sbun[i].destlen, wcbuf, wcsize);
	  wbun[i + nbun].yomi = con->yomi + totalyomilen;
	  totalyomilen += wbun[i + nbun].iniyomilen =
	    wbun[i + nbun].yomilen = SJlen(sbun[i].srcstr, sbun[i].srclen);
	  if (wbun[i + nbun].cand->kanji =
	      (wchar_t *)malloc(n * sizeof(wchar_t))) {
            WSncpy(wbun[i + nbun].cand->kanji, wcbuf, n);
	    wbun[i + nbun].cand->len = n;
#ifndef OBSOLETE_C
	    wbun[i + nbun].cand->dcid = sbun[i].dcid;
#else
	    (void)bcopy(&sbun[i].dcid,
			&(wbun[i + nbun].cand->dcid), sizeof(struct studyrec));
#endif
	    wbun[i + nbun].cand->artificial = NO;
	    continue;
	  }
	  free(wbun[i + nbun].cand);
	}
	for (wbun += nbun, j = 0 ; j < i ; j++) {
	  freeCandidate(wbun->cand, wbun->numcand);
	}
	return -1;
      }
      *bun_return = wbun;
      return res;
    }
  }
  return -1;
}

/* �Ѵ��γ��� */
int RkwBgnBun(ctx, yomi, n, mode)
int ctx;
wchar_t	*yomi;
int n, mode;
/* ARGSUSED */
{
  struct context *con;
  struct clause *wbun;
  int res;

  if ((con = validContext(ctx)) && !henkanChuu(con)) {
    if (con->yomi = (wchar_t *)malloc(n * sizeof(wchar_t))) {
      WSncpy(con->yomi, yomi, n);
      con->len = n;
      sj3_lockserv();
      if ((res = renbun(con, 0, n, 0, &wbun)) > 0) {
	con->bun = wbun;
	con->numbun = res;
	con->curbun = 0;
	return res;
      }
      sj3_unlockserv();
      free(con->yomi);
      con->yomi = (wchar_t *)0;
      con->len = 0;
    }
  }
  return -1;
}


/* �Ѵ��ν�λ */
/*ARGSUSED*/  
int RkwEndBun(ctx,  mode)
int	ctx;
int	mode;			/* �ؽ�����Υե饰 */
{
  int i, nbun;
  struct context *con;
  struct clause *wbun;
  struct candidate *can;

  if ((con = validContext(ctx)) && henkanChuu(con)) {
    wbun = con->bun;
    nbun = con->numbun;
    if (mode) { /* �ؽ�����ΤǤ���� */
      for (i = 0 ; i < nbun ; i++) {
	can = wbun[i].cand + wbun[i].curcand;
	if (!can->artificial) {
	  sj3_gakusyuu(&can->dcid);
	}
	if (wbun[i].iniyomilen != wbun[i].yomilen &&
	    !wbun[i].cand[0].artificial) { /* ʸ��Ĺ���Ѥ�ä� */
	  unsigned char *xx = sbuf;
	  struct studyrec *yy;
	  int sjlen, sjlen2;

	  sjlen = WS2SS(wbun[i].yomi, wbun[i].yomilen, xx, maxbuf);
	  xx[sjlen] = (unsigned char)0;
	  xx += sjlen + 1;
	  if (i + 1 < nbun) {
	    sjlen2 = WS2SS(wbun[i + 1].yomi, wbun[i + 1].yomilen,
			   xx, maxbuf - sjlen - 1);
	    xx[sjlen2] = (unsigned char)0;
	    yy = &(wbun[i + 1].cand->dcid);
	  }
	  else {
	    *xx = (unsigned char)0;
	    yy = (struct studyrec *)0;
	  }
	  sj3_gakusyuu2(sbuf, sbuf + sjlen + 1, yy);
	}
      }
    }
    freeClause(wbun, con->numbun);
    con->bun = (struct clause *)0;
    free(con->yomi); con->yomi = (wchar_t *)0;
    con->numbun = 0;
    sj3_unlockserv();
    return 0;
  }
  return -1;
}


/* ������ʸ����ѹ� */
int RkwGoTo(ctx, bnum)
int	ctx;
int	bnum;
{
  struct context *con;

  if ((con = validContext(ctx)) && henkanChuu(con)) {
    if (0 <= bnum && bnum < con->numbun) {
      con->curbun = bnum;
    }
    return 0;
  }
  return -1;
}


/* �������ʸ��κ���ư */
int RkwLeft(ctx)
int	ctx;
{
  struct context *con;

  if ((con = validContext(ctx)) && henkanChuu(con)) {
    if (--(con->curbun) < 0) {
      con->curbun = con->numbun - 1;
    }
    return 0;
  }
  return -1;
}


/* ������ʸ��α���ư */
int RkwRight(ctx)
int	ctx;
{
  struct context *con;

  if ((con = validContext(ctx)) && henkanChuu(con)) {
    if (con->numbun <= ++(con->curbun)) {
      con->curbun = 0;
    }
    return 0;
  }
  return -1;
}

/* getKanjiList -- ��������μ���

  RkwGetKanjiList �˸¤餺��RkwNext �ʤɤ����ƤӽФ���롣

 */

static int
getKanjiList(con)
struct context *con;
{
  struct clause *wbun = con->bun + con->curbun;
  struct candidate *can;
  int i, j, n, len, usekana = 0, kanalen;

  n = WS2SS(wbun->yomi, wbun->yomilen, sbuf, maxbuf);
  sbuf[n] = (unsigned char)0;
#ifdef notdef
  if ((kanalen =
       RkwCvtKana(wcbuf, maxbuf, wbun->yomi, wbun->yomilen)) > 0) {
    if (kanalen != wbun->yomilen ||
	WSncmp(wcbuf, wbun->yomi, wbun->yomilen)) {
      usekana = 1;
    }
  }
#endif
  if ((n = sj3_getdouon(sbuf, dou)) >= 0) {
    if (can = (struct candidate *)
	malloc((n + usekana + 1)* sizeof(struct candidate))) {
      for (i = 0 ; i < n ; i++) {
	len = SJlen(dou[i].ddata, dou[i].dlen);
	if (can[i].kanji = (wchar_t *)malloc(len * sizeof(wchar_t))) {
	  (void)SS2WS(dou[i].ddata, dou[i].dlen, can[i].kanji, len);
	  can[i].len = len;
#ifndef OBSOLETE_C
	  can[i].dcid = dou[i].dcid;
#else
	  (void)bcopy(&dou[i].dcid, &can[i].dcid, sizeof(struct studyrec));
#endif
	  can[i].artificial = NO;
	  continue;
	}
      getKanjiListError:
	for (j = 0 ; j < i ; j++) {
	  free(can[j].kanji);
	}
	free(can);
	return -1;
      } /* for.. */
      if (usekana) {
	if (can[i].kanji = (wchar_t *)malloc(kanalen * sizeof(wchar_t))) {
	  can[i].len = kanalen;
	  WSncpy(can[i].kanji, wcbuf, kanalen);
	  can[i].artificial = YES;
	  i++;
	}
	else {
	  goto getKanjiListError;
	}
      }

      /* hira */
      if (can[i].kanji =
	  (wchar_t *)malloc(wbun->yomilen * sizeof(wchar_t))) {
	can[i].len = wbun->yomilen;
	WSncpy(can[i].kanji, wbun->yomi, wbun->yomilen);
	can[i].artificial = YES;
	i++;
      }
      else {
	goto getKanjiListError;
      }

      if (wbun->cand) { /* �����Τ� free */
	free(wbun->cand->kanji);
	free(wbun->cand);
      }
      wbun->cand = can;
      wbun->gotall = YES;
      return wbun->numcand = n + usekana + 1;
    }
  }
  return -1;
}

/* ������ʸ��򼡸�����֤������� */
int RkwNext(ctx)
int	ctx;
{
  struct context *con;
  struct clause *wbun;

  if ((con = validContext(ctx)) && henkanChuu(con)) {
    wbun = con->bun + con->curbun;
    if (!wbun->gotall) {
      if (getKanjiList(con) < 0) {
	return -1;
      }
    }
    if (wbun->numcand <= ++(wbun->curcand)) {
      wbun->curcand = 0;
    }
    return 0;
  }
  return -1;
}


/* ������ʸ���ľ���θ�����֤������� */
int RkwPrev(ctx)
int	ctx;
{
  struct context *con;
  struct clause *wbun;

  if ((con = validContext(ctx)) && henkanChuu(con)) {
    wbun = con->bun + con->curbun;
    if (!wbun->gotall) {
      if (getKanjiList(con) < 0) {
	return -1;
      }
    }
    if (--(wbun->curcand) < 0) {
      wbun->curcand = wbun->numcand - 1;
    }
    return 0;
  }
  return -1;
}


/* ������ʸ����б��������������ΰ�kanji�����ꤷ�����Υ��ɥ쥹���֤� */
int
RkwGetKanji(ctx, kanji, MAX)
int ctx;
wchar_t *kanji;
int MAX;
{
  struct context *con;

  if ((con = validContext(ctx)) && henkanChuu(con)) {
    struct clause *wbun;
    struct candidate *wcand;
    int len;

    wbun = con->bun + con->curbun;
    wcand = wbun->cand + wbun->curcand;
    len = (MAX < wcand->len) ? MAX : wcand->len;

    WSncpy(kanji, wcand->kanji, len);
    kanji[len] = (wchar_t)0;
    return len;
  }
  return -1;
}

/* ������������ */
int
RkwGetKanjiList(ctx, kouho, bufsize)
int ctx;
wchar_t *kouho;
int bufsize;
{
  struct context *con;
  struct clause *wbun;
  wchar_t *wp = kouho;
  int i, n, mbuf = bufsize;

  if ((con = validContext(ctx)) && henkanChuu(con)) {
    wbun = con->bun + con->curbun;
    if (!wbun->gotall) {
      if (getKanjiList(con) < 0) {
	return -1;
      }
    }
    n = wbun->numcand;
    for (i = 0 ; i < n ; i++) {
      struct candidate *wcand;
      int len;

      wcand = wbun->cand + i;
      len = (mbuf - 2 < wcand->len) ? mbuf - 2 : wcand->len;

      WSncpy(wp, wcand->kanji, len);
      wp[len] = (wchar_t)0;
      wp += len + 1;
      mbuf -= len + 1;
    }
    *wp = (wchar_t)0;
    return n;
  }
  return -1;
}


/* ���ꤵ�줿�����ֹ�򥫥��ȸ���ˤ��� */
int
RkwXfer(ctx, knum)
int ctx;
int knum;
{
  struct context *con;
  struct clause *wbun;

  if ((con = validContext(ctx)) && henkanChuu(con)) {
    wbun = con->bun + con->curbun;
    if (!wbun->gotall) {
      if (getKanjiList(con) < 0) {
	return -1;
      }
    }
    if (0 <= knum && knum < wbun->numcand) {
      wbun->curcand = knum;
      return 0;
    }
  }
  return -1;
}

/* bunResize -- ʸ��Ĺ���ѹ���

  RkwResize��RkwEnlarge��RkwShorten ����ƤӽФ���롣

  ��äƤ��뤳�Ȥϡ�

   (1) �ꥵ�����������;�ä���ʬ��Ϣʸ���Ѵ����롣

   (2) �ꥵ�������оݤȤʤ�ʸ���ñʸ���Ѵ����롣

  (1)�����(2)�κ���˥��顼�������ä����ˤϲ��⤻�� -1 ���֤�������
  ʸ��Ĺ���⿭�Ф����Ȥ����ꡢ�Ǿ�ʸ��Ĺ����̤�褦�Ȥ������ˤ�
  �����ѹ������꥿���󤹤롣

  ���Υ롼����� sj3 �Ǻ��Τ����ݤ��ä���(^^;)

 */

static int
bunResize(con, len)
struct context *con;
int len;
{
  struct clause *wbun, *savedbun;
  wchar_t *curyomi;
  int yomilen, yoffset, res, i, nbun, savednum;

  nbun = con->curbun;
  savedbun = con->bun;
  savednum = con->numbun;
  for (i = 0, yoffset = 0 ; i < nbun ; i++) {
    yoffset += savedbun[i].yomilen;
  }
  curyomi = con->yomi + yoffset;
  yomilen = con->len - yoffset - len;
  if (len <= 0 || yomilen < 0) {
    return con->numbun;
  }
  if (yomilen > 0) {
    res = renbun(con, yoffset + len, yomilen, nbun + 1, &wbun);
  }
  else { /* yomilen == 0 */
    if (wbun = (struct clause *)malloc((nbun + 1) * sizeof(struct clause))) {
      res = 0;
    }
    else {
      res = -1;
    }
  }
  if (res >= 0) {
    con->bun = wbun;
    con->numbun = nbun + 1 + res;
    wbun[nbun].gotall = NO;
    wbun[nbun].yomi = curyomi;
    wbun[nbun].yomilen = len;
    wbun[nbun].numcand = wbun[nbun].curcand = 0;
    wbun[nbun].cand = (struct candidate *)0;
    if (getKanjiList(con) > 0) {
      for (i = nbun ; i < savednum ; i++) {
	if (savedbun[i].cand) {
	  freeCandidate(savedbun[i].cand, savedbun[i].numcand);
	}
      }
      (void)bcopy(savedbun, wbun, nbun * sizeof(struct clause));
      free(savedbun);
      con->curbun = nbun;
      return nbun + 1 + res;
    }
    for (i = nbun + 1 ; i < nbun + 1 + res ; i++) {
      free(wbun[i].yomi);
      if (wbun[i].cand) {
	freeCandidate(wbun[i].cand, wbun[i].numcand);
      }
    }
    free(wbun);
    con->bun = savedbun;
    con->numbun = savednum;
  }
  return -1;
}

/* ������ʸ����ɤߤ��ʤ�Ĺ�� len �ˤ��� */
int
RkwResize(ctx, len)
int ctx;
int len;		/* len �ϡ�EUC�����ɤǤΥХ��ȿ���Ϳ���� */
{
  struct context *contex;

  if ((contex = validContext(ctx)) && henkanChuu(contex)) {
    return bunResize(contex, len);
  }
  return -1;
}


/* ������ʸ���Ĺ����̤�� */
int RkwShorten(ctx)
int	ctx;
{
  struct context *con;

  if ((con = validContext(ctx)) && henkanChuu(con)) {
    return bunResize(con, con->bun[con->curbun].yomilen - 1);
  }
  return -1;
}


/* ������ʸ���Ĺ���򿭤Ф� */
int RkwEnlarge(ctx)
int	ctx;
{
  struct context *con;

  if ((con = validContext(ctx)) && henkanChuu(con)) {
    return bunResize(con, con->bun[con->curbun].yomilen + 1);
  }
  return -1;
}


/* ������ʸ���̵�Ѵ��ξ��֤��᤹ */
int RkwNfer(ctx)
int	ctx;
{
  struct context *con;
  struct clause *wbun;

  if ((con = validContext(ctx)) && henkanChuu(con)) {
    wbun = con->bun + con->curbun;
    if (!wbun->gotall) {
      if (getKanjiList(con) < 0) {
	return -1;
      }
    }
    wbun->curcand = wbun->numcand - 1;
    return 0;
  }
  return -1;
}


/* ������ʸ����б������ɤߤ��ʤ��ΰ�yomi�����ꤷ�����Υ��ɥ쥹���֤�	*/
int	RkwGetYomi(ctx, yomi, MAX)
int	ctx;
wchar_t	*yomi;
int	MAX;
{
  struct context *con;

  if ((con = validContext(ctx)) && henkanChuu(con)) {
    struct clause *wbun;
    int len;

    wbun = con->bun + con->curbun;
    len = (MAX < wbun->yomilen) ? MAX : wbun->yomilen;

    WSncpy(yomi, wbun->yomi, len);
    yomi[len] = (wchar_t)0;
    return len;
  }
  return -1;
}

/* ����ƥ����Ƚ��� 

  sj3 �ϥ��ơ��ȥ쥹�Ǥ��ꡢ����ƥ����Ȥγ�ǰ��̵���Τǡ����Υ饤�֥� 
  ����ǥ���ƥ����Ȥ���������Ѥ��롣����ƥ����Ȥ�̵���¤˺��뤳��
  �Ȥ����ʲ��Υ롼����Ǵ������롣

  ����ƥ����ȴ����ˤϣ��Ĥ��ѿ����Ѥ��롣

  ncontexts   ���ƤΥ���ƥ����Ȥ��������������礭�����ݤ��ѿ�
  mcontext    malloc ���Ƥ��륳��ƥ����Ȥο����ݤ��ѿ�

 */


#define newcontex() ((struct context *)malloc(sizeof(struct context)))
static struct context *
newcontext()
{
  struct context *con = newcontex();

  if (con) {
    con->vacant = YES;
    con->yomi = (wchar_t *)0;
    con->len = con->numbun = con->curbun = 0;
    con->bun = (struct clause *)0;
  }
  return con;
}

RkwCreateContext()
{
  int i;

  for (i = 0 ; i < mcontext ; i++) { /* �����ˤʤäƤ���Τ�õ�� */
    if (!contexts[i]) { /* ȴ���̤����ʤ��ä� */
      if (contexts[i] = newcontext()) {
	contexts[i]->vacant = NO;
	return i;
      }
      else {
	return -1;
      }
    }
    else if (contexts[i]->vacant) { /* ȴ���̤Ϥ��ä� */
      contexts[i]->vacant = NO;
      return i;
    }
  }

  if (ncontexts > mcontext) { /* ����ݥ��󥿤ޤǤ��ä�����ä����ΰ�Ϥ��� */
    if (contexts[mcontext] = newcontext()) {
      contexts[mcontext]->vacant = NO;
      return mcontext++;
    }
    else {
      return -1;
    }
  }
  else { /* ����ƥ����ȿ������䤵�ʤ���Фʤ�ʤ� */
    if (ncontexts > 0) {
      struct context **xx;
      xx = (struct context **)
	realloc(contexts, (ncontexts + NSTEP) * sizeof(struct context *));
      if (!xx) {
	return -1;
      }
      contexts = xx;
      ncontexts += NSTEP;
    }
    else {
      contexts = (struct context **)malloc(NSTEP * sizeof(struct context *));
      if (!contexts) {
	return -1;
      }
      ncontexts = NSTEP;
    }
    if (contexts[mcontext] = newcontext()) {
      contexts[mcontext]->vacant = NO;
      return mcontext++;
    }
    else {
      return -1;
    }
  }
}

RkwDuplicateContext(cn)
int cn;
/* ARGSUSED */
{
  /* ����ƥ����Ȥˤ�äƥޥ���Ȥ��Ƥ��뼭�񤬰㤦�äƻ���ʤ��Τǡ�
     RkwCreateContext �򤽤Τޤ��Ѥ��롣 */
  return RkwCreateContext();
}

RkwCloseContext(cn)
int cn;
{
  struct context *con;

  if (con = validContext(cn)) {
    con->vacant = YES;
    if (cn == mcontext) {
      while (0 <= mcontext && contexts[mcontext]->vacant) {
	free(contexts[mcontext]);
	contexts[mcontext] = (struct context *)0;
	mcontext--;
      }
    }
  }
}

/* ������ʸ��Υ����ȸ���ξ��֤���Ф� */
RkwGetStat(cn, stat)
int cn;
RkStat *stat;
{
  struct context *con;

  if ((con = validContext(cn)) && henkanChuu(con)) {
    struct clause *wbun;
    struct candidate *wcand;

    wbun = con->bun + con->curbun;
    wcand = wbun->cand + wbun->curcand;

    stat->bunnum = con->curbun;
    stat->candnum = wbun->curcand;
    stat->maxcand = wbun->numcand;
    stat->diccand = !wcand->artificial;
    stat->ylen = wbun->yomilen;
    stat->klen = wcand->len;
    stat->tlen = 1;/* ����Ǥ� */
    return 0;
  }
  return -1;
}

/*
 * ��ʸ���ñ��Ȥ��������ʲ���򤷤Ƥ��롣����Ͼ���Ū�ˤ�ľ��������
 */

RkwGetLex(cn, lex, maxlex)
int cn, maxlex;
RkLex *lex;
/* ARGSUSED */
{
  return -1;
}

/* �������鲼�϶��äݤǤ� */
RkwSetDicPath(cn, path)
int cn;
char *path;
/* ARGSUSED */
{
    return 0;
}

RkwCreateDic(cn, dicname, mode)
int cn, mode;
unsigned char *dicname;
/* ARGSUSED */
{
    return -1;
}

RkwSync(cn, dicname, mode)
int cn, mode;
unsigned char *dicname;
/* ARGSUSED */
{
    return -1;
}

RkwGetDicList(cn, dicnames_return, MAX)
int cn;
char *dicnames_return;
int MAX;
/* ARGSUSED */
{
  return 0;
}

RkwGetMountList(cn, dicnames_return, MAX)
int cn;
char *dicnames_return;
int MAX;
/* ARGSUSED */
{
  return 0;
}

RkwUnmountDic(cn, dicname)
int cn;
unsigned char *dicname;
/* ARGSUSED */
{
  return 0;
}

RkwRemountDic(cn, dicname, where)
int cn, where;
unsigned char *dicname;
/* ARGSUSED */
{
  return 0;
}

RkwStoreYomi(cn, yomi, MAX) /* ����Ϥ����Ǥʤ��Ρ�*/
int cn, MAX;
char *yomi;
/* ARGSUSED */
{
  return -1;
}

RkwDefineDic(cn, dicname, wordrec) /* ������ߤ��� */
int cn;
unsigned char *dicname, *wordrec;
/* ARGSUSED */
{
  return -1;
}

RkwDeleteDic(cn, dicname, wordrec)
int cn;
unsigned char *dicname, *wordrec;
/* ARGSUSED */
{
  return -1;
}

RkGetProtocolVersion(ma, mi)
int *ma, *mi;
{
  if (ma) {
    *ma = 1;
  }
  if (mi) {
    *mi = 1;
  }
  return 0;
}

RkGetServerVersion(ma, mi)
int *ma, *mi;
{
  if (ma) {
    *ma = 1;
  }
  if (mi) {
    *mi = 1;
  }
  return 0;
}

char *
RkGetServerName()
{
  return sj3serv_name;
}

RkwGetHinshi()
{
  return -1;
}

RkwSubstYomi()
{
  return -1;
}

RkwFlushYomi()
{
  return -1;
}

RkwRemoveBun()
{
  return -1;
}

RkwGetLastYomi()
{
  return -1;
}

/*
 *  RkwSetAppName ()
 *
 *  Description:
 *  -----------
 *  ���ץꥱ�������̾����Ͽ
 *
 *  Input:
 *  -----
 *  apname: ���ץꥱ�������̾
 *
 *  Returns:
 *  -------
 *  0 or -1
 */
int
RkwSetAppName( cxnum, apname )					/* S003 */
int cxnum;
unsigned char *apname;
/* ARGSUSED */
{
    return( -1 ) ;
}
							/* S003:begin */
int
RkSetAppName( cxnum, apname )
int cxnum;
unsigned char *apname;
{
    return( RkwSetAppName( cxnum, apname ) );
}							/* S003:end */
							/* S000:begin */


#ifdef ENGINE_SWITCH
struct rkfuncs RkFuncs = {
  RkGetProtocolVersion,
  RkGetServerName,
  RkGetServerVersion,
  RkwInitialize,
  RkwFinalize,
  RkwCreateContext,
  RkwDuplicateContext,
  RkwCloseContext,
  RkwSetDicPath,
  RkwCreateDic,
  RkwSync,
  RkwGetDicList,
  RkwGetMountList,
  RkwMountDic,
  RkwRemountDic,
  RkwUnmountDic,
  RkwDefineDic,
  RkwDeleteDic,
  RkwGetHinshi,
  RkwGetKanji,
  RkwGetYomi,
  RkwGetLex,
  RkwGetStat,
  RkwGetKanjiList,
  RkwFlushYomi,
  RkwGetLastYomi,
  RkwRemoveBun,
  RkwSubstYomi,
  RkwBgnBun,
  RkwEndBun,
  RkwGoTo,
  RkwLeft,
  RkwRight,
  RkwNext,
  RkwPrev,
  RkwNfer,
  RkwXfer,
  RkwResize,
  RkwEnlarge,
  RkwShorten,
  RkwStoreYomi,
  RkwSetAppName,
};
#endif /* ENGINE_SWITCH */

#ifdef TEST
main()
{
  wchar_t foo[1024], bar[1024], *bap = bar;
  int i, j, n, len, con;

  con = RkwInitialize("/");

  while (getws(foo)) {
    n = RkwBgnBun(con, foo, WSlen(foo), 0);
    n = RkwResize(con, 1);
    printf("%d ʸ��\n", n);
    for (i = 0 ; i < 5 ; i++) {
      n = RkwGetKanjiList(con, bap, 1024);
      for (j = 0 ; j < n ; j++) {
	len = RkwGetKanji(con, foo, 1024);
	foo[len] = (wchar_t)0;
	printf("%ws(%ws)/", foo, bap);
	bap += WSlen(bap) + 1;
	RkwNext(con);
      }
      RkwRight(con);
      printf("\n");
    }
    (void)RkwEndBun(con);
  }
  RkwFinalize();
}
#endif
