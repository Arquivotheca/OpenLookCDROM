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
 *
 *  Author : Osamu Hata, NEC Corporation  (hata@d1.bs2.mt.nec.co.jp)
 *
 */

/*
static	char	rcs_id[] = "@(#) 102.1 $Id: ccustom.h,v 2.5 1994/02/03 07:31:07 hamada Exp $";
*/

#include "symbolname.h"

#define NHENKAN_MAX 9999

extern char *kanjidicname[], *userdicname[],  *bushudicname[], *localdicname[]; 
extern int  nkanjidics, nuserdics, nbushudics, nlocaldics;
extern char  *RomkanaTable, *RengoGakushu, *KatakanaGakushu;
extern int InitialMode, CursorWrap, SelectDirect, HexkeySelect, BunsetsuKugiri;
extern int ChBasedMove, ReverseWidely, Gakushu, QuitIchiranIfEnd;
extern int kakuteiIfEndOfBunsetsu, stayAfterValidate, BreakIntoRoman;
extern int kouho_threshold, gramaticalQuestion;
extern char *mode_mei[], null_mode[];
int forceKana, kCount, chikuji, iListCB ,nKouhoBunsetsu;
int keepCursorPosition, CannaVersion, abandonIllegalPhono;
int hexCharacterDefiningStyle, kojin, ReverseWord , allowNextInput;
int indexhankaku,ignorecase,romajiyuusen,autosync,nkeysuu,quicklyescape;

extern char *allKey[], *alphaKey[], *yomiganaiKey[];
extern char *yomiKey[], *jishuKey[], *tankouhoKey[];
extern char *ichiranKey[], *zenHiraKey[], *zenKataKey[];
extern char *zenAlphaKey[], *hanKataKey[], *hanAlphaKey[];
extern char *allFunc[], *alphaFunc[], *yomiganaiFunc[];
extern char *yomiFunc[], *jishuFunc[], *tankouhoFunc[];
extern char *ichiranFunc[], *zenHiraFunc[], *zenKataFunc[];
extern char *zenAlphaFunc[], *hanKataFunc[], *hanAlphaFunc[];
extern int NallKeyFunc, NalphaKeyFunc, NyomiganaiKeyFunc, NyomiKeyFunc;
extern int NjishuKeyFunc, NtankouhoKeyFunc,  NichiranKeyFunc;
extern int NzenHiraKeyFunc, NzenKataKeyFunc, NzenAlphaKeyFunc;
extern int NhanKataKeyFunc, NhanAlphaKeyFunc;
extern char returnKey[];
extern char *funcList[];
extern char *cfuncList[];

char **fList;

char etc_ctm[32];
char **etc_menu;
char **etc_manual;

char **current_acts, **current_keys;
char err_mess[255];
char *initFileSpecified =(char *)NULL;

int is_icustom;
int junban;

char *old_mode_ichiran[] = {
  "����ե��٥åȥ⡼��",
  "[��] �Ѵ����ϥ⡼��",
  "[����] �������ɽ������",
  "��[��] ���ѤҤ餬�ʳ������ϥ⡼��",
  "��[��] ���ѥ������ʳ������ϥ⡼��",
  "��[��] Ⱦ�ѥ������ʳ������ϥ⡼��",
  "��[��] ���ѥ���ե��٥åȳ������ϥ⡼��",
  "��[a] Ⱦ�ѥ���ե��٥åȳ������ϥ⡼��",
  "[��] �ɤ����Ͼ���",
  "[����] ʸ�����Ѵ�����",
  "[����] ñ�����ɽ������",
  "[����] �������ɽ������",
  "[����] ñ����Ͽ����ʸɽ������",
  "[����] ñ����Ͽ����ʸɽ������",
  "[��] ���ѥ��������Ѵ����ϥ⡼��",
  "[��] Ⱦ�ѥ��������Ѵ����ϥ⡼��",
  "[��] ���ѥ���ե��٥å��Ѵ����ϥ⡼��",
  "[a] Ⱦ�ѥ���ե��٥å��Ѵ����ϥ⡼��",
  "[16��] �����ʥ��������Ͼ���",
  "[����] ������ɤ����Ͼ���",
  "[��ĥ] ��ĥ��ǽ�������",
  "[��] ����ʸ���������",
  "[��] ���ꥷ��ʸ���������",
  "[����] �����������",
  "[�ѹ�] �������ѹ�����",
  "[�Ѵ�] �Ѵ���������",
  "[���] ñ��������",
  "[��Ͽ] ñ����Ͽ�⡼��",
  "[�ʻ�] ñ����Ͽ�⡼�ɤ��ʻ��������",
  "[����] ñ����Ͽ�⡼�ɤμ����������",
  "[��] �������ϥ⡼��",
  "[�Խ�] �Խ��⡼��",
  "[����] ����Υޥ���ȡ�����ޥ���Ⱦ���",
  NULL
};

char *old_mode_ichiran2[] = {
  "alphaMode",
  "henkanNyuryokuMode",
  "[����] �������ɽ������",
  "zenHiraKakuteiMode",
  "zenKataKakuteiMode",
  "hanKataKakuteiMode",
  "zenAlphaKakuteiMode",
  "hanAlphaKakuteiMode",
  "[��] �ɤ����Ͼ���",
  "[����] ʸ�����Ѵ�����",
  "[����] ñ�����ɽ������",
  "[����] �������ɽ������",
  "[����] ñ����Ͽ����ʸɽ������",
  "[����] ñ����Ͽ����ʸɽ������",
  "zenKataHenkanMode",
  "hanKataHenkanMode",
  "zenAlphaHenkanMode",
  "hanAlphaHenkanMode",
  "[16��] �����ʥ��������Ͼ���",
  "[����] ������ɤ����Ͼ���",
  "[��ĥ] ��ĥ��ǽ�������",
  "[��] ����ʸ���������",
  "[��] ���ꥷ��ʸ���������",
  "[����] �����������",
  "[�ѹ�] �������ѹ�����",
  "[�Ѵ�] �Ѵ���������",
  "[���] ñ��������",
  "[��Ͽ] ñ����Ͽ�⡼��",
  "[�ʻ�] ñ����Ͽ�⡼�ɤ��ʻ��������",
  "[����] ñ����Ͽ�⡼�ɤμ����������",
  "[��] �������ϥ⡼��",
  "[�Խ�] �Խ��⡼��",
  "[����] ����Υޥ���ȡ�����ޥ���Ⱦ���",
  NULL
};

