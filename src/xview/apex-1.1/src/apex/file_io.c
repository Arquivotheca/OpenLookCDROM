#ifndef lint
static char    *RCSid = "$Header: /n/homeserver/i/gounares/pu/apex/src/apex/RCS/file_io.c,v 1.1 93/01/06 03:27:34 gounares Exp Locker: gounares $";

#endif

/*
 * $Log:	file_io.c,v $
 * Revision 1.1  93/01/06  03:27:34  gounares
 * Initial revision
 * 
 */

/*
 * file_io.c
 * 
 * written by Alex Gounares
 * 
 * 9/22/92
 */

/*
 * Copyright 1993 Alexander Gounares
 * 
 * This source is covered by the GNU General Public License Version 2
 * 
 * see the apeX manual for more details
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xview/xview.h>
#include <xview/textsw.h>
#include <xview/panel.h>
#include <xview/notice.h>
#include <xview/svrimage.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "file_io.h"
#include "editor.h"
#include "misc.h"

static void     process_link();

Textsw_mark     textsw_add_glyph();

static char     szInbuf[5];

static void 
reset_buf()
{
	szInbuf[0] = szInbuf[1] = szInbuf[2] = szInbuf[3] = 0;
}

static char    *
get4(fp)
    FILE           *fp;
{
	int             i,
	                c;

	if (!szInbuf[0]) {
		szInbuf[0] = getc(fp);
		szInbuf[1] = getc(fp);
		szInbuf[2] = getc(fp);
		szInbuf[3] = getc(fp);
	} else {
		szInbuf[0] = szInbuf[1];
		szInbuf[1] = szInbuf[2];
		szInbuf[2] = szInbuf[3];
		szInbuf[3] = getc(fp);
	}
	return szInbuf;
}

/*
 * file_size -- size of a file in bytes
 */
static int
file_size(szFilename)
    char           *szFilename;
{
	struct stat     buf;

	if (stat(szFilename, &buf) == -1) {
		return -1;
	}
	return buf.st_size;
}

/*
 * check dates -- given an apex filename, check to see if the apex file is
 * older than the non-apex file.  NOTE THAT THIS FUNCTION MODIFIES
 * szFilename!!!
 */
static int
check_dates(szFilename)
    char           *szFilename;
{
	struct stat     buf1,
	                buf2;
	char           *temp;

	add_apex(szFilename);

	if (stat(szFilename, &buf1) == -1)
		return 1;

	remove_apex(szFilename);

	if (stat(szFilename, &buf2) == -1)
		return 1;

	if (buf1.st_mtime >= buf2.st_mtime)
		return 1;
	else
		return 0;
}

/*
 * load_file -- load a file, processing all found object links.  The filename
 * may be either full or relative, but if you try to pass a bogus filename,
 * then I'll send a NULL back to you hinting that you ought to rethink your
 * request.
 */
File           *
load_file(szFilename)
    char           *szFilename;
{
	File           *pFile;
	int             size,
	                c,
	                i = 0;
	FILE           *fp;
	char           *szTemp;

	pFile = (File *) acalloc(1, sizeof(File));

	if ((size = file_size(szFilename)) == -1) {
		return NULL;
	}
	/* now allocate memory and load in the file */

	/*
	 * we also know that the final size of the file will be <= to the
	 * original size
	 */

	/*
	 * the algorithm used here is subject to some debate.  One could
	 * argue that it would be better to block read the entire file and
	 * then process it and copy it into memory.  However, at least at
	 * Princeton, all disk operations go through NFS over a butt-slow
	 * ethernet, so we might as well do some processing on the data while
	 * waiting for the disk to catch up eons later.
	 */

	if ((fp = fopen(szFilename, "r")) == NULL) {
		return NULL;
	}
	pFile->szText = (char *) acalloc(size + 1, sizeof(char));

	reset_buf();

	while (*(szTemp = get4(fp)) != EOF) {
		if (!strncmp(szTemp, OBJ_START, 4)) {
			process_link(fp, pFile, i);
			reset_buf();
			continue;
		} else {
			pFile->szText[i++] = szTemp[0];
		}
	}

	split_name(szFilename, pFile->szDirname, pFile->szFilename);

	fclose(fp);

	return pFile;
}

/*
 * process_link -- assume that the beginning token of a link object has been
 * read...go through and process/read the link info.
 */
