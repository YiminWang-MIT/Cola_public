#include "Material.h"
#include "Radiation.h"
#include "QuasiRandom.h"
#include <iostream>

using namespace std;
#define bins 10000

  double stat[2][bins];
  double range[2][2];

#define start -1
#define stop 5

double  avg=0;
int num=0;

inline void file(double w,int p)
{
  if (range[p][0]>w) range[p][0]=w;
  if (range[p][1]<w) range[p][1]=w;

  avg+=w;
  num++;
  int  pos=(int) ((w-start)/(stop-start)*bins);

  if (pos<0) return ;//pos=0;
  if (pos>=bins) return; //pos=bins-1;
  stat[p][pos]++;

}

int main()
{
  Particle Pe=P_electron;
  Particle Pp=P_proton;

 int i;
 range[0][0]=1e30; range[1][0]=1e30; range[0][1]=-1e30; range[1][1]=-1e30;
  for (i=0;i<bins;i++) {
    stat[0][i]=0;    stat[1][i]=0;
  }
    
#define rounds 10
  

  for (  i=0; i<1000000;i++)
    {
      //Pe.setMomentum(0.5,0,0);
      Pp.setMomentum(0.5,0,0);
      //      double Ee=Pe.energy();
      double Ep=Pp.energy();
      /*  for (int j=0;j<10;j++)  LandauLoss(Pe,C12, 0.01, halton[2*j], halton[2*j+1]);
            LandauLoss(Pe,Cu, 0.2, halton[2], halton[3]);
	      LandauLoss(Pe,Fe, 0.3, halton[8], halton[9]);
	      file(1000*(Ee-Pe.energy()),0);*/
      for (int j=0;j<rounds;j++)  { LandauLoss(Pp,C12, 0.001, halton[2*j], halton[2*j+1]);
            Pp+=0.0090757656970159063247738373547690571285784244537354/1000;
      }

      /*       LandauLoss(Pp,Cu, 0.2, halton[6], halton[7]);
	       LandauLoss(Pp,Fe, 0.3, halton[10], halton[11]);*/
      file(1000*(Ep-Pp.energy()),1);
    }

  	   cerr << range[0][0]<<"\t"<<range[0][1]<<"\n"<<range[1][0]<<"\t"<<range[1][1]<<"\n";
	   cerr.precision(50);
	   cerr <<setw(50)<<avg/num<<endl;
  for (i=0;i<bins;i++)
    cout <<i*1.0/bins*(stop-start)+start<<"\t"<<stat[0][i]<<"\t"<<stat[1][i]<<endl;
	     
  return 0;
}
