//                                                                    -*-c++-*-
// authors:
//
// Michael O. Distler                        mailto:distler@mit.edu
// MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// Nikos Kaloskamis                          mailto:kaloskamis@bates.mit.edu
// MIT, Lab for Nuclear Science, 26-456      tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaMIT/StatPage.cc,v 2.3 2002-12-12 23:52:12 distler Exp $
//
// Classes to calculate and print a statistics page
//

#include "StatPage.h"
#include <iomanip>
#include <cstring>

const int w = 12;

statPage::statPage(AquaTree * aquatree) : statPageBase(aquatree)
{
  startTime = 0;
  clear();
}

statPage::~statPage() { ; }

int
statPage::clear()
{
  event3 = event5 = event6 = event8 = event10 = event13 = 0;

  OOPSAsingle = OOPSBsingle = OOPSCsingle = OOPSDsingle = OHIPSsingle = 0;
  OOPSAsincb = OOPSAfired = OOPSAsin12 = OOPSAsin123 = 
  OOPSBsincb = OOPSBfired = OOPSBsin12 = OOPSBsin123 = 
  OOPSCsincb = OOPSCfired = OOPSCsin12 = OOPSCsin123 = 
  OOPSDsincb = OOPSDfired = OOPSDsin12 = OOPSDsin123 = 0;

  // clear scaler counters
  memset(&oopsAscaler, 0, sizeof(struct oopsScalerInfo));
  memset(&oopsBscaler, 0, sizeof(struct oopsScalerInfo));
  memset(&oopsCscaler, 0, sizeof(struct oopsScalerInfo));
  memset(&oopsDscaler, 0, sizeof(struct oopsScalerInfo));

  memset(&heli_positivescaler, 0, sizeof(struct heliScalerInfo));
  memset(&heli_negativescaler, 0, sizeof(struct heliScalerInfo));

  memset(&oopslivescaler, 0, sizeof(struct oopsliveScalerInfo));

  memset(&ohipsscaler, 0, sizeof(struct ohipsScalerInfo));

  memset(&eve5scaler, 0, sizeof(struct eve5ScalerInfo));
  memset(&eve6scaler, 0, sizeof(struct eve6ScalerInfo));

  return 0;
}

int
statPage::handleOOPSscaler(struct oops_scaler * scaler,
			   struct oopsScalerInfo * counter)
{
  if (atree->itemOK(&scaler->sincb))
    counter->sincb += scaler->sincb;
  if (atree->itemOK(&scaler->fired))
    counter->fired += scaler->fired;
  if (atree->itemOK(&scaler->prescaler))
    counter->prescaler += scaler->prescaler;
  if (atree->itemOK(&scaler->and_123))
    counter->and_123 += scaler->and_123;
  if (atree->itemOK(&scaler->and_12))
    counter->and_12 += scaler->and_12;

  for (int i=0; i<3; i++) {
    if (atree->itemOK(&scaler->scint[i+1].right))
      counter->scint[i].right += scaler->scint[i+1].right;
    if (atree->itemOK(&scaler->scint[i+1].left))
      counter->scint[i].left  += scaler->scint[i+1].left;
    if (atree->itemOK(&scaler->scint[i+1].left_AND_right))
      counter->scint[i].left_AND_right  += scaler->scint[i+1].left_AND_right;
  }

  if (atree->itemOK(&scaler->scint[2].low.right))
    counter->scint[1].low.right  += scaler->scint[2].low.right;
  if (atree->itemOK(&scaler->scint[2].low.left))
    counter->scint[1].low.left  += scaler->scint[2].low.left;

  for (int i=0; i<3; i++) {
    if (atree->itemOK(&scaler->hdc[i+1].x.top))
      counter->hdc[i].x.top += scaler->hdc[i+1].x.top;
    if (atree->itemOK(&scaler->hdc[i+1].x.bottom))
      counter->hdc[i].x.bottom  += scaler->hdc[i+1].x.bottom;
    if (atree->itemOK(&scaler->hdc[i+1].y.right))
      counter->hdc[i].y.right += scaler->hdc[i+1].y.right;
    if (atree->itemOK(&scaler->hdc[i+1].y.left))
      counter->hdc[i].y.left  += scaler->hdc[i+1].y.left;
  }

  if (atree->itemOK(&scaler->prescalercb))
    counter->prescalercb += scaler->prescalercb;
  if (atree->itemOK(&scaler->and_123cb))
    counter->and_123cb += scaler->and_123cb;
  if (atree->itemOK(&scaler->and_12cb))
    counter->and_12cb += scaler->and_12cb;

  return 0;
}

int
statPage::handleHELIscaler(struct helicity_scaler * scaler,
			   struct heliScalerInfo * counter)
{
  if (atree->itemOK(&scaler->oopsA_prescal))
    counter->oopsA_prescal += scaler->oopsA_prescal;
  if (atree->itemOK(&scaler->oopsB_prescal))
    counter->oopsB_prescal += scaler->oopsB_prescal;
  if (atree->itemOK(&scaler->oopsC_prescal))
    counter->oopsC_prescal += scaler->oopsC_prescal;
  //if (atree->itemOK(&scaler->oopsD_prescal))
  //  counter->oopsD_prescal += scaler->oopsD_prescal;
  if (atree->itemOK(&scaler->ohips_prescal))
    counter->ohips_prescal += scaler->ohips_prescal;
  if (atree->itemOK(&scaler->oops_prescal_OR))
    counter->oops_prescal_OR += scaler->oops_prescal_OR;
  if (atree->itemOK(&scaler->coincidence))
    counter->coincidence += scaler->coincidence;
  if (atree->itemOK(&scaler->oops_live))
    counter->oops_live += scaler->oops_live;
  if (atree->itemOK(&scaler->ohips_live))
    counter->ohips_live += scaler->ohips_live;
  if (atree->itemOK(&scaler->oops_trigger))
    counter->oops_trigger += scaler->oops_trigger;
  if (atree->itemOK(&scaler->ohips_trigger))
    counter->ohips_trigger += scaler->ohips_trigger;
  if (atree->itemOK(&scaler->master_trigger))
    counter->master_trigger += scaler->master_trigger;

  return 0;
}

int
statPage::handleOOPSLIVEscaler(struct oopslive_scaler * scaler,
			       struct oopsliveScalerInfo * counter)
{
  if (atree->itemOK(&scaler->prescalOR))
    counter->prescalOR += scaler->prescalOR;
  if (atree->itemOK(&scaler->retimingOR))
    counter->retimingOR += scaler->retimingOR;
  if (atree->itemOK(&scaler->live))
    counter->live += scaler->live;
  if (atree->itemOK(&scaler->trigger))
    counter->trigger += scaler->trigger;

  return 0;
}


int
statPage::handleOOPSLIVEscaler(struct oopslive_scaler_2000 * scaler,
			       struct oopsliveScalerInfo2000 * counter)
{
  if (atree->itemOK(&scaler->retimingORcb))
    counter->retimingORcb += scaler->retimingORcb;
  if (atree->itemOK(&scaler->singleORcb))
    counter->singleORcb += scaler->singleORcb;
  if (atree->itemOK(&scaler->livecb))
    counter->livecb += scaler->livecb;
  if (atree->itemOK(&scaler->triggercb))
    counter->triggercb += scaler->triggercb;
  if (atree->itemOK(&scaler->retimingOR))
    counter->retimingOR += scaler->retimingOR;
  if (atree->itemOK(&scaler->singleOR))
    counter->singleOR += scaler->singleOR;
  if (atree->itemOK(&scaler->live))
    counter->live += scaler->live;
  if (atree->itemOK(&scaler->trigger))
    counter->trigger += scaler->trigger;

  return 0;
}


