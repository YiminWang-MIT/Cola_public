//                                                                    -*-c++-*-
// $Id: ScintD.h 2216 2008-06-13 21:13:47Z distler $
//

#include "detectorBase.h"

class scintD : public detectorBase {
private:
  AquaNodeUShort *adcF[5];
  AquaNodeUShort *adcB[5];
  AquaNodeUShort *tdcF[5];
  AquaNodeUShort *tdcB[5];
  AquaNodeInt *trigger;

public:
  scintD(char *name, AquaTree *aquatree, 
         char *oname, AquaTree *onl);
  virtual ~scintD() { ; }
  virtual int handle();
  virtual void Reset();
};
