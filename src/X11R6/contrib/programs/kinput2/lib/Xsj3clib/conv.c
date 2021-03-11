#ifndef lint
static char *rcsid = "$Header: conv.c,v 2.12 93/09/21 09:43:15 nao Exp $";
#endif
/*
 * Copyright 1991 Sony Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Sony not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  Sony makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * SONY DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL SONY
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
/*
 * Author: Naoshi Suzuki, SONY Corporation.  (nao@sm.sony.co.jp)
 */

#include <ctype.h>
#include "common.h"
#include "sj3ctype.h"
#include "util.h"
#include "segment.h"
#include "mode.h"
#include "func.h"

extern Xsj3cCVServerIF      serverIF[SERVER_NUM];

unsigned char              *Xsj3cGetPreeditArea();
Xsj3cEvent                  Xsj3cKeyConv();

int                         _Xsj3cRomaConv();
int                         _Xsj3cKanaConv();

static Xsj3cEvent           _Xsj3cStrConv();
static Xsj3cEvent           _Xsj3cCtrlConv();
static Xsj3cEvent           _Xsj3cThrough();
static Xsj3cEvent           _Xsj3cDirect();
static int                  _Xsj3cCodeConv();

/*
 *  Xsj3cGetPreeditArea()
 * Get area for pre-edit string.
 */
unsigned char *
Xsj3cGetPreeditArea(buf, len)
    register Xsj3cBuf       buf;
    register int           *len;
{
    register Xsj3cSeg       seg;

    if (buf->convmode == DictModeMask) {
        /* $B<-=q%b!<%I$N>l9g$O<-=qFI$_MQ$NF~NO$r9T$&(B */
        seg = buf->dict->seg;
    } else if (buf->convmode != SelectModeMask) {
        if (buf->input[buf->curseg]
                && buf->input[buf->curseg]->status == SEG_CONVED) {
            /* $B8=J8@a$,JQ49:Q$_$N>l9g(B                   */

            buf->convmode = ConvedModeMask;
            switch (buf->flushiconv) {
            case ON:
                /* FlushInConversion on $B$N;~$O8=:_JQ49Cf$N(B  */
                /* $BJ8$r3NDj$7$F?75,$NJ8$NF~NO$r9T$&(B         */

                if (!buf->backup) {
                    if ((buf->backup = (Xsj3cSeg *)calloc(BUNBUFSIZ,
                            sizeof(Xsj3cSeg))) == NULL) {
                        Xsj3cError("Cannot allocate for backup buffers");
                    }
                }
                if (seg = buf->backup[0]) {
                    *seg->str = '\0';
                    seg->sp = seg->str;
                    *seg->oldstr = '\0';
                    seg->oldlen = 0;
                    seg->n_roma = 0;
                    seg->n_kana = -1;
                } else
                    seg = buf->backup[0] = (Xsj3cSeg)Xsj3cCreateSegment(buf);
                break;
            case EDIT:
                /* FlushInConversion edit $B$N;~$O%+%l%s%H(B    */
                /* $BJ8@a$X$NF~NO$r9T$&(B                       */

                if (!(seg = buf->input[buf->curseg])) {
                    seg = buf->input[buf->curseg]
                            = (Xsj3cSeg)Xsj3cCreateSegment(buf);
                } else {
                    *seg->str = '\0';
                    seg->sp = seg->str;
                    *seg->oldstr = '\0';
                    seg->oldlen = 0;
                    seg->n_roma = 0;
                    seg->n_kana = -1;
                }
                break;
            case OFF:
            case NONE:
            default:
                /* FlushInConversion none/off $B$N>l9g!"?7$?$J(B*/
                /* $BJ8@a$NFI$_J8;zNs$NF~NO$r9T$&(B             */

                if (seg = buf->input[buf->segnum]) {
                    Xsj3cClearSegment(buf, buf->input[buf->segnum]);
                } else {
                    seg = buf->input[buf->segnum]
                            = (Xsj3cSeg)Xsj3cCreateSegment(buf);
                }
                break;
            }
        } else if (buf->segnum > 0) {
            /* $B8=J8@a$,L$JQ49$GF~NOJ8;z$,$"$k>l9g(B   */

            buf->convmode = InputModeMask;
            if (buf->input[buf->curseg]
                    && (buf->input[buf->curseg]->edit & SEG_NOEDIT)) {
                /* $BJ8;zF~NO$r5v$5$l$F$$$J$$J8@a$N>l9g(B */
                switch (buf->flushiconv) {
                case ON:
                    /* FlushInConversion on $B$N;~$O8=:_JQ49Cf$N(B  */
                    /* $BJ8$r3NDj$7$F?75,$NJ8$NF~NO$r9T$&(B         */

                    if (!buf->backup) {
                        if ((buf->backup = (Xsj3cSeg *)calloc(BUNBUFSIZ,
                                sizeof(Xsj3cSeg))) == NULL) {
                            Xsj3cError("Cannot allocate for backup buffers");
                        }
                    }
                    if (seg = buf->backup[0]) {
                        *seg->str = '\0';
                        seg->sp = seg->str;
                        *seg->oldstr = '\0';
                        seg->oldlen = 0;
                        seg->n_roma = 0;
                        seg->n_kana = -1;
                    } else
                        seg = buf->backup[0]
                                = (Xsj3cSeg)Xsj3cCreateSegment(buf);
                    break;
                case EDIT:
                    if (!(seg = buf->input[buf->curseg]))
                        seg = buf->input[buf->curseg]
                                = (Xsj3cSeg)Xsj3cCreateSegment(buf);
                    break;
                case OFF:
                case NONE:
                default:
                    if (seg = buf->input[buf->segnum]) {
                        Xsj3cClearSegment(buf, buf->input[buf->segnum]);
                    } else 
                        seg = buf->input[buf->segnum]
                                = (Xsj3cSeg)Xsj3cCreateSegment(buf);
                }
            } else {
                /* $B%+%l%s%HJ8@a$X$NF~NO$r9T$&(B               */

                if (!(seg = buf->input[buf->curseg]))
                    seg = buf->input[buf->curseg]
                            = (Xsj3cSeg)Xsj3cCreateSegment(buf);
            }
        } else {
            /* $BF~NOJ8;z$,$J$$>l9g(B   */

            buf->convmode = NoInputModeMask;
            /* New segment(first segment)   */
            if (seg = buf->input[buf->curseg])
                Xsj3cClearSegment(buf, buf->input[buf->curseg]);
            else 
                seg = buf->input[buf->curseg]
                        = (Xsj3cSeg)Xsj3cCreateSegment(buf);
        }
    } else {
        /* SelectMode   */
        switch (buf->selectstatus) {
        case SELECT_CAND:
            /* $B8uJdA*Br$N>l9g(B */

            switch (buf->flushsconv) {
            case ON:
                /* FlushSelectConversion on $B$N;~$O8=:_JQ49(B  */
                /* $BCf$NJ8$r3NDj$7$F?75,$NJ8$NF~NO$r9T$&(B     */

                if (!buf->backup) {
                    if ((buf->backup = (Xsj3cSeg *)calloc(BUNBUFSIZ,
                            sizeof(Xsj3cSeg))) == NULL) {
                        Xsj3cError("Cannot allocate for backup buffers");
                    }
                } 
                if (seg = buf->backup[0]) {
                    *seg->str = '\0';
                    seg->sp = seg->str;
                    *seg->oldstr = '\0';
                    seg->oldlen = 0;
                    seg->n_roma = 0;
                    seg->n_kana = -1;
                } else {
                    seg = buf->backup[0] = (Xsj3cSeg)Xsj3cCreateSegment(buf);
                }
                break;
            case EDIT:
                /* FlushSelectConversion edit $B$N>l9g!"L5(B    */
                /* $BJQ49>uBV$K$7$F%+%l%s%HJ8@a$X$NF~NO$r9T$&(B */

                if (!(seg = buf->input[buf->curseg])) {
                    seg = buf->input[buf->curseg]
                            = (Xsj3cSeg)Xsj3cCreateSegment(buf);
                } else {
                    *seg->str = '\0';
                    seg->sp = seg->str;
                    *seg->oldstr = '\0';
                    seg->oldlen = 0;
                    seg->n_roma = 0;
                    seg->n_kana = -1;
                }
                break;
            case OFF:
            case NONE:
            default:
                /* FlushSelectConversion none/off $B$N>l9g!"(B  */
                /* $B?7$?$JJ8@a$NFI$_J8;zNs$NF~NO$r9T$&(B       */

                if (seg = buf->input[buf->segnum]) {
                    Xsj3cClearSegment(buf, buf->input[buf->segnum]);
                } else {
                    seg = buf->input[buf->segnum]
                            = (Xsj3cSeg)Xsj3cCreateSegment(buf);
                }
                break;
            }
            break;
        case SELECT_HINSI:
            /* $BIJ;lA*Br$N>l9g(B */

            seg = buf->dict->seg;
            break;
        case SELECT_SYMBOL: 
        default: 
            /* $B5-9fA*Br$N>l9g(B */

            if (seg = buf->input[buf->segnum]) {
                Xsj3cClearSegment(buf, buf->input[buf->segnum]);
            } else {
                seg = buf->input[buf->segnum]
                        = (Xsj3cSeg)Xsj3cCreateSegment(buf);
            }
            break;
        }
    }
    *len = RBUFSIZ - (seg->sp - seg->str);
    buf->current = seg;
    return (seg->sp);
}

