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

#ifndef _MFDEF_H_

/* @(#) 102.1 $Id: irmfdef.h,v 1.6 1992/07/28 12:15:18 kon Exp $ */

/*
  ���Υإå��ե�����Ǥϥ⡼�ɤ˳�꿶��줿�ֹ�䡢�ؿ��˳�꿶��줿
  �ֹ��������롣

  */

/* 
 * ���ʴ����Ѵ��Τ�����͡��ʥ����ޥåץơ��֥� 
 * �����ޥåץơ��֥�Ͻ����ؿ��ؤΥݥ��󥿤�����ȤʤäƤ��롣
 */

typedef struct {
  int (*func)();
  unsigned char *keytbl;
  int flags;			/* ���򸫤� */
  int (**ftbl)();
} *KanjiMode, KanjiModeRec;

/* flags �ξ��� */
#define IROHA_KANJIMODE_TABLE_SHARED	01
#define IROHA_KANJIMODE_EMPTY_MODE	02

extern unsigned char default_kmap[];
extern int  simplefunc();
extern int  multiSequenceFunc();

/* �⡼�� */

/* real modes */
/* �¥⡼��(real mode)�ϥ����ޥåפμ��Τ���äƤ���⡼�� */

#define IROHA_MODE_AlphaMode		0	/* ����ե��٥åȥ⡼�� */
#define IROHA_MODE_EmptyMode		1	/* �ɤ����Ϥ��ʤ����� */
#define IROHA_MODE_KigoMode		2	/* �������ɽ������ */
#define IROHA_MODE_ZenHiraKakuteiMode	3	/* �������ϥ⡼�� */
#define IROHA_MODE_ZenKataKakuteiMode	4	/* �������ϥ⡼�� */
#define IROHA_MODE_HanKataKakuteiMode	5	/* �������ϥ⡼�� */
#define IROHA_MODE_ZenAlphaKakuteiMode	6	/* �������ϥ⡼�� */
#define IROHA_MODE_HanAlphaKakuteiMode	7	/* �������ϥ⡼�� */
#define IROHA_MODE_YomiMode		8	/* �ɤ����Ϥ��Ƥ������ */
#define IROHA_MODE_JishuMode		9	/* ʸ�����Ѵ����Ƥ������ */
#define IROHA_MODE_TankouhoMode		10	/* ñ�����ɽ������ */
#define IROHA_MODE_IchiranMode		11	/* �������ɽ������ */
#define IROHA_MODE_YesNoMode		12	/* ñ����Ͽ����ʸɽ������ */
#define IROHA_MODE_OnOffMode		13	/* ñ����Ͽ����ʸɽ������ */

#define IROHA_MODE_MAX_REAL_MODE	IROHA_MODE_OnOffMode + 1

/* imaginary modes */
/* ���⡼��(imaginary mode)�ϥ����ޥåפμ��Τ���äƤ��ʤ��⡼�� */

#define IROHA_MODE_HenkanMode		IROHA_MODE_EmptyMode
#define IROHA_MODE_ZenHiraHenkanMode	IROHA_MODE_EmptyMode
#define IROHA_MODE_ZenKataHenkanMode	14	/* �Ѵ����ϥ⡼�� */
#define IROHA_MODE_HanKataHenkanMode	15	/* �Ѵ����ϥ⡼�� */
#define IROHA_MODE_ZenAlphaHenkanMode	16	/* �Ѵ����ϥ⡼�� */
#define IROHA_MODE_HanAlphaHenkanMode	17	/* �Ѵ����ϥ⡼�� */
#define IROHA_MODE_HexMode		18	/* �����ʥ��������ϥ⡼�� */
#define IROHA_MODE_BushuMode		19	/* ������ɤߤ����Ͼ��� */
#define IROHA_MODE_ExtendMode		20	/* ��ĥ��ǽ���� */
#define IROHA_MODE_RussianMode		21	/* ����ʸ������ */
#define IROHA_MODE_GreekMode		22	/* ���ꥷ��ʸ������ */
#define IROHA_MODE_LineMode		23	/* �������� */
#define IROHA_MODE_ChangingServerMode	24	/* �������ѹ� */
#define IROHA_MODE_HenkanMethodMode	25	/* �Ѵ��������� */
#define IROHA_MODE_DeleteDicMode	26	/* ñ���� */
#define IROHA_MODE_TourokuMode		27	/* ñ����Ͽ�⡼�� */
#define IROHA_MODE_TourokuEmptyMode	IROHA_MODE_TourokuMode
#define IROHA_MODE_TourokuHinshiMode	28	/* ñ����Ͽ���ʻ�������� */
#define IROHA_MODE_TourokuDicMode	29	/* ñ����Ͽ�μ���������� */
#define IROHA_MODE_QuotedInsertMode	30	/* �������ϥ⡼�� */
#define IROHA_MODE_BubunMuhenkanMode	31	/* ��ʬ̵�Ѵ����� */
#define IROHA_MODE_MountDicMode   	32	/* �����mount,unmount���� */