int
statPage::handleOHIPSscaler(struct ohips_scaler * scaler,
			    struct ohipsScalerInfo * counter)
{
  if (atree->itemOK(&scaler->scaler53))
    counter->scaler53 += scaler->scaler53;
  if (atree->itemOK(&scaler->live54))
    counter->live54 += scaler->live54;
  if (atree->itemOK(&scaler->trigger))
    counter->trigger += scaler->trigger;
  if (atree->itemOK(&scaler->scaler106))
    counter->scaler106 += scaler->scaler106;
  if (atree->itemOK(&scaler->prescal))
    counter->prescal += scaler->prescal;
  if (atree->itemOK(&scaler->coincidence))
    counter->coincidence += scaler->coincidence;
  if (atree->itemOK(&scaler->live109))
    counter->live109 += scaler->live109;
  if (atree->itemOK(&scaler->AND_oopsA))
    counter->AND_oopsA += scaler->AND_oopsA;
  if (atree->itemOK(&scaler->AND_oopsB))
    counter->AND_oopsB += scaler->AND_oopsB;
  if (atree->itemOK(&scaler->AND_oopsC))
    counter->AND_oopsC += scaler->AND_oopsC;
  //  if (atree->itemOK(&scaler->AND_oopsD))
  //    counter->AND_oopsD += scaler->AND_oopsD;
  if (atree->itemOK(&scaler->scaler113))
    counter->scaler113 += scaler->scaler113;
  if (atree->itemOK(&scaler->scaler114))
    counter->scaler114 += scaler->scaler114;
  if (atree->itemOK(&scaler->scaler115))
    counter->scaler115 += scaler->scaler115;
  if (atree->itemOK(&scaler->scaler116))
    counter->scaler116 += scaler->scaler116;
  if (atree->itemOK(&scaler->scaler117))
    counter->scaler117 += scaler->scaler117;
  if (atree->itemOK(&scaler->event5))
    counter->event5 += scaler->event5;
  if (atree->itemOK(&scaler->bb))
    counter->bb += scaler->bb;
  if (atree->itemOK(&scaler->bbcb))
    counter->bbcb += scaler->bbcb;
  int index;
  for (index=1; index<=43; index++) {
    if (atree->itemOK(&scaler->sca[index]))
      counter->sca[index] += scaler->sca[index];
  }

  return 0;
}


int
statPage::handleOHIPSscaler(struct ohips_scaler_2000 * scaler,
			    struct ohipsScalerInfo2000 * counter)
{
  if (atree->itemOK(&scaler->trigger))
    counter->trigger += scaler->trigger;
  if (atree->itemOK(&scaler->prescal))
    counter->prescal += scaler->prescal;
  if (atree->itemOK(&scaler->coincidence))
    counter->coincidence += scaler->coincidence;
  if (atree->itemOK(&scaler->AND_oopsA))
    counter->AND_oopsA += scaler->AND_oopsA;
  if (atree->itemOK(&scaler->AND_oopsB))
    counter->AND_oopsB += scaler->AND_oopsB;
  if (atree->itemOK(&scaler->AND_oopsC))
    counter->AND_oopsC += scaler->AND_oopsC;
  if (atree->itemOK(&scaler->AND_oopsD))
    counter->AND_oopsD += scaler->AND_oopsD;
  if (atree->itemOK(&scaler->pit74))
    counter->pit74 += scaler->pit74;
  if (atree->itemOK(&scaler->pit80cb))
    counter->pit80cb += scaler->pit80cb;
  if (atree->itemOK(&scaler->coincb))
    counter->coincb += scaler->coincb;
  if (atree->itemOK(&scaler->pit90cb))
    counter->pit90cb += scaler->pit90cb;
  if (atree->itemOK(&scaler->spare1))
    counter->spare1 += scaler->spare1;
  if (atree->itemOK(&scaler->spare2))
    counter->spare2 += scaler->spare2;
  if (atree->itemOK(&scaler->live))
    counter->live += scaler->live;
  if (atree->itemOK(&scaler->livecb))
    counter->livecb += scaler->livecb;
  if (atree->itemOK(&scaler->triggercb))
    counter->triggercb += scaler->triggercb;
  if (atree->itemOK(&scaler->pit102))
    counter->pit102 += scaler->pit102;
  if (atree->itemOK(&scaler->prescalcb))
    counter->prescalcb += scaler->prescalcb;
  if (atree->itemOK(&scaler->AND_oopsAcb))
    counter->AND_oopsAcb += scaler->AND_oopsAcb;
  if (atree->itemOK(&scaler->AND_oopsBcb))
    counter->AND_oopsBcb += scaler->AND_oopsBcb;
  if (atree->itemOK(&scaler->AND_oopsCcb))
    counter->AND_oopsCcb += scaler->AND_oopsCcb;
  if (atree->itemOK(&scaler->AND_oopsDcb))
    counter->AND_oopsDcb += scaler->AND_oopsDcb;

  int index;
  for (index=1; index<=64; index++) {
    if (atree->itemOK(&scaler->sca[index]))
      counter->sca[index] += scaler->sca[index];
  }

  return 0;
}


int
statPage::handleEVE5scaler(struct event5 * scaler,
			   struct eve5ScalerInfo * counter)
{
  if (atree->itemOK(&scaler->bt3bic))
    counter->bt3bic += scaler->bt3bic;
  if (atree->itemOK(&scaler->computer_busy))
    counter->computer_busy += scaler->computer_busy;
  if (atree->itemOK(&scaler->event_5))
    counter->event5 += scaler->event_5;
  if (atree->itemOK(&scaler->scaler31))
    counter->scaler31 += scaler->scaler31;
  if (atree->itemOK(&scaler->busclc))
    counter->busclc += scaler->busclc;
  if (atree->itemOK(&scaler->coincidence))
    counter->coincidence += scaler->coincidence;
  if (atree->itemOK(&scaler->final_trigger))
    counter->final_trigger += scaler->final_trigger;
  if (atree->itemOK(&scaler->scaler57))
    counter->scaler57 += scaler->scaler57;
  if (atree->itemOK(&scaler->clock))
    counter->clock += scaler->clock;
  if (atree->itemOK(&scaler->sincb))
    counter->sincb += scaler->sincb;
  if (atree->itemOK(&scaler->retcb))
    counter->retcb += scaler->retcb;
  if (atree->itemOK(&scaler->live))
    counter->live += scaler->live;
  if (atree->itemOK(&scaler->cb))
    counter->cb += scaler->cb;
  if (atree->itemOK(&scaler->trig))
    counter->trig += scaler->trig;

  return 0;
}


int
statPage::handleEVE6scaler(struct event6 * scaler,
			   struct eve6ScalerInfo * counter)
{
  if (atree->itemOK(&scaler->final_trigger))
    counter->final_trigger += scaler->final_trigger;
  if (atree->itemOK(&scaler->coincidence))
    counter->coincidence += scaler->coincidence;
  if (atree->itemOK(&scaler->final_triggercb))
    counter->final_triggercb += scaler->final_triggercb;
  if (atree->itemOK(&scaler->coincidencecb))
    counter->coincidencecb += scaler->coincidencecb;
  if (atree->itemOK(&scaler->sync))
    counter->sync += scaler->sync;
  if (atree->itemOK(&scaler->synccb))
    counter->synccb += scaler->synccb;
  if (atree->itemOK(&scaler->cbinh))
    counter->cbinh += scaler->cbinh;
  if (atree->itemOK(&scaler->event_6))
    counter->event6 += scaler->event_6;
  if (atree->itemOK(&scaler->gun))
    counter->gun += scaler->gun;
  if (atree->itemOK(&scaler->guncb))
    counter->guncb += scaler->guncb;

  int index;
  for (index=1; index<=24; index++) {
    if (atree->itemOK(&scaler->crm[index]))
      if (index==19) {
	counter->crm[index] += (scaler->crm[index]-counter->FC_BIC_G_ped);
      } else if (index==20) {
	counter->crm[index] += (scaler->crm[index]-counter->FC_BIC_UG_ped);
      } else if (index==21) {
	counter->crm[index] += (scaler->crm[index]-counter->BPM_BIC_G_ped);
      } else {
	counter->crm[index] += scaler->crm[index];
      }
  }