#define IsKanaKey(ks)   ((unsigned)(ks) < 0xff00 && ((unsigned)(ks) & 0x0400))
#define IsLatin1Key(ks) ((unsigned)(ks) < 0x0100)

/*
 *  Xsj3cKeyConv()
 * Convert string or keysym and do function.
 */
Xsj3cEvent
Xsj3cKeyConv(buf, n, mod, ks)
    Xsj3cBuf                buf;
    int                     n;
    unsigned long           mod;
    KeySym                  ks;
{
    register Xsj3cKeyTable *keytp;
    Xsj3cSeg                seg = buf->current;
    Xsj3cEvent              ret = KEY_NULL;
    unsigned char          *tmpsp;
    register int            i,  doflg;

    /* KeySym/Modifier$B!"8=:_$NJQ49%b!<%I$H3F%U%!%s%/%7%g%s(B  */
    /* $B$NBP1~%F!<%V%k$r%5!<%A$7$F9gCW$9$k$b$N$,$"$l$P%U%!%s(B */
    /* $B%/%7%g%s$r<B9T$9$k!#(B                                 */
    for (keytp = buf->key, doflg = 0; keytp != NULL; keytp = keytp->next) {
                                    /* KeySym */
        if (ks == keytp->ksym &&
                                    /* Modifier Key */
                (keytp->modmask & ~AllModeMask) == mod &&
                                    /* Conversion mode */
                (keytp->modmask & buf->convmode) == buf->convmode) {
                                    /* Do function  */
            doflg++;

            /* .key.func $B$NBh#3%U%#!<%k%I$,(B off $B$N;~$O%U%!%s%/%7%g%s(B    */
            /* $B$N$_<B9T$7!"(Bnone $B$N;~$O(B .InputSameTime $B$N;XDj$K0MB8$9$k(B  */
            /* InputSameTime off $B$N;~$O%U%!%s%/%7%g%s<B9T$9$k(B           */
            if (!buf->inputsame) {
                if (keytp->inputsame != ON)
                    goto dofunc;
            } else {
                if (!keytp->inputsame)
                    goto dofunc;
            }
            break;
        }
    }

    /* LookupString $B$N7k2L$O(B NULL terminate */
    /* $B$7$F$$$J$$$N$G(B NULL terminate $B$5$;$k(B */
    tmpsp = seg->sp;
    i = n;
    while(i--) {
        if (iscntrl(*tmpsp)) {
            if (buf->throughflg == QUOTE) {
                ret |= KEY_CONTROL;
                tmpsp++;
                continue;
            }
            /* ControlSameTime off $B$N>l9g$O(B   */
            /* $B%U%!%s%/%7%g%s$N$_<B9T(B         */
            if (doflg && !buf->cntrlsame)
                goto dofunc;
            else {
                ret = _Xsj3cCtrlConv(buf, *tmpsp);
                if (buf->cntrlsame)
                    goto dofunc;
                else
                    return (ret);
            }
        }
        tmpsp++;
    }
    *tmpsp = '\0';

    if (!n || !(IsKanaKey(ks)||(IsLatin1Key(ks)))) {
        /* $B%F%-%9%H$KJQ49$G$-$J$+$C$?>l9g$O(B */
        /* $B%U%!%s%/%7%g%s$N$_<B9T(B           */
        goto dofunc;
    }

    switch (buf->convmode) {
    case SelectModeMask:

        switch (buf->selectstatus) {
        case SELECT_CAND:
            /* $B8uJdA*Br$N>l9g(B */

            switch (buf->flushsconv) {
            case ON:
                ret |= (KEY_SELECT_END|KEY_TEXT_CHANGE);
                for (i = 1; i < buf->backsegnum + 1; i++) {
                    Xsj3cFreeSegment(buf->backup[i]);
                    buf->backup[i] = NULL;
                }
                seg->yomi[0] = '\0';
                seg->disp[0] = '\0';
                seg->num = 0;
                seg->cur = 0;
                seg->dnum = 0;
                seg->status = SEG_NOCONV;
                seg->cursegmode = buf->inputmode;
                seg->change = OFF;
                seg->edit = SEG_NOEDIT;
                bzero(&seg->dcid, sizeof(seg->dcid));
                buf->backsegnum = 1;
                ret |= KEY_TEXT_FLUSH;
                if (buf->dispmodechange) {
                    buf->dispmode = buf->inputmode;
                    ret |= KEY_MODE_CHANGE;
                }
                break;
            case EDIT:
                ret |= (KEY_SELECT_END|KEY_TEXT_CHANGE);
                if (buf->curseg == buf->segnum)
                    buf->segnum++;
                else {
                    ret |= _Xsj3cUnConvSeg(buf, ONE, buf->editcurlast);
                    _Xsj3cStoreYomi(buf, buf->input[buf->curseg], 0);
                }
                break;
            case OFF:
                ret |= (KEY_SELECT_END|KEY_TEXT_CHANGE);
                if (buf->dispmodechange) {
                    buf->dispmode = buf->convedsegnum ? MODE_EDIT :
                            buf->inputmode;
                    ret |= KEY_MODE_CHANGE;
                }
                break;
            case NONE:
            default:
                goto dofunc;
            }
            break;
        case SELECT_HINSI:
            /* $BIJ;lA*Br$N>l9g(B */

            ret |= (KEY_DICT_CHANGE|KEY_SELECT_ABORT);
            buf->dict->status = DICT_INPUT;
            if (buf->dispmodechange) {
                buf->dispmode =
                    (buf->dict->mode == REG_STATE ? MODE_TOROKU : MODE_SYOUKYO);
                ret |= KEY_MODE_CHANGE;
            }
            break;
        case SELECT_SYMBOL:
        default:
            /* $B5-9fA*Br$N>l9g(B */
            goto dofunc;
        }
        break;
    case DictModeMask:
        if (buf->dict->status != DICT_INPUT) {
            /* $B<-=qEPO?!?>C5n%b!<%I$G$h$_F~NO;~$G$J$$(B   */
            /* $B$H$-$O$rJV$7$F$J$K$b$7$J$$!#(B       */
            goto dofunc;
        }
        ret |= KEY_DICT_CHANGE;
        seg->edit = SEG_EDIT;
        ret = _Xsj3cStrConv(buf, seg, ks, n, ret);
        _Xsj3cFlushDictMsg(buf);
        goto dofunc;
    case ConvedModeMask:
        ret |= KEY_TEXT_CHANGE;
        if (buf->candidate)
            Xsj3cEndCandidate(buf, ON);
        switch (buf->flushiconv) {
        case ON:
            for (i = 1; i < buf->backsegnum + 1; i++) {
                Xsj3cFreeSegment(buf->backup[i]);
                buf->backup[i] = NULL;
            }
            seg->yomi[0] = '\0';
            seg->disp[0] = '\0';
            seg->num = 0;
            seg->cur = 0;
            seg->dnum = 0;
            seg->status = SEG_NOCONV;
            seg->cursegmode = buf->inputmode;
            seg->change = OFF;
            seg->edit = SEG_NOEDIT;
            bzero(&seg->dcid, sizeof(seg->dcid));
            buf->backsegnum = 1;
            ret |= KEY_TEXT_FLUSH;
            if (buf->dispmodechange) {
                buf->dispmode = buf->inputmode;
                ret |= KEY_MODE_CHANGE;
            }
            break;
        case OFF:
            buf->curseg = buf->segnum;
            buf->segnum++;
            if (buf->dispmodechange) {
                buf->dispmode = 
                    (buf->convedsegnum ? MODE_EDIT : buf->inputmode);
                ret |= KEY_MODE_CHANGE;
            }
            break;
        case EDIT:
            if (buf->curseg == buf->segnum)
                buf->segnum++;
            else {
                ret |= _Xsj3cUnConvSeg(buf, ONE, buf->editcurlast);
                _Xsj3cStoreYomi(buf, seg, 0);
            }
            break;
        case NONE:
        default:
            goto dofunc;
        }
        break;
    case InputModeMask:
        ret |= KEY_TEXT_CHANGE;
        if (buf->input[buf->curseg]->edit & SEG_NOEDIT) {
            switch (buf->flushiconv) {
            case ON:
                for (i = 1; i < buf->backsegnum + 1; i++) {
                    Xsj3cFreeSegment(buf->backup[i]);
                    buf->backup[i] = NULL;
                }
                seg->yomi[0] = '\0';
                seg->disp[0] = '\0';
                seg->num = 0;
                seg->cur = 0;
                seg->dnum = 0;
                seg->status = SEG_NOCONV;
                seg->cursegmode = buf->inputmode;
                seg->change = OFF;
                seg->edit = SEG_NOEDIT;
                bzero(&seg->dcid, sizeof(seg->dcid));
                buf->backsegnum = 1;
                ret |= KEY_TEXT_FLUSH;
                break;
            case OFF:
                buf->curseg = buf->segnum;
                buf->segnum++;
                if (buf->dispmodechange) {
                    buf->dispmode = buf->convedsegnum ? MODE_EDIT :
                            buf->inputmode;
                    ret |= KEY_MODE_CHANGE;
                }
                break;
            case EDIT:
                if (buf->curseg == buf->segnum)
                    buf->segnum++;
                break;
            case NONE:
            default:
                goto dofunc;
            }
        } else if (buf->curseg == buf->segnum)
            buf->segnum++;
        break;
    case NoInputModeMask:
        if (buf->curseg == buf->segnum)
            buf->segnum++;
        if (buf->throughflg == THROUGH) {
            ret |= _Xsj3cThrough(buf, seg, n);
            buf->throughflg = OFF;
            goto dofunc;
        }
        ret |= KEY_TEXT_CHANGE;
        break;
    default:
        Xsj3cWarning ("Unknown conversion mode");
        goto dofunc;
    }

    seg->edit = SEG_EDIT;
    ret = _Xsj3cStrConv(buf, seg, ks, n, ret);

dofunc:
    if (doflg && !(ret & KEY_TEXT_FLUSH)) {
        ret |= ((*(keytp->func))(buf));
        if (!buf->cntrlsame)
            ret |= KEY_FUNC;
    }
    return (ret);
}

