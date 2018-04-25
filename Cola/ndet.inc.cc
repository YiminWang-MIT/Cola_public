unsigned long int ADC_TDC_mismatch = 0;

const int NDetdEPedestals[9] = {0,0,0,0,0,0,0,0,0};

const int NDetOffsets[NDETlayers][NDETbars][3] = { //TDC offsets
  { { 786, 820,  -99 }, { 797, 773,  -45 },
    { 896, 784,  154 }, { 754, 735,   -1 }, { 775, 734,   12 } },
  { { 815, 439,  371 }, { 800, 812,  -30 },
    { 841, 758,   56 }, { 807, 764,   33 }, { 833, 789,   27 } },
  { { 271, 864, -635 }, { 904, 877,   30 },
    { 793, 762,   37 }, { 800, 758,   19 }, { 799, 765,   30 } },
  { { 615, 675,  -73 }, { 646, 650,   10 },
    { 707, 656,   48 }, { 706, 663,  -11 }, { 624, 646,    7 } },
  { { 621, 723,  -54 }, { 679, 624,   27 },
    { 603, 766, -165 }, { 665, 743,  -73 }, {2360, 745, 1612 } },
  { { 762, 671,  146 }, { 678, 639,   81 },
    { 787, 676,   50 }, { 730, 771,  -65 }, { 676, 648,  -43 } }
};

// 5,5  { 617, 745, -125 } },

const int NDetPedestals[NDETlayers][NDETbars][2] = {
  { { 58, 48 }, { 57, 51 }, { 58, 51 }, { 58, 48 }, { 57, 49 } },
  { { 53, 46 }, { 56, 46 }, { 55, 47 }, { 53, 45 }, { 55, 46 } },
  { { 53, 56 }, { 57, 59 }, { 55, 58 }, { 54, 58 }, { 54, 58 } },
  { { 49, 59 }, { 53, 59 }, { 58, 63 }, { 57, 64 }, { 61, 60 } },
  { { 42, 48 }, { 48, 47 }, { 45, 50 }, { 44, 46 }, { 44, 46 } },
  { { 50, 42 }, { 46, 38 }, { 46, 41 }, { 45, 46 }, { 49, 38 } }
};

const double NDetTimeAtTarget[NDETlayers][NDETbars] = {
  {  -1.0,  -1.0,  -1.1,   0.2,  -1.0 },
  {  -1.8,  -0.6,   0.5,   0.6,  -1.8 },
  {  -2.0,  -1.1,  -1.2,   0.1,  -1.1 },
  {  -1.0,  -1.0,   0.0,   0.0,   0.0 },
  {   0.0,   0.0,   0.0,   0.0,   0.0 },
  {   0.0,   0.0,   0.0,   0.0,   0.0 }
};


//bss const double gmpeak = 20.0/225.0;
//bss const double NDetADCscaling[NDETlayers][NDETbars] = {
//bss   {gmpeak/0.8,gmpeak/0.9,gmpeak/0.8,gmpeak/1.22,gmpeak/1.3},
//bss   {gmpeak,gmpeak,gmpeak,gmpeak,gmpeak},
//bss   {gmpeak,gmpeak,gmpeak,gmpeak,gmpeak},
//bss   {gmpeak,gmpeak,gmpeak,gmpeak,gmpeak},
//bss   {gmpeak,gmpeak,gmpeak,gmpeak,gmpeak},
//bss   {gmpeak,gmpeak,gmpeak,gmpeak,gmpeak}};


