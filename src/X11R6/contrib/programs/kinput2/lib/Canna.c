/*
 * Copyright (c) 1990  Software Research Associates, Inc.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Software Research Associates not be
 * used in advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.  Software Research
 * Associates makes no representations about the suitability of this software
 * for any purpose.  It is provided "as is" without express or implied
 * warranty.
 *
 * Author:  Makoto Ishisone, Software Research Associates, Inc., Japan
 */

/* Copyright 1991 NEC Corporation, Tokyo, Japan.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of NEC Corporation
 * not be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.  NEC 
 * Corporation makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 *
 * NEC CORPORATION DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN 
 * NO EVENT SHALL NEC CORPORATION BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF 
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR 
 * OTHER TORTUOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR 
 * PERFORMANCE OF THIS SOFTWARE. 
 *
 * Author: Akira Kon, NEC Corporation.  (kon@d1.bs2.mt.nec.co.jp)
 */

/* $BD>$5$J$1$l$P$J$i$J$$$H$3$m(B

 $B!&(BDestroy $B$,8F$P$l$J$$$N$G(B CloseUIContext $B$G$-$J$$!#(B
 $B!&%b!<%INN0h$NBg$-$5!#(B($B$3$l$OB>$N%U%!%$%k$@$m$&$J(B)

 */

#ifndef lint
static char *rcsid = "$Id: Canna.c,v 1.41 1993/07/29 05:56:51 kon Exp $";
#endif

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Xmu/Atoms.h>
#define XK_KATAKANA
#include <X11/keysym.h>
#if XtSpecificationRelease > 4
#include <X11/Xfuncs.h>
#endif
#include "CannaP.h"

#define _WCHAR_T /* $B$3$NDj5A$O(B jrkanji.h $B$G(B wcKanjiStatus $B$J$I$rDj5A$9$k$?$a(B */
#define wchar_t wchar

#include <canna/jrkanji.h>

static XtResource resources[] = {
#define offset(field) XtOffset(CannaObject, canna.field)
    { XtNcannahost, XtCCannahost, XtRString, sizeof(String),
	offset(cannahost), XtRString, NULL },
    { XtNcannafile, XtCCannafile, XtRString, sizeof(String),
	offset(cannafile), XtRString, NULL },
#undef offset
};

static void ClassInitialize();
static void Initialize(), Destroy();
static Boolean SetValues();
static int InputEvent();
static ICString *GetMode();
static int CursorPos();
static int NumSegments();
static ICString *GetSegment();
static int CompareSegment();
static ICString *GetItemList();
static int SelectItem();
static int ConvertedString();
static int ClearConversion();
static void displayPreEdit();

static initializeCannaConnection();
static toJapaneseMode();
static void convend();
static changeTextForCanna();
static copyInWchar();
static fixProcForCanna();
static shiftRight(), shiftLeft(), shiftLeftAll();
static ibufInitialize(), freeIBuf();

#ifdef KC_SETLISTCALLBACK
static void listfunc();
static void openSelection();
#define SELECTION_SET 0 /* SelectionStart $B$r$7$F$bNI$$$H8@$&>pJs$r@_Dj$9$k(B */
#define SELECTION_DO  1 /* $B<B:]$K(B SelectionStart $B$r3+;O$9$k(B */
#else /* !KC_SETLISTCALLBACK */
#define openSelection(x, y, z)
#endif /* !KC_SETLISTCALLBACK */

static ICString *GetAuxSegments();

CannaClassRec cannaClassRec = {
  { /* object fields */
    /* superclass		*/	(WidgetClass) &inputConvClassRec,
    /* class_name		*/	"Canna",
    /* widget_size		*/	sizeof(CannaRec),
    /* class_initialize		*/	ClassInitialize,
    /* class_part_initialize	*/	NULL,
    /* class_inited		*/	FALSE,
    /* initialize		*/	Initialize,
    /* initialize_hook		*/	NULL,
    /* obj1			*/	NULL,
    /* obj2			*/	NULL,
    /* obj3			*/	0,
    /* resources		*/	resources,
    /* num_resources		*/	XtNumber(resources),
    /* xrm_class		*/	NULLQUARK,
    /* obj4			*/	FALSE,
    /* obj5			*/	FALSE,
    /* obj6			*/	FALSE,
    /* obj7			*/	FALSE,
    /* destroy			*/	Destroy,
    /* obj8			*/	NULL,
    /* obj9			*/	NULL,
    /* set_values		*/	SetValues,
    /* set_values_hook		*/	NULL,
    /* obj10			*/	NULL,
    /* get_values_hook		*/	NULL,
    /* obj11			*/	NULL,
    /* version			*/	XtVersion,
    /* callback_private		*/	NULL,
    /* obj12			*/	NULL,
    /* obj13			*/	NULL,
    /* obj14			*/	NULL,
    /* extension		*/	NULL
  },
  { /* inputConv fields */
    /* InputEvent		*/	InputEvent,
    /* GetMode			*/	GetMode,
    /* CursorPos		*/	CursorPos,
    /* NumSegments		*/	NumSegments,
    /* GetSegment		*/	GetSegment,
    /* CompareSegment		*/	CompareSegment,
    /* GetItemList		*/	GetItemList,
    /* SelectItem		*/	SelectItem,
    /* GetConvertedString	*/	ConvertedString,
    /* ClearConversion		*/	ClearConversion,
    /* GetAuxSegments		*/	GetAuxSegments,
    /* SupportMultipleObjects	*/	True,
    /* NoMoreObjects		*/	False,
  },
  { /* canna fields */
    /* foo			*/	0,
  }
};

WidgetClass cannaObjectClass = (WidgetClass)&cannaClassRec;

static void fix();

static ICString *SymbolList;
static int NumSymbols;

