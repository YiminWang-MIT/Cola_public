/* C-Programm data2ps:  Erzeugen eines Postscript-Bildes aus einem Datenfile.

     V 0.5    by T.Pospischil, 19.12.95
     V 0.96   by T.Pospischil, 02.06.97
     V 0.97   by T.Pospischil, 16.07.97 (Import von anderen PS-Dateien)
     V 0.98   by T.Pospischil, 24.07.97 (Farb. Symbols/Lines/Zusatzsymbol)
     V 0.99   by T.Pospischil, 27.08.97 (leaveplace_y for autoscaling)
     V 0.991  by T.Pospischil, 24.11.97 (ctext)
     V 0.992  by T.Pospischil, 10.12.97 (box, line, circle)
     V 0.993  by T.Pospischil, ...      (debugging)
     V 0.994  by T.Pospischil, 21.01.98 (addarrows)
     V 0.995  by T.Pospischil, 26.01.98 (fbox, fline, fcircle)
     V 0.996  by T.Pospischil, 04.03.98 (addhisto)
     V 0.996a by T.Pospischil, 15.05.98 (auto.-best von x+ybinsize bei 2D)
     V 0.997  by T.Pospischil, 25.09.98 (color2D, ab hier: tasty-Probleme)
     V 0.997a by T.Pospischil, 12.11.98 (debug: min/max with noframe)
     V 0.998  by T.Pospischil, 19.11.98 (triang, ftriang, arc, farc)
     V 0.999  by T.Pospischil, 19.01.99 (psplot -> c++, wird class)
     V 1.0    by T.Pospischil, 21.01.99 (verschied. Symbol-Typen)
     V 1.01   by T.Pospischil, 22.02.99 (scale_xtics, scale_ytics)
     V 1.02   by T.Pospischil, 10.03.99 (empty boxes,.. with color ------)
     V 1.03   by T.Pospischil, 16.03.99 (import binaer codierter eps)
     V 1.04   by T.Pospischil, 21.04.99 Vektorpfeile ueber Buchstaben
     V 1.05   by T.Pospischil, 27.04.99 specify Bounding Box in cm
     V 1.06   by T.Pospischil, 14.05.99 (polygon, fpolygon)
     V 1.07   by T.Pospischil, 13.07.99 (xtitledist, ytitledist)
     V 1.08   by T.Pospischil, 23.07.99 (pline, fpline)
     V 1.09   by T.Pospischil, 29.07.99 (xticstyle, yticstyle)
     V 1.10   by T.Pospischil, 16.08.99 (dotsize)
     V 1.11   by T.Pospischil, 17.08.99 (importclip)
     V 1.12   by T.Pospischil, 18.08.99 (curve mit 3 Punkten)
     V 1.13   by T.Pospischil, 08.09.99 (curve mit 4 statt 3 Punkten!)
     V 1.14   by T.Pospischil, 12.11.99 (zlog in 2D-Histgrammen)
     V 1.15   by T.Pospischil, 15.11.99 (fkttextfont)
     V 1.16   by T.Pospischil, 18.01.00 (fimportclip)
     V 1.17   by T.Pospischil,  4.11.00 ({i..}, {B..} und {I..} in Texten)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <math.h>

#define __no_tasty__
#ifndef __no_tasty__
#include "hcs/HcsHisto.h"
#include "hcs/HcsField.h"
#endif

#include "psclopt.h"
#include "psplot.h"

int ASF(float min, float max){

  int diff;

  diff=(int)max-(int)min;
  if (!(diff % 5)) return 5;
  if (!(diff % 4)) return 4;
  if (!(diff % 6)) return 6;
  if (!(diff % 7)) return 7;
  if (!(diff % 3)) return 3;
  if (!(diff % 2)) return 2;
  return 4;
}


/* +++++++++++++++++++++++++ Hauptprogramm +++++++++++++++++++++++++++++++ */

