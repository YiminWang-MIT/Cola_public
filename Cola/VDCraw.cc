 //
 // $Id: VDCraw.cc 2354 2009-11-13 13:58:04Z merkel $
 //
 // initialize vdc's and fill raw histograms
 //

 #include "VDCraw.h"
 #include "Aqua_abcn.h"
 #include "Aqua_online.h"
#include "rundatabase.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>

// initialize static variables
int vdcBase::vdc_op =
(getenv("VDC_OP") ? strtol(getenv("VDC_OP"), NULL, 16) : 0);

//
// vdcHIST
//

vdcHIST::vdcHIST(AquaTree *atree, VdcPlane *vdcplane, struct vdc *onlptr)
{
  out = atree;
  plane = vdcplane;
  onl = onlptr;

  char pstr[4];
  char *str = (char *) strstr(plane->NameOfPlane(), ".det.vdc.");
  char spec = str[-1] & ~32; // upper case
  strncpy(pstr, str+9, 2);
  pstr[2] = 0;

  char title[64];
  for (int i = 0; i < 16; i++) {
    sprintf(title, "Spec. %c/%s-layer/Paddle/%d", spec, pstr, i);
    paddle[i] = HMFind(title);
  }
  sprintf(title, "Spec. %c/%s-layer/Drift Time", spec, pstr);
  drift = HMFind(title);
  sprintf(title, "Spec. %c/%s-layer/Nr of Wire", spec, pstr);
  wire  = HMFind(title);
  sprintf(title, "Spec. %c/%s-layer/Good Wires", spec, pstr);
  good  = HMFind(title);
  sprintf(title, "Spec. %c/%s-layer/Bad Wires", spec, pstr);
  bad   = HMFind(title);
  sprintf(title, "Spec. %c/%s-layer/Tagged Wire", spec, pstr);
  tagged= HMFind(title); 
  sprintf(title, "Spec. %c/%s-layer/Efficiency", spec, pstr);
  if ((eff = HMFind(title))) {
    eff->unity = "%";
    eff->labely = "Efficiency";
    eff->divide = tagged;
  }
  sprintf(title, "Spec. %c/%s-layer/Cell Time", spec, pstr);
  cell  = HMFind(title);
  sprintf(title, "Spec. %c/%s-layer/Width of Cell", spec, pstr);
  pos   = HMFind(title);
  sprintf(title, "Spec. %c/%s-layer/Time vs Wire", spec, pstr);
  tvw = HMFind(title);
  sprintf(title, "Spec. %c/%s-layer/Minimal Drift Time", spec, pstr);
  minDriftTime = HMFind(title);
  sprintf(title, "Spec. %c/%s-layer/MDT vs Nr", spec, pstr);
  mDT_Nwire= HMFind(title);
  sprintf(title, "Spec. %c/%s-layer/Number of wires", spec, pstr);
  Nwire= HMFind(title);
}  

vdcHIST::~vdcHIST()
{
  ;
}

