static char sccsid[] = "@(#)28	1.1  R2/DPS/examples/texteroids/texteroids.c, gos, gos320 6/27/91 19:51:17";
/*
 *   COMPONENT_NAME: XDPS
 *
 *   FUNCTIONS: ITransform
 *		CheckHit
 *		MaybePause
 *		MapHandler
 *		NewSize
 *		NewSize
 *		main
 *		PrepareExplodedArrays
 *		ExplodeString
 *		PrepareSubject
 *		DrawSubject
 *		SyncAndCheck
 *
 *   ORIGINS: 1
 *
 */

/* texteroids.c

Copyright 1990 Adobe Systems Incorporated.

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notices appear in all copies and that
both those copyright notices and this permission notice appear in
supporting documentation, and that the name of Adobe Systems
Incorporated not be used in advertising or publicity pertaining to
distribution of the software without specific, written prior
permission.  Adobe Systems Incorporated makes no representations about
the suitability of this software for any purpose.  It is provided "as
is" without express or implied warranty.

ADOBE SYSTEMS INCORPORATED DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS
SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS, IN NO EVENT SHALL ADOBE SYSTEMS INCORPORATED BE LIABLE FOR ANY
SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

Author: Adobe Systems Incorporated
*/

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <DPS/psops.h>
#include <DPS/dpsXclient.h>
#include <math.h>

int CheckForAHit(), ExplodeString();

#define PI 3.14159
#define COLORINCR (1.0/200.0)
#define EPSILON 0.001
#define ABS(x) ((x) >= 0 ? (x) : -(x))

typedef struct {
   char *text_font;
   int text_size;
   Boolean rv;
} OptionsRec, *Options;

OptionsRec options;

XtResource app_resources[] = {
     {"font", "Font", XtRString, sizeof(String),
	XtOffset(Options, text_font), XtRString, (caddr_t) "Times-Italic"},
     {"textSize", "TextSize", XtRInt, sizeof(int),
	XtOffset(Options, text_size), XtRImmediate, (caddr_t) 36},
     {"reverseVideo", "ReverseVideo", XtRBoolean, sizeof(Boolean),
	XtOffset(Options, rv), XtRImmediate, (caddr_t) False}
};

XrmOptionDescRec cmdOptions[] = {
     {"-size", "*textSize", XrmoptionSepArg, NULL}
};

XtAppContext app;
DPSContext ctx, tctx;
Boolean resized = FALSE;
Boolean go_to_sleep = FALSE;
Boolean mapped = TRUE;
float hitx = -1.0, hity = -1.0;

Widget w;
Pixmap pix;
GC window_gc, drawing_gc;
Dimension width, height;
int depth;
float uwidth, uheight, old_uheight;
String *subject;
float *twidth, *theight;
float *charwidth;
float *textdx, *textdy, *dx, *dy;
float *vx, *vy;
float *dirx, *diry;
float *centerx, *centery;
float *r, *theta;
float *red, *green, *blue;
typedef enum {Rup, Rdown, Gup, Gdown, Bup, Bdown} Change;
Change *changing;
Boolean *live;
int count, total;

float invctm[6];

void ITransform(x, y, ux, uy)
    int x, y;
    float *ux, *uy;
{
    *ux = invctm[0] * x + invctm[2] * y + invctm[4];
    *uy = invctm[1] * x + invctm[3] * y + invctm[5];
}

void CheckHit(w, event, params, num)
    Widget w;
    XEvent *event;
    String *params;
    Cardinal *num;
{
    if (event->type != ButtonPress) return;
    ITransform(event->xbutton.x, event->xbutton.y - height,
	       &hitx, &hity);
}

void MaybePause(w, event, params, num)
    Widget w;
    XEvent *event;
    String *params;
    Cardinal *num;
{
    switch (event->type) {
	case VisibilityNotify:
	    go_to_sleep =
		    (event->xvisibility.state == VisibilityFullyObscured);
	    break;
    }
}

