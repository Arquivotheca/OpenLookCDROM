/*

    A Moon for the Sun

    Release 2.0

    Designed and implemented by John Walker in December 1987,
    revised and updated in February of 1988.

    Make with:

    cc -O moontool.c -o moontool -lm -lsuntool -lsunwindow -lpixrect

    This program is a SunView tool which displays, as the icon for a closed
    window,  the  current phase of the Moon.  A subtitle in the icon gives
    the age of the Moon in days  and  hours.   If  called  with  the  "-t"
    switch,  it  rapidly  increments  forward  through time to display the
    cycle of phases.

    If you open the window, additional information is displayed  regarding
    the  Moon.	 The  information  is  generally  accurate  to	within ten
    minutes.

    The algorithms used in this program to calculate the positions Sun and
    Moon as seen from the Earth are given in the book "Practical Astronomy
    With  Your  Calculator"  by  Peter  Duffett-Smith,   Second   Edition,
    Cambridge University Press, 1981.  Ignore the word "Calculator" in the
    title;  this  is  an  essential  reference  if  you're  interested  in
    developing	software  which  calculates  planetary	positions, orbits,
    eclipses, and  the  like.   If  you're  interested  in  pursuing  such
    programming, you should also obtain:

    "Astronomical  Formulae for Calculators" by Jean Meeus, Third Edition,
    Willmann-Bell, 1985.  A must-have.

    "Planetary  Programs  and  Tables  from  -4000  to  +2800"  by  Pierre
    Bretagnon  and Jean-Louis Simon, Willmann-Bell, 1986.  If you want the
    utmost  (outside  of  JPL)  accuracy  for  the  planets,  it's   here.

    "Celestial BASIC" by Eric Burgess, Revised Edition, Sybex, 1985.  Very
    cookbook oriented, and many of the algorithms are hard to dig  out	of
    the turgid BASIC code, but you'll probably want it anyway.

    Many of these references can be obtained from Willmann-Bell, P.O.  Box
    35025,  Richmond,  VA 23235, USA.  Phone: (804) 320-7016.  In addition
    to their own publications, they stock most of the standard	references
    for mathematical and positional astronomy.

    This program was written by:

       John Walker
       Autodesk, Inc.
       2320 Marinship Way
       Sausalito, CA  94965
       (415) 332-2344 Ext. 829

       Usenet: {sun!well}!acad!kelvin

    This  program is in the public domain: "Do what thou wilt shall be the
    whole of the law".  I'd appreciate  receiving  any  bug  fixes  and/or
    enhancements,  which  I'll  incorporate  in  future  versions  of  the
    program.  Please leave the original attribution information intact	so
    that credit and blame may be properly apportioned.

*/

#include <stdio.h>
#include <sys/types.h>
#include <math.h>
#include <xview/xview.h>
#include <xview/canvas.h>
#include <xview/icon.h>
#include <xview/font.h>

/*  Astronomical constants  */

#define epoch	    2444238.5	   /* 1980 January 0.0 */

/*  Constants defining the Sun's apparent orbit  */

#define elonge	    278.833540	   /* Ecliptic longitude of the Sun
				      at epoch 1980.0 */
#define elongp	    282.596403	   /* Ecliptic longitude of the Sun at
				      perigee */
#define eccent      0.016718       /* Eccentricity of Earth's orbit */
#define sunsmax     1.495985e8     /* Semi-major axis of Earth's orbit, km */
#define sunangsiz   0.533128       /* Sun's angular size, degrees, at
				      semi-major axis distance */

/*  Elements of the Moon's orbit, epoch 1980.0  */

#define mmlong      64.975464      /* Moon's mean lonigitude at the epoch */
#define mmlongp     349.383063	   /* Mean longitude of the perigee at the
				      epoch */
#define mlnode	    151.950429	   /* Mean longitude of the node at the
				      epoch */
