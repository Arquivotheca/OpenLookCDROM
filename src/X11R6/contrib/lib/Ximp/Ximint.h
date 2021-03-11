/* $XimpImplementGroup: Ximint.h, v 1.1 94/05/31 21:16:03 $ */
/* $XConsortium: Ximint.h,v 1.7 94/05/14 15:38:50 rws Exp $ */
/******************************************************************

                Copyright 1992, 1993, 1994 by FUJITSU LIMITED
                Copyright 1993, 1994 by Sony Corporation

Permission to use, copy, modify, distribute, and sell this software
and its documentation for any purpose is hereby granted without fee,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the name of FUJITSU LIMITED and
Sony Corporation not be used in advertising or publicity pertaining to
distribution of the software without specific, written prior
permission.  FUJITSU LIMITED and Sony Corporation makes no
representations about the suitability of this software for any
purpose.  It is provided "as is" without express or implied warranty. 

FUJITSU LIMITED AND SONY CORPORATION DISCLAIM ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL FUJITSU LIMITED AND
SONY CORPORATION BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFTWARE. 

  Author:   Takashi Fujiwara   FUJITSU LIMITED 
                               fujiwara@a80.tech.yk.fujitsu.co.jp
  Motifier: Makoto Wakamatsu   Sony Corporation
			       makoto@sm.sony.co.jp

******************************************************************/
/*

Copyright (c) 1992 - 1994  FUJITSU LIMITED
Copyright (c) 1993 - 1994  Sony Corporation

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE FUJITSU LIMITED AND SONY CORPORATION BE LIABLE FOR
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the FUJITSU LIMITED and
Sony Corporation shall not be used in advertising or otherwise to promote
the sale, use or other dealings in this Software without prior written
authorization from the FUJITSU LIMITED and Sony Corporation.

  Author :  Takashi Fujiwara   FUJITSU LIMITED 
            Makoto Wakamatsu   Sony Corporation

*/

#ifndef _XIMINT_H
#define _XIMINT_H

#include <stdio.h>
#include <X11/Xutil.h>

#define Public /**/
#define Private static

typedef struct _Xim	*Xim;
typedef struct _Xic	*Xic;

/*
 * Input Method data
 */
#include "XimintL.h"
#include "XimintP.h"

/*
 * XIM dependent data
 */
typedef union _XIMPrivateRec {
    XimLocalPrivateRec   local;
    XimProtoPrivateRec   proto;
} XIMPrivateRec;

/*
 * IM struct
 */
typedef struct _Xim {
    XIMMethods		methods;
    XIMCoreRec		core;
    XIMPrivateRec	private;
} XimRec;

/*
 * IC deprndent data
 */
typedef union _XICPrivateRec {
    XicLocalPrivateRec   local;
    XicProtoPrivateRec   proto;
} XICPrivateRec;

/*
 * IC struct
 */
typedef struct _Xic {
	XICMethods	methods;
	XICCoreRec	core;
	XICPrivateRec	private;
} XicRec;

typedef struct _XimDefIMValues {
	XIMValuesList		*im_values_list;
	XIMValuesList		*ic_values_list;
	XIMStyles		*styles;
	XIMCallback		 destroy_callback;
	char			*res_name;
	char			*res_class;
	Bool			 visible_position;
} XimDefIMValues;

typedef struct _XimDefICValues {
    XIMStyle			 input_style;
    Window			 client_window;
    Window			 focus_window;
    unsigned long		 filter_events;
    XIMCallback			 geometry_callback;
    char			*res_name;
    char			*res_class;
    XIMCallback			 destroy_callback;
    XIMCallback			 preedit_state_notify_callback;
    XIMCallback			 string_conversion_callback;
    XIMStringConversionText	 string_conversion;
    XIMResetState		 reset_state;
    XIMHotKeyTriggers		*hotkey;
    XIMHotKeyState		 hotkey_state;
    ICPreeditAttributes		 preedit_attr;
    ICStatusAttributes		 status_attr;
} XimDefICValues;

#define XIM_MODE_IM_GET		(1 << 0)
#define XIM_MODE_IM_SET		(1 << 1)
#define XIM_MODE_IM_DEFAULT	(1 << 2)

