
/*  @(#)xview.c 1.5 90/04/10
 *
 *  XView dependent graphics routines used by scantool.
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

#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "scantool.h"
#include "scantool_extern.h"
#include "images.h"
#include <rasterfile.h>
#include <xview/xview.h>
#include <xview/canvas.h>
#include <xview/font.h>"
#include <xview/fullscreen.h>
#include <xview/cursor.h>
#include <xview/svrimage.h>
#include <X11/Xlib.h>
#define  NOTIFY_INTERPOSE_DESTROY_FUNC  (void) notify_interpose_destroy_func
#define  NOTIFY_SET_SIGNAL_FUNC         (void) notify_set_signal_func
#define  NOTIFY_SET_WAIT3_FUNC          (void) notify_set_wait3_func
#define  PR_DESTROY                     (void) pr_destroy
#define  PW_WRITE                       (void) pw_write
#define  XV_INPUT_READEVENT             (void) xv_input_readevent
#define  XV_SET                         (void) xv_set

#define  BOLDFONT    "lucidasanstypewriter-bold-12"
#define  DEFFONT     "fixed"
#define  NORMALFONT  "lucidasanstypewriter-12"
#define NULL 0
int menu_proc() ;
void repaint_show_canvas() ;

Canvas canvas, show_canvas ;
Canvas_paint_window spw, wpw ;
Event *cur_event ;
Frame base_frame, show_frame ;
Fullscreen fs ;
Icon scantool_icon ;
Menu menus[MAXMENUS] ;
Notify_value destroy() ;
Pixrect *spr ;
Server_image frame_cursor_pr, help_cursor_pr, hour_cursor_pr, main_cursor_pr ;
Server_image make_server_image() ;

Xv_cursor cursor[MAXCURSORS] ;

Display *dpy ;                  /* Display id of scantool frame. */
Drawable xid ;                  /* Xid for scantool canvas. */
Drawable grey_pr ;
Drawable images[MAXIMAGES] ;    /* Xids for various graphical images. */
GC backgc ;                     /* Graphics context for pw_writebackground. */
GC gc ;                         /* Graphics context for text and lines. */
GC ropgc ;                      /* Graphics context for rops. */
Window root ;
XFontStruct *font[MAXFONTS] ;   /* Xlib handles to the fonts. */
XGCValues gc_val ;              /* Used to setup graphics context values. */
int gc_flags ;                  /* Used to set up graphics context flags. */
int screen ;                    /* Default graphics display screen. */
unsigned long backgnd ;         /* Default background color. */
unsigned long foregnd ;         /* Default foreground color. */
unsigned long gc_mask ;         /* Mask for setting graphic context values. */

enum menu_type cur_menu ;     /* Current menu type being processed. */

int init_height ;             /* Initial height of the scantool window. */
int init_width ;              /* Initial width of the scantool window. */
int started ;                 /* Kludge to correct handle repaints. */

struct rasterfile hdr ;

mpr_static(icon_pr, 64, 64, 1, icon_image) ;

/*ARGSUSED*/
void
canvas_proc(canvas, event)
Canvas canvas ;
Event *event ;
{
  if (!started) return ;
  cur_event = event ;
  handle_event() ;       /* Determine what kind of event it is. */
  process_event() ;      /* And process it. */
}

create_menu(mtype, title, values)    /* Create popup menus for cycle items. */
enum menu_type mtype ;
char *title, *values[] ;
{
  int i = 0 ;
  int more = 1 ;   /* Cleared when current menu is complete.*/

  menus[(int) mtype] = xv_create(XV_NULL,          MENU_COMMAND_MENU,
                                 MENU_NOTIFY_PROC, menu_proc,
                                 MENU_TITLE_ITEM,  title,
                                 0) ;
  do
    {
      if (values[i] != NULL)
        XV_SET(menus[(int) mtype], MENU_STRING_ITEM, values[i], i+1, 0) ;
      else more = 0 ;
      i++ ;
    }
  while (more) ;
}

