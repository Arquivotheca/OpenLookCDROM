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

#ifndef lint
static char rcsid[]="@(#) 112.1 $Id: wtoc.c,v 1.18 1993/05/24 12:09:58 kon Exp $";
#endif
/* wtoi.c  �ƥ����ȷ����μ����ף���֤���ϡפΤ�Τ��Ѵ����롣
 *	wtoi [-f hinshifile] [wnndic] [irohadic]
 */
#include	<stdio.h>
#include        <ctype.h>

#if  __STDC__ || defined(SVR4)
#include <locale.h>
#endif

#ifdef SVR4
extern char *gettxt();
#else
#define	gettxt(x,y)  (y)
#endif

#define		MAXTANGO	256
#define         MAXHINSHI       100

#ifndef AIXV3
typedef	unsigned char	uchar;
#endif


struct hin{
  char *wnn;
  char *iroha;
};

static struct hin table[] = {
  {"̾��","#T35"},{"��̾","#JN"},{"��̾","#CN"},{"��̾&��̾","#JN"},
  {"��ͭ̾��","#KK"},{"����","#NN"},
  {"��ư��","#KSr"},
  {"���Ը���","#K5r"},{"���Ը���","#G5r"},{"���Ը���","#S5r"},
  {"���Ը���","#T5r"},{"�ʹԸ���","#N5"},{"�йԸ���","#B5r"},
  {"�޹Ը���","#M5r"},{"��Ը���","#R5r"},{"��Ը���","#W5r"},
  {"����","#KS"},{"����&̾��","#KSr"},{"����(�Ԥ�)","#C5r"},
  {"����(����)","#SX"},{"����(����)","#ZX"},{"����(����)&̾��","#T30"},
  {"��(��)","#kxo"},{"��(��)","#kxi"},{"��(��)","#kxuru"},
  {"��(��)","#sxi"},{"��(��)","#sxuru"},{"��(��)","#sxe"},
  {"���(������)","#L5"},
  {"���ƻ�","#KY"},
  {"����ư��","#T05"},{"����ư��&̾��","#T05"},{"����ư��(����)","#F00"},
  {"����","#F14"},{"Ϣ�λ�","#RT"},{"��³��,��ư��","#CJ"},
  {"ñ����","#KJ"},
  {"��Ƭ��","#PRE"},{"������","#SUC"},
  {"��Ƭ����","#NNPRE"},{"������","#JS"},{"��Ƭ������","#JS"},
  {"����������","#JSSUC"},
  {"������̾","#JNSUC"},{"��Ƭ��̾","#CNPRE"},{"������̾","#CNSUC1"},
  {"����ư�첽������","#N2T17"},{"����(����)&̾�첽������","#N2T30"},
  {"����ư��","#D2T35"},{"���ƻ첽����ư��","#D2KY"},
  {"��Ƭ��(��)","#PRE"},{"��Ƭ��(��)","#PRE"},{"����","#T35"}
};

char *salloc(s)
     char *s;
{
  char *new,*malloc();
  
  if (new = (char *)malloc(strlen( s ) + 1))
    strcpy(new, s);
  else{
    fprintf(stderr, gettxt("cannacmd:48", "No more memory\n"));
    exit(1);
  }
  return(new);
}

/* �ʻ��б� */
char *chghinshi( hinshi, taiou, fshurui )
     char   *hinshi;
     struct hin   *taiou;
     int    fshurui;  
{
  int   shurui;
  int   i;
  
  shurui = sizeof(table)/sizeof(struct hin);
  for( i = 0 ; i < shurui ; i++) {
    if( !strcmp( hinshi , table[i].wnn ) )
      return( table[i].iroha );
  }
  for( i = 0 ; i <  fshurui ; i++) {
    if( !strcmp( hinshi , taiou[i].wnn ) ) {
      return( taiou[i].iroha );
    }
  }
  return( "#??" );
}

