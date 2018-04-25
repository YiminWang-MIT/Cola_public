//								      -*-c++-*-
// Created 1995 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// All rights reserved.
//
// $Header: /tmp/cvsroot/Cola/Cindy/CindyCard.y,v 4.8 2006-12-08 16:02:11 distler Exp $
//
// YACC Parser fuer die Cindy++ Control Cards (bison++ Version)
//

%name	CindyCardYACC

%define LSP_NEEDED
%define ERROR_BODY	=0
%define LEX_BODY	=0
%define MEMBERS		public:\
			  double beam;\
			  Coinc coinc;\
			  Spec specA;\
			  Spec specB;\
			  Spec specC;

%header{
    typedef struct coinc {
	double	peak[10];
	double	delta[10];
	double	min[10];
	double	max[10];
	long	bins[10];
	int	flag[10];
    } Coinc, *Cptr;

    typedef struct spec {
	double	angle;
	double	refMom;
	double	peak[10];
	double	delta[10];
	double	min[10];
	double	max[10];
	long	bins[10];
	int	flag[10];
    } Spec, *Sptr;

    const int CindyCard_Coinc = 0;
    const int CindyCard_SpecA = 1;
    const int CindyCard_SpecB = 2;
    const int CindyCard_SpecC = 3;
%}

%union {
  int	 num;
  long   lval;
  double val;
}

%start	list
%token	<num>	ANGLE BEAM REFMOM PEAK DELTA DIGIT MIN MAX BINS FLAG NUMBER
%token	<lval>	LVALUE
%token	<val>	VALUE

%%

list:
	| list decl
	| list error	{ yyerrok; }
	;
decl:	ANGLE VALUE	{
		    switch ($1) {
			case CindyCard_SpecA: specA.angle = $2; break;
			case CindyCard_SpecB: specB.angle = $2; break;
			case CindyCard_SpecC: specC.angle = $2; break;
		    }
		}
	| BEAM VALUE	{ beam = $2; break; }
	| REFMOM VALUE	{
		    switch ($1) {
			case CindyCard_SpecA: specA.refMom = $2; break;
			case CindyCard_SpecB: specB.refMom = $2; break;
			case CindyCard_SpecC: specC.refMom = $2; break;
		    }
		}
	| PEAK DIGIT VALUE	{
		    switch ($1) {
			case CindyCard_Coinc: coinc.peak[$2] = $3; break;
			case CindyCard_SpecA: specA.peak[$2] = $3; break;
			case CindyCard_SpecB: specB.peak[$2] = $3; break;
			case CindyCard_SpecC: specC.peak[$2] = $3; break;
		    }
		}
	| DELTA DIGIT VALUE	{
		    switch ($1) {
			case CindyCard_Coinc: coinc.delta[$2] = $3; break;
			case CindyCard_SpecA: specA.delta[$2] = $3; break;
			case CindyCard_SpecB: specB.delta[$2] = $3; break;
			case CindyCard_SpecC: specC.delta[$2] = $3; break;
		    }
		}
	| MIN DIGIT VALUE	{
		    switch ($1) {
			case CindyCard_Coinc: coinc.min[$2] = $3; break;
			case CindyCard_SpecA: specA.min[$2] = $3; break;
			case CindyCard_SpecB: specB.min[$2] = $3; break;
			case CindyCard_SpecC: specC.min[$2] = $3; break;
		    }
		}
	| MAX DIGIT VALUE	{
		    switch ($1) {
			case CindyCard_Coinc: coinc.max[$2] = $3; break;
			case CindyCard_SpecA: specA.max[$2] = $3; break;
			case CindyCard_SpecB: specB.max[$2] = $3; break;
			case CindyCard_SpecC: specC.max[$2] = $3; break;
		    }
		}
	| BINS DIGIT LVALUE	{
		    switch ($1) {
			case CindyCard_Coinc: coinc.bins[$2] = $3; break;
			case CindyCard_SpecA: specA.bins[$2] = $3; break;
			case CindyCard_SpecB: specB.bins[$2] = $3; break;
			case CindyCard_SpecC: specC.bins[$2] = $3; break;
		    }
		}
	| FLAG DIGIT DIGIT	{
		    switch ($1) {
			case CindyCard_Coinc:
				coinc.flag[$2] = ($3 != 0); break;
			case CindyCard_SpecA:
				specA.flag[$2] = ($3 != 0); break;
			case CindyCard_SpecB:
				specB.flag[$2] = ($3 != 0); break;
			case CindyCard_SpecC:
				specC.flag[$2] = ($3 != 0); break;
		    }
		}
	;
%%
