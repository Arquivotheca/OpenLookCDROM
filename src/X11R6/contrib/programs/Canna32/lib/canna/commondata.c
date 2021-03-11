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
static char rcs_id[] = "@(#) 102.1 $Id: commondata.c,v 7.14 1994/04/12 12:55:20 kon Exp $";
#endif /* lint */

#include "canna.h"
#include <canna/mfdef.h>
#include "patchlevel.h"
     
/* ���Ρؤ���ʡ٤ΥС������ */

int CannaVersion = CANNA_MAJOR_MINOR;

/* �ǥե���ȤΥ��޻������Ѵ��ѤΥХåե� */

int defaultContext = -1;
int defaultBushuContext = -1;

/* ���޻������Ѵ����� */
/*
 * ���޻������Ѵ��ơ��֥�ϣ��Ĥ�����ɤ��Ǥ��礦��ʣ����ɬ�פʤΤ�
 * ����� RomeStruct �Υ��ФȤ�������Ƥ���ɬ�פ⤢��ޤ��礦��...��
 * �λ��Ϥ��λ��ǹͤ��ޤ��礦��
 */
     
struct RkRxDic *romajidic, *englishdic, *RkwOpenRoma();

/* ̤��������Ǹ����ν����Τ����� */

int howToBehaveInCaseOfUndefKey = kc_normal;

/*
 * �����̾��������Ƥ����ѿ�
 */

char saveapname[CANNA_MAXAPPNAME]; /* �����ФȤ���³���ڤ�Ȥ���AP̾ */

/*
 * ���޻������Ѵ��򤷤Ĥ������ɤ����Υե饰
 */

int forceKana = 0;

/*
 * ��������λ��˲����ܤθ��䤫��ɽ�����뤫�ɤ����Υե饰
 */

int kCount = 1;

/*
 * ����������˥�����Хå���ػߤ��뤫�ɤ����Υե饰
 */

int iListCB = 0;

/*
 * irohacheck ���ޥ�ɤˤ�äƻȤ��Ƥ��뤫�Ȥ���
 * irohacheck �ʤ��Ǥ� verbose ��ɽ���͡�
 */

int ckverbose = 0;

/*
 * ���顼�Υ�å�����������Ƥ����ѿ�
 */

char *jrKanjiError = "";

/*
 * �ǥХ���å�������Ф����ɤ����Υե饰
 */

int iroha_debug = 0;

/*
 * �Ϥ�Ƥλ��Ѥ��ɤ����򼨤��ե饰
 */

int FirstTime = 1;

/*
 * �ӡ��ײ����Ĥ餹�ؿ����Ǽ����Ȥ���
 */

int (*jrBeepFunc)() = NULL;

/*
 * KC_INITIALIZE ľ��˼¹Ԥ��뵡ǽ����
 */

BYTE *initfunc = (BYTE *)0;
int howToReturnModeInfo = ModeInfoStyleIsString;
int chikuji = 0;
int LearnNumericalType = 0;
int CursorWrap = 1;
int SelectDirect = 0;
int HexkeySelect = 1;
int Gakushu = 1;
int BreakIntoRoman = 0;
int InhibitHankakuKana = 0;
int QuitIchiranIfEnd = 0;
char *RomkanaTable = (char *)NULL;
char *EnglishTable = (char *)NULL;
/* char *Dictionary = (char *)NULL; */
struct dicname *RengoGakushu = (struct dicname *)NULL;
struct dicname *KatakanaGakushu = (struct dicname *)NULL;
struct dicname *HiraganaGakushu = (struct dicname *)NULL;

int nKouhoBunsetsu = 16;
int BunsetsuKugiri = 0;
int stayAfterValidate = 1;
int kakuteiIfEndOfBunsetsu = 0;

int ChBasedMove = 1;
int ReverseWidely = 0;
int ReverseWord = 0;
int keepCursorPosition = 0;
int KeepCursorPosition = 0;
int ChikujiContinue = 1;
int RenbunContinue = 0;
int kouho_threshold = 2;
int strokelimit = STROKE_LIMIT;

