//
// Created 1993-2014 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Chamber/vdcPacket.cc,v 2.18 2004-06-17 15:32:48 bernauer Exp $
//
// Implementation of class VdcPacket
//

#include <iostream>
#include <cstring>

static char rcsid[] =
 "@(#)$Id: vdcPacket.cc 2573 2014-07-31 10:30:03Z distler $";

#include "chlib++.h"
#include "vdcPacket.h"

#ifndef NULL
#define NULL 0
#endif

#define MAX_RECALC_WIRES 150

#ifdef LIKELIHOOD
#include "vdcLikelihood.h"

void
VdcPacket::MemInit()
{
  slope_x = slope_s = 0.0;
  status  = statusX = statusY = destroyPlanes = 0;
  //  fitX    = new VdcLikelihood;
  //  fitS    = new VdcLikelihood;
  fitX = fitS = NULL;
  ident = NULL;
}

VdcPacket::VdcPacket()
{
  MemInit();
}

VdcPacket::VdcPacket(VdcPlane *x_1, VdcPlane *s_1,
		     VdcPlane *x_2, VdcPlane *s_2)
{
  x1 = x_1 ? x_1 : new VdcPlane("",2);
  s1 = s_1 ? s_1 : new VdcPlane("",2);
  x2 = x_2 ? x_2 : new VdcPlane("",2);
  s2 = s_2 ? s_2 : new VdcPlane("",2);

  MemInit();
}

VdcPacket::~VdcPacket()
{
  if (destroyPlanes) {
    delete x1;
    delete s1;
    delete x2;
    delete s2;
  }
  delete fitX;
  delete fitS;
  delete[] ident;

  if (rcsid[0] == '@');
}

int
VdcPacket::InitPositionAndOffset(vdcPosOffs *init, double angle)
{
  int err;

  sin_x_s   = sin(angle * M_PI / 180.0);
  sec_x_s   = 1.0 / sin_x_s;
  cos_x_s   = cos(angle * M_PI / 180.0);
  csc_x_s   = 1.0 / cos_x_s;
  tan_x_s   = tan(angle * M_PI / 180.0);
  cot_x_s   = 1.0 / tan_x_s;
  ccos_x_s  = square(cos_x_s);

  if ( x1 != NULL ) {
    err = x1->InitPositionAndOffset(&(init->x1));
    if ( err ) return err;
  }

  if ( s1 != NULL ) {
    err = s1->InitPositionAndOffset(&(init->s1));
    if ( err ) return err;
  }

  if ( x2 != NULL ) {
    err = x2->InitPositionAndOffset(&(init->x2));
    if ( err ) return err;
  }

  if ( s2 != NULL ) {
    err = s2->InitPositionAndOffset(&(init->s2));
    if ( err ) return err;
  }

  return 0;
}

void
VdcPacket::Reset()
{
  if ( x1 != NULL ) x1->Reset();
  if ( s1 != NULL ) s1->Reset();
  if ( x2 != NULL ) x2->Reset();
  if ( s2 != NULL ) s2->Reset();

  slope_x = slope_s = 0.0;
  status = 0;
}

