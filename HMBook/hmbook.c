#define GCC_VERSION (__GNUC__ * 10000 \
                   + __GNUC_MINOR__ * 100 \
                   + __GNUC_PATCHLEVEL__)
#if GCC_VERSION > 40800
#define _DEFAULT_SOURCE 1
#else
#define _BSD_SOURCE 1
#endif
#include <config.h>
#include "hmbook.h"
#include "hmplot.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <sys/sysinfo.h>


#ifndef O_SYNC
#define O_SYNC	O_FSYNC
#endif

#define OLDPOINTER 4

#define VERSION 1.5


#undef  HMBOOK_CHECKINCREMENT
#define HMBOOK_CHECKINCREMENT_FACTOR (0.75)

#define bn(a,b) id->data[(a)*id->ny+(b)]

FLOAT HMVersion;

extern unsigned int sleep(unsigned int);
/* extern int fork(void); */
extern FLOAT HMmaximumx, HMminimumx;
extern FLOAT HMmaximumy, HMminimumy;
extern FLOAT HMmaximumz, HMminimumz;

HIST *HMFirst=NULL,
     *HMLast;

unsigned int mem_unit;
unsigned long totalram = 0;

double getmemorysize() {
  if (!totalram) {
    struct sysinfo si;
    sysinfo(&si);
    mem_unit = si.mem_unit;
    totalram = si.totalram;
  }
  return totalram * (double) mem_unit;
}

double HMMemory=0;

void *HMcalloc(size_t nmemb, size_t size) {
  HMMemory += nmemb*size;
  if (HMMemory > getmemorysize()/2)
    ;
    //bss: nice idea, but actually does not show that message when it is correct in general
    //    fprintf(stderr,"WARNING: Using more than half of the "
    //	    "physical memory (%.3f GByte) for histograms!\n",
    //	    totalram/1073741824.0*mem_unit);
  void *res = calloc(nmemb,size);
  if (!res) { fprintf(stderr,"Out of Memory\n");exit(-1);}
  return res;
}

void *HMmalloc(size_t size) {
  HMMemory += size;
  if (HMMemory > getmemorysize()/2)
    ;
    //bss: nice idea, but actually does not show that message when it is correct in general
    //    fprintf(stderr,"WARNING: Using more than half of the "
    //	    "physical memory (%.3f GByte) for histograms!\n",
    //      totalram/1073741824.0*mem_unit);
  void *res = malloc(size);
  if (!res) { fprintf(stderr,"Out of Memory\n");exit(-1);}
  return res;
}

static int rebin = 1;
FLOAT HMVersion;

void HMSetRebin(int r){ rebin=r;}

void myfread(void *ptr, size_t size, size_t nmemb, FILE *stream) {
  size_t x=fread(ptr,size,nmemb,stream);
    if (x != nmemb) {
      //     perror("fread failed");
  //    //    exit(-1);
   }
}

void myfwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
  if(fwrite(ptr,size,nmemb,stream)!=nmemb) {
      perror("fread failed");
      //      exit(-1);
    } 
}

HIST *HMBook1   (const char *title, 
		 const char *labelx, const char *labely, 
                 const char *unitx, const char *unity,
		 int nx, FLOAT minx, FLOAT maxx)
{ int i;
  HIST *id;

  id =  HMcalloc(1, sizeof(HIST));
  id->data = HMcalloc(nx, sizeof(FLOAT));

  if (!id || !id->data) 
  { puts("Error in HMBook1: not enough memory. ");
    exit(-1);
  } 

  id->divide=NULL;
  id->scale=1;
  id->title	= title;
  id->name 	= title;
  id->labelx	= labelx;
  id->labely	= labely;
  id->unitx	= unitx;
  id->unity	= unity;
  id->labelz	= id->unitz = "";
  id->nx	= nx;
  id->ny	= 0;
  id->entries	= 0;
  id->maxx	= maxx;
  id->minx	= minx;
  id->maxy	= 1.0;
  id->miny	= 0.0;
  id->random    = NULL;
  id->xvalues   = NULL;
  id->datasqr   = NULL;
  id->error     = NULL;
  id->nbin      = NULL;
  for(i=0;i<nx;i++) id->data[i] = 0;
  if (HMFirst==NULL)
    HMFirst = id;
  else
    HMLast->next = (struct hist *) id;
  id->next = NULL;
  HMLast = id;

  return id;
}

HIST *HMBook2	(const char *title, 
		 const char *labelx, const char *labely, const char *labelz,
                 const char *unitx, const char *unity, const char *unitz,
		 int nx, FLOAT minx, FLOAT maxx,
		 int ny, FLOAT miny, FLOAT maxy)
{ int i;
  HIST *id;

  id = HMcalloc(1, sizeof(HIST));
  id->data = HMcalloc(nx * ny, sizeof(FLOAT));

  if (!id || !id->data) 
  { puts("Error in HMBook2 not enough memory. ");
    exit(-1);
  } 

  id->divide=NULL;
  id->scale=1;
  id->nx	= nx;
  id->ny	= ny;
  id->entries	= 0;
  id->title	= title;
  id->name 	= title;
  id->labelx	= labelx;
  id->labely	= labely;
  id->labelz	= labelz;
  id->unitx	= unitx;
  id->unity	= unity;
  id->unitz	= unitz;
  id->maxx	= maxx;
  id->minx	= minx;
  id->maxy	= maxy;
  id->miny	= miny;
  id->random    = NULL;
  id->xvalues   = NULL;
  id->datasqr   = NULL;
  id->error     = NULL;
  id->nbin      = NULL;
  for(i=0;i<nx*ny;i++) id->data[i] = 0;
  if (HMFirst==NULL)
    HMFirst = id;
  else
    HMLast->next = (struct hist *) id;
  id->next = NULL;
  HMLast = id;
 return id;
}

HIST *HMCounter(char *title, char * unit)
{ return HMBook1(title,"","",unit,"",1, -1.0, 1.0);
}

void HMCount(HIST *id, FLOAT weight)
{ HMFill(id, 0.0, 0.0, weight);
}

void HMFill(HIST *id, FLOAT x, FLOAT y, FLOAT weight)
{	int ix,iy,bin;
	if (!id) return;
	if (x < id->minx || x >=  id->maxx) return; 
	if (id->ny && (y < id->miny || y>=id->maxy)) return; 
	ix = (x - id->minx) / (id->maxx-id->minx) * (FLOAT) id->nx;
	iy = (y - id->miny) / (id->maxy-id->miny) * (FLOAT) id->ny;
	if (           ix<0 || ix>=id->nx)  return;
	if (id->ny && (iy<0 || iy>=id->ny)) return;
	bin = (!id->ny ? ix : id->ny * ix + iy);
#ifdef HMBOOK_CHECKINCREMENT
	FLOAT oldbin=id->data[bin];
	id->data[bin] += weight;
	if ((id->data[bin]-oldbin)<weight*HMBOOK_CHECKINCREMENT_FACTOR)
	  fprintf(stderr,"Warning: \"%s\"-Histogram bin changed by less than %f percent of weight, %e instead of %e!\n",id->title,HMBOOK_CHECKINCREMENT_FACTOR*100,(id->data[bin]-oldbin),weight);
#else
	id->data[bin] += weight;
#endif	   

	if (id->datasqr) id->datasqr[bin] += weight*weight;
	if (id->nbin)    id->nbin[bin]++;
	id->entries++;
}

FLOAT HMExtract1(HIST *id, FLOAT x)
{ int ix;
  if (!id) return 0;
  ix = (x - id->minx) / (id->maxx-id->minx) * (FLOAT) id->nx;
  if (           ix<0 || ix>=id->nx)  return 0;
  return id->data[ix];
}

FLOAT HMExtractError(HIST *id, FLOAT x)
{ int ix;
  if (!id) return 0;
  ix = (x - id->minx) / (id->maxx-id->minx) * (FLOAT) id->nx;
  if (           ix<0 || ix>=id->nx)  return 0;
  return id->error[ix];
}

