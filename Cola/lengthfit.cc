// Path Length Fit
// Input:  ray trace matrix elements
//         x,theta,y,phi and pathlength for an event sample
// Output: ray trace matrix elements for new path length
//
//
// Author: Harald Merkel
//         A1 Collaboration
//         Institut für Kernphysik
//         Johannes Gutenberg-Universität Mainz
//
// $header$
//
// Length of reference trajectory (design): A: 1028 cm 
//                                          B: 1203 cm 
//                                          C:  808 cm
//
//

#define V_LIGHT 29.9792458     // cm/ns

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "ana++/Matrix.h"

// Gaussian Elimination with Row-Pivot and iterative improvement

void solve(double *m, double *b, double *result, int n)
{
  double *mm = new double[n*n];
  double *bb = new double[n];

  for (int z=0; z<n; z++)   bb[z] = b[z];
  for (int s=0; s<n*n; s++) mm[s] = m[s];

  for (int s=0; s<n-1; s++) {
    int zmax = s; 
    double max = fabs(mm[s+n*s]);
    for(int z=s+1; z<n; z++)  // find pivot row
      if (fabs(mm[z+n*s])>max) max = fabs(mm[(zmax=z)+n*s]);
    if (zmax != s) {          // swap pivot row to diagonal element 
      double tmp = bb[s]; 
      bb[s] = bb[zmax];
      bb[zmax] = tmp;
      for(int l=0; l<n; l++) {
	tmp = mm[s + n * l];
        mm[s + n * l] = mm[zmax + n * l];
        mm[zmax + n * l] = tmp; 
      }
    }
    if (mm[s+n*s]==0) {
      fprintf(stderr,"Can't invert matrix\n"); 
      exit(-1); 
    }
    for (int z=s+1; z<n; z++) { // column elimination
      double faktor = mm[z+n*s]/mm[s+n*s];
      for(int i=0; i<n; i++)
        mm[z+n*i] -= mm[s+n*i] * faktor;
      bb[z] -= bb[s] * faktor;
    }
  }
  
  for (int z=n-1;z>=0; z--) { // backsubstitution
    double dummy =0;
    for(int s=n-1; s>z; s--) dummy += mm[z + n * s] * result[s];
    result[z] = (bb[z] - dummy) / mm[z + n * z];
  }
  
  static int call = 0;       // iterative improvement
  if (call == 0) {
    call=1;
    for(int l=1;l<3;l++) {
      for(int z=0;z<n;z++) {
	bb[z] = -b[z];
        for(int s=0;s<n;s++)
          bb[z] += m[z+n*s] * result[s];
	printf("%2d %10g %10g\n",z,bb[z]/b[z], b[z]); 
      }
      double *res2 = new double[n];
      solve (m, bb, res2, n); 
      for(int z=0;z<n;z++) result[z] -= res2[z]; 
      delete res2;
    } 
    call=0;
  } 
  delete mm;
  delete bb;
}

