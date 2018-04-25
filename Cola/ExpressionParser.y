//                                                                    -*-c++-*-
// $Id: ExpressionParser.y 2585 2014-10-17 10:36:50Z miham $
//

%name lineparser

%header{
#include "Aqua/AquaTree.h"
#include "HMBook/hmbook.h"
#include "Cola/evaluate.h"
#include "Cola/ColaScalar.h"
#include "Cola/ntmode.h"
#include "Cola/Functions.h"
#include <stdio.h>
#include <stdlib.h>

///
class Cola_Counter 
{
  expression *var, *cut;
  char       *name, *format;
  double     value, normvalue;
  static Cola_Counter *last;
public:
  ///
  static Cola_Counter *first;
  ///
  Cola_Counter *next;
  ///
  expression *norm;
  ///
  expression *weight;
  ///
  Cola_Counter(expression *ct, expression *Cut, expression *Weight,
	       const char *Name, const char *Format = NULL); 
  ///
  ~Cola_Counter() { delete name; delete format; }
  ///
  static Cola_Counter *find(char *nme);
  ///
  void toString(char *string) {  sprintf(string, format, normvalue); };
  ///
  void reset() { if (var) var->reset(); 
                 if (cut) cut->reset(); 
                 if (norm) norm->reset(); 
  };
  ///
  static void reset_all();
  ///
  static void delete_all();
  ///
  void fill(double weight);
  ///
  static void fill_all(double weight);
};

///
class Cola_Fourvector
{
  Cola_Fourvector *next;
  static Cola_Fourvector *first;
  static Cola_Fourvector *last;
  char *name;
  expression *expr;
  double value;
public:
  ///
  Cola_Fourvector(char *nme, expression *express) {
    next = NULL;
    if (!first) {
      first = this;
    } else last->next = this;
    last = this;
    name = nme;
    expr = express;
  }

  ///
  void reset();
  ///
  static void reset_all();
  ///
  static void delete_all();
  ///
  static Cola_Fourvector *find(const char* name);
  ///
  static char *namefour(const char *name);
  ///
  static expression *findfour(const char *name);
};

///
class Cola_print
{
  ///
  Cola_print *next;
  FILE *filepointer;
public:
  ///
  static Cola_print *first;
  ///
  static Cola_print *last;

  ///
  expression *cut, *scalar;
  ///
  char *format;

  ///
  Cola_print(expression *ct, char *sc, char *frmt, FILE *fp); 
  ///
  Cola_print *First(); 
  ///
  void reset();
  ///
  static void reset_all(); 
  ///
  static void delete_all();
  ///
  void print(FILE *fp);
  ///
  static void print_all(FILE *fp);
};

///
class Cola_histogram
{
private:
  ///
  Cola_histogram *next;

public:
  ///
  static Cola_histogram *first;
  ///
  static Cola_histogram *last;
  ///
  static Cola_histogram *ch;
  ///
  static const char *zLabel;
  ///
  static const char *zUnit;
  ///
  expression *var1, *var2, *cut, *scal, *weight;
  ///
  double convert1, convert2;
  ///
  HIST *id;
  ///
  ~Cola_histogram() { HMDelete(id); }
  ///
  Cola_histogram(expression *cut, expression *weight,
		 char *title, 
		 char *scale, double min, double max, int nx,
		 char *lab=NULL, char *unit=NULL);
  ///
  Cola_histogram(expression *cut, expression *weight,
		 char *title, 
		 char *scale1, double min1, double max1,
		 int nx1,char *lab1,char *unit1,
		 char *scale2, double min2, double max2,
		 int nx2,char *lab2=NULL, char *unit2=NULL);
  ///
  Cola_histogram *First(); 
  ///
  void reset();
  ///
  static void reset_all();
  ///
  static void delete_all();
  ///
  void fill(double weight);
  ///
  static void fill_all(double weight);
  ///
  static Cola_histogram *find(char* name);
};

/// The class Cola convert defines unit conversions for histogram units

class Cola_convert
{
private:
  Cola_convert *next;
  static Cola_convert *first, *last;
public:
  const char *unitold, *unitnew;
  expression *expr;
  Cola_convert(const char *unitOld, const char *unitNew, expression *express) {
    next = NULL;
    if (!first) first = last = this; else last = last->next = this;
    unitold = unitOld;
    unitnew = unitNew;
    expr    = express;
  } 
  static void delete_all();
  static Cola_convert *find(const char* unitOld);
};

///
class Cola_File
{
private:
  ///
  Cola_File *next;
  ///
  static Cola_File *first;
  ///
  static Cola_File *last;
public:
  ///
  const char *name;
  ///
  FILE *fp;
  ///
  Cola_File(const char *nme, const char*filename) {
    next = NULL;
    if (!first)
      first = this; 
    else
      last->next = this;
    last = this;
    name = nme;
    fp = fopen(filename, "w");
    if (!fp) { 
      perror(filename);
      exit (-1);
    }
  }
  ///
  static void delete_all();
  ///
  void close() {fclose(fp);};
  ///
  static Cola_File *find(const char* name);
};
 
class interpolate : public expression {
  expression *left, *right;
  HIST       *id;
  double     value;
public:
  void reset() {
    if (valid == ISFALSE) return;
    valid = ISFALSE;
    left->reset();
    right->reset();
  }

  interpolate(char *file, char *histogram, 
	      expression *left, expression *right) {
    interpolate::left = left;
    interpolate::right = right;
    id = HMScanFile(file, histogram);
    if (!id) std::cerr << "Can't find histogram '"
		       << histogram << "' in file '"
		       << file <<"' for interpolation\n";
  };

  void *eval() {
    valid = NOTVALID;
    if (!id) return 0;
    if (left->isValid() && right->isValid()) valid = EVALUATED;
    if (valid == EVALUATED) 
      value = HMInterpolate2D(id, left->evaluate(), right->evaluate());
    return &value;
  };
};

void setSpinBasis(expression *x, expression *y, expression *z,
		  expression *anax, expression *anay);
%}

