%name hminter
%{
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hmbook.h"
#include "hmplot.h"

void   help(), readfile(), yyerror(), appendfile();
int    yylex();

extern HIST *emptyhist;
extern int linenumber;
extern char *filename;

typedef struct histid { HIST *id; int nr; struct histid *next; } HISTID;
HISTID *firsthist = NULL;
HISTID *lasthist = NULL;

void addhist(int nr, HIST *id)
{ 
  if (!firsthist) 
    firsthist = lasthist = malloc(sizeof(HISTID));
  else {
    lasthist->next = malloc(sizeof(HISTID));
    lasthist = lasthist->next;
  }
  lasthist->next = NULL;
  lasthist->id = id;
  lasthist->nr = nr;
}

HIST *findnr(int nr) 
{
  HISTID *hh = firsthist;
  char err[255];
  while (hh) {
    if (hh->nr == nr)
      return hh->id;
    hh=hh->next;
  }
  sprintf(err, "Can't find histogram %d", nr);
  yyerror(err);
  return emptyhist;
}

FLOAT *newfloat(FLOAT value) 
{
  FLOAT *f = (FLOAT*)malloc(sizeof(FLOAT));
  *f = value;
  return f;
}

void assign(int nr, HIST *new) 
{
  HISTID *hh = firsthist;
  while (hh) {
    if (hh->nr == nr) {
      hh->id = new;
      return;
    }
    hh=hh->next;
  }
  addhist(nr, new);
}

void list()
{
  HISTID *id = firsthist;
  while (id) {
    printf("%3d   %s\n", id->nr, id->id->title);
    id = id->next;
  }
}

#define ifs(s1,functio) if(!strcmp(s1, functio))

%}

%union{
  int   *histnr;
  FLOAT *numi;
  char  *stri;
  HIST  *histi;
}

%token <numi> NUM
%token <stri> STRNG
%token <stri> COMMAND
%left '-' '+'
%left '*' '/' '&' '|'
%left '^'
%left NEG
%type <numi>  numExpr
%type <stri>  stringExpr
%type <histi> histExpr histogram

%%

input:    /* empty */
| input line;

line:
'\n'                 { HMFlush(); linenumber++; }
| Expression '\n'    { HMFlush(); linenumber++; }
| error '\n'         { yyerrok; linenumber++; }  
| '#' NUM STRNG      { linenumber = (int)*$2-1; filename = $3; }
| '#' NUM STRNG NUM  { linenumber = (int)*$2-1; filename = $3; }
| ';'                { }
| Expression ';'            { }
| error ';'          { yyerrok; };