int vdcHIST::handle(int npaddle)
{
  plane->Regression(plane->slope());
  short num = onl->multi = plane->NumberOfWires();
  // jan : hack to get only good wires!
  //  onl->multi=plane->GoodWires();

  onl->error   = plane->errorCode();
  onl->time    = onl->wire = -1;
  onl->pattern = plane->Pattern();
  onl->x       = plane->x();
  onl->slope   = plane->slope();
  onl->chi_sqr = plane->chi_sqr();
  
  out->packEventData(&onl->time,    1);
  out->packEventData(&onl->multi,   1);
  out->packEventData(&onl->error,   1);
  out->packEventData(&onl->pattern, 1);
  out->packEventData(&onl->x,       1);
  out->packEventData(&onl->slope,   1);
  out->packEventData(&onl->chi_sqr, 1);

  const short *wire_ptr = plane->WiresFired();
  const short *time_ptr = plane->RawDriftTimes();

  for (int w=0; w<8; w++) {
    if (w<num) {
      onl->raw_time[w] = time_ptr[w];
      onl->raw_wire[w] = wire_ptr[w];  
    }
    else  {
      onl->raw_time[w] = -1;
      onl->raw_wire[w] = -1;  
    }
  }
  out->packEventData(&onl->raw_time[0],    8);
  out->packEventData(&onl->raw_wire[0],    8);

  double minimalDriftTime=1023;
  int totalWire = 0;
  while (num-- > 0) { 
    if (num >= plane->GoodWires())
      // Bad wires
      HMFill(bad, wire_ptr[num], 0, 1);
    else {
      totalWire++;
      // Good wires
      HMFill(good, wire_ptr[num], 0, 1);
      HMFill(drift, time_ptr[num], 0, 1);
      if (time_ptr[num]<minimalDriftTime) minimalDriftTime=time_ptr[num];
      HMFill(tvw, wire_ptr[num], time_ptr[num], 1);
      if (paddle[npaddle])
	HMFill(paddle[npaddle], time_ptr[num], 0, 1);
    }
    // All wires
    HMFill(wire,  wire_ptr[num], 0, 1);
  }
  HMFill(minDriftTime, minimalDriftTime, 0, 1);
  HMFill(mDT_Nwire, minimalDriftTime, totalWire, 1);
  HMFill(Nwire, totalWire, 0, 1);
  
  onl->mdt = minimalDriftTime;
  out->packEventData(&onl->mdt, 1);
  out->packEventData(&onl->nw, 1);
  const float *path = plane->DriftPath();
  const float *corr = plane->DriftCorrection();

  num = plane->GoodWires() - 1 ;
  while (num-- > 0) {
    if (num) { 
      HMFill(tagged, wire_ptr[num], 0, 0.01);
      HMFill(eff,    wire_ptr[num], 0, 1);
    }
    int num2 = wire_ptr[num+1] - wire_ptr[num];
    if (abs(num2) == 1) {
      if (pos)
	HMFill(pos, fabs((path[num+1]+corr[num+1]-path[num]-corr[num]) 
			 / onl->slope), 
	       wire_ptr[num], 1);
      if (cell)
	HMFill(cell, (time_ptr[num+1] - time_ptr[num]) / abs(num2), 
	       wire_ptr[num], 1);
    }

    while (num2-- > 1) 
      HMFill(tagged, wire_ptr[num+1]-num2, 0, 0.01);
  }
  return 0;
}

//
// vdcBase
//

vdcBase::vdcBase(AquaTree *atree, AquaTree *outTree,
		 Spectrometer *onlPtr, SPECTROMETER *rdbPtr, VdcPacket *vdc)
{
    packet = vdc;
    subX1 = atree->getShortSubTree(packet->x1Plane()->NameOfPlane(), 1);
    subS1 = atree->getShortSubTree(packet->s1Plane()->NameOfPlane(), 1);
    subX2 = atree->getShortSubTree(packet->x2Plane()->NameOfPlane(), 1);
    subS2 = atree->getShortSubTree(packet->s2Plane()->NameOfPlane(), 1);
    out = outTree;
    onl = onlPtr;
    rdb = rdbPtr;
    histX1 = new vdcHIST(out, x1Plane(), &onl->x1);
    histS1 = new vdcHIST(out, s1Plane(), &onl->s1);
    histX2 = new vdcHIST(out, x2Plane(), &onl->x2);
    histS2 = new vdcHIST(out, s2Plane(), &onl->s2);
    wiretime = new u_int16_t[2*maxWire];
}

vdcBase::~vdcBase()
{
  delete histX1;
  delete histS1;
  delete histX2;
  delete histS2;
  delete wiretime;
}

