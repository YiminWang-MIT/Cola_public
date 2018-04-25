//  written 1997 by      Ingo Ewald    16.05.97

#include "Material.h"
#include "QuasiRandom.h"             // Random Generator
#include "rundatabase.h" 
#include <math.h>
#include <stdio.h>
#include <iostream>

static const double Na    = 6.022136E23;      // Avogadro's number

void 
Material::print() const
{
  Particle p = P_proton;
  p = FourVector(3.41626,0,0,3.2848);         //  dEdx(min. Ioni.)
  printf("Density: %6.4f X0[g/cm^2]: %6.2f X0[cm]: %5.1f"
	 " I[eV]: %5.2f dE/dx (min)[MeVcm^2/g]: %4.2f\n",
	 Density, RadiationLength, 
	 RadiationLength/Density, 
         IonisationPotential, -1000*dEdx(p, 1.0)/Density);
}
  



double 
Material::theta_MS_g_onestep(Particle part, double old_mom, double path) const 
{
  if (path<=0.0) return 0.0; 

  // Formula form Particle Data Booklet
  //
  // z = charge number of incident particle
  // p = momentum of incident particle in GeV/c
  // path = length of path through material in cm
  //
  // theta multiple scattering in rad

  double p = old_mom;
  double E = sqrt(p*p + part.getMass()*part.getMass());
  double beta = p / E;

  double z = fabs(part.getCharge());
  
  return 0.0136 / beta / p * z * sqrt(path / (RadiationLength/Density)) *
         (1 + 0.038 * log(path / (RadiationLength/Density)));
}

double 
Material::theta_MS_gauss(Particle part, double old_mom, double path) const
{
  if (path<=0.0) return 0.0; 

  // Formulae from W.R. Leo and GEANT 3.16 Manual (PHYS320/325)
  //
  // z = charge number of incident particle
  // p = momentum of incident particle in GeV/c
  // path = length of path through material in cm
  //
  // theta multiple scattering in rad

  double p = old_mom;
  double E = sqrt(p*p + part.getMass()*part.getMass());
  double beta = p / E;

  double z = fabs(part.getCharge());
  
  static const double a0    = 2.81794092E-13;   // cl. electron radius in cm
  static const double factor = 4 * M_PI * a0*a0 * m_electron*m_electron * Na;

  // ChiC = critical scattering angle

  double ChiC2 = factor * Zs *  z*z * Density * path / 
    (Massnumber * E*E * pow(beta,4));

  // Omega0 might be interpretated as mean numbers of scatters

  double Omega0 = ChiC2 / (1.167 * ChiAlpha2(p, beta, z));

  if (Omega0 < 10) {
    //cout << "Warning: No Multiple Scattering calculated !!!" << endl;
    //cout << "         Path through Material is too short !!!" << endl;
    return 0;
  }
  
  double F = 0.95;  // Fraction of Moliere distribution taken into account
  double nu = Omega0 / (2 * (1 - F));

  return sqrt(ChiC2 / (1 + F*F) * ((1 + nu) / nu * log(1 + nu) - 1));
}

