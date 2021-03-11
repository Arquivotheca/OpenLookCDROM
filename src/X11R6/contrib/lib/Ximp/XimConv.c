/* $XimpImplementGroup: XimConv.c, v 1.1 94/05/31 21:16:00 $ */
/* $XConsortium: imConv.c,v 1.5 94/03/26 16:57:36 rws Exp $ */
/******************************************************************

              Copyright 1991, 1992 by Fuji Xerox Co.,Ltd.
	      Copyright 1993, 1994 by FUJITSU LIMITED

Permission to use, copy, modify, distribute, and sell this software
and its documentation for any purpose is hereby granted without fee,
provided that the above copyright notice appear in all copies and
that both that copyright notice and this permission notice appear
in supporting documentation, and that the name of Fuji Xerox Co.,Ltd.
, and that the name of FUJITSU LIMITED not be used in advertising or
publicity pertaining to distribution of the software without specific,
 written prior permission.
Fuji Xerox Co.,Ltd. , and FUJITSU LIMITED makes no representations about
the suitability of this software for any purpose.
It is provided "as is" without express or implied warranty.

FUJI XEROX CO.,LTD. AND FUJITSU LIMITED DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL FUJI XEROX CO.,LTD.
AND FUJITSU LIMITED BE LIABLE FOR ANY SPECIAL, INDIRECT
OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH
THE USE OR PERFORMANCE OF THIS SOFTWARE.

  Auther:   Kazunori Nishihara, Fuji Xerox Co.,Ltd.
                                kaz@ssdev.ksp.fujixerox.co.jp
  Modifier: Takashi Fujiwara    FUJITSU LIMITED
                                fujiwara@a80.tech.yk.fujitsu.co.jp

******************************************************************/
/*

Copyright (c) 1993 - 1994  FUJITSU LIMITED

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
IN NO EVENT SHALL THE FUJITSU LIMITED BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the FUJITSU LIMITED shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from the FUJITSU LIMITED.

  Auther :  Takashi Fujiwara   FUJITSU LIMITED 

*/

#define NEED_EVENTS
#include <stdio.h>
#include "Xlibint.h"
#include "Xlcint.h"
#include "Ximint.h"

/* maps Cyrillic keysyms to 8859-5 */
static unsigned char cyrillic[128] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x80 - */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x90 - */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xf2, 0xf3, 0xf1, 0xf4, 0xf5, 0xf6, 0xf7, /* 0xa0 - */
    0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0x00, 0xfe, 0xff,
    0xf0, 0xa2, 0xa3, 0xa1, 0xa4, 0xa5, 0xa6, 0xa7, /* 0xb0 - */
    0xa8, 0xa9, 0xaa, 0xab, 0xac, 0x00, 0xae, 0xaf,
    0xee, 0xd0, 0xd1, 0xe6, 0xd4, 0xd5, 0xe4, 0xd3, /* 0xc0 - */
    0xe5, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde,
    0xdf, 0xef, 0xe0, 0xe1, 0xe2, 0xe3, 0xd6, 0xd2, /* 0xd0 - */
    0xec, 0xeb, 0xd7, 0xe8, 0xed, 0xe9, 0xe7, 0xea,
    0xce, 0xb0, 0xb1, 0xc6, 0xb4, 0xb5, 0xc4, 0xb3, /* 0xe0 - */
    0xc5, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe,
    0xbf, 0xcf, 0xc0, 0xc1, 0xc2, 0xc3, 0xb6, 0xb2, /* 0xf0 - */
    0xcc, 0xcb, 0xb7, 0xc8, 0xcd, 0xc9, 0xc7, 0xca
};

/* maps Greek keysyms to 8859-7 */
static unsigned char greek[128] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x80 - */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x90 - */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xb6, 0xb8, 0xb9, 0xba, 0xda, 0x00, 0xbc, /* 0xa0 - */
    0xbe, 0xdb, 0x00, 0xbf, 0x00, 0x00, 0xb5, 0xaf,
    0x00, 0xdc, 0xdd, 0xde, 0xdf, 0xfa, 0xc0, 0xfc, /* 0xb0 - */
    0xfd, 0xfb, 0xe0, 0xfe, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, /* 0xc0 - */
    0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
    0xd0, 0xd1, 0xd3, 0x00, 0xd4, 0xd5, 0xd6, 0xd7, /* 0xd0 - */
    0xd8, 0xd9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, /* 0xe0 - */
    0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
    0xf0, 0xf1, 0xf3, 0xf2, 0xf4, 0xf5, 0xf6, 0xf7, /* 0xf0 - */
    0xf8, 0xf9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static unsigned char get_code(), get_greek(), get_cyril();