#define XIM_MODE_PRE_GET	(1 << 0)
#define XIM_MODE_PRE_SET	(1 << 1)
#define XIM_MODE_PRE_CREATE	(1 << 2)
#define XIM_MODE_PRE_ONCE	(1 << 3)
#define XIM_MODE_PRE_DEFAULT	(1 << 4)

#define XIM_MODE_STS_GET	(1 << 5)
#define XIM_MODE_STS_SET	(1 << 6)
#define XIM_MODE_STS_CREATE	(1 << 7)
#define XIM_MODE_STS_ONCE	(1 << 8)
#define XIM_MODE_STS_DEFAULT	(1 << 9)

#define XIM_MODE_IC_GET		(XIM_MODE_PRE_GET      | XIM_MODE_STS_GET)
#define XIM_MODE_IC_SET		(XIM_MODE_PRE_SET      | XIM_MODE_STS_SET)
#define XIM_MODE_IC_CREATE	(XIM_MODE_PRE_CREATE   | XIM_MODE_STS_CREATE)
#define XIM_MODE_IC_ONCE	(XIM_MODE_PRE_ONCE     | XIM_MODE_STS_ONCE)
#define XIM_MODE_IC_DEFAULT	(XIM_MODE_PRE_DEFAULT  | XIM_MODE_STS_DEFAULT)

#define XIM_MODE_PRE_MASK	(XIM_MODE_PRE_GET    | XIM_MODE_PRE_SET    | \
				 XIM_MODE_PRE_CREATE | XIM_MODE_PRE_ONCE   | \
				 XIM_MODE_PRE_DEFAULT)
#define XIM_MODE_STS_MASK	(XIM_MODE_STS_GET    | XIM_MODE_STS_SET    | \
				 XIM_MODE_STS_CREATE | XIM_MODE_STS_ONCE   | \
				 XIM_MODE_STS_DEFAULT)

#define XIM_SETIMDEFAULTS	(1L << 0)
#define XIM_SETIMVALUES		(1L << 1)
#define XIM_GETIMVALUES		(1L << 2)

#define XIM_SETICDEFAULTS	(1L << 0)
#define XIM_CREATEIC		(1L << 1)
#define XIM_SETICVALUES		(1L << 2)
#define XIM_GETICVALUES		(1L << 3)
#define XIM_PREEDIT_ATTR	(1L << 4)
#define XIM_STATUS_ATTR		(1L << 5)

#define XIM_CHECK_VALID		0
#define XIM_CHECK_INVALID	1
#define XIM_CHECK_ERROR		2

#define FILTERD         True
#define NOTFILTERD      False

#define XIMMODIFIER		"@im="

#define	XIM_TRUE	True
#define	XIM_FALSE	False
#define	XIM_OVERFLOW	(-1)

/*
 * Global symbols
 */

extern Bool _XimSetIMResourceList(
#if NeedFunctionPrototypes
    XIMResourceList	*res_list,
    unsigned int	*list_num
#endif
);

extern Bool _XimSetICResourceList(
#if NeedFunctionPrototypes
    XIMResourceList	*res_list,
    unsigned int	*list_num
#endif
);

extern Bool _XimSetInnerIMResourceList(
#if NeedFunctionPrototypes
    XIMResourceList	*res_list,
    unsigned int	*list_num
#endif
);

extern Bool _XimSetInnerICResourceList(
#if NeedFunctionPrototypes
    XIMResourceList	*res_list,
    unsigned int	*list_num
#endif
);

extern Bool cw_XimCheckCreateICValues(
#if NeedFunctionPrototypes
    XIMResourceList	 res_list,
    unsigned int	 list_num
#endif
);

extern XIMResourceList _XimGetResourceListRec(
#if NeedFunctionPrototypes
    XIMResourceList	 res_list,
    unsigned int	 list_num,
    char		*name
#endif
);

extern void _XimSetIMMode(
#if NeedFunctionPrototypes
    XIMResourceList	 res_list,
    unsigned int	 list_num
#endif
);

extern void _XimSetICMode(
#if NeedFunctionPrototypes
    XIMResourceList	 res_list,
    unsigned int	 list_num,
    XIMStyle		 style
#endif
);

