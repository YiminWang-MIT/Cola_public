#include "SimSpek.h"
#include "abcn/detector.h"
#include <iostream>
#include <math.h>

#define FILE "SimSpek: "

#define TRACEL	 12.0
#define TRACEDIST_X_1_2 272.0
#define TRACEL_2 (2 * TRACEL)

;

SimSpek::SimSpek(AquaIO *io , int type)
{
  speknum=type;
  iset=0;

  switch (type){
  case SPEKA:
    name=".a.";
    maxX1=AX1SIZE;
    maxS1=AS1SIZE;
    maxX2=AX2SIZE;
    maxS2=AS2SIZE;
    sposX1=1420.57;
    sposS1=1161.04;
    sposX2=1772.57;
    sposS2=1430.69;


    transform.LoadFirstOrder(rundb.A.tma.firstorder);
    transform.LoadSolution(rundb.A.tma.filename);
    sin_x_s   = sin(40 * M_PI / 180.0);
    sec_x_s   = 1.0 / sin_x_s;
    cos_x_s   = cos(40 * M_PI / 180.0);
    csc_x_s   = 1.0 / cos_x_s;
    tan_x_s   = tan(40 * M_PI / 180.0);
    cot_x_s   = 1.0 / tan_x_s;
    int i;
    for (i=0;i<maxX1;i++)   EFeffX1[i]=rundb.param[0];
    for (i=0;i<maxS1;i++)   EFeffS1[i]=rundb.param[1];
    for (i=0;i<maxX2;i++)   EFeffX2[i]=rundb.param[2];
    for (i=0;i<maxS2;i++)   EFeffS2[i]=rundb.param[3];

    
    for (i=0;i<10;i++) if (rundb.A.vdc.x1.disabled[i]!=-1) EFeffX1[(int)rundb.A.vdc.x1.disabled[i]]=0;
    for (i=0;i<10;i++) if (rundb.A.vdc.s1.disabled[i]!=-1) EFeffS1[(int)rundb.A.vdc.s1.disabled[i]]=0;
    for (i=0;i<10;i++) if (rundb.A.vdc.x2.disabled[i]!=-1) EFeffX2[(int)rundb.A.vdc.x2.disabled[i]]=0;
    for (i=0;i<10;i++) if (rundb.A.vdc.s2.disabled[i]!=-1) EFeffS2[(int)rundb.A.vdc.s2.disabled[i]]=0;

    dvel=rundb.A.vdc.driftvelocity;
    offx1=(int)rundb.A.vdc.offset[0];
    offs1=(int)rundb.A.vdc.offset[1];
    offx2=(int)rundb.A.vdc.offset[2];
    offs2=(int)rundb.A.vdc.offset[3];
    
    wobdx=rundb.A.wobbler.dx/rundb.Ebeam;
    wobdy=rundb.A.wobbler.dy/rundb.Ebeam;
    wobx0=rundb.A.wobbler.x0;
    woby0=rundb.A.wobbler.y0;



    break;
  case SPEKB:
  case SPEKC:
    std::cerr<<"B and C not implemented!"<<std::endl;
  default:
    std::cerr<<"Unknown Spektrometer!"<<std::endl;
  }



 
}



void SimSpek::rescanPointers(AquaTree *atree)
{

  
  syncinfo= (AquaNodeShort*) atree->find((name+"det.sync_info").c_str(),1);
  triggerinfo= (AquaNodeShort*) atree->find((name+"det.trigger.info").c_str(),1);
  AquaNode ** sn=(AquaNode **) vdcx1;
  atree->multifind(sn,0,maxX1-1,(name+"det.vdc.x1[%d]").c_str());
  sn=(AquaNode **) vdcs1;
  atree->multifind(sn,0,maxS1-1,(name+"det.vdc.s1[%d]").c_str());
  sn=(AquaNode **) vdcx2;
  atree->multifind(sn,0,maxX2-1,(name+"det.vdc.x2[%d]").c_str());
  sn=(AquaNode **) vdcs2;
  atree->multifind(sn,0,maxS2-1,(name+"det.vdc.s2[%d]").c_str());
  
 beamposition=(AquaNodeShort*) atree->find((name+"beam.position").c_str(),1);

}

void SimSpek::trigger(GenState *state)
{


  in[0]=state->tc[speknum].dp;//(state->tc[speknum].dp+100)*660/630-100;
  in[1]=state->tc[speknum].th;
  in[2]=state->tc[speknum].y0*10;
  in[3]=state->tc[speknum].ph;
  
#ifdef EVDEBUGFLAG
  std::cerr << "Dp:" <<state->tc[speknum].dp<<std::endl;
  std::cerr << "th:" <<state->tc[speknum].th<<std::endl;
  std::cerr << "y0:" <<state->tc[speknum].y0<<std::endl;
  std::cerr << "ph:" <<state->tc[speknum].ph<<std::endl;
#endif

  transform.firstorderreconstruct(in);
  transform.lookup(in,out);

#ifdef EVDEBUGFLAG
  std::cerr << "x:" <<out[OUTX]<<std::endl;
  std::cerr << "t:" <<out[OUTTHETA]<<std::endl;
  std::cerr << "y:" <<out[OUTY]<<std::endl;
  std::cerr << "p:" <<out[OUTPHI]<<std::endl;
#endif

  slopex=-tan((out[OUTTHETA]/1E3)-M_PI_2);
  slopes=sec_x_s/(cot_x_s/slopex-tan(out[OUTPHI]/1e3)); 
  
  posx1=out[OUTX];
  poss1=-out[OUTY]/sec_x_s+out[OUTX]*cos_x_s+TRACEL_2/slopes;


  posx2=posx1+TRACEDIST_X_1_2/slopex;
  poss2=poss1+TRACEDIST_X_1_2/slopes;


#ifdef EVDEBUGFLAG
  std::cerr<<FILE<<"Focal Plane Coords:"<<std::endl;
  std::cerr<<"\tPos:  "<<posx1<<"\t"<<poss1<<"\t"<<posx2<<"\t"<<poss2<<std::endl;
  std::cerr<<"\tSlope: "<<slopex<<"\t"<<slopes<<std::endl;
#endif

  //TODO Scintillator check

  state->trigger[speknum]=true;
}

