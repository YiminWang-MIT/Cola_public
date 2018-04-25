#include <iostream>
#include <cstring>
#include "hmbook.h"
#include <stdlib.h>

#include "TH1.h"
#include "TH2.h"
#include "TFile.h"

int main (int argc, char *argv[])
{
  static float threshold = 0;
  char *input  = NULL;
  char *output = NULL;

  if (argc <= 3) {
      std::cout << 
	"\nUSAGE:   " << argv[0] <<" [options]\n\n"
	"options:\n" 
	"-h             this help.\n"
	"-f <file.his>  input file name.\n"
	"-o <file.root> output file name.\n"
	"-t <float>     threshold value for bins.\n";
      exit(0);
  }

  char opt;
  while ((opt = getopt(argc, argv, "o:f:t:h")) != EOF) {
    switch (opt) {
    case 'f': input  = optarg;             break;
    case 'o': output = optarg;             break;
    case 't': threshold = atof(optarg);    break;
    case 'h':             
      std::cout << 
	"\nUSAGE:   " << argv[0] <<" [options]\n\n"
	"options:\n" 
	"-h             this help.\n"
	"-f <file.his>  input file name.\n"
	"-o <file.root> output file name.\n"
	"-t <float>     threshold value for bins.\n";
      exit(0);
      break;
    }
  }

  HMReadAll(input);
  HIST *id = HMFirst;
  char *name;
  char forbidden[5]=".*/ ";

  TFile *rootfile = new TFile(output, "recreate");

  while(id) {
    name= strdup(id->title); 
    for (int i=0; i < strlen(name); i++) {
      if (strchr(forbidden,name[i])!=NULL) name[i]='_';
    }
    if (id->ny) {
      TH2D *hb = new TH2D(name, id->title, id->nx,id->minx, id->maxx,
				id->ny, id->miny, id->maxy);
      for (int x=0; x<id->nx; x++)
	for (int y=0; y<id->ny; y++) {
	  float fx=id->minx+(id->maxx-id->minx)/id->nx*(x+0.5);
	  float fy=id->miny+(id->maxy-id->miny)/id->ny*(y+0.5);
	  float val =  HMExtract2(id, fx, fy);
	  if (threshold && val > threshold || !threshold) 
	    hb->Fill(fx, fy, val); 
	  //	  else hb->Fill(fx, fy, 0);
	}
      rootfile->Write();
      delete hb;
    } else {
      TH1D *hb =new TH1D(name, id->title, id->nx, id->minx, id->maxx);
      double * error = new double[id->nx+1];
       for (int x=0; x<id->nx; x++) {
	float fx=id->minx+(id->maxx-id->minx)/id->nx*(x+0.5);
	//	float ey  = HMExtractError(id, fx);
	float ey  = 0;
	float val = HMExtract1(id, fx);

	// printf("bin: %3u X: %5.3f Y: %5.3f EY: %5.3f\n",x, fx, val, ey);
	
	if (threshold && val > threshold || !threshold) {
	  hb->Fill(fx, val); error[x+1] = ey;}
	//	else hb->Fill(fx, 0);
       }
       hb->SetError(error);
       rootfile->Write();
       delete hb;
    }
    std::cout << id->title << std::endl;

    free(name);
    if (id==HMLast) break;
    id = (HIST*) id->next;
  } 
  rootfile->Close();

  return 0;
}