Notify_value
destroy(client, status)
Notify_client client ;
Destroy_status status ;
{
  stop_scan() ;        /* Stop the current scan (if there is one). */
  exit(0) ;
}

display_menu(menuno)
enum menu_type menuno ;
{
  cur_menu = menuno ;
  menu_show(menus[(int) menuno], canvas, cur_event, 0) ;
  return(0) ;
}

draw_area(x, y, width, height, op)
int x, y, width, height ;
enum op_type op ;
{
  if (op == GNOT)
    {
      XSetFunction(dpy, gc, GXcopyInverted) ;
      XCopyArea(dpy, xid, xid, gc, x, y, width, height, x, y) ;
    }
  else
    {
      XSetFunction(dpy, backgc, ops[(int) op]) ;
      XFillRectangle(dpy, xid, backgc, x, y, width, height) ;
    }
}

draw_image(x, y, width, height, image)
int x, y, width, height ;
enum image_type image ;
{
  gc_mask = GCStipple | GCTileStipXOrigin | GCTileStipYOrigin ;
  gc_val.stipple = images[(int) image] ;
  gc_val.ts_x_origin = x ;
  gc_val.ts_y_origin = y ;
  XChangeGC(dpy, ropgc, gc_mask, &gc_val) ;
  XFillRectangle(dpy, xid, ropgc, x, y, width, height) ;
}

draw_line(x1, y1, x2, y2, op)
int x1, y1, x2, y2 ;
enum op_type op ;
{
  gc_val.foreground = foregnd ;
  gc_val.function = ops[(int) op] ;
  XChangeGC(dpy, gc, GCForeground | GCFunction, &gc_val) ;
  XDrawLine(dpy, xid, gc, x1, y1, x2, y2) ;
}

/*ARGSUSED*/
draw_text(x, y, stencil, ftype, str)
int x, y ;
enum sten_type stencil ;
enum font_type ftype ;
char *str ;
{
  gc_val.foreground = foregnd ;
  gc_val.font = font[(int) ftype]->fid ;
  if (stencil == STEN_INV) gc_val.function = GXcopyInverted ;
  else                     gc_val.function = GXcopy ;
  XChangeGC(dpy, gc, GCFont | GCForeground | GCFunction, &gc_val) ;
  XDrawString(dpy, xid, gc, x, y, str, strlen(str)) ;
}

get_event()       /* Get the next XView event. */
{
  XV_INPUT_READEVENT(wpw, cur_event, 1, 0, NULL) ;
  if (event_is_button(cur_event))
    if (event_is_down(cur_event))
      fs = xv_create(wpw, FULLSCREEN,
                     WIN_CONSUME_EVENTS,
                       WIN_MOUSE_BUTTONS, LOC_DRAG, 0,
                     0) ;
    else if (event_is_up(cur_event)) xv_destroy(fs) ;
}

XFontStruct *
get_font(name)
char *name ;
{
  XFontStruct *f ;
 
  if (!(f = XLoadQueryFont(dpy, name)))
    if (!(f = XLoadQueryFont(dpy, DEFFONT)))
      {
        FPRINTF(stderr, "%s: couldn't get the default font.", progname) ;
        exit(1) ;
      }
  return(f) ;
}

get_strwidth(ftype, str)    /* Get width in pixels of string value. */
enum font_type ftype ;
char *str ;
{
  return(XTextWidth(font[(int) ftype], str, strlen(str))) ;
}

grey_area(x, y, width, height)
{
  gc_mask = GCStipple | GCTileStipXOrigin | GCTileStipYOrigin ;
  gc_val.stipple = grey_pr ;
  gc_val.ts_x_origin = x % 16 ;
  gc_val.ts_y_origin = y % 16 ;
  XChangeGC(dpy, ropgc, gc_mask, &gc_val) ;
  XFillRectangle(dpy, xid, ropgc, x, y, width, height) ;
}