double SimSpek::rnd()
{
  return rand()*1.0/RAND_MAX;

}


double  SimSpek::norm()
{
  double fac, r, v1, v2;
  if  (iset == 0) {
    do {
      v1 = 2 * rnd() - 1;
      v2 = 2 * rnd() - 1;
      r = v1 * v1 + v2 * v2;
    } while (r >= 1);
    fac = sqrt(- 2 * log(r) / r);
    gset = v1 * fac;
    iset = 1;
    return v2 * fac;
  } else {
    iset = 0;
    return gset;
  }
}

#define Ca0   11.7881
#define Ca1   0.735491 
#define Cb0   6.63626 
#define Cb1   -2.34216
#define Cc1   -0.0047097












#define Cma                0.98919 
#define Cmb               -0.448688
#define Cmc                0.00611377
#define Cme             -177.564
#define Cmg                2.99766
#define Cmh               -2.7683e-05
#define Cmi              257.563 
#define Cmj               6.70782 



double SimSpek::Efficency(double dist,double slope,double eff)
{
  //  if (fabs(dist)<0.5) return 0;
  double d2=dist*dist;
  double d4=d2*d2;
  double f= eff*(1+Cc1*slope)/(1+exp( ( fabs(dist) - (Ca0+eff*Ca1*slope)) *(Cb0+eff*Cb1*slope) ) );
  double m=Cma/pow(eff,1.4)+pow(1-eff,1.8)*(Cmb*d2+Cmc*d4+Cme/(Cmg+d2)+Cmh*d4*d2+Cmi/(Cmj+d4));
  return f*m;
}



short SimSpek::Time(double dist,double slope)
{
   double corr;
  if ((10.0<slope)||(slope<0.1)) corr=0; else
    corr=-0.12458+slope*(0.36502+slope*(0.22858-0.024383*slope));

  if (dist<0.3)  dist=norm()*0.4+0.7;
 

  if (dist-corr>1.1) dist-=corr;  else   dist/=(1+corr/1.1);
  short  time=(short ) round(dist/dvel);

  time+=(short) (8*norm());

#ifdef EVDEBUGFLAG
  std::cerr<<FILE<<"Dist: " << dist<<"\tTime: "<<time<<std::endl;
#endif
  return time;
}

#define cut(spos,pos,slope,max) start=(int)((spos-(pos+35.0/slope))/5.0);\
stop=1+(int)((spos-(pos-35.0/slope))/5.0);\
start=start<0?0:start;start=start>max?max:start;stop=stop<0?0:stop;stop=stop>max?max:stop;

void SimSpek::fill(GenState *state)
{
  short sbuf;
  int start,stop;
  slopex=fabs(slopex);
  slopes=fabs(slopes);

  std::cout <<"k "<<posx1<<"\tl\t"<<slopex<<"\t";
  cut(sposX1,posx1,slopex,maxX1)
    for (int i=start;i<stop;i++)
      {
	double wpos=sposX1-5*i;
	double z=(wpos-posx1)*slopex;
	if (rnd()< Efficency(z,slopex,EFeffX1[i])) vdcx1[i]->handle(offx1-Time(fabs(z),slopex));
      }
  cut (sposS1,poss1,slopes,maxS1)
    for (int i=start;i<stop;i++)
      {
	double wpos=sposS1-5*i;
	double z=(wpos-poss1)*slopes;
	if (rnd()< Efficency(z,slopes,EFeffS1[i])) 	vdcs1[i]->handle(offs1-Time(fabs(z),slopes));

      }
  cut (sposX2,posx2,slopex,maxX2)
    for (int i=start;i<stop;i++)
      {
	double wpos=sposX2-5*i;
	double z=(wpos-posx2)*slopex;
	if (rnd()< Efficency(z,slopex,EFeffX2[i])) 	vdcx2[i]->handle(offx2-Time(fabs(z),slopex));

      }
  cut (sposS2,poss2,slopes,maxS2)
    for (int i=start;i<stop;i++)
      {
	double wpos=sposS2-5*i;
	double z=(wpos-poss2)*slopes;
	if (rnd()< Efficency(z,slopes,EFeffS2[i])) 	vdcs2[i]->handle(offs2-Time(fabs(z),slopes));
      }

  unsigned char tempx=(unsigned char)round(state->TargetCoords[0]/wobdx-wobx0);
  unsigned char tempy=(unsigned char)round(state->TargetCoords[1]/wobdy-woby0);

  beamposition->handle((short) ((tempx<<8)+tempy));

  sbuf=(state->EventNumber & 0xfff)+0x2000;
  syncinfo->handle(sbuf);
  triggerinfo->handle(0);
}