%union{
  int         num;
  double     *val;
  expression *exp;
  char       *str;
  AquaNode   *itm;
}

%token       COUNTER CUT DEFFOURVECTOR DEFSCALAR HISTOGRAM NORMALIZE LABEL
%token       PRINT SCALEHISTOGRAM TFALSE TTRUE WEIGHT WRITEEVENT DIR CONVERT
%token       BEAM SPEC SPECA SPECB SPECC OHIPS OOPSA OOPSB OOPSC OOPSD KAOS FILES
%token       GLOBALWEIGHT NTUPLE NTBLOCK NTID DISPLAY SPINBASIS INTERPOLATE
%token <val> NUM RUNDBITEM MEZZOITEM
%token <str> FOURVECTOR FUNC FUNC2 NAME STRING SCALAR
%token <itm> AQUANODE
%type  <exp> exp fexp
%type  <str> string scalar name

%{

expression *one        = new literal<double>(1);  // I need this...
expression *nocut      = new literal<double>(1);  // I need this...
expression *zero       = new literal<double>(0);  // I need this...
expression *currentcut = nocut;                   // that's always true.
expression *noweight   = new literal<double>(1); 
expression *currentweight = noweight;
expression *writeevent;
expression *globalweight = NULL;
expression *dodisplay = zero;
char *directory = "";

void addcut(expression *cut)
{
  currentcut = new bin<double>(currentcut, cut, op_and);
  if (new mmExpr(currentcut));
}

void removecut()
{ if (currentcut==nocut)  return;
  currentcut = ((bin<double> *)currentcut)->left;
}

void addweight(expression *weight)
{
  currentweight = new bin<double>(currentweight, weight, op_mult);
  if (new mmExpr(currentweight));
}

void removeweight()
{ if (currentweight==noweight) return;
  currentweight = ((bin<double> *)currentweight)->left;
}

int quiet = 0;
int linenumber = 0;
char *filename;
literal<FourVector> *speca=NULL, *specb=NULL, *specc=NULL, *speck=NULL;
literal<FourVector> *specd=NULL, *speco=NULL, *spech3=NULL, *spectof=NULL;
literal<FourVector> *specnpol=NULL, *specndet=NULL, *specsi=NULL, *beamv=NULL;
double beam, *timev;

inline void protect(expression *ma) 
{ 
  if (new mmExpr(ma)==NULL) std::cerr << "Not enough Memory" << std::endl;
}

// Variable declarations nedeed for the ntuple commands related
// to the grammar definitions for the COLA++ ntuple output
//
static char *ntvarname[MAX_NT_VAR];  // Scalar name related to ntvar
static char *ntdescr[MAX_NT_VAR];    // NTvariable name
static char *ntblock[MAX_NT_VAR];    // Block name for each ntvar
static char *nttit = NTUPLE_TIT;     // Ntuple Title
// NTID, #elements, header
static int  ntid = NTUPLE_ID, ntvars = 0, ntheader = 0;
char *ntblockname = CHTOP;          // Current ntuple block name

%}

/* BISON Declarations */

%left '|' OR
%left '&' AND
%left NOT
%left '<' '>' EQUAL UNEQUAL LE GE
%left '-' '+'
%left '*' '/' '%'
%left NEG     /* unary minus */
%right '^'    /* exponentiation        */

/* Grammar follows */
%%
 
input:    /* empty string */ | input line;