int main(int argc, char* argv[]){

  psclpara G;
  psplot YU;

  // Benutzerinformationen:
  if (argc<3) { Benutzerinfo(); return 1; }

  /* Einlesen und interpretieren der Kommandozeile: */
  if (getpsclpara(argc, argv, G)) return 1;

  FILE *PSPlotDatei;                 /* erzeugdes Postscript-File (Stream)  */
  FILE *tmpDatei;

  // ------------- Schreiben des Datenfiles und eventuell Bestimmung der 
  // ------------- Wertebereiche von x und y

  char datfilename[256];
  float foundxmin, foundxmax, foundymin, foundymax;

  if (G.cldatatype==0){                       //     0: normales Datenfile
    strcpy(datfilename,G.cldatafilename);
    int i;
    float var[20];
    if (!(G.clxthere && G.clythere)){
      if (!(tmpDatei=fopen(G.cldatafilename,"r"))){
	cout << " ERROR: cannot open Data file." << endl;
	return 1;}
      if (fscanf(tmpDatei,"%f",&var[1]) != EOF){
	for(i=2;i<=G.clges;i++) fscanf(tmpDatei,"%f",&var[i]);
	foundxmin = var[G.clxpos]; foundxmax = var[G.clxpos]; 
	foundymin = var[G.clypos]; foundymax = var[G.clypos];}
      else {
	cout << " ERROR: no data in Data file." << endl;
	return 1;}
      while (fscanf(tmpDatei,"%f",&var[1]) != EOF){
	for(i=2;i<=G.clges;i++) fscanf(tmpDatei,"%f",&var[i]);
	if (var[G.clxpos]<foundxmin) foundxmin=var[G.clxpos];
	if (var[G.clxpos]>foundxmax) foundxmax=var[G.clxpos];
	if (var[G.clypos]<foundymin) foundymin=var[G.clypos];
	if (var[G.clypos]>foundymax) foundymax=var[G.clypos];}
      fclose(tmpDatei);
    }
  }

  else if (G.cldatatype==1){                  //     1: 1-dim tasty-histo
#ifdef __no_tasty__
    fprintf(stderr,"Sorry, this version is compiled without tasty-lib!\n");
    return 1;
#else
    strcpy(datfilename,"tmp.dat");
    if (!G.clotypethere) G.clotype=2;
    if (G.clotype==3) G.clotype=0;
    HcsFastHisto ahist(G.cldatafilename);
    float* con=ahist.contents();
    float xbinsize=ahist.xBinSize();
    /* Schreiben des Datenfiles und Bestimmung des y-Bereiches: */
    if (!(tmpDatei=fopen(datfilename,"w"))) exit(1);
    foundymax=0; foundymin=0;
    for(unsigned i=1;i<=ahist.xlen();i++) {
      static double hyg;
      hyg = ahist.xmin()+(i-1)*xbinsize;
      if (con[i]>foundymax) foundymax=con[i];
      if (con[i]<foundymin) foundymin=con[i];
      if (!((G.clxthere)&&((hyg<G.clxmin)||(hyg>G.clxmax))))
	fprintf(tmpDatei,"%g %g\n",hyg,con[i]);
    }
    fclose(tmpDatei);
    foundxmin=ahist.xmin();
    foundxmax=ahist.xmax();
    if (foundymin>=0) foundymin=0.0; else foundymin*=1.2;
    foundymax*=1.2;
    if ( (foundymax-foundymin) == 0.0) foundymax+=1.2;
#endif
  }

  else if (G.cldatatype==2){                  //     2: 2-dim tasty-histo
#ifdef __no_tasty__
    fprintf(stderr,"Sorry, this version is compiled without tasty-lib!\n");
    return 1;
#else
    strcpy(datfilename,"tmp.dat");
    if (G.clotype==3) G.clotype=0;
    HcsFastField ahist(G.cldatafilename);
    unsigned long i, j, xl, yl;
    float xB, yB, xmi, ymi;
    xl = ahist.xlen(); yl = ahist.ylen();
    xmi = ahist.xmin(); ymi = ahist.ymin(); 
    xB = ahist.xBinSize(); yB = ahist.yBinSize(); 
    /* Schreiben des Datenfiles und Bestimmung des y-Bereiches: */
    if (!(tmpDatei=fopen(datfilename,"w"))) exit(1);
    float xnow, ynow, znow;
    for(i=1;i<=xl;i++) 
      for(j=1;j<=yl;j++) {
	if (ahist.binContents(i,j)){
	  xnow = xmi + (i-1)*xB;
	  ynow = ymi + (j-1)*yB;
	  znow = ahist.binContents(i,j);
	  fprintf(tmpDatei,"%g %g %g\n", xnow, ynow, znow);}}
    fclose(tmpDatei);
    G.clges=3;
    G.clxbinsize=xB; G.clybinsize=yB;
    if (G.clxpos == 2) { G.clxbinsize=yB; G.clybinsize=xB; }
    foundxmin=ahist.xmin();
    foundxmax=ahist.xmax();
    foundymin=ahist.ymin();
    foundymax=ahist.ymax();
#endif
  }

  else if (G.cldatatype != -1){
    cout << " unknown cldatatype !!! " << endl;
    return 1;}

  // Eventuelle Rebinierung von Histogrammen:
  if (G.clrebinhisto) {
    if (G.clotype!=2) {
      cerr << " ERROR: Can only rebin 1D - histograms! " << endl; return 1; }
    FILE *tmp_rb_Datei;
    cout << " rebin histogram by a factor of " << G.clrebinhisto << endl;
    if (!(tmpDatei=fopen(datfilename,"r"))){
	cerr << " ERROR: cannot open Data file." << endl;
	return 1;}
    if (!(tmp_rb_Datei=fopen("tmp_rb.dat","w"))){
	cerr << " ERROR: cannot open file for rebined data." << endl;
	return 1;}
    float readx, ready, xu=0, ysum=0;
    int nim=0;
    while (fscanf(tmpDatei,"%f%f",&readx,&ready) != EOF){
      if (!nim) xu=readx;
      ysum+=ready;
      nim++;
      if (nim>=G.clrebinhisto) {
	fprintf(tmp_rb_Datei,"%g %g\n", xu, ysum);
	if (ysum>foundymax) foundymax=ysum;
	if (ysum<foundymin) foundymin=ysum;
	nim=0; xu=0; ysum=0;}
    }
    if (nim>0) fprintf(tmp_rb_Datei,"%g %g\n", xu, ysum);
    fclose(tmpDatei);
    fclose(tmp_rb_Datei);
    strcpy(datfilename,"tmp_rb.dat");
    foundymax*=1.2;
    foundymin*=1.2;
  }

  // Logarithmischer Massstab:
  if (G.clxlog) { YU.XLog =1; if (G.clxlogstyle) YU.XLog10expStyle=1; }
  if (G.clylog) { YU.YLog =1; if (G.clylogstyle) YU.YLog10expStyle=1; }
  if (G.clotype==2){
    if ((G.clxlog) && (foundxmin ==0)) foundxmin = 0.5;
    if ((G.clylog) && (foundymin ==0)) foundymin = 0.5;
    if (G.clylog) foundymax *= 2;
  }

  // leaveplace_y for autoscaling 
  // (Trotz autoscaling Platz fuer Text unter- und oberhalb der Funktion)
  if (G.clleaveplace_y_there){
    double yspan = foundymax - foundymin;
    foundymax += G.clleaveplace_y_above*yspan;
    foundymin -= G.clleaveplace_y_below*yspan;
  }

  /* >>>>>>>>>>>>>>>>>> Schreiben der Postscript-Datei <<<<<<<<<<<<<<<<<<<< */

  if (G.clorient) YU.Orientation = 1;
  YU.XTitleDist += G.xtitledist;
  YU.YTitleDist += G.ytitledist;
  YU.XTicStyle = G.clxticstyle;
  YU.YTicStyle = G.clyticstyle;
  if (G.BB.there) YU.set_BoundingBox(G.BB.x1, G.BB.y1, G.BB.x2, G.BB.y2);
  YU.openPS(G.clpsfilename,&PSPlotDatei);   /* Oeffnet + initialis. PS-Datei */
  YU.SetFontPS(PSPlotDatei,"Palatino-Roman",18.0);
					 /* Setzt Font und dessen Groesse */

  /* veraenderte Rahmengroesse: */
  if (G.clorient) {
    if (G.clwidth) {
      YU.AktMinX = 11.0 - G.clwidth/2.0;
      YU.AktMaxX = 11.0 + G.clwidth/2.0;
    }
    if (G.clheight) {
      YU.AktMinY = 15.25 - G.clheight/2.0;
      YU.AktMaxY = 15.25 + G.clheight/2.0;
    }
  }
  else {
    if (G.clwidth) {
      YU.AktMinX = 15.75 - G.clwidth/2.0;
      YU.AktMaxX = 15.75 + G.clwidth/2.0;
    }
    if (G.clheight) {
      YU.AktMinY = 10.5 - G.clheight/2.0;
      YU.AktMaxY = 10.5 + G.clheight/2.0;
    }
  }

  if (G.cldatatype == -1){
    if (!G.clxthere) { G.clxmin = YU.AktMinX; G.clxmax = YU.AktMaxX; }
    if (!G.clythere) { G.clymin = YU.AktMinY; G.clymax = YU.AktMaxY; }
  }

  // ---------------------  Definition der Wertebereiche von X und Y
  if (G.clxthere) {
    YU.FktMinX=G.clxmin;                    /* Die X-Achse geht von FktMinX  */
    YU.FktMaxX=G.clxmax;}                   /* bis FktMaxX                   */
  else{
    YU.FktMinX= foundxmin;                 
    YU.FktMaxX= foundxmax;
    cout << " Autorange x = [" << YU.FktMinX << ", " << YU.FktMaxX << "]" << endl;}
  if (G.clythere) {
    YU.FktMinY=G.clymin;                    /* Die Y-Achse geht von FktMinY  */
    YU.FktMaxY=G.clymax;}                   /* bis FktMaxY                   */
  else{
    YU.FktMinY= foundymin;   
    YU.FktMaxY= foundymax;
    cout << " Autorange y = [" << YU.FktMinY << ", " << YU.FktMaxY << "]" << endl;}
  if ( (G.clxlog && ((YU.FktMinX <= 0) && (YU.FktMaxX >= 0))) ||
       (G.clylog && ((YU.FktMinY <= 0) && (YU.FktMaxY >= 0)))   ){
    cerr << " ERROR: Cannot treat 0.0 within a logarithmic scale!" << endl;
    cerr << "        Plaese redefine range!" << endl << endl;
    return 2;
  }
  /* Von nun an werden diese Koordinaten verwendet */

  // Bestimme x- und y- Bingroessen fuer nicht-tasty-2D-Histos
  if ( (G.cldatatype==0) && ( (G.clotype==4) || (G.clotype==6) ) ){ 
    strcpy(datfilename,G.cldatafilename);
    int i;
    float var[20];
    float xakt, yakt, xdiff, ydiff;
    int xdone=0;
    int ydone=0;
    xdiff = (YU.FktMaxX-YU.FktMinX)/10000.0;
    ydiff = (YU.FktMaxY-YU.FktMinY)/10000.0;
    if (!(tmpDatei=fopen(G.cldatafilename,"r"))){
      cout << " ERROR: cannot open Data file." << endl;
      return 1;}
    if (fscanf(tmpDatei,"%f",&var[1]) != EOF){
      for(i=2;i<=G.clges;i++) fscanf(tmpDatei,"%f",&var[i]);
      xakt = var[G.clxpos]; 
      yakt = var[G.clypos]; }
    else {
      cout << " ERROR: no data in Data file." << endl;
      return 1;
    }
    while (fscanf(tmpDatei,"%f",&var[1]) != EOF){
      for(i=2;i<=G.clges;i++) fscanf(tmpDatei,"%f",&var[i]);
      if ( (!xdone) && (fabs(var[G.clxpos]-xakt) > xdiff) ){
	G.clxbinsize = fabs(var[G.clxpos]-xakt); xdone=1;}
      if ( (!ydone) && (fabs(var[G.clypos]-yakt) > ydiff) ){
	G.clybinsize = fabs(var[G.clypos]-yakt); ydone=1;}
      if ( xdone && ydone ) break;
    }
    fclose(tmpDatei);
  }      

  // ++++++++++++++ Geometrische Objekte:
  // box:
  for(int i=1;i<=G.clboxthere;i++)
    YU.boxPS(PSPlotDatei, 
	     G.clbox[i].x1, G.clbox[i].y1, G.clbox[i].x2, G.clbox[i].y2, 
	     G.clbox[i].color, G.clbox[i].linethick, G.clbox[i].framecolor,0);
  // circle:
  for(int i=1;i<=G.clcircthere;i++)
    YU.circlePS(PSPlotDatei, 
		G.clcirc[i].x1, G.clcirc[i].y1, G.clcirc[i].radius, 
		G.clcirc[i].color, G.clcirc[i].linethick, 
		G.clcirc[i].framecolor,0);
  // arc:
  for(int i=1;i<=G.clarcthere;i++)
    YU.arcPS(PSPlotDatei, 
	     G.clarc[i].x1, G.clarc[i].y1, G.clarc[i].radius, 
	     G.clarc[i].start_angle, G.clarc[i].end_angle, 
	     G.clarc[i].color, G.clarc[i].linethick, 
	     G.clarc[i].framecolor,0);
  // triang:
  for(int i=1;i<=G.cltriangthere;i++)
    YU.triangPS(PSPlotDatei, 
		G.cltriang[i].x1, G.cltriang[i].y1, 
		G.cltriang[i].x2, G.cltriang[i].y2, 
		G.cltriang[i].x3, G.cltriang[i].y3, 
		G.cltriang[i].color, G.cltriang[i].linethick, 
		G.cltriang[i].framecolor, 0);
  // polygon:
  for(int i=1;i<=G.clpolygonthere;i++)
    YU.polygonPS(PSPlotDatei,
		 G.clpolygon[i].point_num,
		 G.clpolygon[i].x, G.clpolygon[i].y, 
		 G.clpolygon[i].color, G.clpolygon[i].linethick, 
		 G.clpolygon[i].framecolor, 0);
  // pline:
  for(int i=1;i<=G.clplinethere;i++)
    YU.plinePS(PSPlotDatei,
		 G.clpline[i].point_num,
		 G.clpline[i].x, G.clpline[i].y, 
		 G.clpline[i].linetype, G.clpline[i].linethick, 
		 G.clpline[i].framecolor, 0);
  // line:
  for(int i=1;i<=G.cllinethere;i++)
    YU.linePS(PSPlotDatei, 
	      G.clline[i].x1, G.clline[i].y1, G.clline[i].x2, G.clline[i].y2, 
	      G.clline[i].color, G.clline[i].linethick, 0);

  // curve:
  for(int i=1;i<=G.clcurvethere;i++)
    YU.curvePS(PSPlotDatei, 
	       G.clcurve[i].x1, G.clcurve[i].y1, 
	       G.clcurve[i].x2, G.clcurve[i].y2, 
	       G.clcurve[i].x3, G.clcurve[i].y3,
	       G.clcurve[i].x4, G.clcurve[i].y4,
	       G.clcurve[i].linetype,
	       G.clcurve[i].color, 
	       G.clcurve[i].linethick, 0);

  // symbol:
  for(int i=1;i<=G.clsymthere;i++)
    YU.drawSymbol(PSPlotDatei, G.clsym[i].symtype,
		  G.clsym[i].x1 * YU.cm , G.clsym[i].y1 * YU.cm, 
		  G.clsym[i].color, G.clsym[i].radius * YU.SymbolSize, 
		  G.clsym[i].framecolor, G.clsym[i].linethick);
  
  // ++++++++++++++ Import-Dateien: ++++++++++++++++++++++++++++++++++:
  for(int i=1;i<=G.climportnum;i++)
    YU.import(PSPlotDatei,G.psim[i].filename,
	      G.psim[i].clipmode,
	      G.psim[i].xminclip,G.psim[i].yminclip,
	      G.psim[i].xmaxclip,G.psim[i].ymaxclip,
	      G.psim[i].xpos,G.psim[i].ypos,
	      G.psim[i].xscale,G.psim[i].yscale,
	      G.psim[i].rot, G.psim[i].fktsys);
  
  // --------------- debug-output:
  //   fprintf(stderr," G.clotype = %d\n", G.clotype);
  //   fprintf(stderr," G.clges   = %d\n", G.clges);
  //   fprintf(stderr," G.clxpos  = %d\n", G.clxpos);
  //   fprintf(stderr," G.clypos  = %d\n", G.clypos);
  // return 0;
  YU.PSdotSize = G.cldotsize;
  
  // --------------------- Zeichnen der Kurven:
  if (G.clcolorthere) strncpy(YU.PSGlobalColor,G.clglobalcolor,7);
  if (G.clotype==0) YU.ScatDataPS (PSPlotDatei,datfilename,
				   G.clges,G.clxpos,G.clypos);
  if (G.clotype==1) YU.PlotDataPS (PSPlotDatei,datfilename,G.cllinetype,
				   G.clges,G.clxpos,G.clypos);
  if (G.clotype==2) YU.HistoDataPS(PSPlotDatei,datfilename,G.cllinetype,0.9,
				   G.clges,G.clxpos,G.clypos);
  if (G.clotype==3) {
    if (!G.clsymstylethere) strncpy(G.SymbolColor,YU.PSGlobalColor,7);
    YU.DotDataPS(PSPlotDatei,datfilename,G.clges,G.clxpos,
		 G.clypos,G.clxerr,G.clyerr,
		 G.SymbolType, G.SymbolColor, G.SymbolSize,
		 G.SymbolLineColor, G.SymbolLineThick);
  }
  if (G.clotype==4) YU.Histo2DDataPS(PSPlotDatei,datfilename, G.clges, G.clxpos,
				     G.clypos, G.clzpos, 
				     G.clxbinsize, G.clybinsize,
				     G.clzlog);
  if (G.clotype==5) 
    if (G.clarstyle) YU.RelArrowDataPS (PSPlotDatei,datfilename,
					G.clges,G.clxpos,G.clypos,
					G.clxerr, G.clyerr,
					G.clarthick, G.clarskip, G.clcolor);
    else             YU.AbsArrowDataPS (PSPlotDatei,datfilename,
					G.clges,G.clxpos,G.clypos,
					G.clarthick, G.clarskip, G.clcolor);
  if (G.clotype==6) YU.Color2DDataPS(PSPlotDatei,datfilename, G.clges, G.clxpos,
				     G.clypos, G.clzpos, 
				     G.clxbinsize, G.clybinsize,
				     G.clzlog);
  
  // --------------------- Zeichnen von eventuellen Zusatzplots:
  // Linien:
  if (G.clalthere){
    int i=1;
    for (i=1; i<=G.clalthere; i++)
      YU.PlotDataPS (PSPlotDatei,G.clal[i].filename,G.clal[i].linetype,
		     G.clal[i].ges,G.clal[i].xpos,G.clal[i].ypos);
  }
  // Farbige Linien:
  if (G.claclthere){
    int i=1;
    for (i=1; i<=G.claclthere; i++)
      YU.CPlotDataPS(PSPlotDatei,G.clacl[i].filename,G.clacl[i].linetype,
		     G.clacl[i].ges,G.clacl[i].xpos,G.clacl[i].ypos,
		     G.clacl[i].color, G.clacl[i].linewidth);
  }
  // Farbige Histogramme:
  if (G.clachthere){
    int i=1;
    for (i=1; i<=G.clachthere; i++)
      YU.CHistoDataPS(PSPlotDatei, G.clach[i].filename, G.clach[i].linetype,
		      G.clach[i].ges, G.clach[i].xpos, G.clach[i].ypos,
		      G.clach[i].color, G.clach[i].linecolor, 
		      G.clach[i].linewidth);
  }
  // Farbige Symbols:
  if (G.clasymthere){
    int i=1;
    for (i=1; i<=G.clasymthere; i++){
      strncpy(YU.PSGlobalColor,G.clasym[i].color,7);
      YU.DotDataPS(PSPlotDatei,G.clasym[i].filename,G.clasym[i].ges,
		   G.clasym[i].xpos, G.clasym[i].ypos,
		   G.clasym[i].xerr, G.clasym[i].yerr,
		   G.clasym[i].type, G.clasym[i].color, 
		   G.clasym[i].size, G.clasym[i].linecolor, 
		   G.clasym[i].linethick);

    }
  }
  // Arrows:
  if (G.claarthere){
    int i=1;
    for (i=1; i<=G.claarthere; i++){
      if (G.claar[i].style) 
	YU.RelArrowDataPS(   PSPlotDatei, G.claar[i].filename,
			     G.claar[i].ges, G.claar[i].xpos, G.claar[i].ypos, 
			     G.claar[i].ahx, G.claar[i].ahy, G.claar[i].thick, 
			     G.claar[i].skip, G.claar[i].color);
      else YU.AbsArrowDataPS(PSPlotDatei, G.claar[i].filename,
			     G.claar[i].ges, G.claar[i].xpos, G.claar[i].ypos, 
			     G.claar[i].thick, G.claar[i].skip, 
			     G.claar[i].color);
    }
  }


  // --------------------- Zeichnen und skalieren der Achsen:
  if (G.cldrawframe){
    YU.InitFramePS(PSPlotDatei);            /* Zeichnet die X und Y-Achsen   */

    double xticmin, xticmax;
    int xticnum, xticsubnum;
    if (G.clxticsthere){                    /* Scalierung der X-Achse        */
      xticmin    = G.clxticmin;
      xticmax    = G.clxticmax;
      xticnum    = G.clxticnum;
      xticsubnum = G.clxticsubnum; }
    else {
      if (G.clxlog){
	xticmin    = pow(10, floor(log10(YU.FktMinX)));
	xticmax    = pow(10, ceil(log10(YU.FktMaxX)));
	xticnum    = int(ceil(log10(YU.FktMaxX))-floor(log10(YU.FktMinX)));
	xticsubnum = 9; }
      else
	YU.autoscale(YU.FktMinX, YU.FktMaxX,
		     &xticmin, &xticmax, &xticnum, &xticsubnum);
    }
    YU.XTics(PSPlotDatei, xticmin, xticmax, xticnum, xticsubnum, G.xticscale);
    
    double yticmin, yticmax;
    int yticnum, yticsubnum;
    if (G.clyticsthere){                    /* Scalierung der Y-Achse        */
      yticmin    = G.clyticmin;
      yticmax    = G.clyticmax;
      yticnum    = G.clyticnum;
      yticsubnum = G.clyticsubnum; }
    else {
      if (G.clylog){
	yticmin    = pow(10, floor(log10(YU.FktMinY)));
	yticmax    = pow(10, ceil(log10(YU.FktMaxY)));
	yticnum    = int(ceil(log10(YU.FktMaxY))-floor(log10(YU.FktMinY)));
	yticsubnum = 9; }
      else
	YU.autoscale(YU.FktMinY, YU.FktMaxY,
		    &yticmin, &yticmax, &yticnum, &yticsubnum);
    }
    YU.YTics(PSPlotDatei, yticmin, yticmax, yticnum, yticsubnum, G.yticscale);
  }
  

  // --------------------- Polynom:
  if (G.clpolythere){
    int i;
    FILE *FktDatei;
    float step,x,y;
    step=(YU.FktMaxX-YU.FktMinX)/100.0;
    if (!(FktDatei=fopen("poly.dat","w"))) exit(1);
    for(x=YU.FktMinX;x<=YU.FktMaxX;x=x+step)
      { /* Zu plottende Funktion: */
        y=0;
        for (i=0;i<=G.clpolyord;i++) y+=G.cla[i]*pow(x,i);
        fprintf(FktDatei,"%f %f\n",x,y);
      }
    fclose(FktDatei);
    YU.PlotDataPS(PSPlotDatei,"poly.dat","[] 0",2,1,2);
  }


  // ++++++++++++++ Geometrische Objekte im Funktionskoordinatensystem:
  // fbox:
  for(int i=1;i<=G.clfboxthere;i++)
    YU.boxPS(PSPlotDatei, 
	     G.clfbox[i].x1, G.clfbox[i].y1, G.clfbox[i].x2, G.clfbox[i].y2, 
	     G.clfbox[i].color, G.clfbox[i].linethick, 
	     G.clfbox[i].framecolor,1);
  // fcircle:
  for(int i=1;i<=G.clfcircthere;i++)
    YU.circlePS(PSPlotDatei, 
		G.clfcirc[i].x1, G.clfcirc[i].y1, G.clfcirc[i].radius, 
		G.clfcirc[i].color, G.clfcirc[i].linethick, 
		G.clfcirc[i].framecolor,1);
  // farc:
  for(int i=1;i<=G.clfarcthere;i++)
    YU.arcPS(PSPlotDatei, 
	     G.clfarc[i].x1, G.clfarc[i].y1, G.clfarc[i].radius, 
	     G.clfarc[i].start_angle, G.clfarc[i].end_angle, 
	     G.clfarc[i].color, G.clfarc[i].linethick, 
	     G.clfarc[i].framecolor,1);
  // ftriang
  for(int i=1;i<=G.clftriangthere;i++)
    YU.triangPS(PSPlotDatei, 
		G.clftriang[i].x1, G.clftriang[i].y1, 
		G.clftriang[i].x2, G.clftriang[i].y2, 
		G.clftriang[i].x3, G.clftriang[i].y3, 
		G.clftriang[i].color, G.clftriang[i].linethick, 
		G.clftriang[i].framecolor, 1);
  // fpolygon:
  for(int i=1;i<=G.clfpolygonthere;i++)
    YU.polygonPS(PSPlotDatei,
		 G.clfpolygon[i].point_num,
		 G.clfpolygon[i].x, G.clfpolygon[i].y, 
		 G.clfpolygon[i].color, G.clfpolygon[i].linethick, 
		 G.clfpolygon[i].framecolor, 1);
  // fpline:
  for(int i=1;i<=G.clfplinethere;i++)
    YU.plinePS(PSPlotDatei,
		 G.clfpline[i].point_num,
		 G.clfpline[i].x, G.clfpline[i].y, 
		 G.clfpline[i].linetype, G.clfpline[i].linethick, 
		 G.clfpline[i].framecolor, 1);
  // fline:
  for(int i=1;i<=G.clflinethere;i++)
    YU.linePS(PSPlotDatei, 
	      G.clfline[i].x1, G.clfline[i].y1, G.clfline[i].x2, 
	      G.clfline[i].y2, G.clfline[i].color, G.clfline[i].linethick, 1);

  // fcurve:
  for(int i=1;i<=G.clfcurvethere;i++)
    YU.curvePS(PSPlotDatei, 
	       G.clfcurve[i].x1, G.clfcurve[i].y1, 
	       G.clfcurve[i].x2, G.clfcurve[i].y2, 
	       G.clfcurve[i].x3, G.clfcurve[i].y3,
	       G.clfcurve[i].x4, G.clfcurve[i].y4,
	       G.clfcurve[i].linetype,
	       G.clfcurve[i].color, 
	       G.clfcurve[i].linethick, 1);

  // drawfsymbol:
  for(int i=1;i<=G.clfsymthere;i++)
    YU.drawSymbol(PSPlotDatei, G.clfsym[i].symtype,
		  YU.realX(G.clfsym[i].x1) , YU.realY(G.clfsym[i].y1), 
		  G.clfsym[i].color, G.clfsym[i].radius * YU.SymbolSize, 
		  G.clfsym[i].framecolor, G.clfsym[i].linethick);

  // --------------------- Ausgabe von eventellen Texten:
  // Einzeiler (festes Koordinatensystem):
  for(int i=1;i<=G.cltextnum;i++){
    YU.SetFontPS(PSPlotDatei,"Palatino-Roman",G.td[i].fontsize);
    YU.OutTextPS(PSPlotDatei,G.td[i].text,G.td[i].xpos * YU.cm,
	      G.td[i].ypos * YU.cm,G.td[i].buend);}
  // bunte Einzeiler (festes Koordinatensystem):
  for(int i=1;i<=G.clctextnum;i++){
    YU.SetFontPS(PSPlotDatei,G.ctd[i].fontname,G.ctd[i].fontsize);
    YU.OutCTextPS(PSPlotDatei,G.ctd[i].text,G.ctd[i].xpos * YU.cm,
	       G.ctd[i].ypos * YU.cm,G.ctd[i].buend, G.ctd[i].color);}
  // Einzeiler (Funktions-Koordinatensystem):
  for(int i=1;i<=G.clfkttextnum;i++){
    YU.SetFontPS(PSPlotDatei,G.fkttd[i].fontname,G.fkttd[i].fontsize);
    YU.PosCTextPS(PSPlotDatei,G.fkttd[i].text,G.fkttd[i].xpos,
		  G.fkttd[i].ypos,G.fkttd[i].buend,G.fkttd[i].color);}
  // Block-Texte:
  for(int i=1;i<=G.clbtextnum;i++)
    YU.BlockTextPS(PSPlotDatei,G.btd[i].filename,G.btd[i].fontname,
		   G.btd[i].fontsize,G.btd[i].xpos,G.btd[i].ypos,
		   G.btd[i].lineskip);

  // --------------------- Ueberschrift und Beschriftung von x- und y-Achsen:
  YU.SetFontPS(PSPlotDatei,"Palatino-Roman",20.0);
  if (G.cltitthere)
    YU.OutTextPS(PSPlotDatei,G.cltit,(YU.AktMaxX+YU.AktMinX)/2.0 * YU.cm,
		 (YU.AktMaxY+0.5) * YU.cm,'c');
  else
    if (G.cldrawframe)
      YU.OutTextPS(PSPlotDatei,datfilename,(YU.AktMaxX+YU.AktMinX)/2.0 * YU.cm,
		   (YU.AktMaxY+0.5) * YU.cm,'c');

  YU.SetFontPS(PSPlotDatei,"Palatino-Roman",18.0);
					 /* Setzt Font und dessen Groesse */
  if (G.clxtitthere) YU.XTitle(PSPlotDatei, G.clxtit);
  if (G.clytitthere) YU.YTitle(PSPlotDatei, G.clytit);

  YU.closePS(PSPlotDatei);                       /* Schliessen der PS-Datei */

} 