/*
 *  _Xsj3cStrConv()
 *   Convert pre-edit strings and store results to yomi/disp buffer.
 */
static Xsj3cEvent
_Xsj3cStrConv(buf, seg, ks, n, ret)
    Xsj3cBuf                buf;
    Xsj3cSeg                seg;
    KeySym                  ks;
    int                     n;
    Xsj3cEvent              ret;
{
    register unsigned char *tmpsp;
    unsigned char           tmp1[YBUFSIZ];
    unsigned char           tmp2[YBUFSIZ];
    unsigned char           kanabuf[YBUFSIZ];
    wchar                   wcs[RBUFSIZ];
    register int            i;
    int                     change_pos;
    int                     change_roma = 0;

    if (buf->throughflg == QUOTE) {
        ret |= _Xsj3cDirect(buf, seg, n);
        buf->throughflg = OFF;
        return (ret);
    }
    if (seg->num > seg->size - YBUFSIZ)
        Xsj3cResizeSegment(seg, seg->size + KANABUFSIZ);

    switch (buf->inputmode) {
    case MODE_HIRA:
    case MODE_ZKATA:
    case MODE_HKATA:
        /* $B$R$i$,$J!?A43Q%+%?%+%J!?H>3Q%+%?%+%JF~NO%b!<%I$N>l9g(B */
        if (IsKanaKey(ks) || (IsLatin1Key(ks) && buf->kanaonly)) {

            /* $B$+$JF~NO$N>l9g(B */
            int             len;

            if (seg->n_roma) {

                /* $B$+$J%P%C%U%!(B(seg->str)$B$K%m!<%^;zF~NO;~$N(B  */
                /* $BJ8;zNs$,;D$C$F$$$k>l9g$O$=$l$r:o=|$9$k(B   */
                i = n;
                tmpsp = seg->sp;
                while (i--) {
                    *(tmpsp - seg->n_roma) = *tmpsp;
                    tmpsp++;
                }
                *(tmpsp - seg->n_roma) = '\0';
                seg->sp = seg->sp - seg->n_roma;
                seg->n_roma = 0;
            }
            if (buf->inputmode == MODE_HKATA) {

                /* $BH>3Q%+%?%+%JF~NO%b!<%I$N$H$-$O(B       */
                /* $BJQ49$r9T$o$J$$$GFI$_%P%C%U%!$K%3%T!<(B */
                if (IsKanaKey(ks))
                    strcpy(tmp1, seg->sp);
                else
                    _Xsj3cHAlphaToHKata(buf, tmp1, seg->sp);
                seg->sp = seg->str;
                *seg->oldstr= '\0';
                seg->n_kana = -1;
                change_pos = seg->cur;
                _Xsj3cInsertChar(buf, seg, tmp1, strlen(tmp1));

            } else {
                /* $B$R$i$,$J!?A43Q%+%?%+%JF~NO%b!<%I$N;~$OH>3Q(B   */
                /* $B%+%?%+%J"*$R$i$,$J!?A43Q%+%?%+%JJQ49$r9T$&(B   */
                if (IsLatin1Key(ks)) {
                    strcpy(tmp2, seg->sp);
                    _Xsj3cHAlphaToHKata(buf, seg->sp, tmp2);
                }
                if ((seg->value = _Xsj3cKanaConv(buf, seg, seg->str,
                        tmp1, buf->inputmode)) > 0) {

                    /* $BH>3Q%+%?%+%J"*$R$i$,$JJQ49$,@.8y$7$?>l9g(B */
                    /* $BBy2;!?H>By2;$N>l9g(B                       */
                    seg->sp = seg->str;
                    if (seg->n_kana > 0) {
                        _Xsj3cExtractChar(buf, seg, tmp2, seg->n_kana);
                        strcpy(seg->oldstr, seg->str);
                        tmpsp = seg->oldstr;
                        while(*tmpsp != '\0')
                            tmpsp++;
                        *(tmpsp - seg->n_kana) = '\0';
                    } else {
                        *seg->oldstr = '\0';
                    }
                    seg->n_kana = 0;
                } else if (seg->value == 0) {

                    /* $BH>3Q%+%?%+%J"*$R$i$,$J!?A43Q%+%?%+%J(B */
                    /* $BJQ49$N7k2L$,ITDj$N>l9g(B               */
                    if (seg->n_kana > 0)
                        _Xsj3cExtractChar(buf, seg, tmp2, seg->n_kana);
                    tmpsp = seg->sp + n - 1;
                    seg->sp = seg->str;
                    *seg->sp = *tmpsp;
                    *(++seg->sp) = '\0';
                    seg->n_kana = 1;
                    *seg->oldstr = '\0';
                } else {

                    /* $BH>3Q%+%?%+%J"*$R$i$,$JJQ49$,@.8y$7$?>l9g(B */
                    /* $BBy2;!?H>By2;$G$J$$>l9g(B                   */
                    if (seg->n_kana > 0)
                        _Xsj3cExtractChar(buf, seg, tmp2, seg->n_kana);
                    seg->sp = seg->str;
                    *seg->oldstr = '\0';
                    seg->n_kana = 0;
                }
                change_pos = seg->cur;
                len = _Xsj3cmPStowPS(buf, wcs, tmp1);
                _Xsj3cInsertWchar(seg, wcs, len);
            }
            seg->n_roma = 0;

        } else if (IsLatin1Key(ks)) {

            /* $B%m!<%^;zF~NO$N>l9g(B */

            if (seg->n_kana > 0) {

                /* $B%m!<%^;z%P%C%U%!(B(seg->str)$B$K$+$JF~NO;~$N(B */
                /* $BJ8;zNs$,;D$C$F$$$k>l9g$O$=$l$r:o=|$9$k(B */
                i = n;
                tmpsp = seg->sp;
                while (i--) {
                    *(tmpsp - seg->n_kana) = *tmpsp;
                    tmpsp++;
                }
                *(tmpsp - seg->n_kana) = '\0';
                seg->sp = seg->sp - seg->n_kana;
                seg->n_kana = -1;
            }

            /* $BF~NO%b!<%I$K4X78$J$/$+$JJQ49$r9T$&(B  */

            if ((seg->value = _Xsj3cRomaConv(buf->rktable, 
                        seg->str, kanabuf)) > 0) {

                /* ROMA->$B$+$J(B $BJQ49$,@.8y$7$?>l9g(B    */
                if (buf->alphaconv) {
                    /* AlphabetConversion $B%j%=!<%9$,(B on   */
                    /* $B$@$C$?$iA43Q$rH>3Q$KJQ49$9$k(B     */

                    _Xsj3cExtractChar(buf, seg, tmp1, seg->n_roma);
                    _Xsj3cZAlphaToHAlpha(buf, seg->oldstr, tmp1);
                } else {
                    _Xsj3cExtractChar(buf, seg, seg->oldstr, seg->n_roma);
                }
                change_pos = seg->cur;
                if (buf->inputmode == MODE_HIRA) {

                    /* $B$R$i$,$JF~NO%b!<%I$N$H$-$O$=$N$^$^(B */
                    /* $BFI$_%P%C%U%!$K%3%T!<(B  */
                    seg->oldlen = _Xsj3cmPStowPS(buf, wcs, kanabuf);
                } else if (buf->inputmode == MODE_ZKATA) {

                    /* $BA43Q%+%?%+%JF~NO%b!<%I$N$H$-$O(B */
                    /* $B$R$i$,$J$rA43Q%+%?%+%J$KJQ49(B    */
                    _Xsj3cHiraToZKata(buf, tmp1, kanabuf);
                    seg->oldlen = _Xsj3cmPStowPS(buf, wcs, tmp1);
                } else {

                    /* $BH>3Q%+%?%+%JF~NO%b!<%I$N;~$O$R$i$,$J(B */
                    /* $B!?A43Q%+%?%+%J$rH>3Q%+%?%+%J$KJQ49(B   */
                    _Xsj3cZKanaToHKata(buf, tmp1, kanabuf);
                    seg->oldlen = _Xsj3cmPStowPS(buf, wcs, tmp1);
                }
                _Xsj3cInsertWchar(seg, wcs, seg->oldlen);
                seg->n_roma = 0;

                /* $BJQ49$G$-$J$$J8;z$,%m!<%^;z%P%C%U%!$K;D$C$F(B   */
                /* $B$$$?$i!JNc!V$C!W$KJQ49$5$l$k>l9gEy!K(B         */
                /* $B$+$J%P%C%U%!$N:G8e$K$=$NJ8;z$rIUM?$9$k(B       */
                if (*seg->str != '\0') {
                    _Xsj3cInsertChar(buf, seg, seg->str, strlen(seg->str));
                    seg->sp = seg->str;
                    while (*seg->sp != '\0') {
                        seg->sp++;
                        seg->n_roma++;
                    }
                    change_roma = seg->n_roma;
                } else {
                    seg->sp = seg->str;
                }

            } else if (seg->value == 0) {

                /* ROMA$B"*$+$JJQ49$N7k2L$,ITDj$N>l9g(B */
                change_pos = seg->cur;
                _Xsj3cInsertChar(buf, seg, seg->sp, n);
                i = n;
                while (i--) {
                    seg->sp++;
                    seg->n_roma++;
                }
                change_roma = n;

            } else {
                /* ROMA$B"*$+$J(B $BJQ49$N7k2L$,IT@.8y$N>l9g(B  */
                /* $B:G8e$NF~NO$,%m!<%^$+$JJQ49$N8uJd$H(B   */
                /* $B$7$FM-8z$+$I$&$+D4$Y$k(B               */

                change_pos = seg->cur;
                if ((seg->value = _Xsj3cRomaConv(buf->rktable, seg->sp,
                        kanabuf)) > 0) {
                    /* $B:G8e$NF~NO$,%m!<%^$+$JJQ49$N8uJd$H$7$F(B   */
                    /* $BM-8z$J>l9g(B                               */

                    if (buf->inputmode == MODE_HIRA) {

                        /* $B$R$i$,$JF~NO%b!<%I$N$H$-$O$=$N$^$^(B */
                        /* $BFI$_%P%C%U%!$K%3%T!<(B  */
                        seg->oldlen = _Xsj3cmPStowPS(buf, wcs, kanabuf);
                    } else if (buf->inputmode == MODE_ZKATA) {

                        /* $BA43Q%+%?%+%JF~NO%b!<%I$N$H$-$O(B */
                        /* $B$R$i$,$J$rA43Q%+%?%+%J$KJQ49(B    */
                        _Xsj3cHiraToZKata(buf, tmp1, kanabuf);
                        seg->oldlen = _Xsj3cmPStowPS(buf, wcs, tmp1);
                    } else {

                        /* $BH>3Q%+%?%+%JF~NO%b!<%I$N;~$O$R$i$,$J(B */
                        /* $B!?A43Q%+%?%+%J$rH>3Q%+%?%+%J$KJQ49(B   */
                        _Xsj3cZKanaToHKata(buf, tmp1, kanabuf);
                        seg->oldlen = _Xsj3cmPStowPS(buf, wcs, tmp1);
                    }
                    _Xsj3cInsertWchar(seg, wcs, seg->oldlen);
                    seg->n_roma = 0;

                    /* $BJQ49$G$-$J$$J8;z$,%m!<%^;z%P%C%U%!$K;D$C$F(B   */
                    /* $B$$$?$i!JNc!V$C!W$KJQ49$5$l$k>l9gEy!K(B         */
                    /* $B$+$J%P%C%U%!$N:G8e$K$=$NJ8;z$rIUM?$9$k(B       */
                    if (*seg->str != '\0') {
                        _Xsj3cInsertChar(buf, seg, seg->str, strlen(seg->str));
                        seg->sp = seg->str;
                        while (*seg->sp != '\0') {
                            seg->sp++;
                            seg->n_roma++;
                        }
                        change_roma = seg->n_roma;
                    } else {
                        seg->sp = seg->str;
                    }
                } else if (seg->value == 0) {
                    /* $B%m!<%^$+$JJQ49$N8uJd$H$7$FL$Dj$N>l9g(B */
                    /* $B:G8e$NF~NOJ8;z$r%m!<%^"*$+$JJQ49MQ$N(B */
                    /* $B%P%C%U%!(B(seg->str)$B$K;D$9(B             */

                    change_pos = seg->cur;
                    _Xsj3cInsertChar(buf, seg, seg->sp, n);
                    *seg->oldstr = '\0';
                    tmpsp = seg->str;
                    *seg->str = *seg->sp;
                    seg->sp = seg->str;
                    i = n;
                    while (i--)
                        *tmpsp++ = *seg->sp++;
                    *tmpsp = '\0';
                    seg->n_roma = n;
                    change_roma = seg->n_roma;
                } else {
                    /* $B%m!<%^$+$JJQ49$N8uJd$H$7$FL58z$J>l9g(B */

                    _Xsj3cInsertChar(buf, seg, seg->sp, n);
                    *seg->oldstr = '\0';
                    seg->n_roma = 0;
                    *seg->str = '\0';
                    seg->sp = seg->str;

                    /* rkbell $B%j%=!<%9$,(B on $B$K@_Dj$5$l$F(B    */
                    /* $B$$$k>l9g%Y%k$rLD$i$9(B                 */
                    if (buf->rkbell)
                        ret |= KEY_BELL;
                    change_roma = n;
                }
            }

            /* AlphabetConversion $B%j%=!<%9$,(B on     */
            /* $B$@$C$?$iH>3Q%m!<%^;z$rA43Q%m!<%^;z$KJQ49$9$k(B */
            if (buf->alphaconv && buf->inputmode != MODE_HKATA
                    && change_roma) {
                _Xsj3cExtractChar(buf, seg, tmp1, change_roma);
                _Xsj3cHAlphaToZKana(buf, tmp2, tmp1);
                _Xsj3cInsertChar(buf, seg, tmp2, change_roma); 
            } 
            /* $B$+$JF~NO%b!<%I$G$J$$$H$-$O(B -1  */
            seg->n_kana = -1;
        } else {
            return (KEY_NULL);
        }
        break;

    case MODE_HALPHA:

        /* $BH>3Q%"%k%U%!%Y%C%H(B $BF~NO%b!<%I$N>l9g(B */
        if (IsKanaKey(ks)) 
            return (KEY_BELL);
        change_pos = seg->cur;
        _Xsj3cInsertChar(buf, seg, seg->sp, n); 
        seg->sp = seg->str;
        seg->n_roma = 0;
        seg->n_kana = -1;
        break;

    case MODE_ZALPHA:

        /* $BA43Q%"%k%U%!%Y%C%H(B $BF~NO%b!<%I$N>l9g(B */
        if (IsKanaKey(ks)) 
            return (KEY_BELL);
        _Xsj3cHAlphaToZAlpha(buf, tmp1, seg->sp);
        seg->sp = seg->str;
        change_pos = seg->cur;
        _Xsj3cInsertChar(buf, seg, tmp1, n); 
        seg->n_roma = 0;
        seg->n_kana = -1;
        break;

    case MODE_SJIS:
    case MODE_EUC:
    case MODE_JIS:
    case MODE_KUTEN:

        /* $B%3!<%IF~NO%b!<%I$N>l9g(B */
        tmpsp = seg->str;
        while (*tmpsp != '\0') {
            if (!isxdigit(*tmpsp)
                || (buf->inputmode == MODE_KUTEN && !isdigit(*tmpsp)))
                return (KEY_BELL);
            tmpsp++;
        }

        /* $B%3!<%I"*$R$i$,$J!?A43Q%+%?%+%J!?5-9f!?4A;z$NJQ49$r9T$&(B */
        if ((seg->value = _Xsj3cCodeConv(buf, seg->str, 
                kanabuf, buf->inputmode)) > 0) {

            /* $B%3!<%IJQ49$K@.8y$7$?>l9g(B */
            if (buf->alphaconv) {
                /* AlphabetConversion $B%j%=!<%9$,(B on $B$@$C$?$i(B */
                /* $BA43Q?t;z$rH>3Q?t;z$KJQ49$9$k(B              */
                _Xsj3cExtractChar(buf, seg, tmp1, seg->n_roma);
                _Xsj3cZAlphaToHAlpha(buf, seg->oldstr, tmp1);
            } else {
                _Xsj3cExtractChar(buf, seg, seg->oldstr, seg->n_roma);
            }
            change_pos = seg->cur;
            seg->oldlen = (strlen(kanabuf) + 1)/ sizeof(wchar);
            _Xsj3cInsertChar(buf, seg, kanabuf, seg->oldlen);
            seg->cursegmode = MODE_HIRA;
            seg->n_roma = 0;
            seg->sp = seg->str;
        } else if (seg->value == 0) {

            /* $B%3!<%IJQ49$N7k2L$,ITDj$N>l9g(B */
            change_pos = seg->cur;
            if (buf->alphaconv) {
                /* AlphabetConversion $B%j%=!<%9$,(B on $B$@$C$?$i(B */
                /* $BH>3Q?t;z$rA43Q?t;z$KJQ49$9$k(B              */
                _Xsj3cHAlphaToZKana(buf, tmp1, seg->sp);
                _Xsj3cInsertChar(buf, seg, tmp1, n); 
            } else {
                _Xsj3cInsertChar(buf, seg, seg->sp, n);
            }
            i = n;
            while (i--) {
                seg->n_roma++;
                seg->sp++;
            }
        } else {

            /* $B%3!<%IJQ49$N7k2L$,<:GT$N>l9g(B */
            _Xsj3cExtractChar(buf, seg, tmp1, seg->n_roma);
            change_pos = seg->cur;
            seg->sp = seg->str;
            seg->n_roma = 0;
            if (seg->num == 0 && buf->convmode != DictModeMask) {
                Xsj3cFreeSegment(seg);
                seg = NULL;
                buf->segnum--;
                for (i = buf->curseg; i < buf->segnum; i++) {
                    buf->input[i] = buf->input[i + 1];
                }
                buf->input[buf->segnum] = NULL;
                return ret;
            }
            if (buf->rkbell)
                ret |= KEY_BELL;
        }
        seg->n_kana = -1;
        break;

    default:
        Xsj3cWarning("Illegal current mode");
        return (KEY_BELL);
    }

    /* $BI=<(MQJ8;zNs$X%3%T!<$9$k(B        */
    _Xsj3cStoreYomi(buf, seg, change_pos);
    return ret;
}

