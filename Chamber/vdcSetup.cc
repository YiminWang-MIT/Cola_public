//
// Created 1993-2014 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Chamber/vdcSetup.cc,v 2.17 2002-11-08 20:27:26 distler Exp $
//
// Implementation of class VdcSetup
//

static char rcsid[] =
 "@(#)$Id: vdcSetup.cc 2560 2014-05-21 14:51:35Z distler $";

#include "chlib++.h"
#include "vdcSetup.h"
#ifdef LIKELIHOOD
#include "vdcLikelihood.h"
#else
#include "vdcRegression.h"
#endif
#include <iostream>

VdcSetup::VdcSetup(VdcPacket *a, VdcPacket *b, VdcPacket *c)
{
  speca = a;
  specb = b;
  specc = c;

  if ( speca != NULL ) {
    if ( speca->InitPositionAndOffset(&vdcPosOffsA) )
      std::cout << "error initializing SpecA\n";
    speca->InitTimeToLength(CH_A_TIMETOLENGTH);
    speca->InitFullRange(CH_A_FULLRANGE);
  }

  if ( specb != NULL ) {
    if ( specb->InitPositionAndOffset(&vdcPosOffsB) )
      std::cout << "error initializing SpecB\n";
    specb->InitTimeToLength(CH_B_TIMETOLENGTH);
    specb->InitFullRange(CH_B_FULLRANGE);
  }

  if ( specc != NULL ) {
    if ( specc->InitPositionAndOffset(&vdcPosOffsC) )
      std::cout << "error initializing SpecC\n";
    specc->InitTimeToLength(CH_C_TIMETOLENGTH);
    specc->InitFullRange(CH_C_FULLRANGE);
  }
}

VdcSetup::~VdcSetup()
{
  if (rcsid[0] == '@') return;
}

void
VdcSetup::Reset()
{
  if ( speca != NULL ) speca->Reset();
  if ( specb != NULL ) specb->Reset();
  if ( specc != NULL ) specc->Reset();
}

VdcApacket::VdcApacket(VdcPlane *x_1, VdcPlane *s_1,
		       VdcPlane *x_2, VdcPlane *s_2)
  : VdcPacket(x_1, s_1, x_2, s_2)
{
  Init();
}

void
VdcApacket::Init()
{
  InitPositionAndOffset(&vdcPosOffsA, CH_A_ANGLE);
  InitTimeToLength(CH_A_TIMETOLENGTH);
  InitFullRange(CH_A_FULLRANGE);
  InitIgnoreCriteria(0,0,1000,0,0,0,1000,0);
  ident = new char[2]; ident[0] = 'A'; ident[1] = 0;
}

VdcBpacket::VdcBpacket(VdcPlane *x_1, VdcPlane *s_1,
		       VdcPlane *x_2, VdcPlane *s_2)
  : VdcPacket(x_1, s_1, x_2, s_2)
{
  Init();
}

void
VdcBpacket::Init()
{
  InitPositionAndOffset(&vdcPosOffsB, CH_B_ANGLE);
  InitTimeToLength(CH_B_TIMETOLENGTH);
  InitFullRange(CH_B_FULLRANGE);
  InitIgnoreCriteria(0,0,1000,0,0,0,1000,0);
  //InitIgnoreCriteria(1, 165, 270, -0.26, -1, 220, 350, -0.27);
  ident = new char[2]; ident[0] = 'B'; ident[1] = 0;
}

VdcCpacket::VdcCpacket(VdcPlane *x_1, VdcPlane *s_1,
		       VdcPlane *x_2, VdcPlane *s_2)
  : VdcPacket(x_1, s_1, x_2, s_2)
{
  Init();
}

void
VdcCpacket::Init()
{
  InitPositionAndOffset(&vdcPosOffsC, CH_C_ANGLE);
  InitTimeToLength(CH_C_TIMETOLENGTH);
  InitFullRange(CH_C_FULLRANGE);
  InitIgnoreCriteria(0,0,1000,0,0,0,1000,0);
  ident = new char[2]; ident[0] = 'C'; ident[1] = 0;
}

VdcProtoPacket::VdcProtoPacket(VdcPlane *x, VdcPlane *s)
  : VdcPacket(x, s, NULL, NULL)
{
  Init();
}

void
VdcProtoPacket::Init()
{
  InitPositionAndOffset(&vdcPosOffsProto, CH_P_ANGLE);
  InitTimeToLength(CH_P_TIMETOLENGTH);
  InitFullRange(CH_P_FULLRANGE);
  ident = new char[2]; ident[0] = 'P'; ident[1] = 0;
}

VdcOHIPSpacket::VdcOHIPSpacket(VdcPlane *u1, VdcPlane *v1,
			       VdcPlane *u2, VdcPlane *v2)
  : VdcPacket(u1, v1, u2, v2)
{
  Init();
}

