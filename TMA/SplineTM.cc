//                                                                    -*-c++-*-
// the guy to blame:
// 
// Jan C. Bernauer             mailto:bernauer@kph.uni-mainz.de
// A1 Collaboration 
// Nov. of 2003 
//
// $Header: /tmp/cvsroot/Cola/TMA/SplineTM.cc,v 1.2 2004-07-21 10:35:39 bernauer Exp $
//


#include "transandlook.h"
#include "SplineTM.h"
#include <iostream>

using namespace std;

struct TargetCo *
SPLINE::run(double x_fp, double th_fp, double y_fp, double ph_fp)
{
    double src[DEF_DIMIN];
    double tgt[DEF_DIMOUT];
//    cerr <<"Ping"<<endl;
    src[0]=x_fp;src[1]=th_fp;src[2]=y_fp;src[3]=ph_fp;
    t.firstorderreconstruct(src);
    t.lookup(src,tgt);
//    cerr <<"Pong"<<endl;
    result.dp=tgt[0];result.th=tgt[1];result.y0=tgt[2]/10;result.ph=tgt[3];result.len=tgt[4];
  return &result;
}

SPLINE::SPLINE(const char *filename,const char *filename2):baseTM(filename)
{
  t.LoadSolution(filename);
  t.LoadFirstOrder(filename2);
}

SPLINE::~SPLINE()
{
}

int
SPLINE::setOffsets(double x_offs, double th_offs, double y_offs, double ph_offs)
{
    cerr<<" Warning: setOffsets is NOT IMPLEMENTED!!!"<<endl;
  return 0;
}
