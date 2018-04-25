//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-456      tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaJLab/VDCraw.h,v 1.1.1.1 1999-01-24 21:44:07 distler Exp $
//
// initialize VDCs and make raw histograms
//

#include "HMBook/hmbook.h"
#include "Chamber/chlib++.h"

const int OU1SIZE = 256;
const int OV1SIZE = 256;
const int OU2SIZE = 256;
const int OV2SIZE = 256;

void vdccoord(struct FocalCoord *focal,
	      double x, double th, double y, double ph);
void fillplane(VdcPlane *plane, struct Vdc *onl,
	       HIST *drift, HIST *wire, HIST *tagged, HIST *eff);
void vdcOHIPSinit(VdcOHIPSpacket& vdc,
		  int u1size, int v1size, int u2size, int v2size);
void vdcOHIPSraw(VdcOHIPSpacket& vdc, struct SpectrometerOHIPS *onl);
void FPcoord(struct FocalCoord *chamber,
	     struct FocalCoord *focal, double *param);
