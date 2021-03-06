/*
 *	(c) Copyright 1991 Sun Microsystems, Inc.  All rights reserved.
 *	See LEGAL_NOTICE file for terms and restrictions.
 */

#ifndef DRAWOBJ_DEFINED
#define DRAWOBJ_DEFINED

/* @(#) drawobj.h 1.42 92/10/30  */

#include <xview/generic.h>

/*
 * types 
 */
typedef Xv_opaque Drawobj;
typedef Xv_opaque Drawrect;
typedef Xv_opaque Drawline;
typedef Xv_opaque Drawimage;
typedef Xv_opaque Drawtext;
typedef Xv_opaque Drawicon;
typedef Xv_opaque Drawarea;
typedef Xv_opaque Tacho;
typedef Xv_opaque Clockobj;


/*
 * packages
 */
extern Xv_pkg   	drawrect_pkg;
#define DRAWRECT	&drawrect_pkg

extern Xv_pkg   	drawline_pkg;
#define DRAWLINE	&drawline_pkg

extern Xv_pkg   	drawimage_pkg;
#define DRAWIMAGE	&drawimage_pkg

extern Xv_pkg   	drawtext_pkg;
#define DRAWTEXT	&drawtext_pkg

extern Xv_pkg   	drawicon_pkg;
#define DRAWICON	&drawicon_pkg

extern Xv_pkg   	drawarea_pkg;
#define DRAWAREA	&drawarea_pkg

extern Xv_pkg		tacho_pkg;
#define TACHO		&tacho_pkg

extern Xv_pkg		clockobj_pkg;
#define CLOCKOBJ	&clockobj_pkg


/*
 * attributes
 */
#define ATTR_DRAWOBJ      ATTR_PKG_UNUSED_LAST-14
#define DRAWOBJ_ATTR(type, ordinal)       ATTR(ATTR_DRAWOBJ, type, ordinal)

