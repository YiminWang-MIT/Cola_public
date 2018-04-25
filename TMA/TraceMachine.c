/* 
 *	Copyright (c) 1992 by   Institut fuer Kernphysik,
 *				Johannes Gutenberg-Universitaet Mainz
 *
 *      The guy to blame        Helmut Kramer
 *
 *	%W%	KPH	%E%	%D% 
 *
 */


#ifdef sccs
static char SccsId[] = "%W%\t%G% (h.kramer/kph)";
#endif



#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


#define __TraceMachine_priv__
#include "TraceMachine.h"



#ifdef  TMA_USESIGNALS
static int tmaSigFlt;
#include <signal.h>

static int tmaSig()
{
  tmaSigFlt ++;
  return 0;
}


#endif


/*
   
   (x_fp,th_fp,y_fp,ph_fp) --> (dp_tg,th_tg,y_tg,ph_tg, len)

*/

static void fill_power(float *x, const int high, const float start) {
  if (x) {
    int i;
    *x = start;
    for (i = 0; i < high -1; ++i) {
      x[i+1] = start * x[i];
      /* *x++ = start * *x++  perverse nicht portable version */
    }
  }
}

static struct target_ko nullKo;

static void prepare_in_power(struct focalplane_ko*fp, TraceMachine *bp)
{
  bp->tg = nullKo;
/*  bzero((char *) &(bp -> tg),  sizeof(struct target_ko)); */


  fill_power(bp->pw.x_fp, bp->pw.max_x_fp, fp -> x_fp);
  fill_power(bp->pw.th_fp, bp->pw.max_th_fp, fp -> th_fp);
  fill_power(bp->pw.y_fp, bp->pw.max_y_fp, fp -> y_fp);
  fill_power(bp->pw.ph_fp, bp->pw.max_ph_fp, fp -> ph_fp);
}


