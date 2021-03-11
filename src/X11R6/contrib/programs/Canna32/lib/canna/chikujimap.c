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

#if !defined(lint) && !defined(__CODECENTER__)
static char chikuji_id[] = "$Id: chikujimap.c,v 5.9 1994/04/21 02:44:11 kon Exp $";
#endif

#define BUNPOU_DISPLAY

extern TanKouhoIchiran(), TanKatakana();
extern TanPrintBunpou();
extern TanMuhenkan(), TanBubunKakutei(), TanHankaku();
extern TanHiragana(), TanRomaji(), TanZenkaku();
extern TanForwardBunsetsu(), TanBackwardBunsetsu();
extern TanNextKouho(), TanPreviousKouho();
extern TanBeginningOfBunsetsu(), TanEndOfBunsetsu(), TanBubunMuhenkan();
extern TanUpper(), TanCapitalize(), TanDeletePrevious();

#define NONE CANNA_FN_Undefined

/* CYsearchfunc ���Ȥ���Τǰʲ���ɽ�˲ä��� yomi_mode �Υޥåפ�Ȥ��� */

static struct funccfunc cy_funcs[] = {
  {CANNA_FN_FunctionalInsert	, YomiInsert/* �������פ�ʤ� */},
  {CANNA_FN_DeletePrevious	, ChikujiYomiDeletePrevious	},
  {CANNA_FN_Henkan		, ChikujiHenkan			},
  {CANNA_FN_HenkanOrInsert	, ChikujiHenkanNaive		},
  {CANNA_FN_HenkanOrNothing	, ChikujiHenkanOrNothing	},
  {CANNA_FN_Quit		, ChikujiMuhenkan		},
  {0				, 0				},
};

extern searchfunc(), CYsearchfunc(), Yomisearchfunc();
extern BYTE default_kmap[];

KanjiModeRec cy_mode = {
  CYsearchfunc,
  default_kmap,
  CANNA_KANJIMODE_TABLE_SHARED,
  cy_funcs,
};

static struct funccfunc cb_funcs[] = {
  {CANNA_FN_FunctionalInsert	, YomiInsert/* �������פ�ʤ� */},
  {CANNA_FN_Forward		, TanForwardBunsetsu		},
  {CANNA_FN_Backward		, TanBackwardBunsetsu		},
  {CANNA_FN_Next		, TanNextKouho			},
  {CANNA_FN_Prev		, TanPreviousKouho		},
  {CANNA_FN_BeginningOfLine	, TanBeginningOfBunsetsu	},
  {CANNA_FN_EndOfLine		, TanEndOfBunsetsu		},
  {CANNA_FN_DeletePrevious	, ChikujiTanDeletePrevious	},
  {CANNA_FN_KillToEndOfLine	, TanBubunKakutei		},
  {CANNA_FN_Henkan		, ChikujiHenkan			},
  {CANNA_FN_HenkanOrInsert	, ChikujiHenkanNaive		},
  {CANNA_FN_HenkanOrNothing	, ChikujiHenkanOrNothing	},
  {CANNA_FN_Quit		, TanMuhenkan			},
  {CANNA_FN_Extend		, ChikujiTanExtend		},
  {CANNA_FN_Shrink		, ChikujiTanShrink		},
/*  {CANNA_FN_AdjustBunsetsu	, TanBunsetsuMode		}, */
  {CANNA_FN_KouhoIchiran	, TanKouhoIchiran		},
  {CANNA_FN_BubunMuhenkan	, TanBubunMuhenkan		},
  {CANNA_FN_Zenkaku		, TanZenkaku			},
  {CANNA_FN_Hankaku		, TanHankaku			},
  {CANNA_FN_Hiragana		, TanHiragana			},
  {CANNA_FN_Katakana		, TanKatakana			},
  {CANNA_FN_Romaji		, TanRomaji			},
  {CANNA_FN_ToUpper		, TanUpper			},
  {CANNA_FN_ToLower		, TanRomaji			},
  {CANNA_FN_Capitalize		, TanCapitalize			},
#ifdef BUNPOU_DISPLAY
  {CANNA_FN_ConvertAsHex	,TanPrintBunpou			},
#endif
  {0				, 0				},
};

KanjiModeRec cb_mode = {
  CYsearchfunc,
  default_kmap,
  CANNA_KANJIMODE_TABLE_SHARED,
  cb_funcs
};