extern int _XimCheckIMMode(
#if NeedFunctionPrototypes
    XIMResourceList	 res_list,
    unsigned long	 mode
#endif
);

extern int _XimCheckICMode(
#if NeedFunctionPrototypes
    XIMResourceList	 res_list,
    unsigned long	 mode
#endif
);

extern Bool _XimSetLocalIMDefaults(
#if NeedFunctionPrototypes
    Xim			 im,
    XPointer		 top,
    XIMResourceList	 res_list,
    unsigned int	 list_num
#endif
);

extern Bool _XimSetICDefaults(
#if NeedFunctionPrototypes
    Xic			 ic,
    XPointer		 top,
    unsigned long	 mode,
    XIMResourceList	 res_list,
    unsigned int	 list_num
#endif
);

extern Bool _XimEncodeLocalIMAttr(
#if NeedFunctionPrototypes
    XIMResourceList	 res,
    XPointer		 top,
    XPointer		 val
#endif
);

extern Bool _XimEncodeLocalICAttr(
#if NeedFunctionPrototypes
    Xic			 ic,
    XIMResourceList	 res,
    XPointer		 top,
    XIMArg		*arg,
    unsigned long	 mode
#endif
);

extern Bool _XimCheckLocalInputStyle(
#if NeedFunctionPrototypes
    Xic			 ic,
    XPointer		 top,
    XIMArg		*values,
    XIMStyles           *styles,
    XIMResourceList	 res_list,
    unsigned int	 list_num
#endif
);

extern Bool _XimDecodeLocalIMAttr(
#if NeedFunctionPrototypes
    XIMResourceList	 res,
    XPointer		 top,
    XPointer		 val
#endif
);

extern Bool _XimDecodeLocalICAttr(
#if NeedFunctionPrototypes
    XIMResourceList	 res,
    XPointer		 top,
    XPointer		 val,
    unsigned long	mode
#endif
);

extern void _XimGetCurrentIMValues(
#if NeedFunctionPrototypes
    Xim			 im,
    XimDefIMValues	*im_values
#endif
);

extern void _XimSetCurrentIMValues(
#if NeedFunctionPrototypes
    Xim			 im,
    XimDefIMValues	*im_values
#endif
);

extern void _XimGetCurrentICValues(
#if NeedFunctionPrototypes
    Xic			 ic,
    XimDefICValues	*ic_values
#endif
);

extern void _XimSetCurrentICValues(
#if NeedFunctionPrototypes
    Xic			 ic,
    XimDefICValues	*ic_values
#endif
);

extern void _XimInitialResourceInfo(
#if NeedFunctionPrototypes
    void
#endif
);

extern int	 XimParseStringFile(
#if NeedFunctionPrototypes
    FILE        *fp,
    DefTree     **ptop
#endif
);

extern Bool	 _XimCheckIfLocalProcessing(
#if NeedFunctionPrototypes
    Xim		 im
#endif
);

extern Bool	 _XimCheckIfThaiProcessing(
#if NeedFunctionPrototypes
    Xim		 im
#endif
);

extern Bool	 _XimLocalOpenIM(
#if NeedFunctionPrototypes
    Xim		 im
#endif
);

extern Bool	 _XimThaiOpenIM(
#if NeedFunctionPrototypes
    Xim		 im
#endif
);

extern Bool	 _XimProtoOpenIM(
#if NeedFunctionPrototypes
    Xim		 im
#endif
);

extern void	 _XimLocalIMFree(
#if NeedFunctionPrototypes
    Xim		 im
#endif
);

extern void	 _XimThaiIMFree(
#if NeedFunctionPrototypes
    Xim	         im
#endif
);

extern void	 _XimProtoIMFree(
#if NeedFunctionPrototypes
    Xim		 im
#endif
);

extern char *	 _XimSetIMValueData(
#if NeedFunctionPrototypes
    Xim			 im,
    XPointer		 top,
    XIMArg		*arg,
    XIMResourceList	 res_list,
    unsigned int	 list_num
#endif
);