void MapHandler(w, client_data, event, cont)
    Widget w;
    XtPointer client_data;
    XEvent *event;
    Boolean *cont;
{
    switch (event->type) {
	case UnmapNotify:
	    mapped = FALSE;
	    break;
	case MapNotify:
	    mapped = TRUE;
	    break;
    }
}

void NewSize(w, event, params, num)
    Widget w;
    XEvent *event;
    String *params;
    Cardinal *num;
{
    if (event->type != ConfigureNotify) return;
    if (event->xconfigure.width == width && event->xconfigure.height == height)
	    return;
    width = event->xconfigure.width;
    height = event->xconfigure.height;
    resized = TRUE;
}

String translations =
	"<BtnDown>	: CheckHit() \n\
         <Visible>	: MaybePause() \n\
	 <Configure>	: NewSize()";

XtActionsRec actions[] = {
     {"CheckHit", CheckHit},
     {"MaybePause", MaybePause},
     {"NewSize", NewSize}
};

void main(argc, argv)
    int argc;
    char **argv;
{
    Widget shell;
    Arg args[10];
    int i, angle, start_angle;
    int n;
    String text;
    XGCValues v;
    int hit;
    Pixel bg;
    XSetWindowAttributes att;
    float llx, lly, urx, ury;

    shell = XtAppInitialize(&app, "Texteroids",
			    cmdOptions, XtNumber(cmdOptions),
			    &argc, argv,
			    (String *) NULL, (ArgList) NULL, 0);

    XtAppAddActions(app, actions, XtNumber(actions));

    XtGetApplicationResources(shell, &options,
			      app_resources, XtNumber(app_resources),
			      (ArgList) NULL, 0);
    if (argc <= 1) text = "Adobe";
    else text = argv[1];
    text = XtNewString(text);	/* Make sure it's writable */
    n = strlen(text);

    /* Don't use a translation for this; we don't want to get in the way
       of what the shell might be doing */

    XtAddEventHandler(shell, StructureNotifyMask, FALSE, MapHandler, NULL);

    i = 0;
    XtSetArg(args[i], XtNwidth, 400);		i++;
    XtSetArg(args[i], XtNheight, 400);		i++;
    XtSetArg(args[i], XtNtranslations,
	     XtParseTranslationTable(translations));	i++;
    if (options.rv) {
	XtSetArg(args[i], XtNbackground,
		 WhitePixelOfScreen(XtScreen(shell)));	i++;
    } else {
	XtSetArg(args[i], XtNbackground,
		 BlackPixelOfScreen(XtScreen(shell)));	i++;
    }
	
    w = XtCreateManagedWidget("main", widgetClass, shell, args, i);
    XtRealizeWidget(shell);

    att.bit_gravity = StaticGravity;
    XChangeWindowAttributes(XtDisplay(w), XtWindow(w), CWBitGravity, &att);

    i = 0;
    XtSetArg(args[i], XtNwidth, &width);	i++;
    XtSetArg(args[i], XtNheight, &height);	i++;
    XtSetArg(args[i], XtNdepth, &depth);	i++;
    XtSetArg(args[i], XtNbackground, &bg);	i++;
    XtGetValues(w, args, i);

    /* window_gc is used for copying the off-screen pixmap to the window */
    v.function = GXcopy;
    v.background = bg;
    v.foreground = WhitePixelOfScreen(XtScreen(shell));
    window_gc = XCreateGC(XtDisplay(w), XtWindow(w),
			  GCFunction | GCForeground | GCBackground, &v);

    /* drawing_gc is used for drawing into the pixmap */
    drawing_gc = XtGetGC(w, 0, &v);

    pix = XCreatePixmap(XtDisplay(w), RootWindowOfScreen(XtScreen(w)),
			width, height, depth);

    ctx = XDPSCreateSimpleContext(XtDisplay(w), pix,
				  drawing_gc, 0, height,
				  DPSDefaultTextBackstop,
				  DPSDefaultErrorProc, (DPSSpace) NULL);
/*
    tctx = DPSCreateTextContext(DPSDefaultTextBackstop, DPSDefaultErrorProc);
    DPSChainContext(ctx, tctx);
*/
    DPSSetContext(ctx);
    GetInvCTM(invctm);
    ITransform(width, -height, &uwidth, &uheight); 

    subject = (String *) XtCalloc(n, sizeof(String));
    twidth = (float *) XtCalloc(n, sizeof(float));
    theight = (float *) XtCalloc(n, sizeof(float));
    charwidth = (float *) XtCalloc(n, sizeof(float));
    textdx = (float *) XtCalloc(n, sizeof(float));
    textdy = (float *) XtCalloc(n, sizeof(float));
    dx = (float *) XtCalloc(n, sizeof(float));
    dy = (float *) XtCalloc(n, sizeof(float));
    vx = (float *) XtCalloc(n, sizeof(float));
    vy = (float *) XtCalloc(n, sizeof(float));
    dirx = (float *) XtCalloc(n, sizeof(float));
    diry = (float *) XtCalloc(n, sizeof(float));
    centerx = (float *) XtCalloc(n, sizeof(float));
    centery = (float *) XtCalloc(n, sizeof(float));
    r = (float *) XtCalloc(n, sizeof(float));
    theta = (float *) XtCalloc(n, sizeof(float));
    red = (float *) XtCalloc(n, sizeof(float));
    blue = (float *) XtCalloc(n, sizeof(float));
    green = (float *) XtCalloc(n, sizeof(float));
    changing = (Change *) XtCalloc(n, sizeof(Change));
    live = (Boolean *) XtCalloc(n, sizeof(Boolean));

    CreateFont(options.text_size, options.text_font);
    vx[0] = 2.0;
    vy[0] = 1.0;
    dirx[0] = 1.0;
    diry[0] = 1.0;
    red[0] = 1.0;
    green[0] = 0.0;
    blue[0] = 0.0;
    changing[0] = Gup;

    GetSize(text, &llx, &lly, &urx, &ury, &charwidth[i]);
    llx--; lly--; urx++; ury++;

    PrepareSubject(0, text, uwidth/2, uheight/2, llx, lly, urx, ury);
    count = total = 1;

    PrepareExplodedArrays(text, n);

    Clear(uwidth, uheight, (int) !options.rv);

    while (1) {
	for (angle = 0; angle < 360; angle += 10) {
	    XSetClipMask(XtDisplay(w), drawing_gc, None);
	    DrawSubject(0, angle);
	    SyncAndCheck(angle);
	    Clear(uwidth, uheight, (int) !options.rv);
	    hit = CheckForAHit(angle);
	    UpdatePosition(0, angle);
	    if (hit != -1) goto EXPLODE;
	}
    }

EXPLODE: if (n == 1) exit(0);
    n = count = total = ExplodeString(text, angle + 10, n);

    Clear(uwidth, uheight, (int) !options.rv);
    start_angle = angle + 10;
    while(count > 0) {
	for (angle = start_angle; angle < 360; angle += 10) {
	    XSetClipMask(XtDisplay(w), drawing_gc, None);
	    for (i = 0; i < n; i++) DrawSubject(i, angle);
	    SyncAndCheck(angle);
	    Clear(uwidth, uheight, (int) !options.rv);
	    hit = CheckForAHit(angle);
	    if (hit != -1) {
		live[hit] = FALSE;
		count--;
	     }
	    for (i = 0; i < n; i++) UpdatePosition(i, angle);
	}
	start_angle = 0;
    }

}

