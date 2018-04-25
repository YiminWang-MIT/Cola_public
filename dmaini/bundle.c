/*
 *	Copyright (c) 1992 by   Institut fuer Kernphysik,
 *				Johannes Gutenberg-Universitaet Mainz
 *
 *      The guy to blame:       Helmut Kramer
 *
 *	%W%	KPH	%E%	%D% 
 *
 */



/*  bundle.c
 *
 *  create out of the four paramater files normally used to describe
 *  the raytrace - polynom one file as input data for TraceMachine
 *
 */

static char SccsId[] = "%W%\t%G% (h.kramer/kph)";


#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#ifndef __hpux__
#include <getopt.h>
#endif

#include "thtrees.h"

#define MAXPARAMS  5 

typedef enum _TargetKo  { DP_TG = 0, TH_TG = 1, Y_TG = 2, PH_TG = 3,
                          LEN_TG = 4} TargetKo; 
struct powerNode { 
  short x_fp, th_fp, y_fp, ph_fp, no; 
  struct matrNode *a[1]; 
}; 

struct matrNode { 
  float coeff; 
  int   i; 
}; 
#define max(a,b) ((a) > (b)? (a) : (b))

/*
 *  Global Variables
 *  ----------------
 *
 *  progname : Programm - Name
 */


static char *progname;

static struct  {
  char * inFileName[MAXPARAMS];   /* filenames of input data              */
  FILE * inFile    [MAXPARAMS];   /*           and their file descriptors */ 
  int    i;                       /* number of file currently read in     */
  int   totalNodes;               /* total number of different exponets seen */
  short max_x_fp  ;               /* highest exponent in x_fp seen           */
  short max_th_fp ;             
  short max_y_fp  ;
  short max_ph_fp ;               /* highest exponent in ph_fp seen          */
  THNode *db;                     /* pointer to tree containing all nodes    */
  FILE * outfile;                 /* Outdata file descriptor                 */
} db;

#define SERVICEDB (& db.db)

#define  matrNode_new() ((struct matrNode *) malloc(sizeof(struct matrNode)))


static void   powerNode_print(struct powerNode *np) 
{
  int i;

  fprintf(db.outfile, "%hd %hd %hd %hd ", 
	  np -> x_fp, np -> th_fp, np -> y_fp, np -> ph_fp);

  for (i = 0; i <= LEN_TG; ++i) {
    if (np -> a[i]) 
      fprintf(db.outfile, " %12g", (np->a)[i] -> coeff) ;
    else
      fprintf(db.outfile, "          0.0");
  }

  fprintf(db.outfile, "\n");
}

static int save_it(char **pp, enum VISIT order, void *unused)
{
  powerNode_print((struct powerNode *) *pp);
  return 1;
}

static struct powerNode * powerNode_new(void)
{
  struct powerNode *np;

  if ((np = (struct powerNode *) malloc(sizeof(struct powerNode)))) {
    np -> no = 0;
  }
    
  return np;
}

static  int norm(struct powerNode *a, struct powerNode *b)
{
  int diff;

  if ((diff = a -> x_fp - b -> x_fp) == 0)
    if ((diff = a -> th_fp - b -> th_fp) == 0)
      if ((diff = a -> y_fp - b -> y_fp) == 0)
	return (a -> ph_fp - b -> ph_fp);
  return(diff);
}

static struct powerNode* add_power(struct powerNode *pe)
{
  struct powerNode ** te;

  if ((te = (struct powerNode **) 
      thtsearch((char *) pe, SERVICEDB, (int (*)(char *, char *)) norm))){
    if (*te != pe) { 
      return (*te);
    }

    return(NULL);
  }

  fprintf(stderr, "%s :: (add_power): Fatal error occured\n", progname);
  exit(1);
  return (NULL);
}

int
get_data(struct powerNode *pn, struct matrNode *mp) 
{
#define MAX_LINE 80

  static char buf[MAX_LINE];
  char *sp;

  if (db.i > LEN_TG) {
    return 1; 
  }
  else if ((sp = fgets(buf, MAX_LINE, db.inFile[db.i]))) {
    if (sscanf(buf , "%f %hd %hd %hd %hd", 
	       & mp->coeff, &pn->x_fp, &pn->th_fp, &pn->y_fp, &pn->ph_fp)
	!= 5) return -1;
    mp -> i = db.i;
    return 0;
  }
  else if (feof(db.inFile[db.i])) {
    ++ db.i;
    return(get_data(pn, mp));
  } 
  return -1;
}


