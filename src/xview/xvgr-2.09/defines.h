/* $Id: defines.h,v 1.20 92/07/26 19:21:34 pturner Exp Locker: pturner $
 *
 * definitions of objects and sets
 *
 */

/*
 * defines from pars.y
 */

#ifndef PARS
#include "pars.h"
#endif

/*
 * some constants
 *
 */
#define MAX_SET_COLS 6		/* max number of data columns for a set */
#define MAXPLOT 30		/* max number of sets in a graph */
#define MAXGRAPH 10		/* max number of graphs */
#define MAX_ZOOM_STACK 20	/* max stack depth for world stack */
#define MAXREGION 5		/* max number of regions */
#define MAX_TICK_LABELS 35	/* max number of user defined ticks/labels */
#define MAX_STRING_LENGTH 256	/* max length for strings */
#define MAXBOXES 50		/* max number of boxes */
#define MAXLINES 50		/* max number of lines */
#define MAXSTR 100		/* max number of strings */
#define MAXSTRLEN 140
#define MAXSYM 26		/* max number of symbols */
#define MAX_LINESTYLE 5		/* max number of linestyles */
#define MAX_LINEWIDTH 10	/* max number of line widths */
#define MAXCOLORS 16		/* max number of colors */

#ifndef M_PI
#       define M_PI  3.14159265358979323846
#endif

#define BIG 1.0e+307
#define MBIG -1.0e+307

/*
 * types of locating for objects
 */
#define LOCWORLD 0
#define LOCVIEW 1

#ifndef MAXARR
#	define MAXARR 20000	/* max elements in an array */
#endif

#define MAXFIT 12		/* max degree of polynomial+1 that can be
				 * fitted */

#define HISTOSYM 19		/* plotsymbol for histograms */

/*
 * dataset types
 */
#define DATASET_XY 0
#define DATASET_XY_NXY 1
#define DATASET_XY_IHL 2
#define DATASET_XY_BIN 3
#define DATASET_XY_DX 4
#define DATASET_XY_DY 5
#define DATASET_XY_DXDX 6
#define DATASET_XY_DYDY 7
#define DATASET_XY_Z 8
#define DATASET_XY_ZW 9
#define DATASET_XY_UV 10
#define DATASET_XY_RT 11
#define DATASET_XY_X2Y2 12
#define DATASET_XY_SEG 13
#define DATASET_XY_BOX 14
#define DATASET_XY_ARC 15
#define DATASET_XY_STRING 16
#define DATASET_XY_YY 17
#define DATASET_XY_XX 18
#define DATASET_XY_HILO 19
#define DATASET_MISSING (1.23456789e+307)

/*
 * types of tick displays
 */
#define X_AXIS 0
#define Y_AXIS 1
#define ZX_AXIS 2
#define ZY_AXIS 3
#define XA_AXIS 4
#define YA_AXIS 5
#define POLAR_RADIUS 6
#define POLAR_ANGLE 7

/*
 * grid types
 */
#define GRID_Z 0
#define GRID_UV 1
#define GRID_ZUV 2
#define GRID_ZUVW 3
#define GRID_ZW 4

/* graphics output to the following */
#define GR_X	 	0
#define GR_PS_L 	1	/* PostScript landscape */
#define GR_PS_P 	2	/* PostScript portrait */
#define GR_MIF_L 	3	/* mif landscape */
#define GR_MIF_P 	4	/* mif portrait */
#define GR_HPGL_L 	5	/* hpgl landscape */
#define GR_HPGL_P 	6	/* hpgl portrait */
#define GR_LEAF_L 	7	/* InterLeaf landscape */
#define GR_LEAF_P 	8	/* InterLeaf portrait */
#define GR_HPGLB_L 	9	/* hpgl 11x17 landscape */
#define GR_HPGLB_P 	10	/* hpgl 11x17 portrait */
#define GR_XWD 		11	/* X window dump */

/* set HDEV to the default hardcopy device */
#ifndef HDEV
#	define HDEV GR_PS_L
#endif

/* TDEV is always X */
#define TDEV GR_X

#ifndef TRUE
#	define TRUE 1
#endif
#ifndef FALSE
#	define FALSE 0
#endif

/* 
 * for define_select_set()
 */
#define PANEL_KEY 1

