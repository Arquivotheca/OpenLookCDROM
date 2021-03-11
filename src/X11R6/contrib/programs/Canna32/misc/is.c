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

/* 

  ���Υץ����ϥ��������ץ������󥹤����Ϥ���� XFER������������F1��
  F2��PF1��PF2��HELP���ʤɤΥ�����Ŭ���ʣɣĤ��Ѵ����뤿��Υץ����
  �Ǥ��롣ɽ��������Ƥ����� initIS ��Ƥ٤С����Ȥ� identifySequence 
  ��Ȥ����Ȥˤ�ꥨ�������ץ������󥹤�ǧ�����뤳�Ȥ��Ǥ��롣

  �ܺ٤λȤ����� main() �򻲹ͤˤ����ߤ��������Υե������ñ�Τǥ����
  ���뤷�¹Բ�ǽ�Ǥ��롣a.out ��ɸ�����Ϥ��饨�������ץ������󥹤�����
  �����ؤ���ʡ٤��Ѥ��륭���ɣĤ�ɽ�����롣ñ�Τǥ���ѥ��뤹��Ȥ��� 
  USED_AS_LIBRARY ������򥳥��ȥ����Ȥ�����ɤ���

  $Id: is.c,v 1.10 1992/07/28 13:21:50 kon Exp $
*/

/* #define USED_AS_LIBRARY /* �饤�֥��Ȥ��ƥ���ѥ��뤹�� */

#include <canna/keydef.h>

typedef struct {
  int id;
  char *seq;
} SeqToID;

static SeqToID keywordtable[] = {
  {CANNA_KEY_Nfer,	"&U51"	}, /* nfer		*/
  {CANNA_KEY_PF1,	"&U52"	}, /* PF1		*/
  {CANNA_KEY_PF2,	"&U53"	}, /* PF2		*/
  {CANNA_KEY_PF3,	"&U54"	}, /* PF3		*/
  {CANNA_KEY_PF4,	"&U55"	}, /* PF4		*/
  
  {CANNA_KEY_Up,	"[A"	}, /* ��		*/
  {CANNA_KEY_Down,	"[B"	}, /* ��		*/
  {CANNA_KEY_Right,	"[C"	}, /* ��		*/
  {CANNA_KEY_Left,	"[D"	}, /* ��		*/
  /* ddy880 */
  {CANNA_KEY_Up,	"OA"	}, /* ��		*/
  {CANNA_KEY_Down,	"OB"	}, /* ��		*/
  {CANNA_KEY_Right,	"OC"	}, /* ��		*/
  {CANNA_KEY_Left,	"OD"	}, /* ��		*/
  {CANNA_KEY_PF1,	"OP"	}, /* PF1		*/
  {CANNA_KEY_PF2,	"OQ"	}, /* PF2		*/
  {CANNA_KEY_PF3,	"OR"	}, /* PF3		*/
  {CANNA_KEY_PF4,	"OS"	}, /* PF4		*/
  {CANNA_KEY_F1,	"[11~"	}, /* F1		*/
  {CANNA_KEY_F2,	"[12~"	}, /* F2		*/
  {CANNA_KEY_F3,	"[13~"	}, /* F3		*/
  {CANNA_KEY_F4,	"[14~"	}, /* F4		*/
  {CANNA_KEY_F5,	"[15~"	}, /* F5		*/
  {CANNA_KEY_F6,	"[17~"	}, /* F6		*/
  {CANNA_KEY_F7,	"[18~"	}, /* F7		*/
  {CANNA_KEY_F8,	"[19~"	}, /* F8		*/
  {CANNA_KEY_F9,	"[20~"	}, /* F9		*/
  {CANNA_KEY_F10,	"[21~"	}, /* F10		*/
  {CANNA_KEY_Nfer,	"[209z"	}, /* nfer		*/
  {CANNA_KEY_Xfer,	"[210z"	}, /* xfer		*/
  {CANNA_KEY_Insert,	"[2~"	}, /* insert		*/
  {CANNA_KEY_Insert,	"[193z"	}, /* insert		*/
  {CANNA_KEY_Help,	"[28~"	}, /* help		*/
  {CANNA_KEY_Help,	"[197z"	}, /* help		*/
  {CANNA_KEY_Rollup,	"[5~"	}, /* Roll Up		*/
  {CANNA_KEY_Rollup,	"[194z"	}, /* Roll Up		*/
  {CANNA_KEY_Rolldown,	"[6~"	}, /* Roll Down		*/
  {CANNA_KEY_Rolldown,	"[195z"	}, /* Roll Down		*/
  {0,			0	},
};

#define charToNum(c) charToNumTbl[(c) - ' ']

static int *charToNumTbl;
static int longestkeywordlen;

typedef struct {
  int id;
  int *tbl;
} seqlines;

static seqlines *seqTbl;	/* ������ɽ(�ºݤˤ�ɽ��ɽ) */
static int nseqtbl;		/* ���֤ο������֤ο�����ɽ������ */
static int nseq;
static int seqline;

/* cfuncdef

  initIS -- �����������󥹤��®�˥�����󤹤뤿��Υơ��֥����

  ��®�ơ��֥�� seqTbl �Υݥ��󥿤Ȥ��ƺ�������롣�ޤ�������饯����
  ������ؤ��Ѵ��ѤΥơ��֥����������(charToNumTbl)��

  ����ϥǡ��� SeqToID ���˽��������롣

  ��®�ơ��֥�Ͼ��֤����Ϥ��줿�����Υ����ɤ��鼡�ξ��֤����뤿��Υơ�
  �֥�Ǥ��롣�������󥹤�����ä����ˡ����λ��ξ��֤� id ���Фˤ���
  �������󥹤��б����륭���μ��̻Ҥ����äƤ��롣���ξ��֤��������Ƥ�
  �ʤ�(�������äƤ���)�Ȥ��ϥ������󥹤�����äƤ��롣���ΤȤ��� id ��
  ��У������äƤ�����ϥ��꡼����ʥ������󥹤Ǥ��롣

 */

