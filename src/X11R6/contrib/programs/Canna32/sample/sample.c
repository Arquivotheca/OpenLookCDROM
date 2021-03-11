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

char *uiKanjiError; /* �༡�ΥǥХ��� */

static char rcs_id[] = "@(#)$Id: sample.c,v 2.11 1994/03/10 07:57:55 kon Exp $";

#include <stddef.h>

#include <stdio.h>

#ifdef BIGPOINTER
#define POINTERINT long long
#else
#define POINTERINT long
#endif

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#if defined(SVR4) || defined(sun) || defined(sony)
#define HAVE_LOCALE
#endif

#ifdef HAVE_LOCALE
#include <locale.h>
#if !defined(SYSV386) && !defined(sony) && !defined(sun)
#include <wctype.h>
#endif
#endif

/* libw ����äƤ���ޥ���䤽���Ǥʤ��ޥ��󤬤��ä����ݤʤΤ�
   ��ʬ�ǻ��äƤ��ޤ� */

/*#ifndef HAVE_WCHAR_OPERATION*/
#define mbstowcs MBstowcs
/*#endif*/
static wchar_t *Wsncpy(), *Wscpy();
static int Wslen();

#include <string.h>
#include <sys/types.h>
#include <sys/times.h>
#include <canna/jrkanji.h>

#define ECHO
#define STRBUFSIZE    512	/* �Х��� */
  
static char sccs_id[] = "@(#)echowin.c	2.5	88/09/29 10:45:43";

#define ctl(x) ((x) & 0x1f) /* ����ȥ��륭����ɽ���ޥ��� ctl('a') 
			       �Τ褦�˻Ȥ� */

static char *program = "sample";

typedef struct {
  int length;
  wchar_t str[STRBUFSIZE];
  int revPos, revLen, width, prevWidth;
} strRec;

typedef struct {
  Window win;
  strRec kakutei, mikakutei, mode, gline;
  int    cursor;
} winRec;

#define DRAW 1
#define NODRAW 0

winRec wb[2];

#ifdef DEBUG
static iroha_debug = 0;
#endif /* DEBUG */

main(argc, argv, envp)
int argc;
char *argv[], *envp[];
{
  int i;
  int beep();
  extern (*jrBeepFunc)();

  jrBeepFunc = beep;

#ifdef HAVE_LOCALE
  setlocale(LC_ALL,"");
#endif

  checkOptions(argc, argv);
  initialize (argc, argv, envp);
  mainloop ();
  exit_program ();
}

/* initialize -- ���������
 *
 *
 * ����������Ǥ��뤳��
 *
 *  �������ФΥ����ץ�
 *  ���ե���ȤΥ���
 *  ���ե���Ⱦ�������� (XQueryFont, ascent, descent, width)
 *  ���ƥ����ȥ���������������
 *  ��������ɥ�������
 *  ������ե��å�����ƥ����Ȥ�����
 *  ��������ɥ��Υޥå�
 *
 */

initialize(argc, argv, envp)
int argc;
char *argv[], *envp[];
{
  initializeX(argc, argv, envp);
  initializeCanna(argc, argv, envp);
}

/* �����Х��ѿ������ */

int              i, nbytes;	       /* ������� */
/* ��������κ����褹�뤿��Υ���������֤ʤɤξ������¸���Ƥ����ѿ� */
int              charwi;	/* ʸ������ */
XEvent           event;		/* ���٥�� */
Display          *dpy;
int              ww = 0;


/* mainloop -- �ᥤ��롼��
 *
 * �ᥤ��롼����
 *
 * ���٥�Ȥ򽦤äƥǥ����ѥå����롣
 */

mainloop()
{
  do {
    XNextEvent (dpy, &event);
    erase_textcursor();
    switch (event.type)
      {
      case Expose:
	proc_expose();
	break;
      case KeyPress:
	proc_keypress(&event);
	break;
      case ButtonPress:
	proc_buttonpress(&event);
	break;
      default: /* �ʤ�����褯�狼��ʤ����٥�Ȥ��褿��� */
	break;
      }
    if (wb[ww].cursor == DRAW) {
      draw_textcursor();
    }
  } while (1); /* (event.type != ButtonPress); */
}

#define InnerBorder   10
#define OuterBorder   5
#define BaseFontName  "-*-fixed-*-*-*--24-*-*-*-*-*"

static int  WinWidth = 80; /* [ʸ��] */

Window      win, parwin;
XFontSet    fs;
GC          gc, rgc, mgc;
XFontStruct **fonts;
int         asc, des, cw;	/* ʸ���Υ������/�ǥ������/�� */
int         CursorWidth;
static int glinePosition, abandon_kakutei = 0;

