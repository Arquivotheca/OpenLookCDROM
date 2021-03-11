/*****************************************************************************/
/**       Copyright 1988 by Evans & Sutherland Computer Corporation,        **/
/**                          Salt Lake City, Utah                           **/
/**  Portions Copyright 1989 by the Massachusetts Institute of Technology   **/
/**                        Cambridge, Massachusetts                         **/
/**                                                                         **/
/**                           All Rights Reserved                           **/
/**                                                                         **/
/**    Permission to use, copy, modify, and distribute this software and    **/
/**    its documentation  for  any  purpose  and  without  fee is hereby    **/
/**    granted, provided that the above copyright notice appear  in  all    **/
/**    copies and that both  that  copyright  notice  and  this  permis-    **/
/**    sion  notice appear in supporting  documentation,  and  that  the    **/
/**    names of Evans & Sutherland and M.I.T. not be used in advertising    **/
/**    in publicity pertaining to distribution of the  software  without    **/
/**    specific, written prior permission.                                  **/
/**                                                                         **/
/**    EVANS & SUTHERLAND AND M.I.T. DISCLAIM ALL WARRANTIES WITH REGARD    **/
/**    TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES  OF  MERCHANT-    **/
/**    ABILITY  AND  FITNESS,  IN  NO  EVENT SHALL EVANS & SUTHERLAND OR    **/
/**    M.I.T. BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL  DAM-    **/
/**    AGES OR  ANY DAMAGES WHATSOEVER  RESULTING FROM LOSS OF USE, DATA    **/
/**    OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER    **/
/**    TORTIOUS ACTION, ARISING OUT OF OR IN  CONNECTION  WITH  THE  USE    **/
/**    OR PERFORMANCE OF THIS SOFTWARE.                                     **/
/*****************************************************************************/
/* 
 *  [ ctwm ]
 *
 *  Copyright 1992 Claude Lecommandeur.
 *            
 * Permission to use, copy, modify  and distribute this software  [ctwm] and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above  copyright notice appear  in all copies and that both that
 * copyright notice and this permission notice appear in supporting documen-
 * tation, and that the name of  Claude Lecommandeur not be used in adverti-
 * sing or  publicity  pertaining to  distribution of  the software  without
 * specific, written prior permission. Claude Lecommandeur make no represen-
 * tations  about the suitability  of this software  for any purpose.  It is
 * provided "as is" without express or implied warranty.
 *
 * Claude Lecommandeur DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL  IMPLIED WARRANTIES OF  MERCHANTABILITY AND FITNESS.  IN NO
 * EVENT SHALL  Claude Lecommandeur  BE LIABLE FOR ANY SPECIAL,  INDIRECT OR
 * CONSEQUENTIAL  DAMAGES OR ANY  DAMAGES WHATSOEVER  RESULTING FROM LOSS OF
 * USE, DATA  OR PROFITS,  WHETHER IN AN ACTION  OF CONTRACT,  NEGLIGENCE OR
 * OTHER  TORTIOUS ACTION,  ARISING OUT OF OR IN  CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Claude Lecommandeur [ lecom@sic.epfl.ch ][ April 1992 ]
 */


/**********************************************************************
 *
 * $XConsortium: parse.h,v 1.14 89/12/14 14:51:25 jim Exp $
 *
 * .twmrc parsing externs
 *
 *  8-Apr-88 Tom LaStrange        Initial Version.
 *
 **********************************************************************/

#ifndef _PARSE_
#define _PARSE_

extern int ParseTwmrc(), ParseStringList();
extern int (*twmInputFunc)();
extern void twmUnput();
extern void TwmOutput();

#define F_NOP			0
#define F_BEEP			1
#define F_RESTART		2
#define F_QUIT			3
#define F_FOCUS			4
#define F_REFRESH		5
#define F_WINREFRESH		6
#define F_DELTASTOP		7
#define F_MOVE			8
#define F_POPUP			9
#define F_FORCEMOVE		10
#define F_AUTORAISE		11
#define F_IDENTIFY		12
#define F_ICONIFY		13
#define F_DEICONIFY		14
#define F_UNFOCUS		15
#define F_RESIZE		16
#define F_ZOOM			17
#define F_LEFTZOOM		18
#define F_RIGHTZOOM		19
#define F_TOPZOOM		20
#define F_BOTTOMZOOM		21
#define F_HORIZOOM		22
#define F_FULLZOOM		23
#define F_RAISE			24
#define F_RAISELOWER		25
#define F_LOWER			26
#define F_DESTROY		27
#define F_DELETE		28
#define F_SAVEYOURSELF		29
#define F_VERSION		30
#define F_TITLE			31
#define F_RIGHTICONMGR		32
#define F_LEFTICONMGR		33
#define F_UPICONMGR		34
#define F_DOWNICONMGR		35
#define F_FORWICONMGR		36
#define F_BACKICONMGR		37
#define F_NEXTICONMGR		38
#define F_PREVICONMGR		39
#define F_SORTICONMGR		40
#define F_CIRCLEUP		41
#define F_CIRCLEDOWN		42
#define F_CUTFILE		43
#define F_SHOWLIST		44
#define F_HIDELIST		45
#define F_OCCUPY		46
#define F_OCCUPYALL		47
#define F_SHOWWORKMGR		48
#define F_HIDEWORKMGR		49
#define F_SETBUTTONSTATE	50
#define F_SETMAPSTATE		51
#define F_TOGGLESTATE		52
#define F_PIN			53
#define F_MOVEMENU		54
#define F_VANISH		55
#define F_NEXTWORKSPACE		56
#define F_PREVWORKSPACE		57
#define F_SEPARATOR		58
#define F_ADOPTWINDOW		59
#define F_STARTANIMATION	60
#define F_STOPANIMATION		61
#define F_SPEEDUPANIMATION	62
#define F_SLOWDOWNANIMATION	63
#ifdef SOUNDS
#define F_TOGGLESOUND		64
#define F_REREADSOUNDS		65
#endif
#define F_TESTFUNC		66
#define F_WINWARP		67

#define F_MENU			101	/* string */
#define F_WARPTO		102	/* string */
#define F_WARPTOICONMGR		103	/* string */
#define F_WARPRING		104	/* string */
#define F_FILE			105	/* string */
#define F_EXEC			106	/* string */
#define F_CUT			107	/* string */
#define F_FUNCTION		108	/* string */
#define F_WARPTOSCREEN		109	/* string */
#define F_COLORMAP		110	/* string */
#define F_GOTOWORKSPACE		111	/* string */
#define F_WARPHERE		112	/* string */

#define D_NORTH			1
#define D_SOUTH			2
#define D_EAST			3
#define D_WEST			4

#endif /* _PARSE_ */
