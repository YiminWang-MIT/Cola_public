#ifndef NCUBE_H
#define NCUBE_H

class NCubeDet 
{
  double calibrationx[258][512]; // Calibration of rising edge
  double calibrationy[258][512]; // Calibration of falling edge
  double calibrationoffset[12][512]; // Calibration of falling edge
  int    layer[256];             // Mapping of channel to bar
  char   side[256];              // Mapping of channel to bar
  int    bar[256];               // Mapping of channel to bar
  AquaNodeShort *xfine[258];    
  AquaNodeShort *xcoarse[258]; 
  AquaNodeShort *yfine[258];
  AquaNodeShort *ycoarse[258];
  AquaNodeShort *offset[12];
public:
  NCubeDet(AquaTree  *atree, struct NCUBE &rund);
  int handle(AquaTree  *out, struct NCube &onl);
};

#endif
