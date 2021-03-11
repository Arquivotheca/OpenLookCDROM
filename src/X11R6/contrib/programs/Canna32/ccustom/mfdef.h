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
#define _MFDEF_H_

/* @(#) 102.1 $Id: mfdef.h,v 1.4 1994/01/19 02:33:49 hamada Exp $ */

/*
  ���Υإå��ե�����Ǥϥ⡼�ɤ˳�꿶��줿�ֹ�䡢�ؿ��˳�꿶��줿
  �ֹ��������롣

  */

/* �⡼�� */

/* real modes */
/* �¥⡼��(real mode)�ϥ����ޥåפμ��Τ���äƤ���⡼�� */

#define CANNA_MODE_AlphaMode		0	/* ����ե��٥åȥ⡼�� */
#define CANNA_MODE_EmptyMode		1	/* �ɤ����Ϥ��ʤ����� */
#define CANNA_MODE_KigoMode		2	/* �������ɽ������ */
#define CANNA_MODE_YomiMode		3	/* �ɤ����Ϥ��Ƥ������ */
#define CANNA_MODE_JishuMode		4	/* ʸ�����Ѵ����Ƥ������ */
#define CANNA_MODE_TankouhoMode		5	/* ñ�����ɽ������ */
#define CANNA_MODE_IchiranMode		6	/* �������ɽ������ */
#define CANNA_MODE_YesNoMode		7	/* ñ����Ͽ����ʸɽ������ */
#define CANNA_MODE_OnOffMode		8	/* ñ����Ͽ����ʸɽ������ */
#define CANNA_MODE_AdjustBunsetsuMode   9	/* ʸ�῭�̥⡼�� */
#define CANNA_MODE_ChikujiYomiMode	10	/* �༡�Ѵ��⡼�ɤ��ɤ���ʬ */
#define CANNA_MODE_ChikujiTanMode	11	/* �༡�Ѵ��⡼�ɤθ�����ʬ */

#define CANNA_MODE_MAX_REAL_MODE	(CANNA_MODE_ChikujiTanMode + 1)

/* imaginary modes */
/* ���⡼��(imaginary mode)�ϥ����ޥåפμ��Τ���äƤ��ʤ��⡼�� */

#define CANNA_MODE_HenkanMode		CANNA_MODE_EmptyMode
#define CANNA_MODE_HenkanNyuryokuMode	12

#define CANNA_MODE_ZenHiraHenkanMode	13
#define CANNA_MODE_HanHiraHenkanMode	14
#define CANNA_MODE_ZenKataHenkanMode	15
#define CANNA_MODE_HanKataHenkanMode	16
#define CANNA_MODE_ZenAlphaHenkanMode	17
#define CANNA_MODE_HanAlphaHenkanMode	18

#define CANNA_MODE_ZenHiraKakuteiMode	19
#define CANNA_MODE_HanHiraKakuteiMode	20
#define CANNA_MODE_ZenKataKakuteiMode	21
#define CANNA_MODE_HanKataKakuteiMode	22
#define CANNA_MODE_ZenAlphaKakuteiMode	23
#define CANNA_MODE_HanAlphaKakuteiMode	24

#define CANNA_MODE_HexMode		25	/* �����ʥ��������ϥ⡼�� */
#define CANNA_MODE_BushuMode		26	/* ������ɤߤ����Ͼ��� */
#define CANNA_MODE_ExtendMode		27	/* ��ĥ��ǽ���� */
#define CANNA_MODE_RussianMode		28	/* ����ʸ������ */
#define CANNA_MODE_GreekMode		29	/* ���ꥷ��ʸ������ */
#define CANNA_MODE_LineMode		30	/* �������� */
#define CANNA_MODE_ChangingServerMode	31	/* �������ѹ� */
#define CANNA_MODE_HenkanMethodMode	32	/* �Ѵ��������� */
#define CANNA_MODE_DeleteDicMode	33	/* ñ���� */
#define CANNA_MODE_TourokuMode		34	/* ñ����Ͽ�⡼�� */
#define CANNA_MODE_TourokuEmptyMode	CANNA_MODE_TourokuMode
#define CANNA_MODE_TourokuHinshiMode	35	/* ñ����Ͽ���ʻ�������� */
#define CANNA_MODE_TourokuDicMode	36	/* ñ����Ͽ�μ���������� */
#define CANNA_MODE_QuotedInsertMode	37	/* �������ϥ⡼�� */
#define CANNA_MODE_BubunMuhenkanMode	38	/* ��ʬ̵�Ѵ����� */
#define CANNA_MODE_MountDicMode   	39	/* �����mount,unmount���� */