static void
process_link(fp, pFile, i)
    FILE           *fp;
    File           *pFile;
    int             i;
{
	int             id,
	                link_id,
	                j = 0;
	char            szFilename[MAXPATHLEN + MAXNAMLEN];
	Object_list    *pOL;
	char            szBuf[10000],
	               *szTemp;

	szInbuf[0] = '\0';

	while (*(szTemp = get4(fp)) != EOF) {
		if (!strncmp(szTemp, OBJ_END, 4))
			break;
		else
			szBuf[j++] = szTemp[0];
	}

	szBuf[j] = '\0';

	sscanf(szBuf, OBJ_SZFMT, &id, szFilename, &link_id);

	/*
	 * now go through the list to see if we have already encountered this
	 * token. Currently, objects may only be contiguous blocks of text,
	 * but this may change to allow an arbitrary "set" of blocks to be an
	 * defined as an object.  Anyway, hopefully this code will allow that
	 * enhancement to be made easily.
	 */

	for (pOL = pFile->pOL; pOL != NULL; pOL = pOL->next) {
		if (pOL->id == id) {

			/*
			 * we have found the ending delimitor on the object
			 * (or in the enhanced model, another sub block
			 * within the set
			 */
			if (link_id != pOL->link_id) {
				fprintf(stderr,
					"apeX->Error: Object id Mismatch\n");
				fprintf(stderr,
					"\tunpredictable behavior may occur...\n");
			}
			if (strcmp(szFilename, pOL->szLink_filename)) {
				fprintf(stderr,
					"apeX->Error: Object filename Mismatch\n");
				fprintf(stderr,
					"\tunpredictable behavior may occur...\n");
			}
			pOL->iend = i;
			return;
		}
	}

	/*
	 * if we got this far, then we have encountered the first object
	 * delimitor of id id...create a new Object List jobby and make
	 * things work
	 */

	pOL = (Object_list *) acalloc(1, sizeof(Object_list));
	pOL->id = id;
	if (id > pFile->cObjects) {
		pFile->cObjects = id + 1;
	}
	pOL->ibegin = i;

	pOL->next = pFile->pOL;
	pFile->pOL = pOL;

	if (pOL->next) {
		pOL->next->prev = pOL;
	}
	pOL->prev = NULL;

	strcpy(pOL->szLink_filename, szFilename);
	pOL->link_id = link_id;

	/*
	 * here we set the description field to be whatever is left over
	 * after the "normal" stuff
	 */

	szTemp = szBuf;
	for (j = 0; j < 4; j++) {
		szTemp = strchr(szTemp, ' ');
		szTemp++;
	}

	pOL->szDesc = strdup(szTemp);

}

/*
 * update_File -- update a File structure from the textsw in an Editor
 * structure
 */
File           *
update_File(pFile, pEditor)
    File           *pFile;
    Editor         *pEditor;
{
	Object_list    *pOL;
	int             size;

	for (pOL = pFile->pOL; pOL != NULL; pOL = pOL->next) {
		if (pOL->begin) {
			pOL->ibegin = (int) textsw_find_mark(pEditor->textsw,
				pOL->begin);
		}
		if (pOL->end) {
			pOL->iend = (int) textsw_find_mark(pEditor->textsw,
				pOL->end);
		}
	}

	/* now reset the text field */

	free(pFile->szText);
	size = (int) xv_get(pEditor->textsw, TEXTSW_LENGTH);
	pFile->szText = (char *) acalloc(size + 1, sizeof(char));
	xv_get(pEditor->textsw, TEXTSW_CONTENTS, 0, pFile->szText, size);

	return pFile;
}

/*
 * update_textsw -- update a textsw from the File structure
 */
File           *
update_textsw(pFile, pEditor)
    File           *pFile;
    Editor         *pEditor;
{
	Object_list    *pOL;

	for (pOL = pFile->pOL; pOL != NULL; pOL = pOL->next) {
		if (pOL->begin) {
			textsw_remove_mark(pEditor->textsw, pOL->begin);
		}
		if (pOL->end) {
			textsw_remove_mark(pEditor->textsw, pOL->end);
		}
		pOL->begin = textsw_add_mark(pEditor->textsw,
			pOL->ibegin,
			TEXTSW_MARK_DEFAULTS);
		pOL->end = textsw_add_mark(pEditor->textsw,
			pOL->iend,
			TEXTSW_MARK_DEFAULTS);


	}

	/* now update the names in pEditor */

	strcpy(pEditor->szFilename, pFile->szFilename);
	strcpy(pEditor->szDirname, pFile->szDirname);

	return pFile;
}

/*
 * fFiletoTextsw -- given a File structure and an Editor structure, load the
 * File into the textsw of the Editor
 */
