/* Data Structure for gen beamtime 11/2000 */

struct RnpolPaddle {
    unsigned short   adc1;  
    unsigned short   adc2;   
    unsigned short   tdc1;  
    unsigned short   tdc2;  
    unsigned int     scaler;
};

struct RnpolFrame {
  struct RnpolPaddle  veto[5];
  struct RnpolPaddle  ndet[13];
};


struct Rnpol1stwall {
  struct RnpolPaddle  veto[16];
  struct RnpolPaddle  ndet[31];
};


struct Rnpol2ndwall {
  struct RnpolFrame   top;
  struct RnpolFrame   bottom;
};

struct RnpolUnused {
    unsigned short adc[33];
};


struct Rnpolraw {
  unsigned short sync_info;
  unsigned short interrupts;
  unsigned short runtime;
  unsigned short realtime;
  unsigned short pattern_unit;
  unsigned short trigger_tdc[9];
  unsigned short trigger_scaler[33];
  struct Rnpol1stwall    wall1;
  struct Rnpol2ndwall    wall2;
  struct RnpolPaddle    trigger[4];
  unsigned int clockticks;
  struct RnpolUnused unused;
  unsigned short adc[192];
};