void
VdcPacket::CalcCoord(int bitmask)
{
  if (!(status & PACKET_STATUS_SLOPE_X)) {
    slope_x = calcSlope(x1,x2,DIST_X_1_2);
#ifdef NEVER
    if (x1->GoodWires() > 0)
      x1->Regression(slope_x);
    if (x2->GoodWires() > 0)
      x2->Regression(slope_x);
    if ((x1->GoodWires() < 3) || (x2->GoodWires() < 3))
      slope_x = recalcSlope(x1,x2,DIST_X_1_2);
#endif

    char *id = NULL;
    id = new char[(ident ? strlen(ident)+2 : 2)];
    if (ident) {
      strcpy(id, ident);
    } else *id = 0;
    strcat(id, "X");
    if (fitX) delete fitX;
    fitX = new VdcLikelihood(x1->fit, x2->fit, DIST_X_1_2, id);
    fitX -> calc(slope_x);
    delete[] id; id = NULL;
    
    slope_x = fitX->sl0();
    if (slope_x==0) return;
    status |= PACKET_STATUS_SLOPE_X;
  }
  if (bitmask & PACKET_STATUS_X) {
    status |= PACKET_STATUS_X;
    x_ = fitX->x0();
    return;
  }
  if (bitmask & PACKET_STATUS_THETA) {
    status |= PACKET_STATUS_THETA;
    theta_ = 1E3 * (M_PI_2 - atan(fitX->sl0()));
    return;
  }
  if (bitmask & PACKET_STATUS_dX) {
    status |= PACKET_STATUS_dX;
    dx_ = fitX->dx0();
    return;
  }
  if (bitmask & PACKET_STATUS_dTHETA) {
    status |= PACKET_STATUS_dTHETA;
    dtheta_ = 1E3 * fitX->dsl0() / (1.0 + square(fitX->sl0()));
    return;
  }
  if (!(status & PACKET_STATUS_SLOPE_S)) {
    slope_s = calcSlope(s1,s2,DIST_X_1_2);
    if (s1->GoodWires() > 0)
      s1->Regression(slope_s);
    if (s2->GoodWires() > 0)
      s2->Regression(slope_s);
    if ((s1->GoodWires() < 3) || (s2->GoodWires() < 3))
      slope_s = recalcSlope(s1,s2,DIST_X_1_2);

    char *id = NULL;
    id = new char[(ident ? strlen(ident)+2 : 2)];
    if (ident) {
      strcpy(id, ident);
    } else *id = 0;
    strcat(id, "S");
    if (fitS) delete fitS;
    fitS = new VdcLikelihood(s1->fit, s2->fit, DIST_X_1_2, id);
    fitS -> calc(slope_s);
    delete[] id; id = NULL;

    slope_s = fitS->sl0();
    if (slope_s==0) return;
    status |= PACKET_STATUS_SLOPE_S;
  }
  if (bitmask & PACKET_STATUS_Y) {
    status |= PACKET_STATUS_Y;
    y_ = (fitX->x0() * cos_x_s + L_2 / fitS->sl0() - fitS->x0()) * sec_x_s;
    return;
  }
  if (bitmask & PACKET_STATUS_PHI) {
    status |= PACKET_STATUS_PHI;
    phi_ = 1E3 * atan(cot_x_s / fitX->sl0() - sec_x_s / fitS->sl0());
    return;
  }
  if (bitmask & PACKET_STATUS_S) {
    status |= PACKET_STATUS_S;
    s_ = fitS->x0();
    return;
  }
  if (bitmask & PACKET_STATUS_THETA_S) {
    status |= PACKET_STATUS_THETA_S;
    theta_s_ = 1E3 * (M_PI_2 - atan(fitS->sl0()));
    return;
  }

  double help;

  if (bitmask & PACKET_STATUS_dY) {
    status |= PACKET_STATUS_dY;
    help = L_2 / square(fitS->sl0());
    dy_ = sec_x_s * sqrt(square(fitX->dx0()) * ccos_x_s + square(fitS->dsl0())
			 * square(help) + square(fitS->dx0()));
    return;
  }

  double helpx, helps;

  if (bitmask & PACKET_STATUS_dPHI) {
    status |= PACKET_STATUS_dPHI;
    help = cot_x_s / fitX->sl0() - sec_x_s / fitS->sl0();
    helpx = fitX->dsl0() * cot_x_s / square(fitX->sl0());
    helps = fitS->dsl0() * sec_x_s / square(fitS->sl0());
    dphi_ = 1E3 * sqrt(square(helpx) + square(helps)) / (1.0 + square(help));
    return;
  }
}
#else
#include "vdcRegression.h"

void
VdcPacket::MemInit()
{
  slope_x = slope_s = 0.0;
  status  = statusX = statusY = destroyPlanes = 0;
  regX    = new VdcRegression;
  regS    = new VdcRegression;
  ident   = NULL;
}

VdcPacket::VdcPacket()
{
  MemInit();
}

VdcPacket::VdcPacket(VdcPlane *x_1, VdcPlane *s_1,
		     VdcPlane *x_2, VdcPlane *s_2)
{
  x1 = x_1 ? x_1 : new VdcPlane("",2);
  s1 = s_1 ? s_1 : new VdcPlane("",2);
  x2 = x_2 ? x_2 : new VdcPlane("",2);
  s2 = s_2 ? s_2 : new VdcPlane("",2);

  MemInit();
}

VdcPacket::~VdcPacket()
{
  if (destroyPlanes) {
    delete x1;
    delete s1;
    delete x2;
    delete s2;
  }
  delete regX;
  delete regS;
  delete[] ident;

  if (rcsid[0] == '@');
}

