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
static char rcs_id[] = "@(#) 102.1 $Id: parse.c,v 4.22 1994/03/01 14:07:49 kon Exp $";
#endif /* lint */

#include "canna.h"

#include <stdio.h>
#include <fcntl.h>

extern char *CANNA_initfilename;

#define BUF_LEN 1024

static char CANNA_rcfilename[BUF_LEN] = "";

static int DISPLAY_to_hostname();

/* cfuncdef

   YYparse -- �������ޥ����ե�������ɤࡣ

   �ե�����ǥ�������ץ��ǻ��ꤵ�줿�ե�������ɤ߹��ࡣ

*/

extern ckverbose;

extern YYparse_by_rcfilename();

/* cfuncdef

  parse -- .canna �ե������õ���Ƥ����ɤ߹��ࡣ

  parse �ϥ������ޥ����ե������õ�������Υե�����򥪡��ץ󤷥ѡ�����
  �롣

  �ѡ�����Υե������̾���� CANNA_rcfilename ������Ƥ�����

  */

#define NAMEBUFSIZE 1024
#define RCFILENAME  ".canna"
#define FILEENVNAME "CANNAFILE"
#define HOSTENVNAME "CANNAHOST"

#define OBSOLETE_RCFILENAME  ".iroha"
#define OBSOLETE_FILEENVNAME "IROHAFILE"
#define OBSOLETE_HOSTENVNAME "IROHAHOST"

static
make_initfilename()
{
  if(!CANNA_initfilename) {
    CANNA_initfilename = malloc(1024);
    if (!CANNA_initfilename) {
      return -1;
    }
    strcpy(CANNA_initfilename, CANNA_rcfilename);
  }
  else {
    strcat(CANNA_initfilename, ",");
    strcat(CANNA_initfilename, CANNA_rcfilename);
  }
  return 0;
}

static void
fit_initfilename()
{
  char *tmpstr;

  if (CANNA_initfilename) {
    tmpstr = malloc(strlen(CANNA_initfilename) + 1);
    if (!tmpstr) return;
    strcpy(tmpstr, CANNA_initfilename);
    free(CANNA_initfilename);
    CANNA_initfilename = tmpstr;
  }
}

