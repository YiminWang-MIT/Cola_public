// 
//    Klasse psplot:   Plotten von Funktionen, ...  in ein Postscript-File
//
//                     Version: 1.0, 21.01.99, Thomas Pospischil
//                              (Umwandlung von c-Funktionensammlung psplot.c
//                               zur c++-Klasse psplot)
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "psplot.h"


/* ----------------------------- realX ----------------------------------- */

/* Diese Funktion rechnet eine Koordinate vom Funktionskoordinatensystem ins
   Postscriptkoordinatensystem um.
*/

float 
psplot::realX( float X )
{
  if (XLog) {
    if (X==0.0) 
      return (AktMinX * cm - 1);
    else
      return (AktMinX * cm + (log(X)-log(FktMinX))/
	      (log(FktMaxX)-log(FktMinX))*((AktMaxX-AktMinX)* cm ));
  }
  return (AktMinX * cm+(X-FktMinX)/(FktMaxX-FktMinX)*((AktMaxX-AktMinX)* cm));
}

/* ----------------------------- realY ----------------------------------- */

/* Diese Funktion rechnet eine Koordinate vom Funktionskoordinatensystem ins
   Postscriptkoordinatensystem um.
*/

float 
psplot::realY( float Y )
{
  if (YLog) {
    if (Y==0.0) 
      return (AktMinY * cm - 1);
    else
      return (AktMinY * cm + (log(Y)-log(FktMinY))/
	      (log(FktMaxY)-log(FktMinY))*((AktMaxY-AktMinY)* cm ));
  }
  return (AktMinY * cm +(Y-FktMinY)*(AktMaxY * cm -AktMinY * cm)/(FktMaxY-FktMinY));
}

/* ----------------------------- ColorSet ------------------------------- */

/* Diese Funktion schreibt in die (offene) PSDatei den Postscript-Befehl
   zum setzen einer Farbe. Welche Farbe gesetzt wird, geben die ersten
   6 Zeichen des colorstr im rgb HEX Format an.
   (Bsp.: "ff0000" ist Rot, "000000" schwarz, "aa00aa" lila, usw.) 
*/

int 
psplot::ColorSet ( FILE *PSDatei,
		   char* colorstr)
{
  int red,green,blue;
  char conv[3];
  
  conv[0] = colorstr[0];
  conv[1] = colorstr[1];
  conv[2] = 0;
  if ( (!(sscanf(conv,"%x",&red)==1)) || (red > 255) ){
    fprintf(stderr,"WARNING: Invalid red value in rgb-String!\n");
    return 1;
  }

  conv[0] = colorstr[2];
  conv[1] = colorstr[3];
  conv[2] = 0;
  if ( (!(sscanf(conv,"%x",&green)==1)) || (green > 255) ){
    fprintf(stderr,"WARNING: Invalid green value in rgb-String!\n");
    return 1;
  }

  conv[0] = colorstr[4];
  conv[1] = colorstr[5];
  conv[2] = 0;
  if ( (!(sscanf(conv,"%x",&blue)==1)) || (blue > 255) ){
    fprintf(stderr,"WARNING: Invalid blue value in rgb-String!\n");
    return 1;
  }

  fprintf(PSDatei,"%6.3f %6.3f %6.3f setrgbcolor \n",
	  (red/255.0), (green/255.0), (blue/255.0) );
  return 0;
}


/* ------------------------- ClipOnFktArea ------------------------------ */

int
psplot::ClipOnFktArea(FILE* PSDatei){
  fprintf(PSDatei,"newpath %5.3f %5.3f moveto \n",AktMinX * cm, AktMinY * cm);
  fprintf(PSDatei,"%5.3f %5.3f lineto \n",AktMaxX * cm, AktMinY * cm);
  fprintf(PSDatei,"%5.3f %5.3f lineto \n",AktMaxX * cm, AktMaxY * cm);
  fprintf(PSDatei,"%5.3f %5.3f lineto \n",AktMinX * cm, AktMaxY * cm);
  fprintf(PSDatei,"closepath clip \n");
  return 0;
}


/* ----------------------------- import --------------------------------- */

/* Diese Funktion dient zum Import einer anderen Postscript-Datei. */

int
psplot::import(FILE *PSDatei,
	       char PSimportFileName[1024],
	       int ClipMode, /* 0: kein clipping, 1: rechteck */
	       float clip_xmin,
	       float clip_ymin,
	       float clip_xmax,
	       float clip_ymax,
	       float x,      /* Einfuegepunkt in cm oder Fkt-Sys. */
	       float y,
	       float xscale, /* Skalierung (bezogen auf 1.0) */
	       float yscale,
	       float rot,    /* Einfuegewinkel in Grad */
	       int fktsys)   /* Fktsys oder cm? */
{
  FILE *indatei;
  int tmpchar;
  
  if (!(indatei=fopen(PSimportFileName,"rb"))){
    fprintf(stderr,"  Could\'t open file %s !\n",PSimportFileName);
    return 1;
  }
  
  fprintf(PSDatei,"%c Import der Datei %s \n",37,PSimportFileName);
  fprintf(PSDatei,"gsave \n\n");
  
  if (ClipMode==1) {
    if (fktsys) {
      fprintf(PSDatei,"newpath %5.3f %5.3f moveto \n",
	      realX(clip_xmin), realY(clip_ymin));
      fprintf(PSDatei,"%5.3f %5.3f lineto \n",
	      realX(clip_xmax), realY(clip_ymin));
      fprintf(PSDatei,"%5.3f %5.3f lineto \n",
	      realX(clip_xmax), realY(clip_ymax));
      fprintf(PSDatei,"%5.3f %5.3f lineto \n",
	      realX(clip_xmin), realY(clip_ymax));
      fprintf(PSDatei,"closepath clip \n");
    }
    else {
      fprintf(PSDatei,"newpath %5.3f %5.3f moveto \n",
	      clip_xmin * cm, clip_ymin * cm);
      fprintf(PSDatei,"%5.3f %5.3f lineto \n",clip_xmax * cm, clip_ymin * cm);
      fprintf(PSDatei,"%5.3f %5.3f lineto \n",clip_xmax * cm, clip_ymax * cm);
      fprintf(PSDatei,"%5.3f %5.3f lineto \n",clip_xmin * cm, clip_ymax * cm);
      fprintf(PSDatei,"closepath clip \n");
    }
  }

  /* Verschiebung des Koordinatensystems: */
  if (fktsys) fprintf(PSDatei,"%.5f %.5f translate \n", realX(x), realY(y));
  else fprintf(PSDatei,"%.5f %.5f translate \n", x * cm , y * cm );
  /* Drehen des Blattes */
  fprintf(PSDatei,"%.4f rotate \n",rot);
  /* Skalieren */
  fprintf(PSDatei,"%.6f %.6f scale \n", xscale , yscale );

  fprintf(PSDatei,"BEGINEPSFILE\n");
  fprintf(PSDatei,"%c%cBeginFile: %s\n",37,37,PSimportFileName);
  while( (tmpchar=fgetc(indatei)) != EOF ) fputc(tmpchar, PSDatei);
  fprintf(PSDatei,"%c%cEndFile: %s\n",37,37,PSimportFileName);
  fprintf(PSDatei,"ENDEPSFILE\n\n");
  fclose(indatei);

  fprintf(PSDatei,"grestore \n\n");

  return 0;
 
}

/* ----------------------------- autoscale ------------------------------ */

/* Diese Funktion versucht, zu den uebergebenen Parametern Min_R und Max_R
   (im Sinne der Funktionen XTics und YTics, s.u.) vernueftige Vorschlaege
   zu den Achsenbeschriftungsparametern zu machen. Das Ergebis wird in
   Min_N, Max_N, Num_N und Sub_N zurueckgegeben.
*/

int psplot::autoscale(double Min_R, 
		      double Max_R, 
		      double* Min_N, 
		      double* Max_N, 
		      int* Num_N,
		      int* Sub_N)
{
  double G; /* Groessenordnung */
  int help;
  
  G = pow(10, floor( log10(Max_R - Min_R) ) );
  *Min_N = G * floor(Min_R/G);
  *Max_N = G * ceil(Max_R/G);
  help = int(rint( (*Max_N - *Min_N) / G));
  if (help == 2) { *Num_N = 10; *Sub_N = 1; }
  else if (help < 6) { *Num_N = 2*help; *Sub_N = 4; }
  else { *Num_N = help; *Sub_N = 9; }

  /*  printf(" G=%g help=%d => Min_N=%g Max_N=%g Num_N=%d Sub_N=%d\n",
	 G, help, *Min_N, *Max_N, *Num_N, *Sub_N); */

  return 1;
}

/* --------------------------- SuchKlammerZu --------------------------- */

/*  Unterroutine fuer TextPS : Sucht in einem Restttextstring, der nach
    einer offenen geschweiften Klammer steht, die dazugehoerige schliessende
    geschweifte Klammer. Der Reststring kann dabei beliebige andere
    verschachtelte oder aneinandergereihte geschweifte Klammerpaerchen
    beinhalten.
    Zurueckgeliefert wird ein Zeiger auf die passende Klammer.
    Im Fehlerfall wird NULL zurueckgeliefert.
*/

char* 
psplot::SuchKlammerZu(char *aText)
{
  char *AufZeiger;
  char *ZuZeiger;
  if ( aText == NULL ) return NULL;
  if ( (ZuZeiger=strchr((aText+1),'}')) == NULL ) return NULL;
  if ( (AufZeiger=strchr((aText+1),'{')) == NULL ) return ZuZeiger;
  if ( AufZeiger > ZuZeiger ) return ZuZeiger;
    if ( ZuZeiger > AufZeiger )
      return SuchKlammerZu( SuchKlammerZu(AufZeiger) );
    return NULL;
}

/* ---------------------------- getHeatColor ----------------------------- */

/* lokale Subroutine getHeatColor, verwandelt den als letztes uebergebenen
   Wert frac (zwischen 0 und 1) in 3 rgb-Werte, die in Hred, Hgreen, Hblue
   zurueckgegeben werden. */

int psplot::getHeatColor(float *Hred,
			 float *Hgreen,
			 float *Hblue,
			 float frac    )
{
  /* red: */
  if      (frac <  0.2) *Hred = 0.7*(1.0-(frac-0.07)*(frac-0.07)/0.01);
  else if (frac <  0.4) *Hred = 0.0;
  else if (frac < 0.65) *Hred = (frac-0.4)/0.2501;
  else                  *Hred = 1.0;
  if (*Hred < 0.0) *Hred = 0.0; 
  if (*Hred > 1.0) *Hred = 1.0; 

  /* green: */
  if      (frac <  0.2) *Hgreen = 0.0;
  else if (frac <  0.4) *Hgreen = (frac-0.2)/0.2001;
  else if (frac < 0.65) *Hgreen = 1.0;
  else if (frac < 0.85) *Hgreen = 1.0 - (frac-0.65)/0.2001;
  else                  *Hgreen = (frac-0.8)/0.2001;;
  if (*Hgreen < 0.0) *Hgreen = 0.0; 
  if (*Hgreen > 1.0) *Hgreen = 1.0; 

  /* blue: */
  if      (frac <  0.1) *Hblue = 1.0 - (frac-0.1)*(frac-0.1)/0.018;
  else if (frac <  0.4) *Hblue = 1.0;
  else if (frac < 0.55) *Hblue = 1.0 - (frac-0.4)/0.1501;
  else if (frac <  0.8) *Hblue = 0.0;
  else                  *Hblue = (frac-0.8)/0.2001;;
  if (*Hblue < 0.0) *Hblue = 0.0; 
  if (*Hblue > 1.0) *Hblue = 1.0; 

  return 0;
}

/* ------------------------------ TextPS --------------------------------- */