/* for canvas event proc */
#define SELECT_REGION 256
#define RUBBER_BAND 512
#define ZOOM_1ST 3
#define ZOOM_2ND 4
#define VIEW_1ST 5
#define VIEW_2ND 6
#define STR_LOC 7
#define LEG_LOC 8
#define FIND_POINT 9
#define DEL_POINT 10
#define MOVE_POINT1ST 11
#define MOVE_POINT2ND 12
#define ADD_POINT 13
#define DEL_OBJECT 14
#define MOVE_OBJECT_1ST 15
#define MOVE_OBJECT_2ND 16
#define MAKE_BOX_1ST 17
#define MAKE_BOX_2ND 18
#define MAKE_LINE_1ST 19
#define MAKE_LINE_2ND 20
#define SEL_POINT 21
#define STR_EDIT 22
#define COMP_AREA 23
#define COMP_PERIMETER 24
#define STR_LOC1ST 25
#define STR_LOC2ND 26
#define GRAPH_FOCUS 28
#define TRACKER 29
#define INTERP_GRID 30
#define FIND_GRID 31
#define FIND_GRIDPT 32
#define FIND_GRIDRECT 33
#define SLICE_GRIDDIST 34
#define SLICE_GRIDX 35
#define SLICE_GRIDY 36
#define SLICE_GRIDLINE_1ST 37
#define SLICE_GRIDLINE_2ND 38
#define SLICE_GRIDPATH 39
#define DEF_REGION 27
#define DEF_REGION1ST 40
#define DEF_REGION2ND 41
#define PAINT_POINTS 42
#define KILL_NEAREST 43
#define COPY_NEAREST1ST 44
#define COPY_NEAREST2ND 45
#define MOVE_NEAREST1ST 46
#define MOVE_NEAREST2ND 47
#define REVERSE_NEAREST 48
#define JOIN_NEAREST1ST 49
#define JOIN_NEAREST2ND 50
#define DEACTIVATE_NEAREST 51
#define EXTRACT_NEAREST1ST 52
#define EXTRACT_NEAREST2ND 53
#define DELETE_NEAREST1ST 54
#define DELETE_NEAREST2ND 55
#define INSERT_POINTS 56
#define INSERT_SET 57
#define EDIT_OBJECT 58
#define PLACE_TIMESTAMP 59

#define FREE 0
#define LANDSCAPE 1
#define PORTRAIT 2

/*
 * symbol table entry type
 */
typedef struct {
    char *s;
    int type;
} symtab_entry;

/*
 * typedefs for objects
 */
typedef struct {
    int active;
    int loctype;
    int gno;
    double x1, y1, x2, y2;
    int lines;
    int linew;
    int color;
    int fill;
    int fillcolor;
    int fillpattern;
} boxtype;

typedef struct {
    int active;
    int loctype;
    int gno;
    double x1, y1, x2, y2;
    int lines;
    int linew;
    int color;
    int arrow;
    int atype;
    double asize;
} linetype;

typedef struct {
    int active;
    int loctype;
    int gno;
    double x, y;
    int lines;
    int linew;
    int color;
    int rot;
    int font;
    int just;
    double charsize;
    char s[MAXSTRLEN + 1];
} plotstr;

typedef struct {
    int color;
    int lines;
    int linew;
    double charsize;
    int font;
    int fontsrc;
    double symsize;
} defaults;

typedef struct {
    double xg1, xg2, yg1, yg2;	/* window into world coords */
} world;

typedef struct {
    double xv1, xv2, yv1, yv2;	/* device viewport */
} view;

/*
 * world stack
 */
typedef struct {
    world w;                    /* current world */
    world t[3];                 /* current tick spacing */
} world_stack;

typedef struct {
    plotstr title;		/* graph title */
    plotstr stitle;		/* graph subtitle */
} labels;

