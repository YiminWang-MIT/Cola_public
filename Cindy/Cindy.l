//								      -*-c++-*-
// Copyright (c) 1993-95 by	Michael O. Distler,
//				Institut fuer Kernphysik,
//				Johannes Gutenberg-Universitaet Mainz
//
// All rights reserved.
//
// $Header: /tmp/cvsroot/Cola/Cindy/Cindy.l,v 4.7 1998-08-31 22:09:11 distler Exp $
//
// LEX Parser fuer den Cindy++ Compiler (flex++ Version)
//

%name	CindyLEX

%define CONSTRUCTOR_PARAM	FILE *InputFile
%define CONSTRUCTOR_CODE	YY_CindyLEX_IN = InputFile;
%define CONSTRUCTOR_INIT	: lineno(1)
%define LEX_PARAM		YY_CindyYACC_STYPE *val
%define MEMBERS			public:\
				  int	lineno;

%{
#include <CindyYACC.h>
%}

%START	DESCBLOCK

NAME	[a-zA-Z_][a-zA-Z_0-9]*
NUM	[0-9]*

%%

#.*\n		lineno++;
[ \t]		{ ; }
"extern"	{ ; }
"struct"	return CindyYACC::STRUCT;
"typedef"	return CindyYACC::TYPEDEF;
"short"		return CindyYACC::SHORT;
"long"		return CindyYACC::LONG;
"unsigned"	return CindyYACC::UNSIGNED;
"char"		return CindyYACC::CHAR;
"int"		return CindyYACC::INT;
"float"		return CindyYACC::FLOAT;
"double"	return CindyYACC::DOUBLE;
{NUM}		{ val->num = new Number(yytext);
		  return CindyYACC::NUMBER;
		}
{NAME}		{ val->str = new char[strlen(yytext)+1];
		  strcpy(val->str, yytext); 
		  return CindyYACC::STRING;
		}
\n		lineno++;
.		return yytext[0];
%%