/* Durchsucht Text nach geschweiften Klammern. Wird ein Paar gefunden, so wird
   der erste Buchstabe nach { als Befehl interpretiert, der auf den Rest des
   Strings innerhalb der Klammern wirkt. Moegliche Befehle sind:

	{s..} : Benutze das Symbol-Encoding (griech. Buchstaben,...)
	{z..} : Benutze das ZapfDingbats-Encoding (Symbole)
	{i..} : Benutze Palatino-Italic
	{B..} : Benutze Palatino-Bold
	{I..} : Benutze Palatino-BoldItalic
	{^..} : verkleinerte, hochgestellte Ausgabe
	{_..} : verkleinerte, tiefgestellte Ausgabe
	{u.}  : deutscher Umlaut, z.B. {uu} = ü, {us} = ß, usw.
*/
int psplot::TextPS( FILE *PSDatei,
		    char Text[1024] )
{
  float SaveFontSize;
  char SaveFontName[64];
  char Command;
  char AnfText[1024];
  char SonText[1024];
  char EndText[1024];
  char *SonZeiger;
  char *EndZeiger;

  /* Direktbehandlung von deutschen Umlauten: */
  while ( (SonZeiger=strpbrk(Text,"äöüÄÖÜß()")) ){
    strncpy(AnfText,"",1024);
    strncpy(EndText,"",1024);
    if ( ( (int)(SonZeiger-Text) > 0 ) && ( ((char)(*(SonZeiger-1))) == 92 ) )
      strncat(AnfText,Text,(int)(SonZeiger-Text-1));
    else 
      strncat(AnfText,Text,(int)(SonZeiger-Text));
    strcat(EndText,(char *)(SonZeiger+1));
    Command=(char)(*(SonZeiger));
    switch (Command) {
    case 'ü' : sprintf(Text,"%s\\200%s",AnfText,EndText); break;
    case 'ä' : sprintf(Text,"%s\\201%s",AnfText,EndText); break;
    case 'ö' : sprintf(Text,"%s\\202%s",AnfText,EndText); break;
    case 'Ü' : sprintf(Text,"%s\\203%s",AnfText,EndText); break;
    case 'Ä' : sprintf(Text,"%s\\204%s",AnfText,EndText); break;
    case 'Ö' : sprintf(Text,"%s\\205%s",AnfText,EndText); break;
    case 'ß' : sprintf(Text,"%s\\373%s",AnfText,EndText); break;
    case '(' : sprintf(Text,"%s\\050%s",AnfText,EndText); break;
    case ')' : sprintf(Text,"%s\\051%s",AnfText,EndText); break;
    default  : sprintf(Text,"%s%c%s"   ,AnfText,Command,EndText); 
      break;
    }
  }

  /* Initialisierung: */
  strncpy(AnfText,"",1024);
  strncpy(SonText,"",1024);
  strncpy(EndText,"",1024);
  if ( (SonZeiger=strchr(Text,'{')) == NULL )
    {
      fprintf(PSDatei,"(%s) show \n",Text);
      return 1;
    }
  if ( (EndZeiger=SuchKlammerZu((char *)(SonZeiger+1))) == NULL )
    {
      fprintf(stderr," ERROR: Fehler in Text: '%s'\n",Text);
      return 1;
    }
  strncat(AnfText,Text,(int)(SonZeiger-Text));
  strncat(SonText,(char *)(SonZeiger+2),(int)(EndZeiger-SonZeiger-2));
  strcat(EndText,(char *)(EndZeiger+1));

  if (strlen(AnfText)>=1) fprintf(PSDatei,"(%s) show \n",AnfText);

  Command=(char)(*(SonZeiger+1));
  switch (Command)
    {
    case '>': 
      fprintf(PSDatei," %s 0 rmoveto\n", SonText);
      break;
    case '^': 
      SaveFontSize=FontSize;strcpy(SaveFontName,FontName);
      FontSize=FontSize/1.5;
      fprintf(PSDatei,"/%s findfont ",FontName);
      fprintf(PSDatei,"%5.3f scalefont setfont \n",FontSize);
      fprintf(PSDatei," %.2f %5.3f rmoveto \n",
	      0.0,(4.0/9.0*SaveFontSize));
      TextPS(PSDatei,SonText);
      FontSize=SaveFontSize;strcpy(FontName,SaveFontName);
      fprintf(PSDatei,"/%s findfont ",FontName);
      fprintf(PSDatei,"%5.3f scalefont setfont \n",FontSize);
      fprintf(PSDatei," %.2f %5.3f rmoveto \n",
	      0.0,(-4.0/9.0*SaveFontSize));
      break;
    case '_': 
      SaveFontSize=FontSize;strcpy(SaveFontName,FontName);
      FontSize=FontSize/1.5;
      fprintf(PSDatei,"/%s findfont ",FontName);
      fprintf(PSDatei,"%5.3f scalefont setfont \n",FontSize);
      fprintf(PSDatei," %.2f %5.3f rmoveto \n",
	      0.0,(-1.3/6.0*SaveFontSize));
      TextPS(PSDatei,SonText);
      FontSize=SaveFontSize;strcpy(FontName,SaveFontName);
      fprintf(PSDatei,"/%s findfont ",FontName);
      fprintf(PSDatei,"%5.3f scalefont setfont \n",FontSize);
      fprintf(PSDatei," %.2f %5.3f rmoveto \n",
	      0.0,(1.3/6.0*SaveFontSize));
      break;
    case 's': 
      strcpy(SaveFontName,FontName);strcpy(FontName,"Symbol");
      fprintf(PSDatei,"/%s findfont ",FontName);
      fprintf(PSDatei,"%5.3f scalefont setfont \n",FontSize);
      TextPS(PSDatei,SonText);
      strcpy(FontName,SaveFontName);
      fprintf(PSDatei,"/%s findfont ",FontName);
      fprintf(PSDatei,"%5.3f scalefont setfont \n",FontSize);
      break;
    case 'z': 
      strcpy(SaveFontName,FontName);
      strcpy(FontName,"ZapfDingbats");
      fprintf(PSDatei,"/%s findfont ",FontName);
      fprintf(PSDatei,"%5.3f scalefont setfont \n",FontSize);
      TextPS(PSDatei,SonText);
      strcpy(FontName,SaveFontName);
      fprintf(PSDatei,"/%s findfont ",FontName);
      fprintf(PSDatei,"%5.3f scalefont setfont \n",FontSize);
      break;
    case 'i': 
      strcpy(SaveFontName,FontName);
      strcpy(FontName,"Palatino-Italic");
      fprintf(PSDatei,"/%s findfont ",FontName);
      fprintf(PSDatei,"%5.3f scalefont setfont \n",FontSize);
      TextPS(PSDatei,SonText);
      strcpy(FontName,SaveFontName);
      fprintf(PSDatei,"/%s findfont ",FontName);
      fprintf(PSDatei,"%5.3f scalefont setfont \n",FontSize);
      break;
    case 'B': 
      strcpy(SaveFontName,FontName);
      strcpy(FontName,"Palatino-Bold");
      fprintf(PSDatei,"/%s findfont ",FontName);
      fprintf(PSDatei,"%5.3f scalefont setfont \n",FontSize);
      TextPS(PSDatei,SonText);
      strcpy(FontName,SaveFontName);
      fprintf(PSDatei,"/%s findfont ",FontName);
      fprintf(PSDatei,"%5.3f scalefont setfont \n",FontSize);
      break;
    case 'I': 
      strcpy(SaveFontName,FontName);
      strcpy(FontName,"Palatino-BoldItalic");
      fprintf(PSDatei,"/%s findfont ",FontName);
      fprintf(PSDatei,"%5.3f scalefont setfont \n",FontSize);
      TextPS(PSDatei,SonText);
      strcpy(FontName,SaveFontName);
      fprintf(PSDatei,"/%s findfont ",FontName);
      fprintf(PSDatei,"%5.3f scalefont setfont \n",FontSize);
      break;
    case 'v': 
      fprintf(PSDatei,"gsave ");
      fprintf(PSDatei,"/ZapfDingbats findfont ");
      fprintf(PSDatei,"%5.3f scalefont setfont \n",0.55*FontSize);
      if (SonText[0] < 'a')
	fprintf(PSDatei,"%5.3f %5.3f rmoveto \n", 0.09*FontSize, 0.7*FontSize);
      else if ((SonText[0] == 'b') ||
	       (SonText[0] == 'd') ||
	       (SonText[0] == 'f') ||
	       (SonText[0] == 'h') ||
	       (SonText[0] == 'i') ||
	       (SonText[0] == 'j') ||
	       (SonText[0] == 'k') ||
	       (SonText[0] == 'l') ||
	       (SonText[0] == 't')    )
	fprintf(PSDatei,"%5.3f %5.3f rmoveto \n", 0.03*FontSize, 0.7*FontSize);
      else
	fprintf(PSDatei,"%5.3f %5.3f rmoveto \n", 0.03*FontSize, 0.5*FontSize);
      fprintf(PSDatei,"(\\336) show\n");
      fprintf(PSDatei,"grestore ");
      TextPS(PSDatei,SonText);
      break;
    case 'u': 
      switch (SonText[0]) {
      case 'u' : fprintf(PSDatei,"(\\200) show\n"); break;
      case 'a' : fprintf(PSDatei,"(\\201) show\n"); break;
      case 'o' : fprintf(PSDatei,"(\\202) show\n"); break;
      case 'U' : fprintf(PSDatei,"(\\203) show\n"); break;
      case 'A' : fprintf(PSDatei,"(\\204) show\n"); break;
      case 'O' : fprintf(PSDatei,"(\\205) show\n"); break;
      case 's' : fprintf(PSDatei,"(\\373) show\n"); break;
      default : fprintf(PSDatei,"(%c) show\n",SonText[0]); break; 
      }
      break;
    default : fprintf(stderr," Text-Kommando %c unbekannt.\n",Command);
      return 1;
    }
  if ( strlen(EndText)>=1 ) TextPS(PSDatei,EndText);
  return 1;
}

/* ----------- Cstrip (Hilfsfunktion fuer PutText) -------------------- */

/* Entfernt zur Laengenbestimmung alle Kommandozeichen ('{c' und '}')
   aus TEXT
*/

int 
psplot::Cstrip(char Text[1024], char StrippedText[1024])
{
  char oldtext[1024];
  char cleantext[1024];
  char *Zeiger;
  strncpy(oldtext,Text,1024);
  strncpy(cleantext,"",1024);
  while ( (Zeiger=strchr(oldtext,'{')) != NULL )
    {
      strncpy(cleantext,oldtext,(int)(Zeiger-oldtext));
      strcat(cleantext,(char *)(Zeiger+2));
      strcpy(oldtext,cleantext);
      strncpy(cleantext,"",1024);
    }
  while ( (Zeiger=strchr(oldtext,'}')) != NULL )
    {
      strncpy(cleantext,oldtext,(int)(Zeiger-oldtext));
      strcat(cleantext,(char *)(Zeiger+1));
      strcpy(oldtext,cleantext);
      strncpy(cleantext,"",1024);
    }
  strcpy(StrippedText,oldtext);
  return 0;
}


/* ------------------------------ PutText -------------------------------- */

int 
psplot::PutText ( FILE *PSDatei,
		  char Text[1024],
		  char Pos    )
{
  if (Pos == 'l')
    TextPS(PSDatei,Text);
  else {
    char StrippedText[1024];
    Cstrip(Text,StrippedText);
    fprintf(PSDatei,"(%s) stringwidth \n",StrippedText);
    fprintf(PSDatei,"exch ");
    if (Pos == 'c') fprintf(PSDatei,"2 div neg ");
    if (Pos == 'r') fprintf(PSDatei,"neg ");
    fprintf(PSDatei,"exch ");
    fprintf(PSDatei,"rmoveto \n");
    TextPS(PSDatei,Text);
    fprintf(PSDatei,"\n");
  }
  return 1;
}

/* ------------------------------ PosTextPS -------------------------------- */

/* Diese Funktion gibt den String TEXT an der Stelle
   X,Y (in Funktionskoordinaten)aus. Dabei bestimmen
   der Buchstabe in POS, ob sich der Punkt X,Y in der linken unteren
   Ecke (l=left), unten in der Mitte (c=center) oder rechts unten (r=right)
   von TEXT befindet.
*/

int
psplot::PosTextPS ( FILE *PSDatei,
		    char Text[1024],
		    float x,
		    float y,
		    char Pos    )
{
  fprintf(PSDatei,"%5.2f %5.2f moveto ",realX(x),realY(y));
  PutText(PSDatei,Text,Pos);
  return 1;
}

/* ------------------------------ PosCTextPS -------------------------------- */

/* Wie PosTextPS nur fuer farbige Texte.
*/

int 
psplot::PosCTextPS ( FILE *PSDatei,
		     char Text[1024],
		     float x,
		     float y,
		     char Pos,
		     char *color)
{
  fprintf(PSDatei,"gsave \n");
  ColorSet(PSDatei,color);
  PosTextPS(PSDatei, Text, x, y, Pos);
  fprintf(PSDatei,"grestore \n\n");
  return 1;
}

/* ------------------------------ OutTextPS -------------------------------- */

/* Diese Funktion macht dasselbe wie PosTextPS mit dem Unterschied, das hier
   die Koordinaten im PS-Koordinatensystem angegeben werden.
*/

int
psplot::OutTextPS ( FILE *PSDatei,
		    char Text[1024],
		    float x,
		    float y,
		    char Pos    )
{
  fprintf(PSDatei,"%5.2f %5.2f moveto ",x,y);
  PutText(PSDatei,Text,Pos);
  return 1;
}

/* ------------------------------ OutCTextPS -------------------------------- */

/* Wie OutTextPS, nur fuer farbige Texte.
*/

int
psplot::OutCTextPS ( FILE *PSDatei,
		     char Text[1024],
		     float x,
		     float y,
		     char Pos,
		     char *color)
{
  fprintf(PSDatei,"gsave \n");
  ColorSet(PSDatei,color);
  OutTextPS(PSDatei, Text, x, y, Pos);
  fprintf(PSDatei,"grestore \n\n");
  return 1;
}

/* ++++++++++++ Geometrische Objekte: */

/* ------------------------------ boxPS -------------------------------- */

int
psplot::boxPS ( FILE *PSDatei,
		float x1,          /* linke untere Ecke in cm */
		float y1,
		float x2,          /* rechte obere Ecke in cm */
		float y2,
		char *boxcolor,
		float framethick,
		char *framecolor,
		int fkt)
{
  fprintf(PSDatei,"%c --------------- Box: \n",37);
  fprintf(PSDatei,"gsave \n");
  ColorSet(PSDatei,framecolor);
  fprintf(PSDatei,"%.3f setlinewidth newpath \n",framethick);
  if (fkt) {
    fprintf(PSDatei,"%.3f %.3f moveto \n", realX(x1), realY(y1));
    fprintf(PSDatei,"%.3f %.3f lineto \n", realX(x2), realY(y1));
    fprintf(PSDatei,"%.3f %.3f lineto \n", realX(x2), realY(y2));
    fprintf(PSDatei,"%.3f %.3f lineto \n", realX(x1), realY(y2));
  }
  else {
    fprintf(PSDatei,"%.3f %.3f moveto \n", (x1 * cm ), (y1 * cm ));
    fprintf(PSDatei,"%.3f %.3f lineto \n", (x2 * cm ), (y1 * cm ));
    fprintf(PSDatei,"%.3f %.3f lineto \n", (x2 * cm ), (y2 * cm ));
    fprintf(PSDatei,"%.3f %.3f lineto \n", (x1 * cm ), (y2 * cm ));
  }
  fprintf(PSDatei,"closepath\n");
  if (boxcolor[0] != '-') {
    fprintf(PSDatei,"gsave \n");
    ColorSet(PSDatei,boxcolor);
    fprintf(PSDatei,"fill grestore \n");
  }
  fprintf(PSDatei,"stroke grestore \n\n");

  return 1;
}


