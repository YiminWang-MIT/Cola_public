//
// Created 1994-95 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Cindy/CindyCoinc.cc,v 4.13 2002-12-12 12:24:41 distler Exp $
//
// Main Part of Cindy analysis program
//

#define _POSIX_C_SOURCE 2
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "Cindy++.h"
#include "CindyCoinc.h"
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#ifdef HAVE_OSTREAM
#include <ostream>
#endif
#include <fstream>
#include <iosfwd>

const double runtimeAfactor	= 1E-4;
const double runtimeBfactor	= 1E-3;
const double deadtimeFactor	= 1E-5;
const double beamFactor		= 1E-3;

double runtimeA		= 0.0;
double realtimeA	= 0.0;
double runtimeB		= 0.0;
double realtimeB	= 0.0;

double deadtimeA	= 0.0;
double deadtimeB	= 0.0;
double deadtimeAB	= 0.0;

double beamFoerster	= 0.0;
double beamPhoto	= 0.0;
double beamFaraday	= 0.0;

short beamFoersterPS	= 0;
short beamPhotoPS	= 0;
short beamFaradayPS	= 0;

double foersterAmean	= 0.0;
double photoAmean	= 0.0;
double faradayAmean	= 0.0;

long dEtofA		= 0;
long dEtofAold		= 0;
long dEtofAoverflow	= 0;
long dEtofB		= 0;
long dEtofBold		= 0;
long dEtofBoverflow	= 0;
long abCoinc		= 0;

long singleA	= 0;
long singleB	= 0;
long eventAB	= 0;
long laserAB	= 0;
long total	= 0;

int debugflag	= 0;

void
coincHelp(char *progname)
{
  std::cerr << std::endl << progname << ':' << std::endl;
  std::cerr << '\t' << "cindy++ analysis program" << std::endl;
  std::cerr << " -d\t\t"      << "debug output." << std::endl;
  std::cerr << " -f <file>\t" << "input  file. default: STDIN" << std::endl;
  std::cerr << " -g\t\t"      << "input is gnutar file." << std::endl;
  std::cerr << " -z\t\t"      << "uncompress input." << std::endl << std::endl;
}

int
coincGetOpt(int argc, char *argv[], CindyTree& ct)
{
    char *input = "-";
    int   opt, flags = 0;

    while (EOF != (opt = getopt(argc, argv, "df:ghz?"))) {
	switch (opt) {
	case 'd': debugflag++; break;
	case 'f': input = optarg; break;
	case 'g': flags |= gnutarFlag; break;
	case 'z': flags |= zipFlag; break;
	default:  coincHelp(*argv); return -1;
	}
    }
    return ct.putOpt(flags,input,NULL);
}

