#include "HDCoops.h"
#include <iostream>

HdcPlane Ax1("oopsA.1.x");
HdcPlane Ax2("oopsA.2.x");
HdcPlane Ax3("oopsA.3.x");
HdcPlane Ay1("oopsA.1.y");
HdcPlane Ay2("oopsA.2.y");
HdcPlane Ay3("oopsA.3.y");

HdcChamber A1(&Ax1, &Ay1);
HdcChamber A2(&Ax2, &Ay2);
HdcChamber A3(&Ax3, &Ay3);

HdcPacket hdcA(&A1, &A2, &A3);

int
main(int, char**)
{
  Ax1.Init("/usr/local/oops/share/hdc/970201/OOPS_A_CH_1_X.ini");
  Ay1.Init("/usr/local/oops/share/hdc/970201/OOPS_A_CH_1_Y.ini");
  Ax2.Init("/usr/local/oops/share/hdc/970201/OOPS_A_CH_2_X.ini");
  Ay2.Init("/usr/local/oops/share/hdc/970201/OOPS_A_CH_2_Y.ini");
  Ax3.Init("/usr/local/oops/share/hdc/970201/OOPS_A_CH_3_X.ini");
  Ay3.Init("/usr/local/oops/share/hdc/970201/OOPS_A_CH_3_Y.ini");

  unsigned short ax1data[3] = { 1148,  361,  393 };
  unsigned short ay1data[3] = { 1203,  604,  540 };
  unsigned short ax2data[3] = {  603,  290,  302 };
  unsigned short ay2data[3] = { 1472,  470,  436 };
  unsigned short ax3data[3] = {  515,  573,  592 };
  unsigned short ay3data[3] = {  831,  505,  458 };

  Ax1.FillData(ax1data, 3);
  Ay1.FillData(ay1data, 3);
  Ax2.FillData(ax2data, 3);
  Ay2.FillData(ay2data, 3);
  Ax3.FillData(ax3data, 3);
  Ay3.FillData(ay3data, 3);

  std::cout << "Ax1 : " << Ax1.diff_ns() << '\t' << Ax1.dtime() << std::endl;
  std::cout << "Ay1 : " << Ay1.diff_ns() << '\t' << Ay1.dtime() << std::endl;
  std::cout << "Ax2 : " << Ax2.diff_ns() << '\t' << Ax2.dtime() << std::endl;
  std::cout << "Ay2 : " << Ay2.diff_ns() << '\t' << Ay2.dtime() << std::endl;
  std::cout << "Ax3 : " << Ax3.diff_ns() << '\t' << Ax3.dtime() << std::endl;
  std::cout << "Ay3 : " << Ay3.diff_ns() << '\t' << Ay3.dtime() << std::endl;

  std::cout << "ok  : " << hdcA.ok()     << std::endl;
  std::cout << "x   : " << hdcA.x()
	    << "\t+/- " << hdcA.dx()     << std::endl;
  std::cout << "th  : " << hdcA.theta()
	    << "\t+/- " << hdcA.dtheta() << std::endl;
  std::cout << "y   : " << hdcA.y()
	    << "\t+/- " << hdcA.dy()     << std::endl;
  std::cout << "ph  : " << hdcA.phi()
	    << "\t+/- " << hdcA.dphi()   << std::endl;

  return 0;
}
  
  
