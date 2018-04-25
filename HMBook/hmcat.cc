//
// $Id: hmcat.cc 2244 2008-09-25 17:43:21Z patrick $
//

#include <config.h>
#include <iostream>
#include <unistd.h>
#if defined(__GNU_LIBRARY__)
#  include <getopt.h>
#endif
#include <cmath>
#include <cstring>
#include "hmbook.h"
extern HIST *HMFirst;
inline double sqr(double a) { return a*a; }

int main(int argc, char *argv[])
{
  char opt, *output = NULL;
  char verbose=0;
  char dir[255] = ".";
#if defined(__GNU_LIBRARY__)
  struct option lo[] = {
    {"output",    1, NULL,'o'},  {"help",      0, NULL,'h'},
    {"verbose",   0, NULL,'v'},  {"directory", 1, NULL,'C'},  
    {NULL, 0, NULL, 0}};
  while ((opt = getopt_long(argc, argv, "o:hvC:", lo, NULL))!=EOF) {
#else
  while ((opt = getopt(argc, argv, "o:hvC:")) != EOF) {
#endif
    switch (opt) {
    case 'C': strncpy(dir,optarg,254);  break;
    case 'o': output=optarg;       break;
    case 'v': verbose=1;           break;
    case '?': case ':': case 'h': default:
      std::cout << "Usage:" << std::endl << "  "<<argv[0]
		<<" file1.his file2.his file3.his ... [-o] outputfile.his\n\n" 
		<< "Options:\n"
		<<"-v      --verbose       print each added file to stderr.\n"
		<<"-C dir  --directory=dir change directory.\n"
		<<"-o file --output=file   output file.\n"
		<<"-h      --help          this help.\n";
      return opt=='h' ? 0 : 1;
    }
  }
  int last = output ? argc : argc-1;
  if (!output) output = argv[argc-1];
  if (optind>=last) { std::cerr << "Nothing to do." << std::endl; return 0;}

  char filename[1024];
  sprintf(filename,"%s/%s",dir,argv[optind]);
  HMReadAll(filename);
  if (verbose) std::cerr << output << " =  " << argv[optind] << std::endl;
  for(int file=optind+1; file<last;file++) { 
    sprintf(filename,"%s/%s",dir,argv[file]);
    FILE *fp = HMOpenHisFile(filename);
    HIST *id1 = HMFirst, *id=NULL;
    do { 
      id = HMReadIsolated(fp);
      if (!id) {std::cerr << "Can't read Histograms"  << std::endl;
	return -1;
      } 
      if (id->nx != id1->nx || id->ny != id1->ny) { 
	std::cerr << "File " << argv[1] << " and " << argv[file]
		  << " contain different histograms." << std::endl;
	return -1;
      }
      int bins = id->ny ? id->nx*id->ny : id->nx;
      for(int i=0; i<bins; i++) id1->data[i]+= id->data[i];
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
  if (verbose) std::cerr << output << " += " << argv[file] << std::endl;
  }
  HMWriteAll(output ? output : argv[argc-1]);
  return 0;
}
