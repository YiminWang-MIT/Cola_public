//								      -*-c++-*-
// Created 1995 by	M.O. Distler + T.Pospischil,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// Last Change: T.Pospischil, 10.11.96, HDC V 1.0 
//              T.Pospischil, 31.07.97, changed callback routine for abcn
//
/// Header File for HDC Library
//

#ifndef __HdcPlane__
#define __HdcPlane__

#include "rundatabase.h"
#include "Aqua_online.h"

struct OEdef {
  int lim;              /// threshold of ADC-spectrum
  int side;             /// if side=0 all events lower than lim are assigned  
                        /// to the side with the lower wire no. and those 
                        /// events above lim to the side with the higher
                        /// wire no. (and vice versa in case of side=1) 
};

struct hdcRawData {
  short wire;
  short struct_len;      // dummy, always has to be 2
  short time;
  short oddeven;
};

class HdcPlane {
public:
  short NumberOfWires(void)		/// returns the number of wires
  { return num_of_wires; }		/// which have fired

  short NumberOfGoodWires(void)		/// returns the number of wires
  { return num_of_good_wires; }		/// with positive Drifttime 

  const char *NameOfPlane(void) 	/// returns the name of the Plane
  { return Name; }

  const hdcRawData * lowTime(void)	/// return structure with
  { return &low_time; }		        /// shortest drift time

  const hdcRawData * posTime(void)	/// return structure with
  { return &pos_time; }		        /// drift time used for position calc

  const short *WiresFired(void)	        /// returns a pointer to the array
  { return wire; }			/// containing the wire numbers

  const int *TimeOff(void)	        /// returns a pointer to the array
  { return tOffset; }			/// containing the time offsets

  const OEdef *OE_Conf(void)	        /// returns a pointer to the array
  { return oe_Conf; }			/// containing the OddEven infos

  const short *RawDriftTimes(void)	/// returns a pointer to the array
  { return time; }			/// containing the drift times

  float x(void)                         /// returns Koordinate in mm
  { return xpos; }
  
  float XCell(void)                     /// Distance from signal wire
  { return xcell; }                     

  int ok(void)                          /// returns ok-value (changed in
  { return Ok; }                        /// Sep 97) for the wire plane

  void Reset(void);			/// reset internal memory after
                                        /// calculation.

  void NewDriftParas(DPARA dtp);        /// Sets new driftparameters

  void NewConfig(char *filename);       /// Reads new TDC offsets and  
                                        /// ODD/Even setup (lim,side) from file

  void InitHDC(int* NewOffsets, OEdef* oe); /// Initializes the TDC offsets
                                        /// and ODD/Even setup (lim,side)

  void InitOffset(int* NewOffsets);     /// Initializes the TDC offsets
                                        /// (upper ends of tsum-spectra)

  void AddToOffset(int AddOffset);      /// Adds AddOffset to the TDC offsets

  void AddToOElim(int lim_shift);       /// Adds lim_shift to the OE limits

  int FillOnline(AquaTree *out,         /// fills data into online structur
		 hdc_plane &wp);        

  //  DON'T USE THIS FUNCTION! IT MAKES RESOLUTION WORSE!! :
  int corrDriftDistance(double angle);  // corrects xpos and xcell for a 
                                        // particle passing the plane
                                        // with an angle (rad) to the normal

protected:
  char   Name[255];
  int    *tOffset;
  OEdef  *oe_Conf;
  short  *wire, *time, *oe;
  short  num_of_wires;                
  short  num_of_good_wires;                
  hdcRawData *wiretime;
  hdcRawData low_time;
  hdcRawData positive_low_time;
  hdcRawData pos_time;
  short  max_wire;
  void   MemInit(const char *, int);
  float  xpos, xwire, xcell;
  int    Ok;
  float  dtpara[9];
  float  bigdtlim, bigdtslope, negdtslope;
  AquaShortSubTree *subtree;
public:
  HdcPlane(const HdcPlane &);		/// constructors
  HdcPlane(const char *, int);
  HdcPlane(AquaShortSubTree *, const char *, int);
  ~HdcPlane(void);
  int FillTime(hdcRawData *, int);	/// callback routine
  int handle();
};

#endif















