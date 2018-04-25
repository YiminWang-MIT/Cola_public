#include "Generate.h"
#include "BicubicInterpolation.h"
#include "Formfactor.h"
#include <iostream>
#include <fstream>

// http://en.wikipedia.org/wiki/7075_aluminium_alloy
// 5.6-6.1% zinc, 2.1-2.5% magnesium, 1.2-1.6% copper 
// http://www.foroiberam.org/view/download/ARGENTINA/FP3328.pdf
// Co 42.5, Cr 20, Ni 13, Fe 17.9, W 2.8, Mo 2, Mn 1.6, C 0.2

using namespace std;

extern SobolSequence sobol;

class inclusiveModel {
protected:
  double m_tgt;
  double m_thr;
public:
  inclusiveModel(double mTarget, double mThreshold) {
    m_tgt = mTarget; m_thr = mThreshold;
  }
  virtual ~inclusiveModel() { ; }
  virtual double cross_section(reaction *r, int type) = 0;
};

class deuteronModel : public inclusiveModel {
protected:
  double version;
  double x0, x1, y0, y1;
  int nx, ny;
  BicubicInterpolation *R_L, *R_T;
  DeuteronFormfactor *FF;
  int read10(ifstream& gridstream);
  double xsection10(reaction *r);
public:
  deuteronModel(double mTarget, double mThreshold);
  virtual double cross_section(reaction *r, int type);
};

int deuteronModel::read10(ifstream& gridstream)
{
  gridstream >> nx >> x0 >> x1;
  gridstream >> ny >> y0 >> y1;
  if (nx>0 && ny>0) {
    int x, y;
    double rl, rt;
    R_L = new BicubicInterpolation(nx-1, ny-1);
    R_T = new BicubicInterpolation(nx-1, ny-1);
    while (gridstream.good()) {
      gridstream >> x >> y >> rl >> rt;
      //cerr << x << ' ' << y << ' ' << rl << ' ' << rt << endl;
      R_L->init(x, y, rl);
      R_T->init(x, y, rt);
    }
    R_L->initSlope();
    R_T->initSlope();
  }
  //cout << (*R_L)(11.0, 23.0) << ' ' << (*R_T)(11.0, 23.0) << endl;
  return 0;
}

deuteronModel::deuteronModel(double mTarget, double mThreshold)
  : inclusiveModel(mTarget, mThreshold)
{
  FF = new AbbottFit3();
  char *gridfile = getenv("INCLUSIVE");
  ifstream gridstream;

  x0 = y0 = 0.0; x1 = y1 = 1.0;
  nx = ny = 0;
  R_T = R_L = NULL;
  gridstream.open(gridfile);
  if (gridstream.good()) {
    gridstream >> version;
    if (version == 1.0) read10(gridstream);
  }
  gridstream.close();
}

double deuteronModel::xsection10(reaction *r)
{
  FourVector Photon = r->electronIn - r->electronOut;
  double t = Photon.square();
  double tau = -0.25 * t / square(m_tgt);
  double q = Photon.momentum();
  double theta = r->electronOut.theta();
  double epsilon = 1.0/(1.0+2.0*(1.0+tau)*square(tan(0.5*theta)));
  double epsilonL = -t*epsilon/square(q);
  double Ebeam = r->electronIn.energy();
  double Eout = r->electronOut.energy();
  double Eprime=0.5*(square(m_thr)-2.0*Ebeam*m_tgt-square(m_tgt))/(Ebeam*(cos(theta)-1.0)-m_tgt);
  double x = Eprime>Eout ? (log(1000.0*(Eprime - Eout)) - x0) / x1 : -1.0;
  double y = (theta*180.0*M_1_PI - y0) / y1;
  if (x<-1.0) x = -1.0;
  if (x>nx) x = nx;
  if (y<-1.0) y = -1.0;
  if (y>ny) y = ny;
  //cerr << Ebeam << '\t' << Eprime << '\t' << Eout << '\t' << theta*180.0*M_1_PI << endl;
  double cs = 0.01*0.0072973525698*197.3269718*Eout*((*R_T)(x,y)+2.0*epsilonL*(*R_L)(x,y))/(2.0*square(M_PI)*Ebeam*t*(epsilon-1.0));
  //cerr << x << '\t' << y << '\t' << (*R_L)(x,y) << '\t' << (*R_T)(x,y) << '\t' << cs << endl << endl;
  if (cs != cs) cerr << Ebeam << '\t' << Eprime << '\t' << Eout << '\t' << theta*180.0*M_1_PI << endl;
  return cs;
}

double deuteronModel::cross_section(reaction *r, int type)
{
  double cs = 0.0;
  switch (type) {
  case 0: // elastic
    cs = 0.7*FF->CrossSection(r->electronIn.energy(), r->electronOut.theta());
    break;
  case 1: // breakup
    if (version == 1.0) cs = xsection10(r);
    break;
  default:
    break;
  }
  //    cout << cs << endl;
  return cs;
}

generateInclusive::generateInclusive(reaction *r, simDetectorBase *Sime, 
				     SIMUL *rundb, modeltype ModelType)
{
  generateInclusive::ModelType = ModelType;
  Label      = "dE' d[W]'";
  Unit       = "MeV sr";
  Reaction   = r;
  sime       = Sime;
  targetmass = Reaction->getTarget()->getMass();
  dcte       = sin(sime->getDtheta());
  dphie      = calc_dphie();
  dpe        = sime->getMomentum()*sime->getDmomentum();
  s2thr      =  Reaction->threshold * Reaction->threshold;
  Gamma = 1;
  model = NULL;
  if (getenv("INCLUSIVE")) {
    Label      = "d^2![s]";
    Unit       = "[m]barn";
    model = new deuteronModel(targetmass, Reaction->threshold);
  }
}

double generateInclusive::generateEvent(double)
{
  //double E0 = Reaction->electronIn.energy();
  generateLabAngles(&Reaction->electronOut, 1.0,
		    sime->getAngle(), sime->getOop(), dcte, sime->getDphi());

  double theta = Reaction->electronOut.theta();
  double phi   = Reaction->electronOut.phi();
  double Ep    = sime->getMomentum() + (sobol()*2-1)*dpe;
  Reaction->electronOut.initPolar(energy(m_electron, Ep), Ep, theta, phi);

  Particle photon = P_gamma;
  photon          = Reaction->electronIn - Reaction->electronOut;
  FourVector CMS  = photon + *Reaction->getTarget();
  int inelastic = 0;
  if (CMS.square() < s2thr) {
    //double Eel   = E0/(1+E0/targetmass*(1-cos(theta)));
    //Reaction->electronOut.initPolar(energy(m_electron, Eel), Eel, theta, phi);
    return (model ? 0.0 : 1.0);
  } else inelastic = 1;

  return (model ? model->cross_section(Reaction, inelastic) : 1.0);
}

double generateInclusive::integrationVolume()
{
  return 8.0 * dcte * sime->getDphi() * dpe; // * 4pi * 1000
}
