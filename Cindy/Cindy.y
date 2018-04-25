//								      -*-c++-*-
// Copyright (c) 1993-95 by	Michael O. Distler,
//				Institut fuer Kernphysik,
//				Johannes Gutenberg-Universitaet Mainz
//
// All rights reserved.
//
// $Header: /tmp/cvsroot/Cola/Cindy/Cindy.y,v 4.14 2006-12-08 16:02:11 distler Exp $
//
// YACC Parser fuer den Cindy++ Compiler (bison++ Version)
//

%name	CindyYACC

%define LSP_NEEDED
%define ERROR_BODY	=0
%define LEX_BODY	=0
%define CONSTRUCTOR_CODE	Types=NULL; Structs=NULL; Vars=NULL;
%define MEMBERS		public:\
			  VPtr Vars;\
			protected:\
			  TPtr  Types;\
			  TPtr  Structs;\
			  Member *mlist;\
			  NameList *nlist;\
			  VPtr  VarDeclaration(TPtr, NameList *);\
			  TPtr  InitPlainType(const char *, int, int);\
			  TPtr  InitType(TPtr, NameList *);\
			  TPtr  InitStructType(const char *, Member *);\
			  TPtr  FindType(const char *);\
			  TPtr  FindStructType(const char *);

%header{
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <iostream>
#include <fstream>
#ifdef HAVE_OSTREAM
#include <ostream>
#endif
#include <iosfwd>
#include <cstdlib>
#include <cstring>

class Member;

typedef struct type {
  char   *name;
  char   *sname;
  int	  len;
  int	  align;
  int	  array;
  Member *member;
  struct type *next;
} Type, *TPtr;

typedef struct var {
  char   *name;
  TPtr	  typ;
  int	  array;
  struct var *next;
} Var, *VPtr;

class Number {
  long val;
public:
  Number(const char *txt) { val = atol(txt); }
  long value() { return val; }
  Number *neg() { val = -val; return this; }
  Number *calc(int op, Number *arg) {
    switch (op) {
      case '+': val += arg->value(); break;
      case '-': val -= arg->value(); break;
      case '*': val *= arg->value(); break;
      case '/': val /= arg->value(); break;
      case '%': val = val % arg->value(); break;
      default: break;
    }
    return this;
  }
};

class NameList {
  char     *nam;
  int	    arr;
  NameList *nxt;
public:
  NameList(const char *n, Number *a, NameList *nl=NULL) {
    nam = new char[strlen(n)+1]; strcpy(nam, n); arr = a->value(); nxt = nl;
  }
  NameList(const char *n, NameList *nl=NULL) {
    nam = new char[strlen(n)+1]; strcpy(nam, n); arr = 0; nxt = nl;
  }
  ~NameList() { delete nam; }
  const char *name() { return nam; }
  NameList   *next() { return nxt; }
  int        array() { return arr; }
};

class Member {
  TPtr	  typ;
  char   *nam;
  int	  arr;
  Member *nxt;
public:
  Member(TPtr tp, NameList *nlst) {
    nam = new char[strlen(nlst->name())+1]; strcpy(nam, nlst->name()); 
    typ = tp; arr = nlst->array(); nxt = NULL;
  }
  ~Member() { delete nam; }
  TPtr		   type() { return typ; }
  const char	 *cname() { return nam; }
  Member	  *next() { return nxt; }
  Member *next(Member *n) { return (nxt = n); }
  int		  array() { return arr; }
};

extern const char *  CHAR_NAME;
extern const char * UCHAR_NAME;
extern const char * SHORT_NAME;
extern const char *USHORT_NAME;
extern const char *   INT_NAME;
extern const char *  UINT_NAME;
extern const char *  LONG_NAME;
extern const char * ULONG_NAME;
extern const char * FLOAT_NAME;
extern const char *DOUBLE_NAME;

int PrintVars(char *, const char *, VPtr, char *);
int Vrekursion(std::ostream *, TPtr, const char *);
%}

%{
#include <errno.h>
#include "arch.h"

const char *  CHAR_NAME = "char";
const char * UCHAR_NAME = "unsigned char";
const char * SHORT_NAME = "short";
const char *USHORT_NAME = "unsigned short";
const char *   INT_NAME = "int";
const char *  UINT_NAME = "unsigned int";
const char *  LONG_NAME = "long";
const char * ULONG_NAME = "unsigned long";
const char * FLOAT_NAME = "float";
const char *DOUBLE_NAME = "double";
%}

