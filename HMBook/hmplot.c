#define _POSIX_C_SOURCE 2

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "hmplot.h"
#include <math.h>  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#ifdef __sparc__
extern FILE     *popen(const char *, const char *);
extern int      pclose(FILE *);
#endif

#define NOMINIMUM 32343.0 
#define MAX(a,b) (a>b?a:b)

FILE *  HMfp;
int     HMFrameNr = 0;
FLOAT	HMminimumx, HMminimumy, HMminimumz, 
	HMmaximumx, HMmaximumy, HMmaximumz;

static int ispipe = 0;
static int first_win = -1;
  
static struct {const char *name; double value;} option[] = {
  {"",                  0.00},
  {"SIZE_X",           17.00},
  {"SIZE_Y",           25.00},
  {"MRG_LEFT",          2.54},
  {"MRG_LOW",           2.50},
  {"MRG_TITLE",         1.80},
  {"WIN_X",             1.10},
  {"WIN_Y",             0.90},
  {"WIN_TITLE",         0.40},
  {"LAB_X",             1.50},
  {"LAB_Y",             1.20},
  {"AXI_X",             0.10},
  {"AXI_Y",             0.50},
  {"FONTSIZE_AXIS",     0.40},
  {"FONTSIZE_LABEL",    0.45},
  {"FONTSIZE_COMMENT",  0.45},
  {"FONTSIZE_WINTITLE", 0.50},
  {"FONTSIZE_TITLE",    0.60},
  {"ON_SCALE_X",        0.00},
  {"ON_SCALE_Y",        0.00},
  {"ON_SCALE_Z",        0.00},
  {"LOG_SCALE_X",       0.00},
  {"LOG_SCALE_Y",       0.00},
  {"LOG_SCALE_Z",       0.00},
  {"USE_GHOSTVIEW",     0.00},
  {"TIGHT_GS",          0.00},
  {"GS_RESOLUTION",    72.00},
  {"NO_OPEN",           0.00},
  {"CENTER_SCALE_X",    1.00},
  {"FILL_RED",          1.00},
  {"FILL_GREEN",        1.00},
  {"FILL_BLUE",         1.00},
  {"FIT_ERROR",         0.00},
  {"COLOR_LABEL",       0.00},
  {"COLOR_WINTITLE",    0.00},
  {"COLOR_TITLE",       0.00},
  {"LINE_COLOR",        0.00},
  {"LINE_WIDTH",        0.02},
  {"LINE_STYLE",       -1.00},
  {"FILL_COLOR",        0xFFFFFF},
  {"DATA_MARK",         1.00},
  {"FORMAT_AXIS_X",     0.00},
  {"FORMAT_AXIS_Y",     0.00},
  {"FORMAT_AXIS_Z",     0.00},
  {"TEXT_ANGLE",        0.00},
  {"SHADOW_EFFECT",     0.00},
  {"DATA_SIZE",         0.07},
  {"TIGHT",             0.0},
  {"FILL_ZERO",         0.00},
  {"SCALE_TICKMARKS",   1.00},
  {"LAB_Z",             1.50},
  {"DELAY_TICKS",       0.00},
  {NULL, 0.0}};

static int
  nr_x = 1,  nr_y = 1,
  nr_next_x = 0, nr_next_y = 1,
  seite = 1,
  same_frame = 0;
static long fpboundingbox=0, fppages=0;

static char 
  pattern[255];

static FLOAT	
  minx= -1,	miny= -1, 
  maxx=  1,	maxy=  1,
  xx, yy,	ddy,
  breite=  1, hoehe=  1,
  x0=  0,	oy0=  0;
