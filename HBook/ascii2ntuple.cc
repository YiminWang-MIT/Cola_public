//
// @(#) ascii2ntuple 1.1 18-4-2001
// @(#) Pablo Barneo
// @(#) NIKHEF, Amsterdam
//
// $Header: /tmp/cvsroot/Cola/HBook/ascii2ntuple.cc,v 1.4 2006-12-07 14:06:45 merkel Exp $
//

#include <iostream>
#include <ctime>
#include "HBook.h"

const int PAWCsize = 10000000;
struct pawc {
  float h[PAWCsize];
} pawc_;
// don't change the name 'pawc'

const int CHTsize = 79;

// Use the same string length for all variables.
// Fill with spaces if necessary
char *cht[CHTsize] = {
  "adc4_q  ", "ct_qh3  ", "al1_01  ", "al1_02  ", "al1_03  ", "al1_04  ",
  "al1_05  ", "al1_06  ", "al1_07  ", "al1_08  ", "al1_09  ", "al1_10  ",
  "al1_11  ", "al1_12  ", "al1_13  ", "al1_14  ", "al1_15  ", "al1_16  ",
  "al1_17  ", "al1_18  ", "al1_19  ", "al1_20  ", "al1_21  ", "al1_22  ",
  "al1_23  ", "al1_24  ", "al1_25  ", "al1_26  ",
  "tl1_01  ", "tl1_02  ", "tl1_03  ", "tl1_04  ", "tl1_05  ", "tl1_06  ",
  "tl1_07  ", "tl1_08  ", "tl1_09  ", "tl1_10  ", "tl1_11  ", "tl1_12  ",
  "tl1_13  ", "tl1_14  ", "tl1_15  ", "tl1_16  ", "tl1_17  ", "tl1_18  ",
  "tl1_19  ", "tl1_20  ", "tl1_21  ", "tl1_22  ", "tl1_23  ", "tl1_24  ",
  "tl1_25  ", "tl1_26  ",
  "trfl_b3 ", "tp_b3   ", "tcor1_b3", "tcor2_b3", "tcor3_b3", "tcor4_b3",
  "tcor5_b3", "trl1_b3 ", "trh2_b3 ",
  "tofptt_B", "tofhit_B", "tofpdd_B", "dEptt_B ", "dEhit_B ", "dEpdd_B ",
  "x_B     ", "y_B     ", "x_scnt_B", "y_scnt_B", "t_spBcoi",
  "cermir_1", "cermir_2", "cermir_3", "cermir_4", "cermir_5"
};

int main(int argc, char *argv[])
{
  // keep compiler happy
  if (argc && argv);

  // Don't delete this line.
  // Ignore the compiler warning (in case you get it, I don't)
  NTuple pawc(PAWCsize, 10);

  NTuple nt((const char *)"NTuple 10",
	    (const char *)"ntuple.dat",
	    CHTsize,(const char **)cht);

  // float !! do not use double
  float fn[CHTsize];

  float number;
  int i=0, nevent=0;
  while (!std::cin.eof())
  {
    std::cin >> number;
    fn[i] = number;
    i++;
    if (i == CHTsize) {
    nt.book(fn); i=0;
    nevent++;
    if (!(nevent % 500))
      std::cerr << "\r Processing event "<< nevent << " \r" << std::flush;
    };
  };

  std::cerr << std::endl << "Processed events "<< nevent << std::endl;
  return 0;
}
