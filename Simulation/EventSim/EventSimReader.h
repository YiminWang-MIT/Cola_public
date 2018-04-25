//
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
//

#ifndef __EVENTSIM_READER_H__
#define __EVENTSIM_READER_H__


#include "debug.h"
// #include "AquaFrontend.h"
#include "SimSpek.h"
#include "Aqua/AquaReader.h"


class EventSimReader : public AquaReader {
protected:
  int evnum;
 
  AquaTree *lasttree;
  Generator *Gen;
  SimSpek *SpekA,*SpekB,*SpekC;
  void rescanPointers(AquaTree *atree);
public:
  double Weight;
  EventSimReader(AquaIO *io);
  virtual ~EventSimReader();
  virtual int initTree(AquaTree *atree);
  virtual int initTree(AquaTree*, AquaTree*) {return 4711;};
  virtual int nextEvent(AquaTree *atree);
};

#endif /* __EVENTSIM_READER_H__ */