typedef enum {

	DRAWOBJ_FILLED		= DRAWOBJ_ATTR(ATTR_BOOLEAN, 		1),
	DRAWRECT_BORDER1	= DRAWOBJ_ATTR(ATTR_INT, 		2),
	DRAWRECT_BORDER2	= DRAWOBJ_ATTR(ATTR_INT, 		3),
	DRAWRECT_BORDER3	= DRAWOBJ_ATTR(ATTR_INT, 		4),

	DRAWLINE_X0		= DRAWOBJ_ATTR(ATTR_INT, 		6),
	DRAWLINE_Y0		= DRAWOBJ_ATTR(ATTR_INT, 		7),
	DRAWLINE_X1		= DRAWOBJ_ATTR(ATTR_INT, 		8),
	DRAWLINE_Y1		= DRAWOBJ_ATTR(ATTR_INT, 		9),
	DRAWLINE_X		= DRAWOBJ_ATTR(ATTR_INT_PAIR, 		10),
	DRAWLINE_Y		= DRAWOBJ_ATTR(ATTR_INT_PAIR, 		11),
	DRAWLINE_ARROW_STYLE	= DRAWOBJ_ATTR(ATTR_OPAQUE_PAIR, 	13),
	DRAWLINE_ARROW_ANGLE	= DRAWOBJ_ATTR(ATTR_INT_PAIR,		14),
	DRAWLINE_ARROW_LENGTH	= DRAWOBJ_ATTR(ATTR_INT_PAIR, 		15),
	DRAWLINE_ARROW_INSET_LENGTH = DRAWOBJ_ATTR(ATTR_INT_PAIR, 	16),

	DRAWIMAGE_IMAGE1	= DRAWOBJ_ATTR(ATTR_OPAQUE, 		17),
	DRAWIMAGE_IMAGE1_MASK	= DRAWOBJ_ATTR(ATTR_OPAQUE, 		18),
	DRAWIMAGE_IMAGE2	= DRAWOBJ_ATTR(ATTR_OPAQUE, 		19),
	DRAWIMAGE_IMAGE2_MASK	= DRAWOBJ_ATTR(ATTR_OPAQUE, 		20),

	DRAWTEXT_STRING		= DRAWOBJ_ATTR(ATTR_STRING, 		30),
	DRAWTEXT_STRING_PTR	= DRAWOBJ_ATTR(ATTR_STRING, 		31),
	DRAWTEXT_FONT		= DRAWOBJ_ATTR(ATTR_OPAQUE, 		32),
	DRAWTEXT_JUSTIFY	= DRAWOBJ_ATTR(ATTR_ENUM, 		33),
	DRAWTEXT_EDITABLE	= DRAWOBJ_ATTR(ATTR_BOOLEAN,		34),
	DRAWTEXT_SHOW_UNDERLINE = DRAWOBJ_ATTR(ATTR_BOOLEAN,		35),
	DRAWTEXT_LENGTH		= DRAWOBJ_ATTR(ATTR_INT,		36),

	DRAWICON_TEXT		= DRAWOBJ_ATTR(ATTR_OPAQUE,		40),
	DRAWICON_IMAGE		= DRAWOBJ_ATTR(ATTR_OPAQUE,		41),
	DRAWICON_IMAGE_COLOR	= DRAWOBJ_ATTR(ATTR_INT, 		42),
	DRAWICON_TEXT_COLOR	= DRAWOBJ_ATTR(ATTR_INT, 		43),
	DRAWICON_GAP		= DRAWOBJ_ATTR(ATTR_INT, 		44),
	DRAWICON_LAYOUT_VERTICAL= DRAWOBJ_ATTR(ATTR_BOOLEAN,		45),

	DRAWAREA_LEFT_X		= DRAWOBJ_ATTR(ATTR_OPAQUE, 		50),
	DRAWAREA_RIGHT_X	= DRAWOBJ_ATTR(ATTR_OPAQUE, 		51),
	DRAWAREA_UPPER_Y	= DRAWOBJ_ATTR(ATTR_OPAQUE, 		52),
	DRAWAREA_LOWER_Y	= DRAWOBJ_ATTR(ATTR_OPAQUE, 		53),
	DRAWAREA_MIN_ALLOC	= DRAWOBJ_ATTR(ATTR_INT, 		54),
	DRAWAREA_MAP_EVENTS	= DRAWOBJ_ATTR(ATTR_ENUM, 		55),
	DRAWAREA_LAST_MARK_KEY	= DRAWOBJ_ATTR(ATTR_OPAQUE,		56),

	TACHO_VALUE		= DRAWOBJ_ATTR(ATTR_INT,		60),
	TACHO_MIN_VALUE		= DRAWOBJ_ATTR(ATTR_INT, 		61),
	TACHO_MAX_VALUE		= DRAWOBJ_ATTR(ATTR_INT, 		62),

	CLOCKOBJ_HR		= DRAWOBJ_ATTR(ATTR_OPAQUE,		71),
	CLOCKOBJ_MIN		= DRAWOBJ_ATTR(ATTR_OPAQUE, 		72),
	CLOCKOBJ_24_HOUR	= DRAWOBJ_ATTR(ATTR_BOOLEAN, 		75),
	CLOCKOBJ_HR_DRAWAREA	= DRAWOBJ_ATTR(ATTR_INT,		76),
	CLOCKOBJ_MIN_DRAWAREA	= DRAWOBJ_ATTR(ATTR_INT, 		77),
	CLOCKOBJ_MOVABLE	= DRAWOBJ_ATTR(ATTR_BOOLEAN, 		79),
	CLOCKOBJ_MOVE_PROC	= DRAWOBJ_ATTR(ATTR_OPAQUE, 		80),

} Drawobj_attr;

typedef enum {
	ARROW_FILLED,
	ARROW_HOLLOW,
	ARROW_SIMPLE,
	ARROW_NONE
} Arrow_style;

typedef enum {
	DRAWTEXT_JUSTIFY_LEFT,
	DRAWTEXT_JUSTIFY_RIGHT,
	DRAWTEXT_JUSTIFY_CENTER
} Drawtext_justify_style;

typedef enum {
	DRAWAREA_MAP_ANY,
	DRAWAREA_MAP_FIRST,
	DRAWAREA_MAP_LAST,
} Drawarea_map_event_mode;

EXTERN_FUNCTION(Xv_opaque drawtext_start_edit, (Xv_opaque, Drawtext));
EXTERN_FUNCTION(void drawtext_finish_edit, (Drawtext, int));

/* aliases */
#define DRAWIMAGE_IMAGE		 	DRAWIMAGE_IMAGE1
#define DRAWIMAGE_IMAGE_MASK		DRAWIMAGE_IMAGE1_MASK
#define DRAWIMAGE_HIGHLIGHT_IMAGE	DRAWIMAGE_IMAGE2
#define DRAWIMAGE_HIGHLIGHT_IMAGE_MASK	DRAWIMAGE_IMAGE2_MASK

/* 1.0 equivalent names */
#define DRAWIMAGE_SVRIMAGE DRAWIMAGE_IMAGE
#define DRAWOBJ_OPAQUE DRAWOBJ_FILLED
#define	DRAWICON_ICON_COLOR DRAWICON_IMAGE_COLOR
#define	DRAWICON_ICON DRAWICON_IMAGE

#endif

