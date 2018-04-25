struct RunInfo {
  uword16 runtime;          /* general gate on (100 us)            */
  uword16 lifetime;         /* micro not busy (100 us)             */
};

struct BeamInfo {
  uword16 foerster;
  uword16 position;
};

#define CELLWIRES   8   /*   nr of wires per cell   */
#define XCELLS	    8   /* nr of cells in x-chamber */
#define YCELLS	    2   /* nr of cells in y-chamber */

struct DriftChamber {
  uword16 x[XCELLS*CELLWIRES];
  uword16 y[YCELLS*CELLWIRES];
  uword16 status;
};

/*   8x8 drift cells in x-layer,  2*8 drift cells in y-layer    */

#define PADDLES  5      /*   nr of szintillators   */

struct Pm {
  uword16 time;
  uword16 energy;
  uword32 hits;
};

struct Scintillator {
  struct Pm front, back;
};


struct TriggerD {
  uword32 count[16];
  uword16 info;
  uword16 pattern;
};

struct EventInfo {
  uword16 a_d_time;
  uword16 b_d_time;
  uword16 c_d_time;
};

struct DataD {
  uword16 sync_info;
  /** abcn.d.run	*/  struct RunInfo	run;
  /** abcn.d.beam	*/  struct BeamInfo	beam;           
  /** abcn.d.pdc	*/  struct DriftChamber   pdc;           
  /** abcn.d.scint	*/  struct Scintillator   scint[PADDLES]; 
  /** abcn.d.trigger	*/  struct TriggerD       trigger;        
  /** abcn.d.coinc	*/  struct EventInfo      coinc;         
  uword32 clockticks;
};
