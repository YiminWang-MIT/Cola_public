//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-456      tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaMIT/VDCraw.cc,v 2.1 2002-03-05 03:49:49 distler Exp $
//
// initialize OHIPS VDC and make raw histograms
//

#include "VDCraw.h"
#include <math.h>

//
// Function name : fillplane(Aqua_online *out, VdcPlane *plane, Vdc *onl,
//                           HIST *drift, HIST *wire, HIST *tagged, HIST *eff)
//

void
fillplane(Aqua_online *out, VdcPlane *plane, Vdc *onl,
	  HIST *drift, HIST *wire, HIST *tagged, HIST *eff)
{
    short num;

    onl->time    = -1;
    onl->wire    = -1;
    onl->multi   = num = plane->NumberOfWires();
    onl->error   = plane->errorCode();
    onl->pattern = plane->Pattern();
    onl->x       = plane->x();
    onl->slope   = plane->slope();
    onl->chi_sqr = plane->chi_sqr();
    out->packEventData(&onl->time, 5);
    out->packEventData(&onl->x, 3);
  
    const short *wire_ptr = plane->WiresFired();
    const short *time_ptr = plane->RawDriftTimes();

    while (num-- > 0) {
	HMFill(wire,  wire_ptr[num], 0, 1);
	HMFill(drift, time_ptr[num], 0, 1);
    }

    num = plane->GoodWires() - 1 ;
    while (num-- > 1) {
	HMFill(tagged, wire_ptr[num], 0, 0.01);
	HMFill(eff,    wire_ptr[num], 0, 1);
	int num2 = wire_ptr[num+1] - wire_ptr[num];
	while (num2-- > 1) 
	    HMFill(tagged, wire_ptr[num+1]-num2, 0, 0.01);
    }	

} // end of fillplane(Aqua_online *, VdcPlane *, struct Vdc *, HIST *, ...)


//
// Function name : vdccoord(struct FocalCoord *focal,
//                          double x, double th, double y, double ph)
//
//

void
vdccoord(Aqua_online *out, FocalCoord *focal,
	 double x, double th, double y, double ph)
{
    focal->x  = x;
    focal->th = th;
    focal->y  = y;
    focal->ph = ph;
    out->packEventData(&focal->x, 4);
} // end of vdccoord(Aqua_online *, FocalCoord *, double, ...)


//
// Function name : FPcoord(struct FocalCoord *chamber,
//                         struct FocalCoord *focal, double *param)
//
//

void
FPcoord(Aqua_online *out, FocalCoord *chamber,
	FocalCoord *focal, double *param)
{
  if ((chamber == NULL) || (focal == NULL) || (param == NULL)) return;

  if ((param[0] == 0.0) && (param[1] == 0.0) && (param[2] == 0.0)) {
    focal->x  = chamber->x;
  } else {
    double trackAngle = M_PI_2-chamber->th*0.001;
    double FPslope = tan(param[2]);
    double tanTrack = tan(trackAngle);
    double x0 = (tanTrack*chamber->x-FPslope*param[0])/(tanTrack-FPslope);
    focal->x = (x0 - param[0]) / cos(param[2]) - param[1];
  }

  focal->th = chamber->th -
    (param[3] + chamber->x * (param[4] + chamber->x * param[5]));

  focal->y  = chamber->y;
  focal->ph = chamber->ph;
  out->packEventData(&focal->x, 4);
} // end of vdccoord(struct FocalCoord *, double, double, double, double)


//
// Function name : vdcOHIPSinit(VdcOHIPSpacket& vdc,
//                              int u1size, int v1size, int u2size, int v2size)
//
// Description   : 
// Input         : 
// Output        : 
// Return        : 
//