float *llx, *lly, *urx, *ury, *altwidths;

PrepareExplodedArrays(str, n)
    char *str;
{
    char *altstr = (char *) XtNewString(str);
    int i, texti;

    llx = (float *) XtCalloc(n, sizeof(float));
    lly = (float *) XtCalloc(n, sizeof(float));
    urx = (float *) XtCalloc(n, sizeof(float));
    ury = (float *) XtCalloc(n, sizeof(float));
    altwidths = (float *) XtCalloc(n, sizeof(float));

    i = 0;
    for (texti = 0; texti < n; texti++) {
        if (str[texti] == ' ') continue;
	altstr[i] = str[texti];
	i++;
    }
    altstr[i] = '\0';

    GetAllSizes(altstr, i, llx, lly, urx, ury, altwidths);
    XtFree(altstr);
}

int ExplodeString(str, angle, n)
    String str;
    int angle, n;
{
    int i, texti, num;
    float total_width = 0;
    float startx, starty;
    float temp_r, temp_theta;
    float x, y;
    float cx = centerx[0], cy = centery[0];

    temp_r = r[0];
    temp_theta = PI + theta[0];
    startx = temp_r * cos(temp_theta);
    starty = temp_r * sin(temp_theta);
    
    i = 0;
    for (texti = 0; texti < n; texti++) {
        if (str[texti] == ' ') continue;

	subject[i] = (char *) XtNewString("a");
	subject[i][0] = str[texti];
	str[i] = str[texti];
	i++;
    }
    str[i] = '\0';
    num = i;

    XtFree(charwidth);
    charwidth = altwidths;

    for (i = 0; i < num; i++) {
        llx[i]--; lly[i]--; urx[i]++; ury[i]++;
	PrepareSubject(i, subject[i], 0.0, 0.0,
		       llx[i], lly[i], urx[i], ury[i]);
	x = startx + total_width + llx[i] + twidth[i]/2;
	y = starty + lly[i] + theight[i]/2;
	
	temp_r = sqrt(x*x + y*y);
	temp_theta = atan2(y, x) + (angle) * PI / 180;
	centerx[i] = temp_r * cos(temp_theta) + cx;
	centery[i] = temp_r * sin(temp_theta) + cy;

	vx[i] *= dirx[i];
	vy[i] *= diry[i];
	vx[i] += 2.0 * cos(i * 2*PI / n);
	vy[i] += 2.0 * sin(i * 2*PI / n);

	if (vx[i] < 0) {vx[i] = -vx[i]; dirx[i] = -1.0;}
 	else dirx[i] = 1.0;
	if (vy[i] < 0) {vy[i] = -vy[i]; diry[i] = -1.0;}
	else diry[i] = 1.0;
			
	total_width += charwidth[i];
    }
    XtFree(llx);
    XtFree(lly);
    XtFree(urx);
    XtFree(ury);
    return num;
}