  return 0;
}


// Latch word 1 before 1/25/98
//                    O           C       B       A
//           -  -  -  |  -  -  -  | - - - | - - - |
// Bit:      16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1  

// after  1/25/98
//                    O           B           C   A
//           -  -  -  |  -  -  -  | - - - - - | - |
// Bit:      16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1  

// after  4/29/00
//                    O     D     B           C   A
//           -  -  -  |  -  |  -  | - - - - - | - |
// Bit:      16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1  

// Second Latch word
//                                        S2  Co  S1
//           -  -  -  -  -  -  -  - - - - | - | - |
// Bit:      16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1  
//
// S1 : OOPS Single, S2 : OHIPS Single, Co : Coincidence

int
statPage::handle(void * /*data*/)
{
  // if (atree->itemOK(&oops1.old_scaler.ohips.sca[1]) && !...) event4++;
  if (atree->itemOK(&oops1.det.misc.helicity) ||
      atree->itemOK(&oops1.det.BeamTAGshort) ||
      atree->itemOK(&oops1.det.flag)) { event8++; return 0; }
  if (atree->itemOK(&oops1.beam.flag)) { event10++; return 0; }
  if (atree->itemOK(&oops1.target.bottom.cellTemp)) { event13++; return 0; }
  if (atree->itemOK(&oops1.clear.scaler)) { event3++; return 0; }

  if (atree->itemOK(&oops1.old_scaler.ohips.event5)) event5++;
  if (atree->itemOK(&oops1.scaler.event_6)) event6++;

  // Before Y2000 
  if (atree->itemOK(&oops1.det.latch1) &&
      atree->itemOK(&oops1.det.misc.latch2)) {
    if ((oops1.det.latch1 == 0x0001) && (oops1.det.misc.latch2 == 0x0001))
      OOPSAsingle++;
    if ((oops1.det.latch1 == 0x0100) && (oops1.det.misc.latch2 == 0x0001))
      OOPSBsingle++;
    if ((oops1.det.latch1 == 0x0004) && (oops1.det.misc.latch2 == 0x0001))
      OOPSCsingle++;
    if ((oops1.det.latch1 == 0x0400) && (oops1.det.misc.latch2 == 0x0001))
      OOPSDsingle++;
    if ((oops1.det.latch1 == 0x1000) && (oops1.det.misc.latch2 == 0x0010))
      OHIPSsingle++;
  }
  // After Y2000
  if (atree->itemOK(&oops1.det.latch1) && atree->itemOK(&oops1.det.latch2)) {
    if ((oops1.det.latch1 == 0x0001) && (oops1.det.latch2 == 0x0001))
      OOPSAsingle++;
    if ((oops1.det.latch1 == 0x0100) && (oops1.det.latch2 == 0x0001))
      OOPSBsingle++;
    if ((oops1.det.latch1 == 0x0004) && (oops1.det.latch2 == 0x0001))
      OOPSCsingle++;
    if ((oops1.det.latch1 == 0x0400) && (oops1.det.latch2 == 0x0001))
      OOPSDsingle++;
    if ((oops1.det.latch1 == 0x1000) && (oops1.det.latch2 == 0x0010))
      OHIPSsingle++;
  }

  handleOOPSscaler(&oops1.scaler.oopsA, &oopsAscaler);
  handleOOPSscaler(&oops1.scaler.oopsB, &oopsBscaler);
  handleOOPSscaler(&oops1.scaler.oopsC, &oopsCscaler);
  handleOOPSscaler(&oops1.scaler.oopsD, &oopsDscaler);

  handleHELIscaler(&oops1.old_scaler.heli_positive, &heli_positivescaler);
  handleHELIscaler(&oops1.old_scaler.heli_negative, &heli_negativescaler);

  handleOOPSLIVEscaler(&oops1.old_scaler.oops, &oopslivescaler);
  handleOOPSLIVEscaler(&oops1.scaler.oops, &oopslivescaler2000);

  handleOHIPSscaler(&oops1.old_scaler.ohips, &ohipsscaler);
  handleOHIPSscaler(&oops1.scaler.ohips, &ohipsscaler2000);

  handleEVE5scaler(&oops1.old_scaler, &eve5scaler);
  handleEVE6scaler(&oops1.scaler, &eve6scaler);

  return 0;
}

double
statPage::getIntegrated()
{
  return 0.0;
}

double
statPage::getRunTime()
{
  if (!startTime && atree && atree->getTimer())
    startTime = atree->getTimer()->getStarttime();

  // after 06:39 Jun 19, 2000
  if (!startTime || (startTime > 961411140)) return eve6scaler.sync/600.;
  // before May 2000
  if (startTime < 957153600) return 0.0001*eve5scaler.clock;
  // between May 2000 and 06:39 Jun 19, 2000
  return eve6scaler.event6*10.;
}

double
statPage::getDeadtime()
{
  return 0.0;
}

int
statPage::printOOPSscaler(std::ostream * out, const char * name,
			  struct oopsScalerInfo * counter)
{
  int i;
  for (i=0; i<3; i++) {
    (*out) << name << " oops sin" << i+1 << "_r                             : "
	   << std::setw(w) << counter->scint[i].right << std::endl;
    (*out) << name << " oops sin" << i+1 << "_l                             : "
	   << std::setw(w) << counter->scint[i].left << std::endl;
    (*out) << name << " oops sin" << i+1 << "_l+r                           : "
	   << std::setw(w) << counter->scint[i].left_AND_right << std::endl;
  }

  (*out) << name << " oops sin2_low_l                         : "
	 << std::setw(w) << counter->scint[1].low.left << std::endl;
  (*out) << name << " oops sin2_low_r                         : "
	 << std::setw(w) << counter->scint[1].low.right << std::endl;

  for (i=0; i<3; i++) {
    (*out) << name << " oops hdc" << i+1 << "x_top                          : "
	   << std::setw(w) << counter->hdc[i].x.top << std::endl;
    (*out) << name << " oops hdc" << i+1 << "x_bot                          : "
	   << std::setw(w) << counter->hdc[i].x.bottom << std::endl;
    (*out) << name << " oops hdc" << i+1 << "y_right                        : "
	   << std::setw(w) << counter->hdc[i].y.right << std::endl;
    (*out) << name << " oops hdc" << i+1 << "y_left                         : "
	   << std::setw(w) << counter->hdc[i].y.left << std::endl;
  }

  (*out) << name << " oops fired (and_123)     (S-46-48-50-52): "
	 << std::setw(w) << counter->and_123 << std::endl;
  (*out) << name << " oops fired (and_123 CB)  (S-58-60-62-64): "
	 << std::setw(w) << counter->and_123cb << std::endl;
  (*out) << name << " oops fired (and_12)      (S-47-49-51-53): "
	 << std::setw(w) << counter->and_12 << std::endl;
  (*out) << name << " oops fired (and_12 CB)   (S-59-61-63-65): "
	 << std::setw(w) << counter->and_12cb << std::endl;
  (*out) << name << " oops prescaled single    (S-70-71-72-73): "
	 << std::setw(w) << counter->prescaler << std::endl;
  (*out) << name << " oops prescaled single(CB)(S-76-77-78-79): "
	 << std::setw(w) << counter->prescalercb << std::endl;

  double and_123, prescaler;
  and_123     = counter->and_123;
  prescaler   = counter->prescaler;
  out->precision(2);
  out->setf(std::ios::fixed,std::ios::floatfield);
  if (prescaler != 0.) {
  (*out) << name << "*oops single prescale factor             : "
	 << std::setw(w) << and_123/prescaler << std::endl;
  }
  out->setf(std::ios::right,std::ios::floatfield);
  out->precision(w);

  // `ohips fired raw' is counted in the pit (S-103) and OHIPS (S-146)
  // here use only the ohips measurement
  // (*out) << name << " ohips fired raw                  (S-103): "
  //	 << counter->fired << std::endl;