initializeX(argc, argv, envp)
int argc;
char *argv[], *envp[];
{
  XGCValues   gcv;

  /* ���ݥФȤ���³�򤹤� */
  dpy = XOpenDisplay (0);

  if (dpy == NULL) {
    fprintf(stderr, "�إ����Ф���³�Ǥ��ޤ���Ǥ���\n");
    exit(1);
  }

  /* �ե���Ȥ��ݥɤ��� */
  {
    char **misstr, *defstr;
    int mc, n;
    XFontSetExtents *xfse;

    fs = XCreateFontSet(dpy, BaseFontName, &misstr, &mc, &defstr);
    if (fs) {
      xfse = XExtentsOfFontSet(fs);
      n = XFontsOfFontSet(fs, &fonts, &misstr);
      asc = des = 0;
      for (i = 0 ; i < n ; i++) {
	if (asc < fonts[i]->ascent) {
	  asc = fonts[i]->ascent;
	}
	if (des < fonts[i]->descent) {
	  des = fonts[i]->descent;
	}
      }
      if (n > 1) {
	/* �ե���Ȥβ�����Ȥ롣 */
	cw = fonts[0] ->max_bounds.width * 2 > fonts[1]->max_bounds.width
	  ? fonts[0]->max_bounds.width * 2 : fonts[1]->max_bounds.width;
      
	/* �ƥ����ȥ��������������롣 */
	CursorWidth = XTextWidth(fonts[0], "a", 1);
      }
    }
    else {
      fprintf(stderr, "can not create font set.\n");
      exit(1);
    }
  }


  /* ������ɥ����������� */
  parwin = XCreateSimpleWindow
    (dpy, DefaultRootWindow(dpy), 0, 0,
     cw * WinWidth / 2 + InnerBorder * 2 + OuterBorder * 2 + 2/*border*/,
#ifndef TEISEI
     ((asc + des + InnerBorder ) * 2/*��*/ + InnerBorder 
#else /* TEISEI */
     ((asc + des + InnerBorder ) * (2 + teiseiline + kanaline)/*��*/
      + InnerBorder 
#endif /* TEISEI */
      + OuterBorder + 2/*border*/) * 2 + OuterBorder,
     1,
     BlackPixel(dpy, DefaultScreen(dpy)),
     WhitePixel(dpy, DefaultScreen(dpy)));

  XStoreName(dpy, parwin, "canna sample");
  win = parwin;

  /* �ǣä򿷤����������� */
  gcv.foreground = BlackPixel(dpy, DefaultScreen(dpy));
  gcv.background = WhitePixel(dpy, DefaultScreen(dpy));
  
  gc = XCreateGC (dpy, win, GCForeground|GCBackground, &gcv);
  
  {
    XColor colordef;

    colordef.red   = 0xdf00;
    colordef.green = 0xef00;
    colordef.blue  = 0xff00;
    colordef.flags = DoRed | DoGreen | DoBlue;
    XAllocColor(dpy, DefaultColormap(dpy, DefaultScreen(dpy)), &colordef);

    gcv.background = colordef.pixel;
    gcv.foreground = BlackPixel(dpy, DefaultScreen(dpy));
    mgc = XCreateGC (dpy, win, GCForeground|GCBackground, &gcv);
  }

  gcv.foreground = gcv.background;
  gcv.background = BlackPixel(dpy, DefaultScreen(dpy));

  rgc = XCreateGC (dpy, win, GCForeground|GCBackground, &gcv);
  
  win = XCreateSimpleWindow
    (dpy, parwin, OuterBorder, OuterBorder, 
     cw * WinWidth / 2 + InnerBorder * 2,
#ifndef TEISEI
     (asc + des + InnerBorder) * 2 /* �� */ + InnerBorder,
#else /* TEISEI */
     (asc + des + InnerBorder) * (2 + teiseiline + kanaline) /* �� */
     + InnerBorder,
#endif /* TEISEI */
     1, 
     BlackPixel(dpy, DefaultScreen(dpy)),
     WhitePixel(dpy, DefaultScreen(dpy)));
  XSelectInput (dpy, win, ExposureMask | ButtonPressMask | KeyPressMask);
  wb[0].win = win;
  
  win = XCreateSimpleWindow
    (dpy, parwin, OuterBorder,
#ifndef TEISEI
     (asc + des + InnerBorder) * 2 /* �� */ 
#else /* TEISEI */
     (asc + des + InnerBorder) * (2 + teiseiline + kanaline) /* �� */ 
#endif /* TEISEI */
     + InnerBorder + OuterBorder * 2 + 2,
     cw * WinWidth / 2 + InnerBorder * 2,
#ifndef TEISEI
     (asc + des + InnerBorder) * 2 /* �� */ + InnerBorder,
#else /* TEISEI */
     (asc + des + InnerBorder) * (2 + teiseiline + kanaline) /* �� */
     + InnerBorder,
#endif /* TEISEI */
     1, 
     BlackPixel(dpy, DefaultScreen(dpy)),
     WhitePixel(dpy, DefaultScreen(dpy)));
  XSelectInput (dpy, win, ExposureMask | ButtonPressMask | KeyPressMask);
  wb[1].win = win;

  /* ������ɥ���ޥåפ��� */
  XMapSubwindows (dpy, parwin);
  XMapWindow (dpy, parwin);
  return 0;
}

int double_final = 0;

/* exit_program -- ��λ����
 *
 * ���Υץ�����λ�������ɬ�����δؿ���Ƥܤ�
 */

exit_program()
{
  int res;
  char **warn;

  if (wcKanjiControl ( 0, KC_FINALIZE, (char *)&warn ) < 0) {
    extern char * jrKanjiError;

    fprintf(stderr, "KC_FILENALIZE�ǥ��顼��ȯ��������\n");
    fprintf(stderr, "error: %s\n", jrKanjiError);
  }
  if (double_final) {
    int res;
    res = wcKanjiControl(0, KC_FINALIZE, (char *)&warn);
    fprintf(stderr, "kc_finalize = %d\n", res);
    if (res < 0) {
      extern char *jrKanjiError;
      fprintf(stderr, "%s\n", jrKanjiError);
      return;
    }
  }
  if (warn) {
    char **p;

    for (p = warn; *p ; p++) {
      fprintf(stderr, "%s\n", *p);
    }
  }
  XFreeGC(dpy, gc);
  XFreeGC(dpy, rgc);
  XFreeGC(dpy, mgc);
  XFreeFontSet (dpy, fs);
/*  XUnloadFont(dpy, fn); */
/*  XUnloadFont(dpy, fh); */
/*  XUnloadFont(dpy, fk); */
  XCloseDisplay (dpy);
  exit (0);
}

/* �ƥ����ȥ������������õ�Υ롼����
 *
 * ���������ɤ����������Ȥ���������ѿ� cursorcol �������롣
 */

draw_textcursor()
{
  /* �ƥ����ȥ�����������褹�롣 */
  
  XFillRectangle(dpy, win, gc,
		 InnerBorder + wb[ww].kakutei.width + wb[ww].mikakutei.width,
		 InnerBorder, CursorWidth, asc + des);
}

erase_textcursor()
{
  XClearArea(dpy, win, 
	     InnerBorder + wb[ww].kakutei.width + wb[ww].mikakutei.width,
	     InnerBorder, CursorWidth, asc + des, False);
}

/* proc_expose -- �������ݡ�������
 *
 * col, rcol, rlen �ʤɤξ����Ȥä�ʸ��������褹�롣
 */

proc_expose()
{
  int pos;

  redraw_kakutei();
  redraw_mikakutei();
  draw_both_mode();
  draw_gline();
#ifdef TEISEI
  if (teiseiline) {
    draw_teisei();
  }
  if (kanaline) {
    draw_kana();
  }
#endif /* TEISEI */
  XFlush(dpy);
}

static kanjiContext = 0;
int    msystime = 0, kstime = 0;
int    yomiLevel = 0;
int    given_nbytes, nbytes_given = 0;
extern tpo, euc;

/* proc_keypress -- �������Ϥλ��ν���
 *
 */

static int checkIfFunctionalChar();

proc_keypress(event)
XEvent *event;
{
  char		   lookupbuf[STRBUFSIZE];
  wchar_t          wcbuf[STRBUFSIZE];
  wchar_t          wecho[STRBUFSIZE], wgline[STRBUFSIZE], wmode[STRBUFSIZE];
  KeySym           keysym;
  XComposeStatus   compose_status;
  wcKanjiStatus     kanji_status;
  jrKanjiStatus     euc_kanji_status;
  extern char *jrKanjiError;
  int functionalChar;

  nbytes = XLookupString ((XKeyEvent *)event, lookupbuf, STRBUFSIZE,
			  &keysym, &compose_status);

  if (nbytes == 0 && XK_overline <= keysym && keysym <= XK_semivoicedsound) {
    lookupbuf[0] = (unsigned long)keysym & 0xff;
    nbytes = 1;
  }

  /* Ĺ��������Τ�Τ˴ؤ��ƤϤ���äȵߺ� */
  functionalChar = checkIfFunctionalChar(event, keysym, lookupbuf, STRBUFSIZE);
  /* shift+���Τ褦��ʸ�����ä��飱ʸ���ȿ����� */
  if ( !nbytes && functionalChar ) {
    nbytes = 1;
  }

  /* �פ�ʤ�����̵�뤹�� */
  if (nbytes == 0) return 0;

  if (event->xkey.window == wb[0].win)
    kanjiContext = 0;
  else
    kanjiContext = 1;

  if (optionControl(event, lookupbuf[0],
		    wcbuf, &kanji_status)) {
    int i;
    struct tms tmbuf;

#ifdef TPO
    if (tpo) {
      keyresult((int)(unsigned char)lookupbuf[0]);
    }
#endif

    if (msystime) {
      kstime -= times(&tmbuf);
    }
    if (euc) {
      char xxxx[1024];

      nbytes = jrKanjiString (kanjiContext, lookupbuf[0],
			      lookupbuf,
			      nbytes_given ? given_nbytes : STRBUFSIZE,
			      &euc_kanji_status);

      if (euc_kanji_status.length >= 0) {
	strncpy(xxxx, (char *)euc_kanji_status.echoStr,
		euc_kanji_status.revPos);
	xxxx[euc_kanji_status.revPos] = (unsigned char)0;
	kanji_status.revPos = mbstowcs(wecho, xxxx, STRBUFSIZE);

	strncpy(xxxx, (char *)euc_kanji_status.echoStr
		+ euc_kanji_status.revPos, euc_kanji_status.revLen);
	xxxx[euc_kanji_status.revLen] = (unsigned char)0;
	kanji_status.revLen = mbstowcs(wecho + kanji_status.revPos,
				       xxxx, STRBUFSIZE);
	strncpy((char *)xxxx,
		(char *)euc_kanji_status.echoStr + euc_kanji_status.revPos +
		euc_kanji_status.revLen,
		euc_kanji_status.length - euc_kanji_status.revPos -
		euc_kanji_status.revLen);
	xxxx[euc_kanji_status.length - euc_kanji_status.revPos - 
	     euc_kanji_status.revLen] = (unsigned char)0;
	kanji_status.length = kanji_status.revPos + kanji_status.revLen +
	  mbstowcs(wecho + kanji_status.revPos + kanji_status.revLen,
		   xxxx, STRBUFSIZE);
      }

      if (euc_kanji_status.info & KanjiGLineInfo) {
	strncpy((char *)xxxx, (char *)euc_kanji_status.gline.line,
		euc_kanji_status.gline.revPos);
	xxxx[euc_kanji_status.gline.revPos] = (unsigned char)0;
	kanji_status.gline.revPos = mbstowcs(wgline, xxxx, STRBUFSIZE);

	strncpy((char *)xxxx, (char *)euc_kanji_status.gline.line +
		euc_kanji_status.gline.revPos,
		euc_kanji_status.gline.revLen);
	xxxx[euc_kanji_status.gline.revLen] = (unsigned char)0;
	kanji_status.gline.revLen = mbstowcs(wgline +
					     kanji_status.gline.revPos,
					     xxxx,
					     STRBUFSIZE);
	strncpy((char *)xxxx, (char *)euc_kanji_status.gline.line +
		euc_kanji_status.gline.revPos +
		euc_kanji_status.gline.revLen,
		euc_kanji_status.gline.length -
		euc_kanji_status.gline.revPos -
		euc_kanji_status.gline.revLen);
	xxxx[euc_kanji_status.gline.length - euc_kanji_status.gline.revPos - 
	     euc_kanji_status.gline.revLen] = (unsigned char)0;
	kanji_status.gline.length = kanji_status.gline.revPos +
	  kanji_status.gline.revLen +
	  mbstowcs(wgline + kanji_status.gline.revPos +
		   kanji_status.gline.revLen,
		   xxxx, STRBUFSIZE);
      }

      if (euc_kanji_status.info & KanjiModeInfo) {
	mbstowcs(wmode, euc_kanji_status.mode, 1024);
      }

      lookupbuf[nbytes] = (wchar_t)0;
      nbytes = mbstowcs(wcbuf, lookupbuf, STRBUFSIZE);
      if (euc_kanji_status.info & KanjiYomiInfo) {
	wchar_t *foo = wcbuf + nbytes + 1;
	char *bar = lookupbuf;
	int len;

	while (*bar) bar++;
	bar++;
	len = mbstowcs(foo, bar, wcbuf + STRBUFSIZE - foo);
	while (*bar) bar++;
	bar++;
	foo += len + 1;
	len = mbstowcs(foo, bar, wcbuf + STRBUFSIZE - foo);
      }

      kanji_status.echoStr = wecho;
      kanji_status.mode = wmode;
      kanji_status.gline.line = wgline;
      kanji_status.info = euc_kanji_status.info;
    }
    else {
      nbytes = wcKanjiString (kanjiContext, lookupbuf[0],
			      wcbuf, 
			      nbytes_given ? given_nbytes : STRBUFSIZE,
			      &kanji_status);
    }
    if (msystime) {
      kstime += times(&tmbuf);
    }
#ifdef TPO
    if (tpo) {
      jrKanjiStatus ks;
      unsigned char echo[1024], gline[1024], mode[256], ebuf[1024];
      wchar_t buf[1024];

      if (euc) {
	kanjiresult(lookupbuf, nbytes, &euc_kanji_status, 0);
      }
      else {
	if (kanji_status.length >= 0) {
	  Wsncpy(buf, kanji_status.echoStr, kanji_status.length);
	  buf[kanji_status.length] = (wchar_t)0;
	  wcstombs(echo, buf, 1024);
	}

	if (kanji_status.info & KanjiGLineInfo) {
	  Wsncpy(buf, kanji_status.gline.line, kanji_status.gline.length);
	  buf[kanji_status.gline.length] = (wchar_t)0;
	  wcstombs(gline, buf, 1024);
	}

	if (kanji_status.info & KanjiModeInfo) {
	  wcstombs(mode, kanji_status.mode, 1024);
	}

	wcbuf[nbytes] = (wchar_t)0;
	wcstombs(ebuf, wcbuf, 1024);

	ks.echoStr = echo;
	ks.mode = mode;
	ks.gline.line = gline;
	ks.length = kanji_status.length;
	ks.revPos = kanji_status.revPos;
	ks.revLen = kanji_status.revLen;
	ks.info = kanji_status.info;
	ks.gline.length = kanji_status.gline.length;
	ks.gline.revPos = kanji_status.gline.revPos;
	ks.gline.revLen = kanji_status.gline.revLen;
	kanjiresult(ebuf, nbytes, &ks, 1);
      }
    }
#endif
  }
  if (abandon_kakutei) {
    nbytes = 0;
  }

  /* �ɤߤ�ɽ�����Ƥߤ롣 */
  if (nbytes) {
    if (yomiLevel && (kanji_status.info & KanjiYomiInfo)) {
      if (nbytes < STRBUFSIZE) {
	printf("�ɤ� \"%ws\"", wcbuf + nbytes + 1);
	if (yomiLevel > 1) {
	  int len = 0;
	  wchar_t *p = wcbuf + nbytes + 1;
	  while (*p) p++;
	  len = p - wcbuf - nbytes - 1;
	  if (nbytes + 1 + len < STRBUFSIZE) {
	    printf("������ \"%ws\"", p + 1);
	  }
	}
	printf(" ���б�������ʬ�����ꤷ�ޤ�����\n");
      }
    }
  }

  {
    int i;
    
    win = event->xkey.window;
    for (i = 0 ; i < 2 ; i++) {
      if (wb[i].win == win) {
	ww = i;
      }
    }
    if (kanji_status.length >= 0) {
      wb[ww].cursor = (kanji_status.length != 0 && kanji_status.revLen) ? 
	NODRAW : DRAW;
    }
  }

  if (nbytes < 0) {
    fprintf(stderr, "sample: ���顼: %s\n", jrKanjiError);
  }

#ifdef DEBUG
  if (iroha_debug) {
    print_debug(nbytes, kanjiContext, wcbuf,
		lookupbuf[0], &kanji_status);
  }
#endif

  if (nbytes > 0) { /* wcKanjiString �η�̤�������礭���ȸ������Ȥ�
		       ����ʸ�����֤äƤ���ȸ������ȤʤΤǡ����γ���ʸ
		       ��������褹�롣 */
    draw_determined(nbytes, wcbuf, &kanji_status);
    if (kanji_status.length >= 0) {
      /* ̤���������괺�����ȤäƤ��� */
      wb[ww].mikakutei.length = kanji_status.length;
      Wsncpy(wb[ww].mikakutei.str, kanji_status.echoStr, 
	     wb[ww].mikakutei.length);
      wb[ww].mikakutei.revPos = kanji_status.revPos;
      wb[ww].mikakutei.revLen = kanji_status.revLen;
      wb[ww].mikakutei.prevWidth = wb[ww].mikakutei.width;
      wb[ww].mikakutei.width = 
	XwcTextEscapement(fs, wb[ww].mikakutei.str, wb[ww].mikakutei.length);
    }

    if (kanji_status.length > 0) {
      redraw_mikakutei();
    } 
  }
  else if (kanji_status.length >= 0) { /* kanji_status.length ��������礭 
					  ���ȸ������Ȥ�̤����ʸ�����֤� 
					  ��Ƥ���ȸ������ȤʤΤǡ������
					  ���褹�롣 */
    draw_undetermined(nbytes, wcbuf, &kanji_status);
  }

  if (kanji_status.info & KanjiModeInfo) {
    /* �嵭��Ƚ��ʸ�����λ��ϥ��ʴ����Ѵ��⡼�ɤ˴ؤ�������֤����
       ����ȸ������ȤʤΤǡ���������褹�롣 */
    wb[ww].mode.prevWidth = wb[ww].mode.width;
    Wscpy(wb[ww].mode.str, kanji_status.mode);
    wb[ww].mode.str[Wslen(kanji_status.mode)] = (wchar_t)0;
    wb[ww].mode.length = Wslen(wb[ww].mode.str);
    wb[ww].mode.width = XwcTextEscapement(fs, wb[ww].mode.str, 
					  wb[ww].mode.length);
    XClearArea(dpy, win, InnerBorder, 
	       2 * InnerBorder + asc + des,
	       glinePosition,
	       asc + des, False);
    draw_mode();
  }

  if (kanji_status.info & KanjiGLineInfo) {
    /* �嵭��Ƚ��ʸ�����λ��ϸ�������˴ؤ�������֤���Ƥ���ȸ���
       ���ȤʤΤǡ���������褹�롣 */
    checkGlineWidth(&kanji_status);
    wb[ww].gline.prevWidth = wb[ww].gline.width;
    wb[ww].gline.length = kanji_status.gline.length;
    Wsncpy(wb[ww].gline.str, 
	   kanji_status.gline.line, kanji_status.gline.length);
    wb[ww].gline.revPos = kanji_status.gline.revPos;
    wb[ww].gline.revLen = kanji_status.gline.revLen;
    wb[ww].gline.width = XwcTextEscapement(fs, wb[ww].gline.str,
					   wb[ww].gline.length);
    XClearArea(dpy, win, InnerBorder + glinePosition,
	       2 * InnerBorder + asc + des,
	       wb[ww].gline.prevWidth,
	       asc + des, False);
    draw_gline();
  }
#ifndef KanjiTeiseiInfo
#define KanjiTeiseiWasUndefined
#define KanjiTeiseiInfo 128
#endif /* KanjiTeiseiInfo */

#ifdef TEISEI
  if ((kanji_status.info & KanjiTeiseiInfo) && teiseiline) {
    wb[ww].teisei.prevWidth = wb[ww].teisei.width;
    wb[ww].teisei.length = kanji_status.teisei.length;
    strncpy(wb[ww].teisei.str, 
	    kanji_status.teisei.teiseiStr, kanji_status.teisei.length);
    wb[ww].teisei.revPos = kanji_status.teisei.revPos;
    wb[ww].teisei.revLen = kanji_status.teisei.revLen;
    wb[ww].teisei.width = XwcTextEscapement(fs, wb[ww].teisei.str, 
					    wb[ww].teisei.length);
    XClearArea(dpy, win, InnerBorder,
	       InnerBorder + 2 * (InnerBorder + asc + des),
	       wb[ww].teisei.prevWidth,
	       asc + des, False);
    draw_teisei();
  }
#endif /* TEISEI */
}

#ifdef DEBUG
static printchar(c)
int c;
{
  if (c < ' ') {
    printf("^%c", c + '@');
  }
  else if (c == 0x007f) {
    printf("^?");
  }
  else {
    printf("%wc", c);
  }
}

static printtext(s)
char *s;
{
  unsigned char *p;

  for (p = (unsigned char *)s ; *p ; p++) {
    printchar((int)*p);
  }
}

print_debug(n, cx, lub, key, ks)
     int n;
     int cx;
     wchar_t *lub;
     int key;
     wcKanjiStatus *ks;
{
  wchar_t buf[1024];
  static int count = 0;

  count++;
  printf("\n(sample %d)\n", count);
  printf("len=%d, ", n);
  printf("key=0x%x, ", key);
  printf("&kanjiStat=0x%x, ", ks);
  printf("echo_len=%d, ", ks->length);
  printf("revPos=%d, ", ks->revPos);
  printf("revLen=%d, ", ks->revLen);
  if (ks->length > 0) {
    Wsncpy(buf, ks->echoStr, ks->length);
    buf[ks->length] = '\0';
    printf("ks.echoStr=(%s)\n", buf);
  }
  { /* info ��ɽ�� */
    printf("\n����ե�: ");
    if (ks->info & KanjiModeInfo)
      printf("�⡼�� ");
    if (ks->info & KanjiGLineInfo)
      printf("���� ");
    if (ks->info & KanjiYomiInfo)
      printf("�ɤ� ");
    if (ks->info & KanjiThroughInfo)
      printf("���롼 ");
    if (ks->info & KanjiEmptyInfo)
      printf("�� ");
    printf("(info=0x%x)\n", ks->info);
  }
  printf("glinelen=%d, ", ks->gline.length);
  if ((ks->info & KanjiGLineInfo) && ks->gline.length > 0) {
    Wsncpy(buf, ks->gline.line, ks->gline.length);
    buf[ks->gline.length] = '\0';
    printf("\nks.gline.line=("); printtext(buf); printf(")\n");
    printf("gline.revPos = %d gline.revLen = %d\n",
	   ks->gline.revPos, ks->gline.revLen);
  }
  if (ks->info & KanjiModeInfo) {
    printf("mode("); printtext(ks->mode); printf(")\n");
  }
  printf("buffer[0]=0x%x, ", lub[0]);
  lub[nbytes] = '\0';
  printf("buffer=\""); printtext(lub); printf("\"\n");
}
#endif



/* draw_determined -- ����ʸ���������
 *
 * ����ʸ���������Ǥϰʲ��Τ��Ȥ�Ԥ�
 *
 * ������ȥ���ʸ�����ä��餽�ν����롼����عԤ���
 * �����������ꤷ��ʸ����������Хåե��˥��ڥ�ɤ��롣
 * �����������ꤷ����ʬ�����褹�롣
 * �����������ꤷ��ʸ���������Ĵ�٤ơ�����������������֤�׻����롣
 * ��buf �Υ���ǥå����Ǥ��� col �򹹿����롣
 * �����������ꤷ��ʸ�����������������̤����ʸ����Ȥ������褵���
 *   ����ʸ����������⾮�����ΤǤ���С�XwcDrawImageString ��
 *   ���ǤϾä��ڤ�ʤ��Τǡ�XClearArea ���Ѥ��ƤϤߤ�������ʬ��õ�
 *   ���롣
 */

draw_determined(nbytes, wcbuf, kanji_status)
     int nbytes;
     wchar_t *wcbuf;
     wcKanjiStatus *kanji_status;
{
  int str_width;

  if (nbytes == 1 && ((unsigned int)wcbuf[0] < 0x20 ||
		      (unsigned int)wcbuf[0] == 0x7f) ) {
    wb[ww].kakutei.prevWidth = wb[ww].kakutei.width;
    if (wb[ww].mikakutei.width > 0) {
      XClearArea(dpy, win, InnerBorder + wb[ww].kakutei.width, InnerBorder,
		 wb[ww].mikakutei.width, asc + des, False);
    }
    /* ̤����ʸ�������ʬ��õ���Τǡ� mikakutei.width �򥯥ꥢ����
       ���� */
    wb[ww].mikakutei.width = 0;

    /* ����ȥ��륳���ɤ����ϡʤ��ޤ��ɤ������ǥ��󥰤ǤϤʤ��� */
    wcbuf[2] = (wchar_t)'^';
    wcbuf[3] = (wchar_t)(wcbuf[0] + '@');
    wcbuf[4] = (wchar_t)0;
    if (!proc_ctl_keypress(wcbuf)) {
      draw_determined(2, wcbuf + 2, kanji_status);
    }
  }
  else { /* ����ȥ��륳���ɤǤʤ���� */
    /* length �Ȥ����Τ� str �˳�Ǽ����Ƥ���ʸ������������ޤ��뤿��Υ�
       ��ǥå����Ǥ��롣���������ꤷ��ʸ����� length �ǥ���ǥå�������
       ����ʬ������˳�Ǽ���롣 */
    Wsncpy(wb[ww].kakutei.str + wb[ww].kakutei.length, wcbuf, nbytes); 
    /* �Ȥꤢ�������Ǹ�� NULL ������Ƥ����������פ��⤷��ʤ� */
    wb[ww].kakutei.str[wb[ww].kakutei.length + nbytes] = '\0';

    /* ���������ꤷ����ʬ�Τߤ����褹�롣*/
    XwcDrawImageString(dpy, win, fs, gc,
		       InnerBorder + wb[ww].kakutei.width,
		       InnerBorder + asc,
		       wb[ww].kakutei.str + wb[ww].kakutei.length,
		       nbytes);
    
    /* ���������ꤷ��ʸ���������Ĵ�٤ơ�����������������֤�׻����� */
    str_width = XwcTextEscapement(fs, 
				  wb[ww].kakutei.str + wb[ww].kakutei.length, 
				  nbytes);
    wb[ww].kakutei.prevWidth = wb[ww].kakutei.width;
    wb[ww].kakutei.width += str_width;

    /* str �Υ���ǥå����Ǥ��� length �򹹿����롣 */
    wb[ww].kakutei.length += nbytes;
    
    /* ���������ꤷ��ʸ�����������������̤����ʸ����Ȥ������褵���
       ����ʸ����������⾮�����ΤǤ���С�XwcDrawImageString ��
       ���ǤϾä��ڤ�ʤ��Τǡ�XClearArea ���Ѥ��ƤϤߤ�������ʬ��õ�
       ���ʤ��ƤϤʤ�ʤ��� */
    if (str_width < wb[ww].mikakutei.width) {
      XClearArea(dpy, win, InnerBorder + wb[ww].kakutei.width, InnerBorder,
		 wb[ww].mikakutei.width - str_width, asc + des, False);
    }
    /* ̤����ʸ�������ʬ��õ���Τǡ� mikakutei.width �򥯥ꥢ����
       ���� */
    wb[ww].mikakutei.width = 0;
  }
}

/* draw_undetermined -- ̤����ʸ���������
 *
 */

draw_undetermined(nbytes, wcbuf, kanji_status)
     int nbytes;
     wchar_t *wcbuf;
     wcKanjiStatus *kanji_status;
{
  int rev_width;
  int pos;
  
  pos = wb[ww].kakutei.width;
  
  if (kanji_status->length < 0) /* kanji_status->length ���ޥ��ʥ��λ�
				   ������̤����ʸ������Ѳ����ʤ��ȸ�
				   �����ȤʤΤǲ��⤷�ʤ���*/
    return;
  /* ̤����ʸ����¸�ߤ���ΤǤ���С����ν����򤹤롣�Ȥꤢ������
    �ޤ��ϥХåե��˥��ԡ����Ƥ��� */
  wb[ww].mikakutei.length = kanji_status->length;
  Wsncpy(wb[ww].mikakutei.str, kanji_status->echoStr, 
	 wb[ww].mikakutei.length);
  wb[ww].mikakutei.revPos = kanji_status->revPos;
  wb[ww].mikakutei.revLen = kanji_status->revLen;
  wb[ww].mikakutei.prevWidth = wb[ww].mikakutei.width;
  wb[ww].mikakutei.width = XwcTextEscapement(fs, 
					     wb[ww].mikakutei.str, 
					     wb[ww].mikakutei.length);
  redraw_mikakutei();
  
  /* �������褷�Ƥ��ä�ʸ�����Ĺ���������������褷��ʸ�������Ĺ����
     �����ˤϡ��������褷�Ƥ�����ʬ�ǡ��Ϥ߽Ф��Ƥ�����ʬ�򥯥ꥢ��
     �롣 */
  if (wb[ww].mikakutei.prevWidth > wb[ww].mikakutei.width) {
    XClearArea(dpy, win, 
	       InnerBorder + wb[ww].kakutei.width + wb[ww].mikakutei.width, 
	       InnerBorder, 
	       wb[ww].mikakutei.prevWidth - wb[ww].mikakutei.width,
	       asc + des, False);
  }
}

/* draw_gline -- �������ɽ������ʬ�����褹�롣
 *
 */

int	    max_mode_strlen = 0;
char *malloc();

draw_gline()
{
  static int width, firsttime = 1;

  if (firsttime) {
    char *str;

    if (str = malloc(max_mode_strlen + 1)) {
      for (i = 0 ; i < max_mode_strlen ; i++) {
	str[i] = 'x';
      }
    }
    firsttime = 0;
    glinePosition = XTextWidth(fonts[0], str, max_mode_strlen);
    if (str) {
      free(str);
    }
  }
  
  redraw_general(dpy, win, gc, rgc, fonts,
		 InnerBorder + glinePosition,
		 2 * InnerBorder + 2 * asc + des,
		 &wb[ww].gline);
}

#ifdef TEISEI

/* draw_teisei -- ����ʸ������
 *
 */

draw_teisei()
{
  redraw_general(dpy, win, gc, rgc, fonts,
		 InnerBorder,
		 3 * InnerBorder + 3 * asc + 2 * des,
		 &wb[ww].teisei);
}

draw_kana()
{
  redraw_general(dpy, win, gc, rgc, fonts,
		 InnerBorder,
		 3 * InnerBorder + 3 * asc + 2 * des,
		 &wb[ww].kana);
}

#endif /* TEISEI */

/* proc_ctl_keypress -- ����ȥ��륭���ν����롼����
 *
 */

int
proc_ctl_keypress(wcbuf)
     wchar_t *wcbuf;
{
  switch (wcbuf[0])
    {
    case ctl('M'):
      /* �꥿���󥭡� */
#if defined(ECHO) && defined(HAVE_WCHAR_OPERATION)
      if (!tpo) {
	wb[ww].kakutei.str[wb[ww].kakutei.length] = '\0';
	printf("%ws\n", wb[ww].kakutei.str);
      }
#endif
      if (msystime) {
	printf("�����ޤǤ� wcKanjiString �λ��� %d tics\n", kstime);
      }
      /* ����प��ӥ���������ᤷ��������ɥ��򥯥ꥢ���롣 */
      wb[ww].kakutei.length = 0;
      wb[ww].kakutei.width = 0;
      XClearWindow(dpy, win);
      draw_mode();
      XFlush(dpy);
      return 1;
      break;
    case ctl('H'):
      /* �Хå����ڡ������� */
      if (wb[ww].kakutei.length) {
	if (wb[ww].kakutei.str[wb[ww].kakutei.length - 1] & 0x80) {
	  wb[ww].kakutei.length -= 1;
	  charwi =
	    XwcTextEscapement(fs, 
			      wb[ww].kakutei.str + wb[ww].kakutei.length, 1);
	}
	else {
	  wb[ww].kakutei.length--;
	  charwi =
	    XwcTextEscapement(fs, 
			      wb[ww].kakutei.str + wb[ww].kakutei.length, 1);
	}
	wb[ww].kakutei.width -= charwi;
	XClearArea(dpy, win, InnerBorder + wb[ww].kakutei.width, 
		   InnerBorder, charwi,
		   asc + des, False);
	XFlush(dpy);
      }
      return 1;
      break;
    case 0x7f:
      /* DEL ���Ǥ��줿��� */
      exit_program();
      /* NOTREACHED */
      break;
    case ctl('C'):
      /* ���ʴ����Ѵ���Ƶ�ư���� */
      reset_iroha();
      /* �⡼�ɤ�õ�� */
      strcpy((char *)wb[0].mode.str, "");
      strcpy((char *)wb[1].mode.str, "");
      /* ����प��ӥ���������ᤷ��������ɥ��򥯥ꥢ���롣 */
      wb[0].kakutei.length = 0;
      wb[0].kakutei.width = 0;
      wb[1].kakutei.length = 0;
      wb[1].kakutei.width = 0;
      XClearWindow(dpy, win);
      draw_both_mode();
      XFlush(dpy);
      kstime = 0; /* �����ƥ��¬���֤�ꥻ�å� */
      return 1;
      break;
    default:
      return 0;
      break;
    }
}

reset_iroha()
{
  char **warn;

/*
  printf("����Ϥ���Ȥ��ޤ�...");
  fflush(stdout);
*/
  wcKanjiControl(0, KC_FINALIZE, (char *)&warn);
  if (warn) {
    char **p;

    for (p = warn ; *p ; p++) {
      printf("%s\n", *p);
    }
  }
/*
  printf("����Ϥ�Ƶ�ư���ޤ�...");
  fflush(stdout);
*/
  wcKanjiControl(0, KC_INITIALIZE, (char *)&warn);
  if (warn) {
    char **p;

    for (p = warn ; *p ; p++) {
      printf("%s\n", *p);
    }
  }
/*
  printf("�Ƶ�ư���ޤ���\n");
*/
}

/* draw_mode -- �⡼��ɽ������ʬ�����褹�롣
 *
 */

draw_mode()
{
  int pos;

  /* �⡼��ɽ����ʸ�������褹�� */
  pos = InnerBorder;
  XwcDrawImageString(dpy, wb[ww].win, fs, gc, pos, 
		     2 * InnerBorder + 2 * asc + des,
		     wb[ww].mode.str,
		     Wslen(wb[ww].mode.str));
}

draw_both_mode()
{
  int wwbk = ww;

  for (ww = 0 ; ww < 2 ; ww++) {
    draw_mode();
  }
  ww = wwbk;
}

redraw_kakutei()
{
  XwcDrawImageString(dpy, win, fs, gc, InnerBorder, InnerBorder + asc,
		     wb[ww].kakutei.str, wb[ww].kakutei.length);
}

redraw_mikakutei()
{
  redraw_general(dpy, win, mgc, rgc, fonts,
		 InnerBorder + wb[ww].kakutei.width, InnerBorder + asc,
		 &wb[ww].mikakutei);
}


/* redraw_general -- ����Ū�ʺ�����롼����

  strRec ��¤�ΤؤΥݥ��󥿤��Ϥ��Ƥ��ȡ��̾����衢ȿž���衢�̾������
  ���Ƥ���롣

  */

redraw_general(dpy, win, gc, rgc, fonts, x, y, s)
Display *dpy;
Window win;
GC gc, rgc;
XFontStruct **fonts;
int x, y;
strRec *s;
{
  int pos;

  if (s->length == 0) {
    return;
  }
  pos = x;
  XwcDrawImageString(dpy, win, fs, gc, pos, y, s->str, s->revPos);

  /* ȿžɽ����ʸ�������褹�� */
  pos += XwcTextEscapement(fs, s->str, s->revPos);
  XwcDrawImageString(dpy, win, fs, rgc, pos, y,
		     s->str + s->revPos, s->revLen);
  
  /* �����̾�ɽ����ʸ�������褹�� */
  pos += XwcTextEscapement(fs, s->str + s->revPos, s->revLen);
  XwcDrawImageString(dpy, win, fs, gc, pos, y,
		     s->str + s->revPos + s->revLen,
		     s->length - s->revPos - s->revLen);
}

proc_buttonpress(event)
XButtonEvent *event;
{
  switch (event->button) {
#ifdef DEBUG
  case 3:
    iroha_debug = iroha_debug ? 0 : 1;
    if (iroha_debug) {
      fprintf(stderr, "�ԡԡԥǥХ���å������աա�\n");
      wcKanjiControl(0, KC_DEBUGMODE, 1);
    }
    else {
      fprintf(stderr, "�ԡԡ� ��å�������λ �աա�\n");
      wcKanjiControl(0, KC_DEBUGMODE, 0);
    }
    break;
  case 2:
    {
      int kanjiContext;

      if (event->window == wb[0].win)
	kanjiContext = 0;
      else
	kanjiContext = 1;

      wcKanjiControl(kanjiContext, KC_DEBUGYOMI, 0);
    }
    break;
#endif /* DEBUG */
  default:
#ifdef DEBUG_ALLOC
    {
      extern fail_malloc;
      fail_malloc = !fail_malloc;
      fprintf(stderr, "��%s malloc()\n",
	      fail_malloc ? "���顼����" : "���̤�");
    }
#endif /* DEBUG_ALLOC */
#ifdef SHOW_ROME_STRUCT
    showRomeStruct(event->display, event->window);
#endif
    break;
  }
}

void
listcallback(client_data, func, items, nitems, cur_item)
char *client_data;
int  func;
wchar_t **items;
int nitems, *cur_item;
{
  int i;
  wchar_t **p;

  switch (func) {
  case CANNA_LIST_Start:
    printf("��������\n");
    for (i = 0, p = items ; i < nitems ; i++, p++) {
      if (i == *cur_item) {
	printf("%d: (%ws)\n", i, *p);
      }
      else {
	printf("%d: %ws\n", i, *p);
      }
    }
    break;
  case CANNA_LIST_Select:
    printf("��������\n");
    break;
  case CANNA_LIST_Quit:
    printf("�������\n");
    break;
  case CANNA_LIST_Forward:
    printf("��\n");
    break;
  case CANNA_LIST_Backward:
    printf("��\n");
    break;
  case CANNA_LIST_Next:
    printf("��\n");
    break;
  case CANNA_LIST_Prev:
    printf("��\n");
    break;
  case CANNA_LIST_BeginningOfLine:
    printf("��ü\n");
    break;
  case CANNA_LIST_EndOfLine:
    printf("��ü\n");
    break;
  default:
    printf("�ʤˤ�����������\n");
    break;
  }
}

beep()
{
  XBell(dpy, 100);
}

int	    numControl = 0;
int	    jrik = 0;		/* Ⱦ�ѥ������ʤζػ� */
int         showkeys = 0;	/* ����ե��⡼�ɤǻȤäƤ��륭���򼨤� */
int	    esckill = 0, esckakutei = 0, escControl = 0, ckconn = 0;
int	    double_init = 0, final_initial = 0;
int	    given_width, width_given = 0, showMode = 0, kugiri = 0;
int         detailMode = 0, max_width;
int	    printenv = 0;
int         customtest = 0;
int         storeYomi = 0, storeThen = 0;
int         parse_data = 0;
char        parse_buf[512];
int         rengo_data = 0;
char        rengo_buf[512];
int         check_context = 0;
int	    no_mode = 0;
int	    show_max_mode = 0;
int         restrict = 0;
int         escclose = 0;
int         tpo = 0;
int         euc = 0;
#ifdef TEISEI
int	    teiseiline = 0;
int	    kanaline = 0;
#endif /* TEISEI */
#ifdef KC_SETSERVERNAME
int	    clist = 0;
#endif
int hexBy = 0, modeString = 0, undeffn = kc_normal;
#ifdef KC_DISCONNECTSERVER
int         escdisconnect = 0;
#endif /* KC_DISCONNECTSERVER */
int kakuteikana = 0;
int noname = 0;
int askphono = 0;

checkOptions(argc, argv)
int argc;
char *argv[];
{
  int i;

  for (i = 1 ; i < argc ; i++) {
    if ( !strcmp(argv[i], "-nc") ) {
      numControl = 1;
    }
#ifdef DEBUG
    else if ( !strcmp(argv[i], "-debug") ) {
      iroha_debug |= 1;
      wcKanjiControl(0, KC_DEBUGMODE, 1);
    }
#endif /* DEBUG */
    else if ( !strcmp(argv[i], "-nmode") ) {
      modeString = 1;
    }
    else if ( !strcmp(argv[i], "-imhex") ) {
      hexBy = 1;
    }
    else if ( !strcmp(argv[i], "-kakutei") ) {
      undeffn = kc_kakutei;
    }
    else if ( !strcmp(argv[i], "-through") ) {
      undeffn = kc_through;
    }
    else if ( !strcmp(argv[i], "-kill") ) {
      undeffn = kc_kill;
    }
    else if ( !strcmp(argv[i], "-bell") ) {
      extern (*jrBeepFunc)();
      jrBeepFunc = 0;
    }
    else if ( !strcmp(argv[i], "-ik") ) {
      jrik = 1;
    }
    else if ( !strcmp(argv[i], "-showkeys") ) {
      showkeys = 1;
    }
    else if ( !strcmp(argv[i], "-esckill") ) {
      esckill = 1;
      escControl = 1;
    }
    else if ( !strcmp(argv[i], "-esckakutei") ) {
      esckakutei = 1;
      escControl = 1;
    }
    else if ( !strcmp(argv[i], "-ckconn") ) {
      ckconn = 1;
    }
    else if ( !strcmp(argv[i], "-dinit") ) {
      double_init = 1;
}
    else if ( !strcmp(argv[i], "-dfinal") ) {
      double_final = 1;
    }
    else if ( !strcmp(argv[i], "-fin_init") ) {
      final_initial = 1;
    }
    else if ( !strcmp(argv[i], "-width") ) {
      width_given = 1;
      i++;
      given_width = atoi(argv[i]);
    }
    else if ( !strcmp(argv[i], "-escmode") ) {
      showMode = 1;
    }
    else if ( !strcmp(argv[i], "-escdetail1") ) {
      modeString = 1;
      detailMode = 1;
    }
    else if ( !strcmp(argv[i], "-escdetail2") ) {
      modeString = 2;
      detailMode = 2;
    }
    else if ( !strcmp(argv[i], "-kugiri") ) {
      kugiri = 1;
    }
    else if ( !strcmp(argv[i], "-printenv") ) {
      printenv = 1;
    }
    else if ( !strncmp(argv[i], "-bigmem", 7) ) {
      char *malloc();
      int megabytes = atoi(argv[i] + 7);

      if (argv[i][7] == '\0') {
	megabytes = 2;
      }
      fprintf(stderr, "%d MByte alloc ���Ƥ��ޤ�...", megabytes);
      if (malloc(megabytes * 1024 * 1024) == (char *)0) {
	fprintf(stderr, "����\n");
      }
      else {
	fprintf(stderr, "����\n");
      }
    }
    else if ( (!strcmp(argv[i], "-cs") ||
	       !strcmp(argv[i], "-cannaserver")) && i + 1 < argc ) {
      i++;
      wcKanjiControl(0, KC_SETSERVERNAME, argv[i]);
    }
    else if ( !strcmp(argv[i], "-yomi1") ) {
      yomiLevel = 1;
    }
    else if ( !strcmp(argv[i], "-yomi2") ) {
      yomiLevel = 2;
    }
    else if ( !strcmp(argv[i], "-customtest") ) {
      customtest = 1;
    }
    else if ( !strcmp(argv[i], "-storeyomi") ) {
      storeYomi = 1;
    }
    else if ( !strcmp(argv[i], "-storethen") ) {
      i++;
      storeThen = atoi(argv[i]);
    }
    else if ( !strcmp(argv[i], "-bufsize") ) {
      nbytes_given = 1;
      i++;
      given_nbytes = atoi(argv[i]);
    }
    else if ( !strcmp(argv[i], "-fn14") ) {
      /* 14 �Υե���Ȥˤ��롣*/
    }
    else if ( !strcmp(argv[i], "-fn16") ) {
      /* 16 �Υե���Ȥˤ��롣*/
    }
    else if ( !strcmp(argv[i], "-fn24") ) {
      /* 24 �Υե���Ȥˤ��롣*/
    }
    else if ( !strcmp(argv[i], "-fnhangul") ) {
      /* �ϥ󥰥�ˤ��� */
    }
#ifdef KC_SETINITFILENAME
    else if ( !strcmp(argv[i], "-f") ) {
      i++;
      wcKanjiControl(0, KC_SETINITFILENAME, argv[i]);
    }
#endif /* KC_SETINITFILENAME */
    else if ( !strcmp(argv[i], "-time") ) {
      msystime++;
    }
    else if ( !strcmp(argv[i], "-rengodic") ) {
      rengo_data = 1;
      i++;
      sprintf(rengo_buf, "rengodic \"%s\"\n", argv[i]);
    }
    else if ( !strcmp(argv[i], "-parse") ) {
      parse_data = 1;
      i++;
      sprintf(parse_buf, "%s", argv[i]);
    }
    else if ( !strcmp(argv[i], "-context") ) {
      check_context = 1;
    }
    else if ( !strcmp(argv[i], "-nomode") ) {
      no_mode = 1;
    }
    else if ( !strcmp(argv[i], "-maxmode") ) {
      show_max_mode = 1;
    }
    else if ( !strcmp(argv[i], "-restrict") ) {
      i++;
      if ( !strcmp(argv[i], "none") ) {
	restrict = CANNA_NOTHING_RESTRICTED + 1;
      }
      else if ( !strcmp(argv[i], "alphanum") ) {
	restrict = CANNA_ONLY_ALPHANUM + 1;
      }
      else if ( !strcmp(argv[i], "hex") ) {
	restrict = CANNA_ONLY_HEX + 1;
      }
      else if ( !strcmp(argv[i], "num") ) {
	restrict = CANNA_ONLY_NUMERIC + 1;
      }
      else if ( !strcmp(argv[i], "all") ) {
	restrict = CANNA_NOTHING_ALLOWED + 1;
      }
    }
    else if ( !strcmp(argv[i], "-escclose") ) {
      escclose = 1;
      escControl = 1;
    }
#ifdef TEISEI
    else if ( !strcmp(argv[i], "-teisei") ) {
      teiseiline = 1;
    }
    else if ( !strcmp(argv[i], "-kana") ) {
      kanaline = 1;
    }
#endif /* TEISEI */
#ifdef KC_SETLISTCALLBACK
    else if ( !strcmp(argv[i], "-clist") ) {
      clist = 1;
    }
#endif /* KC_SETLISTCALLBACK */
    else if ( !strcmp(argv[i], "-tpo") ) {
      tpo = 1;
    }
    else if ( !strcmp(argv[i], "-euc") ) {
      euc = 1;
    }
    else if ( !strcmp(argv[i], "-abandon") ) {
      abandon_kakutei = 1;
    }
    else if ( !strcmp(argv[i], "-wide") ) {
      WinWidth = 160;
    }
#ifdef KC_DISCONNECTSERVER
    else if ( !strcmp(argv[i], "-escdis") ) {
      escdisconnect = 1;
      escControl = 1;
    }
#endif /* KC_DISCONNECTSERVER */
    else if ( !strcmp(argv[i], "-kakuteikana") ) {
      kakuteikana = 1;
    }
    else if ( !strcmp(argv[i], "-noname") ) {
      noname = 1;
    }
    else if ( !strcmp(argv[i], "-askphono") ) {
      askphono = 1;
    }
    else {
      fprintf(stderr, "usage: %s [-option ...]\n", argv[0]);
      fprintf(stderr, "options:\n");
/* ���Σ��Ĥ�̵���Ǥ��롣
      fprintf(stderr, "-[no]im   ����Ϻ��Ʊ�����������ư���򤹤�(���ʤ�)\n");
      fprintf(stderr, "-[no]roc  ������ȿž����(���ʤ�)\n");
*/
      fprintf(stderr, "-bell     ̤��������ǥ٥���Ĥ餵�ʤ�\n");
      fprintf(stderr, "-bufsize num buffer_return�Υ�������Ϳ����\n");
      fprintf(stderr, "-ckconn   �����ФȤ���³�Υ����å�\n");
      fprintf(stderr, "-context  ����ƥ����Ȥ�Ĵ�٤�\n");
      fprintf(stderr, "-customtest KC_PARSE�Υƥ���\n");
      fprintf(stderr, "-debug    �ǥХ��⡼��\n");
      fprintf(stderr, "-dinit    KC_INITIALIZE�򣲲�³���ƹԤ��ƥ���\n");
      fprintf(stderr, "-dfinal   KC_FINALIZE�򣲲�³���ƹԤ��ƥ���\n");
      fprintf(stderr, "-esckakutei ESC������̤����ʸ�������ꤹ��\n");
      fprintf(stderr, "-esckill  ESC������̤����ʸ��������ä���\n");
      fprintf(stderr, "-escmode  ESC�����ǥ⡼�ɤ�ɽ������\n");
      fprintf(stderr, "-escdetail1 ESC�����ǥ᥸�㡼�⡼�ɤ�ɽ������\n");
      fprintf(stderr, "-escdetail2 ESC�����ǥޥ��ʡ��⡼�ɤ�ɽ������\n");
      fprintf(stderr, "-escdis   ESC�����ǥ����ФȤ���³���ڤ�\n");
      fprintf(stderr, "-f        ���˥���饤���ե��������ꤹ��\n");
      fprintf(stderr, "-fin_init KC_FINALIZE�θ�KC_INITIALIZE��Ԥ��ƥ���\n");
      fprintf(stderr, "-fn14     14�ɥåȥե���Ȥ�Ȥ�\n");
      fprintf(stderr, "-fn16     16�ɥåȥե���Ȥ�Ȥ�\n");
      fprintf(stderr, "-fn24     24�ɥåȥե���Ȥ�Ȥ�\n");
      fprintf(stderr, "-ik       Ⱦ�ѥ������ʤ˻����Ѵ����ʤ�\n");
      fprintf(stderr, "-imhex    ���������Ϥϣ�ʸ���ܤǳ��ꤹ��\n");
      fprintf(stderr, "-kakutei  ̤���������̤����ʸ�������ꤹ��\n");
      fprintf(stderr, "-kill     ̤���������̤����ʸ��������ä���\n");
      fprintf(stderr, "-kugiri   ʸ��ζ��ڤ�˶���������\n");
      fprintf(stderr, "-nc       ���������ǥ��ޥ�ɤ�¹Ԥ���\n");
      fprintf(stderr, "-nmode    �⡼��ɽ��ʸ������ͤ�ɽ��ʸ���ˤ���\n");
      fprintf(stderr, "-parse str �������ޥ���ʸ�����Ϳ����\n");
      fprintf(stderr, "-printenv �Ķ��ѿ���ɽ������\n");
      fprintf(stderr, "-rengodic dic Ϣ�켭�����ꤹ��\n");
      fprintf(stderr, "-cs       ���ͥ��Ȥ��륵���Ф���ꤹ��\n");
      fprintf(stderr, "-showkeys ͽ�󤵤�Ƥ��륭����ɽ������\n");
      fprintf(stderr, "-storeyomi�ɤߤ򥹥ȥ�����\n");
      fprintf(stderr, "-storethen num �ɤߤ򥹥ȥ����Ƽ��˲��򤹤뤫����\n");
      fprintf(stderr, "-through  ̤��������򤽤Τޤ��̤�\n");
      fprintf(stderr, "-width n  ����Ϳ����\n");
      fprintf(stderr, "-yomi1    ���ꤷ�����ɤߤ�ɽ������\n");
      fprintf(stderr, "-yomi2    ���ꤷ�����ɤߤ����ϥ������ɽ������\n");
      fprintf(stderr, "-tpo      �ƥ����Ѥ�ɽ������Ϥ���\n");
      exit (1);
    }
  }
}

initializeCanna(argc, argv, envp)
int argc;
char *argv[], *envp[];
{
  {
    char **warn;

    if (final_initial) {
      wcKanjiControl(0, KC_INITIALIZE, (char *)0);
      wcKanjiControl(0, KC_FINALIZE, (char *)0);
      wcKanjiControl(0, KC_INITIALIZE, (char *)0);
      wcKanjiControl(0, KC_FINALIZE, (char *)0);
      wcKanjiControl(0, KC_INITIALIZE, (char *)0);
      wcKanjiControl(0, KC_FINALIZE, (char *)0);
    }
    wcKanjiControl(0, KC_INITIALIZE, (char *)&warn);
    if (double_init) {
      int ret;

      ret = wcKanjiControl(0, KC_INITIALIZE, (char *)&warn);
      fprintf(stderr, "�����ܤ� initialize ���ͤ� %d\n", ret);
    }
    if (warn) {
      char **p;
      
      for (p = warn; *p ; p++) {
	fprintf(stderr, "%s\n", *p);
      }
    }

#ifdef KC_SETAPPNAME
    if (!noname) {
      wcKanjiControl(0, KC_SETAPPNAME, "sample");
    }
#endif /* KC_SETAPPNAME */

#ifdef KC_QUERYPHONO
    if (askphono) {
      char *foo;
      wcKanjiControl(0, KC_QUERYPHONO, (char *)&foo);
      if (foo) {
	printf("���޻������Ѵ��ơ��֥�� \"%s\" �Ǥ���\n", foo);
      }
    }
#endif

    wcKanjiControl(0, KC_QUERYMODE, (char *)wb[0].mode.str);
    wcKanjiControl(1, KC_QUERYMODE, (char *)wb[1].mode.str);
    wb[0].mode.length = Wslen(wb[0].mode.str);
    wb[0].mode.revPos = wb[0].mode.revLen = 0;
    wb[1].mode.length = Wslen(wb[1].mode.str);
    wb[1].mode.revPos = wb[1].mode.revLen = 0;
  }
  if (ckconn) {
    if (wcKanjiControl(0, KC_QUERYCONNECTION, (char *)0)) {
      fprintf(stderr, "�����ФȤĤʤ��äƤ����\n");
    }
    else {
      fprintf(stderr, "�����ФȤĤʤ��äƤ��ʤ���\n");
    }
  }
  max_mode_strlen = wcKanjiControl(0, KC_QUERYMAXMODESTR, (char *)0) + 2;
  if (show_max_mode) {
    fprintf(stderr, "�⡼��ʸ����κ��祳������� %d �Ǥ���\n",
	    max_mode_strlen - 2);
  }
  if (width_given) {
    fprintf(stderr, "�������ɽ���Υ�������� %d �Ǥ���\n", given_width);
    max_width = given_width;
  }
  else {
    max_width = WinWidth - 2 - max_mode_strlen;
  }
  wcKanjiControl(0, KC_SETWIDTH, (char *)(POINTERINT)max_width);
  wcKanjiControl(1, KC_SETWIDTH, (char *)(POINTERINT)max_width);

#ifdef KC_SETLISTCALLBACK
  if (clist) {
    jrListCallbackStruct lcs;
    void listcallback();

    lcs.client_data = 0;
    lcs.callback_func = listcallback;
    wcKanjiControl(0, KC_SETLISTCALLBACK, (char *)&lcs);
    wcKanjiControl(1, KC_SETLISTCALLBACK, (char *)&lcs);
  }
#endif
  if (kugiri) {
    wcKanjiControl(0, KC_SETBUNSETSUKUGIRI, (char *)(POINTERINT)1);
  }
  if (rengo_data) {
    int i, n;
    char *p = rengo_buf, **pp;

    n = wcKanjiControl(0, KC_PARSE, (char *)&p);
    pp = (char **)p;
    for (i = 0 ; i < n ; i++) {
      fprintf(stderr, "%s\n", *pp++);
    }
  }
  if (parse_data) {
    int i, n;
    char *p = parse_buf, **pp;

    n = wcKanjiControl(0, KC_PARSE, (char *)&p);
    pp = (char **)p;
    for (i = 0 ; i < n ; i++) {
      fprintf(stderr, "%s\n", *pp++);
    }
  }
  wcKanjiControl(0, KC_SETMODEINFOSTYLE, (char *)(POINTERINT)modeString);
  if (hexBy) {
    wcKanjiControl(0, KC_SETHEXINPUTSTYLE, (char *)(POINTERINT)hexBy);
  }
  wcKanjiControl(0, KC_SETUNDEFKEYFUNCTION, (char *)(POINTERINT)undeffn);
  wcKanjiControl(0, KC_INHIBITHANKAKUKANA, (char *)(POINTERINT)jrik);
  wcKanjiControl(0, KC_YOMIINFO, (char *)(POINTERINT)yomiLevel);

  if (kakuteikana) {
    wcKanjiStatusWithValue ksv;
    wcKanjiStatus ks;
    wchar_t xxxx[512];

    ksv.buffer = xxxx;
    ksv.n_buffer = 512;
    ksv.ks = &ks;
    ksv.buffer[0] = '@';
    ksv.val = CANNA_FN_JapaneseMode;
    wcKanjiControl(0, KC_DO, (char *)&ksv);
    ksv.val = CANNA_FN_BaseKakutei;
    wcKanjiControl(0, KC_DO, (char *)&ksv);
    ksv.val = CANNA_FN_BaseKatakana;
    wcKanjiControl(0, KC_DO, (char *)&ksv);
  }

  if (customtest) {
    char *custom = "henkan C-l\nyomiganai {\n  kigouMode ^l\n}\n";

    printf("C-l ���Ѵ����ɤߤ�̵�����֤Ǥϵ���⡼�ɡ��������Ƥޤ���\n");
    wcKanjiControl(0, KC_PARSE, (char *)&custom);
  }
  if (showkeys) {
    char keys[20];
    int n, i;

    n = wcKanjiControl(0, KC_MODEKEYS, keys);
    fprintf(stderr, "����ե��٥åȥ⡼�ɤ� %d �ĤΥ�����ȤäƤ���\n", n);
    fprintf(stderr, "�����ϡ�");
    for (i = 0 ; i < n ; i++) {
      fprintf(stderr, "0x%02x��", keys[i]);
    }
    fprintf(stderr, "�Ǥ���\n");
  }
  if (printenv) {
    char **p;
    for (p = envp ; *p ; p++) {
      fprintf(stderr, "env�ϡ�(%s)\n", *p);
    }
  }
  if (check_context) {
    fprintf(stderr, "context: %d, bushuContext: %d.\n",
	    wcKanjiControl(0, KC_GETCONTEXT, (char *)(POINTERINT)0),
	    wcKanjiControl(0, KC_GETCONTEXT, (char *)(POINTERINT)1));
  }
  if (no_mode) {
    fprintf(stderr, "�����鲼�����¤����˾�ϥ⡼���ѹ������¤��ޤ�....\n");
    if (wcKanjiControl(1, KC_INHIBITCHANGEMODE, (char *)(POINTERINT)1) < 0) {
      fprintf(stderr, "...�⡼�ɤ����¤򤹤�Τ˼��Ԥ��ޤ�����\n");
    }
    else {
      fprintf(stderr, "...�⡼�ɤ��ѹ������¤��ޤ�����\n");
    }
  }
  if (restrict) {
    if (wcKanjiControl(1,
		      KC_LETTERRESTRICTION, (char *)(POINTERINT)(restrict - 1)) < 0) {
      fprintf(stderr, "ʸ��������¤˼��Ԥ��ޤ�����\n");
    }
  }
}

/*
 *
 * optionControl �Υ꥿�����ͤ� option �ν����� wcKanjiString ������ɥ�
 * ����뤫����ʤ�����ɽ����1 �ϥ���ɥ����줺��wcKanjiString ��Ԥ��٤�
 * �Ǥ��뤳�Ȥ򼨤���0 �ϥ���ɥ�����뤳�Ȥ򼨤���
 *
 */

optionControl(event, key, wcbuf, ks)
XEvent *event;
wchar_t *wcbuf;
int key;
wcKanjiStatus *ks;
{
  int notControl = 1;
  int defineKanji = 0;

  if (escControl && nbytes > 0 && key == 0x1b) {
    wcKanjiStatusWithValue ksv;
    ksv.buffer = (wchar_t *)wcbuf;
    ksv.n_buffer = STRBUFSIZE;
    ksv.ks = ks;
    notControl = 0; defineKanji = 0;
    if (esckill) {
      wcKanjiControl((event->xany.window == wb[0].win) ? 0 : 1, KC_KILL,
		     (char *)&ksv);
      nbytes = ksv.val;
    }
    else if (esckakutei) {
      wcKanjiControl((event->xany.window == wb[0].win) ? 0 : 1,
		     KC_KAKUTEI, (char *)&ksv);
      nbytes = ksv.val;
    }
    else if (escclose) {
      wcKanjiControl((event->xany.window == wb[0].win) ? 0 : 1,
		     KC_CLOSEUICONTEXT, (char *)&ksv);
      nbytes = ksv.val;
    }
#ifdef KC_DISCONNECTSERVER
    else if (escdisconnect) {
      wcKanjiControl((event->xany.window == wb[0].win) ? 0 : 1,
		     KC_DISCONNECTSERVER, (char *)0);
      ksv.ks->length = 0;
      ksv.ks->info = 0;
      nbytes = 0;
    }
#endif /* KC_DISCONNECTSERVER */
  }
  else if (showMode && nbytes > 0 && key == 0x1b) {
    wchar_t foo[100];
    
    wcKanjiControl((event->xany.window == wb[0].win) ? 0 : 1,
		   KC_QUERYMODE, (char *)foo);
    fprintf(stderr, "%ws\n", foo);
    return;
  }
  else if (detailMode && nbytes > 0 && key == 0x1b) {
    unsigned char detailfoo[10];
    
    if ( detailMode == 2 ) {
      detailfoo[2] = 0x99;
    }
    else {
      detailfoo[1] = 0x99;
    }
    jrKanjiControl((event->xany.window == wb[0].win) ? 0 : 1,
		   KC_QUERYMODE, (char *)detailfoo);
    fprintf(stderr, "majorMode = %c\n", detailfoo[0]);
    if ( detailMode == 2 ) {
      fprintf(stderr, "minorMode = %c\n", detailfoo[1]);
      if(detailfoo[2] != 0x99) {
	fprintf(stderr, "error: over run!\n");
      }
      else {
	fprintf(stderr, "not over run\n");
      }
    }
    else {
      if(detailfoo[1] != 0x99) {
	fprintf(stderr, "error: over run!\n");
      }
      else {
	fprintf(stderr, "not over run\n");
      }
    }
    fprintf(stderr, "%s\n", "---------------");
    return;
  }
  else if (storeYomi && nbytes > 0 && key == 0x1b) {
    static int ntime = 0;
    wchar_t wcbuf[STRBUFSIZE], wcbuf2[STRBUFSIZE], wcbuf3[STRBUFSIZE];
    wcKanjiStatusWithValue ksv;

    ntime++;
    mbstowcs(wcbuf2, "���פꤱ������󤫤餹�Ȥ����줿��ߤǤ���", STRBUFSIZE);
    if (ntime % 2) {
      mbstowcs(wcbuf3, "apurike-shonkarasutoasaretayomidesu.", STRBUFSIZE);
    }
    ksv.buffer = (wchar_t *)wcbuf;
    ksv.n_buffer = STRBUFSIZE;
    ksv.ks = ks;
    ks->echoStr = wcbuf2;
    ks->mode = (ntime % 2) ? wcbuf3 : (wchar_t *)0;
    notControl = 0; defineKanji = 0;

    wcKanjiControl((event->xany.window == wb[0].win) ? 0 : 1, KC_STOREYOMI,
		   (char *)&ksv);
#ifdef KC_DO
    if (storeThen) {
      ksv.buffer[0] = '@';
      ksv.val = storeThen;
      wcKanjiControl((event->xany.window == wb[0].win) ? 0 : 1, KC_DO, 
		     (char *)&ksv);
    }
#endif /* KC_DO */
		  
    nbytes = ksv.val;
  }

  if (numControl && nbytes > 0) {
    wchar_t wcbuf[STRBUFSIZE];
    wcKanjiStatusWithValue ksv;
    ksv.buffer = (wchar_t *)wcbuf;
    ksv.n_buffer = STRBUFSIZE;
    ksv.ks = ks;
    notControl = 0; defineKanji = 0;
    switch (key)
      {
      case '0':
	ksv.val = CANNA_MODE_AlphaMode;
	break;
      case '1':
	ksv.val = CANNA_MODE_HenkanMode;
	break;
      case '2':
	ksv.val = CANNA_MODE_HexMode;
	break;
      case '3':
	ksv.val = CANNA_MODE_BushuMode;
	break;
      case '4':
	ksv.val = CANNA_MODE_KigoMode;
	break;
      case '5':
	ksv.val = CANNA_MODE_ZenHiraKakuteiMode;
	break;
      case '6':
	ksv.val = CANNA_MODE_ZenKataKakuteiMode;
	break;
      case '7':
	ksv.val = CANNA_MODE_HanKataKakuteiMode;
	break;
      case '8':
	ksv.val = CANNA_MODE_ZenAlphaKakuteiMode;
	break;
      case '9':
	ksv.val = CANNA_MODE_HanAlphaKakuteiMode;
	break;
      case '-':
	{
	  wchar_t defines[64];
	  int len;

	  len = mbstowcs(defines, "��Ͽ�̥��", 64);
	  ksv.ks->echoStr = defines;
	  ksv.ks->length = len;
	  notControl = 1;
	  defineKanji = 1;
	}
	break;
      case '=':
	ksv.ks->echoStr = (wchar_t *)"";
	ksv.ks->length = 0;
	notControl = 1;
	defineKanji = 1;
	break;
      default:
	notControl = 1;
      }
    if (!notControl) {
      wcKanjiControl((event->xany.window == wb[0].win) ? 0 : 1,
		     KC_CHANGEMODE, (char *)&ksv);
      nbytes = ksv.val;
    }
    else if (defineKanji) {
      wcKanjiControl((event->xany.window == wb[0].win) ? 0 : 1,
		     KC_DEFINEKANJI, (char *)&ksv);
      nbytes = ksv.val;
    }
  }
  return (notControl && !defineKanji);
}

checkGlineWidth(ks)
wcKanjiStatus *ks;
{
#ifdef HAVE_WCHAR_OPERATION
  int len = ks->gline.length;
  wchar_t *gline = ks->gline.line;
  int width = 0;
  static int counter = 0;

  for (i = 0 ; i < len ; i++) {
    if (isG0(gline[i])) {
      width ++;
    }
    else if (isG1(gline[i])) {
      width += 2;
    }
    else if (isG2(gline[i])) {
      width ++;
    }
    else {
      width += 2;
    }
  }
  if (width > max_width) {
    counter++;
    fprintf(stderr, "[%d] �����礭�����ޤ� %d (������ %d)\n",
	    counter, width, max_width);
  }
#endif
  return 0;
}

/* checkIfFunctionalChar -- ���եȥ����Τ褦��̵��̣�ʥ������ɤ�����Ƚ��

  ��:		��	��̣������
  		��	̵��̣(��ǥ��ե����������ʤ�)
*/

static int
checkIfFunctionalChar(event_struct, keysym, buffer_return, bytes_buffer)
XKeyEvent *event_struct;
KeySym keysym;
unsigned char *buffer_return;
int bytes_buffer;
{
  int functionalChar = 0;

  switch ((int)keysym)
    {
    case XK_KP_F1:
    case XK_KP_F2:
    case XK_KP_F3:
    case XK_KP_F4:
      *buffer_return = 	CANNA_KEY_PF1 + (int)keysym - (int)XK_KP_F1;
      functionalChar = 1;
      break;
    case XK_F1:
    case XK_F2:
    case XK_F3:
    case XK_F4:
    case XK_F5:
    case XK_F6:
    case XK_F7:
    case XK_F8:
    case XK_F9:
    case XK_F10:
    case XK_F11:
    case XK_F12:
    case XK_F13:
    case XK_F14:
    case XK_F15:
    case XK_F16:
      *buffer_return = CANNA_KEY_F1 + (int)keysym - (int)XK_F1;
      functionalChar = 1;
      break;
    case XK_Insert:
      *buffer_return = CANNA_KEY_Insert;
      functionalChar = 1;
      break;
    case XK_Prior:
      *buffer_return = CANNA_KEY_Rollup;
      functionalChar = 1;
      break;
    case XK_Next:
      *buffer_return = CANNA_KEY_Rolldown;
      functionalChar = 1;
      break;
    case XK_Muhenkan:
      if (event_struct->state & 4 /* control-shifted */)
	*buffer_return = CANNA_KEY_Cntrl_Nfer;
      else if (event_struct->state & 1 /* shifted */)
	*buffer_return = CANNA_KEY_Shift_Nfer;
      else
	*buffer_return = CANNA_KEY_Nfer;
      functionalChar = 1;
      break;
    case XK_Kanji:
      if (event_struct->state & 4 /* control-shifted */)
	*buffer_return = CANNA_KEY_Cntrl_Xfer;
      else if (event_struct->state & 1 /* shifted */)
	*buffer_return = CANNA_KEY_Shift_Xfer;
      else
	*buffer_return = CANNA_KEY_Xfer;
      functionalChar = 1;
      break;
    case XK_Up:
      if (event_struct->state & 4 /* control-shifted */)
	*buffer_return = CANNA_KEY_Cntrl_Up;
      else if (event_struct->state & 1 /* shifted */)
	*buffer_return = CANNA_KEY_Shift_Up;
      else
	*buffer_return = CANNA_KEY_Up;
      functionalChar = 1;
      break;
    case XK_Down:
      if (event_struct->state & 4 /* control-shifted */)
	*buffer_return = CANNA_KEY_Cntrl_Down;
      else if (event_struct->state & 1 /* shifted */)
	*buffer_return = CANNA_KEY_Shift_Down;
      else
	*buffer_return = CANNA_KEY_Down;
      functionalChar = 1;
      break;
    case XK_Right:
      if (event_struct->state & 4 /* control-shifted */)
	*buffer_return = CANNA_KEY_Cntrl_Right;
      else if (event_struct->state & 1 /* shifted */)
	*buffer_return = CANNA_KEY_Shift_Right;
      else
	*buffer_return = CANNA_KEY_Right;
      functionalChar = 1;
      break;
    case XK_Left:
      if (event_struct->state & 4 /* control-shifted */)
	*buffer_return = CANNA_KEY_Cntrl_Left;
      else if (event_struct->state & 1 /* shifted */)
	*buffer_return = CANNA_KEY_Shift_Left;
      else
	*buffer_return = CANNA_KEY_Left;
      functionalChar = 1;
      break;
    case XK_Help:
      *buffer_return = CANNA_KEY_Help;
      functionalChar = 1;
      break;
    case XK_Home:
      *buffer_return = CANNA_KEY_Home;
      functionalChar = 1;
      break;
    }
  return functionalChar;
}

static wchar_t *
Wscpy(dst, src)
wchar_t *dst, *src;
{
  wchar_t *p = dst;

  while (*src) {
    *p++ = *src++;
  }
  return dst;
}

static wchar_t *
Wsncpy(dst, src, n)
wchar_t *dst, *src;
int n;
{
  wchar_t *p = dst;

  while (n-- > 0) {
    *p++ = *src++;
  }
  return dst;
}

static int
Wslen(src)
wchar_t *src;
{
  int i = 0;

  while (*src++)
    i++;
  
  return i;
}

static unsigned char wchar_type; /* �磻�ɥ���饯���Υ�����(���򸫤�) */

#define CANNA_WCTYPE_16 0  /* 16�ӥå�ɽ�� */
#define CANNA_WCTYPE_32 1  /* 32�ӥå�ɽ�� */
#define CANNA_WCTYPE_OT 99 /* ����¾��ɽ�� */
/*
 WCinit() -- �磻�ɥ���饯���Ȥ��Ƥɤ줬�Ȥ��Ƥ��뤫���ǧ����

        ���δؿ����ƤӽФ����ޤ��� setlocale ���ʤ���Ƥ��ʤ���Фʤ�ʤ�
 */

#define TYPE16A 0x0000a4a2
#define TYPE32A 0x30001222

static int
WCinit()
{
  unsigned char *a = (unsigned char *)"��"; /* 0xa4a2 */
  wchar_t wc[24];

#ifdef HAVE_WCHAR_OPERATION
  if (mbstowcs(wc, a, sizeof(wc) / sizeof(wchar_t)) != 1) {
    /* ¿ʬ setlocale ���ʤ���Ƥ��ʤ� */
    setlocale(LC_CTYPE, "");
    if (mbstowcs(wc, a, sizeof(wc) / sizeof(wchar_t)) != 1) {
      setlocale(LC_CTYPE, JAPANESE_LOCALE);
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


int
MBstowcs(dest, src, destlen)
wchar_t *dest;
unsigned char *src;
int destlen;
{
  register int i, j;
  register unsigned char ec;
  static first_time = 1;

  if (first_time) {
    WCinit();
    first_time = 0;
  }

  if (wchar_type == CANNA_WCTYPE_16) {
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
  else if (wchar_type == CANNA_WCTYPE_32) {
    return 0; /* �ޤ�����ץꤷ�Ƥ��ʤ� */
  }
  else {
    return 0;
  }
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
    break;
  default:
    return 0; /* �ɤ����褦 */
    break;
  }
  /* NOTREACHED */
}

int
isG0(wc)
wchar_t wc;
{
  return (WWhatGPlain(wc) == 0);
}

int
isG1(wc)
wchar_t wc;
{
  return (WWhatGPlain(wc) == 1);
}

int
isG2(wc)
wchar_t wc;
{
  return (WWhatGPlain(wc) == 2);
}

