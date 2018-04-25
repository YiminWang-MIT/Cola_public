// "npolPrint.cc"
//This file is used to save information and print warnings on screen.

//$Id: npolPrint.cc 2216 2008-06-13 21:13:47Z distler $


#define rn rundb.npol  // Shortcut for run.db variables

#include <ctime>   // getting the system time


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
//:::Check some values in rund.db:::::::::::::::::::::::::::::::::::::::::::://
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
void Cnpol::checkRunDB()
{
//1. Critical errors which causes a program exit
  if (strlen(rn.file_position)==0) {
    std::cerr << "ERROR: No position configuration file for NPOL " << std::endl
         << "       Please specify this file in \"run.db\" via "<< std::endl 
         << "      \"nPol.file.position=...\" "<< std::endl;
    exit(1);
  };
  if (strlen(rn.file_tdcCut)==0) {
    std::cerr << "ERROR: No configuration file for tdc cuts " << std::endl
         << "       Please specify this file in \"run.db\" via "<< std::endl 
         << "       \"nPol.file.tdcCut=...\" "<< std::endl;
    exit(1);
  };
  if (strlen(rn.file_adcCut)==0) {
    std::cerr << "ERROR: No configuration file for adc cuts " << std::endl
         << "       Please specify this file in \"run.db\" via "<< std::endl 
         << "       \"nPol.file.adcCut=...\" "<< std::endl;
    exit(1);
  };

//2. Only warnings, no program exit
  if (strlen(rn.file_timeCut)==0) {
    std::cerr << "WARNING: No configuration file for time cuts " << std::endl
         << "       Please specify this file in \"run.db\" via "<< std::endl 
         << "       \"nPol.file.timeCut=...\" "<< std::endl;
  };
  if (strlen(rn.file_pulshCut)==0) {
    std::cerr << "WARNING: No configuration file for pulsheigh cuts " << std::endl
         << "       Please specify this file in \"run.db\" via "<< std::endl 
         << "       \"nPol.file.pulshCut=...\" "<< std::endl;
  };
  if (strlen(rn.file_pedestal)==0) {
    std::cerr << "WARNING: No configuration file for ADC pedestals " << std::endl
         << "         Please specify this file in \"run.db\" via "<< std::endl 
         << "         \"nPol.file.pedestal=...\" "<< std::endl;
  };
  if (strlen(rn.file_tdcDiff)==0) {
    std::cerr << "WARNING: No configuration file for the offset of TDC_diff " 
         << std::endl
         << "         Please specify this file in \"run.db\" via "<< std::endl 
         << "         \"nPol.file.tdcDiff=...\" "<< std::endl;
  };
  if (strlen(rn.file_tdcSum)==0) {
    std::cerr << "WARNING: No configuration file for the offset of TDC_sum " << std::endl
         << "         Please specify this file in \"run.db\" via "<< std::endl 
         << "         \"nPol.file.tdcSum=...\" "<< std::endl;
  };
  if (strlen(rn.file_tdcWalk)==0) {
    std::cerr << "WARNING: No configuration file for walk correction " << std::endl
         << "         Please specify this file in \"run.db\" via "<< std::endl 
         << "         \"nPol.file.Walk=...\" "<< std::endl;
  };
  if (strlen(rn.file_adcRatio)==0) {
    std::cerr << "WARNING: No configuration file for ADC ratio " << std::endl
         << "         Please specify this file in \"run.db\" via "<< std::endl 
         << "         \"nPol.file.adcRatio=...\" "<< std::endl;
  };

/* obsolete
//==>Check the scaling for multiple hits, only warnings, no exit
  if ((rn.W1_double[0]+rn.W1_double[1]) > 1) {
    std::cerr << "WARNING: Scaling for double hits in 1st wall is wrong." << std::endl
         << "         Check  'nPol.Wall1.double' in 'run.db'\n"<< std::endl;
   };
  if ((rn.W2_triple[0]+rn.W2_triple[1]+rn.W2_triple[2]) > 1) {
    std::cerr << "WARNING: Scaling for triple hits in 2nd wall is wrong." << std::endl
         << "         Check  'nPol.Wall2.triple' in 'run.db'\n"<< std::endl;
  };
  if ((rn.W2_double[0]+rn.W2_double[1]) > 1 ||
      (rn.W2_double[1]+rn.W2_double[2]) > 1 || 
      (rn.W2_double[2]+rn.W2_double[0]) > 1 ) {
    std::cerr << "WARNING: Scaling for double hits in 2nd wall is wrong." << std::endl
         << "         Check  'nPol.Wall2.double' in 'run.db'\n"<< std::endl;
  }; */
  
};


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
//:::Save information for a single paddle:::::::::::::::::::::::::::::::::::://
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
void CnpolPaddle::printInfo() {
  
  char name[256];
  char buffer[3];
   
  strcpy(name,rundb.setup);
  strcat(name,"_npol.conf");
  std::ofstream out(name, std::ios::app);
  if (!out) std::cerr << "Cannot write in the documentation file: "<< name << std::endl; 
   
  switch (detector) {
    case 1: 
       strcpy(name,"v.1st.");
       strcat(name,gcvt((nr+1),2,buffer));
    break;
    case 2:
       strcpy(name,"n.1st.");
       strcat(name,gcvt((nr-14),2,buffer));
    break; 
    case 3:
       strcpy(name,"v.2nd.");
       if (nr <49) strcat(name,gcvt((nr-44),2,buffer));
       else strcat(name,gcvt((nr-56),2,buffer)); 
    break;
    case 4:
       strcpy(name,"n.2nd.");
       if (nr <61) strcat(name,gcvt((nr-48),2,buffer));
       else strcat(name,gcvt((nr-52),2,buffer));
    break; 
    case 5:
       strcpy(name,"trigger.0");
       strcat(name,gcvt((nr-76),2,buffer)); 
    break;
  };
  
  out <<"====>> " << name << " <<====" << std::endl
      <<"  Paddle#  : " << nr << std::endl 
      <<"  Position : (" << x << ", " << y << ", "  << z << ") cm" << std::endl
      <<"  SoL      : " << SoL << " * c" << std::endl
      <<"  Diff0    : " << Diff0 << " ns" << std::endl
      <<"  Time0    : " << Time0 << " ns" << std::endl
      <<"  TDCscale : " << *tdcScale << " ns/Channel" << std::endl
      <<"  Lambda   : " << lambda << " cm" << std::endl
      <<"  DeltaQ   : " << deltaQ << std::endl
      <<"  Walk     : TDC1 a0=" <<  walk1 << std::endl 
      <<"           : TDC2 a0=" <<  walk2 << std::endl 
      <<"  TDCCut   : " << tdc1L << " < TDC1 < " << tdc1H << std::endl
      <<"             " << tdc2L << " < TDC2 < " << tdc2H << std::endl
      <<"  TimeCut  : " << timeL << " < Time < " << timeH << std::endl
      <<"  Pedestals: " << pedlow1 << "  " << pedlow2 << std::endl
      <<"             " << pedup1  << "  " << pedup2  << std::endl
      <<"  MinADC   : " << *minADC <<  std::endl
      <<"  ADCCut   : " << adc1L << " < ADC1 < " << adc1H << std::endl
      <<"             " << adc2L << " < ADC2 < " << adc2H << std::endl
      <<" PulshCut  : " << pulshL << " < Pulsh < " << pulshH << std::endl
      <<"\n\n"<< std::endl;
  out.close();
  strcpy(name,rundb.setup);
  strcat(name,"_npol.pos");
  std::ofstream out2(name, std::ios::app);
  if (!out2) std::cerr << "Cannot write in the position file: "<< name << std::endl;
  out2 << nr << "  " 
       << x  << "  " 
       << y  << "  " 
       << z << std::endl;
  out2.close();
};


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
//:::Save information for total npol::::::::::::::::::::::::::::::::::::::::://
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
void Cnpol::printInfo() {
  
  char filename[256];
  strcpy(filename,rundb.setup);
  strcat(filename,"_npol.conf");
  std::ofstream out (filename);
  if (!out) std::cerr << "Cannot create the documentation file: "<< filename << std::endl; 
  out <<filename << "\n" << std::endl;
  time_t now=time(NULL);   
  out << ctime(&now) << std::endl
      << "REACTION   : " << rundb.reaction << std::endl
      << "SETUP      : " << rundb.setup << std::endl
      << "TARGET     : " << rundb.target << "\n" << std::endl  
    //<< "RUN: " << "run_" << ct->getRunName() << "\n" << std::endl;

      << "SPECA" << std::endl
      << "  Angle           : " << rundb.A.angle << " deg." << std::endl
      << "  Momentum        : " << rundb.A.momentum << " MeV/c" <<std::endl
      << "  Collimator      : " << rundb.A.collimator << std::endl
      << "  TMA             : " <<  rundb.A.tma.filename << std::endl
      << "  Coincedence Time: " <<  rundb.coincidencetime << " ns\n" << std::endl

      << "CONFIG FILES FOR NPOL" << std::endl
      << "  Position : " << rn.file_position  << std::endl
      << "  Pedestals: " << rn.file_pedestal << std::endl 
      << "  TDCdiff  : " << rn.file_tdcDiff << std::endl
      << "  TDCsum   : " << rn.file_tdcSum << std::endl
      << "  ADCratio : " << rn.file_adcRatio << std::endl
      << "  TDCwalk  : " << rn.file_tdcWalk << std::endl
      << "  ADCCut   : " << rn.file_adcCut << std::endl
      << "  TDCCut   : " << rn.file_tdcCut << std::endl
      << "  TimeCut  : " << rn.file_timeCut << std::endl
      << "  PulshCut : " << rn.file_pulshCut << "\n" << std::endl
     
      << "CONFIG VALUES" << std::endl
      <<"  ADC.maxjitter : " << rn.maxjitter << std::endl
      <<"  MaxCoincTime  : " << rn.maxcoincT  << std::endl
      <<"  Noradlow      : " << rn.noradlow << std::endl
      <<"  Noradup       : " << rn.noradup << std::endl
      <<"  mu for wall 2 : " << rn.mu << "\n" << std::endl

      << "1ST WALL" << std::endl  
      << "  Angle             : " << rn.alpha << " deg." << std::endl
      << "  Position          : (" << rn.W1_origin[0] << ", " 
      << rn.W1_origin[1] << ", "  << rn.W1_origin[2] << ") cm" << std::endl
      << "  TDC scaling: Vetos: " << rn.tdcScale[0] << " ns/Channel" << std::endl
      << "               Ndets: " << rn.tdcScale[1] << " ns/Channel" << std::endl
      << "  Minimum ADC: Vetos: " << rn.minADC[0] << std::endl
      << "               Ndets: " << rn.minADC[1] << std::endl
      << "  UseVetos          : " << rn.veto_use << std::endl
      << "  UseNVetos         : " << rn.n_veto_use << std::endl
      << "  Singles           : " << rn.W1_single*100 << " %"<< std::endl
      << "  Doubles           : " <<rn.W1_double[0]*100 << " % : " 
      << rn.W1_double[1]*100  << " %  (1st Layer : 2nd Layer)\n"<< std::endl

      << "2ND WALL" << std::endl
      << "  Position TOP      : (" << rn.Top_origin[0] << ", " 
      << rn.Top_origin[1] << ", "  << rn.Top_origin[2] << ") cm" << std::endl
      << "  Position BOTTOM   : (" << rn.Bottom_origin[0] << ", " 
      << rn.Bottom_origin[1] << ", "  << rn.Bottom_origin[2] << ") cm" << std::endl
      << "  TDC scaling: Vetos: " << rn.tdcScale[2] << " ns/Channel" << std::endl
      << "               Ndets: " << rn.tdcScale[3] << " ns/Channel" << std::endl
      << "  Minimum ADC: Vetos: " << rn.minADC[2] << std::endl
      << "               Ndets: " << rn.minADC[3] << std::endl
      << "  UseVetos          : " << rn.veto_use << std::endl
      << "  Singles           : " << rn.W2_single*100 << " %"<< std::endl
      << "  Doubles           : " <<  rn.W2_double[0]*100 << " % : " 
      << rn.W2_double[1]*100  << " %  (1st Layer : 2nd Layer)"<< std::endl
      << "  Doubles           : " <<  rn.W2_double[2]*100 << " % : " 
      << rn.W2_double[3]*100  << " %  (2nd Layer : 3rd Layer)"<< std::endl
      << "  Doubles           : " <<  rn.W2_double[4]*100 << " % : " 
      << rn.W2_double[5]*100  << " %  (1st Layer : 3rd Layer)"<< std::endl
      << "  Triples           : " << rn.W2_triple[0]*100 << " % : "
      << rn.W2_triple[1]*100 << " % : " << rn.W2_triple[1]*100 
      << " %  (1st Layer : 2nd Layer: 3rd Layer:)\n" << std::endl 
 
      <<"MAGNET" <<std::endl
      <<"  Position   : (" << rn.magnet_pos[0] << ", " 
      << rn.magnet_pos[1] << ", "  << rn.magnet_pos[2] << ") cm" << std::endl 
      <<"  Current    : " << rn.magnet_I << " A" << std::endl
      <<"  SearchPath : " << rn.magnet_path <<  std::endl
      <<"  Bint_low   : " <<  rn.Bint_low << std::endl
      <<"  Bint_up    : " <<  rn.Bint_up << std::endl
      << "\n\n\n" <<std::endl;
  out.close(); 
  strcpy(filename,rundb.setup);
  strcat(filename,"_npol.pos");
  std::ofstream out2 (filename);
  if (!out2) std::cerr << "Cannot create the position file: "<< filename << std::endl;
  out2 << filename  << std::endl;
  out2 << ctime(&now) << "\n" << std::endl;
  out2.close();
  for (int i=1; i<16; i++) wall1->veto[i]->printInfo();
  for (int i=1; i<31; i++) wall1->ndet[i]->printInfo();
  for (int i=1; i<5; i++) wall2->top->veto[i]->printInfo();
  for (int i=1; i<13; i++)wall2->top->ndet[i]->printInfo();
  for (int i=1; i<5; i++) wall2->bottom->veto[i]->printInfo();
  for (int i=1; i<13; i++)wall2->bottom->ndet[i]->printInfo();
  for (int i=1; i<4; i++) trigger[i]->printInfo();  
  //system("emacs npol.conf &" );
};

#undef rn
///////////////THE END////////////////////////////////////////////////////////////////////////////




















