#include <config.h>
#include <iostream>
#include "hmbook.h"

int main(int argc, char *argv[])
{ 
  if (argc<3) {
    std::cout << "  Differences between histograms. "
	      << std::endl << "Usage:" << std::endl
	      << "  hmsub file1.his file2.his"
	      << std::endl;
    return -1;
  }
  HMReadAll(argv[1]);
  FILE *fp  = HMOpenHisFile(argv[2]);
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
      if (id1->data[i] != id->data[i] ||
          id1->entries != id->entries)
	std::cerr << "Histogram \""<< id->title <<"\" differs."<<std::endl;
    }
    id1 = id1->next;
    
    HMDelete(id);
  } while (id && id1);
  fclose(fp); 
  
  return 0;
}