extern char *	 _XimGetIMValueData(
#if NeedFunctionPrototypes
    Xim			 im,
    XPointer		 top,
    XIMArg		*arg,
    XIMResourceList	 res_list,
    unsigned int	 list_num
#endif
);

extern char *	 _XimSetICValueData(
#if NeedFunctionPrototypes
    Xic			 ic,
    XPointer		 top,
    XIMResourceList	 res_list,
    unsigned int	 list_num,
    XIMArg		*arg,
    unsigned long	 mode,
    Bool		 flag
#endif
);

extern char *	 _XimGetICValueData(
#if NeedFunctionPrototypes
    Xic			 ic,
    XPointer		 top,
    XIMResourceList	 res_list,
    unsigned int	 list_num,
    XIMArg		*arg,
    unsigned long	 mode
#endif
);

extern char *	 _XimLocalSetIMValues(
#if NeedFunctionPrototypes
    XIM		 im,
    XIMArg	*arg
#endif
);

extern char *	 _XimLocalGetIMValues(
#if NeedFunctionPrototypes
    XIM		 im,
    XIMArg	*arg
#endif
);

extern XIC	 _XimLocalCreateIC(
#if NeedFunctionPrototypes
    XIM		 im,
    XIMArg	*arg
#endif
);

extern Bool	_XimDispatchInit(
#if NeedFunctionPrototypes
    Xim		 im
#endif
);

extern Bool	 _XimGetAttributeID(
#if NeedFunctionPrototypes
    Xim		 im,
    CARD16	*buf
#endif
);

extern Bool	 _XimExtension(
#if NeedFunctionPrototypes
    Xim		 im
#endif
);

extern void	_XimDestroyIMStructureList(
#if NeedFunctionPrototypes
    Xim		 im
#endif
);

extern char *	_XimMakeIMAttrIDList(
#if NeedFunctionPrototypes
    Xim			 im,
    XIMResourceList	 res_list,
    unsigned int	 res_num,
    XIMArg		*arg,
    CARD16		*buf,
    INT16		*len,
    unsigned long        mode
#endif
);

extern char *	_XimMakeICAttrIDList(
#if NeedFunctionPrototypes
    Xic                  ic,
    XIMResourceList	 res_list,
    unsigned int	 res_num,
    XIMArg		*arg,
    CARD16		*idList,
    INT16		*num,
    unsigned long	 mode
#endif
);

extern char *	_XimDecodeIMATTRIBUTE(
#if NeedFunctionPrototypes
    Xim			 im,
    XIMResourceList	 res_list,
    unsigned int	 res_num,
    CARD16		*buf,
    INT16		 buf_len,
    XIMArg		*arg,
    BITMASK32		 mode
#endif
);

extern char *	_XimDecodeICATTRIBUTE(
#if NeedFunctionPrototypes
    Xic			 ic,
    XIMResourceList	 res_list,
    unsigned int	 res_num,
    CARD16		*buf,
    INT16		 buf_len,
    XIMArg		*arg,
    BITMASK32		 mode
#endif
);

extern Bool	_XimRegProtoIntrCallback(
#if NeedFunctionPrototypes
    Xim		 im,
    CARD16	 major_code,
    CARD16	 minor_code,
    Bool	 (*proc)(
#if NeedNestedPrototypes
			Xim, INT16, XPointer, XPointer
#endif
			),
    XPointer	 call_data
#endif
);

extern Bool	_XimErrorCallback(
#if NeedFunctionPrototypes
    Xim		 im,
    INT16	 len,
    XPointer	 data,
    XPointer	 call_data
#endif
);

extern Bool	_XimRegisterTriggerKeysCallback(
#if NeedFunctionPrototypes
    Xim		 im,
    INT16	 len,
    XPointer	 data,
    XPointer	 call_data
#endif
);

extern Bool	_XimSetEventMaskCallback(
#if NeedFunctionPrototypes
    Xim		 im,
    INT16	 len,
    XPointer	 data,
    XPointer	 call_data
#endif
);

extern Bool	_XimForwardEventCallback(
#if NeedFunctionPrototypes
    Xim		 im,
    INT16	 len,
    XPointer	 data,
    XPointer	 call_data
#endif
);