/*
 * _Xsj3cKanaConv()
 *  Kana input mode dispatch routine.
 */
int
_Xsj3cKanaConv(buf, seg, hkana, zkana, mode)
    Xsj3cBuf        buf;
    Xsj3cSeg        seg;
    unsigned char  *hkana, *zkana;
    Xsj3csMode      mode;
{
    register int             len,   zlen;
    register wchar           s;

    if (mode == MODE_HIRA) {
        if (buf->alphaconv)
            _Xsj3cHankakuToHira(buf, zkana, hkana);
        else
            _Xsj3cHKataToHira(buf, zkana, hkana);
    } else {
        if (buf->alphaconv)
            _Xsj3cHankakuToZKata(buf, zkana, hkana);
        else
            _Xsj3cHKataToZKata(buf, zkana, hkana);
    }
    len = 0;
    while (*hkana != '\0') {
        hkana++;
        len++;
    }
    if (isdakuon(*(hkana - 1))) {
        return 0;
    } else {
        zlen = 0;
        while (*zkana != '\0') {
            zkana++;
            zlen++;
        }
        if (zlen > 1) {
            s = (*(zkana - 2) << 8) + (*(zkana - 1) & 0xff);
            if (seg->n_kana && (iszdakuten(s, serverIF[buf->server].lang)
                    || !isdakuten(*(hkana - 1))))
                return -1; 
            else 
                return len; 
        } else {
            return -1; 
        }
    }
}