#define minc        5.145396       /* Inclination of the Moon's orbit */
#define mecc        0.054900       /* Eccentricity of the Moon's orbit */
#define mangsiz     0.5181         /* Moon's angular size at distance a
				      from Earth */
#define msmax       384401.0       /* Semi-major axis of Moon's orbit in km */
#define mparallax   0.9507	   /* Parallax at distance a from Earth */
#define synmonth    29.53058868    /* Synodic month (new Moon to new Moon) */
#define lunatbase   2423436.0      /* Base date for E. W. Brown's numbered
				      series of lunations (1923 January 16) */

/*  Properties of the Earth  */

#define earthrad    6378.16	   /* Radius of Earth in kilometres */

/* Icon definition.  This is just a black field with rounded corners
   that blend into the root desktop pattern. The image of the moon and
   the text are added by the program later. */

static unsigned short moon_img[64][4] = {
/* Format_version=1, Width=64, Height=64, Depth=1, Valid_bits_per_item=16 */
	0x8FFF,0xFFFF,0xFFFF,0xFFE8,0x9FFF,0xFFFF,0xFFFF,0xFFF8,
	0x3FFF,0xFFFF,0xFFFF,0xFFFE,0x7FFF,0xFFFF,0xFFFF,0xFFFE,
	0xFFFF,0xFFFF,0xFFFF,0xFFFE,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0x7FFF,0xFFFF,0xFFFF,0xFFFE,
	0xFFFF,0xFFFF,0xFFFF,0xFFFE,0xBFFF,0xFFFF,0xFFFF,0xFFFC,
	0x3FFF,0xFFFF,0xFFFF,0xFFFA,0x27FF,0xFFFF,0xFFFF,0xFFE2
};

#undef	CAT	/* more headerfile silliness, for mpr_static */
#define       CAT(a,b)        a ## b
mpr_static(moon_icon_image, 64, 64, 1, moon_img);
Icon moon_icon;
mpr_static(icon_mpr, 64, 64, 1, moon_img);

static Frame frame;
static Canvas canvas;
static Font pfont;
static Pixwin *cpw;
static int charhgt, charwid;

#define PI 3.14159265358979323846  /* Assume not near black hole nor in
				      Tennessee */

/*  Handy mathematical functions  */

#define sgn(x) (((x) < 0) ? -1 : ((x) > 0 ? 1 : 0))	  /* Extract sign */
#define abs(x) ((x) < 0 ? (-(x)) : (x)) 		  /* Absolute val */
#define fixangle(a) ((a) - 360.0 * (floor((a) / 360.0)))  /* Fix angle	  */
#define torad(d) ((d) * (PI / 180.0))			  /* Deg->Rad	  */
#define todeg(d) ((d) * (180.0 / PI))			  /* Rad->Deg	  */
#define dsin(x) (sin(torad((x))))			  /* Sin from deg */
#define dcos(x) (cos(torad((x))))			  /* Cos from deg */

static int testmode = FALSE;	   /* Rapid warp through time for debugging */

/*  Forward functions  */

static double jtime(), phase();
static void phasehunt();
static Notify_value ringgg();
static void drawmoon(), jyear(), jhms();

/*  Main program  */

