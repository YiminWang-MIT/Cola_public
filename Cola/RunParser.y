%name RUNDATABASE
%token NUM NAME STRING FUNC TTRUE TFALSE
%{

#include <cmath>
#include <cstdio>
#define __RUNPARSER__
#if defined(__ColaMIT__)
#include "../ColaMIT/rundatabase.h"
#elif defined(__ColaJLab__)
#include "../ColaJLab/rundatabase.h"
#elif defined(__ColaIASA__)
#include "../ColaIASA/rundatabase.h"
#else
#include "rundatabase.h"
#endif
#include <ctime>
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#include <cstring>

#define YY_RUNDATABASE_STYPE double *
#define ifs(s,f) if(!strcmp((char *)s,f))
#define set(a,b,name,var) if(!strcmp((char *)a,name)) \
                            *(double *)var = *(double *) b

static FILE *parsefile;

static time_t thistime;
static time_t stoptime;
static const char *setup ="no setup found";
char lasttime[13];
static const char *filename;
static int linenumber = 0;
static double *array = 0;
static int     arrayindex =0;
extern int quiet;
%}
/* BISON Declarations */

%left '|' OR
%left '&' AND
%left NOT
%left '<' '>' EQUAL UNEQUAL LE GE
%left '-' '+'
%left '*' '/'
%left NEG     /* unary minus */
%right '^'    /* exponentiation        */
%right '²' '³'

/* Grammar follows */
%%
input:    /* empty string */ | input line;

line:    sep
         | error sep       { yyerrok; }
         | string sep      { if (!quiet) puts((char *) $1); }
         | '#' NUM STRING sep { linenumber=(int) *$2; filename = (char *)$3; } 
         | '#' NUM STRING NUM sep
                           { linenumber = (int) *$2; filename = (char *)$3; } 
         | '[' NUM ']'     {
	   char ts[13];
	   sprintf(ts,"%012.0f", *(double *) $2);
	   if(stoptime != -1)  {
	     struct tm tms;
	     memcpy(&tms, localtime(&stoptime), sizeof(tms));
	     sscanf(ts, "%2d%2d%2d%2d%2d%2d",
		    &tms.tm_year, &tms.tm_mon, &tms.tm_mday, 
		    &tms.tm_hour, &tms.tm_min, &tms.tm_sec);
	     if (tms.tm_year < 90) tms.tm_year += 100;
	     tms.tm_mon--;
	     thistime = mktime(&tms);
	     if (difftime(thistime,stoptime)>0) YYABORT;
	   }
	   else if(!strcmp(rundb.setup, setup)) YYABORT;
	   strcpy(lasttime,ts);
	 }
         | NAME '=' exp sep  {
	   int i= -1; 
	   while(lname[++i].Name) 
	     set($1,$3,lname[i].Name,lname[i].addr);
	 }
         | NAME '=' string sep {
	   int i= -1; 
	   while(lname[++i].Name) 
	     if(!strcmp(lname[i].Name,(char *)$1))
	       *(char **)lname[i].addr = (char *) $3;
	 }
         | NAME '=' '{'    { 
	   int i= -1; 
	   while(lname[++i].Name) 
	     if(!strcmp(lname[i].Name,(char *)$1))
	       array = (double *) lname[i].addr;
	   arrayindex=0;
	 }
         | exp '}' sep         { array[arrayindex] = *$1; arrayindex++;}
         | exp ','         { array[arrayindex] = *$1; arrayindex++;}

sep:     ';' | '\n'        { linenumber++ };

string:  STRING            { $$ = $1;}
         | string STRING   {
	   char *st = new char[strlen((char *) $1) + strlen((char *) $2) + 1];
	   strcpy(st, (char *)$1);
	   $$ = (double *) strcat(st, (char *)$2);
	 };

exp:     NUM 
         | NUM '°'
         | '(' exp ')'       { $$ = $2;}
         | exp '^' exp       { $$ = new double(pow(*$1,*$3)) }
         | exp '*' exp       { $$ = new double(*$1 * *$3) }
         | exp '²'           { $$ = new double(*$1 * *$1) }
         | exp '³'           { $$ = new double(pow(*$1,3))}
         | exp '/' exp       { $$ = new double(*$1 / *$3) }
         | exp '+' exp       { $$ = new double(*$1 + *$3) }
         | exp '-' exp       { $$ = new double(*$1 - *$3) }
         | '-' exp %prec NEG { $$ = new double(    - *$2) };
%%