int
VdcPacket::InitPositionAndOffset(vdcPosOffs *init, double angle)
{
  int err;

  sin_x_s   = sin(angle * M_PI / 180.0);
  sec_x_s   = 1.0 / sin_x_s;
  cos_x_s   = cos(angle * M_PI / 180.0);
  csc_x_s   = 1.0 / cos_x_s;
  tan_x_s   = tan(angle * M_PI / 180.0);
  cot_x_s   = 1.0 / tan_x_s;
  ccos_x_s  = square(cos_x_s);

  if ( x1 != NULL ) {
    err = x1->InitPositionAndOffset(&(init->x1));
    if ( err ) return err;
  }

  if ( s1 != NULL ) {
    err = s1->InitPositionAndOffset(&(init->s1));
    if ( err ) return err;
  }

  if ( x2 != NULL ) {
    err = x2->InitPositionAndOffset(&(init->x2));
    if ( err ) return err;
  }

  if ( s2 != NULL ) {
    err = s2->InitPositionAndOffset(&(init->s2));
    if ( err ) return err;
  }

  return 0;
}

void
VdcPacket::Reset()
{
  if ( x1 != NULL ) x1->Reset();
  if ( s1 != NULL ) s1->Reset();
  if ( x2 != NULL ) x2->Reset();
  if ( s2 != NULL ) s2->Reset();

  slope_x = slope_s = 0.0;
  status = 0;
}

void
VdcPacket::CalcCoord(int bitmask)
{


  if (!(status & PACKET_STATUS_SLOPE_X)) {
    slope_x = calcSlope(x1,x2,DIST_X_1_2);
    if (x1->GoodWires() > 0)
      x1->Regression(slope_x);
    if (x2->GoodWires() > 0)
      x2->Regression(slope_x);
    if ((x1->GoodWires() < 3) || (x2->GoodWires() < 3))
      recalcSlope(x1,x2,DIST_X_1_2);
    *regX = *(x1->reg) + (*(x2->reg) + DIST_X_1_2);
    regX -> calc();
    slope_x = regX->sl0();
    if (slope_x==0) return;
    status |= PACKET_STATUS_SLOPE_X;
  }
  if (bitmask & PACKET_STATUS_X) {
    status |= PACKET_STATUS_X;
    x_ = regX->x0();
    return;
  }
  if (bitmask & PACKET_STATUS_THETA) {
    status |= PACKET_STATUS_THETA;
    theta_ = 1E3 * (M_PI_2 - atan(regX->sl0()));
    return;
  }
  if (bitmask & PACKET_STATUS_dX) {
    status |= PACKET_STATUS_dX;
    dx_ = regX->dx0();
    return;
  }
  if (bitmask & PACKET_STATUS_dTHETA) {
    status |= PACKET_STATUS_dTHETA;
    dtheta_ = 1E3 * regX->dsl0() / (1.0 + square(regX->sl0()));
    return;
  }
  if (!(status & PACKET_STATUS_SLOPE_S)) {
    slope_s = calcSlope(s1,s2,DIST_X_1_2);
    if (s1->GoodWires() > 0)
      s1->Regression(slope_s);
    if (s2->GoodWires() > 0)
      s2->Regression(slope_s);
    if ((s1->GoodWires() < 3) || (s2->GoodWires() < 3))
      recalcSlope(s1,s2,DIST_X_1_2);
    *regS = *(s1->reg) + (*(s2->reg) + DIST_X_1_2);
    regS -> calc();
    slope_s = regS->sl0();
    if (slope_s==0) return;
    status |= PACKET_STATUS_SLOPE_S;
  }
  if (bitmask & PACKET_STATUS_Y) {
    status |= PACKET_STATUS_Y;
    y_ = (regX->x0() * cos_x_s + L_2 / regS->sl0() - regS->x0()) * sec_x_s;
    return;
  }
  if (bitmask & PACKET_STATUS_PHI) {
    status |= PACKET_STATUS_PHI;
    phi_ = 1E3 * atan(cot_x_s / regX->sl0() - sec_x_s / regS->sl0());
    return;
  }
  if (bitmask & PACKET_STATUS_S) {
    status |= PACKET_STATUS_S;
    s_ = regS->x0();
    return;
  }
  if (bitmask & PACKET_STATUS_THETA_S) {
    status |= PACKET_STATUS_THETA_S;
    theta_s_ = 1E3 * (M_PI_2 - atan(regS->sl0()));
    return;
  }

  double help;

  if (bitmask & PACKET_STATUS_dY) {
    status |= PACKET_STATUS_dY;
    help = L_2 / square(regS->sl0());
    dy_ = sec_x_s * sqrt(square(regX->dx0()) * ccos_x_s + square(regS->dsl0())
			 * square(help) + square(regS->dx0()));
    return;
  }

  double helpx, helps;

  if (bitmask & PACKET_STATUS_dPHI) {
    status |= PACKET_STATUS_dPHI;
    help = cot_x_s / regX->sl0() - sec_x_s / regS->sl0();
    helpx = regX->dsl0() * cot_x_s / square(regX->sl0());
    helps = regS->dsl0() * sec_x_s / square(regS->sl0());
    dphi_ = 1E3 * sqrt(square(helpx) + square(helps)) / (1.0 + square(help));
    return;
  }
}
#endif



