%name lex
%{
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "hmscan.parse.h"
#include "evaluate.h"

static hmscan *hallo;
static char* inputstream;
static unsigned int pos=0;
extern double constant;
extern int arrayindex;
extern char *funcname;
 
#undef YY_INPUT
#define YY_INPUT(buf, result, max_size)\
{ if (pos==strlen(inputstream)) result=YY_NULL; else \
  { strcpy((char *)buf, inputstream); \
    result = pos = strlen(inputstream); \
  } \
}
%}

INDEX    [0-9]+
FRACT    [0-9]*|"."[0-9]*|[0-9]*+"."[0-9]*
EXPO     "E"|"e"
FLOAT    {FRACT}|{FRACT}{EXPO}[0-9]+ 
NAME     [a-zA-Z_][a-zA-Z_0-9]*

%%
"+"|"-"|"*"|"/"|"^" return yytext[0];
"{"|"}"|"("|")"     return yytext[0];
"["|"]"             return yytext[0];
"true"              return hallo->TTRUE;
"false"             return hallo->TFALSE;
"pi"                return hallo->CPI;
"e"                 return hallo->CE;
"x"                 return hallo->XVAR;
"y"                 return hallo->YVAR;
"p"                 return yytext[0];
{INDEX}             { constant = atof((char *)yytext); return hallo->INDEX;  }
{FLOAT}             { constant = atof((char *)yytext); return hallo->NUM;  }
{NAME}              { funcname = new char[strlen((char *)yytext)+1];
                      strcpy(funcname,(char *)yytext);
                      return hallo->FUNC;
                    }
[ \t\n]+            /* ignore white space */
.                   printf( "Unrecognized character: %s\n", (char *)yytext );
%%
	   

static lex    *lexi; 

void hmscan::yyerror(char *msg)
{ fprintf(stderr,"HMScan: %s\n", msg);
}

int hmscan::yylex()
{
  return lexi->yylex();
}

extern HMexpression *result; 
extern HMexpression *xval;
extern HMexpression *yval;
extern HMexpression **para;
extern int maxindex;

extern "C" void HMSetFunctionParameter(double p[])
{ for(int i=0;i<maxindex;i++) ((HMliteral<double> *)para[i])->value = p[i];
}

extern "C" double HMParsedFunction(double x, double y)
{ ((HMliteral<double> *)xval)->value=x;
  ((HMliteral<double> *)yval)->value=y;
  result->reset();
  return result->evaluate(); 
}

extern "C" double HMFit1Function(double p[], double x)
{ HMSetFunctionParameter(p);
  return HMParsedFunction(x,0); 
}

extern "C" int HMScanFunction(char *func, double p[], int n)
{ maxindex = n;
  para = new HMexpression *[n];
  for(int i=0; i<maxindex; i++) para[i] = new HMliteral<double>(p[i]);
  inputstream = func; 
  pos=0;
 
  hallo = new hmscan;
  lexi = new lex;
  return hallo->yyparse();
  delete hallo;
  delete lexi;
}
 
/*
int main(int, char *argv[])
{ double p[] = {1,2,3,4};
  if (HMScanFunction(argv[1],p,4))
      printf("%f\n", result->evaluate());

  double x;
  for (x=0;x<10;x++) 
    printf("%f %f\n", x, HMParsedFunction(x, 0));
  
  int i;
  for(i=0;i<100000;i++) HMParsedFunction(x, 0) ;
  return 0;
}
*/