char *mode_ichiran3[] = {
  S_AlphaMode,
  S_YomiganaiMode,
  S_KigouMode,
  S_YomiMode,
  S_MojishuMode,
  S_TankouhoMode,
  S_IchiranMode,
  S_YesNoMode,
  S_OnOffMode,
  "S_AdjustBunsetsu",
  S_AutoYomiMode,
  S_AutoBunsetsuMode,
  S_HenkanNyuuryokuMode,
  S_ZenHiraHenkanMode,
  S_HanHiraHenkanMode,
  S_ZenKataHenkanMode,
  S_HanKataHenkanMode,
  S_ZenAlphaHenkanMode,
  S_HanAlphaHenkanMode,
  S_ZenHiraKakuteiMode,
  S_HanHiraKakuteiMode,
  S_ZenKataKakuteiMode,
  S_HanKataKakuteiMode,
  S_ZenAlphaKakuteiMode,
  S_HanAlphaKakuteiMode,
  S_HexMode,
  S_BushuMode,
  S_ExtendMode,
  S_RussianMode,
  S_GreekMode,
  S_LineMode,
  S_ChangingServerMode,
  S_HenkanMethodMode,
  S_DeleteDicMode,
  S_TourokuMode,
  S_TourokuHinshiMode,
  S_TourokuDicMode,
  S_QuotedInsertMode,
  S_BubunMuhenkanMode,
  S_MountDicMode,
  NULL
};

char *mode_ichiran[] = {
  "����ե��٥åȥ⡼��",
  "[ �� ] �Ѵ����ϥ⡼��",
  "[����] �������ɽ������",
  "<����> ���ѤҤ餬�ʳ������ϥ⡼��",
  "<����> ���ѥ������ʳ������ϥ⡼��",
  "<Ⱦ��> Ⱦ�ѥ������ʳ������ϥ⡼��",
  "<����> ���ѥ���ե��٥åȳ������ϥ⡼��",
  "<����> Ⱦ�ѥ���ե��٥åȳ������ϥ⡼��",
  "[ �� ] �ɤ����Ͼ���",
  "[����] ʸ�����Ѵ�����",
  "[����] ñ�����ɽ������",
  "[����] �������ɽ������",
  "[����] ñ����Ͽ����ʸɽ������",
  "[����] ñ����Ͽ����ʸɽ������",
  "[����] ���ѥ��������Ѵ����ϥ⡼��",
  "[Ⱦ��] Ⱦ�ѥ��������Ѵ����ϥ⡼��",
  "[����] ���ѥ���ե��٥å��Ѵ����ϥ⡼��",
  "[Ⱦ��] Ⱦ�ѥ���ե��٥å��Ѵ����ϥ⡼��",
  "[16��] �����ʥ��������Ͼ���",
  "[����] ������ɤ����Ͼ���",
  "[��ĥ] ��ĥ��ǽ�������",
  "[ �� ] ����ʸ���������",
  "[ �� ] ���ꥷ��ʸ���������",
  "[����] �����������",
  "[�ѹ�] �������ѹ�����",
  "[�Ѵ�] �Ѵ���������",
  "[���] ñ��������",
  "[��Ͽ] ñ����Ͽ�⡼��",
  "[�ʻ�] ñ����Ͽ�⡼�ɤ��ʻ��������",
  "[����] ñ����Ͽ�⡼�ɤμ����������",
  "[ �� ] �������ϥ⡼��",
  "[�Խ�] �Խ��⡼��",
  "[����] ����Υޥ���ȡ�����ޥ���Ⱦ���",
  NULL
};

char *mode_ichiran2[] = {
  NULL,
  "",
  "[����] �������ɽ������",
  S_ZenHiraKakuteiMode,
  S_ZenKataKakuteiMode,
  S_HanKataKakuteiMode,
  S_ZenAlphaKakuteiMode,
  S_HanAlphaKakuteiMode,
  "[ �� ] �ɤ����Ͼ���",
  "[����] ʸ�����Ѵ�����",
  "[����] ñ�����ɽ������",
  "[����] �������ɽ������",
  "[����] ñ����Ͽ����ʸɽ������",
  "[����] ñ����Ͽ����ʸɽ������",
  S_ZenKataHenkanMode,
  S_HanKataHenkanMode,
  S_ZenAlphaHenkanMode,
  S_HanAlphaHenkanMode,
  "[16��] �����ʥ��������Ͼ���",
  "[����] ������ɤ����Ͼ���",
  "[��ĥ] ��ĥ��ǽ�������",
  "[ �� ] ����ʸ���������",
  "[ �� ] ���ꥷ��ʸ���������",
  "[����] �����������",
  "[�ѹ�] �������ѹ�����",
  "[�Ѵ�] �Ѵ���������",
  "[���] ñ��������",
  "[��Ͽ] ñ����Ͽ�⡼��",
  "[�ʻ�] ñ����Ͽ�⡼�ɤ��ʻ��������",
  "[����] ñ����Ͽ�⡼�ɤμ����������",
  "[ �� ] �������ϥ⡼��",
  "[�Խ�] �Խ��⡼��",
  "[����] ����Υޥ���ȡ�����ޥ���Ⱦ���",
  NULL
};

