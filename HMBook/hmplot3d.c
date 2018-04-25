#define x3d(x,y)	(as*((x)*cos7 + (1.0-(y))/1.5*cos42) + x0)
#define y3d(x,y,z) (((z)-mini)/(maxi-mini)*zmax+as*(x*sin7+(y)/1.5*sin42)+oy0)

#define winkel42 	30.0
#define winkel7 	7.0
#define pi		3.141592653589793

static FLOAT maxi,mini;
static int flippery=0;
/*static int shadowEffect=0;*/

void HMFlip()
{
  flippery = !flippery;
}

static FLOAT gray(FLOAT x1,FLOAT y1,FLOAT z1,
		  FLOAT x2,FLOAT y2,FLOAT z2,
		  FLOAT x3,FLOAT y3,FLOAT z3)
{
  FLOAT	
    dx1 = x1 - x2, dy1 = y1 - y2, dz1 = (z1 - z2)/(maxi-mini),
    dx2 = x1 - x3, dy2 = y1 - y3, dz2 = (z1 - z3)/(maxi-mini),
    l[3] = {1.0, -1.0, 1.0}, n1,n2,n3;
  n1 = dy1 * dz2 - dz1*dy2;
  n2 = dz1 * dx2 - dx1*dz2;
  n3 = dx1 * dy2 - dy1*dx2;
  if (HMGetOption(SHADOW_EFFECT) == 0) {
    return 0.1 + 0.9*fabs(n1*l[0]+n2*l[1]+n3*l[2])/
      sqrt(n1*n1+n2*n2+n3*n3)/
      sqrt(l[0]*l[0]+l[1]*l[1]+l[2]*l[2]); 
  } else {
    return 0.54 + 0.44*fabs(n1*l[0]+n2*l[1]+n3*l[2])/
      sqrt(n1*n1+n2*n2+n3*n3)/
      sqrt(l[0]*l[0]+l[1]*l[1]+l[2]*l[2]); 
  }
}

