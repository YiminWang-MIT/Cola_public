/*                                                                      -*-c-*-
 * the guy to blame:
 *
 * Michael O. Distler                        mailto:distler@mit.edu
 * MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
 * Cambridge, MA 02139                       fax    (617) 258-5440
 *
 * $Header: /tmp/cvsroot/Cola/ColaJLab/rundatabase.h,v 1.1.1.1 1999-01-24 21:44:07 distler Exp $
 *
 * run database  structure
 */

#ifndef __RUNDATABASE_H__
#define __RUNDATABASE_H__

#include <time.h>

struct HDC {
  struct {
    char * initfile;
  } x[4], y[4];
};

struct TMA {
  char * initfile;
};

struct OOPS {
  struct HDC hdc;
  struct TMA tma;
  double flipped;
  double angle;
  double oop_angle;
  double momentum;
  double drift_distance;
  double FPparam[6];
  double scint_thr[3];
};

struct OHIPS {
  struct TMA tma;
  double angle;
  double momentum;
  double drift_distance;
  double FPparam[6];
  double VDCparam[4];
  double drift_velocity;
  double cerenkov_thr[3];
};

struct TOF {
  double min;
  double center;
  double max;
};

typedef struct RUN {
  char *setup;
  char *reaction;
  double Ebeam;
  double Target;
  double helicity_flipped;
  struct OOPS  oopsA, oopsB, oopsC, oopsD;
  struct OHIPS ohips;
  struct TOF tof;
} rundatabase;

void readRunDatabase(char *name, time_t time);
void readRunDatabase(char *name, char *setup);

#ifdef __RUNPARSER__

rundatabase rundb;