/* ------------------------------ polygonPS -------------------------------- */

int 
psplot::polygonPS( FILE *PSDatei,
		   int num,
		   float* x,          /* Eckpunkte */
		   float* y,
		   char *boxcolor,
		   float framethick,
		   char *framecolor,
		   int fkt)           /* 0:cm, sonst:im Funktions-Koord. */
{
  int i;
  fprintf(PSDatei,"%c --------------- Polygon: \n",37);
  fprintf(PSDatei,"gsave \n");
  ColorSet(PSDatei,framecolor);
  fprintf(PSDatei,"%.3f setlinewidth newpath \n",framethick);
  if (num<2) return 0;
  if (fkt) {
    fprintf(PSDatei,"%.3f %.3f moveto \n", realX(x[0]), realY(y[0]));
    for(i=1;i<num;i++)
      fprintf(PSDatei,"%.3f %.3f lineto \n", realX(x[i]), realY(y[i]));
  }
  else {
    fprintf(PSDatei,"%.3f %.3f moveto \n", (x[0] * cm ), (y[0] * cm ));
    for(i=1;i<num;i++)
      fprintf(PSDatei,"%.3f %.3f lineto \n", (x[i] * cm ), (y[i] * cm )); 
  }
  fprintf(PSDatei,"closepath\n");
  if (boxcolor[0] != '-') {
    fprintf(PSDatei,"gsave \n");
    ColorSet(PSDatei,boxcolor);
    fprintf(PSDatei,"fill grestore \n");
  }
  fprintf(PSDatei,"stroke grestore \n\n");
  
  return 1;
}

/* ------------------------------ plinePS -------------------------------- */

int 
psplot::plinePS( FILE *PSDatei,
		 int num,
		 float* x,          /* Eckpunkte */
		 float* y,
		 char *linetype,
		 float framethick,
		 char *framecolor,
		 int fkt)           /* 0:cm, sonst:im Funktions-Koord. */
{
  int i;
  fprintf(PSDatei,"%c --------------- pline: \n",37);
  fprintf(PSDatei,"gsave %s setdash\n", linetype);
  ColorSet(PSDatei,framecolor);
  fprintf(PSDatei,"%.3f setlinewidth newpath \n",framethick);
  if (num<2) return 0;
  if (fkt) {
    fprintf(PSDatei,"%.3f %.3f moveto \n", realX(x[0]), realY(y[0]));
    for(i=1;i<num;i++)
      fprintf(PSDatei,"%.3f %.3f lineto \n", realX(x[i]), realY(y[i]));
  }
  else {
    fprintf(PSDatei,"%.3f %.3f moveto \n", (x[0] * cm ), (y[0] * cm ));
    for(i=1;i<num;i++)
      fprintf(PSDatei,"%.3f %.3f lineto \n", (x[i] * cm ), (y[i] * cm )); 
  }
  fprintf(PSDatei,"stroke grestore \n\n");
  
  return 1;
}

/* ------------------------------ triangPS -------------------------------- */

int
psplot::triangPS ( FILE *PSDatei,
		   float x1,          /* erste Ecke */
		   float y1,
		   float x2,          /* zweite Ecke */
		   float y2,
		   float x3,          /* dritte Ecke */
		   float y3,
		   char *boxcolor,
		   float framethick,
		   char *framecolor,
		   int fkt)           /* 0:cm, sonst:im Funktions-Koord. */
{
  fprintf(PSDatei,"%c --------------- Triangle: \n",37);
  fprintf(PSDatei,"gsave \n");
  ColorSet(PSDatei,framecolor);
  fprintf(PSDatei,"%.3f setlinewidth newpath \n",framethick);
  if (fkt) {
    fprintf(PSDatei,"%.3f %.3f moveto \n", realX(x1), realY(y1));
    fprintf(PSDatei,"%.3f %.3f lineto \n", realX(x2), realY(y2));
    fprintf(PSDatei,"%.3f %.3f lineto \n", realX(x3), realY(y3));
  }
  else {
    fprintf(PSDatei,"%.3f %.3f moveto \n", (x1 * cm ), (y1 * cm ));
    fprintf(PSDatei,"%.3f %.3f lineto \n", (x2 * cm ), (y2 * cm ));
    fprintf(PSDatei,"%.3f %.3f lineto \n", (x3 * cm ), (y3 * cm ));
  }
  fprintf(PSDatei,"closepath\n");
  if (boxcolor[0] != '-') {
    fprintf(PSDatei,"gsave \n");
    ColorSet(PSDatei,boxcolor);
    fprintf(PSDatei,"fill grestore \n");
  }
  fprintf(PSDatei,"stroke grestore \n\n");

  return 1;
}

/* ------------------------------ circlePS -------------------------------- */

int
psplot::circlePS ( FILE *PSDatei,
		   float x1,          /* Lage des Mittelpunkts in cm */
		   float y1,
		   float radius,      /* Radius in cm */
		   char *circcolor,
		   float framethick,
		   char *framecolor,
		   int fkt)
{
  fprintf(PSDatei,"%c --------------- Circle : \n",37);
  fprintf(PSDatei,"gsave \n");
  ColorSet(PSDatei,framecolor);
  fprintf(PSDatei,"%.3f setlinewidth\n",framethick);
  if (fkt) {
    fprintf(PSDatei,"newpath %.3f %.3f %.2f 0 360 arc \n",
	    realX(x1), realY(y1), (radius * cm ));
  }
  else {
    fprintf(PSDatei,"newpath %.3f %.3f %.2f 0 360 arc \n",
	    (x1 * cm ), (y1 * cm ), (radius * cm ));
  }
  fprintf(PSDatei,"closepath\n");
  if (circcolor[0] != '-') {
    fprintf(PSDatei,"gsave \n");
    ColorSet(PSDatei,circcolor);
    fprintf(PSDatei,"fill grestore \n");
  }
  fprintf(PSDatei,"stroke grestore \n\n");

  return 1;
}

/* ------------------------------ arcPS -------------------------------- */

int
psplot::arcPS ( FILE *PSDatei,
		float x1,          /* Lage des Mittelpunkts in cm */
		float y1,
		float radius,      /* Radius in cm */
		float startangle,
		float endangle,
		char *arccolor,
		float framethick,
		char *framecolor,
		int fkt)
{
  // Feature: wenn radius kleiner Null wird nur der Bogen gezeichnet:
  int only_ang=0;
  if (radius < 0) {
    only_ang = 1;
    radius *= -1.0;
  }

  fprintf(PSDatei,"%c --------------- Arc : \n",37);
  fprintf(PSDatei,"gsave \n");
  ColorSet(PSDatei,framecolor);
  fprintf(PSDatei,"%.3f setlinewidth\n",framethick);
  if (fkt) {
    fprintf(PSDatei,"newpath %.3f %.3f %.2f %.2f %.2f arc \n",
	    realX(x1), realY(y1), (radius * cm ), startangle, endangle);
    if (!only_ang)
      fprintf(PSDatei,"%.3f %.3f lineto\n", realX(x1), realY(y1));
  }
  else {
    fprintf(PSDatei,"newpath %.3f %.3f %.2f %.2f %.2f arc \n",
	    (x1 * cm ), (y1 * cm ), (radius * cm ), startangle, endangle);
    if (!only_ang)
      fprintf(PSDatei,"%.3f %.3f lineto\n", (x1 * cm ), (y1 * cm ) );
  }
  if (!only_ang)
    fprintf(PSDatei,"closepath\n");
  if (arccolor[0] != '-') {
    fprintf(PSDatei,"gsave \n");
    ColorSet(PSDatei,arccolor);
    fprintf(PSDatei,"fill grestore \n");
  }
  fprintf(PSDatei,"stroke grestore \n\n");

  return 1;
}

/* ------------------------------ linePS -------------------------------- */

int
psplot::linePS ( FILE *PSDatei,
		 float x1, 
		 float y1,
		 float x2, 
		 float y2,
		 char *linecolor,
		 float thick,
		 int fkt)
{
  fprintf(PSDatei,"%c --------------- Line: \n",37);
  fprintf(PSDatei,"gsave \n");
  ColorSet(PSDatei,linecolor);
  fprintf(PSDatei,"%.3f setlinewidth newpath \n", thick);
  if (fkt) {
    fprintf(PSDatei,"%.3f %.3f moveto \n", realX(x1), realY(y1));
    fprintf(PSDatei,"%.3f %.3f lineto \n", realX(x2), realY(y2));
  }
  else {
    fprintf(PSDatei,"%.3f %.3f moveto \n", (x1 * cm ), (y1 * cm ));
    fprintf(PSDatei,"%.3f %.3f lineto \n", (x2 * cm ), (y2 * cm ));
  }
  fprintf(PSDatei,"stroke grestore \n\n");

  return 1;
}

/* ------------------------------ curvePS -------------------------------- */

int
psplot::curvePS ( FILE *PSDatei,
		 float x1, 
		 float y1,
		 float x2, 
		 float y2,
		 float x3, 
		 float y3,
		 float x4, 
		 float y4,
		 char *linetype,
		 char *linecolor,
		 float thick,
		 int fkt)
{
  fprintf(PSDatei,"%c --------------- Bezier-Kurve: \n",37);
  fprintf(PSDatei,"gsave %s setdash\n", linetype);
  ColorSet(PSDatei,linecolor);
  fprintf(PSDatei,"%.3f setlinewidth newpath \n", thick);
  if (fkt) {
    fprintf(PSDatei,"%.3f %.3f moveto \n", realX(x1), realY(y1));
    fprintf(PSDatei,"%.3f %.3f %.3f %.3f %.3f %.3f curveto \n", 
	    realX(x2), realY(y2), realX(x3), realY(y3), realX(x4), realY(y4));
  }
  else {
    fprintf(PSDatei,"%.3f %.3f moveto \n", (x1 * cm ), (y1 * cm ));
    fprintf(PSDatei,"%.3f %.3f %.3f %.3f %.3f %.3f curveto \n", 
	    (x2 * cm ), (y2 * cm ), (x3 * cm ), (y3 * cm ), 
	    (x4 * cm ), (y4 * cm ) );
  }
  fprintf(PSDatei,"stroke grestore \n\n");

  return 1;
}

/* --------------------- DefineUmlauteInFont ------------------------------- */

/* Definiert in der Postscript-Datei einen Font, der von dem Postscript Font
   <NewFontName> abgeleitet ist, den Namen New-<NewFontName> bekommt, und
   zusaetzlich zum Normalfont die deutschen Umlaute enthaelt.
*/

int
psplot::DefineUmlauteInFont(FILE *PSDatei,
			    char *NewFontName)
{
  fprintf(PSDatei,"%c ---- Definieren des Fonts New-%s :\n",37,NewFontName);
  fprintf(PSDatei,"/%s findfont dup maxlength dict\n",NewFontName);
  fprintf(PSDatei,"/newdict exch def\n");
  fprintf(PSDatei,"{1 index /FID ne {newdict 3 1 roll put}\n");
  fprintf(PSDatei,"{pop pop} ifelse } forall\n\n");
  fprintf(PSDatei,"newdict /Encoding get dup length array copy\n");
  fprintf(PSDatei,"newdict /Encoding 3 -1 roll put\n\n");
  fprintf(PSDatei,"newdict begin\n");
  fprintf(PSDatei,"Encoding 8#200 /udieresis put\n");
  fprintf(PSDatei,"Encoding 8#201 /adieresis put\n");
  fprintf(PSDatei,"Encoding 8#202 /odieresis put\n");
  fprintf(PSDatei,"Encoding 8#203 /Udieresis put\n");
  fprintf(PSDatei,"Encoding 8#204 /Adieresis put\n");
  fprintf(PSDatei,"Encoding 8#205 /Odieresis put\n");
  fprintf(PSDatei,"end\n");
  fprintf(PSDatei,"newdict /FontName (New-%s) put\n",NewFontName);
  fprintf(PSDatei,"/New-%s newdict definefont pop\n\n",NewFontName);

  return 0;
}

/* ----------------------------- SetFontPS -------------------------------- */

/* Diese Funktion setzt den Font FONTNAME als aktuellen Font und skaliert ihn
   mit FONTSCALE. ( FONTSCALE hat Punkte als Einheiten )
*/

int
psplot::SetFontPS( FILE *PSDatei,
		   char *NewFontName,
		   float FontScale    )
{
  int i;
  int found = 0;
  if ( strcmp("Symbol",NewFontName) && strcmp("ZapfDingbats",NewFontName) ) {
    for(i=0;i<UmlFontDefNum;i++)
      if ( !(strcmp(UmlFontDefNames[i],NewFontName)) ) { 
	found = 1; 
	break; 
      }
    if ( !(found) ) {
      DefineUmlauteInFont(PSDatei,NewFontName);
      UmlFontDefNames[UmlFontDefNum] = (char *) malloc(64*sizeof(char));
      strncpy(UmlFontDefNames[UmlFontDefNum],NewFontName,63);
      UmlFontDefNum++; 
    }
    sprintf(FontName,"New-%s",NewFontName);
  }
  else sprintf(FontName,"%s",NewFontName);
  FontSize = FontScale;
  fprintf(PSDatei,"%c Setzen des aktuellen Fonts \n",37);
  fprintf(PSDatei,"/%s findfont \n",FontName);
  fprintf(PSDatei,"%5.3f scalefont setfont \n",FontSize);
  fprintf(PSDatei,"\n");
  return 1;
}

/* ------------------------------- openPS --------------------------------- */

/* Diese Funktion eroeffnet ein Postscript-File mit dem in FILENAME
   uebergebenen Filenamen und sorgt fuer einen minimalen EPS-Rahmen.
   Zusaetzlich wird das Blatt im Bedarfsfall (Variable: Orientation)
   auf Querformat gedreht und die aktuelle Liniendicke gesetzt.
*/

