#include "base.h"
#include "assert.h"
#include <iostream>
#include "slowctrl.h"
#include "DaLiGenVertex.h"
#include "gsl/gsl_rng.h"
#include "gsl/gsl_qrng.h"
#include "gsl/gsl_cdf.h"

#include "CLHEP/Units/PhysicalConstants.h"

struct rng:public gsl_rng{};
struct qrng:public gsl_qrng{};


typedef struct
{
  unsigned int sequence_count;
}
halton_state_t;


GeneratorBase::GeneratorBase(int num_halton_dim, unsigned int skip,unsigned long int seed)
{
  nBaseDim = 5; //so far, we need five dimensions for the vertex;
  nHaltonDim=num_halton_dim + nBaseDim;
  //quasi random numbers
  quasiRandom=(qrng *) gsl_qrng_alloc(gsl_qrng_halton,nHaltonDim);
  qrndNumbers=new double[nHaltonDim];
  // advance state by skipstruct rng:public gsl_rng;
  
  ((halton_state_t *) gsl_qrng_state(quasiRandom))->sequence_count=skip; // we need this so we can split the simulation of a single run.
  
  //pseudo random
  random=(rng *) gsl_rng_alloc(gsl_rng_mt19937);
  gsl_rng_set(random,seed);
  
  useQuasi=false;
  alwaysSameEvent = false;
  updatedNums = qrndNumbers;

  // the default vertex distribution is the simulation distribution
  vertexDist = 0;
  inputVertexGen=new DaLiGenVertex((char *)"ztrap_1.0_24.0_1.0_-7.0", (char *)"delta");
  //inputVertexGen=new DaLiGenVertex((char *)"xyz_0.0_0.0_-20.0", (char *)"delta");
}

GeneratorBase::~GeneratorBase()
{
  if (updatedNums != qrndNumbers)
    delete[] updatedNums;
  delete[] qrndNumbers;
  gsl_qrng_free(quasiRandom);
  gsl_rng_free(random);
}

void GeneratorBase::useQuasiRandom(bool uqr)
{
  useQuasi=uqr;
}

void GeneratorBase::generateSameEvent(bool doSame)
{
  if (doSame)
    {
      if (!alwaysSameEvent)
	{
	  updatedNums = new double[nHaltonDim];
	  // Let's pick some random numbers for qrndNumbers
	  for( int i=0;i<(nHaltonDim);i++)
	    qrndNumbers[i]=gsl_rng_uniform(random);
	}
    }
  else if (alwaysSameEvent)
    {
      delete[] updatedNums;
      updatedNums = qrndNumbers;
    }
  alwaysSameEvent = doSame;
}

double GeneratorBase::getRandom()
{
  return gsl_rng_uniform(random);
}

int GeneratorBase::generate(GeneratorEvent * eventinfo)
{
  // Produce new random numbers for an event
  if (useQuasi)
    gsl_qrng_get(quasiRandom,updatedNums); //get new quasi random values
  else
    for( int i=0;i<(nHaltonDim);i++)
      updatedNums[i]=gsl_rng_uniform(random); //generate new pseudo random values
  
  eventinfo->particles.clear(); 
  eventinfo->weight.clear();

  applyWidthAndDivergence();

  generateVertex(eventinfo);

  int value = generateEvent(eventinfo);
  handleBeamGeometry(eventinfo);
  return value;
}


int GeneratorBase::generateEvent(GeneratorEvent *) //do nothing, has to be overloaded.
{
  printf("Called Base class generateEvent - no particles generated...\n");
  return 0;

}


void GeneratorBase::setBeamEnergy(double en)
{
  beamEnergy=en;
}

void GeneratorBase::setBeamCharge(int q)
{
  beamCharge = q;
}

void GeneratorBase::setBeamOrigin(TVector3 origin)
{
  beamCenter = origin;
}

void GeneratorBase::setBeamDirection(TVector3 dir)
{
  beamDirection=dir.Unit();
  TVector3 zAxis(0.,0.,-1.);
  TVector3 rotationAxis = dir.Cross(zAxis);
  beamRotation.SetToIdentity();
  beamRotation.Rotate(dir.Theta(),rotationAxis);
}

