#ifndef __TCCC_H__
#define __TCCC_H__

// Defaults if tccorr parameter files are not found: short OOPS, long OHIPS

const double OOPS_offset = 25.9539;     // short OOPS
const double OOPS_pathlen = 4191.81;    // short OOPS
const double OHIPS_offset = 60.1984;    // long OHIPS
const double OHIPS_pathlen = 9722.63;   // long OHIPS

//const double OOPS_offset = 32.7646;   // long OOPS
//const double OOPS_pathlen = 5291.81;  // long OOPS
//const double OHIPS_offset = 57.7217;  // short OHIPS
//const double OHIPS_pathlen = 9322.63; // short OHIPS

class TCCC {
private:
  int maxpow_ndisp, maxpow_disp;
  double central_ray, rzero;
  double *ndisp;
  double *disp;
public:
  TCCC(int spec, const char *fname);
  double central() { return central_ray; }
  double r00() { return rzero; }
  double getnd(int i) { return ndisp[i]; }
  double getd(int i) { return disp[i]; }
  double run(struct TargetCo *result);
  ~TCCC();
};

#endif /* __TCCC_H__ */
