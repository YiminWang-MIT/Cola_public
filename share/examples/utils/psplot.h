// 
//    Klasse psplot:   Plotten von Funktionen, ...  in ein Postscript-File
//
//                     Version: 1.0, 19.01.99, Thomas Pospischil
//                              (Umwandlung von c-Funktionensammlung psplot.c
//                               zur c++-Klasse psplot)

class psplot {

public:

  // Umrechnung von x und y vom Funktionskoord. ins Postscriptkoord.
  float realX( float X );
  float realY( float Y );
  
  // Diese Funktion schreibt in die (offene) PSDatei den Postscript-Befehl
  // zum setzen einer Farbe. Welche Farbe gesetzt wird, geben die ersten
  // 6 Zeichen des colorstr im rgb HEX Format an.
  // (Bsp.: "ff0000" ist Rot, "000000" schwarz, "aa00aa" lila, usw.) 
  int ColorSet ( FILE *PSDatei, char* colorstr);

  /* ClipOnFktArea */
  int ClipOnFktArea(FILE* PSDatei);

  /* Import einer anderen Postscript-Datei. */
  int import(FILE *PSDatei,
	     char PSimportFileName[1024],
	     int ClipMode, /* 0: kein clipping, 1: rechteck */
	     float clip_xmin,
	     float clip_ymin,
	     float clip_xmax,
	     float clip_ymax,
	     float x,      /* Einfuegepunkt in cm oder Fkt-Sys */
	     float y,
	     float xscale, /* Skalierung (bezogen auf 1.0) */
	     float yscale,
	     float rot,    /* Einfuegewinkel in Grad */
	     int fktsys);  /* Fktsys oder cm? */
  
  // Diese Funktion versucht, zu den uebergebenen Parametern Min_R und Max_R
  // (im Sinne der Funktionen XTics und YTics, s.u.) vernueftige Vorschlaege
  // zu den Achsenbeschriftungsparametern zu machen. Das Ergebis wird in
  // Min_N, Max_N, Num_N und Sub_N zurueckgegeben.
  int autoscale(double Min_R, 
		double Max_R, 
		double* Min_N, 
		double* Max_N, 
		int* Num_N,
		int* Sub_N);

  // Durchsucht Text nach geschweiften Klammern. Wird ein Paar gefunden, so wird
  // der erste Buchstabe nach { als Befehl interpretiert, der auf den Rest des
  // Strings innerhalb der Klammern wirkt. Moegliche Befehle sind:
  // {s..} : Benutze das Symbol-Encoding (griech. Buchstaben,...)
  // {z..} : Benutze das ZapfDingbats-Encoding (Symbole)
  // {i..} : Benutze Palatino-Italic
  // {B..} : Benutze Palatino-Bold
  // {I..} : Benutze Palatino-BoldItalic
  // {^..} : verkleinerte, hochgestellte Ausgabe
  // {_..} : verkleinerte, tiefgestellte Ausgabe
  // {u.}  : deutscher Umlaut, z.B. {uu} = ü, {us} = ß, usw.
  int TextPS( FILE *PSDatei,
	      char Text[1024] );

  /* PutText */
  int PutText ( FILE *PSDatei,
		char Text[1024],
		char Pos    );

  // PosTextPS
  /* Diese Funktion gibt den String TEXT an der Stelle
     X,Y (in Funktionskoordinaten)aus. Dabei bestimmen
     der Buchstabe in POS, ob sich der Punkt X,Y in der linken unteren
     Ecke (l=left), unten in der Mitte (c=center) oder rechts unten (r=right)
     von TEXT befindet. */
  int PosTextPS ( FILE *PSDatei,
		  char Text[1024],
		  float x,
		  float y,
		  char Pos    );

  // PosCTextPS
  // Wie PosTextPS nur fuer farbige Texte.
  int PosCTextPS ( FILE *PSDatei,
		   char Text[1024],
		   float x,
		   float y,
		   char Pos,
		   char *color);

  // OutTextPS 
  // Diese Funktion macht dasselbe wie PosTextPS mit dem Unterschied, das hier
  // die Koordinaten im PS-Koordinatensystem angegeben werden.
  int OutTextPS ( FILE *PSDatei,
		  char Text[1024],
		  float x,
		  float y,
		  char Pos    );
  // OutCTextPS 
  // Wie OutTextPS, nur fuer farbige Texte.
  int OutCTextPS ( FILE *PSDatei,
		   char Text[1024],
		   float x,
		   float y,
		   char Pos,
		   char *color);