main(argc, argv)
int argc;
char *argv[];
{
	int i;
	struct itimerval kickme;

	xv_init(XV_INIT_ARGC_PTR_ARGV, &argc, argv, NULL);


	for (i = 1; i < argc; i++) {
           if (argv[i] && *argv[i] == '-' && argv[i][1] == 't')
	      testmode = TRUE;
	}

	frame = xv_create(NULL, FRAME,
		FRAME_LABEL,
        "A Moon for the Sun          by John Walker, Autodesk, Inc.   v2.0",
		FRAME_CLOSED, 1,
		0);
	moon_icon = xv_create(frame, ICON, ICON_IMAGE, &moon_icon_image, 0);
	xv_set(frame,
		FRAME_ICON, moon_icon,
		NULL);

        pfont = xv_create(frame, FONT, NULL);	/* use default font */

	canvas = xv_create(frame, CANVAS, 
		/* only a hint, must be prepared to repaint, read Sect 3.4 */
		CANVAS_RETAINED, TRUE,
		NULL);
	cpw = canvas_pixwin(canvas);
	charwid = xv_get(pfont, FONT_DEFAULT_CHAR_WIDTH);
	charhgt = xv_get(pfont, FONT_DEFAULT_CHAR_HEIGHT);
	xv_set(frame,
		XV_WIDTH, charwid * 70,
		XV_HEIGHT, charhgt * 19,
		NULL);
	ringgg((Notify_client) NULL, 0);
	kickme.it_interval.tv_usec = kickme.it_value.tv_usec =
	   testmode ? 125000 : 0;
	kickme.it_interval.tv_sec = kickme.it_value.tv_sec = 
	   testmode ? 0 : 1;
	notify_set_itimer_func(frame, ringgg, ITIMER_REAL, &kickme, NULL);
	xv_main_loop(frame);
	return 0;
}

/*  DRAWMOON  --  Construct icon for moon, given phase of moon.  */

static void drawmoon(ph)
double ph;
{
	int i, j, lx, rx;
	int lb[4];
	double cp, xscale;

	xscale = cos(2 * PI * ph);
	for (i = 0; i < 24; i++) {
	   lb[0] = lb[1] = lb[2] = lb[3] = 0xFFFF;
	   cp = 24.0 * cos(asin(i / 24.0));
	   if (ph < 0.5) {
	      rx = 32 + cp;
	      lx = 32 + xscale * cp;
	   } else {
	      lx = 33 - cp;
	      rx = 33 - xscale * cp;
	   }
	   for (j = lx; j <= rx; j++) {
	      lb[j >> 4] &= (0x8000 >> (j & 0xF)) ^ 0xFFFF;
	   }
	   for (j = 0; j < 4; j++)
	      moon_img[28 + i][j] = moon_img[28 - i][j] = lb[j];
	}
}

/*  RINGGG  --	Update status on interval timer ticks and redraw
		window if needed.  */