FLOAT HMInterpolate(HIST *id, FLOAT x)
{ int ix;
  double dx;
  if (!id) return 0;
  dx = (id->maxx - id->minx) / id->nx;
  //  ix = (x+dx/2 - id->minx) / dx;
  ix = (x-dx/2 - id->minx) / dx;
  if (ix >= id->nx-1) return id->data[id->nx-1];
  if (ix < 0)         return id->data[0];
  return id->data[ix] + 
    (id->data[ix+1] - id->data[ix])/dx * (x-id->minx-ix*dx-dx/2);
}

FLOAT HMInterpolate2D(HIST *id, FLOAT X, FLOAT Y)
{
  int ix, iy;
  double dx, dy, x=X, y=Y,x1,x2,y1,y2;
  if (!id) return 0;
  dx = (id->maxx - id->minx) / id->nx;
  dy = (id->maxy - id->miny) / id->ny;
  if (x < id->minx+dx/2) x = id->minx+dx/2;
  if (x > id->maxx-dx/2) x = id->maxx-dx/2;
  if (y < id->miny+dy/2) y = id->miny+dy/2;
  if (y > id->maxy-dy/2) y = id->maxy-dy/2;
  ix = (x - dx/2 - id->minx) / dx;
  iy = (y - dy/2 - id->miny) / dy;
  
  x1 = id->minx + (ix + 0.5) * dx ;
  x2 = id->minx + (ix + 0.5) * dx + dx;
  y1 = id->miny + (iy + 0.5) * dy;
  y2 = id->miny + (iy + 0.5) * dy + dy;

  return (bn(ix,iy) * (x2 - x) * (y2 - y)    +
	  bn(ix+1, iy) * (x - x1) * (y2 - y) +
	  bn(ix, iy+1) * (x2 - x) * (y - y1) +
	  bn(ix+1, iy+1) * (x - x1) * (y - y1))/(x2-x1)/(y2-y1);
}

FLOAT HMInterpolateError(HIST *id, FLOAT x)
{ int ix;
  double dx;
  if (!id) return 0;
  dx = (id->maxx - id->minx) / id->nx;
  ix = (x-dx/2 - id->minx) / dx;
  if (ix >= id->nx-1) return id->error[id->nx-1];
  if (ix < 0)         return id->error[0];
  return id->error[ix] + 
    (id->error[ix+1] - id->error[ix])/dx * (x-id->minx-ix*dx-dx/2);
}


FLOAT HMExtract2(HIST *id, FLOAT x, FLOAT y)
{	int ix,iy;
	if (!id) return 0;
	ix = (x - id->minx) / (id->maxx-id->minx) * (FLOAT) id->nx;
	iy = (y - id->miny) / (id->maxy-id->miny) * (FLOAT) id->ny;
	if (           ix<0 || ix>=id->nx)  return 0;
	if (id->ny && (iy<0 || iy>=id->ny)) return 0;
	if (!id->ny)
		return id->data[ix];
	else 
		return id->data[id->ny * ix + iy];
}

void HMFillFunction1(HIST *id, FLOAT (*f)(FLOAT x))
{ FLOAT  dx, x;

  if (id->ny != 0) 
    { puts("HMFillFunction1 is only for 1-dim histograms!");return;}
  dx = (id->maxx - id->minx) / id->nx;
  for(x = id->minx +dx/2.0 ; x < id->maxx; x += dx)
    HMFill(id, x, 0.0, f(x));
}

void HMFillFunction2(HIST *id, FLOAT (*f)(FLOAT x, FLOAT y))
{ FLOAT  dx, dy, x, y;

  if (id->ny == 0) 
    { puts("HMFillFunction2 is only for 2-dim histograms!");return;}
  dx = (id->maxx - id->minx) / id->nx;
  dy = (id->maxy - id->miny) / id->ny;
  for(x = id->minx +dx/2.0 ; x < id->maxx; x += dx)
    for(y = id->miny +dy/2.0 ; y < id->maxy; y += dy)
      HMFill(id, x, y, f(x,y));
}
 
void HMDelete(HIST *id) 
{ int bins = (id->ny ? id->nx*id->ny: id->nx);
  if (HMFirst==id) 
    HMFirst=(HIST *)id->next;
  else 
  { HIST *t = HMFirst;
    while (t) 
    { if (t->next==id) t->next = id->next; 
      t = (HIST *)t->next; 
    }
  } 

                    free(id->data);    HMMemory -= bins * sizeof(FLOAT);
  if (id->xvalues) {free(id->xvalues); HMMemory -= bins * sizeof(FLOAT);} 
  if (id->datasqr) {free(id->datasqr); HMMemory -= bins * sizeof(FLOAT);} 
  if (id->error)   {free(id->error);   HMMemory -= bins * sizeof(FLOAT);} 
  if (id->nbin)    {free(id->nbin);    HMMemory -= bins * sizeof(FLOAT);} 
  if (id->random)  {free(id->random);  HMMemory -= bins * sizeof(FLOAT);} //bss
  HMMemory -= strlen(id->title)+1;  free((void *)id->title);
  HMMemory -= strlen(id->labelx)+1; free((void *)id->labelx);
  HMMemory -= strlen(id->labely)+1; free((void *)id->labely);
  HMMemory -= strlen(id->labelz)+1; free((void *)id->labelz);
  HMMemory -= strlen(id->unitx)+1;  free((void *)id->unitx);
  HMMemory -= strlen(id->unity)+1;  free((void *)id->unity);
  HMMemory -= strlen(id->unitz)+1;  free((void *)id->unitz);
  HMMemory -= strlen(id->name)+1;   free((void *)id->name);
  free(id);
  HMMemory -= sizeof(HIST);
}

#define scalebin(id,i) id->scale/\
           (id->divide ? (id->divide->data[i]?id->divide->data[i]:1e39)\
		       : 1.0)

void HMHistogram(HIST *id)
{
  FLOAT *x, *y;
  int i,reb;
  if (id->ny) puts("Dieser Aufruf ist nur fuer 1-Dim Histogramme!");
  if (rebin<1 || rebin >= (id->nx) ) reb = 1; else reb=rebin;
  
  if (HMminimumx==NOMINIMUM) HMMinimumX(id->minx);
  if (HMmaximumx==NOMINIMUM) HMMaximumX(id->maxx);

  x = calloc(id->nx/reb*2+4, sizeof(FLOAT));
  y = calloc(id->nx/reb*2+4, sizeof(FLOAT));

  if (!x) { fprintf(stderr,"Not enough memory x\n"); exit(-1); }
  if (!y) { fprintf(stderr,"Not enough memory y\n"); exit(-1); }

  for(i=0;i<id->nx+1; i+=reb)
    x[i*2/reb] = x[i*2/reb+1] = id->minx+(id->maxx-id->minx)/id->nx*i;

  for(i=0;(i<id->nx/reb*2+4);i++) y[i] = 0.0;
  for(i=0;i<id->nx;i++)
  { y[(i/reb)*2 + 1]+=id->data[i] *scalebin(id,i);
    y[(i/reb)*2 + 2]+=id->data[i] *scalebin(id,i);
  }

  HMPlotFunction(id->nx/reb*2 + 2, x, y, id->title,
		 id->labelx, id->labely, id->unitx, id->unity);
  free(x);
  free(y);
}

