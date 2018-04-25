//
// Created 1993-2014 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Chamber/vdcPlane.cc,v 2.25 2004-09-16 13:18:35 distler Exp $
//
// Implementation of class VdcPlane
//

/*
a               = 0.89134          +/- 0.3167       (35.53%)
b               = -1.84967         +/- 0.7558       (40.86%)
c               = 1.42821          +/- 0.593        (41.52%)
d               = -0.218862        +/- 0.1531       (69.94%)
f               = 0.00744596       +/- 0.004843     (65.04%)
g               = -0.00482992      +/- 0.001027     (21.27%)
h               = 0.000378187      +/- 6.146e-05    (16.25%)
 
korr(t,slope)= a+b*slope+c*slope²+d*slope³ +f*|t|+g*t**2+h*|t|**3 

*/

static const char rcsid[] =
 "@(#)$Id: vdcPlane.cc 2560 2014-05-21 14:51:35Z distler $";

#include "chlib++.h"
#include "vdcPlane.h"
#ifdef LIKELIHOOD
#include "vdcLikelihood.h"
#else
#include "vdcRegression.h"
#endif
#include <stdlib.h>
#include <iostream>
#include <string.h>

// CorrectionPar[0] is parameter A
// CorrectionPar[1-4] is used for polynomial B
// see diploma thesis of M.Distler page 27f
// http://wwwa1.kph.uni-mainz.de/A1/publications/diploma/distler.ps.gz

const double CorrectionPar[5] = {
  //  2.0, -0.04479051, 0.1588897, 0.3861646, -0.06223269
  1.1, -0.12458, 0.36502, 0.22858, -0.024383
};
// Parameter von Frank Heinemann
// 2.0, -0.2225, 0.4695, 0.2573, -0.02564

double VdcPlane::driftTimeCorrFun(double a)
{
  if (a<0.0) a = -a;
  if ((10.0<a)||(a<0.1)) return 0.0;
  return ((corrPar[4] * a + corrPar[3]) * a + corrPar[2]) * a + corrPar[1];
}

void
VdcPlane::reverseSign()
{
  for (int i=0;i<good_wires;i++)
    path[i] *= -1;
}

void
VdcPlane::ignoreWire(short index)
{
  float tmp_path = path[index];
  float tmp_coord = coord[index];
  short tmp_wire = wire[index];
  short tmp_time = time[index];
  short i;

  --good_wires;
  for (i=index;i<good_wires;i++) {
    path[i] = path[i+1];
    coord[i] = coord[i+1];
    wire[i] = wire[i+1];
    time[i] = time[i+1];
  }
  path[i] = tmp_path;
  coord[i] = tmp_coord;
  wire[i] = tmp_wire;
  time[i] = tmp_time;

  float min_path = full_range * time_to_length;
  num_of_sign_rev = 0;
  for (i=0; i<good_wires; i++) {
    if (i>0) path_diffs[i-1] = path[i] - path[i-1];
    if (i>1) num_of_sign_rev += oppositSign(path_diffs[i-2],path_diffs[i-1]);
    if (min_path > path[i]) {
      central_wire = i;
      min_path = path[i];
    }
  }
}

int
VdcPlane::disableWire(int wire, int count)
{
    for ( ; count>0 ; count--, wire++ ) {
	if ((wire < 0)||(last_wire < wire))
	    return 0;
	disabledWire[wire] = 'x';
    }

    return 1;
}

void
VdcPlane::MemInit(const char *name, int size)
{
  first_wire  = 0;
  last_wire   = size-1;
  max_wire    = size;
  position    = new float[size];
  offset      = new short[size];
  paddle      = new short[16];
  wire        = new short[size];
  tmp_disable = new short[size];
  coord       = new float[size];
  time        = new short[size];
  times       = new float[size];
  path        = new float[size];
  corr        = new float[size];
  Name        = new char[strlen(name)+1];
  strcpy(Name,name);
  path_diffs = new float[size-1];
#ifdef LIKELIHOOD
  fit = new VdcLikelihood;
#else
  reg = new VdcRegression;
#endif
  time_to_length = TIME_TO_LENGTH;
  full_range = TDC_FULL_RANGE;
  disabledWire = new char[size];
  for (int s=0 ; s<size ; s++) {
    disabledWire[s] = 0;
    offset[s] = 0;
    position[s] = 0.0;
  }
  for (int p = 0; p < 16; p++) paddle[p] = 0;
  min_drift = 0.0;
  default_slope = 0;
  for (int i=0; i<5; i++) corrPar[i] = CorrectionPar[i];
  //  corrFunction  = KORREKTUR;
  //  corrParameter = KORREKTUR_PAR;
  Reset();
}
  