parse()
{
  char *p, *getenv();
  int n;
  extern iroha_debug;
  int home_canna_exist = 0;
  extern char *initFileSpecified;

  clisp_init();

  if (initFileSpecified) {
    strcpy(CANNA_rcfilename, initFileSpecified);
    if (YYparse_by_rcfilename(CANNA_rcfilename)) {
      make_initfilename();
      goto quitparse;
    }
    else {
      char buf[256];

      if (ckverbose) {
	printf("�������ޥ����ե�������ɤ߹��ߤޤ���\n");
      }

      sprintf(buf, "���ꤵ�줿������ե����� %s ��¸�ߤ��ޤ���",
	      CANNA_rcfilename);
      addWarningMesg(buf);
      goto quitparse;
    }
  }
  p = getenv(FILEENVNAME);
  if (p) {
    strcpy(CANNA_rcfilename, p);
    if (YYparse_by_rcfilename(CANNA_rcfilename)) {
      make_initfilename();
      goto quitparse;
    }
  }
#ifdef OBSOLETE_FILEENVNAME
  else if ((p = getenv(OBSOLETE_FILEENVNAME)) != (char *)0) {
    char buf[256];

    sprintf(buf,
"���: ������ե��������ꤹ�뤿��δĶ��ѿ� %s �����ꤵ��Ƥ�"
	    , OBSOLETE_FILEENVNAME);
    addWarningMesg(buf);
    sprintf(buf,
"      �ޤ������������Υ������ޥ����ե��������ꤹ�� %s ������"
	    , FILEENVNAME);
    addWarningMesg(buf);
    addWarningMesg(
"      ����Ƥ��ޤ��󡣿������Υ������ޥ����ե��������������Ķ��ѿ�"
		   );
    sprintf(buf,
"      %s �����ꤷ�Ʋ�������"
	    , FILEENVNAME);
    addWarningMesg(buf);
  }
#endif
  p = getenv("HOME");
  if (p) {
    strcpy(CANNA_rcfilename, p);
    strcat(CANNA_rcfilename, "/");
    strcat(CANNA_rcfilename, RCFILENAME);
    n = strlen(CANNA_rcfilename);

    /* $HOME/.canna */

    home_canna_exist = YYparse_by_rcfilename(CANNA_rcfilename);
    if (home_canna_exist) {
      make_initfilename();

      /* $HOME/.canna-DISPLAY */

      p = getenv("DISPLAY");
      if (p) {
	char display[NAMEBUFSIZE];
	
	DISPLAY_to_hostname(p, display, NAMEBUFSIZE);
	
	CANNA_rcfilename[n] = '-';
	strcpy(CANNA_rcfilename + n + 1, display);
	
	if(YYparse_by_rcfilename(CANNA_rcfilename)) {
	  make_initfilename();
	}
      }
      
      /* $HOME/.canna-TERM */
      
      p = getenv("TERM");
      if (p) {
	CANNA_rcfilename[n] = '-';
	strcpy(CANNA_rcfilename + n + 1, p);
	if(YYparse_by_rcfilename(CANNA_rcfilename)) {
	  make_initfilename();
	}	  
      }
    }
#ifdef OBSOLETE_RCFILENAME
    else { /* .canna ��¸�ߤ��Ƥ��ʤ� */
      strcpy(CANNA_rcfilename, p);
      strcat(CANNA_rcfilename, "/");
      strcat(CANNA_rcfilename, OBSOLETE_RCFILENAME);
      n = strlen(CANNA_rcfilename);
      if (close(open(CANNA_rcfilename, O_RDONLY)) == 0) { /* ���� */
	char buf[256];

	sprintf(buf,
"���: ������ν�����ե����� %s ��¸�ߤ��Ƥ��ޤ����������ν�����ե�"
		, OBSOLETE_RCFILENAME);
	addWarningMesg(buf);
	sprintf(buf,
"      ���� %s ��¸�ߤ��Ƥ��ޤ���canvert ���ޥ�ɤ����Ѥ��ƿ�����"
		, RCFILENAME);
	addWarningMesg(buf);
	sprintf(buf,
"      �ν�����ե����� %s ��������Ʋ�������"
		, RCFILENAME);
	addWarningMesg(buf);
	sprintf(buf,
"      (��) canvert -c -o ~/%s -n ~/%s"
		, OBSOLETE_RCFILENAME, RCFILENAME);
	addWarningMesg(buf);
      }
    }
#endif
  }

  if ( !home_canna_exist ) {
    /* �Ǹ�ϥ����ƥ�ǥե���ȤΥե�������ɤ� */
    strcpy(CANNA_rcfilename, CANNALIBDIR);
    n = strlen(CANNA_rcfilename);
  
    strcpy(CANNA_rcfilename + n, "/default");
    strcat(CANNA_rcfilename + n, RCFILENAME);
    if (YYparse_by_rcfilename(CANNA_rcfilename)) {
      make_initfilename();
      p = getenv("DISPLAY");
      if (p) {
	char display[NAMEBUFSIZE];
	
	DISPLAY_to_hostname(p, display, NAMEBUFSIZE);

	CANNA_rcfilename[n] = '/';
	strcpy(CANNA_rcfilename + n + 1, display);
	strcat(CANNA_rcfilename, RCFILENAME);
	if(YYparse_by_rcfilename(CANNA_rcfilename)) {
	  make_initfilename();
	}
      }

      p = getenv("TERM");
      if (p) {
	CANNA_rcfilename[n] = '/';
	strcpy(CANNA_rcfilename + n + 1, p);
	strcat(CANNA_rcfilename, RCFILENAME);
	if(YYparse_by_rcfilename(CANNA_rcfilename)) {
	  make_initfilename();
	}
      }
    }
    else {
      char buf[256];

      if (ckverbose) {
	printf("�������ޥ����ե�������ɤ߹��ߤޤ���\n");
      }
      sprintf(buf, "�����ƥ�ν�����ե����� %s ��¸�ߤ��ޤ���",
	      CANNA_rcfilename);
      addWarningMesg(buf);
    }
  }

 quitparse:
  /* CANNA_initfilename �򥸥㥹�ȥ������˴������ */
  fit_initfilename();
  clisp_fin();
}


static
DISPLAY_to_hostname(name, buf, bufsize)
char *name, *buf;
int bufsize;
{
  if (name[0] == ':' || !strncmp(name, "unix", 4)) {
    gethostname(buf, bufsize);
  }
  else {
    int i, len = strlen(name);
    for (i = 0 ; i < len && i < bufsize ; i++) {
      if (name[i] == ':') {
	break;
      }
      else {
	buf[i] = name[i];
      }
    }
    if (i < bufsize) {
      buf[i] = '\0';
    }
  }
}