void HMFunction(HIST *id)
{ FLOAT *x, *y;
  int i, j, reb;
  if (id->ny) puts("Dieser Aufruf ist nur fuer 1-Dim Histogramme!");

  if (!id->xvalues)
  { if (HMminimumx==NOMINIMUM) HMMinimumX(id->minx);
    if (HMmaximumx==NOMINIMUM) HMMaximumX(id->maxx);
  }
  else
  { id->minx = id->xvalues[0];
    id->maxx = id->xvalues[id->nx-1];
  }
  if (rebin<1 || rebin >= (id->nx) ) reb = 1; else reb=rebin;

  y = (FLOAT *) calloc(id->nx/reb + reb, sizeof(FLOAT));
  x = (FLOAT *) calloc(id->nx/reb + reb, sizeof(FLOAT));
 
  if (id->xvalues)
  { 
     for(i=0; i<id->nx/reb; i++) 
     { x[i] = 0; 
       for(j=0;j<reb;j++) x[i] += id->xvalues[i*reb+j]; 
     } 
     x[i] /= reb; 
  } 
  else
  { for(i=0;i<id->nx/reb;i++)
      x[i] = id->minx+(id->maxx-id->minx)/id->nx * (i*reb+0.5) ;
  }
  for(i=0;i<id->nx/reb;i++) 
    y[i] = 0;
  for(i=0;i<id->nx;i++)
    y[i/reb] += id->data[i] *scalebin(id,i);

  HMPlotFunction(id->nx/reb, x, y, id->title,
		 id->labelx, id->labely, id->unitx, id->unity);
  free(x);
  free(y);
}

void HMDataError(HIST *id)
{ FLOAT *x, *y, *err;
  int i, reb;
  int iserror = (id->error != NULL);
  if (id->ny) puts("Dieser Aufruf ist nur fuer 1-Dim Histogramme!");
  if (rebin<1 || rebin >= (id->nx) ) reb = 1; else reb=rebin;

  if (HMminimumx==NOMINIMUM) HMMinimumX(id->minx);
  if (HMmaximumx==NOMINIMUM) HMMaximumX(id->maxx);

  if (!iserror) HMFillError(id);
  x   = (FLOAT *) calloc(id->nx, sizeof(FLOAT));
  y   = (FLOAT *) calloc(id->nx, sizeof(FLOAT));
  err = (FLOAT *) calloc(id->nx, sizeof(FLOAT));

  for(i=0;i < id->nx/reb; i++)
    x[i] = id->minx+(id->maxx-id->minx)/id->nx * (i+0.5) * reb;

  for(i=0; i<id->nx/reb; i++)  y[i] = err[i] = 0;
  for(i=0; i<id->nx; i++) {
    y[i/reb]     += id->data[i] * scalebin(id,i);
    err[i/reb]   += pow(id->error[i] * scalebin(id, i), 2);
  }

  for(i=0; i<id->nx/reb;i++) err[i] = err[i]>0 ? sqrt(err[i]) : 0;

  HMPlotError(id->nx/reb, x, y, NULL, err, id->title,
	      id->labelx, id->labely, id->unitx, id->unity);
  free(x);
  free(y);
  free(err);
  if (!iserror) { free(id->error); id->error=NULL;}
}

void HMDataAndError(HIST *id, HIST *id2)
{ FLOAT *x, *y, *err;
  int i;
  if (id->ny) puts("Dieser Aufruf ist nur fuer 1-Dim Histogramme!");

  if (HMminimumx==NOMINIMUM) HMMinimumX(id->minx);
  if (HMmaximumx==NOMINIMUM) HMMaximumX(id->maxx);

  x   = calloc((id->nx), sizeof(FLOAT));
  y   = calloc((id->nx), sizeof(FLOAT));
  err = calloc((id->nx), sizeof(FLOAT));

  for(i=0;i<id->nx;i++)
        x[i] = id->minx+(id->maxx-id->minx)/id->nx*(i+0.5);

  for(i=0;i<id->nx;i++)
        y[i] = id->data[i]*scalebin(id,i);

  for(i=0;i<id->nx;i++) 
       err[i] = id2->data[i]*scalebin(id,i);

  HMPlotError(id->nx, x, y, NULL, err, id->title,
              id->labelx, id->labely, id->unitx, id->unity);

  free(x);
  free(y);
  free(err);
}

void HMMarkHist(HIST *id, int where, char *txt)
{ FLOAT *x, *y;
  int i;

  x = calloc(id->nx, sizeof(FLOAT));
  y = calloc(id->nx, sizeof(FLOAT));

  for(i=0;i<id->nx;i++) x[i] = id->minx+(id->maxx-id->minx)/id->nx*(i+0.5);
  for(i=0;i<id->nx;i++) y[i] = id->data[i]*scalebin(id,i);

  HMMarkFunction(where, x, y, txt);
  free(x);  free(y);
}

void HMPlotMeanX(HIST *id)
{if (id->ny == 0) 
    { puts("HMPlotMeanX is only for 2-dim histograms!");return;}
  if (HMminimumx==NOMINIMUM) HMMinimumX(id->minx);
  if (HMmaximumx==NOMINIMUM) HMMaximumX(id->maxx);
  if (HMminimumy==NOMINIMUM) HMMinimumY(id->miny);
  if (HMmaximumy==NOMINIMUM) HMMaximumY(id->maxy);
  HMPlotBox(	id->nx,id->ny,
	        id->minx,id->miny, 
                (id->maxx - id->minx)/(FLOAT)id->nx,
                (id->maxy - id->miny)/(FLOAT)id->ny,
                id->data, id->title,
                id->labelx, id->labely, id->unitx, id->unity, 'x');
}

void HMPlotMeanY(HIST *id)
{if (id->ny == 0) 
    { puts("HMPlotMeanY is only for 2-dim histograms!");return;}
  if (HMminimumx==NOMINIMUM) HMMinimumX(id->minx);
  if (HMmaximumx==NOMINIMUM) HMMaximumX(id->maxx);
  if (HMminimumy==NOMINIMUM) HMMinimumY(id->miny);
  if (HMmaximumy==NOMINIMUM) HMMaximumY(id->maxy);
  HMPlotBox(	id->nx,id->ny,
	        id->minx,id->miny, 
                (id->maxx - id->minx)/(FLOAT)id->nx,
                (id->maxy - id->miny)/(FLOAT)id->ny,
                id->data, id->title,
                id->labelx, id->labely, id->unitx, id->unity, 'y');
}

void HMGray(HIST *id)
{if (id->ny == 0) 
    { puts("HMGray is only for 2-dim histograms!");return;}
  if (HMminimumx==NOMINIMUM) HMMinimumX(id->minx);
  if (HMmaximumx==NOMINIMUM) HMMaximumX(id->maxx);
  if (HMminimumy==NOMINIMUM) HMMinimumY(id->miny);
  if (HMmaximumy==NOMINIMUM) HMMaximumY(id->maxy);
  HMPlotBox(	id->nx,id->ny,
	        id->minx,id->miny, 
                (id->maxx - id->minx)/(FLOAT)id->nx,
                (id->maxy - id->miny)/(FLOAT)id->ny,
                id->data, id->title,
                id->labelx, id->labely, id->unitx, id->unity, 'g');
}

void HMColor(HIST *id)
{if (id->ny == 0) 
    { puts("HMGray is only for 2-dim histograms!");return;}
  if (HMminimumx==NOMINIMUM) HMMinimumX(id->minx);
  if (HMmaximumx==NOMINIMUM) HMMaximumX(id->maxx);
  if (HMminimumy==NOMINIMUM) HMMinimumY(id->miny);
  if (HMmaximumy==NOMINIMUM) HMMaximumY(id->maxy);
  HMPlotBox(	id->nx,id->ny,
	        id->minx,id->miny, 
                (id->maxx - id->minx)/(FLOAT)id->nx,
                (id->maxy - id->miny)/(FLOAT)id->ny,
                id->data, id->title,
                id->labelx, id->labely, id->unitx, id->unity, 'c');
}

void HMBox(HIST *id)
{if (id->ny == 0) 
    { puts("HMGray is only for 2-dim histograms!");return;}
  if (HMminimumx==NOMINIMUM) HMMinimumX(id->minx);
  if (HMmaximumx==NOMINIMUM) HMMaximumX(id->maxx);
  if (HMminimumy==NOMINIMUM) HMMinimumY(id->miny);
  if (HMmaximumy==NOMINIMUM) HMMaximumY(id->maxy);
  HMPlotBox(	id->nx,id->ny,
	        id->minx,id->miny, 
                (id->maxx - id->minx)/(FLOAT)id->nx,
                (id->maxy - id->miny)/(FLOAT)id->ny,
                id->data, id->title,
                id->labelx, id->labely, id->unitx, id->unity, 'b');
}

