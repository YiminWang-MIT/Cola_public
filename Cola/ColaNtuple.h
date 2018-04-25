//
// Ntuple in Cola++, header file
//
// @(#) Cola Ntuple 
// @(#) Pablo Barneo
// @(#) NIKHEF, Amsterdam
//
// $Id: ColaNtuple.h 2216 2008-06-13 21:13:47Z distler $
//
#ifndef __COLA_NTUPLE_H__
#define __COLA_NTUPLE_H__

#include "ntmode.h"
#include "evaluate.h"

class NT_var
{
 public:
  expression *scal;        // base address of the array (variable)
  void	*outaddr;	   // address of output
  short type;
  char  *varname;
  char  *ntfield; 
  char  *ntblock;
  NT_var *next;
  static NT_var *first, *last;

  //-- Constructor
  NT_var(char *scalname, char *fieldname, char *blockname);
  //-- Destructor, not really
  void delete_varlist();
  //-- copy the values of all variables into the output array
  void copy_values();
  //-- only for debugging
  void print_varlist();
  void get_pvs_var();
};

class Cola_Ntuple
{
 private:
  int         id;
  int         lun;
  char        *title;
  NT_var      *vars;
  char        *ntdir;
  Cola_Ntuple *next;
  float       *nt_contents;

 public:
  static Cola_Ntuple *head, *last, *ntl;
  static int nelt;

  //- Constructor
  Cola_Ntuple(int ntid, char *tit = NTUPLE_TIT);
  //-- Destructor, not really
  void delete_ntlist();
  void delete_all();

  //-- only for debugging
  void print_ntlist();
  int existing_ntid(int ntid);
  int InitNtuple(int id, char *tit, int elt, char *varname[], char *descr[],
		 char *block[], char *file);
  int fill_ntuple();

 private:
  int add_var(int id, char *ntvar, char *ntfield, char *block);
  int dump_ntuple();
  int close_ntuple();
  int open_ntuple(int ntid, char *tit, char *arr);
  int create_ntuple(int ntid, char *tit, int nvar, char *ntvarname[],
		    char *descr[], char *block[], char *file);
};

#endif  // __COLA_NTUPLE_H__
