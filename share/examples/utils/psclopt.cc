// C-Modul psclopt : Verarbeitung von Kommandozeilenoptionen fuer
//                   auf PSPLOT basierende Programme.
//
//         Version : 2.5, T.Pospischil,  4.03.98
//                   2.6, T.Pospischil, 25.09.98, 2D-Farbe
//                   2.7, T.Pospischil, 19.11.98, triang, arc
//                   2.9, T.Pospischil, 21.01.99, verschied. Symbol-Typen
//                   3.0, T.Pospischil, 27.04.99, Bounding Box
//                   3.1, T.Pospischil, 14.05.99, Polygone
//                   3.2, T.Pospischil, 13.07.99, x- und y- titledist
//                   3.3, T.Pospischil, 23.07.99, pline
//                   3.4, T.Pospischil, 29.07.99, x- und y- ticstyle

#include <iostream>
#include <stdio.h>
#include <string.h>
#include "psclopt.h"

// Funktion "getpsclpara", die die Variablen von der Kommandozeile liest.
// In dem Fall, dass eine Option oder ein Parameter unverstaendlich oder nicht 
// vorhanden ist, wird eine Fehlermeldung ausgegeben und als Funktionswert
// ein Wert ungleich 0 zurueckgegeben.
// Gleiches gilt auch, falls eine notwendige Option (z.B. datafilename)
// nicht angegeben ist.

