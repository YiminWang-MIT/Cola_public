//                                                                    -*-c++-*-
// $Id: hmdir.h 2216 2008-06-13 21:13:47Z distler $
//

#include <iostream>
#include <string.h>
#include "hmbook.h"

class HistDirEntry {
protected:
  char *name;
public:
  class HistDirectory *up;
  HistDirEntry        *next;
  HistDirEntry        *prev;

  HistDirEntry()           { ; }
  virtual ~HistDirEntry()  { ; }
  virtual class HistDirectory *DirAddr() = 0;
  const char *getName()    { return name; }
};

class HistDirectory : public HistDirEntry {
protected:
  HistDirEntry *first; 
  HistDirEntry *last; 
  HistDirEntry *actual;
  void addentry(HistDirEntry *hd);
  void mkdir(const char *name);
public:
  HistDirectory(const char *nme) {
    first = last = NULL;
    name = new char[strlen(nme)+1];
    strcpy(name, nme);
  }
  virtual ~HistDirectory();
  HistDirectory *DirAddr() { return this; }
  void newscan() { actual = first; }
  HistDirEntry *next();
  void printtree();
  HistDirectory *cd(const char *name);
  void CreateHistoTree();
  HistDirectory *FindBaseDir(const char *histo);
};

class HistEntry : public HistDirEntry {
  HIST *id;
public:
  HistEntry(HIST *hid, const char *nme) {
    id = hid;
    name = new char[strlen(nme)+1];
    strcpy(name, nme);
  }
  HIST *Histogram() { return id; }
  HistDirectory *DirAddr() { return NULL; }
  virtual ~HistEntry() { delete name; }
};

