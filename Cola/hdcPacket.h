//								      -*-c++-*-
// Created 1997 by	T. Pospischil,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
//
/// hdcPacket definitions
//

#include "hdcPlane.h"

const int HDC_WIRES = 103;         /// number of wires / plane

const double DC_DIST = 220;        /// distance between the two U-planes (mm). 
const double WP_DIST  = 20;        /// distance between U and V plane (mm).

class HdcPacket {
public:
  void Reset(void);			/// reset internal memory after
                                        /// calculation.

  int ok(void)                          /// A single wire or two neighbour
  { return Ok; }                        /// wires in all planes have a signal

  void NewConfig(char *filename);       /// Reads new TDC offsets and  
                                        /// ODD/Even setup (lim,side) from file

  void NewDriftParas(HDC aHDC);         /// Sets new driftpara. for all planes 

  void InitHDC(HDC aHDC);               /// does NewConfig and NewDriftparas

  void AddToOffset(int AddOffset);      /// Adds AddOffset to the TDC offsets
                                        /// of all wire planes

                                        /// Shifts OE limits of all wire planes
  void AddToOElim(int lim_shift_U1, int lim_shift_V1,
		  int lim_shift_U2, int lim_shift_V2);

  int FillOnline(AquaTree *out, struct online *onl);

protected:
  char *NameInData;
  int Ok;
  HdcPlane *U1plane, *U2plane, *V1plane, *V2plane; 
  double u1, v1, u2, v2, u2_u1, v2_v1;
  void MemInit();

public:
  HdcPacket(AquaTree *, const char *);
  ~HdcPacket(void);
  int handle();
};





