char *old_mode_ichiran3[] = {
  "Alpha",
  "HenkanNyuryoku",
  "Kigo",
  "ZenHiraKakutei",
  "ZenKataKakutei",
  "HanKataKakutei",
  "ZenAlphaKakutei",
  "HanAlphaKakutei",
  "Yomi",
  "Mojishu",
  "Tankouho",
  "Ichiran",
  "YesNo",
  "YesNo",
  "ZenKataHenkan",
  "HanKataHenkan",
  "ZenAlphaHenkan",
  "HanAlphaHenkan",
  "Hex",
  "Bushu",
  "Extend",
  "Russian",
  "Greek",
  "Line",
  "ChangingServer",
  "HenkanMethod",
  "DeleteDic",
  "Touroku",
  "TourokuHinshi",
  "TourokuDic",
  "QuotedInsert",
  "BubunMuhenkan",
  "MountDic",
  NULL
};

char mode_set[] = { 0, 1, 3, 4, 6, 5, 7, 1, 14, 16, 15, 17, -1};

char *base_menu[] = {
  "�������ޥ����ե�������ɤ߹���",
  "�������ޥ����ե�����ؤ���¸",
  "���Ѥ��뼭�������",
  "�����Υ������ޥ���",
  "�⡼�ɤ�ɽ��ʸ����Υ������ޥ���",
  "����¾�Υ������ޥ���",
  "��λ",
  NULL
};

char *d_mess[] = {
  "����μ����������ذ�ư�ĥ��ڡ�������,�ţӣå���",
  "��Ͽ����ɽ�����ذ�ư�ĥ��ڡ�������",
  "������Ͽ�ļ���μ���������֤ǥ꥿���󥭡�",
  "�������ļ������Ͽ����ɽ�����֤ǣĥ���",
  "���Ѥ��뼭��������λ�ļ���μ���������֤ǣţӣå���",
  NULL
};

char *dic_menu[] = {
  "���޻������Ѵ��ơ��֥�",
  "�����ƥ༭��̾",
  "�����Ѵ�����̾",
  "�桼������̾",
/*
  "Ϣ���Ѵ�����̾",
*/
  NULL
};

char *key_menu[] = {
  "���ƤΥ⡼��",
  "����ե��٥åȥ⡼��",
  "�Ѵ����ϥ⡼��(���ɤ����ϻ�)",
  "�Ѵ����ϥ⡼��(�ɤ����ϻ�)",
  "ʸ�����Ѵ�����",
  "ñ�����ɽ������",
  "�������ɽ������",
  "���ѤҤ餬�ʳ������ϥ⡼��",
  "���ѥ������ʳ������ϥ⡼��",
  "���ѥ���ե��٥åȳ������ϥ⡼��",
  "Ⱦ�ѥ������ʳ������ϥ⡼��",
  "Ⱦ�ѥ���ե��٥åȳ������ϥ⡼��",
  NULL
};

char *kh_mess[] = {
  "�����������ޥ�������������ϡֿ��������פ�����塢�꥿���󥭡��򲡤�",
  "��������������������ǽ����ϡֿ�����ǽ�פ�����塢�꥿���󥭡��򲡤�",
  "��",
  "���������ޥ��������Խ����ѹ�������ʬ������塢",
  "��������������������������������ĥ����򲡤�",
  "�������������������������Ժ�����˥����򲡤�",
  "���������������������������������ɥ����򲡤�",
  "�������������������������ɲá����������򲡤�",
  "���Υ⡼�ɤ�ȴ���롧�ţӣå������ѥ����򲡤�",
  NULL
};

struct dk_block {
  char *str;
  int len;
  int gyousu;
  int keysu;
  int actsu;
} dk_blocks[1024];

struct U_dk_block {
  char str[256];
  int len;
  int gyousu;
  int keysu;
  int actsu;
} U_dk_blocks;

struct wdisp {
  int top;
  int line;
  int kora;
  int ban;
  int current;
} WDisp, U_WDisp;

char *cc_keys[1], *cc_acts[1];
char ccc_keys[256], ccc_acts[256];
char undo_keys[256], undo_acts[256];
int  cc_keys_len = 0, cc_acts_len = 0;
int touroku_start = -1;
int undo_status = -1, undo_ts;

char *key_iroha[] = {
  "all",
  "alpha",
  "yomiganai",
  "yomi",
  "mojishu",
  "tankouho",
  "ichiran",
  "zenHiraKakutei",
  "zenKataKakutei",
  "zenAlphaKakutei",
  "hanKataKakutei",
  "hanAlphaKakutei",
  NULL
  };

char *keyLine[] = {
  "C-@  C-a  C-b  C-c  C-d  C-e  C-f  C-g  C-h  C-i  C-j  C-k",
  "C-l  C-m  C-n  C-o  C-p  C-q  C-r  C-s  C-t  C-u  C-v  C-w",
  "C-x  C-y  C-z  C-[  C-\\  C-]  C-^  C-_  space  !  \"  #  $  %",
  "&  '  (  )  *  +  ,  -  .  /  0  1  2  3  4  5  6  7  8  9",
  ":  ;  <  =  >  ?  @  A  B  C  D  E  F  G  H  I  J  K  L  M",
  "N  O  P  Q  R  S  T  U  V  W  X  Y  Z  [  \\  ]  ^  _  `  a",
  "b  c  d  e  f  g  h  i  j  k  l  m  n  o  p  q  r  s  t  u",
  "v  w  x  y  z  {  |  }  ~  DEL  Nfer  Xfer  Up  Left  Right",
  "Down  Insert  Rollup  Rolldown  HOME  HELP  S-nfer  S-xfer",
  "S-up  S-left  S-right  S-down  C-nfer  C-xfer  C-up  C-left",
  "C-right  C-down  F1  F2  F3  F4  F5  F6  F7  F8  F9  F10",
  "PF1  PF2  PF3  PF4  undefine",
  NULL
};

