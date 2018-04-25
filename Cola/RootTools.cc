#undef ROOTFILEDEBUG

#include "RootTools.h"

void AnElementShort::SetTitle()
{ if( nEle==1 ) Title_Root = Name_Root+"/S";
  else Title_Root = Name_Root+Form("[%d]/S",nEle); }
void AnElementUnsignedShort::SetTitle()
{ if( nEle==1 ) Title_Root = Name_Root+"/s";
  else Title_Root = Name_Root+Form("[%d]/s",nEle); }
void AnElementLong::SetTitle()
{ if( nEle==1 ) Title_Root = Name_Root+"/L";
  else Title_Root = Name_Root+Form("[%d]/L",nEle); }
void AnElementInt::SetTitle()
{ if( nEle==1 ) Title_Root = Name_Root+"/I";
  else Title_Root = Name_Root+Form("[%d]/I",nEle); }
void AnElementDouble::SetTitle()
{ if( nEle==1 ) Title_Root = Name_Root+"/D";
  else Title_Root = Name_Root+Form("[%d]/D",nEle); }

RootTools::RootTools()
{
  Use_ROOT = 0;
  RootFileName = "greentea.root";
  OpenFile();
}

RootTools::RootTools(char *filename)
{
  Use_ROOT = 0;
  RootFileName = filename;
  OpenFile();
}

void RootTools::OpenFile()
{
  RFile = new TFile( RootFileName, "recreate" );
  tree = new TTree( "tree", "tree" );
#ifdef ROOTFILEDEBUG
  std::cout << "<RootTools> file name : " << RFile->GetName()
	    << "\n<RootTools> tree name : " << tree->GetName()
	    << std::endl;
#endif
}

void RootTools::CloseFile()
{
  RFile->Write();
  RFile->Close();
}

void RootTools::Fill()
{
  GetData();
  Fill( "year", Year );
  Fill( "runID", RunID );
  tree->Fill();

  for( int i=0; i<(int)ElementsContainer.size(); i++ ){
    ElementsContainer[i]->Clear();
  }
}

RootTools::~RootTools()
{
  std::cout << " Deconstructor of RootTools " << std::endl;
}

void RootTools::GetData()
{
  // values from "abcn" or "online" in Cola
  for( int i=0; i<(int)ElementsContainer.size(); i++ ){
    int nele = ElementsContainer[i]->nele();
    if( nele==0 ) continue;
    if( ElementsContainer[i]->nanode()==0 ) continue;
    if( ElementsContainer[i]->type()==5 ){ // AnElementVector
      std::string name_root = ElementsContainer[i]->name_root();
      std::string name_cola = ElementsContainer[i]->name_cola();
      for( int ianode=0; ianode<ElementsContainer[i]->nanode(); ianode++ ){
	AquaNode *anode = ElementsContainer[i]->anode(ianode);
	if( anode==NULL ) continue;
	short val = -9999;
	if( anode->ok() ){
	  val = (short)anode->scalar();
	  int aaa=((0xffff&ianode)<<16)+(val&0xffff);
	  ElementsContainer[i]->SetVal( aaa, ianode );
	}
      }
    }
    else{ //  others
      std::string name_root = ElementsContainer[i]->name_root();
      std::string name_cola = ElementsContainer[i]->name_cola();
#if 0
      std::cout << name_root << "  " << name_cola << std::endl;
#endif
      for( int iele=0; iele<nele; iele++ ){
	AquaNode *anode = ElementsContainer[i]->anode(iele);
	if( anode==NULL ) continue;
	double val=-99999;
	if( anode->ok() ){ val = anode->scalar(); }
	ElementsContainer[i]->SetVal( val, iele );
      }
#if 0
      ElementsContainer[i]->PrintElements();
#endif
    }
  }
}

void RootTools::SetElementShort( std::string name_root, int nele )
{
  AnElementShort *ele = new AnElementShort(name_root,"nothing",nele);
  AddElementToTree(ele);
  ElementsContainer.push_back(ele);
}

void RootTools::SetElementShort( std::string name_root, std::string name_cola, int nele, int ele0 )
{
  AnElementShort *ele = new AnElementShort(name_root,name_cola,nele);
  SetAquaNode( ele, name_cola, nele, ele0 );
  AddElementToTree(ele);
  ElementsContainer.push_back(ele);
}

void RootTools::SetElementUnsignedShort( std::string name_root, int nele )
{
  AnElementUnsignedShort *ele = new AnElementUnsignedShort(name_root,"nothing",nele);
  AddElementToTree(ele);
  ElementsContainer.push_back(ele);
}

void RootTools::SetElementUnsignedShort( std::string name_root, std::string name_cola, int nele, int ele0 )
{
  AnElementUnsignedShort *ele = new AnElementUnsignedShort(name_root,name_cola,nele);
  SetAquaNode( ele, name_cola, nele, ele0 );
  AddElementToTree(ele);
  ElementsContainer.push_back(ele);
}

void RootTools::SetElementLong( std::string name_root, int nele )
{
  AnElementLong *ele = new AnElementLong(name_root,"nothing",nele);
  AddElementToTree(ele);
  ElementsContainer.push_back(ele);
}

void RootTools::SetElementLong( std::string name_root, std::string name_cola, int nele, int ele0 )
{
  AnElementLong *ele = new AnElementLong(name_root,name_cola,nele);
  SetAquaNode( ele, name_cola, nele, ele0 );
  AddElementToTree(ele);
  ElementsContainer.push_back( ele);
}

void RootTools::SetElementInt( std::string name_root, int nele )
{
  AnElementInt *ele = new AnElementInt(name_root,"nothing",nele);
  AddElementToTree(ele);
  ElementsContainer.push_back(ele);
}

void RootTools::SetElementInt( std::string name_root, std::string name_cola, int nele, int ele0 )
{
  AnElementInt *ele = new AnElementInt(name_root,name_cola,nele);
  SetAquaNode( ele, name_cola, nele, ele0 );
  AddElementToTree(ele);
  ElementsContainer.push_back( ele);
}

void RootTools::SetElementDouble( std::string name_root, int nele )
{
  AnElementDouble *ele = new AnElementDouble(name_root,"nothing",nele);
  AddElementToTree(ele);
  ElementsContainer.push_back(ele);
}

void RootTools::SetElementDouble( std::string name_root, std::string name_cola, int nele, int ele0 )
{
  AnElementDouble *ele = new AnElementDouble(name_root,name_cola,nele);
  SetAquaNode( ele, name_cola, nele, ele0 );
  AddElementToTree(ele);
  ElementsContainer.push_back( ele);
}

void RootTools::SetElementVector( std::string name_root, std::string name_cola, int nele, int ele0 )
{
  AnElementVector *ele = new AnElementVector(name_root,name_cola);
  SetAquaNode( ele, name_cola, nele, ele0 );
  AddElementVecToTree(ele);
  ElementsContainer.push_back(ele);
}

void RootTools::SetAquaNode( AnElement *ele, std::string name_cola, int nele, int ele0 )
{
  AquaNode *anode = 0;
  char *aname0 = strcpy( new char[name_cola.size()+1],name_cola.c_str() );
  for( int i=0; i<nele; i++ ){
    char *aname;
    if( nele==1 ) aname = aname0;
    else          aname = Form( aname0, ele0+i );
    anode = ATree->find( aname, 0 );
    if( anode==NULL ) anode = Out->find( aname, 0 );
    if( anode==NULL ){
      std::cout << "######################################################"  << std::endl;
      std::cout << " in RootTools::SetAquaNode " << std::endl;
      std::cout << " A name :" << aname << " cannot be found. Please check it." << std::endl;
      std::cout << "######################################################"  << std::endl;
    }
    else{
      ele->SetAquaNode(anode);
    }
  }
  if( nele != (int)ele->nanode() ){
    std::cout << " #Elements are not consistent between nEle:" << nele
	      << " #anode:" << ele->nanode() << std::endl;
  }
}

void RootTools::AddElementToTree( AnElement *anele )
{
  if( anele->val()!=0 ){
    tree->Branch( anele->name_root().c_str(), anele->val(), anele->title_root().c_str() );
  }
  else{
    std::cout << "valuable is not defined" << std::endl;
  }
}