line:    sep
         | error sep            { yyerrok; }
         | '#' NUM STRING       { linenumber = (int)*$2-1; filename = $3; }
         | '#' NUM STRING NUM   { linenumber = (int)*$2-1; filename = $3; }
         | string sep           { if (!quiet) puts($1); }
         | CUT    exp brace             { addcut($2); addweight(one);}
         | WEIGHT exp brace             { addcut(one); addweight($2);}
         | CUT exp    comma WEIGHT exp brace { addcut($2); addweight($5);}
         | WEIGHT exp comma CUT exp brace    { addcut($5); addweight($2);}
         | '}' sep              { if (currentcut==nocut) { 
	                            yyerror("Missmatched '}'");
				    YYERROR;
	                          }
	                          removecut(); removeweight(); }
         | CONVERT string '=' exp string sep { new Cola_convert($2, $5, $4); }
         | DIR string sep       { 
	   if ($2[0] == '/') { 
	     directory = new char[strlen($2) + 1];
	     strcpy(directory, $2 + 1);
	   } else {
	     char *newdir = new char[strlen(directory) + strlen($2) + 2];
	     strcpy(newdir, directory);
	     directory = strcat(newdir, $2); 
	   }
	   if (strlen(directory)>0 && directory[strlen(directory)-1] != '/') 
	     strcat(directory, "/"); 
	   while (!strncmp(directory,"/../",4)) yyerror("invalid Directory"); 
	   while (strstr(directory,"/../")) {
	     char *i, *old = strstr(directory,"/../");
	     for(i = old-1; *i != '/'&& i != directory; i--);
	     memmove(i, old+3, strlen(old)-2);
	   }
	   while (directory[0]=='/') directory ++;
	 }
         | FILES name '(' string ')' sep { if (new Cola_File($2,$4)); }
         | FILES name '(' name ',' string ')' sep   { 
	   if (new Cola_File($2,getenv($4)?getenv($4):$6)); 
	 }
         | PRINT scalar sep     { 
	   if (new Cola_print(currentcut, $2, "%f ",NULL)); 
	 }
         | PRINT scalar string sep { 
	   if (new Cola_print(currentcut, $2, $3, NULL)); 
	 }
         | PRINT '(' name ')' scalar sep     { 
	   if (new Cola_print(currentcut, $5, "%f ",Cola_File::find($3)->fp));
	 }
         | PRINT '(' name ')' scalar string sep { 
	   if (new Cola_print(currentcut, $5, $6, 
			      Cola_File::find($3)->fp)); 
	 }
         | LABEL scalar '(' string ',' string ')' sep
                           { Cola_Scalar *sc = NULL; sc = sc->find($2);
	                     sc->label=$4; sc->unit=$6; }
         | WRITEEVENT      { writeevent=currentcut; }
         | DISPLAY         { dodisplay = currentcut; }
         | WRITEEVENT exp  { addcut($2);writeevent=currentcut;removecut(); }
         | DISPLAY    exp  { addcut($2);dodisplay =currentcut;removecut(); }
         | DEFSCALAR name '=' exp  sep     { if (new Cola_Scalar($2, $4)); }
         | DEFSCALAR name '(' string ',' string ')' '=' exp  sep     
                 { if (new Cola_Scalar($2, $9, $4, $6)); }
         | DEFFOURVECTOR name '=' fexp sep { if (new Cola_Fourvector($2,$4)); }
         | SCALEHISTOGRAM string scalar '(' string ',' string ')' sep  {
                  Cola_histogram *his = Cola_histogram::find($2);
                  if (!his) std::cout << "Can't find " << $2 << std::endl;
                  his->id->labely = $5;
                  his->id->unity = $7;
                  his->scal = Cola_Scalar::findscalar($3);
         }
         | WEIGHT string scalar sep {
                  Cola_histogram *his = Cola_histogram::find($2);
                  if (!his) std::cout << "Can't find " << $2 << std::endl;
		  HMBookWeightError(his->id);
                  his->weight = Cola_Scalar::findscalar($3);
         }
         | GLOBALWEIGHT exp sep { globalweight = $2;  }
         | NORMALIZE NAME scalar sep {
                  Cola_Counter *cc = Cola_Counter::find($2);
                  if (!cc)
                    std::cerr << "Can't find Counter " << $2 << std::endl;
                  else
                    cc->norm = Cola_Scalar::findscalar($3);
         }
         | NORMALIZE string '(' string ',' string ')' sep
                { Cola_histogram *his = Cola_histogram::find($2);
                  if (!his) std::cout << "Can't find " << $2 << std::endl;
                  his->id->divide=HMScanFile($4, $6);
                }
         | COUNTER name '=' exp sep {
	   if (new Cola_Counter($4, currentcut, currentweight, $2));
	 }
         | COUNTER name '(' string ')' '=' exp sep {
	   if (new Cola_Counter($7, currentcut, currentweight, $2, $4));
	 }
         | HISTOGRAM exp string 
                scalar '(' string ',' string ',' exp ',' exp ',' exp ')' sep  {
                       addcut($2);
		       char *tit = new char[strlen(directory)+strlen($3)+1];
		       strcpy(tit, directory);strcat(tit, $3);
                       if (new Cola_histogram(currentcut, currentweight,tit,$4,
					      $10->evaluate(), $12->evaluate(),
					      (int) $14->evaluate(), $6, $8));
                       removecut();
         }
         | HISTOGRAM exp string scalar '(' exp ',' exp ',' exp ')' sep  {
                       addcut($2);
		       char *tit = new char[strlen(directory)+strlen($3)+1];
		       strcpy(tit, directory);strcat(tit, $3);
                       if (new Cola_histogram(currentcut, currentweight,tit,$4,
					      $6->evaluate(), $8->evaluate(),
					      (int) $10->evaluate()));
                       removecut();
         }
         | HISTOGRAM exp string 
                scalar '(' string ',' string ',' exp ',' exp ',' exp ')' 
                scalar '(' string ',' string ',' exp ',' exp ',' exp ')' sep  {
                       addcut($2);
		       char *tit = new char[strlen(directory)+strlen($3)+1];
		       strcpy(tit, directory);strcat(tit, $3);
                       if (new Cola_histogram(currentcut, currentweight,tit,$4,
					      $10->evaluate(), $12->evaluate(),
					      (int) $14->evaluate(),$6,$8,$16,
					      $22->evaluate(), $24->evaluate(),
					      (int) $26->evaluate(),$18,$20));
                       removecut();
         }
         | HISTOGRAM exp string scalar '(' exp ',' exp ',' exp ')' 
                                scalar '(' exp ',' exp ',' exp ')' sep  {
                       addcut($2);
		       char *tit = new char[strlen(directory)+strlen($3)+1];
		       strcpy(tit, directory);strcat(tit, $3);
                       if (new Cola_histogram(currentcut, currentweight,tit,$4,
					      $6->evaluate(), $8->evaluate(),
					      (int) $10->evaluate(),NULL,NULL,
					      $12,
					      $14->evaluate(), $16->evaluate(),
					      (int) $18->evaluate()));
                       removecut();
         }
         | HISTOGRAM string 
                scalar '(' string ',' string ',' exp ',' exp ',' exp ')' sep  {
		       char *tit = new char[strlen(directory)+strlen($2)+1];
		       strcpy(tit, directory);strcat(tit, $2);
                       if (new Cola_histogram(currentcut, currentweight,tit,$3,
					      $9->evaluate(), $11->evaluate(),
					      (int) $13->evaluate(), $5, $7));
         }
         | HISTOGRAM string  scalar '(' exp ',' exp ',' exp ')' sep  {
		       char *tit = new char[strlen(directory)+strlen($2)+1];
		       strcpy(tit, directory);strcat(tit, $2);
                       if (new Cola_histogram(currentcut, currentweight,tit,$3,
					      $5->evaluate(), $7->evaluate(),
					      (int) $9->evaluate()));
         }
         | HISTOGRAM string 
                scalar '(' string ',' string ',' exp ',' exp ',' exp ')' 
                scalar '(' string ',' string ',' exp ',' exp ',' exp ')' sep  {
		       char *tit = new char[strlen(directory)+strlen($2)+1];
		       strcpy(tit, directory);strcat(tit, $2);
                       if (new Cola_histogram(currentcut, currentweight,tit,$3,
					      $9->evaluate(), $11->evaluate(),
					      (int) $13->evaluate(),$5,$7,$15,
					      $21->evaluate(), $23->evaluate(),
					      (int) $25->evaluate(),$17,$19));
         }
         | HISTOGRAM string scalar '(' exp ',' exp ',' exp ')' 
                            scalar '(' exp ',' exp ',' exp ')' sep  {
	               char *tit = new char[strlen(directory)+strlen($2)+1];
		       strcpy(tit, directory);strcat(tit, $2);
                       if (new Cola_histogram(currentcut, currentweight, tit, 
					      $3,$5->evaluate(),$7->evaluate(),
					      (int) $9->evaluate(),NULL,NULL,
					      $11,
					      $13->evaluate(), $15->evaluate(),
					      (int) $17->evaluate()));
         }
         | NTID string NUM sep {
                       if ( ntheader != 0 && $3 != 0 ) {
			 yyerror("Cannot generate more than one ntuple!");
		       }
		       else {
			 nttit = $2;
			 ntid = (int) *$3;
			 ntheader = 1;
		       };
	 }
         | NTID string sep {
                       if ( ntheader != 0 ) {
			 yyerror("Cannot generate more than one ntuple!");
		       }
		       else {
			 nttit = $2;
			 ntheader = 1;
		       };
	 }
         | NTBLOCK string sep {
		       ntblockname = $2; // let's waste some memory
		       if (strlen($2) > BLOCKNAMELEN) {
			 char *mess = new char[strlen($2) + BLOCKNAMELEN + 32];
			 sprintf(mess,"blockname %s truncated to ", $2);
			 ntblockname[BLOCKNAMELEN] = '\0';
			 strcat(mess, ntblockname);
			 yyerror(mess);
			 delete mess;
		       }
	 }
         | NTUPLE string scalar sep {
	               if (ntvars >= MAX_NT_VAR) {
			 char *mess = new char[strlen((char*)MAX_NT_VAR) + 36];
			 sprintf(mess,"Too many variables in ntuple (max=%d)",
				 MAX_NT_VAR);
			 yyerror(mess);
			 delete mess;
		       }
		       else {
			 ntvarname[ntvars] = $3;
			 ntdescr[ntvars] = $2;
			 if (strlen($2) > NT_VAR_LEN) {
			   ntdescr[NT_VAR_LEN-1] = '\0';
			   char *mess = new char[strlen($2) + NT_VAR_LEN + 32];
			   sprintf(mess,"field %s truncated to %s",
				   $2, ntdescr[ntvars]);
			   yyerror(mess);
			   delete mess;
			 }
			 ntblock[ntvars] = ntblockname;
			 ntvars++;
		       };
	 }
         | SPINBASIS '(' fexp ',' fexp ',' fexp ',' exp ',' exp')' {
	               setSpinBasis($3, $5, $7, $9, $11);
         }
         ;
