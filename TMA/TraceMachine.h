#ifndef __TraceMachine__ 
#define __TraceMachine__ 
 
/* 
 *	Copyright (c) 1992 by   Institut fuer Kernphysik, 
 *				Johannes Gutenberg-Universitaet Mainz 
 * 
 *      The guy to blame:       Helmut Kramer 
 * 
 *	%W%	KPH	%E%	%D%  
 * 
 */ 

struct target_ko { 
  float dp_tg; 
  float th_tg; 
  float y_tg; 
  float ph_tg; 
  float len;
}; 
 
struct focalplane_ko { 
  float x_fp; 
  float th_fp; 
  float y_fp; 
  float ph_fp; 
}; 
 
#if defined(__TraceMachine_priv__) 
typedef enum _TargetKo  { DP_TG = 0, TH_TG = 1, Y_TG = 2, PH_TG = 3,
			  LEN_TG = 4} TargetKo; 
typedef enum _FocalPlaneKo {  
  X_FP = 0, TH_FP = 1, Y_FP = 2, PH_FP = 3} FocalPlaneKo; 
 
#define MAXPOWER   4 
#define MAXPARAMS  5 
 
struct in_power { 
  float *x_fp;   
  float *th_fp; 
  float *y_fp; 
  float *ph_fp; 
  short max_x_fp ;  /* MAXPOWER */ 
  short max_th_fp; 
  short max_y_fp; 
  short max_ph_fp; 
}; 
 
struct matr { 
  float  coeff; 
  float  *sum; 
}; 

struct power { 
  float *x_fp;  
  float *th_fp; 
  float *y_fp; 
  float *ph_fp; 
  struct matr **a; 
}; 

struct matrNode { 
  float coeff; 
  int   i; 
}; 

struct powerNode { 
  short x_fp;   /* {0, 1, ... ,  MAXPOWER} */ 
  short th_fp; 
  short y_fp; 
  short ph_fp; 
  short no; 
  struct matrNode *a[MAXPARAMS]; 
}; 
 
#define  TMA_ERR_FLT  1  /* floating point */

struct btrace { 
  struct target_ko  tg;  /* Targetkoordinaten                           */ 
  struct power    * in;  /* Calculation Instructions  (NULL terminated) */ 
  unsigned         len;  
  struct in_power   pw; 
  char            *name; 

  int           sigSeen;        
  int           error;
  void         (*fpf)(int);
}; 

typedef struct  btrace  TraceMachine; 
 
#else 

typedef char TraceMachine; 

#endif  /* __TraceMachine_priv__ */ 
 
#ifdef __cplusplus 
extern "C" { 
#endif 

#include <stdio.h>

TraceMachine     *TMA_new(char *, int mode); 
void             TMA_delete(TraceMachine *); 
int              TMA_compiled(FILE *, TraceMachine *); 
struct target_ko *TMA_run(struct focalplane_ko *, TraceMachine *); 
 
#ifdef __cplusplus 
} 
#endif 
 
#ifdef __cplusplus 
 
#include <iostream> 
 
class TracMachine {
private: 
  TraceMachine *tp; 
  double       x_off, y_off, th_off, ph_off; 
  double       x_scale, y_scale; 
  void init(double x = 0, double th = 0, double y = 0, double ph = 0,  
	    double x_s = 0.1, double y_s = 0.1); 
public: 
  ~TracMachine (); 
  TracMachine (const char *__path, int mode = 0); 
  TracMachine (const char *__path, double x,  
	       double th, double y, double ph,  
	       double x_s = 0.1, double y_s = 0.1, int mode = 0); 
  /* default scaling:  mm -> cm */    

  // run  tma, use offsets and scale !!! 
  struct target_ko * run(double x, double th, double y, double phi);    
  struct target_ko * run(struct focalplane_ko *);   
    
  operator int() const { return tp != NULL;} 
  int error() const; 
}; 
 
inline struct target_ko* TracMachine::run (struct focalplane_ko * fp)   
{ return (TMA_run(fp, tp)); 
} 
#endif 
#endif /* __TraceMachine__ */