  return 0;
}

int
statPage::printHELIscaler(std::ostream * out, const char * name,
			  struct heliScalerInfo * counter)
{
  (*out) << name << "H. oops A fired                 (S-156-167): "
	 << std::setw(w) << counter->oopsA_prescal   << std::endl;
  (*out) << name << "H. oops B fired                 (S-157-168): "
	 << std::setw(w) << counter->oopsB_prescal   << std::endl;
  (*out) << name << "H. oops C fired                 (S-158-169): "
	 << std::setw(w) << counter->oopsC_prescal   << std::endl;
  (*out) << name << "H. ohips fired                  (S-159-170): "
	 << std::setw(w) << counter->ohips_prescal   << std::endl;
  (*out) << name << "H. oops prescaled singles OR    (S-160-171): "
	 << std::setw(w) << counter->oops_prescal_OR << std::endl;
  (*out) << name << "H. ohips-oops coincidences      (S-161-172): "
	 << std::setw(w) << counter->coincidence     << std::endl;
  (*out) << name << "H. oops  live (sin. + coinc.)   (S-162-173): "
	 << std::setw(w) << counter->oops_live       << std::endl;
  (*out) << name << "H. ohips fired raw              (S-163-174): "
	 << std::setw(w) << counter->ohips_live      << std::endl;
  (*out) << name << "H. oops trigger                 (S-164-175): "
	 << std::setw(w) << counter->oops_trigger    << std::endl;
  (*out) << name << "H. ohips trigger                (S-165-176): "
	 << std::setw(w) << counter->ohips_trigger   << std::endl;
  (*out) << name << "H. master trigger (events 8)    (S-166-177): "
	 << std::setw(w) << counter->master_trigger  << std::endl;

  return 0;
}

int
statPage::printOOPSLIVEscaler(std::ostream * out, const char * name,
			      struct oopsliveScalerInfo * counter)
{
  (*out) << name << " oops prescaled singles OR        (S-014): "
         << std::setw(w) << counter->prescalOR   << std::endl;
  (*out) << name << " oops re-timing OR                (S-015): "
         << std::setw(w) << counter->retimingOR   << std::endl;
  (*out) << name << " oops live (sin. + coinc.)        (S-016): "
         << std::setw(w) << counter->live  << std::endl;
  (*out) << name << " oops trigger (sin.+coinc.+veto)  (S-033): "
         << std::setw(w) << counter->trigger   << std::endl;

  return 0;
}


int
statPage::printOOPSLIVEscaler(std::ostream * out, const char * name,
			      struct oopsliveScalerInfo2000 * counter)
{
  (*out) << name << " oops re-timing OR                (S-094): "
	 << std::setw(w) << counter->retimingOR   << std::endl;
  (*out) << name << " oops re-timing OR           (CB) (S-082): "
	 << std::setw(w) << counter->retimingORcb  << std::endl;
  (*out) << name << " oops single OR                   (S-095): "
	 << std::setw(w) << counter->singleOR  << std::endl;
  (*out) << name << " oops single OR              (CB) (S-083): "
	 << std::setw(w) << counter->singleORcb  << std::endl;
  (*out) << name << " oops live (sin. + coinc.)        (S-096): "
	 << std::setw(w) << counter->live  << std::endl;
  (*out) << name << " oops live (sin. + coinc.)   (CB) (S-084): "
	 << std::setw(w) << counter->livecb  << std::endl;
  (*out) << name << " oops trigger (sin.+coinc.)       (S-098): "
	 << std::setw(w) << counter->trigger   << std::endl;
  (*out) << name << " oops trigger (sin.+coinc.)  (CB) (S-086): "
	 << std::setw(w) << counter->triggercb   << std::endl;

  return 0;
}


int
statPage::printOHIPSscaler(std::ostream * out, const char * name,
			   struct ohipsScalerInfo * counter)
{
  (*out) << name << " ohips scint. 1 left              (S-118): "
         << std::setw(w) << counter->sca[1]   << std::endl;
  (*out) << name << " ohips scint. 1 right             (S-119): "
         << std::setw(w) << counter->sca[2]   << std::endl;
  (*out) << name << " ohips scint. 2 left              (S-120): "
         << std::setw(w) << counter->sca[3]   << std::endl;
  (*out) << name << " ohips scint. 2 right             (S-121): "
         << std::setw(w) << counter->sca[4]   << std::endl;
  (*out) << name << " ohips scint. 3 left              (S-122): "
         << std::setw(w) << counter->sca[5]   << std::endl;
  (*out) << name << " ohips scint. 3 right             (S-123): "
         << std::setw(w) << counter->sca[6]   << std::endl;
  (*out) << name << " ohips Cherenkov 1                (S-124): "
         << std::setw(w) << counter->sca[7]   << std::endl;
  (*out) << name << " ohips Cherenkov 2                (S-125): "
         << std::setw(w) << counter->sca[8]   << std::endl;
  (*out) << name << " ohips Cherenkov 3                (S-126): "
         << std::setw(w) << counter->sca[9]   << std::endl;
  (*out) << name << " ohips scint. 1 mean-time         (S-141): "
         << std::setw(w) << counter->sca[24]   << std::endl;
  (*out) << name << " ohips scint. 2 mean-time         (S-142): "
         << std::setw(w) << counter->sca[25]   << std::endl;
  (*out) << name << " ohips scint. 3 mean-time         (S-143): "
         << std::setw(w) << counter->sca[26]   << std::endl;
  (*out) << name << " ohips fired raw                  (S-146): "
         << std::setw(w) << counter->sca[29]   << std::endl;
  // ohips fired = pilot signal to pit (S-53=S-106=S-151)
  (*out) << name << " ohips fired                      (S-053): "
         << std::setw(w) << counter->scaler53   << std::endl;
  // (*out) << name << " ohips fired                      (S-106): "
  //        << counter->scaler106   << std::endl;
  (*out) << name << " ohips pilot                      (S-151): "
         << std::setw(w) << counter->sca[34]   << std::endl;
  (*out) << name << " ohips fast clear                 (S-144): "
         << std::setw(w) << counter->sca[27]   << std::endl;
  (*out) << name << " ohips para. trig.                (S-147): "
         << std::setw(w) << counter->sca[30]   << std::endl;
  (*out) << name << " ohips triggers lost in 2 us      (S-148): "
         << std::setw(w) << counter->sca[31]   << std::endl;
  (*out) << name << " ohips triggers lost in 1/b.b.    (S-149): "
         << std::setw(w) << counter->sca[32]   << std::endl;
  (*out) << name << " ohips trig lost in 2 us * 1/b.b. (S-150): "
         << std::setw(w) << counter->sca[33]   << std::endl;
  // ohips live = pit inhibit signal (S-109 = S-54 = S-145)
  (*out) << name << " ohips live (sin. + coin.)        (S-054): "
         << std::setw(w) << counter->live54   << std::endl;
  // (*out) << name << " ohips live (sin. + coin.)        (S-109): "
  //        << counter->live109   << std::endl;
  (*out) << name << " ohips live (sin. + coin.)        (S-145): "
         << std::setw(w) << counter->sca[28]   << std::endl;
  (*out) << name << " ohips trigger (sin.+coin.+veto)  (S-055): "
         << std::setw(w) << counter->trigger   << std::endl;
  (*out) << name << " ohips pre-scaled singles         (S-107): "
         << std::setw(w) << counter->prescal   << std::endl;
  (*out) << name << " ohips-oops coincidences          (S-108): "
         << std::setw(w) << counter->coincidence   << std::endl;
  (*out) << name << " ohips-oops-A raw coincidences    (S-110): "
         << std::setw(w) << counter->AND_oopsA   << std::endl;
  (*out) << name << " ohips-oops-B raw coincidences    (S-111): "
         << std::setw(w) << counter->AND_oopsB   << std::endl;
  (*out) << name << " ohips-oops-C raw coincidences    (S-112): "
         << std::setw(w) << counter->AND_oopsC   << std::endl;
  (*out) << name << " total events 5                   (S-153): "
         << std::setw(w) << counter->event5   << std::endl;
  (*out) << name << " total beam bursts                (S-154): "
         << std::setw(w) << counter->bb   << std::endl;
  (*out) << name << " total live beam bursts           (S-155): "
         << std::setw(w) << counter->bbcb   << std::endl;
  (*out) << name << " ohips triggers Hel. +            (S-152): "
         << std::setw(w) << counter->sca[35]   << std::endl;
  (*out) << name << " ohips triggers Hel. -            (S-178): "
         << std::setw(w) << counter->sca[36]   << std::endl;
  // scalers 127-140 (Pb glass) not included here
  // scalers 113-117 not used

