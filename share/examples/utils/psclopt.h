// C-Modul psclopt : Verarbeitung von Kommandozeilenoptionen fuer
//                   auf PSPLOT basierende Programme.
//
//         Version : 2.5, T.Pospischil,  4.03.98
//                   2.6, T.Pospischil, 25.09.98, 2D-Farbe
//                   2.7, T.Pospischil, 19.11.98, triang, arc
//                   2.9, T.Pospischil, 21.01.99, verschied. Symbol-Typen
//                   3.0, T.Pospischil, 27.04.99, Bounding Box
//                   3.1, T.Pospischil, 14.05.99, Polygone

// Struktur mit allen Variablen, die in der Kommandozeile festgelegt werden:

struct importdesc{
  char filename[256];
  int clipmode;
  float xminclip;
  float yminclip;
  float xmaxclip;
  float ymaxclip;
  float xpos;
  float ypos;
  float xscale;
  float yscale;
  float rot;
  int fktsys;
};

struct textdesc{
  char filename[256];
  char fontname[256];
  char text[256];
  float fontsize;
  float lineskip;
  float xpos;
  float ypos;
  char buend;
  char color[32];
};

struct aldesc{
  char filename[256];
  char linetype[32];
  int ges;
  int xpos;
  int ypos;
};

// Descriptor for "-addcline" and "-addhisto"
struct acldesc{
  char filename[256];
  char linetype[32];
  int ges;
  int xpos;
  int ypos;
  char color[32];
  char linecolor[32];
  float linewidth;
};

struct asymdesc{
  char filename[256];
  int ges;
  int xpos;
  int ypos;
  int xerr;
  int yerr;
  int type;
  char color[32];
  float size;
  char linecolor[32];
  float linethick;
};

// Descriptor for "-addarrows"
struct aardesc{
  char filename[256];
  int ges;
  int xpos;
  int ypos;
  int ahx;
  int ahy;
  int style;
  char color[32];
  float thick;
  float skip;
};

struct geodesc{
  float x1, y1, x2, y2, x3, y3, x4, y4, radius, start_angle, end_angle;
  float linethick;
  char linetype[32];
  int symtype;
  char color[32];
  char framecolor[32];
};

struct polygondesc{
  float x[32];
  float y[32];
  int point_num;
  char color[32];
  char linetype[32];
  float linethick;
  char framecolor[32];
};


struct BoundingBox{
  float x1, y1, x2, y2;
  int there;
};

struct psclpara{

  // --- Name des zu bearbeitenden Datenfiles:
  int clfthere;     
  int cldatatype;                              // 0:data, 1:tasty, 2:tasty2D  
  char cldatafilename[256];

  // --- Name des Postscript-Outputfiles:
  int clothere;
  char clpsfilename[256];

  BoundingBox BB;

  // Orientierung und Groesse:
  int clorient; // 0: seascape, 1: portrait
  float clwidth;
  float clheight;

  // --- Form der Datenausgabe:
  int clotypethere;
  int clotype;                         // 0:dots, 1:line, 2:histo, 3:symbol 
                                       // 4:2D-Graustufen, 5:arrow 
                                       // 6:2D-Farbe 
  char cllinetype[32];
  int clrebinhisto;
  int clges, clxpos, clypos, clzpos, clxerr, clyerr;

  // --- Titel fuer x- und y-Achse:
  int cltitthere, clxtitthere, clytitthere;     
  char cltit[256], clxtit[256], clytit[256];

  // --- Beliebige Beschriftungen:
  int cltextnum;
  textdesc td[128]; 
  int clctextnum;
  textdesc ctd[128]; 
  int clfkttextnum;
  textdesc fkttd[128]; 
  char clfkttextfontname[256];
  int clbtextnum;
  textdesc btd[32]; 

  // --- PS Import Dateien:
  int climportnum;
  importdesc psim[128];

  // --- Unterteilung der Achsen:
  int clxticsthere, clyticsthere;
  float clxticmin, clxticmax;
  int clxticnum, clxticsubnum;
  float clyticmin, clyticmax;
  int clyticnum, clyticsubnum;
  int clxticstyle, clyticstyle;

  // --- Bereichsgrenzen in x und y:
  int clxthere, clythere;
  float clxmin, clxmax, clymin, clymax;

  // --- Plotten eines Polynoms:
  int clpolythere;
  int clpolyord;
  float cla[10];

  // --- Bingroesse beim Graustufenplot:
  float clxbinsize, clybinsize;

  // --- Logarithmische Achsen:
  float clxlog; int clxlogstyle;
  float clylog; int clylogstyle;
  // --- Logarithmische Graustufen:
  float clzlog;

  // --- Rahmen zeichnen?
  int cldrawframe;

  // --- Arrows:
  int clarstyle; // 0: Pfeil von einem Datenpunkt zum naechsten 
                 // 1: Erstes Wertepaar Ursprung, zweites Richtung
  char clcolor[32];  // Format: <rrggbb> (HEX Werte, z.B. Rot: "ff0000")
  float clarthick; // Dicke der Pfeile (Normalwerte: 1..4)
  float clarskip;  // Nur jeden clarskip. Datenpunkt einen Pfeil

  // --- Zusaetzliche Symbol-Eigenschaften:
  int clsymstylethere;
  int SymbolType;
  char SymbolColor[32]; 
  float SymbolSize;
  char SymbolLineColor[32];
  float SymbolLineThick;
    
  int clalthere;        // Anzahl der Line-Zusatzplots
  aldesc  clal[128];    // Beschreibung der Line-Zusatzplots

  int claclthere;       // Anzahl von farbigen Line-Zusatzplots
  acldesc clacl[128];   // Beschreibung von farbigen Line-Zusatzplots

