//
// Created 1995 by	M.O. Distler + T. Pospischil,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// Last Change: T.Pospischil, 10.11.96, HDC V 1.0 
//              T.Pospischil, 31.07.97, changed callback routine for abcn
//              T.Pospischil, 04.09.97, HDC ok values
//
/// Implementation of class hdcPlane
//

#define _DEFAULT_SOURCE 1
#include "hdcPlane.h"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>

/** - readWireConf --- reads TDC offset and OE infos of one wire */

int readWireConf(FILE *f, const char* name, int& toffset, OEdef& oevar)
{
  char inputline[255];
  int readval1, readval2, readval3;

  /* go to top of files: */
  rewind(f);
  /* read file line by line: */
  while (1){
    /* if end of file -> exit (keyword not found) */
    if (fgets(inputline,255,f)==NULL) {
      std::cerr << " Could not read Config-information for wire "
		<< name << std::endl;
      return 1;
    }
    /* first letter '#' -> line contains comment, go to next line: */
    if (inputline[0]=='#') continue;
    /* Does the line contain the keyword ? */
    if (strstr(inputline,name)){
      /* read second value as number: */
      if (sscanf(inputline,"%*s%d%d%d",&readval1, &readval2, &readval3)==3){
	toffset        = readval1;
	oevar.lim      = readval2;
	oevar.side     = readval3;
	if (oevar.side && (oevar.side!=1)) {
	  std::cout << " WARNING: oevar.side of " << name
		    << " not 0 or 1 !! " << std::endl;
	}
	// std::cout << name << "\t " << toffset << "\t " << oevar.lim;
	// std::cout << "\t " << oevar.side << std::endl;
        return 0;
      }
      else {
        std::cerr << " Keyword " << name
		  << " found, but value can't be read !!" << std::endl;
        return 1;
      }
    }
  }
  return 1;
}

/** -readHDCconfig --- read HDC infos: ---------------------------- */

int readHDCconfig(const char* filename, const char* namepref, int* toffs, 
		  OEdef* oe_inf, int num)
{
  FILE *cfg_file = fopen(filename, "r"); 

  if (!cfg_file) {
    std::cerr << "ERROR in readConfig of hdcplane: Cannot open File\n"; 
    return 1;
  }

  for(int i=0; i<num; i++){
    char varname[32];
    sprintf(varname, "%s_%d", namepref, i);
    if (readWireConf(cfg_file, varname, toffs[i], oe_inf[i])) {
      std::cerr << "ERROR in readConfig of hdcplane" << std::endl;
      exit(1);
    }
  }
  fclose(cfg_file);
  return 0;
}


/// ++++++++++++++++++ Functions of class HdcPlane: ++++++++++++++++++++++

int HdcPlane::handle()
{
  if (!subtree || !wiretime) return -1;

  int len = subtree->pack((u_int16_t *)wiretime, max_wire, 2);
  
  return FillTime(wiretime, 4*len);
}