static int build_up()
{
  struct powerNode *pn = NULL, *wp;
  struct matrNode * mp;

  while (1) {
    mp = matrNode_new();
    if (!pn) pn = powerNode_new();
    
    if (!pn || !mp) 
      return (-1);
    
    switch(get_data(pn, mp)) {
    case 0:
      if (!(wp = add_power(pn))) {  /* a new node is added */
	wp = pn;                    
	pn = NULL;               
	++ db.totalNodes;
      }

      db.max_x_fp = max(wp->x_fp, db.max_x_fp);
      db.max_th_fp = max(wp->th_fp, db.max_th_fp);
      db.max_y_fp = max(wp->y_fp, db.max_y_fp);
      db.max_ph_fp = max(wp->ph_fp, db.max_ph_fp);

      (wp -> a) [mp -> i] = mp;
      
      break;
    case -1:
      return (-1);
      break;
    case 1:
      return 0;
      break;
    }
  }

}


void bundle ()
{

  if (build_up() == -1)  {
    fprintf(stderr, "%s :: build_up failed\n", progname) ;
    exit(1);
  }

  fprintf(db.outfile, "%d %hd %hd %hd %hd\n",  db.totalNodes, 
	  db.max_x_fp, db.max_th_fp, db.max_y_fp, db.max_ph_fp);
  

  thtpost(*SERVICEDB, save_it, NULL);

}



static int open_files(void) 
{
  int i;

  for (i = 0; i <=  LEN_TG; ++i) {
    if ((db.inFile[i] = fopen(db.inFileName[i] ,"r")) != NULL)
      continue;
    else {
      fprintf(stderr, "%s :: Opening %s failed\n", progname, db.inFileName[i]);
      return -1;
    }
  }

  return 0;
}


int
main(int argc, char *argv[])
{
  extern char *optarg;
  int errflg =0;
  int c;

  /* default file names */

  db.inFileName[DP_TG] = "delta.hmx";
  db.inFileName[TH_TG] = "theta0.hmx";
  db.inFileName[Y_TG]  = "y0.hmx";
  db.inFileName[PH_TG] = "phi0.hmx";
  db.inFileName[LEN_TG] = "length.hmx";
  db.outfile = stdout;


  progname = argv[0];

  while((c = getopt(argc, argv, "d:l:t:y:p:o:v?")) != EOF) {
    switch(c) {
    case 'd':
      db.inFileName[DP_TG] = optarg;
      break;
     case 'l':
      db.inFileName[LEN_TG] = optarg;
      break;
    case 't' :
      db.inFileName[TH_TG] = optarg;
      break;
    case 'y' :
      db.inFileName[Y_TG]  = optarg;
      break;
    case 'p' :
      db.inFileName[PH_TG] = optarg;
      break;
    case 'o' :
      if (!(db.outfile = fopen(optarg, "w"))) {
	fprintf(stderr, "%s :: Opening %s failed\n", progname, optarg);
	exit(1);
      }
      break;
    case 'v':
      printf("Version info on %s : %s\n", argv[0], SccsId);
      exit(0);
    case '?':
      errflg ++;
      break;
    }
  }


  if (errflg) {
    fprintf(stderr, "%s :: [options]\n", argv[0]);
    fprintf(stderr, "\t [-d fname] delta-coefficients default: delta.hmx\n");
    fprintf(stderr, "\t [-l fname] length-coefficients default: length.hmx\n");
    fprintf(stderr, "\t [-t fname] theta0-coefficients default: theta0.hmx\n");
    fprintf(stderr, "\t [-y fname] y0-coefficients default: y0.hmx\n");
    fprintf(stderr, "\t [-p fname] phi0-coefficients default: phi0.hmx\n");
    fprintf(stderr, "\t [-o fname] output file  default: stdout\n");
    fprintf(stderr, "\t [-v] print version and exit\n");
    exit(0);
  }

  if (open_files())  {
    return (1);
  }
    
  bundle();
  return 0;
}

