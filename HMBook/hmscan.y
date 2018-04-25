%name hmscan
%token NUM NAME FUNC TTRUE TFALSE CPI CE XVAR YVAR INDEX
%{

#define YY_hmscan_STYPE HMexpression*

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmbook.h"
#include "evaluate.h"
#include "Functions.h"

static HMexpression *one     = new HMliteral<double>(1);
static HMexpression *zero    = new HMliteral<double>(0);
static HMexpression *constpi = new HMliteral<double>(3.14159265358979323846);
static HMexpression *conste  = new HMliteral<double>(2.7182818284590452354);
HMexpression *result;
HMexpression *xval = new HMliteral<double>(0);
HMexpression *yval = new HMliteral<double>(0);
HMexpression **para;
template <> void *HMliteral<double>::eval() {returnv double(value);}

template <> void *HMbin<double>::eval()
     {returnv double(operation(left->evaluate(),right->evaluate()));}

template <> void *HMfunc<double,double>::eval() 
                      {returnv double(operation(argument->evaluate()));}
double constant;
int    maxindex;
char   *funcname;

%}

/* BISON Declarations */
%left '-' '+'
%left '*' '/'
%left NEG     /* unary minus */
%right '^'    /* exponentiation        */

/* Grammar follows */
%%
 
input:   exp                 { result = $1; }

exp:     INDEX               { $$ = new HMliteral<double>(constant);}
         | 'p' '[' INDEX ']'{ int arrayindex = (int) constant;
                               if (arrayindex>maxindex || arrayindex<0) 
                                  yyerror("Index out of bounds"); 
                               $$ = para[arrayindex];}
         | NUM               { $$ = new HMliteral<double>(constant);}
         | XVAR              { $$ = xval; }
         | YVAR              { $$ = yval; }
         | CPI               { $$ = constpi;}
         | CE                { $$ = conste;}
         | TTRUE             { $$ = one;}
         | TFALSE            { $$ = zero;}
         | '(' exp ')'       { $$ = $2;}
         | exp '+' exp       { $$ = new HMbin<double>($1,$3,HMop_add);}
         | exp '-' exp       { $$ = new HMbin<double>($1,$3,HMop_sub);}
         | exp '*' exp       { $$ = new HMbin<double>($1,$3,HMop_mult);}
         | exp '/' exp       { $$ = new HMbin<double>($1,$3,HMop_div);}
         | exp '^' exp       { $$ = new HMbin<double>($1,$3,pow);}
         | '-' exp %prec NEG { $$ = new HMbin<double>(zero,$2,HMop_sub);}
         | FUNC '(' exp ')'  { int pos;
                               pos = 0; 
                               while(HMfuncdef[pos].name?
				     strcmp(funcname, HMfuncdef[pos].name):0)
                                     pos++;
			       if (!HMfuncdef[pos].name)
                               { char err[255];
                                 sprintf(err,"Unkown Function '%s'",funcname); 
                                 yyerror(err);
                               }
			       delete(funcname);
			       $$ = new HMfunc<double,double> 
				 ($3,HMfuncdef[pos].func); };
%%