void
VdcOHIPSpacket::Init()
{
  InitPositionAndOffset(&vdcPosOffsOhips, CH_OHIPS_ANGLE);
  InitTimeToLength(CH_OHIPS_TIMETOLENGTH);
  InitFullRange(CH_OHIPS_FULLRANGE);
  dist_u1_u2 = CH_OHIPS_DIST_U1_U2;
  dist_v1_v2 = CH_OHIPS_DIST_V1_V2;
  dist_u1_v1 = CH_OHIPS_DIST_U1_V1;
  u1Plane()->InitCorrection(CH_OHIPS_CORR1);
  v1Plane()->InitCorrection(CH_OHIPS_CORR1);
  u2Plane()->InitCorrection(CH_OHIPS_CORR2);
  v2Plane()->InitCorrection(CH_OHIPS_CORR2);
  ident = new char[2]; ident[0] = 'O'; ident[1] = 0;
}

void
VdcOHIPSpacket::Init98()
{
  for (int wire=1; wire<=CH_OHIPS_MAX_U1; wire++)
    position_ou1[wire-1] = CH_OHIPS_POSITION_U1_98 + 25.4/6.*wire;
  for (int wire=1; wire<=CH_OHIPS_MAX_V1; wire++)
    position_ov1[wire-1] = CH_OHIPS_POSITION_V1_98 + 25.4/6.*wire;
  for (int wire=1; wire<=CH_OHIPS_MAX_U2; wire++)
    position_ou2[wire-1] = CH_OHIPS_POSITION_U2_98 + sqrt(18.)*wire;
  for (int wire=1; wire<=CH_OHIPS_MAX_V2; wire++)
    position_ov2[wire-1] = CH_OHIPS_POSITION_V2_98 + sqrt(18.)*wire;
  InitPositionAndOffset(&vdcPosOffsOhips, CH_OHIPS_ANGLE);
  InitTimeToLength(CH_OHIPS_TIMETOLENGTH_98);
  InitFullRange(CH_OHIPS_FULLRANGE);
  dist_u1_u2 = CH_OHIPS_DIST_U1_U2_98;
  dist_v1_v2 = CH_OHIPS_DIST_V1_V2_98;
  dist_u1_v1 = CH_OHIPS_DIST_U1_V1_98;
  u1Plane()->InitCorrection(CH_OHIPS_CORR1_98);
  v1Plane()->InitCorrection(CH_OHIPS_CORR1_98);
  u2Plane()->InitCorrection(CH_OHIPS_CORR2);
  v2Plane()->InitCorrection(CH_OHIPS_CORR2);
  ident = new char[2]; ident[0] = 'O'; ident[1] = 0;
}