int
psplot::openPS( char *filename,
		FILE **PSDatei   )

{ /* Feststellen von aktuellem Datum und Uhrzeit */
  time_t *tp,t;
  struct tm *tl;
  tp = (time_t *) malloc(sizeof(time_t));
  tl = (struct tm *) malloc(sizeof(struct tm));
  if ((t=time(tp)) == -1)
    fprintf(stderr,"Can't get current time");
  tl=localtime(tp);

  /* Oeffnen und schreiben */
  if (!(*PSDatei=fopen(filename,"wb"))) {
    fprintf(stderr," Cannot open %s for POSTSCRIPT output!\n", filename);
    exit(1);
  }
  fprintf(*PSDatei,"%c!PS-Adobe-2.0 EPSF-2.0 \n",37);
  fprintf(*PSDatei,"%c%cCreator: PSPLOT++, V 1.0 by T.Pospischil\n",37,37);
  fprintf(*PSDatei,"%c%cCreationDate: %d.%d.%d at %d:%d \n"
          ,37,37,tl->tm_mday,tl->tm_mon+1,tl->tm_year,tl->tm_hour,tl->tm_min);
  fprintf(*PSDatei,"%c%cBoundingBox: %5.2f %5.2f %5.2f %5.2f \n",
	  37,37, PS_bb_x1, PS_bb_y1, PS_bb_x2, PS_bb_y2);
  fprintf(*PSDatei,"\n");

  /* Macros zum umdefinieren von showpage */
  fprintf(*PSDatei,"%c Marcos, die showpage fuer Importdateien umdef.:\n",37);
  fprintf(*PSDatei,"/BEGINEPSFILE\n");
  fprintf(*PSDatei,"{  /EPSFsave save def\n");
  fprintf(*PSDatei,"   0 setgray 0 setlinecap 1 setlinewidth\n");
  fprintf(*PSDatei,"   0 setlinejoin 10 setmiterlimit [] 0 setdash\n");
  fprintf(*PSDatei,"   newpath\n");
  fprintf(*PSDatei,"   /showpage {} def\n");
  fprintf(*PSDatei,"} bind def\n");
  fprintf(*PSDatei,"/ENDEPSFILE\n");
  fprintf(*PSDatei,"{  EPSFsave restore\n");
  fprintf(*PSDatei,"} bind def\n\n");

  /* Vordefinition des Zeichensatzes: */
  strcpy(FontName,"Helvetica");
  SetFontPS(*PSDatei,"Helvetica",14);

  /* Drehen des Blattes */
  if (Orientation) {
    AktMinX = 3.5;
    AktMaxX = 18.5;
    AktMinY = 4;
    AktMaxY = 26.5;
  }
  else {
    fprintf(*PSDatei,"%c Drehen des Druckbereiches auf Querformat \n",37);
    fprintf(*PSDatei,"-90 rotate \n");
    fprintf(*PSDatei,"-841.95 0 translate \n");
    fprintf(*PSDatei,"\n");
  }
  fprintf(*PSDatei,"%.3f setlinewidth \n",PSLineWidth);
  fprintf(*PSDatei,"\n");
  return 1;
}

/* ------------------------------- closePS --------------------------------- */

/* Diese Funktion schliesst das Postscript-File PSDATEI. Vorher wird
   das obligatorische "showpage" ergaenzt.
*/

int
psplot::closePS( FILE *PSDatei )
{
  fprintf(PSDatei,"\n");
  fprintf(PSDatei,"showpage \n");
  fclose(PSDatei);
  return 1;
}

/* ------------------------------- InitFramePS ---------------------------- */

/* Diese Funktion zeichnet einen Rahmen um das aktive Gebiet, das durch die
   Globalen Konstanten AKTMINX, AKTMINY, AKTMAXX und AKTMAXY festgelegt wird.
*/

int
psplot::InitFramePS( FILE *PSDatei )
{
  fprintf(PSDatei,"%c Zeichnen des Rahmens \n",37);
  fprintf(PSDatei,"newpath \n");
  fprintf(PSDatei,"%5.3f %5.3f moveto \n",AktMinX * cm, AktMinY * cm);
  fprintf(PSDatei,"%5.3f %5.3f lineto \n",AktMaxX * cm, AktMinY * cm);
  fprintf(PSDatei,"%5.3f %5.3f lineto \n",AktMaxX * cm, AktMaxY * cm);
  fprintf(PSDatei,"%5.3f %5.3f lineto \n",AktMinX * cm, AktMaxY * cm);
  fprintf(PSDatei,"closepath stroke \n");
  fprintf(PSDatei,"\n");
  return 1;
}

/* ------------------------------- XTics --------------------------------- */

/* Diese Funktion zeichnet von XMIN bis XMAX ANZAHL viele beschriftete
   Unterteilungen an die beiden horizontalen Linien des Rahmens, mit
   SUBANZAHL vielen unbeschrifteten Unter-Unterteilungen.
*/

int
psplot::XTics ( FILE *PSDatei,
		double XMin,
		double XMax,
		int anzahl,
		int subanzahl,
		float xticscale)
{ 
  int i;
  int doText;
  double dum;
  char ScalZahl[20];
  fprintf(PSDatei,"%c Zeichnen der X-Skalierung \n",37);
  fprintf(PSDatei,"gsave\n");
  fprintf(PSDatei,"/n {newpath} def\n");
  fprintf(PSDatei,"/m {moveto} def\n");
  fprintf(PSDatei,"/l {lineto} def\n");
  fprintf(PSDatei,"/s {stroke} def\n");
  fprintf(PSDatei,"/sg {setgray} def\n");

  if (anzahl<0) { doText=0; anzahl *= -1; } else doText=1;

  for(i=0;i<=((anzahl*(subanzahl+1)));i++)
    { if (XLog)
	{
	  if ( (i % (subanzahl+1) ) != 0 )
	    dum = (1 + i%(subanzahl+1)) 
	      * (double)(exp((log(XMin) 
			      + (i-i%(subanzahl+1))*(log(XMax)-log(XMin))
			      /(anzahl*(subanzahl+1)))));
	  else
	    dum=(double)(exp((log(XMin) + i*(log(XMax)-log(XMin))
			      /(anzahl*(subanzahl+1)))));
	}
      else 
	dum = (double)(XMin + i * (XMax-XMin)/(anzahl*(subanzahl+1)));

      if ((dum>=FktMinX) && (dum <=FktMaxX))
	if ( (i % (subanzahl+1) ) != 0 ) { 
	  fprintf(PSDatei,"%.3f sg ", PSTicColor);
	  if (XTicStyle==1) {
	    fprintf(PSDatei,"[2 2] 0 setdash ");
	    fprintf(PSDatei,"n %5.3f %5.3f m ", realX(dum),AktMinY * cm);
	    fprintf(PSDatei,"%5.3f %5.3f l s ", realX(dum),AktMaxY * cm);
	    fprintf(PSDatei,"[] 0 setdash\n");
	  }
	  else {
	    fprintf(PSDatei,"n %5.3f %5.3f m ",
		    realX(dum),AktMinY * cm);
	    fprintf(PSDatei,"%5.3f %5.3f l s\n",
		    realX(dum), AktMinY * cm + TicLen/2 );
	    fprintf(PSDatei,"n %5.3f %5.3f m ",
		    realX(dum),AktMaxY * cm);
	    fprintf(PSDatei,"%5.3f %5.3f l s ",
		    realX(dum), AktMaxY * cm - TicLen/2 );
	  }
	  fprintf(PSDatei,"%.3f sg\n", 0.0);
	}
	else { 
	  if (XTicStyle==1) {
	    fprintf(PSDatei,"n %5.3f %5.3f m ", realX(dum),AktMinY * cm);
	    fprintf(PSDatei,"%5.3f %5.3f l s ", realX(dum),AktMaxY * cm);
	  }
	  else {
	    fprintf(PSDatei,"%.3f sg ", PSTicColor);
	    fprintf(PSDatei,"n %5.3f %5.3f m ",
		    realX(dum),AktMinY * cm);
	    fprintf(PSDatei,"%5.3f %5.3f l s\n",
		    realX(dum), AktMinY * cm + TicLen );
	    fprintf(PSDatei,"n %5.3f %5.3f m ",
		    realX(dum),AktMaxY * cm);
	    fprintf(PSDatei,"%5.3f %5.3f l s ",
		    realX(dum), AktMaxY * cm - TicLen );
	  }
	  fprintf(PSDatei,"%.3f sg\n", 0.0);
	  if (doText) {
	    if (XLog10expStyle) sprintf(ScalZahl,"10{^%.3g}",
					xticscale*log10((float)dum));
	    else sprintf(ScalZahl,"%.6g",xticscale*(float)dum);
	    OutTextPS(PSDatei,ScalZahl,realX(dum),
		      AktMinY * cm - 1.2 * FontSize,'c');
	  }
	}
    }

  fprintf(PSDatei,"grestore\n");
  fprintf(PSDatei,"\n");
  return 1;
}

/* ------------------------------- YTics --------------------------------- */

/* Diese Funktion zeichnet von YMIN bis YMAX ANZAHL viele beschriftete
   Unterteilungen und SUBANZAHL viele unbeschriftete Unter-Unterteilungen
   an die beiden vertikalen Linien des Rahmens.
*/

int
psplot::YTics ( FILE *PSDatei,
		double YMin,
		double YMax,
		int anzahl,
		int subanzahl,
		float yticscale)
{ 
  int i;
  int doText;
  double dum;
  char ScalZahl[20];
  fprintf(PSDatei,"%c Zeichnen der Y-Skalierung \n",37);
  fprintf(PSDatei,"gsave\n");
  fprintf(PSDatei,"/n {newpath} def\n");
  fprintf(PSDatei,"/m {moveto} def\n");
  fprintf(PSDatei,"/l {lineto} def\n");
  fprintf(PSDatei,"/s {stroke} def\n");
  fprintf(PSDatei,"/sg {setgray} def\n");

  if (anzahl<0) { doText=0; anzahl *= -1; } else doText=1;
 
  for(i=0;i<=((anzahl*(subanzahl+1)));i++)
    { if (YLog)
	{
	  if ( (i % (subanzahl+1) ) != 0 )
	    dum = (1 + i%(subanzahl+1)) 
	      * (double)(exp((log(YMin) 
			      + (i-i%(subanzahl+1))*(log(YMax)-log(YMin))
			      /(anzahl*(subanzahl+1)))));
	  else
	    dum=(double)(exp((log(YMin) + i*(log(YMax)-log(YMin))
			      /(anzahl*(subanzahl+1)))));
	}
      else 
	dum = (double)(YMin + i * (YMax-YMin)/(anzahl*(subanzahl+1)));

      if ((dum>=FktMinY) && (dum <=FktMaxY))
	if ( (i % (subanzahl+1) ) != 0 ) {
	  fprintf(PSDatei,"%.3f sg ", PSTicColor);
	  if (YTicStyle==1) {
	    fprintf(PSDatei,"[2 2] 0 setdash ");
	    fprintf(PSDatei,"n %5.3f %5.3f m ", AktMinX * cm, realY(dum));
	    fprintf(PSDatei,"%5.3f %5.3f l s ", AktMaxX * cm, realY(dum));
	    fprintf(PSDatei,"[] 0 setdash\n");
	  }
	  else {
	    fprintf(PSDatei,"n %5.3f %5.3f m ",
		    AktMinX * cm, realY(dum) );
	    fprintf(PSDatei,"%5.3f %5.3f l s\n",
		    AktMinX * cm + TicLen/2, realY(dum) );
	    fprintf(PSDatei,"n %5.3f %5.3f m ",
		    AktMaxX * cm, realY(dum) );
	    fprintf(PSDatei,"%5.3f %5.3f l s ",
		    AktMaxX * cm - TicLen/2, realY(dum) );
	  }
	  fprintf(PSDatei,"%.3f sg\n", 0.0);
	}
	else { 
	  fprintf(PSDatei,"%.3f sg ", PSTicColor);
	  if (YTicStyle==1) {
	    fprintf(PSDatei,"n %5.3f %5.3f m ", AktMinX * cm, realY(dum));
	    fprintf(PSDatei,"%5.3f %5.3f l s ", AktMaxX * cm, realY(dum));
	  }
	  else {
	    fprintf(PSDatei,"n %5.3f %5.3f m ",
		    AktMinX * cm, realY(dum) );
	    fprintf(PSDatei,"%5.3f %5.3f l s \n",
		    AktMinX * cm + TicLen, realY(dum) );
	    fprintf(PSDatei,"n %5.3f %5.3f m ",
		    AktMaxX * cm, realY(dum) );
	    fprintf(PSDatei,"%5.3f %5.3f l s ",
		    AktMaxX * cm - TicLen, realY(dum) );
	  }
	  fprintf(PSDatei,"%.3f sg\n", 0.0);
	  if (doText) {
	    if (YLog10expStyle) sprintf(ScalZahl,"10{^%.3g}",
					yticscale*log10((float)dum));
	    else sprintf(ScalZahl,"%.6g",yticscale*(float)dum);
	    OutTextPS(PSDatei,ScalZahl,(AktMinX-0.3) * cm,
		      realY(dum)-FontSize/4,'r');
	  }
	}
    }
  fprintf(PSDatei,"grestore\n");
  fprintf(PSDatei,"\n");
  return 1;
}

/* ------------------------------- XTitle --------------------------------- */

/* Diese Funktion gibt unterhalb der Mitte der X-Achse TEXT aus
*/

int
psplot::XTitle( FILE *PSDatei,
		char *Text         )
{
  fprintf(PSDatei,"%c Ausgabe der X-Achsen-Beschriftung \n",37);
  OutTextPS(PSDatei,Text,AktMinX * cm + (AktMaxX * cm -AktMinX * cm)/2,
		    AktMinY * cm - XTitleDist * cm,'c');
  fprintf(PSDatei,"\n");
  return 1;
}

