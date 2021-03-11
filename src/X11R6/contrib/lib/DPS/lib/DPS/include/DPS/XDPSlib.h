/*
 * XDPSlib.h - client header file for DPS library.  This file describes the
 * Xlib interface to the Display Postscript extension.  It should be used only
 * by that interface and by code that actually uses this interface (e.g., the
 * client library interface).
 *
 * Copyright (C) 1988-1994 by Adobe Systems Incorporated and Digital Equipment
 * 			      Corporation
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notices appear in all copies and
 * that both those copyright notices and this permission notice appear
 * in supporting documentation, and that the names of Adobe Systems 
 * Incorporated and Digital Equipment Corporation not be used in 
 * advertising or publicity pertaining to distribution of the software 
 * without specific, written prior permission.  Adobe Systems Incorporated
 * and Digital Equipment Corporation make no representations about the 
 * suitability of this software for any purpose.  It is provided "as is" 
 * without express or implied warranty.
 * 
 * ADOBE SYSTEMS INCORPORATED AND DIGITAL EQUIPMENT CORPORATION DISCLAIM
 * ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL ADOBE
 * SYSTEMS INCORPORATED AND DIGITAL EQUIPMENT CORPORATION BE LIABLE FOR
 * ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES 
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN 
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * PostScript, Display PostScript, and Adobe are trademarks of Adobe Systems
 * Incorporated registered in the U.S.A. and other countries.
 *
 * Author: Adobe Systems Incorporated and Digital Equipment Corporation
 */
 
#ifndef _XDPSlib_h
#define _XDPSlib_h

#ifndef _NO_PROTO
#undef _XEVENT_
#endif

#include <X11/Xlib.h>
#include "DPS/XDPS.h"
#include <X11/Xutil.h>

/*
 * Default standard colormap type.
 */

#define DefaultStdCMap	((XStandardColormap *) -1)


/*
 * event masks defined for convenience
 */

#define XDPSL_ALL_EVENTS \
 (PSRUNNINGMASK|PSNEEDSINPUTMASK|PSZOMBIEMASK|PSFROZENMASK)
 
typedef enum {
  csdps_not,
  csdps_noop,
  csdps_output,
  csdps_output_with_len,
  csdps_status,
  csdps_ready                             /* L2-DPS/PROTO 9 addition */
} CSDPSFakeEventTypes;

typedef enum {
  not_pserror,
  pserror_badcontext,
  pserror_badspace,
  pserror_abort
} XDPSLPSErrors;

#define DPSCAP_SYNCMASK_NONE		0x00
#define DPSCAP_SYNCMASK_SYNC		(1<<0)
#define DPSCAP_SYNCMASK_RECONCILE	(1<<1)
#define DPSCAP_SYNCMASK_BUFFER		(1<<2)

#define DPSCAP_SYNCMASK_DFLT	DPSCAP_SYNCMASK_RECONCILE

#ifdef _NO_PROTO

typedef void (*XDPSLEventHandler)();

extern int XDPSLInit();
extern ContextXID XDPSLCreateContextAndSpace();
extern SpaceXID XDPSLCreateSpace();
extern void XDPSLGiveInput();
extern int XDPSLGetStatus();
extern void XDPSLDestroySpace();
extern void XDPSLReset();
extern void XDPSLNotifyContext();
extern ContextXID XDPSLCreateContextFromID();
extern Status XDPSLIDFromContext();
extern ContextPSID XDPSLContextFromXID();
extern void XDPSLSetStatusMask();
extern XExtCodes *XDPSLGetCodes();
extern void XDPSLSetTextEventHandler();
extern void XDPSLCallOutputEventHandler();
extern void XDPSLSetStatusEventHandler();
extern void XDPSLCallStatusEventHandler();
extern void XDPSLInitDisplayFlags();
extern Bool XDPSLGetPassEventsFlag();
extern void XDPSLSetPassEventsFlag();
extern Bool XDPSLGetWrapWaitingFlag();
extern void XDPSLSetWrapWaitingFlag();
/* L2-DPS/PROTO 9 additions */
extern void XDPSLNotifyWhenReady();
extern XDPSLPSErrors XDPSLTestErrorCode();
extern int XDPSLGetVersion();
extern void XDPSLSetReadyEventHandler();
extern void XDPSLCallReadyEventHandler();
/* DPS NX additions */
extern char * XDPSLSetAgentName();
extern void XDPSLSetCodes();
extern Display * XDPSLGetShunt();
extern void XDPSLSetShunt();
extern void XDPSLSetVersion();
extern void XDPSLSetClientMessageHandler();
extern CSDPSFakeEventTypes XDPSLGetCSDPSFakeEventType();
extern Bool XDPSLDispatchCSDPSFakeEvent();
extern void XDPSLGetCSDPSStatus();
extern void XDPSLSetXErrorConvertProc();
extern void XDPSLCAPNotify();
extern void XDPSLSync();
extern void *XDPSLEnumerateEvents();
extern void XDPSLSetAfterProc();
extern int XDPSLGetSyncMask();
extern void XDPSLSetSyncMask();
extern void XDPSLFlush();
extern void XDPSLGetCSDPSReady();
extern void XDPSLReconcileRequests();
extern Status XDPSLSetAgentArg();
extern void XDPSLCleanAll();
extern void XDPSLUpdateAgentArgs();
extern void XDPSLCleanContext();
#ifdef VMS
extern void XDPSLSetDisplay();
#endif /* VMS */
/* DPS NX 2.0 */
extern void XDPSLFlushGC();
extern void XDPSLSetGCFlushMode();
extern int XDPSLGetGCFlushMode();