char **actLine;

char *caLine[] = {
  "self-insert:����ʸ������������",                             /*  2 */
  "quoted-insert:���ΰ�ʸ����̵�������Ϥ���",
  "japanese-mode:���ܸ�⡼�ɤؤΰܹ�",
  "alpha-mode:����ե��٥åȥ⡼�ɤ˰ܹ�",                      /*  5 */
  "henkan-nyuuryoku-mode:�Ѵ����ϥ⡼�ɤ˰ܹ�",
  "forward:����ư(����)",
  "backward:����ư(����)",
  "next:������ʸ�����Ѵ�(�ɤ����ϤΤȤ�)",
  "previous:������ʸ�����Ѵ�(�ɤ����ϤΤȤ�)",                /* 10 */
  "beginning-of-line:��Ƭ(��ü)����",
  "end-of-line:����(��ü)����",
  "delete-next:����ʸ���õ�",
  "delete-previous:����ʸ�����",
  "kill-to-end-of-line:���������걦¦��ʸ��������ޤǺ��",   /* 15 */
  "henkan:�Ѵ�",
  "kakutei:����",
  "extend:�ΰ迭�Ф�",
  "shrink:�ΰ�̤�",
  "shinshuku-mode:ʸ�῭�̥⡼�ɤذܹ�",                        /* 20 */
  "quit:�����",
  "convert-as-hex:�ɤߤ�16�ʥ����ɤȤߤʤ����Ѵ�",
  "convert-as-bushu:�ɤߤ�����̾�Ȥߤʤ����Ѵ�",
  "kouho-ichiran:�������ɽ��",
  "henshu:�Խ�",                                                /* 25 */
  "zenkaku:�����Ѵ�",
  "hankaku:Ⱦ���Ѵ�",
  "to-upper:��ʸ�����Ѵ�",
  "capitalize:��Ƭʸ���Τߤ���ʸ���ˤ���",
  "to-lower:��ʸ�����Ѵ�",                                      /* 30 */
  "hiragana:�Ҥ餬���Ѵ�",
  "katakana:���������Ѵ�",
  "romaji:���޻��Ѵ�",
  "base-hiragana:���ϥ١�����Ҥ餬�ʤˤ���",
  "base-katakana:���ϥ١����򥫥����ʤˤ���",                   /* 35 */
  "base-eisu:���ϥ١�����ѿ��ˤ���",
  "base-zenkaku:���ϥ١��������Ѥˤ���",
  "base-hankaku:���ϥ١�����Ⱦ�Ѥˤ���",
  "base-kana:���ϥ١����򥫥ʤˤ���",
  "base-kakutei:���ϥ⡼�ɤ����⡼�ɤˤ���",                  /* 40 */
  "base-henkan:���ϥ⡼�ɤ��Ѵ��⡼�ɤˤ���",
  "base-hiragana-katakana-toggle:���ϥ١�����Ҥ餬�ʡ��������ʤǥȥ���", 
  "base-zenkaku-hankaku-toggle:���ϥ١��������Ѥ�Ⱦ�Ѥǥȥ���",
  "base-kana-eisu-toggle:���ϥ١����򥫥ʤȱѿ��ǥȥ���",
  "base-kakutei-henkan-toggle:���ϥ⡼�ɤ������Ѵ��ǥȥ���",  /* 45 */
  "base-rotate-forward:���ϥ⡼�ɤ���֤��ڤ��ؤ���(����)",
  "base-rotate-backward:���ϥ⡼�ɤ���֤��ڤ��ؤ���(�ս�)",
  "extend-mode:�桼�ƥ���ƥ���˥塼�˰ܹ�",
  "hex-mode:���������ϥ⡼�ɤ˰ܹ�",
  "bushu-mode:�������ϥ⡼�ɤ˰ܹ�",                            /* 50 */
  "kigou-mode:�������ϥ⡼�ɤ˰ܹ�",
  "zen-hira-kakutei-mode:���ѤҤ餬�ʳ���⡼�ɤ˰ܹ�",
  "zen-kata-kakutei-mode:���ѥ������ʳ���⡼�ɤ˰ܹ�",
  "han-kata-kakutei-mode:Ⱦ�ѥ������ʳ���⡼�ɤ˰ܹ�",
  "zen-alpha-kakutei-mode:���ѥ���ե��٥åȳ���⡼�ɤ˰ܹ�",  /* 55 */
  "han-alpha-kakutei-mode:Ⱦ�ѥ���ե��٥åȳ���⡼�ɤ˰ܹ�",
  "henkan-or-self-insert:�١������Ҥ餬�ʰʳ��ΤȤ�self-insert�򤪤��ʤ�",
  "henkan-or-do-noting:�١������Ҥ餬�ʰʳ��ΤȤ��ʤˤ⤷�ʤ�",
  "switch-server:�����Ф��ڤ��ؤ��򤪤��ʤ�",
  "disconnect-server:�����ФȤ���³���ڤ�",
  "show-server-name:������̾��ɽ������",
  "show-gakushu:�ؽ����֤�ɽ������",
  "show-canna-version:�С�������ɽ������",
  "show-romkana-table:���޻������Ѵ��ơ��֥�̾��ɽ������",
  "show-canna-file:�������ޥ����ե�����̾��ɽ������",
  "sync-dicsyionary:����˽񤭹���",
  NULL
};

