//
// Created 1993 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Cola/scinttest/SCINTraw.cc,v 2.1 2002-03-05 03:49:49 distler Exp $
//

#include <math.h>

#include "stdio.h"
#include "rundatabase.h"
#include "online.h"

// Speed of light in Scintillator (in ns / mm)

#define C_SCINT		(0.15 * 0.050)
#define C_SCINT_DE	(0.16 * 0.050)
      
/////////////////////////////////////////////////////////////////////////////
double
qsddaTimeShift(double y)
{ if (rundb.A.scint.use_dE==0)
    return rundb.A.scint.ToF[online.A.ToF.paddle] * rundb.tdc_resolution 
           - C_SCINT * y +
           rundb.A.scint.walk[1]/sqrt(online.A.ToF.Energy.right);
  else  
    return rundb.A.scint.dE[online.A.dE.paddle] 
           - C_SCINT_DE * y +
           rundb.A.scint.walk[2]/sqrt(online.A.dE.Energy.right);
}
/////////////////////////////////////////////////////////////////////////////
double bclamTimeShift(double y)
{
  return rundb.B.scint.ToF[online.B.ToF.paddle] * rundb.tdc_resolution 
       - C_SCINT * y
       + rundb.B.scint.walk[1]/sqrt(online.B.ToF.Energy.right);
}
/////////////////////////////////////////////////////////////////////
double qsddcTimeShift(double y)
{ if (rundb.C.scint.use_dE==0)
    return rundb.C.scint.ToF[online.C.ToF.paddle] * rundb.tdc_resolution 
         - C_SCINT * y;
  else  
    return rundb.C.scint.dE[online.C.dE.paddle] 
         - C_SCINT_DE * y;
}