#else /* _NO_PROTO */

typedef void (*XDPSLEventHandler)(XEvent *event);

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/*
 * XDPSLInit returns the first event number for the extension.
 */

extern int XDPSLInit(Display *dpy,
		     int *numberType_return,
		     char **floatingName_return);

extern ContextXID XDPSLCreateContext(Display *dpy,
				     SpaceXID sxid,
				     Drawable drawable,
				     GC gc,
				     int x, int y,
				     unsigned int eventMask,
				     XStandardColormap *grayRamp,
				     XStandardColormap *colorCube,
				     unsigned int actual,
				     ContextPSID *cpsid_return,
				     Bool secure); /* L2-DPS/PROTO 9 */

extern ContextXID XDPSLCreateContextAndSpace(Display *dpy,
					     Drawable drawable,
					     GC gc,
					     int x, int y,
					     unsigned int eventMask,
					     XStandardColormap *grayRamp,
					     XStandardColormap *colorCube,
					     unsigned int actual,
					     ContextPSID *cpsid_return,
					     SpaceXID *sxid_return,
					     Bool secure); /* L2-DPS/PROTO 9 */

extern SpaceXID XDPSLCreateSpace(Display *dpy);

extern void XDPSLGiveInput(Display *dpy, ContextXID cxid,
			   char *data, int length);

extern int XDPSLGetStatus(Display *dpy, ContextXID cxid);

extern void XDPSLDestroySpace(Display *dpy, SpaceXID sxid);

extern void XDPSLReset(Display *dpy, ContextXID cxid);

extern void XDPSLNotifyContext(Display *dpy, ContextXID cxid, int type);

extern ContextXID XDPSLCreateContextFromID(Display *dpy,
					   ContextPSID cpsid,
					   SpaceXID *sxid_return);

extern Status XDPSLIDFromContext(Display *dpy,
				 ContextPSID cpsid,
				 ContextXID *cxid_return,
				 SpaceXID *sxid_return);

/* Returns 1 on success, 0 on failure (cpsid not a valid context). */

extern ContextPSID XDPSLContextFromXID(Display *dpy, ContextXID cxid);
  
extern void XDPSLSetStatusMask(Display *dpy, ContextXID cxid,
			       unsigned int enableMask,
			       unsigned int disableMask,
			       unsigned int nextMask);
  

extern XExtCodes *XDPSLGetCodes(Display *dpy);

extern void XDPSLSetTextEventHandler(Display *dpy,
				     XDPSLEventHandler proc);

extern void XDPSLCallOutputEventHandler(Display *dpy, XEvent *event);

extern void XDPSLSetStatusEventHandler(Display *dpy,
				       XDPSLEventHandler proc);

extern void XDPSLCallStatusEventHandler(Display *dpy, XEvent *event);

extern void XDPSLInitDisplayFlags(Display *dpy);

extern Bool XDPSLGetPassEventsFlag(Display *dpy);

extern void XDPSLSetPassEventsFlag(Display *dpy, Bool flag);

extern Bool XDPSLGetWrapWaitingFlag(Display *dpy);

extern void XDPSLSetWrapWaitingFlag(Display *dpy, Bool flag);

/* L2-DPS/PROTO 9 additions */

extern void XDPSLNotifyWhenReady(Display *dpy, ContextXID cxid, int val[4]);