/*---------------------------------------------------------------------------*/
void HMSetFillRGB(FLOAT r, FLOAT g, FLOAT b)
{
  HMSetOption(FILL_COLOR, ((int)(r*0xff)<<16) + ((int)(g*0xFF)<<8) + b*0xFF);
  fprintf(stderr,"WARNING: HMSetFillRGB(%4.2f, %4.2f, %4.2f) is obsolete.\n"
	  "Please use HMSetOption(FILL_COLOR, 0x%6X).\n",
	  r,g,b, ((int)(r*0xff)<<16) + ((int)(g*0xFF)<<8) + (int)(b*0xFF));
}
/*---------------------------------------------------------------------------*/
void HMSetPredefinedStyle(int nr)
{
  switch (nr) {
  case ST_A4:   	
    HMSetOption(SIZE_X, 17.0); HMSetOption(SIZE_Y, 25.0);
    HMSetOption(MRG_LEFT, 2.5); HMSetOption(MRG_LOW, 2.5);
    HMSetOption(MRG_TITLE, 1.5);
    HMSetOption(WIN_X, 0.7);    HMSetOption(WIN_Y, 0.7);
    HMSetOption(WIN_TITLE, 0.4);
    HMSetOption(LAB_X, 1.2);    HMSetOption(LAB_Y, 0.8);
    HMSetOption(AXI_X, 0.1);    HMSetOption(AXI_Y, 0.3);
    HMSetOption(FONTSIZE_AXIS, 0.30);
    HMSetOption(FONTSIZE_LABEL, 0.35);
    HMSetOption(FONTSIZE_COMMENT, 0.35);
    HMSetOption(FONTSIZE_WINTITLE, 0.40);
    HMSetOption(FONTSIZE_TITLE, 0.50);
    break;
  case ST_LETTER:
    HMSetOption(SIZE_X, 17.5); HMSetOption(SIZE_Y, 24.0);
    HMSetOption(MRG_LEFT, 2.54); HMSetOption(MRG_LOW, 2.54); 
    HMSetOption(MRG_TITLE, 1.5);
    HMSetOption(WIN_X, 0.7);    HMSetOption(WIN_Y, 0.7);
    HMSetOption(WIN_TITLE, 0.4);
    HMSetOption(LAB_X, 1.2);    HMSetOption(LAB_Y, 0.8);
    HMSetOption(AXI_X, 0.1);    HMSetOption(AXI_Y, 0.3);
    HMSetOption(FONTSIZE_AXIS, 0.30);
    HMSetOption(FONTSIZE_LABEL, 0.35);
    HMSetOption(FONTSIZE_COMMENT, 0.35);
    HMSetOption(FONTSIZE_WINTITLE, 0.40);
    HMSetOption(FONTSIZE_TITLE, 0.50);
    break;
  case ST_OVERHEAD:
    HMSetOption(SIZE_X, 18.0);  HMSetOption(SIZE_Y, 22.0);
    HMSetOption(MRG_LEFT, 1.5); HMSetOption(MRG_LOW, 5);   
    HMSetOption(MRG_TITLE, 2.0);
    HMSetOption(WIN_X, 1.0);    HMSetOption(WIN_Y, 1.2);   
    HMSetOption(WIN_TITLE, 0.4);
    HMSetOption(LAB_X, 1.8);    HMSetOption(LAB_Y, 1.1);
    HMSetOption(AXI_X, 0.2);    HMSetOption(AXI_Y, 0.5);
    HMSetOption(FONTSIZE_AXIS, 0.47);
    HMSetOption(FONTSIZE_LABEL, 0.50);
    HMSetOption(FONTSIZE_COMMENT, 0.50);
    HMSetOption(FONTSIZE_WINTITLE, 0.55);
    HMSetOption(FONTSIZE_TITLE, 0.70);
    HMSetOption(COLOR_LABEL, 0x0000FF);
    HMSetOption(COLOR_WINTITLE, 0x008000);
    HMSetOption(COLOR_TITLE, 0xFF0000);
    break;
  case ST_PSFIG:    
    HMSetOption(SIZE_X, 16.0);  HMSetOption(SIZE_Y, 10.0);
    HMSetOption(MRG_LEFT, 0.0); HMSetOption(MRG_LOW, 0.0);
    HMSetOption(MRG_TITLE, 0.0);
    HMSetOption(WIN_X, 0.7);    HMSetOption(WIN_Y, 0.7);
    HMSetOption(WIN_TITLE, 0.4);
    HMSetOption(LAB_X, 1.2);    HMSetOption(LAB_Y, 0.8);
    HMSetOption(AXI_X, 0.1);    HMSetOption(AXI_Y, 0.3);
    HMSetOption(FONTSIZE_AXIS, 0.30);
    HMSetOption(FONTSIZE_LABEL, 0.35);
    HMSetOption(FONTSIZE_COMMENT, 0.35);
    HMSetOption(FONTSIZE_WINTITLE, 0.40);
    HMSetOption(FONTSIZE_TITLE, 0.0);
  case ST_PHYSREVLETT:    
    HMSetOption(SIZE_X, 8.5725);
    HMSetOption(SIZE_Y, 6.0);
    HMSetOption(MRG_LEFT,  2.54); HMSetOption(MRG_LOW, 19.5);
    HMSetOption(WIN_X, 0.4);      HMSetOption(WIN_Y, 0.2);
    HMSetOption(LAB_X, 0.9);      HMSetOption(LAB_Y, 0.65);
    HMSetOption(AXI_X, 0.05);     HMSetOption(AXI_Y, 0.27);
    HMSetOption(FONTSIZE_AXIS, 0.285);
    HMSetOption(FONTSIZE_LABEL, 0.30);
    HMSetOption(FONTSIZE_COMMENT, 0.285);
    HMSetOption(FONTSIZE_WINTITLE, 0.3);    HMSetOption(WIN_TITLE, 0.0);
    HMSetOption(FONTSIZE_TITLE, 0.3);       HMSetOption(MRG_TITLE, 0.0);
    break;
  }
}
/*---------------------------------------------------------------------------*/
void HMSetOption(int nr, FLOAT value)
{ option[nr].value = value;
  if (!HMfp) return;
  if (nr == FONTSIZE_AXIS)
    fprintf(HMfp,"/axis     {/fs %lf def (Helvetica) font} def\n",
	    HMGetOption(FONTSIZE_AXIS));
  if (nr == FONTSIZE_LABEL)
    fprintf(HMfp,"/label    {/fs %lf def (Helvetica) font} def\n",
	    HMGetOption(FONTSIZE_LABEL));
  if (nr == FONTSIZE_COMMENT)
    fprintf(HMfp,"/comment  {/fs %lf def (Helvetica) font} def\n",
	    HMGetOption(FONTSIZE_COMMENT));
  if (nr == FONTSIZE_WINTITLE)
    fprintf(HMfp,"/wintitle {/fs %lf def (Helvetica) font} def\n",
	    HMGetOption(FONTSIZE_WINTITLE));
  if (nr == FONTSIZE_TITLE)
    fprintf(HMfp,"/title    {/fs %lf def (Helvetica) font} def\n",
	    HMGetOption(FONTSIZE_TITLE));
}
/*---------------------------------------------------------------------------*/
FLOAT HMGetOption(int nr) {
  return option[nr].value;
}
/*---------------------------------------------------------------------------*/
FLOAT HMGetOptionName(const char *name) {
  int nr;
  for (nr=0; option[nr].name;nr ++)
    if (!strcmp(option[nr].name, name))
      return option[nr].value;
  fprintf(stderr,"Option \"%s\" is not defined\n", name);
  return 0;
}
/*---------------------------------------------------------------------------*/
void HMSetOptionName(const char *name, FLOAT value) {
  int nr;
  for (nr=0; option[nr].name;nr ++)
    if (!strcmp(option[nr].name, name)) {
      HMSetOption(nr, value);
      return;
    }
  fprintf(stderr,"Option \"%s\" is not defined\n", name);
  return;
}
/*---------------------------------------------------------------------------*/
void HMDefault()
{
  HMSetOption(SIZE_X,    17.5);
  HMSetOption(SIZE_Y,    24.0);
  HMSetOption(MRG_LEFT,   2.5);
  HMSetOption(MRG_LOW,    2.5);
  HMSetOption(MRG_TITLE,  1.5);
  HMSetOption(WIN_X,      0.7);
  HMSetOption(WIN_Y,      0.7);
  HMSetOption(WIN_TITLE,  0.4);
  HMSetOption(LAB_X,      1.2);
  HMSetOption(LAB_Y,      0.8);
  HMSetOption(AXI_X,      0.1);
  HMSetOption(AXI_Y,      0.3);
  HMSetOption(FONTSIZE_AXIS,     0.3);
  HMSetOption(FONTSIZE_LABEL,    0.35);
  HMSetOption(FONTSIZE_COMMENT,  0.35);
  HMSetOption(FONTSIZE_WINTITLE, 0.4);
  HMSetOption(FONTSIZE_TITLE,    0.5);
  HMSetOption(FORMAT_AXIS_X, 0.0);
  HMSetOption(FORMAT_AXIS_Y, 0.0);
  HMSetOption(FORMAT_AXIS_Z, 0.0);
}
/*---------------------------------------------------------------------------*/
void HMNewPage()
{ if (!HMfp) return;
  fprintf(HMfp,"grestore\n");
  fprintf(HMfp,"showpage\n");
  seite++;
  if(!ispipe)
  { fseek(HMfp,fppages,0);
    fprintf(HMfp,"%%%%Pages: %3d %3d\n",seite,seite);
    fseek(HMfp,0,2);
  }
  fprintf(HMfp,"%%%%Page: %d %d\n",seite,seite);
  fprintf(HMfp,"72 2.54 div dup scale\n");
  fprintf(HMfp,"0.02 setlinewidth\n");
  nr_next_x = 0,
  nr_next_y = 1; 	   
  first_win=-1;
}
/*---------------------------------------------------------------------------*/
void HMZone(int nrx, int nry, int nrnext)
{ if (nrnext==1 && (nr_next_x!=0 || nr_next_y!=1)) HMNewPage();
  nr_x = nrx;
  nr_y = nry;	
  nr_next_x = (nrnext-1) % nrx;
  nr_next_y = (nrnext-1) / nrx + 1;
}
/*---------------------------------------------------------------------------*/
static void setColor(int color) 
{
  fprintf(HMfp, "%4.3f ",              (color >> 16 & 0xFF)/255.0);
  fprintf(HMfp, "%4.3f ",              (color >>  8 & 0xFF)/255.0);
  fprintf(HMfp, "%4.3f setrgbcolor\n", (color       & 0xFF)/255.0);
}
/*---------------------------------------------------------------------------*/
static void txtconvert(const char *in, char *out)
{ int i=9,j= -1;
  
  sprintf(out,"{tstart (");
  
  while(in[++j]!= 0)
    switch (in[j])
    { case '[' : sprintf(&out[i], ") symb ("); i += 8; break; 
      case ']' : sprintf(&out[i], ") helv ("); i += 8; break; 
      case '(' : sprintf(&out[i], "\\("     ); i += 2; break; 
      case ')' : sprintf(&out[i], "\\)"     ); i += 2; break; 
      case '^' : sprintf(&out[i], ") supr ("); i += 8; break; 
      case '_' : sprintf(&out[i], ") subs ("); i += 8; break; 
      case '?' : sprintf(&out[i], ") subs ("); i += 8; break; 
      case '!' : sprintf(&out[i], ") norm ("); i += 8; break; 
      default  : out[i++] = in[j];
    }	
  sprintf(&out[i],") tstop}");
}

/*---------------------------------------------------------------------------*/
void HMTitle(const char *title)
{
  char tit[1000];
  txtconvert(title,tit);
  fprintf(HMfp,"gsave\n");
  fprintf(HMfp,"title\n");
  setColor((int) HMGetOption(COLOR_TITLE));
  fprintf(HMfp,"0.5 %s %5.3f ", tit, 
	  HMGetOption(MRG_LEFT) + HMGetOption(SIZE_X) / 2);
  fprintf(HMfp,"%5.3f text\n", 
	  HMGetOption(MRG_LOW)  + HMGetOption(SIZE_Y) 
	  - HMGetOption(FONTSIZE_TITLE));
  fprintf(HMfp,"grestore\n");
}
/*---------------------------------------------------------------------------*/
void HMSame()			
{same_frame= -1; HMFrameNr++;}
/*---------------------------------------------------------------------------*/
void HMMinimumX(FLOAT min)	
{HMminimumx = min;}
/*---------------------------------------------------------------------------*/
void HMMaximumX(FLOAT max)	
{HMmaximumx = max;}
/*---------------------------------------------------------------------------*/
void HMMinimumY(FLOAT min)	
{HMminimumy = min;}
/*---------------------------------------------------------------------------*/
void HMMaximumY(FLOAT max)	
{HMmaximumy = max;}
/*---------------------------------------------------------------------------*/
void HMMinimumZ(FLOAT min)	
{HMminimumz = min;}
/*---------------------------------------------------------------------------*/
void HMMaximumZ(FLOAT max)	
{HMmaximumz = max;}
/*---------------------------------------------------------------------------*/
static FLOAT set_max(FLOAT max, FLOAT min)
{ FLOAT lx = floor(log10(max-min)),
  	ddx = pow(10.0,lx);
  return floor(max/ddx*0.9999999 + 1) * ddx;
}
/*---------------------------------------------------------------------------*/
static FLOAT set_min(FLOAT max, FLOAT min)
{ FLOAT lx = floor(log10(max-min)),
  	ddx = pow(10.0,lx);
  return floor(min/ddx) * ddx;
}
/*---------------------------------------------------------------------------*/
static void setdash(const char *patt)
{	sprintf(pattern,"[%s] 0 setdash\n",patt);
}
/*---------------------------------------------------------------------------*/
FLOAT HMXPixel(FLOAT px)
{ return  (px - x0)/breite *(maxx-minx) + minx;
}
/*---------------------------------------------------------------------------*/
FLOAT HMYPixel(FLOAT py)
{   return (py - oy0)/hoehe  * (maxy-miny) + miny;
}
/*---------------------------------------------------------------------------*/
#define xneu(x) (x0 + breite / (maxx - minx) * ((x) - minx))
#define zneu(z) (((z) - mini)/(maxi - mini))