  // ++++++++++++ Geometrische Objekte:

  int boxPS ( FILE *PSDatei,
	      float x1,          /* linke untere Ecke in cm */
	      float y1,
	      float x2,          /* rechte obere Ecke in cm */
	      float y2,
	      char *boxcolor,
	      float framethick,
	      char *framecolor,
	      int fkt);

  int triangPS ( FILE *PSDatei,
		 float x1,          /* erste Ecke */
		 float y1,
		 float x2,          /* zweite Ecke */
		 float y2,
		 float x3,          /* dritte Ecke */
		 float y3,
		 char *boxcolor,
		 float framethick,
		 char *framecolor,
		 int fkt);          /* 0:cm, sonst:im Funktions-Koord. */

  int polygonPS( FILE *PSDatei,
		 int num,
		 float* x,          /* Eckpunkte */
		 float* y,
		 char *boxcolor,
		 float framethick,
		 char *framecolor,
		 int fkt);          /* 0:cm, sonst:im Funktions-Koord. */
  
  int plinePS( FILE *PSDatei,
	       int num,
	       float* x,          /* Eckpunkte */
	       float* y,
	       char *linetype,
	       float framethick,
	       char *framecolor,
	       int fkt);          /* 0:cm, sonst:im Funktions-Koord. */
  
  int circlePS ( FILE *PSDatei,
		 float x1,          /* Lage des Mittelpunkts in cm */
		 float y1,
		 float radius,      /* Radius in cm */
		 char *circcolor,
		 float framethick,
		 char *framecolor,
		 int fkt);
  
  int arcPS ( FILE *PSDatei,
	      float x1,          /* Lage des Mittelpunkts in cm */
	      float y1,
	      float radius,      /* Radius in cm */
	      float startangle,
	      float endangle,
	      char *arccolor,
	      float framethick,
	      char *framecolor,
	      int fkt);

  int linePS ( FILE *PSDatei,
	       float x1, 
	       float y1,
	       float x2, 
	       float y2,
	       char *linecolor,
	       float thick,
	       int fkt);

  int curvePS ( FILE *PSDatei,
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
		int fkt);

  // Diese Funktion setzt den Font FONTNAME als aktuellen Font und skaliert ihn
  // mit FONTSCALE. ( FONTSCALE hat Punkte als Einheiten )
  int SetFontPS( FILE *PSDatei,
		 char *NewFontName,
		 float FontScale    );

  // Diese Funktion eroeffnet ein Postscript-File mit dem in FILENAME
  // uebergebenen Filenamen und sorgt fuer einen minimalen EPS-Rahmen.
  // Zusaetzlich wird das Blatt im Bedarfsfall (Variable: Orientation)
  // auf Querformat gedreht und die aktuelle Liniendicke gesetzt.
  int openPS( char *filename,
	      FILE **PSDatei   );

  // Diese Funktion schliesst das Postscript-File PSDATEI. Vorher wird
  // das obligatorische "showpage" ergaenzt.
  int closePS( FILE *PSDatei );

  // Diese Funktion zeichnet einen Rahmen um das aktive Gebiet, das durch die
  // Globalen Konstanten AKTMINX, AKTMINY, AKTMAXX und AKTMAXY festgelegt wird.
  int InitFramePS( FILE *PSDatei );

  // Diese Funktion zeichnet von XMIN bis XMAX ANZAHL viele beschriftete
  // Unterteilungen an die beiden horizontalen Linien des Rahmens, mit
  // SUBANZAHL vielen unbeschrifteten Unter-Unterteilungen.
  // XTICSCALE sort dafuer, dass die Achsenbeschriftung nicht mit den
  // Funktionswerten selbst, sondern mit XTICSCALE * normaler Beschriftung
  // erfolgt.
  int XTics ( FILE *PSDatei,
	      double XMin,
	      double XMax,
	      int anzahl,
	      int subanzahl,
	      float xticscale);

