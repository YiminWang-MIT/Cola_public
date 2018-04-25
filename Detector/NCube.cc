#include "Cola/rundatabase.h"
#include "Aqua/AquaTree.h"
#include "Cola/include/abcn/abcn.h" 
#include "Cola/include/online/online.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include "Detector/NCube.h"

using namespace std;

NCubeDet::NCubeDet(AquaTree  *atree, struct NCUBE &rund) {
  for (int i=0;i<258;i++)
    for (int j=0;j<512;j++)
      calibrationx[i][j] = calibrationy[i][j] =
	i<31 ? 0 : (j-31) * 5.0 / (500-31);
  for (int i=0;i<12;i++)
    for (int j=0;j<512;j++)
      calibrationoffset[i][j] = i<31 ? 0 : (j-31) * 5.0 / (500-31);
  {
    ifstream in(rund.calibration);
    if (in.good()) {
      while (!in.eof()) {
	char edge;
	int channel=0;
	in >> edge;
	if (edge == 'x') {
	  in >> channel;
	  for (int i=31;i<512;i++) in >> calibrationx[channel][i];
	} else if (edge == 'y') {
	  in >> channel;
	  for (int i=31;i<512;i++) in >> calibrationx[channel][i];
	} else if (edge =='o') {
	  in >> channel;
	  for (int i=31;i<512;i++) in >> calibrationoffset[channel][i];  
	} else
	  cerr<< "Undefined edge in calibration file"<<endl;
      }
      in.close();
      cout << "TRB Fine-Time Calibration file \"" 
	   << rund.calibration << "\" loaded...\n";
    }
  }

  ifstream in(rund.mapping);
  if (in.good()) {
    for(int i=0;i<256;i++) {
      if (in.eof()) {cerr<<"Invalid TRB mapping file!\n";break;}
      int channel;
      char c, line[1024];
      in>>c;
      if (c=='#') {
	in.getline(line,1024);
      } else {
	in.unget();     
	in >> channel;
	in >> layer[channel] >> side[channel] >> bar[channel];
      }
    }
    in.close();
  } else {
    // cerr << "WARNING: No mapping file for NCube, using default mapping" << endl;
    for(int i=0; i<256; i++) {
      layer[i] = i / 128;
      side[i] = (i & 1) ? 0 : 1;
      bar[i] = i % 128;
    }
  }
  
  AquaNode **sn;
#define AQUAFIND(n, a, b) atree->multifind(sn=(AquaNode **)a, 0, n - 1, b);
  
  AQUAFIND(258, xfine,    ".kaos.fibre.x.hit[%d].channel");
  AQUAFIND(258, xcoarse,  ".kaos.fibre.x.hit[%d].time");
  AQUAFIND(258, yfine,    ".kaos.fibre.y.hit[%d].channel");
  AQUAFIND(258, ycoarse,  ".kaos.fibre.y.hit[%d].time");
  AQUAFIND(12,  offset,   ".kaos.fibre.th.hit[%d].channel");
}

int NCubeDet::handle(AquaTree *out, struct NCube &onl)
{
  if (!xfine[0]->ok() || !xcoarse[0]->ok()) return -1;
  if (*xfine[0]>=511) return -1;

  if (xfine[257]->ok() && xcoarse[257]->ok()) {
    double coarsetime    = - *xcoarse[257] * 5.0;
    double finetime      = calibrationx[257][*xfine[257]];
    double referencefine = (*offset[0] - 31) * 5.0/(500-31);
    double combinedtime  = coarsetime - finetime + referencefine; 

    double coarsetime0    = - *xcoarse[0] * 5.0;
    double finetime0      = calibrationx[0][*xfine[0]];
    double referencefine0 = (*offset[0] - 31) * 5.0/(500-31);
    double combinedtime0  = coarsetime0 - finetime0 + referencefine0; 

    onl.fingerTime = combinedtime - combinedtime0;
    out->packEventData(&onl.fingerTime,1);
    if (yfine[257]->ok() && ycoarse[257]->ok()) {
      double coarsetimey    = - *ycoarse[257] * 5.0;
      double finetimey      = calibrationy[257][*yfine[257]];
      double combinedtimey  = coarsetimey - finetimey + referencefine; 
      onl.fingerWidth = combinedtimey - combinedtime;
      out->packEventData(&onl.fingerWidth,1);
    }
  }
  
  onl.multiplicity = 0;
  for (int i=1;i<=256;i++)
    if (xfine[i]->ok() &&xcoarse[i]->ok())
      if (*xfine[i]<512) {
	int oc = (i-1)/32+4;
	double coarsetime    = - *xcoarse[i] * 5.0;
	double finetime      = calibrationx[i][*xfine[i]];
	double referencefine = calibrationoffset[oc][*offset[oc]];
	double combinedtime  = coarsetime - finetime + referencefine; 
	
	double coarsetime0    = - *xcoarse[0] * 5.0;
	double finetime0      = calibrationx[0][*xfine[0]];
	double referencefine0 = (*offset[0] - 31) * 5.0/(500-31);
	double combinedtime0  = coarsetime0 - finetime0 + referencefine0; 

	double time = combinedtime - combinedtime0;
	
	if (side[i-1]=='L') {
	  onl.layer[layer[i-1]].bar[bar[i-1]].left.time = time;  
	  out->packEventData(&onl.layer[layer[i-1]].bar[bar[i-1]].left.time,1);
	} else {
	  onl.layer[layer[i-1]].bar[bar[i-1]].right.time = time;  
	  out->packEventData(&onl.layer[layer[i-1]].bar[bar[i-1]].right.time,1);
	}
	onl.hit[onl.multiplicity].bar = bar[i-1];
	onl.hit[onl.multiplicity].layer = layer[i-1];
	onl.hit[onl.multiplicity].side = ((side[i-1] == 'L') ? 0 : 1);
	onl.hit[onl.multiplicity].time = time;
	out->packEventData(&onl.hit[onl.multiplicity].bar,1);
	out->packEventData(&onl.hit[onl.multiplicity].layer,1);
	out->packEventData(&onl.hit[onl.multiplicity].side,1);
	out->packEventData(&onl.hit[onl.multiplicity].time,1);

	if (yfine[i]->ok() && ycoarse[i]->ok() &&
	    yfine[0]->ok() && ycoarse[0]->ok())
	  if (*yfine[i]<512 && *yfine[0]<512)
	    { int oc = (i-1)/32+4;
	      double coarsetimey    = - *ycoarse[i] * 5.0;
	      double finetimey      = calibrationy[i][*yfine[i]];
	      double referencefiney = calibrationoffset[oc][*offset[oc]];
	      double combinedtimey  = coarsetimey - finetimey + referencefiney; 
	      
	      double width = combinedtimey - combinedtime;
	      if (side[i-1]=='L') {
		onl.layer[layer[i-1]].bar[bar[i-1]].left.width = width;  
		out->packEventData(&onl.layer[layer[i-1]].bar[bar[i-1]].left.width,1);
	      } else {
		onl.layer[layer[i-1]].bar[bar[i-1]].right.width = width;  
		out->packEventData(&onl.layer[layer[i-1]].bar[bar[i-1]].right.width,1);
	      }
	      onl.hit[onl.multiplicity].width = width;
	      out->packEventData(&onl.hit[onl.multiplicity].width,1);
	    }
	onl.multiplicity++;
      }
  out->packEventData(&onl.multiplicity, 1);
  return 0;
}
