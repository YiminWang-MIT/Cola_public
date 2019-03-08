/** 
 * This file has the implementations of the ORT 
 */

#include "dlrawtree.h"

DLRTBase::DLRTBase() {};
DLRTBase::~DLRTBase() {};
ClassImp(DLRTBase);


DLRTRunInfo::DLRTRunInfo() {};
DLRTRunInfo::~DLRTRunInfo() {};
ClassImp(DLRTRunInfo);

DLRTEventInfo::DLRTEventInfo() {};
DLRTEventInfo::~DLRTEventInfo() {};
ClassImp(DLRTEventInfo);


DLRTBinaryBlob::DLRTBinaryBlob() {
data=NULL;
};
DLRTBinaryBlob::~DLRTBinaryBlob() {};
ClassImp(DLRTBinaryBlob);


DLRTGEM::DLRTGEM() {
};
DLRTGEM::~DLRTGEM() {};
ClassImp(DLRTGEM);


DLRTtrigger::DLRTtrigger(){};
DLRTtrigger::~DLRTtrigger(){};
ClassImp(DLRTtrigger);

DLRTTDC::DLRTTDC(){};
DLRTTDC::~DLRTTDC(){};
ClassImp(DLRTTDC);

DLRTSlowData::DLRTSlowData() {};
DLRTSlowData::~DLRTSlowData() {};
ClassImp(DLRTSlowData);

DLRTIndex2Name::DLRTIndex2Name() {};
DLRTIndex2Name::~DLRTIndex2Name() {};
ClassImp(DLRTIndex2Name);