extern Bool	_XimCommitCallback(
#if NeedFunctionPrototypes
    Xim		 im,
    INT16	 len,
    XPointer	 data,
    XPointer	 call_data
#endif
);

extern Bool	_XimSyncCallback(
#if NeedFunctionPrototypes
    Xim		 im,
    INT16	 len,
    XPointer	 data,
    XPointer	 call_data
#endif
);

extern void	_XimFreeProtoIntrCallback(
#if NeedFunctionPrototypes
    Xim		 im
#endif
);

extern XIC	 _XimProtoCreateIC(
#if NeedFunctionPrototypes
    XIM		 im,
    XIMArg	*arg
#endif
);

extern void	_XimRegisterServerFilter(
#if NeedFunctionPrototypes
    Xim		 im
#endif
);

extern void	_XimUnregisterServerFilter(
#if NeedFunctionPrototypes
    Xim		 im
#endif
);

extern Bool	_XimTriggerNotify(
#if NeedFunctionPrototypes
    Xim		 im,
    Xic		 ic,
    int		 mode,
    CARD32	 idx
#endif
);

extern Bool	_XimProcSyncReply(
#if NeedFunctionPrototypes
    Xim		 im,
    Xic		 ic
#endif
);

extern void	_XimSendSetFocus(
#if NeedFunctionPrototypes
    Xim		 im,
    Xic		 ic
#endif
);

extern Bool	_XimForwardEvent(
#if NeedFunctionPrototypes
    Xic		 ic,
    XEvent	*ev,
    Bool	 sync
#endif
);

extern void	_XimFreeRemakeArg(
#if NeedFunctionPrototypes
    XIMArg	*arg
#endif
);

extern void	_XimServerDestroy(
#if NeedFunctionPrototypes
    void
#endif
);

extern char *	_XimEncodeIMATTRIBUTE(
#if NeedFunctionPrototypes
    Xim			  im,
    XIMResourceList	  res_list,
    unsigned int	  res_num,
    XIMArg               *arg, 
    XIMArg		**arg_ret,
    char		 *buf,
    int			  size,
    int			 *ret_len,
    XPointer		  top,
    unsigned long	  mode
#endif
);

extern char *	_XimEncodeICATTRIBUTE(
#if NeedFunctionPrototypes
    Xic			  ic,
    XIMResourceList	  res_list,
    unsigned int	  res_num,
    XIMArg               *arg, 
    XIMArg		**arg_ret,
    char		 *buf,
    int			  size,
    int			 *ret_len,
    XPointer		  top,
    BITMASK32		 *flag,
    unsigned long	  mode
#endif
);

#ifdef EXT_MOVE
extern Bool	_XimExtenMove(
#if NeedFunctionPrototypes
    Xim		 im,
    Xic		 ic,
    CARD32	 flag,
    CARD16	*buf,
    INT16	 length
#endif
);
#endif

extern int	_Ximctstombs(
#if NeedFunctionPrototypes
    Xim		 im,
    char	*from,
    int		from_len,
    char	*to,
    int		to_len,
    Status	*state
#endif
);

extern int	_Ximctstowcs(
#if NeedFunctionPrototypes
    Xim		 im,
    char	*from,
    int		 from_len,
    wchar_t	*to,
    int		 to_len,
    Status	*state
#endif
);

extern void	_XimSetHeader(
#if NeedFunctionPrototypes
    XPointer	 buf,
    CARD8	 major_opcode,
    CARD8	 minor_opcode,
    INT16	*len
#endif
);

extern Bool	_XimSync(
#if NeedFunctionPrototypes
    Xim		 im,
    Xic		 ic
#endif
);

extern int	_XimProtoMbLookupString(
#if NeedFunctionPrototypes
    XIC		 xic,
    XKeyEvent	*ev,
    char	*buffer,
    int		 bytes,
    KeySym	*keysym,
    Status	*state
#endif
);

extern int	_XimProtoWcLookupString(
#if NeedFunctionPrototypes
    XIC		 xic,
    XKeyEvent	*ev,
    wchar_t	*buffer,
    int		 bytes,
    KeySym	*keysym,
    Status	*state
#endif
);