newline:  '\n'             { linenumber++; }
          | '\n' newline   { linenumber++; }
          ;

brace:    '{' | newline '{';
comma:    ',' | ',' newline;

sep:      ';' | '\n'       { linenumber++; }
          ;
string:   STRING           { $$ = $1; }
          | string STRING  { char *st = new char[strlen($1)+strlen($2)+1];
                             strcpy(st, $1); if (new mmChar(st));
                             $$ = strcat(st, $2);
          }
          ;
exp:      NUM                  {
	    $$ = new literal<double>(*$1);
	  }
          | TTRUE              { $$ = one; }
          | TFALSE             { $$ = zero; }
          | AQUANODE          {
	    expression *m=$$= new literal<AquaNode *>($1); protect(m);
          }
          | RUNDBITEM          {
            expression *m=$$= new literal<double    *>($1); protect(m);
          }
          | MEZZOITEM          {
            expression *m=$$= new literal<double    *>($1); protect(m);
          }
          | '(' exp ')'        { $$ = $2; }
          | exp '<' exp        {
            expression *m=$$= new bin<double>($1,$3,op_lt); protect(m);
          }
          | exp '>' exp        {
            expression *m=$$= new bin<double>($1,$3,op_gt); protect(m);
          }
          | exp EQUAL exp      {
            expression *m=$$= new bin<double>($1,$3,op_equal); protect(m);
          }
          | exp UNEQUAL exp    {
            expression *m=$$= new bin<double>($1,$3,op_unequal); protect(m);
          }
          | exp LE  exp        {
            expression *m=$$= new bin<double>($1,$3,op_le); protect(m);
          }
          | exp GE  exp        {
            expression *m=$$= new bin<double>($1,$3,op_ge); protect(m);
          }
          | exp '&' exp        {
            expression *m=$$= new bin<double>($1,$3,op_bitand); protect(m);
          }
          | exp '|' exp        {
            expression *m=$$= new bin<double>($1,$3,op_bitor); protect(m);
          }
          | exp AND exp        {
            expression *m=$$= new bin<double>($1,$3,op_and); protect(m);
          }
          | exp OR  exp        {
            expression *m=$$= new bin<double>($1,$3,op_or); protect(m);
          }
          | exp '+' exp        {
            expression *m=$$= new bin<double>($1,$3,op_add); protect(m);
          }
          | exp '-' exp        {
            expression *m=$$= new bin<double>($1,$3,op_sub); protect(m);
          }
          | exp '*' exp        {
            expression *m=$$= new bin<double>($1,$3,op_mult); protect(m);
          }
          | exp '/' exp        {
            expression *m=$$= new bin<double>($1,$3,op_div); protect(m);
          }
          | exp '%' exp        {
            expression *m=$$= new bin<double>($1,$3,op_mod); protect(m);
          }
          | exp '^' exp        {
            expression *m=$$= new bin<double>($1,$3,pow); protect(m);
          }
          | '-' exp %prec NEG  {
            expression *m=$$= new bin<double>(zero,$2,op_sub); protect(m);
          }
          | NOT exp            {
            expression *m=$$= new func<double,double>($2,func_not); protect(m);
          }
          | FUNC '(' exp ')'   {
            int pos=0; 
            while (funcdef[pos].name ? strcmp($1,funcdef[pos].name) : 0) pos++;
            expression *m=$$=new func<double,double>($3,funcdef[pos].func); 
	    protect(m);
          }
          | fexp '*' fexp      {
            expression *m=$$= new scalarbin<FourVector>($1,$3,op_mult_f); 
	    protect(m);
          }
          | fexp '^' NUM {
	    if (*$3 != 2) yyerror("Only power of 2 defined for Fourvector!");
	    expression *m=$$= new class func<double,FourVector>($1,square); 
	    protect(m);
	  }
          | FUNC '(' fexp ')'  {
            int pos=0; 
            while (fsfuncdef[pos].name ? strcmp($1,fsfuncdef[pos].name) : 0)
              pos++;
            expression *m=$$= new class func<double,FourVector>
			   ($3,fsfuncdef[pos].func); protect(m);
	  }
          | INTERPOLATE '(' string ',' string ',' exp ',' exp ')' {
            expression *m = $$ = new interpolate($3,$5,$7,$9);protect(m);
          } 
          | INTERPOLATE '(' name ',' string ',' exp ',' exp ')' {
            expression *m = $$ = new interpolate(getenv($3),$5,$7,$9);protect(m);
          } 
          
          | scalar             { $$ = Cola_Scalar::findscalar($1); }