void
Material::MultiScat(Particle &part, double old_mom, double path) const
{
  static PseudoRandom psrandom;
  static normal norm(&psrandom, &psrandom);

  double theta = theta_MS_gauss (part, old_mom, path);
  
  // Big angle scattering from electrons (long tails of MS distribution)
  // for 1 percent of events: rutherford_prob=0.01
  // to avoid form factor dependence, maximum angle is 0.1 rad
  //static const double rutherford_prob = 0.01;
  //if (psrandom()<rutherford_prob) {
  //double M_over_Rho = Massnumber * 1.66e-24 / Density * 1e39; 
  //  (unit: cm3 / 1e39 = fm3)
  //double elem_cross_sect = 1.44*1.44 / 16 / M_over_Rho * 1e13;
  //  (unit: fm2 * MeV2 / fm3 / 1e13 = MeV2/cm)
  //double E = sqrt(old_mom*old_mom+part.getMass()*part.getMass())*1000;
  //  (unit: MeV)
  //double np = E*E / (elem_cross_sect * path) * rutherford_prob;
  //double n = np * 0.75 * psrandom();
  //double u = pow(n*n+n+1, 1.0/3);
  //double y = u-1/u;
  //double dth = 2*atan2(1,y);
  // printf("dth = %f mrad (y=%f, np=%f)\n", dth * 1000, y, np);
  //if (dth<0.1)
  //  theta += 2*atan2(1,y);
  //}
  
  //if (psrandom()>0.9) theta *= 2.4; // Rutherford-Scattering
                                    // Value 2.4 taken from (e,e'd) 97
 

  if (!(theta==0)) {
    
    FourVector Dummy = part;
    Dummy = Polar(part.energy(), part.momentum(),
		  norm() * theta, (2 * psrandom() - 1) * M_PI);
    
    Dummy.rot_theta(part.theta());
    Dummy.rot_phi(part.phi());
    
    part = Dummy;
  }
}

///////////////////////////////////////////////////////////////////////////////
Element::Element() 
{
}

void
Element::IonisationPot() 
{
  // Formula and values form W.R. Leo
  //
  // Z = charge number of material
  // A = mass number of material
  // Density in [g/cm^3]
  // Ionisation-Potential in eV
  // Values for Z<6 from Sternheimer (Atomic Data and Nuclear Data Tables 30,261-271 (1984))
  
  if(Z < 13){
    switch (Z)
      {
      case 1: // Hydrogen and Deuterium
        if (A < 1.5) {
          IonisationPotential =   (Density > 0.069) ? 21.8 : 19.2 ;   // different potential for liquid and gaseous state
        } else {
          IonisationPotential =  20.1; // Deuterium
        } ; 
	break ;
      case 2:
        IonisationPotential = 41.8 ; 
	break ;
      case 3:
        IonisationPotential = 40.0 ;
	break ;
      case 4:
	IonisationPotential = 63.7 ;
	break ;
      case 5:
	IonisationPotential = 76.0 ;
	break ;
      case 6:
        IonisationPotential = 78.0 ;
        break ;
      default:
        IonisationPotential = 12.0*Z + 7.0 ;
      }
  } else {
    IonisationPotential = 9.76*Z + 58.8 * pow(Z , -0.19) ;
  
  }  
}

void
Element::Radiationlength() 
{
  // Formula and values from Particle Data Booklet
  //
  // Z = charge number of material
  // A = mass number of material
  //
  // Radiation Length in g/cm^2

  double Lrad, Lradp;
  
  if (A == 1.01) {RadiationLength =  63.05; return;}     // Hydrogen
  if (A == 2.01) {RadiationLength = 125.98; return;}     // Deuterium
  if (A == 4.  ) {RadiationLength =  94.32; return;}     // Helium 4 
 
  Lrad  = log( 184.15 * pow(Z, -1.0 / 3.0)); 
  Lradp = log(1194.0  * pow(Z, -2.0 / 3.0));
  
  double k = Z / 137.0359895;
  double f = k*k * (1 / (1 + k*k) + 0.20206 - 0.0369 * k*k +
                    0.0083 * pow(k, 4) - 0.002 * pow(k, 6));
  
  RadiationLength = 716.408 * A / (Z*Z * (Lrad - f) + Z * Lradp);
}

void
Element::Thicknessbremsstrahlung() 
{
  double Z=Chargenumber;
  double zeta = log(1440/pow(Z, 2/3.0))/log(183/pow(Z,1/3.0));
  double b    = 4.0/3.0*(1 + (Z+1)/9/(Z+zeta)/log(183/pow(Z,1/3.0)));
  ThicknessBremsstrahlung = 1 / ( b * Density / RadiationLength);
  // Unit cm
}

void
Element::LandauParameter() 
{
  LandauPar = 0.0001535 * Chargenumber * Density / Massnumber;
}