static Notify_value ringgg(client, itimer_type)
Notify_client client;
int itimer_type;
{
	int lunation, wclosed;
	long t;
	double jd, p, aom, cphase, cdist, cangdia, csund, csuang, lptime;
	double phasar[5];
	static double nptime = 0.0; /* Next new moon time */
	static int updyet = 0;	    /* Update interval when window closed */
	static int firstime = TRUE; /* Calculate text page first time */
	struct pr_prpos tloc;
	char amsg[12], tbuf[80];
	static double faketime = 0.0;
	static short moonilast[64][4] = {0};
	int yy, mm, dd, hh, mmm, ss;
	struct tm *gm;
        static char *moname[] = {"January", "February", "March",
           "April", "May", "June", "July", "August", "September",
           "October", "November", "December"};

#define CUPDINT  120		   /* Update the icon every CUPDINT seconds
				      when the window is iconic */

	/* If the window is closed, only update the icon every
	   two minutes */

#define BLIP(X) {putchar(X); fflush(stdout); }
	wclosed = (int) xv_get(frame, FRAME_CLOSED);
	if (wclosed && (--updyet > 0) && !testmode)
	   return;

	updyet = CUPDINT;
	(void) time(&t);
	jd = jtime((gm = gmtime(&t)));
	if (testmode) {
	   if (faketime == 0.0)
	      faketime = jd + 1;
	   else
	      faketime += 1.0 / 24;
	   jd = faketime;
	}
	p = phase(jd, &cphase, &aom, &cdist, &cangdia, &csund, &csuang);
	drawmoon(p);
        sprintf(amsg, " %dd %dh  ",
	   (int) aom, ((int) (24 * (aom - floor(aom)))));
	tloc.pr = (Pixrect *) xv_get(moon_icon, ICON_IMAGE);
	if (tloc.pr == NULL) {
		printf("No ICON_IMAGE in moon_icon\n");
		return;
	}
	tloc.pos.x = 2;
	tloc.pos.y = 62;
	
#if	0
	/* This does NOT work in XView; need to convert to XLib. Sigh. */
	xv_pf_text(tloc, PIX_NOT(PIX_SRC), pfont, amsg);
#endif

	/* Only update icon if it changed (this eliminates gratuitous
	   flashing of the icon on-screen). */

	if (bcmp(moonilast, moon_img, sizeof moon_img) != 0) {
	   bcopy(moon_img, moonilast, sizeof moon_img);
	   xv_set(frame, FRAME_ICON, &moon_icon, 0);
	}

        /* If we're iconic, there's nothing more to do. */

	if (wclosed && !firstime)
	   return;

	/* Update textual information for open window */

	/* But we can't do this, as XView doesn't support pw_text... */
	return;

#define prt(x) pw_text(cpw, charwid, charhgt * (x), PIX_SRC, NULL, tbuf)
#define prtxy(x,y) pw_text(cpw,charwid*(y+1),charhgt*(x),PIX_SRC,NULL,tbuf)

	firstime = FALSE;
        sprintf(tbuf, "Julian date:     %.5f", jd + 0.5);
BLIP('>');
	prt(1);
BLIP('<');
	if (testmode) {
	   jyear(jd, &yy, &mm, &dd);
	   jhms(jd, &hh, &mmm, &ss);
	   sprintf(tbuf,
              "Universal time:  %02d:%02d:%02d %d %s %d              ",
	      hh, mmm, ss, dd, moname[mm - 1], yy);
	} else {
	   sprintf(tbuf,
              "Universal time:  %02d:%02d:%02d %d %s %d              ",
	      gm->tm_hour, gm->tm_min, gm->tm_sec,
	      gm->tm_mday, moname[gm->tm_mon], gm->tm_year + 1900);
	}
	prt(2);
	gm = localtime(&t);
        sprintf(tbuf, "Local time:      %02d:%02d:%02d %d %s %d              ",
	   gm->tm_hour, gm->tm_min, gm->tm_sec,
	   gm->tm_mday, moname[gm->tm_mon], gm->tm_year + 1900);
	if (!testmode) {	   /* Ignore local time in test mode */
	   prt(3);
	}
        sprintf(tbuf, "Moon phase:      %d%%   0%% = New, 100%% = Full  ",
	   (int) (cphase * 100));
	prt(5);

	/* Information about the Moon */

#define EPL(x) (x), (x) == 1 ? "" : "s"
	sprintf(tbuf,
           "Age of moon:     %d day%s, %d hour%s, %d minute%s.       ",
	   EPL((int) aom), EPL(((int) (24 * (aom - floor(aom))))),
	   EPL(((int) (1440 * (aom - floor(aom)))) % 60));
	prt(6);
	sprintf(tbuf,
           "Moon's distance: %ld kilometres, %.1f Earth radii.  ",
	   (long) cdist, cdist / earthrad);
	prt(7);
	sprintf(tbuf,
           "Moon subtends:   %.4f degrees.       ", cangdia);
	prt(8);
	/* Draw the moon icon in the text window */
	pw_rop(cpw, 60 * charwid, 4 * charhgt, 64, 64, PIX_SRC,
	   &icon_mpr, 0, 0);

	/* Edit information about the Sun */

	sprintf(tbuf,
           "Sun's distance:  %.0f kilometres, %.3f astronomical units.       ",
	    csund, csund / sunsmax);
	prt(10);
	sprintf(tbuf,
           "Sun subtends:    %.4f degrees.       ", csuang);
	prt(11);

	/* Calculate times of phases of this lunation.	This is sufficiently
	   time-consuming that we only do it once a month. */

	if (jd > nptime) {
#define APOS(x) (x + 13)
	   phasehunt(jd, phasar);
	   lptime = phasar[0];
	   lunation = floor(((lptime + 7) - lunatbase) / synmonth) + 1;
	   jyear(lptime, &yy, &mm, &dd);
	   jhms(lptime, &hh, &mmm, &ss);
	   sprintf(tbuf,
              "Last new moon:   %02d:%02d UTC %d %s %d            ",
	      hh, mmm, dd, moname[mm - 1], yy);
	   prt(APOS(0));
           sprintf(tbuf, "Lunation %d    ", lunation);
	   prtxy(APOS(0), 47);			      

	   lptime = phasar[1];
	   jyear(lptime, &yy, &mm, &dd);
	   jhms(lptime, &hh, &mmm, &ss);
	   sprintf(tbuf,
              "First quarter:   %02d:%02d UTC %d %s %d              ",
	      hh, mmm, dd, moname[mm - 1], yy);
	   prt(APOS(1));

	   lptime = phasar[2];
	   jyear(lptime, &yy, &mm, &dd);
	   jhms(lptime, &hh, &mmm, &ss);
	   sprintf(tbuf,
              "Full moon:       %02d:%02d UTC %d %s %d              ",
	      hh, mmm, dd, moname[mm - 1], yy);
	   prt(APOS(2));

	   lptime = phasar[3];
	   jyear(lptime, &yy, &mm, &dd);
	   jhms(lptime, &hh, &mmm, &ss);
	   sprintf(tbuf,
              "Last quarter:    %02d:%02d UTC %d %s %d              ",
	      hh, mmm, dd, moname[mm - 1], yy);
	   prt(APOS(3));

	   nptime = phasar[4];
	   jyear(nptime, &yy, &mm, &dd);
	   jhms(nptime, &hh, &mmm, &ss);
	   sprintf(tbuf,
              "Next new moon:   %02d:%02d UTC %d %s %d              ",
	      hh, mmm, dd, moname[mm - 1], yy);
	   prt(APOS(4));
           sprintf(tbuf, "Lunation %d    ", lunation + 1);
	   prtxy(APOS(4), 47);			      
	}
#undef APOS
}

