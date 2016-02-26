/*	EVAL.C:	Expresion evaluation functions for
		MicroEMACS

	written 1986 by Daniel Lawrence				*/

#include	<stdio.h>
#include	"estruct.h"
#include	"edef.h"
#include	"evar.h"

varinit()		/* initialize the user variable list */

{
	register int i;

	for (i=0; i < MAXVARS; i++)
		uv[i].u_name[0] = 0;
}

char *gtfun(fname)	/* evaluate a function */

char *fname;		/* name of function to evaluate */

{
	register int fnum;		/* index to function to eval */
	register int status;		/* return status */
	char arg1[NSTRING];		/* value of first argument */
	char arg2[NSTRING];		/* value of second argument */
	char arg3[NSTRING];		/* value of third argument */
	static char result[2 * NSTRING];	/* string result */
#if	ENVFUNC
	char *getenv();
#endif

	/* look the function up in the function table */
	fname[3] = 0;	/* only first 3 chars significant */
	mklower(fname);	/* and let it be upper or lower case */
	for (fnum = 0; fnum < NFUNCS; fnum++)
		if (strcmp(fname, funcs[fnum].f_name) == 0)
			break;

	/* return errorm on a bad reference */
	if (fnum == NFUNCS)
		return(errorm);

	/* if needed, retrieve the first argument */
	if (funcs[fnum].f_type >= MONAMIC) {
		if ((status = macarg(arg1)) != TRUE)
			return(errorm);

		/* if needed, retrieve the second argument */
		if (funcs[fnum].f_type >= DYNAMIC) {
			if ((status = macarg(arg2)) != TRUE)
				return(errorm);
	
			/* if needed, retrieve the third argument */
			if (funcs[fnum].f_type >= TRINAMIC)
				if ((status = macarg(arg3)) != TRUE)
					return(errorm);
		}
	}
		

	/* and now evaluate it! */
	switch (fnum) {
		case UFADD:	return(itoa(atoi(arg1) + atoi(arg2)));
		case UFSUB:	return(itoa(atoi(arg1) - atoi(arg2)));
		case UFTIMES:	return(itoa(atoi(arg1) * atoi(arg2)));
		case UFDIV:	return(itoa(atoi(arg1) / atoi(arg2)));
		case UFMOD:	return(itoa(atoi(arg1) % atoi(arg2)));
		case UFNEG:	return(itoa(-atoi(arg1)));
		case UFCAT:	strcpy(result, arg1);
				return(strcat(result, arg2));
		case UFLEFT:	return(strncpy(result, arg1, atoi(arg2)));
		case UFRIGHT:	return(strcpy(result, &arg1[atoi(arg2)-1]));
		case UFMID:	return(strncpy(result, &arg1[atoi(arg2)-1],
					atoi(arg3)));
		case UFNOT:	return(ltos(stol(arg1) == FALSE));
		case UFEQUAL:	return(ltos(atoi(arg1) == atoi(arg2)));
		case UFLESS:	return(ltos(atoi(arg1) < atoi(arg2)));
		case UFGREATER:	return(ltos(atoi(arg1) > atoi(arg2)));
		case UFSEQUAL:	return(ltos(strcmp(arg1, arg2) == 0));
		case UFSLESS:	return(ltos(strcmp(arg1, arg2) < 0));
		case UFSGREAT:	return(ltos(strcmp(arg1, arg2) > 0));
		case UFIND:	return(getval(arg1));
		case UFAND:	return(ltos(stol(arg1) && stol(arg2)));
		case UFOR:	return(ltos(stol(arg1) || stol(arg2)));
		case UFLENGTH:	return(itoa(strlen(arg1)));
		case UFUPPER:	return(mkupper(arg1));
		case UFLOWER:	return(mklower(arg1));
		case UFTRUTH:	return(ltos(atoi(arg1) == 42));
		case UFASCII:	return(itoa((int)arg1[0]));
		case UFCHR:	result[0] = atoi(arg1);
				result[1] = 0;
				return(result);
		case UFGTKEY:	result[0] = tgetc();
				result[1] = 0;
				return(result);
		case UFRND:	return(itoa((ernd() % abs(atoi(arg1))) + 1));
		case UFABS:	return(itoa(abs(atoi(arg1))));
		case UFSINDEX:	return(itoa(sindex(arg1, arg2)));
		case UFENV:
#if	ENVFUNC
				return(getenv(arg1) == NULL ? "" : getenv(arg1));
#else
				return("");
#endif
		case UFBIND:	return(transbind(arg1));
	}

	exit(-11);	/* never should get here */
}