Expression:      
'@' NUM '=' histExpr { assign(*$2, $4);}
| COMMAND {
  ifs($1, "LIST")  list();
  ifs($1, "DIR")   list();
  ifs($1, "LS")    list(); 
  ifs($1, "WAIT")  getchar();
  ifs($1, "SAME")  HMSame();
  ifs($1, "CLOSE") HMClose();
  ifs($1, "HELP")  help();
  ifs($1, "PIPE")  { HMClose(); HMPipe(); }
}      
| COMMAND stringExpr {
  HIST *id = HMFindTitle($2);  
  if (!id) id = emptyhist;
  ifs($1, "PLOTFILE")  {HMClose();HMPlotFile($2);}
  ifs($1, "READFILE")  readfile($2);
  ifs($1, "APPEND")    appendfile($2);
  ifs($1, "TITLE")     HMTitle($2);
  ifs($1, "EXPLAIN")   HMExplain($2);
  ifs($1, "HISTOGRAM") HMHistogram   (id);
  ifs($1, "DATAERROR") HMDataError   (id);
  ifs($1, "GRAY")      HMGray        (id);
  ifs($1, "COLOR")     HMColor       (id);
  ifs($1, "SCATTER")   HMScatter     (id);
  ifs($1, "MEANX")     HMPlotMeanX   (id);
  ifs($1, "MEANY")     HMPlotMeanY   (id);
  ifs($1, "LEGO")      HMLego        (id);
  ifs($1, "SURFACE")   HMSurface     (id);
  ifs($1, "FUNCTION")  HMFunction    (id);
  ifs($1, "FILLERROR") HMFillError   (id);
  ifs($1, "CLEARERROR")HMClearError  (id);
  ifs($1, "VARIANCE")  HMBookVariance(id);
}
| COMMAND stringExpr histogram {
  ifs($1,"WRITEONE")  HMWriteSingleHistogram($2, $3);
  ifs($1,"WRITEASCII") {
    FILE *fp = fopen($2,"w");
    HMWriteAscii(fp, $3);
    fclose(fp);
  }
  ifs($1,"WRITEASCIITUPLE") {  // das geht nicht, ist nicht eindeutig
    FILE *fp = fopen($2,"w");
    HMWriteAsciiTuple(fp, $3);
    fclose(fp);
  }
}
| COMMAND numExpr {
  ifs($1,"DELETE")    HMDelete      (findnr(*$2));
  ifs($1,"HISTOGRAM") HMHistogram   (findnr(*$2));
  ifs($1,"GRAY")      HMGray        (findnr(*$2));
  ifs($1,"COLOR")     HMColor       (findnr(*$2));
  ifs($1,"MEANX")     HMPlotMeanX   (findnr(*$2));
  ifs($1,"MEANY")     HMPlotMeanY   (findnr(*$2));
  ifs($1,"LEGO")      HMLego        (findnr(*$2));
  ifs($1,"DELETE")    HMDelete      (findnr(*$2));
  ifs($1,"BOX")       HMBox         (findnr(*$2));
  ifs($1,"DATAERROR") HMDataError   (findnr(*$2));
  ifs($1,"SCATTER")   HMScatter     (findnr(*$2));
  ifs($1,"SURFACE")   HMSurface     (findnr(*$2));
  ifs($1,"FUNCTION")  HMFunction    (findnr(*$2));
  ifs($1,"FILLERROR") HMFillError   (findnr(*$2));
  ifs($1,"CLEARERROR")HMClearError  (findnr(*$2));
  ifs($1,"VARIANCE")  HMBookVariance(findnr(*$2));
  ifs($1,"SETREBIN")  HMSetRebin(*$2);   
  ifs($1,"MINIMUMX")  HMMinimumX(*$2);
  ifs($1,"MINIMUMY")  HMMinimumY(*$2);
  ifs($1,"MINIMUMZ")  HMMinimumZ(*$2);
  ifs($1,"MAXIMUMX")  HMMaximumX(*$2);
  ifs($1,"MAXIMUMY")  HMMaximumY(*$2); 
  ifs($1,"MAXIMUMZ")  HMMaximumZ(*$2);
}
| COMMAND numExpr numExpr { 
  ifs($1,"SETOPTION") HMSetOption((int)*$2,*$3);
  ifs($1,"ZONE")      HMZone((int)*$2,(int)*$3,1);
}
| COMMAND numExpr stringExpr { 
  ifs($1,"HTITLE")    findnr(*$2)->title =  $3;
}
| COMMAND numExpr numExpr stringExpr { 
  ifs($1,"TEXT")      HMText(*$2,*$3,$4);
}
| COMMAND numExpr stringExpr numExpr { 
  ifs($1,"FIT") {
    double *par = (double *) calloc(*$4, sizeof(double));
    int i=0;
    for(i=0; i < *$4; i++) par[i]=0;
    if (HMScanFunction($3, par, *$4))
      fprintf(stderr,"Can't scan function \"%s\"\n", $3);
    else 
      HMFit1(findnr(*$2), HMFit1Function, par, *$4); 
  }
}
| COMMAND numExpr stringExpr stringExpr { 
  HIST *id = findnr(*$2);
  ifs($1, "XAXIS") { id->labelx = $3;    id->unitx = $4; }
  ifs($1, "YAXIS") { id->labely = $3;    id->unity = $4; }
}
| COMMAND numExpr numExpr numExpr
{ 
  ifs($1,"ZONE")       HMZone(*$2, *$3, *$4);
  ifs($1,"SETFILLRGB") HMSetFillRGB(*$2, *$3, *$4);
}
| COMMAND numExpr numExpr numExpr numExpr { 
  ifs($1,"RECTANGLE")  HMRectangle(*$2, *$3, *$4, *$5);
  ifs($1,"LINE")       HMLine(*$2, *$3, *$4, *$5);
}
| COMMAND stringExpr numExpr numExpr numExpr { 
  ifs($1,"INCLUDEPS")   HMIncludePS($2, *$3, *$4, *$5);
}
| COMMAND stringExpr numExpr { 
  ifs($1,"SETOPTION")   HMSetOptionName($2, *$3);
}
| COMMAND stringExpr numExpr numExpr { 
  ifs($1,"EXPLAIN")    HMExplainOffset($2, *$3, *$4);
};

