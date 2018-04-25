//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mit.edu
// MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/TMA/ftmInit.y,v 1.7 2006-12-08 16:02:20 distler Exp $
//
// YACC parser (bison++ version)
// initialize the Fast Trace Machine
//

%name   ftmInitYACC

%define LSP_NEEDED
%define ERROR_BODY		=0
%define LEX_BODY		=0
%define MEMBERS			protected: \
				  ftmEntry * ftmEntries; \
				  Number * xOffsets; \
				  Number * thOffsets; \
				  Number * yOffsets; \
				  Number * phOffsets;
%define CONSTRUCTOR_CODE	ftmEntries = NULL; \
				xOffsets = thOffsets = \
				yOffsets = phOffsets = NULL;

%header{
# include <math.h>
# include <iostream>

  class ftmEntry {
    int powers[4];
    double xtyp, factors[5];
    ftmEntry * next, * mom, * dad;
  public:
    ftmEntry(int, int, int, int, double, double, double, double, double);
    ~ftmEntry();
    ftmEntry * putEntry(ftmEntry *);
    ftmEntry * getNext();
    int operator() (int);
    double operator[] (int);
    int operator> (ftmEntry);
    double evaluate(int i) { return xtyp*factors[i]; }
    int initialize(double *);
    int findParents(ftmEntry *);
  };

  class Number {
    double value;
    Number * next;
  public:
    Number(double);
    ~Number();
    void putNumber(Number *);
    Number * getNext();
    double operator()();
  };

%}

%union {
  int     num;
  double  val;
  Number *set;
}

%start list
%token XOFFSET THOFFSET YOFFSET PHOFFSET EOL
%token <val> VALUE
%type  <set> valset

%%

list:	| list entry
	| list offset
	| list error { yyerrok; }
	;
entry:  EOL { ; }
	| VALUE VALUE VALUE VALUE  VALUE VALUE VALUE VALUE VALUE EOL {
	  ftmEntry * newEntry =
	    new ftmEntry((int)$1,(int)$2,(int)$3,(int)$4,$5,$6,$7,$8,$9);
	  if (ftmEntries) {
	    ftmEntries = ftmEntries->putEntry(newEntry);
	  } else {
	    ftmEntries = newEntry;
	  }
	}
	| VALUE VALUE VALUE VALUE VALUE EOL {
	  ; // std::cerr << "FastTM : old style entry - skipped" << std::endl;
	}
	;
valset: VALUE                       { $$ = new Number($1); }
	| valset VALUE              { $$ = $1; $$->putNumber(new Number($2)); }
	;
offset: XOFFSET ':' valset EOL     { xOffsets = $3; }
	| THOFFSET ':' valset EOL  { thOffsets = $3; }
	| YOFFSET ':' valset EOL   { yOffsets = $3; }
	| PHOFFSET ':' valset EOL  { phOffsets = $3; }
	;

%%

ftmEntry::ftmEntry(int ix, int ith, int iy, int iph,
		   double dp, double th, double y0, double ph, double len)
{
  factors[0] = dp;  powers[0] = ix;
  factors[1] = th;  powers[1] = ith;
  factors[2] = y0;  powers[2] = iy;
  factors[3] = ph;  powers[3] = iph;
  factors[4] = len; xtyp = 0.0;
  next = mom = dad = NULL;
}

ftmEntry::~ftmEntry()
{
  delete next;
}

ftmEntry *
ftmEntry::putEntry(ftmEntry * entry)
{
  if (*this > *entry) {
    entry->next = this;
    return entry;
  }

  ftmEntry * tmp;
  for (tmp=this; tmp->next; tmp=tmp->next) {
    if (*tmp->next > *entry) {
      entry->next = tmp->next;
      tmp->next = entry;
      return this;
    }
  }

  tmp->next = entry;
  return this;
}

ftmEntry *
ftmEntry::getNext()
{
  return next;
}

int
ftmEntry::operator()(int index)
{
  return ((0<=index)&&(index<4)) ? powers[index] : 0;
}

double
ftmEntry::operator[](int index)
{
  return ((0<=index)&&(index<5)) ? factors[index] : 0;
}

int
ftmEntry::operator> (ftmEntry b)
{
  for (int i=0; i<4; i++) {
    if (powers[i]>b.powers[i]) return 1;
    if (powers[i]<b.powers[i]) return 0;
  }
  return 0;
}

int
ftmEntry::initialize(double *coo)
{
  if (!mom || !dad) {
    xtyp = 1.0;
    for (int i=0; i<4; i++)
      if (powers[i]) xtyp *= coo[i];
  } else xtyp = mom->xtyp * dad->xtyp;

  return 0;
}

int
ftmEntry::findParents(ftmEntry *entries)
{
  int i, found;
  ftmEntry *myMom, *myDad;

  for (myMom=entries; myMom && myMom!=this; myMom=myMom->next) {
    for (myDad=entries; myDad && myDad!=this; myDad=myDad->next) {
      for (found=1,i=0; i<4; i++)
	found &= ((*myMom)(i)+(*myDad)(i) == powers[i]);
      if (found) {
	mom = myMom;
	dad = myDad;
	return 0;
      }
    }
  }

  return -1;
}

Number::Number(double val)
{
  value = val;
  next = NULL;
}

Number::~Number()
{
  delete next;
}

void
Number::putNumber(Number * num)
{
  Number * last = this;

  while (last->next) last = last->next;

  last->next = num;
}

Number *
Number::getNext()
{
  return next;
}

double
Number::operator()()
{
  return value;
}