#define IROHA_MODE_MAX_IMAGINARY_MODE	IROHA_MODE_MountDicMode + 1


/* �����ؿ� */

/* ̤����ˤ��� */

#define IROHA_FN_Undefined		0 /* ���⤷�ʤ� or �����򥹥롼���̤� */

/* ʸ������ */

#define IROHA_FN_SelfInsert		1 /* ��ʸ���������� */
#define IROHA_FN_FunctionalInsert	2 /* ���޻������Ѵ��ʤ� */
#define IROHA_FN_QuotedInsert		3 /* �������� */

/* �⡼���ڤ괹�� */

#define IROHA_FN_JapaneseMode		4 /* ���ܸ����ϥ⡼�� */
#define IROHA_FN_AlphaMode		5 /* ����ե��٥å����ϥ⡼�� */
#define IROHA_FN_HenkanNyuryokuMode	6 /* �Ѵ����ϥ⡼�� */
#define IROHA_FN_ZenHiraKakuteiMode	7 /* ���ѤҤ餬�ʳ������ϥ⡼�� */
#define IROHA_FN_ZenKataKakuteiMode	8 /* ���ѥ������ʳ������ϥ⡼�� */
#define IROHA_FN_HanKataKakuteiMode	9 /* Ⱦ�ѥ������ʳ������ϥ⡼�� */
#define IROHA_FN_ZenAlphaKakuteiMode	10 /* ���ѥ���ե��٥åȳ������ϥ� */
#define IROHA_FN_HanAlphaKakuteiMode	11 /* Ⱦ�ѥ���ե��٥åȳ������ϥ� */

#define IROHA_FN_HexMode		12 /* ���������ϥ⡼�� */
#define IROHA_FN_BushuMode		13 /* �������ϥ⡼�� */
#define IROHA_FN_KigouMode		14 /* �������ϥ⡼�� */

/* �Խ� */

#define IROHA_FN_Forward		15 /* ���� */
#define IROHA_FN_Backward		16 /* ���� */
#define IROHA_FN_Next			17 /* ���ι� */
#define IROHA_FN_Prev			18 /* ���ι� */
#define IROHA_FN_BeginningOfLine	19 /* ��Ƭ */
#define IROHA_FN_EndOfLine		20 /* ���� */
#define IROHA_FN_DeleteNext		21 /* ��� */
#define IROHA_FN_DeletePrevious		22 /* ��� */
#define IROHA_FN_KillToEndOfLine	23 /* �����ޤǺ�� */

/* ���ʴ����Ѵ���ǽ */

#define IROHA_FN_Henkan			24 /* �Ѵ� */
#define IROHA_FN_Kakutei		25 /* ���� */
#define IROHA_FN_Extend			26 /* ���Ф� */
#define IROHA_FN_Shrink			27 /* �̤� */
#define IROHA_FN_Quit			28 /* ����� */
#define IROHA_FN_ExtendMode		29 /* ��ĥ�⡼�� */
#define IROHA_FN_Touroku                29 /* ��Ͽ */
#define IROHA_FN_ConvertAsHex		30 /* �����ʥ����ɤȤ����Ѵ� */
#define IROHA_FN_ConvertAsBushu		31 /* ����̾�Ȥ����Ѵ� */
#define IROHA_FN_KouhoIchiran		32 /* ������� */
#define IROHA_FN_BubunMuhenkan		33 /* ��ʬ̵�Ѵ� */

/* ʸ�����Ѵ������˻Ȥ��뵡ǽ */

#define IROHA_FN_Zenkaku		34 /* ���Ѥؤ��Ѵ� */
#define IROHA_FN_Hankaku		35 /* Ⱦ�Ѥؤ��Ѵ� */
#define IROHA_FN_ToUpper		36 /* ��ʸ���ؤ��Ѵ� */
#define IROHA_FN_Capitalize		37 /* ��Ƭ������ʸ���ˤ��� */
#define IROHA_FN_ToLower		38 /* ��ʸ���ؤ��Ѵ� */
#define IROHA_FN_Hiragana		39 /* �Ҥ餬���Ѵ� */
#define IROHA_FN_Katakana		40 /* ���������Ѵ� */
#define IROHA_FN_Romaji			41 /* ���޻��Ѵ� */

/* ʣ���ʵ�ǽ */

#define IROHA_FN_FuncSequence		42 /* ʣ���ε�ǽ�γ������ */
#define IROHA_FN_UseOtherKeymap		43 /* �����������󥹤��б� */

#define IROHA_FN_MAX_FUNC		IROHA_FN_UseOtherKeymap + 1

#define IROHA_FN_NextKouho               44
#define IROHA_FN_PrevKouho               45

#define _MFDEF_H_
#endif /* _MFDEF_H_ */