/// ----------------------------------- preevaluation of rawdata (callback):
int
HdcPlane::FillTime(hdcRawData *cht, int len)
{
    int i;
    int hits_with_neg_time;
    short tmptime;

    positive_low_time.time = 4096;
    positive_low_time.wire = -1;
    hits_with_neg_time = 0;
    num_of_good_wires = 0;


    // +++++++++++++++++++++++++++++ loop over buffer with all wire infos 
    for (i = 0 ; i < (len/4) ; i++) {

      // unpack wire infos
      wire[i] = cht[i].wire - 1;
      if ((wire[i] < 0) || (wire[i] >= max_wire)) {
	std::cerr << std::endl << " WARNING HDC " 
		  << Name << " wire (" << (i+1) << " of " 
		  << (len/4) << ") number out of range: " 
		  << wire[i] << std::endl;
	Ok = 0; return 1;
      }
      // if (!((online.nr+1) % 1000)) {
      // std::cerr << std::endl << " Event # " << (online.nr+1) 
      //      << " HDC " << Name << " wire (" << (i+1) << " of " 
      // 	     << (len/4) << ") "
      //	     << wire[i] << std::endl;
      // }
      time[i] = cht[i].time;
      oe[i]   = cht[i].oddeven - oe_Conf[wire[i]].lim;
      tmptime = tOffset[wire[i]] - time[i];
      num_of_wires++;

      // store wire with shortest drifttime
      if (tmptime < low_time.time) {
	low_time.time = tmptime;
	low_time.wire = wire[i];
	low_time.oddeven = oe[i]; 
      }
      
      // store wire with shortest positive drifttime
      if ( (tmptime >= 0) && (tmptime < positive_low_time.time) ) {
	positive_low_time.time = tmptime;
	positive_low_time.wire = wire[i];
	positive_low_time.oddeven = oe[i];
      }
    
      // count hits with positive and negative drifttime separatly 
      if (tmptime < 0) hits_with_neg_time++;
      else num_of_good_wires++;
    }
    

    // ++++++++++++++++++++++++++++++++++++++++++++++ calculate driftlength 
    if (num_of_wires){
      
      // take positive drifttime if there is one
      if (num_of_good_wires) {
	pos_time.time    = positive_low_time.time; 
	pos_time.wire    = positive_low_time.wire; 
	pos_time.oddeven = positive_low_time.oddeven; 
      }
      else {
	pos_time.time    = low_time.time; 
	pos_time.wire    = low_time.wire; 
	pos_time.oddeven = low_time.oddeven; 
      }

      // apply polynom for drifttime-driftway relation:
      if (pos_time.time < 0) xcell = negdtslope * pos_time.time;
      else if (pos_time.time > bigdtlim) 
	xcell =	 dtpara[0]  + bigdtlim *           // a0
	  (      dtpara[1]  + bigdtlim *           // a1
	  (      dtpara[2]  + bigdtlim *           // a2
	  (      dtpara[3]  + bigdtlim *           // a3
	  (      dtpara[4]  + bigdtlim *           // a4
	  (      dtpara[5]  + bigdtlim *           // a5
	  (      dtpara[6]  + bigdtlim *           // a6
	  (      dtpara[7]  + bigdtlim *           // a7
		 dtpara[8]                )))))))  // a8
	  + bigdtslope * (pos_time.time-bigdtlim) 
	  * 0.05 * (5 + 15*exp(-1.5*(pos_time.time-bigdtlim)/bigdtlim));
      else 
	xcell =	 dtpara[0]  + pos_time.time *      // a0
	  (      dtpara[1]  + pos_time.time *      // a1
	  (      dtpara[2]  + pos_time.time *      // a2
	  (      dtpara[3]  + pos_time.time *      // a3
	  (      dtpara[4]  + pos_time.time *      // a4
	  (      dtpara[5]  + pos_time.time *      // a5
	  (      dtpara[6]  + pos_time.time *      // a6
	  (      dtpara[7]  + pos_time.time *      // a7
		 dtpara[8]                ))))))); // a8

      // position of the wire along the wireplane:
      xwire = (pos_time.wire * 20.0 + 10);

      // left-right decision and x calc:
      if (((pos_time.oddeven > 0)) ^ (oe_Conf[pos_time.wire].side)) 
	xpos = (xwire-xcell); 
      else xpos = (xwire+xcell);
    }

    // +++++++++++++++++++++++++++++++++++++++++++++++++ determine ok type:
    // no hit at all:
    Ok = 0;
    // single hit with positive drifttime:
    if (num_of_good_wires == 1) Ok = 1;
    // double hit with two positive drifttimes:
    else if (num_of_good_wires == 2) {
      // find the two good_wires:
      int nn[2], zn; zn = 0;
      for(i=0; i<num_of_wires; i++)
	if ((tOffset[wire[i]] - time[i])>=0) nn[zn++] = i;
      // look for neighbour wires
      if ( abs(wire[nn[1]]-wire[nn[0]]) == 1 ) {
	if ( ( (tOffset[wire[nn[0]]] - time[nn[0]]) +
	       (tOffset[wire[nn[1]]] - time[nn[1]])   ) > 1500 ) Ok = 2; 
	// time sum too small for a real driftcell border track -> cross talk 
	else Ok = 18;
      }
      // two non-neighbour wires
      else Ok = 10;
    }
    // three or more wires with positive drifttimes:
    else if (num_of_good_wires >= 3) Ok = 8;
    // mark hits with negative drifttime
    // make single neg.-time hit worse than the ones with also positive times
    if (hits_with_neg_time) { Ok += 4; if (Ok==4) Ok = 7; }

    return 0;
}

