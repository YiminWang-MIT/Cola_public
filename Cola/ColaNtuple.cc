// Ntuple in Cola++
//
// @(#) Cola Ntuple 
// @(#) Pablo Barneo
// @(#) NIKHEF, Amsterdam
//
// $Id: ColaNtuple.cc 2216 2008-06-13 21:13:47Z distler $
//

#include "ColaNtuple.h"
#include "ColaScalar.h"

#include <sys/param.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

extern "C" {
  void hlimit_(int *);
  void hfnt_(int *);
  void hcdir_(char *, const char *, long, long);
  void hropen_(int *, const char *, const char *, const char *,
	       int *, int *, long, long, long);
  void hbnt_(int *, const char *, const char *, long, long);
  void hbname_(int *, const char *, float *, const char *, long, long);
  void hbset_(const char *, int *, int *, long);
  void hrend_(const char *, long);
  void hrout_(int *, int *, const char *, long);
}

enum { PVST_SHORT, PVST_LONG, PVST_FLOAT, PVST_PTR, PVST_UNSIGNED=16};

#define PVST_M_TYPE (0x000f)

typedef unsigned short USHORT;
typedef USHORT *PUSHORT;
typedef unsigned long ULONG;
typedef ULONG *PULONG;
typedef short SHORT;
typedef SHORT *PSHORT;
typedef long LONG;
typedef LONG *PLONG;
typedef float FLOAT;
typedef FLOAT *PFLOAT;

// Ntuple variable in Cola++
// handling of variables in a ntuple
//
NT_var *NT_var::first = NULL;
NT_var *NT_var::last = NULL;

// create_var:
// create a node for pvsvariable pvsvar and initialize it
// if ind>=0, it is interpreted as array index
// RC = address of the new node
//    = NULL in case of an error
//
NT_var::NT_var(char *scalname, char *fieldname, char *blockname)
{
  int verbosity = (getenv("VERBOSE") != NULL);

  scal = Cola_Scalar::findscalar(scalname);

  if (!scal) {
    if (verbosity) {
    std::cerr << "Cola Ntuple: Error creating variable \"" << scalname 
	      << "\". Variable does not exist" << std::endl;
    }
    return;
  }

  if (!first) first = this;
  else last->next = this;
  last = this;

  //<!-- Temporal while I fix the typing stuff
  type = PVST_FLOAT;
  next = NULL;
  varname = new char[strlen(scalname)+1];
  strcpy(varname, scalname);
  ntfield = new char[strlen(fieldname)+1];
  strcpy(ntfield, fieldname);
  ntblock = new char[strlen(blockname)+1];
  strcpy(ntblock, blockname);
}

// delete_varlist:
// delete all elements in the varlist and free the memory
//
void NT_var::delete_varlist()
{
  NT_var *nxt, *node = first;
  
  while (node) {
    nxt = node->next;
    delete node;
    node = nxt;
  }
  return;
}

// copy_values:
// copy the values of all variables into the output array
// RC = the number of values copied
//
void NT_var::copy_values()
{
  NT_var *ptr = first;
  
  while (ptr) {
    ptr->get_pvs_var(); 
    ptr = ptr->next;
  }
  return;
}

void NT_var::print_varlist()
{
  NT_var *ptr = first;

  while (ptr) {
    //    std::cerr << "address = " << (unsigned long)(ptr->scal)
    //	      << "; type = " << ptr->type << std::endl;
    ptr->get_pvs_var();
    std::cerr << "value = " << *((float*)ptr->outaddr) << std::endl;
    ptr = ptr->next;
  }
  return;
}

// get_pvs_var:
// get the value of a PVS variable and return it in the out parameter
//
void NT_var::get_pvs_var()
{
  switch (type & PVST_M_TYPE) {
  case PVST_LONG:
    if ( type & PVST_UNSIGNED ) {
      if (scal->isValid()) *((PULONG)outaddr) = (ULONG)scal->evaluate();
      else *((PULONG)outaddr) = 0;
    } else {
      if (scal->isValid()) *((PLONG)outaddr) = (LONG)scal->evaluate();
      else *((PLONG)outaddr) = 0;
    }
    break;
  case PVST_FLOAT:
    if (scal->isValid()) *((PFLOAT)outaddr) = (FLOAT)scal->evaluate();
    else *((PFLOAT)outaddr) = -999.0;
    break;
  case PVST_SHORT:
    if ( type & PVST_UNSIGNED ) {
      if (scal->isValid()) *((PULONG)outaddr) = (USHORT)scal->evaluate();
      else *((PULONG)outaddr) = 0;
    } else {
      if (scal->isValid()) *((PLONG)outaddr) = (SHORT)scal->evaluate();
      else *((PLONG)outaddr) = 0;
    }
    break;
  default:
    break;
  }
  return;
}