handle_event()        /* Work out what kind of event this is. */
{
  curx = event_x(cur_event) ;
  cury = event_y(cur_event) ;
  cur_ch = event_id(cur_event) ;

       if (cur_ch == LOC_WINENTER)   win_set_kbd_focus(canvas, xid) ;
  else if (cur_ch == WIN_REPAINT)    type = REPAINT ;
  else if (cur_ch == LOC_MOVE)       type = MOUSE_MOVE ;
  else if (cur_ch == LOC_DRAG)       type = MOUSE_DRAG ;
  else if (event_is_button(cur_event) && event_is_down(cur_event))
    {
           if (cur_ch == MS_LEFT)    type = LEFT_DOWN ;
      else if (cur_ch == MS_MIDDLE)  type = MIDDLE_DOWN ;
      else if (cur_ch == MS_RIGHT)   type = RIGHT_DOWN ;
    }
  else if (event_is_button(cur_event) && event_is_up(cur_event))
    {
           if (cur_ch == MS_LEFT)    type = LEFT_UP ;
      else if (cur_ch == MS_MIDDLE)  type = MIDDLE_UP ;
      else if (cur_ch == MS_RIGHT)   type = RIGHT_UP ;
    }
  else if (event_is_ascii(cur_event) &&
           event_is_down(cur_event)) type = KEYBOARD ;
  else                               type = IGNORE ;
}

init_fonts()
{
  font[(int) NFONT] = get_font(NORMALFONT) ;
  font_width = font[(int) NFONT]->max_bounds.rbearing -
               font[(int) NFONT]->min_bounds.lbearing ;
  font[(int) BFONT] = get_font(BOLDFONT) ;
}

init_ws_type()
{
  ops[(int) GSET] = GXset ;
  ops[(int) GCLR] = GXclear ;
  ops[(int) GXOR] = GXxor ;
  ops[(int) GSRC] = GXcopy ;
  ops[(int) GOR]  = GXor ;
  ops[(int) GNOT] = GXcopyInverted ;

  started = 0 ;      /* Kludge to correctly handle repaints. */
}

make_frames(argc, argv)
int argc ;
char *argv[] ;
{
  xv_init(XV_INIT_ARGS, argc, argv, 0) ;
  scantool_icon = xv_create(XV_NULL, ICON,
                            ICON_IMAGE, &icon_pr,
                            0) ;
  base_frame = xv_create(XV_NULL,                     FRAME,
                         FRAME_ICON,                  scantool_icon,
                         FRAME_LABEL,                 progname,
                         FRAME_SHOW_RESIZE_CORNER,    FALSE,
                         0) ;

  show_frame = xv_create(base_frame, FRAME,
                         FRAME_SHOW_LABEL, TRUE,
                         XV_WIDTH,         650,
                         XV_HEIGHT,        900,
                         XV_X,             0,
                         XV_Y,             0,
                         XV_SHOW,          FALSE,
                         0) ;
}

