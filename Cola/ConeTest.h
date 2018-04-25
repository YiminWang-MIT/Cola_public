// ConeTest.cc, V 1.0: T.Pospischil, 7.4.98
//

#include "online.h"

/// --------------------------------------------------------- ConeTest:
/** 
   This rountine gets its input data from the data structure online, refering 
   to vth, hdc and scat coordinates. Therefore, the routine PPolFillOnline() 
   (module ppol.cc) has to be called before starting this routine.
   
   ConeTest calculates the proton's coordinates in the 4 HDC planes for an
   azimutal angle of scattering of online.ppolA.scat.ph + 180 deg.

   If these coordinates are located inside of so called restricted 
   areas (outside the acceptance of the HDC, in driftcells of dead 
   wires, ...), defined in the source code, the return value is 1. 
   Are the actual located inside of the restricted areas, the return 
   value is 2. Otherwise it is 0.
*/

int ConeTest(struct online *onl);

