void HMScatter(HIST *id)
{if (id->ny == 0) 
    { puts("HMGray is only for 2-dim histograms!");return;}
  if (HMminimumx==NOMINIMUM) HMMinimumX(id->minx);
  if (HMmaximumx==NOMINIMUM) HMMaximumX(id->maxx);
  if (HMminimumy==NOMINIMUM) HMMinimumY(id->miny);
  if (HMmaximumy==NOMINIMUM) HMMaximumY(id->maxy);
  HMPlotBox(	id->nx,id->ny,
	        id->minx,id->miny, 
                (id->maxx - id->minx)/(FLOAT)id->nx,
                (id->maxy - id->miny)/(FLOAT)id->ny,
                id->data, id->title,
                id->labelx, id->labely, id->unitx, id->unity, 's');

/*  HMPlotScatter(id->entries,
		id->nx,id->ny,id->data, id->title,
                id->labelx, id->labely, id->unitx, id->unity);
*/
}

void HMSurface(HIST *id)
{ if (id->ny == 0) 
    { puts("HMSurface is only for 2-dim histograms!");return;}
  if (HMminimumx==NOMINIMUM) HMMinimumX(id->minx);
  if (HMmaximumx==NOMINIMUM) HMMaximumX(id->maxx);
  if (HMminimumy==NOMINIMUM) HMMinimumY(id->miny);
  if (HMmaximumy==NOMINIMUM) HMMaximumY(id->maxy);
  HMPlot3D(id->nx,id->ny,id->data, id->title,
                id->labelx, id->labely, id->labelz,
	        id->unitx, id->unity, id->unitz,-1);
}

void HMLego(HIST *id)
{ if (id->ny == 0) 
    { puts("HMLego is only for 2-dim histograms!");return;}
  if (HMminimumx==NOMINIMUM) HMMinimumX(id->minx);
  if (HMmaximumx==NOMINIMUM) HMMaximumX(id->maxx);
  if (HMminimumy==NOMINIMUM) HMMinimumY(id->miny);
  if (HMmaximumy==NOMINIMUM) HMMaximumY(id->maxy);
  HMPlot3D(	id->nx,id->ny,id->data, id->title,
                id->labelx, id->labely, id->labelz,
                id->unitx, id->unity, id->unitz, 0);
}

HIST *HMOperation(HIST *id1, char oper, HIST *id2, 
		const char *title, const char *labz, const char *unitz)
{ 
  HIST *id;
  int i;
  
  if (id1->nx != id2->nx || id1->ny != id2->ny) { 
    puts ("HMOperation: Operation with different kinds of histograms!");
    exit(0);
  }
  
  if (id1->ny == 0) {
    id =  HMBook1(title, id1->labelx, labz,
		  id1->unitx, unitz, id1->nx, id1->minx, id1->maxx);
    
  } else {
    id =  HMBook2(title, id1->labelx, id1->labely, labz,
		  id1->unitx, id1->unity,  unitz,
		  id1->nx, id1->minx, id1->maxx, id1->ny, id1->miny,id1->maxy);
  }
  for(i=0; i < (id->ny ? id->nx * id->ny : id->nx); i++) {
    if (oper=='/') id->data[i] = id2->data[i]==0?0:id1->data[i]/id2->data[i];
    if (oper=='*') id->data[i] = id1->data[i]*id2->data[i]; 
    if (oper=='+') id->data[i] = id1->data[i]+id2->data[i];
    if (oper=='-') id->data[i] = id1->data[i]-id2->data[i];
  }

  HMFillError(id);
  HMFillError(id1);
  HMFillError(id2);
  for(i=0; i < (id->ny ? id->nx * id->ny : id->nx); i++) {
    if (oper=='/') id->error[i] = id2->data[i]==0 ? 0 : 
      pow(id1->error[i]/id2->data[i], 2) +
		     pow(id2->error[i]*id1->data[i]/pow(id2->data[i],2), 2);
    if (oper=='*') id->error[i] = pow(id1->error[i]*id2->data[i], 2) +
		                  pow(id2->error[i]*id1->data[i], 2);
    if (oper=='+') id->error[i] = pow(id1->error[i],2)+pow(id2->error[i],2);
    if (oper=='-') id->error[i] = pow(id1->error[i],2)+pow(id2->error[i],2);
    id->error[i] = sqrt(id->error[i]);
  }
  return id;

}

HIST  *HMSmooth (HIST *id, int bins)
{ int i;
  HIST *result = HMScalar(id,'*',0);
  printf("Smoothing %s, %d bins\n",id->title,bins);
  for (i=0;i<id->nx;i++)
  { int j,k=0;
    for(j=i-bins;j<=i+bins;j++)
      if (j>=0 && j<id->nx) 
      { result->data[i] += id->data[j];
	k++;
      }
    if (k) result->data[i]/=k;
  }
  return result;
}

void HMSetScale(HIST *id, FLOAT scale)
{ if (scale!=0.0) id->scale=scale;
}

void HMSetDivide(HIST*id, HIST *divide)
{ id->divide = (struct hist *) divide; 
}

HIST *HMScalar(HIST *id1, char oper, FLOAT scalar)
{ 
  HIST *id;
  int i, nrdata;

  if (id1->ny == 0) {
    id =  HMBook1(id1->title,id1->labelx, id1->labely,
		  id1->unitx, id1->unity, id1->nx, id1->minx, id1->maxx);
    nrdata = id1->nx;
  } else {
    id =  HMBook2(id1->title, id1->labelx, id1->labely, id1->labelz,
		  id1->unitx, id1->unity, id1->unitz,
		  id1->nx, id1->minx,id1->maxx, id1->ny, id1->miny, id1->maxy);
    nrdata = id1->nx * id1->ny;
  }
  for(i=0;i<nrdata;i++) { 
    if (oper =='/')  id->data[i] = scalar == 0 ? 0: id1->data[i]/scalar; 
    if (oper == '*') id->data[i] = id1->data[i]*scalar;
    if (oper == '+') id->data[i] = id1->data[i]+scalar;
    if (oper == '-') id->data[i] = id1->data[i]-scalar; ;
    if (oper == '^') id->data[i] = id->data[i]<0 ?0: pow(id1->data[i],scalar); 
  }
  if (id1->error) {
    HMFillError(id);
    for(i=0;i<nrdata;i++) { 
      id->error[i] = id1->error[i];
      if (oper =='/')  id->error[i] *= scalar == 0 ? 0 : fabs(1.0/scalar); 
      if (oper == '*') id->error[i] *= fabs(scalar);
      if (oper == '^') id->error[i] *= fabs(scalar*id->data[i]/id1->data[i]);
    }
  }

  return id;
}
/*---------------------------------------------------------------------------*/
void HMCSource(HIST *id, FILE *fp)
{ int i;
  fprintf(fp,"HIST *id = {%d,%d,%d,\n",id->nx, id->ny, id->entries);
  fprintf(fp,"            %f,%f,\n",id->minx, id->maxx);
  fprintf(fp,"            %f,%f,\n",id->miny, id->maxy);
  fprintf(fp,"            ""%s"",\n",id->title);
  fprintf(fp,"            ""%s"",""%s"",""%s"",\n",
	  id->labelx, id->labely, id->labelz);
  fprintf(fp,"            (HIST *) 0,\n");
  //  if (id->ny == 0) totn = id->nx;
  //else totn = id->nx * id->ny;
  fprintf(fp,"            {");
  for (i=0;i<id->ny;i++) 
    { fprintf(fp,"%g",id->data[i]);
      if (i!=id->ny-1) fprintf(fp,",");
    }
  fprintf(fp,"            }\n");
}

static FILE *fp_update = 0;

