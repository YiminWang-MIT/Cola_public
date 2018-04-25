//
// Created 1995 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Chamber/number.cc,v 2.10 2002-11-08 20:27:26 distler Exp $
//

static char rcsid[] =
 "@(#)$Id: number.cc 2216 2008-06-13 21:13:47Z distler $";

#include <iomanip>
#include <math.h>

const int NUMBER_X	= 448;
const int NUMBER_S	= 352;

const float X_OFFSET	= -802.60;
const float S_OFFSET	= -732.92;
const float S2_OFFSET	= -2.5;

const float DELTA_X	= 423.0;
const float DELTA_S	= DELTA_X * cos(39.83 * M_PI / 180.0);

const char  ch = 'c';
const char* CH = "CH_C_";

int
main()
{
  int i;

  if (*rcsid != '@') return(0);

  cout << "static short" << endl
       << "offset_" << ch << "x1[" << CH << "MAX_X1] = {" << endl;
  for (i=1; i<NUMBER_X; i++)
      cout << "  " << CH << "OFFSET_X1,\t\t\t/" << "/ wire " << i << endl;
  cout << "  " << CH << "OFFSET_X1\t\t\t/" << "/ wire " << i << endl
       << "};" << endl << endl;

  cout << "static short" << endl
       << "offset_" << ch << "s1[" << CH << "MAX_S1] = {" << endl;
  for (i=1; i<NUMBER_S; i++)
      cout << "  " << CH << "OFFSET_S1,\t\t\t/" << "/ wire " << i << endl;
  cout << "  " << CH << "OFFSET_S1\t\t\t/" << "/ wire " << i << endl
       << "};" << endl << endl;

  cout << "static short" << endl
       << "offset_" << ch << "x2[" << CH << "MAX_X2] = {" << endl;
  for (i=1; i<NUMBER_X; i++)
      cout << "  " << CH << "OFFSET_X2,\t\t\t/" << "/ wire " << i << endl;
  cout << "  " << CH << "OFFSET_X2\t\t\t/" << "/ wire " << i << endl
       << "};" << endl << endl;

  cout << "static short" << endl
       << "offset_" << ch << "s2[" << CH << "MAX_S2] = {" << endl;
  for (i=1; i<NUMBER_S; i++)
      cout << "  " << CH << "OFFSET_S2,\t\t\t/" << "/ wire " << i << endl;
  cout << "  " << CH << "OFFSET_S2\t\t\t/" << "/ wire " << i << endl
       << "};" << endl << endl;

  cout << "static float" << endl
       << "position_" << ch << "x1[" << CH << "MAX_X1] = {" << endl;
  for (i=1; i<NUMBER_X; i++)
      cout << setprecision(2) << setw(9)
	   << setiosflags(ios::fixed) << X_OFFSET+5.0*i
	   << ",\t\t/" << "/ x1 wire " << i << endl;
  cout << setprecision(2) << setw(9)
       << setiosflags(ios::fixed) << X_OFFSET+5.0*i
       << "\t\t/" << "/ x1 wire " << i << endl << "};" << endl << endl;

  cout << "static float" << endl
       << "position_" << ch << "s1[" << CH << "MAX_S1] = {" << endl;
  for (i=1; i<NUMBER_S; i++)
      cout << setprecision(2) << setw(9)
	   << setiosflags(ios::fixed) << S_OFFSET+5.0*i
	   << ",\t\t/" << "/ s1 wire " << i << endl;
  cout << setprecision(2) << setw(9)
       << setiosflags(ios::fixed) << S_OFFSET+5.0*i
       << "\t\t/" << "/ s1 wire " << i << endl << "};" << endl << endl;

  cout << "static float" << endl
       << "position_" << ch << "x2[" << CH << "MAX_X2] = {" << endl;
  for (i=1; i<NUMBER_X; i++)
      cout << setprecision(2) << setw(9)
	   << setiosflags(ios::fixed) << DELTA_X+X_OFFSET+5.0*i
	   << ",\t\t/" << "/ x2 wire " << i << endl;
  cout << setprecision(2) << setw(9)
       << setiosflags(ios::fixed) << DELTA_X+X_OFFSET+5.0*i
       << "\t\t/" << "/ x2 wire " << i << endl << "};" << endl << endl;

  cout << "static float" << endl
       << "position_" << ch << "s2[" << CH << "MAX_S2] = {" << endl;
  for (i=1; i<NUMBER_S; i++)
      cout << setprecision(2) << setw(9)
	   << setiosflags(ios::fixed) << DELTA_S+S_OFFSET+S2_OFFSET+5.0*i
	   << ",\t\t/" << "/ s2 wire " << i << endl;
  cout << setprecision(2) << setw(9)
       << setiosflags(ios::fixed) << DELTA_S+S_OFFSET+S2_OFFSET+5.0*i
       << "\t\t/" << "/ s2 wire " << i << endl << "};" << endl << endl;

  return(0);
}
