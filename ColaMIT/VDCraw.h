//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-456      tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaMIT/VDCraw.h,v 2.1 2002-03-05 03:49:49 distler Exp $
//
// initialize VDCs and make raw histograms
//

#include "Aqua_online.h"
#include "Chamber/chlib++.h"
#include "HMBook/hmbook.h"

const int OU1SIZE = 256;
const int OV1SIZE = 256;
const int OU2SIZE = 256;
const int OV2SIZE = 256;

void vdccoord(Aqua_online *out, FocalCoord *focal,
	      double x, double th, double y, double ph);
void vdcOHIPSinit(VdcOHIPSpacket& vdc,
		  int u1size, int v1size, int u2size, int v2size);
void vdcOHIPSraw(Aqua_online *out, VdcOHIPSpacket& vdc,
		 SpectrometerOHIPS *onl);
void FPcoord(Aqua_online *out, FocalCoord *chamber,
	     FocalCoord *focal, double *param);