FILE *HMHistogramFile(char *filename)
{ fp_update = fopen(filename,"w");
  if (!fp_update) {perror("Can't open Histogram File"); exit(0); }
  fprintf(fp_update,"HMBook %f\n0\n",VERSION);
  return fp_update;
}

/* Change these routines if you want to swap bytes... */

#define charWrite(fl)      { myfwrite(&fl,sizeof(char),  1, fp); }
#define charRead(fl)       { myfread(&fl, sizeof(char),  1, fp); }
#define intWrite(fl,n)     { myfwrite(&fl,sizeof(int),   n, fp); }
#define intRead(fl,n)      { myfread(&fl, sizeof(int),   n, fp); }
#define floatWrite(fl,n)   { myfwrite(&fl,sizeof(FLOAT), n, fp); }

void floatRead(FLOAT *fl, int n, FILE *fp)    {
  if (HMVersion <1.5) {
    float *b = calloc(n, sizeof(float));
    int i;
    myfread(b, sizeof(float), n, fp); 
    for (i=0; i<n; i++) fl[i] = b[i];
  }
  else
    myfread(fl, sizeof(FLOAT), n, fp); 
}

#define stringWrite(strng) { l = strlen(strng)+1;\
			     myfwrite(&l, sizeof(int), 1, fp);\
			     myfwrite(strng, 1, l, fp);}
#define stringRead(strng)  { myfread(&l,sizeof(int),1,fp);\
			     strng = (const char *) HMmalloc(l);\
			     myfread((char *) strng, 1, l, fp);}
 
void HMWrite(HIST *id, FILE *fp, char *name)
{
  int l;
  int bins = (id->ny ? id->nx*id->ny: id->nx);
  char yes = 1, no = 0;
  
  stringWrite(name);
  intWrite(id->nx,      1);
  intWrite(id->ny,      1);
  intWrite(id->entries, 1);
  floatWrite(id->minx,  1);
  floatWrite(id->maxx,  1);
  floatWrite(id->miny,  1);
  floatWrite(id->maxy,  1);
  floatWrite(id->scale, 1);
  if (id->xvalues) charWrite(yes) else charWrite(no);
  if (id->datasqr) charWrite(yes) else charWrite(no);
  if (id->nbin)    charWrite(yes) else charWrite(no);
  if (id->error)   charWrite(yes) else charWrite(no);
  
                   floatWrite((id->data[0]),    bins);
  if (id->xvalues) floatWrite((id->xvalues[0]), bins);
  if (id->datasqr) floatWrite((id->datasqr[0]), bins);
  if (id->nbin)    floatWrite((id->nbin[0]),    bins);
  if (id->error)   floatWrite((id->error[0]),   bins);
  stringWrite(id->title);
  stringWrite(id->labelx);  
  stringWrite(id->labely);  
  stringWrite(id->labelz);
  stringWrite(id->unitx);   
  stringWrite(id->unity);   
  stringWrite(id->unitz);
}

void HMBookVariance(HIST *id)
{
  int i, bins = (id->ny ? id->nx*id->ny: id->nx);
  id->datasqr = (FLOAT *) HMcalloc(bins, sizeof(FLOAT)); 
  id->nbin    = (FLOAT *) HMcalloc(bins, sizeof(FLOAT)); 

  for(i=0;i<bins;i++) id->datasqr[i] = id->nbin[i] = 0;
}

void HMBookWeightError(HIST *id)
{
  int i, bins = (id->ny ? id->nx*id->ny: id->nx);
  if (!id) {fprintf(stderr, "Histogram does not exist\n");return;}
  if (id->datasqr) { 
    fprintf(stderr,"Histogram %s has allready errors\n",id->title); 
    return;
  }
  id->datasqr = (FLOAT *) HMcalloc(bins, sizeof(FLOAT)); 

  for(i=0;i<bins;i++) id->datasqr[i] = 0;
}

void HMFillError(HIST *id)
{
  int i, bins = (id->ny ? id->nx*id->ny: id->nx);
  if (id->error) return;
  if (!id->error) id->error = (FLOAT *) HMcalloc(bins, sizeof(FLOAT)); 

  for(i=0; i<bins; i++) {
    if (id->datasqr && id->nbin) {
      id->error[i] = ! id->nbin[i] > 0 ? 0 :
	sqrt((id->datasqr[i]/id->nbin[i] 
	      - pow(id->data[i]/id->nbin[i],2))/id->nbin[i]);
    } else if (id->datasqr) {
      id->error[i] = id->datasqr[i]>0 ? sqrt(id->datasqr[i]) : 0;
    } else 
      id->error[i] = id->data[i]   >0 ? sqrt(id->data[i]+2) : 0;
  }
}

void HMClearError(HIST *id)
{
  int i, bins = (id->ny ? id->nx*id->ny: id->nx);
  if (!id->error) return;
  for(i=0; i<bins; i++) id->error[i]=0; 
}

HIST *HMReadIsolated(FILE *fp)
{
  int l,bins;
  char havexvalues = 0;
  char havedatasqr = 0;
  char havenbin    = 0;
  char haveerror   = 0;
  HIST *id;
  const char *name;
  if(feof(fp)) return NULL;
  myfread(&l,sizeof(int),1,fp);         if(feof(fp)) return NULL;
  name = HMmalloc(l); 
  myfread((char *) name,l,1,fp);        if(feof(fp)) return NULL;

  id = HMcalloc(1, sizeof(HIST));  

  id->datasqr = id->nbin = id->error = id->xvalues = NULL;
  intRead(id->nx,      1); if(feof(fp)) return NULL;
  intRead(id->ny,      1);
  intRead(id->entries, 1);
  floatRead(&id->minx,  1, fp);
  floatRead(&id->maxx,  1, fp);
  floatRead(&id->miny,  1, fp);
  floatRead(&id->maxy,  1, fp);
  if(HMVersion<1.35)  myfread(&id->random,  OLDPOINTER, 1, fp);
  if(HMVersion<1.35)  myfread(&id->name,    OLDPOINTER, 1, fp);
  if(HMVersion<1.35)  myfread(&id->title,   OLDPOINTER, 1, fp);
  if(HMVersion<1.35)  myfread(&id->labelx,  OLDPOINTER, 1, fp);
  if(HMVersion<1.35)  myfread(&id->labely,  OLDPOINTER, 1, fp);
  if(HMVersion<1.35)  myfread(&id->labelz,  OLDPOINTER, 1, fp);
  if(HMVersion<1.35)  myfread(&id->unitx,   OLDPOINTER, 1, fp);
  if(HMVersion<1.35)  myfread(&id->unity,   OLDPOINTER, 1, fp);
  if(HMVersion<1.35)  myfread(&id->unitz,   OLDPOINTER, 1, fp);
  if(HMVersion<1.35)  myfread(&id->next,    OLDPOINTER, 1, fp);
  if(HMVersion<1.35)  myfread(&id->divide,  OLDPOINTER, 1, fp);
  floatRead(&id->scale,  1, fp);
  if(HMVersion<1.35)  myfread(&id->xvalues, OLDPOINTER, 1, fp);
  if(HMVersion<1.35)  myfread(&id->data,    OLDPOINTER, 1, fp);
  if (HMVersion > 1.25 && HMVersion<1.35) {
    myfread(&id->datasqr, OLDPOINTER, 1, fp);
    myfread(&id->nbin,    OLDPOINTER, 1, fp);
    myfread(&id->error,   OLDPOINTER, 1, fp);
  }
  if (HMVersion>1.35) {
    charRead (havexvalues);
    charRead (havedatasqr);
    charRead (havenbin);
    charRead (haveerror);
  }

  bins = (id->ny ? id->ny * id->nx : id->nx ); 

  id->data = (FLOAT *) HMcalloc(bins, sizeof(FLOAT));
  floatRead(id->data, bins, fp);
  
  if (id->xvalues || havexvalues)  {
    id->xvalues = (FLOAT *) HMcalloc(bins, sizeof(FLOAT)); 
    floatRead(id->xvalues, bins, fp);
  }
  if (id->datasqr || havedatasqr) {
    id->datasqr = (FLOAT *) HMcalloc(bins, sizeof(FLOAT)); 
    floatRead(id->datasqr, bins, fp);
  }
  if (id->nbin    || havenbin)  {
    id->nbin = (FLOAT *) HMcalloc(bins, sizeof(FLOAT)); 
    floatRead(id->nbin, bins, fp);
  }
  if (id->error   || haveerror) {
    id->error = (FLOAT *) HMcalloc(bins, sizeof(FLOAT)); 
    floatRead(id->error, bins, fp);
  }

  stringRead(id->title);
  stringRead(id->labelx);
  stringRead(id->labely);
  stringRead(id->labelz);
  stringRead(id->unitx);
  stringRead(id->unity);
  stringRead(id->unitz);
  id->name = name;
  id->divide = NULL; 

  return id;
}

