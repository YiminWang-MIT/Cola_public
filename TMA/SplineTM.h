// the guy to blame:
//
// Jan C. Bernauer
// A1 Collaboration
// J.G. University Mainz
// mailto: bernauer@kph.uni-mainz.de


#ifndef __SplineTM_h__
#define __SplineTM_h__

#include "transandlook.h"
#include "baseTM.h"

class SPLINE: public baseTM{
private:
    TL t;

public:
  /// Contructor of the class. Reads the solution file and prepares evaluation
  SPLINE(const char *filename,const char *filename2);
  /// Destructor
  ~SPLINE();
  /// Method for evaluation
  struct TargetCo * run(double x, double th, double y, double ph);
  int setOffsets(double x_offs, double th_offs, double y_offs, double ph_offs);
  /// Variable containing the results of evaluation
  TargetCo result;
};

#endif