  double bb, bbcb, scaler53, prescal, live54, coincidence, trigger;
  double AND_oopsA, AND_oopsB, AND_oopsC;

  bb          = counter->bb;
  bbcb        = counter->bbcb;
  scaler53    = counter->scaler53;
  prescal     = counter->prescal;
  live54      = counter->live54;
  trigger     = counter->trigger;
  AND_oopsA   = counter->AND_oopsA;
  AND_oopsB   = counter->AND_oopsB;
  AND_oopsC   = counter->AND_oopsC;
  coincidence = counter->coincidence;

  if (bbcb > 1) {
    out->precision(6);
    out->setf(std::ios::fixed,std::ios::floatfield);
    (*out) << "*** beam bursts          / live beam bursts : "
           << std::setw(w) << bb / bbcb << std::endl;
    (*out) << "*** ohips fired          / live beam bursts : "
           << std::setw(w) << scaler53 / bbcb << std::endl;
    (*out) << "*** ohips prescaled sin. / live beam bursts : "
           << std::setw(w) << prescal / bbcb << std::endl;
    (*out) << "*** ohips live           / live beam bursts : "
           << std::setw(w) << live54 / bbcb << std::endl;
    (*out) << "*** ohips triggers       / live beam bursts : "
           << std::setw(w) << trigger / bbcb << std::endl;
    (*out) << "*** ohips-oops coin.     / live beam bursts : "
           << std::setw(w) << coincidence / bbcb << std::endl;
    (*out) << "*** ohips-oopsA raw coin./ live beam bursts : "
           << std::setw(w) << AND_oopsA / bbcb << std::endl;
    (*out) << "*** ohips-oopsB raw coin./ live beam bursts : "
           << std::setw(w) << AND_oopsB / bbcb << std::endl;
    (*out) << "*** ohips-oopsC raw coin./ live beam bursts : "
           << std::setw(w) << AND_oopsC / bbcb << std::endl;
    out->setf(std::ios::right,std::ios::floatfield);
    out->precision(w);
  }

  return 0;
}


int
statPage::printOHIPSscaler(std::ostream * out, const char * name,
			   struct ohipsScalerInfo2000 * counter)
{
  (*out) << name << " ohips PIT                        (S-102): "
         << std::setw(w) << counter->pit102 << std::endl;
  (*out) << name << " ohips PIT (CB)                   (S-090): "
         << std::setw(w) << counter->pit90cb << std::endl;
  (*out) << name << " ohips Read                       (S-093): "
         << std::setw(w) << counter->spare2 << std::endl;
  (*out) << name << " ohips Read (CB)                  (S-092): "
         << std::setw(w) << counter->spare1 << std::endl;

  (*out) << name << " ohips pre-scaled singles         (S-103): "
         << std::setw(w) << counter->prescal   << std::endl;
  (*out) << name << " ohips pre-scaled singles    (CB) (S-091): "
         << std::setw(w) << counter->prescalcb   << std::endl;
  /* After Aug.20 ohips PIT after 8LM was used for OOPS OR after 8LM 
  (*out) << name << " ohips PIT after 8LM              (S-074): "
         << std::setw(w) << counter->pit74   << std::endl;
  (*out) << name << " ohips PIT after 8LM (CB)         (S-080): "  
         << std::setw(w) << counter->pit80cb   << std::endl;                 */

  (*out) << name << " oops OR after 8LM                (S-074): "
         << std::setw(w) << counter->pit74   << std::endl;
  (*out) << name << " oops OR after 8LM (CB)           (S-080): "
         << std::setw(w) << counter->pit80cb   << std::endl;

  (*out) << name << " ohips-oops coincidences          (S-075): "
         << std::setw(w) << counter->coincidence   << std::endl;
  (*out) << name << " ohips-oops coincidences     (CB) (S-081): "
         << std::setw(w) << counter->coincb   << std::endl;
  (*out) << name << " ohips live (single + coin.)      (S-097): "
         << std::setw(w) << counter->live   << std::endl;
  (*out) << name << " ohips live (single + coin.) (CB) (S-085): "
         << std::setw(w) << counter->livecb   << std::endl;
  (*out) << name << " ohips trigger (single+coin.)     (S-099): "
         << std::setw(w) << counter->trigger   << std::endl;
  (*out) << name << " ohips trigger (single+coin.)(CB) (S-087): "
         << std::setw(w) << counter->triggercb   << std::endl;

  (*out) << name << " ohips-oops-A raw coincidences    (S-054): "
         << std::setw(w) << counter->AND_oopsA   << std::endl;
  (*out) << name << " ohips-oops-A raw coincidences(CB)(S-066): "
         << std::setw(w) << counter->AND_oopsAcb   << std::endl;
  (*out) << name << " ohips-oops-B raw coincidences    (S-055): "
         << std::setw(w) << counter->AND_oopsB   << std::endl;
  (*out) << name << " ohips-oops-B raw coincidences(CB)(S-067): "
         << std::setw(w) << counter->AND_oopsBcb   << std::endl;
  (*out) << name << " ohips-oops-C raw coincidences    (S-056): "
         << std::setw(w) << counter->AND_oopsC   << std::endl;
  (*out) << name << " ohips-oops-C raw coincidences(CB)(S-068): "
         << std::setw(w) << counter->AND_oopsCcb   << std::endl;
  (*out) << name << " ohips-oops-D raw coincidences    (S-057): "
         << std::setw(w) << counter->AND_oopsD   << std::endl;
  (*out) << name << " ohips-oops-D raw coincidences(CB)(S-069): "
         << std::setw(w) << counter->AND_oopsDcb   << std::endl;

  (*out) << name << " OHIPS scint. 1A left                    : "    
         << std::setw(w) << counter->sca[1]   << std::endl;
  (*out) << name << " OHIPS scint. 1A right                   : "
         << std::setw(w) << counter->sca[2]   << std::endl;
  (*out) << name << " OHIPS scint. 1B left                    : "
         << std::setw(w) << counter->sca[3]   << std::endl;
  (*out) << name << " OHIPS scint. 1B right                   : "
         << std::setw(w) << counter->sca[4]   << std::endl;
  (*out) << name << " OHIPS scint. 1C left                    : "
         << std::setw(w) << counter->sca[5]   << std::endl;
  (*out) << name << " OHIPS scint. 1C right                   : "
         << std::setw(w) << counter->sca[6]   << std::endl;
  (*out) << name << " OHIPS scint. 1D left                    : "
         << std::setw(w) << counter->sca[7]   << std::endl;
  (*out) << name << " OHIPS scint. 1D right                   : "
         << std::setw(w) << counter->sca[8]   << std::endl;
  (*out) << name << " OHIPS scint. 2 left                     : "
         << std::setw(w) << counter->sca[9]   << std::endl;
  (*out) << name << " OHIPS scint. 2 right                    : "
         << std::setw(w) << counter->sca[10]   << std::endl;
  (*out) << name << " OHIPS scint. 3 left                     : "
         << std::setw(w) << counter->sca[11]   << std::endl;
  (*out) << name << " OHIPS scint. 3 right                    : "
         << std::setw(w) << counter->sca[12]   << std::endl;

