#ifndef __tma_utils__
#define __tma_utils__

/*
  kinkor :

   p0       : prim"arer Elektronenimpuls in MeV
   pref     : Referenzimpuls in MeV
   M        : Atomgewicht des Kerns
   phi0_spec: Spektrometerwinkel in Grad
   phi0     : Winkel des Elektrons relativ zur Spektrometermitte
	      (phi0_spec) in mrad
  
   das Resultat dieser Funktion ist von der unkorrigierten Impulsablage
   delta abzuziehen.
  
*/

double kinkor(double p0, double pref, double M, double phi0_spec, double phi0);
#define delta2p(delta, pref) ((1+(delta)*0.01)*(pref))


#endif