void 
Element::DensCorr() 
{
  // Formula from GEANT 3.16 Manual and W.R. Leo
  //
  // Z = charge number of material
  // A = mass number of material
  // I = Ionisationpotential of the material in eV
  // Density in [g/cm^3]

  double hmu_p = 28.816 * sqrt( Density * Z / A) ;   // Plasmafrequency*h in [eV] (Don't add artificial factors!)

  DC_C = -1.0 * (1.0 + 2.0 * log( IonisationPotential / hmu_p )) ;
  DC_m = 3 ;
  double xa = DC_C / (2.0 * M_LN10);
  
  if (IonisationPotential < 100) {
    DC_x1 = 2.0;
    if (DC_C < 3.681) 
      DC_x0 = 0.2;
    else
      DC_x0 = 0.326 * DC_C - 1.0;
  }
  else {
    DC_x1 = 3.0;
    if (DC_C < 5.215)
      DC_x0 = 0.2;
    else
      DC_x0 = 0.326 * DC_C - 1.5;
  }
  
  
  DC_a = 2.0 * M_LN10 * (xa - DC_x0) / pow((DC_x1 - DC_x0),DC_m);
  
  
  
}

double 
Element::ChiAlpha2(double p, double beta, double z) const 
{
  // Formulas from W.R. Leo and GEANT 3.16 Manual (PHYS325)
  // ChiAlpha is the atomic electron screening angle
  //
  // Z = charge number of material

  static const double alpha = 1/137.0359895;    // fine structure constant
  
  return 2.007E-11 * pow(Z,2/3) * log(1 + 3.34 * pow(alpha*Z*z/beta,2))/p/p;
}


double
Element::BetheBloch(Particle p) const 
{
  // Formula form C. Grupen / W.R. Leo
  //
  // z = charge number of incident particle
  // Z = charge number of material
  // A = mass number of material
  // I = Ionisation-Potential of material in ! [eV] !
  // delta = Density Correction of material
  //
  // dE/dx in GeV cm^2/g
  
  double Eloss = 0 ;
  double beta = p.momentum()/p.energy() ;
  double gamma = 1 / sqrt(1 - beta*beta) ;

    
  double betasq = beta * beta ;
  
  if (beta > 1 || beta < 0) 
    return 0;
  

  double z = p.getCharge();
  double I = IonisationPotential;  // N.B.: [I] = eV !

  double eta = beta*gamma ;
  double eta2 = eta*eta ;
  
   
  // Density Corrections
  
  double X = log(eta)/( M_LN10);
  double delta = 0.0;      

  if (X > DC_x0) {
    delta  = 2.0 * M_LN10 * X + DC_C;
    if (X < DC_x1)
      delta += DC_a * pow((DC_x1 - X),DC_m);
  }


  // Shell Corrections

  double C = 0.0;
  
  if (eta > 0.13) {
    C = (0.422377 / eta2 + 0.0304043 / (eta2*eta2) - 
         0.00038106 / (eta2*eta2*eta2)) * 1E-6 * I*I +
        (3.850190 / eta2 - 0.1667989 / (eta2*eta2) +
         0.00157955 / (eta2*eta2*eta2)) * 1E-9 * I*I*I;
  }
  else {
    double T = p.energy() - p.getMass();
    double T_eta13 = (1.00842 * p.getMass() - p.getMass()); 
    double eta_const = 0.13*0.13;
    
    C = ((0.422377 / eta_const + 0.0304043 / (eta_const*eta_const) - 
         0.00038106 / (eta_const*eta_const*eta_const)) * 1E-6 * I*I +
        (3.850190 / eta_const - 0.1667989 / (eta_const*eta_const) +
         0.00157955 / (eta_const*eta_const*eta_const)) * 1E-9 * I*I*I) *
         log(T / 2.0E-3) / log(T_eta13 / 2.0E-3);
  }

    
  if(p==P_electron){

    double tau = (p.energy() - p.getMass()) / p.getMass() ;    // Kinetic Energy of electrons in units of mc^2 
    double tausq = tau * tau ;
    double F_tau = 1 - betasq + ( ((tausq/8)-(2+tau)*log(2))/(pow((tau+1),2)) ) ;
    return Eloss = -0.1535375E-3*(Z/A)*(1/betasq)*(log(tausq*(tau+2)/(2*pow((I/511000),2))) + F_tau - delta - 2*C/Z) ;
    
  }else if(p==P_positron){

    double tau = (p.energy() - p.getMass()) / p.getMass() ;    // Kinetic Energy of positrons in units of mc^2 
    double tausq = tau * tau ;
    double F_tau = 2*log(2)-(betasq/12)*(23 + 14/(tau+2) + 10/pow(tau+2,2) + 4/pow((tau+2),2)) ;  
    return Eloss = -0.1535375E-3*(Z/A)*(1/betasq)*(log(tausq*(tau+2)/(2*pow((I/511000),2))) + F_tau - delta - 2*C/Z) ;
    
  }else{

    
    double M_part = p.getMass() ;
    double Tmax = (0.001022 * betasq * gamma*gamma ) / (1 + 2*gamma*(0.511E-3/M_part) + pow((0.511E-3/M_part),2)) ;
    return Eloss = -0.1535375E-3*(Z/A)*(1/betasq)*z*z* ( log(2*0.511E-3*gamma*gamma*betasq*Tmax/(pow(I/(1E9),2)))-2*betasq-delta-2*C/Z) ;
    
  };
  
};




