//
// $Id: hmcatw.cc 2216 2008-06-13 21:13:47Z distler $
//

#include <config.h>
#include <iostream>
#include <math.h>
#include "hmbook.h"
extern HIST *HMFirst;
inline double sqr(double a) { return a*a; }

int main(int argc, char *argv[])
{
  if (argc<5) { 
    cout << "Usage:" << endl;
    cout << "  hmcat weight_file file1.his file2.his file3.his ... outputfile.his" << endl;
    cout << " weight_file contains the weights for the histograms in file1, file2,..." << endl;
    return -1;
  }

  FILE *wgt = fopen(argv[1], "r");
  int ii=0;
  double weight[1000];
  while (fscanf(wgt, "%lg", weight+ii) == 1) ii++;
  fclose(wgt);
  if (ii != argc-3) {
    cout << " weight_file contains "<<ii<<" numbers, but there are "<<argc-3
	 <<" histogram files." << endl;
    return -1;
  }
  
  HMReadAll(argv[2]);
  ii=1;
  for(int file=3; file<argc-1;file++) { 
    FILE *fp = HMOpenHisFile(argv[file]);
    HIST *id1 = HMFirst, *id;
    do { 
      id = HMReadIsolated(fp);
      if (id->nx != id1->nx || id->ny != id1->ny) { 
	cerr<< "File "<<argv[2]<< " and "<<argv[file]<< 
	  " contain different histograms."<<endl;
	return -1;
      }
      int bins = id->ny ? id->nx*id->ny : id->nx;
      if (ii==1)
	for(int i=0; i<bins; i++) id1->data[i] *= weight[0];
      for  (int i=0; i<bins; i++) id1->data[i] += id->data[i] * weight[ii];
      if (id->error && id1->error)
	for(int i=0;i<bins;i++) 
	  id1->error[i] = sqrt(sqr(id1->error[i]) + sqr(id->error[i]));
      if (id->nbin && id1->nbin)
	for(int i=0;i<bins;i++) id1->nbin[i]+= id->nbin[i];
	  
      id1->entries += id->entries;
      id1 = id1->next;
      HMDelete(id);
    } while (id && id1);
    pclose(fp); 
    ii++;
  }
  HMWriteAll(argv[argc-1]);
  return 0;
}



