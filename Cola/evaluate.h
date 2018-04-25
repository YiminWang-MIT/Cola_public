//                                                                    -*-c++-*-
// Harald Merkel                  A1 Collaboration
//                                Institut für Kernphysik
//                                Universität Mainz
//
// $Header: /tmp/cvsroot/Cola/Cola/evaluate.h,v 2.7 2006-06-27 12:34:08 distler Exp $
//
#ifndef __EVALUATE_H__
#define __EVALUATE_H__

#include "../FourVector/FourVector.h"
#include "Aqua/AquaTree.h"
#include <iostream>

const char EVALUATED =  1;
const char ISFALSE   =  0;
const char NOTVALID  = -1;

/// base class expression to build an evaluation tree
class expression 
{
private:
  void *result;                            // contains result of evaluation

protected:
  char valid;                              // TRUE if already evaluated
  void *res() { if (!valid) result = eval(); return result; };
  virtual void *eval() = 0;  
  expression() { valid = ISFALSE; };       // Never called by user!

public:
  virtual ~expression() { ; }
  static expression *current;
  const char *name;
  /// expression valid? (or e.g. access to AquaNode not OK)
  int isValid() { if (!valid) result = eval(); return (valid == EVALUATED); }
  /// evaluate expression and return double
  double       evaluate()   { return * (double     *) res(); };
  /// evaluate expression and return FourVector
  FourVector   evaluate_f() { return * (FourVector *) res(); };
  /// reset expression to guarantee new evaluation
  virtual void reset()      { valid = ISFALSE; };
  virtual void print(int ident=0);
};

/// expression containing only a literal value. e.g. a const number
template <class data> class literal : public expression
{
private:
  void *eval();
  double doublevalue;

public:
  virtual ~literal() { ; }
  ///
  data value;
  /// constructor with value of this literal object
  literal (data f) { name=NULL; valid=ISFALSE; value = f; };
  void reset ();
  void print(int ident);
  /// mark expression as valid or not valid.
  void setValid (char isvalid) { valid = isvalid; }
};

/// expression consists of a binary operator and two expressions
template <class data> class bin : public expression
{
private:
  data value;
  data (*operation) (data left, data right);

public:
  expression *left, *right;
  void print(int ident);
  void reset() {
    if (valid != ISFALSE) {
      valid = ISFALSE;
      left->reset();
      right->reset();
    }
  }
  bin(expression *a, expression *b, data (*op)(data, data)) {
    name=NULL; valid=ISFALSE;left=a; right=b; 
    operation = (data (*)(data, data)) op; 
  }
  void *eval();
};

/// expression consists of a binary operator and two expressions, result scalar
template <class data> class scalarbin : public expression
{
private:
  double value;
  double (*operation) (data left, data right);

public:
  expression *left, *right;
  void print(int ident);
  void reset() {
    if (valid != ISFALSE) {
      valid = ISFALSE;
      left->reset();
      right->reset();
    }
  }
  scalarbin(expression *a, expression *b, double (*op)(data, data)) {
    name=NULL; valid=ISFALSE;left=a; right=b; 
    operation = (double (*)(data, data)) op; 
  }
  void *eval();
};

/// expression consists of function and argument
template <class Result, class arg> class func : public expression
{
private:
  Result value;
  Result (*operation)(arg a);
  expression *argument;
public:
  void print(int ident);
  void reset() {if (valid!=ISFALSE) {valid=ISFALSE; argument->reset();}};
  func(expression *ag, Result (*op)(arg)) {
    name=NULL; valid=ISFALSE;argument=ag; operation=op; 
  }
  void *eval();
};

template <class Result, class arg> class checkvalid : public expression
{
private:
  Result value;
  expression *argument;
public:
  void print(int indent){
    char *ind = new char[indent+1];
    for(int i = ind[indent] = 0; i < indent; i++) ind[i] = '_';
    std::cout << ind << (int) valid
	      << " Check <scalar>" << std::endl;
    argument->print(indent+2);
  }
  void reset() {if (valid!=ISFALSE) {valid=ISFALSE; argument->reset();}};
  checkvalid(expression *ag) {
    name=NULL; valid=ISFALSE;argument=ag;
  }
  void *eval();
};