int getpsclpara(int ac, char *av[], psclpara& V){
  
  int argnow=1;

  while (argnow<ac){

    if (!strcmp(av[argnow],"-f")){               // histogram name
      V.clfthere=1;
      if ( (argnow+1) > ac){
	cerr << " ERROR: Missing data file name! " << endl; return 1;}
      strcpy(V.cldatafilename,av[argnow+1]);
      argnow+=2;}

    else if (!strcmp(av[argnow],"-tasty")){      // input data type
      V.cldatatype=1;
      argnow+=1;}
    else if (!strcmp(av[argnow],"-tasty2D")){
      V.cldatatype=2;
      argnow+=1;}

    else if (!strcmp(av[argnow],"-o")){          // name of PS output file
      V.clothere=1;
      if ( (argnow+1) > ac){
	cerr << " ERROR: Missing file name after -o !" << endl; return 1;}
      strcpy(V.clpsfilename,av[argnow+1]);
      argnow+=2;}

    // Orientierung und eventuelle Groessenaenderung:
    else if (!strcmp(av[argnow],"-portrait")){       // portrait
      V.clorient=1;
      argnow+=1;}
    else if (!strcmp(av[argnow],"-width")){          // width
      if ( (argnow+1) > ac){
	cerr << " ERROR: Need number after width " << endl; return 1;}
      if (!(sscanf(av[argnow+1],"%f",&V.clwidth)==1)){
	cerr << " ERROR: Cannot read width: " << av[argnow+1];
	cerr << " ????? " << endl; return 1;}
      argnow+=2;}
    else if (!strcmp(av[argnow],"-height")){         // height
      if ( (argnow+1) > ac){
	cerr << " ERROR: Need number after height " << endl; return 1;}
      if (!(sscanf(av[argnow+1],"%f",&V.clheight)==1)){
	cerr << " ERROR: Cannot read height: " << av[argnow+1];
	cerr << " ????? " << endl; return 1;}
      argnow+=2;}

    else if (!strcmp(av[argnow],"-dotsize")){         // dotsize
      if ( (argnow+1) > ac){
	cerr << " ERROR: Need number after dotsize " << endl; return 1;}
      if (!(sscanf(av[argnow+1],"%f",&V.cldotsize)==1)){
	cerr << " ERROR: Cannot read dotsize: " << av[argnow+1];
	cerr << " ????? " << endl; return 1;}
      argnow+=2;}

    else if (!strcmp(av[argnow],"-xtitledist")){     // xtitledist
      if ( (argnow+1) > ac){
	cerr << " ERROR: Need number after xtitledist " << endl; return 1;}
      if (!(sscanf(av[argnow+1],"%f",&V.xtitledist)==1)){
	cerr << " ERROR: Cannot read xtitledist: " << av[argnow+1];
	cerr << " ????? " << endl; return 1;}
      argnow+=2;}
    else if (!strcmp(av[argnow],"-ytitledist")){     // ytitledist
      if ( (argnow+1) > ac){
	cerr << " ERROR: Need number after ytitledist " << endl; return 1;}
      if (!(sscanf(av[argnow+1],"%f",&V.ytitledist)==1)){
	cerr << " ERROR: Cannot read ytitledist: " << av[argnow+1];
	cerr << " ????? " << endl; return 1;}
      argnow+=2;}

    else if (!strcmp(av[argnow],"-xticstyle")){     // xticstyle
      if ( (argnow+1) > ac){
	cerr << " ERROR: Need number after xticstyle " << endl; return 1;}
      if (!(sscanf(av[argnow+1],"%d",&V.clxticstyle)==1)){
	cerr << " ERROR: Cannot read xticstyle: " << av[argnow+1];
	cerr << " ????? " << endl; return 1;}
      argnow+=2;}
    else if (!strcmp(av[argnow],"-yticstyle")){     // yticstyle
      if ( (argnow+1) > ac){
	cerr << " ERROR: Need number after yticstyle " << endl; return 1;}
      if (!(sscanf(av[argnow+1],"%d",&V.clyticstyle)==1)){
	cerr << " ERROR: Cannot read yticstyle: " << av[argnow+1];
	cerr << " ????? " << endl; return 1;}
      argnow+=2;}

    // Ausgabeformen
    else if (!strcmp(av[argnow],"-dots")){       // dots
      V.clotype=0;
      V.clotypethere=1;
      argnow+=1;}
    else if (!strcmp(av[argnow],"-line")){       // line
      V.clotype=1;
      V.clotypethere=1;
      if ( (argnow+1) > ac){
	cerr << " ERROR: Missing linetype name after -line!"<<endl;return 1;}
      strcpy(V.cllinetype,av[argnow+1]);
      argnow+=2;}
    else if (!strcmp(av[argnow],"-histo")){      // histo
      V.clotype=2;
      V.clotypethere=1;
      argnow+=1;}
    else if (!strcmp(av[argnow],"-symbol")){     // symbol
      V.clotype=3;
      V.clotypethere=1;
      argnow+=1;}
    else if (!strcmp(av[argnow],"-symbolstyle")){ // symbolstyle
      V.clsymstylethere=1;
      if ( (argnow+5) > ac){
	cerr << " ERROR: Need 5 parameters after -symbolstyle" << endl; 
	return 1;}
      if (!(sscanf(av[argnow+1],"%d",&V.SymbolType)==1)){
	cerr << " ERROR: Cannot read symbolstyle type value: " << av[argnow+1];
	cerr << " ????? " << endl; return 1;}
      strcpy(V.SymbolColor,av[argnow+2]);
      if (!(sscanf(av[argnow+3],"%f",&V.SymbolSize)==1)){
	cerr << " ERROR: Cannot read symbolstyle size value: " << av[argnow+3];
	cerr << " ????? " << endl; return 1;}
      strcpy(V.SymbolLineColor,av[argnow+4]);
      if (!(sscanf(av[argnow+5],"%f",&V.SymbolLineThick)==1)){
	cerr << " ERROR: Cannot read symbolstyle linethick value: ";
	cerr << av[argnow+5] << " ????? " << endl; return 1;}
      argnow+=6;}
    else if (!strcmp(av[argnow],"-gray2D")){     // gray2D
      V.clotype=4;
      V.clges=3;
      V.clotypethere=1;
      argnow+=1;}
    else if (!strcmp(av[argnow],"-color2D")){    // color2D
      V.clotype=6;
      V.clges=3;
      V.clotypethere=1;
      argnow+=1;}
    else if (!strcmp(av[argnow],"-arrows")){     // arrows
      V.clotype=5;
      V.clotypethere=1;
      if ( (argnow+4) > ac){
	cerr << " ERROR: Need 4 parameters after -arrows" << endl; return 1;}
      if (!(sscanf(av[argnow+1],"%d",&V.clarstyle)==1)){
	cerr << " ERROR: Cannot read arrows style value: " << av[argnow+1];
	cerr << " ????? " << endl; return 1;}
      strcpy(V.clcolor,av[argnow+2]);
      if (!(sscanf(av[argnow+3],"%f",&V.clarthick)==1)){
	cerr << " ERROR: Cannot read arrows thickness value: " << av[argnow+3];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+4],"%f",&V.clarskip)==1)){
	cerr << " ERROR: Cannot read arrows skip value: " << av[argnow+4];
	cerr << " ????? " << endl; return 1;}
      argnow+=5;}

    else if (!strcmp(av[argnow],"-columns")){     // columns
      if ( (argnow+5) > ac){
	cerr << " ERROR: Need five values after -columns" << endl; return 1;}
      if (!(sscanf(av[argnow+1],"%d",&V.clges)==1)){
	cerr << " ERROR: Cannot read columns ges value: " << av[argnow+1];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+2],"%d",&V.clxpos)==1)){
	cerr << " ERROR: Cannot read columns xpos value: " << av[argnow+2];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+3],"%d",&V.clypos)==1)){
	cerr << " ERROR: Cannot read columns ypos value: " << av[argnow+3];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+4],"%d",&V.clxerr)==1)){
	cerr << " ERROR: Cannot read columns xerr value: " << av[argnow+4];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+5],"%d",&V.clyerr)==1)){
	cerr << " ERROR: Cannot read columns yerr value: " << av[argnow+4];
	cerr << " ????? " << endl; return 1;}
      argnow+=6;}

    else if (!strcmp(av[argnow],"-addline")){        // addline
      if ( (argnow+5) > ac){
	cerr << " ERROR: Need 5 parameters after -addline" << endl; return 1;}
      V.clalthere++;
      if (V.clalthere>127){
	cerr << " Sorry, only 127 line-plots possible" << endl; return 1;}
      strcpy(V.clal[V.clalthere].filename,av[argnow+1]);
      if (!(sscanf(av[argnow+2],"%d",&V.clal[V.clalthere].ges)==1)){
	cerr << " ERROR: Cannot read addline ges value: " << av[argnow+2];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+3],"%d",&V.clal[V.clalthere].xpos)==1)){
	cerr << " ERROR: Cannot read addline xpos value: " << av[argnow+3];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+4],"%d",&V.clal[V.clalthere].ypos)==1)){
	cerr << " ERROR: Cannot read addline ypos value: " << av[argnow+4];
	cerr << " ????? " << endl; return 1;}
      strcpy(V.clal[V.clalthere].linetype,av[argnow+5]);
      argnow+=6;}

    else if (!strcmp(av[argnow],"-addcline")){       // addcline
      if ( (argnow+7) > ac){
	cerr << " ERROR: Need 7 parameters after -addcline" << endl; return 1;}
      V.claclthere++;
      if (V.claclthere>127){
	cerr << " Sorry, only 127 color line-plots possible" << endl; return 1;}
      strcpy(V.clacl[V.claclthere].filename,av[argnow+1]);
      if (!(sscanf(av[argnow+2],"%d",&V.clacl[V.claclthere].ges)==1)){
	cerr << " ERROR: Cannot read addcline ges value: " << av[argnow+2];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+3],"%d",&V.clacl[V.claclthere].xpos)==1)){
	cerr << " ERROR: Cannot read addcline xpos value: " << av[argnow+3];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+4],"%d",&V.clacl[V.claclthere].ypos)==1)){
	cerr << " ERROR: Cannot read addcline ypos value: " << av[argnow+4];
	cerr << " ????? " << endl; return 1;}
      strcpy(V.clacl[V.claclthere].linetype,av[argnow+5]);
      strcpy(V.clacl[V.claclthere].color,av[argnow+6]);
      if (!(sscanf(av[argnow+7],"%f",&V.clacl[V.claclthere].linewidth)==1)){
	cerr << " ERROR: Cannot read addcline linewidth value: ";
	cerr << av[argnow+7];
	cerr << " ????? " << endl; return 1;}
      argnow+=8;}

    else if (!strcmp(av[argnow],"-addhisto")){       // addhisto
      if ( (argnow+8) > ac){
	cerr << " ERROR: Need 8 parameters after -addhisto" << endl; return 1;}
      V.clachthere++;
      if (V.clachthere>127){
	cerr << " Sorry, only 127 histo plots possible" << endl; return 1;}
      strcpy(V.clach[V.clachthere].filename,av[argnow+1]);
      if (!(sscanf(av[argnow+2],"%d",&V.clach[V.clachthere].ges)==1)){
	cerr << " ERROR: Cannot read addhisto ges value: " << av[argnow+2];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+3],"%d",&V.clach[V.clachthere].xpos)==1)){
	cerr << " ERROR: Cannot read addhisto xpos value: " << av[argnow+3];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+4],"%d",&V.clach[V.clachthere].ypos)==1)){
	cerr << " ERROR: Cannot read addhisto ypos value: " << av[argnow+4];
	cerr << " ????? " << endl; return 1;}
      strcpy(V.clach[V.clachthere].linetype,av[argnow+5]);
      strcpy(V.clach[V.clachthere].color,av[argnow+6]);
      if (!(sscanf(av[argnow+7],"%f",&V.clach[V.clachthere].linewidth)==1)){
	cerr << " ERROR: Cannot read addhisto linewidth value: ";
	cerr << av[argnow+7];
	cerr << " ????? " << endl; return 1;}
      strcpy(V.clach[V.clachthere].linecolor,av[argnow+8]);
      argnow+=9;}

    else if (!strcmp(av[argnow],"-addsymbols")){       // addsymbols
      if ( (argnow+7) > ac){
	cerr << " ERROR: Need 7 parameters after -addsymbols" << endl;return 1;}
      V.clasymthere++;
      if (V.clasymthere>127){
	cerr << " Sorry, only 127 symbol plots possible" << endl; return 1;}
      strcpy(V.clasym[V.clasymthere].filename,av[argnow+1]);
      if (!(sscanf(av[argnow+2],"%d",&V.clasym[V.clasymthere].ges)==1)){
	cerr << " ERROR: Cannot read addsymbols ges value: " << av[argnow+2];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+3],"%d",&V.clasym[V.clasymthere].xpos)==1)){
	cerr << " ERROR: Cannot read addsymbols xpos value: " << av[argnow+3];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+4],"%d",&V.clasym[V.clasymthere].ypos)==1)){
	cerr << " ERROR: Cannot read addsymbols ypos value: " << av[argnow+4];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+5],"%d",&V.clasym[V.clasymthere].xerr)==1)){
	cerr << " ERROR: Cannot read addsymbols xerr value: " << av[argnow+5];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+6],"%d",&V.clasym[V.clasymthere].yerr)==1)){
	cerr << " ERROR: Cannot read addsymbols yerr value: " << av[argnow+6];
	cerr << " ????? " << endl; return 1;}
      strcpy(V.clasym[V.clasymthere].color,av[argnow+7]);
      V.clasym[V.clasymthere].type = 1;
      V.clasym[V.clasymthere].size = 1.0;
      strcpy(V.clasym[V.clasymthere].linecolor,"000000");
      V.clasym[V.clasymthere].linethick = 0.3;
      argnow+=8;}

    else if (!strcmp(av[argnow],"-addTsymbols")){       // addTsymbols
      if ( (argnow+11) > ac){
	cerr << " ERROR: Need 11 parameters after -addTsymbols" << endl;
	return 1;}
      V.clasymthere++;
      if (V.clasymthere>127){
	cerr << " Sorry, only 127 symbol plots possible" << endl; return 1;}
      strcpy(V.clasym[V.clasymthere].filename,av[argnow+1]);
      if (!(sscanf(av[argnow+2],"%d",&V.clasym[V.clasymthere].ges)==1)){
	cerr << " ERROR: Cannot read addTsymbols ges value: " << av[argnow+2];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+3],"%d",&V.clasym[V.clasymthere].xpos)==1)){
	cerr << " ERROR: Cannot read addTsymbols xpos value: " << av[argnow+3];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+4],"%d",&V.clasym[V.clasymthere].ypos)==1)){
	cerr << " ERROR: Cannot read addTsymbols ypos value: " << av[argnow+4];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+5],"%d",&V.clasym[V.clasymthere].xerr)==1)){
	cerr << " ERROR: Cannot read addTsymbols xerr value: " << av[argnow+5];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+6],"%d",&V.clasym[V.clasymthere].yerr)==1)){
	cerr << " ERROR: Cannot read addTsymbols yerr value: " << av[argnow+6];
	cerr << " ????? " << endl; return 1;}
      strcpy(V.clasym[V.clasymthere].color,av[argnow+7]);
      if (!(sscanf(av[argnow+8],"%d",&V.clasym[V.clasymthere].type)==1)){
	cerr << " ERROR: Cannot read addTsymbols type value: " << av[argnow+8];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+9],"%f",&V.clasym[V.clasymthere].size)==1)){
	cerr << " ERROR: Cannot read addTsymbols size value: " << av[argnow+9];
	cerr << " ????? " << endl; return 1;}
      strcpy(V.clasym[V.clasymthere].linecolor,av[argnow+10]);
      if (!(sscanf(av[argnow+11],"%f",&V.clasym[V.clasymthere].linethick)==1)){
	cerr << " ERROR: Cannot read addTsymbols linsthick value: ";
	cerr << av[argnow+11] << " ????? " << endl; return 1;}
     argnow+=12;}

    else if (!strcmp(av[argnow],"-addarrows")){       // addarrows
      if ( (argnow+10) > ac){
	cerr << " ERROR: Need 10 parameters after -addarrows" << endl;return 1;}
      V.claarthere++;
      if (V.claarthere>127){
	cerr << " Sorry, only 127 arrows plots possible" << endl; return 1;}
      strcpy(V.claar[V.claarthere].filename,av[argnow+1]);
      if (!(sscanf(av[argnow+2],"%d",&V.claar[V.claarthere].ges)==1)){
	cerr << " ERROR: Cannot read addarrows ges value: " << av[argnow+2];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+3],"%d",&V.claar[V.claarthere].xpos)==1)){
	cerr << " ERROR: Cannot read addarrows xpos value: " << av[argnow+3];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+4],"%d",&V.claar[V.claarthere].ypos)==1)){
	cerr << " ERROR: Cannot read addarrows ypos value: " << av[argnow+4];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+5],"%d",&V.claar[V.claarthere].ahx)==1)){
	cerr << " ERROR: Cannot read addarrows ahx value: " << av[argnow+5];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+6],"%d",&V.claar[V.claarthere].ahy)==1)){
	cerr << " ERROR: Cannot read addarrows ahy value: " << av[argnow+6];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+7],"%d",&V.claar[V.claarthere].style)==1)){
	cerr << " ERROR: Cannot read addarrows style value: " << av[argnow+7];
	cerr << " ????? " << endl; return 1;}
      strcpy(V.claar[V.claarthere].color,av[argnow+8]);
      if (!(sscanf(av[argnow+9],"%f",&V.claar[V.claarthere].thick)==1)){
	cerr << " ERROR: Cannot read addarrows thick value: " << av[argnow+9];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+10],"%f",&V.claar[V.claarthere].skip)==1)){
	cerr << " ERROR: Cannot read addarrows skip value: " << av[argnow+10];
	cerr << " ????? " << endl; return 1;}
      argnow+=11;}

    else if (!strcmp(av[argnow],"-noframe")){        // no frame
      V.cldrawframe=0;
      argnow+=1;}

    else if (!strcmp(av[argnow],"-rebin")){          // rebin Histogram
      V.clrebinhisto=1;
      if ( (argnow+1) > ac){
	cerr << " ERROR: Need factor to rebin histogram " << endl; return 1;}
      if (!(sscanf(av[argnow+1],"%u",&V.clrebinhisto)==1)){
	cerr << " ERROR: Wrong factor to rebin histogram: " << av[argnow+1];
	cerr << " ????? " << endl; return 1;}
      argnow+=2;}

    else if (!strcmp(av[argnow],"-x")){          // x-Range
      V.clxthere=1;
      if ( (argnow+2) > ac){
	cerr << " ERROR: Need two values for X-Range! " << endl; return 1;}
      if (!(sscanf(av[argnow+1],"%f",&V.clxmin)==1)){
	cerr << " ERROR: Cannot read X-Range min value: " << av[argnow+1];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+2],"%f",&V.clxmax)==1)){
	cerr << " ERROR: Cannot read X-Range max value: " << av[argnow+2];
	cerr << " ????? " << endl; return 1;}
      if (V.clxmin>=V.clxmax){
	cerr << " ERROR: Xmin not smaller than Xmax!" << endl; return 1;}
      argnow+=3;}

    else if (!strcmp(av[argnow],"-xtics")){          // x-Tics
      V.clxticsthere=1;
      if ( (argnow+4) > ac){
	cerr << " ERROR: Need four values after X-Tics! " << endl; return 1;}
      if (!(sscanf(av[argnow+1],"%f",&V.clxticmin)==1)){
	cerr << " ERROR: Cannot read X-Tic min value: " << av[argnow+1];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+2],"%f",&V.clxticmax)==1)){
	cerr << " ERROR: Cannot read X-Tic max value: " << av[argnow+2];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+3],"%d",&V.clxticnum)==1)){
	cerr << " ERROR: Cannot read Number of X-Tics: " << av[argnow+3];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+4],"%d",&V.clxticsubnum)==1)){
	cerr << " ERROR: Cannot read Number of X-Subtics: " << av[argnow+4];
	cerr << " ????? " << endl; return 1;}
      if (V.clxticmin>=V.clxticmax){
	cerr << " ERROR: XTicMin not smaller than XTicMax!" << endl; return 1;}
      argnow+=5;}

    else if (!strcmp(av[argnow],"-y")){          // y-Range
      V.clythere=1;
      if ( (argnow+2) > ac){
	cerr << " ERROR: Need two values for Y-Range! " << endl; return 1;}
      if (!(sscanf(av[argnow+1],"%f",&V.clymin)==1)){
	cerr << " ERROR: Cannot read Y-Range min value: " << av[argnow+1];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+2],"%f",&V.clymax)==1)){
	cerr << " ERROR: Cannot read Y-Range max value: " << av[argnow+2];
	cerr << " ????? " << endl; return 1;}
      if (V.clymin>=V.clymax){
	cerr << " ERROR: Ymin not smaller than Ymax!" << endl; return 1;}
      argnow+=3;}

    else if (!strcmp(av[argnow],"-ytics")){          // y-Tics
      V.clyticsthere=1;
      if ( (argnow+4) > ac){
	cerr << " ERROR: Need four values after Y-Tics! " << endl; return 1;}
      if (!(sscanf(av[argnow+1],"%f",&V.clyticmin)==1)){
	cerr << " ERROR: Cannot read Y-Tic min value: " << av[argnow+1];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+2],"%f",&V.clyticmax)==1)){
	cerr << " ERROR: Cannot read Y-Tic max value: " << av[argnow+2];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+3],"%d",&V.clyticnum)==1)){
	cerr << " ERROR: Cannot read Number of Y-Tics: " << av[argnow+3];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+4],"%d",&V.clyticsubnum)==1)){
	cerr << " ERROR: Cannot read Number of Y-Subtics: " << av[argnow+4];
	cerr << " ????? " << endl; return 1;}
      if (V.clyticmin>=V.clyticmax){
	cerr << " ERROR: YTicMin not smaller than YTicMax!" << endl; return 1;}
      argnow+=5;}

    else if (!strcmp(av[argnow],"-xticscale")){      // scale xtics
      if ( (argnow+1) > ac){
	cerr << " ERROR: Need factor after -xticscale" << endl; return 1;}
      if (!(sscanf(av[argnow+1],"%f",&V.xticscale)==1)){
	cerr << " ERROR: Cannot read factor after -xticscale: " << av[argnow+1];
	cerr << " ????? " << endl; return 1;}
      argnow+=2;}

    else if (!strcmp(av[argnow],"-yticscale")){      // scale ytics
      if ( (argnow+1) > ac){
	cerr << " ERROR: Need factor after -yticscale" << endl; return 1;}
      if (!(sscanf(av[argnow+1],"%f",&V.yticscale)==1)){
	cerr << " ERROR: Cannot read factor after -yticscale: " << av[argnow+1];
	cerr << " ????? " << endl; return 1;}
      argnow+=2;}

    else if (!strcmp(av[argnow],"-poly")){          // Polynom
      V.clpolythere=1;
      if ( (argnow+1) > ac){
	cerr << " ERROR: Specify order of polynom!" << endl; return 1;}
      if (!(sscanf(av[argnow+1],"%d",&V.clpolyord)==1)){
	cerr << " ERROR: Cannot read order of polynom: " << av[argnow+1];
	cerr << " ????? " << endl; return 1;}
      if (V.clpolyord>9){
	cerr << " ERROR: Order of polynom to high (max 9)." << endl; return 1;}
      for (int ii=0; ii<=V.clpolyord; ii++){
	if ( (argnow+2+ii) > ac){
	  cerr << " ERROR: Missing koefficient for poly!" << endl; return 1;}
	if (!(sscanf(av[argnow+2+ii],"%f",&V.cla[ii])==1)){
	  cerr << " ERROR: Cannot read Poly-Koeff.: " << av[argnow+2+ii];
	  cerr << " ????? " << endl; return 1;}}
      argnow+=(3+V.clpolyord);}

    else if (!strcmp(av[argnow],"-title")){         // title
      V.cltitthere=1;
      if ( (argnow+1) > ac){
	cerr << " ERROR: Missing title after -title !" << endl; return 1;}
      strcpy(V.cltit,av[argnow+1]);
      argnow+=2;}

    else if (!strcmp(av[argnow],"-color")){         // color (global)
      V.clcolorthere=1;
      if ( (argnow+1) > ac){
	cerr << " ERROR: Missing color after -color !" << endl; return 1;}
      strcpy(V.clglobalcolor,av[argnow+1]);
      argnow+=2;}

    else if (!strcmp(av[argnow],"-addbtxt")){          // addbtxt
      V.clbtextnum++;
      if ( (argnow+6) > ac){
	cerr << " ERROR: Missing parameters after -addtxt !" << endl; return 1;}
      strcpy(V.btd[V.clbtextnum].filename,av[argnow+1]);
      strcpy(V.btd[V.clbtextnum].fontname,av[argnow+2]);
      if (!(sscanf(av[argnow+3],"%f",&V.btd[V.clbtextnum].fontsize)==1)){
	cerr << " ERROR: Cannot read addbtxt-fontsize: " << av[argnow+3];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+4],"%f",&V.btd[V.clbtextnum].xpos)==1)){
	cerr << " ERROR: Cannot read addbtxt-xpos: " << av[argnow+4];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+5],"%f",&V.btd[V.clbtextnum].ypos)==1)){
	cerr << " ERROR: Cannot read addbtxt-ypos: " << av[argnow+5];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+6],"%f",&V.btd[V.clbtextnum].lineskip)==1)){
	cerr << " ERROR: Cannot read addbtxt-lineskip: " << av[argnow+6];
	  cerr << " ????? " << endl; return 1;}
      argnow+=7;}

    else if (!strcmp(av[argnow],"-ctext")){          // ctext
      V.clctextnum++;
      if ( (argnow+7) > ac){
        cerr << " ERROR: Missing parameters after -ctext !" << endl; return 1;}
      strcpy(V.ctd[V.clctextnum].text,av[argnow+1]);
      strcpy(V.ctd[V.clctextnum].fontname,av[argnow+2]);
      if (!(sscanf(av[argnow+3],"%f",&V.ctd[V.clctextnum].fontsize)==1)){
        cerr << " ERROR: Cannot read ctext-fontsize: " << av[argnow+3];
          cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+4],"%f",&V.ctd[V.clctextnum].xpos)==1)){
        cerr << " ERROR: Cannot read ctext-xpos: " << av[argnow+4];
          cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+5],"%f",&V.ctd[V.clctextnum].ypos)==1)){
        cerr << " ERROR: Cannot read ctext-ypos: " << av[argnow+5];
          cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+6],"%c",&V.ctd[V.clctextnum].buend)==1)){
        cerr << " ERROR: Cannot read ctext rel-pos character: " << av[argnow+6];
          cerr << " ????? " << endl; return 1;}
      strcpy(V.ctd[V.clctextnum].color,av[argnow+7]);
      argnow+=8;}

    else if (!strcmp(av[argnow],"-text")){          // text
      V.cltextnum++;
      if ( (argnow+5) > ac){
        cerr << " ERROR: Missing parameters after -text !" << endl; return 1;}
      strcpy(V.td[V.cltextnum].text,av[argnow+1]);
      if (!(sscanf(av[argnow+2],"%f",&V.td[V.cltextnum].fontsize)==1)){
        cerr << " ERROR: Cannot read text-fontsize: " << av[argnow+2];
          cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+3],"%f",&V.td[V.cltextnum].xpos)==1)){
        cerr << " ERROR: Cannot read text-xpos: " << av[argnow+3];
          cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+4],"%f",&V.td[V.cltextnum].ypos)==1)){
        cerr << " ERROR: Cannot read text-ypos: " << av[argnow+4];
          cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+5],"%c",&V.td[V.cltextnum].buend)==1)){
        cerr << " ERROR: Cannot read text rel-pos character: " << av[argnow+5];
          cerr << " ????? " << endl; return 1;}
      argnow+=6;}

    else if (!strcmp(av[argnow],"-fkttext")){          // fkttext
      V.clfkttextnum++;
      if ( (argnow+6) > ac){
        cerr << " ERROR: Missing parameters after -fkttext !" <<endl; return 1;}
      strcpy(V.fkttd[V.clfkttextnum].text,av[argnow+1]);
      if (!(sscanf(av[argnow+2],"%f",&V.fkttd[V.clfkttextnum].fontsize)==1)){
        cerr << " ERROR: Cannot read fkttext-fontsize: " << av[argnow+2];
          cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+3],"%f",&V.fkttd[V.clfkttextnum].xpos)==1)){
        cerr << " ERROR: Cannot read fkttext-xpos: " << av[argnow+3];
          cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+4],"%f",&V.fkttd[V.clfkttextnum].ypos)==1)){
        cerr << " ERROR: Cannot read fkttext-ypos: " << av[argnow+4];
          cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+5],"%c",&V.fkttd[V.clfkttextnum].buend)==1)){
        cerr << " ERROR: Cannot read fkttext rel-pos character: " << av[argnow+5];
          cerr << " ????? " << endl; return 1;}
      strcpy(V.fkttd[V.clfkttextnum].color,av[argnow+6]);
      strcpy(V.fkttd[V.clfkttextnum].fontname,V.clfkttextfontname);
      argnow+=7;}

    else if (!strcmp(av[argnow],"-fkttextfont")){        // fkttextfont
      if ( (argnow+1) > ac){
	cerr << " ERROR: Missing font name after -fkttextfont !" << endl; 
	return 1;}
      strcpy(V.clfkttextfontname,av[argnow+1]);
      argnow+=2;}

    else if (!strcmp(av[argnow],"-xtitle")){        // xtitle
      V.clxtitthere=1;
      if ( (argnow+1) > ac){
	cerr << " ERROR: Missing title after -xtitle !" << endl; return 1;}
      strcpy(V.clxtit,av[argnow+1]);
      argnow+=2;}

    else if (!strcmp(av[argnow],"-ytitle")){        // ytitle
      V.clytitthere=1;
      if ( (argnow+1) > ac){
	cerr << " ERROR: Missing title after -ytitle !" << endl; return 1;}
      strcpy(V.clytit,av[argnow+1]);
      argnow+=2;}

    else if (!strcmp(av[argnow],"-xlog")){          //  logarithm. X-Achse
      V.clxlog=1;
      if ( (argnow+1) > ac){
	cerr << " ERROR: No style for xlog given " << endl; return 1;}
      if (!(sscanf(av[argnow+1],"%u",&V.clxlogstyle)==1)){
	cerr << " ERROR: Cannot read xlog style parameter " << av[argnow+1];
	cerr << " ????? " << endl; return 1;}
      argnow+=2;}

    else if (!strcmp(av[argnow],"-ylog")){          //  logarithm. Y-Achse
      V.clylog=1;
      if ( (argnow+1) > ac){
	cerr << " ERROR: No style for ylog given " << endl; return 1;}
      if (!(sscanf(av[argnow+1],"%u",&V.clylogstyle)==1)){
	cerr << " ERROR: Cannot read ylog style parameter " << av[argnow+1];
	cerr << " ????? " << endl; return 1;}
      argnow+=2;}

    else if (!strcmp(av[argnow],"-zlog")){  //  logarithm. Grau-/Farbstufen
      if ( (argnow+1) > ac){
	cerr << " ERROR: No min value for zlog given " << endl; return 1;}
      if (!(sscanf(av[argnow+1],"%f",&V.clzlog)==1)){
	cerr << " ERROR: Cannot read zlog min value " << av[argnow+1];
	cerr << " ????? " << endl; return 1;}
      if (V.clzlog <= 0.0) {
	V.clzlog = 0.0;
	cerr << " WARNING: zlog min value too low." << av[argnow+1];
      }
      argnow+=2;}

    else if (!strcmp(av[argnow],"-import")){          // import
      V.climportnum++;
      if ( (argnow+6) > ac){
	cerr << " ERROR: Missing parameters after -import !" << endl; 
	return 1;}
      V.psim[V.climportnum].clipmode=0;
      strcpy(V.psim[V.climportnum].filename,av[argnow+1]);
      if (!(sscanf(av[argnow+2],"%f",&V.psim[V.climportnum].xpos)==1)){
	cerr << " ERROR: Cannot read importfile xpos: " << av[argnow+2];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+3],"%f",&V.psim[V.climportnum].ypos)==1)){
	cerr << " ERROR: Cannot read importfile ypos: " << av[argnow+3];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+4],"%f",&V.psim[V.climportnum].xscale)==1)){
	cerr << " ERROR: Cannot read importfile xscale: " << av[argnow+4];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+5],"%f",&V.psim[V.climportnum].yscale)==1)){
	cerr << " ERROR: Cannot read importfile yscale: " << av[argnow+5];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+6],"%f",&V.psim[V.climportnum].rot)==1)){
	cerr << " ERROR: Cannot read importfile rotation: " << av[argnow+6];
	  cerr << " ????? " << endl; return 1;}
      argnow+=7;}

    else if ( (!strcmp(av[argnow],"-importclip")) ||         // importclip
	      (!strcmp(av[argnow],"-fimportclip")) ){        // fimportclip
      V.climportnum++;
      if ( (argnow+10) > ac){
	cerr << " ERROR: Missing parameters after -importclip !" << endl; 
	return 1;}
      V.psim[V.climportnum].clipmode=1;
      strcpy(V.psim[V.climportnum].filename,av[argnow+1]);
      if (!(sscanf(av[argnow+2],"%f",&V.psim[V.climportnum].xminclip)==1)){
	cerr << " ERROR: Cannot read importclip xminclip: " << av[argnow+2];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+3],"%f",&V.psim[V.climportnum].yminclip)==1)){
	cerr << " ERROR: Cannot read importclip yminclip: " << av[argnow+3];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+4],"%f",&V.psim[V.climportnum].xmaxclip)==1)){
	cerr << " ERROR: Cannot read importclip xmaxclip: " << av[argnow+4];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+5],"%f",&V.psim[V.climportnum].ymaxclip)==1)){
	cerr << " ERROR: Cannot read importclip ymaxclip: " << av[argnow+5];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+6],"%f",&V.psim[V.climportnum].xpos)==1)){
	cerr << " ERROR: Cannot read importclip file xpos: " << av[argnow+6];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+7],"%f",&V.psim[V.climportnum].ypos)==1)){
	cerr << " ERROR: Cannot read importclip file ypos: " << av[argnow+7];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+8],"%f",&V.psim[V.climportnum].xscale)==1)){
	cerr << " ERROR: Cannot read importclip file xscale: " << av[argnow+8];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+9],"%f",&V.psim[V.climportnum].yscale)==1)){
	cerr << " ERROR: Cannot read importclip file yscale: " << av[argnow+9];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+10],"%f",&V.psim[V.climportnum].rot)==1)){
	cerr << " ERROR: Cannot read importclip file rotation: ";
	cerr << av[argnow+10] << " ????? " << endl; return 1;}
      if (!strcmp(av[argnow],"-importclip")) V.psim[V.climportnum].fktsys=0;
      else V.psim[V.climportnum].fktsys=1;
      argnow+=11;}

    else if (!strcmp(av[argnow],"-leaveplace_y")){     // leaveplace_y
      V.clleaveplace_y_there=1;
      if ( (argnow+2) > ac){
	cerr << " ERROR: Need two values for -leaveplace_y! " << endl;return 1;}
      if (!(sscanf(av[argnow+1],"%f",&V.clleaveplace_y_below)==1)){
	cerr << " ERROR: Cannot read leaveplace_y below value: " <<av[argnow+1];
	cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+2],"%f",&V.clleaveplace_y_above)==1)){
	cerr << " ERROR: Cannot read leaveplace_y above value: " <<av[argnow+2];
	cerr << " ????? " << endl; return 1;}
      argnow+=3;}

    else if (!strcmp(av[argnow],"-box")){          // box
      V.clboxthere++;
      if ( (argnow+7) > ac){
	cerr << " ERROR: Missing parameters after -box !" << endl; 
	return 1;}
      if (!(sscanf(av[argnow+1],"%f",&V.clbox[V.clboxthere].x1)==1)){
	cerr << " ERROR: Cannot read box x1-position: " << av[argnow+1];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+2],"%f",&V.clbox[V.clboxthere].y1)==1)){
	cerr << " ERROR: Cannot read box y1-position: " << av[argnow+2];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+3],"%f",&V.clbox[V.clboxthere].x2)==1)){
	cerr << " ERROR: Cannot read box x2-position: " << av[argnow+3];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+4],"%f",&V.clbox[V.clboxthere].y2)==1)){
	cerr << " ERROR: Cannot read box y2-position: " << av[argnow+4];
	  cerr << " ????? " << endl; return 1;}
      strcpy(V.clbox[V.clboxthere].color,av[argnow+5]);
      if (!(sscanf(av[argnow+6],"%f",&V.clbox[V.clboxthere].linethick)==1)){
	cerr << " ERROR: Cannot read box frame thickness " << av[argnow+6];
	  cerr << " ????? " << endl; return 1;}
      strcpy(V.clbox[V.clboxthere].framecolor,av[argnow+7]);
      argnow+=8;}

    else if (!strcmp(av[argnow],"-fbox")){          // fbox
      V.clfboxthere++;
      if ( (argnow+7) > ac){
	cerr << " ERROR: Missing parameters after -fbox !" << endl; 
	return 1;}
      if (!(sscanf(av[argnow+1],"%f",&V.clfbox[V.clfboxthere].x1)==1)){
	cerr << " ERROR: Cannot read fbox x1-position: " << av[argnow+1];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+2],"%f",&V.clfbox[V.clfboxthere].y1)==1)){
	cerr << " ERROR: Cannot read fbox y1-position: " << av[argnow+2];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+3],"%f",&V.clfbox[V.clfboxthere].x2)==1)){
	cerr << " ERROR: Cannot read fbox x2-position: " << av[argnow+3];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+4],"%f",&V.clfbox[V.clfboxthere].y2)==1)){
	cerr << " ERROR: Cannot read fbox y2-position: " << av[argnow+4];
	  cerr << " ????? " << endl; return 1;}
      strcpy(V.clfbox[V.clfboxthere].color,av[argnow+5]);
      if (!(sscanf(av[argnow+6],"%f",&V.clfbox[V.clfboxthere].linethick)==1)){
	cerr << " ERROR: Cannot read fbox frame thickness " << av[argnow+6];
	  cerr << " ????? " << endl; return 1;}
      strcpy(V.clfbox[V.clfboxthere].framecolor,av[argnow+7]);
      argnow+=8;}

    else if (!strcmp(av[argnow],"-circle")){          // circle
      V.clcircthere++;
      if ( (argnow+6) > ac){
	cerr << " ERROR: Missing parameters after -circle !" << endl; 
	return 1;}
      if (!(sscanf(av[argnow+1],"%f",&V.clcirc[V.clcircthere].x1)==1)){
	cerr << " ERROR: Cannot read circle x1-position: " << av[argnow+1];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+2],"%f",&V.clcirc[V.clcircthere].y1)==1)){
	cerr << " ERROR: Cannot read circle y1-position: " << av[argnow+2];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+3],"%f",&V.clcirc[V.clcircthere].radius)==1)){
	cerr << " ERROR: Cannot read circle radius: " << av[argnow+3];
	  cerr << " ????? " << endl; return 1;}
      strcpy(V.clcirc[V.clcircthere].color,av[argnow+4]);
      if (!(sscanf(av[argnow+5],"%f",&V.clcirc[V.clcircthere].linethick)==1)){
	cerr << " ERROR: Cannot read circle frame thickness " << av[argnow+5];
	  cerr << " ????? " << endl; return 1;}
      strcpy(V.clcirc[V.clcircthere].framecolor,av[argnow+6]);
      argnow+=7;}

    else if (!strcmp(av[argnow],"-fcircle")){          // fcircle
      V.clfcircthere++;
      if ( (argnow+6) > ac){
	cerr << " ERROR: Missing parameters after -fcircle !" << endl; 
	return 1;}
      if (!(sscanf(av[argnow+1],"%f",&V.clfcirc[V.clfcircthere].x1)==1)){
	cerr << " ERROR: Cannot read fcircle x1-position: " << av[argnow+1];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+2],"%f",&V.clfcirc[V.clfcircthere].y1)==1)){
	cerr << " ERROR: Cannot read fcircle y1-position: " << av[argnow+2];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+3],"%f",&V.clfcirc[V.clfcircthere].radius)==1)){
	cerr << " ERROR: Cannot read fcircle radius: " << av[argnow+3];
	  cerr << " ????? " << endl; return 1;}
      strcpy(V.clfcirc[V.clfcircthere].color,av[argnow+4]);
      if(!(sscanf(av[argnow+5],"%f",&V.clfcirc[V.clfcircthere].linethick)==1)){
	cerr << " ERROR: Cannot read fcircle frame thickness " << av[argnow+5];
	  cerr << " ????? " << endl; return 1;}
      strcpy(V.clfcirc[V.clfcircthere].framecolor,av[argnow+6]);
      argnow+=7;}

    else if (!strcmp(av[argnow],"-drawline")){          // line
      V.cllinethere++;
      if ( (argnow+6) > ac){
	cerr << " ERROR: Missing parameters after -drawline !" << endl; 
	return 1;}
      if (!(sscanf(av[argnow+1],"%f",&V.clline[V.cllinethere].x1)==1)){
	cerr << " ERROR: Cannot read drawline x1-position: " << av[argnow+1];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+2],"%f",&V.clline[V.cllinethere].y1)==1)){
	cerr << " ERROR: Cannot read drawline y1-position: " << av[argnow+2];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+3],"%f",&V.clline[V.cllinethere].x2)==1)){
	cerr << " ERROR: Cannot read drawline x2-position: " << av[argnow+3];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+4],"%f",&V.clline[V.cllinethere].y2)==1)){
	cerr << " ERROR: Cannot read drawline y2-position: " << av[argnow+4];
	  cerr << " ????? " << endl; return 1;}
      strcpy(V.clline[V.cllinethere].color,av[argnow+5]);
      if (!(sscanf(av[argnow+6],"%f",&V.clline[V.cllinethere].linethick)==1)){
	cerr << " ERROR: Cannot read drawline frame thickness" << av[argnow+6];
	  cerr << " ????? " << endl; return 1;}
      argnow+=7;}

    else if (!strcmp(av[argnow],"-drawfline")){          // fline
      V.clflinethere++;
      if ( (argnow+6) > ac){
	cerr << " ERROR: Missing parameters after -drawfline !" << endl; 
	return 1;}
      if (!(sscanf(av[argnow+1],"%f",&V.clfline[V.clflinethere].x1)==1)){
	cerr << " ERROR: Cannot read drawfline x1-position: " << av[argnow+1];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+2],"%f",&V.clfline[V.clflinethere].y1)==1)){
	cerr << " ERROR: Cannot read drawfline y1-position: " << av[argnow+2];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+3],"%f",&V.clfline[V.clflinethere].x2)==1)){
	cerr << " ERROR: Cannot read drawfline x2-position: " << av[argnow+3];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+4],"%f",&V.clfline[V.clflinethere].y2)==1)){
	cerr << " ERROR: Cannot read drawfline y2-position: " << av[argnow+4];
	  cerr << " ????? " << endl; return 1;}
      strcpy(V.clfline[V.clflinethere].color,av[argnow+5]);
      if(!(sscanf(av[argnow+6],"%f",&V.clfline[V.clflinethere].linethick)==1)){
	cerr<<" ERROR: Cannot read drawfline frame thickness " << av[argnow+6];
	  cerr << " ????? " << endl; return 1;}
      argnow+=7;}

    else if (!strcmp(av[argnow],"-curve")){          // curve
      V.clcurvethere++;
      if ( (argnow+11) > ac){
	cerr << " ERROR: Missing parameters after -curve !" << endl; 
	return 1;}
      if (!(sscanf(av[argnow+1],"%f",&V.clcurve[V.clcurvethere].x1)==1)){
	cerr << " ERROR: Cannot read curve x1-position: " << av[argnow+1];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+2],"%f",&V.clcurve[V.clcurvethere].y1)==1)){
	cerr << " ERROR: Cannot read curve y1-position: " << av[argnow+2];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+3],"%f",&V.clcurve[V.clcurvethere].x2)==1)){
	cerr << " ERROR: Cannot read curve x2-position: " << av[argnow+3];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+4],"%f",&V.clcurve[V.clcurvethere].y2)==1)){
	cerr << " ERROR: Cannot read curve y2-position: " << av[argnow+4];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+5],"%f",&V.clcurve[V.clcurvethere].x3)==1)){
	cerr << " ERROR: Cannot read curve x3-position: " << av[argnow+5];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+6],"%f",&V.clcurve[V.clcurvethere].y3)==1)){
	cerr << " ERROR: Cannot read curve y3-position: " << av[argnow+6];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+7],"%f",&V.clcurve[V.clcurvethere].x4)==1)){
	cerr << " ERROR: Cannot read curve x4-position: " << av[argnow+7];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+8],"%f",&V.clcurve[V.clcurvethere].y4)==1)){
	cerr << " ERROR: Cannot read curve y4-position: " << av[argnow+8];
	  cerr << " ????? " << endl; return 1;}
      strcpy(V.clcurve[V.clcurvethere].linetype,av[argnow+9]);
      strcpy(V.clcurve[V.clcurvethere].color,av[argnow+10]);
      if (!(sscanf(av[argnow+11],"%f",
		   &V.clcurve[V.clcurvethere].linethick)==1)){
	cerr << " ERROR: Cannot read curve frame thickness" << av[argnow+11];
	cerr << " ????? " << endl; return 1;}
      argnow+=12;}

    else if (!strcmp(av[argnow],"-fcurve")){          // fcurve
      V.clfcurvethere++;
      if ( (argnow+11) > ac){
	cerr << " ERROR: Missing parameters after -fcurve !" << endl; 
	return 1;}
      if (!(sscanf(av[argnow+1],"%f",&V.clfcurve[V.clfcurvethere].x1)==1)){
	cerr << " ERROR: Cannot read fcurve x1-position: " << av[argnow+1];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+2],"%f",&V.clfcurve[V.clfcurvethere].y1)==1)){
	cerr << " ERROR: Cannot read fcurve y1-position: " << av[argnow+2];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+3],"%f",&V.clfcurve[V.clfcurvethere].x2)==1)){
	cerr << " ERROR: Cannot read fcurve x2-position: " << av[argnow+3];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+4],"%f",&V.clfcurve[V.clfcurvethere].y2)==1)){
	cerr << " ERROR: Cannot read fcurve y2-position: " << av[argnow+4];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+5],"%f",&V.clfcurve[V.clfcurvethere].x3)==1)){
	cerr << " ERROR: Cannot read fcurve x3-position: " << av[argnow+5];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+6],"%f",&V.clfcurve[V.clfcurvethere].y3)==1)){
	cerr << " ERROR: Cannot read fcurve y3-position: " << av[argnow+6];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+7],"%f",&V.clfcurve[V.clfcurvethere].x4)==1)){
	cerr << " ERROR: Cannot read fcurve x4-position: " << av[argnow+7];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+8],"%f",&V.clfcurve[V.clfcurvethere].y4)==1)){
	cerr << " ERROR: Cannot read fcurve y4-position: " << av[argnow+8];
	  cerr << " ????? " << endl; return 1;}
      strcpy(V.clfcurve[V.clfcurvethere].linetype,av[argnow+9]);
      strcpy(V.clfcurve[V.clfcurvethere].color,av[argnow+10]);
      if (!(sscanf(av[argnow+11],"%f",
		   &V.clfcurve[V.clfcurvethere].linethick)==1)){
	cerr << " ERROR: Cannot read fcurve frame thickness" << av[argnow+11];
	  cerr << " ????? " << endl; return 1;}
      argnow+=12;}

    else if (!strcmp(av[argnow],"-arc")){          // arc
      V.clarcthere++;
      if ( (argnow+8) > ac){
	cerr << " ERROR: Missing parameters after -arc !" << endl; 
	return 1;}
      if (!(sscanf(av[argnow+1],"%f",&V.clarc[V.clarcthere].x1)==1)){
	cerr << " ERROR: Cannot read arc x1-position: " << av[argnow+1];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+2],"%f",&V.clarc[V.clarcthere].y1)==1)){
	cerr << " ERROR: Cannot read arc y1-position: " << av[argnow+2];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+3],"%f",&V.clarc[V.clarcthere].radius)==1)){
	cerr << " ERROR: Cannot read arc radius: " << av[argnow+3];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+4],"%f",&V.clarc[V.clarcthere].start_angle)==1)){
	cerr << " ERROR: Cannot read arc start angle: " << av[argnow+4];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+5],"%f",&V.clarc[V.clarcthere].end_angle)==1)){
	cerr << " ERROR: Cannot read arc end angle: " << av[argnow+5];
	  cerr << " ????? " << endl; return 1;}
      strcpy(V.clarc[V.clarcthere].color,av[argnow+6]);
      if (!(sscanf(av[argnow+7],"%f",&V.clarc[V.clarcthere].linethick)==1)){
	cerr << " ERROR: Cannot read arc frame thickness " << av[argnow+7];
	  cerr << " ????? " << endl; return 1;}
      strcpy(V.clarc[V.clarcthere].framecolor,av[argnow+8]);
      argnow+=9;}

    else if (!strcmp(av[argnow],"-farc")){          // farc
      V.clfarcthere++;
      if ( (argnow+8) > ac){
	cerr << " ERROR: Missing parameters after -farc !" << endl; 
	return 1;}
      if (!(sscanf(av[argnow+1],"%f",&V.clfarc[V.clfarcthere].x1)==1)){
	cerr << " ERROR: Cannot read farc x1-position: " << av[argnow+1];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+2],"%f",&V.clfarc[V.clfarcthere].y1)==1)){
	cerr << " ERROR: Cannot read farc y1-position: " << av[argnow+2];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+3],"%f",&V.clfarc[V.clfarcthere].radius)==1)){
	cerr << " ERROR: Cannot read farc radius: " << av[argnow+3];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+4],"%f",&V.clfarc[V.clfarcthere].start_angle)==1)){
	cerr << " ERROR: Cannot read farc start angle: " << av[argnow+4];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+5],"%f",&V.clfarc[V.clfarcthere].end_angle)==1)){
	cerr << " ERROR: Cannot read farc end angle: " << av[argnow+5];
	  cerr << " ????? " << endl; return 1;}
      strcpy(V.clfarc[V.clfarcthere].color,av[argnow+6]);
      if (!(sscanf(av[argnow+7],"%f",&V.clfarc[V.clfarcthere].linethick)==1)){
	cerr << " ERROR: Cannot read farc frame thickness " << av[argnow+7];
	  cerr << " ????? " << endl; return 1;}
      strcpy(V.clfarc[V.clfarcthere].framecolor,av[argnow+8]);
      argnow+=9;}

    else if (!strcmp(av[argnow],"-triang")){          // triang
      V.cltriangthere++;
      if ( (argnow+9) > ac){
	cerr << " ERROR: Missing parameters after -triang !" << endl; 
	return 1;}
      if (!(sscanf(av[argnow+1],"%f",&V.cltriang[V.cltriangthere].x1)==1)){
	cerr << " ERROR: Cannot read triang x1-position: " << av[argnow+1];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+2],"%f",&V.cltriang[V.cltriangthere].y1)==1)){
	cerr << " ERROR: Cannot read triang y1-position: " << av[argnow+2];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+3],"%f",&V.cltriang[V.cltriangthere].x2)==1)){
	cerr << " ERROR: Cannot read triang x2-position: " << av[argnow+3];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+4],"%f",&V.cltriang[V.cltriangthere].y2)==1)){
	cerr << " ERROR: Cannot read triang y2-position: " << av[argnow+4];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+5],"%f",&V.cltriang[V.cltriangthere].x3)==1)){
	cerr << " ERROR: Cannot read triang x3-position: " << av[argnow+5];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+6],"%f",&V.cltriang[V.cltriangthere].y3)==1)){
	cerr << " ERROR: Cannot read triang y3-position: " << av[argnow+6];
	  cerr << " ????? " << endl; return 1;}
      strcpy(V.cltriang[V.cltriangthere].color,av[argnow+7]);
      if (!(sscanf(av[argnow+8],"%f",
		   &V.cltriang[V.cltriangthere].linethick)==1)){
	cerr << " ERROR: Cannot read triang frame thickness " << av[argnow+8];
	cerr << " ????? " << endl; return 1;}
      strcpy(V.cltriang[V.cltriangthere].framecolor,av[argnow+9]);
      argnow+=10;}

    else if (!strcmp(av[argnow],"-polygon")){          // polygon
      V.clpolygonthere++;
      if ( (argnow+1) > ac){
	cerr << " ERROR: Missing parameters after -polygon !" << endl; 
	return 1;}
      if (!(sscanf(av[argnow+1],
		   "%d",&V.clpolygon[V.clpolygonthere].point_num)==1)){
	cerr << " ERROR: Cannot read polygon point number: " << av[argnow+1];
	cerr << " ????? " << endl; return 1;}
     if (V.clpolygon[V.clpolygonthere].point_num > 32){
	cerr << " Sorry, only 32 polygon corners allowed!" << endl; 
	return 1;}
     if ( (argnow + (2*V.clpolygon[V.clpolygonthere].point_num) +3) > ac){
	cerr << " ERROR: Missing parameters after -polygon !" << endl; 
	return 1;}
     argnow += 2;
     int i;
     for (i=0;i<V.clpolygon[V.clpolygonthere].point_num;i++){
       if (!(sscanf(av[argnow],"%f",&V.clpolygon[V.clpolygonthere].x[i])==1)){
	cerr << " ERROR: Cannot read polygon point x-position: ";
	cerr << av[argnow] << " ????? " << endl; return 1;}
       if (!(sscanf(av[argnow+1],"%f",&V.clpolygon[V.clpolygonthere].y[i])==1)){
	cerr << " ERROR: Cannot read polygon point y-position: ";
	cerr << av[argnow+1] << " ????? " << endl; return 1;}
       argnow += 2;
     }
     strcpy(V.clpolygon[V.clpolygonthere].color,av[argnow]);
     if (!(sscanf(av[argnow+1],"%f",
		  &V.clpolygon[V.clpolygonthere].linethick)==1)){
       cerr << " ERROR: Cannot read polygon frame thickness " << av[argnow+1];
       cerr << " ????? " << endl; return 1;}
     strcpy(V.clpolygon[V.clpolygonthere].framecolor,av[argnow+2]);
     argnow+=3;}
    
    else if (!strcmp(av[argnow],"-fpolygon")){          // fpolygon
      V.clfpolygonthere++;
      if ( (argnow+1) > ac){
	cerr << " ERROR: Missing parameters after -fpolygon !" << endl; 
	return 1;}
      if (!(sscanf(av[argnow+1],
		   "%d",&V.clfpolygon[V.clfpolygonthere].point_num)==1)){
	cerr << " ERROR: Cannot read fpolygon point number: " << av[argnow+1];
	cerr << " ????? " << endl; return 1;}
     if ( (argnow + (2*V.clfpolygon[V.clfpolygonthere].point_num) +3) > ac){
	cerr << " ERROR: Missing parameters after -fpolygon !" << endl; 
	return 1;}
     if (V.clfpolygon[V.clfpolygonthere].point_num > 32){
	cerr << " Sorry, only 32 polygon corners allowed!" << endl; 
	return 1;}
     argnow += 2;
     int i;
     for (i=0;i<V.clfpolygon[V.clfpolygonthere].point_num;i++){
       if (!(sscanf(av[argnow],"%f",&V.clfpolygon[V.clfpolygonthere].x[i])==1)){
	cerr << " ERROR: Cannot read fpolygon point x-position: ";
	cerr << av[argnow] << " ????? " << endl; return 1;}
       if (!(sscanf(av[argnow+1],"%f",&V.clfpolygon[V.clfpolygonthere].y[i])==1)){
	cerr << " ERROR: Cannot read fpolygon point y-position: ";
	cerr << av[argnow+1] << " ????? " << endl; return 1;}
       argnow += 2;
     }
     strcpy(V.clfpolygon[V.clfpolygonthere].color,av[argnow]);
     if (!(sscanf(av[argnow+1],"%f",
		  &V.clfpolygon[V.clfpolygonthere].linethick)==1)){
       cerr << " ERROR: Cannot read fpolygon frame thickness " << av[argnow+1];
       cerr << " ????? " << endl; return 1;}
     strcpy(V.clfpolygon[V.clfpolygonthere].framecolor,av[argnow+2]);
     argnow+=3;}
    
    else if (!strcmp(av[argnow],"-pline")){          // pline
      V.clplinethere++;
      if ( (argnow+1) > ac){
	cerr << " ERROR: Missing parameters after -pline !" << endl; 
	return 1;}
      if (!(sscanf(av[argnow+1],
		   "%d",&V.clpline[V.clplinethere].point_num)==1)){
	cerr << " ERROR: Cannot read pline point number: " << av[argnow+1];
	cerr << " ????? " << endl; return 1;}
     if (V.clpline[V.clplinethere].point_num > 32){
	cerr << " Sorry, only 31 pline segments allowed!" << endl; 
	return 1;}
     if ( (argnow + (2*V.clpline[V.clplinethere].point_num) +3) > ac){
	cerr << " ERROR: Missing parameters after -pline !" << endl; 
	return 1;}
     argnow += 2;
     int i;
     for (i=0;i<V.clpline[V.clplinethere].point_num;i++){
       if (!(sscanf(av[argnow],"%f",&V.clpline[V.clplinethere].x[i])==1)){
	cerr << " ERROR: Cannot read pline point x-position: ";
	cerr << av[argnow] << " ????? " << endl; return 1;}
       if (!(sscanf(av[argnow+1],"%f",&V.clpline[V.clplinethere].y[i])==1)){
	cerr << " ERROR: Cannot read pline point y-position: ";
	cerr << av[argnow+1] << " ????? " << endl; return 1;}
       argnow += 2;
     }
     strcpy(V.clpline[V.clplinethere].linetype,av[argnow]);
     if (!(sscanf(av[argnow+1],"%f",
		  &V.clpline[V.clplinethere].linethick)==1)){
       cerr << " ERROR: Cannot read pline line thickness " << av[argnow+1];
       cerr << " ????? " << endl; return 1;}
     strcpy(V.clpline[V.clplinethere].framecolor,av[argnow+2]);
     argnow+=3;}
    
    else if (!strcmp(av[argnow],"-fpline")){          // fpline
      V.clfplinethere++;
      if ( (argnow+1) > ac){
	cerr << " ERROR: Missing parameters after -fpline !" << endl; 
	return 1;}
      if (!(sscanf(av[argnow+1],
		   "%d",&V.clfpline[V.clfplinethere].point_num)==1)){
	cerr << " ERROR: Cannot read fpline point number: " << av[argnow+1];
	cerr << " ????? " << endl; return 1;}
     if ( (argnow + (2*V.clfpline[V.clfplinethere].point_num) +3) > ac){
	cerr << " ERROR: Missing parameters after -fpline !" << endl; 
	return 1;}
     if (V.clfpline[V.clfplinethere].point_num > 32){
	cerr << " Sorry, only 31 fpline segments allowed!" << endl; 
	return 1;}
     argnow += 2;
     int i;
     for (i=0;i<V.clfpline[V.clfplinethere].point_num;i++){
       if (!(sscanf(av[argnow],"%f",&V.clfpline[V.clfplinethere].x[i])==1)){
	cerr << " ERROR: Cannot read fpline point x-position: ";
	cerr << av[argnow] << " ????? " << endl; return 1;}
       if (!(sscanf(av[argnow+1],"%f",&V.clfpline[V.clfplinethere].y[i])==1)){
	cerr << " ERROR: Cannot read fpline point y-position: ";
	cerr << av[argnow+1] << " ????? " << endl; return 1;}
       argnow += 2;
     }
     strcpy(V.clfpline[V.clfplinethere].linetype,av[argnow]);
     if (!(sscanf(av[argnow+1],"%f",
		  &V.clfpline[V.clfplinethere].linethick)==1)){
       cerr << " ERROR: Cannot read fpline line thickness " << av[argnow+1];
       cerr << " ????? " << endl; return 1;}
     strcpy(V.clfpline[V.clfplinethere].framecolor,av[argnow+2]);
     argnow+=3;}
    
    else if (!strcmp(av[argnow],"-ftriang")){          // ftriang
      V.clftriangthere++;
      if ( (argnow+9) > ac){
	cerr << " ERROR: Missing parameters after -ftriang !" << endl; 
	return 1;}
      if (!(sscanf(av[argnow+1],"%f",&V.clftriang[V.clftriangthere].x1)==1)){
	cerr << " ERROR: Cannot read ftriang x1-position: " << av[argnow+1];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+2],"%f",&V.clftriang[V.clftriangthere].y1)==1)){
	cerr << " ERROR: Cannot read ftriang y1-position: " << av[argnow+2];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+3],"%f",&V.clftriang[V.clftriangthere].x2)==1)){
	cerr << " ERROR: Cannot read ftriang x2-position: " << av[argnow+3];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+4],"%f",&V.clftriang[V.clftriangthere].y2)==1)){
	cerr << " ERROR: Cannot read ftriang y2-position: " << av[argnow+4];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+5],"%f",&V.clftriang[V.clftriangthere].x3)==1)){
	cerr << " ERROR: Cannot read ftriang x3-position: " << av[argnow+5];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+6],"%f",&V.clftriang[V.clftriangthere].y3)==1)){
	cerr << " ERROR: Cannot read ftriang y3-position: " << av[argnow+6];
	  cerr << " ????? " << endl; return 1;}
      strcpy(V.clftriang[V.clftriangthere].color,av[argnow+7]);
      if (!(sscanf(av[argnow+8],"%f",
		   &V.clftriang[V.clftriangthere].linethick)==1)){
	cerr << " ERROR: Cannot read ftriang frame thickness " << av[argnow+8];
	cerr << " ????? " << endl; return 1;}
      strcpy(V.clftriang[V.clftriangthere].framecolor,av[argnow+9]);
      argnow+=10;}

    else if (!strcmp(av[argnow],"-drawsym")){          // drawsym
      V.clsymthere++;
      if ( (argnow+7) > ac){
	cerr << " ERROR: Missing parameters after -drawsym !" << endl; 
	return 1;}
      if (!(sscanf(av[argnow+1],"%f",&V.clsym[V.clsymthere].x1)==1)){
	cerr << " ERROR: Cannot read drawsym x-position: " << av[argnow+1];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+2],"%f",&V.clsym[V.clsymthere].y1)==1)){
	cerr << " ERROR: Cannot read drawsym y-position: " << av[argnow+2];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+3],"%d",&V.clsym[V.clsymthere].symtype)==1)){
	cerr << " ERROR: Cannot read drawsym type: " << av[argnow+3];
	  cerr << " ????? " << endl; return 1;}
      strcpy(V.clsym[V.clsymthere].color,av[argnow+4]);
      if (!(sscanf(av[argnow+5],"%f",&V.clsym[V.clsymthere].radius)==1)){
	cerr << " ERROR: Cannot read drawsym size: " << av[argnow+5];
	  cerr << " ????? " << endl; return 1;}
      strcpy(V.clsym[V.clsymthere].framecolor,av[argnow+6]);
      if (!(sscanf(av[argnow+7],"%f",&V.clsym[V.clsymthere].linethick)==1)){
	cerr << " ERROR: Cannot read drawsym frame thickness " << av[argnow+7];
	  cerr << " ????? " << endl; return 1;}
      argnow+=8;}

    else if (!strcmp(av[argnow],"-drawfsym")){          // drawfsym
      V.clfsymthere++;
      if ( (argnow+7) > ac){
	cerr << " ERROR: Missing parameters after -drawfsym !" << endl; 
	return 1;}
      if (!(sscanf(av[argnow+1],"%f",&V.clfsym[V.clfsymthere].x1)==1)){
	cerr << " ERROR: Cannot read drawfsym x-position: " << av[argnow+1];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+2],"%f",&V.clfsym[V.clfsymthere].y1)==1)){
	cerr << " ERROR: Cannot read drawfsym y-position: " << av[argnow+2];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+3],"%d",&V.clfsym[V.clfsymthere].symtype)==1)){
	cerr << " ERROR: Cannot read drawfsym type: " << av[argnow+3];
	  cerr << " ????? " << endl; return 1;}
      strcpy(V.clfsym[V.clfsymthere].color,av[argnow+4]);
      if (!(sscanf(av[argnow+5],"%f",&V.clfsym[V.clfsymthere].radius)==1)){
	cerr << " ERROR: Cannot read drawfsym size: " << av[argnow+5];
	  cerr << " ????? " << endl; return 1;}
      strcpy(V.clfsym[V.clfsymthere].framecolor,av[argnow+6]);
      if (!(sscanf(av[argnow+7],"%f",&V.clfsym[V.clfsymthere].linethick)==1)){
	cerr << " ERROR: Cannot read drawfsym frame thickness " << av[argnow+7];
	  cerr << " ????? " << endl; return 1;}
      argnow+=8;}

    else if (!strcmp(av[argnow],"-BoundingBox")){          // BoundingBox
      V.BB.there++;
      if ( (argnow+4) > ac){
	cerr << " ERROR: Missing parameters after -BoundingBox !" << endl; 
	return 1;}
      if (!(sscanf(av[argnow+1],"%f",&V.BB.x1)==1)){
	cerr << " ERROR: Cannot read BoundingBox x1-position: " << av[argnow+1];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+2],"%f",&V.BB.y1)==1)){
	cerr << " ERROR: Cannot read BoundingBox y1-position: " << av[argnow+2];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+3],"%f",&V.BB.x2)==1)){
	cerr << " ERROR: Cannot read BoundingBox x2-position: " << av[argnow+3];
	  cerr << " ????? " << endl; return 1;}
      if (!(sscanf(av[argnow+4],"%f",&V.BB.y2)==1)){
	cerr << " ERROR: Cannot read BoundingBox y2-position: " << av[argnow+4];
	  cerr << " ????? " << endl; return 1;}
      argnow+=5;}

    else{                                  // unknown options, errors
      cout << "Unknown option " << av[argnow] << endl;
      return 1;}
  }

  if (!V.clfthere){
    if (!V.cldrawframe) {
      V.cldatatype=-1;
      V.clotype=-1;
      if (!V.clxthere) { V.clxthere = 1; V.clxmin = 0; V.clxmax = 20; }
      if (!V.clythere) { V.clythere = 1; V.clymin = 0; V.clymax = 30; }
    }
    else {
      cout << " ERROR: no data file given." << endl;
      return 1;
    }
  }
  if (!V.clothere){
    cout << " No filename for postscript output given, writing to out.ps";
    cout << endl;}

  return 0;
}