make_subframes()
{
  canvas = xv_create(base_frame,         CANVAS,
                     CANVAS_RETAINED,    FALSE,
                     OPENWIN_AUTO_CLEAR, FALSE,
                     XV_WIDTH,           SCAN_WIDTH,
                     XV_HEIGHT,          SCAN_HEIGHT,
                     CANVAS_PAINTWINDOW_ATTRS,
                       WIN_CONSUME_EVENTS,
                         MS_LEFT, MS_MIDDLE, MS_RIGHT,
                         WIN_ASCII_EVENTS, KBD_USE, KBD_DONE,
                         LOC_WINENTER, LOC_WINEXIT, LOC_DRAG, LOC_MOVE,
                         WIN_LEFT_KEYS, WIN_TOP_KEYS, WIN_RIGHT_KEYS,
                         WIN_REPAINT,
                       0,
                       WIN_EVENT_PROC,   canvas_proc,
                       0,
                     0) ;
  wpw = canvas_paint_window(canvas) ;
 
  dpy = (Display *) xv_get(base_frame, XV_DISPLAY) ;
  xid = (Drawable) xv_get(wpw, XV_XID) ;
 
  screen = DefaultScreen(dpy) ;
  root = RootWindow(dpy, screen) ;
  foregnd = BlackPixel(dpy, screen) ;
  backgnd = WhitePixel(dpy, screen) ;
 
  gc_mask = GCForeground | GCBackground | GCGraphicsExposures ;
  gc_val.foreground = foregnd ;
  gc_val.background = backgnd ;
  gc_val.graphics_exposures = False ;
  gc = XCreateGC(dpy, root, gc_mask, &gc_val) ;

  backgc = XCreateGC(dpy, root, gc_mask, &gc_val) ;
  XSetForeground(dpy, backgc, gc_val.background) ;

  ropgc = XCreateGC(dpy, root, gc_mask, &gc_val) ;
  gc_val.function = GXcopy ;
  gc_val.fill_style = FillOpaqueStippled ;
  XChangeGC(dpy, ropgc, GCFunction | GCFillStyle, &gc_val) ;

  grey_pr = (Drawable) xv_get(make_server_image(grey_image, 16, 16), XV_XID) ;
  frame_cursor_pr = make_server_image(frame_cursor_array, 16, 16) ;
  help_cursor_pr  = make_server_image(help_cursor_array,  16, 16) ;
  hour_cursor_pr  = make_server_image(hour_cursor_array,  16, 16) ;
  main_cursor_pr  = make_server_image(main_cursor_array,  16, 16) ;
  images[(int) B_NORMAL] = (Drawable)
               xv_get(make_server_image(button_normal_image, 64, 64), XV_XID) ;
  images[(int) B_INVERT] = (Drawable)
               xv_get(make_server_image(button_invert_image, 64, 64), XV_XID) ;
  images[(int) S_NORMAL] = (Drawable)
               xv_get(make_server_image(switch_normal_image, 16, 16), XV_XID) ;
  images[(int) S_INVERT] = (Drawable)
               xv_get(make_server_image(switch_invert_image, 16, 16), XV_XID) ;
  images[(int) EXCLAIM_IMAGE] = (Drawable)
               xv_get(make_server_image(exclaim_image, 64, 64), XV_XID) ;

  cursor[(int) FRAME_CUR] = xv_create(XV_NULL,      CURSOR,
                                      CURSOR_IMAGE, frame_cursor_pr,
                                      CURSOR_XHOT,  7,
                                      CURSOR_YHOT,  7,
                                      0) ;
  cursor[(int) HELP_CUR] = xv_create(XV_NULL,      CURSOR,
                                     CURSOR_IMAGE, help_cursor_pr,
                                     0) ;
  cursor[(int) HOUR_CUR] = xv_create(XV_NULL,      CURSOR,
                                     CURSOR_IMAGE, hour_cursor_pr,
                                     0) ;
  cursor[(int) MAIN_CUR] = xv_create(XV_NULL,      CURSOR,
                                     CURSOR_IMAGE, main_cursor_pr,
                                     0) ;

  show_canvas = xv_create(show_frame, CANVAS,
                          CANVAS_REPAINT_PROC, repaint_show_canvas,
                          0) ;
  spw = canvas_paint_window(show_canvas) ;
  spr = NULL ;

  XV_SET(canvas, WIN_CURSOR, cursor[(int) MAIN_CUR], 0) ;
}

menu_proc(menu, menu_item)
Menu menu ;
Menu_item menu_item ;
{
  int choice ;

  choice = (int) menu_get(menu_item, MENU_VALUE) ;
  if (choice) process_menu(cur_menu, choice) ;
}

Server_image
make_server_image(image, width, height)
unsigned short image[] ;
int width, height ;
{
  return(xv_create(XV_NULL,           SERVER_IMAGE,
                   XV_WIDTH,          width,
                   XV_HEIGHT,         height,
                   SERVER_IMAGE_BITS, image,
                   0)) ;
}

/*ARGSUSED*/
void
repaint_show_canvas(canvas, pw, repaint_area)     /* Show current picture. */
Canvas canvas ;
Pixwin *pw ;
Rectlist *repaint_area ;
{
  PW_WRITE(pw, 0, 0, hdr.ras_width, hdr.ras_height, PIX_SRC, spr, 0, 0) ;
}