char *aLine[] = {
  "SelfInsert:��ʸ����������",
  "QuotedInsert:���ΰ�ʸ����̵�������Ϥ���",
  "JapaneseMode:���ܸ�⡼�ɤؤΰܹ�",
  "AlphaMode:����ե��٥åȥ⡼�ɤ˰ܹ�",
  "HenkanNyuryokuMode:�Ѵ����ϥ⡼�ɤ˰ܹ�",
  "ZenHiraKakuteiMode:���ѤҤ餬�ʳ���⡼�ɤ˰ܹ�",
  "ZenKataKakuteiMode:���ѥ������ʳ���⡼�ɤ˰ܹ�",
  "HanKataKakuteiMode:Ⱦ�ѥ������ʳ���⡼�ɤ˰ܹ�",
  "ZenAlphaKakuteiMode:���ѥ���ե��٥åȳ���⡼�ɤ˰ܹ�",
  "HanAlphaKakuteiMode:Ⱦ�ѥ���ե��٥åȳ���⡼�ɤ˰ܹ�",
  "HexMode:�����ʥ��������ϥ⡼�ɤ˰ܹ�",
  "BushuMode:�������ϥ⡼�ɤ˰ܹ�",
  "KigouMode:�������ϥ⡼�ɤ˰ܹ�",
  "Forward:����ư(����)",
  "Backward:����ư(����)",
  "Next:������ʸ�����Ѵ�(�ɤ����ϤΤȤ�)",
  "Previous:������ʸ�����Ѵ�(�ɤ����ϤΤȤ�)",
  "BeginningOfLine:��Ƭ(��ü)����",
  "EndOfLine:����(��ü)����",
  "DeleteNext:����������ʬ��ʸ�����",
  "DeletePrevious:��������κ���ʸ�����",
  "KillToEndOfLine:���������걦¦��ʸ��������ޤǺ��",
  "Henkan:�Ѵ�",
  "Kakutei:����",
  "Extend:�ΰ迭�Ф�������ʸ�����ڤ��ؤ���(�ɤ����ϤΤȤ�)",
  "Shrink:�ΰ�̤ᡢ����ʸ�����ڤ��ؤ���(�ɤ����ϤΤȤ�)",
  "Quit:�����",
  "Touroku:�桼�ƥ���ƥ�(��ĥ)�⡼�ɤذܹ�",
  "ConvertAsHex:�ɤߤ�16�ʥ����ɤȤߤʤ����Ѵ�",
  "ConvertAsBushu:�ɤߤ�����̾�Ȥߤʤ����Ѵ�",
  "KouhoIchiran:�������ɽ��",
  "BubunMuhenkan:��ʬ̵�Ѵ�",
  "Zenkaku:���Ѥ��Ѵ�",
  "Hankaku:Ⱦ�Ѥ��Ѵ�",
  "ToUpper:��ʸ�����Ѵ�",
  "Capitalize:��Ƭʸ���Τߤ���ʸ���ˤ���",
  "ToLower:��ʸ�����Ѵ�",
  "Hiragana:�Ҥ餬�ʤ��Ѵ�",
  "Katakana:�������ʤ��Ѵ�",
  "Romaji:���޻����Ѵ�",
  "NextKouho:������",
  "PrevKouho:������",
  NULL
};

char *old_mode_menu[] = {
  "         ����ե��٥åȥ⡼��",
  "  [��]   �Ѵ����ϥ⡼��",
  "��[��]   ���ѤҤ餬�ʳ������ϥ⡼��",
  "��[��]   ���ѥ������ʳ������ϥ⡼��",
  "��[��]    Ⱦ�ѥ������ʳ������ϥ⡼��",
  "��[��]   ���ѥ���ե��٥åȳ������ϥ⡼��",
  "��[a]    Ⱦ�ѥ���ե��٥åȳ������ϥ⡼��",
  "  [����] ʸ�����Ѵ�����",
  "  [����] ñ�����ɽ������",
  "  [����] �������ɽ������",
  "  [��]   ���ѥ��������Ѵ����ϥ⡼��",
  "  [��]    Ⱦ�ѥ��������Ѵ����ϥ⡼��",
  "  [��]   ���ѥ���ե��٥å��Ѵ����ϥ⡼��",
  "  [a]    Ⱦ�ѥ���ե��٥å��Ѵ����ϥ⡼��",
  "  [16��] �����ʥ��������Ͼ���",
  "  [����] ������ɤ����Ͼ���",
  "  [����] �������ɽ������",
  "  [��ĥ] ��ĥ��ǽ�������",
  "  [��]   ����ʸ���������",
  "  [��]   ���ꥷ��ʸ���������",
  "  [����] �����������",
  "  [�ѹ�] �������ѹ�����",
  "  [���] ñ��������",
  "  [��Ͽ] ñ����Ͽ�⡼��",
  "  [�ʻ�] ñ����Ͽ�⡼�ɤ��ʻ��������",
  "  [����] ñ����Ͽ�⡼�ɤμ����������",
  "  [��]   �������ϥ⡼��",
  "  [�Խ�] �Խ��⡼��",
  "  [����] ����Υޥ���ȡ�����ޥ���Ⱦ���",
  NULL
};