%union {
  int       val;
  Number   *num;
  char     *str;
  TPtr      typ;
  NameList *nlst;
  Member   *mem;
}

%start	list
%token	STRUCT TYPEDEF
%token	SHORT LONG UNSIGNED
%token	CHAR INT FLOAT DOUBLE
%token	<num>	NUMBER
%token	<str>	STRING
%type	<num>	calc
%type	<typ>	type_name struct_decl
%type	<nlst>	name_list
%type	<mem>	struct_member

%left '-' '+'
%left '*' '/' '%'
%left NEG     /* unary minus */

%%

list:
		| list var_decl ';'
		| list type_decl ';'
		| list error		{ yyerrok; }
		;
type_decl:	TYPEDEF type_name name_list	{
		  while ($3 != NULL) {
		    nlist = $3;
		    InitType($2, $3);
		    $3 = $3->next();
		  }
		}
		| TYPEDEF struct_decl name_list	{
		  while ($3 != NULL) {
		    nlist = $3;
		    InitType($2, $3);
		    $3 = $3->next();
		  }
		}
		| TYPEDEF STRUCT STRING name_list	{
		  while ($4 != NULL) {
		    nlist = $4;
		    InitType(FindStructType($3), $4);
		    $4 = $4->next();
		  }
		}
		| STRUCT STRING '{' struct_member '}'	{
		  InitStructType($2, $4);
		}
		;
struct_decl:	STRUCT STRING '{' struct_member '}'	{
		  $$ = InitStructType($2, $4);
		}
		| STRUCT '{' struct_member '}'	{
		  $$ = InitStructType("", $3);
		}
		;
struct_member:	struct_decl name_list ';' struct_member	{
		  $$ = NULL;
		  while ($2 != NULL) {
		    nlist = $2;
		    if ( $$ == NULL )
		      mlist = $$ = new Member($1, $2);
		    else
		      mlist = mlist->next(new Member($1, $2));
		    $2 = $2->next();
		  }
		  mlist->next($4);
		}
		| type_name name_list ';' struct_member	{
		  $$ = NULL;
		  while ($2 != NULL) {
		    nlist = $2;
		    if ( $$ == NULL )
		      mlist = $$ = new Member($1, $2);
		    else
		      mlist = mlist->next(new Member($1, $2));
		    $2 = $2->next();
		  }
		  mlist->next($4);
		}
		| STRUCT STRING name_list ';' struct_member	{
		  $$ = NULL;
		  while ($3 != NULL) {
		    nlist = $3;
		    if ( $$ == NULL )
		      mlist = $$ = new Member(FindStructType($2), $3);
		    else
		      mlist = mlist->next(new Member(FindStructType($2), $3));
		    $3 = $3->next();
		  }
		  mlist->next($5);
		}
		| struct_decl name_list ';'		{
		  $$ = NULL;
		  while ($2 != NULL) {
		    nlist = $2;
		    if ( $$ == NULL )
		      mlist = $$ = new Member($1, $2);
		    else
		      mlist = mlist->next(new Member($1, $2));
		    $2 = $2->next();
		  }
		}
		| type_name name_list ';'		{
		  $$ = NULL;
		  while ($2 != NULL) {
		    nlist = $2;
		    if ( $$ == NULL )
		      mlist = $$ = new Member($1, $2);
		    else
		      mlist = mlist->next(new Member($1, $2));
		    $2 = $2->next();
		  }
		}
		| STRUCT STRING name_list ';'		{
		  $$ = NULL;
		  while ($3 != NULL) {
		    nlist = $3;
		    if ( $$ == NULL )
		      mlist = $$ = new Member(FindStructType($2), $3);
		    else
		      mlist = mlist->next(new Member(FindStructType($2), $3));
		    $3 = $3->next();
		  }
		}
		;
var_decl:	type_name name_list	{
		  while ($2 != NULL) {
		    nlist = $2;
		    VarDeclaration($1, $2);
		    $2 = $2->next();
		  }
		}
		| struct_decl name_list {
		  while ($2 != NULL) {
		    nlist = $2;
		    VarDeclaration($1, $2);
		    $2 = $2->next();
		  }
		}
		| STRUCT STRING	name_list {
		  while ($3 != NULL) {
		    nlist = $3;
		    VarDeclaration(FindStructType($2), $3);
		    $3 = $3->next();
		  }
		}
		;