double
VdcPacket::DoTheLinearRegressionSeparately()
{
  static chRawData chtemp[4][15];
  int i;
  int lx1=x1->NumberOfWires(); if (lx1>15) lx1=15;
  int ls1=s1->NumberOfWires(); if (ls1>15) ls1=15;
  int lx2=x2->NumberOfWires(); if (lx2>15) lx2=15;
  int ls2=s2->NumberOfWires(); if (ls2>15) ls2=15;
  if (lx1==0 || lx2==0 || ls1==0 || ls2==0) return -4711;
  for (i=0; i < lx1; i++){
    chtemp[0][i].wire = x1->WiresFired()[i]+1;
    chtemp[0][i].time = x1->RawDriftTimes()[i];
  }
  for (i=0; i < ls1; i++){
    chtemp[1][i].wire = s1->WiresFired()[i]+1;
    chtemp[1][i].time = s1->RawDriftTimes()[i];
  }
  for (i=0; i < lx2; i++){
    chtemp[2][i].wire = x2->WiresFired()[i]+1;
    chtemp[2][i].time = x2->RawDriftTimes()[i];
  }
  for (i=0; i < ls2; i++){
    chtemp[3][i].wire = s2->WiresFired()[i]+1;
    chtemp[3][i].time = s2->RawDriftTimes()[i];
  }
  Reset();
  x1->FillTime(chtemp[0], 2*lx1);
  s1->FillTime(chtemp[1], 2*ls1);
  double c1=x();
  double b1=x1Plane()->slope();
  Reset();
  x2->FillTime(chtemp[2], 2*lx2);
  s2->FillTime(chtemp[3], 2*ls2);
  double c2=x();
  double b2=x2Plane()->slope();
  Reset();
  x1->FillTime(chtemp[0], 2*lx1);
  s1->FillTime(chtemp[1], 2*ls1);
  x2->FillTime(chtemp[2], 2*lx2);
  s2->FillTime(chtemp[3], 2*ls2);
  double db=b1-b2;
  return (db==0) ? -4711 : b1*b2*(c1-c2)/db;
}

void
VdcPacket::CorrectPaddleOffset(int nPaddle)
{

  static chRawData chtemp[MAX_RECALC_WIRES];
  int DoReset = 0;

  if (x1->PaddleOffset()[nPaddle]) {
    int lx1 = x1->NumberOfWires(); 
    if (lx1 > MAX_RECALC_WIRES) lx1 = MAX_RECALC_WIRES;
    for (int i = 0; i < lx1; i++){
      chtemp[i].wire = x1->WiresFired()[i]+1;
      chtemp[i].time = x1->RawDriftTimes()[i];
    }
    x1->Reset();
    x1->CorrectPaddleOffset(nPaddle);
    x1->FillTime(chtemp, 2*lx1);
    DoReset = 1;
  }

  if (s1->PaddleOffset()[nPaddle]) {
    int ls1 = s1->NumberOfWires(); 
    if (ls1 > MAX_RECALC_WIRES) ls1 = MAX_RECALC_WIRES;
    for (int i=0; i < ls1; i++){
      chtemp[i].wire = s1->WiresFired()[i]+1;
      chtemp[i].time = s1->RawDriftTimes()[i];
    }
    s1->Reset();
    s1->CorrectPaddleOffset(nPaddle);
    s1->FillTime(chtemp, 2*ls1);
    DoReset = 1;
  }

  if (x2->PaddleOffset()[nPaddle]) {
    int lx2 = x2->NumberOfWires(); 
    if (lx2 > MAX_RECALC_WIRES) lx2 = MAX_RECALC_WIRES;
    for (int i=0; i < lx2; i++){
      chtemp[i].wire = x2->WiresFired()[i]+1;
      chtemp[i].time = x2->RawDriftTimes()[i];
    }
    x2->Reset();
    x2->CorrectPaddleOffset(nPaddle);
    x2->FillTime(chtemp, 2*lx2);
    DoReset = 1;
  }

  if (s2->PaddleOffset()[nPaddle]) {
    int ls2 = s2->NumberOfWires(); 
    if (ls2 > MAX_RECALC_WIRES) ls2 = MAX_RECALC_WIRES;
    for (int i=0; i < ls2; i++){
      chtemp[i].wire = s2->WiresFired()[i]+1;
      chtemp[i].time = s2->RawDriftTimes()[i];
    }
    s2->Reset();
    s2->CorrectPaddleOffset(nPaddle);
    s2->FillTime(chtemp, 2*ls2);
    DoReset = 1;
  }

  if (DoReset) {
    slope_x = slope_s = 0.0;
    status = 0;
  }
}

