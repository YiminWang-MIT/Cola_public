/*	Copyright (c) 1992 by   Institut fuer Kernphysik,
 *				Johannes Gutenberg-Universitaet Mainz
 *
 *                              Helmut Kramer
 *
 *	%W%	KPH	%E%	%D% 
 */

#define __TraceMachine_priv__ 1

#ifdef sccs
static char SccsId[] = "%W%\t%G% (h.kramer/kph)";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "TraceMachine.h"

int TracMachine::error() const { return tp ?  tp-> error : 1;}

void TracMachine:: init(double x, double th, double y , 
                        double ph , double x_s, double y_s) 
{
  x_off = x;
  th_off = th;
  y_off = y;
  ph_off = ph;
  x_scale = x_s;
  y_scale = y_s;

  return;
}

struct target_ko* TracMachine::run (double x, double th, double y, double phi)
{
  struct focalplane_ko fp;  
  fp.x_fp = (x - x_off) * x_scale;  
  fp.th_fp = th - th_off;
  fp.y_fp = (y - y_off) * y_scale;     
  fp.ph_fp = phi - ph_off;
  return (TMA_run(&fp, tp));
}

TracMachine::TracMachine (const char *path, int mode) 
{
  if ((tp = TMA_new ((char *) path, mode)) == NULL) {
    std::cerr << "TraceMachine construction failed with \"" << path << "\"" << std::endl;
    exit(-1);
  }

  init();
  return ;
}  

TracMachine::TracMachine(const char *path, double x, double th, double y, 
                         double ph, double x_s, double y_s, int mode)
{ if ((tp = TMA_new ((char *) path, mode)) == NULL) {
    std::cerr << "TraceMachine construction failed with \"" << path << "\"" << std::endl;
    exit(-1);
  }
  init(x, th, y , ph , x_s, y_s);
  return;
}

TracMachine::~TracMachine () { if (tp) TMA_delete (tp); } 
