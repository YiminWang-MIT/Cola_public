//
// $Id: ScintD.cc 2216 2008-06-13 21:13:47Z distler $
//

#include "ScintD.h"
#include <cstdio>
#include <cstring>

scintD::scintD(char *name, AquaTree *aquatree, 
	       char *oname, AquaTree *onl) : detectorBase(aquatree, onl)
{
  char nme[255];
  for (int i=0;i<5;i++) {
    sprintf(nme,"%s.scint[%d].front.energy",name,i);
    adcF[i] = (AquaNodeUShort *) atree->find(nme);
    sprintf(nme,"%s.scint[%d].back.energy",name,i);
    adcB[i] = (AquaNodeUShort *) atree->find(nme);
    sprintf(nme,"%s.scint[%d].front.time",name,i);
    tdcF[i] = (AquaNodeUShort *) atree->find(nme);
    sprintf(nme,"%s.scint[%d].back.time",name,i);
    tdcB[i] = (AquaNodeUShort *) atree->find(nme);
  }
  sprintf(nme,"%s.trigger",oname);
  trigger = (AquaNodeInt *) online->find(nme);
}

int scintD::handle()
{
  int pattern= 0;
  for (int i=0;i<5;i++) {
    if (adcF[i]->ok() && adcB[i]->ok() && 
	tdcF[i]->ok() && tdcB[i]->ok())
      pattern += 1<<i;
  }

  *trigger = (int) (pattern != 0);
  return *trigger;
}

void scintD::Reset()
{
}