double
Element::MostProbEloss(Particle p, double pathlength) const
{
  if (pathlength<=0.0) return 0.0; 

  //
  // z = charge number of incident particle
  // Z = charge number of material
  // A = mass number of material
  // I = Ionisation-Potential of material
  // delta = Density Correction of material
  // dE/dx in GeV/cm, pathlength in cm


  // This is a complete rewrite, using the Landau Distribution Formulas found in W.R. Leo.
  // For older versions, check out from CVS with a date selection prior 6/17/2004.

  // N.B.: This formula is valid for THIN absorbers (or electrons), where the energy-loss distribution 
  // is not symmetric (Gauss-like) and the number of collisions is small, so the energy-loss-distribution is Landau-like
  
  // This code is compatible with the data from geant, MODs Code, and estar. Estar *DOES* give correct
  // results if you don't take radiative stopping power into account. 

  double beta = p.momentum()/p.energy();
  double betasq = beta * beta;
  
  if (beta > 1 || beta < 0) 
    return 0;

  // avoid division by zero error
  double gamma=1e30;
  if ((1-betasq)>1e-60) gamma=(double)1.0/sqrt((double)1.0-betasq);

  double z = p.getCharge();
  double I = 0.001*IonisationPotential;   // in keV
  double gammasq = gamma*gamma ;
 
  // Density Corrections
  
  double X = log(gamma*beta)/( M_LN10);
  double delta = 0.0;      
  
  if (X > DC_x0) {
    delta  = 2.0 * M_LN10 * X + DC_C;
    if (X < DC_x1)
      delta += DC_a * pow((DC_x1 - X),DC_m);
  }
  if(X < DC_x0){
    delta = 0.14*pow(10,2*(X-DC_x0)) ;
  }
  
  // Attention: for Conductors we have a nonzero delta for X < X0 ! See Sternheimer for further details !
  // if(X < DC_x0){
  //   delta = 0.12*pow(10,2*(X-DC_x0) ;
  // }
  
  // Shell Correction (W.R. Leo)
  
  double eta = beta*gamma ;
  double C_shell = 0 ;
  if(eta >= 0.1){
    C_shell=(0.422377*pow(eta,-2)+0.0304043*pow(eta,-4)-0.00038106*pow(eta,-6))*I*I ;
    C_shell+=(3.850190*pow(eta,-2)-0.1667989*pow(eta,-4)+0.00157955*pow(eta,-6))*I*I*I ;
  }
  
  // formula from PDG & Physics Letters Vol 30 A Number 4 (Maccabee & Papworth)

  double zeta = 0.0001535*(Z/(A*betasq))*fabs(z*z)*(Density*pathlength);                 // in [GeV]

  // P. Achenbach  06.07.2015
  // caution: negative logarithm in Eprob appears for very thin thicknesses zeta < 1e-6
  // remark: different values of the constant 0.198 can be found in literature:
  // Claude Leroy & Pier-Giorgio Rancoita, Principles of Radiation Interaction in Matter and Detection, 2008
  // gives a value of 0.194.
  double Eprob = zeta*(log(2*511.6*betasq*gammasq*zeta*1E6/(I*I))+0.198-betasq-delta-2*C_shell/z);
  
  return -Eprob; 
}