  (*out) << name << " OHIPS LeadGlass 1                       : "
         << std::setw(w) << counter->sca[13] << std::endl;
  (*out) << name << " OHIPS LeadGlass 2                       : "
         << std::setw(w) << counter->sca[14] << std::endl;
  (*out) << name << " OHIPS LeadGlass 3                       : "
         << std::setw(w) << counter->sca[15] << std::endl;
  (*out) << name << " OHIPS LeadGlass 4                       : "
         << std::setw(w) << counter->sca[16] << std::endl;
  (*out) << name << " OHIPS LeadGlass 5                       : "
         << std::setw(w) << counter->sca[17] << std::endl;
  (*out) << name << " OHIPS LeadGlass 6                       : "
         << std::setw(w) << counter->sca[18] << std::endl;
  (*out) << name << " OHIPS LeadGlass 7                       : "
         << std::setw(w) << counter->sca[19] << std::endl;
  (*out) << name << " OHIPS LeadGlass 8                       : "
         << std::setw(w) << counter->sca[20] << std::endl;
  (*out) << name << " OHIPS LeadGlass 9                       : "
         << std::setw(w) << counter->sca[21] << std::endl;
  (*out) << name << " OHIPS LeadGlass 10                      : "
         << std::setw(w) << counter->sca[22] << std::endl;
  (*out) << name << " OHIPS LeadGlass 11                      : "
         << std::setw(w) << counter->sca[23] << std::endl;
  (*out) << name << " OHIPS LeadGlass 12                      : "
         << std::setw(w) << counter->sca[24] << std::endl;
  (*out) << name << " OHIPS LeadGlass 13                      : "
         << std::setw(w) << counter->sca[25] << std::endl;
  (*out) << name << " OHIPS LeadGlass 14                      : "
         << std::setw(w) << counter->sca[26] << std::endl;
  (*out) << name << " OHIPS LeadGlass 15                      : "
         << std::setw(w) << counter->sca[27] << std::endl;
  (*out) << name << " OHIPS LeadGlass 16                      : "
         << std::setw(w) << counter->sca[28] << std::endl;
  (*out) << name << " OHIPS LeadGlass 17                      : "
         << std::setw(w) << counter->sca[29] << std::endl;
  (*out) << name << " OHIPS LeadGlass 18                      : "
         << std::setw(w) << counter->sca[30] << std::endl;

  (*out) << name << " OHIPS Cherenkov 1                       : "
         << std::setw(w) << counter->sca[31]   << std::endl;
  (*out) << name << " OHIPS Cherenkov 2                       : "
         << std::setw(w) << counter->sca[32] << std::endl;
  (*out) << name << " OHIPS Cherenkov 3                       : "
         << std::setw(w) << counter->sca[33] << std::endl;

  (*out) << name << " OHIPS scint. 1A (L&R)                   : "
         << std::setw(w) << counter->sca[34] << std::endl;
  (*out) << name << " OHIPS scint. 1B (L&R)                   : "
         << std::setw(w) << counter->sca[35] << std::endl;
  (*out) << name << " OHIPS scint. 1C (L&R)                   : "
         << std::setw(w) << counter->sca[36] << std::endl;
  (*out) << name << " OHIPS scint. 1D (L&R)                   : "
         << std::setw(w) << counter->sca[37] << std::endl;
  (*out) << name << " OHIPS scint. 1 (1A.or.1B.or.1C.or.1D)   : "
         << std::setw(w) << counter->sca[38] << std::endl;
  (*out) << name << " OHIPS scint. 2  (L&R)                   : "
         << std::setw(w) << counter->sca[39] << std::endl;
  (*out) << name << " OHIPS scint. 3  (L&R)                   : "
         << std::setw(w) << counter->sca[40] << std::endl;

  (*out) << name << " OHIPS Scint-A Trigger                   : "
         << std::setw(w) << counter->sca[53] << std::endl;
  (*out) << name << " OHIPS Scint-B Trigger                   : "
         << std::setw(w) << counter->sca[54] << std::endl;
  (*out) << name << " OHIPS Scint-C Trigger                   : "
         << std::setw(w) << counter->sca[55] << std::endl;
  (*out) << name << " OHIPS Scint-D Trigger                   : "
         << std::setw(w) << counter->sca[56] << std::endl;

  (*out) << name << " OHIPS FEI                               : "
         << std::setw(w) << counter->sca[52] << std::endl;
  (*out) << name << " OHIPS CB                                : "
         << std::setw(w) << counter->sca[43] << std::endl;
  (*out) << name << " OHIPS 1.3 micro sec.                    : "
         << std::setw(w) << counter->sca[42] << std::endl;
  (*out) << name << " OHIPS CB or 1.3 micro sec.              : "
         << std::setw(w) << counter->sca[44] << std::endl;
  (*out) << name << " OHIPS trigger                           : "
         << std::setw(w) << counter->sca[41] << std::endl;
  (*out) << name << " OHIPS trigger lost in 1.3 micro sec.    : "
         << std::setw(w) << counter->sca[45] << std::endl;
  (*out) << name << " OHIPS Pilot w/o CB                      : "
         << std::setw(w) << counter->sca[58] << std::endl;
  (*out) << name << " OHIPS Pilot lost in CB                  : "
         << std::setw(w) << counter->sca[59] << std::endl;
  (*out) << name << " OHIPS Pilot CB                          : "
         << std::setw(w) << counter->sca[47] << std::endl;
  (*out) << name << " OHIPS ADC Gate                          : "
         << std::setw(w) << counter->sca[49] << std::endl;
  (*out) << name << " OHIPS TDC Start                         : "
         << std::setw(w) << counter->sca[50] << std::endl;
  (*out) << name << " OHIPS ExpCOM                            : "
         << std::setw(w) << counter->sca[51] << std::endl;
  (*out) << name << " OHIPS Read                              : "
         << std::setw(w) << counter->sca[48] << std::endl;
  (*out) << name << " OHIPS Fast Clear                        : "
         << std::setw(w) << counter->sca[57] << std::endl;
  (*out) << name << " OHIPS trigger lost in CB+1.3 micro sec.): "
         << std::setw(w) << counter->sca[46] << std::endl;

  double prescal, not_prescal, coincidence, trigger;
  double ohips_notCB1, ohips_CB1, ohips_notCB2, ohips_CB2;
  double AND_oopsA, AND_oopsB, AND_oopsC, AND_oopsD;

  prescal     = counter->prescal;
  not_prescal = counter->pit102;
  ohips_notCB1= counter->pit102;
  ohips_CB1   = counter->pit90cb;
  ohips_notCB2= counter->prescal;
  ohips_CB2   = counter->prescalcb;
  coincidence = counter->coincidence;
  trigger     = counter->trigger;
  AND_oopsA   = counter->AND_oopsA;
  AND_oopsB   = counter->AND_oopsB;
  AND_oopsC   = counter->AND_oopsC;
  AND_oopsD   = counter->AND_oopsD;

  out->precision(3);
  out->setf(std::ios::fixed,std::ios::floatfield);
  (*out) << "*** ohips prescale factor                   : "
	 << std::setw(w) << not_prescal/prescal << std::endl;
  (*out) << "*** ohips computer busy S-102/S-090         : "
	 << std::setw(w) << ohips_notCB1/ohips_CB1 << std::endl;
  (*out) << "*** ohips computer busy S-103/S-091         : "
	 << std::setw(w) << ohips_notCB2/ohips_CB2 << std::endl;

  (*out) << "*** ohips-oops coin.                        : "
	 << std::setw(w) << coincidence << std::endl;
  (*out) << "*** ohips triggers                          : "
	 << std::setw(w) << trigger << std::endl;
  (*out) << "*** ohips-oopsA raw coin.                   : "
	 << std::setw(w) << AND_oopsA << std::endl;
  (*out) << "*** ohips-oopsB raw coin.                   : "
	 << std::setw(w) << AND_oopsB << std::endl;
  (*out) << "*** ohips-oopsC raw coin.                   : "
	 << std::setw(w) << AND_oopsC << std::endl;
  (*out) << "*** ohips-oopsD raw coin.                   : "
	 << std::setw(w) << AND_oopsD << std::endl;
  out->setf(std::ios::right,std::ios::floatfield);
  out->precision(w);

