//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-456      tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/Cola/StatBase.h,v 2.3 2002-12-12 23:51:49 distler Exp $
//
// Classes to calculate and print a statistics page
//

#ifndef __STATBASE_H__
#define __STATBASE_H__

#include "Aqua/AquaTree.h"
#include <iostream>
#ifdef HAVE_OSTREAM
#include <ostream>
#endif
#include <iosfwd>

/// statistics page base class
class statPageBase {
protected:
  /// reference to AquaTree object containing the raw data
  AquaTree * atree;
public:
  /// constructor
  statPageBase(AquaTree * aquatree) { atree = aquatree; }

  /// destructor
  virtual ~statPageBase() { ; }

  /// handle event data, calculate statistic page
  virtual int handle(void * data) = 0;

  /// Returns integrated luminosity for current experiment in particles/mubarn
  virtual double getIntegrated(void) = 0;

  /// Returns integrated run time for current experiment in seconds
  virtual double getRunTime(void) = 0;

  /// Returns Deadtime for current experiment in %
  virtual double getDeadtime(void) = 0;

  /// print statistic page to a stream
  virtual int print(std::ostream * out) = 0;

  /// clear statistic page
  virtual int clear(void) = 0;
};

#endif /* __STATBASE_H__ */




