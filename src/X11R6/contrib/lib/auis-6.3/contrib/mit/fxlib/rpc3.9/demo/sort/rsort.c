/* @(#)rsort.c	1.2 87/11/24 3.9 RPCSRC */

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

#ifndef NORCSID
#define NORCSID
static char rcsid[]="$Header: /afs/cs.cmu.edu/project/atk-dist/auis-6.3/contrib/mit/fxlib/rpc3.9/demo/sort/RCS/rsort.c,v 1.3 1992/12/15 21:53:39 rr2b R6tape $";
#endif

/*
 * rsort.c
 * Client side application which sorts argc, argv.
 */
#include <stdio.h>
#include <rpc/rpc.h>
#include "sort.h"

main(argc, argv)
	int argc;
	char **argv;
{
	char *machinename;
	struct sortstrings args, res;
	int i;

	if (argc < 3) {
		fprintf(stderr, "usage: %s machinename [s1 ...]\n", argv[0]);
		exit(1);
	}
	machinename = argv[1];
	args.ss.ss_len = argc - 2;     /* substract off progname, machinename */
	args.ss.ss_val = &argv[2];
	res.ss.ss_val = (char **)NULL;

	if ((i = callrpc(machinename, SORTPROG, SORTVERS, SORT,
	    xdr_sortstrings, &args, xdr_sortstrings, &res)))
	{
	    fprintf(stderr, "%s: call to sort service failed. ", argv[0]);
	    clnt_perrno(i);
	    fprintf(stderr, "\n");
	    exit(1);
	}

	for (i = 0; i < res.ss.ss_len; i++) {
		printf("%s\n", res.ss.ss_val[i]);
	}

	/* should free res here */
	exit(0);
}