void RootTools::AddElementVecToTree( AnElement *anele )
{
  if( anele->val()!=0 ){
    std::vector <int> *vec = (std::vector <int>*)anele->val();
    tree->Branch( anele->name_root().c_str(), vec );
  }
  else{
    std::cout << "valuable is not defined" << std::endl;
  }
}

AnElement *RootTools::SearchElement( std::string name_root )
{
  for( int ia=0; ia<(int)ElementsContainer.size(); ia++ ){
    if( ElementsContainer[ia]->name_root() == name_root ) return ElementsContainer[ia];
  }
#if 0
  std::cout << "######################################################"  << std::endl;
  std::cout << " in RootTools::SearchElement" << std::endl;
  std::cout << " An element of " << name_root << " cannot be found." << std::endl;
  std::cout << "######################################################"  << std::endl;
#endif
  return 0;
}

void RootTools::Fill( std::string name_root, short val, int iele )
{ AnElement *anele = SearchElement( name_root );
  if( anele==0 ){ return; }
  anele->SetVal( val, iele ); }
void RootTools::Fill( std::string name_root, unsigned short val, int iele )
{ AnElement *anele = SearchElement( name_root );
  if( anele==0 ){ return; }
  anele->SetVal( val, iele ); }
void RootTools::Fill( std::string name_root, long val, int iele )
{ AnElement *anele = SearchElement( name_root );
  if( anele==0 ){ return; }
  anele->SetVal( val, iele ); }
void RootTools::Fill( std::string name_root, int val, int iele )
{ AnElement *anele = SearchElement( name_root );
  if( anele==0 ){ return; }
  anele->SetVal( val, iele ); }
void RootTools::Fill( std::string name_root, double val, int iele )
{ AnElement *anele = SearchElement( name_root );
  if( anele==0 ){ return; }
  anele->SetVal( val, iele ); }

