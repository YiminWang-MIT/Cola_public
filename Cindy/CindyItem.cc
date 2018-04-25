//
// Created 1994-95 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Cindy/CindyItem.cc,v 4.7 1997-07-13 04:56:13 distler Exp $
//
// Implementation of class CindyItem
//

#include "Cindy++.h"
#include "CindyNode.h"

unsigned short
CindyItem::ok()
{
    return (item ? item->ok() : 0);
}

int
CindyItem::operator()()
{
    if (!item) return 0;

    switch (item->typeOf()) {
    case CindyNode::S  :
    case CindyNode::US :
	return *(short *)item->addrOf();
	break;
    case CindyNode::I  :
    case CindyNode::UI :
    case CindyNode::L  :
    case CindyNode::UL :
	return *(int *)item->addrOf();
	break;
    default :
	return 0;
	break;
    }

    return 0;
}

double
CindyItem::scalar()
{
    if (!item) return 0.0;

    switch (item->typeOf()) {
    case CindyNode::S  :
	return *(short *)item->addrOf();
	break;
    case CindyNode::US :
	return *(unsigned short *)item->addrOf();
	break;
    case CindyNode::I  :
    case CindyNode::L  :
	return *(int *)item->addrOf();
	break;
    case CindyNode::UI :
    case CindyNode::UL :
	return *(unsigned int *)item->addrOf();
	break;
    case CindyNode::F :
	return *(float *)item->addrOf();
	break;
    case CindyNode::D :
	return *(double *)item->addrOf();
	break;
    default :
	return 0.0;
	break;
    }

    return 0.0;
}

#define ADDOP(typ,var,tag) \
int CindyItem::operator()(typ *var) \
{ \
  if (item->typeOf() != CindyNode::tag) return -1; \
  *var = *(typ *)item->addrOf(); return 0; \
}

ADDOP(short,s,S)
ADDOP(unsigned short,us,US)
ADDOP(int,i,I)
ADDOP(unsigned int,ui,UI)
ADDOP(long,l,L)
ADDOP(unsigned long,ul,UL)
ADDOP(float,f,F);
ADDOP(double,d,D);
