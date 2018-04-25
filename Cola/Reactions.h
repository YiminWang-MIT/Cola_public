//                                                                    -*-c++-*-
// $Id: Reactions.h 2435 2011-11-03 15:14:08Z patrick $
//

#ifndef __REACTIONS_H__
#define __REACTIONS_H__

#include "Masses.h"
#include <iostream>

/** @name Reactions and Processes
*/
//@{
/** Definition of reactions.
    Assigns particles to reaction types in a unique manner. The
    reaction is given in run.db
 */
class reaction {
protected:
  const char *name;
  Particle *A, *B, *C, *D, *O, *H3, *TOF, *NPOL, *NDET, *SI, *KAOS;
public:
  const Particle *m1, *m2, *m3, *m4;
  Particle target;
  Particle electronIn;
  Particle electronOut;
  Particle Out1;
  Particle Out2;
  Particle Out3;
  Particle Decay1;
  Particle Decay2;
  Momentum CMSPolarization;

  /** Name of the Reaction.
 
    For example:\\
    H(e,B:e'A:p)pi0\\
    gives a Reaction on Proton, the scattered electron in B and the
    recoil proton in A
    */
  const char *getName() const{ return name; };

  /// target particle
  const Particle *getTarget() const {  return &target; };
  /// reaction particles
  const Particle *getM1() const { return m1; };
  ///
  const Particle *getM2() const { return m2; };
  ///
  const Particle *getM3() const { return m3; };
  ///
  const Particle *getM4() const { return m4; };
  ///
  const Momentum getCMSPolarization() const { return CMSPolarization; };

  /// Threshold, might vary for Resonances!!!
  double threshold;

  /// Particles in the spectrometer
  Particle *get(int nr) { 
    switch (nr) {
    case 0 : return A; 
    case 1 : return B; 
    case 2 : return C; 
    case 3 : return D; 
    case 4 : return O; 
    case 5 : return H3; 
    case 6 : return TOF; 
    case 7 : return NPOL;
    case 8 : return NDET;
    case 9 : return SI;
    case 10 : return KAOS; 
    }
    std::cerr << "spectrometer number not found.\n";
    return NULL;
  };
  /// Particles in the spectrometer
  Particle *getA() { return A; };
  ///
  Particle *getB() { return B; };
  ///
  Particle *getC() { return C; };
  ///
  Particle *getD() { return D; };
  ///
  Particle *getO() { return O; };
  ///
  Particle *getH3() { return H3; };
  ///
  Particle *getTOF() { return TOF; };
  ///
  Particle *getNPOL() { return NPOL; };
  ///
  Particle *getNDET() { return NDET; };
  ///
  Particle *getSI() { return SI; };
  ///
  Particle *getKAOS() { return KAOS; };

  /// Constructor using the parser
  reaction(const char * Name);
  /// Constructor without parsing
  reaction(const char * Name,
	   Particle *a, Particle *b, Particle *c, Particle *d, Particle *o,
	   Particle *mtarget,
	   const Particle *M1,  // Elastic scattering   if M1=0 
	   const Particle *M2,  // Two body reaction    if M1,M2  given
	   const Particle *M3,  // Isotropic threebody  if M1,M2,M3
	   const Particle *M4); // M2 decays in M3, M4  if M1,M2,M3,M4

  /**Set the target momentum for colliding beams.
     Energy is given in A*(MeV/u) which is the total kinetic energy of the ion.
  **/
  int setTargetMomentum(double energy) {
    target.setMomentum(0, 0, -sqrt(energy*(2.0*target.getMass()+energy)));
    return 0;
  }
};

///Set the reaction type given by a string
reaction *set_reaction(char *rundbreaction);

//@}
#endif