int main(int argc, char *argv[])
{
  if (argc!=5) {
    cerr << "Usage: " << argv[0] << " tmafile lengthfitflag eventdata newtmafile" << endl; 
    exit (1);
  }

  FILE *fp = fopen(argv[1],"r");
  if (!fp) return fprintf(stderr,"Can't open %s\n",argv[1]);
  
  int z,s;

  int d=0, lines, maxx, maxy, maxtheta, maxphi;
  int i=0;
  int START_MATRIX = 0;
  fscanf(fp, "%d%d%d%d%d", &lines, &maxx, &maxtheta, &maxy, &maxphi);
  long   *x     = new long[lines],    *theta = new long[lines],
         *y     = new long[lines],    *phi   = new long[lines];
  double d1, d2, d3, d4, a;
  
  double length = 0;
  while (i<lines) {
    //  while (!feof(fp)) {
    fscanf(fp, "%d%d%d%d%lf%lf%lf%lf%lf", &x[d], &theta[d], &y[d], &phi[d],
	                                  &d1, &d2, &d3, &d4, &a);
    if (feof(fp)) break;
    i++;
    if (x[d]==0 && theta[d]==0 && y[d]==0 && phi[d]==0) {
      length = a;
      cout << "len = " << length << endl;
    }
    if (a != 0.0) d++;
    
  }
  fclose(fp);

  int matrix_dim = d;

  //Fit of mean length?
  if (atoi(argv[2])==1 ) {
    int add_elements = 1;
    d++;
  }
  
  fp = (!strcmp(argv[3],"-") ? stdin : fopen(argv[3],"r")); 
  if (!fp) return fprintf(stderr, "Error: Can't open \"%s\"!\n",argv[3]);

  double *m   = new double[d * d], 
         *b   = new double[d],
         *result= new double[d],
         *pot = new double[d],
         offset_x = 0, offset_theta = 0, offset_y = 0, offset_phi = 0, 
         scale_x = 0.1, scale_y = 0.1;
  fscanf(fp,"%lf%lf%lf%lf%lf%lf", &offset_x, &offset_theta, &offset_y, 
                                  &offset_phi, &scale_x, &scale_y);
  for(s=0; s<d*d; s++) m[s] = 0;
  for(z=0; z<d;   z++) b[z] = 0;

  int count=0;
  double lengthtotime = 1;
  while (!feof(fp)) {
    double dx,dy,dtheta,dphi,dl,beta;
    fscanf(fp,"%lf%lf%lf%lf%lf%lf", &dx, &dtheta, &dy, &dphi, &dl, &beta);
    if (count >= 0) {
      dx     = (dx - offset_x) * scale_x;
      dtheta -= offset_theta;
      dy     = (dy - offset_y) * scale_y;
      dphi   -= offset_phi;
      //      cout << "dx= " << dx << " dth = " << dtheta << " dy = " << dy;
      //cout << " dph = " << dphi << endl;
      if(feof(fp)) break;
      for (int j=0; j<matrix_dim; j++) { 
	lengthtotime = V_LIGHT * beta;
	if (j==0) lengthtotime = 1;
	pot[j] = pow(dx, x[j])*pow(dtheta, theta[j])
	  *pow(dy, y[j])*pow(dphi,   phi[j]) / lengthtotime;
      }
      lengthtotime = V_LIGHT * beta;
      if (atoi(argv[2])==1 ) {
	pot[matrix_dim] = 1 / lengthtotime;
      }
      else {
	dl += length / lengthtotime;
      }

      for(z=0;z<d;z++) {
	b[z] -= dl * pot[z]; 
	for(s=0; s < d ; s++)  m[z + d * s] += pot[s] * pot[z];
      }
    }
    if (!((count++ + 1)%1000)) fprintf(stderr,"\r%d    ", count);
  }
  fprintf(stderr,"\r%d    \n", count);

  solve(m, b, result, d);
  
  Vector ve(d,b);
  Matrix ma(d,m);
  Matrix im = ma.inv();
  Vector erg = im * ve;

  for(z=0; z<d;   z++) {
    printf("result[%2i] = %10.3e erg[%2i] = %10.3e\n",z,result[z],z,erg[z]);
  }

  //
  // Create the new TMA file, using the old file for the other elements
  //

  fp = fopen(argv[1],"r");
  fscanf(fp, "%d%d%d%d%d", &lines, &maxx, &maxtheta, &maxy, &maxphi);

  FILE *out= fopen(argv[4],"w");
  if(!out) {
    perror("Can't open outfile");
    return 1;
  }
  z=0;
  i=0;
  fprintf(out,"%d %d %d %d %d\n", lines, maxx, maxtheta, maxy, maxphi);
  while (i<lines) {
    fscanf(fp, "%d%d%d%d%lf%lf%lf%lf%lf", &x[0], &theta[0], &y[0], &phi[0],
  	                                  &d1, &d2, &d3, &d4, &a);
    if (feof(fp)) break;
    i++;
    if (!x[0] &&  !theta[0] && !y[0] && !phi[0]) {
      if (atoi(argv[2])==1) {
	result[z]=result[matrix_dim];
      }
      else {
	result[z]=length;
      }
    }
    fprintf(out,"%d %d %d %d %12.6g %12.6g %12.6g %12.6g %12.6g\n",
	   x[0], theta[0], y[0], phi[0],
	   d1, d2, d3, d4,(a != 0.0 ? result[z++]:0));
  }
  fclose(fp);
  fclose(out);
  
  return 0;
}
















