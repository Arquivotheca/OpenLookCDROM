/* $Id: computils.c,v 1.16 92/06/30 19:30:34 pturner Exp Locker: pturner $
 *
 * procedures for performing transformations from the command
 * line interpreter and the GUI.
 *
 */

#include <stdio.h>
#include <math.h>

#include "globals.h"

void do_runavg();
void do_fourier();
void do_fft();
void do_histo();

void do_running_command(type, setno, rlen)
    int type, setno, rlen;
{
    switch (type) {
    case RUNAVG:
	type = 0;
	break;
    case RUNMED:
	type = 1;
	break;
    case RUNMIN:
	type = 2;
	break;
    case RUNMAX:
	type = 3;
	break;
    case RUNSTD:
	type = 4;
	break;
    }
    do_runavg(setno, rlen, type);
}

void do_fourier_command(ftype, setno, ltype)
    int ftype, setno, ltype;
{
    int type;

    switch (ftype) {
    case DFT:
	do_fourier(0, setno, 0, ltype, 0, 0, 0);
	break;
    case INVDFT:
	do_fourier(0, setno, 0, ltype, 1, 0, 0);
	break;
    case FFT:
	do_fourier(1, setno, 0, ltype, 0, 0, 0);
	break;
    case INVFFT:
	do_fourier(1, setno, 0, ltype, 1, 0, 0);
	break;
    }
}

void do_histo_command(setno, minb, binw, nbins)
    int setno, nbins;
    double minb, binw;
{
    do_histo(setno, binw, minb, minb + nbins * binw, 0);
}

/*
 * evaluate a formula
 */
void do_compute(setno, loadto, graphto, fstr)
    int setno, loadto, graphto;
    char *fstr;
{
    int i, idraw = 0;

    if (graphto < 0) {
	graphto = cg;
    }
    if (strlen(fstr) == 0) {
	errwin("Define formula first");
	return;
    }
    if (setno == -1) {
	for (i = 0; i < g[cg].maxplot; i++) {
	    if (isactive(cg, i)) {
		if (formula(cg, i, fstr)) {
		    return;
		}
		idraw = 1;
	    }
	}
    } else if (isactive(cg, setno)) {
	/* both loadto and setno do double duty here */
	if (loadto) {
	    loadto = nextset(graphto);
	    if (loadto != -1) {
		do_copyset(cg, setno, graphto, loadto);
		setno = loadto;
	    } else {
		return;
	    }
	} else if (graphto != cg) {
	    loadto = setno;
	    if (isactive(graphto, loadto)) {
		killset(graphto, loadto);
	    }
	    do_copyset(cg, setno, graphto, loadto);
	    setno = loadto;
	}
	if (formula(graphto, setno, fstr)) {
	    return;
	}
	if (!isactive_graph(graphto)) {
	    set_graph_active(graphto);
	}
	idraw = 1;
    }
    if (idraw) {
	drawgraph();
    } else {
	errwin("Set(s) not active");
    }
}

/*
 * load a set
 */
void do_load(setno, toval, startstr, stepstr)
    int setno, toval;
    char *startstr, *stepstr;
{
    int i, ier = 0, idraw = 0;
    double x, y, a, b, c, d;
    extern double result;
    double start, step;

    if (strlen(startstr) == 0) {
	errwin("Start item undefined");
	return;
    }
    fixupstr(startstr);
    scanner(startstr, &x, &y, 1, &a, &b, &c, &d, 1, 0, 0, &ier);
    if (ier) {
	return;
    }
    start = result;

    if (strlen(stepstr) == 0) {
	errwin("Step item undefined");
	return;
    }
    fixupstr(stepstr);
    scanner(stepstr, &x, &y, 1, &a, &b, &c, &d, 1, 0, 0, &ier);
    if (ier) {
	return;
    }
    step = result;

    if (setno == -1) {
	for (i = 0; i < g[cg].maxplot; i++) {
	    if (isactive(cg, i)) {
		loadset(cg, i, toval, start, step);
		idraw = 1;
	    }
	}
    } else if (isactive(cg, setno)) {
	loadset(cg, setno, toval, start, step);
	idraw = 1;
    }
    if (idraw) {
	drawgraph();
    } else {
	errwin("Set(s) not active");
    }
}