int vdcBase::disable(double *x1disabled, double *s1disabled,
		     double *x2disabled, double *s2disabled, int max) 
{ 
  for (int i=0;i<max;i++) {
    if (x1disabled[i] != -1)
      packet->x1Plane()->disableWire((int) x1disabled[i]);
    if (s1disabled[i] != -1)
      packet->s1Plane()->disableWire((int) s1disabled[i]);
    if (x2disabled[i] != -1)
      packet->x2Plane()->disableWire((int) x2disabled[i]);
    if (s2disabled[i] != -1)
      packet->s2Plane()->disableWire((int) s2disabled[i]);
    }
    return 0;
}

int vdcBase::vdcSpecials()
{
  printf("Info: doing specials, vdc_op=%d", vdc_op);
  if (vdc_op & 0x000888) {
    double toff = rdb->scint.ToF[onl->ToF.paddle - 1];
    //  - rdb->scint.walk[2] * securesqrt(onl.dE.Energy.right);
    //  (would this be correct?)
    if (vdc_op & 0x000ccc) printf("dx was %f\n", packet->dx());
    packet->DoTheLinearRegressionAgainWithCorrectOffset( - toff / 0.5);
    // (0.5 ns/channel)
    if (vdc_op & 0x000ccc) printf("dx is  %f (Offset %f)\n", 
				  packet->dx(), -toff/0.5);
  }
  else {
    int plane1 = vdc_op & 0x000111;
    int plane2 = vdc_op & 0x000222;
    if (plane1 && plane2) {
      onl->z_fp = packet->DoTheLinearRegressionSeparately();
      if (onl->z_fp!=-4711)
	  out->packEventData(&onl->z_fp, 1);
    }
    else {
      if (vdc_op & 0x000111)
	packet->DoTheLinearRegressionAgainWithX1S1Only();
      else if (vdc_op & 0x000222)
	packet->DoTheLinearRegressionAgainWithX2S2Only();
    }
  }
  return 0;
}

int vdcBase::handle()
{

  if (!packet) return -1;

  int wires;

  if ((wires = subX1->pack(wiretime, maxWire)) > 0)
    x1Plane()->FillTime((chRawData *)wiretime, 2*wires);
  if ((wires = subS1->pack(wiretime, maxWire)) > 0)
    s1Plane()->FillTime((chRawData *)wiretime, 2*wires);
  if ((wires = subX2->pack(wiretime, maxWire)) > 0)
    x2Plane()->FillTime((chRawData *)wiretime, 2*wires);
  if ((wires = subS2->pack(wiretime, maxWire)) > 0)
    s2Plane()->FillTime((chRawData *)wiretime, 2*wires);
  
  int nPaddle = rdb->scint.use_dE==0 ? onl->ToF.paddle-1 : onl->dE.paddle-1;
  packet->CorrectPaddleOffset(nPaddle);
  if (vdc_op & 0x000f00) vdcSpecials();

  if (nPaddle < 0) nPaddle = 0;
  if (nPaddle > 15) nPaddle = 15;

  histX1->handle(nPaddle);
  histS1->handle(nPaddle);
  histX2->handle(nPaddle);
  histS2->handle(nPaddle);

  if      (packet->ok3() == PACKET_STATUS_BOTH) onl->vdcOK = 3;
  else if (packet->ok()  == PACKET_STATUS_BOTH) onl->vdcOK = 2;
  else if (packet->ok1() == PACKET_STATUS_BOTH) onl->vdcOK = 1;
  else                                          onl->vdcOK = 0;
  out->packEventData(&onl->vdcOK, 1);

  return 0;
}

int vdcBase::handle(double &x, double &th, double &y, double &ph)
{
    onl->x     = x  = packet->x();
    onl->y     = y  = packet->y();
    onl->theta = th = packet->theta();
    onl->phi   = ph = packet->phi();
    out->packEventData(&(onl->x), 4);

    if (fabs(ph)<1e10 && fabs(th)<1e10 && fabs(x)<1e10 && fabs(y)<1e10) {
      onl->dx     = packet->dx();
      onl->dy     = packet->dy();
      onl->dtheta = packet->dtheta();
      onl->dphi   = packet->dphi();
      out->packEventData(&(onl->dx), 4);
      return 1;
    }

    return 0;
}