void VdcPacket::drawTrack(char *fname, char *comment)
{
  FILE *out=fopen(fname,"w");
  const float S_DIST         =   5.0;
  const float DIST_X_1_2     = 272.0;
  const float L              =  12.0; 
  float length[3] = { 0, x1->SignalWires()*S_DIST, 
		      x2->SignalWires()*S_DIST };
  float shift     =   fabs(x1->getFirstPosition()) - 
    fabs(x2->getFirstPosition());                      // mm
  float minPos[3];
  if (shift>0) { minPos[1]=0;      minPos[2]=shift; }  // mm
  else         { minPos[1]=-shift; minPos[2]=0;     }
  float maxPos[3] = { 0, minPos[1]+length[1], minPos[2]+length[2] }; // mm
  float maxX   = ( maxPos[1]>maxPos[2] ? maxPos[1] : maxPos[2] );    // mm
  
  float offX   =   50;
  float offYs  =  500, ssc = 0.2;    // Small scale (mm<->dpi)
  float offYb  =  150, bsc = 2.0;    // Big scale
  float offX2  =   offX+maxX*ssc/2;
  float sPosX[3][2] = { {                  0,                   0  }, 
			{ minPos[1]*ssc+offX,  maxPos[1]*ssc+offX  },
			{ minPos[2]*ssc+offX,  maxPos[2]*ssc+offX  } };
  float sPosY[3] =    { 0, offYs, DIST_X_1_2*ssc+offYs };
  float bPosY[3] =    { 0, offYb, offYb + 7*L*bsc };

  fprintf(out, "%%!PS-Adobe-2.0\n");
  fprintf(out, "%%%%Creator: VdcPacket of Cola++, Jan Friedrich (1998)\n");
  fprintf(out, "%%%%BoundingBox:  42.520 141.732 552.756 765.354\n");
  fprintf(out,
#include "../HMBook/hmplot_prolog.h"
	  );
  fprintf(out, "/signalwire  {n 0.1 0 360 arc fill} def\n");
  fprintf(out, "/signalwireG {n 1.0 0 360 arc fill} def\n");
  fprintf(out, "/slwS {0.1 setlinewidth} def\n");
  fprintf(out, "/slwG {1.0 setlinewidth} def\n");
  fprintf(out, "/bigAtext {/fs 25.0 def (Helvetica-Bold) font} def\n");
  fprintf(out, "/bigBtext {/fs 20.0 def (Helvetica-Bold) font} def\n");
  fprintf(out, "/smallAtext {/fs 15.0 def (Helvetica) font} def\n");
  fprintf(out, "%%%%EndProlog\n");

  fprintf(out, "1.0 0.0 0.0 setrgbcolor\n");
  fprintf(out, "bigAtext 0.5 {tstart (%s) tstop} %.2f 770.000 text\n",
	  "Trajectory reconstruction in VDC", offX2);
  fprintf(out, "bigBtext 0.5 {tstart (%s) tstop} %.2f 740.000 text\n",
	  comment, offX2);
  fprintf(out, "0.0 0.0 0.0 setrgbcolor\n");
  char rstring[80]; sprintf(rstring, "x  = (%.4f \261 %.4g) mm", 
			    x(), dx());
  fprintf(out, "smallAtext 0.0 {tstart (%s) tstop} %.2f 700.000 text\n",
	  rstring, offX);
  sprintf(rstring, "th = (%.4f \261 %.4g) mrad", theta(), dtheta());
  fprintf(out, "smallAtext 0.0 {tstart (%s) tstop} %.2f 680.000 text\n",
	  rstring, offX);

  const short *wf[5] = { NULL, 
			 x1->WiresFired(), x2->WiresFired(), 
			 s1->WiresFired(), s2->WiresFired() };
  short gw[5]        = { 0, 
			 x1->GoodWires(),  x2->GoodWires(),
			 s1->GoodWires(),  s2->GoodWires()  };

  // AWMM 
  // check
  const short *wire123 = x2->WiresFired();  
  std::cout<<"Error: "<<x2->errorCode()<<" Number of wires: "<<x2->NumberOfWires()<<", good wires: "<<x2->GoodWires();
  for(int i = 0; i<x2->NumberOfWires(); i++) std::cout<<" "<<wire123[i];
  std::cout<<std::endl;
  


  // Now the reconstructed track (blue line)
  float tt   = tan(theta()/1000.0);

  // This is correct for Spec. B and C :
  float xx    = x1->getFirstPosition();  
  float sign  = -1;

  // This is correct for Spec. A :
  if (fabs(x2->getFirstPosition())>fabs(x1->getFirstPosition())) {  
    xx   = x2->getFirstPosition();       
    sign = 1;
  }

  float 
    sYdown = sPosY[1] - 2*L*ssc, 
    sYup   = sPosY[2] + 6*L*ssc,
    sXdown = sign*((-x()+xx) +             3*L* tt)*ssc + offX,
    sXup   = sign*((-x()+xx) - (DIST_X_1_2+5*L)*tt)*ssc + offX;

  fprintf(out, "0.00 0.00 1.00 setrgbcolor\n");
  fprintf(out, "slwS %f %f m %f %f l s\n", sXdown, sYdown, sXup, sYup);

  char allstr[5][300]={"", "X1:", "X2:", "S1:", "S2:"};
  char help[300];

  sprintf( allstr[1], "X1 (err %2d) :", x1->errorCode() );
  sprintf( allstr[2], "X2 (err %2d) :", x2->errorCode() );

  sprintf( allstr[3], "S1 (err %2d) :", s1->errorCode() );
  sprintf( allstr[4], "S2 (err %2d) :", s2->errorCode() );


  for (int pl=1; pl<=2; pl++) {
    fprintf(out, "0.00 0.00 0.00 setrgbcolor\n");
    fprintf(out, "slwS %f %f m %f %f l s\n", 
	    sPosX[pl][0], sPosY[pl], sPosX[pl][1], sPosY[pl]);
    fprintf(out, "slwG %f  %f m %f %f l s\n", 
	    offX, bPosY[pl], maxX*ssc+offX, bPosY[pl]);
    for (float i=0; 
	 i<(float)(pl==1 ? x1->SignalWires() : x2->SignalWires()); i++)
      fprintf(out, "%f %f signalwire\n", 
	      sPosX[pl][0]+i*S_DIST*ssc, sPosY[pl]+L*ssc);
    for (float i=0; i<maxX*ssc/(S_DIST*bsc); i++)
      fprintf(out, "%f %f signalwireG\n", 
	      offX+i*S_DIST*bsc, bPosY[pl]+L*bsc);
    fprintf(out, "0.00 1.00 0.00 setrgbcolor\n");
    float off = ((float)wf[pl][gw[pl]/2] - rint(maxX*ssc/(2*S_DIST*bsc)))
      * S_DIST;                   // in mm

    for (int i=0; i<(pl==1 ? s1->NumberOfWires() : s2->NumberOfWires()); 
	 i++) {
      sprintf(help, "%s", allstr[pl+2]);
      sprintf(allstr[pl+2], "%s %d", help, wf[pl+2][i]);
      if (i == (gw[pl+2]-1) ) { 
	//fprintf(out, "1.00 0.00 0.00 setrgbcolor\n");
        sprintf(help, "%s", allstr[pl+2]);
	sprintf(allstr[pl+2], "%s |", help);
      }
    }


    for (int i=0; i<(pl==1 ? x1->NumberOfWires() : x2->NumberOfWires()); i++) {
/*
      if (i > (gw[pl]-1) ) { 
	fprintf(out, "1.00 0.00 0.00 setrgbcolor\n");
        sprintf(help, "%s", allstr[pl]);
	sprintf(allstr[pl], "%s |", help);
      }
*/
      sprintf(help, "%s", allstr[pl]);
      sprintf(allstr[pl], "%s %d", help, wf[pl][i]);
      float w=(float)wf[pl][i] *S_DIST*ssc + sPosX[pl][0];
      float p=(pl==1 ? x1->DriftPath() : x2->DriftPath())[i];
      fprintf(out, "slwS %f %f m %f %f l s\n", 
	      w, sPosY[pl]+(L+p)*ssc , w, sPosY[pl]+(L-p)*ssc);
      float w2=((w-sPosX[pl][0])/ssc-off)*bsc + offX;
      if (w2>offX && w2<maxX*ssc){
	fprintf(out, "slwG %f %f m %f %f l s\n", 
		w2, bPosY[pl]+(L+p)*bsc , w2, bPosY[pl]+(L-p)*bsc);
      }
      if (i == (gw[pl]-1) ) { 
	fprintf(out, "1.00 0.00 0.00 setrgbcolor\n");
        sprintf(help, "%s", allstr[pl]);
	sprintf(allstr[pl], "%s |", help);
      }
    }

    fprintf(out, "0.00 0.00 0.00 setrgbcolor\n");
    //fprintf(out, "smallAtext 0.0 {tstart (%s) tstop} %.2f 650.000 text\n",
	//    allstr[4], offX);
    fprintf(out, "smallAtext 0.0 {tstart (%s) tstop} %.2f 635.000 text\n",
	    allstr[2], offX);
    //fprintf(out, "smallAtext 0.0 {tstart (%s) tstop} %.2f 620.000 text\n",
	//    allstr[3], offX);
    fprintf(out, "smallAtext 0.0 {tstart (%s) tstop} %.2f 605.000 text\n",
	    allstr[1], offX);

    fprintf(out, "slwS %f %f m %f %f l s\n",sPosX[pl][0], 
	    sPosY[pl]+2*L*ssc, sPosX[pl][1], sPosY[pl]+2*L*ssc);
    fprintf(out, "%f %f m %f %f l s\n", sPosX[pl][0], 
	    sPosY[pl]+4*L*ssc, sPosX[pl][1], sPosY[pl]+4*L*ssc);
    fprintf(out, "slwG %f %f m %f %f l s\n", 
	    offX, bPosY[pl]+2*L*bsc, maxX*ssc+offX, bPosY[pl]+2*L*bsc);
    fprintf(out, "%f %f m %f %f l s\n", 
	    offX, bPosY[pl]+4*L*bsc, maxX*ssc+offX, bPosY[pl]+4*L*bsc);

    // reconstructed track in enlargement
    sYdown = bPosY[pl]-1*L*bsc; 
    sYup   = bPosY[pl]+5*L*bsc;
    float xxx  = ( pl==1 ? x1->getFirstPosition():x2->getFirstPosition() );
    float dist = ( pl==1 ?                      0:            DIST_X_1_2 );
    sXdown = ( sign*((-x()+xxx) - (dist-2*L) * tt) - off) * bsc + offX;
    sXup   = ( sign*((-x()+xxx) - (dist+4*L) * tt) - off) * bsc + offX;

    fprintf(out, "0.00 0.00 1.00 setrgbcolor\n");
    fprintf(out, "slwG %f %f m %f %f l s\n", sXdown, sYdown, sXup, sYup);
  }    
  fprintf(out, "showpage\n");

  //##########################################
  // 
  //--- The second page shows the s-planes!
  //
  //##########################################



  float length2[3] = { 0, s1->SignalWires()*S_DIST, s2->SignalWires()*S_DIST };

  shift     =   fabs(s1->getFirstPosition()) - fabs(s2->getFirstPosition()); // mm
  if (shift>0) { minPos[1]=0;      minPos[2]=shift; }  // mm
  else         { minPos[1]=-shift; minPos[2]=0;     }
  float maxPos2[3] = { 0, minPos[1]+length2[1], minPos[2]+length2[2] }; // mm
  maxX   = ( maxPos2[1]>maxPos2[2] ? maxPos2[1] : maxPos2[2] );    // mm
  
  offX   =   50;
  offYs  =  500; ssc = 0.2;    // Small scale (mm<->dpi)
  offYb  =  150; bsc = 2.0;    // Big scale
  offX2  =   offX+maxX*ssc/2;
  float sPosX2[3][2] = { {                  0,                   0  }, 
			{ minPos[1]*ssc+offX,  maxPos2[1]*ssc+offX  },
			{ minPos[2]*ssc+offX,  maxPos2[2]*ssc+offX  } };
  float sPosY2[3] =    { 0, offYs, DIST_X_1_2*ssc+offYs };
  float bPosY2[3] =    { 0, offYb, offYb + 7*L*bsc };


  fprintf(out, "%%Page: 2 2\n");

  sprintf(rstring, "y  = (%.4f \261 %.4g) mm", y(), dy());
  fprintf(out, "smallAtext 0.0 {tstart (%s) tstop} %.2f 700.000 text\n", rstring, offX);
  sprintf(rstring, "ph = (%.4f \261 %.4g) mrad", phi(), dphi());
  fprintf(out, "smallAtext 0.0 {tstart (%s) tstop} %.2f 680.000 text\n", rstring, offX);

  tt   = tan(theta_s()/1000.0);
  xx    = s1->getFirstPosition();  
  sign  = -1;

  // This is correct for Spec. A :
  if (fabs(s2->getFirstPosition())>fabs(s1->getFirstPosition())) {  
    xx   = s2->getFirstPosition();       
    sign = 1;
  }

  sYdown = sPosY2[1] - 2*L*ssc + 2*L*ssc;
  sYup   = sPosY2[2] + 6*L*ssc + 2*L*ssc;
  sXdown = sign*((-s()+xx) + 3*L* tt)*ssc + offX;
  sXup   = sign*((-s()+xx) - (DIST_X_1_2+5*L)*tt)*ssc + offX;

  fprintf(out, "0.00 0.00 1.00 setrgbcolor\n");
  fprintf(out, "slwS %f %f m %f %f l s\n", sXdown, sYdown, sXup, sYup);


  for (int pl=1; pl<=2; pl++) {

    //--- draws wires and cathode planes
    fprintf(out, "0.00 0.00 0.00 setrgbcolor\n");
    fprintf(out, "slwS %f %f m %f %f l s\n", 
	    sPosX2[pl][0], sPosY2[pl], sPosX2[pl][1], sPosY2[pl]);
    fprintf(out, "slwG %f  %f m %f %f l s\n", 
	    offX, bPosY2[pl], maxX*ssc+offX, bPosY2[pl]);

    for (float i=0; i<(float)(pl==1 ? s1->SignalWires() : s2->SignalWires()); i++)
      fprintf(out, "%f %f signalwire\n", sPosX2[pl][0]+i*S_DIST*ssc, sPosY2[pl]+L*ssc*3);

    for (float i=0; i<maxX*ssc/(S_DIST*bsc); i++) 
      fprintf(out, "%f %f signalwireG\n", offX+i*S_DIST*bsc, bPosY2[pl]+L*bsc*3);

    fprintf(out, "0.00 1.00 0.00 setrgbcolor\n");

    float off = ((float)wf[pl+2][gw[pl+2]/2] - rint(maxX*ssc/(2*S_DIST*bsc)))* S_DIST; // in mm


    //--- now lets draw the hit wires
    for (int i=0; i<(pl==1 ? s1->NumberOfWires() : s2->NumberOfWires()); i++) {

      float w=(float)wf[pl+2][i] *S_DIST*ssc + sPosX2[pl][0];
      float p=(pl==1 ? s1->DriftPath() : s2->DriftPath())[i];
      //--- draw on tiny chamber
      fprintf(out, "slwS %f %f m %f %f l s\n", w, sPosY2[pl]+(L*3+p)*ssc , w, sPosY2[pl]+(L*3-p)*ssc);
      float w2=((w-sPosX2[pl][0])/ssc-off)*bsc + offX;
      if (w2>offX && w2<maxX*ssc){
        //--- draw on big chamber
	fprintf(out, "slwG %f %f m %f %f l s\n", w2, bPosY2[pl]+(L*3+p)*bsc , w2, bPosY2[pl]+(L*3-p)*bsc);
      }
    }

    fprintf(out, "0.00 0.00 0.00 setrgbcolor\n");
    fprintf(out, "smallAtext 0.0 {tstart (%s) tstop} %.2f 635.000 text\n",
	    allstr[4], offX);
    fprintf(out, "smallAtext 0.0 {tstart (%s) tstop} %.2f 605.000 text\n",
	    allstr[3], offX);

    fprintf(out, "slwS %f %f m %f %f l s\n",sPosX2[pl][0], 
	    sPosY2[pl]+2*L*ssc, sPosX2[pl][1], sPosY2[pl]+2*L*ssc);
    fprintf(out, "%f %f m %f %f l s\n", sPosX2[pl][0], 
	    sPosY2[pl]+4*L*ssc, sPosX2[pl][1], sPosY2[pl]+4*L*ssc);
    fprintf(out, "slwG %f %f m %f %f l s\n", 
	    offX, bPosY2[pl]+2*L*bsc, maxX*ssc+offX, bPosY2[pl]+2*L*bsc);
    fprintf(out, "%f %f m %f %f l s\n", 
	    offX, bPosY2[pl]+4*L*bsc, maxX*ssc+offX, bPosY2[pl]+4*L*bsc);

    // reconstructed track in enlargement
    sYdown = bPosY2[pl]-1*L*bsc; 
    sYup   = bPosY2[pl]+5*L*bsc;
    float xxx  = ( pl==1 ? s1->getFirstPosition():s2->getFirstPosition() );
    float dist = ( pl==1 ?                     0:            DIST_X_1_2 );
    sXdown = ( sign*((-s()+xxx) - (dist-2*L - 2*L) * tt) - off) * bsc + offX;
    sXup   = ( sign*((-s()+xxx) - (dist+4*L - 2*L) * tt) - off) * bsc + offX;

    fprintf(out, "0.00 0.00 1.00 setrgbcolor\n");
    fprintf(out, "slwG %f %f m %f %f l s\n", sXdown, sYdown, sXup, sYup);
  }    



  fprintf(out, "showpage\n");

  fclose(out);
}