  // Diese Funktion zeichnet von YMIN bis YMAX ANZAHL viele beschriftete
  // Unterteilungen und SUBANZAHL viele unbeschriftete Unter-Unterteilungen
  // an die beiden vertikalen Linien des Rahmens.
  // YTICSCALE sort dafuer, dass die Achsenbeschriftung nicht mit den
  // Funktionswerten selbst, sondern mit YTICSCALE * normaler Beschriftung
  // erfolgt.
  int YTics ( FILE *PSDatei,
	      double YMin,
	      double YMax,
	      int anzahl,
	      int subanzahl,
	      float yticscale);

  // Diese Funktion gibt unterhalb der Mitte der X-Achse TEXT aus
  int XTitle( FILE *PSDatei,
	      char *Text         );
  // Diese Funktion gibt in der Mitte der Y-Achse TEXT (vertikal) aus
  int YTitle( FILE *PSDatei,
	      char *Text         );

  // PlotDataPS:
  // Diese Funktion liest von DataFile X-Y-Wertepaare ein und zeichnet mit
  // ihnen als Stuetzpunkte eine Kurve die Linientyp als Muster verwendet.
  // Bsp.: Linientyp=    "[] 0" : voller Strich
  //                    "[2] 0" : abwechselnd 2 voll 2 leer
  //		      "[6 2] 2" : 6 voll, 2 leer, beginnen mit 4 voll
  //                     ^   ^
  //		     Muster  Anzahl der am Anfang uebersprungenen Einheiten
  //
  // AnzSpalt gibt die Anzahl der Werte pro Zeile in DataFile an.
  // XSpalt gibt an welcher davon die X-Koordinate und YSpalt welcher die
  // Y-Koordinate ist.
  int PlotDataPS( FILE *PSDatei,
		  char *DataFile,
		  char *Linientyp,
		  int AnzSpalt,
		  int XSpalt,
		  int YSpalt );

  // CPlotDataPS:
  // Im Unterschied zur Funktion PlotDataPS wird noch eine Farbe und eine
  // Liniendicke an die Funktion uebergeben.
  int CPlotDataPS( FILE *PSDatei,
		   char *DataFile,
		   char *Linientyp,
		   int AnzSpalt,
		   int XSpalt,
		   int YSpalt,
		   char* color,
		   float linewidth);

  /* --------------------------- BlockTextPS ------------------------------- */
  /* Diese Funktion stellt den gesamten Text der Datei TextFileName dar.
     Die erste Zeile wird am Punkt (ulx,uly) (in cm) begonnen, die folgenden
     Zeilen liegen dann jeweils im Abstand lineskip (auch in cm) darunter.
     
     Verwendet wird der Postscript-Font mit dem Namen ThisFontName (z.B.
     "Courier", "Palatino-Roman", "Palatino-Bold", "Times-Roman", ...) in der
     Skalierung ThisFontSize (Standardwerte: 6.0-50.0).
     
     Wie in den anderen Text-Funktionen koennen mit z.B. {s..} (Symbol-Encoding
     i.a. griech. Buchstaben,...), {z..} (ZapfDingbats-Encoding, Symbole), 
     {^..} (verkleinerte, hochgestellte Ausgabe), {_..} (tiefgestellt), ...
     Textteile oder Einzelbuchstaben veraendert dargestellt werden.         */
  int BlockTextPS( FILE *PSDatei,
		   char *TextFileName,
		   char *ThisFontName,
		   float ThisFontSize,
		   float ulx,
		   float uly,
		   float lineskip);