int
main(int argc, char *argv[])
{
    CindyTree ct(0);
    if (coincGetOpt(argc,argv,ct)) return -1;
 
    std::ostream *cdbg = NULL;
    if (debugflag)  
      cdbg = &std::cerr;
    else
      cdbg = new std::ofstream("/dev/null");

    if (ct.nextArchiveUnpackAndCompile()) return -1;

    const struct increment *inc = getIncrement(ct.getTimeOfDataTaking());
    unsigned int flags = inc->flags;

    CindyItem coincAB	= ct.item(".a.coinc.a_b_coinc");
    CindyItem coincA	= ct.item(".a.det.trigger.dE_ToF_coinc");
    CindyItem coincB	= ct.item(".b.det.trigger.dE_ToF_coinc");

    CindyItem syncA	= ct.item(".a.det.sync_info");
    CindyItem syncB	= ct.item(".b.det.sync_info");

    CindyItem triggerA	= ct.item(".a.det.trigger.info");
    CindyItem triggerB	= ct.item(".b.det.trigger.info");

    CindyItem runA	= ct.item(".a.run.runtime");
    CindyItem realA	= ct.item(".a.run.realtime");
    CindyItem runAps	= ct.item(".a.run.ps_runtime");
    CindyItem realAps	= ct.item(".a.run.ps_realtime");

    CindyItem foersterA	  = ct.item(".a.beam.foerster");
    CindyItem foersterAps = ct.item(".a.beam.ps_foerster");
    CindyItem photoA	  = ct.item(".a.beam.photo");
    CindyItem photoAps	  = ct.item(".a.beam.ps_photo");
    CindyItem faradayA	  = ct.item(".a.beam.faraday");
    CindyItem faradayAps  = ct.item(".a.beam.ps_faraday");

    CindyItem runB	= ct.item(".b.run.runtime");
    CindyItem realB	= ct.item(".b.run.realtime");
    CindyItem runBps	= ct.item(".b.run.ps_runtime");
    CindyItem realBps	= ct.item(".b.run.ps_realtime");

    CindyItem deadA  = ct.item(".a.coinc.dead_a");
    CindyItem deadB  = ct.item(".a.coinc.dead_b");
    CindyItem deadAB = ct.item(".a.coinc.dead_ab");

    int AtriggerOK, AlaserOK;
    int BtriggerOK, BlaserOK;

    while (!ct.analyseNextEvent()) {

	AtriggerOK = AlaserOK = 0;
	if (syncA.ok()) {
	    AtriggerOK = (syncA() & 0x2000);
	    if (triggerA.ok()) AlaserOK = (triggerA() & 0x1);
	    if (runA.ok())
		runtimeA += inc->runtimeA + runtimeAfactor * (runA() & 0x3fff);
	    if (realA.ok()) realtimeA += runtimeAfactor * (realA() & 0x3fff);
	    if (foersterA.ok())
		beamFoerster += beamFactor * (foersterA() & 0x3fff);
	    if (photoA.ok()) beamPhoto += beamFactor * (photoA() & 0x3fff);
	    if (faradayA.ok())
		beamFaraday += beamFactor * (faradayA() & 0x3fff);
	}

	BtriggerOK = BlaserOK = 0;
	if (syncB.ok()) {
	    BtriggerOK = (syncB() & 0x4000);
	    if (triggerB.ok()) BlaserOK = (triggerB() & 0x1);
	    if (runtimeB == 0.0) runtimeB = runtimeA;
	    if (runB.ok())
		runtimeB += inc->runtimeB + runtimeBfactor * (runB() & 0x3fff);
	    if (realB.ok()) realtimeB += runtimeBfactor * (realB() & 0x3fff);
	}

	total++;
	if (AtriggerOK && !BtriggerOK) singleA++;
	if (BtriggerOK && !AtriggerOK) singleB++;
	if (AtriggerOK && BtriggerOK) {
	    if (!AlaserOK && !BlaserOK)
		{ eventAB++; }
	    else
		{ laserAB++; }
	}

	if (coincA.ok()) {
	    dEtofA = 0x3fff & coincA();
	    if (dEtofA < dEtofAold) dEtofAoverflow += 0x4000;
	    dEtofAold = dEtofA;
	}

	if (coincB.ok()) {
	    dEtofB = 0x3fff & coincB();
	    if (dEtofB < dEtofBold) dEtofBoverflow += 0x4000;
	    dEtofBold = dEtofB;
	}

	if (coincAB.ok()) abCoinc = 0x3fff & coincAB();

	static double deadAtmp = 0.0;
	static double deadBtmp = 0.0;

	if (deadA.ok() || deadB.ok() || (deadAB.ok())) {
	    *cdbg << "dead :\t";
	    if (deadA.ok()) {
		if (flags & INCFL1) {
		    deadtimeA = runtimeA - realtimeA;
		    flags &= ~INCFL1;
		}
		else deadtimeA += deadtimeFactor * deadA();
		*cdbg << deadA()  << "\t'"
		      << (runtimeA - realtimeA - deadAtmp) << "'\t";
		deadAtmp = runtimeA - realtimeA;
	    }
	    if (deadB.ok()) {
		if (flags & INCFL2) {
		    deadtimeB = runtimeB - realtimeB;
		    flags &= ~INCFL2;
		}
		else deadtimeB += deadtimeFactor * deadB();
		*cdbg << deadB()  << "\t'"
		      << (runtimeB - realtimeB - deadBtmp) << "'\t";
		deadBtmp = runtimeB - realtimeB;
	    }
	    if (deadAB.ok()) {
		if (flags & INCFL3) {
		    if (deadtimeAB == 0.0)
			deadtimeAB = deadtimeFactor * deadAB();
		    else {
			deadtimeAB = 2.0 * deadtimeFactor * deadAB();
			flags &= ~INCFL3;
		    }
		}
		else deadtimeAB += deadtimeFactor * deadAB();
		*cdbg << deadAB() << '\t';
	    }
	    *cdbg << std::endl;
	}

	if (runAps.ok() || realAps.ok()) {
	    *cdbg << "run A :\t";
	    if (runAps.ok())  *cdbg << (0x3fff & runAps())  << '\t';
	    if (realAps.ok()) *cdbg << (0x3fff & realAps()) << '\t';
	    *cdbg << std::endl;
	}

	if (runBps.ok() || realBps.ok()) {
	    *cdbg << "run B :\t";
	    if (runBps.ok())  *cdbg << (0x3fff & runBps())  << '\t';
	    if (realBps.ok()) *cdbg << (0x3fff & realBps()) << '\t';
	    *cdbg << std::endl;
	}

	if (foersterAps.ok() || photoAps.ok() || (faradayAps.ok())) {
	    *cdbg << "beam :\t";
	    if (foersterAps.ok()) {
		beamFoersterPS = (0x3fff & foersterAps());
		if (runAps.ok()) foersterAmean = (double)beamFoersterPS
				     / (double)(0x3fff & runAps()) * 5.0;
		*cdbg << beamFoersterPS << " '" << beamFoerster << "' ("
		      << (beamFoerster + inc->foersterA * foersterAmean
			  * (total - singleB)) << ")\t";
	    }
	    if (photoAps.ok()) {
		beamPhotoPS = (0x3fff & photoAps());
		if (runAps.ok()) photoAmean = (double)beamPhotoPS
				     / (double)(0x3fff & runAps()) * 5.0;
		*cdbg << beamPhotoPS << " '" << beamPhoto << "' ("
		      << (beamPhoto + inc->photoA * photoAmean
			  * (total - singleB)) << ")\t";
	    }
	    if (faradayAps.ok()) {
		beamFaradayPS = (0x3fff & faradayAps());
		if (runAps.ok()) faradayAmean = (double)beamFaradayPS
				     / (double)(0x3fff & runAps()) * 5.0;
		*cdbg << beamFaradayPS << " '" << beamFaraday << "' ("
		      << (beamFaraday + inc->faradayA * faradayAmean
			  * (total - singleB)) << ")\t";
	    }
	    *cdbg << std::endl;
	}
    }

    *cdbg << std::endl;
    std::cout << "dE/ToF A :\t" << (dEtofA + dEtofAoverflow) * 5
	      << " k" << std::endl;
    std::cout << "dE/ToF B :\t" << (dEtofB + dEtofBoverflow) * 5
	      << " k" << std::endl;
    std::cout << "coinc AB :\t" << abCoinc << " k" << std::endl << std::endl;

    std::cout << "realtime A :\t" << realtimeA << '\t'
	 << (runtimeA-realtimeA) << std::endl;
    std::cout << "runtime  A :\t" << runtimeA  << std::endl;
    std::cout << "realtime B :\t" << realtimeB << '\t'
	 << (runtimeB-realtimeB) << std::endl;
    std::cout << "runtime  B :\t" << runtimeB  << std::endl;

    std::cout << std::endl;
    std::cout << "deadtime A  :\t" << deadtimeA  << std::endl;
    std::cout << "deadtime B  :\t" << deadtimeB  << std::endl;
    std::cout << "deadtime AB :\t" << deadtimeAB << std::endl;

    std::cout << std::endl;
    std::cout << "foerster A :\t"
	      << ((beamFoerster + inc->foersterA * foersterAmean
		   * (total - singleB)) / runtimeA) << std::endl;
    std::cout << "photo    A :\t"
	      << ((beamPhoto + inc->photoA * photoAmean
		   * (total - singleB)) / runtimeA) << std::endl;
    std::cout << "faraday  A :\t"
	      << ((beamFaraday + inc->faradayA * faradayAmean
		   * (total - singleB)) / runtimeA) << std::endl;

    std::cout << std::endl;
    std::cout << "single A :\t" << singleA << std::endl;
    std::cout << "single B :\t" << singleB << std::endl;
    std::cout << "laser AB :\t" << laserAB << std::endl;
    std::cout << "event AB :\t" << eventAB << std::endl;
    std::cout << "total    :\t" << total   << std::endl;

    return 0;
}