//          | '?' exp {
//	    expression *m=$$= new checkvalid<double,double>($2); 
//	    protect(m); } 
          ;

name: NAME         { $$ = $1;}
      | SCALAR     { char err[strlen($1)+255];
                     sprintf(err,"Scalar \"%s\" already defined", $1);
		     yyerror(err);	  YYERROR; } 
      | FOURVECTOR { char err[strlen($1)+255];
                     sprintf(err,"Fourvector \"%s\" already defined", $1);
		     yyerror(err);	  YYERROR; }; 

scalar: SCALAR     { $$ = $1;};
      | NAME       { char err[strlen($1)+255];
                     sprintf(err,"Undefined Scalar \"%s\"", $1);
		     yyerror(err);	  YYERROR;	 };

fexp: 
 '(' fexp ')'     { $$ = $2; }
  | fexp '+' fexp      {
    expression *m=$$= new bin<FourVector>($1,$3,op_add_f); protect(m);
  }
  | fexp '-' fexp      {
    expression *m=$$= new bin<FourVector>($1,$3,op_sub_f); protect(m);
  }
  | '-' fexp %prec NEG {
    expression *m=$$= new func<FourVector,FourVector>($2,op_neg_f); protect(m);
  }
  | FOURVECTOR         { $$ = Cola_Fourvector::findfour($1); }
  | NAME               { char err[strlen($1)+255];
                         sprintf(err,"Undefined Fourvector \"%s\"", $1);
		         yyerror(err);
		         YYERROR;
	               };
  | '{' exp '}'        {
    expression *m=$$= new literal<FourVector>
		   (FourVector($2->evaluate(),0,0,0)); 
    protect(m);
  }
  | '{' exp ',' exp ',' exp ',' exp  '}'  {
    //    expression *m=$$= new literal<FourVector>(FourVector($2->evaluate(),
    //							 $4->evaluate(),
    //							 $6->evaluate(),
    //							 $8->evaluate()));
    expression *m=$$= new constFour($2, $4, $6, $8);
    protect(m);
  }
  | SPEC '(' string ')' {
    if (!strcmp($3,"A"))   {expression *m=$$= speca = new literal<FourVector>
			      (FourVector(0, 0, 0, 0)); protect(m);}
    if (!strcmp($3,"B"))   {expression *m=$$= specb = new literal<FourVector>
			      (FourVector(0, 0, 0, 0)); protect(m);}
    if (!strcmp($3,"C"))   {expression *m=$$= specc = new literal<FourVector>
			      (FourVector(0, 0, 0, 0)); protect(m);}
    if (!strcmp($3,"D"))   {expression *m=$$= specd = new literal<FourVector>
			      (FourVector(0, 0, 0, 0)); protect(m);}
    if (!strcmp($3,"O"))   {expression *m=$$= speco = new literal<FourVector>
			      (FourVector(0, 0, 0, 0)); protect(m);}
    if (!strcmp($3,"H3"))  {expression *m=$$= spech3 = new literal<FourVector>
			      (FourVector(0, 0, 0, 0)); protect(m);}
    if (!strcmp($3,"TOF")) {expression *m=$$= spectof = new literal<FourVector>
			      (FourVector(0, 0, 0, 0)); protect(m);}
    if (!strcmp($3,"NPOL")) {expression *m=$$= specnpol = new literal<FourVector>
			      (FourVector(0, 0, 0, 0)); protect(m);}
    if (!strcmp($3,"NDET")) {expression *m=$$= specndet = new literal<FourVector>
			      (FourVector(0, 0, 0, 0)); protect(m);}
    if (!strcmp($3,"SI"))   {expression *m=$$= specsi   = new literal<FourVector>
			      (FourVector(0, 0, 0, 0)); protect(m);}
    if (!strcmp($3,"KAOS"))   {expression *m=$$= speck = new literal<FourVector>
			      (FourVector(0, 0, 0, 0)); protect(m);}
  }
  | SPECA '(' NUM ')'  {
    std::cerr << "WARNING: SpectrometerA(mass) is deprecated. "
	      << "Use Spectrometer(\"A\")" << std::endl;
    expression *m=$$= speca = new literal<FourVector>
		   (FourVector(*$3, 0, 0, 0)); 
    protect(m);
  }
  | SPECB '(' NUM ')'  {
    std::cerr << "WARNING: SpectrometerB(mass) is deprecated. "
	      << "Use Spectrometer(\"B\")" << std::endl;
    expression *m=$$= specb = new literal<FourVector>
		   (FourVector(*$3, 0, 0, 0)); 
    protect(m);
  }
  | SPECC '(' NUM ')'  {
    std::cerr << "WARNING: SpectrometerC(mass) is deprecated. "
	      << "Use Spectrometer(\"C\")" << std::endl;
    expression *m=$$= specc = new literal<FourVector>
		   (FourVector(*$3, 0, 0, 0)); 
    protect(m);
  }
  | OHIPS '(' NUM ')'  {
    std::cerr << "WARNING: OHIPS(mass) is deprecated. "
	      << "Use Spectrometer(\"O\")" << std::endl;
    expression *m=$$= speco = new literal<FourVector>
		   (FourVector(*$3, 0, 0, 0)); 
    protect(m);
  }
  | OOPSA '(' NUM ')'  {
    std::cerr << "WARNING: OOPSA(mass) is deprecated. "
	      << "Use Spectrometer(\"A\")" << std::endl;
    expression *m=$$= speca = new literal<FourVector>
		   (FourVector(*$3, 0, 0, 0)); 
    protect(m);
  }
  | OOPSB '(' NUM ')'  {
    std::cerr << "WARNING: OOPSB(mass) is deprecated. "
	      << "Use Spectrometer(\"B\")" << std::endl;
    expression *m=$$= specb = new literal<FourVector>
		   (FourVector(*$3, 0, 0, 0)); 
    protect(m);
  }
  | OOPSC '(' NUM ')'  {
    std::cerr << "WARNING: OOPSC(mass) is deprecated. "
	      << "Use Spectrometer(\"C\")" << std::endl;
    expression *m=$$= specc = new literal<FourVector>
		   (FourVector(*$3, 0, 0, 0)); 
    protect(m);
  }
  | OOPSD '(' NUM ')'  {
    std::cerr << "WARNING: OOPSD(mass) is deprecated. "
	      << "Use Spectrometer(\"D\")" << std::endl;
    expression *m=$$= specd = new literal<FourVector>
		   (FourVector(*$3, 0, 0, 0)); 
    protect(m);
  }
  | FUNC2 '(' fexp ',' fexp ')'  {
    int pos=0; 
    while (fsfunc2def[pos].name ? strcmp($1, fsfunc2def[pos].name) : 0) pos++;
    expression *m=$$= new bin<FourVector>($3,$5,fsfunc2def[pos].func); 
    protect(m);
  }
  | BEAM  {
    expression *m=$$= beamv = new literal<FourVector>(FourVector(0, 0, 0, 0)); 
    protect(m);
  }
  ;