struct CodesetRec {
    unsigned long kset;
    char *designator;
    unsigned char (*char_code)();
};

static struct CodesetRec codeset[] = {
    {0x00l, "\033-A", NULL},	  /* ISO 8859-1 (Latin 1) */
    {0x01l, "\033-B", get_code},  /* ISO 8859-2 (Latin 2) */
    {0x02l, "\033-C", get_code},  /* ISO 8859-3 (Latin 3) */
    {0x03l, "\033-D", get_code},  /* ISO 8859-4 (Latin 4) */
    {0x04l, "\033)I", get_code},  /* JIS x0201 (Katakana) */
    {0x05l, "\033-G", get_code},  /* ISO 8859-6 (Arabic) */
    {0x06l, "\033-L", get_cyril}, /* ISO 8859-5 (Cyrillic) */
    {0x07l, "\033-F", get_greek}, /* ISO 8859-7 (Greek) */
    {0x0cl, "\033-H", get_code},  /* ISO 8859-8 (Hebrew) */
};

static codeset_size = sizeof(codeset) / sizeof(codeset[0]);

static unsigned char
get_code(keysym)
KeySym keysym;
{
    return((unsigned char)(keysym & 0xff));
}

static unsigned char
get_cyril(keysym)
KeySym keysym;
{
    return(cyrillic[keysym & 0x7f]);
}

static unsigned char
get_greek(keysym)
KeySym keysym;
{
    return(greek[keysym & 0x7f]);
}

#define BUF_SIZE (20)
static char local_buf[BUF_SIZE] = {0};	/* Clean up bss */
static unsigned char look[BUF_SIZE] = {0};	/* Clean up bss */

#ifndef MAXINT
#define MAXINT          (~((unsigned int)1 << (8 * sizeof(int)) - 1))
#endif /* !MAXINT */

Private int
_XimctsConv(conv, from, from_len, to, to_len)
    XlcConv	 conv;
    char	*from;
    int		 from_len;
    char	*to;
    int		 to_len;
{
    int		 from_left;
    int		 to_left;
    int		 from_savelen;
    int		 to_savelen;
    int		 from_cnvlen;
    int		 to_cnvlen;
    char	*from_buf;
    char	*to_buf;

    if (!conv || !from || !from_len) {
	return 0;
    }

    if (to && to_len) {
	from_left = from_len;
	to_left = to_len - 1;
	from_cnvlen = 0;
	to_cnvlen = 0;
	for (;;) {
	    from_savelen = from_left;
	    to_savelen = to_left;
	    from_buf = &from[from_cnvlen];
	    to_buf = &to[to_cnvlen];
	    if (_XlcConvert(conv, (XPointer *)&from_buf, &from_left,
				 (XPointer *)&to_buf, &to_left, NULL, 0) < 0) {
		return 0;
	    }
	    from_cnvlen += (from_savelen - from_left);
	    to_cnvlen += (to_savelen - to_left);
	    if (from_left == 0) {
		if (to_cnvlen > 0) {
		    to[to_cnvlen] = '\0';
		}
		return to_cnvlen;
	    }
	    if (to_left == 0)
		break;
	}
    }

    from_left = from_len;
    from_cnvlen = 0;
    to_cnvlen = 0;
    to_buf = NULL;
    for (;;) {
	from_savelen = from_left;
	to_left = MAXINT;
	from_buf = &from[from_cnvlen];
	if (_XlcConvert(conv, (XPointer *)&from_buf, &from_left,
				 (XPointer *)&to_buf, &to_left, NULL, 0) < 0) {
	    return 0;
	}
	from_cnvlen += (from_savelen - from_left);
	to_cnvlen += (MAXINT - to_left);
	if (from_left == 0) {
	    break;
	}
    }
    return to_cnvlen;
}