void HMPlot3D(int nx, int ny, FLOAT *z, const char *title,
	      const char *xlab, const char *ylab, const char *zlab,
	      const char *xunit, const char *yunit, 
	      const char *zunit, int surf)
{
  FLOAT cos42, sin42, cos7, sin7, as, zmax,
    zw, x1, x2, x3, y1, y2, y3, xx, ddx;
  int 	i, j, labi, exponent, i5, i10;
  char  format[255], myzlab[255], myxlab[255], myylab[255];
  
  maxx  = HMmaximumx;
  minx  = HMminimumx;
  maxy  = HMmaximumy;
  miny  = HMminimumy;
  
  scale_figure(title);
  
  cos42 = cos(winkel42 * pi / 180);
  sin42 = sin(winkel42 * pi / 180);
  cos7  = cos(winkel7  * pi / 180);
  sin7  = sin(winkel7  * pi / 180);
  as    = breite/(cos42/1.5+cos7);
  zmax 	= hoehe - as * (sin7 + sin42/1.5);
  
  /*	+---------------------------------------------------------------+ */
  /*        Bestimme Minimum und Maximum in z                             */
  /*    +---------------------------------------------------------------+ */

  maxi=z[0]; mini=z[0];
  for(i=0;i<nx;i++)
    for(j=0;j<ny;j++) {
      if(z[i*ny+j]>maxi) maxi = z[i*ny+j];
      if(z[i*ny+j]<mini) mini = z[i*ny+j];
    }
  
  if (maxi<=mini) {maxi = 1.0; mini = -1.0;};
  if (HMmaximumz==NOMINIMUM) maxi = set_max(maxi,mini); else maxi = HMmaximumz;
  if (HMminimumz==NOMINIMUM) mini = set_min(maxi,mini); else mini = HMminimumz;
  HMminimumx=HMminimumy=HMminimumz
    =HMmaximumx=HMmaximumy=HMmaximumz = NOMINIMUM;

  if (maxi<=mini) {
    puts("Maximum<=Minimum while Plotsurface"); 
    return;
  } 

/*	+---------------------------------------------------------------+
		Zeichne Box
	+---------------------------------------------------------------+
*/

  if (zmax<0) puts("ratio height/width not suitable for 3D plot");
  fprintf(HMfp, "n %5.3f %5.3f m ",	x3d(0,0), y3d(0.0,0.0,mini));
  fprintf(HMfp, "%5.3f %5.3f l ",	x3d(1,0), y3d(1.0,0.0,mini));
  fprintf(HMfp, "%5.3f %5.3f l ",	x3d(1,1), y3d(1.0,1.0,mini));
  fprintf(HMfp, "%5.3f %5.3f l c s\n",	x3d(0,1), y3d(0.0,1.0,mini));
  fprintf(HMfp, "n %5.3f %5.3f m ",	x3d(0,1), y3d(0.0,1.0,mini));
  fprintf(HMfp, "%5.3f %5.3f l ",	x3d(0,1), y3d(0.0,1.0,maxi));
  fprintf(HMfp, "%5.3f %5.3f l ",	x3d(1,1), y3d(1.0,1.0,maxi));
  fprintf(HMfp, "%5.3f %5.3f l ",	x3d(1,0), y3d(1.0,0.0,maxi));
  fprintf(HMfp, "%5.3f %5.3f l s\n",	x3d(1,0), y3d(1.0,0.0,mini));
  fprintf(HMfp, "n %5.3f %5.3f m ",	x3d(1,1), y3d(1.0,1.0,maxi));
  fprintf(HMfp, "%5.3f %5.3f l s\n",	x3d(1,1), y3d(1.0,1.0,mini));
                                                
/*	+---------------------------------------------------------------+
		Oberflaeche Zeichnen
	+---------------------------------------------------------------+
*/

  if (surf) {
    fprintf(HMfp," %f setlinewidth\n", HMGetOption(LINE_WIDTH));
    for(i=nx-2;i>=0;i--)
      for(j=ny-2;j>=0;j--) {
	zw = (z[i*ny+j] + z[(i+1)*ny+j+1] + z[i*ny+j + 1]+z[(i+1)*ny+j])/4.0;
	x1 = 1.0/nx * i + 0.5/nx;
	x2 = 1.0/nx * i + 1.0/nx;
	x3 = 1.0/nx * i + 1.5/nx;
	if (flippery==-1) {
	  y1 = 1.0/ny * (ny-j) - 0.5/ny;
	  y2 = 1.0/ny * (ny-j) - 1.0/ny;
	  y3 = 1.0/ny * (ny-j) - 1.5/ny;
	} else {
	  y1 = 1.0/ny * j + 0.5/ny;
	  y2 = 1.0/ny * j + 1.0/ny;
	  y3 = 1.0/ny * j + 1.5/ny;
	}	
#define formt "%5.3f %5.3f %5.3f %5.3f %5.3f %5.3f dr\n"
	
	fprintf(HMfp,"%4.2f ",gray(x2, y2, zw, x1, y3, z[i*ny+j+1],
				   x3, y3, z[(i+1)*ny+j+1]));
	fprintf(HMfp,formt,x3d(x2,y2), y3d(x2,y2,zw),
		x3d(x1,y3), y3d(x1,y3,z[i*ny+j+1]),
		x3d(x3,y3), y3d(x3,y3,z[(i+1)*ny+j+1]));
        fprintf(HMfp,"%4.2f ",gray(x2, y2, zw, x1, y1, z[i*ny+j],
				   x1, y3, z[i*ny+j+1]));
        fprintf(HMfp,formt,x3d(x2,y2), y3d(x2,y2,zw),
		x3d(x1,y1), y3d(x1,y1,z[i*ny+j]),
		x3d(x1,y3), y3d(x1,y3,z[i*ny+j+1]));
        fprintf(HMfp,"%4.2f ",gray(x2, y2, zw, x3, y1, z[(i+1)*ny+j],
				   x3, y3, z[(i+1)*ny+j+1]));
        fprintf(HMfp,formt,x3d(x2,y2), y3d(x2,y2,zw),
		x3d(x3,y1), y3d(x3,y1,z[(i+1)*ny+j]),
		x3d(x3,y3), y3d(x3,y3,z[(i+1)*ny+j+1]));
        fprintf(HMfp,"%4.2f ",gray(x2, y2, zw, x1, y1, z[i*ny+j],
				   x3, y1, z[(i+1)*ny+j]));
        fprintf(HMfp,formt,x3d(x2,y2), y3d(x2,y2,zw),
		x3d(x1,y1), y3d(x1,y1,z[i*ny+j]),
		x3d(x3,y1), y3d(x3,y1,z[(i+1)*ny+j]));
      }
    fprintf(HMfp," 0.02 setlinewidth\n");
  } else
    
    /*	+---------------------------------------------------------------+
	!	Oder Lego-Plot zeichnen					!
	+---------------------------------------------------------------+
	*/
#define form2 "%5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f sq\n"
    
    for(i=nx-1;i>=0;i--)
      for(j=ny-1;j>=0;j--) {
	x1 = 1.0/nx * i;        x2 = 1.0/nx * i + 1.0/nx;
        y1 = 1.0/ny * j;        y2 = 1.0/ny * j + 1.0/ny;
	fprintf(HMfp,form2,x3d(x1,y1),y3d(x1,y1,z[i*ny+j]),	 
		x3d(x1,y2),y3d(x1,y2,z[i*ny+j]),
		x3d(x2,y2),y3d(x2,y2,z[i*ny+j]),
		x3d(x2,y1),y3d(x2,y1,z[i*ny+j]));
	fprintf(HMfp,form2,x3d(x1,y1),y3d(x1,y1,z[i*ny+j]),
		x3d(x1,y2),y3d(x1,y2,z[i*ny+j]),
		x3d(x1,y2),y3d(x1,y2,mini),
		x3d(x1,y1),y3d(x1,y1,mini));
	fprintf(HMfp,form2,x3d(x1,y1),y3d(x1,y1,z[i*ny+j]),
		x3d(x2,y1),y3d(x2,y1,z[i*ny+j]),
		x3d(x2,y1),y3d(x2,y1,mini),
		x3d(x1,y1),y3d(x1,y1,mini));
      }
  
  fprintf(HMfp, "n %5.3f %5.3f m ",	x3d(0,0),y3d(0.0,0.0,mini));
  fprintf(HMfp, "%5.3f %5.3f l ",	x3d(0,0),y3d(0.0,0.0,maxi));
  fprintf(HMfp, "%5.3f %5.3f l s\n",	x3d(0,1),y3d(0.0,1.0,maxi));
  fprintf(HMfp, "n %5.3f %5.3f m ",	x3d(0,0),y3d(0.0,0.0,maxi));
  fprintf(HMfp, "%5.3f %5.3f l s\n",	x3d(1,0),y3d(1.0,0.0,maxi));          
  
  fprintf(HMfp, "axis\n");
  /*	+---------------------------------------------------------------+
	z-Achsen-Beschriftung
	+---------------------------------------------------------------+
	*/
  scale_axis(myzlab,zlab,zunit,format,
	     &exponent,&i,&labi,&i5,&i10,&xx,&ddx,
	     maxi,mini,HMGetOption(ON_SCALE_Z), HMGetOption(FORMAT_AXIS_Z),
	     HMGetOption(LOG_SCALE_Z) );
  fprintf(HMfp, "%5.3f %5.3f gs translate ", x3d(0,1),y3d(0.0,1.0,mini));
  fprintf(HMfp, "[1 %f 0 1 0 0 ] concat\n", tan(winkel7/180.0*pi));

  if (HMGetOption(LOG_SCALE_Z) == 2.0) { 
    /**FAKE LOG-Z-SCALE (HISTORAM MUST CONTAIN LOG(Z) INSTEAD OF Z)**/
    FLOAT lxstart, lxstop;
    FLOAT ii,j;
    lxstart = floor(mini); lxstop  = ceil(maxi);  
    for(ii = lxstart; ii<=lxstop; ii++) { 
      for (j=pow(10,ii); j< 10*pow(10,ii); j+=pow(10,ii)) {
        FLOAT jj = log10(j);
        if (mini<=jj && jj<=maxi) {
	  fprintf(HMfp,"n %5.3f %5.3f m ",  0.0,  (jj-mini)/(maxi-mini)*zmax);
	  fprintf(HMfp,"%5.3f %5.3f l s\n", 0.15, (jj-mini)/(maxi-mini)*zmax);
	}
      }
      if (mini<=ii && ii<=maxi) {
	fprintf(HMfp,"n %5.3f %5.3f m ",  0.0, (ii-mini)/(maxi-mini)*zmax);
	fprintf(HMfp,"%5.3f %5.3f l s\n", 0.3, (ii-mini)/(maxi-mini)*zmax);
        fprintf(HMfp,"1 ");
        if      (ii==0) {txtconvert( "1"    ,format); fprintf(HMfp,format,0);}
        else if (ii==1) {txtconvert("10"    ,format); fprintf(HMfp,format,0);}
        else            {txtconvert("10^%d!",format); fprintf(HMfp,format, 
                                                              (int) ii);}
        fprintf(HMfp," %5.3f %5.3f text\n", -HMGetOption(AXI_X), 
                (ii-mini)/(maxi-mini)*(zmax-HMGetOption(FONTSIZE_AXIS)*0.66));
      }
    }
  }
  else {
    for (;xx<=maxi*1.001;i++) {
      if(xx>mini+ddx/20.0 && xx<maxi-ddx/20.0) {
	fprintf(HMfp,"n %5.3f %5.3f m ",  0.0, (xx-mini)/(maxi-mini)*zmax);
	if(i%i10 == 0)
	  fprintf(HMfp,"%5.3f %5.3f l s\n", 0.3, (xx-mini)/(maxi-mini)*zmax);
	else if(i%i5==0)
	  fprintf(HMfp,"%5.3f %5.3f l s\n", 0.2, (xx-mini)/(maxi-mini)*zmax);
	else
	  fprintf(HMfp,"%5.3f %5.3f l s\n", 0.1, (xx-mini)/(maxi-mini)*zmax);
      }
      if(i%labi == 0) {
	  fprintf(HMfp,"1 ");
	  fprintf(HMfp,format, xx/pow(10,exponent));
	  fprintf(HMfp,"%5.3f %5.3f text\n", 
		  -HMGetOption(AXI_X), (xx-mini)/(maxi-mini)
		  *(zmax-HMGetOption(FONTSIZE_AXIS)*0.66));
      }
      xx += ddx/10.0;
    }
  }
  fprintf(HMfp,"%5.3f %5.3f translate ", 
	  -HMGetOption(LAB_X)+HMGetOption(FONTSIZE_LABEL), zmax);
  fprintf(HMfp,"label 90 rotate 1 %s 0 0 text gr\n", myzlab);
  
  /*	+---------------------------------------------------------------+
	X-Achsen-Beschriftung
	+---------------------------------------------------------------+
	*/
  scale_axis(myxlab,xlab,xunit,format,&exponent,
	     &i,&labi,&i5,&i10,&xx,&ddx,maxx,minx,
	     HMGetOption(ON_SCALE_X), HMGetOption(FORMAT_AXIS_X),
	     HMGetOption(LOG_SCALE_X) );
  fprintf(HMfp, "%5.3f %5.3f gs translate ", x3d(0,0),y3d(0.0,0.0,mini));
  fprintf(HMfp, "[1 %f 0 1 0 0 ] concat\n", tan(winkel7/180.0*pi));
  for (;xx<=maxx*1.001;i++) {
    if(xx>minx+ddx/20.0 && xx<maxx-ddx/20.0) {
      fprintf(HMfp,"n %5.3f %5.3f m ",  (xx-minx)/(maxx-minx)*as, 0.0);
      if(i%i10 == 0)
	fprintf(HMfp,"%5.3f %5.3f l s\n", (xx-minx)/(maxx-minx)*as,0.3);
      else if(i%i5==0)
	fprintf(HMfp,"%5.3f %5.3f l s\n", (xx-minx)/(maxx-minx)*as,0.2);
      else
	fprintf(HMfp,"%5.3f %5.3f l s\n", (xx-minx)/(maxx-minx)*as,0.1);
    }
    if(i%labi == 0) {
      fprintf(HMfp,"%5.3f ", (xx-minx)/(maxx-minx));
      fprintf(HMfp,format, xx/pow(10,exponent));
      fprintf(HMfp,"%5.3f %5.3f text\n", (xx-minx)/(maxx-minx)*as,
	      -HMGetOption(AXI_Y));
    }
    xx += ddx/10.0;
  }
  fprintf(HMfp,"label 1 %s %5.3f %5.3f text gr\n", 
	  myxlab, as, -HMGetOption(LAB_Y));
  
  /*	+---------------------------------------------------------------+
		y-Achsen-Beschriftung
	+---------------------------------------------------------------+
	*/
  scale_axis(myylab,ylab,yunit,format,&exponent,
	     &i,&labi,&i5,&i10,&xx,&ddx,maxy,miny,
	     HMGetOption(ON_SCALE_Y), HMGetOption(FORMAT_AXIS_Y),
	     HMGetOption(LOG_SCALE_Y) );
  fprintf(HMfp, "%5.3f %5.3f gs translate ", x3d(0,1),y3d(0.0,1.0,mini));
  fprintf(HMfp, "[%f %f 0 1 0 0 ] concat\n",
	  cos(winkel42/180.0*pi)/1.5,
	  -tan(winkel42/180.0*pi)*cos(winkel42/180.0*pi)/1.5);
  for (;xx<=maxy*1.001;i++) {
    FLOAT ratio = (maxy-xx)/(maxy-miny);
    if (flippery==-1) {
      if(xx>miny+ddx/20.0 && xx<maxy-ddx/20.0) {
	fprintf(HMfp,"n %5.3f %5.3f m ",  (1-ratio)*as, 0.0);
        if(i%i10 == 0)	fprintf(HMfp,"%5.3f %5.3f l s\n", (1-ratio)*as,0.3);
        else if(i%i5==0)fprintf(HMfp,"%5.3f %5.3f l s\n", (1-ratio)*as,0.2);
        else		fprintf(HMfp,"%5.3f %5.3f l s\n", (1-ratio)*as,0.1);
      }
      if(i%labi == 0) {
	fprintf(HMfp,"%5.3f ", ratio);
        fprintf(HMfp,format, xx/pow(10,exponent));
        fprintf(HMfp,"%5.3f %5.3f text\n", (1-ratio)*as, -HMGetOption(AXI_Y));
      }
    } else {
      if(xx>miny+ddx/20.0 && xx<maxy-ddx/20.0) {
	fprintf(HMfp,"n %5.3f %5.3f m ",  ratio*as, 0.0);
        if(i%i10 == 0)	fprintf(HMfp,"%5.3f %5.3f l s\n", (ratio)*as,0.3);
        else if(i%i5==0)fprintf(HMfp,"%5.3f %5.3f l s\n", (ratio)*as,0.2);
        else		fprintf(HMfp,"%5.3f %5.3f l s\n", (ratio)*as,0.1);
      }
      if(i%labi == 0) {
	fprintf(HMfp,"%5.3f ", ratio);
        fprintf(HMfp,format, xx/pow(10,exponent));
        fprintf(HMfp,"%5.3f %5.3f text\n", ratio*as,-HMGetOption(AXI_Y));
      }
    }
    
    xx += ddx/10.0;
  }
  fprintf(HMfp,"label 0 %s 0 %5.3f text gr\n", myylab, -HMGetOption(LAB_Y));
}




