#ifdef LIKELIHOOD
void
VdcOHIPSpacket::CalcCoord(int bitmask)
{
  if (!(status & PACKET_STATUS_SLOPE_X)) {
    slope_x = calcSlope(x1, x2, dist_u1_u2);
    if (x1->NumberOfWires() > 0) x1->Regression(slope_x);
    if (x2->NumberOfWires() > 0) x2->Regression(slope_x);
    if ((x1->NumberOfWires() < 3) || (x2->NumberOfWires() < 3))
      slope_x = recalcSlope(x1, x2, dist_u1_u2);
    fitX = new VdcLikelihood(x1->fit, x2->fit, dist_u1_u2);
    fitX -> calc(slope_x);
    slope_x = fitX->sl0();
    status |= PACKET_STATUS_SLOPE_X;
  }

  if (!(status & PACKET_STATUS_SLOPE_S)) {
    slope_s = calcSlope(s1, s2, dist_v1_v2);
    if (s1->NumberOfWires() > 0)
      s1->Regression(slope_s);
    if (s2->NumberOfWires() > 0)
      s2->Regression(slope_s);
    if ((s1->NumberOfWires() < 3) || (s2->NumberOfWires() < 3))
      slope_s = recalcSlope(s1, s2, dist_v1_v2);
    fitS = new VdcLikelihood(s1->fit, s2->fit, dist_v1_v2);
    fitS -> calc(slope_s);
    slope_s = fitS->sl0();
    status |= PACKET_STATUS_SLOPE_S;
  }

  if (bitmask & PACKET_STATUS_X) {
    status |= PACKET_STATUS_X;
    x_ = (fitX->x0() + fitS->x0() - dist_u1_v1/slope_s) * M_SQRT1_2;
    return;
  }

  if (bitmask & PACKET_STATUS_Y) {
    status |= PACKET_STATUS_Y;
    y_ = (-fitX->x0() + fitS->x0() - dist_u1_v1/slope_s) * M_SQRT1_2;
    return;
  }

  if (bitmask & PACKET_STATUS_THETA) {
    status |= PACKET_STATUS_THETA;
    theta_ = 1E3 * atan((1.0/slope_x + 1.0/slope_s)*M_SQRT1_2);
    return;
  }

  if (bitmask & PACKET_STATUS_PHI) {
    status |= PACKET_STATUS_PHI;
    phi_ = 1E3 * atan((1.0/slope_s - 1.0/slope_x)*M_SQRT1_2);
    return;
  }

  if ((bitmask & PACKET_STATUS_dX) || (bitmask & PACKET_STATUS_dY)) {
    status |= PACKET_STATUS_dX + PACKET_STATUS_dY;
    dx_ = dy_ = sqrt(square(fitX->dx0()) + square(fitS->dx0()) +
		     square(dist_u1_v1*fitS->dsl0()/
			    square(slope_s))) * M_SQRT1_2;
    return;
  }

  if (bitmask & PACKET_STATUS_dTHETA) {
    status |= PACKET_STATUS_dTHETA;
    double help = 2.0 /
      (2.0*square(slope_x)*square(slope_s) + square(slope_x + slope_s));
    dtheta_ = 1E3 * sqrt(square(help*square(slope_s)*fitX->dsl0()) +
			 square(help*square(slope_x)*fitS->dsl0()));
    return;
  }

  if (bitmask & PACKET_STATUS_dPHI) {
    status |= PACKET_STATUS_dPHI;
    double help = 2.0 /
      (2.0*square(slope_x)*square(slope_s) + square(slope_x - slope_s));
    dphi_ = 1E3 * sqrt(square(help*square(slope_s)*fitX->dsl0()) +
			 square(help*square(slope_x)*fitS->dsl0()));
    return;
  }
}
#else /* LIKELIHOOD */
void
VdcOHIPSpacket::CalcCoord(int bitmask)
{
  if (!(status & PACKET_STATUS_SLOPE_X)) {
    slope_x = calcSlope(x1, x2, dist_u1_u2);
    if (x1->NumberOfWires() > 0) x1->Regression(slope_x);
    if (x2->NumberOfWires() > 0) x2->Regression(slope_x);
    if ((x1->NumberOfWires() < 3) || (x2->NumberOfWires() < 3))
      recalcSlope(x1, x2, dist_u1_u2);
    *regX = *(x1->reg) + (*(x2->reg) + dist_u1_u2);
    regX -> calc();
    slope_x = regX->sl0();
    status |= PACKET_STATUS_SLOPE_X;
  }

  if (!(status & PACKET_STATUS_SLOPE_S)) {
    slope_s = calcSlope(s1, s2, dist_v1_v2);
    if (s1->NumberOfWires() > 0)
      s1->Regression(slope_s);
    if (s2->NumberOfWires() > 0)
      s2->Regression(slope_s);
    if ((s1->NumberOfWires() < 3) || (s2->NumberOfWires() < 3))
      recalcSlope(s1, s2, dist_v1_v2);
    *regS = *(s1->reg) + (*(s2->reg) + dist_v1_v2);
    regS -> calc();
    slope_s = regS->sl0();
    status |= PACKET_STATUS_SLOPE_S;
  }

  if (bitmask & PACKET_STATUS_X) {
    status |= PACKET_STATUS_X;
    x_ = (regX->x0() + regS->x0() - dist_u1_v1/slope_s) * M_SQRT1_2;
    return;
  }

  if (bitmask & PACKET_STATUS_Y) {
    status |= PACKET_STATUS_Y;
    y_ = (-regX->x0() + regS->x0() - dist_u1_v1/slope_s) * M_SQRT1_2;
    return;
  }

  if (bitmask & PACKET_STATUS_THETA) {
    status |= PACKET_STATUS_THETA;
    theta_ = 1E3 * atan((1.0/slope_x + 1.0/slope_s)*M_SQRT1_2);
    return;
  }

  if (bitmask & PACKET_STATUS_PHI) {
    status |= PACKET_STATUS_PHI;
    phi_ = 1E3 * atan((1.0/slope_s - 1.0/slope_x)*M_SQRT1_2);
    return;
  }

  if ((bitmask & PACKET_STATUS_dX) || (bitmask & PACKET_STATUS_dY)) {
    status |= PACKET_STATUS_dX + PACKET_STATUS_dY;
    dx_ = dy_ = sqrt(square(regX->dx0()) + square(regS->dx0()) +
		     square(dist_u1_v1*regS->dsl0()/
			    square(slope_s))) * M_SQRT1_2;
    return;
  }

  if (bitmask & PACKET_STATUS_dTHETA) {
    status |= PACKET_STATUS_dTHETA;
    double help = 2.0 /
      (2.0*square(slope_x)*square(slope_s) + square(slope_x + slope_s));
    dtheta_ = 1E3 * sqrt(square(help*square(slope_s)*regX->dsl0()) +
			 square(help*square(slope_x)*regS->dsl0()));
    return;
  }

  if (bitmask & PACKET_STATUS_dPHI) {
    status |= PACKET_STATUS_dPHI;
    double help = 2.0 /
      (2.0*square(slope_x)*square(slope_s) + square(slope_x - slope_s));
    dphi_ = 1E3 * sqrt(square(help*square(slope_s)*regX->dsl0()) +
			 square(help*square(slope_x)*regS->dsl0()));
    return;
  }
}
#endif /* LIKELIHOOD */