  return 0;
}


int
statPage::printEVE5scaler(std::ostream * out, const char * name,
			  struct eve5ScalerInfo * counter)
{
  (*out) << name << " integrated charge                (S-001): "
         << std::setw(w) << counter->bt3bic   << std::endl;
  (*out) << name << " ohips-oops coincidences          (S-052): "
         << std::setw(w) << counter->coincidence   << std::endl;
  (*out) << name << " 10 kHz clock                     (S-094): "
         << std::setw(w) << counter->clock   << std::endl;
  (*out) << name << " oops pre-scaled sin. OR          (S-096): "
         << std::setw(w) << counter->sincb   << std::endl;
  (*out) << name << " oops re-timing OR                (S-097): "
         << std::setw(w) << counter->retcb   << std::endl;
  (*out) << name << " oops live (sin. + coinc.)        (S-098): "
         << std::setw(w) << counter->live   << std::endl;
  (*out) << name << " computer busy                    (S-099): "
         << std::setw(w) << counter->cb   << std::endl;
  (*out) << name << " master triggers (events 8)       (S-101): "
         << std::setw(w) << counter->trig  << std::endl;
  (*out) << name << " master triggers (events 8)       (S-056): "
         << std::setw(w) << counter->final_trigger   << std::endl;
  //  scalers 17, 30, 31, 32, 57 not used

  double bt3bic, clock, cb, coincidence, sincb, retcb, live, trig;

  bt3bic      = counter->bt3bic;
  clock       = counter->clock;
  cb          = counter->cb;
  coincidence = counter->coincidence;
  sincb       = counter->sincb;
  retcb       = counter->retcb;
  live        = counter->live;
  trig        = counter->trig;

  out->precision(6);
  out->setf(std::ios::fixed,std::ios::floatfield);
  (*out) << "*** integrated charge (mC)                  : "
         << std::setw(w) << bt3bic / 50000. << std::endl;
  if (clock > 1) {
    (*out) << "*** average current (uA)                    : "
           << std::setw(w) << bt3bic / clock / 0.005 << std::endl;
  }
  (*out) << "*** run time (min)                          : "
         << std::setw(w) << clock / 600000. << std::endl;
  if (cb > 1) {
    (*out) << "*** oops prescaled sin.OR/ live beam bursts : "
           << std::setw(w) << sincb / cb << std::endl;
    (*out) << "*** oops re-timing OR    / live beam bursts : "
           << std::setw(w) << retcb / cb << std::endl;
    (*out) << "*** oops live            / live beam bursts : "
           << std::setw(w) << live / cb << std::endl;
    (*out) << "*** ohips-oops coin.     / live beam bursts : "
           << std::setw(w) << coincidence / cb << std::endl;
    (*out) << "*** master triggers      / live beam bursts : "
           << std::setw(w) << trig / cb << std::endl;
  }
  out->setf(std::ios::right,std::ios::floatfield);
  out->precision(w);

 return 0;
}


int
statPage::printEVE6scaler(std::ostream * out, const char * name,
			  struct eve6ScalerInfo * counter)
{

  (*out) << name << " CRM ohips live    (single + coin.)      : "
	 << std::setw(w) << counter->crm[1]  << std::endl;
  (*out) << name << " CRM ohips trigger (single + coin.)      : "
	 << std::setw(w) << counter->crm[2]  << std::endl;
  (*out) << name << " CRM oops live     (single + coin.)      : "
	 << std::setw(w) << counter->crm[3]  << std::endl;
  (*out) << name << " CRM oops trigger  (single + coin.)      : "
	 << std::setw(w) << counter->crm[4]  << std::endl;
  (*out) << name << " CRM ohips-oops coincidence              : "
	 << std::setw(w) << counter->crm[5]  << std::endl;
  (*out) << name << " CRM ohips-oops coincidence         [INH]: "
	 << std::setw(w) << counter->crm[6]  << std::endl;

  (*out) << name << " CRM oops-A fired                        : "
	 << std::setw(w) << counter->crm[7]  << std::endl;
  (*out) << name << " CRM oops-B fired                        : "
	 << std::setw(w) << counter->crm[8]  << std::endl;
  (*out) << name << " CRM oops-C fired                        : "
	 << std::setw(w) << counter->crm[9]  << std::endl;
  (*out) << name << " CRM oops-D fired                        : "
	 << std::setw(w) << counter->crm[10]  << std::endl;
  //  (*out) << name << " CRM ohips trigger                       : "
  //	 << std::setw(w) << counter->crm[11]  << std::endl;
  (*out) << name << " CRM master triggers (event8)            : "
	 << std::setw(w) << counter->crm[12]  << std::endl;

  (*out) << name << " CRM oops-A fired                   [INH]: "
	 << std::setw(w) << counter->crm[13]  << std::endl;
  (*out) << name << " CRM oops-B fired                   [INH]: "
	 << std::setw(w) << counter->crm[14]  << std::endl;
  (*out) << name << " CRM oops-C fired                   [INH]: "
	 << std::setw(w) << counter->crm[15]  << std::endl;
  (*out) << name << " CRM oops-D fired                   [INH]: "
	 << std::setw(w) << counter->crm[16]  << std::endl;
  //  (*out) << name << " CRM ohips trigger                  [INH]: "
  //	 << std::setw(w) << counter->crm[17]  << std::endl;
  (*out) << name << " CRM master triggers (event8)       [INH]: "
	 << std::setw(w) << counter->crm[18]  << std::endl;

  (*out) << name << " master triggers (events 8)       (S-100): "
	 << std::setw(w) << counter->final_trigger  << std::endl;
  (*out) << name << " master triggers (events 8,CB)    (S-088): "
	 << std::setw(w) << counter->final_triggercb   << std::endl;
  (*out) << name << " ohips-oops coincidences          (S-101): "
	 << std::setw(w) << counter->coincidence   << std::endl;
  (*out) << name << " ohips-oops coincidences (CB)     (S-089): "
	 << std::setw(w) << counter->coincidencecb   << std::endl;

  (*out) << name << " mod sync                                : "
	 << std::setw(w) << counter->sync       << std::endl;
  (*out) << name << " event6 scaler                           : "
	 << std::setw(w) << counter->event6     << std::endl;
  (*out) << name << " computer busy                           : "
	 << std::setw(w) << counter->cbinh      << std::endl;
  (*out) << name << " gun pulse                               : "
	 << std::setw(w) << counter->gun        << std::endl;
  (*out) << name << " gun pulse (CB)                          : "
	 << std::setw(w) << counter->guncb      << std::endl;

  (*out) << name << " integrated charge ----------- <FC Gated>: "
         << std::setw(w) << counter->crm[19]  << std::endl;
  (*out) << name << " integrated charge --------- <FC UnGated>: "
         << std::setw(w) << counter->crm[20]  << std::endl;
  (*out) << name << " integrated charge --------- <BPM1 Gated>: "
         << std::setw(w) << counter->crm[21]  << std::endl;

  double FCbicG, FCbicG_scale, FCbicUNG, FCbicUNG_scale,
    BPMbicG, BPMbicG_scale, clock, gun;
  gun          = counter->gun;
  // clock is given by mod sync after 06:39 Jun 19, 2000 
  //  clock        = counter->event6;  
  clock          = counter->sync;
  FCbicG         = counter->crm[19];
  FCbicUNG       = counter->crm[20];
  BPMbicG        = counter->crm[21];
  FCbicG_scale   = counter->FC_BIC_G_scale;
  FCbicUNG_scale = counter->FC_BIC_UG_scale;
  BPMbicG_scale  = counter->BPM_BIC_G_scale;