histExpr:
'(' histExpr ')'        { $$ = $2; }
| histExpr '+' histExpr { $$ = HMOperation($1, '+', $3, $1->title, "", "");}  
| histExpr '-' histExpr { $$ = HMOperation($1, '-', $3, $1->title, "", "");}
| histExpr '/' histExpr { $$ = HMOperation($1, '/', $3, $1->title, "", "");} 
| histExpr '*' histExpr { $$ = HMOperation($1, '*', $3, $1->title, "", "");}
| histExpr '&' histExpr { $$ = HMConvolute($1, $3);       } 
| histExpr '|' histExpr { $$ = HMDeconvolute($1, $3);     } 

| histExpr '*' numExpr   { $$ = HMScalar   ($1, '*', *$3); }
| histExpr '/' numExpr   { $$ = HMScalar   ($1, '/', *$3); }
| histExpr '+' numExpr   { $$ = HMScalar   ($1, '+', *$3); }
| histExpr '-' numExpr   { $$ = HMScalar   ($1, '-', *$3); }
| histExpr '^' numExpr   { $$ = HMScalar   ($1, '^', *$3); }

| numExpr '*' histExpr   { $$ = HMScalar   ($3, '*', *$1); }
| numExpr '/' histExpr   { $$ = HMScalar   ($3, '/', *$1); }
| numExpr '+' histExpr   { $$ = HMScalar   ($3, '+', *$1); }
| numExpr '-' histExpr   { $$ = HMScalar   ($3, '-', *$1); }

| COMMAND histogram NUM { ifs($1,"SMOOTH") $$ = HMSmooth($2, *$3);}
| histogram;

histogram:
'@' NUM {   $$ = findnr(*$2);}
| stringExpr {
  if (index($1,':'))
    $$ = HMFind($1);
  else
    $$ = HMFindTitle($1);
  if (!$$) { 
    char err[255];
    sprintf(err, "Histogram \"%s\" not defined.", $1);
    yyerror(err);
    $$ = emptyhist;
  }
};

numExpr: 
NUM
| '(' numExpr ')'      { $$ = $2;}
| '(' '-' NUM ')'      { $$ = newfloat(    - *$3); }
| numExpr '+' numExpr  { $$ = newfloat(*$1 + *$3); }
| numExpr '-' numExpr  { $$ = newfloat(*$1 - *$3); }
| numExpr '*' numExpr  { $$ = newfloat(*$1 * *$3); }
| numExpr '/' numExpr  { $$ = newfloat(*$1 / *$3); };

stringExpr: 
STRNG |
stringExpr '@' stringExpr { 
  $$ = malloc(strlen($1)+strlen($3) + 1);
  strcpy($$,$1);
  strcat($$,$3);
};
%%