VdcPlane::VdcPlane(const VdcPlane & plane)
{
  MemInit( Name,  plane.max_wire );
  first_wire    = plane.first_wire;
  last_wire     = plane.last_wire;
  max_wire      = plane.max_wire;
  default_slope = plane.default_slope;
  ignore_mode   = plane.ignore_mode;
  ignore_min    = plane.ignore_min;
  ignore_max    = plane.ignore_max;
  ignore_slope  = plane.ignore_slope;
  memcpy(position,plane.position,max_wire*sizeof(float));
  memcpy(offset,plane.offset,max_wire*sizeof(short));
  memcpy(paddle,plane.paddle,16*sizeof(short));
  memcpy(wire,plane.wire,max_wire*sizeof(short));
  memcpy(tmp_disable,plane.tmp_disable,max_wire*sizeof(short));
  memcpy(coord,plane.coord,max_wire*sizeof(float));
  memcpy(time,plane.time,max_wire*sizeof(short));
  memcpy(path,plane.path,max_wire*sizeof(float));
  memcpy(corr,plane.corr,max_wire*sizeof(float));
  memcpy(path_diffs,plane.path_diffs,(max_wire-1)*sizeof(float));
}

VdcPlane::VdcPlane(const char *name, int size)
{
  MemInit(name,size);
}

VdcPlane::~VdcPlane()
{
  if (*rcsid == '@') {
    delete position;
    delete offset;
    delete paddle;
    delete wire;
    delete tmp_disable;
    delete coord;
    delete time;
    delete times;
    delete path;
    delete corr;
    delete Name;
#ifdef LIKELIHOOD
    delete fit;
#else
    delete reg;
#endif
    delete path_diffs;
    delete disabledWire;
  }
}

int 
compare_channel(const void *ch1, const void *ch2)
{
  if (((chRawData*)ch1)->wire < ((chRawData*)ch2)->wire)
    return -1;
  else if (((chRawData*)ch1)->wire == ((chRawData*)ch2)->wire)
    return 0;
  return 1;
}

void
sortWireTime(chRawData *cht, int len)
{
  qsort(cht, len >> 1, sizeof(chRawData), compare_channel);

  /*
    int i, j, k;
    chRawData tmp;
    
    k = len = (len >> 1) - 1;
    if (len <= 0) return;
    if (cht[0].wire < cht[len].wire) return;

    for (i = 0 ; i < len ; i++,k--)
      for (j = 0 ; j < k ; j++)
	if (cht[j].wire > cht[j+1].wire) {
	    tmp = cht[j]; cht[j] = cht[j+1]; cht[j+1] = tmp;
	}
    */
}

int
VdcPlane::InitPositionAndOffset(vdcPosOff *init)
{
  if (init->len < max_wire) return -1;

  InitPosition(init->pos);
  InitOffset(init->off);
  default_slope = 0;

  return 0;
}  

void
VdcPlane::InitOffset(short offs, short start, short count)
{
  if (count < 0)
    count = max_wire;
  if (start + count > max_wire)
    count = max_wire - start;
  for (int i = 0; i < count; i++)
    offset[start + i] = offs;
}

void
VdcPlane::InitOffset(short *offs)
{
  for (int i = 0; i < max_wire; i++)
    offset[i] = offs[i];
}

void 
VdcPlane::InitPaddleOffset(short *offs)
{
  for (int i = 0; i < 16; i++)
    paddle[i] = offs[i];
}

void
VdcPlane::InitPaddleOffset(short offs, int nPaddle)
{
  if (nPaddle >= 0 && nPaddle < 16)
    paddle[nPaddle] = offs;
}

void
VdcPlane::InitPosition(float *pos)
{
  for (int i = 0; i < max_wire; i++)
    position[i] = pos[i];
}

