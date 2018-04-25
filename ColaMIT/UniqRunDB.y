//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                           mailto:distler@mit.edu
// MIT, Lab for Nuclear Science, 26-402B        tel.   (617) 253-6997
// Cambridge, MA 02139                          fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaMIT/UniqRunDB.y,v 2.4 2006-12-08 16:00:42 distler Exp $
//
// YACC parser (bison++ version)
// remove duplicate lines from run.db
//

%name	UniqRunDbYACC

%define LSP_NEEDED
%define ERROR_BODY	  =0
%define LEX_BODY	  =0
%define CONSTRUCTOR_CODE  defs=NULL;
%define MEMBERS		  protected:\
                            Definition *defs; std::ostream *out;
%header{
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
# include <iostream>
#ifdef HAVE_OSTREAM
# include <ostream>
#endif
# include <iosfwd>
# include <limits.h>
# include <cmath>
# include <ctime>
# include <cstring>

  class Parameter {
    double value;
    Parameter *next;
  public:
    Parameter(double val) { value = val; next = NULL; }
    ~Parameter() { delete next; }
    Parameter *add(Parameter *param) {
      if (next) {
	next->add(param);
      } else {
	next = param;
      }
      return this;
    }
    int getDim() {
      if (!next) return 1;
      return 1+next->getDim();
    }
    void putValues(double *val) {
      *val = value;
      if (next) next->putValues(val+1);
    }
  };

  class Definition {
    std::ostream *out;
    static long lastDate;
    int dimension;
    int hasChanged;
    double *value;
    char *string, *name;
    Definition *next;
    void init(char *);
  public:
    Definition(std::ostream *output, char *, double);
    Definition(std::ostream *output, char *, Parameter *);
    Definition(std::ostream *output, char *, char *);
    ~Definition();
    void add(Definition *);
    int check4Changes();
    int printChanges(long);
    friend std::ostream& operator<<(std::ostream& out, const Definition& def);
  };

%}

%union {
  char      *str;
  double     val;
  long       lval;
  Parameter *par;
}

%start	list
%token	EndOfFile
%token	<str>	KEY STRING
%token	<val>	VALUE
%token	<lval>	DATE
%type	<val>	exp
%type	<par>	params
%left	'-' '+'
%left	'*' '/'
%left	NEG POS
%right	'^'
%right	'²' '³'

%%

list:
	| list decl
	| list error	{ yyerrok; }
	;
exp:	VALUE
	| VALUE '°'
	| '(' exp ')'       { $$ = $2; }
	| exp '^' exp       { $$ = pow($1, $3); }
	| exp '*' exp       { $$ = $1 * $3; }
	| exp '²'           { $$ = $1 * $1; }
	| exp '³'           { $$ = $1 * $1 * $1; }
	| exp '/' exp       { $$ = $1 / $3; }
	| exp '+' exp       { $$ = $1 + $3; }
	| exp '-' exp       { $$ = $1 - $3; }
	| '-' exp %prec NEG { $$ =    - $2; }
	| '+' exp %prec POS { $$ =      $2; }
	;
params:	exp {
	  $$ = new Parameter($1);
	}
	| params ',' exp {
	  $$ = $1->add(new Parameter($3));
	}
	| params ',' '\n' exp {
	  $$ = $1->add(new Parameter($4));
	}
	;
sep:	'\n'
	| ';'
	;
decl:	sep
	| EndOfFile {
	  if (defs) if (defs->check4Changes()>0) defs->printChanges(LONG_MAX);
	}
	| DATE sep {
	  if (defs) if (defs->check4Changes()>0) defs->printChanges($1);
	}
	| KEY '=' exp sep {
	  Definition *dtmp = new Definition(out, $1, $3);
	  if (defs) defs->add(dtmp);
	  else defs = dtmp;
	}
	| KEY '=' STRING sep {
	  Definition *dtmp = new Definition(out, $1, $3);
	  if (defs) defs->add(dtmp);
	  else defs = dtmp;
	}
	| KEY '=' '{' params '}' sep {
	  Definition *dtmp = new Definition(out, $1, $4);
	  if (defs) defs->add(dtmp);
	  else defs = dtmp;
	}
	;

%%

long Definition::lastDate = 0;

void
Definition::init(char *key)
{
  dimension = -1;
  hasChanged = 1;
  name = key;
  next = NULL;
  string = NULL;
  value = NULL;
}

Definition::Definition(std::ostream* output, char *key, double val)
{
  out = output;
  init(key);
  dimension = 1;
  value = new double[1];
  *value = val;
}

Definition::Definition(std::ostream* output, char *key, Parameter *par)
{
  out = output;
  init(key);
  dimension = par->getDim();
  value = new double[dimension];
  par->putValues(value);
  delete par;
}

Definition::Definition(std::ostream* output, char *key, char *str)
{
  out = output;
  init(key);
  dimension = 0;
  string = str;
}

Definition::~Definition()
{
  delete value;
  delete string;
  delete name;
  delete next;
}

void
Definition::add(Definition *ndef)
{
  Definition *dtmp;

  for (dtmp = this; dtmp; dtmp = dtmp->next) {
    if (!strcmp(dtmp->name, ndef->name)) {
      if (dtmp->dimension == ndef->dimension) {
	if (dtmp->dimension) {
	  dtmp->hasChanged = 0;
	  for (int i=0; i<dtmp->dimension; i++)
	    dtmp->hasChanged |= (dtmp->value[i] != ndef->value[i]);
	} else {
	  dtmp->hasChanged = strcmp(dtmp->string, ndef->string);
	}
      } else {
	// this should never happen
	delete ndef; return;
      }
      if (dtmp->hasChanged) {
	if (dtmp->dimension) {
	  delete dtmp->value;
	  dtmp->value = ndef->value;
	  ndef->value = NULL;
	} else {
	  delete dtmp->string;
	  dtmp->string = ndef->string;
	  ndef->string = NULL;
	}
      }
      delete ndef; return;
    }
    if (!dtmp->next) {
      dtmp->next = ndef;
      return;
    }
  }
}

int
Definition::check4Changes()
{
  if (next) return hasChanged+next->check4Changes();
  return hasChanged;
}

int
Definition::printChanges(long date)
{
  if (lastDate > 0) {
    char dstr[16];
    struct tm *tms = localtime(&lastDate);
    sprintf(dstr,"[%02d%02d%02d%02d%02d%02d]", tms->tm_year, tms->tm_mon+1,
	    tms->tm_mday, tms->tm_hour, tms->tm_min, tms->tm_sec);
    *out << dstr << std::endl;
  }

  Definition *dtmp;
  for (dtmp = this; dtmp; dtmp = dtmp->next)
    if (dtmp->hasChanged) {
      *out << *dtmp;
      dtmp->hasChanged = 0;
    }
  
  return  lastDate = date;
}

std::ostream& operator<<(std::ostream& out, const Definition& def)
{
  out << '\t' << def.name << " = ";
  switch (def.dimension) {
  case 0:
    out << '"' << def.string << '"';
    break;
  case 1:
    out << *(def.value);
    break;
  default:
    out << "{" ;
    for (int i=0; i<def.dimension-1; i++) {
      out << def.value[i] << (!((i+1)%5) ? ",\n\t\t" : ", ");
    }
    out << def.value[def.dimension-1] << "}";
  }

  return out << std::endl;
}