static void addObject();
static void deleteObject();

static int checkIfFunctionalChar();

static int ignoreListfunc = 0;

static Display *displaybell = (Display *)0;

static void
ClassInitialize()
{
  /* $B2?$b$7$J$$(B */
}

static int
XKanaLookup(event_struct, buffer_return, bytes_buffer,
	    keysym, status_return)
XKeyEvent *event_struct;
char *buffer_return;
int bytes_buffer;
KeySym *keysym;
XComposeStatus *status_return;
{
  int res;
  res = XLookupString(event_struct, buffer_return, bytes_buffer,
		      keysym, status_return);
  if (!res && XK_overline <= *keysym && *keysym <= XK_semivoicedsound) {
    buffer_return[0] = (unsigned long)(*keysym) & 0xff;
    res = 1;
  }
  return res;
}

static int
InputEvent(w, event)
Widget w;
XEvent *event;
{
    CannaObject obj = (CannaObject)w;
    wchar buf[1024];
    char kanabuf[20];
    KeySym ks;
    XComposeStatus   compose_status;
    wcKanjiStatus    kanji_status;
    int len, nbytes, functionalChar;

    /* KeyPress$B0J30$O<N$F$k(B */
    if (event->type != KeyPress /*&& event->type != KeyRelease*/) return 0;

    obj->canna.textchanged = False;

    /* $B<h$j$"$($:J8;z$KD>$7$F$7$^$&(B */
    kanabuf[0] = '\0';
    nbytes = XKanaLookup(event, kanabuf, 20, &ks, &compose_status);

    buf[0] = (wchar)kanabuf[0]; /* $B$-$?$J$$(B */

    if (ks == XK_space && (event->xkey.state & ShiftMask)) {
      void convend();

      convend(obj);
      return 0;
    }

    /* $BD9$5$,%<%m$N$b$N$K4X$7$F$O$A$g$C$H5_:Q(B */
    functionalChar = checkIfFunctionalChar(event, ks, buf, 1024);
    /* shift+$B"*$N$h$&$JJ8;z$@$C$?$i#1J8;z$H?t$($k(B */
    if ( !nbytes && functionalChar ) {
      nbytes = 1;
    }

    /* $BMW$i$J$$>l9g$OL5;k$9$k(B */
    if (nbytes == 0) return 0;

    /* $B%Y%k$rLD$i$9%G%#%9%W%l%$$N@_Dj(B */
 
    displaybell = XtDisplayOfObject((Widget)obj);

    /* $B$+$J4A;zJQ49$9$k(B */
    len = wcKanjiString((int)obj, (int)buf[0],
			(wchar_t *)buf, 1024, &kanji_status);

    displayPreEdit(obj, len, buf, &kanji_status);
    return (kanji_status.info & KanjiThroughInfo) ? 1 : 0;
}

static void
displayPreEdit(obj, len, buf, ks)
CannaObject obj;
int len;
wchar *buf;
wcKanjiStatus *ks;
{
    Widget w = (Widget)obj;

    if (len > 0) { /* $B3NDjJ8;z$,$"$k>l9g(B */
      if (len == 1 && (ks->info & KanjiThroughInfo) &&
	  (((buf[0] & 0x7f) < 0x20 && buf[0] != '\r' && buf[0] != '\t') ||
	   buf[0] == '\177'))
	/* XSendEvent$B$GAw$C$F$b$i$&$h$&$JJ8;z(B($B2?$b$7$J$$(B) */;
      else {
	ks->info &= ~KanjiThroughInfo;
	fixProcForCanna(obj, buf, len);
	fix(obj);
      }
    }

    changeTextForCanna(obj, ks);

    /* $B%F%-%9%H$NJQ2=$r%A%'%C%/$9$k(B */
    if (obj->canna.textchanged) {
	XtCallCallbackList(w, obj->inputConv.textchangecallback,
			   (XtPointer)NULL);
	obj->canna.textchanged = False;
    }

    /* $BF~NO%b!<%I$r%A%'%C%/$9$k(B */
    if (ks->info & KanjiModeInfo) {
      XtCallCallbackList(w, obj->inputConv.modechangecallback,
			 (XtPointer)NULL);
    }

    if (ks->info & KanjiGLineInfo) { /* $B0lMw9T$,$"$k>l9g(B */
      ICAuxControlArg arg;

      switch (obj->canna.ibuf->candstat) {
      case CANNA_GLINE_Start:
	arg.command = ICAuxStart;
	XtCallCallbackList((Widget)obj, obj->inputConv.auxcallback,
			   (XtPointer)&arg);
	break;
      case CANNA_GLINE_End:
	arg.command = ICAuxEnd;
	XtCallCallbackList((Widget)obj, obj->inputConv.auxcallback,
			   (XtPointer)&arg);
	break;
      case CANNA_GLINE_Change:
	arg.command = ICAuxChange;
	XtCallCallbackList((Widget)obj, obj->inputConv.auxcallback,
			   (XtPointer)&arg);
	break;
      }
    }

    /* $B=*$o$j$+$I$&$+%A%'%C%/$9$k(B */
    if (ks->info & KanjiModeInfo) {
      wchar modeinfo[4];

      wcKanjiControl((int)obj, KC_SETMODEINFOSTYLE, (char *)1);
      wcKanjiControl((int)obj, KC_QUERYMODE, (char *)modeinfo);
      wcKanjiControl((int)obj, KC_SETMODEINFOSTYLE, (char *)0);
      if (modeinfo[0] - '@' == CANNA_MODE_AlphaMode) {
	toJapaneseMode(obj);
	convend(obj);
      }
    }
    openSelection(obj, SELECTION_DO, 0/* dummy */);
}