%%
///
expression *expression::current = NULL;

///
Cola_Counter *Cola_Counter::first = NULL;
///
Cola_Counter *Cola_Counter::last = NULL;

///
Cola_Fourvector *Cola_Fourvector::first = NULL;
///
Cola_Fourvector *Cola_Fourvector::last = NULL;

///
Cola_print *Cola_print::first = NULL;
///
Cola_print *Cola_print::last = NULL;

///
Cola_Scalar *Cola_Scalar::first = NULL;
///
Cola_Scalar *Cola_Scalar::last = NULL;

Cola_convert *Cola_convert::first = NULL;
Cola_convert *Cola_convert::last = NULL;

Cola_histogram *Cola_histogram::first = NULL;
Cola_histogram *Cola_histogram::last = NULL;
Cola_histogram *Cola_histogram::ch = NULL;

const char *Cola_histogram::zLabel = "Counts";
const char *Cola_histogram::zUnit  = "?";

///
Cola_File *Cola_File::first = NULL;
///
Cola_File *Cola_File::last = NULL;

Cola_Counter::Cola_Counter(expression *ct, expression *Cut, expression *Weight,
			   const char *Name, const char *Format)
{
  next = NULL;
  if (!first)  first = this; 
  else         last->next = this;
  last = this;
  
  name = new char[strlen(Name)+1];   
  strcpy(name, Name);
  if (Format) {
    format = new char[strlen(Format) + 1]; 
    strcpy(format, Format);
  } else {
    format = new char[strlen(Name) + 5];
    sprintf(format, "%s%s", Name, ": %g");
  }
  var    = ct;
  cut    = Cut;
  weight = Weight;
  norm   = NULL;
  value  = normvalue = 0;
}

