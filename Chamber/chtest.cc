//
// Created 1993 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Chamber/chtest.cc,v 2.14 2002-11-08 20:27:26 distler Exp $
//
// Testprogramm fuer die Chamber++ Library
//

static char rcsid[] =
 "@(#)$Id: chtest.cc 2640 2015-04-01 14:03:00Z aqua $";
#define _DEFAULT_SOURCE 1
#include <iostream>
#include <stdio.h>
#include <time.h>
#ifdef m68kph
#define time_t _G_time_t
#endif
#include "chlib++.h"
#include "pi95/abc.h"

#define LEN_AX1 6
chRawData cht_ax1[LEN_AX1] = {
    { 157, 212 },
    { 158, 315 },
    { 159, 417 },
    { 160, 385 },
    { 161, 283 },
    { 162, 179 }
//    { 168, 346 },
//    { 169, 426 },
//    { 170, 395 }
};

#define LEN_AS1 5
chRawData cht_as1[LEN_AS1] = {
    { 138, 113 },
    { 139, 250 },
    { 140, 390 },
    { 141, 371 },
    { 142, 240 }
};

#define LEN_AX2 6
chRawData cht_ax2[LEN_AX2] = {
    { 160, 153 },
    { 161, 252 },
    { 162, 357 },
    { 163, 406 },
    { 164, 312 },
    { 165, 216 }
};

