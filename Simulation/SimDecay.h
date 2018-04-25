//
// $Id: SimDecay.h 2216 2008-06-13 21:13:47Z distler $
//

#ifndef __SIMDECAY_H__
#define __SIMDECAY_H__

#include "SimParticle.h"

// Method which handles the decay of the pi+ into mu+ and neutrino 
int simParticleDecay(SimParticle *simParticle1, SimParticleGroup *simParticleGroup1);

#endif
