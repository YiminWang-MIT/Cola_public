#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "hmbook.h"
/* Random generators rndm() and norran() are taken from Numerical Recipes */

#define M1 259200
#define IA1 7141
#define IC1 54773
#define RM1 (1.0/M1)
#define M2 134456
#define IA2 8121
#define IC2 28411
#define RM2 (1.0/M2)
#define M3 243000
#define IA3 4561
#define IC3 51349

static int idum = 0;

FLOAT rndm()
{
	static long ix1,ix2,ix3;
	static FLOAT r[98];
	FLOAT temp;
	static int iff=0;
	int j;
	void nrerror();

	if (idum < 0 || iff == 0) {
		iff=1;
		ix1=(IC1-(idum)) % M1;
		ix1=(IA1*ix1+IC1) % M1;
		ix2=ix1 % M2;
		ix1=(IA1*ix1+IC1) % M1;
		ix3=ix1 % M3;
		for (j=1;j<=97;j++) {
			ix1=(IA1*ix1+IC1) % M1;
			ix2=(IA2*ix2+IC2) % M2;
			r[j]=(ix1+ix2*RM2)*RM1;
		}
		idum=1;
	}
	ix1=(IA1*ix1+IC1) % M1;
	ix2=(IA2*ix2+IC2) % M2;
	ix3=(IA3*ix3+IC3) % M3;
	j=1 + ((97*ix3)/M3);
	if (j > 97 || j < 1) fprintf(stderr,"rndm: This cannot happen.");
	temp=r[j];
	r[j]=(ix1+ix2*RM2)*RM1;
	return temp;
}

static int iset=0;
static FLOAT gset;

void norran(FLOAT *randomnumber)
{
	FLOAT fac,r,v1,v2;
	FLOAT rndm();

	if  (iset == 0) {
		do {
			v1=2.0*rndm()-1.0;
			v2=2.0*rndm()-1.0;
			r=v1*v1+v2*v2;
		} while (r >= 1.0);
		fac=sqrt(-2.0*log(r)/r);
		gset=v1*fac;
		iset=1;
		*randomnumber = v2*fac;
	} else {
		iset=0;
		*randomnumber = gset;
	}
}

FLOAT HMRandom1(HIST *id)
{ int i, ix=0;
  FLOAT yneu, x, y, x1, x2, y1, y2, dx;

  y = rndm();
  x1 = y1 = 0.0;
  x2 = y2 = 1.0;

  if(id->random==0)
    {  id->random = (FLOAT *) calloc(id->nx + 1, sizeof(FLOAT));
       id->random[0] = 0.0;
       for(i=0;i<id->nx;i++) id->random[i+1] = id->random[i]+id->data[i];
       for(i=0;i<=id->nx;i++) id->random[i] /= id->random[id->nx];
     } 
  for(i=0;i<100;i++)
    { x = x1 + (y-y1) / (y2-y1) * (x2-x1);
      if (x2 - x1 <= 1.001 / (FLOAT) id->nx) break;
      ix = (int) (x * (FLOAT)id->nx);
      dx = x - (FLOAT) ix / (FLOAT)id->nx; 
      yneu = id->random[ix] 
	+ dx * (FLOAT)id->nx *(id->random[ix+1]-id->random[ix]);
      if(yneu<y) {x1 = x; y1 = yneu;}
      else       {x2 = x; y2 = yneu;}
    }
  return id->minx + x * (id->maxx - id->minx);
}

void HMRandom2(HIST *id, FLOAT *xr, FLOAT *yr)
{ int i,j, iii,ix=0;
  FLOAT yneu, x, y = rndm(), x1 = 0.0, x2 = 1.0, y1= 0.0, y2 = 1.0, dx;
  
#define dat(ix,iy) id->data  [(ix)*id->ny + (iy)]
#define ran(ix,iy) id->random[(ix)*id->ny + (iy)]
#define yran(iy)   id->random[(id->nx+1)*id->ny+(iy)]
  
  if(id->random==0)
    { id->random = (FLOAT *) calloc((id->nx+1)*id->ny + id->ny + 1, 
				    sizeof(FLOAT));
      yran(0)=0.0;
      for(j=0;j<id->ny;j++)
	{ ran(0,j) = 0.0;
	  for(i=0;i<id->nx;i++)  ran(i+1,j) = ran(i,j) + dat(i,j);
	  yran(j+1) = ran(id->nx,j) + yran(j);
	  for(i=0;i<=id->nx;i++) ran(i,j) /= ran(id->nx,j);
	}
      for(j=0;j<=id->ny;j++) yran(j)/= yran(id->ny);
    } 
  
  for(i=0;i<100;i++)
    { x = x1 + (y-y1) / (y2-y1) * (x2-x1);
      if (x2 - x1 <= 1.001 / (FLOAT) id->ny) break;
      ix = (int) (x * (FLOAT) id->ny);
      dx = x - (FLOAT) ix / (FLOAT)id->ny; 
      yneu = yran(ix)
	+ dx * (FLOAT) id->ny *(yran(ix+1)-yran(ix));
      if(yneu<y) {x1 = x; y1 = yneu;}
      else       {x2 = x; y2 = yneu;}
    }
  *yr = id->miny + x * (id->maxy - id->miny);

  iii = ix;
  y = rndm();
  x1 = y1 = 0.0;
  x2 = y2 = 1.0;
  for(i=0;i<100;i++)
    { x = x1 + (y-y1) / (y2-y1) * (x2-x1);
      if (x2 - x1 <= 1.001 / (FLOAT) id->nx) break;
      ix = (int) (x * (FLOAT)id->nx);
      dx = x - (FLOAT) ix / (FLOAT)id->nx; 
      yneu = ran(ix,iii)+dx*(FLOAT)id->nx *(ran(ix+1,iii)-ran(ix,iii));
      if(yneu<y) {x1 = x; y1 = yneu;}
      else       {x2 = x; y2 = yneu;}
    }
  *xr = id->minx + x * (id->maxx - id->minx);
}