static
initIS()
{
  SeqToID *p;
  char *s;
  int i;
  seqlines seqTbls[1024];

  seqTbl = (seqlines *)0;
  seqline = 0;
  nseqtbl = 0;
  nseq = 0;
  longestkeywordlen = 0;
  for (i = 0 ; i < 1024 ; i++) {
    seqTbls[i].tbl = (int *)0;
    seqTbls[i].id = 0;
  }
  charToNumTbl = (int *)calloc('~' - ' ' + 1, sizeof(int));
  if ( !charToNumTbl ) {
    return 0;
  }

  /* �ޤ���ʸ���Ȥ��Ƥ��뤫��Ĵ�٤롣
     nseq �ϻȤ��Ƥ���ʸ������꣱�礭���ͤǤ��� */
  for (p = keywordtable ; p->id ; p++) {
    int len = 0;
    for (s = p->seq ; *s ; s++) {
      if ( !charToNumTbl[*s - ' '] ) {
	charToNumTbl[*s - ' '] = nseq; /* ��ʸ���˥��ꥢ���ֹ�򿶤� */
	nseq++;
      }
      len ++;
    }
    if (len > longestkeywordlen) {
      longestkeywordlen = len;
    }
  }
  /* ʸ����ʬ�Υơ��֥� */
  seqTbls[nseqtbl].tbl = (int *)calloc(nseq, sizeof(int));
  if ( !seqTbls[nseqtbl].tbl ) {
    goto initISerr;
  }
  nseqtbl++;
  for (p = keywordtable ; p->id ; p++) {
    int line, nextline;
    
    line = 0;
    for (s = p->seq ; *s ; s++) {
      if (seqTbls[line].tbl == 0) { /* �ơ��֥뤬�ʤ� */
	seqTbls[line].tbl = (int *)calloc(nseq, sizeof(int));
	if ( !seqTbls[line].tbl ) {
	  goto initISerr;
	}
      }
      nextline = seqTbls[line].tbl[charToNum(*s)];
      /* ���ʤߤˡ�charToNum(*s) �����Фˣ��ˤʤ�ʤ� */
      if ( nextline ) {
	line = nextline;
      }
      else { /* �ǽ�˥����������� */
	line = seqTbls[line].tbl[charToNum(*s)] = nseqtbl++;
      }
    }
    seqTbls[line].id = p->id;
  }
  seqTbl = (seqlines *)calloc(nseqtbl, sizeof(seqlines));
  if ( !seqTbl ) {
    goto initISerr;
  }
  for (i = 0 ; i < nseqtbl ; i++) {
    seqTbl[i].id  = seqTbls[i].id;
    seqTbl[i].tbl = seqTbls[i].tbl;
  }
  return 1;

 initISerr:
  free(charToNumTbl);
  charToNumTbl = (int *)0;
  if (seqTbl) {
    free(seqTbl);
    seqTbl = (seqlines *)0;
  }
  for (i = 0 ; i < nseqtbl ; i++) {
    if (seqTbls[i].tbl) {
      free(seqTbls[i].tbl);
      seqTbls[i].tbl = (int *)0;
    }
  }
  return 0;
}

static
finIS() /* identifySequence ���Ѥ�������񸻤������� */
{
  int i;

  for (i = 0 ; i < nseqtbl ; i++) {
    if (seqTbl[i].tbl) free(seqTbl[i].tbl);
    seqTbl[i].tbl = (int *)0;
  }
  free(seqTbl);
  seqTbl = (seqlines *)0;
  free(charToNumTbl);
  charToNumTbl = (int *)0;
}

/* cvariable

  seqline: identifySequence �Ǥξ��֤��ݻ������ѿ�

 */

#define CONTINUE 1
#define END	 0

static
identifySequence(c, val)
char c;
int *val;
{
  int nextline;

  if (charToNum(c) &&
      (nextline = seqTbl[seqline].tbl[charToNum(c)]) ) {
    seqline = nextline;
    if (*val = seqTbl[seqline].id) {
      seqline = 0;
      return END;
    }
    else {
      return CONTINUE; /* continue */
    }
  }
  else {
    *val = -1;
    seqline = 0;
    return END;
  }
}

#ifndef USED_AS_LIBRARY
main()
{
  int c, code;

  initIS(2); /* 2 �� VT ����ѥ������ߥʥ� */

  c = getchar();

  while (c != -1) {
    if (c == 0x1b) {
      char foo[8];
      int n = 1;
      int res;

      foo[0] = 0x1b;
      do {
	foo[n++] = c = getchar();
	res = identifySequence(c, &code);
      } while (res == CONTINUE);
      if (code != -1) {
	printf("<0x%x>", code);
      }
      else {
	int i;

	for (i = 0 ; i < n ; i++) {
	  printchar(foo[i]);
	}
      }
    }
    else {
      printchar(c);
    }
    c = getchar();
  }
  finIS();
}

static
printchar(c)
char c;
{
  if (c < 0x20) {
    if (c == '\n' || c == '\t' || c == '\b') {
      putchar(c);
    }
    else if (c == 0x1b) {
      printf("\\e");
    }
    else {
      printf("^%c", c + '@');
    }
  }
  else {
    putchar(c);
  }
}
#endif /* USED_AS_LIBRARY */
