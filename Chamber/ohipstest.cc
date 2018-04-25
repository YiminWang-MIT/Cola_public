//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-456      tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/Chamber/ohipstest.cc,v 2.8 2002-11-08 20:27:26 distler Exp $
//

#include "chlib++.h"
#include <time.h>
#include <iostream>
#include <iomanip>

#define CASE1

#ifdef CASE1
#define LEN_U1 5
chRawData cht_u1[LEN_U1] = {
    { 44, 154 },
    { 45, 273 },
    { 46, 394 },
    { 47, 335 },
    { 48, 216 }
};

#define LEN_V1 5
chRawData cht_v1[LEN_V1] = {
    { 52, 196 },
    { 53, 308 },
    { 54, 400 },
    { 55, 294 },
    { 56, 184 }
};

#define LEN_U2 4
chRawData cht_u2[LEN_U2] = {
    { 87, 255 },
    { 88, 368 },
    { 89, 386 },
    { 90, 283 }
};

#define LEN_V2 5
chRawData cht_v2[LEN_V2] = {
    { 91, 215 },
    { 92, 327 },
    { 93, 402 },
    { 94, 313 },
    { 95, 207 }
};
#endif 

#ifdef CASE2
#define LEN_U1 5
chRawData cht_u1[LEN_U1] = {
    { 113, 180 },
    { 114, 291 },
    { 115, 398 },
    { 116, 338 },
    { 117, 224 }
};

#define LEN_V1 5
chRawData cht_v1[LEN_V1] = {
    { 128, 212 },
    { 129, 312 },
    { 130, 403 },
    { 131, 318 },
    { 132, 197 }
};

#define LEN_U2 5
chRawData cht_u2[LEN_U2] = {
    { 158, 260 },
    { 159, 357 },
    { 160, 388 },
    { 161, 297 },
    { 162, 186 }
};

#define LEN_V2 6
chRawData cht_v2[LEN_V2] = {
    { 118, 503 },
    { 168, 205 },
    { 169, 303 },
    { 170, 395 },
    { 171, 349 },
    { 172, 242 }
};
#endif 

VdcPlane u1(".det.ohips.chamber.u1", 256);
VdcPlane v1(".det.ohips.chamber.v1", 256);
VdcPlane u2(".det.ohips.chamber.u2", 256);
VdcPlane v2(".det.ohips.chamber.v2", 256);
VdcOHIPSpacket vdc(&u1, &v1, &u2, &v2);

const int TIMES = 1000;

int
main(int argc, char *argv[])
{
  char *progname = NULL;
  time_t start, stop;
  int times = TIMES;

  if (argc) progname = argv[0];
  if (argc>1) sscanf(argv[1], "%d", &times);

  std::cout << "Jetzt geht's los\n";
  std::cout << times << "mal\n";

  time(&start);

  for (int i=0 ; i<times ; i++) {
    vdc.Reset();

    u1.FillTime(cht_u1, 2*LEN_U1);
    v1.FillTime(cht_v1, 2*LEN_V1);
    u2.FillTime(cht_u2, 2*LEN_U2);
    v2.FillTime(cht_v2, 2*LEN_V2);

    vdc.x();
    vdc.y();
    vdc.theta();
    vdc.phi();

    vdc.dx();
    vdc.dy();
    vdc.dtheta();
    vdc.dphi();
  }

  time(&stop);

  std::cout << "time for one cycle: " << 1E3 * (stop - start) / times
       << " msec\n\n";

  std::cout << progname << ": Ergebnisse der linearen Regression\n";

  std::cout << std::endl << "ok  : " << std::hex << vdc.ok()  << std::dec;
  std::cout << std::endl << "ok1 : " << std::hex << vdc.ok1() << std::dec;
  std::cout << std::endl << "ok3 : " << std::hex << vdc.ok3() << std::dec
	    << std::endl << std::endl;

  std::cout.setf(std::ios::fixed);
  std::cout << "x : "  << std::setw(8) << std::setprecision(3) << vdc.x()
	    << " +/- " << std::setw(6) << vdc.dx() << " mm" << std::endl;
  std::cout << "y : "  << std::setw(8) << std::setprecision(3) << vdc.y()
	    << " +/- " << std::setw(6) << vdc.dy() << " mm" << std::endl;
  std::cout << "t : "  << std::setw(8) << std::setprecision(3) << vdc.theta()
	    << " +/- " << std::setw(6) << vdc.dtheta() << " mrad" << std::endl;
  std::cout << "p : "  << std::setw(8) << std::setprecision(3) << vdc.phi()
	    << " +/- " << std::setw(6) << vdc.dphi() << " mrad"
	    << std::endl << std::endl;

  std::cout << u1.NameOfPlane() << " : "   << std::setprecision(3)  << std::setw(8)
	    << u1.x()           << " +/- " << u1.sigma_x()     << " mm : "
	    << u1.slope()       << " +/- " << u1.sigma_slope() << std::endl;
  std::cout << u2.NameOfPlane() << " : "   << std::setprecision(3)  << std::setw(8)
	    << u2.x()           << " +/- " << u2.sigma_x()     << " mm : "
	    << u2.slope()       << " +/- " << u2.sigma_slope() << std::endl;
  std::cout << v1.NameOfPlane() << " : "   << std::setprecision(3)  << std::setw(8)
	    << v1.x()           << " +/- " << v1.sigma_x()     << " mm : "
	    << v1.slope()       << " +/- " << v1.sigma_slope() << std::endl;
  std::cout << v2.NameOfPlane() << " : "   << std::setprecision(3)  << std::setw(8)
	    << v2.x()           << " +/- " << v2.sigma_x()     << " mm : "
	    << v2.slope()       << " +/- " << v2.sigma_slope() << std::endl;

  double vcorr = (v2.x()-v1.x())/150.0*36.52;
  std::cout << "x1 :\t" << (v1.x()-vcorr + u1.x())*M_SQRT1_2 << std::endl;
  std::cout << "y1 :\t" << (v1.x()-vcorr - u1.x())*M_SQRT1_2 << std::endl;
  vcorr = (v2.x()-v1.x())/150.0*23.61;
  std::cout << "x2 :\t" << (v2.x()-vcorr + u2.x())*M_SQRT1_2 << std::endl;
  std::cout << "y2 :\t" << (v2.x()-vcorr - u2.x())*M_SQRT1_2 << std::endl;

  return 0;
}