Cola_Counter *Cola_Counter::find(char *nme)
{
  Cola_Counter *cc = first;
  while(cc) {
    if (!strcmp(cc->name, nme)) return cc; 
    cc = cc->next; 
  }
  return NULL;
}

void Cola_Counter::reset_all()
{
  Cola_Counter *ch = first;
  while(ch) {
    ch->reset(); 
    ch=ch->next; 
  }
}

void Cola_Counter::fill_all(double weight)
{
  Cola_Counter *ch = first;
  while(ch) { 
    ch->fill(weight); 
    ch=ch->next; 
  }
}

void Cola_Counter::fill(double wght)
{
  if (!(cut->isValid()))    return;
  if (cut->evaluate() == 0) return;
  if (!(var->isValid()))    return;
  double w = wght;
  if (weight) {if (!(weight->isValid())) return; else w *= weight->evaluate();}
  normvalue = (value += var->evaluate() * w);
  if (norm) {
    if (norm->isValid() && norm->evaluate()!=0)
      normvalue = value / norm->evaluate();
    else
      normvalue = -1;
  }
}

void Cola_Counter::delete_all()
{
  Cola_Counter *nxt, *base = first;
  while (base) {
    nxt = base->next;
    delete base;
    base = nxt;
  }
  first = last = NULL;
}

///
Cola_Fourvector *Cola_Fourvector::find(const char *nme)
{
    Cola_Fourvector *ch = first;
    while (ch) {
	if (!strcmp(ch->name,nme)) return ch; 
	ch=ch->next; 
    }
    return NULL;
}

///
void Cola_Fourvector::reset()
{
    if (expr) expr->reset();
}

///
void Cola_Fourvector::reset_all()
{
    Cola_Fourvector *ch = first;
    while (ch) {
	ch->reset();
	ch=ch->next;
    }
}

///
char *Cola_Fourvector::namefour(const char *name)
{
  Cola_Fourvector *base=Cola_Fourvector::find(name);
  return (base ? base->name : NULL);
}

///
expression *Cola_Fourvector::findfour(const char *name)
{
  Cola_Fourvector *base = Cola_Fourvector::find(name);
  if (base) return base->expr; 
  return NULL;
}

///
void Cola_Fourvector::delete_all()
{
    Cola_Fourvector *nxt, *base = first;
    while (base) {
	nxt = base->next;
	delete base;
	base = nxt;
    }
    first = last = NULL;
}

///
void Cola_print::reset()
{ if (!this) return;
  if (scalar) scalar->reset();
  if (cut)  cut->reset();
}

///
void Cola_print::reset_all()
{ Cola_print *ch = first;
  while(ch) { ch->reset(); ch=ch->next; }
}

///
void Cola_print::print_all(FILE *fp)
{ Cola_print *ch = first;
  while(ch) { ch->print(fp); ch=ch->next; }
}

///
void Cola_print::print(FILE *fp)
{
  if (!cut->isValid())      return;
  if (cut->evaluate() == 0) return;
  if (scalar->isValid())    
    fprintf(filepointer?filepointer:fp, format, scalar->evaluate());
}

///
Cola_print::Cola_print(expression *ct, char *sc, char *frmt, FILE *fp)
{ next = NULL;
  if (!first)  first = this; 
  else         last->next = this;
  last = this;

  cut = ct;
  format = frmt;
  scalar = Cola_Scalar::findscalar(sc);
  filepointer = fp;
  if (!scalar) std::cout << "ERROR: '" << sc << "' not found" << std::endl;
}

///
void Cola_print::delete_all()
{
  Cola_print *nxt, *base = first;
  while (base) {
    nxt = base->next;
    delete base;
    base = nxt;
  }
  first = last = NULL;
}

///
Cola_Scalar *Cola_Scalar::find(const char *nme)
{
  Cola_Scalar *ch = first;
  while(ch) {
    if (!strcmp(ch->name,nme)) return ch; 
    ch=ch->next; 
  }
  return NULL;
}

///
void Cola_Scalar::reset_all()
{
  Cola_Scalar *ch = first;
  while (ch) {
    ch->reset();
    ch=ch->next;
  }
}

///
char *Cola_Scalar::namescalar(const char *name)
{
  Cola_Scalar *base=Cola_Scalar::find(name);
  return (base ? (char *) base->name : NULL);
}

///
expression *Cola_Scalar::findscalar(const char *name)
{
  Cola_Scalar *base = Cola_Scalar::find(name);
  return (base ? base->expr : NULL);
}

///
void Cola_Scalar::delete_all()
{
  Cola_Scalar *nxt, *base = first;
  while (base) {
    nxt = base->next;
    delete base;
    base = nxt;
  }
  first = last = NULL;
}

Cola_convert *Cola_convert::find(const char *unitOld)
{ for (Cola_convert *ch=first; ch; ch=ch->next) 
    if (!strcmp(ch->unitold, unitOld)) return ch; 
  return NULL;
}

void Cola_convert::delete_all()
{
  Cola_convert *nxt, *base = first;
  while (base) {
    nxt = base->next;
    delete base;
    base = nxt;
  }
  first = last = NULL;
}