void
VdcPlane::InitPosition(int wire, float pos)
{
    if (position == NULL)
      std::cerr << "position ptr is zero" << std::endl;
    else
      position[wire] = pos;
    default_slope = 0;
}


void
VdcPlane::InitCorrection(const double * par)
{
  for (int i=0; i<5; i++) corrPar[i] = par[i];
}




double korrhelper(double x,double y)
{
 // korrtest!

  double a               = -125.057;
  double b               = 0.660133;
  double c               = 87.9723;
  double d               = -0.0406913;
  double e               = -0.436205;
  double f               = -36.0893;
  double g               = 0.000854753;
  double h               = 0.0148914;
  double i               = 0.0527181;
  double j               = 6.22842;
  double k               = -5.84561;
  double l               = 130.974;
  double m               = 7.82765;
  
  double xx=x*x;
  double yy=y*y;

  return a+b*x+c*y+d*xx+e*x*y+f*yy+g*xx*x+h*xx*y+i*x*yy+j*yy*y+k/(1+x)+l/(1+y)+m/(1+x*y);
}

double
VdcPlane::Regression(double sl)
{
  if ( sl == 0.0 ) {
    if (status & PLANE_STATUS_SLOPE) return slope_;
    sl = (good_wires < 2) ? 1.0
      : (path[good_wires-1] - path[0]) / (coord[good_wires-1] - coord[0]);
  }

#ifdef LIKELIHOOD
  last_slope = sl;
#else
  if ( oppositSign(sl,last_slope) ) reverseSign();

  last_slope = sl;

 double korrektur = driftTimeCorrFun(sl);
 

  for (short i = 0; i < good_wires; i++) {
    double drift = path[i];
    /* 
   double t=fabs(drift);
    double korrektur= korrhelper(t,sl);
    if (drift>0.3) corr[i]=-korrektur ; else 
      if (drift>-0.3) corr[i]=-pow((drift/0.3),5)*korrektur;
      else corr[i]=korrektur;
    */
   
 if (drift > corrPar[0]) {
      corr[i] = korrektur;
    }
    else if (drift > -corrPar[0]) {
      corr[i] = drift * korrektur / corrPar[0];
    }
    else corr[i] = -korrektur;
  
    }
#endif

  CalcSums();

  return slope_;
}

void
VdcPlane::Reset()
{
    last_slope=1;
    num_of_wires = good_wires = central_wire = status = tmpOffset = 0;
    error_code = ErrorCode1;
#ifdef LIKELIHOOD
  fit->reset();
#endif
}


const char *
VdcPlane::NameOfPlane()
{
  return Name;
}

void
VdcPlane::printCoordinates()
{
  if (num_of_wires)
    std::cout << Name << ":\t" << x_     << " +/- " << sx
	      << '\t'  << slope_ << " +/- " << sslope << std::endl;
}

void
VdcPlane::printWireTime()
{
  if (num_of_wires) {
    int i;
    std::cout << Name << ":\t"
	      << good_wires << '\t' << num_of_wires << std::endl;
    for (i=0; i<num_of_wires; i++) {
      if (i == good_wires) std::cout << ">><<\t";
      std::cout << wire[i] << '\t';
    }
    std::cout << std::endl;
    for (i=0; i<num_of_wires; i++) {
      if (i == good_wires) std::cout << ">><<\t";
      std::cout << time[i] << '\t';
    }
    std::cout << std::endl;
  }
}

void
VdcPlane::checkSingleWires()
{
  long sum, i;
  long diff0, diffN;

  for (i=sum=0; i<good_wires; i++) sum += wire[i];
  diff0 = sum - good_wires * wire[0];
  diffN = good_wires * wire[good_wires-1] - sum;

  if (diff0 > diffN)
    ignoreWire(0);
  else
    ignoreWire(good_wires-1);

  if (error_code & ErrorCode4)
    error_code |= ErrorCode5;
  else
    error_code |= ErrorCode4;
}

void
VdcPlane::checkSignRev()
{
  if (num_of_sign_rev == 2) {
    while (path_diffs[0] > 0.0) ignoreWire(0);
    while (path_diffs[good_wires-2] < 0.0) ignoreWire(good_wires-1);
    error_code |= ErrorCode2;
    return;
  }
  good_wires = 0;
  error_code |= ErrorCode3;
}

