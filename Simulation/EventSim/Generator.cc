
#include "Generator.h"
#include "Model/Model.h"
#include "Cola/Radiation.h"
#include <iostream>
#include <string>
#include <cstring>

using std::string;
#define FILE "Generator: "

modeltype ModelType;

Generator::Generator(AquaIO *io)
{

  EVDEBUG("Constuctor")

  // copy all we need!
  BeamEnergy   = rundb.Ebeam / 1e3;
  simwobx=rundb.sim.wobx;
  simwoby=rundb.sim.woby;
  beamoffsetx=rundb.beam.offset.x;
  beamoffsety=rundb.beam.offset.y;
  targetoffset_simx=rundb.Target.offset_sim.x;
  targetoffset_simy=rundb.Target.offset_sim.y;


  // for later:
  simq2=rundb.sim.q2;
  simq2_range=rundb.sim.q2_range;


  EVDEBUG((string("Reaction:")+string(rundb.reaction)))

  reactionstring=rundb.reaction;
  

  BeamMomentum=sqrt(BeamEnergy*BeamEnergy - m_electron*m_electron);


  // Setting up reaction H(e,A:e')p
  rea=new reaction(reactionstring.c_str());
  rea->electronIn.setMomentum(0,0,BeamMomentum);

  EVDEBUG("Reaction constructed")

  //Setting up Cryo Target
  tgt=SetTargetFromRunDB(rea);

 EVDEBUG("Target constructed");

  int Andp=0, Bndp=0, Cndp=0, Dndp=0;
  for (int i=0; i<20; i++) {
    if (rundb.A.angularRatio[i] != -1 &&
	rundb.A.momRatio[i]     != -1 &&
	rundb.A.dpcuts[i]       != -100) Andp++;
    if (rundb.B.angularRatio[i] != -1 &&
	rundb.B.momRatio[i]     != -1 &&
	rundb.B.dpcuts[i]       != -100) Bndp++;
    if (rundb.C.angularRatio[i] != -1 &&
	rundb.C.momRatio[i]     != -1 &&
	rundb.C.dpcuts[i]       != -100) Cndp++;
    if (rundb.D.angularRatio[i] != -1 &&
	rundb.D.momRatio[i]     != -1 &&
	rundb.D.dpcuts[i]       != -100) Dndp++;
  }


  //Setting up Spektrometer-Collicheck
  simA = new simSpecEventSim( - rundb.A.angle * rad, 
			       rundb.A.angularResolution  / 1000,
			       rundb.A.angularResolution2 / 1000,
			       rundb.A.verticalAngleRatio,
			       rundb.A.momentum / 1e3, 
			       rundb.A.momResolution, rundb.A.momResolution2, 
			       Andp, rundb.A.dpcuts,
			       rundb.A.angularRatio, rundb.A.momRatio, 
			       refmomA, 20/2.0, rundb.A.collimator, ColliAList,
			       (int) rundb.A.vacuum, rea->getA(),
			      NULL,NULL,
			       0.110, 0.08, 0.10*1.2, rundb.A.oopangle * rad);

  EVDEBUG("simA constructed")


  // Spectrometer B
  if (rundb.B.left) rundb.B.angle  = -rundb.B.angle;

  double Bdphi   = 0.025;
  double Bdtheta = 0.080;
  if (!strncmp(rundb.B.collimator, "(", 1)) {
    double horiL = atof(rundb.B.collimator+1);
    double horiR = atof(rundb.B.collimator+5);
    double vertT = atof(rundb.B.collimator+11);
    double vertB = atof(rundb.B.collimator+15);
    Bdphi   = (horiL>horiR?horiL:horiR)/1000.0 + 0.005; 
    Bdtheta = (vertT>vertB?vertT:vertB)/1000.0 + 0.005; 
  }
  
  simB = new simSpecEventSim( rundb.B.angle * rad, 
			       rundb.B.angularResolution  / 1000,
			       rundb.B.angularResolution2 / 1000,
			       rundb.B.verticalAngleRatio,
			       rundb.B.momentum / 1e3, 
			       rundb.B.momResolution, rundb.B.momResolution2,
			       Bndp, rundb.B.dpcuts,
			       rundb.B.angularRatio, rundb.B.momRatio, 
			       refmomB, 15/2.0, rundb.B.collimator, ColliBList,
			       (int)rundb.B.vacuum, rea->getB(),
			     NULL,NULL,
			       Bdphi, Bdtheta, 0.075 * 1.2,
			       - rundb.B.oopangle * rad);

  EVDEBUG("simB constructed")
        gen = new generateElastic(rea,simA,simB,&rundb.sim);
  //gen = new generateElasticProton(rea,simA,simB,&rundb.sim);

  EVDEBUG("generateElastic constructed")

  evnum=0;
}

Generator::~Generator()
{
  delete rea;
  delete tgt;
  delete simA;
  delete simB;
  delete gen;

}

GenState * Generator::Generate()
{
  evnum++;
  myState.EventNumber=evnum;


  static PseudoRandom psrandom;
  static normal norm(&psrandom, &psrandom);


  double random[3] = {0,0,0};
  do{
    sobol.nextValues();
    random[2] = sobol();
  } while (!tgt->Generate_Vertex(random, myState.TargetCoords, simwobx, 
				    simwoby, ModelType)); 
  
#ifdef EVLOGFLAG
  std::cerr << "Generator: Target: "<<myState.TargetCoords[0]<<"\t"<<myState.TargetCoords[1]<<"\t"<<myState.TargetCoords[2]<<std::endl;
#endif

  myState.Volume = gen->integrationVolume();
  myState.Weight = gen->generateEvent(((evnum & 1)==0)?1:-1);
 
 
  rea->electronIn.setMomentum(0, 0, BeamMomentum);
  //  tgt->EnergyLossSimBeam(rea->electronIn, myState.TargetCoords[0],myState.TargetCoords[1],myState.TargetCoords[2],1); //1=steps_beam

  myState.trigger[SPEKA]=(0!=simA->check(rea->electronOut,myState.TargetCoords,tgt,&myState.tc[SPEKA], NULL, NULL, ModelType));
  rea->electronIn.setMomentum(0, 0, BeamMomentum);
  // double E0 = rea->electronIn.energy();
  // double q2      = (rea->electronIn - rea->electronOut).square();
  // double theta_e = rea->electronOut.theta(); 
  // double Eprime  = rea->electronOut.momentum(); 
  // myState.Weight *= vertexCorrection(q2, E0, Eprime, theta_e);

#ifdef EVLOGFLAG
  std::cerr<<"Generator: Volume: "<<myState.Volume<<"\tWeight: "<<myState.Weight<<"\tHelicity: "<<((evnum & 1 )?1:-1)<<std::endl;
#endif


  
#ifdef EVLOGFLAG
  std::cerr << "Generator: SimA: TC:\n";
  std::cerr<<"\t\tdp: "<<myState.tc[SPEKA].dp<<"\n";
  std::cerr<<"\t\tth: "<<myState.tc[SPEKA].th<<"\n";
  std::cerr<<"\t\ty0: "<<myState.tc[SPEKA].y0<<"\n";
  std::cerr<<"\t\tph: "<<myState.tc[SPEKA].ph<<"\n";
  std::cerr<<"\t\tlen: "<<myState.tc[SPEKA].len<<"\n";
#endif

 

 return &myState;
}
