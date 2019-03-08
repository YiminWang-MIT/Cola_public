/*
 * This is the definition of the olympus raw data structures
 * DLRT = DarkLight Root Tree
 */

#ifndef __DLRAWTREE_H__
#define __DLRAWTREE_H__


#include <TObject.h>
#include <TTimeStamp.h>
#include <vector>
#include <map>

/**
 * Base class for all components. We may use this for function prototypes.
 */

class DLRTBase: public TObject
{
 public:
  DLRTBase();
  virtual ~DLRTBase();
  ClassDef(DLRTBase,1);
};

/**
 * A class to store information of the current run
 */

class DLRTRunInfo:public DLRTBase
{
	public:
		UInt_t runNumber;
		TTimeStamp startTime;
		TTimeStamp stopTime;
		UInt_t nrOfEvents;
		DLRTRunInfo();
		virtual ~DLRTRunInfo();
		ClassDef(DLRTRunInfo,1);
};

// Per event info 

class DLRTEventInfo:public DLRTBase
{
	public:
		UInt_t eventNumber;
		TTimeStamp eventTime;
		UInt_t trigFired;		
	        unsigned long long liveTimeCounter;
		unsigned long long freeClock;
		unsigned long long deadTimeCounter;
		double weight;
		DLRTEventInfo();
		virtual ~DLRTEventInfo();
		ClassDef(DLRTEventInfo,1);
};


/**
 * A Class saving trigger information
 */

class DLRTtrigger:public DLRTBase
{
 public:
  unsigned long long inputs_at_trigger;
  unsigned int vme_trigger_cnt;
  unsigned int orand_trigger_cnt;
  DLRTtrigger();
  virtual ~DLRTtrigger();
  ClassDef(DLRTtrigger,1);
};



/**
 * A Class saving a binary blob, with a pointer and a size
 */

class DLRTBinaryBlob:public DLRTBase
{
 public:
  Int_t size; 
  UChar_t *data; //[size]
  DLRTBinaryBlob();
  virtual ~DLRTBinaryBlob();
  ClassDef(DLRTBinaryBlob,1);
};




class DLRTGEM:public DLRTBase
{
 public:
  std::vector<unsigned short> data[16];
  DLRTGEM ();
  virtual ~DLRTGEM();
  ClassDef(DLRTGEM,1);
};


/**
 * A class for v1190 hit info 
 */

class DLRTTDC: public DLRTBase
{
 public:
  std::multimap<unsigned int,unsigned int> hits;
  std::vector<unsigned int> errors;
  DLRTTDC();
  virtual ~DLRTTDC();
  ClassDef(DLRTTDC,1);
};



/**
 * A class for the slow control in band data
 */
class DLRTSlowData:public DLRTBase
{
 public:
  struct slowStruct{
  unsigned short index;
  double time;
  float value;
  short status;
  };
  std::vector<DLRTSlowData::slowStruct> slowData;
  DLRTSlowData();
  virtual ~DLRTSlowData();
  ClassDef(DLRTSlowData,1);
};

/**
 * A wrapper class for the slowcontrol index to name mapping
 */
 
class DLRTIndex2Name: public DLRTBase
{
 public:
  std::map<unsigned short, std::string> index2name;
  DLRTIndex2Name();
  virtual ~DLRTIndex2Name();
  ClassDef(DLRTIndex2Name,1);
}; 


#endif