/*  JDATE  --  Convert internal GMT date and time to Julian day
	       and fraction.  */

static long jdate(t)
struct tm *t;
{
	long c, m, y;

	y = t->tm_year + 1900;
	m = t->tm_mon + 1;
	if (m > 2)
	   m = m - 3;
	else {
	   m = m + 9;
	   y--;
	}
	c = y / 100L;		   /* Compute century */
	y -= 100L * c;
	return t->tm_mday + (c * 146097L) / 4 + (y * 1461L) / 4 +
	    (m * 153L + 2) / 5 + 1721119L;
}

/* JTIME --    Convert internal GMT date and time to astronomical Julian
	       time (i.e. Julian date plus day fraction, expressed as
	       a double).  */

static double jtime(t)
struct tm *t;
{
	return (jdate(t) - 0.5) + 
	   (t->tm_sec + 60 * (t->tm_min + 60 * t->tm_hour)) / 86400.0;
}

/*  JYEAR  --  Convert Julian date to year, month, day, which are
	       returned via integer pointers to integers.  */

static void jyear(td, yy, mm, dd)
double td;
int *yy, *mm, *dd;
{
	double j, d, y, m;

	td += 0.5;		   /* Astronomical to civil */
	j = floor(td);
	j = j - 1721119.0;
	y = floor(((4 * j) - 1) / 146097.0);
	j = (j * 4.0) - (1.0 + (146097.0 * y));
	d = floor(j / 4.0);
	j = floor(((4.0 * d) + 3.0) / 1461.0);
	d = ((4.0 * d) + 3.0) - (1461.0 * j);
	d = floor((d + 4.0) / 4.0);
	m = floor(((5.0 * d) - 3) / 153.0);
	d = (5.0 * d) - (3.0 + (153.0 * m));
	d = floor((d + 5.0) / 5.0);
	y = (100.0 * y) + j;
	if (m < 10.0)
	   m = m + 3;
	else {
	   m = m - 9;
	   y = y + 1;
	}
	*yy = y;
	*mm = m;
	*dd = d;
}

