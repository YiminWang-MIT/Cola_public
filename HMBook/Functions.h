static double HMop_add(double a, double b)      {return a+b;}
static double HMop_sub(double a, double b)      {return a-b;}
static double HMop_mult(double a, double b)     {return a*b;}
static double HMop_div(double a, double b)      {return a/b;}

static double HMlog2(double x)  { return log(x)/log(2);}
static double HMssqrt(double x) { return (x<0?0:sqrt(x));}
static double HMgauss(double x) { return 0.39894228 * exp(- x * x * 0.5);}

static struct {const char *name; double (*func)(double);} HMfuncdef[] =
{ {"exp",exp},    {"log",log},    {"log2",HMlog2},  {"log10",log10},
  {"sin",sin},    {"cos",cos},    {"tan",tan},
  {"asin",asin},  {"acos",acos},  {"atan",atan},
  {"sinh",sinh},  {"cosh",cosh},  {"tanh",tanh},
  {"ceil",ceil},  {"abs", fabs},  {"fabs", fabs},  {"floor",floor}, 
  {"sqrt",HMssqrt},
  {"gauss",HMgauss},
  {NULL,NULL}};

