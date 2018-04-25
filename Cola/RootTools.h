#ifndef ROOTTOOLS_H
#define ROOTTOOLS_H 1
#include <iostream>
#include <vector>
#include <map>

#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>

#include "Aqua/AquaTree.h"

class AnElement{
 protected:
  int nEle;
  int Type;
  std::string Name_Root, Name_Cola;
  std::string Title_Root;
  std::vector <AquaNode *> ANode;
 public:
  AnElement( std::string name1, std::string name2 )
    { Name_Root=name1; Name_Cola=name2; nEle=0; Type=0; }
  virtual ~AnElement(){};

 public:
  int type() { return Type; }
  int nele() { return nEle; }
  int nanode() { return ANode.size(); }
  AquaNode *anode(int i) { return ANode[i]; }
  std::string title_root() { return Title_Root; }
  std::string name_root() { return Name_Root; }
  std::string name_cola() { return Name_Cola; }

  virtual void SetAquaNode( AquaNode *anode ) { ANode.push_back(anode); }
  virtual void *val() { return 0; }
  virtual double val(int i) { return 0; }
  virtual void SetVal( const double &v, const int &i ) {}
  virtual void SetTitle() {};
  virtual void PrintElements()
  { std::cout << Name_Root << "  " << Name_Cola << std::endl;
    for(int i=0;i<nEle;i++){ std::cout << "  " << i << "  " << val(i) << std::endl; } }
  virtual void Clear(){}
};

class AnElementShort : public AnElement
{
 private:
  short *Val;
 public:
  AnElementShort( std::string name1, std::string name2, int nele )
    : AnElement(name1,name2) { Val=new short[nele]; nEle=nele; Type=1; SetTitle(); }
  ~AnElementShort(){ delete Val; }
  void *val() { return Val; }
  virtual double val(int i) { return Val[i]; }
  void SetVal( const double &v, const int &i ) { if(i<nEle) Val[i] = (short)v; }
  void SetVal( const short &v, const int &i ) { if(i<nEle) Val[i] = v; }
  void SetTitle();
};

class AnElementUnsignedShort : public AnElement
{
 private:
  unsigned short *Val;
 public:
  AnElementUnsignedShort( std::string name1, std::string name2, int nele )
    : AnElement(name1,name2) { Val=new unsigned short[nele]; nEle=nele; SetTitle(); }
  ~AnElementUnsignedShort(){ delete Val; }
  void *val() { return Val; }
  virtual double val(int i) { return Val[i]; }
  void SetVal( const double &v, const int &i ) { if(i<nEle) Val[i] = (unsigned short)v; }
  void SetVal( const unsigned short &v, const int &i ) { if(i<nEle) Val[i] = v; }
  void SetTitle();
};

class AnElementLong : public AnElement
{
 private:
  long *Val;
 public:
  AnElementLong( std::string name1, std::string name2, int nele )
    : AnElement(name1,name2) { Val=new long[nele]; nEle=nele; Type=2; SetTitle(); }
  ~AnElementLong(){ delete Val; }
  void *val() { return Val; }
  virtual double val(int i) { return Val[i]; }
  void SetVal( const double &v, const int &i ) { if(i<nEle) Val[i] = (long)v; }
  void SetVal( const long &v, const int &i ) { if(i<nEle) Val[i] = v; }
  void SetTitle();
};

class AnElementInt : public AnElement
{
 private:
  int *Val;
 public:
  AnElementInt( std::string name1, std::string name2, int nele )
    : AnElement(name1,name2) { Val=new int[nele]; nEle=nele; Type=3; SetTitle(); }
  ~AnElementInt() { delete Val; }
  void *val() { return Val; }
  virtual double val(int i) { return Val[i]; }
  void SetVal( const double &v, const int &i ) { if(i<nEle) Val[i] = (int)v; }
  void SetVal( const int &v, const int &i ) { if(i<nEle) Val[i] = v; }
  void SetTitle();
};

class AnElementDouble : public AnElement
{
 private:
  double *Val;
 public:
  AnElementDouble( std::string name1, std::string name2, int nele )
    : AnElement(name1,name2) { Val=new double[nele]; nEle=nele; Type=4; SetTitle(); }
  ~AnElementDouble(){ delete Val; }
  void *val() { return Val; }
  virtual double val(int i) { return Val[i]; }
  void SetVal( const double &v, const int &i ) { if(i<nEle) Val[i] = (double)v; }
  void SetTitle();
};

class AnElementVector : public AnElement  // used for mwpc information
{
 private:
  std::vector <int> Vec;
 public:
  AnElementVector( std::string name1, std::string name2 )
    : AnElement(name1,name2) { Vec.clear(); nEle=99; Type=5; }
  ~AnElementVector() { Vec.clear(); }
  int nele() { return (int)Vec.size(); }
  void *val() { return &Vec; }
  virtual double val(int i) { return -9999; } // not to be used
  void SetVal( const double &v, const int &i ) { Vec.push_back((int)v); }
  void SetVal( const short  &v, const int &i ) { Vec.push_back((int)v); }
  void SetVal( const int    &v, const int &i ) { Vec.push_back(v); }
  //void SetTitle();
  virtual void PrintElements()
  { std::cout << Name_Root << "  " << Name_Cola << std::endl;
    for(int i=0;i<(int)Vec.size();i++){ std::cout << "  " << i << "  " << Vec[i] << std::endl; }
  }
  virtual void Clear(){ Vec.clear(); }
};

class RootTools {
 private:
  char *RootFileName;
  TFile *RFile;
  TTree *tree;

  std::vector <AnElement*> ElementsContainer;

  int Use_ROOT;
  AquaTree *ATree; // for abcn
  AquaTree *Out;   // for online

  int Year, RunID;

 public:
  RootTools();
  RootTools(char *filename);
  ~RootTools();

 public:
  void OpenFile();
  void CloseFile();
  void Fill();

  void SetOutput( AquaTree *atree, AquaTree *out, int use_root=1 );
  void SetRunID( time_t starttime );

 private:
  void SetElementShort(         std::string name_root, int nele=1 );
  void SetElementUnsignedShort( std::string name_root, int nele=1 );
  void SetElementLong(          std::string name_root, int nele=1 );
  void SetElementInt(           std::string name_root, int nele=1 );
  void SetElementDouble(        std::string name_root, int nele=1 );
  void SetAquaNode( AnElement *ele, std::string name_cola, int nele, int ele0=0 );

  // for values extracted from Cola (abcn or online)
  void SetElementShort(         std::string name_root, std::string name_cola, int nele=1, int ele0=0 );
  void SetElementUnsignedShort( std::string name_root, std::string name_cole, int nele=1, int ele0=0 );
  void SetElementLong(          std::string name_root, std::string name_cola, int nele=1, int ele0=0 );
  void SetElementInt(           std::string name_root, std::string name_cola, int nele=1, int ele0=0 );
  void SetElementDouble(        std::string name_root, std::string name_cola, int nele=1, int ele0=0 );
  void SetElementVector(        std::string name_root, std::string name_cola, int nele=1, int ele0=0 );
  
  void AddElementToTree( AnElement *anele );
  void AddElementVecToTree( AnElement *anele );
  void GetData();

  AnElement *SearchElement( std::string name );

 public:  
  int use_root() { return Use_ROOT; }
  void Fill( std::string name_root, short          val, int iele=0 );
  void Fill( std::string name_root, unsigned short val, int iele=0 );
  void Fill( std::string name_root, long           val, int iele=0 );
  void Fill( std::string name_root, int            val, int iele=0 );
  void Fill( std::string name_root, double         val, int iele=0 );

  void PrintElements();
};


#endif
