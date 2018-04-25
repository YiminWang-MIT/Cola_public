#include "HMBook/hmbook.h"
#include "HMBook/hmplot.h"
#include <fstream.h>
#include <getopt.h>
#include <stdlib.h>

int const MAXPEDESTAL = 1023;
int const CHANNELS    = 96;
int const BIN         =  8;

int main (int argc, char *argv[])
{

  char *filename = "Online.his";
  char *outfile  = "pedestals.dat";
  int  slot = 17;
  char opt;
  while ((opt = getopt(argc, argv, "i:o:s:h")) != EOF)  
    switch(opt) {
    case '?' :
    case ':' :
    case 'h' : cout << "Usage: "<<argv[0]
		    <<" -i histograms -o pedestals -s slot"<<endl;
               exit(0);
    case 'i' : filename = optarg;break;
    case 'o' : outfile = optarg;break;
    case 's' : slot = atoi(optarg);break;
    }

  HMReadAll(filename);

  ofstream out(outfile);
  out << "ADC-Slot: " << slot << endl;
  out << "Chan.  low  high" << endl;

  for (int i=0;i<CHANNELS;i++) {
    char histtitle[25];
    sprintf(histtitle, "Channel %02d",i);
    HIST *id = HMFindTitle(histtitle);
    if (!id)
      out.form("%3d %5d %5d\n", i, MAXPEDESTAL, MAXPEDESTAL );
    else {
      double maximum = 0;
      int maxbin = 0;
      for (int j=0; j<MAXPEDESTAL; j++) {
	double sum = 0;
	for(int k=0;k<BIN;k++) sum+=id->data[j+k];  
	if (sum>maximum) { 
	  maximum = sum;
	  maxbin=j; 
	}
      }
      out.form("%3d %5d %5d\n", i, maxbin+BIN/2, (maxbin+BIN/2)/6);
    }
  }
  out.close();
}
