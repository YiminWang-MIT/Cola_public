/*
 * Created 1993 by	Klaus Werner Krygier, Alfred Richter
 *                      Institut fuer Kernphysik,
 *                      Johannes Gutenberg-Universitaet Mainz
 *
 * $Header: /tmp/cvsroot/Cola/Chamber/pi95/statistics.h,v 1.1 1997-07-09 20:35:40 merkel Exp $
 *
 * Teststrahlzeit: Raytrace Spektrometer B
 *
 */

#ifndef _STATISTICS_H_
#define _STATISTICS_H_

struct run_statistics {
  long	total;		/* total number of events */
  short	interupts;	/* prescaled number of interupts */
  short	dE_ToF_coinc;	/* prescaled number of scintillator coincidences */
  short	ps_foerster;	/* prescaled foerster detector */
  short ps_photo;       /* prescaled photo effect */
  short ps_faraday;     /* prescaled faraday cup */
  short	ps_runtime;	/* prescaled runtime  -> microbusy */
  short	ps_realtime;	/* prescaled realtime -> microbusy */
  short	ps_top;         /* prescaled top scintillator rates  */
  short	ps_cher;	/* prescaled cherenkov rates */
  short a_b_coinc;      /* prescaled ab coincidences */
};

#endif