char *mode_menu[] = {
  "         ����ե��٥åȥ⡼��",
  "[ �� ]   �Ѵ����ϥ⡼��",
  "<����>   ���ѤҤ餬�ʳ������ϥ⡼��",
  "<����>   ���ѥ������ʳ������ϥ⡼��",
  "<Ⱦ��>   Ⱦ�ѥ������ʳ������ϥ⡼��",
  "<����>   ���ѥ���ե��٥åȳ������ϥ⡼��",
  "<Ⱦ��>   Ⱦ�ѥ���ե��٥åȳ������ϥ⡼��",
  "[����]   ʸ�����Ѵ�����",
  "[����]   ñ�����ɽ������",
  "[����]   �������ɽ������",
  "[����]   ���ѥ��������Ѵ����ϥ⡼��",
  "[Ⱦ��]   Ⱦ�ѥ��������Ѵ����ϥ⡼��",
  "[����]   ���ѥ���ե��٥å��Ѵ����ϥ⡼��",
  "[Ⱦ��]   Ⱦ�ѥ���ե��٥å��Ѵ����ϥ⡼��",
  "[�༡]   �༡��ư�Ѵ��ɤ���ʬ",
  "[�༡]   �༡��ư�Ѵ�ʸ����ʬ",
  "[16��]   �����ʥ��������Ͼ���",
  "[����]   ������ɤ����Ͼ���",
  "[����]   �������ɽ������",
  "[��ĥ]   ��ĥ��ǽ�������",
  "[ �� ]   ����ʸ���������",
  "[ �� ]   ���ꥷ��ʸ���������",
  "[����]   �����������",
  "[�ѹ�]   �������ѹ�����",
  "[���]   ñ��������",
  "[��Ͽ]   ñ����Ͽ�⡼��",
  "[�ʻ�]   ñ����Ͽ�⡼�ɤ��ʻ��������",
  "[����]   ñ����Ͽ�⡼�ɤμ����������",
  "[ �� ]   �������ϥ⡼��",
  "[�Խ�]   �Խ��⡼��",
  "[����]   ����Υޥ���ȡ�����ޥ���Ⱦ���",
  NULL
};

char mm_set[] = { 0, 1, 3, 4, 5, 6, 7, 9, 10, 11, 14, 15, 16, 17, 18,
		     19, 2, 20, 21, 22, 23, 24, 26, 27, 28, 29, 30, 31, 32 };

char c_mm_set[] = { 0, 12, 19, 21, 22, 23, 24, 4, 5, 6, 15,
		      16, 17, 18, 10, 11,25, 26,2, 27, 28, 
		      29, 30, 31, 33, 34, 35, 36, 37, 38, 39 };

char *ce_menu[] = {
  "initialMode",
  "cursorWrap",
  "numericalKeySelect",
  "selectDirect",
  "bunsetsuKugiri",
  "characterBasedMove",
  "reverseWidely",
  "quitIfEndOfIchiran",
  "breakIntoRoman",
  "gakushu",
  "stayAfterValidate",
  "kakuteiIfEndOfBunsetsu",
  "gramaticalQuestion",
  "nHenkanForIchiran",
  "kouhoCount",
  "kojin",
  "auto",
  "nKouhoBunsetsu",
  "abandonIllegalPhono",
  "hexDirect",
  "allowNextInput",
  "indexHankaku",
  "ignoreCase",
  "romajiYuusen",
  "auto-sync",
  "nKeyToDisconnect",
  "quicklyEscape",
  NULL
};

char *e_menu[] = {
  "initialMode",
  "cursorWrap",
  "numericalKeySelect",
  "selectDirect",
  "bunsetsuKugiri",
  "characterBasedMove",
  "reverseWidely",
  "quitIfEndOfIchiran",
  "breakIntoRoman",
  "gakushu",
  "stayAfterValidate",
  "kakuteiIfEndOfBunsetsu",
  "gramaticalQuestion",
  "nHenkanForIchiran",
  NULL
};
  
#define ON   1
#define OFF -1
#define MID  2

