/* Copyright 1990-93 GROUPE BULL -- See license conditions in file COPYRIGHT */
/*****************************************************************************\
* XpmRdFToP.c:                                                                *
*                                                                             *
*  XPM library                                                                *
*  Parse an XPM file and create the pixmap and possibly its mask              *
*                                                                             *
*  Developed by Arnaud Le Hors                                                *
\*****************************************************************************/

#include "xpmP.h"

int
XpmReadFileToPixmap(display, d, filename, pixmap_return,
		    shapemask_return, attributes)
    Display *display;
    Drawable d;
    char *filename;
    Pixmap *pixmap_return;
    Pixmap *shapemask_return;
    XpmAttributes *attributes;
{
    XImage *image, **imageptr = NULL;
    XImage *shapeimage, **shapeimageptr = NULL;
    int ErrorStatus;
    XGCValues gcv;
    GC gc;

    /*
     * initialize return values
     */
    if (pixmap_return) {
	*pixmap_return = 0;
	imageptr = &image;
    }
    if (shapemask_return) {
	*shapemask_return = 0;
	shapeimageptr = &shapeimage;
    }

    /*
     * create the images
     */
    ErrorStatus = XpmReadFileToImage(display, filename, imageptr,
				     shapeimageptr, attributes);
    if (ErrorStatus < 0)
	return (ErrorStatus);

    /*
     * create the pixmaps
     */
    if (imageptr && image) {
	*pixmap_return = XCreatePixmap(display, d, image->width,
				       image->height, image->depth);
	gcv.function = GXcopy;
	gc = XCreateGC(display, *pixmap_return, GCFunction, &gcv);

	XPutImage(display, *pixmap_return, gc, image, 0, 0, 0, 0,
		  image->width, image->height);

	XDestroyImage(image);
	XFreeGC(display, gc);
    }
    if (shapeimageptr && shapeimage) {
	*shapemask_return = XCreatePixmap(display, d, shapeimage->width,
					  shapeimage->height,
					  shapeimage->depth);
	gcv.function = GXcopy;
	gc = XCreateGC(display, *shapemask_return, GCFunction, &gcv);

	XPutImage(display, *shapemask_return, gc, shapeimage, 0, 0, 0, 0,
		  shapeimage->width, shapeimage->height);

	XDestroyImage(shapeimage);
	XFreeGC(display, gc);
    }
    return (ErrorStatus);
}
