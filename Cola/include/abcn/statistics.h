#ifndef _STATISTICS_H_
#define _STATISTICS_H_

struct MpsRunStatistics {
  long	total;		/* total number of events */
  short	interrupts;	/* prescaled number of interrupts */
  short	dE_ToF_coinc;	/* prescaled number of scintillator coincidences */
  short	ps_foerster;	/* prescaled foerster detector */
  short ps_photo;       /* prescaled photo effect */
  short ps_faraday;     /* prescaled faraday cup */
  short	ps_runtime;	/* prescaled runtime  -> microbusy */
  short	ps_realtime;	/* prescaled realtime -> microbusy */
  short	ps_top;         /* prescaled top scintillator rates  */
  short	ps_cher;	/* prescaled cherenkov rates */
  short a_b_c_coinc;    /* prescaled abc coincidences */
  short a_b_coinc;      /* prescaled ab coincidences */
  short a_c_coinc;      /* prescaled ac coincidences */
  short b_c_coinc;      /* prescaled bc coincidences */
};

#endif /* _STATISTICS_H_ */