static ICString *
GetMode(w)
Widget w;
{
    CannaObject obj = (CannaObject)w;
    wchar *mode;
    static ICString icstr;

    icstr.nchars = obj->canna.ibuf->modelen;
    if (icstr.nchars > 0) {
      mode = obj->canna.ibuf->curmode;
      icstr.data = (char *)mode;
      icstr.nbytes = icstr.nchars * sizeof(wchar);
      icstr.attr = ICAttrNormalString;
    }
    return &icstr;
}

static int
CursorPos(w, nsegp, ncharp)
Widget w;
Cardinal *nsegp;
Cardinal *ncharp;
{
    CannaObject obj = (CannaObject)w;
    iBuf *ib = obj->canna.ibuf;
    Cardinal nseg, nchar;
    int ret = 0;

    if (ib->curseg < ib->nseg) {
      nseg = ib->curseg;
      nchar = 0;
    }
    else {
      nseg = ib->offset;
      nchar = ib->len[0 + ib->offset];
      ret = 1;
    }
    if (nsegp) {
      *nsegp = nseg;
    }
    if (ncharp) {
      *ncharp = nchar;
    }

    return ret;
}

static int
NumSegments(w)
Widget w;
{
    CannaObject obj = (CannaObject)w;
    iBuf *ib = obj->canna.ibuf;

    return ib->nseg;
}

static ICString *
GetSegment(w, n)
Widget w;
Cardinal n;
{
    CannaObject obj = (CannaObject)w;
    iBuf *ib = obj->canna.ibuf;
    static ICString seg;

    seg.data = (char *)ib->str[n];
    seg.nchars = ib->len[n];
    seg.nbytes = seg.nchars * sizeof(wchar);
    if (ib->curseg == n) {
      seg.attr = ICAttrConverted | ICAttrCurrentSegment;
    }
    else if (n < ib->offset) {
      seg.attr = ICAttrConverted;
    }
    else {
      seg.attr = ICAttrNotConverted;
    }

    return &seg;
}

/* ARGSUSED */
static int
CompareSegment(w, seg1, seg2, n)
Widget w;
ICString *seg1;
ICString *seg2;
Cardinal *n;
{
    wchar *p, *q;
    int len, nsame;
    int result = 0;

    if (seg1->attr != seg2->attr) result |= ICAttrChanged;

    len = seg1->nchars > seg2->nchars ? seg2->nchars : seg1->nchars;
    nsame = 0;
    p = (wchar *)seg1->data;
    q = (wchar *)seg2->data;
    while (nsame < len && *p++ == *q++) nsame++;

    if (nsame != len || len != seg1->nchars || len != seg2->nchars)
	result |= ICStringChanged;

    if (n) *n = nsame;

    return result;
}

static ICString *
GetItemList(w, n)
Widget w;
Cardinal *n;
{
    CannaObject obj = (CannaObject)w;

    *n = obj->canna.numcand;
    return obj->canna.candlist;
}

#define SELECTBUFSIZE 1024

static int
SelectItem(w, n)
Widget w;
int n;
{
  CannaObject obj = (CannaObject)w;
  wcKanjiStatus ks;
  wcKanjiStatusWithValue ksv;
  wchar buf[SELECTBUFSIZE];

  if (n >= 0)
    *(obj->canna.cur_addr) = n;

  ksv.ks = &ks;
  buf[0] = (wchar)'\r';
  ksv.buffer = buf;
  ksv.n_buffer = SELECTBUFSIZE;
  ksv.val = CANNA_FN_Kakutei;
  {
    ignoreListfunc = 1;
    wcKanjiControl((int)obj, KC_DO, (char *)&ksv);
    ignoreListfunc = 0;
  }
  displayPreEdit(obj, ksv.val, buf, ksv.ks);
  return 0;
}

static int
ConvertedString(w, encoding, format, length, string)
Widget w;
Atom *encoding;
int *format;
int *length;
XtPointer *string;
{
    CannaObject obj = (CannaObject)w;
    iBuf *ib = obj->canna.ibuf;
    wchar *wbuf, *wp;
    int len, wlen;
    extern int convJWStoCT();
    int offset = ib->offset;

    if (ib == NULL) return -1;

    if (ib->nseg == 0 || ib->offset == 0) return -1;

    wlen = ib->len[offset - 1];
    if (wlen == 0) return -1;

    wbuf = ib->str[offset - 1];

    /*
     * Canna $B%*%V%8%'%/%H$O(B COMPOUND_TEXT $B%(%s%3!<%G%#%s%0$7$+%5%]!<%H$7$J$$(B
     * COMPOUND_TEXT $B$KJQ49$9$k(B
     */
    *encoding = XA_COMPOUND_TEXT(XtDisplayOfObject((Widget)obj));
    *format = 8;

    /* COMPOUND_TEXT $B$O(B \r $B$,Aw$l$J$$$N$G(B \n $B$KJQ49$7$F$*$/(B */
    for (wp = wbuf; *wp != 0; wp++) {
	if (*wp == '\r') *wp = '\n';
    }

    *length = len = convJWStoCT(wbuf, (unsigned char *)NULL, 0);
    *string = XtMalloc(len + 1);
    (void)convJWStoCT(wbuf, (unsigned char *)*string, 0);

    shiftLeftAll(ib);

    return 0;
}

static int
ClearConversion(w)
Widget w;
{
  CannaObject obj = (CannaObject)w;

  wcKanjiStatus ks;
  wcKanjiStatusWithValue ksv;
  wchar buf[SELECTBUFSIZE];

  ksv.ks = &ks;
  ksv.buffer = buf;
  ksv.n_buffer = SELECTBUFSIZE;
  wcKanjiControl((int)obj, KC_KILL, (char *)&ksv);
  displayPreEdit(obj, ksv.val, buf, ksv.ks);
  return 0;
}

