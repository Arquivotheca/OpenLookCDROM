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
#include <malloc.h>
#include <stdio.h>

/*
 * dmatrix()
 *
 * memory allocation for a matrix of doubles
 * returns m so that m[nrl][ncl],...,m[nrh][nch] are the valid locations
 *
 * last modified:  8/21/91  paw
 */

double
  **dmatrix(nrl,nrh,ncl,nch)
int nrl,nrh,ncl,nch;
{
  int i, n_cols, n_rows, total_pts;
  double **m;
  int free_dmatrix();

  n_cols = nch-ncl+1;
  n_rows = nrh-nrl+1;
  if (n_rows<=0 || n_cols<=0) return(NULL);
  total_pts = n_cols*n_rows;
  
  if ( (m = (double **) malloc( (unsigned) (n_rows * sizeof(double *)))) == NULL)
    {
      system_mess_proc(1,"dmatrix: memory allocation failure!");
    }
  else
    {
      if ( (m[0] = (double *) malloc( (unsigned) (total_pts * sizeof(double)))) == NULL)
	{
	  free(m);
	  m = NULL;
	  system_mess_proc(1,"dmatrix: memory allocation failure!");
	}
      else
	{
	  m[0] = m[0] - ncl;
	  for (i=1; i<n_rows; i++) m[i] = m[i-1] + n_cols;
	  m = m-nrl;
	}
    }
  return(m);
}



/*
 * free_dmatrix()
 *
 * frees memory allocated by dmatrix()
 *
 * last modified: 8/21/91  paw
 */
int
  free_dmatrix(m,nrl,nrh,ncl,nch)
double **m;
int nrl,nrh,ncl,nch;
{
  if (m==NULL) return;
  free( (char *) (m[nrl] + ncl) );
  free( (char *) (m + nrl) );
}