/*
 * evaluate a formula loading the next set
 */
void do_compute2(fstrx, fstry, startstr, stopstr, npts, toval)
    char *fstrx, *fstry, *startstr, *stopstr;
    int toval;
{
    int i, setno, ier;
    double start, stop, step, x, y, a, b, c, d;
    extern double result;

    if (npts < 1) {
	errwin("Number of points < 1");
	return;
    }
    setno = nextset(cg);
    if (setno < 0) {
	return;
    }
    activateset(cg, setno);
    setlength(cg, setno, npts);
    if (strlen(fstrx) == 0) {
	errwin("Undefined expression for X");
	return;
    }
    if (strlen(fstry) == 0) {
	errwin("Undefined expression for Y");
	return;
    }
    if (strlen(startstr) == 0) {
	errwin("Start item undefined");
	return;
    }
    fixupstr(startstr);
    scanner(startstr, &x, &y, 1, &a, &b, &c, &d, 1, 0, 0, &ier);
    if (ier)
	return;
    start = result;

    if (strlen(stopstr) == 0) {
	errwin("Stop item undefined");
	return;
    }
    fixupstr(stopstr);
    scanner(stopstr, &x, &y, 1, &a, &b, &c, &d, 1, 0, 0, &ier);
    if (ier) {
	return;
    }
    stop = result;

    if (npts - 1 == 0) {
	errwin("Number of points = 0");
	return;
    }
    step = (stop - start) / (npts - 1);
    loadset(cg, setno, toval, start, step);
    strcpy(buf, "X=");
    strcat(buf, fstrx);
    formula(cg, setno, buf);
    strcpy(buf, "Y=");
    strcat(buf, fstry);
    formula(cg, setno, buf);
    drawgraph();
}

/*
 * forward, backward and centered differences
 */
static void forwarddiff(x, y, resx, resy, n)
    double x[], y[], resx[], resy[];
int n;

{
    int i, eflag = 0;
    double h;

    for (i = 1; i < n; i++) {
	resx[i - 1] = x[i - 1];
	h = x[i - 1] - x[i];
	if (h == 0.0) {
	    resy[i - 1] = MBIG;
	    eflag = 1;
	} else {
	    resy[i - 1] = (y[i - 1] - y[i]) / h;
	}
    }
    if (eflag) {
	errwin("Warning: infinite slope, check set status before proceeding");
    }
}

static void backwarddiff(x, y, resx, resy, n)
    double x[], y[], resx[], resy[];
int n;

{
    int i, eflag = 0;
    double h;

    for (i = 0; i < n - 1; i++) {
	resx[i] = x[i];
	h = x[i + 1] - x[i];
	if (h == 0.0) {
	    resy[i] = MBIG;
	    eflag = 1;
	} else {
	    resy[i] = (y[i + 1] - y[i]) / h;
	}
    }
    if (eflag) {
	errwin("Warning: infinite slope, check set status before proceeding");
    }
}

static void centereddiff(x, y, resx, resy, n)
    double x[], y[], resx[], resy[];
int n;

{
    int i, eflag = 0;
    double h1, h2;

    for (i = 1; i < n - 1; i++) {
	resx[i - 1] = x[i];
	h1 = x[i] - x[i - 1];
	h2 = x[i + 1] - x[i];
	if (h1 + h2 == 0.0) {
	    resy[i - 1] = MBIG;
	    eflag = 1;
	} else {
	    resy[i - 1] = (y[i + 1] - y[i - 1]) / (h1 + h2);
	}
    }
    if (eflag) {
	errwin("Warning: infinite slope, check set status before proceeding");
    }
}

/*
 * trapezoidal rule
 */
double trapint(x, y, resx, resy, n)
    double x[], y[], resx[], resy[];
int n;