char *ce_manual[] = {
  "  ��ư���Υ⡼�ɤ��������(��,��)�ǻ��ꤷ�ޤ����ǥե���Ȥϥ���ե��٥å�\
�⡼�ɤǤ���",
  "  �ɤߤ����Ϥ��Ƥ�����֤ǥ���������ư������˱�ü���鱦\
�ذ�ư�������򤷤����亸ü���麸�ذ�ư�������򤷤�����\
ȿ��¦�Υ������뤬��ư���뤳�Ȥ���ꤷ�ޤ���on�ǰ�ư����off\
�ǰ�ư���ޤ��󡣥ǥե���Ȥ�on�Ǥ���",
  "��������ǡ������������Ѥ��Ƹ��������Ǥ��뤫�ɤ��������\
���ޤ����ǥե���Ȥ�on�Ǥ���off����ꤹ��ȡ������Υ�����\
������������򤵤�Ƥ�����䤬���ꤷ���������������ϼ����ɤߤȤ���\
��갷���ޤ���",
  "numericalKeySelect��on�Ǥ���Ȥ��ˡ����������Ǹ�������򤷤��Ȥ�\
�˸������ɽ���Τޤޤ������Ǥʤ�������ꤷ�ޤ���on�ξ��ϸ��������\
��λ���ޤ����ǥե���Ȥ�off�Ǥ���",
  "�����ɽ�����Ƥ���Ȥ���ʸ�ᤴ�Ȥ˶���Ƕ��ڤ뤫�ɤ��������\
���ޤ���on�Ƕ��ڤ�ޤ����ǥե���Ȥ�off�Ǥ���",
  "�ɤߤ����Ϥ��Ƥ���Ȥ��˥��������ư��Ԥ����ˡ�ʸ��ñ�̤ǰ�ư��\
�Ԥ����ɤ�������ꤷ�ޤ����ǥե���Ȥ�on�Ǥ���off�ˤ���ȡ����޻�\
�����Ѵ��γ����ñ�̤��ˤ��ƥ���������ư���ޤ���ʸ�������Ʊ�ͤ�\
�Ԥ��ޤ���",
  "on����ꤹ����ɤߤ����Ϥ��Ƥ������ʸ�����ȿž�ϰϤ������ʤ�ޤ���\
�ǥե���Ȥ�off�Ǥ���",
  "��������Ǻǽ������ɽ�����Ƥ�����֤Ǽ���������Ԥ��ȸ��������\
��λ�����ɤߤ��Τ�Τ����Ȥ���ɽ������褦�ˤʤ�ޤ��������ܤ�\
���ڡ���������������ɽ���˥������ޥ������Ƥ�����ʤɤ�on�ˤ��Ƥ���\
�������Ǥ����ǥե���Ȥ�off�Ǥ���",
  "�Хå����ڡ����������Ǥä��Ȥ���ľ���Υ��޻������Ѵ����줿ʸ����\
���޻�����뤫�ɤ�������ꤷ�ޤ����ǥե���Ȥ�off�Ǥ���",
  "���ʴ����Ѵ����ؽ���Ԥ����ɤ�������ꤷ�ޤ����ǥե���Ȥ�on�Ǥ���",
  "����������֤Ǹ�������򤷤�ñ�����ɽ�����֤���ä��Ȥ��ˡ�������\
ʸ��򼡤�ʸ��˰�ư�����뤫�ɤ�������ꤷ�ޤ���off�ǥ�����ʸ��˰�ư\
���ޤ���on�Ǥϥ�����ʸ����Ѥ��ޤ��󡣥ǥե���Ȥ�on�Ǥ���",
  "�Ǳ�ʸ��Ǽ�ʸ��ذ�ư���褦�Ȥ����Ȥ��ˡ����ꤹ�뤫�ݤ�����ꤷ�ޤ���\
off����ꤹ��ȺǺ�ʸ�᤬������ʸ��ˤʤ�ޤ����ǥե���Ȥ�off�Ǥ���",
  "ñ����Ͽ���ʻ����ꤷ���塢�ܺ٤��ʻ�ʬ��Τ���μ����Ԥ����ݤ���\
���ꤷ�ޤ���on�Ǽ����Ԥ���off�Ǥϼ���򤷤ޤ��󡣥ǥե���Ȥ�on�Ǥ���",
  "�Ѵ������򲿲󤫲����ȸ������ɽ�����Ԥ���褦�ˤ����\
���Ǥ��ޤ��������ǤϤ��β�����������(��,��)�ǻ��ꤷ�ޤ���\
0��ˤ���Ȳ����Ѵ�������\
�����Ƥ����ɽ�����Ԥ��ʤ��ʤ�ޤ����ǥե���Ȥ�2��ǡ�2���Ѵ�������\
�����Ȱ���ɽ���ˤʤ�ޤ���",
  "���������桼�ƥ���ƥ���ǽ��ɽ�����̤���ʬ�θ�����ޤ��ϥ����ƥ��\
��ɽ�����뤫���ʤ��������ꤷ�ޤ���on��ɽ������off��ɽ������ʤ��ʤ�ޤ���\
�ǥե���Ȥ�on�Ǥ���",
  "�Ŀ��̳ؽ��ե����뤬¸�ߤ��뼭����Ф��ơ��Ŀ��̳ؽ���Ԥ����ɤ�����\
���ꤷ�ޤ���on�ǹԤ���off�ǹԤ��ޤ��󡣥ǥե���Ȥ�on�ǸĿ��̳ؽ���Ԥ��ޤ���",
  "�Ѵ��������༡��ư�Ѵ��ˤ��뤫���ʤ��������ꤷ�ޤ���on�Ǥ��༡��ư�Ѵ�\
�ˤʤ�off�Ǥ�Ϣʸ���Ѵ��ˤʤ�ޤ����ǥե���Ȥ�off(Ϣʸ���Ѵ�)�Ǥ���",
  "�༡��ư�Ѵ��ˤ������Ѵ�����ݻ����Ƥ������ʸ��ο������ꤷ�ޤ���\
�ǥե���Ȥ�16ʸ��Ǥ��������ǽ�ʿ���3��32�Ǥ��� �����ǤϤ��β����\
�������(��,��)�ǻ��ꤷ�ޤ���",
  "���޻������Ѵ��������ʥ��޻������ϤȤ��ƻĤ뤫�ɤ�������ꤷ�ޤ���\
on�������ʥ��޻����ΤƤ��ޤ���off�Ǥ������ʥ��޻��ϻĤ�ޤ����ǥե����\
��off�Ǥ���",
  "���������Ϥˤ�����4��Υ����ɤ����ϸ塢������ʸ������ꤹ�뤫���ʤ���\
�����ꤷ�ޤ���on�ǳ��ꤷ�ơ�off�ǳ��ꤷ�ޤ��󡣥ǥե���Ȥ�off�ǳ���\
���ޤ���",
  "�������ɽ�����֤ǿ����ʳ��Υ����򲡤������ˡ����ߥ������뤬���֤���\
������䤬���򤵤켡�����ϤȤʤ뤫�ɤ�������ꤷ�ޤ���",
  "����������ֹ������ʸ������Ⱦ��ʸ�����ѹ����뤫�ɤ�������ꤷ�ޤ����ǥե���Ȥ�off������ʸ���Ǥ���", 
  "���޻������Ѵ��ơ��֥���̾����ʸ�������Ϥ���ȥ���ե��٥åȤΤޤ����Ϥ��졢���ʤˤ��Ѵ�����ޤ���on�ˤ������ʸ�������Ϥ��Ƥ���޻������Ѵ��ǤϾ�ʸ���Ȥ��Ƽ�갷���ޤ����ǥե���Ȥ�off�Ǥ���",
  "on�ˤ�������Ϥ��줿ʸ�������޻������Ѵ���ͭ���Ȥʤ륭���Ǥ���С����Υ����˳�����Ƥ��Ƥ��뵡ǽ�����Ǥ����� self-insert �������ʤ��ޤ����ǥե���Ȥ�off�Ǥ���",
  "ñ����Ͽ����Ӻ���򤷤��Ȥ���ưŪ�˼����Ʊ�����Ԥ��뤫�ɤ�������ꤷ�ޤ���on�ˤ����Ʊ�����Ԥ��ޤ����ǥե���Ȥ�on�Ǥ���",
  "����ե��٥åȤ����Ϥ�³�������Υ����ФȤ���³���ڤ�륹�ȥ���������ꤷ�ޤ��������Ǥϲ�����������(��,��)�ǻ��ꤷ�ޤ�����������100���Ѳ����ޤ����ǥե���Ȥ�500�Ǥ���",
  "�������ϥ⡼�ɤǵ�������򤷤���⵭������⡼�ɤ�α�ޤ뤫�ɤ�������ꤷ�ޤ���off �ˤ����α�ޤ�ޤ���",
  NULL
};