static FLOAT yneu(FLOAT y)
{
  if (HMGetOption(LOG_SCALE_Y)==1) {
    if (y < miny)
      return oy0;
    else
      return oy0+hoehe/(log10(maxy)-log10(miny)) * (log10(y) - log10(miny));
  } else
    return oy0 + hoehe  / (maxy - miny) * ((y) - miny);
}
/*---------------------------------------------------------------------------*/
static void scale_figure(const char *title)
{ char mytitle[1000];

  if (same_frame) return;
  txtconvert(title, mytitle);
  
  nr_next_x ++;
  if(nr_next_x > nr_x)
  { nr_next_x = 1;
    nr_next_y ++;
    if(nr_next_y > nr_y) { HMNewPage(); nr_next_x++;}
  }
  
  if (maxx<=minx) { maxx = 1; minx = -1; }
  if (maxy<=miny) { maxy = 1; miny = -1; }
  
  if (HMmaximumx == NOMINIMUM)  maxx = set_max(maxx, minx); 
  else                          maxx = HMmaximumx;
  if (HMminimumx == NOMINIMUM)  minx = set_min(maxx, minx); 
  else                          minx = HMminimumx;
  if (HMmaximumy == NOMINIMUM)  maxy = set_max(maxy, miny); 
  else                          maxy = HMmaximumy;
  if (HMminimumy == NOMINIMUM)  miny = set_min(maxy, miny); 
  else                          miny = HMminimumy;
  HMminimumx = HMminimumy = HMmaximumx = HMmaximumy = NOMINIMUM;
  
  if (maxx<=minx) puts("Minimum >= Maximum fuer X-Achse");
  if (maxy<=miny) puts("Minimum >= Maximum fuer Y-Achse");
  if (HMGetOption(LOG_SCALE_Y==1.0) && miny <= 0) miny = 0.001;
  if (HMGetOption(LOG_SCALE_Y) && miny >= maxy  ) maxy = 100*miny;
  
  breite = (HMGetOption(SIZE_X) - (nr_x-1) * HMGetOption(WIN_X))/nr_x;
  hoehe  = (HMGetOption(SIZE_Y) - (nr_y-1) * HMGetOption(WIN_Y) 
          - HMGetOption(MRG_TITLE))/nr_y;
  x0     = (nr_next_x-1)*(breite+HMGetOption(WIN_X)) 
          + HMGetOption(MRG_LEFT) + HMGetOption(LAB_X);

  oy0    = (nr_y - nr_next_y) * (hoehe +HMGetOption(WIN_Y)) 
          + HMGetOption(MRG_LOW) + HMGetOption(LAB_Y);
  breite -= HMGetOption(LAB_X);
  hoehe  -= (HMGetOption(LAB_Y) + HMGetOption(WIN_TITLE));

  if (first_win == -1) {
    fprintf(HMfp,          "gsave\nwintitle\n");
    first_win = 0;
  }
  else { 
    fprintf(HMfp,"grestore\ngsave\nwintitle\n");
  }

  setColor((int) HMGetOption(COLOR_WINTITLE));
  fprintf(HMfp,"0.5 %s %5.3f %5.3f text 0 setgray\n", 
	  mytitle, x0  + (breite - HMGetOption(LAB_X))/2.0, 
	           oy0 +   hoehe + HMGetOption(WIN_TITLE));
}
/*---------------------------------------------------------------------------*/
static void frame() { 
  fprintf(HMfp,"n %5.3f %5.3f m ",                 xneu(minx),yneu(miny));
  fprintf(HMfp,  "%5.3f %5.3f l ",                 xneu(minx),yneu(maxy));
  fprintf(HMfp,  "%5.3f %5.3f l ",                 xneu(maxx),yneu(maxy));
  fprintf(HMfp,  "%5.3f %5.3f l c gs s gr gsave clip s\n", xneu(maxx),yneu(miny));
}
static void next_frame() { 
  fprintf(HMfp,"n %5.3f %5.3f m ",                 xneu(minx),yneu(miny));
  fprintf(HMfp,  "%5.3f %5.3f l ",                 xneu(minx),yneu(maxy));
  fprintf(HMfp,  "%5.3f %5.3f l ",                 xneu(maxx),yneu(maxy));
  fprintf(HMfp,  "%5.3f %5.3f l c clip s\n",       xneu(maxx),yneu(miny));
}
/*---------------------------------------------------------------------------*/
static void 
calcunit(char *myunit, const char *label, const char *unit, 
	 char *format, int *exponent, int l, FLOAT min,
	 FLOAT onscale, FLOAT fmt, FLOAT logscale)
{ char lbl[1000],form[10];
/*  char *strcpy(char *, const char *);*/
  
  if (fmt==0) {

    if (l<0)  *exponent = (int)((l-1)/3)*3;
    else      *exponent = (int)(l/3)*3;
    
    if (l==3) *exponent = 0;
    
    if(min<0.0)
      { if ((l - *exponent)== -1)     strcpy(form, "%5.2f"); 
      else 
	if ((l - *exponent)== 0) strcpy(form, "%4.1f"); 
	else                     strcpy(form, "%4.0f"); 
      }
    else
      { if ((l - *exponent)== -1)     strcpy(form, "%4.2f"); 
      else 
	if ((l - *exponent)== 0) strcpy(form, "%3.1f"); 
	else                     strcpy(form, "%3.0f"); 
      }
  }  
  else {
    *exponent = 0;
    if (l<0) sprintf(form, "%%.%.0ff", fmt);
    else     sprintf(form, "%%.%.0ff", fmt);
  }
  
  if (onscale==1.0 && *exponent == 0)
  {     if (!strcmp(unit,"%"))
 	  sprintf(lbl,"%s\\045",form);
 	else
	  sprintf(lbl,"%s%s",form,unit);
	txtconvert(lbl,format);
	sprintf(lbl,"%s",label);
  } else
  { sprintf(lbl,"%s",form);
    txtconvert(lbl,format);
    if (unit[0])
      { 
	if (!*exponent || logscale )
	  sprintf   (lbl,"%s  \\133%s\\135",label,unit);
	else sprintf(lbl,"%s  \\13310^%d! %s\\135", label,*exponent,unit);
      } else { 
	if (!*exponent || logscale ) 
	  sprintf(lbl,"%s",label);
	else sprintf(lbl,"%s  \\13310^%d!\\135",label,*exponent);
      }
  }
  
  txtconvert(lbl,myunit);
}
/*---------------------------------------------------------------------------*/
static void 
scale_axis(char *myunit, const char *label, 
	   const char *unit, char *format, int  *exponent,
	   int  *i, int *di, int *i5, int *i10,
	   FLOAT *start, FLOAT *dx, FLOAT max, FLOAT min,
	   FLOAT onscale, FLOAT fmt, FLOAT logsc)
{ FLOAT	
    l = floor(log10(max-min)),
    dd = pow(10.0,l);

  int min000, min090, min180, max090, max180, max360;

  if (!strcmp(unit,"^o!") || !strcmp(unit,"\260") || !strcmp(unit,"%"))
    calcunit(myunit,label,unit,format, exponent, l, min, 1.0, fmt, logsc);
  else
    calcunit(myunit,label,unit,format, exponent, l, min, onscale, fmt, logsc);
  
  if (!strcmp(unit,"^o!") || !strcmp(unit,"\260"))
  { *start = min; *i=0;
    *i5 = 3; *i10 = 9;

    if (fabs(max-360.0)<0.8) max360 = 1; else max360 = 0;
    if (fabs(min-000.0)<0.8) min000 = 1; else min000 = 0;
    if (max360 && min000) {*start=floor(min+0.8); *dx= 50.0; *di = 18; return;}

    if (fabs(min+180.0)<0.8) min180 = 1; else min180 = 0; 
    if (fabs(max-180.0)<0.8) max180 = 1; else max180 = 0; 
    if (max180 && min180) {*start=floor(min+0.8); *dx= 50.0; *di = 18; return;}
    if (max180 && min000) {*start=floor(min+0.8); *dx= 50.0; *di =  9; return;}

    if (fabs(min+090.0)<0.8) min090 = 1; else min090 = 0; 
    if (fabs(max-090.0)<0.8) max090 = 1; else max090 = 0; 
    if (max090 && min090) {*start=floor(min+0.8); *dx= 50.0; *di =  9; return;}
    if (max090 && min000) {*start=floor(min+0.8); *dx= 50.0; *di =  3; return;}
  }
  if ((max-min)/dd>7)      *di = 20;
  else if ((max-min)/dd>3) *di = 10;
  else                     *di =  5;
  
  *start = floor((min - 0.00001*dd) * 10.0 / dd + 1.0) * dd / 10.0;
  if (fabs(*start-min)<dd/20) 
    *i = (*start - floor(*start/(*di*dd/10.))*(*di*dd/10.))/dd*10.0 +.5;
  else
    *i = (*start-floor(*start*0.999999/(*di*dd/10.))*(*di*dd/10.))/dd*10. +.5;
  *dx = dd;
  *i5 = 5; *i10=10;
}
/*---------------------------------------------------------------------------*/
static void 
plot_axis(const char *xlab, const char *ylab,
	  const char *xunit, const char *yunit)
{
  int i,labi,exponent,i5,i10;
  char myxlab[1000],myylab[1000],format[1000];
  FLOAT ddx;
  
  if (same_frame)   
    fprintf(HMfp,"axis\n");
  else
    fprintf(HMfp,"grestore axis\n");
  
  scale_axis(myxlab,xlab,xunit,format,&exponent,
	     &i,&labi,&i5,&i10,&xx,&ddx,maxx,minx,
	     HMGetOption(ON_SCALE_X), HMGetOption(FORMAT_AXIS_X),
	     HMGetOption(LOG_SCALE_X) );
  
  /**FAKE LOG-X-SCALE (LOG(X) HAS TO BE HISTOGRAMMED BEFORE EXPLICITLY)**/
  if (HMGetOption(LOG_SCALE_X) == 1.0) { 
    FLOAT lxstart, lxstop;
    FLOAT ii,j;
    lxstart = floor(minx); lxstop  = ceil(maxx);  
    for(ii = lxstart; ii<=lxstop;ii++) { 
      for (j=pow(10,ii); j< 10 * pow(10,ii); j+=pow(10,ii)) {
	FLOAT jj = log10(j);
	if ( minx<=jj && jj<=maxx )
	  fprintf(HMfp,"n %5.3f %5.3f m 0 %f rlineto s\n", 
		  xneu(jj), oy0, 0.2*HMGetOption(SCALE_TICKMARKS));
      }
      if (minx<=ii && ii<=maxx) {
	fprintf(HMfp, "%5.3f ",
		0.5*HMGetOption(CENTER_SCALE_X)+
		(1-HMGetOption(CENTER_SCALE_X)) * (ii-minx)/(maxx-minx));
	if      (ii==0) {txtconvert( "1"    ,format); fprintf(HMfp,format,0);}
	else if (ii==1) {txtconvert("10"    ,format); fprintf(HMfp,format,0);}
	else            {txtconvert("10^%d!",format); fprintf(HMfp,format, 
							      (int) ii);}
	fprintf(HMfp," %5.3f %5.3f text\n", xneu(ii), 
		oy0-1.3*HMGetOption(AXI_Y));
      }
    }
  } 
  else {
    for (;xx<=maxx+(maxx-minx)*0.00001;i++)  {
      if(xx>minx+ddx/20.0 && xx<maxx-ddx/20.0) {
	fprintf(HMfp,"n %5.3f %5.3f m ",  xneu(xx),     oy0);
	if(i%i10 == 0) fprintf(HMfp,"%5.3f %5.3f l s\n", 
			       xneu(xx), 
			       oy0+0.30*HMGetOption(SCALE_TICKMARKS));
	else if(i%i5==0) fprintf(HMfp,"%5.3f %5.3f l s\n", 
				 xneu(xx), 
				 oy0+0.2*HMGetOption(SCALE_TICKMARKS));
	else	   fprintf(HMfp,"%5.3f %5.3f l s\n", xneu(xx), 
			   oy0+0.10*HMGetOption(SCALE_TICKMARKS));
      }
      if(i%labi == 0){
	fprintf(HMfp, "%5.3f ",
		0.5*HMGetOption(CENTER_SCALE_X)+
		(1-HMGetOption(CENTER_SCALE_X)) * (xx-minx)/(maxx-minx));
	fprintf(HMfp, format,   	      xx/pow(10,exponent));
	fprintf(HMfp, "%5.3f %4.2f text\n", xneu(xx), 
		oy0-HMGetOption(AXI_Y));
      }
      xx += ddx/10.0;
    }
  }
  
  scale_axis(myylab,ylab,yunit,format,&exponent,&i,&labi,
	     &i5, &i10,&yy,&ddy,
	     maxy,miny,HMGetOption(ON_SCALE_Y), HMGetOption(FORMAT_AXIS_Y),
	     HMGetOption(LOG_SCALE_Y) );
  if (HMGetOption(LOG_SCALE_Y) == 1.0) { 
    FLOAT lxstart, lxstop, lminy, lmaxy;
    FLOAT ii,j;
    int from_atom_to_galaxy = 0;
    if (miny<=0.0) miny = 0.001;
    if (maxy<miny) maxy = miny*100;
    lminy = log10(miny);
    lmaxy = log10(maxy);
    if (lmaxy-lminy>11) from_atom_to_galaxy = 1;
    lxstart = floor(lminy); lxstop  = ceil(lmaxy);  
    for(ii = lxstart; ii<=lxstop;ii++) {
      FLOAT ee = pow(10,ii);
      for (j=ee; j< 10*ee; j+=ee)
	if (miny<=j && j<=maxy) {
	  if (j==ee)
	    fprintf(HMfp,"n %5.3f %5.3f m %f 0 rlineto s\n", x0, yneu(j),
		    0.2 * HMGetOption(SCALE_TICKMARKS));
	  else
	    fprintf(HMfp,"n %5.3f %5.3f m %f 0 rlineto s\n", x0, yneu(j),
		    0.1 * HMGetOption(SCALE_TICKMARKS));
	}
      if (!from_atom_to_galaxy || !((int)ii%3) )
	if (lminy<=ii && ii<=lmaxy) {
	  fprintf(HMfp,"1 ");
	  if      (ii==0) {txtconvert( "1"    ,format); fprintf(HMfp,format,0);}
	  else if (ii==1) {txtconvert("10"    ,format); fprintf(HMfp,format,0);}
	  else            {txtconvert("10^%d!",format); fprintf(HMfp,format, 
								(int) ii);}
	  fprintf(HMfp," %5.3f %5.3f text\n", x0-HMGetOption(AXI_X), 
		  yneu(pow(10,ii))-(ii-lxstart)/(lxstop-lxstart)*
		  HMGetOption(FONTSIZE_AXIS) * 0.66);
	}
    }
  } 
  else if (HMGetOption(LOG_SCALE_Y) == 2.0) { 
    /**FAKE LOG-Y-SCALE (HISTORAM MUST CONTAIN LOG(Y) INSTEAD OF Y)**/
    FLOAT lxstart, lxstop;
    FLOAT ii,j;
    printf("miny=%lf, maxy=%lf\n", miny,maxy);
    lxstart = floor(miny); lxstop  = ceil(maxy);  
    for(ii = lxstart; ii<=lxstop;ii++) { 
      for (j=pow(10,ii); j< 10 * pow(10,ii); j+=pow(10,ii)) {
        FLOAT jj = log10(j);
        if (miny<=jj && jj<=maxy)
          fprintf(HMfp,"n %5.3f %5.3f m %f 0 rlineto s\n", x0, yneu(jj),
		  0.2*HMGetOption(SCALE_TICKMARKS));
      }
      if (miny<=ii && ii<=maxy) {
        fprintf(HMfp,"1 ");
        if      (ii==0) {txtconvert( "1"    ,format); fprintf(HMfp,format,0);}
        else if (ii==1) {txtconvert("10"    ,format); fprintf(HMfp,format,0);}
        else            {txtconvert("10^%d!",format); fprintf(HMfp,format, 
                                                              (int) ii);}
        fprintf(HMfp," %5.3f %5.3f text\n", x0-HMGetOption(AXI_X), 
                yneu(ii)-(ii-lxstart)/(lxstop-lxstart)*
                HMGetOption(FONTSIZE_AXIS) * 0.66);
      }
    }
  }
  else
    {
    for (; yy<=maxy+(maxy-miny)*0.00001; i++)
    { if(yy>miny+ddy/20.0 && yy<maxy-ddy/20.0)
      { fprintf(HMfp,"n %5.3f %5.3f m ",  x0,     yneu(yy));
	if(i%i10 == 0)  fprintf(HMfp,"%5.3f %5.3f l s\n",
				x0+0.30*HMGetOption(SCALE_TICKMARKS),yneu(yy));
	else if(i%i5==0)fprintf(HMfp,"%5.3f %5.3f l s\n",
				x0+0.2*HMGetOption(SCALE_TICKMARKS),yneu(yy));
	else	    fprintf(HMfp,"%5.3f %5.3f l s\n", 
			    x0+0.10*HMGetOption(SCALE_TICKMARKS), yneu(yy));
      }
      if(i%labi == 0)
      { fprintf(HMfp,"1 ");
	fprintf(HMfp,format, yy/pow(10,exponent));
	fprintf(HMfp," %5.3f %5.3f text\n", x0-HMGetOption(AXI_X), 
		yneu(yy)-(yy-miny)/(maxy-miny)*HMGetOption(FONTSIZE_AXIS) 
		* 0.66);
      }
      yy+= ddy/10.0;
    }
  }
  
  fprintf(HMfp,"label\n");
  setColor((int) HMGetOption(COLOR_LABEL));
  fprintf(HMfp,"gs %5.3f %5.3f ", 
	  x0-HMGetOption(LAB_X)+HMGetOption(FONTSIZE_LABEL), 
          oy0 + hoehe/2.0);
  fprintf(HMfp,"translate 90 rotate 0.5 %s 0 0 text gr\n", myylab);
  fprintf(HMfp,"0.5 %s %5.3f %5.3f text 0 setgray\n", myxlab, 
          x0 + breite/2.0, oy0-HMGetOption(LAB_Y));
}

