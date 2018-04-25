//								      -*-c++-*-
// Copyright (c) 2003
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
// $Id: ReadRundb.cc 2354 2009-11-13 13:58:04Z merkel $
//

#define _XOPEN_SOURCE 1
#include "FourVector/FourVector.h"
#include "RunParser.parse.cc"
#include <ctype.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
extern int verbose;

void RUNDATABASE::yyerror (char *s)
{
  printf ("%s:%d: %s\n", filename, linenumber, s);
}


int RUNDATABASE::yylex ()
{
  int c;
  while ((c = fgetc(parsefile)) == ' ' || c == '\t');
  if (c=='\\'){ c=fgetc(parsefile); linenumber++; return yylex();}
  if (c=='!') { c=fgetc(parsefile);
		if(c=='=')return UNEQUAL;ungetc(c,parsefile);c='!';}
  if (c=='=') { c=fgetc(parsefile);
		if(c=='=')return EQUAL;  ungetc(c,parsefile);c='=';}
  if (c=='<') { c=fgetc(parsefile);
		if(c=='=')return LE;     ungetc(c,parsefile);c='<';}
  if (c=='>') { c=fgetc(parsefile);
		if(c=='=')return GE;     ungetc(c,parsefile);c='>';}
  if (c=='&') { c=fgetc(parsefile);
		if(c=='&')return AND;    ungetc(c,parsefile);c='&';}
  if (c=='|') { c=fgetc(parsefile);
		if(c=='|')return OR;     ungetc(c,parsefile);c='|';}
  if (c=='/') { c=fgetc(parsefile);
		if(c=='/')
                { while (c !='\n' && c != EOF) c=fgetc(parsefile); 
		  return c;
		}
		ungetc(c,parsefile);
		c='/';
	      }


  if (c=='!') {while (c !='\n' && c != EOF) c=fgetc(parsefile); return c;}
  if (c=='"') { int i=c=0;
		char function[256];
		while  (c!='"' && c!=EOF) function[i++] = c = fgetc(parsefile);
		function[i-1]=0;
		yylval = (double *) new char[sizeof(function)+1];
		strcpy((char *)yylval, function); 
		return STRING;
	      }
  if (isdigit (c))
         { ungetc(c,parsefile);
	   yylval = new double;
           if (fscanf (parsefile,"%lf", yylval));
           return NUM;
         }
  if ((c>='a' && c<='z') || (c>='A' && c<='Z') || c=='.' || c=='@') { 
    int i=0;
    char function[255];
    while  ((c>='a' && c<='z') || (c>='A' && c<='Z') || isdigit(c) || 
	    c=='.' || c=='_' || c=='@')  { 
      function[i++]=c;
      c = fgetc(parsefile);
    }
    
    function[i]=0;
    ungetc(c,parsefile);
    
    i=-1; 
    while(lname[++i].Name) 
      if (!strcmp(lname[i].Name,function)) { 
	yylval = (double *) lname[i].Name;
	if (lname[i].type == 4711) {
	  fprintf(stderr, "Warning: rundb-entry %s not valid anymore!\n", 
		  lname[i].Name );
	  fprintf(stderr, "         For more details about rundb-entries"
		  " please check\n"
		  "         http://wwwa1.kph.uni-mainz.de/"
		  "A1/trigger/doc/ColaSourceDoc/index.html!\n\n");
	}
	return NAME;
      }
  }
  if (c==EOF ) return 0;
  return c;
}

void dumprundb(const char *runname, FILE *fp)
{ 
  fprintf(fp, "[%s]\n",runname);
  int l=0, i = -1;
  while (lname[++i].Name) {
    if (lname[i].type !=4711) l = fprintf(fp, "\t%s = ", lname[i].Name);
    switch (lname[i].type) {
    case    1 : fprintf(fp, "%g\n",     *(double *)lname[i].addr); break;
    case   -1 : 
      if (*(char  **)lname[i].addr) 
	fprintf(fp, "\"%s\"\n", *(char  **)lname[i].addr); 
      else
	fprintf(fp, "\"\"\n"); 
	break;
    case 4711 : break;
    default   :
      l += fprintf(fp, "{");
      for(int j=0;j<lname[i].type;j++) {
	l += fprintf(fp, "%g", ((double *)lname[i].addr)[j]);
	if (j==lname[i].type-1) fprintf(fp, "}\n");
	else if (l>55) l=fprintf(fp, ",\n\t\t"); else l+=fprintf(fp, ", ");
      }
    } 
  }
}

void dumprundb(FILE *fp)
{
  dumprundb(lasttime,fp);
}

void readRunDatabase(const char *name, time_t time)
{
  RUNDATABASE *pr =new RUNDATABASE;
  // TZ_ENV is defined in rundatabase.h
  // static int putenvresult = putenv((char *)TZ_ENV);  // ******FIXME******
  tzset();
  filename="-";
  if (!strcmp(name,"-")) 
    parsefile = stdin; 
  else { 
    parsefile = fopen(name, "r");
    if (!parsefile) { perror(name);  return;  }
    fclose(parsefile);

    filename=name; // Save for parser error messages
    char command[255]; 
    sprintf(command, GNUCPP " %s",name);
    parsefile = popen(command,"r");
  }
  if (!parsefile) { perror(name); return ; }

  stoptime = time;
  setup = NULL;
  pr->yyparse();
  while (!feof(parsefile)) fgetc(parsefile); // clean exit of gnucpp subprocess
  fclose(parsefile);

  if (verbose) dumprundb(stdout);
}

void readRunDatabase(const char *name, const char *set)
{ RUNDATABASE *pr =new RUNDATABASE;

  filename="-";
  if (!strcmp(name,"-")) 
    parsefile = stdin; 
  else { 
    parsefile = fopen(name, "r");
    if (!parsefile) { perror(name);  return;  }
    fclose(parsefile);

    filename=name; // Save for parser error messages
    char command[255]; 
    sprintf(command, GNUCPP " %s",name);
    parsefile = popen(command,"r");
  }
  if (!parsefile) { perror(name);  return;  }

  stoptime = -1;
  setup = set;
  rundb.setup = "No line 'Setup = \"...\"' found";
  pr->yyparse();
  while (!feof(parsefile)) fgetc(parsefile); // clean exit of gnucpp subprocess
  fclose(parsefile);

  if (verbose) dumprundb(stdout);
}

void readRunDatabase(const char *name, const char *runname, 
		     const char *setup, time_t time)
{
  char dbpath[strlen(name)+1];
  strcpy(dbpath,name);
  for(char *token=strtok(dbpath,":");token;token=strtok(NULL,":")) {
    if (!strncmp(token, "//", 2)) {
      char *dbname = index(token+2,'/')+1;
      if (dbname) {
	if (index(dbname,'/')) *index(dbname,'/') = '\0';
	*index(token+2,'/') = '\0';
	char *dbhost=token+2;
	readSQLdatabase(runname, setup, dbhost, dbname);
	struct tm *tms = localtime(&time);
	char dstr[16];
	sprintf(dstr,"%02d%02d%02d%02d%02d%02d", tms->tm_year, tms->tm_mon+1,
		tms->tm_mday, tms->tm_hour, tms->tm_min, tms->tm_sec);
	if (verbose) dumprundb(dstr, stdout);
      }
    } else
      if (setup) readRunDatabase(token, setup);
      else 	 readRunDatabase(token, time);
  }
}
