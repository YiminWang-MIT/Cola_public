//                                                                    -*-c++-*-
// Harald Merkel                  A1 Collaboration
//                                Institut für Kernphysik
//                                Universität Mainz
//
// Pablo Barneo -> splitting evaluate.h in two files: 
//                 evaluate.h and evaluate.cc
//
// $Id: evaluate.cc 2216 2008-06-13 21:13:47Z distler $
//
#include "evaluate.h"
#include "FourVector/FourVector.h"

memManager *memManager::first = NULL;
memManager *memManager::last  = NULL;

//////////////////////////////////////////////////////////////////////////////
// print(int indent) for debugging only

void expression::print(int indent)
{
  char *ind = new char[indent+1];
  for(int i = ind[indent] = 0; i < indent; i++) ind[i] = '_';
  std::cout << ind << (int) valid <<" "<< "expression" << std::endl;
}

template <> void bin<double>::print(int indent)
{
  char *ind = new char[indent+1];
  for(int i = ind[indent] = 0; i < indent; i++) ind[i] = '_';
  std::cout << ind << (int) valid
	    << " Binary Operator <double, double>" << std::endl;
  left->print(indent+2);
  right->print(indent+2);
}

template <> void bin<FourVector>::print(int indent) 
{ 
  char *ind = new char[indent+1];
  for(int i = ind[indent] = 0; i < indent; i++) ind[i] = '_';
  std::cout << ind << (int) valid
	    << " Binary Operator <4Vec, 4Vec>" << std::endl;
  left->print(indent+2);
  right->print(indent+2);
}

template <> void scalarbin<FourVector>::print(int indent) 
{ 
  char *ind = new char[indent+1];
  for(int i = ind[indent] = 0; i < indent; i++) ind[i] = '_';
  std::cout << ind << (int) valid << " Scalar binary Operator <4Vec, 4Vec>" 
	    << std::endl;
  left->print(indent+2);
  right->print(indent+2);
}

template <> void func<double, double>::print(int indent) 
{
  char *ind = new char[indent+1];
  for(int i = ind[indent] = 0; i < indent; i++) ind[i] = '_';
  std::cout << ind << (int) valid << " double Function <double>" << std::endl;
  argument->print(indent+2);
}

template <> void func<double, FourVector>::print(int indent)
{
  char *ind = new char[indent+1];
  for(int i = ind[indent] = 0; i < indent; i++) ind[i] = '_';
  std::cout << ind << (int) valid << " double Function <4Vec>" << std::endl;
  argument->print(indent+2);
}

template <> void func<FourVector, FourVector>::print(int indent)
{
  char *ind = new char[indent+1];
  for(int i = ind[indent] = 0; i < indent; i++) ind[i] = '_';
  std::cout << ind << (int) valid << " 4Vec Function <4Vec>" << std::endl;
  argument->print(indent+2);
}

template <> void literal<double     >::print(int indent) 
{
  char *ind = new char[indent+1];
  for(int i = ind[indent] = 0; i < indent; i++) ind[i] = '_';
  std::cout << ind << (int) valid << " <double>" << std::endl;
}
template <> void literal<double    *>::print(int indent)
{
  char *ind = new char[indent+1];
  for(int i = ind[indent] = 0; i < indent; i++) ind[i] = '_';
  std::cout << ind << (int) valid << " <double *>" << std::endl;
}
template <> void literal<AquaNode *>::print(int indent)
{
  char *ind = new char[indent+1];
  for(int i = ind[indent] = 0; i < indent; i++) ind[i] = '_';
  std::cout << ind << (int) valid << " <Aqua>" << std::endl;
}

template <> void literal<FourVector >::print(int indent)
{
  char *ind = new char[indent+1];
  for(int i = ind[indent] = 0; i < indent; i++) ind[i] = '_';
  std::cout << ind << (int) valid <<" <4Vec>" << std::endl;
}

void constFour::print(int indent)
{
  char *ind = new char[indent+1];
  for(int i = ind[indent] = 0; i < indent; i++) ind[i] = '_';
  std::cout << ind << (int) valid
	    << " 4Vector constructor(double,double,double,double)"
	    << std::endl;
  arg1->print(indent+2);
  arg2->print(indent+2);
  arg3->print(indent+2);
  arg4->print(indent+2);
}