static void 
tickmarks(const char *xlab, const char *ylab,
	  const char *xunit, const char *yunit)
{
  int i,labi,exponent,i5,i10;
  char myxlab[1000],myylab[1000],format[1000];
  FLOAT ddx;
  
  scale_axis(myxlab,xlab,xunit,format,&exponent,
	     &i,&labi,&i5,&i10,&xx,&ddx,maxx,minx,
	     HMGetOption(ON_SCALE_X), HMGetOption(FORMAT_AXIS_X),
	     HMGetOption(LOG_SCALE_X) );
  
  /**FAKE LOG-X-SCALE (LOG(X) HAS TO BE HISTOGRAMMED BEFORE EXPLICITLY)**/
  if (HMGetOption(LOG_SCALE_X) == 1.0) { 
    FLOAT lxstart, lxstop;
    FLOAT ii,j;
    lxstart = floor(minx); lxstop  = ceil(maxx);  
    for(ii = lxstart; ii<=lxstop;ii++) { 
      for (j=pow(10,ii); j< 10 * pow(10,ii); j+=pow(10,ii)) {
	FLOAT jj = log10(j);
	if ( minx<=jj && jj<=maxx )
	  fprintf(HMfp,"n %5.3f %5.3f m 0 %f rlineto s\n", 
		  xneu(jj), oy0, 0.2*HMGetOption(SCALE_TICKMARKS));
      }
    }
  } 
  else {
    for (;xx<=maxx+(maxx-minx)*0.00001;i++)  {
      if(xx>minx+ddx/20.0 && xx<maxx-ddx/20.0) {
	fprintf(HMfp,"n %5.3f %5.3f m ",  xneu(xx),     oy0);
	if(i%i10 == 0) fprintf(HMfp,"%5.3f %5.3f l s\n", xneu(xx), 
			       oy0+0.30*HMGetOption(SCALE_TICKMARKS));
	else if(i%i5==0) fprintf(HMfp,"%5.3f %5.3f l s\n", 
				 xneu(xx), 
				 oy0+0.2*HMGetOption(SCALE_TICKMARKS));
	else	   fprintf(HMfp,"%5.3f %5.3f l s\n", 
			   xneu(xx), oy0+0.1*HMGetOption(SCALE_TICKMARKS));
      }
      xx += ddx/10.0;
    }
  }
  
  scale_axis(myylab,ylab,yunit,format,&exponent,&i,&labi,
	     &i5, &i10,&yy,&ddy,
	     maxy,miny,HMGetOption(ON_SCALE_Y), HMGetOption(FORMAT_AXIS_Y),
	     HMGetOption(LOG_SCALE_Y) );
  if (HMGetOption(LOG_SCALE_Y) == 1.0) { 
    FLOAT lxstart, lxstop, lminy, lmaxy;
    FLOAT ii,j;
    //    int from_atom_to_galaxy = 0;
    if (miny<=0.0) miny = 0.001;
    if (maxy<miny) maxy = miny*100;
    lminy = log10(miny);
    lmaxy = log10(maxy);
    //    if (lmaxy-lminy>11) from_atom_to_galaxy = 1;
    lxstart = floor(lminy); lxstop  = ceil(lmaxy);  
    for(ii = lxstart; ii<=lxstop;ii++) {
      FLOAT ee = pow(10,ii);
      for (j=ee; j< 10*ee; j+=ee)
	if (miny<=j && j<=maxy) {
	  if (j==ee)
	    fprintf(HMfp,"n %5.3f %5.3f m %f 0 rlineto s\n", x0, yneu(j),
		    0.2 * HMGetOption(SCALE_TICKMARKS));
	  else
	    fprintf(HMfp,"n %5.3f %5.3f m %f 0 rlineto s\n", x0, yneu(j),
		    0.1 * HMGetOption(SCALE_TICKMARKS));
	}
    }
  } 
  else if (HMGetOption(LOG_SCALE_Y) != 2.0) {
    for (; yy<=maxy+(maxy-miny)*0.00001; i++)
      { if(yy>miny+ddy/20.0 && yy<maxy-ddy/20.0)
	  { fprintf(HMfp,"n %5.3f %5.3f m ",  x0,     yneu(yy));
	    if(i%i10 == 0)  fprintf(HMfp,"%5.3f %5.3f l s\n",
				    x0+0.20* HMGetOption(SCALE_TICKMARKS),
				    yneu(yy));
	    else if(i%i5==0)fprintf(HMfp,"%5.3f %5.3f l s\n",
				    x0+0.12* HMGetOption(SCALE_TICKMARKS),
				    yneu(yy));
	    else	    fprintf(HMfp,"%5.3f %5.3f l s\n", 
				    x0+0.06* HMGetOption(SCALE_TICKMARKS),
				    yneu(yy));
	  }
	yy+= ddy/10.0;
      }
  }
}
/*---------------------------------------------------------------------------*/
void
HMExplain(const char *text) { HMExplainOffset(text,0,0);}

