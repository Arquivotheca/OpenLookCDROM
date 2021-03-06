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
#include <model_headers.h>


/* Lorenz system 
        A three dimensional vector field resulting from a 
        truncation of a partial differential equation which
        models two-dimensional fluid convection.  The system
        was first studied by Lorenz in  J. ATMOS. SCI. 63 
        (1963).  The vector field is defined by
                x' = sigma (y -x)
                y' = rho x - y - xz
                z' = -beta z + xy

        There are two auxiliary functions:
        x+y = sum of x and y coordinates
        x-y = difference of x and y coordinates
*/

/* ------------------------------------------------------------------------
   proc used to define the vector field or map
   ------------------------------------------------------------------------ */

int lorenz(f,x,p)
double f[],x[],p[];
{
	f[0] = p[0] * ( x[1] - x[0] );
	f[1] = p[1] * x[0] - x[1] - x[0] * x[2];
	f[2] = - p[2] * x[2] + x[0] * x[1];
	return 0;
}

/* ------------------------------------------------------------------------
   proc used to define jacobian
   ------------------------------------------------------------------------ */
int lorenz_jac(m,x,p)
double  **m, *x, *p;
{
  m[0][0] = -p[0];
  m[0][1] = p[0];
  m[0][2] = 0.0;
  m[1][0] = p[1] - x[2];
  m[1][1] = -1.0;
  m[1][2] = -x[0];
  m[2][0] = x[1];
  m[2][1] = x[0];
  m[2][2] = -p[2];
  return 0;
}


/* ------------------------------------------------------------------------
   proc used to define functions of the variable, parameters or derivatives
   ------------------------------------------------------------------------ */
int lorenz_func(f,x,p)
double f[],x[],p[];
{
	f[0] = x[0] + x[1];
	f[1] = x[0] - x[1];
	return 0;
}



/* ------------------------------------------------------------------------
   proc to define the default data for the dynamical system
   Note: You may change the entries for each variable but
	 DO NOT change the list of items.  If a variable is
	 unused, NULL or zero the entry, as appropriate.
   ------------------------------------------------------------------------ */
int lorenz_init()
{

  /* define the dynamical system in this segment 
     -------------------------------------------------------------------------------------------- */
  int	        n_varb=3;				       /* dim of the phase spase */
  static double	variables[]={0.1,0.1,0.1};		       /* initial conditions for the variables */
  static double	variable_min[]={-30.,-30.,-5.};		       /* default varb min for display */
  static double	variable_max[]={30.,30.,50.};		       /* default varb max for display */
  double        indep_varb_min=0.;			       /* default indep varb min for display */
  double        indep_varb_max=100.;			       /* default indep varb max for display */
  static char	*variable_names[]={"x","y","z"};	       /* list of phase varb names */
  static char	*indep_varb_name="time";		       /* name of indep variable */

  int	        n_param=3;				       /* dim of the parameter space */
  static double	parameters[]={10.0,28.0,2.66666666};	       /* initial parameter values */
  static double	parameter_min[]={-20.0,0.,-5.};		       /* default param min for display */
  static double	parameter_max[]={20.,40.,5.};		       /* default param max for display */
  static char	*parameter_names[]={"sigma","rho","beta"};     /* list of param names */

  int	        n_funct=2;				       /* number of user-defined functions */
  static double	funct_min[]={-60.,-60.};		       /* default funct min for display */
  static double	funct_max[]={60.,60.};			       /* default funct max for display */
  static char	*funct_names[]={"x+y","x-y"};		       /* list of funct names */

  int	        manifold_type = EUCLIDEAN;		       /* PERIODIC (a periodic varb) or EUCLIDEAN */
  static int	periodic_varb[] = {FALSE, FALSE, FALSE};       /* if PERIODIC, which variables are periodic */
  static double period_start[] = {0.,0.,0.};		       /* if PERIODIC, beginning of fundamental domain */
  static double period_end[] = {0., 0., 0.};		       /* if PERIODIC, end of fundamental domain */
	
  int	        mapping_toggle=FALSE;			       /* is this a map? */
  int           inverse_toggle=FALSE;			       /* if so, is inverse FALSE, APPROX_INV, */
                                                               /* or EXPLICIT_INV? FALSE for vec field */

  /*  In this section, input NULL or the name of the function which contains... */
  int           (*def_name)()=lorenz;			       /* the eqns of motion */
  int           (*jac_name)()=lorenz_jac;		       /* the jacobian (deriv w.r.t. space) */
  int           (*aux_func_name)()=lorenz_func;		       /* the auxiliary functions */
  int           (*inv_name)()=NULL;			       /* the inverse or approx inverse */
  int           (*dfdt_name)()=NULL;			       /* the deriv w.r.t time */
  int           (*dfdparam_name)()=NULL;		       /* the derivs w.r.t. parameters */

  /* end of dynamical system definition 
     -------------------------------------------------------------------------------------------------- */

#include <ds_define.c>

}

