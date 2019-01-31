// This program is a simple executable used to test the radiative generator

#include "radiative.h"
#include "radiative_helpers.hpp"
#include <iostream>

using namespace std;

int main(int argc, char ** argv)
{
  if (argc != 2)
    {
      cerr << "Error: wrong number of arguments supplied.\n";
      cerr << "Use 'radGenTest n' where n is the test routine to run.\n";
      return -1;
    }

  cout << "Beginning the test program...\n";

  int test = atoi(argv[1]);

  GeneratorRadiative generator(0,12341234);
  
  if (test==1)
    generator.test1();
  else if (test ==2)
    generator.test2();
  else if (test ==3)
    generator.test3();
  else if (test ==4)
    generator.test4();
  else if (test ==5)
    generator.test5();
  else
    {
      cerr << "Error: a valid test program was not supplied.\n";
      return -1;
    }

  cout << "Test program " << test << " has finished.\n";

  return 0;
}
