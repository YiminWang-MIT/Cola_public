//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-456      tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaMIT/HDCraw.h,v 2.1 2002-03-05 03:49:49 distler Exp $
//
// initialize VDCs and make raw histograms
//

#include "Aqua_online.h"
#include "Chamber/HDCoops.h"
#include "rundatabase.h"

void hdccoord(Aqua_online *out, struct FocalCoord *focal,
	      double x, double th, double y, double ph);
void hdcinit(HdcPacket& hdc, struct OOPS& rdb);
void hdcraw(Aqua_online *out, HdcPacket& hdc, struct SpectrometerOOPS *onl);