static ICString *
GetAuxSegments(w, n, ns, nc)
Widget w;
Cardinal *n, *ns, *nc;
{
  CannaObject obj = (CannaObject)w;
  iBuf *ib = obj->canna.ibuf;
  Cardinal nseg, nchar;

  if (n) {
    *n = ib->ngseg;
  }

  if (ib->curgseg < ib->ngseg) {
    nseg = ib->curgseg;
    nchar = 0;
  }
  else {
    nseg = 0;
    nchar = ib->gllen[0];
  }
  if (ns) {
    *ns = nseg;
  }
  if (nc) {
    *nc = nchar;
  }

  return ib->ics;
}

/* ARGSUSED */
static void
Initialize(req, new, args, num_args)
Widget req;
Widget new;
ArgList args;
Cardinal *num_args;
{
    CannaObject obj = (CannaObject)new;

    obj->canna.selectionending = False;
    obj->canna.textchanged = False;
    obj->canna.symbollist = SymbolList;
    obj->canna.numsymbols = NumSymbols;
    obj->canna.cursymbol = 0;
    obj->canna.candlist = NULL;
    obj->canna.candlistsize = 0;
    obj->canna.numcand = 0;
    obj->canna.lastTextLengthIsZero = False;
      
    ibufInitialize(obj);

    /* $BJQ49$N=i4|2=(B */
    initializeCannaConnection(obj);

    addObject(obj);
}

static int
bell()
{
  if (displaybell) {
    XBell(displaybell, 0);
  }
  return 0;
}

static int nCannaContexts = 0;

static
initializeCannaConnection(obj)
CannaObject obj;
{
  char **warn = 0;
  extern (*jrBeepFunc)();

  if (nCannaContexts == 0) {
#ifdef KC_SETSERVERNAME
    if (obj->canna.cannahost != NULL) {
      wcKanjiControl((int)obj, KC_SETSERVERNAME, obj->canna.cannahost);
    }
#endif /* KC_SETSERVERNAME */
#ifdef KC_SETINITFILENAME
    if (obj->canna.cannafile != NULL) {
      wcKanjiControl((int)obj, KC_SETINITFILENAME, obj->canna.cannafile);
    }
#endif /* KC_SETINITFILENAME */
  
    wcKanjiControl((int)obj, KC_INITIALIZE, (char *)&warn);
    if (warn) {
      char **p;
    
      for (p = warn; *p ; p++) {
	XtAppWarning(XtWidgetToApplicationContext((Widget)obj), *p);
      }
    }
  }
  nCannaContexts++;

#ifdef KC_SETAPPNAME
  wcKanjiControl((int)obj, KC_SETAPPNAME, "kinput2");
#endif

#ifdef DONT_USE_HANKAKU_KATAKANA
  /* $BH>3Q%+%?%+%J$N6X;_(B */
  wcKanjiControl((int)obj, KC_INHIBITHANKAKUKANA, (char *)1);
#endif
  
  /* $B0lMw9T$r#7#8%3%i%`$K@_Dj(B */
  wcKanjiControl((int)obj, KC_SETWIDTH, (char *)78);

  /* $B8uJd0lMw%3!<%k%P%C%/$r@_Dj(B */

#ifdef KC_SETLISTCALLBACK
  {
    jrListCallbackStruct lcs;

    lcs.client_data = (char *)obj;
    lcs.callback_func = listfunc;
    wcKanjiControl((int)obj, KC_SETLISTCALLBACK, (char *)&lcs);
  }
#endif /* KC_SETLISTCALLBACK */

  jrBeepFunc = bell;

  /* $B$5$i$KF|K\8lF~NO%b!<%I$K$7$F$*$/(B */
  toJapaneseMode(obj);
}


static void
Destroy(w)
Widget w;
{
    CannaObject obj = (CannaObject)w;
    wcKanjiStatusWithValue ksv;
    wcKanjiStatus ks;
    wchar buf[512];
    int i;
    
    /* $B%P%C%U%!$N2rJ|(B */

    freeIBuf(obj->canna.ibuf);

    if (obj->canna.candlist) {
      for (i = 0 ; i < obj->canna.candlistsize ; i++) {
	if ((obj->canna.candlist + i)->data) {
	  XtFree((obj->canna.candlist + i)->data);
	}
      }

      XtFree((char *)obj->canna.candlist);
    }

    /* $B!X$+$s$J!Y$N=hM}(B */
    ksv.buffer = buf;
    ksv.n_buffer = 512;
    ksv.ks = &ks;

    wcKanjiControl((int)obj, KC_CLOSEUICONTEXT, (char *)&ksv);

    if (--nCannaContexts == 0) {
      char **warn = 0;

      wcKanjiControl(0, KC_FINALIZE, (char *)&warn);
      if (warn) {
	char **p;
	
	for (p = warn; *p ; p++) {
	  XtAppWarning(XtWidgetToApplicationContext((Widget)obj), *p);
	}
      }
    }

    deleteObject(obj);
}

static Boolean
SetValues(cur, req, wid, args, num_args)
Widget cur;
Widget req;
Widget wid;
ArgList args;
Cardinal *num_args;
/* ARGSUSED */
{
    CannaObject old = (CannaObject)cur;
    CannaObject new = (CannaObject)wid;

    return False;	     
}

static void
fix(obj)
CannaObject obj;
{
    /* $B3NDj$N=hM}(B */
    XtCallCallbackList((Widget)obj, obj->inputConv.fixcallback,
		       (XtPointer)NULL);	/* $B!)!)!)(B */
}

static void
convend(obj)
CannaObject obj;
{
    XtCallCallbackList((Widget)obj, obj->inputConv.endcallback,
		       (XtPointer)NULL);
}

/*
 * keeping list of objects
 */