double NDetBar::TdcDistanceToMulti1Gauge(double x0, double y0, double R, double Tdc1, double Tdc2)
{
  //par: 0:x0, 1:y0, 2:R
  //point: 0:x, 1:y
  double R2=pow(R,2);
  double dy=Tdc2-y0;
  if (Tdc1-dy<x0-R)
    return dy;  //distance to the y=y0 line 
  double dx=Tdc1-x0;
  if (Tdc2-dx<=y0-R)   //to be changed to < later maybe
    return dx;//distance to the x=x0 line 

  //otherwise its in the banana part:
  if (pow(Tdc1-(x0-R),2) + pow(Tdc2-(y0-R), 2) > R2) {
    if (Tdc1 > x0 - R  && Tdc2 > y0 - R){
      double m = 0.;
      while (pow(m+Tdc1-(x0-R),2) + pow(m+Tdc2-(y0-R), 2) > R2)
	m-=1.;
      while (pow(m+Tdc1-(x0-R),2) + pow(m+Tdc2-(y0-R), 2) < R2)
	m+=0.1;
      while (pow(m+Tdc1-(x0-R),2) + pow(m+Tdc2-(y0-R), 2) > R2)
	m-=0.01;
      return (-m);
    } else if (Tdc1 < x0 - R  && Tdc2 < y0 - R){
      double m = 0.;
      while (pow(m+Tdc1-(x0-R),2) + pow(m+Tdc2-(y0-R), 2) >= R2)
	m+=1.;
      while (pow(m+Tdc1-(x0-R),2) + pow(m+Tdc2-(y0-R), 2) < R2)
	m+=1.;  //sign is correct!!
      while (pow(m+Tdc1-(x0-R),2) + pow(m+Tdc2-(y0-R), 2) > R2)
	m-=0.1;
      while (pow(m+Tdc1-(x0-R),2) + pow(m+Tdc2-(y0-R), 2) < R2)
	m+=0.01;
      return (-m);
    } else std::cerr << "oh, no good!\n";
  } else {
    double m = 0.;
    while (pow(m+Tdc1-(x0-R),2) + pow(m+Tdc2-(y0-R), 2) < R2)//DO B
      m+=1.;
    //    return 300;
    return (-m);
  }
  return 999;
  //  else exit(-1);
}




int NDetector::PrintTdcPattern(long int TdcUpPattern, long int TdcDnPattern)
{
  std::cerr << "|";
  for (int c = 1; c<=30; c++) {
    if (1<<(c-1) & TdcUpPattern) std::cerr << "u";
    else std::cerr << "-";
    if (c%5==0) std::cerr << "|";
  }
  std::cerr << std::endl << "|";
  for (int c = 1; c<=30; c++) {
    if (1<<(c-1) & TdcDnPattern) std::cerr << "d";
    else std::cerr << "-";
    if (c%5==0) std::cerr << "|";
  }
  std::cerr << std::endl << std::endl;
  return 0;
}






int NDetector::identify(unsigned int multiplicity = 0)
{
  int easypattern[NDETlayers][NDETbars];
  for (int i=0; i<NDETlayers; i++) {
    for (int j=0; j<NDETbars; j++) {
      easypattern[i][j]=gmeanOK(i,j);
    }
  }
 
  int veto_layer1 = 0;
  int veto_layer2 = 0;
  for (int j=0; j<NDETbars; j++) {
    veto_layer1+=gmeanOK(0, j);
    veto_layer2+=gmeanOK(1, j);
  }

  //  if (veto_layer1+veto_layer2==0) { //not a proton!! maybe neutron
  //  std::cerr << "mult= " << multiplicity << "\n";



  //  if ((veto_layer1==0)&&(multiplicity==bsstools)) { //not a proton!! maybe neutron
  //    easyPatternPlot(easypattern, multiplicity);
  //    temp_check->handle(0); //maybe neutron
  //  } else temp_check->handle(1); //veto signal!!
easyPatternPlot(easypattern, multiplicity);
  return -1;
}



