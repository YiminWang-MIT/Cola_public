/*
 *	%W%	KPH	%E%	%D% 
 *
 *
 *      M.Korn
 */
/*
   p0       : primaerer Elektronenimpuls in MeV
   pref     : Referenzimpuls in MeV
   M        : Atomgewicht des Kerns
   phi0_spec: Spektrometerwinkel in Grad
   phi0     : Winkel des Elektrons relativ zur Spektrometermitte
	      (phi0_spec) in mrad
  
   das Resultat dieser Funktion ist von der unkorrigierten Impulsablage
   delta abzuziehen.
  
*/


#ifdef sccs
static char SccsId[] = "%W%\t%G%";
#endif

#define _XOPEN_SOURCE 1
#define _DEFAULT_SOURCE 1
#include <math.h>
#include "utils.h"

double 
kinkor(double p0, double pref, double M, double phi0_spec, double phi0)
{

#define DEG_TO_RAD M_PI/180.0
#define A_MU 931.501

  double p_center, p, phi0_scatter;

/* Grad und mrad  in rad umrechnen, Kernmasse in MeV/c^2 */

  phi0_spec *= DEG_TO_RAD;
  phi0 *= 0.001;
  M *= A_MU;

/*
   Berechne Impuls eines Elektrons f"ur die Spektrometermitte
   und den Impuls des gestreuten Elektrons
*/
  phi0_scatter = phi0_spec+phi0;
  p_center = p0/(1+2*p0/M*pow(sin(phi0_spec*0.5),2));
  p = p0/(1+2*p0/M*pow(sin((phi0_scatter)*0.5),2));

  return ((p-p_center)/pref*100.0);
}


