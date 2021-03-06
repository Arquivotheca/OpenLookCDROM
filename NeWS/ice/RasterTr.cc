/*
 *	Copyright (c) 1990 by Columbia University.
 */

#include <stdio.h>

#include "ice_defines.h"
#include "ice_externs.h"
#include "Raster.h"

extern void	ice_err(char *, int);
extern void	ras_drawoutline(Raster *);
extern void	pg_draw();
extern void	pg_erasecoords();
extern void	pg_showcoords(int, int, int, int, float, float, int, int);

extern Grobj *loc_grobj;
extern boolean pgcoords_drawn;
extern boolean rasoutline_drawn;
extern ras_x, ras_y;

Raster *tr_ras;

void
trras_proc(Menu *m, Menu_item *mi)
{
	float fx, fy;
	int ix, iy;
	char xbuf[30], ybuf[30];

	if ((tr_ras= (Raster *) menuitem_get(mi, LXMI_CLIENTDATA)) == (Raster *) NULL) {
		ice_err("Cannot locate selected raster.", NONFATAL);
		ice_op= MAIN_MENU;
		return;
	}

	ice_op= RAS_TRANSLATE;

	if (pg_loc == PG_CURSORLOC) {
		XSelectInput(dpy, pg_cwin, ExposureMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask | PointerMotionMask);
		return;
	}

	loc_grobj= (Grobj *) tr_ras;
	loc_grobj->getloc(&fx, &fy, &ix, &iy);
	panelitem_set(locattr_panel, locattr_lab, LXPI_STATE, LXPI_INACTIVE, LXPI_NULL);
	panelitem_set(locattr_panel, locattr_abort, LXPI_STATE, LXPI_ACTIVE, LXPI_NULL);
	switch (pg_units) {
	case PG_PIXELS:
		(void) sprintf(xbuf, "%1d", ix);
		(void) sprintf(ybuf, "%1d", iy);
		break;
	case PG_POINTS:
		(void) sprintf(xbuf, "%4.2f", (float) (fx*72.));
		(void) sprintf(ybuf, "%4.2f", (float) (fy*72.));
		break;
	case PG_INCHES:
		(void) sprintf(xbuf, "%4.2f", fx);
		(void) sprintf(ybuf, "%4.2f", fy);
		break;
	case PG_USER:
		fx= (float) ((fx-pg_xri)*pg_hsi)+pg_xru;
		fy= (float) ((fy-pg_yri)*pg_vsi)+pg_yru;
		(void) sprintf(xbuf, "%4.2f", fx);
		(void) sprintf(ybuf, "%4.2f", fy);
		break;
	}
	panelitem_set(locattr_panel, locattr_x, LXPTEXT_VALUE, xbuf, LXPI_NULL);
	panelitem_set(locattr_panel, locattr_y, LXPTEXT_VALUE, ybuf, LXPI_NULL);
	XMapRaised(dpy, locattr_frame);
	return;
}

void
rastr_event(XEvent *event)
{
	XButtonEvent *bevt;
	XMotionEvent *mevt;
	int vx, vy, x, y, w, h, d, junk;
	float fx, fy;

	vx= *((int *) canvas_get(pg_canvas, LXC_XOFFSET));
	vy= *((int *) canvas_get(pg_canvas, LXC_YOFFSET));

	switch (event->type) {

	case ButtonRelease:
		if (!rasoutline_drawn)
			return;
		bevt= (XButtonEvent *) event;
		if (pgcoords_drawn)
			pg_erasecoords();
		ras_drawoutline(tr_ras);
		XDefineCursor(dpy, pg_cwin, hg_cursor);
		XSync(dpy, False);
		x= bevt->x+vx;
		y= bevt->y+vy;
		tr_ras->setploc(x, pg_pixheight-1-y, (float) pg_dpi);
		pg_draw();
		XDefineCursor(dpy, pg_cwin, std_cursor);
		XSync(dpy, False);
		XSelectInput(dpy, pg_cwin, ExposureMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask);
		ice_op= MAIN_MENU;
		return;

	case ButtonPress:
		bevt= (XButtonEvent *) event;
		x= bevt->x+vx;
		y= bevt->y+vy;
		tr_ras->setploc(x, pg_pixheight-1-y, (float) pg_dpi);
		tr_ras->getloc(&fx, &fy, &x, &junk);
		break;

	case MotionNotify:
		mevt= (XMotionEvent *) event;
		x= mevt->x+vx;
		y= mevt->y+vy;
		tr_ras->setploc(x, pg_pixheight-1-y, (float) pg_dpi);
		tr_ras->getloc(&fx, &fy, &x, &junk);
		break;
	default:
		return;
	}

	if (rasoutline_drawn)
		ras_drawoutline(tr_ras);
	tr_ras->getsize(&w, &h, &d);
	pg_showcoords(x-vx+1, y-vy+1, w-2, h-2, fx, fy, x, pg_pixheight-1-y);
	ras_x= x;
	ras_y= y;
	ras_drawoutline(tr_ras);

	return;
}
