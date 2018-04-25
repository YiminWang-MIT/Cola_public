#include <stdio.h>
#include <math.h>
#include "hmbook.h"
#include "hmplot.h"

float examplefunction(float x)
{	return sin(x)/x;
}

int main(int argc, char *argv[])
{
  HIST *histogram;

  histogram = HMBook1("A Function","x","y","deg","arb",100, 0.0, 5.0);
  HMFillFunction1(histogram, examplefunction);
  HMPlotFile("output.ps");
  HMTitle("My First Example");
  HMFunction(histogram);
  HMClose();
  return 1;
}