void
HMExplainOffset(const char *text, double ofx, double ofy)
{ char expl[1000];
  txtconvert(text, expl);
  fprintf(HMfp,"gs %s",pattern);
  setColor((int) HMGetOption(LINE_COLOR));
  fprintf(HMfp,"n %lf %lf m 0.5 0 r s gr\n" ,
          x0 +0.7+ofx, 
          ofy+oy0+hoehe-(HMFrameNr + 1.5 - 0.5)*HMGetOption(FONTSIZE_COMMENT)*1.2);
  fprintf(HMfp,"comment 0 %s %lf %lf text\n", expl,
          x0 +ofx+1.4, 
          ofy+oy0+hoehe-(HMFrameNr + 1.5) * HMGetOption(FONTSIZE_COMMENT)*1.2);
}
/*---------------------------------------------------------------------------*/
void
HMComment(const char *text, int line)
{ char expl[1000];
  txtconvert(text,expl);
  fprintf(HMfp,"comment 0 %s %lf %lf text\n", expl,
	  x0 +0.7, oy0+hoehe-(line + 1.5) * HMGetOption(FONTSIZE_COMMENT)*1.2);
}

void
HMLine(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2)
{
  fprintf(HMfp,"gs %s",pattern);
  setColor((int) HMGetOption(LINE_COLOR));
  fprintf(HMfp,"%lf %lf %lf %lf m l s\n", xneu(x1), yneu(y1), xneu(x2), yneu(y2));
  fprintf(HMfp,"gr\n");
}

void
HMRectangle(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2)
{
  fprintf(HMfp,"gs\n");
  setColor((int) HMGetOption(LINE_COLOR));
  fprintf(HMfp,"%lf %lf m ",      xneu(x1), yneu(y1));
  fprintf(HMfp,"%lf %lf l ",      xneu(x1), yneu(y2));
  fprintf(HMfp,"%lf %lf l ",      xneu(x2), yneu(y2));
  fprintf(HMfp,"%lf %lf l c s\n", xneu(x2), yneu(y1));
  fprintf(HMfp,"gr\n");
}

void
HMText(FLOAT x, FLOAT y, const char *text)
{
  char expl[1000];
  setColor((int) HMGetOption(COLOR_LABEL));
  txtconvert(text,expl);
  fprintf(HMfp,"comment gsave %lf %lf translate"
	  " %lf rotate 0 %s 0 0 text grestore\n", 
	  xneu(x),yneu(y),HMGetOption(TEXT_ANGLE), expl);
  fprintf(HMfp,"0 setgray\n");
}

void
HMGlobalText(FLOAT x, FLOAT y, FLOAT angle, FLOAT align ,const char *text)
{
  char expl[1000];
  setColor((int) HMGetOption(COLOR_LABEL));
  txtconvert(text,expl);
  fprintf(HMfp,"comment gsave %lf %lf translate"
	  " %lf rotate %lf %s 0 0 text grestore\n",x,y,angle,align,expl);
  fprintf(HMfp,"0 setgray\n");
}
/*---------------------------------------------------------------------------*/
static char *oldxlab  = NULL;
static char *oldylab  = NULL;
static char *oldxunit = NULL;
static char *oldyunit = NULL;

static FLOAT *statError = NULL;

void HMSetStatError(FLOAT *err){ statError = err; };