void GeneratorBase::setBeamWidth(double x, double y)
{
  beamWidthX = x;
  beamWidthY = y;
}

void GeneratorBase::setBeamDivergence(double x, double y)
{
  beamDivergenceX = x;
  beamDivergenceY = y;
}

void GeneratorBase::setTargetDensity(double maxdens)
{
  targetDensity=maxdens;
}

void GeneratorBase::setVertexDist(int dist)
{
  vertexDist = dist;
}

void GeneratorBase::applyWidthAndDivergence()
{
  // Beam width correction to beamCenter
  double x,y;
  generateRealisticXY(qrndNumbers[nHaltonDim-nBaseDim+1],qrndNumbers[nHaltonDim-nBaseDim+2],x,y);
  beamWidthOffset = TVector3(x,y,0);

  beamCenter += beamWidthOffset;

  // Beam divergence correction to beamDirection and beamRotation
  double dx,dy;
  generateRealisticDXDY(qrndNumbers[nHaltonDim-nBaseDim+3],qrndNumbers[nHaltonDim-nBaseDim+4],dx,dy);
  beamDivergenceRotation = TRotation();
  beamDivergenceRotation.RotateX(dx);
  beamDivergenceRotation.RotateY(dy);

  beamDirection.Transform(beamDivergenceRotation);
  TVector3 zAxis(0.,0.,-1.);
  TVector3 rotationAxis = beamDirection.Cross(zAxis);
  beamRotation.SetToIdentity();
  beamRotation.Rotate(beamDirection.Theta(),rotationAxis);
}

void GeneratorBase::generateRealisticXY(double r1, double r2, double &x, double &y)
{
  x = gsl_cdf_gaussian_Pinv(r1, beamWidthX);
  y = gsl_cdf_gaussian_Pinv(r2, beamWidthY);
}

void GeneratorBase::generateRealisticDXDY(double r1, double r2, double &x, double &y)
{
  x = gsl_cdf_gaussian_Pinv(r1, beamDivergenceX);
  y = gsl_cdf_gaussian_Pinv(r2, beamDivergenceY);
}

void GeneratorBase::generateUniformXY(double r1, double r2, double &x, double &y)
{
  double theta = r1 * 2. * M_PI;
  double z = sqrt(r2);
  x = 27.*z*cos(theta);
  y = 9. * z * sin(theta);
}

void GeneratorBase::generateVertex(GeneratorEvent *ev)
{
  ev->lepton_prescatter.particle=(beamCharge==-1) ? "e-" : "e+";
  ev->lepton_prescatter.momentum = TLorentzVector(0.,0.,sqrt(beamEnergy*beamEnergy - CLHEP::electron_mass_c2*CLHEP::electron_mass_c2), beamEnergy);
  G4ThreeVector vx=inputVertexGen->NextVertex();
  ev->vertex.SetXYZ(vx.x(),vx.y(),vx.z());

}

void GeneratorBase::handleBeamGeometry(GeneratorEvent *ev)
{
  // first, rotate the beam axis.
  ev->lepton_prescatter.momentum.Transform(beamRotation);
  
  // loop over final state
  for (std::vector<GeneratorParticle>::iterator it=ev->particles.begin() ;
       it != ev->particles.end() ;
       ++it)
      it->momentum.Transform(beamRotation);
}

void GeneratorBase::setVertexGenerator(char *allParamZ,char *allParamT){
  delete inputVertexGen;
  inputVertexGen=new DaLiGenVertex(allParamZ, allParamT);
  printf("%s %s\n", allParamZ, allParamT);
}

//static!
double GeneratorBase::getMass(std::string particle)
{
  if (particle=="e+" || particle=="e-")
    return  CLHEP::electron_mass_c2/CLHEP::MeV;
  
  if (particle=="proton" )
    return  CLHEP::proton_mass_c2/CLHEP::MeV;
  
  if (particle=="neutron" )
    return  CLHEP::neutron_mass_c2/CLHEP::MeV;

  if (particle=="mu-" || particle=="mu+")
    return 105.6584;

  if (particle=="gamma" )
    return  0.0;

  std::cerr<<"Could not find Mass for "<<particle<<" please add in GeneratorBase!\n";
  exit(-1);
  
  return 0;
}