void
vdcOHIPSinit(VdcOHIPSpacket& vdc,
	     int u1size, int v1size, int u2size, int v2size)
{
  short *u1offs, *v1offs, *u2offs, *v2offs, index;

  u1offs = new short[u1size];
  for (index=0; index<u1size; index++) u1offs[index] = 450;
  v1offs = new short[v1size];
  for (index=0; index<v1size; index++) v1offs[index] = 447;
  u2offs = new short[u2size];
  for (index=0; index<u2size; index++) u2offs[index] = 451;
  v2offs = new short[v2size];
  for (index=0; index<v2size; index++) v2offs[index] = 452;

  vdc.InitOffset(u1offs, v1offs, u2offs, v2offs);
} // end of vdcOHIPSinit(VdcOHIPSpacket&, int, int, int, int)


//
// Function name : vdcOHIPSraw(VdcOHIPSpacket& vdc,
//                             struct SpectrometerOHIPS *onl)
//

void
vdcOHIPSraw(Aqua_online *out, VdcOHIPSpacket& vdc, SpectrometerOHIPS *onl)
{
    static HIST *u1drift, *u1wire, *u1tagged, *u1eff;
    static HIST *v1drift, *v1wire, *v1tagged, *v1eff;
    static HIST *u2drift, *u2wire, *u2tagged, *u2eff;
    static HIST *v2drift, *v2wire, *v2tagged, *v2eff;
    static HIST *lastfirst = NULL, *lastlast = NULL;

    if (lastfirst != HMFirst || lastlast != HMLast) {
	u1drift = HMFind("OHIPS/chamber/u1/Drift Time");
	u1wire  = HMFind("OHIPS/chamber/u1/Nr of Wire");
	u1tagged= HMFind("OHIPS/chamber/u1/Tagged Wire"); 
	u1eff   = HMFind("OHIPS/chamber/u1/Efficiency");
	v1drift = HMFind("OHIPS/chamber/v1/Drift Time");
	v1wire  = HMFind("OHIPS/chamber/v1/Nr of Wire");
	v1tagged= HMFind("OHIPS/chamber/v1/Tagged Wire"); 
	v1eff   = HMFind("OHIPS/chamber/v1/Efficiency");
	u2drift = HMFind("OHIPS/chamber/u2/Drift Time");
	u2wire  = HMFind("OHIPS/chamber/u2/Nr of Wire");
	u2tagged= HMFind("OHIPS/chamber/u2/Tagged Wire"); 
	u2eff   = HMFind("OHIPS/chamber/u2/Efficiency");
	v2drift = HMFind("OHIPS/chamber/v2/Drift Time");
	v2wire  = HMFind("OHIPS/chamber/v2/Nr of Wire");
	v2tagged= HMFind("OHIPS/chamber/v2/Tagged Wire"); 
	v2eff   = HMFind("OHIPS/chamber/v2/Efficiency");

	lastfirst = HMFirst;
	lastlast  = HMLast;

	if (u1eff) {
	    u1eff->unity  = "%"; 
	    u1eff->labely ="Efficiency"; 
	    u1eff->divide = u1tagged;
	}
	if (v1eff) {
	    v1eff->unity  = "%"; 
	    v1eff->labely = "Efficiency"; 
	    v1eff->divide = v1tagged;
	}
	if (u2eff) {
	    u2eff->unity  = "%"; 
	    u2eff->labely = "Efficiency"; 
	    u2eff->divide = u2tagged;
	}
	if (v2eff) {
	    v2eff->unity  = "%"; 
	    v2eff->labely = "Efficiency"; 
	    v2eff->divide = v2tagged;
	}
    }

    fillplane(out, vdc.u1Plane(), &onl->u1, u1drift, u1wire, u1tagged, u1eff);
    fillplane(out, vdc.v1Plane(), &onl->v1, v1drift, v1wire, v1tagged, v1eff);
    fillplane(out, vdc.u2Plane(), &onl->u2, u2drift, u2wire, u2tagged, u2eff);
    fillplane(out, vdc.v2Plane(), &onl->v2, v2drift, v2wire, v2tagged, v2eff);
} // end of vdcOHIPSraw(Aqua_online *, VdcOHIPSpacket&, SpectrometerOHIPS *)
