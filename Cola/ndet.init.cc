NDetector::NDetector(AquaTree *atree, AquaTree *out)
{
  for (int i=0; i<NDETlayers; i++) {
    layer[i] = new NDetLayer(atree, out, i);
  }
  dE = new NDetdE(atree, out);
  temp_check= (AquaNodeInt *) out->find(".N.temp_check");
  multi   = (AquaNodeInt *) out->find(".N.multi");
  pattern = (AquaNodeInt *) out->find(".N.pattern");
  charged = (AquaNodeInt *) out->find(".N.charged");
  paddle  = (AquaNodeShort *) out->find(".N.paddle");
  timeAtTarget = (AquaNodeDouble *) out->find(".N.TimeAtTarget");
  char str[128];  
  sprintf(str, ".N.highest_ndet_gmean");
  highest_ndet_gmean = (AquaNodeDouble *) out->find(str);
  sprintf(str, ".N.highest_ndet_26gmean");
  highest_ndet_26gmean = (AquaNodeDouble *) out->find(str);
  sprintf(str, ".N.sum_ndet_gmean");
  sum_ndet_gmean = (AquaNodeDouble *) out->find(str);
  sprintf(str, ".ndet.det.sync_info");
  sync_info_ndet = (AquaNodeInt *) atree->find(str);
  sprintf(str, ".a.det.sync_info");
  sync_info_a = (AquaNodeInt *) atree->find(str);
  sprintf(str, ".N.BestTdcMulti1Distance");
  mAN_BestTdcMulti1Distance = (AquaNodeDouble *) out->find(str);

  sprintf(str, ".a.coinc.a_c_time_1");
  mAND_TdcAN = (AquaNodeInt *) atree->find(str);
  sprintf(str, ".A.TimeAtTarget");
  mAND_ATimeAtTarget = (AquaNodeDouble *) out->find(str);

  sprintf(str, ".N.TimeOfTrigger");
  mANDo_TimeOfTrigger = (AquaNodeDouble *) out->find(str);



  bestTiming = 1E6; bestPaddle = -1;
}

NDetector::~NDetector()
{for (int i=0; i<NDETlayers; i++) delete layer[i];}