typedef struct _oblist_ {
    CannaObject obj;
    struct _oblist_ *next;
} ObjRec;

static ObjRec *ObjList = NULL;

static void
addObject(obj)
CannaObject obj;
{
    ObjRec *objp = XtNew(ObjRec);

    objp->obj = obj;
    objp->next = ObjList;
    ObjList = objp;
}

static void
deleteObject(obj)
CannaObject obj;
{
    ObjRec *objp, *objp0;

    for (objp0 = NULL, objp = ObjList;
	 objp != NULL;
	 objp0 = objp, objp = objp->next) {
	if (objp->obj == obj) {
	    if (objp0 == NULL) {
		ObjList = objp->next;
	    } else {
		objp0->next = objp->next;
	    }
	    XtFree((char *)objp);
	    return;
	}
    }
}

/*
 * Operations to canna.ibuf
 */

/* cfuncdef

   changeTextForCanna -- ibuf $B$NFbMF$r(B kanji_status $B$rMQ$$$F=q$-49$($k!#(B


   $B4pK\E*$K$O8uJdJ8;zNs$H8uJd0lMw9TJ8;zNs$K4X$7$F0J2<$N=hM}$r9T$&!#(B

   (1) $B$$$:$l$NJ8;zNs$b%F%-%9%H$,H?E>$7$F$$$kItJ,$H$=$&$G$J$$ItJ,(B
       $B$,B8:_$7!"H?E>$7$F$$$kItJ,$O#1%+=j$7$+B8:_$7$J$$!#(B
   (2) $B$7$?$,$C$F$$$:$l$NJ8;zNs$bH?E>$7$F$$$k$H$3$m$H$=$NN>C<$KH?E>(B
       $B$7$F$$$J$$ItJ,$H$,B8:_$9$k>l9g$K#3ItJ,$KJ,$+$l$k$3$H$K$J$j!"(B
       $BH?E>$7$F$$$kItJ,$N0LCV$d!"H?E>$7$F$$$k2U=j$,$J$$>l9g$J$I$r(B
       $B9g$o$;$F9M$($F$b#3ItJ,0J>e$KJ,$+$l$k$3$H$O$J$$!#(B
   (3) $B$7$?$,$C$F!"$$$:$l$NJ8;zNs$b:GBg#3$D$N%;%0%a%s%H$KJ,$1$FI=<((B
       $B%&%#%8%'%C%H$KEO$9$h$&$K$9$k!#(B

 */