/// ------------------------------------- functions for offset initialisation

void
HdcPlane::InitOffset(int* NewOffsets)                    /// new time offsets
{
  int i;
  for(i=0;i<max_wire;i++) tOffset[i] = NewOffsets[i];
}

void 
HdcPlane::AddToOffset(int AddOffset)    /// Adds AddOffset to the TDC offsets
{
  int i;
  for(i=0;i<max_wire;i++) tOffset[i] += AddOffset;
}

void 
HdcPlane::AddToOElim(int lim_shift)       /// Adds lim_shift to the OE limits
{  
  int i;
  for(i=0;i<max_wire;i++) oe_Conf[i].lim  += lim_shift;   
}
  
  
void            /// Initializes the TDC offsets and the OddEven-Configuration
HdcPlane::InitHDC(int* NewOffsets, OEdef* oe) 
{              
  int i;
  for(i=0;i<max_wire;i++) {
    tOffset[i] = NewOffsets[i];
    oe_Conf[i].lim  = oe[i].lim;
    oe_Conf[i].side = oe[i].side;
  }
}

void /// reads new TDC offsets and the OddEven-Configuration from config-file 
HdcPlane::NewConfig(char *filename) {
  if (readHDCconfig(filename, Name, tOffset, oe_Conf, max_wire)) exit(1);
}

void 
HdcPlane::NewDriftParas(DPARA dtp) {        /// Sets new driftparameters
  int i;
  if (dtp.bigdtlim != 0.0)   bigdtlim   = dtp.bigdtlim; 
  bigdtslope = dtp.bigdtslope; 
  negdtslope = dtp.negdtslope;
  for(i=0; i<9; i++) dtpara[i] = dtp.dtpara[i];
}  

void                                /// should be called after every event:
HdcPlane::Reset()
{
    num_of_wires = 0;
    num_of_good_wires = 0;
    low_time.wire = -1;
    low_time.time = 4096;
    pos_time.wire = -1;
    pos_time.time = 4096;
    xpos = 0;
    Ok = 0;
}

void              /// things to be done in the beginning (mem alloc + init)
HdcPlane::MemInit(const char *name, int size)
{
  int i;
  max_wire   = size;
  subtree = NULL;
  wiretime = NULL;
  wire  = new short[size];
  time  = new short[size];
  oe    = new short[size];
  strcpy(Name,name);
  tOffset = new int[size]; 
  oe_Conf = new OEdef[size];
  for(i=0;i<size;i++) { 
      tOffset[i] = 3200; 
      oe_Conf[i].lim  = 2600;
      oe_Conf[i].side = 1;
  }
  num_of_wires = 0;
  xpos = 0;
  Ok = 0;
  low_time.wire = -1;
  low_time.time = 4096; 
  num_of_good_wires = 0;
  pos_time.wire = -1;
  pos_time.time = 4096;

  // drifttime-driftlength relation parameters 
  // just for initialisation purpose here, should be read again out of run.db
  bigdtlim   = 1000; 
  bigdtslope = 0.00333; 
  negdtslope = 0.00143; 
  dtpara[0]  =   -0.034703   ;
  dtpara[1]  =    0.018168   ;
  dtpara[2]  =  -2.8452e-05  ;
  dtpara[3]  =   1.76652e-07 ;
  dtpara[4]  =  -7.04523e-10 ;
  dtpara[5]  =   1.45702e-12 ;
  dtpara[6]  =  -1.64336e-15 ;
  dtpara[7]  =   9.67842e-19 ;
  dtpara[8]  =  -2.33462e-22 ;
}

