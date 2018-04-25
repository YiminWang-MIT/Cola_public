//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-456      tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaMIT/StatPage.h,v 2.3 2002-12-12 23:52:12 distler Exp $
//
// Classes to calculate and print a statistics page
//

#ifndef __STATPAGE_H__
#define __STATPAGE_H__

#include "Aqua_oops1.h"
#include "StatBase.h"

struct oopsScalerInfo {
  double fired;
  double sincb;
  double prescaler;
  double and_123;
  double and_12;
  struct {
    double right;
    double left;
    double left_AND_right;
    struct {
      double right;
      double left;
    } low;
  } scint[3];
  struct {
    struct {
      double top;
      double bottom;
    } x;
    struct {
      double right;
      double left;
    } y;
  } hdc[3];
  double prescalercb;
  double and_123cb;
  double and_12cb;
};

struct heliScalerInfo {
    double oopsA_prescal;
    double oopsB_prescal;
    double oopsC_prescal;
    double ohips_prescal;
    double oops_prescal_OR;
    double coincidence;
    double oops_live;
    double ohips_live;
    double oops_trigger;
    double ohips_trigger;
    double master_trigger;
};

struct oopsliveScalerInfo {
  double prescalOR;
  double retimingOR;
  double live;
  double trigger;
};

struct oopsliveScalerInfo2000 {
  double retimingOR;
  double live;
  double trigger;
  double singleOR;
  double retimingORcb;
  double singleORcb;
  double livecb;
  double triggercb;
};

struct ohipsScalerInfo {
  double scaler53;
  double live54;
  double trigger;
  double scaler106;
  double prescal;
  double coincidence;
  double live109;
  double AND_oopsA;
  double AND_oopsB;
  double AND_oopsC;
  double scaler113;
  double scaler114;
  double scaler115;
  double scaler116;
  double scaler117;
  double sca[56];
  double event5;
  double bb;
  double bbcb;   
};

struct ohipsScalerInfo2000 {
  double live;
  double trigger;
  double prescal;
  double coincidence;
  double livecb;
  double triggercb;
  double prescalcb;
  double coincb;
  double AND_oopsA;
  double AND_oopsB;
  double AND_oopsC;
  double AND_oopsD;
  double AND_oopsAcb;
  double AND_oopsBcb;
  double AND_oopsCcb;
  double AND_oopsDcb;
  double sca[65];
  double event6;
  double pit74;
  double pit80cb;
  double pit90cb;
  double spare1;
  double spare2;
  double pit102;
};

struct eve5ScalerInfo {
  double bt3bic;
  double computer_busy;
  double event5;
  double scaler31;
  double busclc;
  double coincidence;
  double final_trigger;
  double scaler57;
  double clock;
  double sincb;
  double retcb;
  double live;
  double cb;
  double trig;
};

struct eve6ScalerInfo {
  double final_trigger;
  double coincidence;
  double final_triggercb;
  double coincidencecb;
  double sync;
  double synccb;
  double cbinh;
  double gun;
  double guncb;
  double crm[25];
  double event6;
  double BPM_BIC_G_scale;
  double FC_BIC_G_scale;
  double FC_BIC_UG_scale;
  double BPM_BIC_G_ped;
  double FC_BIC_G_ped;
  double FC_BIC_UG_ped;
};

///statistics page class for OOPS

class statPage : public statPageBase {
protected:
  u_int32_t startTime;
public:
  /// constructor
  statPage(AquaTree *);

  /// destructor
  virtual ~statPage();

  /// handle event data, calculate statistic page
  virtual int handle(void * data);

  /// Returns integrated luminosity for current experiment in particles/mubarn
  virtual double getIntegrated(void);

  /// Returns integrated run time for current experiment in seconds
  virtual double getRunTime(void);

  /// Returns Deadtime for current experiment in %
  virtual double getDeadtime(void);

