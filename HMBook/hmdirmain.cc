#include "hmbook.h"
#include "hmdir.h"
#include <iostream>

int main(int argc, char *argv[])
{
  if (argc!=2) {
    std::cerr << "Usage: " << argv[0] << " Histogramfile" << std::endl;
    return -1;
  }
  HMReadAll(argv[1]);
  
  HistDirectory *root = new HistDirectory("/");
  root->CreateHistoTree();
  root->printtree();
  return 0;

}
