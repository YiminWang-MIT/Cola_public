#include "FourVector/FourVector.h"
#include "Generate.h"
#include "QED.h"
#include "Simulation/Formfactor.h"

/**
   PWIA off shell cross section cc1 of 
   T. deForest Jr., Nucl. Phys. A392 (1983) 232

   ein:  incoming electron
   eout: outgoing electron
   pout: outgoing struck proton
*/
 
double PWIA::deForestCC1(FourVector ein, FourVector eout, FourVector pout)
{

// NOTE: Ep = E' is not the electron energy!
// magentic moment is included in definition of F2:   F2(0)=kappa

  static const NucleonFormfactor *ff = new MergellFit();
  FourVector photon  = ein - eout;
  FourVector missing = photon - pout;
  double 
    theta = eout.theta(),                 // Scattering angle
    phi   = pout.rotate(photon).phi(),    // Angle scattering/emisson-plane
    gamma = pout.rotate(photon).theta(),  // photon-proton angle
    //    q2    = photon * photon,              // Fourmomentum transfer squared
    q     = photon.momentum(),            // photon momentum
    pp    = pout.momentum(),              // outgoing proton momentum
    Ep    = pout.energy(),                // outgoing proton energy
    p     = missing.momentum(),           // inital proton momentum
    Ebar  = sqrt(sqr(m_proton) + sqr(p)), // inital proton energy
    omegabar = Ep - Ebar,                 
    q2bar = omegabar*omegabar - q*q,
    f1  = ff->F1(q2bar),
    f2  = ff->F2(q2bar),          
    ff1 = f1 * f1 - q2bar / 4 / sqr(m_proton) * f2 * f2,
    ff2 = sqr(f1 + f2),

    WC =                    1/(4 * Ebar*Ep) * (sqr(Ebar+Ep) * ff1 - q*q*ff2),
    WT =               -q2bar/(2 * Ebar*Ep) * ff2,
    WS = sqr(pp * sin(gamma))/(    Ebar*Ep) * ff1,
    WI =   - pp * sin(gamma) /(    Ebar*Ep) * (Ebar+Ep) * ff1,

    qq = -q2bar/q/q,
    sigma_Mott = mubarn * sqr(2 * alpha * eout.energy() * cos(theta/2) / q2bar),
    cs = sigma_Mott * (qq*qq                       * WC +
    		       (qq/2  + sqr(tan(theta/2))) * WT +
		       qq*sqrt(qq + sqr(tan(theta/2))) * WI * cos(phi) +
		       (qq*sqr(cos(phi))+sqr(tan(theta/2))) * WS);
  return cs;
}

double PWIA::deForestCC1_neutron(FourVector ein, FourVector eout, FourVector nout)
{

// NOTE: Ep = E' is not the electron energy!
// magentic moment is included in definition of F2:   F2(0)=kappa

  static const NucleonFormfactor *ff = new DipoleGalsterFit();
  FourVector photon  = ein - eout;
  FourVector missing = photon - nout;
  double 
    theta = eout.theta(),                 // Scattering angle
    phi   = nout.rotate(photon).phi(),    // Angle scattering/emisson-plane
    gamma = nout.rotate(photon).theta(),  // photon-proton angle
    q2    = photon * photon,              // Fourmomentum transfer squared (negative)
    q     = photon.momentum(),            // photon momentum
    pn    = nout.momentum(),              // outgoing neutron momentum
    En    = nout.energy(),                // outgoing neutron energy
    p     = missing.momentum(),           // inital neutron momentum
    Ebar  = sqrt(sqr(m_neutron) + sqr(p)), // inital neutron energy
    omegabar = En - Ebar,                 
    q2bar = omegabar*omegabar - q*q,
    f1  = ff->F1(q2bar),
    f2  = ff->F2(q2bar),          
    ff1 = f1 * f1 - q2 / 4 / sqr(m_neutron) * f2 * f2,  //sign from definition q2=(q,iw)*(q,iw)=-q2
    ff2 = sqr(f1 + f2),
    
    WC =                    1/(4 * Ebar*En) * (sqr(Ebar+En) * ff1 - q*q*ff2),
    WT =                   -q2/(2 * Ebar*En) * ff2,  //sign from definition q2=(q,iw)*(q,iw)=-q2
    WS = sqr(pn * sin(gamma))/(    Ebar*En) * ff1,
    WI =   - pn * sin(gamma) /(    Ebar*En) * (Ebar+En) * ff1,
    
    qq = -q2bar/q/q,//sign from definition q2=(q,iw)*(q,iw)=-q2
    sigma_Mott = mubarn * sqr(2 * alpha * eout.energy() * cos(theta/2) / q2bar),
    cs = sigma_Mott * (qq*qq                       * WC +
		       (qq/2  + sqr(tan(theta/2))) * WT +
		       qq*sqrt(qq + sqr(tan(theta/2))) * WI * cos(phi) +
		       (qq*sqr(cos(phi))+sqr(tan(theta/2))) * WS);


  return cs;
}