extern XDPSLPSErrors XDPSLTestErrorCode(Display *dpy, int ecode);	

extern int XDPSLGetVersion(Display *dpy);

extern void XDPSLSetReadyEventHandler(Display *dpy,
				       XDPSLEventHandler proc);

extern void XDPSLCallReadyEventHandler(Display *dpy, XEvent *event);


/* DPS NX additions */

extern char * XDPSLSetAgentName(Display *dpy, char *name, int deflt);

extern void XDPSLSetCodes(Display *dpy, XExtCodes *codes);

extern Display * XDPSLGetShunt(Display *dpy_in);

extern void XDPSLSetShunt(Display *dpy_in, Display *dpy_out);

extern void XDPSLSetVersion(Display *dpy, unsigned ver);

extern void XDPSLSetClientMessageHandler(Display *dpy);

extern CSDPSFakeEventTypes XDPSLGetCSDPSFakeEventType(Display *dpy,
                                                      XEvent *event);

extern Bool XDPSLDispatchCSDPSFakeEvent(Display *dpy,
                                        XEvent *event, CSDPSFakeEventTypes t);

extern void XDPSLGetCSDPSStatus(Display *dpy,
                                XEvent *event,
				void **ret_ctxt,
				int *ret_status);
				
extern void XDPSLSetXErrorConvertProc(Display *dpy);

extern void XDPSLCAPNotify(Display *xdpy,
                           ContextXID cxid,
                           unsigned int ntype,
                           unsigned int data,
                           unsigned int extra);

extern void XDPSLSync(Display *xdpy);

extern void *XDPSLEnumerateEvents(Display *dpy,
                                 XEvent *event,
                                 Bool (*predicate)(Display *pdpy,
                                                   XEvent *ev,
                                                   char *a),
                                 char *arg,
                                 void *next);
                                 

extern void XDPSLSetAfterProc(Display *dpy);

extern int XDPSLGetSyncMask(Display *dpy);

extern void XDPSLSetSyncMask(Display *dpy, int mask);

extern void XDPSLFlush(Display *dpy);

extern void XDPSLGetCSDPSReady(Display *xdpy,
                                 XEvent *event,
                                 void **ret_ctxt,
                                 int *ret_val);

extern void XDPSLReconcileRequests(Display *xdpy, ContextXID cxid);

extern Status XDPSLSetAgentArg(Display *xdpy, int arg, int val);

extern void XDPSLCleanAll(Display *xdpy);

extern void XDPSLUpdateAgentArgs(Display *xdpy);

extern void XDPSLCleanContext(Display *xdpy, ContextXID cxid);

/* L2-DPS/PROTO 9 additions for DPS NX */
extern void XDPSLGetCSDPSReady(Display *dpy,
                                XEvent *event,
				void **ret_ctxt,
				int *ret_val);


#ifdef VMS
extern void XDPSLSetDisplay(Display *dpy);
#endif /* VMS */

/* DPS NX 2.0 */
extern void XDPSLFlushGC(Display *dpy, GC gc);

extern void XDPSLSetGCFlushMode(Display *dpy, int value);

extern int XDPSLGetGCFlushMode(Display *dpy);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* _NO_PROTO */

typedef struct _XDPSLOutputEvent {
    int type;			/* of event. */
    unsigned long serial;       /* # of last request processed by server. */
    Bool send_event;		/* True if this came from a SendEvent req. */
    Display *display;		/* Display the event was read from. */
    ContextXID cxid;		/* Context from which data comes. */
    int length;			/* Number of characters of data. */
    char data[24];		/* Data received. */
} XDPSLOutputEvent;


typedef struct _XDPSLStatusEvent {
    int type;			/* of event. */
    unsigned long serial;       /* # of last request processed by server. */
    Bool send_event;		/* True if this came from a SendEvent req. */
    Display *display;		/* Display the event was read from. */
    ContextXID cxid;		/* Context whose status we're reporting. */
    int status;			/* Status of context. */
} XDPSLStatusEvent;

typedef struct _XDPSLReadyEvent {
    int type;			/* of event. */
    unsigned long serial;       /* # of last request processed by server. */
    Bool send_event;		/* True if this came from a SendEvent req. */
    Display *display;		/* Display the event was read from. */
    ContextXID cxid;		/* Context whose status we're reporting. */
    int val[4];		        /* Array of client defined integer values */
} XDPSLReadyEvent;

#endif /* _XDPSlib_h */