/*
 * _Xsj3cCodeConv()
 *  Code input mode dispatch routine.
 */
static int
_Xsj3cCodeConv(buf, code, kanji, mode)
    Xsj3cBuf                    buf;
    register unsigned char     *code;
    register unsigned char     *kanji;
    Xsj3csMode                  mode;
{
    register int            i,  j,  len;
    register wchar          k,  (*conv)();
    register unsigned char  c1, c2;
    unsigned char           kbuf[4];

    if ((len = strlen(code)) < 4 && (mode == MODE_EUC || mode == MODE_KUTEN))
        return CONV_UNFIXED;
    else if ((len < 2 || len == 3) && (mode == MODE_SJIS || mode == MODE_JIS))
        return CONV_UNFIXED;

    k = 0;  
    for (j = 0; j < 4 && *code != '\0'; code++, j++) {
        if (isdigit(*code))
            i = *code - '0';
        else if (islower(*code))
            i = 10 + *code - 'a';
        else if (isupper(*code))
            i = 10 + *code - 'A';
        else
            return CONV_FAILED;
        kbuf[j] = i;
        k += i << ((3 - j) * 4);
    }
    switch (mode) {
    case MODE_SJIS:
        if (len == 4 && issjis1(c1 = (k >> 8)) && issjis2(c2 = (k & 0xff))) {
            if (conv = CodeConvFunc[JP_SJIS][serverIF[buf->server].lang]) {
                k = conv(k);
                *kanji++ = k >> 8;
                *kanji++ = k & 0xff;
                *kanji = '\0';
            } else {
                *kanji++ = c1;
                *kanji++ = c2;
                *kanji = '\0';
            }
        } else if (iskana(k >> 8)) {
            *kanji++ = k >> 8;
            *kanji = '\0';
        } else {
            if (len < 4)
                return CONV_UNFIXED;
            else 
                return CONV_FAILED;
        }
        break;
    case MODE_EUC:
        if (iseuc(k >> 8) && iseuc(k & 0xff)) {
            if (conv = CodeConvFunc[JP_EUC][serverIF[buf->server].lang])
                k = conv(k);
            *kanji++ = k >> 8;
            *kanji++ = k & 0xff;
            *kanji = '\0';
        } else if (iseuckana(k >> 8) && iskana2(k & 0xff)) {
            *kanji++ = k & 0xff;
            *kanji = '\0';
        } else {
            return CONV_FAILED;
        }
        break;
    case MODE_JIS:
        if (len == 4 && isjis(c1 = (k >> 8)) && isjis(c2 = (k & 0xff))) {
            if (conv = CodeConvFunc[JP_JIS8][serverIF[buf->server].lang]) {
                k = conv(k);
                *kanji++ = k >> 8;
                *kanji++ = k & 0xff;
                *kanji = '\0';
            } else {
                *kanji++ = c1;
                *kanji++ = c2;
                *kanji = '\0';
            }
        } else if (iskana(k >> 8)) {
            *kanji++ = k >> 8;
            *kanji = '\0';
        } else {
            if (len < 4)
                return CONV_UNFIXED;
            else 
                return CONV_FAILED;
        }
        break;
    case MODE_KUTEN:
        conv = CodeConvFunc[JP_JIS8][serverIF[buf->server].lang];
        c1 = kbuf[0] * 10 + kbuf[1];
        c2 = kbuf[2] * 10 + kbuf[3];
        k = (c1 << 8) + c2;
        k = conv(k + 0x2020);
        if (iskan1(c1 = (k >> 8), serverIF[buf->server].lang)
                && iskan2(c2 = (k & 0xff), serverIF[buf->server].lang)) {
            *kanji++ = c1;
            *kanji++ = c2;
            *kanji = '\0';
        } else {
            return CONV_FAILED;
        }
        break;
    default:
        Xsj3cWarning("Illegal mode");
        return CONV_FAILED;
    }   
        
    return CONV_FIXED;
}