void RootTools::SetOutput( AquaTree *atree, AquaTree *out, int use_root )
{
  ATree = atree;
  Out = out;
  Use_ROOT = use_root;
  int nele,ele0;

  SetElementInt( "evID" );
  SetElementInt( "year" );
  SetElementInt( "runID" );
  SetElementDouble( "deadtime" );

  if( Use_ROOT == 1 ){ // tohoku group setting
    /**************/
    /* SPECA abcn */
    /**************/
    SetElementShort( "a_runtime",       "abcn.a.run.runtime" );
    SetElementShort( "a_realtime",      "abcn.a.run.realtime" );
    SetElementShort( "a_actime1",       "abcn.a.coinc.a_c_time_1" );
    SetElementShort( "a_actime2",       "abcn.a.coinc.a_c_time_2" );
    SetElementShort( "a_actime3",       "abcn.a.coinc.a_c_time_3" );
    SetElementInt(   "a_accoinc",       "abcn.a.coinc.a_c_coinc" );
    SetElementInt(   "a_cdead",         "abcn.a.coinc.dead_c" );
    SetElementInt(   "a_acdead",        "abcn.a.coinc.dead_ac" );
    SetElementShort( "a_beam_position", "abcn.a.beam.position" );

    /**************/
    /* SPECB abcn */
    /**************/
    SetElementShort( "b_runtime",       "abcn.b.run.runtime" );
    SetElementShort( "b_realtime",      "abcn.b.run.realtime" );
    SetElementShort( "b_abtime1",       "abcn.b.coinc.a_b_time_1" );
    SetElementShort( "b_abtime2",       "abcn.b.coinc.a_b_time_2" );
    SetElementShort( "b_abtime3",       "abcn.b.coinc.a_b_time_3" );
    SetElementShort( "b_actime1",       "abcn.b.coinc.a_c_time_1" );
    SetElementShort( "b_actime2",       "abcn.b.coinc.a_c_time_2" );
    SetElementShort( "b_actime3",       "abcn.b.coinc.a_c_time_3" );
    SetElementInt(   "b_accoinc",       "abcn.b.coinc.a_c_coinc" );
    SetElementInt(   "b_cdead",         "abcn.b.coinc.dead_c" );
    SetElementInt(   "b_acdead",        "abcn.b.coinc.dead_ac" );
    SetElementShort( "b_beam_position", "abcn.b.beam.position" );

    /**************/
    /* SPECC abcn */
    /**************/
    SetElementShort( "c_runtime",       "abcn.c.run.runtime" );
    SetElementShort( "c_realtime",      "abcn.c.run.realtime" );
    SetElementShort( "c_actime1",       "abcn.c.coinc.a_c_time_1" );
    SetElementShort( "c_actime2",       "abcn.c.coinc.a_c_time_2" );
    SetElementShort( "c_actime3",       "abcn.c.coinc.a_c_time_3" );
    SetElementInt(   "c_accoinc",       "abcn.c.coinc.a_c_coinc" );
    SetElementInt(   "c_cdead",         "abcn.c.coinc.dead_c" );
    SetElementInt(   "c_acdead",        "abcn.c.coinc.dead_ac" );
    SetElementShort( "c_beam_position", "abcn.c.beam.position" );

    /*************/
    /* KAOS abcn */
    /*************/
    SetElementShort( "label",      "abcn.kaos.label" );
    SetElementShort( "k_runtime",  "abcn.kaos.run.runtime" );
    SetElementShort( "k_realtime", "abcn.kaos.run.realtime" );
    SetElementInt(   "kdead",      "abcn.kaos.dead_k" );
    SetElementInt(   "cdead",      "abcn.kaos.dead_c" );
    SetElementInt(   "kcdead",     "abcn.kaos.dead_kc" );
    // TOF
    SetElementShort( "gtdct", "abcn.kaos.tof.gdet[%d].top.time",      nele=30 );
    SetElementShort( "gadct", "abcn.kaos.tof.gdet[%d].top.charge",    nele=30 );
    SetElementShort( "gtdcb", "abcn.kaos.tof.gdet[%d].bottom.time",   nele=30 );
    SetElementShort( "gadcb", "abcn.kaos.tof.gdet[%d].bottom.charge", nele=30 );
    SetElementShort( "htdct", "abcn.kaos.tof.fdet[%d].top.time",      nele=30 );
    SetElementShort( "hadct", "abcn.kaos.tof.fdet[%d].top.charge",    nele=30 );
    SetElementShort( "htdcb", "abcn.kaos.tof.fdet[%d].bottom.time",   nele=30 );
    SetElementShort( "hadcb", "abcn.kaos.tof.fdet[%d].bottom.charge", nele=30 );
    // Taggers
    SetElementShort( "tagtdct", "abcn.kaos.tof.gdet[%d].top.time",      nele=2, ele0=28 );
    SetElementShort( "tagadct", "abcn.kaos.tof.gdet[%d].top.charge",    nele=2, ele0=28 );
    SetElementShort( "tagtdcb", "abcn.kaos.tof.gdet[%d].bottom.time",   nele=2, ele0=28 );
    SetElementShort( "tagadcb", "abcn.kaos.tof.gdet[%d].bottom.charge", nele=2, ele0=28 );
    // Aerogel
    SetElementShort( "acadct", "abcn.kaos.aerogel.segment[%d].top.charge",    nele=6 );
    SetElementShort( "acadcb", "abcn.kaos.aerogel.segment[%d].bottom.charge", nele=6 );
    // MWPC
    SetElementVector( "mwpclx", "abcn.kaos.mwpc.l.x[%d]", nele=240 );
    SetElementVector( "mwpcly", "abcn.kaos.mwpc.l.y[%d]", nele=80 );
    SetElementVector( "mwpcmx", "abcn.kaos.mwpc.m.x[%d]", nele=240 );
    SetElementVector( "mwpcmy", "abcn.kaos.mwpc.m.y[%d]", nele=80 );

    /**********/
    /* online */
    /**********/
    SetElementDouble( "utime",    "online.datataking" );
    SetElementDouble( "utime2",   "online.datataking2" );
    SetElementDouble( "actdc",    "online.ca_tdc" );
    SetElementDouble( "actime",   "online.cacorr" );
    SetElementDouble( "lumi",     "online.IntegratedLuminosity" );
    SetElementDouble( "vx",       "online.Vertex.x" );
    SetElementDouble( "vy",       "online.Vertex.y" );
    SetElementDouble( "vz",       "online.Vertex.z" );
    SetElementInt(    "helicity", "online.Beam.helicity" );

    /****************/
    /* SPECA online */
    /****************/
    SetElementInt(    "a_trigger",       "online.A.trigger" );
    SetElementInt(    "a_vdcok",         "online.A.vdcOK" );
    SetElementInt(    "a_tof_hits",      "online.A.ToF.hits" );
    SetElementInt(    "a_tof_paddle",    "online.A.ToF.paddle" );
    SetElementInt(    "a_tof_pattern",   "online.A.ToF.pattern" );
    SetElementDouble( "a_tof_scint",     "online.A.ToF.scint" );
    SetElementDouble( "a_tof_enel",      "online.A.ToF.Energy.left" );
    SetElementDouble( "a_tof_ener",      "online.A.ToF.Energy.right" );
    SetElementDouble( "a_tof_enel_corr", "online.A.ToF.Energy.left_corr" );
    SetElementDouble( "a_tof_ener_corr", "online.A.ToF.Energy.right_corr" );
    SetElementInt(    "a_de_hits",       "online.A.dE.hits" );
    SetElementInt(    "a_de_paddle",     "online.A.dE.paddle" );
    SetElementInt(    "a_de_pattern",    "online.A.dE.pattern" );
    SetElementDouble( "a_de_scint",      "online.A.dE.scint" );
    SetElementDouble( "a_de_enel",       "online.A.dE.Energy.left" );
    SetElementDouble( "a_de_ener",       "online.A.dE.Energy.right" );
    SetElementDouble( "a_de_enel_corr",  "online.A.dE.Energy.left_corr" );
    SetElementDouble( "a_de_ener_corr",  "online.A.dE.Energy.right_corr" );
    SetElementShort(  "a_cere_hit",      "online.A.Cerenkov.hit" );
    SetElementDouble( "a_cere_sum",      "online.A.Cerenkov.sum" );
    SetElementShort(  "a_cere_pattern",  "online.A.Cerenkov.pattern" );
    SetElementInt(    "a_x1_multi",      "online.A.x1.multi" );
    SetElementInt(    "a_x1_error",      "online.A.x1.error" );
    SetElementInt(    "a_x1_pattern",    "online.A.x1.pattern" );
    SetElementDouble( "a_x1_x",          "online.A.x1.x" );
    SetElementDouble( "a_x1_slope",      "online.A.x1.slope" );
    SetElementDouble( "a_x1_chi",        "online.A.x1.chi_sqr" );
    SetElementInt(    "a_s1_multi",      "online.A.s1.multi" );
    SetElementInt(    "a_s1_error",      "online.A.s1.error" );
    SetElementInt(    "a_s1_pattern",    "online.A.s1.pattern" );
    SetElementDouble( "a_s1_x",          "online.A.s1.x" );
    SetElementDouble( "a_s1_slope",      "online.A.s1.slope" );
    SetElementDouble( "a_s1_chi",        "online.A.s1.chi_sqr" );
    SetElementInt(    "a_x2_multi",      "online.A.x2.multi" );
    SetElementInt(    "a_x2_error",      "online.A.x2.error" );
    SetElementInt(    "a_x2_pattern",    "online.A.x2.pattern" );
    SetElementDouble( "a_x2_x",          "online.A.x2.x" );
    SetElementDouble( "a_x2_slope",      "online.A.x2.slope" );
    SetElementDouble( "a_x2_chi",        "online.A.x2.chi_sqr" );
    SetElementInt(    "a_s2_multi",      "online.A.s2.multi" );
    SetElementInt(    "a_s2_error",      "online.A.s2.error" );
    SetElementInt(    "a_s2_pattern",    "online.A.s2.pattern" );
    SetElementDouble( "a_s2_x",          "online.A.s2.x" );
    SetElementDouble( "a_s2_slope",      "online.A.s2.slope" );
    SetElementDouble( "a_s2_chi",        "online.A.s2.chi_sqr" );
    SetElementDouble( "a_x_scint",       "online.A.x_scint" );
    SetElementDouble( "a_y_scint",       "online.A.y_scint" );
    SetElementDouble( "a_timeattarget",  "online.A.TimeAtTarget" );
    
    /****************/
    /* SPECB online */
    /****************/
    SetElementInt(    "b_trigger",       "online.B.trigger" );
    SetElementInt(    "b_vdcok",         "online.B.vdcOK" );
    SetElementInt(    "b_tof_hits",      "online.B.ToF.hits" );
    SetElementInt(    "b_tof_paddle",    "online.B.ToF.paddle" );
    SetElementInt(    "b_tof_pattern",   "online.B.ToF.pattern" );
    SetElementDouble( "b_tof_scint",     "online.B.ToF.scint" );
    SetElementDouble( "b_tof_enel",      "online.B.ToF.Energy.left" );
    SetElementDouble( "b_tof_ener",      "online.B.ToF.Energy.right" );
    SetElementDouble( "b_tof_enel_corr", "online.B.ToF.Energy.left_corr" );
    SetElementDouble( "b_tof_ener_corr", "online.B.ToF.Energy.right_corr" );
    SetElementInt(    "b_de_hits",       "online.B.dE.hits" );
    SetElementInt(    "b_de_paddle",     "online.B.dE.paddle" );
    SetElementInt(    "b_de_pattern",    "online.B.dE.pattern" );
    SetElementDouble( "b_de_scint",      "online.B.dE.scint" );
    SetElementDouble( "b_de_enel",       "online.B.dE.Energy.left" );
    SetElementDouble( "b_de_ener",       "online.B.dE.Energy.right" );
    SetElementDouble( "b_de_enel_corr",  "online.B.dE.Energy.left_corr" );
    SetElementDouble( "b_de_ener_corr",  "online.B.dE.Energy.right_corr" );
    SetElementShort(  "b_cere_hit",      "online.B.Cerenkov.hit" );
    SetElementDouble( "b_cere_sum",      "online.B.Cerenkov.sum" );
    SetElementShort(  "b_cere_pattern",  "online.B.Cerenkov.pattern" );
    SetElementInt(    "b_x1_multi",      "online.B.x1.multi" );
    SetElementInt(    "b_x1_error",      "online.B.x1.error" );
    SetElementInt(    "b_x1_pattern",    "online.B.x1.pattern" );
    SetElementDouble( "b_x1_x",          "online.B.x1.x" );
    SetElementDouble( "b_x1_slope",      "online.B.x1.slope" );
    SetElementDouble( "b_x1_chi",        "online.B.x1.chi_sqr" );
    SetElementInt(    "b_s1_multi",      "online.B.s1.multi" );
    SetElementInt(    "b_s1_error",      "online.B.s1.error" );
    SetElementInt(    "b_s1_pattern",    "online.B.s1.pattern" );
    SetElementDouble( "b_s1_x",          "online.B.s1.x" );
    SetElementDouble( "b_s1_slope",      "online.B.s1.slope" );
    SetElementDouble( "b_s1_chi",        "online.B.s1.chi_sqr" );
    SetElementInt(    "b_x2_multi",      "online.B.x2.multi" );
    SetElementInt(    "b_x2_error",      "online.B.x2.error" );
    SetElementInt(    "b_x2_pattern",    "online.B.x2.pattern" );
    SetElementDouble( "b_x2_x",          "online.B.x2.x" );
    SetElementDouble( "b_x2_slope",      "online.B.x2.slope" );
    SetElementDouble( "b_x2_chi",        "online.B.x2.chi_sqr" );
    SetElementInt(    "b_s2_multi",      "online.B.s2.multi" );
    SetElementInt(    "b_s2_error",      "online.B.s2.error" );
    SetElementInt(    "b_s2_pattern",    "online.B.s2.pattern" );
    SetElementDouble( "b_s2_x",          "online.B.s2.x" );
    SetElementDouble( "b_s2_slope",      "online.B.s2.slope" );
    SetElementDouble( "b_s2_chi",        "online.B.s2.chi_sqr" );
    SetElementDouble( "b_x_scint",       "online.B.x_scint" );
    SetElementDouble( "b_y_scint",       "online.B.y_scint" );
    SetElementDouble( "b_timeattarget",  "online.B.TimeAtTarget" );
    
    /****************/
    /* SPECC online */
    /****************/
    SetElementInt(    "c_trigger",       "online.C.trigger" );
    SetElementInt(    "c_vdcok",         "online.C.vdcOK" );
    SetElementInt(    "c_tof_hits",      "online.C.ToF.hits" );
    SetElementInt(    "c_tof_paddle",    "online.C.ToF.paddle" );
    SetElementInt(    "c_tof_pattern",   "online.C.ToF.pattern" );
    SetElementDouble( "c_tof_scint",     "online.C.ToF.scint" );
    SetElementDouble( "c_tof_enel",      "online.C.ToF.Energy.left" );
    SetElementDouble( "c_tof_ener",      "online.C.ToF.Energy.right" );
    SetElementDouble( "c_tof_enel_corr", "online.C.ToF.Energy.left_corr" );
    SetElementDouble( "c_tof_ener_corr", "online.C.ToF.Energy.right_corr" );
    SetElementInt(    "c_de_hits",       "online.C.dE.hits" );
    SetElementInt(    "c_de_paddle",     "online.C.dE.paddle" );
    SetElementInt(    "c_de_pattern",    "online.C.dE.pattern" );
    SetElementDouble( "c_de_scint",      "online.C.dE.scint" );
    SetElementDouble( "c_de_enel",       "online.C.dE.Energy.left" );
    SetElementDouble( "c_de_ener",       "online.C.dE.Energy.right" );
    SetElementDouble( "c_de_enel_corr",  "online.C.dE.Energy.left_corr" );
    SetElementDouble( "c_de_ener_corr",  "online.C.dE.Energy.right_corr" );
    SetElementShort(  "c_cere_hit",      "online.C.Cerenkov.hit" );
    SetElementDouble( "c_cere_sum",      "online.C.Cerenkov.sum" );
    SetElementShort(  "c_cere_pattern",  "online.C.Cerenkov.pattern" );
    SetElementInt(    "c_x1_multi",      "online.C.x1.multi" );
    SetElementInt(    "c_x1_error",      "online.C.x1.error" );
    SetElementInt(    "c_x1_pattern",    "online.C.x1.pattern" );
    SetElementDouble( "c_x1_x",          "online.C.x1.x" );
    SetElementDouble( "c_x1_slope",      "online.C.x1.slope" );
    SetElementDouble( "c_x1_chi",        "online.C.x1.chi_sqr" );
    SetElementInt(    "c_s1_multi",      "online.C.s1.multi" );
    SetElementInt(    "c_s1_error",      "online.C.s1.error" );
    SetElementInt(    "c_s1_pattern",    "online.C.s1.pattern" );
    SetElementDouble( "c_s1_x",          "online.C.s1.x" );
    SetElementDouble( "c_s1_slope",      "online.C.s1.slope" );
    SetElementDouble( "c_s1_chi",        "online.C.s1.chi_sqr" );
    SetElementInt(    "c_x2_multi",      "online.C.x2.multi" );
    SetElementInt(    "c_x2_error",      "online.C.x2.error" );
    SetElementInt(    "c_x2_pattern",    "online.C.x2.pattern" );
    SetElementDouble( "c_x2_x",          "online.C.x2.x" );
    SetElementDouble( "c_x2_slope",      "online.C.x2.slope" );
    SetElementDouble( "c_x2_chi",        "online.C.x2.chi_sqr" );
    SetElementInt(    "c_s2_multi",      "online.C.s2.multi" );
    SetElementInt(    "c_s2_error",      "online.C.s2.error" );
    SetElementInt(    "c_s2_pattern",    "online.C.s2.pattern" );
    SetElementDouble( "c_s2_x",          "online.C.s2.x" );
    SetElementDouble( "c_s2_slope",      "online.C.s2.slope" );
    SetElementDouble( "c_s2_chi",        "online.C.s2.chi_sqr" );
    SetElementDouble( "c_x_scint",       "online.C.x_scint" );
    SetElementDouble( "c_y_scint",       "online.C.y_scint" );
    SetElementDouble( "c_timeattarget",  "online.C.TimeAtTarget" );
    
    /***************/
    /* KAOS online */
    /***************/
    // trigger
    SetElementInt(    "k_trigger",    "online.kaos.trigger" );
    SetElementDouble( "coinctime_k",  "online.kaos.CoincTimeAtTarget" );
    SetElementDouble( "tof_k",        "online.kaos.TimeAtTarget" );
    SetElementDouble( "coinc_time_H", "online.kaos.xtra.time[0]" );
    SetElementDouble( "coinc_time_G", "online.kaos.xtra.time[4]" );
    // TOF
    SetElementShort( "hmulti",   "online.kaos.hdet.clusterSize" );
    SetElementShort( "gmulti",   "online.kaos.gdet.clusterSize" );
    SetElementShort( "hcluster", "online.kaos.hdet.cluster" );
    SetElementShort( "gcluster", "online.kaos.gdet.cluster" );
    SetElementDouble( "hhit",    "online.kaos.hdet.positionXZ" );
    SetElementDouble( "ghit",    "online.kaos.gdet.positionXZ" );
    SetElementDouble( "hhity",   "online.kaos.hdet.positionY" );
    SetElementDouble( "ghity",   "online.kaos.gdet.positionY" );
    SetElementDouble( "hde",     "online.kaos.hdet.dE" );
    SetElementDouble( "gde",     "online.kaos.gdet.dE" );
    SetElementDouble( "htime",   "online.kaos.hdet.time" );
    SetElementDouble( "gtime",   "online.kaos.gdet.time" );
    // MWPC
    SetElementDouble( "theta",      "online.kaos.mwpc.theta" );
    SetElementDouble( "phi",        "online.kaos.mwpc.phi" );
    SetElementDouble( "quality",    "online.kaos.mwpc.quality" );
    SetElementDouble( "sndquality", "online.kaos.mwpc.sndquality" );
    SetElementDouble( "lxpos",      "online.kaos.mwpc.l.x.hit" );
    SetElementDouble( "lypos",      "online.kaos.mwpc.l.y.hit" );
    SetElementDouble( "mxpos",      "online.kaos.mwpc.m.x.hit" );
    SetElementDouble( "mypos",      "online.kaos.mwpc.m.y.hit" );
    
    // focal plane
    SetElementDouble( "k_fx",     "online.kaos.coord.x" );
    SetElementDouble( "k_fy",     "online.kaos.coord.y" );
    SetElementDouble( "k_ftheta", "online.kaos.coord.th" );
    SetElementDouble( "k_fphi",   "online.kaos.coord.ph" );
    // Matrix results
    SetElementDouble( "a_tdp",    "online.A.target.dp" );
    SetElementDouble( "a_ttheta", "online.A.target.th" );
    SetElementDouble( "a_ty0",    "online.A.target.y0" );
    SetElementDouble( "a_tphi",   "online.A.target.ph" );
    SetElementDouble( "a_tlen",   "online.A.target.len" );
    SetElementDouble( "b_tdp",    "online.B.target.dp" );
    SetElementDouble( "b_ttheta", "online.B.target.th" );
    SetElementDouble( "b_ty0",    "online.B.target.y0" );
    SetElementDouble( "b_tphi",   "online.B.target.ph" );
    SetElementDouble( "b_tlen",   "online.B.target.len" );
    SetElementDouble( "c_tdp",    "online.C.target.dp" );
    SetElementDouble( "c_ttheta", "online.C.target.th" );
    SetElementDouble( "c_ty0",    "online.C.target.y0" );
    SetElementDouble( "c_tphi",   "online.C.target.ph" );
    SetElementDouble( "c_tlen",   "online.C.target.len" );
    SetElementDouble( "k_tdp",    "online.kaos.target.dp" );
    SetElementDouble( "k_ttheta", "online.kaos.target.th" );
    SetElementDouble( "k_ty0",    "online.kaos.target.y0" );
    SetElementDouble( "k_tphi",   "online.kaos.target.ph" );
    SetElementDouble( "k_tlen",   "online.kaos.target.len" );

    // neither abcn nor online
    SetElementDouble( "a_fx" );
    SetElementDouble( "a_fy" );
    SetElementDouble( "a_ftheta" );
    SetElementDouble( "a_fphi" );
    SetElementDouble( "b_fx" );
    SetElementDouble( "b_fy" );
    SetElementDouble( "b_ftheta" );
    SetElementDouble( "b_fphi" );
    SetElementDouble( "c_fx" );
    SetElementDouble( "c_fy" );
    SetElementDouble( "c_ftheta" );
    SetElementDouble( "c_fphi" );
    //PrintElements();
  }
  else if( Use_ROOT==2 ){ // Hyper2012
    /**************/
    /* SPECA abcn */
    /**************/
    SetElementShort( "a_runtime",       "abcn.a.run.runtime" );
    SetElementShort( "a_realtime",      "abcn.a.run.realtime" );
    SetElementShort( "a_actime1",       "abcn.a.coinc.a_c_time_1" );
    SetElementShort( "a_actime2",       "abcn.a.coinc.a_c_time_2" );
    SetElementShort( "a_actime3",       "abcn.a.coinc.a_c_time_3" );
    SetElementInt(   "a_accoinc",       "abcn.a.coinc.a_c_coinc" );
    SetElementInt(   "a_cdead",         "abcn.a.coinc.dead_c" );
    SetElementInt(   "a_acdead",        "abcn.a.coinc.dead_ac" );
    SetElementShort( "a_beam_position", "abcn.a.beam.position" );

    /**************/
    /* SPECB abcn */
    /**************/
    SetElementShort( "b_runtime",       "abcn.b.run.runtime" );
    SetElementShort( "b_realtime",      "abcn.b.run.realtime" );
    SetElementShort( "b_abtime1",       "abcn.b.coinc.a_b_time_1" );
    SetElementShort( "b_abtime2",       "abcn.b.coinc.a_b_time_2" );
    SetElementShort( "b_abtime3",       "abcn.b.coinc.a_b_time_3" );
    SetElementShort( "b_actime1",       "abcn.b.coinc.a_c_time_1" );
    SetElementShort( "b_actime2",       "abcn.b.coinc.a_c_time_2" );
    SetElementShort( "b_actime3",       "abcn.b.coinc.a_c_time_3" );
    SetElementInt(   "b_accoinc",       "abcn.b.coinc.a_c_coinc" );
    SetElementInt(   "b_cdead",         "abcn.b.coinc.dead_c" );
    SetElementInt(   "b_acdead",        "abcn.b.coinc.dead_ac" );
    SetElementShort( "b_beam_position", "abcn.b.beam.position" );

    /**************/
    /* SPECC abcn */
    /**************/
    SetElementShort( "c_runtime",       "abcn.c.run.runtime" );
    SetElementShort( "c_realtime",      "abcn.c.run.realtime" );
    SetElementShort( "c_actime1",       "abcn.c.coinc.a_c_time_1" );
    SetElementShort( "c_actime2",       "abcn.c.coinc.a_c_time_2" );
    SetElementShort( "c_actime3",       "abcn.c.coinc.a_c_time_3" );
    SetElementInt(   "c_accoinc",       "abcn.c.coinc.a_c_coinc" );
    SetElementInt(   "c_cdead",         "abcn.c.coinc.dead_c" );
    SetElementInt(   "c_acdead",        "abcn.c.coinc.dead_ac" );
    SetElementShort( "c_beam_position", "abcn.c.beam.position" );

    /*************/
    /* KAOS abcn */
    /*************/
    SetElementShort( "label",      "abcn.kaos.label" );
    SetElementShort( "k_runtime",  "abcn.kaos.run.runtime" );
    SetElementShort( "k_realtime", "abcn.kaos.run.realtime" );
    SetElementInt(   "kdead",      "abcn.kaos.dead_k" );
    SetElementInt(   "cdead",      "abcn.kaos.dead_c" );
    SetElementInt(   "kcdead",     "abcn.kaos.dead_kc" );
    // TOF
    SetElementShort( "gtdct", "abcn.kaos.tof.gdet[%d].top.time",      nele=30 );
    SetElementShort( "gadct", "abcn.kaos.tof.gdet[%d].top.charge",    nele=30 );
    SetElementShort( "gtdcb", "abcn.kaos.tof.gdet[%d].bottom.time",   nele=30 );
    SetElementShort( "gadcb", "abcn.kaos.tof.gdet[%d].bottom.charge", nele=30 );
    SetElementShort( "htdct", "abcn.kaos.tof.fdet[%d].top.time",      nele=30 );
    SetElementShort( "hadct", "abcn.kaos.tof.fdet[%d].top.charge",    nele=30 );
    SetElementShort( "htdcb", "abcn.kaos.tof.fdet[%d].bottom.time",   nele=30 );
    SetElementShort( "hadcb", "abcn.kaos.tof.fdet[%d].bottom.charge", nele=30 );
    // Aerogel
    SetElementShort( "acadct", "abcn.kaos.aerogel.segment[%d].top.charge",    nele=6 );
    SetElementShort( "acadcb", "abcn.kaos.aerogel.segment[%d].bottom.charge", nele=6 );
    SetElementShort( "actdct", "abcn.kaos.aerogel.segment[%d].top.time",      nele=6 );
    SetElementShort( "actdcb", "abcn.kaos.aerogel.segment[%d].bottom.time",   nele=6 );
    // Aerogel2
    SetElementShort( "ac2adct", "abcn.kaos.aerogel.segment[%d].top.charge",    nele=6, ele0=6 );
    SetElementShort( "ac2adcb", "abcn.kaos.aerogel.segment[%d].bottom.charge", nele=6, ele0=6 );
    SetElementShort( "ac2tdct", "abcn.kaos.aerogel.segment[%d].top.time",      nele=6, ele0=6 );
    SetElementShort( "ac2tdcb", "abcn.kaos.aerogel.segment[%d].bottom.time",   nele=6, ele0=6 );

    /**********/
    /* online */
    /**********/
    SetElementDouble( "utime",     "online.datataking" );
    SetElementDouble( "utime2",    "online.datataking2" );
    SetElementDouble( "actdc",     "online.ca_tdc" );
    SetElementDouble( "actime",    "online.cacorr" );
    SetElementDouble( "lumi",      "online.IntegratedLuminosity" );
    SetElementDouble( "vx",        "online.Vertex.x" );
    SetElementDouble( "vy",        "online.Vertex.y" );
    SetElementDouble( "vz",        "online.Vertex.z" );
    SetElementInt(    "helicity",  "online.Beam.helicity" );

    /****************/
    /* SPECA online */
    /****************/
    SetElementInt(    "a_trigger",       "online.A.trigger" );
    SetElementInt(    "a_vdcok",         "online.A.vdcOK" );
    SetElementInt(    "a_tof_hits",      "online.A.ToF.hits" );
    SetElementInt(    "a_tof_paddle",    "online.A.ToF.paddle" );
    SetElementInt(    "a_tof_pattern",   "online.A.ToF.pattern" );
    SetElementDouble( "a_tof_scint",     "online.A.ToF.scint" );
    SetElementDouble( "a_tof_enel",      "online.A.ToF.Energy.left" );
    SetElementDouble( "a_tof_ener",      "online.A.ToF.Energy.right" );
    SetElementDouble( "a_tof_enel_corr", "online.A.ToF.Energy.left_corr" );
    SetElementDouble( "a_tof_ener_corr", "online.A.ToF.Energy.right_corr" );
    SetElementInt(    "a_de_hits",       "online.A.dE.hits" );
    SetElementInt(    "a_de_paddle",     "online.A.dE.paddle" );
    SetElementInt(    "a_de_pattern",    "online.A.dE.pattern" );
    SetElementDouble( "a_de_scint",      "online.A.dE.scint" );
    SetElementDouble( "a_de_enel",       "online.A.dE.Energy.left" );
    SetElementDouble( "a_de_ener",       "online.A.dE.Energy.right" );
    SetElementDouble( "a_de_enel_corr",  "online.A.dE.Energy.left_corr" );
    SetElementDouble( "a_de_ener_corr",  "online.A.dE.Energy.right_corr" );
    SetElementShort(  "a_cere_hit",      "online.A.Cerenkov.hit" );
    SetElementDouble( "a_cere_sum",      "online.A.Cerenkov.sum" );
    SetElementShort(  "a_cere_pattern",  "online.A.Cerenkov.pattern" );
    SetElementShort(  "a_cere_de",      ".a.det.trigger.cerenkov.mirror[%d].energy", nele=12);
    SetElementInt(    "a_x1_multi",      "online.A.x1.multi" );
    SetElementInt(    "a_x1_error",      "online.A.x1.error" );
    SetElementInt(    "a_x1_pattern",    "online.A.x1.pattern" );
    SetElementDouble( "a_x1_x",          "online.A.x1.x" );
    SetElementDouble( "a_x1_slope",      "online.A.x1.slope" );
    SetElementDouble( "a_x1_chi",        "online.A.x1.chi_sqr" );
    SetElementInt(    "a_s1_multi",      "online.A.s1.multi" );
    SetElementInt(    "a_s1_error",      "online.A.s1.error" );
    SetElementInt(    "a_s1_pattern",    "online.A.s1.pattern" );
    SetElementDouble( "a_s1_x",          "online.A.s1.x" );
    SetElementDouble( "a_s1_slope",      "online.A.s1.slope" );
    SetElementDouble( "a_s1_chi",        "online.A.s1.chi_sqr" );
    SetElementInt(    "a_x2_multi",      "online.A.x2.multi" );
    SetElementInt(    "a_x2_error",      "online.A.x2.error" );
    SetElementInt(    "a_x2_pattern",    "online.A.x2.pattern" );
    SetElementDouble( "a_x2_x",          "online.A.x2.x" );
    SetElementDouble( "a_x2_slope",      "online.A.x2.slope" );
    SetElementDouble( "a_x2_chi",        "online.A.x2.chi_sqr" );
    SetElementInt(    "a_s2_multi",      "online.A.s2.multi" );
    SetElementInt(    "a_s2_error",      "online.A.s2.error" );
    SetElementInt(    "a_s2_pattern",    "online.A.s2.pattern" );
    SetElementDouble( "a_s2_x",          "online.A.s2.x" );
    SetElementDouble( "a_s2_slope",      "online.A.s2.slope" );
    SetElementDouble( "a_s2_chi",        "online.A.s2.chi_sqr" );
    SetElementDouble( "a_x_scint",       "online.A.x_scint" );
    SetElementDouble( "a_y_scint",       "online.A.y_scint" );
    SetElementDouble( "a_timeattarget",  "online.A.TimeAtTarget" );
    SetElementDouble( "a_vx",            "online.A.vertex.x" );
    SetElementDouble( "a_vy",            "online.A.vertex.y" );
    SetElementDouble( "a_vz",            "online.A.vertex.z" );
    SetElementDouble( "a_fx" ,           "online.A.x" );
    SetElementDouble( "a_fy" ,           "online.A.y" );
    SetElementDouble( "a_ftheta" ,       "online.A.theta" );
    SetElementDouble( "a_fphi" ,         "online.A.phi" );
    SetElementDouble( "a_fdx" ,          "online.A.dx" );
    SetElementDouble( "a_fdy" ,          "online.A.dy" );
    SetElementDouble( "a_fdtheta" ,      "online.A.dtheta" );
    SetElementDouble( "a_fdphi" ,        "online.A.dphi" );
    SetElementDouble( "a_tdp",           "online.A.target.dp" );
    SetElementDouble( "a_ttheta",        "online.A.target.th" );
    SetElementDouble( "a_ty0",           "online.A.target.y0" );
    SetElementDouble( "a_tphi",          "online.A.target.ph" );
    SetElementDouble( "a_tlen",          "online.A.target.len" );
    SetElementDouble( "a_eloss",         "online.A.Eloss_corr" );
    SetElementDouble( "a_mom",           "online.A.Momentum" );
 
    /****************/
    /* SPECB online */
    /****************/
    SetElementInt(    "b_trigger",       "online.B.trigger" );
    SetElementInt(    "b_vdcok",         "online.B.vdcOK" );
    SetElementInt(    "b_tof_hits",      "online.B.ToF.hits" );
    SetElementInt(    "b_tof_paddle",    "online.B.ToF.paddle" );
    SetElementInt(    "b_tof_pattern",   "online.B.ToF.pattern" );
    SetElementDouble( "b_tof_scint",     "online.B.ToF.scint" );
    SetElementDouble( "b_tof_enel",      "online.B.ToF.Energy.left" );
    SetElementDouble( "b_tof_ener",      "online.B.ToF.Energy.right" );
    SetElementDouble( "b_tof_enel_corr", "online.B.ToF.Energy.left_corr" );
    SetElementDouble( "b_tof_ener_corr", "online.B.ToF.Energy.right_corr" );
    SetElementInt(    "b_de_hits",       "online.B.dE.hits" );
    SetElementInt(    "b_de_paddle",     "online.B.dE.paddle" );
    SetElementInt(    "b_de_pattern",    "online.B.dE.pattern" );
    SetElementDouble( "b_de_scint",      "online.B.dE.scint" );
    SetElementDouble( "b_de_enel",       "online.B.dE.Energy.left" );
    SetElementDouble( "b_de_ener",       "online.B.dE.Energy.right" );
    SetElementDouble( "b_de_enel_corr",  "online.B.dE.Energy.left_corr" );
    SetElementDouble( "b_de_ener_corr",  "online.B.dE.Energy.right_corr" );
    SetElementShort(  "b_cere_hit",      "online.B.Cerenkov.hit" );
    SetElementDouble( "b_cere_sum",      "online.B.Cerenkov.sum" );
    SetElementShort(  "b_cere_pattern",  "online.B.Cerenkov.pattern" );
    SetElementShort(  "b_cere_de",       ".b.det.trigger.cerenkov.mirror[%d].energy", nele=5);
    SetElementInt(    "b_x1_multi",      "online.B.x1.multi" );
    SetElementInt(    "b_x1_error",      "online.B.x1.error" );
    SetElementInt(    "b_x1_pattern",    "online.B.x1.pattern" );
    SetElementDouble( "b_x1_x",          "online.B.x1.x" );
    SetElementDouble( "b_x1_slope",      "online.B.x1.slope" );
    SetElementDouble( "b_x1_chi",        "online.B.x1.chi_sqr" );
    SetElementInt(    "b_s1_multi",      "online.B.s1.multi" );
    SetElementInt(    "b_s1_error",      "online.B.s1.error" );
    SetElementInt(    "b_s1_pattern",    "online.B.s1.pattern" );
    SetElementDouble( "b_s1_x",          "online.B.s1.x" );
    SetElementDouble( "b_s1_slope",      "online.B.s1.slope" );
    SetElementDouble( "b_s1_chi",        "online.B.s1.chi_sqr" );
    SetElementInt(    "b_x2_multi",      "online.B.x2.multi" );
    SetElementInt(    "b_x2_error",      "online.B.x2.error" );
    SetElementInt(    "b_x2_pattern",    "online.B.x2.pattern" );
    SetElementDouble( "b_x2_x",          "online.B.x2.x" );
    SetElementDouble( "b_x2_slope",      "online.B.x2.slope" );
    SetElementDouble( "b_x2_chi",        "online.B.x2.chi_sqr" );
    SetElementInt(    "b_s2_multi",      "online.B.s2.multi" );
    SetElementInt(    "b_s2_error",      "online.B.s2.error" );
    SetElementInt(    "b_s2_pattern",    "online.B.s2.pattern" );
    SetElementDouble( "b_s2_x",          "online.B.s2.x" );
    SetElementDouble( "b_s2_slope",      "online.B.s2.slope" );
    SetElementDouble( "b_s2_chi",        "online.B.s2.chi_sqr" );
    SetElementDouble( "b_x_scint",       "online.B.x_scint" );
    SetElementDouble( "b_y_scint",       "online.B.y_scint" );
    SetElementDouble( "b_timeattarget",  "online.B.TimeAtTarget" );
    SetElementDouble( "b_vx",            "online.B.vertex.x" );
    SetElementDouble( "b_vy",            "online.B.vertex.y" );
    SetElementDouble( "b_vz",            "online.B.vertex.z" );
    SetElementDouble( "b_fx" ,           "online.B.x" );
    SetElementDouble( "b_fy" ,           "online.B.y" );
    SetElementDouble( "b_ftheta" ,       "online.B.theta" );
    SetElementDouble( "b_fphi" ,         "online.B.phi" );
    SetElementDouble( "b_fdx" ,          "online.B.dx" );
    SetElementDouble( "b_fdy" ,          "online.B.dy" );
    SetElementDouble( "b_fdtheta" ,      "online.B.dtheta" );
    SetElementDouble( "b_fdphi" ,        "online.B.dphi" );
    
    /****************/
    /* SPECC online */
    /****************/
    SetElementInt(    "c_trigger",       "online.C.trigger" );
    SetElementInt(    "c_vdcok",         "online.C.vdcOK" );
    SetElementInt(    "c_tof_hits",      "online.C.ToF.hits" );
    SetElementInt(    "c_tof_paddle",    "online.C.ToF.paddle" );
    SetElementInt(    "c_tof_pattern",   "online.C.ToF.pattern" );
    SetElementDouble( "c_tof_scint",     "online.C.ToF.scint" );
    SetElementDouble( "c_tof_enel",      "online.C.ToF.Energy.left" );
    SetElementDouble( "c_tof_ener",      "online.C.ToF.Energy.right" );
    SetElementDouble( "c_tof_enel_corr", "online.C.ToF.Energy.left_corr" );
    SetElementDouble( "c_tof_ener_corr", "online.C.ToF.Energy.right_corr" );
    SetElementInt(    "c_de_hits",       "online.C.dE.hits" );
    SetElementInt(    "c_de_paddle",     "online.C.dE.paddle" );
    SetElementInt(    "c_de_pattern",    "online.C.dE.pattern" );
    SetElementDouble( "c_de_scint",      "online.C.dE.scint" );
    SetElementDouble( "c_de_enel",       "online.C.dE.Energy.left" );
    SetElementDouble( "c_de_ener",       "online.C.dE.Energy.right" );
    SetElementDouble( "c_de_enel_corr",  "online.C.dE.Energy.left_corr" );
    SetElementDouble( "c_de_ener_corr",  "online.C.dE.Energy.right_corr" );
    SetElementShort(  "c_cere_hit",      "online.C.Cerenkov.hit" );
    SetElementDouble( "c_cere_sum",      "online.C.Cerenkov.sum" );
    SetElementShort(  "c_cere_pattern",  "online.C.Cerenkov.pattern" );
    SetElementShort(  "c_cere_de",       ".c.det.trigger.cerenkov.mirror[%d].energy", nele=12);
    SetElementInt(    "c_x1_multi",      "online.C.x1.multi" );
    SetElementInt(    "c_x1_error",      "online.C.x1.error" );
    SetElementInt(    "c_x1_pattern",    "online.C.x1.pattern" );
    SetElementDouble( "c_x1_x",          "online.C.x1.x" );
    SetElementDouble( "c_x1_slope",      "online.C.x1.slope" );
    SetElementDouble( "c_x1_chi",        "online.C.x1.chi_sqr" );
    SetElementInt(    "c_s1_multi",      "online.C.s1.multi" );
    SetElementInt(    "c_s1_error",      "online.C.s1.error" );
    SetElementInt(    "c_s1_pattern",    "online.C.s1.pattern" );
    SetElementDouble( "c_s1_x",          "online.C.s1.x" );
    SetElementDouble( "c_s1_slope",      "online.C.s1.slope" );
    SetElementDouble( "c_s1_chi",        "online.C.s1.chi_sqr" );
    SetElementInt(    "c_x2_multi",      "online.C.x2.multi" );
    SetElementInt(    "c_x2_error",      "online.C.x2.error" );
    SetElementInt(    "c_x2_pattern",    "online.C.x2.pattern" );
    SetElementDouble( "c_x2_x",          "online.C.x2.x" );
    SetElementDouble( "c_x2_slope",      "online.C.x2.slope" );
    SetElementDouble( "c_x2_chi",        "online.C.x2.chi_sqr" );
    SetElementInt(    "c_s2_multi",      "online.C.s2.multi" );
    SetElementInt(    "c_s2_error",      "online.C.s2.error" );
    SetElementInt(    "c_s2_pattern",    "online.C.s2.pattern" );
    SetElementDouble( "c_s2_x",          "online.C.s2.x" );
    SetElementDouble( "c_s2_slope",      "online.C.s2.slope" );
    SetElementDouble( "c_s2_chi",        "online.C.s2.chi_sqr" );
    SetElementDouble( "c_x_scint",       "online.C.x_scint" );
    SetElementDouble( "c_y_scint",       "online.C.y_scint" );
    SetElementDouble( "c_timeattarget",  "online.C.TimeAtTarget" );
    SetElementDouble( "c_vx",            "online.C.vertex.x" );
    SetElementDouble( "c_vy",            "online.C.vertex.y" );
    SetElementDouble( "c_vz",            "online.C.vertex.z" );
    SetElementDouble( "c_fx" ,           "online.C.x" );
    SetElementDouble( "c_fy" ,           "online.C.y" );
    SetElementDouble( "c_ftheta" ,       "online.C.theta" );
    SetElementDouble( "c_fphi" ,         "online.C.phi" );
    SetElementDouble( "c_fdx" ,          "online.C.dx" );
    SetElementDouble( "c_fdy" ,          "online.C.dy" );
    SetElementDouble( "c_fdtheta" ,      "online.C.dtheta" );
    SetElementDouble( "c_fdphi" ,        "online.C.dphi" );
    SetElementDouble( "c_tdp",           "online.C.target.dp" );
    SetElementDouble( "c_ttheta",        "online.C.target.th" );
    SetElementDouble( "c_ty0",           "online.C.target.y0" );
    SetElementDouble( "c_tphi",          "online.C.target.ph" );
    SetElementDouble( "c_tlen",          "online.C.target.len" );
    SetElementDouble( "c_eloss",         "online.C.Eloss_corr" );
    SetElementDouble( "c_mom",           "online.C.Momentum" );
    
    /***************/
    /* Kaos Online */
    /***************/
    // trigger
    SetElementInt(    "k_trigger",    "online.kaos.trigger" );
    SetElementDouble( "coinctime_k",  "online.kaos.CoincTimeAtTarget" );
    SetElementDouble( "tof_k",        "online.kaos.TimeAtTarget" );
    SetElementDouble( "coinc_time_H", "online.kaos.xtra.time[0]" );
    SetElementDouble( "coinc_time_G", "online.kaos.xtra.time[4]" );
    // TOF
    SetElementShort(  "hmulti",    "online.kaos.hdet.clusterSize" );
    SetElementShort(  "gmulti",    "online.kaos.gdet.clusterSize" );
    SetElementShort(  "imulti",    "online.kaos.idet.clusterSize" );
    SetElementShort(  "hcluster",  "online.kaos.hdet.cluster" );
    SetElementShort(  "gcluster",  "online.kaos.gdet.cluster" );
    SetElementShort(  "icluster",  "online.kaos.idet.cluster" );
    SetElementDouble( "hhit",      "online.kaos.hdet.paddle" );
    SetElementDouble( "ghit",      "online.kaos.gdet.paddle" );
    SetElementDouble( "ihit",      "online.kaos.idet.paddle" );
    SetElementDouble( "hhitx",     "online.kaos.hdet.positionXZ" );
    SetElementDouble( "ghitx",     "online.kaos.gdet.positionXZ" );
    SetElementDouble( "ihitx",     "online.kaos.idet.positionXZ" );
    SetElementDouble( "hhity",     "online.kaos.hdet.positionY" );
    SetElementDouble( "ghity",     "online.kaos.gdet.positionY" );
    SetElementDouble( "ihity",     "online.kaos.idet.positionY" );
    SetElementDouble( "hde",       "online.kaos.hdet.dE" );
    SetElementDouble( "gde",       "online.kaos.gdet.dE" );
    SetElementDouble( "ide",       "online.kaos.idet.dE" );
    SetElementDouble( "hdedx",     "online.kaos.hdet.dEdx" );
    SetElementDouble( "gdedx",     "online.kaos.gdet.dEdx" );
    SetElementDouble( "idedx",     "online.kaos.idet.dEdx" );
    SetElementDouble( "htime",     "online.kaos.hdet.time" );
    SetElementDouble( "gtime",     "online.kaos.gdet.time" );
    SetElementDouble( "itime",     "online.kaos.idet.time" );
    SetElementDouble( "tof1m",     "online.kaos.tof.flighttime" );
    SetElementDouble( "toflength", "online.kaos.tof.tracklength" );
    SetElementDouble( "k_fx",      "online.kaos.coord.x" );
    SetElementDouble( "k_fy",      "online.kaos.coord.y" );
    SetElementDouble( "k_ftheta",  "online.kaos.coord.th" );
    SetElementDouble( "k_fphi",    "online.kaos.coord.ph" );
    SetElementDouble( "k_tdp",     "online.kaos.target.dp" );
    SetElementDouble( "k_ttheta",  "online.kaos.target.th" );
    SetElementDouble( "k_ty0",     "online.kaos.target.y0" );
    SetElementDouble( "k_tphi",    "online.kaos.target.ph" );
    SetElementDouble( "k_tlen",    "online.kaos.target.len" );
    SetElementDouble( "k_eloss",   "online.kaos.Eloss_corr" );
    SetElementDouble( "k_mom",     "online.kaos.Momentum" );
    //PrintElements();
  } // Use_ROOT==2 for hyper2012 & hyper2014
  else if( Use_ROOT==99 ){ // NC
    SetElementShort( "c_runtime",  "abcn.c.run.runtime" );
    SetElementShort( "c_realtime", "abcn.c.run.realtime" );
    SetElementShort( "c_actime1",  "abcn.c.coinc.a_c_time_1" );
    SetElementShort( "c_actime2",  "abcn.c.coinc.a_c_time_2" );
    SetElementShort( "c_actime3",  "abcn.c.coinc.a_c_time_3" );
    SetElementInt(   "c_accoinc",  "abcn.c.coinc.a_c_coinc" );
    SetElementInt(   "c_cdead",    "abcn.c.coinc.dead_c" );
    SetElementInt(   "c_acdead",   "abcn.c.coinc.dead_ac" );
    SetElementInt(   "c_x1",       "abcn.c.det.vdc.x1[%d]", 448 );
    SetElementInt(   "c_s1",       "abcn.c.det.vdc.s1[%d]", 352 );
    SetElementInt(   "c_x2",       "abcn.c.det.vdc.x2[%d]", 448 );
    SetElementInt(   "c_s2",       "abcn.c.det.vdc.s2[%d]", 352 );
    SetElementInt(   "c_tofadcl",  "abcn.c.det.trigger.scint.tof.pad[%d].left.energy", 15 );
    SetElementInt(   "c_tofadcr",  "abcn.c.det.trigger.scint.tof.pad[%d].right.energy", 15 );
    SetElementInt(   "c_deadcl",   "abcn.c.det.trigger.scint.de.pad[%d].left.energy", 15 );
    SetElementInt(   "c_deadcr",   "abcn.c.det.trigger.scint.de.pad[%d].right.energy", 15 );
    SetElementInt(   "nc1t",       "abcn.c.det.trigger.scint.de_tof_time" );
    SetElementInt(   "nc2t",       "abcn.c.coinc.a_c_time_1" );
    SetElementInt(   "nc3t",       "abcn.c.coinc.b_c_time_1" );
    SetElementInt(   "nc1a",       "abcn.c.det.trigger.cerenkov.mirror[0].energy" );
    SetElementInt(   "nc2a",       "abcn.c.det.trigger.cerenkov.mirror[1].energy" );
    SetElementInt(   "nc3a",       "abcn.c.det.trigger.cerenkov.mirror[5].energy" );
    SetElementDouble( "utime",     "online.datataking" );
    SetElementDouble( "utime2",    "online.datataking2" );
    SetElementDouble( "lumi",      "online.IntegratedLuminosity" );
    
    SetElementInt( "c_trigger",          "online.C.trigger" );
    SetElementInt( "c_vdcok",            "online.C.vdcOK" );
    SetElementInt( "c_tof_hits",         "online.C.ToF.hits" );
    SetElementInt( "c_tof_paddle",       "online.C.ToF.paddle" );
    SetElementInt( "c_tof_pattern",      "online.C.ToF.pattern" );
    SetElementDouble( "c_tof_scint",     "online.C.ToF.scint" );
    SetElementDouble( "c_tof_enel",      "online.C.ToF.Energy.left" );
    SetElementDouble( "c_tof_ener",      "online.C.ToF.Energy.right" );
    SetElementDouble( "c_tof_enel_corr", "online.C.ToF.Energy.left_corr" );
    SetElementDouble( "c_tof_ener_corr", "online.C.ToF.Energy.right_corr" );
    SetElementInt( "c_de_hits",          "online.C.dE.hits" );
    SetElementInt( "c_de_paddle",        "online.C.dE.paddle" );
    SetElementInt( "c_de_pattern",       "online.C.dE.pattern" );
    SetElementDouble( "c_de_scint",      "online.C.dE.scint" );
    SetElementDouble( "c_de_enel",       "online.C.dE.Energy.left" );
    SetElementDouble( "c_de_ener",       "online.C.dE.Energy.right" );
    SetElementDouble( "c_de_enel_corr",  "online.C.dE.Energy.left_corr" );
    SetElementDouble( "c_de_ener_corr",  "online.C.dE.Energy.right_corr" );
    SetElementInt( "c_x1_multi",         "online.C.x1.multi" );
    SetElementInt( "c_x1_error",         "online.C.x1.error" );
    SetElementInt( "c_x1_pattern",       "online.C.x1.pattern" );
    SetElementDouble( "c_x1_x",          "online.C.x1.x" );
    SetElementDouble( "c_x1_slope",      "online.C.x1.slope" );
    SetElementDouble( "c_x1_chi",        "online.C.x1.chi_sqr" );
    SetElementInt( "c_s1_multi",         "online.C.s1.multi" );
    SetElementInt( "c_s1_error",         "online.C.s1.error" );
    SetElementInt( "c_s1_pattern",       "online.C.s1.pattern" );
    SetElementDouble( "c_s1_x",          "online.C.s1.x" );
    SetElementDouble( "c_s1_slope",      "online.C.s1.slope" );
    SetElementDouble( "c_s1_chi",        "online.C.s1.chi_sqr" );
    SetElementInt( "c_x2_multi",         "online.C.x2.multi" );
    SetElementInt( "c_x2_error",         "online.C.x2.error" );
    SetElementInt( "c_x2_pattern",       "online.C.x2.pattern" );
    SetElementDouble( "c_x2_x",          "online.C.x2.x" );
    SetElementDouble( "c_x2_slope",      "online.C.x2.slope" );
    SetElementDouble( "c_x2_chi",        "online.C.x2.chi_sqr" );
    SetElementInt( "c_s2_multi",         "online.C.s2.multi" );
    SetElementInt( "c_s2_error",         "online.C.s2.error" );
    SetElementInt( "c_s2_pattern",       "online.C.s2.pattern" );
    SetElementDouble( "c_s2_x",          "online.C.s2.x" );
    SetElementDouble( "c_s2_slope",      "online.C.s2.slope" );
    SetElementDouble( "c_s2_chi",        "online.C.s2.chi_sqr" );
    SetElementDouble( "c_x_scint",       "online.C.x_scint" );
    SetElementDouble( "c_y_scint",       "online.C.y_scint" );
    SetElementDouble( "c_timeattarget",  "online.C.TimeAtTarget" );

    SetElementDouble( "c_tdp",    "online.C.target.dp" );
    SetElementDouble( "c_ttheta", "online.C.target.th" );
    SetElementDouble( "c_ty0",    "online.C.target.y0" );
    SetElementDouble( "c_tphi",   "online.C.target.ph" );
    SetElementDouble( "c_tlen",   "online.C.target.len" );

    SetElementDouble( "c_mom" );
  }
}

void RootTools::SetRunID( time_t starttime )
{
  struct tm *t;
#ifdef ROOTFILEDEBUG 
  std::cout << "<RootTools> start time : " << starttime << std::endl;
#endif
  t = localtime(&starttime);
#if 0
  std::cout << " year : " << t->tm_year-100
	    << " month: " << t->tm_mon+1
	    << " date : " << t->tm_mday
	    << " week : " << t->tm_wday
	    << " hour : " << t->tm_hour
	    << " minit: " << t->tm_min
	    << " secon: " << t->tm_sec 
	    << std::endl;

#endif
  Year = 1970<t->tm_year ? t->tm_year : t->tm_year-100+2000;
  RunID = int((t->tm_mon+1)*1e6 + (t->tm_mday)*1e4 + (t->tm_hour)*1e2 + t->tm_min);
}

void RootTools::PrintElements()
{
  for( int ia=0; ia<(int)ElementsContainer.size(); ia++ ){
    ElementsContainer[ia]->PrintElements();
  }
}