void NDetector::easyPatternPlot(int easypattern[NDETlayers][NDETbars], unsigned int multiplicity = 0)
{
  ////////dE's/////////////////////////////////////
  std::cout << " ";
  for (int v = 3; v>=0; v--){
    if (dE->Veto(v)&0x4) std::cout << "V "; else if (dE->Veto(v)&0x2) std::cout << "v "; 
    else if (dE->Veto(v)&0x1) std::cout << "~ "; else std::cout << ". ";
  }   
  std::cout << " \t ";
  for (int v = 3; v>=0; v--){
    if (dE->Veto(v)&0x20) std::cout << "T "; else if (dE->Veto(v)&0x10) std::cout << "° "; else std::cout << ". ";
  }   
  std::cout << std::endl;
  for (int v = 8; v>=4; v--){
    if (dE->Veto(v)&0x4) std::cout << "V "; else if (dE->Veto(v)&0x2) std::cout << "v "; 
    else if (dE->Veto(v)&0x1) std::cout << "~ "; else std::cout << ". ";
  }   
  std::cout << " \t";
  for (int v = 8; v>=4; v--){
    if (dE->Veto(v)&0x20) std::cout << "T "; else if (dE->Veto(v)&0x10) std::cout << "° "; else std::cout << ". ";
  }   
  std::cout << std::endl;

//    if(v==0) std::cout << "\t\t" << multiplicity;
//    if(v==5 || v == 0) std::cout << "\n";

  for (int i=0; i<NDETlayers; i++) {
    ////////gmean's/////////////////////////////////////
    for (int j1=NDETbars-1; j1>=0; j1--) {
      if (easypattern[i][j1]==0) {
	std::cout << ". ";
      } else if (easypattern[i][j1]==1) {
	std::cout << "* ";
      } else if (easypattern[i][j1]==2) {
	std::cout << "x ";
      } else if (easypattern[i][j1]==3) {
	std::cout << "X ";
      } else std::cout << "F "; //easypattern[i][j1];
    }
    std::cout << "\t";
    ////////TDC's/////////////////////////////////////
    for (int j2=NDETbars-1; j2>=0; j2--) {  //change order for simple plot output .xX... with (bar5, bar4, ..., bar1)
      if (TDCsignalFlag(i, j2)&0x2) {  //best paddle ndet
	std::cout << "T ";
	number_TDCs++;
      } else if (TDCsignalFlag(i, j2)&0x4) {  //best paddle of its layer
	std::cout << "t ";
      } else if (TDCsignalFlag(i, j2)&0x1) {  //not a best paddle, but at least a TDC signal without overflow
	std::cout << "* ";
      } else if (TDCsignalFlag(i, j2)==0) {  //no TDC signal
	std::cout << ". ";
      } else std::cout << "0 ";  //might include an (overflow && not best paddle)
    }
    std::cout << "\n";
  }
  //  std::cout << "\t\t\t" << multiplicity;
  std::cout << "\n";
}




























// *** old stuff which has never been used:
// in int NDetector::handle() after number_TDCs=0:
  /*  if (TriggeR && TriggeR->ok()) {
    int trig = 0;
    //    if ((*TriggeR & 0xb000) == 0xb000) trig = 1;
    //    else if ((*TriggeR & 0x9000) == 0x9000) trig = 2;
    trig = (*TriggeR & 0xF000)>>12;
    //    std::cout << "sync_info = ";
    //    if ((*TriggeR & 0x2000)!=0) std::cout << "A"; else std::cout << " ";
    //    if ((*TriggeR & 0x8000)!=0) std::cout << "N";
    //    std::cout << std::endl;
  }
  else std::cout << "no trigger\n";
  */






#ifdef NEVER
#include "Aqua_online.h"
#include "Aqua_abcn.h"
#include "rundatabase.h"
#include "ndet.h"
#include "HMBook/hmbook.h"     // HMBook Histogram library

/* Die Funktion ndetFill
   - fuellt die beiden Histogramme
       hard wired/pattern
       hard wired/multiplicity
   - ermittelt die Nummer des Szinitillators, der
     den Trigger ausgeloest hat
       online.N.paddle
     und liefert den entsprechenden Offset zur Zeitkorrektur
       online.N.TimeAtTarget
     Hierfuer werden die 20 Parameter
       N.TDC
     aus der Rundatenbank run.db verwendet.


#if HASNDET == 1
void
ndetFill(AquaTree *atree, AquaTree *out, struct MpsDataN *ndet, struct Ndet *N)
{
  if (atree->itemOK(&ndet->det.dE[0].count)) {
    int i,j,sum=0;
    HIST *hist;
    if ((hist = HMFindTitle("hard wired/pattern")))
      for (i = 0; i < 3; i++)
	for (j = 0; j < 16; j++)
	  if (ndet->det.pattern[i] & (1<<j)) HMFill(hist,i*16+j,0,1);
    if ((hist = HMFindTitle("hard wired/multiplicity"))) {
      for (i = 0; i < 3; i++)
	for (j = 0; j < 16; j++)
	  if (ndet->det.pattern[i] & (1<<j)) sum++;
      HMFill(hist,sum,0,1);
    }
  }
  if (atree->itemOK(&ndet->det.dE[0].pm.energy)) {
    int i,min,mini;
    
    min = 4000;
    mini = -1;
    for (i = 0; i < 20; i++) {
      if ( ndet->det.E[i].down.time > 100 && 
	   ndet->det.E[i].down.time < 4000 &&
	   ndet->det.E[i].down.time < min) 
	min = ndet->det.E[i].down.time,mini = i;
    }
    N->paddle = mini;
    N->TimeAtTarget = rundb.N.offset[mini];
    out->packEventData(&N->paddle, 1);
    out->packEventData(&N->TimeAtTarget, 1);
  }
}
#endif
*/


#endif
