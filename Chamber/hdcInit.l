//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mit.edu
// MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/Chamber/hdcInit.l,v 2.3 1998-04-09 00:46:20 distler Exp $
//
// LEX scanner (flex++ version)
// initialize OOPS horizontal driftchambers (HDCs)
//

%name   hdcInitLEX

%define CONSTRUCTOR_PARAM	FILE *InputFile
%define CONSTRUCTOR_CODE	YY_hdcInitLEX_IN = InputFile;
%define CONSTRUCTOR_INIT	: lineno(1)
%define LEX_PARAM		YY_hdcInitYACC_STYPE *val
%define MEMBERS			public:\
				  int	lineno;

%{
#include "hdcInitYACC.h"
%}

%s	BRAKET

WS	[ \t\f]
FLOAT	([0-9]+\.[0-9]*|[0-9]*\.[0-9]+|[0-9]+)([eE][+\-]?[0-9]+)?
INTEGER	[+\-]?[0-9]+

%%

"//".*\n		lineno++;

\[			BEGIN BRAKET; return yytext[0];
\]			BEGIN INITIAL; return yytext[0];

"wire0offset"		return hdcInitYACC::WIRE0OFFSET;
"delayLineUnit"		return hdcInitYACC::DELAYLINEUNIT;
"driftTimeOffset"	return hdcInitYACC::DRIFTTIMEOFFSET;
"zPosition"		return hdcInitYACC::ZPOSITION;
"xOffset"		return hdcInitYACC::XOFFSET;
"rotation"		return hdcInitYACC::ROTATION;
"lowDrifttime"		return hdcInitYACC::LOWDRIFTTIME;
"highDrifttime"		return hdcInitYACC::HIGHDRIFTTIME;
"entries"		return hdcInitYACC::ENTRIES;
"bintons"		return hdcInitYACC::BINTONS;
"adcLRinfo"		return hdcInitYACC::ADCLRINFO;
"adcOEflip"             return hdcInitYACC::ADCOEFLIP;
"drifttimeTable"	return hdcInitYACC::DRIFTTIMETABLE;

[tT][rR][uU][eE]	val->val = 1.0; return hdcInitYACC::VALUE;
[fF][aA][lL][sS][eE]	val->val = 0.0; return hdcInitYACC::VALUE;

<INITIAL>{FLOAT}	val->val = atof(yytext); return hdcInitYACC::VALUE;
<BRAKET>{INTEGER}	val->num = atoi(yytext); return hdcInitYACC::INDEX;

{WS}			;
\n			lineno++;
.			return yytext[0];

%%