static int in_power_init(TraceMachine *tp) {
#define INP_ALLOC_MAX(x) \
  if (tp -> pw.max_ ## x ## _fp &&\
      !(tp -> pw.x ## _fp = (float *)\
	malloc(tp -> pw.max_ ## x ## _fp * sizeof(float))))\
	  return (-1);
  
  INP_ALLOC_MAX(x);
  INP_ALLOC_MAX(th);
  INP_ALLOC_MAX(y);
  INP_ALLOC_MAX(ph);

  return 0;
}

/*
 *  matr_new : Constructor of a matr
 *  --------
 *  It allocates the memory and interprets the matrNode np. Thereby
 *  all pointers in the new matr are adjusted. bp provides the necessary
 *  information.
 */

static struct matr * matr_new(struct matrNode *np, struct btrace *bp) 
{
  struct matr *mp ;
  if ((mp = (struct matr *) malloc(sizeof(struct matr))) != NULL) {
    mp -> coeff = np -> coeff;
    switch (np -> i) {
    case DP_TG:
      mp -> sum   = &(bp->tg.dp_tg);
      break;
    case TH_TG:
      mp -> sum   = &(bp->tg.th_tg);
      break;
    case Y_TG:
      mp -> sum   = &(bp->tg.y_tg);
      break;
    case PH_TG:
      mp -> sum   = &(bp->tg.ph_tg);
      break;
    case LEN_TG:
      mp -> sum   = &(bp->tg.len);
      break;
    }
  }
  return mp;
}


static void matr_del(struct matr *mp) {
  free((char *) mp);
}

static void Power_del(struct power *p) {
  if (p) {
    struct matr **mp = p -> a;
    if (mp) {
      while (*mp) {
	matr_del(*mp);
	++mp;
      }
      free ((char *) p -> a);
    }
    free((char *) p);
  }
}


/*
 * power_init 
 * ----------
 * interprets the powerNode n. The information is used to
 * initialize p. 
 */

static int power_init(struct power *p, struct powerNode *n, struct btrace *bp) 
{

#define ADD_OF(x)\
  p->x ## _fp  = n->x ## _fp ? bp->pw.x ## _fp + (n->x ## _fp  - 1): NULL;
    
  ADD_OF(x);
  ADD_OF(th);
  ADD_OF(y);
  ADD_OF(ph);

  if ((p->a = (struct matr **) calloc(n -> no + 1, sizeof(struct matr *)))) {
    int i;

    for (i = 0; i < n -> no; ++i)  
      if (((p->a)[i] = matr_new((n->a)[i], bp)) == NULL) 
	return (-1);

    p->a[i] = NULL;
    return 0;
  }
  return -1;
}



/*
 *  class TraceMachine
 *
 */

TraceMachine * TMA_new(char * name, int mode)
{
  TraceMachine * aMachine;
  FILE *fp;

  if ((fp = fopen(name, "r")) == NULL) return NULL;
  
  if ((aMachine = (TraceMachine *) calloc(1, sizeof(TraceMachine)))) {
    if (TMA_compiled(fp, aMachine) != 0) {
      TMA_delete(aMachine);
      aMachine = NULL;
    }
    (void) fclose(fp);
  }


#ifdef  TMA_USESIGNALS
  if (aMachine) {    /* fast hack */
    signal(SIGFPE, tmaSig);
  }
#endif

  return aMachine;
}


void TMA_delete(TraceMachine *aMachine)
{

  if (aMachine) {
#ifdef  TMA_USESIGNALS
    signal(SIGFPE, aMachine->fpf);
#endif

    free (aMachine->name);
    if (aMachine -> pw.x_fp) free((char *) aMachine -> pw.x_fp);
    if (aMachine -> pw.th_fp) free((char *) aMachine -> pw.th_fp);
    if (aMachine -> pw.y_fp) free((char *) aMachine -> pw.y_fp);
    if (aMachine -> pw.ph_fp) free((char *) aMachine -> pw.ph_fp);
    Power_del(aMachine->in); 
    free ((char *) aMachine);
  }

}





/*
 *  Given a btrace object and a FILE containing precompiled
 *  Matrixdata. The object is initialized for further processing.
 *  
 *  The number of different powers is read. The following lines contain
 *  exact one powerNode. Each Node is read and parsed by init_power().
 *
 *  0 is returned on success.
 */

int 
TMA_compiled(FILE *fp, TraceMachine *bp) {

  struct powerNode pNode;
  struct matrNode cf[] = {
    {0.0, DP_TG}, {0.0,  TH_TG}, {0.0, Y_TG}, {0.0, PH_TG}, {0.0, LEN_TG}
  };

  if (fscanf(fp, "%d %hd %hd %hd %hd", & bp->len,  & bp->pw.max_x_fp, 
	     & bp->pw.max_th_fp,  & bp->pw.max_y_fp,  & bp->pw.max_ph_fp
	     ) == 5) {
    if (in_power_init(bp) != 0) 
      return (-1);

    if ((bp->in = (struct power *) calloc(bp -> len, sizeof(struct power)))) {
      struct power *ap;
      int n;
      
      for (ap = bp->in, n = bp->len; n; --n, ++ap) {
	if (fscanf(fp, "%hd %hd %hd %hd %f %f %f %f %f",
		   &pNode.x_fp, &pNode.th_fp, &pNode.y_fp, &pNode.ph_fp,
		   &cf[DP_TG].coeff, &cf[TH_TG].coeff, &cf[Y_TG].coeff,
		   &cf[PH_TG].coeff, &cf[LEN_TG].coeff) == 9) {
	  int i;

	  pNode.no = 0;
	  for (i = 0; i <= LEN_TG; ++i) 
	    if (cf[i].coeff != 0.0) {
	      (pNode.a)[pNode.no++] = cf + i;
	    }
	  
	  if (power_init(ap, &pNode, bp) != 0) 
	    return (-1);
	} else
	  return (-1);
      }
      return 0;
    }
  }
  return (-1);
}

struct target_ko * 
TMA_run(struct focalplane_ko *fp, TraceMachine *bp) {
  int i;
  int j;
  struct power   * ap;
  float prod;

#ifdef TMA_USESIGNALS
  tmaSigFlt = 0;
#endif

  prepare_in_power(fp, bp);

  ap = bp -> in;

  /* while (ap) { */
  for (j = 0; j < bp -> len; ++ j) {
    ap = bp -> in + j;
    for (i = 0; ap -> a[i]; ++i) {
      prod = 1.0;
#define PROD_IT(x)   if (ap -> x ## _fp)  prod  *=  (* ap-> x ## _fp) 
      PROD_IT(x);
      PROD_IT(th);
      PROD_IT(y);
      PROD_IT(ph);
      * ((ap -> a[i]) -> sum) += prod *  (ap -> a[i]) -> coeff;
    }
    ++ ap;
  }
#ifdef  TMA_USESIGNALS
  if (tmaSigFlt) {
    bp -> sigSeen = 1;        
    bp -> error   = TMA_ERR_FLT;
    return NULL;
  } 
#endif
  return(&(bp -> tg));
}