int
VdcPlane::NumberOfCards() const
{
    return max_wire / CHANNELS_CARD;
}

double
VdcPlane::angle()
{
    return 1E3 * (M_PI_2 - atan(slope()));
}

unsigned int
VdcPlane::Pattern()
{
    int i, diff;
    unsigned pattern = 0;

    for (i=0 ; i<GoodWires() ; i++) {
	diff = wire[i] - wire[0];
	if (diff > (int)(8*sizeof(unsigned int))) break;
	pattern |= (1 << diff);
    }

    return pattern;
}

#ifdef LIKELIHOOD
int
VdcPlane::FillTime(chRawData *cht, int len)
{
  short j, k, t, num_tmp_disable = 0;

  error_code = ErrorCode0;
  num_of_wires = num_of_sign_rev = 0;

  sortWireTime(cht, len);	// ueberprueft, ob die Kanaele in aufsteigender
				// Reihenfolge sortiert sind.

  for (j = k = 0 ; k < (len/2) ; k++) {
    short chan = cht[k].wire - 1;

    if ((first_wire <= chan) && (chan <= last_wire)) {
      if (disabledWire && disabledWire[chan]) {
	tmp_disable[num_tmp_disable++] = chan;
      }
      
      t = offset[chan] + tmpOffset - cht[k].time;
      path[j] = time_to_length * (t>0 ? t : 0);
      wire[j] = chan;
      coord[j] = position[chan];
      time[j] = cht[k].time;
      times[j] = t;
      corr[j] = 0.0;
      num_of_wires++;
      j++;
    }
  }

  fit->reset();

  //  if (flag)  std::cerr<<"Stop"<<std::endl;  
  if ((good_wires = num_of_wires)) {
    // Disable all bad wires
    for (int i = 0, d = 0; (i < good_wires) && (d < num_tmp_disable); )
      if (wire[i] == tmp_disable[d]) {
	ignoreWire(i);
	d++;
      } else i++;
    if (!good_wires) {
      Reset();
      return 0;
    }
  } else {
    Reset();
    return 0;
  }

  if (!default_slope)
    default_slope = (position[0] < position[max_wire-1]) ? 1 : -1;

  fit->init(good_wires, coord, path, times);

  switch (good_wires) {
  case 0:
    x_ = slope_ = last_slope = chisqr = FLT_MAX;
    break;
  case 1:
    x_ = coord[0];
    slope_ = last_slope = default_slope;
    chisqr = 0;
    break;
  default:
    fit->calc(last_slope);
    x_ = fit->x0();
    slope_ = last_slope = fit->sl0();
    chisqr = fit->chisqr();
    break;
  }

  status = PLANE_STATUS_X | PLANE_STATUS_SLOPE | PLANE_STATUS_CHISQR;

  return 0;
}

void
VdcPlane::CalcSums()
{
  if (good_wires == 0) {
    x_ = slope_ = chisqr = sx = sslope = FLT_MAX;
    status = PLANE_STATUS_ALL;
    return;
  }

  if (good_wires == 1) {
    x_ = coord[0];
    sx = S_DIST / sqrt(12.0);
    slope_ = sslope = FLT_MAX;
    chisqr = 0;
    status = PLANE_STATUS_ALL;
    return;
  }

  fit -> calc(last_slope);
  x_     = fit -> x0();
  slope_ = fit -> sl0();
  chisqr = fit -> chisqr();
  sx     = fit -> dx0();
  sslope = fit -> dsl0();

  status = PLANE_STATUS_ALL;
}

double
calcSlope(VdcPlane *a, VdcPlane *b, double dist)
{
  if (a->good_wires == 0) {
    if (b->status & PLANE_STATUS_SLOPE)
      return b->slope_;
    else
      return b->Regression(b->Regression());
  }
      
  if (b->good_wires == 0) {
    if (a->status & PLANE_STATUS_SLOPE)
      return a->slope_;
    else
      return a->Regression(a->Regression());
  }

  if (b->x_ == a->x_) return FLT_MAX;

  return dist / (b->x_ - a->x_);
}

