//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-456      tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/Chamber/hdcInit.y,v 2.9 2006-12-08 16:02:03 distler Exp $
//
// YACC parser (bison++ version)
// initialize OOPS horizontal driftchambers (HDCs)
//

%name   hdcInitYACC

%define LSP_NEEDED
%define ERROR_BODY		=0
%define LEX_BODY		=0
%define MEMBERS			protected: \
				  PlaneDefault planeDefault;
%define CONSTRUCTOR_CODE	memset(&planeDefault, 0, sizeof(PlaneDefault));

%header{
# include <math.h>
# ifdef __hpux__
# include <float.h>
# endif
# ifndef DBL_MAX
# include <float.h>
# endif
# include <string.h>
# include <limits.h>
# include <iostream>

  typedef struct {
    float bintons[2];
    float wire0offset;      // ns 
    float delayLineUnit;    // #/ns
    float driftTimeOffset;  // ns
    float adcLRinfo[4];     // left, mid, mid, right
    int   adcOEflip;        // true/false
    float zPosition;        // mm
    float xOffset;          // mm
    float rotation;         // mrad
    float lowDrifttime;     // ns
    float highDrifttime;    // ns
    int   entries;
    float * drifttimeTable;
  } PlaneDefault;
%}

%union {
  int	 num;
  double val;
}

%start list
%token WIRE0OFFSET DELAYLINEUNIT DRIFTTIMEOFFSET ZPOSITION
%token XOFFSET ROTATION LOWDRIFTTIME HIGHDRIFTTIME ENTRIES
%token BINTONS ADCLRINFO ADCOEFLIP DRIFTTIMETABLE
%token <num> INDEX
%token <val> VALUE
%type  <val> decl expr

%right  '='
%left   '+' '-'
%left   '*' '/' '%'
%left   UNARYMINUS
%left   UNARYPLUS

%%

list:
	| list ';'
	| list decl ';'
	| list error { yyerrok; }
	;
expr:   VALUE
	| expr '+' expr              { $$ = $1 + $3; }
	| expr '-' expr              { $$ = $1 - $3; }
	| expr '*' expr              { $$ = $1 * $3; }
	| expr '/' expr  {
	  if ($3 == 0.0) {
	    std::cerr << "ERROR (hdcInit) : devision by zero" << std::endl;
	    $$ = ($1 < 0.0) ?  -DBL_MAX : DBL_MAX;
	  } else {
	    $$ = $1 / $3;
	  }
	}
	| expr '%' expr              { $$ = fmod($1, $3); }
	| '(' expr ')'               { $$ = $2; }
	| '-' expr %prec UNARYMINUS  { $$ = -$2; }
	| '+' expr %prec UNARYPLUS   { $$ = $2; }
	;
decl:	expr
	| WIRE0OFFSET '=' decl      { $$ = planeDefault.wire0offset = $3; }
	| DELAYLINEUNIT '=' decl    { $$ = planeDefault.delayLineUnit = $3; }
	| DRIFTTIMEOFFSET '=' decl  { $$ = planeDefault.driftTimeOffset = $3; }
	| ZPOSITION '=' decl        { $$ = planeDefault.zPosition = $3; }
	| XOFFSET '=' decl          { $$ = planeDefault.xOffset = $3; }
	| ROTATION '=' decl         { $$ = planeDefault.rotation = $3; }
	| LOWDRIFTTIME '=' decl     { $$ = planeDefault.lowDrifttime = $3; }
	| HIGHDRIFTTIME '=' decl    { $$ = planeDefault.highDrifttime = $3; }
	| ADCOEFLIP '=' decl        { planeDefault.adcOEflip =
					(($$=$3) != 0.0); }
	| ENTRIES '=' decl          {
	  $$ = $3; planeDefault.entries = (int)$3;
	  planeDefault.drifttimeTable = new float[(int)$3];
	}
	| BINTONS '[' INDEX ']' '=' decl  {
	  if ((0 <= $3) && ($3 < 2)) {
	    planeDefault.bintons[$3] = $6;
	  } else {
	    std::cerr << "ERROR (hdcInit) : index [" << $3
		      << "] is out of range" << std::endl;
	  }
	  $$ = $6;
	}
	| ADCLRINFO '[' INDEX ']' '=' decl  {
	  if ((0 <= $3) && ($3 < 4)) {
	    planeDefault.adcLRinfo[$3] = $6;
	  } else {
	    std::cerr << "ERROR (hdcInit) : index [" << $3
		      << "] is out of range" << std::endl;
	  }
	  $$ = $6;
	}
	| DRIFTTIMETABLE '[' INDEX ']' '=' decl {
	  if (planeDefault.drifttimeTable &&
	      (0 <= $3) && ($3 < planeDefault.entries)) {
	    planeDefault.drifttimeTable[$3] = $6;
	  } else {
	    std::cerr << "ERROR (hdcInit) : index [" << $3
		      << "] is out of range" << std::endl;
	  }
	  $$ = $6;
	}
	;
%%