Element::Element(int z, double a, double Dens) 
{
  Z = z; 
  A = a;
  Massnumber = a;
  Chargenumber = Z;
  Numbernumber = 1;
  Density = Dens;
  if (z) { 
    Radiationlength();
    Thicknessbremsstrahlung();
    LandauParameter();
    IonisationPot(); 
    DensCorr();
    Zs = z * (z + 1);
  }
}

Element::Element(int z, double a, double Dens,double C, double dca, double m, double x0, double x1) 
{
  Z = z; 
  A = a;
  Massnumber = a;
  Chargenumber = Z;
  Numbernumber = 1;
  Density = Dens;
  if (z) { /*  Radiationlength();
    Thicknessbremsstrahlung();
    LandauParameter();
    IonisationPot(); 
    DensCorr();
    Zs = z * (z + 1);
	   */
    Radiationlength();
    Thicknessbremsstrahlung();
    LandauParameter();
    IonisationPot(); 
    DC_C=C;
    DC_a=dca;
    DC_m=m;
    DC_x0=x0;
    DC_x1=x1;  
    Zs = z * (z + 1);
  }


}



double 
Element::dEdx(Particle p, double pathlength) const 
{
  if (pathlength<=0.0) return 0.0; 
  if (p == P_electron) return MostProbEloss(p, pathlength);
  if (p == P_positron) return MostProbEloss(p, pathlength);
  return BetheBloch(p) * Density * pathlength;
} 

double
Element::dLdx(Particle p, double pathlength) const
{
  double S = 1;
  double kB = 10.1;
  
  if (pathlength<=0.0) return 0.0; 
  return S * fabs(BetheBloch(p)) * 1000 / 
    (1 + kB * fabs(BetheBloch(p))) * pathlength;
}

Compound::Compound(double density, 
		   const Element e0, const double n0,
		   const Element e1, const double n1,
		   const Element e2, const double n2,
		   const Element e3, const double n3,
		   const Element e4, const double n4,
		   const Element e5, const double n5,
		   const Element e6, const double n6)
{
  Density = density;
  e[0] = Element(e0); n[0] = n0;
  e[1] = Element(e1); n[1] = n1;
  nmax = 2; nsum = n0 + n1;
  if (n2) { e[2] = Element(e2); n[2] = n2; nmax++; nsum += n2;}
  if (n3) { e[3] = Element(e3); n[3] = n3; nmax++; nsum += n3;}
  if (n4) { e[4] = Element(e4); n[4] = n4; nmax++; nsum += n4;}
  if (n5) { e[5] = Element(e5); n[5] = n5; nmax++; nsum += n5;}
  if (n6) { e[6] = Element(e6); n[6] = n6; nmax++; nsum += n6;}
  
  double A_sum;
  int Z_sum, Zs_sum, Number_sum;
  A_sum = Z_sum = Zs_sum = Number_sum = 0;

  for (int i=0;i<nmax;i++) {
    Number_sum += n[i];
    A_sum  += n[i] * e[i].A;
    Z_sum  += n[i] * e[i].Z;
    Zs_sum += n[i] * e[i].Z * (e[i].Z + 1); 
  }

  Massnumber = A_sum;
  Chargenumber = Z_sum;
  Numbernumber = Number_sum;
  Zs = Zs_sum;
  
  double invradlen = 0;
  for (int i=0;i<nmax;i++) 
    invradlen += e[i].A * n[i] / e[i].RadiationLength;
  RadiationLength = A_sum/invradlen;

  if (Density == 1.205/1000) RadiationLength = 36.66;    // Air
  if (Density == 1.)         RadiationLength = 36.08;    // Water

  // Bremsstrahlung in compounds: The thicknesses are summed, weightened
  // by their mass/weight (number of nuclei) contribution
  ThicknessBremsstrahlung=0;
  LandauPar=0;
  for (int i=0;i<nmax;i++) {
    ThicknessBremsstrahlung += e[i].A * n[i] * 
      e[i].ThicknessBremsstrahlung * e[i].Density; 
    LandauPar += e[i].A * n[i] * 
      e[i].LandauPar / e[i].Density; 
  }
  ThicknessBremsstrahlung /= Density*A_sum;
  LandauPar *= Density/A_sum;
  
  // Formula and values from W.R. Leo

  double ln_I_eff = 0;
  for (int i=0;i<nmax;i++)
    ln_I_eff += e[i].Z * n[i] * log(e[i].IonisationPotential) / Z_sum;
    
 
  
  IonisationPotential =  exp(ln_I_eff);
  
  
  if (Density == 1.032)        IonisationPotential = 64.7;    // Scint.
  if (Density == 1.205/1000.0) IonisationPotential = 85.7;    // Air
  if (Density == 1.)           IonisationPotential = 75. ;    // Water
}