char *gtusr(vname)	/* look up a user var's value */

char *vname;		/* name of user variable to fetch */

{

	register int vnum;	/* ordinal number of user var */

	/* scan the list looking for the user var name */
	for (vnum = 0; vnum < MAXVARS; vnum++)
		if (strcmp(vname, uv[vnum].u_name) == 0)
			break;

	/* return errorm on a bad reference */
	if (vnum == MAXVARS)
		return(errorm);

	return(uv[vnum].u_value);
}

char *gtenv(vname)

char *vname;		/* name of environment variable to retrieve */

{
	register int vnum;	/* ordinal number of var refrenced */
	char *getkill();

	/* scan the list, looking for the referenced name */
	for (vnum = 0; vnum < NEVARS; vnum++)
		if (strcmp(vname, envars[vnum]) == 0)
			break;

	/* return errorm on a bad reference */
	if (vnum == NEVARS)
		return(errorm);

	/* otherwise, fetch the appropriate value */
	switch (vnum) {
		case EVFILLCOL:	return(itoa(fillcol));
		case EVPAGELEN:	return(itoa(term.t_nrow + 1));
		case EVCURCOL:	return(itoa(getccol(FALSE)));
		case EVCURLINE: return(itoa(getcline()));
		case EVRAM:	return(itoa((int)(envram / 1024l)));
		case EVFLICKER:	return(ltos(flickcode));
		case EVCURWIDTH:return(itoa(term.t_nrow));
		case EVCBUFNAME:return(curbp->b_bname);
		case EVCFNAME:	return(curbp->b_fname);
		case EVSRES:	return(sres);
		case EVDEBUG:	return(ltos(macbug));
		case EVSTATUS:	return(ltos(cmdstatus));
		case EVPALETTE:	return(palstr);
		case EVASAVE:	return(itoa(gasave));
		case EVACOUNT:	return(itoa(gacount));
		case EVLASTKEY: return(itoa(lastkey));
		case EVCURCHAR:
			return(curwp->w_dotp->l_used ==
					curwp->w_doto ? itoa('\n') :
				itoa(lgetc(curwp->w_dotp, curwp->w_doto)));
		case EVDISCMD:	return(ltos(discmd));
		case EVVERSION:	return(VERSION);
		case EVPROGNAME:return(PROGNAME);
		case EVSEED:	return(itoa(seed));
		case EVDISINP:	return(ltos(disinp));
		case EVWLINE:	return(itoa(curwp->w_ntrows));
		case EVCWLINE:	return(itoa(getwpos()));
		case EVTARGET:	saveflag = lastflag;
				return(itoa(curgoal));
		case EVSEARCH:	return(pat);
		case EVREPLACE:	return(rpat);
		case EVMATCH:	return((patmatch == NULL)? "": patmatch);
		case EVKILL:	return(getkill());
		case EVCMODE:	return(itoa(curbp->b_mode));
		case EVGMODE:	return(itoa(gmode));
		case EVTPAUSE:	return(itoa(term.t_pause));
		case EVPENDING:
#if	TYPEAH
				return(ltos(typahead()));
#else
				return(falsem);
#endif
		case EVLWIDTH:	return(itoa(llength(curwp->w_dotp)));
		case EVLINE:	return(getctext());
	}
	exit(-12);	/* again, we should never get here */
}

char *getkill()		/* return some of the contents of the kill buffer */

{
	register int size;	/* max number of chars to return */
	char value[NSTRING];	/* temp buffer for value */

	if (kbufh == NULL)
		/* no kill buffer....just a null string */
		value[0] = 0;
	else {
		/* copy in the contents... */
		if (kused < NSTRING)
			size = kused;
		else
			size = NSTRING - 1;
		strncpy(value, kbufh->d_chunk, size);
	}

	/* and return the constructed value */
	return(value);
}

int setvar(f, n)		/* set a variable */

int f;		/* default flag */
int n;		/* numeric arg (can overide prompted value) */

