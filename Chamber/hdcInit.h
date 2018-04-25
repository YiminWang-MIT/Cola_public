//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-456      tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/Chamber/hdcInit.h,v 2.2 1998-01-13 17:52:41 distler Exp $
//
// Parser to initialize OOPS horizontal driftchambers (HDCs)
//

#ifndef __hdcInit_h__
#define __hdcInit_h__

#include <errno.h>
#include "../Chamber/hdcInitYACC.h"
#include "../Chamber/hdcInitLEX.h"

//
//	actually defined in hdcInitYACC.h
//
/*
typedef struct {
  float bintons[2];
  float wire0offset;      // ns 
  float delayLineUnit;    // #/ns
  float driftTimeOffset;  // ns
  float adcLRinfo[4];     // left, mid, mid, right 
  float zPosition;        // mm   
  float xOffset;          // mm
  float rotation;         // mrad
  float lowDrifttime;     // ns
  float highDrifttime;    // ns
  int   entries;
  float * drifttimeTable;
} PlaneDefault;
*/

class hdcInit : public hdcInitYACC {
  FILE *file;
  int yerrs;
  hdcInitLEX * lex;
  virtual int  yylex();
  virtual void yyerror(char * m);
public:
  hdcInit(const char * initfile);
  virtual ~hdcInit();
  void planeInit(PlaneDefault * init);
};

#endif /* __hdcInit_h__ */