  /* cerr << std::endl;
  cerr << "FC_BIC_UG_scale: " << FCbicUNG_scale << " | "
       << "FC_BIC_G_scale: " << FCbicG_scale << " | "
       << "BPM_BIC_G_scale: " << BPMbicG_scale << std::endl; */
  // cout << FCbicG << " " << FCbicUNG << " " << BPMbicG << std::endl;

  out->precision(2);
  out->setf(std::ios::fixed,std::ios::floatfield);
  (*out) << name << " BPM1 Gated / FC Gated                   : "
         << std::setw(w) << BPMbicG*BPMbicG_scale/FCbicG/FCbicG_scale  << std::endl;
  (*out) << "*** total run time (sec.)                   : "
         << std::setw(w) << clock / 600. << std::endl;

  out->precision(5);
  out->setf(std::ios::fixed,std::ios::floatfield);
  (*out) << "*** integrated charge (mC) ----- <BPM Gated>: "
         << std::setw(w) << BPMbicG * BPMbicG_scale << std::endl;
  (*out) << "*** integrated charge (mC) ------ <FC Gated>: "
         << std::setw(w) << FCbicG * FCbicG_scale << std::endl;
  (*out) << "*** integrated charge (mC) ---- <FC UnGated>: "
         << std::setw(w) << FCbicUNG * FCbicUNG_scale << std::endl;
  if ( FCbicUNG != 0 )
  (*out) << "*** FC Gated charge / Total [%]             : " << std::setw(w)
	 << FCbicG*FCbicG_scale /
           (FCbicG*FCbicG_scale+FCbicUNG*FCbicUNG_scale)*100. << std::endl;
  if ( clock != 0 ) {
  (*out) << "*** average current [microA] --- <BPM Gated>: "
         << std::setw(w) << BPMbicG * BPMbicG_scale * 1000 / clock * 600 << std::endl;
  (*out) << "*** average current [microA] ---- <FC Gated>: "
         << std::setw(w) << FCbicG * FCbicG_scale * 1000 / clock * 600 << std::endl;
  (*out) << "*** average current [microA] -- <FC UnGated>: "
         << std::setw(w) << FCbicUNG * FCbicUNG_scale * 1000 / clock * 600 << std::endl;
  (*out) << "*** average current [microA] ------ <FC sum>: "
	 << std::setw(w)
	 << (FCbicUNG*FCbicUNG_scale+FCbicG*FCbicG_scale)*1000/clock*600
	 << std::endl;
  }

  out->setf(std::ios::right,std::ios::floatfield);
  out->precision(w);

  /*
  (*out) << "*** oops prescaled sin.OR/ gun (cb)              : "
	 << std::setw(w) << prescalecb / guncb << std::endl;
  (*out) << "*** oops re-timing OR    / gun (cb)              : "
         << std::setw(w) << retcb / guncb << std::endl;
  (*out) << "*** oops live            / gun (cb)              : "
         << std::setw(w) << live / guncb << std::endl;
  (*out) << "*** ohips-oops coin.     / gun (cb)              : "
         << std::setw(w) << coincidence / guncb << std::endl;
  (*out) << "*** master triggers      / gun (cb)              : "
         << std::setw(w) << trig / guncb << std::endl;

  double bt3bic, clock, cb, coincidence, sincb, retcb, live, trig;

  bt3bic      = counter->bt3bic;
  clock       = counter->clock;
  cb          = counter->cb;
  coincidence = counter->coincidence;
  sincb       = counter->sincb;
  retcb       = counter->retcb;
  live        = counter->live;
  trig        = counter->trig; */

  /*
  out->precision(6);
  out->setf(std::ios::fixed,std::ios::floatfield);
  (*out) << "*** integrated charge (mC)                  : "
         << std::setw(w) << bt3bic / 50000. << std::endl;
  if (clock > 1) {
    (*out) << "*** average current (uA)                    : "
	   << std::setw(w) << bt3bic / clock / 0.005 << std::endl;
  }
  (*out) << "*** run time (min)                          : "
         << std::setw(w) << clock / 600000. << std::endl;
  if (cb > 1) {
    (*out) << "*** oops prescaled sin.OR/ live beam bursts : "
	   << std::setw(w) << sincb / cb << std::endl;
    (*out) << "*** oops re-timing OR    / live beam bursts : "
	   << std::setw(w) << retcb / cb << std::endl;
    (*out) << "*** oops live            / live beam bursts : "
	   << std::setw(w) << live / cb << std::endl;
    (*out) << "*** ohips-oops coin.     / live beam bursts : "
	   << std::setw(w) << coincidence / cb << std::endl;
    (*out) << "*** master triggers      / live beam bursts : "
	   << std::setw(w) << trig / cb << std::endl;
  }
  out->setf(0,std::ios::floatfield);
  out->precision(w);
  */

 return 0;
}


int
statPage::print(std::ostream *out, const char *str)
{
  (*out) << " Scaler listing for " << str << std::endl;
  (*out) << " ____________________________________"
	 << "_____________________" << std::endl << std::endl;
  return 0;
}


int
statPage::print(std::ostream *out)
{
  out->precision(w);

  printOOPSscaler(out, " A ", &oopsAscaler);
  printOOPSscaler(out, " B ", &oopsBscaler);
  printOOPSscaler(out, " C ", &oopsCscaler);
  printOOPSscaler(out, " D ", &oopsDscaler);
  
  if (atree->itemOK(&oops1.old_scaler.ohips.event5)) {
    printOOPSLIVEscaler(out, "   ", &oopslivescaler);
    printOHIPSscaler(out, "   ", &ohipsscaler);
    printEVE5scaler(out, "   ", &eve5scaler);
    printHELIscaler(out, "  +", &heli_positivescaler);
    printHELIscaler(out, "  -", &heli_negativescaler);
  }

  //if (atree->itemOK(&oops1.scaler.event6)) {
    printOOPSLIVEscaler(out, "   ", &oopslivescaler2000);
    printOHIPSscaler(out, "   ", &ohipsscaler2000);
    printEVE6scaler(out, "   ", &eve6scaler);
  //}

  (*out) << "    total events   3 (software)             : "
	 << std::setw(w) << event3  << std::endl;
  (*out) << "    total events   5 (software)             : "
	 << std::setw(w) << event5  << std::endl;
  (*out) << "    total events   6 (software)             : "
	 << std::setw(w) << event6  << std::endl;
  (*out) << "    total events   8 (software)             : "
	 << std::setw(w) << event8  << std::endl;
  (*out) << "    total events  10 (software)             : "
	 << std::setw(w) << event10 << std::endl;
  (*out) << "    total events  13 (software)             : "
	 << std::setw(w) << event13 << std::endl;
  (*out) << "    software ohips singles                  : "
	 << std::setw(w) << OHIPSsingle << std::endl;
  (*out) << "    software oops A singles                 : "
	 << std::setw(w) << OOPSAsingle << std::endl;
  (*out) << "    software oops B singles                 : "
	 << std::setw(w) << OOPSBsingle << std::endl;
  (*out) << "    software oops C singles                 : "
         << std::setw(w) << OOPSCsingle << std::endl;
  (*out) << "    software oops D singles                 : "
         << std::setw(w) << OOPSDsingle << std::endl;

  return 0;
}

void
statPage::BIC(double BPM_G_scale, double FC_G_scale, double FC_UG_scale,
	      double BPM_G_ped, double FC_G_ped, double FC_UG_ped)
{
  eve6scaler.BPM_BIC_G_scale = BPM_G_scale;
  eve6scaler.FC_BIC_G_scale = FC_G_scale;
  eve6scaler.FC_BIC_UG_scale = FC_UG_scale;
  eve6scaler.BPM_BIC_G_ped = BPM_G_ped;
  eve6scaler.FC_BIC_G_ped = FC_G_ped;
  eve6scaler.FC_BIC_UG_ped = FC_UG_ped;
}
