//								      -*-c++-*-
//
// $Id: fitspline.h 2216 2008-06-13 21:13:47Z distler $
//

#ifndef _FITSPLINE_H
#define _FITSPLINE_H

const int NDAT    = (getenv("NDAT") ? atoi(getenv("NDAT")) : 20000);
const int SAMPLES = (getenv("SAMPLES") ? atoi(getenv("SAMPLES")) : 7);

const int MAX_ITERATION_NR = (getenv("MAX_ITERATION_NR") 
			      ? atoi(getenv("MAX_ITERATION_NR")) : 10);
const double DDRSCALE  = (getenv("DDRSCALE") 
			  ? (0.05 * atoi(getenv("DDRSCALE")))  : 0.05);
const double TIMESCALE = (getenv("TIMESCALE") 
			  ?  (0.5 * atoi(getenv("TIMESCALE"))) : 0.0);

double chisqrpdc(double parameter[]);
double fitSplineData(int nr_d_points,
	   double d_point[], double d_value[], double d_error[],
	   int nr_param, double (* func)(double x, double parameter[]),
	   double param[], double scale[], double acure);

#endif