Cola_histogram *Cola_histogram::find(char *name)
{
  Cola_histogram *ch = first;
  char fullname[strlen(directory)+strlen(name)+1];
  strcpy(fullname,directory);
  strcat(fullname,name);
  while (ch) {
    if (!strcmp(ch->id->name, fullname) ||
        !strcmp(ch->id->name, name) ) return ch; 
    ch = ch->next; 
  }
  return NULL;
}

void Cola_histogram::reset()
{
  if (!this) return;
  if (scal)   scal->reset();
  if (var1)   var1->reset();
  if (var2)   var2->reset();
  if (cut)    cut->reset();
  if (weight) weight->reset();
}

void Cola_histogram::reset_all()
{
  Cola_histogram *ch = first;
  while (ch) {
    ch->reset();
    ch = ch->next;
  }
}

void Cola_histogram::fill_all(double weight)
{
  ch = first;
  while (ch) {
    ch->fill(weight);
    ch = ch->next;
  }
  ch = NULL;
}

void Cola_histogram::fill(double wght)
{
  if (!cut->isValid()) return;
  double w = wght;
  if (cut->evaluate() != 0) {
    if (weight!=NULL) w *=  (weight->isValid() ? weight->evaluate() : 0);
    if (scal) id->scale = scal->evaluate();
    if (var1->isValid()) {
      expression::current = var1;
      double x = var1->evaluate()*convert1;
      if (var2) {
	if (var2->isValid()) {
	  expression::current = var2;
	  double y = var2->evaluate()*convert2;
	  HMFill(id, x, y, w);
	}
      }
      else 
	HMFill(id, x, 0, w);
    }
  }
  expression::current = NULL;
}

Cola_histogram::Cola_histogram(expression *ct, 
			       expression *wght,
			       char *title, 
			       char *sc, double min, double max, int nx,
			       char *lab, char *unit)
{
  Cola_Scalar *base = NULL;
  base = base->find(sc);

  if (lab==NULL && unit==NULL && (base->unit==NULL || base->label==NULL))
    std::cerr << "Missing Label/Unit for "<< title << std::endl;

  next = NULL;
  if (!first) last = first = this; else last = last->next = this;

  cut = ct;
  weight = wght;
  scal = var2 = NULL;
  const char *Label = lab  ? lab  : base->label;
  const char *Unit  = unit ? unit : base->unit;

  Cola_convert *conv1 = Cola_convert::find(Unit);
  convert1 = conv1 ? conv1->expr->evaluate() : 1; 
  if (conv1) Unit = conv1->unitnew;
  if (base) var1 = base->expr; 
  if (!var1) std::cerr << "Error creating 1D Histogram " << title << std::endl;
  id = HMBook1(title, Label, zLabel, Unit, zUnit, nx, min, max);
}

Cola_histogram::Cola_histogram(expression *ct, 
			       expression *wght, 
			       char *title, 
			       char *sc1, double min1, double max1,
			       int nx1, char *lab1, char *unit1,
			       char *sc2, double min2, double max2,
			       int nx2, char *lab2, char *unit2)
{
  Cola_Scalar *base1 = NULL, *base2 = NULL;
  base1 = base1->find(sc1);
  base2 = base2->find(sc2);
  if (base1) var1 = base1->expr; 
  if (base2) var2 = base2->expr; 
  if (!var1) std::cerr << "Error creating 2D Histogram "
		       << title << " " << sc1 << std::endl;
  if (!var2) std::cerr << "Error creating 2D Histogram "
		       << title << " " << sc2 << std::endl;

  if (lab1==NULL && unit1==NULL && lab2==NULL && unit2==NULL &&
      (base1->unit==NULL || base1->label==NULL ||
       base2->unit==NULL || base2->label==NULL))
      std::cerr << "Missing Label/Unit for "<< title << std::endl;

  next = NULL;
  if (!first) last = first = this; else last = last->next = this;

  cut = ct;
  weight = wght;
  scal = NULL;

  const char *Label1 = lab1  ? lab1  : base1->label;
  const char *Unit1  = unit1 ? unit1 : base1->unit;
  const char *Label2 = lab2  ? lab2  : base2->label;
  const char *Unit2  = unit2 ? unit2 : base2->unit;

  Cola_convert *conv1 = Cola_convert::find(Unit1);
  Cola_convert *conv2 = Cola_convert::find(Unit2);
  convert1 = conv1 ? conv1->expr->evaluate() : 1; 
  convert2 = conv2 ? conv2->expr->evaluate() : 1; 
  if (conv1) Unit1 = conv1->unitnew;
  if (conv2) Unit2 = conv2->unitnew;

  id = HMBook2(title, Label1, Label2, zLabel, Unit1, Unit2, zUnit,
	       nx1, min1, max1, nx2, min2, max2);
}

void Cola_histogram::delete_all()
{
  Cola_histogram *nxt, *base = first;
  while (base) {
    nxt = base->next;
    delete base;
    base = nxt;
  }
  first = last = NULL;
}

const char *currentHistogram() { 
  return Cola_histogram::ch?Cola_histogram::ch->id->title:"UNKNOWN"; 
}

///
Cola_File *
Cola_File::find(const char *nme)
{
  Cola_File *ch = first;
  while(ch) {
    if (!strcmp(ch->name,nme)) return ch; 
    ch=ch->next; 
  }
  return NULL;
}

///
void
Cola_File::delete_all()
{
  Cola_File *nxt, *base = first;
  while (base) {
    nxt = base->next;
    delete base;
    base = nxt;
  }
  first = last = NULL;
}