// ---------------------------------------- Ausgabe der Benutzerinformationen:

int Benutzerinfo(){
  cout << endl;
  cout << " data2ps   : Plotting a data file to";
  cout << " postscript." << endl << endl;
  cout << "    Version: 1.17 BETA, T.Pospischil, 4.11.2000" << endl << endl;
  cout << " usage: data2ps -f <data file name>" << endl;
  cout << "          [-o <name of postscript output file, d:out.ps>]" << endl;
  cout << "          [-portrait (otherwise default: seascape)]" << endl;
  cout << "          [-BoundingBox <x1/cm> <y1/cm> <x2/cm> <y2/cm>]" << endl;
  cout << "          [-width  <framewidth  in cm>]" << endl;
  cout << "          [-height <frameheight in cm>]" << endl;
  cout << "          [-x <X-Range min> <X-Range max>]" << endl;
  cout << "          [-y <Y-Range min> <Y-Range max>]" << endl;
  cout << "          [-xtics <min> <max> <num> <sub>]" << endl;
  cout << "          [-ytics <min> <max> <num> <sub>]" << endl;
  cout << "          [-xticscale <fac>]" << endl;
  cout << "          [-yticscale <fac>]" << endl;
  cout << "          [-xticstyle <n (0/1)>]" << endl;
  cout << "          [-yticstyle <n (0/1)>]" << endl;
  cout << "          [-xlog <style>] (log. x-axis, style (0/1) is about tics)"<<endl;
  cout << "          [-ylog <style>] (log. y-axis, style (0/1) is about tics)"<<endl;
  cout << "          [-zlog <positive min value>] (log. gray/colorscale for 2D histos)"<<endl;
  cout << "          [-title  <Title on top>]" << endl;
  cout << "          [-xtitle <Title on x axis>]" << endl;
  cout << "          [-ytitle <Title on y axis>]" << endl;
  cout << "          [-xtitledist <shift of x-Title/cm>]" << endl;
  cout << "          [-ytitledist <shift of y-Title/cm>]" << endl;
  cout << "          [-rebin <fact>] (for 1-dim. histos, fact positive integer)"<<endl;
  cout << "          [-noframe] (don't draw axis and title, works also without datafile)" << endl << endl;
  cout << "   input format (d: x y in ascii format): " << endl;
  cout << "          [-columns <ges,d:2> <xpos,d:1> <ypos,d:2> <xerr/ahx> <yerr/ahy>]" << endl;
  cout << "          [-tasty] (input file is tasty histogram)" << endl;
  cout << "          [-tasty2D] (input file is tasty2D histogram)" << endl << endl;
  cout << "   plot options: " << endl;
  cout << "          [-dots] (d, a dot per data point)" << endl;
  cout << "          [-dotsize <dot scale factor>] " << endl;
  cout << "          [-line] <linetypedef>] (full line:\"[] 0\")" << endl;
  cout << "          [-symbol] (draws symbols with errorbars)" << endl;
  cout << "          [-symbolstyle <type:1..> <color> <size> <linecolor> <linethick>]" << endl;
  cout << "          [-color <color>] (Color for line)" << endl;
  cout << "          [-histo] (output in histogram format)" << endl;
  cout << "          [-arrows <style,0/1> <color> <thick> <skip>]" << endl;
  cout << "          [-leaveplace_y <below> <above>] (in fkt-ampl.)" << endl;
  cout << "          [-gray2D] (2D-greyscale, if ascii: input format x y z)" << endl;
  cout << "          [-color2D] (2D-color, if ascii: input format x y z)" << endl << endl;
  cout << "   geometric objects (not filled -> use color \"------\"): " << endl;
  cout << "          [-arc <x1> <y1> <radius/cm> <start_ang/deg> <end_ang/deg>" << endl;
  cout << "                <color> <framethick> <framecolor>]" << endl;
  cout << "          [-box <x1> <y1> <x2> <y2/cm> <color> <framethick> <framecolor>]" << endl;
  cout << "          [-triang <x1> <y1> <x2> <y2> <x3> <y3/cm>" << endl;
  cout << "                   <color> <framethick> <framecolor>]" << endl;
  cout << "          [-circle <x1> <y1> <radius/cm> <color> <framethick> <framecolor>]" << endl;
  cout << "          [-polygon <n> <x1> <y1> ... <xn> <yn/cm>" << endl;
  cout << "                   <color> <framethick> <framecolor>]" << endl;
  cout << "          [-pline <n> <x1> <y1> ... <xn> <yn/cm>" << endl;
  cout << "                   <linetype> <linethick> <color>]" << endl;
  cout << "          [-drawline <x1> <y1> <x2> <y2/cm> <color> <linethick>]" << endl;
  cout << "          [-curve <x1> <y1> .. <x4> <y4/cm> <ltype> <color> <lthick>]" << endl;
  cout << "          [-drawsym <x> <y/cm> <type> <color> <size> <linecolor> <linethick>]" << endl;
  cout << "          [-farc .., -fbox .., -ftriang .., -fpolygon .., -fcircle ..," << endl;
  cout << "           -fpline .., -drawfline .., -drawfsym .., -fcurve ...]" << endl;
  cout << "           do the same with coords in fkt.-system" << endl << endl;
  cout << "   additionals (just one polynom, others: max. 32 per item): " << endl;
  cout << "          [-poly <n>  <a0>...<an>] (n max 9)" << endl;
  cout << "          [-addline <filename> <ges> <xpos> <ypos> <linetypedef>]" << endl;
  cout << "          [-addcline <filename> <ges> <xpos> <ypos> <linetypedef>" << endl;
  cout << "                     <color> <linewidth>]" << endl;
  cout << "          [-addhisto <filename> <ges> <xpos> <ypos> <linetypedef>" << endl;
  cout << "                     <color> <linewidth> <linecolor>]" << endl;
  cout << "          [-addsymbols <filename> <ges> <xpos> <ypos> <xerr> <yerr> <color>]" << endl;
  cout << "          [-addTsymbols <filename> <ges> <xpos> <ypos> <xerr> <yerr> <color>" << endl;
  cout << "                        <type:1..> <size> <linecolor> <linethick>]" << endl;
  cout << "          [-addarrows <filename> <ges> <xpos> <ypos> <ahx> <ahy>" << endl;
  cout << "                      <style,0/1> <color> <thick> <skip>]" << endl;
  cout << "          [-text <text> <fontsize> <x(cm)> <y(cm)> <l/r/c>]" << endl;
  cout << "          [-ctext <text> <fontname> <fontsize> <x(cm)> <y(cm)> <l/r/c> <color>]" << endl;
  cout << "          [-fkttext <text> <fontsize> <x(fkt)> <y(fkt)> <l/r/c> <color>]" << endl;
  cout << "          [-fkttextfont <fontname>]" << endl;
  cout << "          [-addbtxt <textfile> <fontname> <fontsize> <x> <y> <lineskip(cm)>]" << endl;
  cout << "          [-import <filename> <xpos/cm> <ypos/cm> <xscal> <yscal> <rot/deg>]" << endl;
  cout << "          [-importclip <filename> <clip: xmin/cm> <ymin> <xmax> <ymax>" << endl;
  cout << "                       <xpos> <ypos> <xscal> <yscal> <rot/deg>]" << endl;
  cout << "          [-fimportclip <filename> <clip: xmin/fkt> <ymin> <xmax> <ymax>" << endl;
  cout << "                       <xpos> <ypos> <xscal> <yscal> <rot/deg>]" << endl;
  cout << "                   (Import of other postscript (ps/eps) files)" << endl;
  cout << endl;
  cout << " In texts and titles: greek symbols with {s }, ZapfDingbats {z }," << endl;
  cout << " Palatino-Italic {i }, Palatino-Bold {B }, Palatino-BoldItalic {I }," << endl;
  cout << " lower index {_ }, upper index {^ }, deutsche Umlaute {u }, vector {v }." << endl;
  cout << " <color> is in X11-rgb HEX notation (i.e. green: '00ff00',..)." << endl;
  cout << endl;
  return 0;
}