struct {
    int type; 
    char *Name; 
    void *addr;
} lname[] = {
    { -1, "Setup",              &rundb.setup                   },
    { -1, "Reaction",           &rundb.reaction                },
    {  1, "Ebeam",              &rundb.Ebeam                   },
    {  1, "Target",             &rundb.Target                  },
    {  1, "Helicity",           &rundb.helicity_flipped        },
    { -1, "OOPSA.hdc.1.x.ini",  &rundb.oopsA.hdc.x[1].initfile }, 
    { -1, "OOPSA.hdc.1.y.ini",  &rundb.oopsA.hdc.y[1].initfile }, 
    { -1, "OOPSA.hdc.2.x.ini",  &rundb.oopsA.hdc.x[2].initfile }, 
    { -1, "OOPSA.hdc.2.y.ini",  &rundb.oopsA.hdc.y[2].initfile }, 
    { -1, "OOPSA.hdc.3.x.ini",  &rundb.oopsA.hdc.x[3].initfile }, 
    { -1, "OOPSA.hdc.3.y.ini",  &rundb.oopsA.hdc.y[3].initfile }, 
    { -1, "OOPSA.tma.initfile", &rundb.oopsA.tma.initfile      }, 
    {  1, "OOPSA.angle",        &rundb.oopsA.angle             }, 
    {  1, "OOPSA.oop.angle",    &rundb.oopsA.oop_angle         }, 
    {  1, "OOPSA.momentum",     &rundb.oopsA.momentum          }, 
    {  1, "OOPSA.drift.dist",   &rundb.oopsA.drift_distance    }, 
    {  1, "OOPSA.drift.dist",   &rundb.oopsA.drift_distance    }, 
    {  1, "OOPSA.flipped",      &rundb.oopsA.flipped           }, 
    {  6, "OOPSA.fp.param",     &rundb.oopsA.FPparam           }, 
    {  3, "OOPSA.proton.thr",   &rundb.oopsA.scint_thr         }, 
    { -1, "OOPSB.hdc.1.x.ini",  &rundb.oopsB.hdc.x[1].initfile }, 
    { -1, "OOPSB.hdc.1.y.ini",  &rundb.oopsB.hdc.y[1].initfile }, 
    { -1, "OOPSB.hdc.2.x.ini",  &rundb.oopsB.hdc.x[2].initfile }, 
    { -1, "OOPSB.hdc.2.y.ini",  &rundb.oopsB.hdc.y[2].initfile }, 
    { -1, "OOPSB.hdc.3.x.ini",  &rundb.oopsB.hdc.x[3].initfile }, 
    { -1, "OOPSB.hdc.3.y.ini",  &rundb.oopsB.hdc.y[3].initfile }, 
    { -1, "OOPSB.tma.initfile", &rundb.oopsB.tma.initfile      }, 
    {  1, "OOPSB.angle",        &rundb.oopsB.angle             }, 
    {  1, "OOPSB.oop.angle",    &rundb.oopsB.oop_angle         }, 
    {  1, "OOPSB.momentum",     &rundb.oopsB.momentum          }, 
    {  1, "OOPSB.drift.dist",   &rundb.oopsB.drift_distance    }, 
    {  1, "OOPSB.flipped",      &rundb.oopsB.flipped           }, 
    {  6, "OOPSB.fp.param",     &rundb.oopsB.FPparam           }, 
    {  3, "OOPSB.proton.thr",   &rundb.oopsB.scint_thr         }, 
    { -1, "OOPSC.hdc.1.x.ini",  &rundb.oopsC.hdc.x[1].initfile }, 
    { -1, "OOPSC.hdc.1.y.ini",  &rundb.oopsC.hdc.y[1].initfile }, 
    { -1, "OOPSC.hdc.2.x.ini",  &rundb.oopsC.hdc.x[2].initfile }, 
    { -1, "OOPSC.hdc.2.y.ini",  &rundb.oopsC.hdc.y[2].initfile }, 
    { -1, "OOPSC.hdc.3.x.ini",  &rundb.oopsC.hdc.x[3].initfile }, 
    { -1, "OOPSC.hdc.3.y.ini",  &rundb.oopsC.hdc.y[3].initfile }, 
    { -1, "OOPSC.tma.initfile", &rundb.oopsC.tma.initfile      }, 
    {  1, "OOPSC.angle",        &rundb.oopsC.angle             }, 
    {  1, "OOPSC.oop.angle",    &rundb.oopsC.oop_angle         }, 
    {  1, "OOPSC.momentum",     &rundb.oopsC.momentum          }, 
    {  1, "OOPSC.drift.dist",   &rundb.oopsC.drift_distance    }, 
    {  1, "OOPSC.flipped",      &rundb.oopsC.flipped           }, 
    {  6, "OOPSC.fp.param",     &rundb.oopsC.FPparam           }, 
    {  3, "OOPSC.proton.thr",   &rundb.oopsC.scint_thr         }, 
    { -1, "OOPSD.hdc.1.x.ini",  &rundb.oopsD.hdc.x[1].initfile }, 
    { -1, "OOPSD.hdc.1.y.ini",  &rundb.oopsD.hdc.y[1].initfile }, 
    { -1, "OOPSD.hdc.2.x.ini",  &rundb.oopsD.hdc.x[2].initfile }, 
    { -1, "OOPSD.hdc.2.y.ini",  &rundb.oopsD.hdc.y[2].initfile }, 
    { -1, "OOPSD.hdc.3.x.ini",  &rundb.oopsD.hdc.x[3].initfile }, 
    { -1, "OOPSD.hdc.3.y.ini",  &rundb.oopsD.hdc.y[3].initfile }, 
    { -1, "OOPSD.tma.initfile", &rundb.oopsD.tma.initfile      }, 
    {  1, "OOPSD.angle",        &rundb.oopsD.angle             }, 
    {  1, "OOPSD.oop.angle",    &rundb.oopsD.oop_angle         }, 
    {  1, "OOPSD.momentum",     &rundb.oopsD.momentum          }, 
    {  1, "OOPSD.drift.dist",   &rundb.oopsD.drift_distance    }, 
    {  1, "OOPSD.flipped",      &rundb.oopsD.flipped           }, 
    {  6, "OOPSD.fp.param",     &rundb.oopsD.FPparam           }, 
    {  3, "OOPSD.proton.thr",   &rundb.oopsD.scint_thr         }, 
    { -1, "OHIPS.tma.initfile", &rundb.ohips.tma.initfile      }, 
    {  1, "OHIPS.angle",        &rundb.ohips.angle             }, 
    {  1, "OHIPS.momentum",     &rundb.ohips.momentum          }, 
    {  1, "OHIPS.drift.dist",   &rundb.ohips.drift_distance    }, 
    {  6, "OHIPS.fp.param",     &rundb.ohips.FPparam           }, 
    {  4, "OHIPS.vdc.param",    &rundb.ohips.VDCparam          }, 
    {  1, "OHIPS.vdc.velocity", &rundb.ohips.drift_velocity    }, 
    {  3, "OHIPS.electron.thr", &rundb.ohips.cerenkov_thr      }, 
    {  1, "TOF.cut.min",        &rundb.tof.min                 }, 
    {  1, "TOF.center",         &rundb.tof.center              }, 
    {  1, "TOF.cut.max",        &rundb.tof.max                 }, 
    {  0, NULL,                 0                              }
};

#else /* __RUNPARSER__ */

extern rundatabase rundb;

extern struct {
    int type; 
    char *Name; 
    void *addr;
} lname[];

#endif /* __RUNPARSER__ */

#endif /* __RUNDATABASE_H__ */
