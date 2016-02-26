/* Copyright 1990-93 GROUPE BULL -- See license conditions in file COPYRIGHT */
/*****************************************************************************\
* xpm.h:                                                                      *
*                                                                             *
*  XPM library                                                                *
*  Include file                                                               *
*                                                                             *
*  Developed by Arnaud Le Hors                                                *
\*****************************************************************************/

#ifndef XPM_h
#define XPM_h

#ifdef VMS
#include "decw$include:Xlib.h"
#include "decw$include:Xutil.h"
#else
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif

/* let's define Pixel if it is not done yet */
#ifndef _XtIntrinsic_h
typedef unsigned long Pixel;		/* Index into colormap */
#endif

/* Return ErrorStatus codes:
 * null     if full success
 * positive if partial success
 * negative if failure
 */

#define XpmColorError    1
#define XpmSuccess       0
#define XpmOpenFailed   -1
#define XpmFileInvalid  -2
#define XpmNoMemory     -3
#define XpmColorFailed  -4

/* the following should help people wanting to use their own functions */
#define XpmFree(ptr) free(ptr)

typedef struct {
    char *name;				/* Symbolic color name */
    char *value;			/* Color value */
    Pixel pixel;			/* Color pixel */
}      XpmColorSymbol;

typedef struct {
    char *name;				/* name of the extension */
    unsigned int nlines;		/* number of lines in this extension */
    char **lines;			/* pointer to the extension array of
					 * strings */
}      XpmExtension;

typedef struct {
    unsigned long valuemask;		/* Specifies which attributes are
					 * defined */

    Visual *visual;			/* Specifies the visual to use */
    Colormap colormap;			/* Specifies the colormap to use */
    unsigned int depth;			/* Specifies the depth */
    unsigned int width;			/* Returns the width of the created
					 * pixmap */
    unsigned int height;		/* Returns the height of the created
					 * pixmap */
    unsigned int x_hotspot;		/* Returns the x hotspot's
					 * coordinate */
    unsigned int y_hotspot;		/* Returns the y hotspot's
					 * coordinate */
    unsigned int cpp;			/* Specifies the number of char per
					 * pixel */
    Pixel *pixels;			/* List of used color pixels */
    unsigned int npixels;		/* Number of pixels */
    XpmColorSymbol *colorsymbols;	/* Array of color symbols to
					 * override */
    unsigned int numsymbols;		/* Number of symbols */
    char *rgb_fname;			/* RGB text file name */
    unsigned int nextensions;		/* number of extensions */
    XpmExtension *extensions;		/* pointer to array of extensions */

    /* Infos */
    unsigned int ncolors;		/* Number of colors */
    char ***colorTable;			/* Color table pointer */
    char *hints_cmt;			/* Comment of the hints section */
    char *colors_cmt;			/* Comment of the colors section */
    char *pixels_cmt;			/* Comment of the pixels section */
    unsigned int mask_pixel;		/* Transparent pixel's color table
					 * index */
    /* Color Allocation Directives */
    unsigned int exactColors;		/* Only use exact colors for visual */
    unsigned int closeness;		/* Allowable RGB deviation */
    unsigned int red_closeness;		/* Allowable red deviation */
    unsigned int green_closeness;	/* Allowable green deviation */
    unsigned int blue_closeness;	/* Allowable blue deviation */
    int color_key;			/* Use colors from this color set */

}      XpmAttributes;

/* Xpm attribute value masks bits */
#define XpmVisual	   (1L<<0)
#define XpmColormap	   (1L<<1)
#define XpmDepth	   (1L<<2)
#define XpmSize		   (1L<<3)	/* width & height */
#define XpmHotspot	   (1L<<4)	/* x_hotspot & y_hotspot */
#define XpmCharsPerPixel   (1L<<5)
#define XpmColorSymbols	   (1L<<6)
#define XpmRgbFilename	   (1L<<7)
#define XpmInfos	   (1L<<8)	/* all infos members */
#define XpmExtensions      (1L<<10)

#define XpmReturnPixels	   (1L<<9)
#define XpmReturnInfos	   XpmInfos
#define XpmReturnExtensions XpmExtensions

#define XpmExactColors     (1L<<11)
#define XpmCloseness	   (1L<<12)
#define XpmRGBCloseness	   (1L<<13)
#define XpmColorKey	   (1L<<14)

/*
 * color keys for visual type, they must match those defined in xpmP.h
 */
#define XPM_MONO	2
#define XPM_GREY4	3
#define XPM_GRAY4	3
#define XPM_GREY 	4
#define XPM_GRAY 	4
#define XPM_COLOR	5

/*
 * minimal portability layer between ansi and KR C
 */

/* forward declaration of functions with prototypes */

#if __STDC__ || defined(__cplusplus) || defined(c_plusplus)
 /* ANSI || C++ */
#define FUNC(f, t, p) extern t f p
#define LFUNC(f, t, p) static t f p
#else					/* K&R */
#define FUNC(f, t, p) extern t f()
#define LFUNC(f, t, p) static t f()
#endif					/* end of K&R */


