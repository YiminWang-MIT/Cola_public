//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                           mailto:distler@mit.edu
// MIT, Lab for Nuclear Science, 26-402B        tel.   (617) 253-6997
// Cambridge, MA 02139                          fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaMIT/UniqRunDB.l,v 2.2 2002-12-12 12:24:12 distler Exp $
//
// LEX scanner (flex++ version)
// remove duplicate lines from run.db
//

%name   UniqRunDbLEX

%define CONSTRUCTOR_PARAM	FILE *InputFile
%define CONSTRUCTOR_CODE	YY_UniqRunDbLEX_IN = InputFile;
%define CONSTRUCTOR_INIT	: lineno(1)
%define LEX_PARAM		YY_UniqRunDbYACC_STYPE *val
%define MEMBERS			public:\
				  int	lineno;

%{
#include "UniqRunDB.parse.h"
#include <cstring>
#include <ctime>
static struct tm tms;
%}

%s	ENDOFFILE
WS	[ \t\f]
FLOAT	([0-9]+\.[0-9]*|[0-9]*\.[0-9]+|[0-9]+)([eE][+\-]?[0-9]+)?
DATE	\[[0-9]*{12}\]
KEY	[a-zA-Z][a-zA-Z0-9._]*

%%

^"//".*\n		lineno++;
^#.*\n			lineno++;

{DATE}			{ sscanf((char *)yytext,"[%2d%2d%2d%2d%2d%2d]",
				 &tms.tm_year, &tms.tm_mon, &tms.tm_mday,
				 &tms.tm_hour, &tms.tm_min, &tms.tm_sec);
			  tms.tm_isdst = -1; tms.tm_mon--;
			  val->lval = mktime(&tms);
			  return UniqRunDbYACC::DATE;
			}
{FLOAT}			{ val->val  = atof((char *)yytext);
			  return UniqRunDbYACC::VALUE;
			}
{KEY}			{ val->str  = new char[strlen((char *)yytext)+1];
			  strcpy(val->str, (char *)yytext);
			  return UniqRunDbYACC::KEY;
			}
\".*\"			{ val->str  = new char[strlen((char *)yytext)+1];
			  strcpy(val->str, (char *)yytext+1);
			  val->str[strlen((char *)yytext)-2] = 0;
			  return UniqRunDbYACC::STRING;
			}
"//".*			;

{WS}			;
\n			lineno++; return yytext[0];
.			return yytext[0];
<INITIAL><<EOF>>	BEGIN ENDOFFILE; return UniqRunDbYACC::EndOfFile;
<ENDOFFILE><<EOF>>	return 0;

%%