void
HMPlotError(int nx, FLOAT *x, FLOAT *y, FLOAT *xerr, FLOAT *yerr,
		 const char *title, const char *xlab, const char *ylab,
		 const char *xunit, const char *yunit)
{
  int	i;
  
  if(!same_frame) {
    minx=x[0]; miny=y[0]; 
    maxx=x[0]; maxy=y[0];
    
    for(i=0;i<nx;i++) {
      xx = x[i]; if(xerr) xx -= xerr[i]; if (xx<minx) minx = xx;  
      xx = x[i]; if(xerr) xx += xerr[i]; if (xx>maxx) maxx = xx;  
      yy = y[i]; if(yerr) yy -= yerr[i]; if (yy<miny) miny = yy;  
      yy = y[i]; if(yerr) yy += yerr[i]; if (yy>maxy) maxy = yy;  
    }          
    
    if (HMGetOption(LOG_SCALE_Y)) {
      miny = maxy;
      for(i=0; i<nx; i++)
	if (y[i] > 0 && y[i] < miny) miny = y[i];  
      miny = pow(10,floor(log10(miny)));
      maxy = pow(10, ceil(log10(maxy)));
      if (HMminimumy == NOMINIMUM) HMminimumy = miny;
    }

    free(oldxlab); free(oldylab); free(oldxunit); free(oldyunit);
    oldxlab  = (char *) calloc(sizeof(char), strlen(xlab)+1);
    oldylab  = (char *) calloc(sizeof(char), strlen(ylab)+1);
    oldxunit = (char *) calloc(sizeof(char), strlen(xunit)+1);
    oldyunit = (char *) calloc(sizeof(char), strlen(yunit)+1);
    strcpy(oldxlab,xlab);
    strcpy(oldylab,ylab);
    strcpy(oldxunit,xunit);
    strcpy(oldyunit,yunit);
    scale_figure(title);
    fprintf(HMfp,"0 setgray\n");
    plot_axis(oldxlab,oldylab,oldxunit,oldyunit);
    frame();
  }
  else {
    scale_figure(title);
    next_frame();
  }

  setColor((int) HMGetOption(LINE_COLOR));
  
  fprintf(HMfp, "/marker {%5.3f} def\n", HMGetOption(DATA_SIZE));
  for(i=0; i<nx; i++) {
   fprintf(HMfp,"gsave %lf setlinewidth\n", HMGetOption(LINE_WIDTH));
   if(xerr) {
      fprintf(HMfp,"n %5.3f %5.3f m ",  xneu(x[i]-xerr[i]),yneu(y[i]));
      fprintf(HMfp,"%5.3f %5.3f l s\n", xneu(x[i]+xerr[i]),yneu(y[i]));
    }
    if(yerr) {
      fprintf(HMfp,"n %5.3f %5.3f m ",  xneu(x[i]), yneu(y[i]-yerr[i]));
      fprintf(HMfp,"%5.3f %5.3f l s\n", xneu(x[i]), yneu(y[i]+yerr[i]));
    }
    if(statError) {
      fprintf(HMfp,"n %5.3f %5.3f m ",  xneu(x[i])-HMGetOption(DATA_SIZE), 
	                                yneu(y[i]-statError[i]));
      fprintf(HMfp,"%5.3f 0 rlineto s\n", HMGetOption(DATA_SIZE)*2);
      fprintf(HMfp,"n %5.3f %5.3f m ",  xneu(x[i])-HMGetOption(DATA_SIZE), 
	                                yneu(y[i]+statError[i]));
      fprintf(HMfp,"%5.3f 0 rlineto s\n", HMGetOption(DATA_SIZE)*2);
    }
    fprintf(HMfp,"grestore %5.3f %5.3f d%.0f\n",  xneu(x[i]),   yneu(y[i]), 
	    HMGetOption(DATA_MARK));	
  }
  fprintf(HMfp,"0 setgray\n");
  same_frame=0;
}
/*--------------------------------------------------------------------------*/
void 
HMPlotFunction(int nx, FLOAT *x, FLOAT *y, 
	       const char *title, const char *xlab, const char *ylab,
	       const char *xunit, const char *yunit)
{ 
  int	i;
  
  if (!same_frame) {
    minx = maxx = x[0]; 
    maxy = miny = y[0];
    
    setdash("1 0");                                  
    for(i=0; i<nx; i++) {
      if (x[i] < minx) minx = x[i];  
      if (x[i] > maxx) maxx = x[i];  
      if (y[i] > maxy) maxy = y[i];  
      if (y[i] < miny) miny = y[i];  
    }
    
    if (HMGetOption(LOG_SCALE_Y)) {
      miny = maxy;
      for(i=0; i<nx; i++)
	if (y[i] > 0 && y[i] < miny) miny = y[i];  
      miny = pow(10,floor(log10(miny)));
      maxy = pow(10, ceil(log10(maxy)));
      if (HMminimumy == NOMINIMUM) HMminimumy = miny;
    }
    
    /*    free(oldxlab); free(oldylab); free(oldxunit); free(oldyunit); */
    oldxlab  = (char *) calloc(sizeof(char), strlen(xlab)+1);
    oldylab  = (char *) calloc(sizeof(char), strlen(ylab)+1);
    oldxunit = (char *) calloc(sizeof(char), strlen(xunit)+1);
    oldyunit = (char *) calloc(sizeof(char), strlen(yunit)+1);
    strcpy(oldxlab,xlab);
    strcpy(oldylab,ylab);
    strcpy(oldxunit,xunit);
    strcpy(oldyunit,yunit);
    scale_figure(title);
    frame();
    plot_axis(oldxlab, oldylab, oldxunit, oldyunit);
    HMFrameNr=0;
  } else {
    switch (HMFrameNr) {
    case 1:  setdash("0.1 0.1"); break;
    case 2:  setdash("0.095 0.09 0.02 0.09"); break;
    case 3:  setdash("0.01 0.1"); break;
    case 4:  setdash("0.12 0.1 0.01 0.05 0.01 0.1"); break;
    default: 
      setdash("1 0"); break;
      HMFrameNr = 0;
    }
    fprintf(HMfp,"%s",pattern);
  }
  
  if (HMGetOption(LINE_STYLE)!=-1) {
    switch ((int) HMGetOption(LINE_STYLE)) {
    case 1:  setdash("0.1 0.1"); break;
    case 2:  setdash("0.095 0.09 0.02 0.09"); break;
    case 3:  setdash("0.01 0.1"); break;
    case 4:  setdash("0.12 0.1 0.01 0.05 0.01 0.1"); break;
    default: setdash("1 0"); break;
    }
    fprintf(HMfp,"%s",pattern);
  }

  fprintf(HMfp,"gsave n %5.3f %5.3f m ",           xneu(minx), yneu(miny));
  fprintf(HMfp,        "%5.3f %5.3f l ",           xneu(maxx), yneu(miny));
  fprintf(HMfp,        "%5.3f %5.3f l ",           xneu(maxx), yneu(maxy));
  fprintf(HMfp,        "%5.3f %5.3f l c clip s\n", xneu(minx), yneu(maxy));
  fprintf(HMfp," %lf setlinewidth\n", HMGetOption(LINE_WIDTH));
  setColor((int) HMGetOption(LINE_COLOR));
  fprintf(HMfp,"n %5.3f %5.3f m ", xneu(x[0]),yneu(y[0]));
  for (i=1; i<nx; i++) {
    if (!(i % 1000) && HMGetOption(FILL_COLOR)==0xFFFFFF) {
      /* do a stroke every 1000 lineto's to work around memory
	 limitations of old postscript printers. For filling we need
	 the full path! */
      fprintf(HMfp, "%5.3f %5.3f l s n %5.3f %5.3f m\n",
	      xneu(x[i]), yneu(y[i]), xneu(x[i]), yneu(y[i]));
    } else {
      fprintf(HMfp, "%5.3f %5.3f l\n", xneu(x[i]), yneu(y[i]));
    }
  }
  if (HMGetOption(FILL_COLOR) != 0xFFFFFF) {
    fprintf(HMfp, "gsave closepath\n");
    setColor((int) HMGetOption(FILL_COLOR));
    fprintf(HMfp, "fill grestore\n");
  }
  fprintf(HMfp,"s grestore \n[1 0] 0 setdash\n");
  if (HMGetOption(FILL_COLOR) != 0xFFFFFF &&
      (HMGetOption(DELAY_TICKS)==0 || HMGetOption(DELAY_TICKS)==HMFrameNr)
      ) 
    tickmarks(oldxlab, oldylab, oldxunit, oldyunit);
  
  fprintf(HMfp,"n %5.3f %5.3f m ",    xneu(minx),yneu(miny));
  fprintf(HMfp,"%5.3f %5.3f l ",      xneu(minx),yneu(maxy));
  fprintf(HMfp,"%5.3f %5.3f l ",      xneu(maxx),yneu(maxy));
  fprintf(HMfp,"%5.3f %5.3f l c s\n",xneu(maxx),yneu(miny));

  same_frame=0;
}
/*---------------------------------------------------------------------------*/
void
HMMarkFunction(int where,  FLOAT *x, FLOAT *y, const char *mark)
{ char mm[1000];
  FLOAT 
    x1=xneu(x[where]),
    x2=xneu(x[where+1]),
    y1=yneu(y[where]),
    y2=yneu(y[where+1]),
    angle = atan2(y2-y1, x2-x1) / 3.1415926535 * 180;
  if (angle>90) angle -= 180;
  if (angle<-90) angle += 180;
  txtconvert(mark,mm);
  fprintf(HMfp,"gsave\n");
  setColor((int) HMGetOption(LINE_COLOR));
  fprintf(HMfp,"%lf %lf translate\n",(x1+x2)/2,(y1+y2)/2);
  fprintf(HMfp,"%lf rotate\n",angle);
  fprintf(HMfp,"comment 0.5 %s 0 0 marktext\n",mm);
  fprintf(HMfp,"grestore\n");
}
/*---------------------------------------------------------------------------*/
FILE *
HMPipe()
{ 
if (HMGetOption(NO_OPEN) != 1.0)
  { if (HMGetOption(USE_GHOSTVIEW)==1.0)
    HMfp = popen(GHOSTVIEW" - >/dev/null 2>&1","w");
   
  else
    if (HMGetOption(TIGHT_GS))
    { char command[255];
         sprintf(command, GHOSTSCRIPT
	     " -q -NOPLATFONTS -r%.0f -g%.0fx%.0f - >/dev/null 2>&1",
	      HMGetOption(GS_RESOLUTION),
	      (HMGetOption(SIZE_X)+2*HMGetOption(MRG_LEFT))/2.54*HMGetOption(GS_RESOLUTION), 
	      (HMGetOption(SIZE_Y)+2*HMGetOption(MRG_LOW) )/2.54*HMGetOption(GS_RESOLUTION));
      HMfp = popen(command,"w");
    }
    else
       HMfp = popen(GHOSTSCRIPT" -q - >/dev/null 2>&1","w");
      printf("gs\n");
    if(!HMfp) 
    { fprintf(stderr, "Can't open ghostscript pipe!\n");
      exit(0);
    }
  }
  ispipe = 1;
  HMminimumx = HMminimumy = HMminimumz = NOMINIMUM;
  HMmaximumx = HMmaximumy = HMmaximumz = NOMINIMUM;

  fprintf(HMfp,"%%!PS-Adobe-2.0 EPSF-2.0\n");
  fprintf(HMfp,"%%%%Creator: HMBook 1.02, by Harald Merkel (1994)\n");
  fpboundingbox = fppages = 0; /* No seek possible! */
  fprintf(HMfp,"%%%%BoundingBox: %7.3f %7.3f %7.3f %7.3f\n", 
	  HMGetOption(MRG_LEFT)/2.54*72, HMGetOption(MRG_LOW)/2.54*72, 
	  (HMGetOption(SIZE_X)+HMGetOption(MRG_LEFT))/2.54*72, 
	  (HMGetOption(SIZE_Y)+HMGetOption(MRG_LOW))/2.54*72);
  fprintf(HMfp,"%%%%Pages:   1   1\n");
  fprintf(HMfp,
#include	"hmplot_prolog.h"
	  );
  fprintf(HMfp,"/axis     {/fs %lf def (Helvetica) font} def\n",
	  HMGetOption(FONTSIZE_AXIS));
  fprintf(HMfp,"/label    {/fs %lf def (Helvetica) font} def\n",
	  HMGetOption(FONTSIZE_LABEL));
  fprintf(HMfp,"/comment  {/fs %lf def (Helvetica) font} def\n",
	  HMGetOption(FONTSIZE_COMMENT));
  fprintf(HMfp,"/wintitle {/fs %lf def (Helvetica) font} def\n",
	  HMGetOption(FONTSIZE_WINTITLE));
  fprintf(HMfp,"/title    {/fs %lf def (Helvetica) font} def\n",
	  HMGetOption(FONTSIZE_TITLE));
  fprintf(HMfp,"%%%%EndProlog\n%%%%Page: 1 1\n");
  fprintf(HMfp,"72 2.54 div dup scale\n");
  fprintf(HMfp,"0.02 setlinewidth\n");
  return HMfp;
}
/*---------------------------------------------------------------------------*/
void
HMFilePointer(FILE *fp_new)
{
  HMfp=fp_new;
  ispipe=1;
  nr_x = nr_y = 1;
  nr_next_x = 0;
  nr_next_y = 1;
  seite = 1;   
  HMSetOption(NO_OPEN, 1.0);
}
/*---------------------------------------------------------------------------*/