typedef struct {
    int active;			/* active flag */
    int type;			/* dataset type */
    int deact;			/* deactivated set */
    double missing;		/* value for missing data */
    double *ex[MAX_SET_COLS];	/* x, y, dx, z, r, hi depending on dataset type */
    char **s;			/* pointer to strings */
    int font;			/* font for strings */
    int format;			/* format for drawing values */
    int prec;			/* precision for drawing values */
    double xmin, xmax;		/* min max for x */
    double ymin, ymax;		/* min max for y */
    int len;			/* set length */
    int sym;			/* set plot symbol */
    char symchar;		/* character for symbol */
    int symskip;		/* How many symbols to skip */
    int symfill;		/* Symbol fill type */
    int symdot;			/* Symbol dot in center */
    double symsize;		/* size of symbols */
    int lines;			/* set line style */
    int linew;			/* line width */
    int color;			/* color for symbol and linestyle */
    int fill;			/* fill type */
    int fillusing;		/* fill using color or pattern */
    int fillcolor;		/* fill color */
    int fillpattern;		/* fill pattern */
    int errbar;			/* if type is _DX, _DY, _DXDY and errbar = TRUE */
    int errbarxy;		/* type of error bar */
    int errbar_linew;		/* error bar line width */
    int errbar_lines;		/* error bar line style */
    int errbar_riser;		/* connecting line between error limits */
    int errbar_riser_linew;	/* connecting line between error limits line width */
    int errbar_riser_lines;	/* connecting line between error limits line style */
    double errbarper;		/* length of error bar */
    double hilowper;		/* length of hi-low */
    int density_plot;		/* if type is XYZ then density_plot  = 1 */
    double zmin, zmax;		/* min max for density plots */
    char comments[256];		/* how did this set originate */
} plotarr;

typedef struct {
    int axis;			/* which axis */
    int active;			/* active or not */
    int alt;			/* alternate map if TRUE */
    double tmin, tmax;		/* mapping for alternate tickmarks */
    double tmajor, tminor;	/* major, minor tick divisions */
    double offsx, offsy;	/* offset of axes in viewport coords */
    plotstr label;		/* graph axis label */
    int label_layout;		/* axis label orientation (h or v) */
    int label_place;		/* axis label placement (specfied or auto) */
    int tl_flag;		/* toggle ticmark labels on or off */
    int tl_type;		/* either auto or specified (below) */
    int tl_layout;		/* horizontal, vertical, or specified */
    int tl_angle;		/* angle to draw labels if layout is specified */
    int tl_sign;		/* tick labels normal, absolute value, or negate */
    int tl_just;		/* justification of ticklabel and type of anchor point */
    int tl_prec;		/* places to right of decimal point */
    int tl_format;		/* decimal or exponential ticmark labels .. */
    int tl_skip;		/* tick labels to skip */
    int tl_staggered;		/* tick labels staggered */
    int tl_starttype;		/* start at graphmin or use tl_start/stop */
    int tl_stoptype;		/* start at graphmax or use tl_start/stop */
    double tl_start;		/* value of x to begin tick labels and major ticks */
    double tl_stop;		/* value of x to begin tick labels and major ticks */
    int tl_op;			/* tick labels on opposite side or both */
    double tl_vgap;		/* tick label to tickmark distance vertically */
    double tl_hgap;		/* tick label to tickmark distance horizontally */
    int tl_font;		/* font to use for labels */
    double tl_charsize;		/* character size for labels */
    int tl_color;		/* color */
    int tl_linew;		/* line width for labels */
    char tl_appstr[256];	/* append string to tick label */
    char tl_prestr[256];	/* prepend string to tick label */
    int t_type;			/* type of tickmarks, usual, xticstart, or specified */
    int t_flag;			/* toggle tickmark display */
    int t_mflag;		/* toggle minor tickmark display */
    int t_integer;		/* major tic marks on integer divisions */
    int t_num;			/* approximate default number of X-axis ticks */
    int t_inout;		/* ticks inward, outward or both */
    int t_log;			/* logarithmic ticmarks */
    int t_op;			/* ticks on opposite side */
    int t_color;		/* colors and linestyles */
    int t_lines;
    int t_linew;
    int t_mcolor;
    int t_mlines;
    int t_mlinew;		/* minor grid colors and linestyles */
    double t_size;		/* length of tickmarks */
    double t_msize;		/* length of minor tickmarks */
    int t_drawbar;		/* draw a bar connecting tick marks */
    int t_drawbarcolor;		/* color of bar */
    int t_drawbarlines;		/* linestyle of bar */
    int t_drawbarlinew;		/* line width of bar */
    int t_gridflag;		/* grid lines at major tick marks */
    int t_mgridflag;		/* grid lines at minor tick marks */
    int t_spec;			/* number of ticks at specified locations */
    double t_specloc[MAX_TICK_LABELS];
    plotstr t_speclab[MAX_TICK_LABELS];
    int spec_font;
    double spec_charsize;
    int spec_color;
    int spec_linew;
} tickmarks;

