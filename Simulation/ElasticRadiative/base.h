#ifndef __GENBASE_H_
#define __GENBASE_H_


//forward declaration to make root happy. gsl has this as an anonymous struct (gsl_(q)rng), which can not be forward declared. so we have to do some magick(tm).

struct qrng;
struct rng;


#include "generatortree.h"
#include "TVector3.h"
#include "TRotation.h"

class DaLiGenVertex;

// This is the definition of a Generator base class

class GeneratorBase
{
 protected:
  int nHaltonDim; // Number of unused dimensions
  int nBaseDim; // Number of dimensions used by the base
  double targetDensity;
  qrng *quasiRandom;
  rng *random;
  double *qrndNumbers; // This array holds that random numbers to be used by the generator each event.
  double *updatedNums; // This pointer points to the array of numbers that are being updated each event. Points to the same as above unless alwaysSameEvent.
  bool useQuasi;
  bool alwaysSameEvent;
  TVector3 beamWidthOffset;
  TRotation beamDivergenceRotation;
  TRotation beamRotation;
  int vertexDist; // (0 for triangular, 1 for fixed, 2 for uniform, 3 for entire cell, 4 for cosmic)
  DaLiGenVertex *inputVertexGen;

  void generateUniformXY(double r1, double r2, double &x, double &y);
  void generateRealisticXY(double r1, double r2, double &x, double &y);
  void generateRealisticDXDY(double r1, double r2, double &x, double &y);

 public:
  TVector3 beamDirection;
  TVector3 beamCenter;
  double beamEnergy;
  int beamCharge; //(-1 for electrons, +1 for positrons)
  double beamWidthX, beamWidthY;
  double beamDivergenceX, beamDivergenceY;

  GeneratorBase(int num_halton_dim, unsigned int skip,unsigned long int seed); //how many dimensions, how many events to skip (for halton), and the seed for 
  ~GeneratorBase();
    

  virtual double getRandom(); //gets the next random number from the Mersenne Twister implementation.

  virtual void useQuasiRandom(bool uqr);
  virtual void generateSameEvent(bool doSame);


  virtual int generate(GeneratorEvent *eventinfo); // the "typical" function.
  virtual int generateEvent(GeneratorEvent *eventinfo); //eventinfo is supplied from the outside (to write to trees directly). Return : positive: Number of tracks. Negative: Error.  This should be overloaded by the real generators.
  virtual void generateVertex(GeneratorEvent *eventinfo); //helper function to generate a new vertex position.

  virtual void setBeamEnergy(double en);
  virtual void setBeamCharge(int q);  
  virtual void setBeamOrigin(TVector3 origin);
  virtual void setBeamDirection(TVector3 dir);
  virtual void setBeamWidth(double x, double y); //in mm
  virtual void setBeamDivergence(double x, double y); //in rad
  virtual void setTargetDensity(double maxdens); //to scale energy loss
  virtual void setVertexDist(int dist);
  virtual void setVertexGenerator(char *allParamZ,char *allParamT);
  static double getMass(std::string particle);
  virtual void handleBeamGeometry(GeneratorEvent *eventinfo); // handles beam position and slope
  virtual void applyWidthAndDivergence();
};

#endif