  int clachthere;       // Anzahl von farbigen Histo-Zusatzplots
  acldesc clach[128];   // Beschreibung von farbigen Histo-Zusatzplots

  int clasymthere;      // Anzahl von farbiger Symbol-Zusatzplots
  asymdesc clasym[128]; // Beschreibung der farbigen Symbol-Zusatzplots

  int claarthere;       // Anzahl von Arrow-Zusatzplots 
  aardesc claar[128];   // Beschreibung der Arrow Zusatzplots

  // --- Globale Farbe:
  int clcolorthere;
  char clglobalcolor[32];

  // --- Geometrische Objekte
  int clboxthere;     struct geodesc clbox[32];
  int clcircthere;    struct geodesc clcirc[32];
  int cllinethere;    struct geodesc clline[32];
  int clcurvethere;   struct geodesc clcurve[32];
  int cltriangthere;  struct geodesc cltriang[32];
  int clarcthere;     struct geodesc clarc[32];
  int clsymthere;     struct geodesc clsym[32];
  int clpolygonthere; struct polygondesc clpolygon[32];
  int clplinethere;   struct polygondesc clpline[32];

  int clfboxthere;     struct geodesc clfbox[32];
  int clfcircthere;    struct geodesc clfcirc[32];
  int clflinethere;    struct geodesc clfline[32];
  int clfcurvethere;   struct geodesc clfcurve[32];
  int clftriangthere;  struct geodesc clftriang[32];
  int clfarcthere;     struct geodesc clfarc[32];
  int clfsymthere;     struct geodesc clfsym[32];
  int clfpolygonthere; struct polygondesc clfpolygon[32];
  int clfplinethere;   struct polygondesc clfpline[32];

  // --- Platz unter- und oberhalb des Plots bei autoscaling:
  int clleaveplace_y_there;
  float clleaveplace_y_below;
  float clleaveplace_y_above;

  // --- Zusatz-Skalierung der Achsen:
  float xticscale, yticscale;

  // Skalierung der Dot-Groesse
  float cldotsize;

  // --- Verschiebung der Abstaende der Achsenbeschriftung von der Achse:
  float xtitledist, ytitledist;

  // --- Initialisierung:
  psclpara(){
    clorient = 0;    // Voreinstellung: seascape
    BB.there = 0;					  
    clwidth  = 0.0;     
    clheight = 0.0;
    cldatatype  = 0; // Voreinstellung: normales Datenfile
    clotypethere= 0;     clotype= 0; // Voreinstellung: dots
    strcpy(cllinetype,"[] 0"); // volle Linie
    clcolorthere = 0;
    strcpy(clglobalcolor,"000000"); // schwarz
    clrebinhisto=0;
    clges = 2; clxpos=1; clypos=2; clzpos=3; clxerr=-1; clyerr=-1;
    clfthere    = 0;     strcpy(cldatafilename,"nodatafile");
    clothere    = 0;     strcpy(clpsfilename,"out.ps");
    cltitthere  = 0;
    clxtitthere = 0;
    clytitthere = 0;
    clxthere    = 0;     clxmin=0; clxmax=0;
    clythere    = 0;     clymin=0; clymax=0;
    clxticsthere= 0;
    clyticsthere= 0;
    clxticstyle = 0;
    clyticstyle = 0;    
    clpolythere = 0; 
    cltextnum   = 0;
    clctextnum   = 0;
    clfkttextnum   = 0;
    strcpy(clfkttextfontname,"Palatino-Roman");
    clbtextnum   = 0;
    climportnum  = 0;
    clxbinsize  = 1;
    clybinsize  = 1;
    cldotsize = 1.0;    
    clxlog = 0; clxlogstyle = 0;
    clylog = 0; clylogstyle = 0;
    clzlog = 0;
    cldrawframe = 1; // Rahmen wird normalerweise gezeichnet
    clalthere = 0;
    claclthere = 0;
    clachthere = 0;
    clasymthere = 0;
    claarthere = 0;
    clleaveplace_y_there = 0;
    clarstyle =0; strcpy(clcolor,"000000"); clarthick = 2; clarskip = 0;

    clsymstylethere=0;
    SymbolType=1; // Kreise;
    strcpy(SymbolColor,"ffffff");
    SymbolSize=1.0;
    strcpy(SymbolLineColor,"000000");
    SymbolLineThick=0.3;
    
    // Geometrische Objekte:
    clboxthere = 0;
    cllinethere = 0;
    clcurvethere = 0;
    clcircthere = 0;
    cltriangthere = 0;
    clarcthere = 0;
    clsymthere = 0;
    clpolygonthere = 0;
    clplinethere = 0;

    clfboxthere = 0;
    clflinethere = 0;
    clfcurvethere = 0;
    clfcircthere = 0;
    clftriangthere = 0;
    clfarcthere = 0;
    clfsymthere = 0;
    clfpolygonthere = 0;
    clfplinethere = 0;
    
    xticscale = 1.0;
    yticscale = 1.0;

    xtitledist = 0.0;
    ytitledist = 0.0;
  }

};

// Funktion "getpsclpara", die die Variablen von der Kommandozeile liest.
// In dem Fall, dass eine Option oder ein Parameter unverstaendlich oder nicht 
// vorhanden ist, wird eine Fehlermeldung ausgegeben und als Funktionswert
// ein Wert ungleich 0 zurueckgegeben.
// Gleiches gilt auch, falls eine notwendige Option (z.B. datafilename)
// nicht angegeben ist.

int getpsclpara(int, char**, psclpara&);


// Ausgabe der Benutzerinformationen:

int Benutzerinfo();
