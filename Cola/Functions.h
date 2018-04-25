#include "Aqua_online.h"
#include "Simulation/spinTransfer.h"

double op_add(double a, double b)      {return a+b;}
double op_sub(double a, double b)      {return a-b;}
double op_mult(double a, double b)     {return a*b;}
double op_div(double a, double b)      {return a/b;}
double op_lt(double a, double b)       {return a<b;}
double op_gt(double a, double b)       {return a>b;}
double op_le(double a, double b)       {return a<=b;}
double op_ge(double a, double b)       {return a>=b;}
double op_equal(double a, double b)    {return a==b;}
double op_unequal(double a, double b)  {return a!=b;}
double op_and(double a, double b)      {return (int)(a+0.5) && (int) (b+0.5);}
double op_or(double a, double b)       {return (int)(a+0.5) || (int) (b+0.5);}
double op_mod(double a, double b)      {return (int)(a+0.5) % (int) (b+0.5);}
#include <math.h>
#include <string.h>

double op_bitand(double a, double b)   {return (int)(a+0.5) & (int) (b+0.5);}
double op_bitor(double a, double b)    {return (int)(a+0.5) | (int) (b+0.5);}
double func_not(double a)              {return ! (int) (a+0.5);}

double log2(double x) {return log(x)/log(2);}
double ssqrt(double x) { return (x<0?0:sqrt(x));}
double sign(double x) {return x>=0 ? 1 : -1;}

struct fdef {char *name; double (*func)(double);} funcdef[] =
{ {"exp",exp},    {"log",log},    {"log2",log2},  {"log10",log10},
  {"sin",sin},    {"cos",cos},    {"tan",tan},
  {"asin",asin},  {"acos",acos},  {"atan",atan},
  {"sinh",sinh},  {"cosh",cosh},  {"tanh",tanh},
  {"ceil",ceil},  {"abs", fabs},  {"floor",floor},
  {"sign",sign},  {"sqrt",ssqrt},
  {NULL,NULL}};

FourVector op_neg_f(FourVector a)                { return -a; }

double op_mult_f(FourVector a, FourVector b)     { return a*b; }
FourVector op_add_f(FourVector a, FourVector b)  { return a+b; }
FourVector op_sub_f(FourVector a, FourVector b)  { return a-b; }
FourVector SpinEuler        (FourVector particle, FourVector refFrame);
FourVector SpinProjection   (FourVector particle, FourVector refFrame);
FourVector SpinNormalization(FourVector particle, FourVector refFrame);
// Actually, not defined for FourVector, i.e. only momentum stays defined:
FourVector mult(FourVector a, FourVector b) {return FourVector(0,a.mult(a,b));}

double momentum(FourVector f) { return f.momentum(); }
double momentumx(FourVector f) { return f[1]; }
double momentumy(FourVector f) { return f[2]; }
double momentumz(FourVector f) { return f[3]; }
double energy(FourVector f)   { return f.energy(); }
double mass(FourVector f)     { return f.mass(); }
double square(FourVector f)   { return f.square(); }
double beta(FourVector f)     { return f.beta().abs(); }
double gamma(FourVector f)    { return f.gamma(); }
double theta(FourVector f)    { return f.theta(); }
double phi(FourVector f)      { return f.phi(); }
double vartheta(FourVector f) { return f.varTheta(); }
double varphi(FourVector f)   { return f.varPhi(); }

struct fsfdef {char *name; double (*func)(FourVector f);} fsfuncdef[] =
{ {"momentum", momentum}, 
  {"momentumx", momentumx}, 
  {"momentumy", momentumy}, 
  {"momentumz", momentumz},
  {"X",         momentumx}, 
  {"Y",         momentumy}, 
  {"Z",         momentumz},
  {"energy",   energy},
  {"mass",     mass},
  {"square",   square},
  {"beta",     beta}, 
  {"gamma",    gamma}, 
  {"theta",    theta}, 
  {"phi",      phi},
  {"vartheta", vartheta},
  {"varphi",   varphi},
  {NULL,       NULL}
};

struct fsf2def {char *name; FourVector (*func)(FourVector a, FourVector b);} 
  fsfunc2def[] =
 { {"Lorentz",  Lorentz}, 
   {"Mult",     mult},
   {"Rotate",   rotate},
   {"RotateTo", rotateTo},
   {"SpinEuler",         SpinEuler},
   {"SpinProjection",    SpinProjection},
   {"SpinNormalization", SpinNormalization}, 
   {NULL,NULL}
 };

char *isfunction(char *name)
{ int pos=0; 
  while(funcdef[pos].name ? strcmp(name, funcdef[pos].name) : 0) pos++;
  if (funcdef[pos].name) return funcdef[pos].name;
  pos=0; 

  while(fsfuncdef[pos].name ? strcmp(name, fsfuncdef[pos].name) : 0) pos++;
  if (fsfuncdef[pos].name) return fsfuncdef[pos].name;
  return NULL;
}

char *isfunction2(char *name)
{ int pos=0; 
  while(fsfunc2def[pos].name ? strcmp(name, fsfunc2def[pos].name) : 0) pos++;
  if (fsfunc2def[pos].name) return fsfunc2def[pos].name;
  return NULL;
}