double 
Compound::ChiAlpha2(double p, double beta, double z) const 
{
  // Formulas from W.R. Leo and GEANT 3.16 Manual (PHYS 325)
  //
  // Z = charge number of material
    
  static const double alpha = 1/137.0359895;    // fine structure constant

  double Ze = 0;
  double Zx = 0;

  for (int i=0;i<nmax;i++) {
    Ze += n[i] * e[i].Z * (e[i].Z + 1) * log(pow(e[i].Z,-2/3));
    Zx += n[i] * e[i].Z * (e[i].Z + 1) * 
      log(1 + 3.34 * pow(alpha*e[i].Z*z/beta,2));
  }  
    
  return 2.007E-11 * exp((Zx - Ze)/Zs) / p / p;
}


double 
Compound::dEdx(Particle p, double pathlength) const 
{
  if (pathlength<=0.0) return 0.0; 

  // Formula from W.R. Leo

  double dEdx=0;
  for (int i=0;i<nmax;i++) {
    dEdx += e[i].A * n[i] * e[i].dEdx(p, pathlength) / e[i].Density;
  }
  //  std::cerr<<"Dens: "<<Density<<"\t"<<Density*pathlength<<std::endl;
  return dEdx/Massnumber * Density;
}

double 
Compound::dLdx(Particle p, double pathlength) const
{
  if (pathlength<=0.0) return 0.0; 

  double S = 1;
  double kB = 10.1;
  
  double dEdx=0;
  for (int i=0;i<nmax;i++) 
    dEdx += e[i].A * n[i] * e[i].dEdx(p, pathlength) / e[i].Density / 
      pathlength;
  dEdx /= A_sum;

  return S * fabs(dEdx) * 1000 / (1 + kB * fabs(dEdx)) * pathlength;
}

/* Defintion of some Materials */

/* Data for C,a,m,X0,X1 from Physical Review B, Vol26, page 6067 */

Element *E_H1        = new Element( 1,   1.01,  0.0838 / 1000, -9.5835, 0.14092, 5.7273, 1.8639, 3.2718);
Material *H1 = E_H1; 

// old density for liquid hydrogen of 0.0708 g/cm3 is valid
// only for 26K and 4bar. Taken from an old Particle Data Booklet (1996?).
// the density 0.069 g/cm3 is a reasonable value for the A1 target
// 21.84K and 2bar. See: http://webbook.nist.gov/chemistry/fluid/
// Order of Constants in Brackets (Z, A, density, C, a, m, X0, X1)

Element *E_LH2       = new Element( 1,   1.01,  0.069, -3.263, 0.13483, 5.6249, 0.476, 1.9215);
Material *LH2 = E_LH2;

