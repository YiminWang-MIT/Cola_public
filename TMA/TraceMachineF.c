/*
 *	Copyright (c) 1992 by   Institut fuer Kernphysik,
 *				Johannes Gutenberg-Universitaet Mainz
 *
 *                              Helmut Kramer
 *
 *	%W%	KPH	%E%	%D% 
 *
 */


#ifdef sccs
static char SccsId[] = "%W%\t%G% (h.kramer/kph)";
#endif

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TraceMachine.h"

#ifndef min 
#define min(a,b)               ((a)>(b) ? (b) : (a))
#endif

TraceMachine * tma_new_(char * cp, int mode)
{
  char fname[80];
  int len = 79;
  char *bp;

  /* String Handling Fortran and C        */
  /*     files should have no blanks       */

  len = min(len, strlen(cp));

  if ((bp = strchr(cp, ' '))) {
    len = min((long) bp - (long) cp, len);
  }

  strncpy(fname, cp, len);
  fname[len] = 0;
  return(TMA_new(fname, mode));
}


void tma_delete_(TraceMachine **mp)
{
  if (mp) TMA_delete(*mp);
}

void tma_run_ (struct target_ko * tg, struct focalplane_ko *fp, 
	       TraceMachine ** tm) 
{
  struct target_ko *result =  TMA_run(fp, *tm);
  memcpy(result, tg, sizeof(struct target_ko));
}