#define MATCH           0
#define NOT             1

/*
 *  _Xsj3cRomaConv()
 *   Get roman characters from the second argument and put back hiragana or
 *  katakana characters to the third argument in conversion table.
 *   If roman characters (third argument) mach with the "roma" entry in table, 
 *  put back the "str" entry to the third argument and the "yomi" entry
 *  to the third argument, then it returns number of converted roman
 *  characters.
 *   If not but any letters of roman characters is match with the "roma"
 *  entry in table and the number of roman characters is less than
 *  the number of the letters of "roma" entry, put back the same to
 *  the third argument and nothing to the third argument, then it returns
 *  zero.
 *   Then last (not converted case) it returns minus 1.
 *   Any arguments must be null('\0') terminated.
 *   First argument is pointer to conversion table;
 */ 
int
_Xsj3cRomaConv(rktable, roma, yomi)
    Xsj3cRKTable               *rktable;
    register unsigned char     *roma;
    register unsigned char     *yomi;
{
    Xsj3cRKTable               *rktp;
    register unsigned char     *p,     *q;
    unsigned char               tmp[RBUFSIZ];
    int                         match, result = CONV_FAILED;                    
    register int                len;

    if (!roma || (len = strlen(roma)) == 0)
        return (CONV_FAILED);
    for (rktp = rktable; rktp != NULL; rktp = rktp->next) {
        if (!rktp->roma) {
            continue;
        }
        p = roma;
        q = rktp->roma;
        if (len > rktp->rlen) {
            if (*q++ == *p++) {
                match = MATCH;
                while (*q != '\0') {
                    if (*q != *p) {
                        match = NOT;
                        break;
                    }
                    q++;
                    p++;
                }
                if (match == NOT) {
                    continue;
                }
                if (result < 0) {
                    result = rktp->rlen;
                    strcpy(yomi, rktp->yomi);
                    strcpy(tmp, p);
                } else {
                    continue;
                }
            } else {
                continue;
            }
        } else {
            if (*p++ == *q++) {
                match = MATCH;
                while (*p != '\0') {
                    if (*p++ != *q++) {
                        match = NOT;
                        break;
                    }
                }
                if (match == NOT) {
                    continue;
                }
                if (*q != '\0') {
                    result = CONV_UNFIXED;
                    continue;
                } else if (result == CONV_UNFIXED) {
                    continue;
                } else {
                    result = rktp->rlen;
                    strcpy(yomi, rktp->yomi);
                    strcpy(tmp, rktp->str);
                }
            } else {
                continue;
            }
        }
    }
    if (result > 0)
        strcpy(roma, tmp);
    return (result);
}