{
    int i;
    double sum = 0.0;
    double h;

    for (i = 1; i < n; i++) {
	h = (x[i] - x[i - 1]);
	if (resx != NULL) {
	    resx[i - 1] = (x[i - 1] + x[i]) * 0.5;
	}
	sum = sum + h * (y[i - 1] + y[i]) * 0.5;
	if (resy != NULL) {
	    resy[i - 1] = sum;
	}
    }
    return sum;
}

/*
 * apply a digital filter
 */
void do_digfilter(set1, set2)
    int set1, set2;
{
    int digfiltset;

    if (!(isactive(cg, set1) && isactive(cg, set2))) {
	errwin("Set not active");
	return;
    }
    if ((getsetlength(cg, set1) < 3) || (getsetlength(cg, set2) < 3)) {
	errwin("Set length < 3");
	return;
    }
    digfiltset = nextset(cg);
    if (digfiltset != (-1)) {
	activateset(cg, digfiltset);
	setlength(cg, digfiltset, getsetlength(cg, set1) - getsetlength(cg, set2) + 1);
	sprintf(buf, "Digital filter from set %d applied to set %d", set2, set1);
	filterser(getsetlength(cg, set1),
		  getx(cg, set1),
		  gety(cg, set1),
		  getx(cg, digfiltset),
		  gety(cg, digfiltset),
		  gety(cg, set2),
		  getsetlength(cg, set2));
	setcomment(cg, digfiltset, buf);
	updatesetminmax(cg, digfiltset);
	update_set_status(cg, digfiltset);
	drawgraph();
    }
}

/*
 * linear convolution
 */
void do_linearc(set1, set2)
    int set1, set2;
{
    int linearcset, i, itmp;
    double *xtmp;

    if (!(isactive(cg, set1) && isactive(cg, set2))) {
	errwin("Set not active");
	return;
    }
    if ((getsetlength(cg, set1) < 3) || (getsetlength(cg, set2) < 3)) {
	errwin("Set length < 3");
	return;
    }
    linearcset = nextset(cg);
    if (linearcset != (-1)) {
	activateset(cg, linearcset);
	setlength(cg, linearcset, (itmp = getsetlength(cg, set1) + getsetlength(cg, set2) - 1));
	linearconv(gety(cg, set2), gety(cg, set1), gety(cg, linearcset), getsetlength(cg, set2), getsetlength(cg, set1));
	xtmp = getx(cg, linearcset);
	for (i = 0; i < itmp; i++) {
	    xtmp[i] = i;
	}
	sprintf(buf, "Linear convolution of set %d with set %d", set1, set2);
	setcomment(cg, linearcset, buf);
	updatesetminmax(cg, linearcset);
	update_set_status(cg, linearcset);
	drawgraph();
    }
}

/*
 * cross correlation
 */
void do_xcor(set1, set2, itype, lag)
    int set1, set2, itype, lag;
{
    int xcorset, i, ierr;
    double *xtmp;

    if (!(isactive(cg, set1) && isactive(cg, set2))) {
	errwin("Set not active");
	return;
    }
    if (lag == 0 || (getsetlength(cg, set1) - 1 < lag)) {
	errwin("Lag incorrectly specified");
	return;
    }
    if ((getsetlength(cg, set1) < 3) || (getsetlength(cg, set2) < 3)) {
	errwin("Set length < 3");
	return;
    }
    xcorset = nextset(cg);
    if (xcorset != (-1)) {
	activateset(cg, xcorset);
	setlength(cg, xcorset, lag);
	if (set1 != set2) {
	    sprintf(buf, "X-correlation of set %d and %d at lag %d", set1, set2, lag);
	} else {
	    sprintf(buf, "Autocorrelation of set %d at lag %d", set1, lag);
	}
	ierr = crosscorr(gety(cg, set1), gety(cg, set2), getsetlength(cg, set1), lag, itype, getx(cg, xcorset), gety(cg, xcorset));
	xtmp = getx(cg, xcorset);
	for (i = 0; i < lag; i++) {
	    xtmp[i] = i;
	}
	setcomment(cg, xcorset, buf);
	updatesetminmax(cg, xcorset);
	update_set_status(cg, xcorset);
	drawgraph();
    }
}