Cola_Ntuple *Cola_Ntuple::head = NULL;
Cola_Ntuple *Cola_Ntuple::ntl = NULL;
Cola_Ntuple *Cola_Ntuple::last = NULL;
int Cola_Ntuple::nelt = 0;

// Constructor of the class
// create a new ntuple list element and add it at the end of the
// list of ntuples
// RC = adddress of the new element
//    = NULL in case of an error
//
Cola_Ntuple::Cola_Ntuple(int ntid, char *tit)
{
  if (ntid == 0) return; 

  if (!head) head = this;
  else last->next = this;
  last = this;

  nelt++;
  id = ntid;
  lun = LUN;
  title = tit;
  ntdir = NULL;
  next = NULL;
  nt_contents = new float[MAX_NT_VAR];     
}

int Cola_Ntuple::InitNtuple(int id, char *tit, int elt,
			    char *varname[], char *descr[],
			    char *block[], char *file)
{
  int verbosity = (getenv("VERBOSE") != NULL);

  if (verbosity) {
    std::cerr << "Started NTUPLE module\n";
  }

  if (sizeof(int) != sizeof(float)) {	// make sure dirty trick works
    std::cerr << "Cola Ntuple: FATAL ERROR, Ntuples will not work in this no matter you do. Inform the maintainer and stop using Ntuple commands\n";
    return -1;
  }

  create_ntuple(id, tit, elt, varname, descr, block, file);

  if (nelt < 1) {
    std::cerr << "Cola Ntuple: calling the ntuple routine without specifying content to put in" << std::endl;
  }

  return 0;
}

// create_ntuple:	
// create a columnwise ntuple with identifier ntid and
// title tit; the ntuple will contain nvar variables
// corresponding to the PVS variables pvsvars with names
// varnames in the ntuple
// RC =  0 after success
//    = -1 in case of an error
//
int Cola_Ntuple::create_ntuple(int ntid, char *tit, int nvar, char *varname[],
			       char *descr[], char *block[], char *file)
{
  int unknown=0;
  //char *file = "";

  if ( nvar <= 0 ) return 0;

  if (! (ntl = new Cola_Ntuple(ntid, tit)) ) return -1;

  int hbookmem = HBOOKMEM;
  hlimit_(&hbookmem);
  for (int i=0; i<nvar; i++) {
    if (ntl->add_var(ntid, varname[i], descr[i], block[i])) unknown++;
  }
  if (unknown > 0) {
    std::cerr << "Cola Ntuple: " << unknown
	 << " variables were NOT added to the ntuple" << std::endl;
  }

  //file = (char *) getenv("ONLINE_RZ");
  //if (!file) file = "Online.rz";
  ntl->open_ntuple(ntid, tit, file);
  return 0;
}

// add_var:
// add the PVS variable to the list of variables to be put
// in ntuple id, under name ntfield
// RC =  0 if ok
//    = -1 after an error
//
int Cola_Ntuple::add_var(int id, char *ntvar, char *ntfield, char *block)
{
  static int last_id;
  Cola_Ntuple *ntp;
  
  if ( last_id == id && id != 0 ) {	// add to existing list
    if (! (vars = new NT_var(ntvar, ntfield, block)) ) return -1;
  } else {		// store variables in a new ntuple
    ntp = head;
    while (ntp && ntp->id != id) {
      ntp = ntp->next;
    }
    if (!ntp) {
      std::cerr << "Cola Ntuple: Invalid Ntuple Identity ID = " << id << std::endl;
      return -1;
    }

    if (! (ntp->vars = new NT_var(ntvar, ntfield, block)) ) return -1;
    last_id = id;
  }
  return 0;
}

// delete_ntlist:
// remove all entries from the histogram list and free the memory
//
void Cola_Ntuple::delete_ntlist()
{
  Cola_Ntuple *rm, *tail;
  
  tail = head;
  while (tail) {
    tail->close_ntuple();
    tail->vars->delete_varlist();
    rm	 = tail;
    delete rm;
    tail = tail->next;
  }
  nelt = 0;
  head = last = NULL;
  return;
}

void Cola_Ntuple::delete_all()
{
  Cola_Ntuple *rm, *tail = head;

  while (tail) {
    tail->vars->delete_varlist();
    rm = tail;
    delete rm;
    tail = tail->next;
  }
  nelt = 0;
  head = last = NULL;
  return;
}

