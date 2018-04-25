//                                                                    -*-c++-*-
// $Id: PWIA.h 2515 2013-08-08 12:52:30Z kohly $
//
//   Plane Wave Impulse Approximation 
//   Formalism of e.g. T. deForest Jr., Nucl. Phys. A392 (1983) 232
//
//
class PWIA {
public:
  virtual ~PWIA() { ; }
  virtual double S(double Emiss, double pmiss) {return 1;};
  double deForestCC1(FourVector ein, FourVector eout, FourVector pout);
  double deForestCC1_neutron(FourVector ein, FourVector eout, FourVector nout);

//   MAKE SURE TO DEFINE YOUR EMISS AND PMISS THE SAME WAY 
//   FOR YOUR S(EMISS,PMISS) AS I DO!

  inline double CrossSection(FourVector ein, FourVector eout, 
			     FourVector target, FourVector pout) 
  {
    FourVector miss = ein - eout + target - pout ;
    return pout.momentum() * pout.momentum() * deForestCC1(ein, eout, pout) 
          * 1 / (1 - (pout.energy()/miss.energy() * pout.getP() * miss.getP() 
	  	      / (pout.getP()*pout.getP())))
      * S(miss.energy(), miss.momentum());
  };
};

// Use this as a model of TWO body breakup of He3 in PWIA
// mom-distr. from a fit to data from R. Florizone 

class PWIA_He3_2Body : public PWIA {
public:
  virtual ~PWIA_He3_2Body() { ; }
  inline double S(double E, double p) {
     // const double A = 14326/(4*M_PI);
     // const double B = 30.38;
     // const double C = 0.192;
     // return A/(1+exp(B*p+C));  // is there a p^2 missing? 
     //                           //  Don;t think so, but check it!
    const double A = 14025.5;
    const double B = 0.304644;
    const double C = 3.18892;
    const double D = 41.6073;
    return A*(C*p+B)*(C*p+B)/(1+exp(D*p+B));
  }
};

// Use this as a model of THREE body breakup of He3 in PWIA
// Insert Spektral function!

class PWIA_He3_3Body : public PWIA {
public:
  virtual ~PWIA_He3_3Body() { ; }
  inline double S(double E, double p) { 

// Make sure it returns 0.0 for Emiss below separation energy....
    if (E<p || E*E-p*p < sqr(m_proton+m_neutron)) return 0;

    return 1;  
  }
};
