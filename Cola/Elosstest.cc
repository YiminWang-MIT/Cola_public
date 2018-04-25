#include <iostream>
#include "Material.h"


using namespace std;


void tabl(Material *mat)
{
  Particle pe=P_electron;
  Particle pp=P_proton;

  double p;
  for (p=0.2;p<0.801;p+=0.1) 
    cout<<setw(10) <<p*1000;
  cout<<endl;
    
  for (p=0.2;p<0.801;p+=0.1) {
    pe.setMomentum(p,0,0);
    cout <<setw(10)<< -mat->dEdx(pe,1)*1e6/mat->Density;
} 
  cout <<endl;
  for (p=0.2;p<0.801;p+=0.1) {
    pp.setMomentum(p,0,0);
    cout <<setw(10)<<- mat->dEdx(pp,1)*1e6/mat->Density;
}
  cout <<endl;
}


int main ()
{
  cout <<"LH2"<<endl;
  tabl(LH2);
  cout <<"C12 bulk"<<endl;
  tabl(C12);
  cout <<"C12 pure"<<endl;
  tabl(C12pure);
  cout <<"Fe"<<endl;
  tabl(Fe);

return 0;
}