/*  JHMS  --  Convert Julian time to hour, minutes, and seconds.  */

static void jhms(j, h, m, s)
double j;
int *h, *m, *s;
{
	long ij;

	j += 0.5;		   /* Astronomical to civil */
	ij = (j - floor(j)) * 86400.0;
	*h = ij / 3600L;
	*m = (ij / 60L) % 60L;
	*s = ij % 60L;
}

/*  MEANPHASE  --  Calculates mean phase of the Moon for a given
		   base date and desired phase:
		       0.0   New Moon
		       0.25  First quarter
		       0.5   Full moon
		       0.75  Last quarter
		    Beware!!!  This routine returns meaningless
                    results for any other phase arguments.  Don't
		    attempt to generalise it without understanding
		    that the motion of the moon is far more complicated
		    that this calculation reveals. */

static double meanphase(sdate, phase, usek)
double sdate, phase;
double *usek;
{
	int yy, mm, dd;
	double k, t, t2, t3, nt1;

	jyear(sdate, &yy, &mm, &dd);

	k = (yy + ((mm - 1) * (1.0 / 12.0)) - 1900) * 12.3685;

	/* Time in Julian centuries from 1900 January 0.5 */
	t = (sdate - 2415020.0) / 36525;
	t2 = t * t;		   /* Square for frequent use */
	t3 = t2 * t;		   /* Cube for frequent use */

	*usek = k = floor(k) + phase;
	nt1 = 2415020.75933 + synmonth * k
	      + 0.0001178 * t2
	      - 0.000000155 * t3
	      + 0.00033 * dsin(166.56 + 132.87 * t - 0.009173 * t2);

	return nt1;
}

/*  TRUEPHASE  --  Given a K value used to determine the
		   mean phase of the new moon, and a phase
		   selector (0.0, 0.25, 0.5, 0.75), obtain
		   the true, corrected phase time.  */

static double truephase(k, phase)
double k, phase;
{
	double t, t2, t3, pt, m, mprime, f;
	int apcor = FALSE;

	k += phase;		   /* Add phase to new moon time */
	t = k / 1236.85;	   /* Time in Julian centuries from
				      1900 January 0.5 */
	t2 = t * t;		   /* Square for frequent use */
	t3 = t2 * t;		   /* Cube for frequent use */
	pt = 2415020.75933	   /* Mean time of phase */
	     + synmonth * k
	     + 0.0001178 * t2
	     - 0.000000155 * t3
	     + 0.00033 * dsin(166.56 + 132.87 * t - 0.009173 * t2);

        m = 359.2242               /* Sun's mean anomaly */
	    + 29.10535608 * k
	    - 0.0000333 * t2
	    - 0.00000347 * t3;
        mprime = 306.0253          /* Moon's mean anomaly */
	    + 385.81691806 * k
	    + 0.0107306 * t2
	    + 0.00001236 * t3;
        f = 21.2964                /* Moon's argument of latitude */
	    + 390.67050646 * k
	    - 0.0016528 * t2
	    - 0.00000239 * t3;
	if ((phase < 0.01) || (abs(phase - 0.5) < 0.01)) {

	   /* Corrections for New and Full Moon */

	   pt +=     (0.1734 - 0.000393 * t) * dsin(m)
		    + 0.0021 * dsin(2 * m)
		    - 0.4068 * dsin(mprime)
		    + 0.0161 * dsin(2 * mprime)
		    - 0.0004 * dsin(3 * mprime)
		    + 0.0104 * dsin(2 * f)
		    - 0.0051 * dsin(m + mprime)
		    - 0.0074 * dsin(m - mprime)
		    + 0.0004 * dsin(2 * f + m)
		    - 0.0004 * dsin(2 * f - m)
		    - 0.0006 * dsin(2 * f + mprime)
		    + 0.0010 * dsin(2 * f - mprime)
		    + 0.0005 * dsin(m + 2 * mprime);
	   apcor = TRUE;
	} else if ((abs(phase - 0.25) < 0.01 || (abs(phase - 0.75) < 0.01))) {
	   pt +=     (0.1721 - 0.0004 * t) * dsin(m)
		    + 0.0021 * dsin(2 * m)
		    - 0.6280 * dsin(mprime)
		    + 0.0089 * dsin(2 * mprime)
		    - 0.0004 * dsin(3 * mprime)
		    + 0.0079 * dsin(2 * f)
		    - 0.0119 * dsin(m + mprime)
		    - 0.0047 * dsin(m - mprime)
		    + 0.0003 * dsin(2 * f + m)
		    - 0.0004 * dsin(2 * f - m)
		    - 0.0006 * dsin(2 * f + mprime)
		    + 0.0021 * dsin(2 * f - mprime)
		    + 0.0003 * dsin(m + 2 * mprime)
		    + 0.0004 * dsin(m - 2 * mprime)
		    - 0.0003 * dsin(2 * m + mprime);
	   if (phase < 0.5)
	      /* First quarter correction */
	      pt += 0.0028 - 0.0004 * dcos(m) + 0.0003 * dcos(mprime);
	   else
	      /* Last quarter correction */
	      pt += -0.0028 + 0.0004 * dcos(m) - 0.0003 * dcos(mprime);
	   apcor = TRUE;
	}
	if (!apcor) {
           fprintf(stderr, "TRUEPHASE called with invalid phase selector.\n");
	   abort();
	}
	return pt;
}

