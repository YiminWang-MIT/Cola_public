//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-456      tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaJLab/StatPage.h,v 1.1.1.1 1999-01-24 21:44:07 distler Exp $
//
// Classes to calculate and print a statistics page
//

#ifndef __STATPAGE_H__
#define __STATPAGE_H__

#include "Cindy/Cindy++.h"
#include "Cola/StatBase.h"
#include "hrs.h"

///statistics page class for HallA
class statPage : public statPageBase {
public:
  /// constructor
  statPage(CindyTree * ctree);

  /// destructor
  virtual ~statPage();

  /// handle event data, calculate statistic page
  virtual int handle(void * data);

  /// Returns integrated luminosity for current experiment in particles/mubarn
  virtual double getIntegrated(void);

  /// Returns integrated run time for current experiment in seconds
  virtual double getRunTime(void);

  /// Returns Deadtime for current experiment in %
  virtual double getDeadtime(void);

  /// print statistic page to a stream
  virtual int print(ostream * out);

  /// clear statistic page
  virtual int clear(void);

protected:

};

#endif /* __STATPAGE_H__ */