extern void	_XimRegisterFilter(
#if NeedFunctionPrototypes
    Xic		 ic
#endif
);

extern void	_XimUnregisterFilter(
#if NeedFunctionPrototypes
    Xic		 ic
#endif
);

extern void	_XimReregisterFilter(
#if NeedFunctionPrototypes
    Xic		 ic
#endif
);

extern Status	_XimProtoEventToWire(
#if NeedFunctionPrototypes
    XEvent	*re,
    xEvent	*event,
    Bool	sw
#endif
);

extern Bool	_XimProtoWireToEvent(
#if NeedFunctionPrototypes
    XEvent	*re,
    xEvent	*event,
    Bool	 sw
#endif
);

#ifdef EXT_FORWARD
extern Bool	_XimExtForwardKeyEvent(
#if NeedFunctionPrototypes
    Xic		 ic,
    XKeyEvent	*ev,
    Bool	 sync
#endif
);
#endif

extern int	_XimLookupMBText(
#if NeedFunctionPrototypes
    Xic			 ic,
    XKeyEvent		*event,
    unsigned char	*buffer,
    int			 nbytes,
    KeySym		*keysym,
    XComposeStatus	*status
#endif
);

extern int	_XimLookupWCText(
#if NeedFunctionPrototypes
    Xic			 ic,
    XKeyEvent		*event,
    wchar_t		*buffer,
    int			 nbytes,
    KeySym		*keysym,
    XComposeStatus	*status
#endif
);

extern EVENTMASK	_XimGetWindowEventmask(
#if NeedFunctionPrototypes
    Xic		 ic
#endif
);

extern Xic	_XimICOfXICID(
#if NeedFunctionPrototypes
    Xim		im,
    XICID	icid
#endif
);

extern void	_XimResetIMInstantiateCallback(
#if NeedFunctionPrototypes
    Xim         xim
#endif
);

extern Bool	_XimRegisterIMInstantiateCallback(
#if NeedFunctionPrototypes
    XLCd	 lcd,
    Display	*display,
    XrmDatabase	 rdb,
    char	*res_name,
    char	*res_class,
    XIMProc	 callback,
    XPointer	*client_data
#endif
);

extern Bool	_XimUnRegisterIMInstantiateCallback(
#if NeedFunctionPrototypes
    XLCd	 lcd,
    Display	*display,
    XrmDatabase	 rdb,
    char	*res_name,
    char	*res_class,
    XIMProc	 callback,
    XPointer	*client_data
#endif
);

extern void	_XimFreeCommitInfo(
#if NeedFunctionPrototypes
    Xic		 ic
#endif
);

extern Bool	_XimConnect(
#if NeedFunctionPrototypes
    Xim		 im
#endif
);

extern Bool	_XimShutdown(
#if NeedFunctionPrototypes
    Xim		 im
#endif
);

extern Bool	_XimWrite(
#if NeedFunctionPrototypes
    Xim		 im,
    INT16	 len,
    XPointer	 data
#endif
);

extern Bool	_XimRead(
#if NeedFunctionPrototypes
    Xim		 im,
    INT16	*len,
    XPointer	 data,
    int		 data_len,
    Bool	 (*predicate)(
#if NeedNestedPrototypes
			      Xim, INT16, XPointer, XPointer
#endif
			      ),
    XPointer	 arg
#endif
);

extern Bool	_XimIntrCallback(
#if NeedFunctionPrototypes
    Xim		 im,
    Bool	 (*callback)(
#if NeedNestedPrototypes
			     Xim, INT16, XPointer, XPointer
#endif
			     ),
    XPointer	 call_data
#endif
);

extern void	_XimFlush(
#if NeedFunctionPrototypes
    Xim		 im
#endif
);

extern Bool	_XimFilterWaitEvent(
#if NeedFunctionPrototypes
    Xim		 im
#endif
);

extern void   _XimProcError(
#if NeedFunctionPrototypes
    Xim		im,
    Xic		ic,
    XPointer	data
#endif
);

#ifdef EXT_MOVE
extern CARD32	_XimExtenArgCheck(
#if NeedFunctionProtoTypes
    XIMArg	*arg
#endif
);
#endif

