/* @(#)rpc_util.h	1.2 87/11/24 3.9 RPCSRC */

/*
	$Disclaimer: 
 * Permission to use, copy, modify, and distribute this software and its 
 * documentation for any purpose is hereby granted without fee, 
 * provided that the above copyright notice appear in all copies and that 
 * both that copyright notice, this permission notice, and the following 
 * disclaimer appear in supporting documentation, and that the names of 
 * IBM, Carnegie Mellon University, and other copyright holders, not be 
 * used in advertising or publicity pertaining to distribution of the software 
 * without specific, written prior permission.
 * 
 * IBM, CARNEGIE MELLON UNIVERSITY, AND THE OTHER COPYRIGHT HOLDERS 
 * DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING 
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.  IN NO EVENT 
 * SHALL IBM, CARNEGIE MELLON UNIVERSITY, OR ANY OTHER COPYRIGHT HOLDER 
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY 
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, 
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS 
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
 * OF THIS SOFTWARE.
 *  $
*/

/*
 * Sun RPC is a product of Sun Microsystems, Inc. and is provided for
 * unrestricted use provided that this legend is included on all tape
 * media and as a part of the software program in whole or part.  Users
 * may copy or modify Sun RPC without charge, but are not authorized
 * to license or distribute it to anyone else except as part of a product or
 * program developed by the user.
 * 
 * SUN RPC IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING THE
 * WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 * 
 * Sun RPC is provided with no support and without any obligation on the
 * part of Sun Microsystems, Inc. to assist in its use, correction,
 * modification or enhancement.
 * 
 * SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
 * INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY SUN RPC
 * OR ANY PART THEREOF.
 * 
 * In no event will Sun Microsystems, Inc. be liable for any lost revenue
 * or profits or other special, indirect and consequential damages, even if
 * Sun has been advised of the possibility of such damages.
 * 
 * Sun Microsystems, Inc.
 * 2550 Garcia Avenue
 * Mountain View, California  94043
 */
/* @(#)rpc_util.h 1.6 87/06/24 (C) 1987 SMI */

/*
 * rpc_util.h, Useful definitions for the RPC protocol compiler 
 * Copyright (C) 1987, Sun Microsystems, Inc. 
 */
extern char *malloc();

#define alloc(size)		malloc((unsigned)(size))
#define ALLOC(object)   (object *) malloc(sizeof(object))

#define s_print	(void) sprintf
#define f_print (void) fprintf

struct list {
	char *val;
	struct list *next;
};
typedef struct list list;

/*
 * Global variables 
 */
#define MAXLINESIZE 1024
extern char curline[MAXLINESIZE];
extern char *where;
extern int linenum;

extern char *infilename;
extern FILE *fout;
extern FILE *fin;

extern list *defined;

/*
 * rpc_util routines 
 */
void storeval();

#define STOREVAL(list,item)	\
	storeval(list,(char *)item)

char *findval();

#define FINDVAL(list,item,finder) \
	findval(list, (char *) item, finder)

char *fixtype();
char *stringfix();
void pvname();
void ptype();
int isvectordef();
int streq();
void error();
void expected1();
void expected2();
void expected3();
void tabify();
void record_open();

/*
 * rpc_cout routines 
 */
void cprint();
void emit();

/*
 * rpc_hout routines 
 */
void print_datadef();

/*
 * rpc_svcout routines 
 */
void write_most();
void write_register();
void write_rest();
void write_programs();

/*
 * rpc_clntout routines
 */
void write_stubs();