static
changeTextForCanna(cldata, ksp)
caddr_t cldata;
wcKanjiStatus *ksp;
{
    CannaObject obj = (CannaObject)cldata;
    iBuf *ibuf = obj->canna.ibuf;
    int offset = ibuf->offset, i;

    if (ksp->length == 0) {
      ibuf->curseg = offset;
      ibuf->nseg = offset;
      ibuf->len[0 + offset] = ibuf->len[1 + offset] =
	ibuf->len[2 + offset] = 0;
      if (!obj->canna.lastTextLengthIsZero) {
	obj->canna.textchanged = True;
	obj->canna.lastTextLengthIsZero = True;
      }
    }
    else if (ksp->length > 0) {
      obj->canna.lastTextLengthIsZero = False;
      ibuf->len[1 + offset] = ibuf->len[2 + offset] = 0;
      if (ksp->revLen > 0) {
	if (ksp->revPos == 0) {
	  int remain = ksp->length - ksp->revLen;

	  copyInWchar(ksp->echoStr, ksp->revLen,
		      &(ibuf->str[0 + offset]), &(ibuf->size[0 + offset]),
		      &(ibuf->len[0 + offset]));
	  ibuf->curseg = offset;
	  ibuf->nseg = offset + 1;
	  if (remain) {
	    copyInWchar(ksp->echoStr + ksp->revLen, remain,
			&(ibuf->str[1 + offset]), &(ibuf->size[1 + offset]),
			&(ibuf->len[1 + offset]));
	    ibuf->nseg = offset + 2;
	  }
	}
	else {
	  int remain = ksp->length - ksp->revPos - ksp->revLen;

	  copyInWchar(ksp->echoStr, ksp->revPos,
		      &(ibuf->str[0 + offset]), &(ibuf->size[0 + offset]),
		      &(ibuf->len[0 + offset]));
	  copyInWchar(ksp->echoStr + ksp->revPos, ksp->revLen,
		      &(ibuf->str[1 + offset]), &(ibuf->size[1 + offset]),
		      &(ibuf->len[1 + offset]));
	  ibuf->curseg = offset + 1;
	  ibuf->nseg = offset + 2;
	  if (remain) {
	    copyInWchar(ksp->echoStr + ksp->revPos + ksp->revLen,
			remain,
			&(ibuf->str[2 + offset]), &(ibuf->size[2 + offset]),
			&(ibuf->len[2 + offset]));
	  ibuf->nseg = offset + 3;
	  }
	}
      }
      else {
	copyInWchar(ksp->echoStr, ksp->length,
		    &(ibuf->str[0 + offset]), &(ibuf->size[0 + offset]),
		    &(ibuf->len[0 + offset]));
	ibuf->len[1 + offset] = ibuf->len[2 + offset] = 0;
	ibuf->nseg = offset + 1;
	ibuf->curseg = offset + 1;
      }
      obj->canna.textchanged = True;
    }
    /* $B%b!<%I(B */
    if (ksp->info & KanjiModeInfo) {
      int modelen = 0;

      while (ksp->mode[modelen]) modelen++;
      copyInWchar(ksp->mode, modelen,
		  &(ibuf->curmode), &(ibuf->modesize), &(ibuf->modelen));
    }
    /* $B0lMw9T(B */
    if (ksp->info & KanjiGLineInfo) {
      if (ksp->gline.length == 0) {
	switch (ibuf->candstat) {
	case CANNA_GLINE_Empty:
	case CANNA_GLINE_End:
	  ibuf->candstat = CANNA_GLINE_Empty;
	  break;
	case CANNA_GLINE_Start:
	case CANNA_GLINE_Change:
	  ibuf->candstat = CANNA_GLINE_End;
	  break;
	}
	ibuf->curgseg = 0;
	ibuf->ngseg = 0;
	ibuf->gllen[0] = ibuf->gllen[1] = ibuf->gllen[2] = 0;
      }
      else if (ksp->gline.length > 0) {
	switch (ibuf->candstat) {
	case CANNA_GLINE_Empty:
	case CANNA_GLINE_End:
	  ibuf->candstat = CANNA_GLINE_Start;
	  break;
	case CANNA_GLINE_Start:
	case CANNA_GLINE_Change:
	  ibuf->candstat = CANNA_GLINE_Change;
	  break;
	}
	ibuf->gllen[1] = ibuf->gllen[2] = 0;
	if (ksp->gline.revLen > 0) {
	  if (ksp->gline.revPos == 0) {
	    int remain = ksp->gline.length - ksp->gline.revLen;
	    
	    copyInWchar(ksp->gline.line, ksp->gline.revLen,
			&(ibuf->gline[0]), &(ibuf->glsize[0]),
			&(ibuf->gllen[0]));
	    ibuf->curgseg = 0;
	    ibuf->ngseg = 1;
	    if (remain) {
	      copyInWchar(ksp->gline.line + ksp->gline.revLen, remain,
			  &(ibuf->gline[1]), &(ibuf->glsize[1]),
			  &(ibuf->gllen[1]));
	      ibuf->ngseg = 2;
	    }
	  }
	  else {
	    int remain = ksp->gline.length
	      - ksp->gline.revPos - ksp->gline.revLen;
	    
	    copyInWchar(ksp->gline.line, ksp->gline.revPos,
			&(ibuf->gline[0]), &(ibuf->glsize[0]),
			&(ibuf->gllen[0]));
	    copyInWchar(ksp->gline.line + ksp->gline.revPos, ksp->gline.revLen,
			&(ibuf->gline[1]), &(ibuf->glsize[1]),
			&(ibuf->gllen[1]));
	    ibuf->curgseg = 1;
	    ibuf->ngseg = 2;
	    if (remain) {
	      copyInWchar(ksp->gline.line
			  + ksp->gline.revPos + ksp->gline.revLen,
			  remain,
			  &(ibuf->gline[2]), &(ibuf->glsize[2]),
			  &(ibuf->gllen[2]));
	      ibuf->ngseg = 3;
	    }
	  }
	}
	else {
	  copyInWchar(ksp->gline.line, ksp->gline.length,
		      &(ibuf->gline[0]), &(ibuf->glsize[0]),
		      &(ibuf->gllen[0]));
	  ibuf->gllen[1] = ibuf->gllen[2] = 0;
	  ibuf->ngseg = 1;
	  ibuf->curgseg = 1;
	}
      }
      for (i = 0 ; i < ibuf->ngseg ; i++) {
	ibuf->ics[i].data = (char *)ibuf->gline[i];
	ibuf->ics[i].nchars = ibuf->gllen[i];
	ibuf->ics[i].nbytes = ibuf->gllen[i] * sizeof(wchar);
	ibuf->ics[i].attr = ICAttrConverted;
      }
      if (ibuf->curgseg < ibuf->ngseg) {
	ibuf->ics[ibuf->curgseg].attr |= ICAttrCurrentSegment;
      }
    }
}

/* cfuncdef

   copyInWchar -- wchar $B$r%3%T!<$9$k!#(B

   ws, wlen $B$G<($5$l$?(B wchar $BJ8;zNs$r(B wsbuf $B$N%]%$%s%H@h$N%P%C%U%!$K3J(B
   $BG<$9$k!#(Bwsbuf $B$N%5%$%:$O(B wssize $B$N%]%$%s%H@h$K3JG<$5$l$F$$$kCM$G;X(B
   $BDj$5$l$k$,!"$=$l$G$O>.$5$$;~$O(B copyInWchar $BFb$G(B XtRealloc $B$5$l!"?7(B
   $B$?$K%"%m%1!<%H$5$l$?%P%C%U%!$,(B wsbuf $B$N%]%$%s%H@h$K3JG<$5$l$k!#$^$?!"(B
   $B%P%C%U%!$N?7$?$J%5%$%:$,(B wssize $B$N%]%$%s%H@h$K3JG<$5$l$k!#F@$i$l$?(B
   $BJ8;z?t$,(Bwslen $B$N%]%$%s%H@h$K3JG<$5$l$k!#(B

 */

static
copyInWchar(ws, wlen, wsbuf, wssize, wslen)
wchar *ws;
int wlen;
wchar **wsbuf;
int *wssize, *wslen;
{
  int i;

  if (*wssize == 0) {
    if ((*wsbuf = (wchar *)XtMalloc((wlen + 1) * sizeof(wchar)))
	 == (wchar *)0) {
      /* $B%(%i!<$@(B */
    }
    else {
      *wssize = wlen + 1;
    }
  }
  if (wlen + 1 > *wssize) {
    if ((*wsbuf = (wchar *)XtRealloc((char *)*wsbuf,
				     (wlen + 1) * sizeof(wchar)))
	 == (wchar *)0) {
      /* $B%(%i!<$@!#$I$&$9$l$PNI$$!)(B */
    }
    else {
      *wssize = wlen + 1;
    }
  }
  *wslen = wlen;
  (void)bcopy(ws, *wsbuf, wlen * sizeof(wchar));
  *(*wsbuf + wlen) = (wchar)0;

  return 0; /* $B#0$OFC$K0UL#$,L5$$(B */
}