extern Bool _XimCbDispatch(
#if NeedFunctionPrototypes
    Xim im, 
    INT16 len, 
    XPointer data, 
    XPointer call_data
#endif
);

extern void _XimGetResourceName(
#if NeedFunctionPrototypes
    Xim im, 
    char *res_name,
    char *res_class
#endif
);

extern Bool _XimLocalFilter(
#if NeedFunctionPrototypes
    Display		*d,
    Window		 w,
    XEvent		*ev,
    XPointer		 client_data
#endif
);

extern XIMResourceList _XimGetResourceListRecByQuark(
#if NeedFunctionPrototypes
    XIMResourceList	 res_list,
    unsigned int	 list_num,
    XrmQuark		 quark
#endif
);

extern Bool _XimReconnectModeCreateIC(
#if NeedFunctionPrototypes
    Xic			 ic
#endif
);

extern char *_XimLocalSetICValues(
#if NeedFunctionPrototypes
    XIC			 ic,
    XIMArg		*values
#endif
);

extern char * _XimLocalGetICValues(
#if NeedFunctionPrototypes
    XIC			 ic,
    XIMArg		*values
#endif
);

extern int _XimLocalMbLookupString(
#if NeedFunctionPrototypes
    XIC			 ic,
    XKeyEvent		*ev,
    char		*buffer,
    int			 bytes,
    KeySym		*keysym,
    Status		*status
#endif
);

extern int _XimLocalWcLookupString(
#if NeedFunctionPrototypes
    XIC			 ic,
    XKeyEvent		*ev,
    wchar_t		*buffer,
    int			 bytes,
    KeySym		*keysym,
    Status		*status
#endif
);

extern Bool _XimThaiFilter(
#if NeedFunctionPrototypes
    Display		*d,
    Window		 w,
    XEvent		*ev,
    XPointer		 client_data
#endif
);

extern XIC _XimThaiCreateIC(
#if NeedFunctionPrototypes
    XIM			 im,
    XIMArg		*values
#endif
);

extern Status _XimThaiCloseIM(
#if NeedFunctionPrototypes
    XIM			 xim
#endif
);

extern XIM _XimOpenIM(
#if NeedFunctionPrototypes
    XLCd		 lcd,
    Display		*dpy,
    XrmDatabase		 rdb,
    char		*res_name,
    char		*res_class
#endif
);

extern void _XimProtoAllIMFree(
#if NeedFunctionPrototypes
    Xim			 im
#endif
);

#ifdef XIM_CONNECTABLE
extern void _XimSetProtoResource(
#if NeedFunctionPrototypes
    Xim im
#endif
);

extern Bool _XimConnectServer(
#if NeedFunctionPrototypes
    Xim im
#endif
);

extern Bool _XimDelayModeSetAttr(
#if NeedFunctionPrototypes
    Xim			 im
#endif
);

extern void	_XimServerReconectableDestroy(
#if NeedFunctionPrototypes
    void
#endif
);

extern Bool _XimReCreateIC(
#if NeedFunctionPrototypes
    Xic			ic
#endif
);

extern Bool _XimEncodeSavedIMATTRIBUTE(
#if NeedFunctionPrototypes
    Xim			 im,
    XIMResourceList	 res_list,
    unsigned int	 res_num,
    int			*idx,
    char		*buf,
    int			 size,
    int			 ret_len,
    XPointer		 top,
    unsigned long	 mode
#endif
);

extern Bool _XimEncodeSavedICATTRIBUTE(
#if NeedFunctionPrototypes
    Xic			 ic,
    XIMResourceList	 res_list,
    unsigned int	 res_num,
    int			*idx,
    char		*buf,
    int			 size,
    int			*ret_len,
    XPointer		 top,
    unsigned long	 mode
#endif
);

extern XIM _XimDelayOpenIM(
#if NeedFunctionPrototypes
    XLCd		 lcd,
    Display		*dpy,
    XrmDatabase		 rdb,
    char		*res_name,
    char		*res_class
#endif
);

extern Bool _XimProtoDelayOpenIM(
#if NeedFunctionPrototypes
    Xim			 im
#endif
);
#endif

#endif /* _XIMINT_H */