{
	register int status;	/* status return */
#if	DEBUGM
	register char *sp;	/* temp string pointer */
	register char *ep;	/* ptr to end of outline */
#endif
	VDESC vd;		/* variable num/type */
	char var[NVSIZE+1];	/* name of variable to fetch */
	char value[NSTRING];	/* value to set variable to */

	/* first get the variable to set.. */
	if (clexec == FALSE) {
		status = mlreply("Variable to set: ", &var[0], NVSIZE);
		if (status != TRUE)
			return(status);
	} else {	/* macro line argument */
		/* grab token and skip it */
		execstr = token(execstr, var);
	}

	/* check the legality and find the var */
	findvar(var, &vd);
	
	/* if its not legal....bitch */
	if (vd.v_type == -1) {
		mlwrite("%%No such variable as '%s'", var);
		return(FALSE);
	}

	/* get the value for that variable */
	if (f == TRUE)
		strcpy(value, itoa(n));
	else {
		status = mlreply("Value: ", &value[0], NSTRING);
		if (status != TRUE)
			return(status);
	}

	/* and set the appropriate value */
	status = svar(&vd, value);

#if	DEBUGM
	/* if $debug == TRUE, every assignment will echo a statment to
	   that effect here. */
	
	if (macbug) {
		strcpy(outline, "(((");

		/* assignment status */
		strcat(outline, ltos(status));
		strcat(outline, ":");

		/* variable name */
		strcat(outline, var);
		strcat(outline, ":");

		/* and lastly the value we tried to assign */
		strcat(outline, value);
		strcat(outline, ")))");

		/* expand '%' to "%%" so mlwrite wont bitch */
		sp = outline;
		while (*sp)
			if (*sp++ == '%') {
				/* advance to the end */
				ep = --sp;
				while (*ep++)
					;
				/* null terminate the string one out */
				*(ep + 1) = 0;
				/* copy backwards */
				while(ep-- > sp)
					*(ep + 1) = *ep;

				/* and advance sp past the new % */
				sp += 2;					
			}

		/* write out the debug line */
		mlforce(outline);
		update(TRUE);

		/* and get the keystroke to hold the output */
		if (get1key() == abortc) {
			mlforce("[Macro aborted]");
			status = FALSE;
		}
	}
#endif

	/* and return it */
	return(status);
}

findvar(var, vd)	/* find a variables type and name */

char *var;	/* name of var to get */
VDESC *vd;	/* structure to hold type and ptr */

{
	register int vnum;	/* subscript in varable arrays */
	register int vtype;	/* type to return */

fvar:	vtype = -1;
	switch (var[0]) {

		case '$': /* check for legal enviromnent var */
			for (vnum = 0; vnum < NEVARS; vnum++)
				if (strcmp(&var[1], envars[vnum]) == 0) {
					vtype = TKENV;
					break;
				}
			break;

		case '%': /* check for existing legal user variable */
			for (vnum = 0; vnum < MAXVARS; vnum++)
				if (strcmp(&var[1], uv[vnum].u_name) == 0) {
					vtype = TKVAR;
					break;
				}
			if (vnum < MAXVARS)
				break;

			/* create a new one??? */
			for (vnum = 0; vnum < MAXVARS; vnum++)
				if (uv[vnum].u_name[0] == 0) {
					vtype = TKVAR;
					strcpy(uv[vnum].u_name, &var[1]);
					break;
				}
			break;

		case '&':	/* indirect operator? */
			var[4] = 0;
			if (strcmp(&var[1], "ind") == 0) {
				/* grab token, and eval it */
				execstr = token(execstr, var);
				strcpy(var, getval(var));
				goto fvar;
			}
	}

	/* return the results */
	vd->v_num = vnum;
	vd->v_type = vtype;
	return;
}

int svar(var, value)		/* set a variable */

VDESC *var;	/* variable to set */
char *value;	/* value to set to */