/* �ʻ�ե������ɤ߹��� */
int read_hinshi( fp, taiou )
     FILE    *fp;
     struct  hin  *taiou;
{
  int     shurui;
  char    H[MAXTANGO];
  char    wnn[MAXTANGO],iroha[MAXTANGO];
  
  shurui = 0;
  while( fgets( H, MAXTANGO, fp ) ){
    if( 2 != sscanf( H, "%s %s", wnn, iroha ) )
      continue;
    else {
      taiou[shurui].wnn = salloc(wnn);
      taiou[shurui].iroha = salloc(iroha);
      shurui++;
    }
  }
  return ( shurui );
} 

/* ���� */
wtoi_write( fp, yomi, hinshi, kouho, hindo )
     FILE	*fp;
     uchar   *yomi, *hinshi, *kouho;
     int	hindo;
{
  if( !strcmp(hinshi,"#kxuru") || !strcmp(hinshi,"#sxuru")){
    strcat(yomi,"��");
    strcat(kouho,"��");
  }
  if( hindo == 0 )
    fprintf( fp, "%s %s %s \n", yomi, hinshi, kouho );
  else
    fprintf( fp, "%s %s*%d %s \n", yomi, hinshi, hindo, kouho );
}

int suuji(kazu)
     char *kazu;
{
  int i;

  for(i = 0; i < strlen(kazu); i++ ){
    if(!isdigit(kazu[i]))
      return(0);
  }
  return(atoi(kazu));
}

main(argc,argv)
     int  argc;
     char *argv[]; 
{
  struct hin taiou[MAXHINSHI];
  uchar	S[MAXTANGO],y[MAXTANGO], h[MAXTANGO], k[MAXTANGO],nd[10];
  int	d,option,fshurui;
  FILE	*fph,*fpi,*fpo;
  
#if  __STDC__ || defined(SVR4)
  (void)setlocale(LC_ALL,""); 
#endif 
  option = 0;
  if( argc <= 5 ) { /* �������������� */ 
    if( argc > 2 && !strcmp(argv[1],"-f") ) { /* �ʻ�ե�������ɤ߹��फ�� */
      if( (fph = fopen( argv[2], "r" ) ) == NULL) { /* �ʻ�ե���������� */
	fprintf(stderr,gettxt("cannacmd:49", "%s: cannot open %s\n"), argv[0], argv[2] );
	exit(2);
      }
      fshurui = read_hinshi( fph, taiou );
      close( fph );
      option = 1;
    }
    fpi = stdin;
    fpo = stdout;
  }
  else { /* ���������� */
    fprintf(stderr,gettxt("cannacmd:50", "Usage: wtoc [-f part-of-speach table] [wnndic] [cannadic]\n"),argv[0]);
    exit(2);
  }
  if( argc >= (2 + option*2) ) { /* �ף���򥪡��ץ� */
    if( (fpi = fopen( argv[1 + option*2], "r" ) ) == NULL) {
      fprintf(stderr,gettxt("cannacmd:51", "%s: cannot open %s\n"), argv[0], argv[1 + option*2] );
      exit(2);
    }
    if( argc == (3 + option*2) ) { /* ����ϼ���򥪡��ץ� */
      if( (fpo = fopen( argv[2 + option*2], "w" ) ) == NULL) {
	fprintf(stderr,gettxt("cannacmd:52", "%s: cannot create %s\n"), argv[0], argv[2 + option*2] );
	exit(2);
      }
    }
  }
  
  /* ����� */
  while( fgets( (char *)S, sizeof(S), fpi ) ) {
    if( 4 >  sscanf( (char *)S, "%s %s %s %s", y, k, h, nd ) )
      continue;
    else      
      d = suuji(nd);
      wtoi_write( fpo, y, chghinshi( h, taiou, fshurui ), k, d );
  } 
  
  fclose( fpi );
  fclose( fpo );
  exit( 0 );
}
