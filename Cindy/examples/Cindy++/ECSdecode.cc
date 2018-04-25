//
// ECSdecode: Das Cindy++ ECSdecode Programm
//

#include <iostream>
#include <ostream>
#include <iosfwd>

#include <cstddef> 
#include <cstdlib>
#include <cstring>
#include <memory.h>

#include "ana++/Cindy++.h"

#include "ecsC/ipc/RecBuffer.h"
#include "ecsC/basicCput.h"
#include "ecsC/statusC.h"

CindyTree ct(0);

static char *progname = NULL;

std::ostream& operator<<(std::ostream &s,  aPlainStatus &data);

static void
cb(char *buf, int len, ostream &out)
{
  long alligned [32768];
  
  if (len < 32768) {
    bcopy (buf, &alligned, len);
    InRecBuffer a((char *) &alligned, len);
    long got;
    char * p;

    while ((p = a.next(&got))) {
      aPlainStatus * rp =  aBufToStatus (p, got);
      if (rp) {
	if (rp -> data) 
	  out << *rp << endl;
	else {
	  std::cerr << rp -> info -> submitter << ": no Data" << std::endl;
	  std::cerr << *rp << std::endl;
	}
	aPlainStatus_del(rp);
      }
    }
  }
}

int
main(int argc, char *argv[])
{
  progname = argv[0];

  char *outFile = ct.ecsGetOpt(argc,argv);
  char *ecsData;
  int   ecsDataLength;
  std::ostream &out = std::cout;

  if (outFile == NULL) exit(0);
  while (ecsData = ct.ecsNext(&ecsDataLength))
    cb ( ecsData, ecsDataLength, out);

  return 0;
}