#define CANNA_MODE_MAX_IMAGINARY_MODE	(CANNA_MODE_MountDicMode + 1)

#ifdef IROHA_BC
/* real modes */
/* �¥⡼��(real mode)�ϥ����ޥåפμ��Τ���äƤ���⡼�� */

#define IROHA_MODE_AlphaMode		CANNA_MODE_AlphaMode
#define IROHA_MODE_EmptyMode		CANNA_MODE_EmptyMode
#define IROHA_MODE_KigoMode		CANNA_MODE_KigoMode
#define IROHA_MODE_YomiMode		CANNA_MODE_YomiMode
#define IROHA_MODE_JishuMode		CANNA_MODE_JishuMode
#define IROHA_MODE_TankouhoMode		CANNA_MODE_TankouhoMode
#define IROHA_MODE_IchiranMode		CANNA_MODE_IchiranMode
#define IROHA_MODE_YesNoMode		CANNA_MODE_YesNoMode
#define IROHA_MODE_OnOffMode		CANNA_MODE_OnOffMode
#define IROHA_MODE_AdjustBunsetsuMode   CANNA_MODE_AdjustBunsetsuMode

#define IROHA_MODE_MAX_REAL_MODE	CANNA_MODE_MAX_REAL_MODE

/* imaginary modes */
/* ���⡼��(imaginary mode)�ϥ����ޥåפμ��Τ���äƤ��ʤ��⡼�� */

#define IROHA_MODE_HenkanMode		CANNA_MODE_HenkanMode
#define IROHA_MODE_HenkanNyuryokuMode	CANNA_MODE_HenkanNyuryokuMode
#define IROHA_MODE_HexMode		CANNA_MODE_HexMode
#define IROHA_MODE_BushuMode		CANNA_MODE_BushuMode
#define IROHA_MODE_ExtendMode		CANNA_MODE_ExtendMode
#define IROHA_MODE_RussianMode		CANNA_MODE_RussianMode
#define IROHA_MODE_GreekMode		CANNA_MODE_GreekMode
#define IROHA_MODE_LineMode		CANNA_MODE_LineMode
#define IROHA_MODE_ChangingServerMode	CANNA_MODE_ChangingServerMode
#define IROHA_MODE_HenkanMethodMode	CANNA_MODE_HenkanMethodMode
#define IROHA_MODE_DeleteDicMode	CANNA_MODE_DeleteDicMode
#define IROHA_MODE_TourokuMode		CANNA_MODE_TourokuMode
#define IROHA_MODE_TourokuEmptyMode	CANNA_MODE_TourokuEmptyMode
#define IROHA_MODE_TourokuHinshiMode	CANNA_MODE_TourokuHinshiMode
#define IROHA_MODE_TourokuDicMode	CANNA_MODE_TourokuDicMode
#define IROHA_MODE_QuotedInsertMode	CANNA_MODE_QuotedInsertMode
#define IROHA_MODE_BubunMuhenkanMode	CANNA_MODE_BubunMuhenkanMode
#define IROHA_MODE_MountDicMode   	CANNA_MODE_MountDicMode

#define IROHA_MODE_MAX_IMAGINARY_MODE	CANNA_MODE_MAX_IMAGINARY_MODE

#endif /* IROHA_BC */

/* �����ؿ� */

/* ̤����ˤ��� */

#define DEFAULTBEHAVIOR 0
#define CANNA_FN_Undefined		0 /* ���⤷�ʤ�/�����򥹥롼���̤� */

/* ʸ������ */

#define CANNA_FN_SelfInsert		1 /* ��ʸ���������� */
#define CANNA_FN_FunctionalInsert	2 /* ���޻������Ѵ��ʤ� */
#define CANNA_FN_QuotedInsert		3 /* �������� */

/* ����Ū�⡼���ڤ괹�� */

#define CANNA_FN_JapaneseMode		4 /* ���ܸ����ϥ⡼�� */
#define CANNA_FN_AlphaMode		5 /* ����ե��٥å����ϥ⡼�� */
#define CANNA_FN_HenkanNyuryokuMode	6 /* �Ѵ����ϥ⡼�� */

/* �Խ� */

#define CANNA_FN_Forward		7 /* ���� */
#define CANNA_FN_Backward		8 /* ���� */
#define CANNA_FN_Next			9 /* ���ι� */
#define CANNA_FN_Prev			10 /* ���ι� */
#define CANNA_FN_BeginningOfLine	11 /* ��Ƭ */
#define CANNA_FN_EndOfLine		12 /* ���� */
#define CANNA_FN_DeleteNext		13 /* ��� */
#define CANNA_FN_DeletePrevious		14 /* ��� */
#define CANNA_FN_KillToEndOfLine	15 /* �����ޤǺ�� */