/*
 * _Xsj3cCtrlConv()
 *
 * <NoInputMode> Through all control events.
 * <InputMode> Put the control code (now only HT & NL) after current position,
 *   then fix all segments.
 * <ConvedMode> Put the control code (now only HT & NL) after current segment,
 *   then fix all segments and end convertion.
 * <SelectMode/DictMode> Does Nothing.
 *
 * DisplayModeChange on: Change the display mode string.  
 * ThroughNext code: 
 * SetNormal code: 
 */
static Xsj3cEvent
_Xsj3cCtrlConv(buf, code)
    Xsj3cBuf                buf;
    unsigned char           code;
{
    unsigned char           ch[2];
    register unsigned char *c;
    int                     change_pos;
    register int            i;

    if (code == '\t' || code == '\n')
        goto compound_text;
    if (buf->throughnext) {
        c = buf->throughnext;
        while (*c != '\0') {
            if (*c == code) {
                buf->throughflg = THROUGH;
                if (buf->dispmodechange) {
                    buf->dispmode = buf->inputmode;
#ifdef THROUGH_CONT
                    return (KEY_TEXT_CLEAR|KEY_CONTROL|KEY_MODE_CHANGE);
                } else {
                    return (KEY_TEXT_CLEAR|KEY_CONTROL);
#else /* THROUGH_CONT */
                    return (KEY_NULL);
                } else {
                    return (KEY_NULL);
#endif /* THROUGH_CONT */
                }
            }
            c++;
        }
    }
    if (buf->setnormal) {
        c = buf->setnormal;
        while (*c != '\0') {
            if (*c == code) {
                if (buf->dispmodechange) {
                    buf->dispmode = buf->inputmode;
#ifdef THROUGH_CONT
                    return (KEY_TEXT_FIXED|KEY_CONTROL|KEY_HENKAN_END
                            |KEY_MODE_CHANGE);
                } else {
                    return (KEY_TEXT_FIXED|KEY_CONTROL|KEY_HENKAN_END);
#else /* THROUGH_CONT */
                    return (KEY_NULL);
                } else {
                    return (KEY_NULL);
#endif /* THROUGH_CONT */
                }
            }
            c++;
        }
    }
    /* Through all control events by XtNunusedEventCallback */
    return (KEY_NULL);

    /* Push by compound text */
compound_text:
    switch (buf->convmode) {
    case NoInputModeMask:
        buf->convmode = InputModeMask;
    case InputModeMask:
        if (buf->segnum == buf->curseg)
            buf->segnum++;
        ch[0] = code;
        ch[1] = '\0';
        change_pos = buf->input[buf->curseg]->cur;
        _Xsj3cInsertChar(buf, buf->input[buf->curseg], ch, 1);
        _Xsj3cStoreYomi(buf, buf->input[buf->curseg], change_pos);
        if (buf->dispmodechange) {
            buf->dispmode = buf->inputmode;
            return (KEY_TEXT_FIXED|KEY_TEXT_CHANGE|KEY_MODE_CHANGE);
        } else
            return (KEY_TEXT_FIXED|KEY_TEXT_CHANGE);
    case ConvedModeMask:
        i = buf->input[buf->curseg]->dnum;
        buf->input[buf->curseg]->disp[i++] = code;
        buf->input[buf->curseg]->disp[i] = '\0';
        buf->input[buf->curseg]->dnum++;
        if (buf->dispmodechange) {
            buf->dispmode = buf->inputmode;
            return (KEY_TEXT_FIXED|KEY_TEXT_CHANGE|KEY_MODE_CHANGE);
        } else
            return (KEY_TEXT_FIXED|KEY_TEXT_CHANGE);
    case DictModeMask:
    case SelectModeMask:
    default:
        return (KEY_NULL);
    }
}