/* cfuncdef

   fixProcForCanna -- $B3NDj$7$?J8;z$N=hM}$r9T$&(B

 */

static
fixProcForCanna(cldata, fixedstr, fixedlen)
caddr_t cldata;
wchar *fixedstr;
int fixedlen;
{
  CannaObject obj = (CannaObject)cldata;
  iBuf *ib = obj->canna.ibuf;
  int offset = ib->offset;
  
  if (offset < NConvertedSegments) {
    shiftRight(ib);
    offset = ib->offset;
  }
  else {
    shiftLeft(ib);
  }
  copyInWchar(fixedstr, fixedlen,
	      &(ib->str[offset - 1]), &(ib->size[offset - 1]),
	      &(ib->len[offset - 1]));
}

/* cfuncdef

   shiftRight -- ibuf $B$NJ8;z%P%C%U%!$N(B offset $B0J9_$r1&$K%7%U%H$9$k(B

 */

static
shiftRight(ib)
iBuf *ib;
{
  int i;
  wchar *tmpbuf;
  int tmpsize, tmplen;
  int offset = ib->offset;

  tmpbuf = ib->str[ib->nseg];
  tmpsize = ib->size[ib->nseg];
  for (i = ib->nseg ; offset < i ; i--) {
    ib->str[i] = ib->str[i - 1];
    ib->size[i] = ib->size[i - 1];
    ib->len[i] = ib->len[i - 1];
  }
  ib->str[offset] = tmpbuf;
  ib->size[offset] = tmpsize;
  ib->offset++;
  ib->nseg++;
  ib->curseg++;
}

/* cfuncdef

   shiftLeft -- ibuf $B$NJ8;z%P%C%U%!$N(B offset $B0JA0$r:8$K%7%U%H$9$k(B

 */

static
shiftLeft(ib)
iBuf *ib;
{
  int i;
  wchar *tmpbuf;
  int tmpsize, tmplen;
  int offset = ib->offset;

  tmpbuf = ib->str[0];
  tmpsize = ib->size[0];
  for (i = 0 ; i < offset - 1 ; i++) {
    ib->str[i] = ib->str[i + 1];
    ib->size[i] = ib->size[i + 1];
    ib->len[i] = ib->len[i + 1];
  }
  ib->str[offset - 1] = tmpbuf;
  ib->size[offset - 1] = tmpsize;
}

/* cfuncdef

   shiftLeftAll -- ibuf $B$NJ8;z%P%C%U%!A4BN$r:8$K%7%U%H$9$k(B

 */

static
shiftLeftAll(ib)
iBuf *ib;
{
  int i;
  wchar *tmpbuf;
  int tmpsize, tmplen;
  int nseg = ib->nseg;

  tmpbuf = ib->str[0];
  tmpsize = ib->size[0];
  for (i = 0 ; i < nseg - 1 ; i++) {
    ib->str[i] = ib->str[i + 1];
    ib->size[i] = ib->size[i + 1];
    ib->len[i] = ib->len[i + 1];
  }
  ib->str[nseg - 1] = tmpbuf;
  ib->size[nseg - 1] = tmpsize;
  ib->nseg--;
  ib->curseg--;
  ib->offset--;
}

/* cfuncdef

   ibufInitialize -- ibuf $B$N=i4|2==hM}(B
 */

static
ibufInitialize(obj)
CannaObject obj;
{
  int i;
  iBuf *ib;

  ib = obj->canna.ibuf = (iBuf *)XtMalloc(sizeof(iBuf));
  if (ib == 0) {
    /* $B%(%i!<$@!#$I$&$9$Y(B */
  }
  for (i = 0 ; i < NConvertedSegments + 3 ; i++) {
    ib->size[i] = 0;
    ib->len[i] = 0;
  }
  ib->offset = ib->curseg = ib->nseg = 0;
  ib->candstat = CANNA_GLINE_Empty;
  for (i = 0 ; i < 3 ; i++) {
    ib->gline[i] = 0;
    ib->glsize[i] = ib->gllen[i] = 0;
  }
  ib->modesize = 0;
  ib->modelen = 0;
}

/* cfuncdef

   freeIBuf -- ibuf $B$N(B free

 */

static
freeIBuf(ib)
iBuf *ib;
{
  int i;

  if (ib == 0) {
    return 0;
  }
  for (i = 0 ; i < NConvertedSegments + 3 ; i++) {
    if (ib->size[i] > 0) {
      XtFree((char *)ib->str[i]);
    }
  }
  for (i = 0 ; i < 3 ; i++) {
    if (ib->glsize[i] > 0) {
      XtFree((char *)ib->gline[i]);
    }
  }
  if (ib->modesize > 0) {
    XtFree((char *)ib->curmode);
  }
  free(ib);
  return 0;
}

static
toJapaneseMode(obj)
CannaObject obj;
{
  wcKanjiStatusWithValue ksv;
  wcKanjiStatus ks;
  wchar buf[1024];
  
  ksv.ks = &ks;
  ksv.buffer = buf;
  ksv.n_buffer = 1024;
  ksv.val = CANNA_MODE_HenkanMode;
  wcKanjiControl((int)obj, KC_CHANGEMODE, (char *)&ksv);
  changeTextForCanna(obj, &ks);
}

/* checkIfFunctionalChar -- $B%7%U%H%-!<$N$h$&$KL50UL#$J%-!<$+$I$&$+$NH=JL(B

  $BCM(B:		$B#1(B	$B0UL#$,$"$k(B
  		$B#0(B	$BL50UL#(B($B%b%G%#%U%!%$%"%-!<$J$I(B)
*/