/*
 * functions declarations
 */

#ifdef __cplusplus
extern "C" {
#endif

    FUNC(XpmCreatePixmapFromData, int, (Display *display,
					Drawable d,
					char **data,
					Pixmap *pixmap_return,
					Pixmap *shapemask_return,
					XpmAttributes *attributes));

    FUNC(XpmCreateDataFromPixmap, int, (Display *display,
					char ***data_return,
					Pixmap pixmap,
					Pixmap shapemask,
					XpmAttributes *attributes));

    FUNC(XpmReadFileToPixmap, int, (Display *display,
				    Drawable d,
				    char *filename,
				    Pixmap *pixmap_return,
				    Pixmap *shapemask_return,
				    XpmAttributes *attributes));

    FUNC(XpmWriteFileFromPixmap, int, (Display *display,
				       char *filename,
				       Pixmap pixmap,
				       Pixmap shapemask,
				       XpmAttributes *attributes));

    FUNC(XpmCreateImageFromData, int, (Display *display,
				       char **data,
				       XImage **image_return,
				       XImage **shapemask_return,
				       XpmAttributes *attributes));

    FUNC(XpmCreateDataFromImage, int, (Display *display,
				       char ***data_return,
				       XImage *image,
				       XImage *shapeimage,
				       XpmAttributes *attributes));

    FUNC(XpmReadFileToImage, int, (Display *display,
				   char *filename,
				   XImage **image_return,
				   XImage **shapeimage_return,
				   XpmAttributes *attributes));

    FUNC(XpmWriteFileFromImage, int, (Display *display,
				      char *filename,
				      XImage *image,
				      XImage *shapeimage,
				      XpmAttributes *attributes));

    FUNC(XpmCreateImageFromBuffer, int, (Display *display,
					 char *buffer,
					 XImage **image_return,
					 XImage **shapemask_return,
					 XpmAttributes *attributes));

    FUNC(XpmCreatePixmapFromBuffer, int, (Display *display,
					  Drawable d,
					  char *buffer,
					  Pixmap *pixmap_return,
					  Pixmap *shapemask_return,
					  XpmAttributes *attributes));

    FUNC(XpmCreateBufferFromImage, int, (Display *display,
					 char **buffer_return,
					 XImage *image,
					 XImage *shapeimage,
					 XpmAttributes *attributes));

    FUNC(XpmCreateBufferFromPixmap, int, (Display *display,
					  char **buffer_return,
					  Pixmap pixmap,
					  Pixmap shapemask,
					  XpmAttributes *attributes));

    FUNC(XpmReadFileToBuffer, int, (char *filename, char **buffer_return));
    FUNC(XpmWriteFileFromBuffer, int, (char *filename, char *buffer));

    FUNC(XpmReadFileToData, int, (char *filename, char ***data_return));
    FUNC(XpmWriteFileFromData, int, (char *filename, char **data));

    FUNC(XpmAttributesSize, int, ());
    FUNC(XpmFreeAttributes, void, (XpmAttributes *attributes));
    FUNC(XpmFreeExtensions, void, (XpmExtension *extensions,
				   int nextensions));

#ifdef __cplusplus
}					/* for C++ V2.0 */
#endif


/* backward compatibility */

/* for version 3.0c */
#define XpmPixmapColorError  XpmColorError
#define XpmPixmapSuccess     XpmSuccess
#define XpmPixmapOpenFailed  XpmOpenFailed
#define XpmPixmapFileInvalid XpmFileInvalid
#define XpmPixmapNoMemory    XpmNoMemory
#define XpmPixmapColorFailed XpmColorFailed

#define XpmReadPixmapFile(dpy, d, file, pix, mask, att) \
    XpmReadFileToPixmap(dpy, d, file, pix, mask, att)
#define XpmWritePixmapFile(dpy, file, pix, mask, att) \
    XpmWriteFileFromPixmap(dpy, file, pix, mask, att)

/* for version 3.0b */
#define PixmapColorError  XpmColorError
#define PixmapSuccess     XpmSuccess
#define PixmapOpenFailed  XpmOpenFailed
#define PixmapFileInvalid XpmFileInvalid
#define PixmapNoMemory    XpmNoMemory
#define PixmapColorFailed XpmColorFailed

#define ColorSymbol XpmColorSymbol

#define XReadPixmapFile(dpy, d, file, pix, mask, att) \
    XpmReadFileToPixmap(dpy, d, file, pix, mask, att)
#define XWritePixmapFile(dpy, file, pix, mask, att) \
    XpmWriteFileFromPixmap(dpy, file, pix, mask, att)
#define XCreatePixmapFromData(dpy, d, data, pix, mask, att) \
    XpmCreatePixmapFromData(dpy, d, data, pix, mask, att)
#define XCreateDataFromPixmap(dpy, data, pix, mask, att) \
    XpmCreateDataFromPixmap(dpy, data, pix, mask, att)

#endif