static const char *fname;

void
HMPlotFile(const char *filename)
{
  HMfp = fopen(fname = filename,"w");
  if(!HMfp) {
    printf("Can't open \"%s\", I try to write /tmp/HMPlot.eps\n",filename);
    HMfp = fopen("/tmp/HMPlot.eps","w");
    if(!HMfp) exit(0);
  }
  ispipe = 0;
  nr_x = 1;
  nr_y = 1;
  nr_next_x = 0;
  nr_next_y = 1;
  seite = 1;   
  
  HMminimumx = HMminimumy = HMminimumz = NOMINIMUM;
  HMmaximumx = HMmaximumy = HMmaximumz = NOMINIMUM;
  
  if (strstr(filename, ".eps") || strstr(filename, ".EPS")) 
    fprintf(HMfp,"%%!PS-Adobe-2.0 EPSF-2.0\n");
  else
    fprintf(HMfp,"%%!PS-Adobe-2.0\n");
  
  fprintf(HMfp,"%%%%Creator: HMBook 1.01, by Harald Merkel (1994)\n");
  fpboundingbox = ftell(HMfp);
  fprintf(HMfp,"%%%%BoundingBox: %7.3f %7.3f %7.3f %7.3f\n", 
	  HMGetOption(MRG_LEFT)/2.54*72-10, HMGetOption(MRG_LOW)/2.54*72-10, 
	  (HMGetOption(SIZE_X)+HMGetOption(MRG_LEFT))/2.54*72+20, 
	  (HMGetOption(SIZE_Y)+HMGetOption(MRG_LOW))/2.54*72+20);
  fppages = ftell(HMfp);
  fprintf(HMfp,"%%%%Pages:   1   1\n");
  fprintf(HMfp,
#include	"hmplot_prolog.h"
	  );
  fprintf(HMfp,"/axis     {/fs %lf def (Helvetica) font} def\n",
	  HMGetOption(FONTSIZE_AXIS));
  fprintf(HMfp,"/label    {/fs %lf def (Helvetica) font} def\n",
	  HMGetOption(FONTSIZE_LABEL));
  fprintf(HMfp,"/comment  {/fs %lf def (Helvetica) font} def\n",
	  HMGetOption(FONTSIZE_COMMENT));
  fprintf(HMfp,"/wintitle {/fs %lf def (Helvetica) font} def\n",
	  HMGetOption(FONTSIZE_WINTITLE));
  fprintf(HMfp,"/title    {/fs %lf def (Helvetica) font} def\n",
	  HMGetOption(FONTSIZE_TITLE));
  fprintf(HMfp,"%%%%EndProlog\n%%%%Page: 1 1\n");
  fprintf(HMfp,"72 2.54 div dup scale\n");
  fprintf(HMfp,"0.02 setlinewidth\n");
}
/*---------------------------------------------------------------------------*/
void
HMFlush() 
{
  if (!HMfp) return;
  /*  if (ispipe) 
      fprintf(HMfp,"copypage\n"); */
  fflush(HMfp);
}
/*---------------------------------------------------------------------------*/
void
HMClose()
{ 
  if (!HMfp) return;
  fprintf(HMfp,"showpage\n%%%%Trailer\n%%%%EOF\n");
  if (ispipe)
    pclose(HMfp);
  else {
    if (fseek(HMfp, fpboundingbox,SEEK_SET)) perror("Writing Bounding Box");
    fprintf(HMfp,"%%%%BoundingBox: %7.3f %7.3f %7.3f %7.3f\n", 
	    HMGetOption(MRG_LEFT)/2.54*72-10, HMGetOption(MRG_LOW)/2.54*72-10, 
	    (HMGetOption(SIZE_X)+HMGetOption(MRG_LEFT))/2.54*72+20, 
	    (HMGetOption(SIZE_Y)+HMGetOption(MRG_LOW))/2.54*72+20);
    fclose(HMfp);

    if (HMGetOption(TIGHT)==1) { char argv[1024],dummy[1024];
      FILE *fp;
      double x,y,w,h;
      sprintf(argv,"%s -dQUIET -dBATCH -dNOPAUSE -sDEVICE=bbox %s 2>&1",
	      GHOSTSCRIPT, fname);
      fp = popen(argv,"r");
      if (EOF==fscanf(fp,"%s%lf%lf%lf%lf%*s%*s%*s%*s%*s",dummy,&x,&y,&w,&h))
	perror(argv);

      HMfp = fopen(fname,"r+");
      if (fseek(HMfp, fpboundingbox,SEEK_SET)) perror("Seeking Bounding Box");
      fprintf(HMfp,"%%%%BoundingBox: %7.3f %7.3f %7.3f %7.3f\n", x,y,w,h);
      fclose(HMfp);
    }
  }
  HMfp = NULL;
  first_win = -1;
}
/*---------------------------------------------------------------------------*/
void
HMPlotBox(int nx, int ny, FLOAT xx0, FLOAT yy0, FLOAT dx, FLOAT dy,
	  FLOAT *z, 
	  const char *title, const char *xlab, const char *ylab,
	  const char *xunit, const char *yunit, const char type)
{ int i, j, newpath=1;
  FLOAT maxi=z[0],mini=z[0],error=0;
  
  maxx = HMmaximumx;	minx = HMminimumx;
  maxy = HMmaximumy;	miny = HMminimumy;
  
  if (type=='s' && mini<0) 
  { fprintf(stderr,"Histogram not suitable for Scatterplots\n");
    return;
  }
  
  scale_figure(title);
  if (type=='c') breite -= HMGetOption(LAB_Z);

  for(i=0;i<nx;i++)
    for(j=0;j<ny;j++)
    { if(z[i*ny+j]>maxi) maxi = z[i*ny+j];
      if(z[i*ny+j]<mini) mini = z[i*ny+j];
    }
  
  frame();
  
  fprintf(HMfp,"/dx %5.3f def /dy %5.3f def\n",
	  breite/(maxx - minx) * dx,hoehe /(maxy - miny) * dy);
  if (HMGetOption(FILL_COLOR)!=0xFFFFFF) {
      double h = ((int) HMGetOption(FILL_COLOR) >>  8 & 0xFF)/255.0;
      double s = ((int) HMGetOption(FILL_COLOR)       & 0xFF)/255.0;
      fprintf(HMfp,"/hueSaturation {%5.3f %5.3f} def\n",h,s);
  } else 
    fprintf(HMfp,"/hueSaturation {1 0} def\n");
  if (HMmaximumz != NOMINIMUM) maxi = HMmaximumz;
  if (HMminimumz != NOMINIMUM) mini = HMminimumz;
  fprintf(HMfp,"gs\n");
  if (maxi>mini) {
    for(i=0;i<nx;i++)
      for(j=0;j<ny;j++)
	if (z[i*ny+j]>mini || HMGetOption(FILL_ZERO)) {
	  if (type!='x' && type!='y') {
	    fprintf(HMfp,"%5.3f ", xneu(xx0) + breite/(maxx - minx) * dx * i);
	    fprintf(HMfp,"%5.3f ", yneu(yy0) + hoehe /(maxy - miny) * dy * j);
	  }
	  switch (type)
	  { case 'b': fprintf(HMfp,"%4.2f box\n",
			      (z[i*ny+j]-mini)/(maxi-mini)*0.9);
	      break;
	    case 'g': fprintf(HMfp,"%4.2f gry\n",
			      MAX(0,(z[i*ny+j]-mini)/(maxi-mini)*0.9));
	              break;
	    case 'c': fprintf(HMfp,"%4.2f clrbox\n",
			      MAX(0,(z[i*ny+j]-mini)/(maxi-mini)));
	              break;
	    case 's': fprintf(HMfp,"%d sct\n", (int) (z[i*ny+j] + error));
	              error = z[i*ny+j] + error - (int) (z[i*ny+j] + error);
	  }
	}
    if (type=='x') {
      for(j=0; j<ny; j++) {
	double mean=0, square=0, weight=0, rms, 
	  sy = yneu(yy0) + hoehe /(maxy-miny) * dy * (j+0.5);
	for(i=0;i<nx;i++) {
          double thisx = xneu(xx0) + breite/(maxx - minx) * dx * (i+0.5);
	  mean   += thisx         * z[i*ny+j];
	  square += thisx * thisx * z[i*ny+j];
	  weight += z[i*ny+j];
	}
        if (weight>0) {
	  mean /= weight;
	  if (square/weight - mean * mean>0)
	    rms = sqrt(square/weight - mean * mean);
	  else rms=0;
	  
	  if (HMGetOption(DATA_MARK)==0) {
	    fprintf(HMfp," %lf setlinewidth\n", HMGetOption(LINE_WIDTH));
	    setColor((int) HMGetOption(LINE_COLOR));
	    fprintf(HMfp,"%5.3f %5.3f %c\n",  mean, sy, i==0?'m':'l');	
	  } else {
	    fprintf(HMfp,"n %5.3f %5.3f m ",  mean - rms, sy);
	    fprintf(HMfp,"%5.3f %5.3f l s\n", mean + rms, sy);
	    fprintf(HMfp,"%5.3f %5.3f d1\n",  mean,       sy);
	  }
	}
      }
    }
    if (type=='y') {
      for(i=0; i<nx; i++) {
	double mean=0, square=0, weight=0, rms, 
	  sx = xneu(xx0) + breite /(maxx-minx) * dx * (i+0.5);
	for(j=0;j<ny;j++) {
          double thisy = yneu(yy0) + hoehe /(maxy - miny) * dy * (j+0.5);
	  mean   += thisy         * z[i*ny+j];
	  square += thisy * thisy * z[i*ny+j];
	  weight += z[i*ny+j];
	}
        if (weight>0) {
	  mean /= weight;
	  if (square/weight - mean * mean>0)
	    rms = sqrt(square/weight - mean * mean);
	  else rms=0;
	  if (HMGetOption(DATA_MARK)==0) {
	    fprintf(HMfp," %lf setlinewidth\n", HMGetOption(LINE_WIDTH));
	    if (i==0) setColor((int) HMGetOption(LINE_COLOR));
	    fprintf(HMfp,"%5.3f %5.3f %c\n",  sx, mean, newpath?'m':'l');
	    newpath=0;
	  } else {
	    fprintf(HMfp,"n %5.3f %5.3f m ",  sx, mean - rms);
	    fprintf(HMfp,"%5.3f %5.3f l s\n", sx, mean + rms);
	    fprintf(HMfp,"%5.3f %5.3f d1\n",  sx, mean);	
	  }
	}
      }
      if (HMGetOption(DATA_MARK)==0) fprintf(HMfp," s\n");
    }
  }
  fprintf(HMfp,"gr\n");
  frame(); 
  fprintf(HMfp,"gr\n");
  setColor(0);
  plot_axis(xlab, ylab, xunit, yunit);

  if (type=='c' && HMGetOption(LAB_Z) !=0) { 
    int i,labi,exponent,i5,i10;
    char mylab[1000],format[1000];
    FLOAT dd,zz,z;
    
    fprintf(HMfp,"axis\n");
    
    scale_axis(mylab,"","",format,&exponent,
	       &i,&labi,&i5,&i10,&zz,&dd,maxi,mini, 0,0,0);
    
    fprintf(HMfp,"/dx %5.3f def /dy %5.3f def\n",0.3, hoehe *0.01);
	    
    for (z=0;z<1;z+=0.01) {
      fprintf(HMfp,"%5.3f ", xneu(xx0) + breite + 0.2);
      fprintf(HMfp,"%5.3f ", yneu(yy0) + hoehe * z);
      fprintf(HMfp,"%4.2f clrbox\n",z);
    }
    fprintf(HMfp,"%5.3f %5.3f m dx 0 rlineto "
	    "0 %f rlineto dx neg 0 rlineto closepath stroke\n",
	    xneu(xx0) + breite + 0.2, yneu(yy0), hoehe);
    for (;zz<=maxi+(maxi-mini)*0.00001;i++)  {
      if(i%labi == 0){ 
	fprintf(HMfp, "%5.3f %4.3f m dx 0 rlineto s\n", 
		xneu(xx0) + breite + 0.2,
		yneu(yy0) + hoehe *(zz-mini)/(maxi-mini));
	fprintf(HMfp, "-1");
	fprintf(HMfp, format,   	      zz/pow(10,exponent));
	fprintf(HMfp, "%5.3f %4.2f text\n", xneu(xx0) + breite + 0.1,
		yneu(yy0) + hoehe *(zz-mini)/(maxi-mini));
      }
      zz += dd/10.0;
    }
  }

  same_frame = 0;
}