PrepareSubject(i, str, initx, inity, llx, lly, urx, ury)
    int i;
    String str;
    float initx, inity;
    float llx, lly, urx, ury;
{
    subject[i] = str;
    centerx[i] = initx;
    centery[i] = inity;
    twidth[i] = urx - llx;
    theight[i] = ury - lly;
    textdx[i] = -(twidth[i]/2 + llx);
    textdy[i] = -(theight[i]/2 + lly);
    dx[i] = -twidth[i]/2;
    dy[i] = -theight[i]/2;
    r[i] = sqrt(twidth[i] * twidth[i]/4 + theight[i] * theight[i]/4);
    theta[i] = atan(theight[i] / twidth[i]);
    live[i] = TRUE;
    if (i == 0) return;
    vx[i] = vx[0];
    vy[i] = vy[0];
    dirx[i] = dirx[0];
    diry[i] = diry[0];
    red[i] = red[0];
    green[i] = green[0];
    blue[i] = blue[0];
    changing[i] = changing[0];
}

DrawSubject(i, angle)
    int i, angle;
{
    if (!live[i]) return;
    RenderString(centerx[i], centery[i], textdx[i], textdy[i], (float) angle,
		 subject[i], red[i], green[i], blue[i]);
}

Region empty = NULL;

SyncAndCheck(angle)
    int angle;
{
    int i;
    static Region r1 = NULL, r2 = NULL;
    static int which_region = 1;
    static Region reg = NULL;

    DPSWaitContext(ctx);
    XSync(XtDisplay(w), FALSE);

    if (empty == NULL) {
	empty = XCreateRegion();
	reg = XCreateRegion();
	r1 = XCreateRegion();
	r2 = XCreateRegion();
    }
    
    if (which_region == 1) CreateRegionForClipping(r1, angle);
    else CreateRegionForClipping(r2, angle);

    which_region = 3 - which_region;

    XUnionRegion(r1, r2, reg);

    XSetRegion(XtDisplay(w), window_gc, reg);
    XSetRegion(XtDisplay(w), drawing_gc, reg);

    XCopyArea(XtDisplay(w), pix, XtWindow(w), window_gc, 0, 0,
	      width, height, 0, 0);

    while (XtAppPending(app)) {
	XtAppProcessEvent(app, XtIMAll);
	while (go_to_sleep || !mapped) {
	    XtAppProcessEvent(app, XtIMAll);
	}
    }

    if (resized) {
        XFreePixmap(XtDisplay(w), pix);
	pix = XCreatePixmap(XtDisplay(w), RootWindowOfScreen(XtScreen(w)),
			    width, height, depth);

	SetXDrawable(pix, height);
	/* Make next Clear clear everything */
	XSetClipMask(XtDisplay(w), drawing_gc, None);
	old_uheight = uheight;
	GetInvCTM(invctm);
	ITransform(width, -height, &uwidth, &uheight);

	for (i = 0; i < total; i++) {
	    if (centerx[i] > uwidth + r[i]) centerx[i] = uwidth - r[i];
	    centery[i] += uheight - old_uheight;
	    if (centery[i] > uheight + r[i]) centery[i] = uheight - r[i];
	    if (centery[i] < -r[i]) centery[i] = r[i];
	}

	resized = FALSE;
    }
}

