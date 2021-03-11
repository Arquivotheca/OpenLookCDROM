/* ********************************************************************** *\
 *         Copyright IBM Corporation 1988,1991 - All Rights Reserved      *
 *        For full copyright information see:'andrew/config/COPYRITE'     *
\* ********************************************************************** */

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
static char rcsid[]="$Header: /afs/cs.cmu.edu/project/atk-dist/auis-6.3/ams/libs/ms/RCS/abspath.c,v 2.10 1994/03/31 20:13:45 rr2b Exp $";
#endif

/*
 * abspath -- determine absolute pathname
 *
 *     abspath (name,result)
 *     char *name;
 *     char *result;
 *
 * Abspath places the absolute pathname of the string name into
 * the string result.
 *
 * Abspath takes a pathname and converts it to an absolute pathname by
 * prepending the name of the current working directory if necessary.
 * Then the absolute pathname is compacted by removing and resolving
 * superfluous steps.
 *
 * Steps of "" (two adjacent slashes) and steps of "." are removed
 * because they have no effect on the meaning of the pathname.
 *
 * Steps of ".." are resolved by removing them together with the
 * preceeding step.  However, resolution is not possible if the
 * preceeding step is also ".."
 *
 * Abspath calls getwd to obtain the name of the current working
 * directory when needed.  To improve performance, the result from
 * getwd is saved so that getwd need not be invoked again during
 * subsequent calls on abspath.  If you change the current working
 * directory (via chdir) you must call abspath(0,0) which causes
 * abspath to flush its saved result from getwd.  If you do not do
 * this abspath will continue to use its saved result from getwd
 * and this will most likely cause it to produce erronious results.
 *
 * Abspath returns 0 on success and -1 on failure.  The only failure
 * that can happen is a failure of getwd.  See the documentation on
 * getwd.  Failures in getwd are pretty catastrophic.
 *
 */


#define TRUE   1
#define FALSE  0

#include <andrewos.h>

static char havecurwd = FALSE;
static char curwd [1024];    /* remember the current working directory */

int abspath (name,result)

char * name;
char * result;
{
    register char * src;    /* source pointer for copy operations */
    register char * dst;    /* destination pointer for copy operations */
    register char * fence;  /* pointer to slash that cannot be backed over */
    register char * t;      /* scanback pointer in dst when we hit a slash */

    fence = 0;
    if (name == 0  ||  result == 0)
    {
	havecurwd = FALSE;  /* flush knowledge of current working directory */
	return (0);
    }

    /*
     * Construct the initial result pathname, which is basically just
     * a concatenation of the current working directory (if the name
     * is a relative pathname) and the name.  If we need to know the
     * current working directory but don't have it saved away, we call
     * getwd to figure it out for us.
     */

    dst = result;

    if (name[0] != '/')
    {
	if (!havecurwd  &&  getwd(curwd)==NULL)  return (-1);
	havecurwd = TRUE;

	src = curwd;
	while ((*dst++ = *src++) != 0)  ;    /* copy curwd to result */
	dst[-1] = '/';                       /* tack on a trailing slash */
    }

    src = name;
    while ((*dst++ = *src++) != 0)  ;   /* copy name to result */
    dst[-1] = '/';                      /* tack on a trailing slash */
    *dst = 0;                           /* make it null-terminated */

    /*
     * Now scan through result and compact the pathname.
     *
     *   "//"      =>  "/"
     *   "/./"     =>  "/"
     *   "/x/../"  =>  "/"
     *
     * where x is a string without a slash.  Note that x
     * cannot be "", ".", or ".."
     *
     * There is guaranteed to be a trailing slash on result when
     * we start, so that we don't need any special cases to handle
     * trailing steps--all steps in the pathname end with a slash.
     *
     * The fence points to the most recent slash that ".." cannot
     * back over.  Basically, all steps to the left of the fence
     * are ".."  Initially the fence points to the first slash.  We
     * are paranoid so we scan for the first slash.  Any characters
     * coming before the first slash (which must be the result of
     * getwd) are assumed to be magical incantations and we leave
     * them alone.  This is never expected to happen, but who knows?
     */

    src = result;
    dst = result;

    while (*src)
    {
	if ((*dst++ = *src++) == '/')
	{
	    fence = dst-1;  /* set fence to first slash */
	    break;
	}
    }

    while (*src)
    {
	if ((*dst++ = *src++) == '/')
	{
	    t = dst-1;      /* address of slash */


	    switch (*--t)
	    {
	    case '/':           /* found "//" */
		dst = t+1;      /* take off "/" */
		break;

	    case '.': 
		switch (*--t)
		{
		case '/':           /* found "/./" */
		    dst = t+1;      /* take off "./" */
		    break;

		case '.': 
		    if (*--t == '/')
		    {                   /* found "/../" */
			if (t == fence)
			{                   /* it is a leading ".." */
			    fence = dst-1;  /* move fence over it */
			}
			else
			{
			    while (*--t != '/')  ;
			    dst = t+1;      /* take off "x/../" */
			}
		    }
		    break;
		}
		break;
	    }
	}
    }

    *dst = 0;      /* null-terminate the result */
    /*
     * Now get rid of a trailing slash provided it is not also an
     * initial slash.
     *
     * Note that we tacked on a trailing slash originally and the
     * compaction shouldn't affect it so it should still be there,
     * but we check anyway because we're paranoid.
     */

    if (--dst > result  &&  *dst == '/')  *dst = 0;
    return (0);
}