/*
 * splines
 */
void do_spline(set, startstr, stopstr, n)
    int set, n;
    char *startstr, *stopstr;
{
    int i, splineset, len, ier;
    double u, delx, *x, *y, *b, *c, *d, *xtmp, *ytmp;
    double xmin, xmax, ymin, ymax, start, stop, seval();
    extern double result;

    if (!isactive(cg, set)) {
	errwin("Set not active");
	return;
    }
    if ((len = getsetlength(cg, set)) < 3) {
	errwin("Improper set length");
	return;
    }
    getsetminmax(cg, set, &xmin, &xmax, &ymin, &ymax);
    strcpy(buf, startstr);
    if (strlen(buf) == 0) {
	errwin("Start item undefined");
	return;
    }
    fixupstr(buf);
    scanner(buf, &x, &y, 1, &xmin, &xmax, &ymin, &ymax, 1, 0, 0, &ier);
    if (ier)
	return;
    start = result;
    strcpy(buf, stopstr);
    if (strlen(buf) == 0) {
	errwin("Stop item undefined");
	return;
    }
    fixupstr(buf);
    scanner(buf, &x, &y, 1, &xmin, &xmax, &ymin, &ymax, 1, 0, 0, &ier);
    if (ier)
	return;
    stop = result;
    if (n <= 1) {
	errwin("Number of steps must be > 1");
	return;
    }
    delx = (stop - start) / (n - 1);
    splineset = nextset(cg);
    if (splineset != -1) {
	activateset(cg, splineset);
	setlength(cg, splineset, n);
	sprintf(buf, "Spline fit from set %d", set);
	x = getx(cg, set);
	y = gety(cg, set);
	b = (double *) calloc(len, sizeof(double));
	c = (double *) calloc(len, sizeof(double));
	d = (double *) calloc(len, sizeof(double));
	if (b == NULL || c == NULL || d == NULL) {
	    errwin("Not enough memory for splines");
	    cxfree(b);
	    cxfree(c);
	    cxfree(d);
	    killset(cg, splineset);
	    return;
	}
	spline(len, x, y, b, c, d);
	xtmp = getx(cg, splineset);
	ytmp = gety(cg, splineset);

	for (i = 0; i < n; i++) {
	    xtmp[i] = start + i * delx;
	    ytmp[i] = seval(len, xtmp[i], x, y, b, c, d);
	}
	setcomment(cg, splineset, buf);
	updatesetminmax(cg, splineset);
	update_set_status(cg, splineset);
	cxfree(b);
	cxfree(c);
	cxfree(d);
	drawgraph();
    }
}

/*
 * numerical integration
 */
double do_int(setno, itype)
    int setno, itype;
{
    int intset;
    double sum;

    if (!isactive(cg, setno)) {
	errwin("Set not active");
	return 0.0;
    }
    if (getsetlength(cg, setno) < 3) {
	errwin("Set length < 3");
	return 0.0;
    }
    if (itype == 0) {
	intset = nextset(cg);
	if (intset != (-1)) {
	    activateset(cg, intset);
	    setlength(cg, intset, getsetlength(cg, setno) - 1);
	    sprintf(buf, "Cumulative sum of set %d", setno);
	    sum = trapint(getx(cg, setno), gety(cg, setno), getx(cg, intset), gety(cg, intset), getsetlength(cg, setno));
	    setcomment(cg, intset, buf);
	    updatesetminmax(cg, intset);
	    update_set_status(cg, intset);
	    drawgraph();
	}
    } else {
	sum = trapint(getx(cg, setno), gety(cg, setno), NULL, NULL, getsetlength(cg, setno));
    }
    return sum;
}

/*
 * difference a set
 * itype means
 *  0 - forward
 *  1 - backward
 *  2 - centered difference
 */