HIST *HMReadFilename(FILE *fp, char *filename) {
  HIST *id=HMRead(fp);
  if (id) {
      HMMemory -= strlen(id->name)+1;
      free((void *)id->name);
      id->name = HMmalloc(strlen(filename)+strlen(id->title)+2);
      strcpy((char *) id->name, (const char *)filename);
      *(char *)(id->name +strlen(filename))=':';
      strcpy((char *) (id->name+strlen(filename)+1), (const char *) id->title);
    }
  return id;
}

HIST *HMRead(FILE *fp)
{ HIST *id = HMReadIsolated(fp);
  if (!id) return NULL;
  if (!HMFirst)
    HMFirst = id;
  else
    HMLast->next = id;
  id->next = NULL;
  HMLast = id;
  
  return id;
}

void HMWriteAllPipe(char *command)
{
  FILE *fp = popen(command,"w");
  HIST *id = HMFirst;

  if (!fp) fprintf(stderr, "HMBook: can't open pipe \'%s\'", command);
  fprintf(fp,"HMBook %f\n0\n", VERSION);
  while (id) {
    HMWrite(id,fp,"");
    id = (HIST *) id->next;
  }
  pclose(fp);  
}

void HMWriteAll(char *filename)
{
  FILE *fp;
  HIST *id = HMFirst;
  char command[255];
  sprintf(command,GZIP " > %s 2>/dev/null",filename);
  fp = popen(command,"w");
  if (!fp) {
    fprintf(stderr, "can't open gzip pipe for %s",filename);
    exit(-1);
  }
  fprintf(fp,"HMBook %f\n0\n",VERSION);
  while (id) {
    HMWrite(id,fp,"");
    id = (HIST *) id->next;
  }
  pclose(fp);  
}

void HMWriteSingleHistogram(char *filename, HIST *id)
{
  FILE *fp;
  char command[255];
  sprintf(command,GZIP " > %s 2>/dev/null",filename);
  fp = popen(command,"w");
  if (!fp) {
    fprintf(stderr, "can't open gzip pipe for %s",filename);
    exit(-1);
  }
  fprintf(fp,"HMBook %f\n0\n",VERSION);
  HMWrite(id,fp,"");
  pclose(fp);  
}

/* Close his file without gzip warning */
void HMpclose(FILE *fp)
{
  char *buf[1024];
  while (0 < fread(buf,1,1024,fp));
  pclose(fp);
}

FILE *HMOpenHisFile(const char *filename)
{
  char sc;
  char command[255];
  FILE *fp;
  sprintf(command, GZIP " -c -d %s", filename);
  fp = popen(command,"r");
  if (!fp) {
    fprintf(stderr, "Can't open gzip pipe for file '%s'",filename);
    exit(-1);
  }

  if (fscanf(fp,"HMBook %lf\n%c\n", &HMVersion, &sc)!=2) {
    perror("fscanf failed");
  };
  return sc==1 ? NULL : fp;
}

int HMReadAll(const char *filename)
{
  FILE *fp = HMOpenHisFile(filename);
  HIST *id;
  //     fprintf(stderr,"Reading %s\n",filename);
  if (!fp) return 0;
  do { id = HMRead(fp);  } while (id);
  HMpclose(fp); 
  return 1;
}

HIST *HMScanFile(const char *filename, char *histogram)
{ 
  FILE *fp = HMOpenHisFile(filename);
  HIST *id;
  do {
    id = HMReadIsolated(fp);
    if (!id) break; 
    if (!strcmp(id->title,histogram)) { HMpclose(fp); return id; }
    if (id->data)    free(id->data);
    if (id->datasqr) free(id->datasqr);
    if (id->nbin)    free(id->nbin);
    if (id->error)   free(id->error);
    if (id->xvalues) free(id->xvalues);

    if (id->random) free(id->random);        //bss 2015-03-19
    if (id->name)   free((void*)id->name);   //bss 2015-03-19
    if (id->title)  free((void*)id->title);  //bss 2015-03-19
    if (id->labelx) free((void*)id->labelx); //bss 2015-03-19
    if (id->labely) free((void*)id->labely); //bss 2015-03-19
    if (id->labelz) free((void*)id->labelz); //bss 2015-03-19
    if (id->unitx)  free((void*)id->unitx);  //bss 2015-03-19
    if (id->unity)  free((void*)id->unity);  //bss 2015-03-19
    if (id->unitz)  free((void*)id->unitz);  //bss 2015-03-19

    free(id);
  } while (-1);
  HMpclose(fp); 
  return NULL;
}

int HMUpdate(char *filename, int seconds, FLOAT changerate)
{
  int pid = fork();
  if (pid>0) return pid;
  if (pid<0) {perror("Can's spawn subprocess for histogram update"); exit(0);}
  for(;;) {
    HIST *id = HMFirst;
    fp_update = fopen(filename,"a+");
    rewind(fp_update);
    fprintf(fp_update,"HMBook %f\n1\n",VERSION);
    while (id) {
      HMWrite(id,fp_update,"");
      id = id->next;
    }
    
    rewind(fp_update);
    fprintf(fp_update,"HMBook %f\n0\n",VERSION);
    fclose(fp_update);  
    
    sleep(seconds);
  }
  return -1; /* should never return this way */
}

HIST *HMFindTitle(char *name)
{ 
  HIST *id = HMFirst;
  while (id){
    if (!strcmp(name, id->title)) return id;
    id = (HIST *) id->next;
  }
  return NULL;
}

HIST *HMFind(char *name)
{ HIST *id = HMFirst;
  while (id)
    { if (!strcmp(name, id->name)) return id;
      id = (HIST *) id->next;
    }
  return NULL;
}

FLOAT HMBinX(HIST *id, int i)
{ return id->minx + (id->maxx - id->minx)/(FLOAT)id->nx *(i+0.5);}

FLOAT HMBinY(HIST *id, int i)
{ return id->miny + (id->maxy - id->miny)/(FLOAT)id->ny *(i+0.5);}