{
	register int vnum;	/* ordinal number of var refrenced */
	register int vtype;	/* type of variable to set */
	register int status;	/* status return */
	register int c;		/* translated character */
	register char * sp;	/* scratch string pointer */

	/* simplify the vd structure (we are gonna look at it a lot) */
	vnum = var->v_num;
	vtype = var->v_type;

	/* and set the appropriate value */
	status = TRUE;
	switch (vtype) {
	case TKVAR: /* set a user variable */
		if (uv[vnum].u_value != NULL)
			free(uv[vnum].u_value);
		sp = malloc(strlen(value) + 1);
		if (sp == NULL)
			return(FALSE);
		strcpy(sp, value);
		uv[vnum].u_value = sp;
		break;

	case TKENV: /* set an environment variable */
		status = TRUE;	/* by default */
		switch (vnum) {
		case EVFILLCOL:	fillcol = atoi(value);
				break;
		case EVPAGELEN:	status = newsize(TRUE, atoi(value));
				break;
		case EVCURCOL:	status = setccol(atoi(value));
				break;
		case EVCURLINE:	status = gotoline(TRUE, atoi(value));
				break;
		case EVRAM:	break;
		case EVFLICKER:	flickcode = stol(value);
				break;
		case EVCURWIDTH:status = newwidth(TRUE, atoi(value));
				break;
		case EVCBUFNAME:strcpy(curbp->b_bname, value);
				curwp->w_flag |= WFMODE;
				break;
		case EVCFNAME:	strcpy(curbp->b_fname, value);
				curwp->w_flag |= WFMODE;
				break;
		case EVSRES:	status = TTrez(value);
				break;
		case EVDEBUG:	macbug = stol(value);
				break;
		case EVSTATUS:	cmdstatus = stol(value);
				break;
		case EVPALETTE:	strncpy(palstr, value, 48);
				spal(palstr);
				break;
		case EVASAVE:	gasave = atoi(value);
				break;
		case EVACOUNT:	gacount = atoi(value);
				break;
		case EVLASTKEY:	lastkey = atoi(value);
				break;
		case EVCURCHAR:	ldelete(1, FALSE);	/* delete 1 char */
				c = atoi(value);
				if (c == '\n')
					lnewline(FALSE, 1);
				else
					linsert(1, c);
				backchar(FALSE, 1);
				break;
		case EVDISCMD:	discmd = stol(value);
				break;
		case EVVERSION:	break;
		case EVPROGNAME:break;
		case EVSEED:	seed = atoi(value);
				break;
		case EVDISINP:	disinp = stol(value);
				break;
		case EVWLINE:	status = resize(TRUE, atoi(value));
				break;
		case EVCWLINE:	status = forwline(TRUE,
						atoi(value) - getwpos());
				break;
		case EVTARGET:	curgoal = atoi(value);
				thisflag = saveflag;
				break;
		case EVSEARCH:	strcpy(pat, value);
				rvstrcpy(tap, pat);
				mcclear();
				break;
		case EVREPLACE:	strcpy(rpat, value);
				break;
		case EVMATCH:	break;
		case EVKILL:	break;
		case EVCMODE:	curbp->b_mode = atoi(value);
				curwp->w_flag |= WFMODE;
				break;
		case EVGMODE:	gmode = atoi(value);
				break;
		case EVTPAUSE:	term.t_pause = atoi(value);
				break;
		case EVPENDING:	break;
		case EVLWIDTH:	break;
		case EVLINE:	putctext(value);
		}
		break;
	}
	return(status);
}

/*	atoi:	ascii string to integer......This is too
		inconsistant to use the system's	*/

atoi(st)

char *st;

{
	int result;	/* resulting number */
	int sign;	/* sign of resulting number */
	char c;		/* current char being examined */

	result = 0;
	sign = 1;

	/* skip preceding whitespace */
	while (*st == ' ' || *st == '\t')
		++st;

	/* check for sign */
	if (*st == '-') {
		sign = -1;
		++st;
	}
	if (*st == '+')
		++st;

	/* scan digits, build value */
	while ((c = *st++))
		if (c >= '0' && c <= '9')
			result = result * 10 + c - '0';
		else
			return(0);

	return(result * sign);
}

/*	itoa:	integer to ascii string.......... This is too
		inconsistant to use the system's	*/

char *itoa(i)

int i;	/* integer to translate to a string */

{
	register int digit;		/* current digit being used */
	register char *sp;		/* pointer into result */
	register int sign;		/* sign of resulting number */
	static char result[INTWIDTH+1];	/* resulting string */

	/* record the sign...*/
	sign = 1;
	if (i < 0) {
		sign = -1;
		i = -i;
	}

	/* and build the string (backwards!) */
	sp = result + INTWIDTH;
	*sp = 0;
	do {
		digit = i % 10;
		*(--sp) = '0' + digit;	/* and install the new digit */
		i = i / 10;
	} while (i);

	/* and fix the sign */
	if (sign == -1) {
		*(--sp) = '-';	/* and install the minus sign */
	}

	return(sp);
}

int gettyp(token)	/* find the type of a passed token */

char *token;	/* token to analyze */

{
	register char c;	/* first char in token */

	/* grab the first char (this is all we need) */
	c = *token;

	/* no blanks!!! */
	if (c == 0)
		return(TKNUL);

	/* a numeric literal? */
	if (c >= '0' && c <= '9')
		return(TKLIT);

	switch (c) {
		case '"':	return(TKSTR);

		case '!':	return(TKDIR);
		case '@':	return(TKARG);
		case '#':	return(TKBUF);
		case '$':	return(TKENV);
		case '%':	return(TKVAR);
		case '&':	return(TKFUN);
		case '*':	return(TKLBL);

		default:	return(TKCMD);
	}
}

char *getval(token)	/* find the value of a token */