void do_differ(setno, itype)
    int setno, itype;
{
    int diffset;

    if (!isactive(cg, setno)) {
	errwin("Set not active");
	return;
    }
    if (getsetlength(cg, setno) < 3) {
	errwin("Set length < 3");
	return;
    }
    diffset = nextset(cg);
    if (diffset != (-1)) {
	activateset(cg, diffset);
	switch (itype) {
	case 0:
	    sprintf(buf, "Forward difference of set %d", setno);
	    setlength(cg, diffset, getsetlength(cg, setno) - 1);
	    forwarddiff(getx(cg, setno), gety(cg, setno), getx(cg, diffset), gety(cg, diffset), getsetlength(cg, setno));
	    break;
	case 1:
	    sprintf(buf, "Backward difference of set %d", setno);
	    setlength(cg, diffset, getsetlength(cg, setno) - 1);
	    backwarddiff(getx(cg, setno), gety(cg, setno), getx(cg, diffset), gety(cg, diffset), getsetlength(cg, setno));
	    break;
	case 2:
	    sprintf(buf, "Centered difference of set %d", setno);
	    setlength(cg, diffset, getsetlength(cg, setno) - 2);
	    centereddiff(getx(cg, setno), gety(cg, setno), getx(cg, diffset), gety(cg, diffset), getsetlength(cg, setno));
	    break;
	}
	setcomment(cg, diffset, buf);
	updatesetminmax(cg, diffset);
	update_set_status(cg, diffset);
	drawgraph();
    }
}

/*
 * regression
 */
void do_regress(setno, ideg, iresid)
    int setno, ideg, iresid;
{
    int fitset, i;
    double *yd, *yf;

    if (!isactive(cg, setno)) {
	errwin("Set not active");
	return;
    }
    fitset = nextset(cg);
    if (fitset != (-1)) {
	activateset(cg, fitset);
	setlength(cg, fitset, getsetlength(cg, setno));
	copyx(cg, setno, fitset);
	fitcurve(getx(cg, setno), gety(cg, setno), getsetlength(cg, setno), ideg, gety(cg, fitset));
	if (iresid) {
	    yf = gety(cg, fitset);
	    yd = gety(cg, setno);
	    for (i = 0; i < getsetlength(cg, fitset); i++) {
		yf[i] = yd[i] - yf[i];
	    }
	}
	sprintf(buf, "%d deg fit of set %d", ideg, setno);
	setcomment(cg, fitset, buf);
	updatesetminmax(cg, fitset);
	update_set_status(cg, fitset);
    }
}

/*
 * running averages, medians, min, max, std. deviation
 */
void do_runavg(setno, runlen, runtype)
    int setno, runlen, runtype;
{
    int runset;

    if (!isactive(cg, setno)) {
	errwin("Set not active");
	return;
    }
    if (runlen >= getsetlength(cg, setno)) {
	errwin("Length of running average > set length");
	return;
    }
    if (runlen < 2) {
	errwin("Length of running average < 2");
	return;
    }
    runset = nextset(cg);
    if (runset != (-1)) {
	activateset(cg, runset);
	setlength(cg, runset, getsetlength(cg, setno) - runlen + 1);
	switch (runtype) {
	case 0:
	    runavg(getx(cg, setno), gety(cg, setno), getx(cg, runset), gety(cg, runset), getsetlength(cg, setno), runlen);
	    sprintf(buf, "%d-pt. avg. on set %d ", runlen, setno);
	    break;
	case 1:
	    runmedian(getx(cg, setno), gety(cg, setno), getx(cg, runset), gety(cg, runset), getsetlength(cg, setno), runlen);
	    sprintf(buf, "%d-pt. median on set %d ", runlen, setno);
	    break;
	case 2:
	    runminmax(getx(cg, setno), gety(cg, setno), getx(cg, runset), gety(cg, runset), getsetlength(cg, setno), runlen, 0);
	    sprintf(buf, "%d-pt. min on set %d ", runlen, setno);
	    break;
	case 3:
	    runminmax(getx(cg, setno), gety(cg, setno), getx(cg, runset), gety(cg, runset), getsetlength(cg, setno), runlen, 1);
	    sprintf(buf, "%d-pt. max on set %d ", runlen, setno);
	    break;
	case 4:
	    runstddev(getx(cg, setno), gety(cg, setno), getx(cg, runset), gety(cg, runset), getsetlength(cg, setno), runlen);
	    sprintf(buf, "%d-pt. std dev., set %d ", runlen, setno);
	    break;
	}
	setcomment(cg, runset, buf);
	updatesetminmax(cg, runset);
	update_set_status(cg, runset);
    }
}

