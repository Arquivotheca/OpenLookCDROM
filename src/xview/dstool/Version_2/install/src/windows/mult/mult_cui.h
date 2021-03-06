/*  -------------------------------------------------------------------

This program is the property of:

                             Cornell University 
                        Center for Applied Mathematics 
                              Ithaca, NY 14853

and may be used, modified and distributed freely, subject to the 
following restrictions:

       Any product which incorporates source code from the dstool
       program or utilities, in whole or in part, is distributed
       with a copy of that source code, including this notice. You
       must give the recipients all the rights that you have with
       respect to the use of this software. Modifications of the
       software must carry prominent notices stating who changed
       the files and the date of any change.

DsTool is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of FITNESS 
FOR A PARTICULAR PURPOSE.  The software is provided as is without 
any obligation on the part of Cornell faculty, staff or students to 
assist in its use, correction, modification or enhancement.

  -----------------------------------------------------------------  */
#ifndef	mult_HEADER
#define	mult_HEADER

/*
 * mult_ui.h - User interface object and function declarations.
 * This file was generated by `gxv' from `mult.G'.
 * DO NOT EDIT BY HAND.
 */

#include <xview/xview.h>
#include <xview/panel.h>

extern Attr_attribute	INSTANCE;



typedef struct {
	Xv_opaque	win;
	Xv_opaque	pan;
	Xv_opaque	forwards;
	Xv_opaque	backwards;
	Xv_opaque	contin;
	Xv_opaque	images;
	Xv_opaque	load;
	Xv_opaque	copy;
	Xv_opaque	info;
	Xv_opaque	loadchoice;
	Xv_opaque	transformation;
	Xv_opaque	transparam;
	Xv_opaque	label;
	Xv_opaque	*radius;
	Xv_opaque	*points;
} mult_win_objects;

extern mult_win_objects	*mult_win_objects_initialize();

extern Xv_opaque	mult_win_win_create();
extern Xv_opaque	mult_win_pan_create();
extern Xv_opaque	mult_win_forwards_create();
extern Xv_opaque	mult_win_backwards_create();
extern Xv_opaque	mult_win_contin_create();
extern Xv_opaque	mult_win_images_create();
extern Xv_opaque	mult_win_load_create();
extern Xv_opaque	mult_win_copy_create();
extern Xv_opaque	mult_win_info_create();
extern Xv_opaque	mult_win_loadchoice_create();
extern Xv_opaque	mult_win_transformation_create();
extern Xv_opaque	mult_win_transparam_create();
extern Xv_opaque	mult_win_label_create();
extern Xv_opaque	mult_win_radius_create();
extern Xv_opaque	mult_win_points_create();
#endif
