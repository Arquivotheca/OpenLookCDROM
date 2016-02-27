/* FILE export_stubs.c
 *
 * xplan - project planning tool
 * Copyright (C) 1992 Brian Gaubert, Mark M. Lacey, Richard Malingkas,
 * and Mike Marlow.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License (distributed with this program in the file
 * COPYING) for more details.
 * 
 * If you did not received a copy of the GNU General Public License
 * along with this program, write to the Free Software Foundation,
 * Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 * Since this was a project for a one semester software engineering
 * course, the authors will not be offering support for the product
 * after its release.
 *
 * DESCRIPTION OF CONTENTS
 *
 * export_stubs.c - Notify and event callback function stubs.
 * This file was generated by `gxv' from `export.G'.
 *
 */

#include <stdio.h>
#include <sys/param.h>
#include <sys/types.h>
#include <xview/xview.h>
#include <xview/panel.h>
#include <xview/textsw.h>
#include <xview/xv_xrect.h>
#include "db.h"
#include "xplan.h"
#include "latpert.h"

/* the directory and filename to export LaTeX files to files to */
char export_filename[256];

/* FUNCTION export_exportPopup_done_callback ******************************

   PURPOSE

   Done callback function for `exportPopup'.

   INPUTS

   See XView manual.

   AUTHOR/AUDITOR/TESTER
   
   Author.... Mark M. Lacey, 7 Nov 1992
   Tester.... Mark M. Lacey, 8 Nov 1992

   MODIFICATIONS (most recent to least)

*/
void
export_exportPopup_done_callback(Frame frame)
{
	/* gxv_start_connections DO NOT EDIT THIS SECTION */

	xv_set(Export_exportPopup->exportPopup, FRAME_CMD_PUSHPIN_IN, FALSE, NULL);
	xv_set(Export_exportPopup->exportPopup, XV_SHOW, FALSE, NULL);
	
	/* gxv_end_connections */

}

/* FUNCTION export_apply_callback ******************************

   PURPOSE

   Notify callback function for `applyButton'.

   INPUTS

   See XView manual.

   AUTHOR/AUDITOR/TESTER
   
   Author.... Mark M. Lacey, 7 Nov 1992
   Tester.... Mark M. Lacey, 8 Nov 1992

   MODIFICATIONS (most recent to least)

*/
void
export_apply_callback(Panel_item item, Event *event)
{
	export_exportPopup_objects *ip = (export_exportPopup_objects *) xv_get(item, XV_KEY_DATA, INSTANCE);

	/* copy the directory name from the directory prompt */
	strcpy(export_filename, (char *)
	       xv_get(Export_exportPopup->directory,
		      PANEL_VALUE));
	/* append a slash */
	strcat(export_filename, "/");

	/* append the filename from the filename prompt */
	strcat(export_filename, (char *)xv_get(Export_exportPopup->file,
					       PANEL_VALUE));
	
	/* gxv_start_connections DO NOT EDIT THIS SECTION */

	/* gxv_end_connections */

}