/*
 * DFT by FFT or definition
 */
void do_fourier(fftflag, setno, load, loadx, invflag, type, wind)
    int fftflag, setno, load, loadx, invflag, type, wind;
{
    int i, ilen;
    double *x, *y, *xx, *yy, delt, T;
    int i2, specset;

    if (!isactive(cg, setno)) {
	errwin("Set not active");
	return;
    }
    ilen = getsetlength(cg, setno);
    if (ilen < 2) {
	errwin("Set length < 2");
	return;
    }
    if (fftflag) {
	if ((i2 = ilog2(ilen)) <= 0) {
	    errwin("Set length not a power of 2");
	    return;
	}
    }
    specset = nextset(cg);
    if (specset != -1) {
	activateset(cg, specset);
	setlength(cg, specset, ilen);
	xx = getx(cg, specset);
	yy = gety(cg, specset);
	x = getx(cg, setno);
	y = gety(cg, setno);
	copyx(cg, setno, specset);
	copyy(cg, setno, specset);
	if (wind != 0) {	/* apply data window if needed */
	    apply_window(xx, yy, ilen, type, wind);
	}
	if (type == 0) {	/* real data */
	    for (i = 0; i < ilen; i++) {
		xx[i] = yy[i];
		yy[i] = 0.0;
	    }
	}
	if (fftflag) {
	    fft(xx, yy, ilen, i2, !invflag);
	} else {
	    dft(xx, yy, ilen, invflag);
	}
	switch (load) {
	case 0:
	    delt = x[1] - x[0];
	    T = (ilen - 1) * delt;
	    setlength(cg, specset, ilen / 2);
	    xx = getx(cg, specset);
	    yy = gety(cg, specset);
	    for (i = 0; i < ilen / 2; i++) {
		yy[i] = hypot(xx[i], yy[i]);
		switch (loadx) {
		case 0:
		    xx[i] = i;
		    break;
		case 1:
		    /* xx[i] = 2.0 * M_PI * i / ilen; */
		    xx[i] = i / T;
		    break;
		case 2:
		    if (i == 0) {
			xx[i] = T + delt;	/* the mean */
		    } else {
			/* xx[i] = (double) ilen / (double) i; */
			xx[i] = T / i;
		    }
		    break;
		}
	    }
	    break;
	case 1:
	    delt = x[1] - x[0];
	    T = (x[ilen - 1] - x[0]);
	    setlength(cg, specset, ilen / 2);
	    xx = getx(cg, specset);
	    yy = gety(cg, specset);
	    for (i = 0; i < ilen / 2; i++) {
		yy[i] = -atan2(yy[i], xx[i]);
		switch (loadx) {
		case 0:
		    xx[i] = i;
		    break;
		case 1:
		    /* xx[i] = 2.0 * M_PI * i / ilen; */
		    xx[i] = i / T;
		    break;
		case 2:
		    if (i == 0) {
			xx[i] = T + delt;
		    } else {
			/* xx[i] = (double) ilen / (double) i; */
			xx[i] = T / i;
		    }
		    break;
		}
	    }
	    break;
	}
	if (fftflag) {
	    sprintf(buf, "FFT of set %d", setno);
	} else {
	    sprintf(buf, "DFT of set %d", setno);
	}
	setcomment(cg, specset, buf);
	updatesetminmax(cg, specset);
	update_set_status(cg, specset);
    }
}

/*
 * Apply a window to a set, result goes to a new set.
 */
