#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void invert(double *in, double *out, int n)
{ int z,s,i;
  double faktor, *mm = (double *) calloc(n*n, sizeof(double));

  for (z=0;z<n;z++) 
    for (s=0;s<n;s++) {
      out[z+n*s] = (z==s ? 1 : 0);
      mm[z+n*s]  = in[z+n*s];
    }
  for (s=0;s<n-1;s++)
    for (z=s+1;z<n;z++) {
      int pivo = s,p,ss;
      for (p=s;p<n;p++) 
	if(fabs(mm[p+n*s]) > fabs(mm[pivo+n*s])) 
	  pivo=p;
      for(ss=0;ss<n;ss++) {
	double sz = mm[s+n*ss];	
	mm[s+n*ss] = mm[pivo+n*ss];
	mm[pivo+n*ss] = sz;
	sz = out[s+n*ss]; 
	out[s+n*ss] = out[pivo+n*ss];
	out[pivo+n*ss] = sz;
      }
      
      if (mm[s+n*s]==0.0) {
	fprintf(stderr,"Can't invert matrix\n");
	return;
      }
      faktor = mm[z+n*s]/mm[s+n*s];
      for(i=0;i<n;i++) {
	mm[z+n*i] -= mm[s+n*i]*faktor;
	out[z+n*i] -= out[s+n*i]*faktor;
      }
    }
  for (s=n-1;s>0;s--)
    for (z = s-1;z>=0;z--) {
      if (mm[s+n*s]==0.0) {fprintf(stderr,"Can't invert matrix\n");return;}
      faktor = mm[z+n*s]/mm[s+n*s];
      for(i=0;i<n;i++) {
	mm[z+n*i] -= mm[s+n*i]*faktor;
	out[z+n*i] -= out[s+n*i]*faktor;
      }
    }
  for (z=0;z<n;z++)
    for (s = 0;s<n;s++)
      out[z+n*s] /= mm[z+n*z];
  
  free(mm);
}






