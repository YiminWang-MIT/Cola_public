//
// Created 1993 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Chamber/chtest2.cc,v 2.3 2002-11-08 20:27:26 distler Exp $
//
// Testprogramm fuer die Chamber++ Library
//

static char rcsid[] =
 "@(#)$Id: chtest2.cc 2560 2014-05-21 14:51:35Z distler $";

#include <iostream>
#include "chlib++.h"
#include "pi95/abc.h"

using namespace std;

// event 925

#define LEN_AX1_1 5
chRawData cht_ax1_1[LEN_AX1_1] = {
  { 64, 158 },
  { 65, 308 },
  { 66, 433 },
  { 67, 284 },
  { 68, 160 }
};

#define LEN_AS1_1 4
chRawData cht_as1_1[LEN_AS1_1] = {
  { 39, 229 },
  { 40, 403 },
  { 41, 253 },
  { 42, 129 }
};

#define LEN_AX2_1 3
chRawData cht_ax2_1[LEN_AX2_1] = {
  { 87, 224 },
  { 88, 379 },
  { 89, 370 }
};

#define LEN_AS2_1 0
chRawData *cht_as2_1 = 0;

// event 924

#define LEN_AX1_2 6
chRawData cht_ax1_2[LEN_AX1_2] = {
  { 18, 217 },
  { 19, 326 },
  { 20, 437 },
  { 21, 378 },
  { 22, 267 },
  { 23, 154 }
};

#define LEN_AS1_2 4
chRawData cht_as1_2[LEN_AS1_2] = {
  { 41, 200 },
  { 42, 353 },
  { 43, 416 },
  { 44, 273 }
};

#define LEN_AX2_2 6
chRawData cht_ax2_2[LEN_AX2_2] = {
  { 27, 191 },
  { 28, 303 },
  { 29, 415 },
  { 30, 368 },
  { 31, 266 },
  { 32, 153 }
};

#define LEN_AS2_2 4
chRawData cht_as2_2[LEN_AS2_2] = {
  { 50, 214 },
  { 51, 362 },
  { 52, 406 },
  { 53, 237 }
};

// original chtest

#ifndef NEVER
#define LEN_AX1_3 9
chRawData cht_ax1_3[LEN_AX1_3] = {
    { 157, 212 },
    { 158, 315 },
    { 159, 417 },
    { 160, 385 },
    { 161, 283 },
    { 162, 179 },
    { 168, 346 },
    { 169, 426 },
    { 170, 395 }
};
#else
#define LEN_AX1_3 6
chRawData cht_ax1_3[LEN_AX1_3] = {
    { 157, 212 },
    { 158, 315 },
    { 159, 417 },
    { 160, 385 },
    { 161, 283 },
    { 162, 179 }
};
#endif

#define LEN_AS1_3 5
chRawData cht_as1_3[LEN_AS1_3] = {
    { 138, 113 },
    { 139, 250 },
    { 140, 390 },
    { 141, 371 },
    { 142, 240 }
};

#define LEN_AX2_3 6
chRawData cht_ax2_3[LEN_AX2_3] = {
    { 160, 153 },
    { 161, 252 },
    { 162, 357 },
    { 163, 406 },
    { 164, 312 },
    { 165, 216 }
};

#define LEN_AS2_3 5
chRawData cht_as2_3[LEN_AS2_3] = {
    { 142, 126 },
    { 143, 261 },
    { 144, 406 },
    { 145, 416 },
    { 146, 211 }
};

VdcPlane ax1(".a.det.vdc.x1", AX1SIZE);
VdcPlane as1(".a.det.vdc.s1", AS1SIZE);
VdcPlane ax2(".a.det.vdc.x2", AX2SIZE);
VdcPlane as2(".a.det.vdc.s2", AS2SIZE);
VdcApacket vdcA(&ax1, &as1, &ax2, &as2);

int
main(int argc, char *argv[])
{
  char *progname = NULL;

  if (rcsid[0] != '@') return -1;
  if (argc) progname = argv[0];

  vdcA.InitTimeToLength(0.052 * 0.75);
  vdcA.InitOffset(446, 442, 438, 433);

  int seq[] = { 0, 1, 2 };
  if (argc==2) { seq[0] = 1; seq[1] = 0; }

  for (int i=0 ; i<3 ; i++) {
    vdcA.Reset();
    if (seq[i] == 0) {
      ax2.FillTime(cht_ax2_1, 2*LEN_AX2_1);
      as2.FillTime(cht_as2_1, 2*LEN_AS2_1);
      ax1.FillTime(cht_ax1_1, 2*LEN_AX1_1);
      as1.FillTime(cht_as1_1, 2*LEN_AS1_1);
    } else if (seq[i] == 1) {
      ax2.FillTime(cht_ax2_2, 2*LEN_AX2_2);
      as2.FillTime(cht_as2_2, 2*LEN_AS2_2);
      ax1.FillTime(cht_ax1_2, 2*LEN_AX1_2);
      as1.FillTime(cht_as1_2, 2*LEN_AS1_2);
    } else if (seq[i] == 2) {
      ax2.FillTime(cht_ax2_3, 2*LEN_AX2_3);
      as2.FillTime(cht_as2_3, 2*LEN_AS2_3);
      ax1.FillTime(cht_ax1_3, 2*LEN_AX1_3);
      as1.FillTime(cht_as1_3, 2*LEN_AS1_3);
    }

    //vdcA.x();
    //vdcA.y();
    //vdcA.theta();
    //vdcA.phi();

    //vdcA.dx();
    //vdcA.dy();
    //vdcA.dtheta();
    //vdcA.dphi();

    cout << progname << ": Ergebnisse der linearen Regression\n";

    cout << endl << "ok  : " << hex << vdcA.ok()  << dec;
    cout << endl << "ok1 : " << hex << vdcA.ok1() << dec;
    cout << endl << "ok3 : " << hex << vdcA.ok3() << dec << endl;
    cout << "x: "   << vdcA.x()
	 << " +/- " << vdcA.dx()     << " mm\n";
    cout << "y: "   << vdcA.y()
	 << " +/- " << vdcA.dy()     << " mm\n";
    cout << "t: "   << vdcA.theta()
	 << " +/- " << vdcA.dtheta() << " mrad\n";
    cout << "p: "   << vdcA.phi()
	 << " +/- " << vdcA.dphi()   << " mrad\n";
    
    cout << ax1.NameOfPlane() << ": "
	 << ax1.x()     << " +/- " << ax1.sigma_x()     << " mm\t: "
	 << ax1.slope() << " +/- " << ax1.sigma_slope() << endl;
    cout << as1.NameOfPlane() << ": "
	 << as1.x()     << " +/- " << as1.sigma_x()     << " mm\t: "
	 << as1.slope() << " +/- " << as1.sigma_slope() << endl;
    cout << ax2.NameOfPlane() << ": "
	 << ax2.x()     << " +/- " << ax2.sigma_x()     << " mm\t: "
	 << ax2.slope() << " +/- " << ax2.sigma_slope() << endl;
    cout << as2.NameOfPlane() << ": "
	 << as2.x()     << " +/- " << as2.sigma_x()     << " mm\t: "
	 << as2.slope() << " +/- " << as2.sigma_slope() << endl;
  }

  return 0;
}