  /* --------------------------- AbsArrowDataPS ---------------------------- */
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
     "aa00aa" lila, usw.)   
   */

  int AbsArrowDataPS( FILE *PSDatei,
		      char *DataFile,
		      int AnzSpalt,
		      int XSpalt,
		      int YSpalt,
		      float arthick,
		      float arskip,
		      char* arcolor);

  /* --------------------------- RelArrowDataPS ---------------------------- */
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

  int RelArrowDataPS( FILE *PSDatei,
		      char *DataFile,
		      int AnzSpalt,
		      int XSpalt,
		      int YSpalt,
		      int AHXSpalt,
		      int AHYSpalt,
		      float arthick,
		      float arskip,
		      char* arcolor);

  /* --------------------------- HistoDataPS ------------------------------- */
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
     Y-Koordinate ist. */
  int HistoDataPS(FILE *PSDatei,
		  char *DataFile,
		  char *Linientyp,
		  double grey,
		  int AnzSpalt,
		  int XSpalt,
		  int YSpalt );

  // CHistoDataPS:
  // Wie HistoDataPS, nur dass anstatt einem Grauton eine Farbe 
  // uebergeben wird.
  int CHistoDataPS(FILE *PSDatei,
		   char *DataFile,
		   char *Linientyp,
		   int AnzSpalt,
		   int XSpalt,
		   int YSpalt,
		   char *HistoColor,
		   char *LineColor,
		   double Linewidth);

  /* --------------------------- Histo2DDataPS ----------------------------- */
  /* Diese Funktion liest von DataFile X-Y-Z-Wertepaare ein und zeichnet mit
     ihnen als Stuetzpunkte ein Graustufen-Histogramm, wobei der groesste
     Z-Wert die Farbe Schwarz abgibt und die Werte dazwischen linear 
     bei zlog == 0 und logarithmisch von zlog != 0 zu weiss
     hin abgestuft werden.
     
     AnzSpalt gibt die Anzahl der Werte pro Zeile in DataFile an.
     XSpalt gibt an welcher davon die X-Koordinate, YSpalt welcher die
     Y-Koordinate und ZSpalt welcher die Z-Koordinate ist.
     
     binX und binY geben die Breite des grauen Kastens an, der an dem
     Datenpunkt gezeichnet wird. */
  int Histo2DDataPS(FILE *PSDatei,
		    char *DataFile,
		    int AnzSpalt,
		    int XSpalt,
		    int YSpalt,
		    int ZSpalt,
		    float binX,
		    float binY,
		    float zlog);

  /* --------------------------- Color2DDataPS ----------------------------- */
  // Wie Histo2DDataPS, nur Farbe statt Grautoenen
  int Color2DDataPS(FILE *PSDatei,
		    char *DataFile,
		    int AnzSpalt,
		    int XSpalt,
		    int YSpalt,
		    int ZSpalt,
		    float binX,
		    float binY,
		    float zlog);

  /* ----------------------------- DotDataPS ------------------------------- */
  /* Diese Funktion liest von DataFile X-Y-Wertepaare ein und zeichnet
     an ihre Position ein Symbol.
     AnzSpalt gibt die Anzahl der Werte pro Zeile in DataFile an.
     XSpalt gibt an welcher davon die X-Koordinate und YSpalt welcher die
     Y-Koordinate ist.
     dXSpalt und dYSpalt geben die Nummern derjenigen Spalten an, in denen die
     Fehler fuer die X- und die Y-Koordinate stehen. Werden in diesen Variablen
     Werte kleiner Null uebergeben, so werden keine Fehlerbalken gezeichnet.
     Auch wenn die Fehlerbalken kleiner als SymbolSize sind, werden sie
     weggelassen. */
  int DotDataPS( FILE *PSDatei,
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
		 float LineThick);
    
  /* ----------------------------- ScatDataPS ------------------------------- */
  /* Diese Funktion liest von DataFile X-Y-Wertepaare ein und zeichnet
     an ihre Position einen kleinen Punkt.
     AnzSpalt gibt die Anzahl der Werte pro Zeile in DataFile an.
     XSpalt gibt an welcher davon die X-Koordinate und YSpalt welcher die
     Y-Koordinate ist. */
  int ScatDataPS( FILE *PSDatei,
		  char *DataFile,
		  int AnzSpalt,
		  int XSpalt,
		  int YSpalt );

  /* ------------------------------ GrayBetPS --------------------------------*/
  /* Diese Funktion zeichnet zwischen den beiden Kurven, die in den beiden
     Datenfiles GrenzVorDataFile und GrenzNachDataFile beschrieben sind, eine
     ausgefuellte graue Flaeche, deren Helligkeit durch GrayScale gegeben ist.
     Grayscale muss zwischen Null (schwarz) und Eins (weiss) liegen. */
  int GrayBetPS( FILE *PSDatei,
		 char *GrenzVorDataFile,
		 char *GrenzRueckDataFile,
		 float GrayScale          );


  /* ------------------------------ drawSymbol -------------------------------*/
  // Zeichnet ein Symbol der Groesse SymSize und der Farbe SymColor
  // mit Randfarbe SymLineColor und Randdicke SymLineThick um den Punkt (x,y).
  //
  // SymType:  1: Kreis, 2: Quadrat
  int drawSymbol(FILE *PSDatei,
		 int SymType, 
		 float x,
		 float y,
		 char *SymColor, 
		 float SymSize,
		 char *LineColor,
		 float LineThick);

  /* ------------------------- set_BoundingBox ------------------------------ */
  int set_BoundingBox(float x1, // cm
		      float y1, // cm
		      float x2, // cm
		      float y2);// cm

  // ======================================================= public Variables:

  float PSmaxX;
  float PSmaxY;
  float cm;              /* 841.95/29.7 */

  /* Bounding Box */
  float PS_bb_x1;
  float PS_bb_y1;
  float PS_bb_x2;
  float PS_bb_y2;

  /* Definition des Laenge der Skalierungsstriche in Punkten */
  float TicLen;

  /* Defintion der Postscript-Strichstaerke */
  float PSLineWidth;
  /* Vordefinition des Abstands des Y-Titels von der Y-Achse (in cm) */
  float YTitleDist;
  /* Vordefinition des Abstands des X-Titels von der X-Achse (in cm) */
  float XTitleDist;

  /* Tics durchgezogen ? */
  int XTicStyle, YTicStyle;

  /* Vordefinition der Textgreosse */
  float FontSize;
  /* Vordefinition des Zeichensatzes (init. in der Funktion openPS) */
  char FontName[255];

  /* Vordefinition der Symbolgroesse fuer die Ausgabe einzelner Datenpunkte */
  float SymbolSize;

  /* Skalierung der Dot-Groesse: */
  float PSdotSize;

  /* Definition der Orientierung: */
  int Orientation; /* O: seascape, 1: portrait */

  /* Definition des aktiven Plotbereiches Bereiches in Zentimetern */
  float AktMinX;
  float AktMinY;
  float AktMaxX;
  float AktMaxY;
  
  /* Deklaration und Initialisierung der Funktionsbereiche in X und Y */
  float FktMinX;
  float FktMaxX;
  float FktMinY;
  float FktMaxY;
  
  /* Boolsche Variablen die x- und y-Achse logarithmisch setzen koennen    */
  int XLog;                       /* != 0 bedeutet logarithmische X-Achse  */
  int YLog;                       /* != 0 bedeutet logarithmische Y-Achse  */
  int XLog10expStyle;      /* !=0 => Bemassungsstil fuer log-x-Achse: 10^n */
  int YLog10expStyle;      /* !=0 => Bemassungsstil fuer log-y-Achse: 10^n */
  
  /* Vorbesetzung der Farbe: */
  char PSGlobalColor[8];

  /* Vorbesetzung der Farbe der Skalierungs-Striche: */
  float PSTicColor;

  int UmlFontDefNum;
  char *UmlFontDefNames[64];


  // ======================================================= private Functions:

