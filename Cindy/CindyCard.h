//								-*-c++-*-
// Copyright (c) 1995 by	Michael O. Distler,
//				Institut fuer Kernphysik,
//				Johannes Gutenberg-Universitaet Mainz
//
// All rights reserved.
//
// $Header: /tmp/cvsroot/Cola/Cindy/CindyCard.h,v 4.6 1997-07-09 20:39:45 distler Exp $
//

#ifndef __CindyCard_h__
#define __CindyCard_h__

#include <errno.h>
#include "Cindy/CindyCardYACC.h"
#include "Cindy/CindyCardLEX.h"

//
//	actually defined in CindyCardYACC.h
//
//    typedef struct coinc {
//	double	peak[10];
//	double	delta[10];
//	double	min[10];
//	double	max[10];
//	double	bins[10];
//	int	flag[10];
//    } Coinc, *Cptr;
//
//    typedef struct spec {
//	double	angle;
//	double	refMom;
//	double	peak[10];
//	double	delta[10];
//	double	min[10];
//	double	max[10];
//	double	bins[10];
//	int	flag[10];
//    } Spec, *Sptr;
//

class CindyCard : public CindyCardYACC {
//
// from class CindyCardYACC
//
// public:
//	  double beam;
//	  Coinc coinc;
//	  Spec specA;
//	  Spec specB;
//	  Spec specC;
//
  FILE *file;
  int yerrs;
  CindyCardLEX *cindyCardLEX;
  virtual int yylex();
  virtual void yyerror(char *m);
public:
  CindyCard(char *cardfile);
  virtual ~CindyCard();
};

#endif /* __CindyCard_h__ */