/* ------------------------------- YTitle --------------------------------- */

/* Diese Funktion gibt in der Mitte der Y-Achse TEXT (vertikal) aus
*/

int
psplot::YTitle( FILE *PSDatei,
		char *Text         )
{
  fprintf(PSDatei,"%c Ausgabe der Y-Achsen-Beschriftung \n",37);
  fprintf(PSDatei,"gsave ");
  fprintf(PSDatei,"%.3f %.3f translate \n",(AktMinX-YTitleDist) * cm,
			 ((AktMinY+ ((AktMaxY-AktMinY))/2) * cm));
  fprintf(PSDatei,"90 rotate \n");
  OutTextPS(PSDatei,Text,0,0,'c');
  fprintf(PSDatei,"grestore \n");
  fprintf(PSDatei,"\n");
  return 1;
}

/* ----------------------------- PlotDataPS ------------------------------- */

/* Diese Funktion liest von DataFile X-Y-Wertepaare ein und zeichnet mit
   ihnen als Stuetzpunkte eine Kurve die Linientyp als Muster verwendet.
   Bsp.: Linientyp=    "[] 0" : voller Strich
                      "[2] 0" : abwechselnd 2 voll 2 leer
		    "[6 2] 2" : 6 voll, 2 leer, beginnen mit 4 voll
                       ^   ^
		   Muster  Anzahl der am Anfang uebersprungenen Einheiten

   AnzSpalt gibt die Anzahl der Werte pro Zeile in DataFile an.
   XSpalt gibt an welcher davon die X-Koordinate und YSpalt welcher die
   Y-Koordinate ist.
*/

int
psplot::PlotDataPS( FILE *PSDatei,
		    char *DataFile,
		    char *Linientyp,
		    int AnzSpalt,
		    int XSpalt,
		    int YSpalt )
{
  FILE *PlotFile;
  float z[20];
  int i;
  if (!(PlotFile=fopen(DataFile,"r"))) { 
    printf(" Cannot open DATA FILE: %s\n",DataFile); return 1; }
  if (fscanf(PlotFile,"%f",&z[1]) != EOF)
    for(i=2;i<=AnzSpalt;i++) fscanf(PlotFile,"%f",&z[i]);
  fprintf(PSDatei,"%c Plotten der Datei %s",37,DataFile);
  fprintf(PSDatei," Spalte %d (y) gegen Spalte %d (x)\n",YSpalt,XSpalt);
  fprintf(PSDatei,"gsave %s setdash\n", Linientyp);
  ClipOnFktArea(PSDatei);
  ColorSet(PSDatei,PSGlobalColor);
  fprintf(PSDatei,"newpath %.3f %.3f moveto \n",realX(z[XSpalt]),realY(z[YSpalt]));

  while (fscanf(PlotFile,"%f",&z[1]) != EOF)
    {
      for(i=2;i<=AnzSpalt;i++) fscanf(PlotFile,"%f",&z[i]);
      fprintf(PSDatei,"%.3f %.3f lineto \n",realX(z[XSpalt]),realY(z[YSpalt]));
    }
  fprintf(PSDatei,"stroke grestore \n");
  fprintf(PSDatei,"\n");
  fclose(PlotFile);
  return 0;
}
 
/* ----------------------------- CPlotDataPS ------------------------------- */

/* Diese Funktion liest von DataFile X-Y-Wertepaare ein und zeichnet mit
   ihnen als Stuetzpunkte eine Kurve die Linientyp als Muster verwendet.
   Bsp.: Linientyp=    "[] 0" : voller Strich
                      "[2] 0" : abwechselnd 2 voll 2 leer
		    "[6 2] 2" : 6 voll, 2 leer, beginnen mit 4 voll
                       ^   ^
		   Muster  Anzahl der am Anfang uebersprungenen Einheiten

   AnzSpalt gibt die Anzahl der Werte pro Zeile in DataFile an.
   XSpalt gibt an welcher davon die X-Koordinate und YSpalt welcher die
   Y-Koordinate ist.

   Im Unterschied zur Funktion PlotDataPS wird noch eine Farbe und eine
   Liniendicke an die Funktion uebergeben.
*/

int
psplot::CPlotDataPS( FILE *PSDatei,
		     char *DataFile,
		     char *Linientyp,
		     int AnzSpalt,
		     int XSpalt,
		     int YSpalt,
		     char* color,
		     float linewidth)
{
  FILE *PlotFile;
  float z[20];
  int i;
  if (!(PlotFile=fopen(DataFile,"r"))) { 
    printf(" Cannot open DATA FILE: %s\n",DataFile); return 1; }
  if (fscanf(PlotFile,"%f",&z[1]) != EOF)
    for(i=2;i<=AnzSpalt;i++) fscanf(PlotFile,"%f",&z[i]);
  fprintf(PSDatei,"%c Plotten der Datei %s",37,DataFile);
  fprintf(PSDatei," Spalte %d (y) gegen Spalte %d (x)\n",YSpalt,XSpalt);
  fprintf(PSDatei,"gsave %s setdash\n", Linientyp);
  ClipOnFktArea(PSDatei);
  ColorSet(PSDatei,color);
  fprintf(PSDatei,"%.3f setlinewidth newpath \n",linewidth);

  fprintf(PSDatei,"\n\n%.3f %.3f moveto \n",realX(z[XSpalt]),realY(z[YSpalt]));

  while (fscanf(PlotFile,"%f",&z[1]) != EOF)
    {
      for(i=2;i<=AnzSpalt;i++) fscanf(PlotFile,"%f",&z[i]);
      fprintf(PSDatei,"%.3f %.3f lineto \n",realX(z[XSpalt]),realY(z[YSpalt]));
    }
  fprintf(PSDatei,"stroke grestore \n");
  fprintf(PSDatei,"\n");
  fclose(PlotFile);
  return 0;
}
 
/* ----------------------------- BlockTextPS ------------------------------- */

/* Diese Funktion stellt den gesamten Text der Datei TextFileName dar.
   Die erste Zeile wird am Punkt (ulx,uly) (in cm) begonnen, die folgenden
   Zeilen liegen dann jeweils im Abstand lineskip (auch in cm) darunter.

   Verwendet wird der Postscript-Font mit dem Namen ThisFontName (z.B.
   "Courier", "Palatino-Roman", "Palatino-Bold", "Times-Roman", ...) in der
   Skalierung ThisFontSize (Standardwerte: 6.0-50.0).

   Wie in den anderen Text-Funktionen koennen mit z.B. {s..} (Symbol-Encoding
   i.a. griech. Buchstaben,...), {z..} (ZapfDingbats-Encoding, Symbole), 
   {^..} (verkleinerte, hochgestellte Ausgabe), {_..} (tiefgestellt), ...
   Textteile oder Einzelbuchstaben veraendert dargestellt werden.
*/

int
psplot::BlockTextPS( FILE *PSDatei,
		     char *TextFileName,
		     char *ThisFontName,
		     float ThisFontSize,
		     float ulx,
		     float uly,
		     float lineskip)
{
  FILE *TextFile;
  char inputline[1024];
  char OldFontName[64];
  float OldFontSize;
  float x,y;

  if (!(TextFile=fopen(TextFileName,"r"))) { 
    fprintf(stderr," Cannot open TEXT FILE: %s\n",TextFileName); return 1; }
  if (fgets(inputline,1024,TextFile)==NULL) {
    fprintf(stderr," WARNING: Text file: %s empty!\n",TextFileName); return 2; }

  fprintf(PSDatei,"%c Block Text der Datei %s\n",37,TextFileName);
  fprintf(PSDatei,"gsave\n");

  strcpy(OldFontName,FontName);
  OldFontSize = FontSize;
  SetFontPS(PSDatei, ThisFontName, ThisFontSize);

  x = ulx * cm;
  y = uly * cm;
  OutTextPS(PSDatei,inputline,x,y,'l');
  y -= lineskip * cm;
  while (fgets(inputline,1024,TextFile)!=NULL)
    {
      OutTextPS(PSDatei,inputline,x,y,'l');
      y -= lineskip * cm;
    }

  fprintf(PSDatei,"grestore \n");
  fprintf(PSDatei,"\n");
  strcpy(FontName,OldFontName);
  FontSize = OldFontSize;
  fclose(TextFile);
  return 1;
}


/* ----------------------------- AbsArrowDataPS ---------------------------- */

/* Diese Funktion liest von DataFile X-Y-Wertepaare ein und zeichnet mit
   ihnen als Stuetzpunkte eine Kurve die aus Pfeilen vom jeweiligen
   Datenpunkt zum naechsten besteht.

   AnzSpalt gibt die Anzahl der Werte pro Zeile in DataFile an.
   XSpalt gibt an welcher davon die X-Koordinate und YSpalt welcher die
   Y-Koordinate ist.

   arthick gibt die Dicke der Pfeile an und wird im Regelfall im Bereich
   1..5 liegen. Ist arskip ungleich 0, so werden nicht alle Pfeile gezeichnet,
   sondern immer arskip viele uebersprungen. arcolor gibt die Farbe der
   Pfeile im rgb HEX Format an (Bsp.: "ff0000" ist Rot, "000000" schwarz, 
   "aa00aa" lila, usw.) */

int
psplot::AbsArrowDataPS( FILE *PSDatei,
			char *DataFile,
			int AnzSpalt,
			int XSpalt,
			int YSpalt,
			float arthick,
			float arskip,
			char* arcolor)
{
  FILE *PlotFile;
  float z[20];
  int i,count;
  if (!(PlotFile=fopen(DataFile,"r"))) { 
    printf(" Cannot open DATA FILE: %s\n",DataFile); return 1; }
  if (fscanf(PlotFile,"%f",&z[1]) != EOF)
    for(i=2;i<=AnzSpalt;i++) fscanf(PlotFile,"%f",&z[i]);
  fprintf(PSDatei,"%c AbsArrowPlot der Datei %s",37,DataFile);
  fprintf(PSDatei," Spalte %d (y) gegen Spalte %d (x)\n",YSpalt,XSpalt);
  fprintf(PSDatei,"gsave\n");
  ClipOnFktArea(PSDatei);
  ColorSet(PSDatei,arcolor);

  /* Arrow Definition: */
  fprintf(PSDatei,"\n%c Definition des Pfeiles:\n",37);
  fprintf(PSDatei,"/dicke %f def \n",arthick);
  fprintf(PSDatei,"/tilhead 0.6 def \n");
  fprintf(PSDatei,"/arrowto {  \n");
  fprintf(PSDatei," /y2 exch def /x2 exch def  \n");
  fprintf(PSDatei," currentpoint /y1 exch def /x1 exch def \n");
  fprintf(PSDatei," /dy y2 y1 sub def \n");
  fprintf(PSDatei," /dx x2 x1 sub def \n");
  fprintf(PSDatei," dx dup mul dy dup mul add sqrt /betrag exch def \n");
  fprintf(PSDatei," /ndx dx betrag div def \n");
  fprintf(PSDatei," /ndy dy betrag div def \n");
  fprintf(PSDatei," newpath \n");
  fprintf(PSDatei," x1 y1 moveto \n");
  fprintf(PSDatei," dicke ndy mul -1.0 dicke mul ndx mul rlineto \n");
  fprintf(PSDatei," tilhead dx mul tilhead dy mul rlineto \n");
  fprintf(PSDatei," 1.5 dicke mul ndy mul -1.5 dicke mul ndx mul rlineto \n");
  fprintf(PSDatei," x2 y2 lineto closepath fill \n");
  fprintf(PSDatei," newpath \n");
  fprintf(PSDatei," x1 y1 moveto \n");
  fprintf(PSDatei," -1.0 dicke mul ndy mul dicke ndx mul rlineto \n");
  fprintf(PSDatei," tilhead dx mul tilhead dy mul rlineto \n");
  fprintf(PSDatei," -1.5 dicke mul ndy mul 1.5 dicke mul ndx mul rlineto \n");
  fprintf(PSDatei," x2 y2 lineto closepath fill 	 \n");
  fprintf(PSDatei,"} def \n\n");

  fprintf(PSDatei,"%.3f %.3f moveto \n",realX(z[XSpalt]),realY(z[YSpalt]));

  count = int(arskip);
  while (fscanf(PlotFile,"%f",&z[1]) != EOF)
    {
      for(i=2;i<=AnzSpalt;i++) fscanf(PlotFile,"%f",&z[i]);
      count++;
      if (count>arskip){
	fprintf(PSDatei,"%.3f %.3f arrowto\n",
		realX(z[XSpalt]),realY(z[YSpalt]));
	count=0;
      }
      fprintf(PSDatei,"%.3f %.3f moveto\n",
	      realX(z[XSpalt]),realY(z[YSpalt]));
    }
  fprintf(PSDatei,"grestore \n");
  fprintf(PSDatei,"\n");
  fclose(PlotFile);
  return 0;
}


/* ----------------------------- RelArrowDataPS ---------------------------- */

/* Diese Funktion liest von DataFile X-Y-AX-AY Werte-Quadros ein und zeichnet
   von den Punkten X-Y Pfeile nach AX-AY, wobei AX und AY relativ zu X-Y
   zu sehen sind.

   AnzSpalt gibt die Anzahl der Werte pro Zeile in DataFile an.
   XSpalt gibt an, welcher davon die X-Koordinate, YSpalt welcher die
   Y-Koordinate, AHXSpalt welcher die Pfeilspitzen-X-Koordinate und
   AHYSpalt welcher Pfeilspitzen-Y-Koordinate ist.

   arthick gibt die Dicke der Pfeile an und wird im Regelfall im Bereich
   1..5 liegen. Ist arskip ungleich 0, so werden nicht alle Pfeile gezeichnet,
   sondern immer arskip viele uebersprungen. arcolor gibt die Farbe der
   Pfeile im rgb HEX Format an (Bsp.: "ff0000" ist Rot, "000000" schwarz, 
   "aa00aa" lila, usw.) 
   31.01.2000: Ist arskip negativ, so gilt es als Mindestabstand
   zwischen zwei Pfeilen (cm), dazwischenliegende werden nicht gezeichnet. 
*/

