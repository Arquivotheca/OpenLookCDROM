/* FILE latpert.h *****************************************************
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
 * FILE latpert.h */

#ifndef _latpert_h_
#define _latpert_h_

#define PERT_BOXES_ACROSS 2
#define PERT_BOXES_DOWN   4

#define LATEX_PERT_BOX_WIDTH  150
#define LATEX_PERT_BOX_HEIGHT 100

#define LATEX_PERT_BOX_X_SKIP 75
#define LATEX_PERT_BOX_Y_SKIP 50

#define PAGE_HEIGHT 520
#define PAGE_WIDTH  320

void generate_latex_pert_pages(void);

#endif