private:
  // Unterroutine fuer TextPS : Sucht in einem Restttextstring, der nach
  // einer offenen geschweiften Klammer steht, die dazugehoerige schliessende
  // geschweifte Klammer. Der Reststring kann dabei beliebige andere
  // verschachtelte oder aneinandergereihte geschweifte Klammerpaerchen
  // beinhalten.
  // Zurueckgeliefert wird ein Zeiger auf die passende Klammer.
  // Im Fehlerfall wird NULL zurueckgeliefert.
  char *SuchKlammerZu(char *aText);

  // lokale Subroutine getHeatColor, verwandelt den als letztes uebergebenen
  // Wert frac (zwischen 0 und 1) in 3 rgb-Werte, die in Hred, Hgreen, Hblue
  // zurueckgegeben werden.
  int getHeatColor(float *Hred,
		   float *Hgreen,
		   float *Hblue,
		   float frac    );

  // Cstrip (Hilfsfunktion fuer PutText) --------------------
  // Entfernt zur Laengenbestimmung alle Kommandozeichen ('{c' und '}')
  // aus TEXT
  int Cstrip(char Text[1024], char StrippedText[1024]);

  // Definiert in der Postscript-Datei einen Font, der von dem Postscript Font
  // <NewFontName> abgeleitet ist, den Namen New-<NewFontName> bekommt, und
  // zusaetzlich zum Normalfont die deutschen Umlaute enthaelt.
  int DefineUmlauteInFont(FILE *PSDatei,
			  char *NewFontName);

  public:

  // Konstruktor
  psplot();

};