/*
 *  _Xsj3cThrough()
 * Store characters to yomi/disp buffer and fix.
 */
static Xsj3cEvent
_Xsj3cThrough(buf, seg, n)
    Xsj3cBuf                buf;
    Xsj3cSeg                seg;
    int                     n;
{
    int                     change_pos;

    change_pos = seg->cur;
    _Xsj3cInsertChar(buf, seg, seg->str, n);
    _Xsj3cStoreYomi(buf, seg, change_pos);
    return (KEY_TEXT_FIXED);
}

/*
 *  _Xsj3cDirect()
 * Store characters to yomi/disp buffer.
 *
 * DisplayModeChange on: Change the display mode string.  
 */
static Xsj3cEvent
_Xsj3cDirect(buf, seg, n)
    Xsj3cBuf                buf;
    Xsj3cSeg                seg;
    int                     n;
{
    Xsj3cEvent              ret = KEY_TEXT_CHANGE;
    int                     change_pos;

    change_pos = seg->cur;
    _Xsj3cInsertChar(buf, seg, seg->str, n);
    _Xsj3cStoreYomi(buf, seg, change_pos);
    *seg->str = '\0';
    seg->sp = seg->str;
    *seg->oldstr = '\0';
    seg->oldlen = 0;
    seg->n_roma = 0;
    seg->n_kana = -1;
    if (buf->dispmodechange) {
        buf->dispmode = (buf->convedsegnum == buf->segnum ? MODE_KANJI :
                (buf->convedsegnum ? MODE_EDIT : buf->inputmode));
        ret |= KEY_MODE_CHANGE;
    }
    return (ret);
}