void do_window(setno, type, wind)
    int setno, type, wind;
{
    int i, ilen;
    double *xx, *yy;
    int specset;

    if (!isactive(cg, setno)) {
	errwin("Set not active");
	return;
    }
    ilen = getsetlength(cg, setno);
    if (ilen < 2) {
	errwin("Set length < 2");
	return;
    }
    specset = nextset(cg);
    if (specset != -1) {
	activateset(cg, specset);
	setlength(cg, specset, ilen);
	xx = getx(cg, specset);
	yy = gety(cg, specset);
	copyx(cg, setno, specset);
	copyy(cg, setno, specset);
	if (wind != 0) {
	    apply_window(xx, yy, ilen, type, wind);
	}
	sprintf(buf, "Windowed set %d", setno);
	setcomment(cg, specset, buf);
	updatesetminmax(cg, specset);
	update_set_status(cg, specset);
    }
}

apply_window(xx, yy, ilen, type, wind)
    double *xx, *yy;
    int type, wind;
{
    int i;

    for (i = 0; i < ilen; i++) {
	switch (wind) {
	case 1:		/* triangular */
	    if (type != 0) {
		xx[i] *= 1.0 - fabs((i - 0.5 * (ilen - 1.0)) / (0.5 * (ilen - 1.0)));
	    }
	    yy[i] *= 1.0 - fabs((i - 0.5 * (ilen - 1.0)) / (0.5 * (ilen - 1.0)));

	    break;
	case 2:		/* Hanning */
	    if (type != 0) {
		xx[i] = xx[i] * (0.5 - 0.5 * cos(2.0 * M_PI * i / (ilen - 1.0)));
	    }
	    yy[i] = yy[i] * (0.5 - 0.5 * cos(2.0 * M_PI * i / (ilen - 1.0)));
	    break;
	case 3:		/* Welch (from Numerical Recipes) */
	    if (type != 0) {
		xx[i] *= 1.0 - pow((i - 0.5 * (ilen - 1.0)) / (0.5 * (ilen + 1.0)), 2.0);
	    }
	    yy[i] *= 1.0 - pow((i - 0.5 * (ilen - 1.0)) / (0.5 * (ilen + 1.0)), 2.0);
	    break;
	case 4:		/* Hamming */
	    if (type != 0) {
		xx[i] = xx[i] * (0.54 - 0.46 * cos(2.0 * M_PI * i / (ilen - 1.0)));
	    }
	    yy[i] = yy[i] * (0.54 - 0.46 * cos(2.0 * M_PI * i / (ilen - 1.0)));
	    break;
	case 5:		/* Blackman */
	    if (type != 0) {
		xx[i] = xx[i] * (0.42 - 0.5 * cos(2.0 * M_PI * i / (ilen - 1.0)) + 0.08 * cos(4.0 * M_PI * i / (ilen - 1.0)));
	    }
	    yy[i] = yy[i] * (0.42 - 0.5 * cos(2.0 * M_PI * i / (ilen - 1.0)) + 0.08 * cos(4.0 * M_PI * i / (ilen - 1.0)));
	    break;
	case 6:		/* Parzen (from Numerical Recipes) */
	    if (type != 0) {
		xx[i] *= 1.0 - fabs((i - 0.5 * (ilen - 1)) / (0.5 * (ilen + 1)));
	    }
	    yy[i] *= 1.0 - fabs((i - 0.5 * (ilen - 1)) / (0.5 * (ilen + 1)));
	    break;
	}
    }
}


/*
 * histograms
 */
void do_histo(hset, binw, xmin, xmax, hist_type)
    int hset, hist_type;
    double binw, xmin, xmax;
{
    void histogram();

    if (!isactive(cg, hset)) {
	errwin("Set not active");
	return;
    }
    if (getsetlength(cg, hset) <= 0) {
	errwin("Set length = 0");
	return;
    }
    if (binw <= 0.0) {
	errwin("Bin width <= 0");
	return;
    }
    histogram(hset, binw, xmin, xmax, hist_type);
    drawgraph();
}

