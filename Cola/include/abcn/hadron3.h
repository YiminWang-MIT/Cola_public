/*								        -*-c-*-
 * AQUA: data AcQUisition for A1 experiments
 *
 * Copyright (c) 1999-2001
 *
 * Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-5802
 * 55099 Mainz, Germany				fax  +49 6131 39-2964
 *
 * $Id: hadron3.h 2216 2008-06-13 21:13:47Z distler $
 */

#ifndef __HADRON3_H__
#define __HADRON3_H__

struct HDM_Module {
  unsigned short time;
  unsigned short energy;
  unsigned short flag;
};

struct hadron3 {
  unsigned short label;
  unsigned short pattern1;
  unsigned short pattern2;
  struct HDM_Module spBcoi;
  struct HDM_Module Diode;
  struct HDM_Module H1[26];
  struct HDM_Module H2[26];
  struct HDM_Module L1[27];
  struct HDM_Module L2[14];
  struct HDM_Module L3[14];
  struct HDM_Module L4[14];
  struct HDM_Module L5[14];
  struct HDM_Module L6[14];
  struct HDM_Module TP[5];
  unsigned int clockticks;
};

#endif /* __HADRON3_H__ */