int
fFiletoTextsw(pFile, pEditor)
    File           *pFile;
    Editor         *pEditor;
{
	int             size;
	FILE           *fp;
	char            szFullname[MAXPATHLEN + MAXNAMLEN];


	size = strlen(pFile->szText);

	remove_apex(pFile->szFilename);

	sprintf(szFullname, "%s/%s", pFile->szDirname, pFile->szFilename);

	if (!check_dates(szFullname)) {
		char            szBuf[1024];
		int             answer;

		sprintf(szBuf, "File '%s' was modified more recently than '%s.apex'",
			pFile->szFilename, pFile->szFilename);

		answer = notice_prompt(pEditor->frame, NULL,
			NOTICE_MESSAGE_STRINGS,
			szBuf,
			"You may lose data if you continue with this operation",
			NULL,
			NOTICE_BUTTON_YES, "Cancel",
			NOTICE_BUTTON_NO, "Continue",
			NULL);
		if (answer != NOTICE_NO)
			return 1;
	}
	remove_apex(szFullname);

	if (pFile->pOL) {
		if ((fp = fopen(szFullname, "w")) == NULL) {
			fprintf(stderr,
				"apeX->Error: problems opening file '%s' for writing\n",
				szFullname);
			return 0;
		}

		/*
		 * we are going to write the text to a file and then set the
		 * TEXTSW_FILE field of the editor to load the file.  This is
		 * done to 1. keep the textsw edit buffer smaller and 2. to
		 * let us use some of the nicer features of the textsw's like
		 * checkpointing and some recovery of edits when things f*&!
		 * up...
		 */

		write(fileno(fp), pFile->szText, size);

		fclose(fp);
	} else {
		if ((fp = fopen(szFullname, "r")) == NULL) {
			if ((fp = fopen(szFullname, "w")) == NULL) {
				fprintf(stderr,
					"apeX->Error: problems opening file '%s' for writing\n",
					szFullname);
				return 0;
			}
			write(fileno(fp), pFile->szText, size);
			fclose(fp);
		}
	}


	xv_set(pEditor->textsw, TEXTSW_FILE, szFullname,
		TEXTSW_FIRST, 0,
		NULL);

	add_apex(szFullname);

#ifdef hpux
	utime(szFullname, NULL);
#else hpux
	utimes(szFullname, NULL);
#endif hpux

	pEditor->pFile = update_textsw(pFile, pEditor);

	pEditor->pOL_current = NULL;
	update_object_status(pEditor);
	return 1;
}

static struct _temp {
	int             line;
	Object_list    *pOL;
};

static int
compare(a, b)
    struct _temp   *a,
                   *b;
{
	return a->line - b->line;
}


/*
 * save_file -- save a file to disk, inserting all relevant object links
 */
int
save_file(szFilename, pEditor)
    char           *szFilename;
    Editor         *pEditor;
{
	int             count = 0,
	                i = 0;
	Object_list    *pOL;
	struct _temp   *rgtemp;
	FILE           *fp;
	char            szBuf[1024];

	/* first, update the File structure of the Editor */

	pEditor->pFile = update_File(pEditor->pFile, pEditor);

	split_name(szFilename, pEditor->pFile->szDirname,
		pEditor->pFile->szFilename);

	for (pOL = pEditor->pFile->pOL; pOL != NULL; pOL = pOL->next) {
		count += 2;
	}

	/* this can be non-portable because of malloc(0) case */

	rgtemp = (struct _temp *) amalloc(count * sizeof(struct _temp));

	rgtemp[0].line = -1;
	for (pOL = pEditor->pFile->pOL; pOL != NULL; pOL = pOL->next) {
		rgtemp[i].line = pOL->ibegin;
		rgtemp[i].pOL = pOL;
		i++;
		rgtemp[i].line = pOL->iend;
		rgtemp[i].pOL = pOL;
		i++;
	}

	qsort(rgtemp, count, sizeof(struct _temp), compare);


	remove_apex(szFilename);

	if (textsw_append_file_name(pEditor->textsw, szBuf) != 0) {
		textsw_store_file(pEditor->textsw, szFilename, 0, 0);
	} else {
		textsw_save(pEditor->textsw, 0, 0);
	}

	/* we'll only write out a .apex file if there are links in the file */
	if (count) {
		int             j = 0;

		i = 0;

		add_apex(szFilename);

		if ((fp = fopen(szFilename, "w")) == NULL) {
			fprintf(stderr,
				"apeX->Error: problem opening file '%s' for writing\n",
				szFilename);
			return 0;
		}
		while (pEditor->pFile->szText[i] != '\0') {
			if (j < count && i == rgtemp[j].line) {
				fprintf(fp, "%s %d %s %d %s %s",
					OBJ_START,
					rgtemp[j].pOL->id,
					rgtemp[j].pOL->szLink_filename,
					rgtemp[j].pOL->link_id,
					rgtemp[j].pOL->szDesc,
					OBJ_END);
				j++;
			}
			putc(pEditor->pFile->szText[i++], fp);
		}

		fclose(fp);
	}
	return 1;
}

/*
 * remove_apex -- if the end of a string ends with ".apex", remove it
 */
void
remove_apex(szFilename)
    char           *szFilename;
{
	int             len;

	len = strlen(szFilename);

	if (len <= APEX_EXT_LEN) {
		return;
	}
	if (!strcmp(szFilename + len - APEX_EXT_LEN, APEX_EXT)) {
		szFilename[len - APEX_EXT_LEN] = '\0';
	}
}

/*
 * add_apex -- if .apex does not exist at the end of a string, add it
 */
void
add_apex(szFilename)
    char           *szFilename;
{
	remove_apex(szFilename);
	strcat(szFilename, APEX_EXT);
}

/*
 * destroy_file -- deallocate storage for a file structure
 */
void
destroy_file(pFile)
    File           *pFile;
{
	Object_list    *pOL,
	               *pOLnext;

	if (pFile) {
		for (pOL = pFile->pOL; pOL != NULL;) {
			pOLnext = pOL->next;
			free(pOL);
			pOL = pOLnext;
		}
		if (pFile->szText) {
			free(pFile->szText);
		}
		free(pFile);
	}
}
