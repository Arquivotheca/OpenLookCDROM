/* Simple tool to display a bitmap in the center of the display surrounded
 * by a weave pattern. Based on xsetroot source. Written by K.Osterberg
 * 4/1993. Public domain as far as I'm concerned. */

/*
 * $XConsortium: xsetroot.c,v 1.21 91/04/24 08:22:41 gildea Exp $
 *
 * Copyright 1987, Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * M.I.T. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL M.I.T.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <stdio.h>
#include "weave"

char *index();

#define Dynamic 1

char *program_name;
Display *dpy;
int screen;
Window root;
char *fore_color = NULL;
char *back_color = NULL;
int reverse = 0;
int save_colors = 0;
int unsave_past = 0;
Pixmap save_pixmap = (Pixmap)None;
int bitcenter = 0;

usage()
{
    fprintf(stderr, "usage: %s [options]\n", program_name);
    fprintf(stderr, "  where options are:\n");
    fprintf(stderr, "  -display <display>   or   -d <display>\n");
    fprintf(stderr, "  -fg <color>   or   -foreground <color>\n");
    fprintf(stderr, "  -bg <color>   or   -background <color>\n");
    fprintf(stderr, "  -rv   or   -reverse\n");
    fprintf(stderr, "  -def   or   -default\n");
    fprintf(stderr, "  -gray   or   -grey\n");
    fprintf(stderr, "  -bitcenter <filename>\n");
    exit(1);
    /*NOTREACHED*/
}

Pixmap ReadBitmapFile();
unsigned long NameToPixel();

main(argc, argv) 
    int argc;
    char **argv;
{
    int excl = 0;
    int nonexcl = 0;
    int restore_defaults = 0;
    char *display_name = NULL;
    char *name = NULL;
    int gray = 0;
    char *bitmap_file = NULL;
    register int i;
    unsigned int ww, hh;
    Pixmap bitmap;

    program_name=argv[0];

    for (i = 1; i < argc; i++) {
	if (!strcmp ("-display", argv[i]) || !strcmp ("-d", argv[i])) {
	    if (++i>=argc) usage ();
	    display_name = argv[i];
	    continue;
	}
	if (!strcmp("-def", argv[i]) || !strcmp("-default", argv[i])) {
	    restore_defaults = 1;
	    continue;
	}
	if (!strcmp("-fg",argv[i]) || !strcmp("-foreground",argv[i])) {
	    if (++i>=argc) usage();
	    fore_color = argv[i];
	    continue;
	}
	if (!strcmp("-bg",argv[i]) || !strcmp("-background",argv[i])) {
	    if (++i>=argc) usage();
	    back_color = argv[i];
	    continue;
	}
	if (!strcmp("-gray", argv[i]) || !strcmp("-grey", argv[i])) {
	    gray = 1;
	    excl++;
	    continue;
	}
	if (!strcmp("-bitcenter", argv[i])) {
	    if (++i>=argc) usage();
	    bitmap_file = argv[i];
            bitcenter = 1;
	    excl++;
	    continue;
	}
	if (!strcmp("-rv",argv[i]) || !strcmp("-reverse",argv[i])) {
	    reverse = 1;
	    continue;
	}
	usage();
    } 

    /* Check for multiple use of exclusive options */
    if (excl > 1) {
	fprintf(stderr, "%s: choose only one of {solid, gray, bitmap, mod}\n",
		program_name);
	usage();
    }

    dpy = XOpenDisplay(display_name);
    if (!dpy) {
	fprintf(stderr, "%s:  unable to open display '%s'\n",
		program_name, XDisplayName (display_name));
	exit (2);
    }
    screen = DefaultScreen(dpy);
    root = RootWindow(dpy, screen);
  
    /* If there are no arguments then restore defaults. */
    if (!excl && !nonexcl)
	restore_defaults = 1;
  
    /* Handle -gray and -grey options */
    if (gray) {
	bitmap = XCreateBitmapFromData(dpy, root, weave_bits,
				       weave_width, weave_height);
	SetBackgroundToBitmap(bitmap, weave_width, weave_height);
    }
  
    /* Handle -bitcenter option */
    if (bitmap_file && bitcenter) {
	Pixmap grbitmap;

	bitmap = ReadBitmapFile(bitmap_file, &ww, &hh, (int *)NULL, (int *)NULL);
	grbitmap = XCreateBitmapFromData(dpy, root, weave_bits,
				       weave_width, weave_height);
	CenterBackgroundBitmap(bitmap, grbitmap, ww, hh, weave_width, weave_height);
    }
  
    /* Handle restore defaults */
    if (restore_defaults) {
        XUndefineCursor(dpy, root);
	if (!excl) {
	    XSetWindowBackgroundPixmap(dpy, root, (Pixmap) None);
	    XClearWindow(dpy, root);
	    unsave_past = 1;
	}
    }
  
    FixupState();
    XCloseDisplay(dpy);
    exit (0);
}


/* Free past incarnation if needed, and retain state if needed. */
FixupState()
{
    Atom prop, type;
    int format;
    unsigned long length, after;
    unsigned char *data;

    if (!(DefaultVisual(dpy, screen)->class & Dynamic))
	unsave_past = 0;
    if (!unsave_past && !save_colors)
	return;
    prop = XInternAtom(dpy, "_XSETROOT_ID", False);
    if (unsave_past) {    
	(void)XGetWindowProperty(dpy, root, prop, 0L, 1L, True, AnyPropertyType,
				 &type, &format, &length, &after, &data);
	if ((type == XA_PIXMAP) && (format == 32) &&
	    (length == 1) && (after == 0))
	    XKillClient(dpy, *((Pixmap *)data));
	else if (type != None)
	    fprintf(stderr, "%s: warning: _XSETROOT_ID property is garbage\n",
		    program_name);
    }
    if (save_colors) {
	if (!save_pixmap)
	    save_pixmap = XCreatePixmap(dpy, root, 1, 1, 1);
	XChangeProperty(dpy, root, prop, XA_PIXMAP, 32, PropModeReplace,
			(unsigned char *) &save_pixmap, 1);
	XSetCloseDownMode(dpy, RetainPermanent);
    }
}

