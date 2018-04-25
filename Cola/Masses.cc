//
// Created 1997 by      A1 Collaboration 
//                      Institut fuer Kernphysik
//			Johannes Gutenberg-Universitaet Mainz 
//
//                      Harald Merkel
//                      Uli Mueller
//                      Ingo Ewald
//                      Michael Distler
//
// $Header: /tmp/cvsroot/Cola/Cola/Masses.cc,v 2.15 2007-07-05 15:41:18 sirca Exp $
//

#include <string.h>
#include "Masses.h"

int Particle::lastid = 1;
Particle *Particle::first = NULL;

int Particle::operator==(const class Particle particle) const
{
  return particle.id == id; // This comparison only checks the id!
}

Particle::Particle(const FourVector& FV, const FourVector & pol)
  :  FourVector(FV[0],FV[1],FV[2],FV[3])
{
  id = lastid++;
  name = "";
  mass = FV.mass();
  width = charge = 0; 
  polarization = pol;
}

Particle::Particle(const char *Name, int ch, double mss, double wdth)
:  FourVector(mss, 0, 0, 0)  // Particle in rest
{
  id     = lastid++;             // Just increase the id ....
  next = first;
  first = this;

  name = Name;
  mass = mss;
  charge = ch;
  width  = wdth;

  polarization = FourVector();
}

Particle Particle::operator=(FourVector fv)
{
  E    = fv[0];
  p[0] = fv[1];
  p[1] = fv[2];
  p[2] = fv[3];

  return *this;
}

Particle *
Particle::find(const char *name)
{ 
  Particle *res = first;
  while (res) { 
    if (!strcmp(res->name,name)) return res;
    res = res->next;
  }
  std::cerr << "ERROR: Can't find particle \"" << name << "\"" << std::endl;
  return NULL;
}


Momentum
Particle::getSpin(void) const
{
  Momentum S = this->getPolarization().getP();
  Momentum beta = this->beta();
  double gamma = this->gamma();
  return S-beta*(gamma/(gamma+1))*(beta*S);
}


//
// Class name  : Particle
// Method name : setMomentum(double px, double py, double pz)
//
// Description : set momentum (and energy) for an existing particle
// Input       : px, py, pz components of momentum
// Output      : none
// Return      : none
//

void
Particle::setMomentum(double px, double py, double pz)
{
    E    = sqrt(mass*mass + px*px + py*py + pz*pz);
    p[0] = px;
    p[1] = py;
    p[2] = pz;
} // end of Particle::setMomentum();



Particle
Particle::setSpin(const Momentum &s)
{
  Momentum beta = Particle::beta();
  double gamma = Particle::gamma();
  Momentum Sp = s+beta*(gamma*gamma/(gamma+1))*(beta*s);
  polarization.setE(gamma*(beta*s)) ;
  polarization.setP(Sp) ;
  return *this;
}

Particle 
Particle::Lorentz(const FourVector& ref)
{ 
  Particle par = *this;
  double g = ref.gamma();
  Momentum beta = ref.beta();

  Momentum p = *this;
  double beta_p = beta * p;
  double factor = g * (beta_p * g / (1 + g) + E);
  par.setE(g * (E + beta_p));
  par.setP( p + beta * factor);
  
  p = this->polarization;
  beta_p = beta * p;
  factor = g * (beta_p * g / (1 + g) + polarization[0]);
  par.polarization.setE(g * (polarization[0] + beta_p));
  par.polarization.setP( p + beta * factor);
  return par;
}

Particle 
Particle::rotate(const Momentum& direction) const
{ 
  Particle par = *this;  
  par.rot_phi(   -direction.phi());
  par.rot_theta( -direction.theta());
  par.polarization.rot_phi(   -direction.phi());
  par.polarization.rot_theta( -direction.theta());
  return par;
}

Particle 
Particle::rotateTo(const Momentum& direction) const
{ 
  Particle par = *this;  
  par.rot_theta( direction.theta());
  par.rot_phi(   direction.phi());
  par.polarization.rot_theta( direction.theta());
  par.polarization.rot_phi(   direction.phi());
  return par;
}

std::ostream& operator<<(std::ostream& out, Particle part)
{
  out.setf(std::ios::fixed, std::ios::floatfield);
  int p = out.precision(6);
  out << (FourVector)part << std::setw(10) << part.mass
      << std::setw(10) << part.width << std::setw(3) << part.charge
      << " '" << part.name << "'";
  out.precision(p);
  out.setf(std::ios::right, std::ios::adjustfield);

  return out;
}

// Gauge Bosons