/*  PHASEHUNT  --  Find time of phases of the moon which surround
		   the current date.  Five phases are found, starting
		   and ending with the new moons which bound the
		   current lunation.  */

static void phasehunt(sdate, phases)
double sdate;
double phases[5];
{
	double adate, k1, k2, nt1, nt2;

	adate = sdate - 45;
	nt1 = meanphase(adate, 0.0, &k1);
	while (TRUE) {
	   adate += synmonth;
	   nt2 = meanphase(adate, 0.0, &k2);
	   if (nt1 <= sdate && nt2 > sdate)
	      break;
	   nt1 = nt2;
	   k1 = k2;
	}
	phases[0] = truephase(k1, 0.0);
	phases[1] = truephase(k1, 0.25);
	phases[2] = truephase(k1, 0.5);
	phases[3] = truephase(k1, 0.75);
	phases[4] = truephase(k2, 0.0);
}

/*  KEPLER  --	Solve the equation of Kepler.  */

static double kepler(m, ecc)
double m, ecc;
{
	double e, delta;
#define EPSILON 1E-6

	e = m = torad(m);
	do {
	   delta = e - ecc * sin(e) - m;
	   e -= delta / (1 - ecc * cos(e));
	} while (abs(delta) > EPSILON);
	return e;
}

/*  PHASE  --  Calculate phase of moon as a fraction:

	The argument is the time for which the phase is requested,
	expressed as a Julian date and fraction.  Returns the terminator
	phase angle as a percentage of a full circle (i.e., 0 to 1),
	and stores into pointer arguments the illuminated fraction of
        the Moon's disc, the Moon's age in days and fraction, the
	distance of the Moon from the centre of the Earth, and the
	angular diameter subtended by the Moon as seen by an observer
	at the centre of the Earth.

*/