int
psplot::RelArrowDataPS( FILE *PSDatei,
			char *DataFile,
			int AnzSpalt,
			int XSpalt,
			int YSpalt,
			int AHXSpalt,
			int AHYSpalt,
			float arthick,
			float arskip,
			char* arcolor)
{
  FILE *PlotFile;
  float z[20];
  float lastx, lasty;
  int i,count;
  if (!(PlotFile=fopen(DataFile,"r"))) { 
    printf(" Cannot open DATA FILE: %s\n",DataFile); return 1; }
  if (fscanf(PlotFile,"%f",&z[1]) != EOF)
    for(i=2;i<=AnzSpalt;i++) fscanf(PlotFile,"%f",&z[i]);
  fprintf(PSDatei,"%c RelArrowPlot der Datei %s",37,DataFile);
  fprintf(PSDatei," Spalte %d (y) gegen Spalte %d (x)\n",YSpalt,XSpalt);
  fprintf(PSDatei,"gsave\n");
  ClipOnFktArea(PSDatei);
  ColorSet(PSDatei,arcolor);

  int drawframe = 0;
  if (arthick < 0) { drawframe = 1; arthick *= -1.0; }

  /* Arrow Definition: */
  fprintf(PSDatei,"\n%c Definition des Pfeiles:\n",37);
  fprintf(PSDatei,"/dicke %f def \n",arthick);
  fprintf(PSDatei,"/tilhead 0.6 def \n");
  fprintf(PSDatei,"/lenhead -0.4 def \n");
  fprintf(PSDatei,"/arrowto {  \n");
  fprintf(PSDatei," /y2 exch def /x2 exch def  \n");
  fprintf(PSDatei," currentpoint /y1 exch def /x1 exch def \n");
  fprintf(PSDatei," /dy y2 y1 sub def \n");
  fprintf(PSDatei," /dx x2 x1 sub def \n");
  fprintf(PSDatei," dx dup mul dy dup mul add sqrt /betrag exch def \n");
  fprintf(PSDatei," /ndx dx betrag div def \n");
  fprintf(PSDatei," /ndy dy betrag div def \n");
  fprintf(PSDatei," newpath \n");
  fprintf(PSDatei," x1 y1 moveto \n");
  fprintf(PSDatei," dicke ndy mul -1.0 dicke mul ndx mul rlineto \n");
  fprintf(PSDatei," tilhead dx mul tilhead dy mul rlineto \n");
  fprintf(PSDatei," 1.5 dicke mul ndy mul -1.5 dicke mul ndx mul rlineto \n");
  fprintf(PSDatei," x2 y2 lineto \n");
  fprintf(PSDatei," lenhead dx mul -2.5 dicke mul ndy mul add\n");
  fprintf(PSDatei," lenhead dy mul  2.5 dicke mul ndx mul add");
  fprintf(PSDatei," rlineto\n");
  fprintf(PSDatei," 1.5 dicke mul ndy mul -1.5 dicke mul ndx mul rlineto \n");
  fprintf(PSDatei," -1.0 tilhead mul dx mul -1.0 tilhead mul dy mul rlineto\n");
  fprintf(PSDatei," closepath");
  if (drawframe)
    fprintf(PSDatei," gsave %f setlinewidth 0 setgray stroke grestore",
	    arthick/3.0);
  fprintf(PSDatei," fill\n");
  fprintf(PSDatei,"} def \n\n");

//   fprintf(PSDatei," closepath fill\n");
//   fprintf(PSDatei," newpath x1 y1 moveto \n");
//   fprintf(PSDatei," -1.0 dicke mul ndy mul dicke ndx mul rlineto \n");
//   fprintf(PSDatei," tilhead dx mul tilhead dy mul rlineto \n");
//   fprintf(PSDatei," -1.5 dicke mul ndy mul 1.5 dicke mul ndx mul rlineto \n");
//   fprintf(PSDatei," x2 y2 lineto ");
//   fprintf(PSDatei," closepath fill\n");
//   fprintf(PSDatei,"} def \n\n");

  /* Ausgabe des ersten Pfeils: */
  lastx = realX(z[XSpalt]); lasty = realY(z[YSpalt]); 
  if  ( (fabs(realX(z[XSpalt]+z[AHXSpalt])-realX(z[XSpalt])) > 0.001) ||
	(fabs(realY(z[YSpalt]+z[AHYSpalt])-realY(z[YSpalt])) > 0.001)  ){
    fprintf(PSDatei,"%.3f %.3f moveto %.3f %.3f arrowto\n",
	    realX(z[XSpalt]),realY(z[YSpalt]), 
	    realX(z[XSpalt]+z[AHXSpalt]),realY(z[YSpalt]+z[AHYSpalt]));
  }
  
  count = 0;
  while (fscanf(PlotFile,"%f",&z[1]) != EOF)
    {
      for(i=2;i<=AnzSpalt;i++) fscanf(PlotFile,"%f",&z[i]);
      count++;
      if (arskip<0){
	if ( ( sqrt((realX(z[XSpalt])-lastx)*(realX(z[XSpalt])-lastx)+  
		    (realY(z[YSpalt])-lasty)*(realY(z[YSpalt])-lasty) )
	       + arskip * cm ) > 0 ){
	  lastx = realX(z[XSpalt]); lasty = realY(z[YSpalt]);
	  if ((fabs(realX(z[XSpalt]+z[AHXSpalt])-realX(z[XSpalt])) > 0.001) ||
	      (fabs(realY(z[YSpalt]+z[AHYSpalt])-realY(z[YSpalt])) > 0.001)  ){
	    fprintf(PSDatei,"%.3f %.3f moveto %.3f %.3f arrowto\n",
		    realX(z[XSpalt]),realY(z[YSpalt]), 
		    realX(z[XSpalt]+z[AHXSpalt]),realY(z[YSpalt]+z[AHYSpalt]));
	  }
	}
      } 
      else {
	if (count>arskip){
	  count=0;
	  if ((fabs(realX(z[XSpalt]+z[AHXSpalt])-realX(z[XSpalt])) > 0.001) ||
	      (fabs(realY(z[YSpalt]+z[AHYSpalt])-realY(z[YSpalt])) > 0.001)  ){
	    fprintf(PSDatei,"%.3f %.3f moveto %.3f %.3f arrowto\n",
		    realX(z[XSpalt]),realY(z[YSpalt]), 
		    realX(z[XSpalt]+z[AHXSpalt]),realY(z[YSpalt]+z[AHYSpalt]));
	  }
	}
      }
    }
  /* Ausgabe des letzten Pfeils: */
  if (count>0){
    if  ( (fabs(realX(z[XSpalt]+z[AHXSpalt])-realX(z[XSpalt])) > 0.001) ||
	  (fabs(realY(z[YSpalt]+z[AHYSpalt])-realY(z[YSpalt])) > 0.001)  ){
      fprintf(PSDatei,"%.3f %.3f moveto %.3f %.3f arrowto\n",
	      realX(z[XSpalt]),realY(z[YSpalt]), 
	      realX(z[XSpalt]+z[AHXSpalt]),realY(z[YSpalt]+z[AHYSpalt]));
      count=0;
    }
  }
  fprintf(PSDatei,"grestore \n");
  fprintf(PSDatei,"\n");
  fclose(PlotFile);
  return 0;
}


/* ----------------------------- HistoDataPS ------------------------------- */

/* Diese Funktion liest von DataFile X-Y-Wertepaare ein und zeichnet mit
   ihnen als Stuetzpunkte ein Histogramm, dessen Konturen den Linientyp 
   als Muster verwendet und dessen Flaeche mit einem Grauton der Helligkeit
   grey gefuellt werden.

   Bsp.: Linientyp=    "[] 0" : voller Strich
                      "[2] 0" : abwechselnd 2 voll 2 leer
		    "[6 2] 2" : 6 voll, 2 leer, beginnen mit 4 voll
                       ^   ^
		   Muster  Anzahl der am Anfang uebersprungenen Einheiten

   Bsp.: grey     =        1  : Weisses Histogramm (ungefuellt)
                  =        0  : voll schwarz gefuelltes Histogramm

   AnzSpalt gibt die Anzahl der Werte pro Zeile in DataFile an.
   XSpalt gibt an welcher davon die X-Koordinate und YSpalt welcher die
   Y-Koordinate ist.
*/

int
psplot::HistoDataPS(FILE *PSDatei,
		    char *DataFile,
		    char *Linientyp,
		    double grey,
		    int AnzSpalt,
		    int XSpalt,
		    int YSpalt )
{
  float MiX=(AktMinX-(AktMaxX-AktMinX)/10) * cm;
  float MaX=(AktMaxX+(AktMaxX-AktMinX)/10) * cm;

  FILE *PlotFile;
  float z[20],yold=0,xbinini=1,xold=0;
  int i;

  if (!(PlotFile=fopen(DataFile,"r"))) { 
    printf(" Cannot open DATA FILE: %s\n",DataFile); return 1; }
  if (fscanf(PlotFile,"%f",&z[1]) != EOF)
    for(i=2;i<=AnzSpalt;i++) fscanf(PlotFile,"%f",&z[i]);
  fprintf(PSDatei,"%c Plotten des Histogramms %s",37,DataFile);
  fprintf(PSDatei," Spalte %d (y) gegen Spalte %d (x)\n",YSpalt,XSpalt);
  fprintf(PSDatei,"gsave %s setdash\n", Linientyp);
  ClipOnFktArea(PSDatei);
  fprintf(PSDatei,"newpath %.3f %.3f moveto \n",realX(z[XSpalt]),realY(0.0));
  fprintf(PSDatei,"%.3f %.3f lineto \n",realX(z[XSpalt]),realY(z[YSpalt]));
  yold=realY(z[YSpalt]);

  while (fscanf(PlotFile,"%f",&z[1]) != EOF)
    { 
      for(i=2;i<=AnzSpalt;i++) fscanf(PlotFile,"%f",&z[i]);
      if ( (realX(z[XSpalt])<MiX) || (realX(z[XSpalt])>MaX) ) continue; 
      xbinini=realX(z[XSpalt])-xold;
      xold=realX(z[XSpalt]);
      fprintf(PSDatei,"%.3f %.3f lineto \n",realX(z[XSpalt]),yold);
      fprintf(PSDatei,"%.3f %.3f lineto \n",realX(z[XSpalt]),realY(z[YSpalt]));
      yold=realY(z[YSpalt]);
    }
  fprintf(PSDatei,"%.3f %.3f lineto \n",xold+xbinini,yold);
  fprintf(PSDatei,"%.3f %.3f lineto \n",xold+xbinini,realY(0.0));

  fprintf(PSDatei,"closepath \n");
  fprintf(PSDatei,"gsave %.3f setgray fill grestore \n",grey);

  fprintf(PSDatei,"stroke grestore \n");
  fprintf(PSDatei,"\n");
  fclose(PlotFile);
  return 0;
}


/* ---------------------------- CHistoDataPS ------------------------------- */
/* Wie HistoDataPS, nur anstatt Grauton eine Farbe */

int
psplot::CHistoDataPS(FILE *PSDatei,
		     char *DataFile,
		     char *Linientyp,
		     int AnzSpalt,
		     int XSpalt,
		     int YSpalt,
		     char *HistoColor,
		     char *LineColor,
		     double Linewidth)
{
  float MiX=(AktMinX-(AktMaxX-AktMinX)/10) * cm;
  float MaX=(AktMaxX+(AktMaxX-AktMinX)/10) * cm;
  
  FILE *PlotFile;
  float z[20],yold=0,xbinini=1,xold=0;
  int i;
  
  if (!(PlotFile=fopen(DataFile,"r"))) { 
    printf(" Cannot open DATA FILE: %s\n",DataFile); return 1; }
  if (fscanf(PlotFile,"%f",&z[1]) != EOF)
    for(i=2;i<=AnzSpalt;i++) fscanf(PlotFile,"%f",&z[i]);
  fprintf(PSDatei,"%c Plotten des Histogramms %s",37,DataFile);
  fprintf(PSDatei," Spalte %d (y) gegen Spalte %d (x)\n",YSpalt,XSpalt);
  fprintf(PSDatei,"gsave %s setdash\n", Linientyp);
  fprintf(PSDatei,"%.3f setlinewidth\n", Linewidth);
  ClipOnFktArea(PSDatei);
  ColorSet(PSDatei,LineColor);
  fprintf(PSDatei,"newpath %.3f %.3f moveto \n",realX(z[XSpalt]),realY(0.0));
  fprintf(PSDatei,"%.3f %.3f lineto \n",realX(z[XSpalt]),realY(z[YSpalt]));
  yold=realY(z[YSpalt]);

  while (fscanf(PlotFile,"%f",&z[1]) != EOF)
    { 
      for(i=2;i<=AnzSpalt;i++) fscanf(PlotFile,"%f",&z[i]);
      if ( (realX(z[XSpalt])<MiX) || (realX(z[XSpalt])>MaX) ) continue; 
      xbinini=realX(z[XSpalt])-xold;
      xold=realX(z[XSpalt]);
      fprintf(PSDatei,"%.3f %.3f lineto \n",realX(z[XSpalt]),yold);
      fprintf(PSDatei,"%.3f %.3f lineto \n",realX(z[XSpalt]),realY(z[YSpalt]));
      yold=realY(z[YSpalt]);
    }
  fprintf(PSDatei,"%.3f %.3f lineto \n",xold+xbinini,yold);
  fprintf(PSDatei,"%.3f %.3f lineto \n",xold+xbinini,realY(0.0));

  fprintf(PSDatei,"closepath \n");
  if (HistoColor[0] != '-') {
    fprintf(PSDatei,"gsave \n");
    ColorSet(PSDatei,HistoColor);
    fprintf(PSDatei,"fill grestore \n");
  }
  fprintf(PSDatei,"stroke grestore \n");
  fprintf(PSDatei,"\n");
  fclose(PlotFile);
  return 0;
}