/// ------------------------------------------------------- corrDriftDistance
/// corrects xpos and xcell for a particle going not perpendicular
/// but under an angle (to the normal in x-direction) through the plane i
///
///  DON'T USE THIS FUNCTION! IT MAKES RESOLUTION WORSE!!
///
int 
HdcPlane::corrDriftDistance(double angle)
{  
  // look if the angle is between -90 and 90 deg:
  if ((angle < -1.5707963) || (angle > 1.5707963)) return 1;

  xcell /= cos(angle);

  // left-right decision and x calc:
  if (((pos_time.oddeven > 0)) ^ (oe_Conf[pos_time.wire].side)) 
    xpos = (xwire-xcell); 
  else xpos = (xwire+xcell);
  
  return 0;
}

/// ------------------------------------------------------- for callback

int 
scheduleHdc(unsigned short *data, int len, char *ptr) 
{                                           /// links callback routine
  return ((HdcPlane *) ptr) -> FillTime((hdcRawData *)data,len);
}

  
/// ------------------------------------------------------- Constructors:

/// Copy-Constructor:
HdcPlane::HdcPlane(const HdcPlane & plane)
{
  MemInit( plane.Name, plane.max_wire);
  strcpy( Name, plane.Name );
  memcpy(tOffset, plane.tOffset,max_wire *sizeof(int));
  memcpy(oe_Conf, plane.oe_Conf,max_wire *sizeof(OEdef));
  memcpy(wire,    plane.wire,   max_wire *sizeof(short));
  memcpy(time,    plane.time,   max_wire *sizeof(short));
  memcpy(oe,      plane.oe,     max_wire *sizeof(short));
  num_of_wires = plane.num_of_wires;
  num_of_good_wires = plane.num_of_good_wires;
  low_time.wire       = plane.low_time.wire;
  low_time.struct_len = plane.low_time.struct_len;
  low_time.time       = plane.low_time.time;
  low_time.oddeven   = plane.low_time.oddeven;
  positive_low_time.wire       = plane.positive_low_time.wire;
  positive_low_time.struct_len = plane.positive_low_time.struct_len;
  positive_low_time.time       = plane.positive_low_time.time;
  positive_low_time.oddeven    = plane.positive_low_time.oddeven;
  pos_time.wire       = plane.pos_time.wire;
  pos_time.struct_len = plane.pos_time.struct_len;
  pos_time.time       = plane.pos_time.time;
  pos_time.oddeven    = plane.pos_time.oddeven;
  max_wire = plane.max_wire;
  xpos = plane.xpos; xwire = plane.xwire; xcell = plane.xcell;
  Ok = plane.Ok;
  for (int i=0; i<=8; i++) dtpara[i] = plane.dtpara[i];
  bigdtlim = plane.bigdtlim;
  bigdtslope = plane.bigdtslope;
  negdtslope = plane.negdtslope;
}

HdcPlane::HdcPlane(const char *name, int size)
{
  MemInit(name,size);
}

HdcPlane::HdcPlane(AquaShortSubTree *sub, const char *name, int size)
{
  MemInit(name, size);
  wiretime = new hdcRawData[size];
  subtree = sub;
}

/// -------------------------- fill and sort data in online structure: 
int 
HdcPlane::FillOnline(AquaTree *out, hdc_plane &wp)
{
  wp.ok = Ok;
  if ( (wp.multi=num_of_wires) ){
    wp.low_time  = low_time.time;
    wp.pos_time  = pos_time.time;
    wp.wire      = pos_time.wire;
    wp.oe        = pos_time.oddeven;
    wp.x     = xpos;
    wp.xcell = xcell;

    out->packEventData(&(wp.pos_time), 5);
    out->packEventData(&(wp.x), 2);
  } else out->packEventData(&(wp.multi), 1);
  out->packEventData(&(wp.ok      ), 1);

  return 0;
}

// ------------------------------------------------------------ Destructor:
HdcPlane::~HdcPlane()
{
  delete wire;
  delete time;
  delete oe;
  delete tOffset;
  delete oe_Conf;
}








































