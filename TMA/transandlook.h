// the guy to blame:
// 
// Jan C. Bernauer             mailto:bernauer@kph.uni-mainz.de
// A1 Collaboration 
// Nov. of 2003 
//

#ifndef TRANSANDLOOK_H
#define TRANSANDLOOK_H

#include <fstream>
#include <iostream>


// 5 output variables, 4 input, atm.

#define DEF_DIMIN 4
#define DEF_DIMOUT 5

// Factor the grid is larger
#define DEF_GRIDOVER 1.01

//input grid dimensions
#define DEF_UNIGRID 8
#define DEF_0 DEF_UNIGRID
#define DEF_1 DEF_UNIGRID
#define DEF_2 DEF_UNIGRID
#define DEF_3 DEF_UNIGRID

#define DEF_NMAX ((DEF_0+4)*(DEF_1+4)*(DEF_2+4)*(DEF_3+4))

class TL 
{
private:
double solution[DEF_NMAX][DEF_DIMOUT];
double firstorderconst[DEF_DIMIN];
double firstorderlin[DEF_DIMIN][DEF_DIMIN];
double firstorderquad[DEF_DIMIN][DEF_DIMIN][DEF_DIMIN];
double m_centerSrc[DEF_DIMIN],m_lengthSrc[DEF_DIMIN];
double m_centerTgt[DEF_DIMOUT];
public:
 TL();
int DEF_GRIDSIZE[DEF_DIMIN];
 int DEF_N;
void firstorderreconstruct(double *v);
void lookup(double *src,double *tgt);
 void LoadSolution(const char *name);
void LoadFirstOrder(const char *name);
};

#endif