//expression consists of four doubles, result fourvector
class constFour : public expression
{
private:
  FourVector value;
  expression *arg1, *arg2, *arg3, *arg4;
public:
  virtual ~constFour() { ; }
  void print(int ident);
  void reset() {
    if (valid!=ISFALSE) {
      valid=ISFALSE; 
      arg1->reset();    arg2->reset();    arg3->reset();    arg4->reset();
    }
  };
  constFour(expression *ag1,expression *ag2,expression *ag3,expression *ag4) {
    name=NULL; valid=ISFALSE;arg1=ag1;arg2=ag2;arg3=ag3;arg4=ag4;
  };
  inline void *eval() 
  { 
    valid = ((arg1->isValid() && arg2->isValid()
	      && arg3->isValid() && arg4->isValid()) ? EVALUATED : NOTVALID);
    if (valid==EVALUATED) 
      value = FourVector(arg1->evaluate(),arg2->evaluate(),
			 arg3->evaluate(),arg4->evaluate());
    return &value;
  }
};

class memManager {
protected:
  static memManager * first;
  static memManager * last;
  memManager * next;
public:
  memManager() {
    next = NULL;
    if (last) {
      last->next = this;
    } else {
      first = this;
    }
    last = this;
  }
  virtual ~memManager() { ; }
  static void delete_all() {
    memManager *nxt, *base = first;
    while (base) {
      nxt = base->next;
      delete base;
      base = nxt;
    }
    first = last = NULL;
  }
};

class mmDouble : public memManager {
  double * mem;
public:
  mmDouble(double * m) { mem = m; }
  ~mmDouble() { delete mem; }
};

class mmChar : public memManager {
  char * mem;
public:
  mmChar(char * m) { mem = m; }
  ~mmChar() { delete mem; }
};

class mmAqua : public memManager {
  AquaNode * mem;
public:
  mmAqua(AquaNode * m) { mem = m; }
  ~mmAqua() { delete mem; }
};

class mmExpr : public memManager {
  expression * mem;
public:
  mmExpr(expression * m) { mem = m; }
  ~mmExpr() { delete mem; }
};

//
template<> inline void *literal<double    >::eval() 
{ 
  valid = EVALUATED; return &value; 
}

template<> inline void *literal<FourVector>::eval() 
{ 
  valid = EVALUATED; return &value; 
}

template<> inline void *literal<double   *>::eval() 
{ 
  valid = EVALUATED; return value; 
}

template<> inline void *literal<AquaNode *>::eval()  
{
  valid = (value->ok() ? EVALUATED : NOTVALID);
  doublevalue = value->scalar();
  return &doublevalue;
}

template<> inline void literal<double    >::reset() { }
template<> inline void literal<double   *>::reset() { valid = ISFALSE; }
template<> inline void literal<AquaNode *>::reset() { valid = ISFALSE; }
template<> inline void literal<FourVector>::reset() { valid = ISFALSE; }

//
double op_and(double, double);
double op_or(double, double);

template<> inline void *
bin<double>::eval()
{
  if (operation == op_or) {
    valid = ((left->isValid() || right->isValid()) ? EVALUATED : NOTVALID);
    value =  (left->isValid() && (left->evaluate() != 0.0))
      || (right->isValid() && (right->evaluate() != 0.0));    
  } else {
    if (operation == op_and && left->isValid())
      if (left->evaluate() == 0.0) {
	valid = EVALUATED;
	value = 0;
	return &value;
      }
    valid = ((left->isValid() && right->isValid()) ? EVALUATED : NOTVALID);
    if (valid==EVALUATED) value=operation(left->evaluate(),right->evaluate());
  }
  return &value;
}

template<> inline void *
bin<FourVector>::eval() 
{ 
  valid = ((left->isValid() && right->isValid()) ? EVALUATED : NOTVALID);
  if (valid==EVALUATED) 
    value = operation(left->evaluate_f(), right->evaluate_f());
  return &value;
}

template<> inline void *
scalarbin<FourVector>::eval() 
{ 
  valid = ((left->isValid() && right->isValid()) ? EVALUATED : NOTVALID);
  if (valid==EVALUATED) 
    value = operation(left->evaluate_f(), right->evaluate_f());
  return &value;
}

template<> inline void *
func<double, double>::eval() 
{
  valid = (argument->isValid() ? EVALUATED : NOTVALID);
  if (valid==EVALUATED) value = operation(argument->evaluate());
  return &value;
}

template<> inline void *
checkvalid<double, double>::eval() 
{
  valid= EVALUATED;
  value = argument->isValid();
  return  &value;
}

template<> inline void *
func<double, FourVector>::eval() 
{
  valid = (argument->isValid() ? EVALUATED : NOTVALID);
  if (valid==EVALUATED) value = operation(argument->evaluate_f());
  return &value;
}

template<> inline void *
func<FourVector, FourVector>::eval()
{
  valid = (argument->isValid() ? EVALUATED : NOTVALID);
  if (valid==EVALUATED) value = operation(argument->evaluate_f());
  return &value;
}

#endif