#define LEN_AS2 5
chRawData cht_as2[LEN_AS2] = {
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


#define LEN_BX1 5
chRawData cht_bx1[LEN_BX1] = {
    { 143, 221 },
    { 144, 324 },
    { 145, 459 },
    { 146, 339 },
    { 147, 206 }
};

#define LEN_BS1 4
chRawData cht_bs1[LEN_BS1] = {
    { 152, 199 },
    { 153, 360 },
    { 154, 430 },
    { 155, 258 }
};

#define LEN_BX2 12
chRawData cht_bx2[LEN_BX2] = {
    { 121, 169 },
    { 122, 206 },
    { 123, 248 },
    { 124, 388 },
    { 125, 443 },
    { 126, 368 },
    { 127, 281 },
    { 149, 259 },
    { 150, 371 },
    { 151, 427 },
    { 152, 301 },
    { 153, 173 }
};

#define LEN_BS2 11
chRawData cht_bs2[LEN_BS2] = {
    { 140, 213 },
    { 141, 320 },
    { 142, 435 },
    { 143, 369 },
    { 144, 257 },
    { 145, 199 },
    { 146, 147 },
    { 161, 193 },
    { 162, 367 },
    { 163, 422 },
    { 164, 258 }
};

VdcPlane bx1(".b.det.vdc.x1", BX1SIZE);
VdcPlane bs1(".b.det.vdc.s1", BS1SIZE);
VdcPlane bx2(".b.det.vdc.x2", BX2SIZE);
VdcPlane bs2(".b.det.vdc.s2", BS2SIZE);
VdcBpacket vdcB(&bx1, &bs1, &bx2, &bs2);

const int TIMES = 1000;

int
main(int argc, char *argv[])
{
  char *progname = NULL;
  struct timespec start, stop;
  int times = TIMES;

  if (rcsid[0] != '@') return -1;
  if (argc) progname = argv[0];
  if (argc>1) sscanf(argv[1], "%d", &times);

  vdcA.InitTimeToLength(0.052 * 0.75);
  vdcA.InitOffset(446, 442, 438, 433);

  vdcB.InitTimeToLength(0.052 * 0.75);
  vdcB.InitOffset(462, 460, 458, 453);

  std::cout << "Jetzt geht's los\n";
  std::cout << times << "mal\n";

  clock_gettime(CLOCK_REALTIME, &start);

  for (int i=0 ; i<times ; i++) {
    vdcA.Reset();
    vdcB.Reset();

    ax2.FillTime(cht_ax2, 2*LEN_AX2);
    as2.FillTime(cht_as2, 2*LEN_AS2);
    ax1.FillTime(cht_ax1, 2*LEN_AX1);
    as1.FillTime(cht_as1, 2*LEN_AS1);

    bx2.FillTime(cht_bx2, 2*LEN_BX2);
    bs2.FillTime(cht_bs2, 2*LEN_BS2);
    bx1.FillTime(cht_bx1, 2*LEN_BX1);
    bs1.FillTime(cht_bs1, 2*LEN_BS1);

    vdcA.x();
    vdcA.y();
    vdcA.theta();
    vdcA.phi();

    vdcB.x();
    vdcB.y();
    vdcB.theta();
    vdcB.phi();

    vdcA.dx();
    vdcA.dy();
    vdcA.dtheta();
    vdcA.dphi();

    vdcB.dx();
    vdcB.dy();
    vdcB.dtheta();
    vdcB.dphi();
  }

  clock_gettime(CLOCK_REALTIME, &stop);

  std::cout << "time for one cycle: "
	    << (1E6 * (stop.tv_sec - start.tv_sec) +
		1E-3 * (stop.tv_nsec - start.tv_nsec)) / times
	    << " usec\n\n";
  std::cout << progname << ": Ergebnisse der linearen Regression\n";

  std::cout << std::endl << "ok  : " << std::hex << vdcA.ok()  << std::dec;
  std::cout << std::endl << "ok1 : " << std::hex << vdcA.ok1() << std::dec;
  std::cout << std::endl << "ok3 : "
	    << std::hex << vdcA.ok3() << std::dec << std::endl;
  std::cout << "x: "   << vdcA.x()
	    << " +/- " << vdcA.dx()     << " mm\n";
  std::cout << "y: "   << vdcA.y()
	    << " +/- " << vdcA.dy()     << " mm\n";
  std::cout << "t: "   << vdcA.theta()
	    << " +/- " << vdcA.dtheta() << " mrad\n";
  std::cout << "p: "   << vdcA.phi()
	    << " +/- " << vdcA.dphi()   << " mrad\n";

  std::cout << ax1.NameOfPlane() << ": "
	    << ax1.x()     << " +/- " << ax1.sigma_x()     << " mm\t: "
	    << ax1.slope() << " +/- " << ax1.sigma_slope() << std::endl;
  std::cout << as1.NameOfPlane() << ": "
	    << as1.x()     << " +/- " << as1.sigma_x()     << " mm\t: "
	    << as1.slope() << " +/- " << as1.sigma_slope() << std::endl;
  std::cout << ax2.NameOfPlane() << ": "
	    << ax2.x()     << " +/- " << ax2.sigma_x()     << " mm\t: "
	    << ax2.slope() << " +/- " << ax2.sigma_slope() << std::endl;
  std::cout << as2.NameOfPlane() << ": "
	    << as2.x()     << " +/- " << as2.sigma_x()     << " mm\t: "
	    << as2.slope() << " +/- " << as2.sigma_slope() << std::endl;

  std::cout << std::endl << "ok  : " << std::hex << vdcB.ok()  << std::dec;
  std::cout << std::endl << "ok1 : " << std::hex << vdcB.ok1() << std::dec;
  std::cout << std::endl << "ok3 : " << std::hex
	    << vdcB.ok3() << std::dec << std::endl;
  std::cout << "x: "   << vdcB.x()
	    << " +/- " << vdcB.dx()     << " mm\n";
  std::cout << "y: "   << vdcB.y()
	    << " +/- " << vdcB.dy()     << " mm\n";
  std::cout << "t: "   << vdcB.theta()
	    << " +/- " << vdcB.dtheta() << " mrad\n";
  std::cout << "p: "   << vdcB.phi()
	    << " +/- " << vdcB.dphi()   << " mrad\n";

  std::cout << bx1.NameOfPlane() << ": "
	    << bx1.x()     << " +/- " << bx1.sigma_x()     << " mm\t: "
	    << bx1.slope() << " +/- " << bx1.sigma_slope() << std::endl;
  std::cout << bs1.NameOfPlane() << ": "
	    << bs1.x()     << " +/- " << bs1.sigma_x()     << " mm\t: "
	    << bs1.slope() << " +/- " << bs1.sigma_slope() << std::endl;
  std::cout << bx2.NameOfPlane() << ": "
	    << bx2.x()     << " +/- " << bx2.sigma_x()     << " mm\t: "
	    << bx2.slope() << " +/- " << bx2.sigma_slope() << std::endl;
  std::cout << bs2.NameOfPlane() << ": "
	    << bs2.x()     << " +/- " << bs2.sigma_x()     << " mm\t: "
	    << bs2.slope() << " +/- " << bs2.sigma_slope() << std::endl;

  return 0;
}
