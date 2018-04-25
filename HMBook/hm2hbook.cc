#include <iostream>
#include "HMBook/hmbook.h"
#include "HBook/HBook.h"

const long PAWCsize = 8*1024*1024;
struct paw { float h[PAWCsize]; } pawc_;
HBookBase histograms(PAWCsize);

int main (int argc, char *argv[])
{
  if (argc != 3) {
    std::cout << "Usage:\n     hm2hbook infile.his outfile.hbook" << std::endl;
    return 0;
  }  
  HMReadAll(argv[1]);
  HIST *id = HMFirst;
  while(id) {
    if (id->ny) {
      HBook2D *hb = new HBook2D(id->title, id->nx,id->minx, id->maxx,
				id->ny, id->miny, id->maxy);
      for (int x=0; x<id->nx; x++)
	for (int y=0; y<id->ny; y++) {
	  float fx=id->minx+(id->maxx-id->minx)/id->nx*(x+0.5);
	  float fy=id->miny+(id->maxy-id->miny)/id->ny*(y+0.5);
	  hb->inc(fx, fy, HMExtract2(id, fx, fy));
	}
    } else {
      HBook1D *hb =new HBook1D(id->title, id->nx, id->minx, id->maxx);
      for (int x=0; x<id->nx; x++) {
	float fx=id->minx+(id->maxx-id->minx)/id->nx*(x+0.5);
	hb->inc(fx, HMExtract1(id,fx));
      }
    }
    std::cout << id->title << std::endl;
    if (id==HMLast) break;
    id = (HIST*) id->next;
  } 
  histograms.write(argv[2]);
  return 0;
}