/* ���ʴ����Ѵ���ǽ */

#define CANNA_FN_Henkan			16 /* �Ѵ� */
#define CANNA_FN_Kakutei		17 /* ���� */
#define CANNA_FN_Extend			18 /* ���Ф� */
#define CANNA_FN_Shrink			19 /* �̤� */
#define CANNA_FN_AdjustBunsetsu		20 /* ʸ�῭�̥⡼�ɤ����� */
#define CANNA_FN_Quit			21 /* ����� */
#define CANNA_FN_ConvertAsHex		22 /* �����ʥ����ɤȤ����Ѵ� */
#define CANNA_FN_ConvertAsBushu		23 /* ����̾�Ȥ����Ѵ� */
#define CANNA_FN_KouhoIchiran		24 /* ������� */
#define CANNA_FN_BubunMuhenkan		25 /* ��ʬ̵�Ѵ� */

/* ʸ�����Ѵ������˻Ȥ��뵡ǽ */

#define CANNA_FN_Zenkaku		26 /* ���Ѥؤ��Ѵ� */
#define CANNA_FN_Hankaku		27 /* Ⱦ�Ѥؤ��Ѵ� */
#define CANNA_FN_ToUpper		28 /* ��ʸ���ؤ��Ѵ� */
#define CANNA_FN_Capitalize		29 /* ��Ƭ������ʸ���ˤ��� */
#define CANNA_FN_ToLower		30 /* ��ʸ���ؤ��Ѵ� */
#define CANNA_FN_Hiragana		31 /* �Ҥ餬���Ѵ� */
#define CANNA_FN_Katakana		32 /* ���������Ѵ� */
#define CANNA_FN_Romaji			33 /* ���޻��Ѵ� */

/* �ɤߥ⡼�ɤǤΥ١���ʸ�����ڤ��ؤ� */

#define CANNA_FN_BaseHiragana		34 /* �١���ʸ����Ҥ餬�ʤ� */
#define CANNA_FN_BaseKatakana		35 /* �١���ʸ���򥫥����ʤ� */
#define CANNA_FN_BaseEisu		36 /* �١���ʸ����ѿ��� */
#define CANNA_FN_BaseZenkaku		37 /* �١���ʸ�������Ѥ� */
#define CANNA_FN_BaseHankaku		38 /* �١���ʸ����Ⱦ�Ѥ� */
#define CANNA_FN_BaseKana		39 /* �١���ʸ���򤫤ʤ� */
#define CANNA_FN_BaseKakutei		40 /* ����١����� */
#define CANNA_FN_BaseHenkan		41 /* �Ѵ��١����� */
#define CANNA_FN_BaseHiraKataToggle	42 /* �Ҥ餬��/�������ʤǥȥ��뤹�� */
#define CANNA_FN_BaseZenHanToggle	43 /* ����/Ⱦ�Ѥǥȥ��뤹�� */
#define CANNA_FN_BaseKanaEisuToggle	44 /* ����/�ѿ��ǥȥ��뤹�� */
#define CANNA_FN_BaseKakuteiHenkanToggle 45 /* ����/�Ѵ��ǥȥ��뤹�� */
#define CANNA_FN_BaseRotateForward	46 /* �١���ʸ����緫����ڤ��ؤ��� */
#define CANNA_FN_BaseRotateBackward	47 /* �١���ʸ����շ�����ڤ��ؤ��� */

/* ����¾�Υ⡼���ڤ��ؤ� */

#define CANNA_FN_ExtendMode		48 /* ��ĥ�⡼�� */
#define CANNA_FN_Touroku                CANNA_FN_ExtendMode /* ��ߴ� */

#define CANNA_FN_HexMode		49 /* ���������ϥ⡼�� */
#define CANNA_FN_BushuMode		50 /* �������ϥ⡼�� */
#define CANNA_FN_KigouMode		51 /* �������ϥ⡼�� */

#define CANNA_FN_ZenHiraKakuteiMode	52 /* ���ѤҤ餬�ʳ������ϥ⡼�� */
#define CANNA_FN_ZenKataKakuteiMode	53 /* ���ѥ������ʳ������ϥ⡼�� */
#define CANNA_FN_HanKataKakuteiMode	54 /* Ⱦ�ѥ������ʳ������ϥ⡼�� */
#define CANNA_FN_ZenAlphaKakuteiMode	55 /* ���ѥ���ե��٥åȳ������ϥ� */
#define CANNA_FN_HanAlphaKakuteiMode	56 /* Ⱦ�ѥ���ե��٥åȳ������ϥ� */

