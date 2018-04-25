//								      -*-c++-*-
// Copyright (c) 1995 by	Michael O. Distler,
//				Institut fuer Kernphysik,
//				Johannes Gutenberg-Universitaet Mainz
//
// All rights reserved.
//
// $Header: /tmp/cvsroot/Cola/Cindy/CindyCard.l,v 4.7 2002-12-12 12:24:41 distler Exp $
//
// LEX Parser fuer die Cindy++ Control Cards (flex++ Version)
//

%name	CindyCardLEX

%define CONSTRUCTOR_PARAM	FILE *InputFile
%define CONSTRUCTOR_CODE	YY_CindyCardLEX_IN = InputFile;
%define CONSTRUCTOR_INIT	: lineno(1)
%define LEX_PARAM		YY_CindyCardYACC_STYPE *val
%define MEMBERS			public:\
				  int	lineno;

%{
#include <iostream>
#include "CindyCardYACC.h"
%}

%s	COINC SPEC_A SPEC_B SPEC_C

WS	[ \t\f]
FLOAT	[+\-]?([0-9]+\.[0-9]*|[0-9]*\.[0-9]+|[0-9]+)(e[+\-]?[0-9]+)?

%%

^#.*\n			lineno++;

coinc			BEGIN COINC;
speca			BEGIN SPEC_A;
specb			BEGIN SPEC_B;
specc			BEGIN SPEC_C;

[0-9]			val->num=atoi(yytext); return CindyCardYACC::DIGIT;
[+\-]?[0-9]*		val->lval=atol(yytext); return CindyCardYACC::LVALUE;
{FLOAT}			val->val=atof(yytext); return CindyCardYACC::VALUE;

<SPEC_A,INITIAL>angle	val->num=CindyCard_SpecA; return CindyCardYACC::ANGLE;
<SPEC_B>angle		val->num=CindyCard_SpecB; return CindyCardYACC::ANGLE;
<SPEC_C>angle		val->num=CindyCard_SpecC; return CindyCardYACC::ANGLE;

<SPEC_A,INITIAL>refmom	val->num=CindyCard_SpecA; return CindyCardYACC::REFMOM;
<SPEC_B>refmom		val->num=CindyCard_SpecB; return CindyCardYACC::REFMOM;
<SPEC_C>refmom		val->num=CindyCard_SpecC; return CindyCardYACC::REFMOM;

<COINC,INITIAL>beam	val->num=CindyCard_Coinc; return CindyCardYACC::BEAM;

<COINC,INITIAL>peak	val->num=CindyCard_Coinc; return CindyCardYACC::PEAK;
<SPEC_A>peak		val->num=CindyCard_SpecA; return CindyCardYACC::PEAK;
<SPEC_B>peak		val->num=CindyCard_SpecB; return CindyCardYACC::PEAK;
<SPEC_C>peak		val->num=CindyCard_SpecC; return CindyCardYACC::PEAK;

<COINC,INITIAL>delta	val->num=CindyCard_Coinc; return CindyCardYACC::DELTA;
<SPEC_A>delta		val->num=CindyCard_SpecA; return CindyCardYACC::DELTA;
<SPEC_B>delta		val->num=CindyCard_SpecB; return CindyCardYACC::DELTA;
<SPEC_C>delta		val->num=CindyCard_SpecC; return CindyCardYACC::DELTA;

<COINC,INITIAL>min	val->num=CindyCard_Coinc; return CindyCardYACC::MIN;
<SPEC_A>min		val->num=CindyCard_SpecA; return CindyCardYACC::MIN;
<SPEC_B>min		val->num=CindyCard_SpecB; return CindyCardYACC::MIN;
<SPEC_C>min		val->num=CindyCard_SpecC; return CindyCardYACC::MIN;

<COINC,INITIAL>max	val->num=CindyCard_Coinc; return CindyCardYACC::MAX;
<SPEC_A>max		val->num=CindyCard_SpecA; return CindyCardYACC::MAX;
<SPEC_B>max		val->num=CindyCard_SpecB; return CindyCardYACC::MAX;
<SPEC_C>max		val->num=CindyCard_SpecC; return CindyCardYACC::MAX;

<COINC,INITIAL>bins	val->num=CindyCard_Coinc; return CindyCardYACC::BINS;
<SPEC_A>bins		val->num=CindyCard_SpecA; return CindyCardYACC::BINS;
<SPEC_B>bins		val->num=CindyCard_SpecB; return CindyCardYACC::BINS;
<SPEC_C>bins		val->num=CindyCard_SpecC; return CindyCardYACC::BINS;

<COINC,INITIAL>flag	val->num=CindyCard_Coinc; return CindyCardYACC::FLAG;
<SPEC_A>flag		val->num=CindyCard_SpecA; return CindyCardYACC::FLAG;
<SPEC_B>flag		val->num=CindyCard_SpecB; return CindyCardYACC::FLAG;
<SPEC_C>flag		val->num=CindyCard_SpecC; return CindyCardYACC::FLAG;

{WS}			;
\n			lineno++;

%%
