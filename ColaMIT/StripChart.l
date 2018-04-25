//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mit.edu
// MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaMIT/StripChart.l,v 2.1 2002-03-05 03:49:49 distler Exp $
//
// LEX scanner (flex++ version)
// initialize ColaMIT StripChart
//

%name   StripChartLEX

%define CONSTRUCTOR_PARAM	FILE *InputFile
%define CONSTRUCTOR_CODE	YY_StripChartLEX_IN = InputFile;
%define CONSTRUCTOR_INIT	: lineno(1)
%define LEX_PARAM		YY_StripChartYACC_STYPE *val
%define MEMBERS			public:\
				  int	lineno;

%{
#include "StripChart.parse.h"
%}

WS	[ \t\f]
FLOAT	([0-9]+\.[0-9]*|[0-9]*\.[0-9]+|[0-9]+)([eE][+\-]?[0-9]+)?
KEY	[a-zA-Z][a-zA-Z0-9._\[\]]*

%%

^"//".*\n		lineno++;
^#.*\n			lineno++;

{FLOAT}			{ val->val  = atof((char *)yytext);
			  return StripChartYACC::VALUE;
			}
{KEY}			{ val->str  = new char[strlen((char *)yytext)+1];
			  strcpy(val->str, (char *)yytext);
			  return StripChartYACC::KEY;
			}
{WS}			;
\n			lineno++; return yytext[0];
.			return yytext[0];

%%
