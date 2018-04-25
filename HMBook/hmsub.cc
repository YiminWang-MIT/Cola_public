#include <config.h>
#include <iostream>
#include "hmbook.h"

int main(int argc, char *argv[])
{ 
  if (argc<4) {
    std::cout << "  subtract histograms: "
	      << "file1.his - file2.his - file3.his ... outputfile.his"
	      << std::endl << "Usage:" << std::endl
	      << "  hmsub file1.his file2.his file3.his ... outputfile.his"
	      << std::endl;
    return -1;
  }
  HMReadAll(argv[1]);
  int file;
  for(file=2;file<argc-1;file++) {
    FILE *fp  = HMOpenHisFile(argv[2]);
    HIST *id1 = HMFirst, *id;
    do { 
      id = HMReadIsolated(fp);
      if (id->nx != id1->nx || id->ny != id1->ny) {
	std::cerr << "File " << argv[1] << " and " << argv[file]
		  << " contain different histograms." << std::endl;
	return -1;
      }
      
      HMFillError(id);
      HMFillError(id1);
      for(int i=0; i<id->nx*(id->ny ? id->ny : 1);i++) {
	id1->data[i] -= id->data[i];
	id1->entries -= id->entries;
      }
	id1 = id1->next;

      HMDelete(id);
    } while (id && id1);
    pclose(fp); 
  }
  HMWriteAll(argv[argc-1]);
  return 0;
}