typedef struct {
    int axis;			/* 0 = x, 1 = y */
} polartickmarks;

typedef struct {
    int active;			/* legend on or off */
    int loctype;		/* locate in world or viewport coords */
    int layout;			/* verticle or horizontal */
    int vgap;			/* verticle gap between entries */
    int hgap;			/* horizontal gap between entries */
    int len;			/* length of line to draw */
    int box;			/* box around legend on or off */
    double legx;		/* location on graph */
    double legy;
    int font;
    double charsize;
    int color;
    int linew;
    int lines;
    int boxfill;		/* legend frame fill toggle */
    int boxfillusing;		/* legend frame fill type */
    int boxfillcolor;		/* legend frame fill color */
    int boxfillpat;		/* legend frame fill pattern */
    int boxlcolor;		/* legend frame line color */
    int boxlinew;		/* legend frame line width */
    int boxlines;		/* legend frame line style */
    plotstr str[MAXPLOT];	/* legend for each of MAXPLOT sets */
} legend;

typedef struct {
    int active;			/* region on or off */
    int type;			/* region type */
    int color;			/* region color */
    int lines;			/* region linestyle */
    int linew;			/* region line width */
    int linkto[MAXGRAPH];	/* associated with graphs in linkto */
    int n;			/* number of points if type is POLY */
    double *x, *y;		/* coordinates if type is POLY */
    double x1, y1, x2, y2;	/* starting and ending points if type is not POLY */
} region;

typedef struct {
    int active;			/* frame on or off */
    int type;			/* frame type */
    int color;			/* frame color */
    int lines;			/* frame linestyle */
    int linew;			/* frame line width */
    int fillbg;			/* fill background */
    int bgcolor;		/* background color inside frame */
} framep;

/*
 * a graph
 */
typedef struct {
    int active;			/* alive or dead */
    int hidden;			/* display or not */
    int label;			/* label graph */
    int type;			/* type of graph */
    int noauto_world;		/* only time this is used is at startup */
    int noauto_tics;		/* only time this is used is at startup */
    int auto_type;		/* */
    int parmsread;		/* was a paramter file read for this graph */
    int revx, revy;		/* reverse mapping for x and y if true */
    int maxplot;		/* max number of sets for this graph */
    plotarr *p;			/* sets go here */
    legend l;			/* legends */
    defaults d;			/* defaults */
    world w;			/* world */
    view v;			/* world/view */
    labels labs;		/* title, subtitle, axes labels */
    tickmarks t[6];		/* flags etc. for tickmarks for all axes */
    framep f;			/* type of box around plot */
    int pointset;		/* if (dsx, dsy) have been set */
    int pt_type;		/* type of locator display */
    double dsx, dsy;		/* locator fixed point */
    int fx, fy;			/* locator format type */
    int px, py;			/* locator precision */
    world_stack ws[MAX_ZOOM_STACK]; /* zoom stack */
    int ws_top;			/* stack pointer */
    int curw;			/* for cycling through the stack */
} graph;

