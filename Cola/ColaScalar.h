//                                                                -*-c++-*-

#ifndef __COLASCALAR_H__
#define __COLASCALAR_H__

#include "evaluate.h"

///
class Cola_Scalar
{
  ///
  Cola_Scalar *next;
  ///
  static Cola_Scalar *first;
  ///
  static Cola_Scalar *last;
public:
  ///
  const char *name;
  ///
  const char *unit;
  ///
  const char *label;
  ///
  expression *expr;
  ///
  double value;

  ///
  Cola_Scalar(const char *nme, expression *express, 
	      const char *Label=NULL, const char *Unit=NULL) {
    next = NULL;
    if (!first)
      first = this; 
    else
      last->next = this;
    last = this;
    name = nme;
    expr = express;
    expr->name = name;
    unit = Unit;
    label = Label;
  } 
  ///
  void reset() { if (expr) expr->reset(); };
  ///
  static void reset_all();
  ///
  static void delete_all();
  ///
  static Cola_Scalar *find(const char* name);
  ///
  static char *namescalar(const char *name);
  ///
  static expression *findscalar(const char *name);
};

#endif