type_name:	UNSIGNED CHAR	{
		  $$ = InitPlainType(UCHAR_NAME, CHAR_SIZE, CHAR_ALIGN);
		}
		| CHAR		{
		  $$ = InitPlainType(CHAR_NAME, CHAR_SIZE, CHAR_ALIGN);
		}
		| UNSIGNED SHORT INT	{
		  $$ = InitPlainType(USHORT_NAME, SHORT_SIZE, SHORT_ALIGN);
		}
		| UNSIGNED SHORT	{
		  $$ = InitPlainType(USHORT_NAME, SHORT_SIZE, SHORT_ALIGN);
		}
		| SHORT INT		{
		  $$ = InitPlainType(SHORT_NAME, SHORT_SIZE, SHORT_ALIGN);
		}
		| SHORT			{
		  $$ = InitPlainType(SHORT_NAME, SHORT_SIZE, SHORT_ALIGN);
		}
		| UNSIGNED LONG INT	{
		  $$ = InitPlainType(ULONG_NAME, LONG_SIZE, LONG_ALIGN);
		}
		| UNSIGNED LONG	{
		  $$ = InitPlainType(ULONG_NAME, LONG_SIZE, LONG_ALIGN);
		}
		| UNSIGNED INT	{
		  $$ = InitPlainType(UINT_NAME, INT_SIZE, INT_ALIGN);
		}
		| LONG INT	{
		  $$ = InitPlainType(LONG_NAME, LONG_SIZE, LONG_ALIGN);
		}
		| LONG FLOAT	{
		  $$ = InitPlainType(DOUBLE_NAME, DOUBLE_SIZE, DOUBLE_ALIGN);
		}
		| LONG		{
		  $$ = InitPlainType(LONG_NAME, LONG_SIZE, LONG_ALIGN);
		}
		| INT		{
		  $$ = InitPlainType(INT_NAME, INT_SIZE, INT_ALIGN);
		}
		| UNSIGNED	{
		  $$ = InitPlainType(UINT_NAME, INT_SIZE, INT_ALIGN);
		}
		| FLOAT		{
		  $$ = InitPlainType(FLOAT_NAME, FLOAT_SIZE, FLOAT_ALIGN);
		}
		| DOUBLE	{
		  $$ = InitPlainType(DOUBLE_NAME, DOUBLE_SIZE, DOUBLE_ALIGN);
		}
		| STRING	{
		  $$ = FindType($1);
		}
		;
calc:		NUMBER			{ $$ = $1; }
		| '(' calc ')'		{ $$ = $2; }
		| '-' calc %prec NEG	{ $$ = $2->neg(); }
		| calc '+' calc		{ $$ = $1->calc('+', $3); }
		| calc '-' calc		{ $$ = $1->calc('-', $3); }
		| calc '*' calc		{ $$ = $1->calc('*', $3); }
		| calc '/' calc		{ $$ = $1->calc('/', $3); }
		| calc '%' calc		{ $$ = $1->calc('%', $3); }
		;
name_list:	STRING '[' calc ']' ',' name_list	{
		  $$ = new NameList($1, $3, $6);
		}
		| STRING ',' name_list	{
		  $$ = new NameList($1, $3);
		}
		| STRING '[' calc ']'	{
		  $$ = new NameList($1, $3);
		}
		| STRING	{
		  $$ = new NameList($1);
		}
		;
%%

TPtr
CindyYACC::FindType(const char *nam)
{
  TPtr typ;

  typ = Types;
  while (typ != NULL) {
    if (!strcmp(typ->name, nam)) return(typ);
    typ = typ->next;
  }
  if (typ == NULL) {
    std::cerr << "CindyYACC: unknown type " << nam << std::endl;
    exit(errno);
  }
  return(typ);
}

TPtr
CindyYACC::FindStructType(const char *nam)
{
  TPtr typ;

  typ = Structs;
  while (typ != NULL) {
    if (!strcmp(typ->name, nam)) return(typ);
    typ = typ->next;
  }
  if (typ == NULL) {
    std::cerr << "CindyYACC: unknown struct " << nam << std::endl;
    exit(errno);
  }
  return(typ);
}