/* Values for C,a,m,X0,X1 not found for Deuterium. Scince effects accord to atomic magnitudes, values for LH2 were used.*/ 
Element *E_LD2       = new Element( 1,   2.01,  0.169, -3.263, 0.13483, 5.6249, 0.476, 2.0); 
Material *LD2 = E_LD2;

//Values for He & Li from R.M.Sternheimer "Density Effect for the Ionization Loss of charges Particles
// in Various Substances", Atomic Data and Nuclear Data Tables 30,261-271 (1984)

Element *E_He3       = new Element( 2,   3.00,  1.8 / 1000, -11.1393, 0.13443, 5.8347, 2.2017, 3.6122 );
Material *He3 = E_He3;

Element *E_He4       = new Element( 2,   4.00,  1.8 / 1000, -11.1393, 0.13443, 5.8347, 2.2017, 3.6122 );
Material *He4 = E_He4;

Element *E_Li6       = new Element( 3,   6.00, 0.534, -3.1221, 0.95136, 2.499, 0.1304, 1.6397 );
Material *Li6 = E_Li6;

Element *E_Li7       = new Element( 3,   7.00, 0.534, -3.1221, 0.95136, 2.499, 0.1304, 1.6397 );
Material *Li7 = E_Li7;

Element *E_Be        = new Element( 4,   9.012, 1.848, -2.7847, 0.80392 , 2.4339, 0.0592, 1.6922 );
Material *Be = E_Be;

Element *E_C12       = new Element( 6,  12.01,  2.265, -2.868 , 0.26142, 2.8697, -0.0178, 2.4860 );
Material *C12 = E_C12;

Element *E_C12pure     = new Element( 6,  12.01,  1.7, -3.155 , 0.2076, 2.9532, 0.0480, 2.5387 );
Material *C12pure = E_C12pure;

//Liquid Nitrogene! 
Element *E_N14     = new Element( 7,  14.01,  0.807, -4.204, 0.15349, 3.2125 , 1.7378, 4.1323 );
Material *N14 = E_N14;

//Liquid Oxygen !
Element *E_O16       = new Element( 8,  16.00, 1.429 , 10.7004, 0.1178, 3.2913, 1.7541, 4.3213 );
Material *O16 = E_O16;

Element *E_Al        = new Element(13,  26.98,  2.70 , -4.2395, 0.08025, 3.6345 , 0.1708, 3.0127);
Material *Al = E_Al;

Element *E_Si        = new Element(14,  28.086, 2.33 , -4.4351, 0.14921, 3.2546, 0.2014, 2.8715 );
Material *Si = E_Si;

Element *E_Cr        = new Element(24,  52.00,  7.14 , -4.1781, 0.15419, 2.9896, 0.0340, 3.0415 );
Material *Cr = E_Cr;

Element *E_Mn        = new Element(25,  54.04,  7.44, -4.2702, 0.14973, 2.9796, 0.0447, 3.1704 );
Material *Mn = E_Mn;

Element *E_Fe        = new Element(26,  55.85,  7.87 , -4.2911, 0.14680, 2.9632, -0.0012, 3.1531 );
Material *Fe = E_Fe;

Element *E_Co        = new Element(27,  58.93,  8.89 , -4.2601, 0.14474, 2.9502, -0.0187, 3.1790 );
Material *Co = E_Co;

Element *E_Ni        = new Element(28,  58.69,  8.91, -4.3115, 0.16496, 2.8430, -0.0566, 3.1851  );
Material *Ni = E_Ni;

Element *E_Cu        = new Element(29,  63.546, 8.92,-4.4190, 0.14339, 2.9044, -0.0254, 3.2793  );
Material *Cu = E_Cu;

Element *E_Mo        = new Element(42,  95.94, 10.28 ,-4.8793 , 0.15025, 3.2549, 0.2267, 3.2784 );
Material *Mo = E_Mo;

Element *E_Ta        = new Element(73, 180.95, 16.65, -5.5265, 0.17798, 2.7623, 0.2117 , 3.4805 );
Material *Ta = E_Ta;