double
recalcSlope(VdcPlane *a, VdcPlane *b, double dist)
{
  return calcSlope(a, b, dist);
}
#else
int
VdcPlane::FillTime(chRawData *cht, int len)
{
  short j, k, t, num_tmp_disable = 0;
  float min_path = full_range * time_to_length;

  error_code = ErrorCode0;
  num_of_wires = num_of_sign_rev = 0;

  sortWireTime(cht, len);	// ueberprueft, ob die Kanaele in aufsteigender
				// Reihenfolge sortiert sind.

  for (j = k = 0 ; k < (len/2) ; k++) {
    short chan = cht[k].wire - 1;
    short wire_ok = 1;

    if ((first_wire <= chan) && (chan <= last_wire)) {
      if (disabledWire && disabledWire[chan]) {
	tmp_disable[num_tmp_disable++] = chan;
	wire_ok = 0;
      }


      // jan
      /*        
      if ((cht[k].time>(offset[chan]-75)) &&(wire_ok)){
	tmp_disable[num_tmp_disable++] = chan;
	wire_ok = 0;
	}
      */
      
      
      if ((5*cht[k].time > 6*offset[chan]) && (wire_ok)) {
		tmp_disable[num_tmp_disable++] = chan;
		wire_ok = 0;
      }
      
      if (ignore_mode && (wire_ok)) {
	int checkTime = 0, difference = 0;
	float minDiff = ignore_min + (float)chan * ignore_slope;
	float maxDiff = ignore_max + (float)chan * ignore_slope;
	int numNeighbor = 0;
	// First check: If wire k is not the first in cluster (k>0) and if 
	// wire k-1 is neighbor wire, compare drift time difference
	if ((k > 0) && !disabledWire[cht[k-1].wire-1] && 
	    (cht[k-1].wire == chan)) {
	  numNeighbor++;
	  difference = abs(cht[k-1].time - cht[k].time);
	  if (difference>minDiff && difference<maxDiff) checkTime =  1;
	  else                                          checkTime = -1;
	}
	// Second check, if wire is not positively checked by first check
	// and other neighbor wire is available
	if (checkTime != 1)
	  if ( (k < (len/2) - 1) && !disabledWire[cht[k+1].wire-1] && 
	       (cht[k+1].wire == chan + 2)) {
	    numNeighbor++;
	    difference = abs(cht[k+1].time - cht[k].time);
	    if (difference>minDiff && difference<maxDiff) checkTime =  1;
	    else                                          checkTime = -1;
	}
	// Third/fourth check for second-neighbor wires?
	//if ( ( ignore_mode ==  1 && checkTime !=  1 ) || 
	//     ( ignore_mode == -1 && checkTime == -1 ) )
	//cerr<<"ignoring wire "<<chan<<endl;

	if ( ignore_mode ==  1 && checkTime != 1 ) {
	  //  if (!flag)  std::cerr<<"Start"<<std::endl;
	  //	  flag++;
	  //std::cerr <<chan<<"\t"<<cht[k].time<<std::endl;
	  tmp_disable[num_tmp_disable++] = chan;
	  wire_ok = 0;
	}
	if ( ignore_mode == -1  && checkTime == -1 ) {
	  //if (!flag)  std::cerr<<"Start"<<std::endl;
	  //flag++;
	  //std::cerr <<chan<<"\t"<<cht[k].time<<std::endl;
	  tmp_disable[num_tmp_disable++] = chan;
	  wire_ok = 0;
	}
	if ( ignore_mode == 2 && checkTime == -1 && numNeighbor > 1 ) {
	  //if (!flag)  std::cerr<<"Start"<<std::endl;
	  //flag++;
	  //std::cerr <<chan<<"\t"<<cht[k].time<<std::endl;
	  tmp_disable[num_tmp_disable++] = chan;
	  wire_ok = 0;
	  }
	
      }

      t = offset[chan] + tmpOffset - cht[k].time;
      path[j] = time_to_length * (t>0 ? t : 0);
            if ((min_drift > path[j]) && (wire_ok)) {
	tmp_disable[num_tmp_disable++] = chan;
	wire_ok = 0;
	}
      wire[j] = chan;
      coord[j] = position[chan];
      time[j] = cht[k].time;
      times[j] = t;
      corr[j] = 0.0;
      if (j>0) path_diffs[j-1] = path[j] - path[j-1];
      if (j>1) num_of_sign_rev += oppositSign(path_diffs[j-2],path_diffs[j-1]);
      if (min_path > path[j]) {
	central_wire = j;
	min_path = path[j];
      }
      num_of_wires++;
      j++;
    }

  }
  //  if (flag)  std::cerr<<"Stop"<<std::endl;  
  if ((good_wires = num_of_wires)) {
    // Disable all bad wires
    for (int i = 0, d = 0; (i < good_wires) && (d < num_tmp_disable); )
      if (wire[i] == tmp_disable[d]) {
	ignoreWire(i);
	d++;
      } else i++;
    if (!good_wires) {
      Reset();
      return 0;
    }
    // simple formula to find large spaces between wires
    while (2*(wire[good_wires-1]-wire[0]) > 3*good_wires)
      checkSingleWires();
    if (num_of_sign_rev > 1)	// normal hit pattern has one sign reversal
      checkSignRev();		// a multi hit has more sign reversals
  } else {
    Reset();
    return 0;
  }

  if (!default_slope)
    default_slope = (position[0] < position[max_wire-1]) ? 1 : -1;

  switch (good_wires) {
  case 0:
    x_ = slope_ = last_slope = FLT_MAX;
    break;
  case 1:
    x_ = coord[0];
    slope_ = last_slope = default_slope;
    break;
  case 2:
    if (default_slope > 0) {
      path[0] *= -1;
    } else {
      path[1] *= -1;
    }
    x_ = (coord[0] + coord[1]) * 0.5;
    slope_ = last_slope = (path[0] - path[1]) / (coord[0] - coord[1]);
    break;
  default:
    if (default_slope > 0) {
      for (j = 0 ; j < central_wire ; j++) path[j] *= -1;
    } else {
      for (j = central_wire+1 ; j < good_wires ; j++) path[j] *= -1;
    }
    reg -> reset();
    for (j = 0 ; j < good_wires ; j++) {
      if (j == central_wire) continue;
      reg -> add(coord[j],path[j]);
    }
    VdcRegression regNeg = *reg;
    VdcRegression regNull = *reg;
    VdcRegression regNullW=*reg;
    reg  ->add(coord[central_wire], path[central_wire]);
    regNeg.add(coord[central_wire],-path[central_wire]);
    regNull.add(coord[central_wire],0);
    regNullW.addw(coord[central_wire],0,10);
    reg  ->calc();
    regNeg.calc();
    regNull.calc();
    regNullW.calc();

    
    //    std::cerr << "Normal chi:"<< reg->chisqr()<<"\tNeg chi:"<<regNeg.chisqr()<<"\tNull chi:"<<regNull.chisqr()<<"\tTime: "<<fabs(path[central_wire])<<std::endl;
    if (regNeg.chisqr() < reg->chisqr()) {
      path[central_wire] *= -1.0;
      *reg = regNeg;
    }
    if (regNull.chisqr()< reg->chisqr()){
      path[central_wire]=0;
      *reg=regNullW;
    }

    x_ = reg -> x0();
    
    slope_ = last_slope = reg -> sl0();
    break;
  }

  status = PLANE_STATUS_X | PLANE_STATUS_SLOPE;

  return 0;
}

