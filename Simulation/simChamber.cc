#include "simChamber.h"

#include <fstream>

Chamber::Chamber(char * filename)
{
 
  //std::cout <<"Chamber filename is"<<filename<<std::endl;
  std::ifstream file(filename, std::ios::in |std::ios::binary);
  if (!file) 
    {
      nofile=true;
      if (filename!=NULL)
	std::cerr <<"chamber file not found: "<<filename<<std::endl;
      return;
    } 
  //  else if (filename!=NULL)
  //    std::cerr << "using chamber file " << filename << std::endl;
  file.read((char*) bins,4*sizeof(int));
  file.read((char*)start,4*sizeof(double));
  file.read((char*)increment,4*sizeof(double));

  int numberofvalues=2*bins[0]*bins[1]*bins[2]*bins[3];
  values=new double[numberofvalues];
  file.read((char*)values,numberofvalues*sizeof(double));
  nofile=false;
  file.close();
  bininc[3]=2;
  bininc[2]=bininc[3]*bins[3];
  bininc[1]=bininc[2]*bins[2];
  bininc[0]=bininc[1]*bins[1];

}


bool Chamber::ok(double angle, double dp,double z, double th, double ph)
{


  if (nofile) return true;


  angle*=-180.0/M_PI;

//   angle=70.5;
//   dp=0;
//   z=20;
//   th= 61.0865238181;

  int iA,iDP,iZ,iT;
  double wA,wDP,wZ,wT;
  iA=( angle-start[0])/increment[0];
  wA=(angle-start[0])/increment[0]-iA;
 


  if (iA>=bins[0]-1 || iA<0)
    {
      std::cerr<< "Warning, angle is not inside chamber definition -> not accepted"<< angle<<" "<< iA<<std::endl;
      return false;
      }

  iDP=(dp-start[1])/increment[1];
  wDP=(dp-start[1])/increment[1]-iDP;
  if (iDP>=bins[1]-1 || iDP<0)
    {
      //           std::cerr<< "Warning, dP is not inside chamber definition -> not accepted"<<std::endl;
      return false;
      }

  iZ=(z-start[2])/increment[2];
  wZ=(z-start[2])/increment[2]-iZ;
  if (iZ>=bins[2]-1 || iZ<0)
    {
      //      std::cerr<< "Warning, z is not inside chamber definition -> clamped"<<std::endl;
      if (iZ<0)
	iZ=0;
      else
	iZ=bins[2]-2;
	       //      return false;
      }

  iT=(th-start[3])/increment[3];
  wT=(th-start[3])/increment[3]-iT;
  if (iT>=bins[3]-1 || iT<0)
    {
      std::cerr<< "Warning, theta is not inside chamber definition -> not accepted"<<std::endl;
      return false;
      }

  int base=(iT*bininc[3]+bininc[2]*iZ+bininc[1]*iDP+iA*bininc[0]);

 //   if ( true  )
//     {
//   std::cerr <<"spec angle, dp, z, theta:"<<angle<<" "<<dp<<" "<<z<<" "<<th<<" "<<ph<<std::endl;
//   std::cerr<<" indices: " <<iA<<" "<<iDP<<" "<<iZ<<" "<<iT<<std::endl;
//   std::cerr<<" weights: " <<wA<<" "<<wDP<<" "<<wZ<<" "<<wT<<std::endl;
//   //  std::cerr<<" from/to:"<< from<<" "<<to<<std::endl;
//   std::cerr <<" base:"<<base<<std::endl;
//   std::cerr    <<bininc[0]<<" "    <<bininc[1]<<" "    <<bininc[2]<<" "    <<bininc[3]<<std::endl;
//   std::cerr    <<bins[0]<<" "    <<bins[1]<<" "    <<bins[2]<<" "    <<bins[3]<<std::endl;
//   std::cerr <<values<<" "<<values[0]<<" "<<values[3517075]<<std::endl;
//     }

  double from=
    values[base]*(1-wA)*(1-wDP)*(1-wZ)*(1-wT)+
    values[base+bininc[3]]*(1-wA)*(1-wDP)*(1-wZ)*(wT)+
    values[base+bininc[2]]*(1-wA)*(1-wDP)*(wZ)*(1-wT)+
    values[base+bininc[2]+bininc[3]]*(1-wA)*(1-wDP)*(wZ)*(wT)+
    values[base+bininc[1]]*(1-wA)*(wDP)*(1-wZ)*(1-wT)+
    values[base+bininc[1]+bininc[3]]*(1-wA)*(wDP)*(1-wZ)*(wT)+
    values[base+bininc[1]+bininc[2]]*(1-wA)*(wDP)*(wZ)*(1-wT)+
    values[base+bininc[1]+bininc[2]+bininc[3]]*(1-wA)*(wDP)*(wZ)*(wT)+
    values[base+bininc[0]]*(wA)*(1-wDP)*(1-wZ)*(1-wT)+
    values[base+bininc[3]+bininc[0]]*(wA)*(1-wDP)*(1-wZ)*(wT)+
    values[base+bininc[2]+bininc[0]]*(wA)*(1-wDP)*(wZ)*(1-wT)+
    values[base+bininc[2]+bininc[3]+bininc[0]]*(wA)*(1-wDP)*(wZ)*(wT)+
    values[base+bininc[1]+bininc[0]]*(wA)*(wDP)*(1-wZ)*(1-wT)+
    values[base+bininc[1]+bininc[3]+bininc[0]]*(wA)*(wDP)*(1-wZ)*(wT)+
    values[base+bininc[1]+bininc[2]+bininc[0]]*(wA)*(wDP)*(wZ)*(1-wT)+
    values[base+bininc[1]+bininc[2]+bininc[3]+bininc[0]]*(wA)*(wDP)*(wZ)*(wT);


  base++;



  double to=
    values[base]*(1-wA)*(1-wDP)*(1-wZ)*(1-wT)+
    values[base+bininc[3]]*(1-wA)*(1-wDP)*(1-wZ)*(wT)+
    values[base+bininc[2]]*(1-wA)*(1-wDP)*(wZ)*(1-wT)+
    values[base+bininc[2]+bininc[3]]*(1-wA)*(1-wDP)*(wZ)*(wT)+
    values[base+bininc[1]]*(1-wA)*(wDP)*(1-wZ)*(1-wT)+
    values[base+bininc[1]+bininc[3]]*(1-wA)*(wDP)*(1-wZ)*(wT)+
    values[base+bininc[1]+bininc[2]]*(1-wA)*(wDP)*(wZ)*(1-wT) +
    values[base+bininc[1]+bininc[2]+bininc[3]]*(1-wA)*(wDP)*(wZ)*(wT)+
    values[base+bininc[0]]*(wA)*(1-wDP)*(1-wZ)*(1-wT)+
    values[base+bininc[3]+bininc[0]]*(wA)*(1-wDP)*(1-wZ)*(wT)+
    values[base+bininc[2]+bininc[0]]*(wA)*(1-wDP)*(wZ)*(1-wT)+
    values[base+bininc[2]+bininc[3]+bininc[0]]*(wA)*(1-wDP)*(wZ)*(wT)+
    values[base+bininc[1]+bininc[0]]*(wA)*(wDP)*(1-wZ)*(1-wT)+
    values[base+bininc[1]+bininc[3]+bininc[0]]*(wA)*(wDP)*(1-wZ)*(wT)+
    values[base+bininc[1]+bininc[2]+bininc[0]]*(wA)*(wDP)*(wZ)*(1-wT)+
    values[base+bininc[1]+bininc[2]+bininc[3]+bininc[0]]*(wA)*(wDP)*(wZ)*(wT);


  return (from < ph && ph <to);



  
    
}