Element *E_W         = new Element(74, 183.85, 19.26, -5.4059, 0.15509, 2.8447 , 0.2167, 3.4960 );
Material *W = E_W;

//Gaseous Hydrogene!
Element *E_H2        = new Element( 1,   1.01,  0.0838 / 1000, -9.5835, 0.14092, 5.7273, 1.863, 3.2718);
Material *H2 = E_H2;

//Gaseous Nitrogene!
Element *E_N2        = new Element( 7,  14.01,  1.2505 / 1000, -10.54, 0.15349, 3.2125 , 1.7378, 4.1323);
Material *N2 = E_N2; 

//Values for gaseous Oxygen !
Element *E_O2       = new Element( 8,  16.00,  1.141 , -10.7004, 0.1178, 3.2962, 1.7541, 4.3213 );
Material *O2 = E_O2;

Element *E_Heliumgas   = new Element(2 , 4.0 , 0.1786/1000, -11.139, 0.01140, 5.8347, 2.2017, 3.6122);
Material *Heliumgas = E_Heliumgas;

Material *Argongas     = new Element(18, 39.948, 1.784/1000, -1.753, 0.19714, 2.9618, 1.7635, 4.4855);

Material *Isobutane    = new Compound(2.67 / 1000,
					    *E_C12,  1,
					    *E_H1,   2);

Material *Air          = new Compound(1.205 / 1000,
					    *E_N2, 755,
					    *E_O2, 231);

Material *FrozenAir    = new Compound(0.86,
				            *E_N2, 0.7888,
				            *E_O2, 0.2111);

Material *Vacuum       = new Compound(1.205 / 1E11,
					    *E_N2, 755,
					    *E_O2, 231);

Material *Water        = new Compound(1.00 , 
					    *E_H1,   2,
					    *E_O16,  1);

Material *Snow         = new Compound(0.4 , 
					    *E_H1,   2,
					    *E_O16,  1);

Material *Scintillator = new Compound(1.032, 
					    *E_C12, 10, 
					    *E_H1,  11);

Material *CH2          = new Compound(0.94,
					    *E_C12,  1,
				            *E_H1,   2); //2 

//bss
//this is old, bernauer didnt use it because: 
// This is parts per weight!
//not consistent with the rest which is parts per parts...
//Material *Havar        = new Compound(8.30 ,
//					    *E_Co, 428,
//					    *E_Cr, 200,
//					    *E_Fe, 176,
//					    *E_Ni, 130,
//					    *E_W ,  28,
//					    *E_Mo,  20,
//					    *E_Mn,  16);

//from bernauer superstar
//this is parts per weight divided by A multiplied/normalized by something unknown so far: 116.76
//
//Material *Havar        = new Compound(8.30 ,
//					    *E_Co, 848,
//					    *E_Cr, 449,
//					    *E_Fe, 368,
//					    *E_Ni, 259,
//					    *E_W ,  18,
//					    *E_Mo,  24,
//				      *E_Mn,  35);

//-- Here the numbers on the right are mol fractions, so that
// the entry is consistent with the class Material
Material *Havar        = new Compound(8.30 ,
					    *E_Co, 0.4155,
					    *E_Cr, 0.2218,
					    *E_Fe, 0.1839,
					    *E_Ni, 0.1293,
					    *E_W , 0.00878,
					    *E_Mo, 0.0118,
				      	    *E_Mn, 0.0168);


Material *Kapton        = new Compound(1.42 , 
					    *E_N2,    2,
					    *E_C12,  22,
					    *E_H1,   10,
					    *E_O16,   5);

Material *Mylar        = new Compound(1.39 , 
					    *E_C12,  5,
					    *E_H1,   4,
					    *E_O16,  2);

Material *Quartz       = new Compound(2.20 ,
					    *E_Si,  1,
					    *E_O16, 2);

Material *Kevlar       = new Compound(1.44,
				            *E_C12, 14,    
				            *E_H1,  10,
				            *E_O16,  2,
				            *E_N14,  2);