vdcSpecA::vdcSpecA(AquaTree *atree, AquaTree *outTree, const char *name,
		   Spectrometer *onlPtr, SPECTROMETER *rdbPtr,
		   int ax1size, int as1size, int ax2size, int as2size)
  : vdcBase(atree, outTree, onlPtr, rdbPtr,
	    new VdcApacket(NULL, name, ax1size, as1size, ax2size, as2size))
{
    short *ax1offs, *as1offs, *ax2offs, *as2offs, index;

    ax1offs = new short[ax1size];
    for (index=0; index<ax1size; index++) ax1offs[index] = 434;
    as1offs = new short[as1size];
    for (index=0; index<as1size; index++) as1offs[index] = 428;
    ax2offs = new short[ax2size];
    for (index=0; index<ax2size; index++) ax2offs[index] = 427;
    as2offs = new short[as2size];
    for (index=0; index<320; index++) as2offs[index] = 424;
    for (index=320; index<as2size; index++) as2offs[index] = 418;
    packet->InitOffset(ax1offs,as1offs,ax2offs,as2offs);
    packet->InitTimeToLength(0.052 * 0.50);	// driftvelocity 52 um/ns
						// resolution   .75 ns/bit
}

vdcSpecB::vdcSpecB(AquaTree *atree, AquaTree *outTree, const char *name,
		   Spectrometer *onlPtr, SPECTROMETER *rdbPtr,
		   int bx1size, int bs1size, int bx2size, int bs2size)
    : vdcBase(atree, outTree, onlPtr, rdbPtr,
	      new VdcBpacket(NULL, name, bx1size, bs1size, bx2size, bs2size))
{
    short *bx1offs, *bs1offs, *bx2offs, *bs2offs, index;

    bx1offs = new short[bx1size];
    for (index=0; index<bx1size; index++) bx1offs[index] = 460;
    bs1offs = new short[bs1size];
    for (index=0; index<bs1size; index++) bs1offs[index] = 458;
    bx2offs = new short[bx2size];
    for (index=0; index<bx2size; index++) bx2offs[index] = 457;
    bs2offs = new short[bs2size];
    for (index=0; index<bs2size; index++) bs2offs[index] = 455;
    packet->InitOffset(bx1offs,bs1offs,bx2offs,bs2offs);
    packet->InitTimeToLength(0.050 * 0.50);	// driftvelocity 50 um/ns
						// resolution   .75 ns/bit
}

vdcSpecC::vdcSpecC(AquaTree *atree, AquaTree *outTree, const char *name,
		   Spectrometer *onlPtr, SPECTROMETER *rdbPtr,
		   int cx1size, int cs1size, int cx2size, int cs2size)
    : vdcBase(atree, outTree, onlPtr, rdbPtr,
	      new VdcCpacket(NULL, name, cx1size, cs1size, cx2size, cs2size))
{
    short *cx1offs, *cs1offs, *cx2offs, *cs2offs, index;

    cx1offs = new short[cx1size];
    for (index=0; index<cx1size; index++) cx1offs[index] = 2490;//2467;
    cs1offs = new short[cs1size];
    for (index=0; index<cs1size; index++) cs1offs[index] = 2490;//2476;
    cx2offs = new short[cx2size];
    for (index=0; index<cx2size; index++) cx2offs[index] = 2487;//2477;
    cs2offs = new short[cs2size];
    for (index=0; index<cs2size; index++) cs2offs[index] = 2489;//2477;
    packet->InitOffset(cx1offs,cs1offs,cx2offs,cs2offs);
    packet->InitTimeToLength(0.052 * 0.25);	// driftvelocity 52 um/ns
						// resolution   250 ps/bit
}
