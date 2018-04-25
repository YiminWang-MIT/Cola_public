//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mit.edu
// MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/TMA/ftmInit.l,v 1.4 2002-11-26 17:11:27 distler Exp $
//
// LEX scanner (flex++ version)
// initialize the Fast Trace Machine
//

%name   ftmInitLEX

%define CONSTRUCTOR_PARAM	FILE *InputFile
%define CONSTRUCTOR_CODE	YY_ftmInitLEX_IN = InputFile;
%define CONSTRUCTOR_INIT	: lineno(1)
%define LEX_PARAM		YY_ftmInitYACC_STYPE *val
%define MEMBERS			public:\
				  int	lineno;

%{
#include "ftmInitYACC.h"
%}

WS	[ \t\f]
FLOAT	[+\-]?([0-9]+\.[0-9]*|[0-9]*\.[0-9]+|[0-9]+)([eE][+\-]?[0-9]+)?
STRING	[a-zA-Z]+[a-zA-Z0-9]*

%%

#.*\n		lineno++; return ftmInitYACC::EOL;

"x-offset"	return ftmInitYACC::XOFFSET;
"th-offset"	return ftmInitYACC::THOFFSET;
"y-offset"	return ftmInitYACC::YOFFSET;
"ph-offset"	return ftmInitYACC::PHOFFSET;

{FLOAT}		val->val = atof(yytext); return ftmInitYACC::VALUE;
{STRING}	{ std::cerr << "FastTM : unknown keyword '" << yytext
			    << "' - skipped" << std::endl;
		}
{WS}		;
\n		lineno++; return ftmInitYACC::EOL;
.		return yytext[0];

%%