UpdatePosition(i, angle)
    int i, angle;
{
    float incr;

    if (!live[i]) return;

    centerx[i] += vx[i] * dirx[i];
    centery[i] += vy[i] * diry[i];
    {
	float c1x, c1y, c2x, c2y;
	c1x = r[i] * cos(angle * PI / 180 + theta[i]); 
	c1y = r[i] * sin(angle * PI / 180 + theta[i]);
	c2x = r[i] * cos(angle * PI / 180 - theta[i]);
	c2y = r[i] * sin(angle * PI / 180 - theta[i]);

	if (c1x + centerx[i] > uwidth) dirx[i] = -1.0;
	else if (c2x + centerx[i] > uwidth) dirx[i] = -1.0;
	else if (-c1x + centerx[i] > uwidth) dirx[i] = -1.0;
	else if (-c2x + centerx[i] > uwidth) dirx[i] = -1.0;
	else if (c1x + centerx[i] < 0) dirx[i] = 1.0;
	else if (c2x + centerx[i] < 0) dirx[i] = 1.0;
	else if (-c1x + centerx[i] < 0) dirx[i] = 1.0;
	else if (-c2x + centerx[i] < 0) dirx[i] = 1.0;

	if (c1y + centery[i] > uheight) diry[i] = -1.0;
	else if (c2y + centery[i] > uheight) diry[i] = -1.0;
	else if (-c1y + centery[i] > uheight) diry[i] = -1.0;
	else if (-c2y + centery[i] > uheight) diry[i] = -1.0;
	else if (c1y + centery[i] < 0) diry[i] = 1.0;
	else if (c2y + centery[i] < 0) diry[i] = 1.0;
	else if (-c1y + centery[i] < 0) diry[i] = 1.0;
	else if (-c2y + centery[i] < 0) diry[i] = 1.0;
    }
    incr = 4.0 * ((float) i) / (((float) total) + 1.0);
    incr += 1.0;
    incr *= COLORINCR;
    switch (changing[i]) {
	case Gup:
	    green[i] += incr;
	    if (green[i] >= 1.0 - EPSILON) {
		green[i] = 1.0;
		changing[i] = Rdown;
	    }
	    break;
	case Rup:
	    red[i] += incr;
	    if (red[i] >= 1.0 - EPSILON) {
	        red[i] = 1.0;
		changing[i] = Bdown;
	    }
	    break;
	case Bup:
	    blue[i] += incr;
	    if (blue[i] >= 1.0 - EPSILON) {
	        blue[i] = 1.0;
		changing[i] = Gdown;
	    }
	    break;

	case Gdown:
	    green[i] -= incr;
	    if (green[i] < EPSILON) {
	        green[i] = 0.0;
		changing[i] = Rup;
	    }
	    break;
	case Rdown:
	    red[i] -= incr;
	    if (red[i] < EPSILON) {
	        red[i] = 0.0;
		changing[i] = Bup;
	    }
	    break;
	case Bdown:
	    blue[i] -= incr;
	    if (blue[i] < EPSILON) {
	        blue[i] = 0.0;
		changing[i] = Gup;
	    }
	    break;

    }
    return;
}