/*
 * SetBackgroundToBitmap: Set the root window background to a caller supplied 
 *                        bitmap.
 */
SetBackgroundToBitmap(bitmap, width, height)
    Pixmap bitmap;
    unsigned int width, height;
{
    Pixmap pix;
    GC gc;
    XGCValues gc_init;

    gc_init.foreground = NameToPixel(fore_color, BlackPixel(dpy, screen));
    gc_init.background = NameToPixel(back_color, WhitePixel(dpy, screen));
    if (reverse) {
	unsigned long temp=gc_init.foreground;
	gc_init.foreground=gc_init.background;
	gc_init.background=temp;
    }
    gc = XCreateGC(dpy, root, GCForeground|GCBackground, &gc_init);
    pix = XCreatePixmap(dpy, root, width, height,
			(unsigned int)DefaultDepth(dpy, screen));
    XCopyPlane(dpy, bitmap, pix, gc, 0, 0, width, height, 0, 0, (unsigned long)1);
    XSetWindowBackgroundPixmap(dpy, root, pix);
    XFreeGC(dpy, gc);
    XFreePixmap(dpy, bitmap);
    if (save_colors)
	save_pixmap = pix;
    else
	XFreePixmap(dpy, pix);
    XClearWindow(dpy, root);
    unsave_past = 1;
}

/*
 * CenterBackgroundBitmap: Set the root window background to a caller supplied 
 *                        bitmap.
 */
CenterBackgroundBitmap(bitmap, grbitmap, width, height, grwidth, grheight)
    Pixmap bitmap;
    Pixmap grbitmap;
    unsigned int width, height;
    unsigned int grwidth, grheight;
{
    Pixmap pix;
    GC gc;
    XGCValues gc_init;
    unsigned int dw, dh, dd, x, y;

    gc_init.foreground = NameToPixel(fore_color, BlackPixel(dpy, screen));
    gc_init.background = NameToPixel(back_color, WhitePixel(dpy, screen));
    if (reverse) {
	unsigned long temp=gc_init.foreground;
	gc_init.foreground=gc_init.background;
	gc_init.background=temp;
    }
    gc = XCreateGC(dpy, root, GCForeground|GCBackground, &gc_init);
    dw = DisplayWidth(dpy, screen);
    dh = DisplayHeight(dpy, screen);
    dd = DefaultDepth(dpy, screen);
    pix = XCreatePixmap(dpy, root, dw, dh, dd);
    for (y = 0; y < dh; y += grheight)
      for (x = 0; x < dw; x += grwidth)
        XCopyPlane(dpy, grbitmap, pix, gc, 0, 0, grwidth, grheight, x, y, (unsigned long)1);
    XCopyPlane(dpy, bitmap, pix, gc, 0, 0, width, height,
               (dw - width) / 2, (dh - height) / 2, (unsigned long)1);
    XSetWindowBackgroundPixmap(dpy, root, pix);
    XFreeGC(dpy, gc);
    XFreePixmap(dpy, bitmap);
    if (save_colors)
	save_pixmap = pix;
    else
	XFreePixmap(dpy, pix);
    XClearWindow(dpy, root);
    unsave_past = 1;
}

unsigned long NameToPixel(name, pixel)
    char *name;
    unsigned long pixel;
{
    XColor ecolor;

    if (!name || !*name)
	return pixel;
    if (!XParseColor(dpy,DefaultColormap(dpy,screen),name,&ecolor)) {
	fprintf(stderr,"%s:  unknown color \"%s\"\n",program_name,name);
	exit(1);
	/*NOTREACHED*/
    }
    if (!XAllocColor(dpy, DefaultColormap(dpy, screen),&ecolor)) {
	fprintf(stderr, "%s:  unable to allocate color for \"%s\"\n",
		program_name, name);
	exit(1);
	/*NOTREACHED*/
    }
    if ((ecolor.pixel != BlackPixel(dpy, screen)) &&
	(ecolor.pixel != WhitePixel(dpy, screen)) &&
	(DefaultVisual(dpy, screen)->class & Dynamic))
	save_colors = 1;
    return(ecolor.pixel);
}

Pixmap ReadBitmapFile(filename, width, height, x_hot, y_hot)
    char *filename;
    unsigned int *width, *height;
    int *x_hot, *y_hot;
{
    Pixmap bitmap;
    int status;

    status = XReadBitmapFile(dpy, root, filename, width,
			     height, &bitmap, x_hot, y_hot);
    if (status == BitmapSuccess)
      return(bitmap);
    else if (status == BitmapOpenFailed)
	fprintf(stderr, "%s: can't open file: %s\n", program_name, filename);
    else if (status == BitmapFileInvalid)
	fprintf(stderr, "%s: bad bitmap format file: %s\n",
			program_name, filename);
    else
	fprintf(stderr, "%s: insufficient memory for bitmap: %s",
			program_name, filename);
    exit(1);
    /*NOTREACHED*/
}