TPtr
CindyYACC::InitType(TPtr knowntyp, NameList *nlst)
{
  TPtr typ;

  typ = Types;
  while (typ != NULL) {
    if (!strcmp(typ->name, nlst->name())) {
      std::cerr << "CindyYACC: redeclaration of type "
		<< nlst->name() << std::endl;
      exit(errno);
    }
    typ = typ->next;
  }

  typ = Types;
  if (typ == NULL)
    Types = typ = new Type;
  else {
    while (typ->next != NULL)
      typ = typ->next;
    typ = typ->next = new Type;
  }
  typ->name   = new char[strlen(nlst->name())+1];
  strcpy(typ->name, nlst->name());
  typ->sname  = NULL;
  typ->member = knowntyp->member;
  typ->len    = knowntyp->len;
  typ->align  = knowntyp->align;
  if (knowntyp->array) {
    typ->array = (nlst->array() ? nlst->array() : 1) * knowntyp->array;
  } else {
    if (!(typ->array = nlst->array())) typ->sname = knowntyp->sname;
  }
  typ->next = NULL;

  return(typ);
}

TPtr
CindyYACC::InitStructType(const char *nam, Member *mlst)
{
  TPtr typ;

  if (nam[0]) {
    typ = Structs;
    while (typ != NULL) {
      if (!strcmp(typ->name, nam)) {
	std::cerr << "CindyYACC: redeclaration of struct " << nam << std::endl;
	exit(errno);
      }
      typ = typ->next;
    }
  }

  typ = Structs;
  if (typ == NULL)
    Structs = typ = new Type;
  else {
    while (typ->next != NULL)
      typ = typ->next;
    typ = typ->next = new Type;
  }
  typ->name = new char[strlen(nam)+1]; strcpy(typ->name, nam);
  typ->member = mlst;
  typ->array = typ->len = 0;
  while (mlst != NULL) {
    typ->len += mlst->type()->len * (mlst->array() ? mlst->array() : 1);
    mlst = mlst->next();
  }
  typ->next = NULL;

  return(typ);
}

TPtr
CindyYACC::InitPlainType(const char *nam, int size, int algn)
{
  TPtr typ;

  typ = Types;
  while (typ != NULL) {
    if (!strcmp(typ->name, nam)) return(typ);
    typ = typ->next;
  }

  typ = Types;
  if (typ == NULL)
    Types = typ = new Type;
  else {
    while (typ->next != NULL)
      typ = typ->next;
    typ = typ->next = new Type;
  }
  typ->name = typ->sname = new char[strlen(nam)+1];
  strcpy(typ->name, nam);
  typ->len = size;
  typ->align = algn;
  typ->array = 0;
  typ->member = NULL;
  typ->next = NULL;

  return(typ);
}

VPtr
CindyYACC::VarDeclaration(TPtr typ, NameList *nlst)
{
  VPtr var;

  var = Vars;
  while (var != NULL) {
    if (!strcmp(var->name, nlst->name())) {
      std::cerr << "CindyYACC: var " << nlst->name() << " previously defined\n";
      exit(errno);
    }
    var = var->next;
  }

  var = Vars;
  if (var == NULL)
    Vars = var = new Var;
  else {
    while (var->next != NULL)
      var = var->next;
    var = var->next = new Var;
  }
  var->name = new char[strlen(nlst->name())+1];
  strcpy(var->name, nlst->name());
  var->typ = typ;
  var->array = nlst->array();
  var->next = NULL;

  return(var);
}

int
PrintVars(char *out, const char *exp, VPtr var, char *progname)
{
  std::ostream *fout = NULL;

  if (out == NULL) return 0;

  if (strcmp(out, "-")) {
    if ((fout = new std::ofstream(out)) == NULL) {
      perror(progname);
      exit(errno);
    }
  }
  else fout = &std::cout;

  *fout << "/" << "*" << std::endl;

  while (var != NULL) {
    if (exp == NULL) break;
    if (!strcmp(var->name, exp)) break;
    var = var -> next;
  }

  if (var == NULL) {
    std::cerr << progname << ": " << exp << " not defined" << std::endl;
    exit(errno);
  }

  Vrekursion(fout, var->typ, var->name);

  *fout << "*" << "/" << std::endl;

  return 0;
}

int
Vrekursion(std::ostream *fout, TPtr typ, const char *nam)
{
  char name[256];
  Member *mem;

  if (typ->member == NULL) {
    *fout << "  " << typ->name << '\t' << nam << ";\n";
    return(0);
  }

  mem = typ->member;
  while (mem != NULL) {
    if (mem->array())
      sprintf(name, "%s.%s[%d]", nam, mem->cname(), mem->array());
    else
      sprintf(name, "%s.%s", nam, mem->cname());
    Vrekursion(fout, mem->type(), name);
    mem = mem->next();
  }

  return(0);
}
