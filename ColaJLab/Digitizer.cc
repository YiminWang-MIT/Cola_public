//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaJLab/Digitizer.cc,v 1.1 1999-02-03 23:00:43 distler Exp $
//
// Classes to decode digitizer information
//

#include "Digitizer.h"
#include <iomanip.h>

DigitBase::DigitBase(unsigned int d, unsigned int c,
		     unsigned int o, unsigned int w) {
  model = 0; channels = 0; dataMask = d; wdcntMask = w;
  chanMask = c; for (chanShift=0; c && !(c&1); c/=2) chanShift++;
  optMask = o;  for ( optShift=0; o && !(o&1); o/=2)  optShift++;
}

int
DigitBase::print(unsigned int data) {
  cout << "\tslot " << setw(2) << (data >> 27)
       << "  chan " << setw(2) << ((data & chanMask) >> chanShift)
       << "  data 0x" << hex << (data & dataMask) << dec << endl;

  return 0;
}

int
DigitBase::handle(unsigned int) {
  return 0;
}