void
VdcPlane::CalcSums()
{
  reg -> reset();

  if (good_wires == 0) {
    x_ = slope_ = chisqr = sx = sslope = FLT_MAX;
    status = PLANE_STATUS_ALL;
    return;
  }

  if (good_wires == 1) {
    x_ = coord[0];
    sx = S_DIST / sqrt(12.0);
    slope_ = chisqr = sslope = FLT_MAX;
    reg -> add(coord[0],path[0]+corr[0]);
    status = PLANE_STATUS_ALL;
    return;
  }

  for (short i = 0; i < good_wires; i++)
    reg -> addw(coord[i],path[i]+corr[i],(path[i]!=0)?1:10);

  reg -> calc();
  x_     = reg -> x0();
  slope_ = reg -> sl0();
  chisqr = reg -> chisqr();
  sx     = reg -> dx0();
  sslope = reg -> dsl0();

  status = PLANE_STATUS_ALL;

}

double
calcSlope(VdcPlane *a, VdcPlane *b, double dist)
{
  if (a->good_wires == 0) {
    a->reg->reset();
    if (b->status & PLANE_STATUS_SLOPE)
      return b->slope_;
    else
      return b->Regression(b->Regression());
  }
      
  if (b->good_wires == 0) {
    b->reg->reset();
    if (a->status & PLANE_STATUS_SLOPE)
      return a->slope_;
    else
      return a->Regression(a->Regression());
  }

  if (b->x_ == a->x_) return FLT_MAX;

  return dist / (b->x_ - a->x_);
}