  /// print statistic page to a stream
  virtual int print(std::ostream * out, const char * str);
  virtual int print(std::ostream * out);

  /// clear statistic page
  virtual int clear(void);

  /// send over BIC scales and pedestals
  void BIC(double, double, double, double, double, double);

protected:

  // handle oopsX scaler
  int handleOOPSscaler(struct oops_scaler * scaler,
		       struct oopsScalerInfo * counter);
  int printOOPSscaler(std::ostream * out, const char * name,
		      struct oopsScalerInfo * counter);

  // handle helicity scaler
  int handleHELIscaler(struct helicity_scaler * scaler,
		       struct heliScalerInfo * counter);
  int printHELIscaler(std::ostream * out, const char * name,
		      struct heliScalerInfo * counter);

  // handle oops-live scaler
  int handleOOPSLIVEscaler(struct oopslive_scaler * scaler,
			   struct oopsliveScalerInfo * counter);
  int handleOOPSLIVEscaler(struct oopslive_scaler_2000 * scaler,
			   struct oopsliveScalerInfo2000 * counter);
  int printOOPSLIVEscaler(std::ostream * out, const char * name,
			  struct oopsliveScalerInfo * counter);
  int printOOPSLIVEscaler(std::ostream * out, const char * name,
			  struct oopsliveScalerInfo2000 * counter);

  // handle ohips scaler
  int handleOHIPSscaler(struct ohips_scaler * scaler,
			struct ohipsScalerInfo * counter);
  int handleOHIPSscaler(struct ohips_scaler_2000 * scaler,
			struct ohipsScalerInfo2000 * counter);
  int printOHIPSscaler(std::ostream * out, const char * name,
		       struct ohipsScalerInfo * counter);
  int printOHIPSscaler(std::ostream * out, const char * name,
		       struct ohipsScalerInfo2000 * counter);

  // handle event 5 scaler
  int handleEVE5scaler(struct event5 * scaler,
		       struct eve5ScalerInfo * counter);
  int printEVE5scaler(std::ostream * out, const char * name,
		      struct eve5ScalerInfo * counter);

  // handle event 6 scaler
  int handleEVE6scaler(struct event6 * scaler,
		       struct eve6ScalerInfo * counter);
  int printEVE6scaler(std::ostream * out, const char * name,
		      struct eve6ScalerInfo * counter);

  // event counter
  double event3, event5, event6, event8, event10, event13;

  // single events
  double OHIPSsingle,
    OOPSAsingle, OOPSBsingle, OOPSCsingle, OOPSDsingle;

  // OOPS-ABCD scintillator scalers
  double OOPSAsin12, OOPSAsin123, OOPSAsincb, OOPSAfired;
  double OOPSBsin12, OOPSBsin123, OOPSBsincb, OOPSBfired;
  double OOPSCsin12, OOPSCsin123, OOPSCsincb, OOPSCfired;
  double OOPSDsin12, OOPSDsin123, OOPSDsincb, OOPSDfired;

  // structure for oops scaler information
  struct oopsScalerInfo oopsAscaler;
  struct oopsScalerInfo oopsBscaler;
  struct oopsScalerInfo oopsCscaler;
  struct oopsScalerInfo oopsDscaler;

  // structure for helicity scaler information
  struct heliScalerInfo heli_positivescaler;
  struct heliScalerInfo heli_negativescaler;

  // structure for oops-live scaler information
  struct oopsliveScalerInfo oopslivescaler;
  struct oopsliveScalerInfo2000 oopslivescaler2000;

  // structure for ohips scaler information
  struct ohipsScalerInfo ohipsscaler;
  struct ohipsScalerInfo2000 ohipsscaler2000;

  // structure for event 5 scaler information
  struct eve5ScalerInfo eve5scaler;

  // structure for event 6 scaler information
  struct eve6ScalerInfo eve6scaler;

};

#endif /* __STATPAGE_H__ */
