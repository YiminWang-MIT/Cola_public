#include "Generate.h"
#include "ElasticRadiative.h"

double 
generateElasticRadiative::generateEvent(double helicity)
{
  double E0 = Reaction->electronIn.energy();
  FourVector In = FourVector(E0,0,0,Reaction->electronIn.momentum());
  // Set the electronOut FourVector with momentum = 1
  generateLabAngles(&Reaction->electronOut,1,sime->getAngle(), sime->getOop(), 
		    dcte,sime->getDphi());
  
  double theta = Reaction->electronOut.theta();
  double phi   = Reaction->electronOut.phi();


  double ct    = cos(theta), st = sin(theta);
  // energy of the outgoing electron in elastic scattering
  
  double Ep    = (-(E0 + m_proton)*(m_e_sqr+E0*m_proton)
		  + (m_e_sqr-E0*E0)*ct*sqrt(m_proton*m_proton-m_e_sqr*st*st))/
    ((E0*E0-m_e_sqr)*ct*ct-(E0+m_proton)*(E0+m_proton));

  // Set the electronOut FourVector
  Reaction->electronOut.initPolar(Ep, sqrt(Ep*Ep-m_e_sqr), theta, phi);
  Reaction->Out1 = In - Reaction->electronOut + *Reaction->getTarget();
  double q2 = (In-Reaction->electronOut).square();

  // correction to elastic electron scattering at the proton side as in Physical Review C, Volume 62, 025501, QED radiative corrections to virtual Compton scattering (M. Vanderhaeghen); p. 36 Eq. A74-A76. The energy loss is generated like on p. 19 only the term with Delta Es is used, the rest is assumed to be constant in the regime of interest. For Eq. A74 it is used: exp(deltaR)=(2*DeltaEs/Q/sqrt(x))**(4*alpha/pi*log(eta)); it is DeltaEs1=Q*sqrt(x)/2*random**(1/(4*alpha/pi*log(eta))); for Eq. A76 it is used: exp(deltaR)=(2*DeltaEs/MN)**(2*alpha/pi*(ENprime/pNprime*log(x)-1)); it is DeltaEs=MN/2*random**(1/(2*alpha/pi*(ENprime/pNprime*log(x)-1))); DeltaEs=DeltaEs+DeltaEs1; Eprime=-DeltaEs/eta+Ep (Eq. A48)

  double random = halton[4]();
  double eta = E0/Ep;
  double rho = sqrt(fabs(q2) + 4*m_proton*m_proton);
  double x = pow(sqrt(fabs(q2)) + rho,2)/(4*pow(m_proton,2));
  double t = 4*alpha/M_PI*log(eta);
  double DeltaEs = pow(fabs(q2)*x,0.5)/2*securePow(random,1./t);
  t = 2*alpha/M_PI*(Reaction->Out1.energy()/Reaction->Out1.momentum()*log(x)-1);
  DeltaEs+=m_proton/2*securePow(random,1./t);
  double Eprime = -DeltaEs/eta+Ep;
  if(Eprime <= m_electron) // make sure that there is any energy for the electron left
    {
      Eprime = m_electron+1e-10;
    }

  // Out1 is the excited proton
  Reaction->electronOut.initPolar(Eprime, sqrt(Eprime*Eprime-m_e_sqr), theta, phi);

  /*double Eproton = Reaction->Out1.energy();  
  double beta4 = Reaction->Out1.momentum()/Eproton;
  double tproton = 4 * log(E0/Ep) + log((1+beta4) / (1-beta4)) / beta4 - 2;*/
  // Internal radiative corrections for outgoing electron
  
  t =alpha/M_PI*(2*(log(fabs(q2)/m_e_sqr)-1));// + (protonContrib?0*tproton:0)); this part does not make any difference for 1M events, but it is not sure, if we can use the generation for the energy loss the same way, if we include the proton part.
  if ( t <= 0 ) return 0.0; // if t<=0 ,then q2 < m_e_sqr and then the formula is not valid any more
  //  cout <<"protonContrib:"<<protonContrib<<" "<<tproton<<" "<<log(fabs(q2)/m_e_sqr)-1<<endl;
  class Momentum P_Spin;
  class FourVector photon;
  
  double EBH,k;
  double dPhotonNorm;

  FourVector eoutsave = Reaction->electronOut;

  Reaction->electronOut=eoutsave;

  random = halton[2]();

  dPhotonNorm = 1.0;
  if ((BHmin != 0) && (BHmax != 0)) {
    double min =  pow(BHmin/Ep,t);
    double max =  pow(BHmax/Ep,t);
    random = min + random*(max - min);
    dPhotonNorm = (max - min);
  }
  double cutoff = securePow(RadCutOff,t);
  if (random<cutoff) return 0;

  // Energy loss because internal radiative corrections
  k = (Ep-m_electron)*securePow(random, 1.0/t);// the maximum energy the electron can lose is Ep-m_electron, if it would be more, than the mass of the electron would be smaller than m_electron
  double kk = k;  // store the real value of k
  if (k<1e-10)  k=1e-10; // k is the energy loss we put in the Bethe-Heitler calculation
  if ( kk == 0 ) return 0; // the weight would be 0 anway but some calculation does not work when kk == 0

  Reaction->electronOut -= k;


  k = Ep - Reaction->electronOut.energy(); // to minimize roundoff error

  double weight = 1.0;

  theta= Reaction->electronOut.getP()* Reaction->electronIn.getP();
  theta/=Reaction->electronOut.getP().abs()* Reaction->electronIn.getP().abs();
  theta=acos(theta);


  generateBetheHeitlerPeak BH(In, Reaction->electronOut);

  P_Spin=Momentum();

  bool lowerlimit;
  EBH=k;
  photon = BH.generate(weight,&lowerlimit,&EBH,&helicity,&P_Spin);
  double w2 = k/t/EBH;//  kk/t*securePow(kk/(Ep-m_electron),-t)/EBH: this is the correct factor, but k/t/EBH seems to work better;  kk/t*securePow(kk/(Ep-m_electron),-t) is correcting for the distribution with which the radiative tail is generated; /EBH is giving back the missing photon momentum
  if (weight<0){
    invbh++;
    if (invbh %100 ==0)   printf("Warning, Invalid BH.generate %i\n",invbh);
    return 0; // invalid event, very rare case 
  }

  // Out1 is now the final proton
  Reaction->Out1 = In + *Reaction->getTarget()-Reaction->electronOut - photon;
  
  Reaction->Out1 = Reaction->Out1.setSpin(P_Spin);


  /*
  
 //--- Now we need to consider, that the second order correction to the BH
  // and not to elastic are needed. Since Generator a priori considers the
  // elastic correction, we need to correct for the ratio:
  
  double deltaR = RealBHCorrection(Reaction->electronIn, Reaction->electronOut, Reaction->target, Reaction->Out1, photon2);
  RadiativeCorrection = exp(deltaR); // -- this is correction factor
  //cout<<"Correction: "<< RadiativeCorrection<< " Photon2 " << photon2.energy() << " real " << exp(deltaReal) << " Eprime " << Reaction->electronOut.energy() << " Photon " << photon.energy() << endl;
  //cout << Reaction->electronOut.energy() << " " << RadiativeCorrection << endl;
  weight *= RadiativeCorrection;
*/
  
  //--- For the complete higher order corrections some terms need to be added to 
  // the weight. With approx. random generator, we consider only part of the 
  // higher order correction. To get the complete exp(deltaR) correction, we 
  // need additional eta^(t/2) term. Of course, the VertexCorrection method still 
  // needs to be considered, to account for the part of the correction, that
  // does not depend on energy of emitted photon.
  /*
  if (protonContrib) {    
    //--- Missing terms for the whole Vanderhaeghen type higher order proton correction
    // are calculated using this function!
    double protonCorr = protonVertexCorrectionVanderhaeghen(Reaction->electronIn, Reaction->electronOut,  Reaction->Out1);
    //cout<<"Proton Correction: "<<Reaction->electronOut.energy()<<",  "<<protonCorr<<endl;  
    if (protonCorr>1.1) cout <<"pVC: "<<protonCorr<<endl;
    weight *= protonCorr;    
  }
  */
  
  
  // correction to Bethe-Heitler Diagrams as in Physical Review C, Volume 62, 025501, QED radiative corrections to virtual Compton scattering (M. Vanderhaeghen); p. 14 Eq. 58 and following. The energy loss is generated like on p. 19 only the term with Delta Es is used, the rest is assumed to be constant in the regime of interest.
  random = halton[3]();
  q2 = (In-Reaction->electronOut).square();
  double ktimesqprime = In * photon;
  double kprimetimesqprime = Reaction->electronOut * photon;
  double EtildeEtildeprimesqrt = securePow(4*(In.energy()-fabs(q2)/2/m_proton-ktimesqprime/m_proton)*(Reaction->electronOut.energy()+fabs(q2)/2/m_proton-kprimetimesqprime/m_proton),0.5); // = ( 4* Etilde * Etildeprime * Mm1**2 / MN**2)**0.5
  t =2*alpha*alpha/M_PI*(log(fabs(q2)/m_e_sqr)-1);  //  alpha*alpha because it is second order correction
  if ( t <= 0 ) return 0.0; // if t<=0 ,then q2 < m_e_sqr and then the formula is not valid any more
  k = EtildeEtildeprimesqrt * securePow(random, 1.0/t); // = (Mm1**2-MN**2)/MN; assume softphoton = l *(Reaction->electronOut.momentum.abs(),Reaction->electronOut.momentum()); it is Mm1**2-MN**2 = 2*Reaction->Out1 * softphoton => l = (Mm1**2-MN**2) / (2*Reaction->Out1*(Reaction->electronOut.momentum.abs(),Reaction->electronOut.momentum())) = k * MN / (2*Reaction->Out1*(Reaction->electronOut.momentum.abs(),Reaction->electronOut.momentum())), therefore the new electron out is: Reaction->electronOut-= l*Reaction->electronOut.momentum.abs()
  
  FourVector photon2 = FourVector(Reaction->electronOut.momentum(),Reaction->electronOut.getP());// this is just used for the calculation and not the soft photon
  if( (k * m_proton / 2 / (Reaction->Out1*photon2)*(Reaction->electronOut.momentum())) < Reaction->electronOut.energy() - m_electron - 1e-10) // make sure that there is still energy for the electron left
    {
      Reaction->electronOut-=(k * m_proton / 2 / (Reaction->Out1*photon2)*(Reaction->electronOut.momentum()));
      }
      //cout << k/2. << " " << Reaction->Out1*photon2 << " " << (Reaction->Out1.energy())*(photon2.energy()) - (Reaction->Out1.momentum())*(photon2.momentum()) << " " << EtildeEtildeprimesqrt << endl;
      

  double we=weight*w2;
  if (!(we>=0))
    {
      we=0;
      //cout <<"Error:"<< weight<<" "<<w2<<endl;//" "<<w3<<endl;
    }
  return we*dPhotonNorm;
}