Public int
_XimConvertCharCode(buffer, nbytes, symbol, conv)
    char *buffer;
    int nbytes;
    KeySym symbol;
    XlcConv conv;
{
    unsigned long kset;
    int count, local_count;
    struct CodesetRec *cset = (struct CodesetRec *)NULL;
    register int i;
    unsigned char c;

    count = 0;
    kset = (symbol >> 8) & 0xffffff;
    for (i = 0; i < codeset_size; i++) {
	if (kset == codeset[i].kset) {
	    cset = &codeset[i];
	    break;
	}
    }
    if ((cset) && (c = (*cset->char_code)(symbol))) {
	strcpy(local_buf, cset->designator);
	local_count = strlen(cset->designator);
	local_buf[local_count] = c;
	local_count++;
	local_buf[local_count] = '\0';
	if ((count = _XimctsConv(conv,
		local_buf, local_count, (char *)buffer, nbytes)) < 0) {
	    count = 0;
	}
    }
    return count;
}

int
_XimLookupMBText(ic, event, buffer, nbytes, keysym, status)
    Xic			 ic;
    XKeyEvent		*event;
    unsigned char	*buffer;
    int			 nbytes;
    KeySym		*keysym;
    XComposeStatus	*status;
{
    int count, local_count;
    KeySym symbol;
    unsigned long kset;
    struct CodesetRec *cset = (struct CodesetRec *)NULL;
    int i;
    unsigned char c;
    Status	dummy;
    Xim	im = (Xim)ic->core.im;

    count = im->methods->lookup_string(event, (char *)buffer,
						nbytes, &symbol, status);
    if (keysym) *keysym = symbol;
    if ((nbytes == 0) || (symbol == NoSymbol)) {
	return(count);
    }
    if (count == 0) {
	kset = (symbol >> 8) & 0xffffff;
	for (i = 0; i < codeset_size; i++) {
	    if (kset == codeset[i].kset) {
		cset = &codeset[i];
		break;
	    }
	}
	if ((cset) && (c = (*cset->char_code)(symbol))) {
	    strcpy(local_buf, cset->designator);
	    local_count = strlen(cset->designator);
	    local_buf[local_count] = c;
	    local_count++;
	    local_buf[local_count] = '\0';
	    if ((count = _Ximctstombs((Xim)ic->core.im,
				local_buf, local_count,
				(char *)buffer, nbytes, &dummy)) < 0) {
		count = 0;
	    }
	}
    } else if ((count != 1) || (buffer[0] >= 0x80)) { /* not ASCII Encoding */
	memcpy((char *)look, (char *)buffer,count);
	look[count] = '\0';
	if ((count = _Ximctstombs((Xim)ic->core.im,
				(char *)look, count,
				(char *)buffer, nbytes, &dummy)) < 0) {
	    count = 0;
	}
    }
    return(count);
}

int
_XimLookupWCText(ic, event, buffer, nbytes, keysym, status)
    Xic			 ic;
    XKeyEvent		*event;
    wchar_t		*buffer;
    int			 nbytes;
    KeySym		*keysym;
    XComposeStatus	*status;
{
    int count, local_count;
    KeySym symbol;
    unsigned long kset;
    struct CodesetRec *cset = (struct CodesetRec *)NULL;
    int i;
    unsigned char c;
    Status	dummy;
    Xim	im = (Xim)ic->core.im;

    count = im->methods->lookup_string(event, (char *)look,
						nbytes, &symbol, status);
    if (keysym) *keysym = symbol;
    if ((nbytes == 0) || (symbol == NoSymbol)) {
	return(count);
    }
    if (count == 0) { /* Not ISO 8859-1 Encoding */
	kset = (symbol >> 8) & 0xffffff;
	for (i = 0; i < codeset_size; i++) {
	    if (kset == codeset[i].kset) {
		cset = &codeset[i];
		break;
	    }
	}
	if ((cset) && (c = (*cset->char_code)(symbol))) {
	    strcpy(local_buf, cset->designator);
	    local_count = strlen(cset->designator);
	    local_buf[local_count] = c;
	    local_count++;
	    local_buf[local_count] = '\0';
	    if ((count = _Ximctstowcs((Xim)ic->core.im,
				local_buf, local_count,
				buffer, nbytes, &dummy)) < 0) {
		count = 0;
	    }
	}
    } else if ((count == 1) && (look[0] < 0x80)) { /* ASCII Encoding */
	buffer[0] = look[0];
    } else {
	if ((count = _Ximctstowcs((Xim)ic->core.im,
				(char *)look, count,
				buffer, nbytes, &dummy)) < 0) {
	    count = 0;
	}
    }
    return(count);
}