// print_ntlist:
// walk through the list and print information about
// each node (debugging purposes only)
//
void Cola_Ntuple::print_ntlist()
{
  Cola_Ntuple *ptr;
  
  ptr = head;
  while (ptr) {
    std::cerr << "NtupleID = " << ptr->id
	      << "; title = " << ptr->title << std::endl;
    ptr->vars->print_varlist();
    ptr = ptr->next;
  }
  return;
}

// existing_hid: 
// check the ntuple list for already existing 
// ntuple identifiers
// RC = 0 if hid is unique
//    = 1 if hid already exists
//
int Cola_Ntuple::existing_ntid(int ntid)
{
  Cola_Ntuple *ptr;
  
  ptr = head;
  while (ptr) {
    if ( ptr->id == ntid ) return 1;
    ptr = ptr->next;
  }
  return 0;
}

// fill_ntuple:
// walk through the list and fill all ntuple variables
// RC = 0 always
//
int Cola_Ntuple::fill_ntuple()
{
  Cola_Ntuple *ptr;
  
  if ( nelt == 0 ) return 0;
  
  ptr = head;
  while (ptr) {
    ptr->vars->copy_values();
    hfnt_(&id);
    ptr = ptr->next;
  }
  return 0;
}

// open_ntuple:
// create a columnwise ntuple on disk
// RC =  0 after success
//    = -1 in case of an error
//
int Cola_Ntuple::open_ntuple(int ntid, char *tit, char *nt_file)
{	
  int  istat=0, ierr=0;
  char *nt_field = NULL;
  int offset=0, sl;
  NT_var *var;

  if (!head) {
    std::cerr << "Cola Ntuple: No contents for Ntuple. Ntuple has not been created\n";
    return 0;
  }
  unlink(nt_file);   // remove old file
  std::cerr << "Cola Ntuple: Ntuple " << ntid << " output written to file " 
	    << nt_file << std::endl;

  delete ntdir;
  ntdir = new char[strlen(NTNAME)+3];
  sprintf(ntdir, "//%s", NTNAME);

  int reclength = LREC;
  hropen_(&lun, ntdir+2, nt_file, "pn", &reclength, &istat,
	  strlen(ntdir+2), strlen(nt_file), 2);
  if (istat) {
    std::cerr << "Cola Ntuple: Error in call to hropen() routine.\n";
    return -1;
  }
  hbnt_(&ntid, tit, " ", strlen(tit), 1);
  hbset_("BSIZE", &reclength, &ierr, 5);
  if (ierr) {
    std::cerr << "Cola Ntuple: Error in call to HBSET routine.\n"; 
    return -1;
  }
  
  var = head->vars->first;
  while (var) {
    sl = strlen(var->ntfield);
    sl = (sl > NT_VAR_LEN) ? NT_VAR_LEN : sl;
    nt_field = new char[strlen(var->ntfield)+12];
    strncpy(nt_field, var->ntfield, sl);
    nt_field[sl] = 0;

    switch (var->type & PVST_M_TYPE) {
    case PVST_FLOAT:
      strcat(nt_field, ":R*4");
      break;
    case PVST_LONG:
      if ( var->type & PVST_UNSIGNED ) {
	strcat(nt_field, ":U*4");
      } else {
	strcat(nt_field, ":I*4");
      }
      break;
    case PVST_SHORT:
      if ( var->type & PVST_UNSIGNED ) {
	strcat(nt_field, ":U*4");
	strcat(nt_field, "::[0,65535]");
      } else {
	strcat(nt_field, ":I*4");
      }
      break;
    default:
      std::cerr << "Cola Ntuple: Warning:\n" << "no variable type known for " 
		<< var->ntfield << ", taking FLOAT as default.\n";
      strcat(nt_field, ":I*4");
    }
    var->outaddr=(void *)(nt_contents+offset);
    hbname_(&ntid, var->ntblock, nt_contents+offset, nt_field,
	   strlen(var->ntblock), strlen(nt_field));
    delete nt_field;
    nt_field = NULL;
    offset++;
    var = var->next;
  }
  return 0;
}

// dump_ntuple: 
// fill the ntuple
// RC = 0 always
//
int Cola_Ntuple::dump_ntuple()
{
  //fill ntuple id with contents from nt_contents
  hfnt_(&id);
  return 0;
}

// close_ntuple:
// terminate the ntuple file for ntuple id
// do NOT use id==0, because it interferes with the histograms
// RC = 0 always
//
int Cola_Ntuple::close_ntuple()
{
  int  icycle;
  
  hrout_(&id, &icycle, " ", 1);  // write ntuple ID in current directory
  hrend_(ntdir+2, strlen(ntdir+2));

  return 0;
}