char *token;		/* token to evaluate */

{
	register int status;	/* error return */
	register BUFFER *bp;	/* temp buffer pointer */
	register int blen;	/* length of buffer argument */
	register int distmp;	/* temporary discmd flag */
	char pad[20];		/* pad 20 bytes on stack for safety */
	char buf[NSTRING];	/* string buffer for some returns */

	switch (gettyp(token)) {
		case TKNUL:	return("");

		case TKARG:	/* interactive argument */
				strcpy(token, getval(&token[1]));
				distmp = discmd;	/* echo it always! */
				discmd = TRUE;
				status = getstring(token,
					   buf, NSTRING, ctoec('\n'));
				discmd = distmp;
				if (status == ABORT)
					return(errorm);
				return(buf);

		case TKBUF:	/* buffer contents fetch */

				/* grab the right buffer */
				strcpy(token, getval(&token[1]));
				bp = bfind(token, FALSE, 0);
				if (bp == NULL)
					return(errorm);
		
				/* if the buffer is displayed, get the window
				   vars instead of the buffer vars */
				if (bp->b_nwnd > 0) {
					curbp->b_dotp = curwp->w_dotp;
					curbp->b_doto = curwp->w_doto;
				}

				/* make sure we are not at the end */
				if (bp->b_linep == bp->b_dotp)
					return(errorm);
		
				/* grab the line as an argument */
				blen = bp->b_dotp->l_used - bp->b_doto;
				if (blen > NSTRING)
					blen = NSTRING;
				strncpy(buf, bp->b_dotp->l_text + bp->b_doto,
					blen);
				buf[blen] = 0;
		
				/* and step the buffer's line ptr ahead a line */
				bp->b_dotp = bp->b_dotp->l_fp;
				bp->b_doto = 0;

				/* if displayed buffer, reset window ptr vars*/
				if (bp->b_nwnd > 0) {
					curwp->w_dotp = curbp->b_dotp;
					curwp->w_doto = 0;
					curwp->w_flag |= WFMOVE;
				}

				/* and return the spoils */
				return(buf);		

		case TKVAR:	return(gtusr(token+1));
		case TKENV:	return(gtenv(token+1));
		case TKFUN:	return(gtfun(token+1));
		case TKDIR:	return(errorm);
		case TKLBL:	return(itoa(gtlbl(token)));
		case TKLIT:	return(token);
		case TKSTR:	return(token+1);
		case TKCMD:	return(token);
	}
}

gtlbl(token)	/* find the line number of the given label */

char *token;	/* label name to find */

{
	return(1);
}

int stol(val)	/* convert a string to a numeric logical */

char *val;	/* value to check for stol */

{
	/* check for logical values */
	if (val[0] == 'F')
		return(FALSE);
	if (val[0] == 'T')
		return(TRUE);

	/* check for numeric truth (!= 0) */
	return((atoi(val) != 0));
}

char *ltos(val)		/* numeric logical to string logical */

int val;	/* value to translate */

{
	if (val)
		return(truem);
	else
		return(falsem);
}

char *mkupper(str)	/* make a string upper case */

char *str;		/* string to upper case */

{
	char *sp;

	sp = str;
	while (*sp) {
		if ('a' <= *sp && *sp <= 'z')
			*sp += 'A' - 'a';
		++sp;
	}
	return(str);
}

char *mklower(str)	/* make a string lower case */

char *str;		/* string to lower case */

{
	char *sp;

	sp = str;
	while (*sp) {
		if ('A' <= *sp && *sp <= 'Z')
			*sp += 'a' - 'A';
		++sp;
	}
	return(str);
}

int abs(x)	/* take the absolute value of an integer */

int x;

{
	return(x < 0 ? -x : x);
}

int ernd()	/* returns a random integer */

{
	seed = abs(seed * 1721 + 10007);
	return(seed);
}

int sindex(source, pattern)	/* find pattern within source */

char *source;	/* source string to search */
char *pattern;	/* string to look for */

{
	char *sp;	/* ptr to current position to scan */
	char *csp;	/* ptr to source string during comparison */
	char *cp;	/* ptr to place to check for equality */

	/* scanning through the source string */
	sp = source;
	while (*sp) {
		/* scan through the pattern */
		cp = pattern;
		csp = sp;
		while (*cp) {
			if (!eq(*cp, *csp))
				break;
			++cp;
			++csp;
		}

		/* was it a match? */
		if (*cp == 0)
			return((int)(sp - source) + 1);
		++sp;
	}

	/* no match at all.. */
	return(0);
}