double
recalcSlope(VdcPlane *a, VdcPlane *b, double dist)
{
  VdcRegression reg[4];
  double chisqrmin;
  int minindex = 0;
  int i, sign;

  switch ( a->good_wires ) {
  case 0:
    if ( b->slope_ < 0.0 )
      b->Regression(-b->slope_);
    break;
  case 1:
    reg[0] = reg[1] = *(b->reg) + dist;
    reg[0].add(a->coord[0],a->path[0] + a->corr[0]);
    reg[0].calc();
    reg[1].add(a->coord[0],a->path[0] - a->corr[0]);
    reg[1].calc();
    if (reg[0].chisqr()>reg[1].chisqr()) {
      a->path[0] *= -1;
      a->corr[0] *= -1;
      a->CalcSums();
    }
    break;
  case 2:
    reg[0] = reg[1] = reg[2] = reg[3] = *(b->reg) + dist;
    chisqrmin = FLT_MAX;
    for ( i=0 ; i<4 ; i++ ) {
      sign = (i & 1) ? -1 : 1;
      reg[i].add(a->coord[0],sign * a->path[0] + sign * a->corr[0]);
      sign = (i & 2) ? -1 : 1;
      reg[i].add(a->coord[1],sign * a->path[1] + sign * a->corr[1]);
      reg[i].calc();
      if (reg[i].chisqr()<chisqrmin) {
	chisqrmin = reg[i].chisqr();
	minindex = i;
      }
    }
    if (minindex & 1) {
      a->path[0] *= -1;
      a->corr[0] *= -1;
    }
    if (minindex & 2) {
      a->path[1] *= -1;
      a->corr[1] *= -1;
    }
    if ( minindex ) a->CalcSums();
    break;
  }

  switch ( b->good_wires ) {
  case 0:
    if ( a->slope_ < 0.0 )
      a->Regression(-a->slope_);
    break;
  case 1:
    reg[0] = reg[1] = *(a->reg);
    reg[0].add(b->coord[0],dist + b->path[0] + b->corr[0]);
    reg[0].calc();
    reg[1].add(b->coord[0],dist - b->path[0] - b->corr[0]);
    reg[1].calc();
    if (reg[0].chisqr()>reg[1].chisqr()) {
      b->path[0] *= -1;
      b->corr[0] *= -1;
      b->CalcSums();
    }
    break;
  case 2:
    reg[0] = reg[1] = reg[2] = reg[3] = *(a->reg);
    chisqrmin = FLT_MAX;
    for ( i=0 ; i<4 ; i++ ) {
      sign = (i & 1) ? -1 : 1;
      reg[i].add(b->coord[0],dist + sign * b->path[0] + sign * b->corr[0]);
      sign = (i & 2) ? -1 : 1;
      reg[i].add(b->coord[1],dist + sign * b->path[1] + sign * b->corr[1]);
      reg[i].calc();
      if (reg[i].chisqr()<chisqrmin) {
	chisqrmin = reg[i].chisqr();
	minindex = i;
      }
    }
    if (minindex & 1) {
      b->path[0] *= -1;
      b->corr[0] *= -1;
    }
    if (minindex & 2) {
      b->path[1] *= -1;
      b->corr[1] *= -1;
    }
    if ( minindex ) b->CalcSums();
    break;
  }

  if (!a->good_wires) return b->slope_;
  if (!b->good_wires) return a->slope_;

  if (b->x_ == a->x_) return FLT_MAX;

  return dist/(b->x_ - a->x_);
}
#endif
