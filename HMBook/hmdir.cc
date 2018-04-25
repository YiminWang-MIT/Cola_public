//                                                                    -*-c++-*-
// $Id: hmdir.cc 2354 2009-11-13 13:58:04Z merkel $
//

#include "hmdir.h"

HistDirectory::~HistDirectory()
{
  while (first) {
    actual = first;
    first = actual->next;
    delete actual;
  }
  delete name;
}


HistDirectory *
HistDirectory::FindBaseDir(const char *histo)
{
  char *slash = (char *) strchr(histo, '/');

  if (!slash) return this;

  int slpos = slash-histo;
  char *dir = new char[slpos+1];
  strncpy(dir, histo, slpos); dir[slpos] = 0;
  HistDirectory * hdir = cd(dir);
  delete dir;

  if (!hdir) return NULL;

  return hdir->FindBaseDir(slash+1);
}


HistDirectory *
HistDirectory::cd(const char *nme)
{
  if (!strcmp(nme, "..")) return up;
  HistDirEntry *hde = first;
  while (hde) {
    if (!strcmp(nme, hde->getName()) && hde->DirAddr()) return hde->DirAddr();
    hde = hde->next;
  }
  return NULL;
}


void
HistDirectory::mkdir(const char *nme)
{
  HistDirectory *newdir = new HistDirectory(nme);
  addentry(newdir);
}


void
HistDirectory::CreateHistoTree()
{
  HIST *id = HMFirst;
  do {
    HistDirectory *top = this;
    const char *title = id->title;
    int p=-1, start = 0;
    while(title[++p]) {
      if (title[p]=='/') {
	char dir[255];
	strncpy(dir,&title[start],p-start);
	dir[p-start]=0;
	if (!top->cd(dir)) top->mkdir(dir);
	top = top->cd(dir);
	if (!top) std::cout << "oops" << std::endl;
	start = p+1;
      }
    }
    top->addentry(new HistEntry(id,&title[start]));
    id = (HIST *) id->next;
  } while(id);
}

void
HistDirectory::addentry(HistDirEntry *hd)
{
  hd->up = this;
  if (first==NULL) {
    first = last = hd;
    hd->next = NULL;
    hd->prev = NULL;
  } else {
    last->next = hd;
    hd->prev = last;
    last = hd;
    hd->next = NULL;
  }
}


void
HistDirectory::printtree()
{
  static int level=0;
  level ++;
  HistDirEntry *hde = first;
  while (hde) {
    int i;
    for (i=0;i<level;i++) std::cout << "  ";
    std::cout << "[" << hde->getName() << "]" << std::endl;;
    if (hde->DirAddr()) hde->DirAddr()->printtree();
    hde = hde->next;
  }
  level--;
}


HistDirEntry *
HistDirectory::next()
{
  HistDirEntry *ret = actual;
  if (ret) actual = actual->next;
  return ret;
}