const Particle P_gamma   = Particle("gamma", 0, 0.0);

// Leptons

const Particle P_electron = Particle("e-", -1, m_electron);
const Particle P_positron = Particle("e+", +1, m_electron);
const Particle P_muplus   = Particle("mu+", +1, m_mu);
const Particle P_muminus  = Particle("mu-", -1, m_mu);

// Mesons

const Particle P_piminus  = Particle("pi-", -1, m_pion);
const Particle P_pi0      = Particle("pi0",  0, m_pi0);
const Particle P_piplus   = Particle("pi+", +1, m_pion);
const Particle P_eta      = Particle("eta", 0, m_eta);
const Particle P_kplus    = Particle("K+", +1, m_kaon);
const Particle P_kminus   = Particle("K-", -1, m_kaon);

// Nucleons

const Particle P_proton   = Particle("p", 1, m_proton);
const Particle P_neutron  = Particle("n", 0, m_neutron);
const Particle P_Lambda   = Particle("Lambda", 0, m_Lambda);
const Particle P_Sigma0   = Particle("Sigma0", 0, m_Sigma0);
const Particle P_deuteron = Particle("d", 1, m_deuteron);
const Particle P_triton   = Particle("t", 1, m_H3);

// Nuclei

const Particle P_H1       = Particle("H",      1, m_H1);
const Particle P_H1_nb1   = Particle("NB1004", 1, m_H1_nb1);
const Particle P_H1_nb2   = Particle("NB1044", 1, m_H1_nb2);
const Particle P_H1_nb3   = Particle("NB1094", 1, m_H1_nb3);
const Particle P_H2       = Particle("D",      1, m_H2);
const Particle P_H3       = Particle("H3",     1, m_H3);
const Particle P_He3      = Particle("He3",    2, m_He3);
const Particle P_He4      = Particle("He4",    2, m_He4);
const Particle P_Li6      = Particle("Li6",    3, m_Li6);
const Particle P_Li7      = Particle("Li7",    3, m_Li7);
const Particle P_Be7      = Particle("Be7",    4, m_Be7);
const Particle P_Be9      = Particle("Be9",    4, m_Be9);
const Particle P_Be10     = Particle("Be10",   4, m_Be10);
const Particle P_B10      = Particle("B10",    5, m_B10);
const Particle P_B11      = Particle("B11",    5, m_B11);
const Particle P_B12      = Particle("B12",    5, m_B12);
const Particle P_C11      = Particle("C11",    6, m_C11);
const Particle P_C12      = Particle("C12",    6, m_C12);
const Particle P_C13      = Particle("C13",    6, m_C13);
const Particle P_C14      = Particle("C14",    6, m_C14);
const Particle P_N12      = Particle("N12",    7, m_N12);
const Particle P_N14      = Particle("N14",    7, m_N14);
const Particle P_N15      = Particle("N15",    7, m_N15);
const Particle P_O15      = Particle("O15",    8, m_O15);
const Particle P_O16      = Particle("O16",    8, m_O16);
const Particle P_Al27     = Particle("Al27",  13, m_Al27);
const Particle P_Cr52     = Particle("Cr52",  24, m_Cr52);
const Particle P_Mn55     = Particle("Mn55",  25, m_Mn55);
const Particle P_Fe56     = Particle("Fe56",  26, m_Fe56);
const Particle P_Co59     = Particle("Co59",  27, m_Co59);
const Particle P_Ni58     = Particle("Ni58",  28, m_Ni58);
const Particle P_Cu63     = Particle("Cu63",  29, m_Cu63);
const Particle P_Mo98     = Particle("Mo98",  42, m_Mo98);
const Particle P_Ta181    = Particle("Ta181", 73, m_Ta181);
const Particle P_W184     = Particle("W184",  74, m_W184);




// Resonances

const Particle P_Deltam   = Particle("Delta-",  -1, m_Delta, 0.120);
const Particle P_Delta0   = Particle("Delta",    0, m_Delta, 0.120);
const Particle P_Deltap   = Particle("Delta+",   1, m_Delta, 0.120);
const Particle P_Deltapp  = Particle("Delta++",  2, m_Delta, 0.120);
const Particle P_rhominus = Particle("rho-",    -1, m_rho,   0.1512);
const Particle P_rho0     = Particle("rho",      0, m_rho,   0.1512);
const Particle P_rhoplus  = Particle("rho+",     1, m_rho,   0.1512);

// Special particles

const Particle P_rho600   = Particle("rho600",  0, 0.600,   0.1512);
const Particle P_rho700   = Particle("rho700",  0, 0.700,   0.1512);