static int
checkIfFunctionalChar(event_struct, keysym, buffer_return, n_buffer)
XKeyEvent *event_struct;
KeySym keysym;
wchar *buffer_return;
int n_buffer;
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

#ifdef KC_SETLISTCALLBACK

static void
moveSelection(obj, dir)
CannaObject obj;
int dir;
{
    ICSelectionControlArg arg;

    arg.command = ICSelectionMove;
    arg.u.dir = dir;
    XtCallCallbackList((Widget)obj, obj->inputConv.selectioncallback,
		       (XtPointer)&arg);
}

static int
insertSelection(obj, selected)
CannaObject obj;
int selected;
{
    obj->canna.curcand = selected;
    *(obj->canna.cur_addr) = selected;

    return 0;
}

static int
endSelection(obj, abort)
CannaObject obj;
int abort;
{
    ICSelectionControlArg arg;
    int selected;
    int ret = 0;

    if (ignoreListfunc) return; /* SelectItem $B$G=hM}$5$l$k$N$G$3$3$OITMW(B */
    arg.command = ICSelectionEnd;
    arg.u.current_item = -1;
    XtCallCallbackList((Widget)obj, obj->inputConv.selectioncallback,
		       (XtPointer)&arg);

    if (!abort && (selected = arg.u.current_item) >= 0) {
	ret = insertSelection(obj, selected);
    }

    return ret;
}

static void
openSelection(obj, func, curitem)
CannaObject obj;
int func; /* $B0lHV>e$r8+$h(B */
int curitem;
{
  ICSelectionControlArg arg;
  static int current = 0;
  static int doit = 0;

  if (func == SELECTION_SET) {
    current = curitem;
    doit = 1;
  }
  else if (func == SELECTION_DO && doit) {
    doit = 0;
    arg.command = ICSelectionStart;
    arg.u.selection_kind = ICSelectionCandidates;
    XtCallCallbackList((Widget)obj, obj->inputConv.selectioncallback,
		       (XtPointer)&arg);

    /* set current item */
    arg.command = ICSelectionSet;
    arg.u.current_item = current;
    XtCallCallbackList((Widget)obj, obj->inputConv.selectioncallback,
		       (XtPointer)&arg);
  }
}

static void getAllCandidates();

/* listfunc -- $B8uJd0lMw%b!<%I$K$J$C$?;~$K8F$S=P$5$l$k4X?t(B
 *
 * obj      : KC_SETLISTCALLBACK $B$G@_Dj$7$?%/%i%$%"%s%H%G!<%?(B
 * func     : $B8uJd0lMw5!G=$N8GM-$N5!G=$rI=$9#I#D(B
 * items    : Start $B$N;~$K$O8uJd0lMw$NA4%"%$%F%`$,F~$k(B
 * nitems   :      $B!7(B       $BA4%"%$%F%`$N?t$,F~$k(B
 * cur_item :      $B!7(B       $B%+%l%s%H%"%$%F%`HV9f$r3JG<$9$k%"%I%l%9$,F~$k(B
 */

static void
listfunc(obj, func, items, nitems, cur_item)
CannaObject obj;
int  func;
wchar **items;
int nitems, *cur_item;
{
  ICSelectionControlArg arg;
  int i;
  wchar **p;

  switch (func) {
  case CANNA_LIST_Start:
    getAllCandidates(obj, nitems, items);
    obj->canna.numcand = nitems;
    obj->canna.curcand = *cur_item;
    obj->canna.cur_addr = cur_item;
    openSelection(obj, SELECTION_SET, *cur_item);
    break;
  case CANNA_LIST_Select:
    endSelection(obj, False);
    break;
  case CANNA_LIST_Quit:
    endSelection(obj, True);
    break;
  case CANNA_LIST_Forward:
    moveSelection(obj, ICMoveRight);
    break;
  case CANNA_LIST_Backward:
    moveSelection(obj, ICMoveLeft);
    break;
  case CANNA_LIST_Next:
    moveSelection(obj, ICMoveDown);
    break;
  case CANNA_LIST_Prev:
    moveSelection(obj, ICMoveUp);
    break;
  case CANNA_LIST_BeginningOfLine:
    moveSelection(obj, ICMoveLeftMost);
    break;
  case CANNA_LIST_EndOfLine:
    moveSelection(obj, ICMoveRightMost);
    break;
  }
}

static void
allocCandlist(obj, n)
CannaObject obj;
int n;
{
    ICString *p;
    int i;

    if (n <= obj->canna.candlistsize) return;

    if (obj->canna.candlistsize == 0) {
	p = (ICString *)XtMalloc(n * sizeof(ICString));
    } else {
	p = (ICString *)XtRealloc((char *)obj->canna.candlist,
				  n * sizeof(ICString));
    }
    for (i = obj->canna.candlistsize ; i < n ; i++) {
      p[i].nbytes = p[i].nchars = (unsigned short)0;
      p[i].data = (char *)0;
      p[i].attr = 0;
    }

    obj->canna.candlist = p;
    obj->canna.candlistsize = n;
}

static void
getAllCandidates(obj, ncand, items)
CannaObject obj;
int ncand;
wchar **items;
{
    ICString *strp;
    wchar **p;
    int i, bytes, chars;

    allocCandlist(obj, ncand);

    for (i = 0, strp = obj->canna.candlist, p = items ;
	 i < ncand; i++, strp++, p++) {
        int len = 0;

        bytes = strp->nbytes;
	chars = strp->nchars;
	while ((*p)[len]) len++;
        copyInWchar(*p, len, (wchar **)&(strp->data), &bytes, &chars);
	strp->nbytes = bytes;
	strp->nchars = chars;
	strp->attr = ICAttrNormalString;
    }
}
#endif /* KC_SETLISTCALLBACK */