int CheckForAHit(angle)
    int angle;
{
    int i;
    float transx, transy;
    float point_angle;
    float point_r;

    if (hitx == -1 && hity == -1) return -1;

    for (i = 0; i < total; i++) {
        if (!live[i]) continue;
	point_angle = atan2(hity - centery[i], hitx - centerx[i]);
	point_angle -= angle * PI / 180;
	point_r = sqrt((hity - centery[i]) * (hity - centery[i]) +
		       (hitx - centerx[i]) * (hitx - centerx[i]));
	transx = point_r * cos(point_angle);
	transy = point_r * sin(point_angle);
	if (transx >= -twidth[i]/2 && transx <= twidth[i]/2 &&
	        transy >= -theight[i]/2 && transy <= theight[i]/2) {
	    hitx = hity = -1;
	    return i;
	}
    }
    hitx = hity = -1;
    return -1;
}

CreateRegionForClipping(reg, angle)
    Region reg;
    int angle;
{
    int i;
    XRectangle rec;
    float c1x, c1y, c2x, c2y;
    float left, bottom, wid, hgt;

    XIntersectRegion(reg, empty, reg);

    for (i = 0; i < total; i++) {
	c1x = r[i] * cos(angle * PI / 180 + theta[i]); 
	c1y = r[i] * sin(angle * PI / 180 + theta[i]);
	c2x = r[i] * cos(angle * PI / 180 - theta[i]);
	c2y = r[i] * sin(angle * PI / 180 - theta[i]);

	left = centerx[i] - c1x;
	if (centerx[i] - c2x < left) left = centerx[i] - c2x;
	if (centerx[i] + c1x < left) left = centerx[i] + c1x;
	if (centerx[i] + c2x < left) left = centerx[i] + c2x;
	bottom = centery[i] - c1y;
	if (centery[i] - c2y < bottom) bottom = centery[i] - c2y;
	if (centery[i] + c1y < bottom) bottom = centery[i] + c1y;
	if (centery[i] + c2y < bottom) bottom = centery[i] + c2y;

	c1x = ABS(c1x);
	c2x = ABS(c2x);
	c1y = ABS(c1y);
	c2y = ABS(c2y);

	if (c1x > c2x) wid = 2*c1x;
	else wid = 2*c2x;
	if (c1y > c2y) hgt = 2*c1y;
	else hgt = 2*c2y;

	left = left * ((float) width) / uwidth; 
	bottom = ((float) height) - bottom * ((float) height) / uheight;
	wid = wid * ((float) width) / uwidth;
	hgt = hgt * ((float) height) / uheight;

	rec.x = (int) floor(left) - 1;
	if (rec.x < 0) rec.x = 0;
	rec.y = (int) floor(bottom - hgt) - 1;
	if (rec.y < 0) rec.y = 0;
	rec.width = (int) ceil(wid) + 2;
	rec.height = (int) ceil(hgt) + 2;

	XUnionRectWithRegion(&rec, reg, reg);
    }
}