void histogram(setno, bins, xmin, xmax, hist_type)
    int setno, hist_type;
    double bins, xmin, xmax;
{
    int binmax, n1, n, i, j, nbins, hset;
    double sum = 0.0, spread, xi, *x, *y;
    int *ind;

    if ((hset = nextset(cg)) == -1) {
	return;
    }
    n = getsetlength(cg, setno);
    spread = xmax - xmin;
    nbins = (int) (spread / bins);
    if (nbins <= 0) {
	errwin("No bins, no work to do");
	killset(cg, hset);
	return;
    }
    ind = (int *) calloc(nbins, sizeof(int));
    if (ind == NULL) {
	errwin("Not enough memory for histogram");
	killset(cg, hset);
	return;
    }
    activateset(cg, hset);
    setlength(cg, hset, nbins);
    j = 0;
    y = gety(cg, setno);
    for (i = 0; i < n; i++) {
	xi = y[i];
	j = (int) ((xi - xmin) / bins);
	ind[j] = ind[j] + 1;
    }
    n1 = 0;
    binmax = 0;
    x = getx(cg, hset);
    y = gety(cg, hset);
    for (i = 0; i < nbins; i++) {
	x[i] = i * bins + xmin;
	sum = sum * hist_type + ind[i];	/* hist_type = 0 => regular histo */
	y[i] = sum;
	n1 = n1 + ind[i];
	if (ind[i] > binmax) {
	    binmax = ind[i];
	}
    }
    i = HISTOSYM;
    setplotsym(cg, hset, i);
    i = 0;
    setplotlines(cg, hset, i);
    updatesymbols(cg, hset);
    updatesetminmax(cg, hset);
    sprintf(buf, "Histogram from set # %d", setno);
    setcomment(cg, hset, buf);
    update_set_status(cg, hset);
    cxfree(ind);
    drawgraph();
}

/*
 * sample a set, by start/step or logical expression
 */
void do_sample(setno, typeno, exprstr, startno, stepno)
    int setno, typeno, startno, stepno;
    char *exprstr;
{
    int len, npts, i, resset, ier;
    double *xres, *yres, *x, *y;
    double a, b, c, d;
    extern double result;

    if (!isactive(cg, setno)) {
	errwin("Set not active");
	return;
    }
    len = getsetlength(cg, setno);
    resset = nextset(cg);
    if (resset < 0) {
	return;
    }
    if (typeno == 0) {
	if (len <= 2) {
	    errwin("Set has <= 2 points");
	    return;
	}
	if (startno < 1) {
	    errwin("Start point < 1 (locations in sets are numbered starting from 1)");
	    return;
	}
	if (stepno < 1) {
	    errwin("Step < 1");
	    return;
	}
	x = getx(cg, setno);
	y = gety(cg, setno);
	for (i = startno - 1; i < len; i += stepno) {
	    add_point(cg, resset, x[i], y[i], 0.0, 0.0, XY);
	    npts++;
	}
	sprintf(buf, "Sample, %d, %d set #%d", startno, stepno, setno);
    } else {
	if (!strlen(exprstr)) {
	    errwin("Enter logical expression first");
	    return;
	}
	x = getx(cg, setno);
	y = gety(cg, setno);
	npts = 0;
	fixupstr(exprstr);
	for (i = 0; i < len; i++) {
	    scanner(exprstr, &x[i], &y[i], 1, &a, &b, &c, &d, 1, i, setno, &ier);
	    if (ier) {
		killset(cg, resset);
		return;
	    }
	    if ((int) result) {
		add_point(cg, resset, x[i], y[i], 0.0, 0.0, XY);
		npts++;
	    }
	}
	if (npts > 0) {
	    sprintf(buf, "Sample from %d, using '%s'", setno, exprstr);
	}
    }
    if (npts > 0) {
	updatesetminmax(cg, resset);
	setcomment(cg, resset, buf);
	update_set_status(cg, resset);
	drawgraph();
    }
}