/* ----------------------------- Histo2DDataPS ----------------------------- */

/* Diese Funktion liest von DataFile X-Y-Z-Wertepaare ein und zeichnet mit
   ihnen als Stuetzpunkte ein Graustufen-Histogramm, wobei der groesste
   Z-Wert die Farbe Schwarz abgibt und die Werte dazwischen linear 
   bei zlog == 0 und logarithmisch von zlog != 0 zu weiss
   hin abgestuft werden.

   AnzSpalt gibt die Anzahl der Werte pro Zeile in DataFile an.
   XSpalt gibt an welcher davon die X-Koordinate, YSpalt welcher die
   Y-Koordinate und ZSpalt welcher die Z-Koordinate ist.

   binX und binY geben die Breite des grauen Kastens an, der an dem
   Datenpunkt gezeichnet wird
*/

int
psplot::Histo2DDataPS(FILE *PSDatei,
		      char *DataFile,
		      int AnzSpalt,
		      int XSpalt,
		      int YSpalt,
		      int ZSpalt,
		      float binX,
		      float binY,
		      float zlog)
{
  FILE *PlotFile;
  float r[20];
  float minZ=0;
  float maxZ=0;
  float MiX, MaX, MiY, MaY;
  int i;
  float grayval=1.0;

  if (!(PlotFile=fopen(DataFile,"r"))) { 
    printf(" Cannot open DATA FILE: %s\n",DataFile); return 1; }
  if (fscanf(PlotFile,"%f",&r[1]) != EOF)
    for(i=2;i<=AnzSpalt;i++) fscanf(PlotFile,"%f",&r[i]);
  else return 2;
  minZ=0.0;
  maxZ=r[ZSpalt];
  MiX = FktMinX - binX;
  MaX = FktMaxX + binX;
  MiY = FktMinY - binY;
  MaY = FktMaxY + binY;
  while (fscanf(PlotFile,"%f",&r[1]) != EOF)
    { 
      for(i=2;i<=AnzSpalt;i++) fscanf(PlotFile,"%f",&r[i]);
      if ( (r[XSpalt]<MiX) || (r[XSpalt]>MaX) ) continue; 
      if ( (r[YSpalt]<MiY) || (r[YSpalt]>MaY) ) continue; 
      if (maxZ < r[ZSpalt]) maxZ=r[ZSpalt];
      if (minZ > r[ZSpalt]) minZ=r[ZSpalt];
    }

  if (maxZ<=minZ) maxZ=minZ+1.0;

  fprintf(PSDatei,"%c Plotten des 2D-Histogramms %s\n",37,DataFile);
  fprintf(PSDatei,"%c Spalte %d (y) gegen Spalte %d (x),",37,YSpalt,XSpalt);
  fprintf(PSDatei," Werte: Spalte %d (z) \n",ZSpalt);
  fprintf(PSDatei,"gsave\n");
  fprintf(PSDatei,"/b {moveto %.3f 0 rlineto 0 %.3f rlineto -%.3f 0 rlineto closepath} def \n", (realX(r[XSpalt]+binX) - realX(r[XSpalt])), (realY(r[YSpalt]+binY) - realY(r[YSpalt])), (realX(r[XSpalt]+binX) - realX(r[XSpalt])));
  fprintf(PSDatei,"/n {newpath} def \n");
  fprintf(PSDatei,"/g {setgray fill} def \n");
  ClipOnFktArea(PSDatei);

  rewind(PlotFile);
  while (fscanf(PlotFile,"%f",&r[1]) != EOF)
    { 
      for(i=2;i<=AnzSpalt;i++) fscanf(PlotFile,"%f",&r[i]);
      if ( (r[XSpalt]<MiX) || (r[XSpalt]>MaX) ) continue; 
      if ( (r[YSpalt]<MiY) || (r[YSpalt]>MaY) ) continue; 
      grayval = 1.0;
      if (zlog){
	if (r[ZSpalt] > zlog) 
	  grayval = 1.0 - (log(r[ZSpalt])-log(zlog))/(log(maxZ)-log(zlog));
      }
      else 
	grayval = 1.0 - (r[ZSpalt]-minZ)/(maxZ-minZ);
      if (grayval < 1.0 )
	fprintf(PSDatei,"n %.2f %.2f b %.3f g\n", realX(r[XSpalt]),
	      realY(r[YSpalt]),grayval);
    }
  fprintf(PSDatei,"grestore \n");
  fprintf(PSDatei,"\n");
  fclose(PlotFile);
  return 0;
}

/* ----------------------------- Color2DDataPS ----------------------------- */

/* Wie Histo2DDataPS, nur Farbe statt Grautoenen
*/

int
psplot::Color2DDataPS(FILE *PSDatei,
		      char *DataFile,
		      int AnzSpalt,
		      int XSpalt,
		      int YSpalt,
		      int ZSpalt,
		      float binX,
		      float binY,
		      float zlog)
{
  FILE *PlotFile;
  float r[20];
  float minZ=0;
  float maxZ=0;
  float MiX, MaX, MiY, MaY;
  int i;
  float Hred, Hgreen, Hblue;
  float grayval=1.0;

  if (!(PlotFile=fopen(DataFile,"r"))) { 
    printf(" Cannot open DATA FILE: %s\n",DataFile); return 1; }
  if (fscanf(PlotFile,"%f",&r[1]) != EOF)
    for(i=2;i<=AnzSpalt;i++) fscanf(PlotFile,"%f",&r[i]);
  else return 2;
  minZ=0.0;
  maxZ=r[ZSpalt];
  MiX = FktMinX - binX;
  MaX = FktMaxX + binX;
  MiY = FktMinY - binY;
  MaY = FktMaxY + binY;
  while (fscanf(PlotFile,"%f",&r[1]) != EOF)
    { 
      for(i=2;i<=AnzSpalt;i++) fscanf(PlotFile,"%f",&r[i]);
      if ( (r[XSpalt]<MiX) || (r[XSpalt]>MaX) ) continue; 
      if ( (r[YSpalt]<MiY) || (r[YSpalt]>MaY) ) continue; 
      if (maxZ < r[ZSpalt]) maxZ=r[ZSpalt];
      if (minZ > r[ZSpalt]) minZ=r[ZSpalt];
    }

  if (maxZ<=minZ) maxZ=minZ+1.0;
  fprintf(PSDatei,"%c Farb-Plot des 2D-Histogramms %s\n",37,DataFile);
  fprintf(PSDatei,"%c Spalte %d (y) gegen Spalte %d (x),",37,YSpalt,XSpalt);
  fprintf(PSDatei," Werte: Spalte %d (z) \n",ZSpalt);
  fprintf(PSDatei,"gsave\n");
  fprintf(PSDatei,"/b {moveto %.3f 0 rlineto 0 %.3f rlineto -%.3f 0 rlineto closepath} def \n", (realX(r[XSpalt]+binX) - realX(r[XSpalt])), (realY(r[YSpalt]+binY) - realY(r[YSpalt])), (realX(r[XSpalt]+binX) - realX(r[XSpalt])));
  fprintf(PSDatei,"/n {newpath} def \n");
  fprintf(PSDatei,"/sr {setrgbcolor fill} def \n");
  ClipOnFktArea(PSDatei);

  /* Hintergrund mit schwarz fuellen: */
  fprintf(PSDatei,"n %5.3f %5.3f moveto \n",AktMinX * cm, AktMinY * cm);
  fprintf(PSDatei,"%5.3f %5.3f lineto \n",AktMaxX * cm, AktMinY * cm);
  fprintf(PSDatei,"%5.3f %5.3f lineto \n",AktMaxX * cm, AktMaxY * cm);
  fprintf(PSDatei,"%5.3f %5.3f lineto \n",AktMinX * cm, AktMaxY * cm);
  fprintf(PSDatei,"closepath 0.0 0.0 0.0 sr\n");
  PSTicColor=1.0;

  /* Datei zeichnen: */
  rewind(PlotFile);
  while (fscanf(PlotFile,"%f",&r[1]) != EOF)
    { 
      for(i=2;i<=AnzSpalt;i++) fscanf(PlotFile,"%f",&r[i]);
      if ( (r[XSpalt]<MiX) || (r[XSpalt]>MaX) ) continue; 
      if ( (r[YSpalt]<MiY) || (r[YSpalt]>MaY) ) continue; 
      grayval = 0.0;
      if (zlog){
	if (r[ZSpalt] > zlog) 
	  grayval = (log(r[ZSpalt])-log(zlog))/(log(maxZ)-log(zlog));}
      else 
	grayval = (r[ZSpalt]-minZ)/(maxZ-minZ);
      if (grayval > 0.0 ){
	getHeatColor(&Hred, &Hgreen, &Hblue, grayval ); 
	fprintf(PSDatei,"n %.2f %.2f b ", realX(r[XSpalt]), realY(r[YSpalt]));
	fprintf(PSDatei,"%.3f %.3f %.3f sr\n", Hred, Hgreen, Hblue);
      }
    }
  fprintf(PSDatei,"grestore \n");
  fprintf(PSDatei,"\n");
  fclose(PlotFile);
  return 0;
}


/* ----------------------------- DotDataPS ------------------------------- */

/* Diese Funktion liest von DataFile X-Y-Wertepaare ein und zeichnet
   an ihre Position einen kleinen Kreis.
   AnzSpalt gibt die Anzahl der Werte pro Zeile in DataFile an.
   XSpalt gibt an welcher davon die X-Koordinate und YSpalt welcher die
   Y-Koordinate ist.
   dXSpalt und dYSpalt geben die Nummern derjenigen Spalten an, in denen die
   Fehler fuer die X- und die Y-Koordinate stehen. Werden in diesen Variablen
   Werte kleiner Null uebergeben, so werden keine Fehlerbalken gezeichnet.
   Auch wenn die Fehlerbalken kleiner als SymbolSize sind, werden sie
   weggelassen.
*/

int
psplot::DotDataPS( FILE *PSDatei,
		   char *DataFile,
		   int AnzSpalt,
		   int XSpalt,
		   int YSpalt,
		   int dXSpalt,
		   int dYSpalt,
		   int SymType,  // 1: Kreise, 2: Quadrate
		   char *SymColor,
		   float SymSize,
		   char *LineColor,
		   float LineThick)
{
  FILE *DotFile;
  float z[20];
  int i;
  float LocSymSize;
  float ErrEndWidth=1.3;

  // Alter Hack, bevor es SymSize als Uebergabeparameter gab:
  if (dXSpalt < -1) LocSymSize = SymbolSize/(-1.0*dXSpalt); 
  else LocSymSize = SymbolSize;
  // Neu, d.h. bei alter Notation Funktion mit SymSize < 0 aufrufen:
  if (SymSize > 0) LocSymSize = SymbolSize * SymSize;

  if (!(DotFile=fopen(DataFile,"r"))) {
    printf(" Cannot open DATA FILE: %s\n",DataFile); return 1; }
  fprintf(PSDatei,"%c Datenpunkte der Datei %s ",37,DataFile);
  fprintf(PSDatei," Spalte %d (y) gegen Spalte %d (x)\n",YSpalt,XSpalt);
  fprintf(PSDatei,"%% Symbole: Typ %d\n", SymType);

  fprintf(PSDatei,"gsave\n");
  ClipOnFktArea(PSDatei);
  ColorSet(PSDatei,LineColor);
  fprintf(PSDatei,"%.3f setlinewidth\n", LineThick);
  while (fscanf(DotFile,"%f",&z[1]) != EOF)
    {
      for(i=2;i<=AnzSpalt;i++) fscanf(DotFile,"%f",&z[i]);

      /* Zeichnen der Fehlerbalken in X- und Y-Richtung */
      fprintf(PSDatei,"newpath\n");
      if (dXSpalt>0)
	{
	  if ((realX(z[XSpalt]+z[dXSpalt])-realX(z[XSpalt]))>LocSymSize)
	    {
	      fprintf(PSDatei,"%.3f %.3f moveto\n",realX(z[XSpalt]-z[dXSpalt]),
		      realY(z[YSpalt]) );
	      fprintf(PSDatei,"%.3f %.3f lineto\n",realX(z[XSpalt]+z[dXSpalt]),
		      realY(z[YSpalt]) );
	      fprintf(PSDatei,"%.3f %.3f moveto ",realX(z[XSpalt]+z[dXSpalt]),
		      realY(z[YSpalt])-ErrEndWidth*LocSymSize);
	      fprintf(PSDatei,"%.3f %.3f lineto\n",realX(z[XSpalt]+z[dXSpalt]),
		      realY(z[YSpalt])+ErrEndWidth*LocSymSize);
	      fprintf(PSDatei,"%.3f %.3f moveto ",realX(z[XSpalt]-z[dXSpalt]),
		      realY(z[YSpalt])-ErrEndWidth*LocSymSize);
	      fprintf(PSDatei,"%.3f %.3f lineto\n",realX(z[XSpalt]-z[dXSpalt]),
		      realY(z[YSpalt])+ErrEndWidth*LocSymSize);
	    };
	};
      if (dYSpalt>0)
	{
	  if ((realY(z[YSpalt]+z[dYSpalt])-realY(z[YSpalt]))>LocSymSize)
	    {
	      fprintf(PSDatei,"%.3f %.3f moveto\n",realX(z[XSpalt]),
		      realY(z[YSpalt]-z[dYSpalt]) );
	      fprintf(PSDatei,"%.3f %.3f lineto\n",realX(z[XSpalt]),
		      realY(z[YSpalt]+z[dYSpalt]) );
	      fprintf(PSDatei,"%.3f %.3f moveto ",
		      realX(z[XSpalt])-ErrEndWidth*LocSymSize,
		      realY(z[YSpalt]+z[dYSpalt]) );
	      fprintf(PSDatei,"%.3f %.3f lineto\n",
		      realX(z[XSpalt])+ErrEndWidth*LocSymSize,
		      realY(z[YSpalt]+z[dYSpalt]) );
	      fprintf(PSDatei,"%.3f %.3f moveto ",
		      realX(z[XSpalt])-ErrEndWidth*LocSymSize,
		      realY(z[YSpalt]-z[dYSpalt]) );
	      fprintf(PSDatei,"%.3f %.3f lineto\n",
		      realX(z[XSpalt])+ErrEndWidth*LocSymSize,
		      realY(z[YSpalt]-z[dYSpalt]) );
	    };
	};
      fprintf(PSDatei,"stroke \n");

      drawSymbol(PSDatei, SymType, realX(z[XSpalt]), realY(z[YSpalt]), 
		 SymColor, LocSymSize, LineColor, LineThick);
      
    };
  fprintf(PSDatei,"grestore \n");
  fprintf(PSDatei,"\n");
  fclose(DotFile);
  return 0;
}