void HMInfo1D(HIST *id, FLOAT *Maximum, FLOAT *Maxposition,
	                FLOAT *Minimum, FLOAT *Minposition,
	                FLOAT *FWHM, FLOAT *FWHMposition,
	                FLOAT *RMS, FLOAT *Mean,
	                FLOAT *Integral)
{ int i;
  FLOAT  
    maxi = id->data[0],  mini=maxi, 
    maxipos = HMBinX(id,0), minipos=maxipos,
    inte=0,  sumx=0,  sumx2=0;
   
  for(i=0;i<id->nx;i++) 
  { FLOAT x = HMBinX(id,i);
    inte += id->data[i];
    if (maxi<id->data[i]) { maxi = id->data[i]; maxipos = x;}
    if (mini>id->data[i]) { mini = id->data[i]; minipos = x;}
    sumx  += x     * id->data[i];
    sumx2 += x * x * id->data[i];
  }

  if (Integral)    *Integral = inte;
  if (Maximum)     *Maximum=maxi;
  if (Maxposition) *Maxposition= maxipos;
  if (Minimum)     *Minimum=mini;
  if (Minposition) *Minposition= minipos;
  if (inte==0) inte=1.0;
  sumx/=inte;  sumx2/=inte;
  if (Mean)        *Mean = sumx;
  if (RMS && (sumx2 - sumx*sumx)>=0) *RMS = sqrt(sumx2 - sumx*sumx);

  if (FWHM) {
    if (mini==maxi) {
      *FWHM=0;
    } else {
      FLOAT left=0, right=0, mm = (maxi+mini)/2;
      for (i=0;i<id->nx-1;i++)
	if (id->data[i]<mm && id->data[i+1]>mm) {
	  left = (mm - id->data[i]) * 
	    ( id->maxx-id->minx)/id->nx/(id->data[i]-id->data[i+1]) 
	    + HMBinX(id,i);
	}
      for (i=id->nx-2;i>=0;i--)
	if (id->data[i]>mm && id->data[i+1]<mm) {
	  right = (mm - id->data[i]) * 
	    ( id->maxx-id->minx)/id->nx/(id->data[i]-id->data[i+1]) 
	    + HMBinX(id,i);
	}
      if (FWHMposition) *FWHMposition= (left+right)/2.0; 
      *FWHM = right-left;
    }
  }

}

FLOAT
HMIntegralRegion(HIST *id, FLOAT *RMS, FLOAT *Mean)
{
  int i;
  FLOAT  Integral=0;
  FLOAT  sumx=0;
  FLOAT  sumxx=0;

  for(i=0;i<id->nx;i++) {
    FLOAT x = HMBinX(id,i);
    if ((x>=HMminimumx || HMminimumx==NOMINIMUM) &&
	(x<=HMmaximumx || HMmaximumx==NOMINIMUM)) {
      Integral += id->data[i];
      sumx  += x   * id->data[i];
      sumxx += x*x * id->data[i];
    }
  }

  if (Integral) {
    sumx  /= Integral;
    sumxx /= Integral;
  }

  if (Mean) *Mean = sumx;
  if (RMS) {
    *RMS = 0;
    if ((sumxx - sumx*sumx) >= 0) {
      *RMS = sqrt(sumxx - sumx*sumx);
    }
  }

  return Integral;
}

int HMDimension(HIST *id) 
{ return (id->ny>0?2:1);}

FLOAT HMMin(HIST *id)
{
  int i, bins;
  FLOAT min = FLT_MAX;

  bins = (id->ny>0 ? id->nx*id->ny : id->nx);
  for (i=0; i<bins; i++)
    if (id->data[i] < min) min = id->data[i];

  return min;
}

FLOAT HMMax(HIST *id)
{
  int i, bins;
  FLOAT max = -FLT_MAX;

  bins = (id->ny>0 ? id->nx*id->ny : id->nx);
  for (i=0; i<bins; i++)
    if (id->data[i] > max) max = id->data[i];

  return max;
}

void HMBookParams(HIST *id, int *nx, int *ny, FLOAT *minx, FLOAT *maxx, 
		  FLOAT *miny, FLOAT *maxy)
{ *nx   = id->nx;
  *ny   = id->ny;
  *minx = id->minx;
  *maxx = id->maxx;
  *miny = id->miny;
  *maxy = id->maxy;
 
}
	
void HMWriteAscii(FILE *fp, HIST *id)
{
  int i;
  /*
  fprintf(fp, "Title: \"%s\" Name: \"%s\"\n", id->title, id->name);
  fprintf(fp, "X-Axis: [%8.4g, %8.4g] %5d Bins, Label: \"%s\" Unit: \"%s\"\n", 
  id->minx, id->maxx, id->nx, id->labelx, id->unitx);
  if(id->ny)
  { fprintf(fp, 
	    "Y-Axis: [%8.4g, %8.4g] %5d Bins, Label: \"%s\" Unit: \"%s\"\n", 
	    id->miny, id->maxy, id->ny, id->labely, id->unity);
    fprintf(fp, "Abscissa: Label: \"%s\" Unit: \"%s\"\n", 
	    id->labelz, id->unitz);
  }
  else
    fprintf(fp, "Abscissa: Label: \"%s\" Unit: \"%s\"\n", 
	    id->labely, id->unity);
  */
  for(i=0; i<(id->ny?id->nx*id->ny:id->nx); i++) 
    //    fprintf(fp, "%8.4g%c", id->data[i], ((i+1)%8?' ':'\n'));
    fprintf(fp, "%8.4g\n", id->data[i]);
  //  fprintf(fp, "\n---- End of Histogram ----\n");
  fflush(fp);
}
void HMWriteAsciiTuple(FILE *fp, HIST *id)
{
  fprintf(stderr,"asciiTuple");
  int i, j;
  if (id->ny==0) { //1d histogram
    for(i=0; i<id->nx; i++) 
      fprintf(fp, "%.8f\t%.8f\n", HMBinX(id, i),   id->data[i]);
  } else { //2d histogram
    for (i=0; i<id->nx; i++) {
      double posx = HMBinX(id, i); 
      for (j=0; j<id->ny; j++) {
	double posy=HMBinY(id, j);
	fprintf(fp, "%.8f\t%.8f\t%.8f\n", posx, posy,  HMExtract2(id, posx, posy));
      }
      fprintf(fp, "\n");
    }
  }
  fflush(fp);
}

void HMSetArray(HIST *id, int n, FLOAT *x, FLOAT *y)
{
  id->nx = n;
  id->ny = 0;
  id->xvalues = x;
  id->data    = y;
}

void HMServer(FILE *in, FILE *out, int (* work)(void *)) 
{
  int position;
  char line[255];
  
  if (fcntl(fileno(in), F_SETFL, O_NONBLOCK | O_NDELAY | O_SYNC))
    perror("Error HMBook server calling FCNTL: "); 
  
  position = 0;
  while (-1) {
    int r = read(fileno(in), &line[position], 255-position);
    work(NULL);
    if (r>0) {
      int i;
      position += r;
      for (i=position-1;i;i--) 
	if (line[i]=='\n') {
	  if (!strncmp(line, "exit", 4)) return;
	  if (!strncmp(line, "list", 4)) {
	    HIST *id = HMFirst;
	    while (id) {
	      fprintf(out, "%1dD \"%s\"\n", (id->ny?2:1), id->title);
	      id = (HIST *) id->next;
	    }
	    fprintf(out,"\"***END***\"");
	  }
	  if (!strncmp(line, "get ", 4)) {
	    HIST *id;
            line[i]=0;
            id = HMFindTitle(&line[4]);
	    if (id) HMWriteAscii(out, id);
	  }
	  
	  position = 0;
	}
    }
  }
}

#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

FLOAT *vector(long nl, long nh)
{
  FLOAT *v;
  v = (FLOAT *)malloc((size_t) ((nh-nl+2)*sizeof(FLOAT)));
  if (!v) fprintf(stderr,"vector: allocation failure");
  return v-nl+1;
}

void free_vector(FLOAT *v, long nl, long nh)
{
  free((char *) (v+nl-1));
}

void twofft(FLOAT data1[], FLOAT data2[], FLOAT fft1[], FLOAT fft2[],
            unsigned long n)
{
  void four1(FLOAT data[], unsigned long nn, int isign);
  unsigned long nn3, nn2, jj, j;
  FLOAT rep, rem, aip, aim;

  nn3 = 1 + (nn2=2+n+n);
  for (j = 1, jj = 2; j <= n; j++, jj += 2) {
    fft1[jj-1] = data1[j];
    fft1[jj] = data2[j];
  }
  four1(fft1, n, 1);
  fft2[1] = fft1[2];
  fft1[2] = fft2[2] = 0.0;
  for (j = 3; j <= n+1; j += 2) {
    rep = 0.5 * ( fft1[j] + fft1[nn2-j] );
    rem = 0.5 * ( fft1[j] - fft1[nn2-j] );
    aip = 0.5 * ( fft1[j+1] + fft1[nn3-j] );
    aim = 0.5 * ( fft1[j+1] - fft1[nn3-j] );
    fft1[j] = rep;
    fft1[j+1] = aim;
    fft1[nn2-j] = rep;
    fft1[nn3-j] = -aim;
    fft2[j] = aip;
    fft2[j+1] = -rem;
    fft2[nn2-j] = aip;
    fft2[nn3-j] = rem;
  }
}

