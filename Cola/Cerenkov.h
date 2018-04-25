//                                                                    -*-c++-*-
// $Id: Cerenkov.h 2516 2013-08-16 09:54:35Z kohly $
//

#include "Aqua_abcn.h"
#include "Aqua_online.h"
#include "VDCraw.h"
#include <cstdio>

/// Base class for cerenkov detector
class cerenkovDetector {
protected:
  struct CERENKOV *rundbcer;
  struct cerenkov *onl;
public:
  virtual ~cerenkovDetector() { ; }
  /// calculate rawdata and fill online structure
  virtual void rawdata(AquaTree *atree, vdcBase *vdc, int nr) = 0;
  virtual void GetPedestalsMax() = 0;
  virtual void LoadPedestals() = 0;
};


/// Cerenkov detector for spectrometer A and C
class cerenkovAC : public cerenkovDetector {
private:
  struct MpsCerenkovAC *cer;
  int CheckPedestalsAC[12][1000];
  float Pedestals[12];
  FILE *out;
  FILE *in;
public:
  virtual ~cerenkovAC() { ; }
  /// constructor
  cerenkovAC(struct cerenkov *onlinestruct, struct MpsCerenkovAC *ac,
	     struct CERENKOV *rdb) 
  {
    onl = onlinestruct;
    cer = ac;
    rundbcer = rdb;
    
    for (int j=0; j<12; j++) {
      for (int i=0; i<1000; i++) CheckPedestalsAC[j][i]=0;
      Pedestals[j] = 0;
    }
   
    if (rundbcer->FloatingPed==1) {
      out=fopen(rundbcer->PedFile,"w");
      if (!out) perror("Can't open output file for Cerenkov");
    }
    if (rundbcer->FloatingPed>=2 && rundbcer->FloatingPed!=17) {
      in=fopen(rundbcer->PedFile,"r");
      if (!in) perror("Can't open input file for Cerenkov");
    } 
  };
  
  /// calculate rawdata and fill online structure
  void rawdata(AquaTree *atree, vdcBase *vdc, int nr);
  
  void GetPedestalsMax();
  void LoadPedestals();
};

/// Cerenkov detector for spectrometer B
class cerenkovB : public cerenkovDetector {
private:
  struct MpsCerenkovB *cer;
  int CheckPedestalsB[5][1000];
  float Pedestals[5];
  FILE *out;
  FILE *in;
public:
  virtual ~cerenkovB() { ; }
  /// constructor
  cerenkovB(struct cerenkov *onlinestruct, struct MpsCerenkovB *b,
	     struct CERENKOV *rdb) 
  {
    onl = onlinestruct;
    cer = b;
    rundbcer = rdb;
    
    for (int j=0; j<5; j++) {
      for (int i=0; i<1000; i++) CheckPedestalsB[j][i]=0;
      Pedestals[j] = 0;
    }
     
    if (rundbcer->FloatingPed==1) {
      out=fopen(rundbcer->PedFile,"w");
      if (!out) perror("Can't open output file for Cerenkov");
    }
    if (rundbcer->FloatingPed>=2 && rundbcer->FloatingPed!=17) {
      in=fopen(rundbcer->PedFile,"r");
      if (!in) perror("Can't open input file for Cerenkov");
    } 
 };
  
  /// calculate rawdata and fill online structure
  void rawdata(AquaTree *atree, vdcBase *vdc, int nr);

  void GetPedestalsMax();
  void LoadPedestals();
};