/* ----------------------------- ScatDataPS ------------------------------- */

/* Diese Funktion liest von DataFile X-Y-Wertepaare ein und zeichnet
   an ihre Position einen kleinen Punkt.
   AnzSpalt gibt die Anzahl der Werte pro Zeile in DataFile an.
   XSpalt gibt an welcher davon die X-Koordinate und YSpalt welcher die
   Y-Koordinate ist.
*/

int
psplot::ScatDataPS( FILE *PSDatei,
		    char *DataFile,
		    int AnzSpalt,
		    int XSpalt,
		    int YSpalt )
{
  float MiX=(AktMinX-(AktMaxX-AktMinX)/10) * cm;
  float MaX=(AktMaxX+(AktMaxX-AktMinX)/10) * cm;
  float MiY=(AktMinY-(AktMaxY-AktMinY)/10) * cm;
  float MaY=(AktMaxY+(AktMaxY-AktMinY)/10) * cm;
  FILE *ScatFile;
  float z[20];
  int i;
  if (!(ScatFile=fopen(DataFile,"r"))) {
    printf(" Cannot open DATA FILE: %s\n",DataFile); return 1; }
  fprintf(PSDatei,"%c Scatterplot der der Datei %s ",37,DataFile);
  fprintf(PSDatei," Spalte %d (y) gegen Spalte %d (x)\n",YSpalt,XSpalt);
  fprintf(PSDatei,"gsave \n");
  ColorSet(PSDatei,PSGlobalColor);
  fprintf(PSDatei,"/p {%.3f 0 360 arc fill} def \n", 0.2*PSdotSize);
  ClipOnFktArea(PSDatei);
  fprintf(PSDatei,"newpath \n");
  while (fscanf(ScatFile,"%f",&z[1]) != EOF)
    {
      for(i=2;i<=AnzSpalt;i++) fscanf(ScatFile,"%f",&z[i]);
      if (!( (realX(z[XSpalt])<MiX) || (realX(z[XSpalt])>MaX) ||
	     (realY(z[YSpalt])<MiY) || (realY(z[YSpalt])>MaY) ))
	fprintf(PSDatei,"%.2f %.2f p\n",realX(z[XSpalt]),realY(z[YSpalt]));
    };
  fprintf(PSDatei,"grestore \n");
  fprintf(PSDatei,"\n");
  fclose(ScatFile);
  return 0;
}


/* ------------------------------ GrayBetPS --------------------------------*/

/* Diese Funktion zeichnet zwischen den beiden Kurven, die in den beiden
   Datenfiles GrenzVorDataFile und GrenzNachDataFile beschrieben sind, eine
   ausgefuellte graue Flaeche, deren Helligkeit durch GrayScale gegeben ist.
   Grayscale muss zwischen Null (schwarz) und Eins (weiss) liegen.
*/

int
psplot::GrayBetPS( FILE *PSDatei,
		   char *GrenzVorDataFile,
		   char *GrenzRueckDataFile,
		   float GrayScale          )
{
  FILE *PlotFileVor;
  FILE *PlotFileRueck;
  float x,y;
  int i,zaehler;
  float ax[500],ay[500];
  if (!(PlotFileVor=fopen(GrenzVorDataFile,"r"))) return 1;
  fscanf(PlotFileVor,"%f %f",&x,&y);
  fprintf(PSDatei,"%c Graufuellen zwischen %s und %s \n",37,
	  GrenzVorDataFile,GrenzRueckDataFile);
  fprintf(PSDatei,"gsave\n");
  ClipOnFktArea(PSDatei);
  fprintf(PSDatei,"gsave newpath %.3f %.3f moveto \n",realX(x),realY(y));
  while (fscanf(PlotFileVor,"%f %f",&x,&y) != EOF)
    fprintf(PSDatei,"%.3f %.3f lineto \n",realX(x),realY(y));
  fclose(PlotFileVor);
  zaehler = 0;
  if (!(PlotFileRueck=fopen(GrenzRueckDataFile,"r"))) return 1;
  while (fscanf(PlotFileRueck,"%f %f",&x,&y) != EOF)
    { zaehler++;
      ax[zaehler] = x;
      ay[zaehler] = y;
    }
  fclose(PlotFileRueck);
  for(i=zaehler;i>=1;i--)
     fprintf(PSDatei,"%.3f %.3f lineto \n",realX(ax[i]),realY(ay[i]));
  fprintf(PSDatei,"closepath %.3f setgray fill grestore grestore \n",
	  GrayScale);
  fprintf(PSDatei,"\n");
  return 0;
}

/* ------------------------------ drawSymbol ------------------------------- */

// Zeichnet ein Symbol der Groesse SymSize und der Farbe SymColor
// mit Randfarbe SymLineColor und Randdicke SymLineThick um den Punkt (x,y).
//
// SymType:  1: Kreis, 2: Quadrat, 3: Dreieck normal, 4: Dreieck auf der Spitze
//           5: Raute, 6: x
int
psplot::drawSymbol(FILE *PSDatei,
		   int SymType, 
		   float x,
		   float y,
		   char *SymColor, 
		   float SymSize,
		   char *LineColor,
		   float LineThick)
{
  float fac=1.0;
  float v;

  fprintf(PSDatei,"gsave ");
  ColorSet(PSDatei,LineColor);
  fprintf(PSDatei,"%.3f setlinewidth\n", LineThick);
  fprintf(PSDatei,"newpath\n");
  if      (SymType == 2){                // Quadratische Symbole
    fac = 1.0;
    fprintf(PSDatei,"%.3f %.3f moveto\n", x-fac*SymSize, y-fac*SymSize);
    fprintf(PSDatei,"%.3f %.3f lineto\n", x+fac*SymSize, y-fac*SymSize);
    fprintf(PSDatei,"%.3f %.3f lineto\n", x+fac*SymSize, y+fac*SymSize);
    fprintf(PSDatei,"%.3f %.3f lineto\n", x-fac*SymSize, y+fac*SymSize);
    fprintf(PSDatei,"%.3f %.3f lineto\n", x-fac*SymSize, y-fac*SymSize);
  }
  else if (SymType == 3){                // Dreieck Symbole, Spitze nach oben
    fac = 1.2;
    fprintf(PSDatei,"%.3f %.3f moveto\n", x-fac*SymSize, y-fac*SymSize);
    fprintf(PSDatei,"%.3f %.3f lineto\n", x+fac*SymSize, y-fac*SymSize);
    fprintf(PSDatei,"%.3f %.3f lineto\n", x, y+fac*SymSize);
    fprintf(PSDatei,"%.3f %.3f lineto\n", x-fac*SymSize, y-fac*SymSize);
  }
  else if (SymType == 4){                // Dreieck Symbole, Spitze nach unten
    fac = 1.2;
    fprintf(PSDatei,"%.3f %.3f moveto\n", x, y-fac*SymSize);
    fprintf(PSDatei,"%.3f %.3f lineto\n", x+fac*SymSize, y+fac*SymSize);
    fprintf(PSDatei,"%.3f %.3f lineto\n", x-fac*SymSize, y+fac*SymSize);
    fprintf(PSDatei,"%.3f %.3f lineto\n", x, y-fac*SymSize);
  }
  else if (SymType == 5){                // Rautenfoermige Symbole
    fac = 1.3;
    fprintf(PSDatei,"%.3f %.3f moveto\n", x, y-fac*SymSize);
    fprintf(PSDatei,"%.3f %.3f lineto\n", x+fac*SymSize, y);
    fprintf(PSDatei,"%.3f %.3f lineto\n", x, y+fac*SymSize);
    fprintf(PSDatei,"%.3f %.3f lineto\n", x-fac*SymSize, y);
    fprintf(PSDatei,"%.3f %.3f lineto\n", x, y-fac*SymSize);
  }
  else if (SymType == 6){                // X-foermige Symbole
    fac = 1.3;
    v = fac*SymSize/2;
    fprintf(PSDatei,"%.3f %.3f moveto\n", x-v, y-2*v);
    fprintf(PSDatei,"%.3f %.3f lineto\n", x, y-v);
    fprintf(PSDatei,"%.3f %.3f lineto\n", x+v, y-2*v);
    fprintf(PSDatei,"%.3f %.3f lineto\n", x+2*v, y-v);
    fprintf(PSDatei,"%.3f %.3f lineto\n", x+v, y);
    fprintf(PSDatei,"%.3f %.3f lineto\n", x+2*v, y+v);
    fprintf(PSDatei,"%.3f %.3f lineto\n", x+v, y+2*v);
    fprintf(PSDatei,"%.3f %.3f lineto\n", x, y+v);
    fprintf(PSDatei,"%.3f %.3f lineto\n", x-v, y+2*v);
    fprintf(PSDatei,"%.3f %.3f lineto\n", x-2*v, y+v);
    fprintf(PSDatei,"%.3f %.3f lineto\n", x-v, y);
    fprintf(PSDatei,"%.3f %.3f lineto\n", x-2*v, y-v);
    fprintf(PSDatei,"%.3f %.3f lineto\n", x-v, y-2*v);
  }
  else {                                 // SymType == 1  -> Kreise
    fac =1.15;
    fprintf(PSDatei,"%.3f %.3f %.2f 0 360 arc \n",x,y,fac*SymSize);
  }
  fprintf(PSDatei,"closepath ");
  if (SymColor[0] != '-') {
    fprintf(PSDatei,"gsave \n");
    ColorSet(PSDatei,SymColor);
    fprintf(PSDatei,"fill grestore \n");
  }
  fprintf(PSDatei,"stroke grestore\n");

  return 0;
}

int
psplot::set_BoundingBox(float x1, // cm
			float y1, // cm
			float x2, // cm
			float y2) // cm
{
  PS_bb_x1 = x1 * cm;
  PS_bb_x2 = x2 * cm;
  PS_bb_y1 = y1 * cm;
  PS_bb_y2 = y2 * cm;

  return 0;
}

psplot::psplot()
{

  PSmaxX =595.35;
  PSmaxY =841.95;
 
  cm = 28.348;        /* 841.95/29.7 */

  /* Bounding Box */
  PS_bb_x1 = 0;
  PS_bb_y1 = 0;
  PS_bb_x2 = PSmaxX;
  PS_bb_y2 = PSmaxY;
  
  /* Definition des Laenge der Skalierungsstriche in Punkten */
  TicLen = 8;

  /* Defintion der Postscript-Strichstaerke */
  PSLineWidth =0.3;
  /* Vordefinition des Abstands des Y-Titels von der Y-Achse (in cm) */
  YTitleDist = 2.0;
  /* Vordefinition des Abstands des X-Titels von der X-Achse (in cm) */
  XTitleDist = 1.5;
  
  /* Vordefinition der Textgreosse */
  FontSize = 10.0;
  
  /* Vordefinition der Symbolgroesse fuer die Ausgabe einzelner Datenpunkte */
  SymbolSize = 3.0;
  
  /* Definition der Orientierung: */
  Orientation = 0; /* O: seascape, 1: portrait */
  
  /* Skalierung der Dot-Groesse: */
  PSdotSize = 1.0;

  /* Tics nicht durchgezogen */
  XTicStyle = 0;
  YTicStyle = 0;

  /* Definition des aktiven Plotbereiches Bereiches in Zentimetern */
  AktMinX = 4.5;
  AktMinY = 3.0;
  AktMaxX = 27.0;
  AktMaxY = 18.0;
  
  /* Deklaration und Initialisierung der Funktionsbereiche in X und Y */
  FktMinX=AktMinX;
  FktMaxX=AktMaxX;
  FktMinY=AktMinY;
  FktMaxY=AktMaxX;
  
  /* Boolsche Variablen die x- und y-Achse logarithmisch setzen koennen    */
  XLog=0;                     /* != 0 bedeutet logarithmische X-Achse  */
  YLog=0;                     /* != 0 bedeutet logarithmische Y-Achse  */
  XLog10expStyle=0;    /* !=0 => Bemassungsstil fuer log-x-Achse: 10^n */
  YLog10expStyle=0;    /* !=0 => Bemassungsstil fuer log-y-Achse: 10^n */
  
  /* Vorbesetzung der Farbe: */
  strcpy(PSGlobalColor,"000000");
  
  /* Vorbesetzung der Farbe der Skalierungs-Striche: */
  PSTicColor=0.0;
  
  UmlFontDefNum=0;
  
}