void
HMIncludePS (const char *filename, double x, double y, double zoom)
{ 
  FLOAT bx1=0, by1=0, bx2, by2;
  FILE *read = fopen(filename, "r");
  if (!read) { 
    fprintf(stderr,"Can't open %s!\n", filename); 
    perror("READ");
    return; 
  }

  do {
    char command[255];
    if (fscanf(read, "%s",command)){};
    if (!strcmp(command,"%%BoundingBox:")) {
      if (fscanf(read, "%lf%lf%lf%lf", &bx1, &by1, &bx2, &by2)){};
      break;
    }
  } while (!feof(read));
  if (feof(read)) fprintf(stderr,"No Bounding Box found!\n");
  rewind(read);

  fprintf(HMfp,"%%Figure: %s\ngsave save\n",filename);
  fprintf(HMfp,"/showpage {} def /copypage {} def /erasepage {} def\n");
  fprintf(HMfp,"%lf %lf translate\n", x, y);
  fprintf(HMfp,"2.54 72 div dup scale\n");
  fprintf(HMfp,"%lf dup scale\n",zoom);
  fprintf(HMfp,"%lf %lf translate\n", -bx1, -by1);

  while(!feof(read)) {
    char buffer[255];
    int got = fread(buffer, sizeof(char), 255, read);
    if (got) if(fwrite(buffer, sizeof(char), got, HMfp)){};
  }
  fprintf(HMfp,"\n restore grestore\n");
  fprintf(HMfp,"%%End of Figure: %s\n",filename);
  fclose(read);
}

#include "hmplot3d.c"


