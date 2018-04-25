#include <config.h>
#include <iostream>
#include <math.h>
#include "hmbook.h"

inline double sqr(double a){ return a*a; }

int main(int argc, char *argv[])
{
  if (argc != 4) {
    std::cout << "  divide histograms: file1.his / file2.his outputfile.his" 
	      << std::endl << "Usage:" << std::endl
	      << "  hmdiv file1.his file2.his outputfile.his" << std::endl;
    return -1;
  }

  HMReadAll(argv[1]);
  FILE *fp = HMOpenHisFile(argv[2]);
  if (!fp) return -1;
    
  HIST *id1 = HMFirst, *id;
  do {
    id = HMReadIsolated(fp);
      if (id->nx != id1->nx || id->ny != id1->ny) {
      std::cerr << "File " << argv[1] << " and " << argv[2]
		<< " contain different histograms." << std::endl;
      return -1;
    } 

    HMFillError(id);
    HMFillError(id1);

    for(int i=0; i<id->nx*(id->ny ? id->ny : 1);i++) {
      id1->error[i] = id->data[i]==0 ? 0 :
	sqrt(sqr(id1->error[i]/id->data[i]) +
	sqr(id->error[i]*id1->data[i]/sqr(id->data[i])));
      id1->data[i] = id->data[i] != 0 ? id1->data[i] / id->data[i] : 0 ;  
    }
    id1 = id1->next;
    HMDelete(id);
  } while (id && id1);
  pclose(fp); 
  HMWriteAll(argv[3]);
  return 0;
}



