#include "spectrometer.h"
#include "statistics.h"

struct MpsNpm {
	short	time;
	short	energy;
	long	count;
};

struct MpsNE {
	struct MpsNpm	up;
	struct MpsNpm	down;
	long	count;
};

struct MpsNdE {
	struct MpsNpm	pm;
	long	count;
	long	count_hplus; 
	long	count_hminus; 
};

struct MpsNtrigger {
	short	info;
};

struct MpsDetN {
	struct MpsNE	E[30];
	struct MpsNdE	dE[9];
	struct MpsNdE	pinu;
	struct MpsNdE	pind;
	short	pattern[8];
	short	sync_info;
	struct MpsNtrigger trigger;
	short  hz50;
	long   hplus[16];
	long   hminus[16];
	short  laseru;
	short  laserd;
	unsigned short coinc_time;
	unsigned short pulser_time;
	unsigned int counter[16];
};

struct MpsDetN2 {
	struct MpsNE	E[20];
	struct MpsNdE	dE[9];
	struct MpsNdE	pinu;
	struct MpsNdE	pind;
	short	pattern[3];
	unsigned short coinc_time;
	unsigned short pulser_time;
	unsigned int counter[16];
};

struct MpsDataN {
	struct MpsRunInfo	run;
	struct MpsBeamInfo	beam;
	struct MpsDetN		det;
	struct MpsDetN2		det2;
	unsigned int clockticks;
};