#define copyx(gno, setfrom, setto)	copycol(gno, setfrom, setto, 0)
#define copyy(gno, setfrom, setto)	copycol(gno, setfrom, setto, 1)
#define getsetlength(gno, set)	(g[gno].p[set].len)
#define getx(gno, set)		((double *) g[gno].p[set].ex[0])
#define gety(gno, set)		((double *) g[gno].p[set].ex[1])
#define getcol(gno, set, col)	((double *) g[gno].p[set].ex[col])
#define getcomment(gno, set)	((char *) g[gno].p[set].comments)
#define getsetlines(gno, set)	(g[gno].p[set].lines)
#define getsetlinew(gno, set)	(g[gno].p[set].linew)
#define getsetcolor(gno, set)	(g[gno].p[set].color)
#define getsetplotsym(gno, set)	(g[gno].p[set].sym)
#define getsetplotsymsize(gno, set)	(g[gno].p[set].symsize)
#define getsetplotsymchar(gno, set)	(g[gno].p[set].symchar)
#define getsetplotsymskip(gno, set)	(g[gno].p[set].symskip)
#define dataset_type(gno, set)	(g[gno].p[set].type)
#define graph_type(gno)	(g[gno].type)
#define setcomment(gno, set, s)	(strcpy(g[gno].p[set].comments, s))
#define settype(gno, i, it)  (g[gno].p[i].type = it)
#define setplotsym(gno, i, hsym)  (g[gno].p[i].sym = hsym)
#define setplotsymsize(gno, i, hsym)  (g[gno].p[i].symsize = hsym)
#define setplotsymskip(gno, i, hsym)  (g[gno].p[i].symskip = hsym)
#define setplotsymchar(gno, i, hsym)  (g[gno].p[i].symchar = hsym)
#define setplotlines(gno, i, line)  (g[gno].p[i].lines = line)
#define setplotlinew(gno, i, wid)  (g[gno].p[i].linew = wid)
#define setplotcolor(gno, i, pen)  (g[gno].p[i].color = pen)
#define isactive(gno, set)	(g[gno].p[set].active == ON)
#define isactive_set(gno, set)	(g[gno].p[set].active == ON)
#define isactive_graph(gno)	(g[gno].active == ON)
#define ishidden_graph(gno)	(g[gno].hidden)
#define on_or_off(x) ((x == ON)?"on":"off")
#define yes_or_no(x) ((x)?"yes":"no")
#define w_or_v(x) ((x == WORLD)?"world":"view")

#define HISTOSYM 19		/* plotsymbol for histograms */

#ifdef MOTIF

#define define_select_set_panel0(panel,panel_item,panelname,x,y) \
                                panel_item=CreatePanelChoice0( panel,\
                                x,\
                                y,\
                                panelname,\
                                3,\
                                31,\
                        "0", "1", "2", "3", "4", "5",\
                        "6", "7", "8", "9", "10", "11",\
                        "12", "13", "14",\
			"15", "16", "17", "18", "19",\
			"20", "21", "22", "23", "24",\
			"25", "26", "27", "28", "29",\
			0, 0 );

#define define_select_set_panel1(panel,panel_item,panelname,x,y) \
                                panel_item=CreatePanelChoice0( panel,\
                                x,\
                                y,\
                                panelname,\
                                3,\
                                32,\
                        "0", "1", "2", "3", "4", "5",\
                        "6", "7", "8", "9", "10", "11",\
                        "12", "13", "14",\
			"15", "16", "17", "18", "19",\
			"20", "21", "22", "23", "24",\
			"25", "26", "27", "28", "29",\
			"All", 0, 0);

#define define_select_set_panel2(panel,panel_item,s) \
                                panel_item=CreatePanelChoice1( panel,\
                                s,\
                                3,\
                                31,\
                        "0", "1", "2", "3", "4", "5",\
                        "6", "7", "8", "9", "10", "11",\
                        "12", "13", "14",\
			"15", "16", "17", "18", "19",\
			"20", "21", "22", "23", "24",\
			"25", "26", "27", "28", "29",\
			0, 0 );

#define define_select_set_panel3(panel,panel_item) \
                                panel_item=CreatePanelChoice1( panel,\
                                "Select set:",\
                                3,\
                                32,\
                        "0", "1", "2", "3", "4", "5",\
                        "6", "7", "8", "9", "10", "11",\
                        "12", "13", "14",\
			"15", "16", "17", "18", "19",\
			"20", "21", "22", "23", "24",\
			"25", "26", "27", "28", "29",\
			"All", 0, 0 );

#define define_select_set_panel4(panel,panel_item,panelname,x,y) \
                                panel_item=CreatePanelChoice0( panel,\
                                x,\
                                y,\
                                panelname,\
                                3,\
                                32,\
                        "Next", "0", "1", "2", "3", "4", "5",\
                        "6", "7", "8", "9", "10", "11",\
                        "12", "13", "14",\
			"15", "16", "17", "18", "19",\
			"20", "21", "22", "23", "24",\
			"25", "26", "27", "28", "29",\
			0, 0 );

#define define_select_set_panel5(panel,panel_item,panelname,x,y) \
                                panel_item=CreatePanelChoice0( panel,\
                                x,\
                                y,\
                                panelname,\
                                3,\
                                33,\
                        "Next", "Same", "0", "1", "2", "3", "4", "5",\
                        "6", "7", "8", "9", "10", "11",\
                        "12", "13", "14",\
			"15", "16", "17", "18", "19",\
			"20", "21", "22", "23", "24",\
			"25", "26", "27", "28", "29",\
			0, 0 );
