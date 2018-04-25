#define TDC_RES 0.050 // TDC Resolution in nsec/channel

double qsddaTimeShift(unsigned short tof_pattern, double y_tof);
double bclamTimeShift(unsigned short tof_pattern, double y_tof);
double qsddcTimeShift(unsigned short tof_pattern, double y_tof);
double qsddaTimeShift(unsigned short tof_pattern);
double bclamTimeShift(unsigned short tof_pattern);
double qsddcTimeShift(unsigned short tof_pattern);
int qsddaScintIndex(unsigned short pattern);
int bclamScintIndex(unsigned short pattern);
int qsddcScintIndex(unsigned short pattern);

