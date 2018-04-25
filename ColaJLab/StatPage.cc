//                                                                    -*-c++-*-
// authors:
//
// Michael O. Distler                        mailto:distler@mit.edu
// MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// Nikos Kaloskamis                          mailto:kaloskamis@bates.mit.edu
// MIT, Lab for Nuclear Science, 26-456      tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaJLab/StatPage.cc,v 1.1.1.1 1999-01-24 21:44:07 distler Exp $
//
// Classes to calculate and print a statistics page
//

#include "StatPage.h"
#include <iomanip.h>

const int w = 12;

// callback function increments event counter
int
eventCallback(unsigned short *, int, char *ptr)
{
  *((double *)ptr) += 1;
  return 0;
}


// callback function increments 16bit scaler counter
int
scaler16callback(unsigned short *data, int, char *ptr)
{
  *((double *)ptr) += *data;
  return 0;
}

#ifdef NEVER
void
XDRcpy(unsigned long *ul, unsigned short *s)
{
  char *L = (char *)ul;
  char *S = (char *)s;

  L[0] = S[2];
  L[1] = S[3];
  L[2] = S[0];
  L[3] = S[1];
}

// callback function increments 32bit scaler counter
int
scaler32callback(unsigned short *data, int, char *ptr)
{
  unsigned long ul;
  XDRcpy(&ul, data);
  *((double *)ptr) += ul;
  return 0;
}
#endif

statPage::statPage(CindyTree * ctree) : statPageBase(ctree)
{
  clear();
}

int
statPage::clear()
{
  return 0;
}

statPage::~statPage()
{
  ;
}

int
statPage::handle(void * /*data*/)
{
  return 0;
}

double
statPage::getIntegrated()
{
  return 0.0;
}

double
statPage::getRunTime()
{
  return 0.0;
}

double
statPage::getDeadtime()
{
  return 0.0;
}

int
statPage::print(ostream *out)
{
  out->precision(w);

  return 0;
}