#endif

#ifdef XVIEW
#define define_select_set_panel0(panel,panel_item,panelname,x,y) \
                                panel_item=xv_create( panel,\
                                PANEL_CYCLE,\
				PANEL_CHOICE_NCOLS, 3,\
                                XV_X,x,\
                                XV_Y,y,\
                                PANEL_LABEL_STRING,panelname,\
                                PANEL_CHOICE_STRINGS,\
                        "0", "1", "2", "3", "4", "5",\
                        "6", "7", "8", "9", "10", "11",\
                        "12", "13", "14",\
			"15", "16", "17", "18", "19",\
			"20", "21", "22", "23", "24",\
			"25", "26", "27", "28", "29",NULL,\
                        NULL);

#define define_select_set_panel1(panel,panel_item,panelname,x,y) \
                                panel_item=xv_create( panel,\
                                PANEL_CYCLE,\
				PANEL_CHOICE_NCOLS, 3,\
                                XV_X,x,\
                                XV_Y,y,\
                                PANEL_LABEL_STRING,panelname,\
                                PANEL_CHOICE_STRINGS,\
                        "0", "1", "2", "3", "4", "5",\
                        "6", "7", "8", "9", "10", "11",\
                        "12", "13", "14",\
			"15", "16", "17", "18", "19",\
			"20", "21", "22", "23", "24",\
			"25", "26", "27", "28", "29","All",NULL,\
                        NULL);

#define define_select_set_panel2(panel,panel_item,s) \
                                panel_item=xv_create( panel,\
                                PANEL_CYCLE,\
				PANEL_CHOICE_NCOLS, 3,\
                                PANEL_LABEL_STRING,s,\
                                PANEL_CHOICE_STRINGS,\
                        "0", "1", "2", "3", "4", "5",\
                        "6", "7", "8", "9", "10", "11",\
                        "12", "13", "14",\
			"15", "16", "17", "18", "19",\
			"20", "21", "22", "23", "24",\
			"25", "26", "27", "28", "29", NULL,\
                        0 );

#define define_select_set_panel3(panel,panel_item) \
                                panel_item=xv_create( panel,\
                                PANEL_CYCLE,\
				PANEL_CHOICE_NCOLS, 3,\
                                PANEL_LABEL_STRING,"Select set:",\
                                PANEL_CHOICE_STRINGS,\
                        "0", "1", "2", "3", "4", "5",\
                        "6", "7", "8", "9", "10", "11",\
                        "12", "13", "14",\
			"15", "16", "17", "18", "19",\
			"20", "21", "22", "23", "24",\
			"25", "26", "27", "28", "29", "All", NULL,\
                        0 );

#define define_select_set_panel4(panel,panel_item,panelname,x,y) \
                                panel_item=xv_create( panel,\
                                PANEL_CYCLE,\
				PANEL_CHOICE_NCOLS, 3,\
                                XV_X,x,\
                                XV_Y,y,\
                                PANEL_LABEL_STRING,panelname,\
                                PANEL_CHOICE_STRINGS,\
                        "Next", "0", "1", "2", "3", "4", "5",\
                        "6", "7", "8", "9", "10", "11",\
                        "12", "13", "14",\
			"15", "16", "17", "18", "19",\
			"20", "21", "22", "23", "24",\
			"25", "26", "27", "28", "29",NULL,\
                        NULL);

#define define_select_set_panel5(panel,panel_item,panelname,x,y) \
                                panel_item=xv_create( panel,\
                                PANEL_CYCLE,\
				PANEL_CHOICE_NCOLS, 3,\
                                XV_X,x,\
                                XV_Y,y,\
                                PANEL_LABEL_STRING,panelname,\
                                PANEL_CHOICE_STRINGS,\
                        "Next", "Same", "0", "1", "2", "3", "4", "5",\
                        "6", "7", "8", "9", "10", "11",\
                        "12", "13", "14",\
			"15", "16", "17", "18", "19",\
			"20", "21", "22", "23", "24",\
			"25", "26", "27", "28", "29",NULL,\
                        NULL);

#endif