#define CANNA_FN_HenkanOrInsert	        57
#define CANNA_FN_HenkanOrNothing        58
#define CANNA_FN_ChangeServerMode       59
#define CANNA_FN_DisconnectServer       60
#define CANNA_FN_ShowServer             61
#define CANNA_FN_ShowGakushu            62
#define CANNA_FN_ShowVersion            63
#define CANNA_FN_ShowPhonogramFile      64
#define CANNA_FN_ShowCannaFile          65
#define CANNA_FN_SyncDic                66


/* ʣ���ʵ�ǽ */

#define CANNA_FN_FuncSequence		67 /* ʣ���ε�ǽ�γ������ */
#define CANNA_FN_UseOtherKeymap		68 /* �����������󥹤��б� */


#define CANNA_FN_MAX_FUNC		(CANNA_FN_UseOtherKeymap + 1)

#ifdef IROHA_BC

#define IROHA_FN_Undefined		CANNA_FN_Undefined

/* ʸ������ */

#define IROHA_FN_SelfInsert		CANNA_FN_SelfInsert
#define IROHA_FN_FunctionalInsert	CANNA_FN_FunctionalInsert
#define IROHA_FN_QuotedInsert		CANNA_FN_QuotedInsert

/* ����Ū�⡼���ڤ괹�� */

#define IROHA_FN_JapaneseMode		CANNA_FN_JapaneseMode
#define IROHA_FN_AlphaMode		CANNA_FN_AlphaMode
#define IROHA_FN_HenkanNyuryokuMode	CANNA_FN_HenkanNyuryokuMode


/* �Խ� */

#define IROHA_FN_Forward		CANNA_FN_Forward
#define IROHA_FN_Backward		CANNA_FN_Backward
#define IROHA_FN_Next			CANNA_FN_Next
#define IROHA_FN_Prev			CANNA_FN_Prev
#define IROHA_FN_BeginningOfLine	CANNA_FN_BeginningOfLine
#define IROHA_FN_EndOfLine		CANNA_FN_EndOfLine
#define IROHA_FN_DeleteNext		CANNA_FN_DeleteNext
#define IROHA_FN_DeletePrevious		CANNA_FN_DeletePrevious
#define IROHA_FN_KillToEndOfLine	CANNA_FN_KillToEndOfLine

/* ���ʴ����Ѵ���ǽ */

#define IROHA_FN_Henkan			CANNA_FN_Henkan
#define IROHA_FN_Kakutei		CANNA_FN_Kakutei
#define IROHA_FN_Extend			CANNA_FN_Extend
#define IROHA_FN_Shrink			CANNA_FN_Shrink
#define IROHA_FN_AdjustBunsetsu		CANNA_FN_AdjustBunsetsu
#define IROHA_FN_Quit			CANNA_FN_Quit
#define IROHA_FN_ConvertAsHex		CANNA_FN_ConvertAsHex
#define IROHA_FN_ConvertAsBushu		CANNA_FN_ConvertAsBushu
#define IROHA_FN_KouhoIchiran		CANNA_FN_KouhoIchiran
#define IROHA_FN_BubunMuhenkan		CANNA_FN_BubunMuhenkan

/* ʸ�����Ѵ������˻Ȥ��뵡ǽ */

#define IROHA_FN_Zenkaku		CANNA_FN_Zenkaku
#define IROHA_FN_Hankaku		CANNA_FN_Hankaku
#define IROHA_FN_ToUpper		CANNA_FN_ToUpper
#define IROHA_FN_Capitalize		CANNA_FN_Capitalize
#define IROHA_FN_ToLower		CANNA_FN_ToLower
#define IROHA_FN_Hiragana		CANNA_FN_Hiragana
#define IROHA_FN_Katakana		CANNA_FN_Katakana
#define IROHA_FN_Romaji			CANNA_FN_Romaji

/* ����¾�Υ⡼���ڤ��ؤ� */

#define IROHA_FN_ExtendMode		CANNA_FN_ExtendMode
#define IROHA_FN_Touroku                CANNA_FN_Touroku

#define IROHA_FN_HexMode		CANNA_FN_HexMode
#define IROHA_FN_BushuMode		CANNA_FN_BushuMode
#define IROHA_FN_KigouMode		CANNA_FN_KigouMode

#define IROHA_FN_UserMode		CANNA_FN_UserMode

/* ʣ���ʵ�ǽ */

#define IROHA_FN_FuncSequence		CANNA_FN_FuncSequence
#define IROHA_FN_UseOtherKeymap		CANNA_FN_UseOtherKeymap

#define IROHA_FN_MAX_FUNC		CANNA_FN_MAX_FUNC

#endif /* IROHA_BC */

#endif /* _MFDEF_H_ */