void realft(FLOAT data[], unsigned long n, int isign)
{
  void four1(FLOAT data[], unsigned long nn, int isign);
  unsigned long i, i1, i2, i3, i4, np3;
  FLOAT c1 = 0.5, c2, h1r, h1i, h2r, h2i;
  double wr, wi, wpr, wpi, wtemp, theta;

  theta = M_PI/(double) (n>>1);
  if (isign == 1) {
    c2 = -0.5;
    four1(data, n>>1, 1);
  } else {
    c2 = 0.5;
    theta = -theta;
  }
  wtemp = sin(0.5*theta);
  wpr = -2.0 * wtemp * wtemp;
  wpi = sin(theta);
  wr = 1.0 + wpr;
  wi = wpi;
  np3 = n + 3;
  for (i = 2; i <= (n>>2); i++) {
    i4 = 1 + (i3=np3-(i2=1+(i1=i+i-1)));
    h1r =  c1*( data[i1] + data[i3] );
    h1i =  c1*( data[i2] - data[i4] );
    h2r = -c2*( data[i2] + data[i4] );
    h2i =  c2*( data[i1] - data[i3] );
    data[i1] =  h1r + wr*h2r - wi*h2i;
    data[i2] =  h1i + wr*h2i + wi*h2r;
    data[i3] =  h1r - wr*h2r + wi*h2i;
    data[i4] = -h1i + wr*h2i + wi*h2r;
    wr = (wtemp=wr)*wpr - wi*wpi + wr;
    wi = wi*wpr + wtemp*wpi + wi;
  }
  if (isign == 1) {
    data[1] = (h1r=data[1]) + data[2];
    data[2] = h1r - data[2];
  } else {
    data[1] = c1*( (h1r=data[1])+data[2] );
    data[2] = c1*( h1r - data[2] );
    four1(data, n>>1, -1);
  }
}

void four1(FLOAT data[], unsigned long nn, int isign)
{
  unsigned long n, mmax, m, j, istep, i;
  double wtemp, wr, wpr, wpi, wi, theta;
  FLOAT tempr, tempi;

  n = nn << 1;
  j = 1;
  for (i = 1; i < n; i += 2) {
    if (j > i) {
      SWAP(data[j], data[i]);
      SWAP(data[j+1], data[i+1]);
    }
    m = n >> 1;
    while (m >= 2 && j > m) {
      j -= m;
      m >>= 1;
    }
    j += m;
  }
  mmax = 2;
  while (n > mmax) {
    istep = mmax << 1;
    theta = isign * (2.0*M_PI/mmax);
    wtemp = sin(0.5*theta);
    wpr = -2.0 * wtemp * wtemp;
    wpi = sin(theta);
    wr = 1.0;
    wi = 0.0;
    for (m = 1; m < mmax; m += 2) {
      for (i = m; i <= n; i += istep) {
        j = i + mmax;
        tempr = wr*data[j] - wi*data[j+1];
        tempi = wr*data[j+1] + wi*data[j];
        data[j] = data[i] - tempr;
        data[j+1] = data[i+1] - tempi;
        data[i] += tempr;
        data[i+1] += tempi;
      }
      wr = (wtemp=wr)*wpr - wi*wpi + wr;
      wi = wi*wpr + wtemp*wpi + wi;
    }
    mmax = istep;
  }
}

void convolv(FLOAT data[], unsigned long n, FLOAT respns[], unsigned long m,
             int isign, FLOAT ans[])
{
  void realft(FLOAT data[], unsigned long n, int isign);
  void twofft(FLOAT data1[], FLOAT data2[], FLOAT fft1[], FLOAT fft2[],
              unsigned long n);
  unsigned long i, no2;
  FLOAT dum, mag2, *fft;

  fft = vector(1, n<<1);
  for (i = 1; i <= (m-1)/2; i++)
    respns[n+1-i] = respns[m+1-i];
  for (i = (m+3)/2; i <= n-(m-1)/2; i++)
    respns[i] = 0.0;

  twofft(data, respns, fft, ans, n);

  no2 = n>>1;
  for (i = 2; i <= n+2; i += 2) {
    if (isign == 1) {
      ans[i-1] = (fft[i-1]*(dum=ans[i-1])-fft[i]*ans[i])/no2;
      ans[i] = (fft[i]*dum + fft[i-1]*ans[i])/no2;
    } else if (isign == -1) {
      if ((mag2=(ans[i-1])*(ans[i-1])+(ans[i])*(ans[i])) == 0.0)
        fprintf(stderr,"convolv: Deconvolving at response zero in convlv\n");
      ans[i-1] = (fft[i-1]*(dum=ans[i-1])+fft[i]*ans[i])/mag2/no2;
      ans[i] = (fft[i]*dum - fft[i-1]*ans[i])/mag2/no2;
    } else fprintf(stderr,"convolv: No meaning for isign in convlv\n");
  }
  ans[2] = ans[n+1];
  realft(ans, n, -1);
  free_vector(fft, 1, n<<1);
}

HIST *HMConvolute(HIST *id1, HIST *id2)
{
  int i, nm = id1->nx;
  HIST *result = HMScalar(id1, '*', 0);
  FLOAT *data  = (FLOAT *) calloc(sizeof(FLOAT), nm+1);
  FLOAT *resp  = (FLOAT *) calloc(sizeof(FLOAT), nm+1);
  FLOAT *conv  = (FLOAT *) calloc(sizeof(FLOAT), (nm+1)*2);
  FLOAT data_norm = 0.0, resp_norm = 0.0;

  if (id1->ny!=0) {
    fprintf(stderr,"Convolute: only for 1d histos\n");
    exit(-1);
  } else if (id1->nx!=id2->nx) {
    fprintf(stderr,"Convolute: histos should have equal # of bins (2^n)\n");
    exit(-1);
  }

  for (i = 0; i < nm; i++) {
    data_norm += id1->data[i];
    resp_norm += id2->data[i];
  }
  for (i = 0; i < nm; i++) {
    data[i+1] = id1->data[i]/data_norm;
    resp[i+1] = id2->data[i]/resp_norm;
  }

  convolv(data, nm, resp, nm, +1, conv);
  for (i = 0; i < nm; i++) result->data[i] = conv[i+1]*data_norm;

  free(data);
  free(resp);
  free(conv);
  
  return result;
}

HIST *HMDeconvolute(HIST *id1, HIST *id2)
{
  int i, nm = id1->nx;
  HIST *result = HMScalar(id1, '*', 0);
  FLOAT *data   = (FLOAT *) calloc(sizeof(FLOAT), nm+1);
  FLOAT *resp   = (FLOAT *) calloc(sizeof(FLOAT), nm+1);
  FLOAT *deconv = (FLOAT *) calloc(sizeof(FLOAT), (nm+1)*2);
  FLOAT data_norm = 0.0, resp_norm = 0.0;

  if (id1->ny!=0) {
    fprintf(stderr,"Deconvolute: only for 1d histos\n");
    exit(-1);
  } else if (id1->nx!=id2->nx) {
    fprintf(stderr,"Deconvolute: histos should have equal # of bins (2^n)\n");
    exit(-1);
  }

  for (i = 0; i < nm; i++) {
    data_norm += id1->data[i];
    resp_norm += id2->data[i];
  }
  for (i = 0; i < nm; i++) {
    data[i+1] = id1->data[i]/data_norm;
    resp[i+1] = id2->data[i]/resp_norm;
  }

  convolv(data, nm, resp, nm, -1, deconv);
  for (i = 0; i < nm; i++) result->data[i] = deconv[i+1]*data_norm;

  free(data);
  free(resp);
  free(deconv);
  
  return result;
}