int grammaticalQuestion = 1;

int nothermodes = 0;

keySupplement keysup[MAX_KEY_SUP];
int nkeysup = 0;

int abandonIllegalPhono = 0; /* �롼�볰�����Ϥ�ΤƤ뤫�ɤ��� */

int hexCharacterDefiningStyle = HEX_USUAL;

/*
 * ������κݻ��Ѥ���������ե�����̾�����ƤȤäƤ����Хåե���
 * �ե�����̾��","�Ƕ��ڤ��롣(��ĥ��ǽ�ǻ���)
 */

char *CANNA_initfilename = (char *)0;

/*
 * �С������
 */

int protocol_version = -1;
int server_version = -1;
char *server_name = (char *)0;

int kojin = 1;
int indexHankaku = 0;
int indexSeparator = DEFAULTINDEXSEPARATOR;
int allowNextInput = 1;
int chikujiRealBackspace = 1;
int BackspaceBehavesAsQuit = 1;
int doKatakanaGakushu = 1;
int doHiraganaGakushu = 1;
int chikuji_debug = 0;
int ignore_case = 0;
int romaji_yuusen = 0;
int auto_sync = 1;
int quickly_escape = 0;

int locale_insufficient = 0;

void (*keyconvCallback)() = (void (*)())0;

extraFunc *extrafuncp = (extraFunc *)0;
struct dicname *kanjidicnames; /* .canna �ǻ��ꤷ�Ƥ��뼭��ꥹ�� */

/*
  �ǥե�������ͤˤ�ɤ���
*/
void
restoreBindings()
{
  if (initfunc) free(initfunc);
  initfunc = (BYTE *)0;

  if (server_name) free(server_name);
  server_name = (char *)0;

  RomkanaTable = (char *)NULL;
  EnglishTable = (char *)NULL;
  romajidic = (struct RkRxDic *)0;
  englishdic = (struct RkRxDic *)0;
  RengoGakushu = (struct dicname *)NULL;
  KatakanaGakushu = (struct dicname *)NULL;
  HiraganaGakushu = (struct dicname *)NULL;
  howToBehaveInCaseOfUndefKey = kc_normal;
  CursorWrap = 1;
  SelectDirect = 0;
  HexkeySelect = 1;
  BunsetsuKugiri = 0;
/*  kanjidicname[nkanjidics = 0] = (char *)NULL; ����Τ��Ȥ򤷤ʤ���� */
  kanjidicnames = NULL;
  saveapname[0] = '\0';
  ChBasedMove = 1;
  ReverseWidely = 0;
  Gakushu = 1;
  QuitIchiranIfEnd = 0;
  BreakIntoRoman = 0;
  kouho_threshold = 2;
  grammaticalQuestion = 1;
  stayAfterValidate = 1;
  kakuteiIfEndOfBunsetsu = 0;  
  kCount = 1;
  hexCharacterDefiningStyle = HEX_USUAL;
  keepCursorPosition = 0;
  KeepCursorPosition = 0;
  ChikujiContinue = 1;
  RenbunContinue = 0;

  InhibitHankakuKana = 0;
  nothermodes = 0;
  protocol_version = server_version = -1;
  kojin = 1;
  indexHankaku = 0;
  indexSeparator = DEFAULTINDEXSEPARATOR;
  allowNextInput = 1;
  LearnNumericalType = 0;
  chikujiRealBackspace = 1;
  nKouhoBunsetsu = 16;
  ReverseWord = 0;
  nkeysup = 0;
  abandonIllegalPhono = 0;
  BackspaceBehavesAsQuit = 1;
  doKatakanaGakushu = 1;
  doHiraganaGakushu = 1;
  chikuji_debug = 0;
  strokelimit = STROKE_LIMIT;
  ignore_case = 0;
  romaji_yuusen = 0;
  auto_sync = 1;
  quickly_escape = 0;
  keyconvCallback = (void (*)())0;
  locale_insufficient = 0;
}
