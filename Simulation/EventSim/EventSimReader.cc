//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
//

#include "EventSimReader.h"
#include <cstring>
#include <iostream>

#define FILE "EventSim: "

EventSimReader::EventSimReader(AquaIO *io) : AquaReader(io)
{

  EVDEBUG("Construct of EventSim")

  evnum=0;

 readRunDatabase("./sim.db","SIMRUN",NULL,1);

 EVDEBUG("Database read")

  Gen=new Generator(io);

 EVDEBUG("Generator constructed")
  
  //For now, just SpekA
  SpekA=new SimSpek(io,SPEKA);

 EVDEBUG("Spek constructed")
}



EventSimReader::~EventSimReader()
{

  EVDEBUG("Destruct of EventSim")


  // For now, just SpekA
  delete Gen;
  delete SpekA;
}

int EventSimReader::initTree(AquaTree *atree)
{


  EVDEBUG("InitTree Tree")

  // For now, just SpekA
  SpekA->rescanPointers(atree);

  rescanPointers( atree);
  return 0;

}



void EventSimReader::rescanPointers(AquaTree *atree)
{
  lasttree=atree;
}





int EventSimReader::nextEvent(AquaTree *atree)
{

  GenState *state;

  if (evnum==0) {
    atree->setIdent(NULL);
    atree->setRuntime(0,1);
    }

  //  if (evnum>3)   return -1;



 if (atree!=lasttree){
  initTree(atree);
 }
 
 atree->clear();   // Clear validation bitfield


  // For now, only single events are simulated!
 
 // do {
  state=Gen->Generate();

  atree->setEventNum(state->EventNumber); // Set EventNumber


  if (state->trigger[SPEKA]) SpekA->trigger(state); //tracevdc



  //triggerbehandlung
  //        vvv hier trigger bedingung reinschreiben
  // } while (!(state->trigger[SPEKA]  ));
 
  if (state->trigger[SPEKA])   SpekA->fill(state);

  Weight=state->Weight*state->Volume;

  evnum=state->EventNumber; 
  #ifdef EVLOGFLAG
   if (evnum & 0xfff==0) std::cerr<<FILE<<"Real Eventnumber: "<<evnum<<std::endl;
  #endif
  

  return 0;

}


