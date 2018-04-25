//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                           mailto:distler@mit.edu
// MIT, Lab for Nuclear Science, 26-402B        tel.   (617) 253-6997
// Cambridge, MA 02139                          fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaMIT/StripChart.y,v 2.2 2002-12-12 12:24:12 distler Exp $
//
// YACC parser (bison++ version)
// initialize ColaMIT StripChart
//

%name	StripChartYACC

%define LSP_NEEDED
%define ERROR_BODY	=0
%define LEX_BODY	=0
%define CONSTRUCTOR_CODE  atree=NULL;
%define MEMBERS		protected:\
			  AquaTree *atree;

%header{
# ifdef HAVE_CONFIG_H
# include "config.h"
# endif /* HAVE_CONFIG_H */
# include "Aqua/AquaTree.h"
# include "HMBook/hmbook.h"
# include <iostream>
# include <cmath>
# include <cstring>

  class ExprBase
  {
  public:
    ExprBase() { ; }
    virtual double operator()() = 0;
    virtual int ok() = 0;
  };

  class ExprItem : public ExprBase
  {
    AquaNode *item;
  public:
    ExprItem(const char *name, AquaTree *atree) { item = atree->find(name); }
    virtual double operator()() { return item->scalar(); };
    virtual int ok() { return item->ok(); }
    virtual ~ExprItem() { ; }
  };

  class ExprConst : public ExprBase
  {
    double value;
  public:
    ExprConst(double val) { value = val; }
    virtual double operator()() { return value; };
    virtual int ok() { return 1; }
    virtual ~ExprConst() { ; }
  };

  class ExprCalc : public ExprBase
  {
    char op;
    ExprBase *left, *right;
  public:
    ExprCalc(ExprBase *l, char o, ExprBase *r) {
      left = l; op = o; right = r;
    }
    virtual double operator()() {
      switch (op) {
      case '+': return (*left)() + (*right)(); break;
      case '-': return (*left)() - (*right)(); break;
      case '*': return (*left)() * (*right)(); break;
      case '/': return (*left)() / (*right)(); break;
      case '^': return pow((*left)(), (*right)()); break;
      }
      return 0;
    };
    virtual int ok() { return left->ok() && right->ok(); }
    virtual ~ExprCalc() { delete left; delete right; }
  };

  class StripChart
  {
  protected:
    static StripChart *first;
    static StripChart *last;
    StripChart *next;
    ExprBase *expr;
    HIST *hist;
    char *name;
    int n, nmax;
    float *x, *y;
    void fill(float, float);
  public:
    static time_t startTime;
    StripChart(const char *, ExprBase *);
    static int handle(time_t);
  };

%}

%union {
  char     *str;
  double    val;
  ExprBase *exp;
}

%start	list
%token	<str>	KEY
%token	<val>	VALUE
%type	<exp>	expr
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
expr:	VALUE			{ $$ = new ExprConst($1); }
	| KEY			{ $$ = new ExprItem($1, atree); }
	| '(' expr ')'		{ $$ = $2; }
	| expr '^' expr		{ $$ = new ExprCalc($1, '^', $3); }
	| expr '*' expr		{ $$ = new ExprCalc($1, '*', $3); }
	| expr '²'		{ $$ = new ExprCalc($1, '^',
						    new ExprConst(2)); }
	| expr '³'		{ $$ = new ExprCalc($1, '^',
						    new ExprConst(3)); }
	| expr '/' expr		{ $$ = new ExprCalc($1, '/', $3); }
	| expr '+' expr		{ $$ = new ExprCalc($1, '+', $3); }
	| expr '-' expr		{ $$ = new ExprCalc($1, '-', $3); }
	| '-' expr %prec NEG	{ $$ = new ExprCalc(new ExprConst(0),
						    '-', $2); }
	| '+' expr %prec POS	{ $$ = $2; }
	;
sep:	'\n'
	| ';'
	;
decl:	sep
	| KEY sep {
	  if (new StripChart($1, new ExprItem($1, atree)));
	  delete $1;
	}
	| KEY '=' expr sep {
	  if (new StripChart($1, $3));
	  delete $1;
	}
	;

%%

StripChart *StripChart::first = NULL;
StripChart *StripChart::last  = NULL;
time_t StripChart::startTime = 0;


StripChart::StripChart(const char *pname, ExprBase *exp)
{
  expr = exp;
  next = NULL;
  name = new char[strlen(pname)+1];
  strcpy(name, pname);

  if (first == NULL) first = this;
  if (last != NULL) last->next = this;
  last = this;

  char *title = new char[strlen(pname)+1];
  *title = 0;
  if (!strncmp("oops1.", pname, 6)) {
    strcpy(title, pname+6);
    char *point = title;
    while (*point) {
      if (*point == '.') *point = '/';
      if (*point == '_') *point = ' ';
      point++;
    }
  }

  hist = HMBook1(title, "t", "", "hours", "", 100, 0.0, 1.0);

  x = new float[1024];
  y = new float[1024];
  nmax = 1024;
  n = 0;

  // delete title;
}

int
StripChart::handle(time_t now)
{
  StripChart *stripChart = first;

  while (stripChart) {
    if (stripChart->expr->ok()) {
      stripChart->fill((now-startTime)/3600.0, (*stripChart->expr)());
    }
    stripChart = stripChart->next;
  }

  return 0;
}

void 
StripChart::fill(float xx, float yy)
{
  if (!hist) return;
  n++;
  if (n == nmax) {
    nmax = (int) (nmax * 1.5);
    float *xn = new float[nmax];
    float *yn = new float[nmax];
    int i; for (i=0; i<n;i++) { xn[i] =x[i]; yn[i]=y[i];}
    delete x;
    delete y;
    x = xn;
    y = yn;
  }
  x[n]=xx;
  y[n]=yy;
  HMSetArray(hist,n,x,y);
}

