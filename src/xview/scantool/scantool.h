
/*  @(#)scantool.h 1.4 90/04/04
 *
 *  Definitions used by scantool.
 *
 *  Copyright (c) Rich Burridge.
 *                Sun Microsystems, Australia - All rights reserved.
 *
 *  Permission is given to distribute these sources, as long as the
 *  copyright messages are not removed, and no monies are exchanged.
 *
 *  No responsibility is taken for any errors or inaccuracies inherent
 *  either to the comments or the code of this program, but if
 *  reported to me, then an attempt will be made to fix them.
 */

#define  CLOSE       (void) close      /* To make lint happy. */
#define  EXECL       (void) execl
#define  FGETS       (void) fgets
#define  FPRINTF     (void) fprintf
#define  KILL        (void) kill
#define  READ        (void) read
#define  SPRINTF     (void) sprintf
#define  STRCPY      (void) strcpy
#define  STRNCAT     (void) strncat
#define  STRNCPY     (void) strncpy

#define  BUTTON_HEIGHT  28        /* Height of a command button in pixels. */
#define  BUTTON_WIDTH   64        /* Width of a command button in pixels. */
#define  EQUAL          !strcmp   /* For character comparisons. */
#define  FONT_HEIGHT    14        /* Dimensions of the character fonts used. */
#define  FRAMEGAP       LINEGAP / 2       /* Scanning frame interval. */

#ifndef  HELPNAME
#define  HELPNAME       "scantool.help"   /* Name of scantool help file. */
#endif /*HELPNAME*/

#define  INC            argc-- ; argv++ ;
#define  LINEGAP        10      /* Gap between lines in scanning frame. */
#define  MAXCURSORS     4       /* Maximum number of cursors used. */
#define  MAXFONTS       2       /* Maximum number of fonts used. */
#define  MAXIMAGES      5       /* Maximum number of graphics images. */
#define  MAXLINE        80      /* Length of character strings. */
#define  MAXMENUS       6       /* Maximum number of popup menus. */
#define  MAXOPS         6       /* Number of pseudo rasterop functions. */
#define  MBAR_HEIGHT    20      /* Height of the top menu bar. */
#define  MBAR_WIDTH     100     /* Width of a portion of the top menu bar. */
#define  OFF            0       /* Used to define toggle switch positions. */
#define  ON             1
#define  SCAN_FRAME_X   26      /* Initial start position of scanning frame. */
#define  SCAN_FRAME_Y   47
#define  SCAN_HEIGHT    506     /* Height of the scantool window. */
#define  SCAN_WIDTH     600     /* Width of the scantool window. */
#define  SWITCH_HEIGHT  90      /* Height of a toggle box switch. */

#define  X1             0       /* Coordinate values within framevals. */
#define  Y1             1
#define  X2             2
#define  Y2             3

#define  OK             1       /* Text button states. */
#define  CANCEL         0

#define  BACKSPACE      8       /* Used for filename construction. */
#define  DEL            127
#define  LINEFEED       13
#define  RETURN         10

/* Pseudo events generated by scantool. */
#define  LEFT_DOWN         0    /* Left mouse button was depressed. */
#define  LEFT_UP           1    /* Left mouse button was debounced. */
#define  MIDDLE_DOWN       2    /* Middle mouse button was depressed. */
#define  MIDDLE_UP         3    /* Middle mouse button was debounced. */
#define  RIGHT_DOWN        4    /* Right mouse button was depressed. */
#define  RIGHT_UP          5    /* Right mouse button was debounced. */
#define  KEYBOARD          6    /* A keyboard key has been pressed. */
#define  REPAINT           7    /* Scantool canvas needs repainting. */
#define  MOUSE_MOVE        8    /* The mouse is being moved. */
#define  MOUSE_DRAG        9    /* The mouse is being dragged. */
#define  IGNORE            10   /* Not interested in this event. */

/* Abbreviations for box switch values. */
#define  MODE              switches[0]
#define  DATA_TRANSFER     switches[1]
#define  SERIAL_PORT       switches[2]
#define  BAUD_RATE         switches[3]

#define  SET_PICTURE      1     /* Values for the Set menu commands. */
#define  SET_DEFAULTS     2
#define  SET_CLEAR_FRAME  3
#define  SET_NEW_FRAME    4
#define  SET_CURRENT      5

/* Cursor types used by scantool. */
enum cur_type { FRAME_CUR, HELP_CUR, HOUR_CUR, MAIN_CUR } ;

/* States that the drawing canvas can be in. */
enum draw_type { DO_NOTHING, DO_PICNAME, DO_MESSAGE, DO_HELP, DO_SETTINGS } ;

enum font_type { BFONT, NFONT } ;          /* Fonts used by scantool. */

/* Various images types used by scantool. */
enum image_type { B_NORMAL, B_INVERT, S_NORMAL, S_INVERT,
                  M_NORMAL, M_INVERT, EXCLAIM_IMAGE } ;

/* Menus used by scantool. */
enum menu_type { BRIGHTNESS_M, CONTRAST_M, GRAIN_M, HELP_M,
                 RESOLUTION_M, SET_M, WINDOW_M } ;

/* Pseudo rasterop functions. */
enum op_type { GSET, GCLR, GXOR, GSRC, GOR, GNOT } ;

enum sten_type { STEN_OFF, STEN_ON, STEN_INV } ;   /* Text stencil types. */

#ifndef LINT_CAST
#ifdef lint
#define LINT_CAST(arg)  (arg ? 0 : 0)
#else
#define LINT_CAST(arg)  (arg)
#endif lint
#endif LINT_CAST