static double phase(pdate, pphase, mage, dist, angdia, sudist, suangdia)
double pdate;
double *pphase; 		   /* Illuminated fraction */
double *mage;			   /* Age of moon in days */
double *dist;			   /* Distance in kilometres */
double *angdia; 		   /* Angular diameter in degrees */
double *sudist; 		   /* Distance to Sun */
double *suangdia;                  /* Sun's angular diameter */
{

	double Day, N, M, Ec, Lambdasun, ml, MM, MN, Ev, Ae, A3, MmP,
	       mEc, A4, lP, V, lPP, NP, y, x, Lambdamoon, BetaM,
	       MoonAge, MoonPhase,
	       MoonDist, MoonDFrac, MoonAng, MoonPar,
	       F, SunDist, SunAng;

        /* Calculation of the Sun's position */

	Day = pdate - epoch;	    /* Date within epoch */
	N = fixangle((360 / 365.2422) * Day); /* Mean anomaly of the Sun */
	M = fixangle(N + elonge - elongp);    /* Convert from perigee
				       co-ordinates to epoch 1980.0 */
	Ec = kepler(M, eccent);     /* Solve equation of Kepler */
	Ec = sqrt((1 + eccent) / (1 - eccent)) * tan(Ec / 2);
	Ec = 2 * todeg(atan(Ec));   /* True anomaly */
        Lambdasun = fixangle(Ec + elongp);  /* Sun's geocentric ecliptic
					       longitude */
	/* Orbital distance factor */
	F = ((1 + eccent * cos(torad(Ec))) / (1 - eccent * eccent));
	SunDist = sunsmax / F;	    /* Distance to Sun in km */
        SunAng = F * sunangsiz;     /* Sun's angular size in degrees */


        /* Calculation of the Moon's position */

        /* Moon's mean longitude */
	ml = fixangle(13.1763966 * Day + mmlong);

        /* Moon's mean anomaly */
	MM = fixangle(ml - 0.1114041 * Day - mmlongp);

        /* Moon's ascending node mean longitude */
	MN = fixangle(mlnode - 0.0529539 * Day);

	/* Evection */
	Ev = 1.2739 * sin(torad(2 * (ml - Lambdasun) - MM));

	/* Annual equation */
	Ae = 0.1858 * sin(torad(M));

	/* Correction term */
	A3 = 0.37 * sin(torad(M));

	/* Corrected anomaly */
	MmP = MM + Ev - Ae - A3;

	/* Correction for the equation of the centre */
	mEc = 6.2886 * sin(torad(MmP));

	/* Another correction term */
	A4 = 0.214 * sin(torad(2 * MmP));

	/* Corrected longitude */
	lP = ml + Ev + mEc - Ae + A4;

	/* Variation */
	V = 0.6583 * sin(torad(2 * (lP - Lambdasun)));

	/* True longitude */
	lPP = lP + V;

	/* Corrected longitude of the node */
	NP = MN - 0.16 * sin(torad(M));

	/* Y inclination coordinate */
	y = sin(torad(lPP - NP)) * cos(torad(minc));

	/* X inclination coordinate */
	x = cos(torad(lPP - NP));

	/* Ecliptic longitude */
	Lambdamoon = todeg(atan2(y, x));
	Lambdamoon += NP;

	/* Ecliptic latitude */
	BetaM = todeg(asin(sin(torad(lPP - NP)) * sin(torad(minc))));

	/* Calculation of the phase of the Moon */

	/* Age of the Moon in degrees */
	MoonAge = lPP - Lambdasun;

	/* Phase of the Moon */
	MoonPhase = (1 - cos(torad(MoonAge))) / 2;

	/* Calculate distance of moon from the centre of the Earth */

	MoonDist = (msmax * (1 - mecc * mecc)) /
	   (1 + mecc * cos(torad(MmP + mEc)));

        /* Calculate Moon's angular diameter */

	MoonDFrac = MoonDist / msmax;
	MoonAng = mangsiz / MoonDFrac;

        /* Calculate Moon's parallax */

	MoonPar = mparallax / MoonDFrac;

	*pphase = MoonPhase;
	*mage = synmonth * (fixangle(MoonAge) / 360.0);
	*dist = MoonDist;
	*angdia = MoonAng;
	*sudist = SunDist;
	*suangdia = SunAng;
	return fixangle(MoonAge) / 360.0;
}