int NDetector::init(struct NDET *rundb)
{
  if (!rundb) return -1;

  for (int i=0; i<NDETlayers; i++)
    layer[i]->init(rundb->offset+i*2*NDETbars,
		   rundb->flasherpedestal+i*2*NDETbars,
		   rundb->position+i*NDETbars,
		   rundb->timeAtTarget+i*NDETbars,
		   rundb->flasherpeak+i*2*NDETbars,
		   rundb->pedestal+i*2*NDETbars,
		   rundb->AdcParticlePeak+i*2*NDETbars, 
		   rundb->TdcMulti1Gauge+i*3*NDETbars);
 return 0;
		   //rundb->flasherstarting+i*2*NDETbars,
                   //rundb->flasher2energy+i*2*NDETbars 
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

NDetLayer::NDetLayer(AquaTree *atree, AquaTree *out, int layer)
{
  char str[128];  
  sprintf(str, ".N.layer[%d].highest_layer_gmean", layer+1);
  highest_layer_gmean = (AquaNodeDouble *) out->find(str);
  sprintf(str, ".N.layer[%d].sum_layer_gmean", layer+1);
  sum_layer_gmean = (AquaNodeDouble *) out->find(str);
  sprintf(str, ".N.layer[%d].BestTdcMulti1Distance", layer+1);
  mAN_BestTdcMulti1Distance = (AquaNodeDouble *) out->find(str);

  for (int i=0; i<NDETbars; i++) {
    bar[i] = new NDetBar(atree, out, layer, i);
  }
  bestTiming = 1E6; bestPaddle = -1;
}

NDetLayer::~NDetLayer()
{
  for (int i=0; i<NDETbars; i++) delete bar[i];
}

//bssold: int NDetLayer::init(double *offs, double *ped, double *pos, double *tim)
int NDetLayer::init(double *offs, double *flped, double *pos, double *tim, double *flpeak, double *adcped, double *adcpeak, double *TdcMulti1Gauge)
{
  if (!offs || !flped || !pos || !tim || !flpeak || !adcped || !adcpeak || !TdcMulti1Gauge) {
    std::cerr << "PROBLEMS IN NDetLayer::init\n";
    sleep(5);
    return -1;
  }

  for (int i=0; i<NDETbars; i++)
    bar[i]->init(offs+2*i, flped+2*i, pos+i, tim+i, flpeak+2*i, adcped+2*i, adcpeak+2*i, TdcMulti1Gauge+3*i);

  return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

NDetBar::NDetBar(AquaTree *atree, AquaTree *out, int layer, int bar)
{
  char str[128];
  index = bar+layer*NDETbars;
  la_yer = layer; //0..5
  ba_r = bar; //0..4
  sprintf(str, ".ndet.det.E[%d].up.energy", index);
  mAN_AdcUp = (AquaNodeShort *) atree->find(str);
  sprintf(str, ".ndet.det.E[%d].down.energy", index);
  mAN_AdcDown = (AquaNodeShort *) atree->find(str);

  //cables of TDC layer 3 had been switched: up <-> down ==>> this has been repaired in AquaBasel.cc!!!!
  //  if (layer!=3-1) {
  sprintf(str, ".ndet.det.E[%d].up.time", index);
  mIn_TdcUp = (AquaNodeShort *) atree->find(str);
  sprintf(str, ".ndet.det.E[%d].down.time", index);
  mIn_TdcDown = (AquaNodeShort *) atree->find(str);
  //  } else {  //layer==3-1
  //    sprintf(str, ".ndet.det.E[%d].down.time", index);
  //    mIn_TdcUp = (AquaNodeShort *) atree->find(str);
  //    sprintf(str, ".ndet.det.E[%d].up.time", index);
  //    mIn_TdcDown = (AquaNodeShort *) atree->find(str);
    //  }


  sprintf(str, ".N.layer[%d].bar[%d].Gmean", layer+1, bar+1);
  mAN_Gmean = (AquaNodeDouble *) out->find(str);
  sprintf(str, ".N.layer[%d].bar[%d].AdcParticlePeakUp", layer+1, bar+1);
  mAN_AdcParticlePeakUp = (AquaNodeDouble *) out->find(str);
  sprintf(str, ".N.layer[%d].bar[%d].AdcParticlePeakDown", layer+1, bar+1);
  mAN_AdcParticlePeakDown = (AquaNodeDouble *) out->find(str);
  sprintf(str, ".N.layer[%d].bar[%d].GmeanParticlePeak", layer+1, bar+1);
  mAN_GmeanParticlePeak = (AquaNodeDouble *) out->find(str);
  sprintf(str, ".N.layer[%d].bar[%d].Position", layer+1, bar+1);
  mAN_Position = (AquaNodeDouble *) out->find(str);
  sprintf(str, ".N.layer[%d].bar[%d].EUp", layer+1, bar+1);
  mAN_EUp = (AquaNodeDouble *) out->find(str);
  sprintf(str, ".N.layer[%d].bar[%d].EDown", layer+1, bar+1);
  mAN_EDown = (AquaNodeDouble *) out->find(str);
  sprintf(str, ".N.layer[%d].bar[%d].AdcPedestalUp", layer+1, bar+1);
  mAN_AdcPedestalUp = (AquaNodeDouble *) out->find(str);
  sprintf(str, ".N.layer[%d].bar[%d].AdcPedestalDown", layer+1, bar+1);
  mAN_AdcPedestalDown = (AquaNodeDouble *) out->find(str);
  sprintf(str, ".N.layer[%d].bar[%d].AdcUpCorr", layer+1, bar+1);
  mAN_AdcUpCorr = (AquaNodeDouble *) out->find(str);
  sprintf(str, ".N.layer[%d].bar[%d].AdcDownCorr", layer+1, bar+1);
  mAN_AdcDownCorr = (AquaNodeDouble *) out->find(str);

  sprintf(str, ".N.layer[%d].bar[%d].TdcUp", layer+1, bar+1);
  mOut_TdcUp = (AquaNodeDouble *) out->find(str);
  sprintf(str, ".N.layer[%d].bar[%d].TdcDown", layer+1, bar+1);
  mOut_TdcDown = (AquaNodeDouble *) out->find(str);
  sprintf(str, ".N.layer[%d].bar[%d].tUp", layer+1, bar+1);
  mAN_tUp = (AquaNodeDouble *) out->find(str);
  sprintf(str, ".N.layer[%d].bar[%d].tDown", layer+1, bar+1);
  mAN_tDown = (AquaNodeDouble *) out->find(str);

  sprintf(str, ".N.layer[%d].bar[%d].TdcMulti1Distance", layer+1, bar+1);
  mAN_TdcMulti1Distance = (AquaNodeDouble *) out->find(str);

  okFlag  = 0;
  pedUp   = NDetPedestals[layer][bar][0];
  pedDown = NDetPedestals[layer][bar][1];
  offsUp   =  NDetOffsets[layer][bar][0];
  offsDown =  NDetOffsets[layer][bar][1];
  offsPos  =  NDetOffsets[layer][bar][2];
  offsTime = NDetTimeAtTarget[layer][bar];
  //  std::cout << "check " << layer+1 << bar+1<< "up:" << pedUp << "  down:"<< pedDown<<"\n";
  //  usleep(100000);

}

NDetBar::~NDetBar()
{
  delete mAN_AdcUp; delete mAN_AdcDown; delete mIn_TdcUp; delete mIn_TdcDown;
  delete mAN_Gmean; delete mAN_Position;
  delete mAN_EUp; delete mAN_EDown; delete mAN_tUp; delete mAN_tDown;
}

//bssold: int NDetBar::init(double *offs, double *ped, double *pos, double *tim)
int NDetBar::init(double *offs, double *flped, double *pos, double *tim, double *flpeak, double *adcped, double *adcpeak, double *TdcMulti1Gauge)
{
  if (!offs || !flped || !pos || !tim || !flpeak || !adcped || !adcpeak || !TdcMulti1Gauge) {
    std::cerr << "PROBLEMS IN NDetBar::init\n";
    sleep(5);
    return -1;
  }
  offsUp   = (int)offs[0];
  offsDown = (int)offs[1];
  //pedUp    = (int)flped[0];   //pedestals while beam are not the same like pedestals with flasher without beam
  //pedDown  = (int)flped[1];
  pedUp    = adcped[0];
  pedDown  = adcped[1];
  offsPos  = (int)*pos;
  offsTime = *tim;
  AdcParticlePeakUp   = adcpeak[0];
  AdcParticlePeakDown = adcpeak[1];
  flasherUp    = flpeak[0];
  flasherDown  = flpeak[1];
  TdcMulti1GaugeX0 = TdcMulti1Gauge[0];
  TdcMulti1GaugeY0 = TdcMulti1Gauge[1];
  TdcMulti1GaugeR  = TdcMulti1Gauge[2];
  //  std::cout << "L" << la_yer+1 << "B" << ba_r+1 << " TdcMulti1Gauge:\tx0="<< TdcMulti1GaugeX0 <<"  \ty0="<< TdcMulti1GaugeY0 <<"  \t R="<< TdcMulti1GaugeR << std::endl;
  return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

NDetdE::NDetdE(AquaTree *atree, AquaTree *out)
{
  char str[128];

  for (int index = 0; index<9; index++) {
    dEBar[index] = new NDetdEBar(atree, out, index);
    sprintf(str, ".ndet.det.dE[%d].pm.energy", index); mANS_Adc[index] = (AquaNodeShort *) atree->find(str);
    sprintf(str, ".ndet.det.dE[%d].pm.time", index);   mANS_Tdc[index] = (AquaNodeShort *) atree->find(str);

    /*    sprintf(str, ".N.dE.ECorr[%d]", index);   mAND_ECorr[index] = (AquaNodeDouble *) out->find(str);
    sprintf(str, ".N.dE.TCorr[%d]", index);   mAND_TCorr[index] = (AquaNodeDouble *) out->find(str);
    sprintf(str, ".N.dE.Veto[%d]", index);    mANI_Veto[index] = (AquaNodeInt *) out->find(str);
    */
  }
}

NDetdE::~NDetdE()
{
  delete mANS_Adc; delete mANS_Tdc;
}


NDetdEBar::NDetdEBar(AquaTree *atree, AquaTree *out, int index)
{
  char str[128];
  m_index=index;
  sprintf(str, ".ndet.det.dE[%d].pm.energy", index); 
  mANSa_Adc = (AquaNodeShort *) atree->find(str);
  sprintf(str, ".ndet.det.dE[%d].pm.time", index);   
  mANSa_Tdc = (AquaNodeShort *) atree->find(str);
  sprintf(str, ".N.dE.bar[%d].veto", index);   
  mANIo_Veto = (AquaNodeInt *) out->find(str);
  
  
}

NDetdEBar::~NDetdEBar()
{
  ;
}