char *e_manual[] = {
  "  ��ư���Υ⡼�ɤ��������(��,��)�ǻ��ꤷ�ޤ����ǥե���Ȥϥ���ե��٥å�\
�⡼�ɤǤ���",
  "  �ɤߤ����Ϥ��Ƥ�����֤ǥ���������ư������˱�ü���鱦\
�ذ�ư�������򤷤����亸ü���麸�ذ�ư�������򤷤�����\
ȿ��¦�Υ������뤬��ư���뤳�Ȥ���ꤷ�ޤ���on�ǰ�ư����off\
�ǰ�ư���ޤ��󡣥ǥե���Ȥ�on�Ǥ���",
  "��������ǡ������������Ѥ��Ƹ��������Ǥ��뤫�ɤ��������\
���ޤ����ǥե���Ȥ�on�Ǥ���off����ꤹ��ȡ������Υ�����\
������������򤵤�Ƥ�����䤬���ꤷ���������������ϼ����ɤߤȤ���\
��갷���ޤ���",
  "numericalKeySelect��on�Ǥ���Ȥ��ˡ����������Ǹ�������򤷤��Ȥ�\
�˸������ɽ���Τޤޤ������Ǥʤ�������ꤷ�ޤ���on�ξ��ϸ��������\
��λ���ޤ����ǥե���Ȥ�off�Ǥ���",
  "�����ɽ�����Ƥ���Ȥ���ʸ�ᤴ�Ȥ˶���Ƕ��ڤ뤫�ɤ��������\
���ޤ���on�Ƕ��ڤ�ޤ����ǥե���Ȥ�off�Ǥ���",
  "�ɤߤ����Ϥ��Ƥ���Ȥ��˥��������ư��Ԥ����ˡ�ʸ��ñ�̤ǰ�ư��\
�Ԥ����ɤ�������ꤷ�ޤ����ǥե���Ȥ�on�Ǥ���off�ˤ���ȡ����޻�\
�����Ѵ��γ����ñ�̤��ˤ��ƥ���������ư���ޤ���ʸ�������Ʊ�ͤ�\
�Ԥ��ޤ���",
  "on����ꤹ����ɤߤ����Ϥ��Ƥ������ʸ�����ȿž�ϰϤ������ʤ�ޤ���\
�ǥե���Ȥ�off�Ǥ���",
  "��������Ǻǽ������ɽ�����Ƥ�����֤Ǽ���������Ԥ��ȸ��������\
��λ�����ɤߤ��Τ�Τ����Ȥ���ɽ������褦�ˤʤ�ޤ��������ܤ�\
���ڡ���������������ɽ���˥������ޥ������Ƥ�����ʤɤ�on�ˤ��Ƥ���\
�������Ǥ����ǥե���Ȥ�off�Ǥ���",
  "�Хå����ڡ����������Ǥä��Ȥ���ľ���Υ��޻������Ѵ����줿ʸ����\
���޻�����뤫�ɤ�������ꤷ�ޤ����ǥե���Ȥ�off�Ǥ���",
  "���ʴ����Ѵ����ؽ���Ԥ����ɤ�������ꤷ�ޤ����ǥե���Ȥ�on�Ǥ���",
  "����������֤Ǹ�������򤷤�ñ�����ɽ�����֤���ä��Ȥ��ˡ�������\
ʸ��򼡤�ʸ��˰�ư�����뤫�ɤ�������ꤷ�ޤ���off�ǥ�����ʸ��˰�ư\
���ޤ���on�Ǥϥ�����ʸ����Ѥ��ޤ��󡣥ǥե���Ȥ�on�Ǥ���",
  "�Ǳ�ʸ��Ǽ�ʸ��ذ�ư���褦�Ȥ����Ȥ��ˡ����ꤹ�뤫�ݤ�����ꤷ�ޤ���\
off����ꤹ��ȺǺ�ʸ�᤬������ʸ��ˤʤ�ޤ����ǥե���Ȥ�off�Ǥ���",
  "ñ����Ͽ���ʻ����ꤷ���塢�ܺ٤��ʻ�ʬ��Τ���μ����Ԥ����ݤ���\
���ꤷ�ޤ���on�Ǽ����Ԥ���off�Ǥϼ���򤷤ޤ��󡣥ǥե���Ȥ�on�Ǥ���",
  "�Ѵ������򲿲󤫲����ȸ������ɽ�����Ԥ���褦�ˤ����\
���Ǥ��ޤ��������ǤϤ��β�����������(��,��)�ǻ��ꤷ�ޤ���\
0��ˤ���Ȳ����Ѵ�������\
�����Ƥ����ɽ�����Ԥ��ʤ��ʤ�ޤ����ǥե���Ȥ�2��ǡ�2���Ѵ�������\
�����Ȱ���ɽ���ˤʤ�ޤ���",
  NULL
};