set_cursor(ctype)
enum cur_type ctype ;
{
  XV_SET(wpw, WIN_CURSOR, cursor[(int) ctype], 0) ;
}

show()      /* Display scanned image. */
{
  Rect *temprect ;
  char errmes[MAXLINE] ;
  int fd ;

  showing = 1 ;
  make_button(SCAN_WIDTH-150, 4*SWITCH_HEIGHT+100, "Show", B_INVERT) ;
  if (spr) PR_DESTROY(spr) ;
  if ((fd = open(picname, O_RDONLY | O_NDELAY, 0644)) == -1)
    {
      SPRINTF(errmes, "Cannot open %s.", picname) ;
      make_display(errmes) ;
    }
  else
    { 
      READ(fd, (char *) &hdr, sizeof(struct rasterfile)) ;
      if (hdr.ras_maplength)
        {
          make_display("Color rasterfiles currently not supported.") ;
          return ;
        }
      spr = mem_create(hdr.ras_width, hdr.ras_height, hdr.ras_depth) ;
      READ(fd, (char *) (mpr_d(spr)->md_image),
           hdr.ras_width * hdr.ras_height) ;
      CLOSE(fd) ;
      temprect = (Rect *) xv_get(show_frame, FRAME_OPEN_RECT) ;
      temprect->r_left = temprect->r_top = 0 ;
      temprect->r_height = hdr.ras_height ;
      temprect->r_width = hdr.ras_width ;
      XV_SET(show_frame, FRAME_OPEN_RECT, temprect, 0) ;
      XV_SET(show_frame, FRAME_LABEL, picname, 0) ;
      XV_SET(show_frame, XV_SHOW, TRUE, 0) ;
    }
  make_button(SCAN_WIDTH-150, 4*SWITCH_HEIGHT+100, "Show", B_NORMAL) ;
  showing = 0 ;
}

start_tool()
{
  window_fit(base_frame) ;
  NOTIFY_INTERPOSE_DESTROY_FUNC(base_frame, destroy) ;
  started = 1 ;
  window_main_loop(base_frame) ;
}

/*ARGSUSED*/
Notify_value
wait_child(frame, pid, status, rusage)
Window frame ;
int pid ;
union wait *status ;
struct rusage *rusage ;
{
  char output[MAXLINE] ;
  int high, low ;

  if (WIFSTOPPED(*status)) return (NOTIFY_IGNORED) ;
       
  high = (status->w_status & 0xFF00) >> 8 ;
  low = status->w_status & 0xFF ;
  if (low) SPRINTF(output, "Scan: terminated with signal %1d", (low & 0x3F)) ;
  else 
    {
      switch (high)
        {
          case 0  : SPRINTF(output, "Scan: successful. %s saved",
                                     picname) ;
                    break ;
          case 1  : SPRINTF(output, "Scan: cannot open tty port %c",
                                     switches[SERIAL_PORT] + 'A') ;
                    break ;
          case 2  : SPRINTF(output, "Scan: cannot open temporary image file") ;
                    break ;
          case 3  : SPRINTF(output, "Scan: cannot open raster header file") ;
                    break ;
          case 4  : SPRINTF(output, "Scan: scanner not responding; aborting") ;
                    break ;
          case 5  : SPRINTF(output, "Scan: invalid command line argument") ;
                    break ;
          default : if (high >= 100 && high < 200)
                      SPRINTF(output, "Scan: scanner error %1d; aborting",
                                       high - 100) ;
                    else
                      SPRINTF(output, "Scan: terminated with signal %1d",
                                       high - 200) ;
        }
    }    
  make_display(output) ;
  make_button(SCAN_WIDTH-150, 4*SWITCH_HEIGHT+20, "Scan",   B_NORMAL) ;
  make_button(SCAN_WIDTH-150, 4*SWITCH_HEIGHT+60, "Cancel", B_NORMAL) ;
  scanning = 0 ;
  return (NOTIFY_DONE) ;
}

wait_on_child(pid)      /* Wait on child scan process. */
int pid ;
{
  NOTIFY_SET_WAIT3_FUNC(base_frame, wait_child, pid) ;
}
