/* Lexical analyzer returns a double floating point
   number on the stack and the token NUM, or the ASCII
   character read if not a number.  Skips all blanks
   and tabs, returns 0 for EOF. 
*/

#define _POSIX_C_SOURCE 2
#define _XOPEN_SOURCE
#include "hmbook.h"
#include "hmplot.h"
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <math.h>

#include <readline/readline.h>
#include <readline/history.h>

#include <unistd.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef __hpux__
FILE *popen(const char *command, const char *type);
int pclose(FILE *stream);
#endif

HIST *emptyhist;

#define	NUM	 258
#define	STRNG	 259
#define	COMMAND  260

int  isstdin = 1;
int  pos = 0;
int linenumber = 0;
char *filename = "";

FILE *input = NULL;
char **define;
char **defval;
char *deftyp;
int defnr=0;


typedef struct histid { HIST *id; int nr; struct histid *next; } HISTID;
extern HISTID *firsthist, *lasthist;
void addhist(int nr, HIST *id);
void assign(int nr, HIST *new);

void yyerror(char *s)
{
  printf ("%s:%d: %s\n", filename, linenumber, s);
}

void myungetc(char c, FILE *dummy) 
{
  if(!isstdin) ungetc(c,dummy);
  else if (pos) --pos; 
}

char mygetchar()
{
  static char buffer[1024] = "";
  if (!isstdin) return fgetc(input);
  if (!buffer[pos]) {
    char *b = readline("HM> ");
    if (!b) return EOF;
    add_history(b);
    strcpy(buffer,b);
    buffer[strlen(b)    ] = '\n';
    buffer[strlen(b) + 1] = 0;
    free(b);	 
    pos = 0; 
  }
  return buffer[pos++];
}

extern FLOAT *yylval;     
void yyparse();
int strncasecmp(const char *st1, const char *str2, size_t n);

char *func[] = {
  "FIT",     "DELETE",   "HISTOGRAM",  "PLOTFILE",  "PIPE",    "GRAY",
  "LEGO",    "SURFACE",  "FUNCTION",   "OPERATION", "SCALAR",  "MEANX",
  "MEANY",   "ZONE",     "TITLE",      "SETOPTION", "PLOTERROR",
  "PLOTFUNCTION",        "PLOTBOX",    "CLOSE",     "PLOT3D",  "MINIMUMX",  
  "MINIMUMY","MINIMUMZ", "MAXIMUMX",   "MAXIMUMY",  "MAXIMUMZ",
  "SAME",    "EXPLAIN",  "READFILE",   "LIST",      "DIR",     "LS",
  "HELP",    "WAIT",     "QUIT",       "EXIT",      "SETREBIN","SETSTYLE", 
  "DATAERROR",  "BOX",   "COLOR",     "SCATTER", "MARK",     
  "SCALE",   "SETFILLRGB",
  "SMOOTH",    "XAXIS",     "YAXIS",   "HTITLE",
  "INCLUDEPS", "APPEND",  "WRITEONE",  "TEXT",      "LINE",    "RECTANGLE",
  "CONVOLUTE", "DECONVOLUTE", "FILLERROR", "CLEARERROR", "VARIANCE", 
  "WRITEASCII", "q"
};

int yylex ()
{
  int  i = 0;
  char c;

  while ((c = mygetchar ()) == ' ' || c == '\t');
  if (c == '#' || c == '!') { while (mygetchar() != '\n'); return '\n'; }  
  if (c == '$') {
    char str[255];
    while ((str[i++] = mygetchar()) != '\n');
    str[i-1] = 0;
    if (system(str)){};
    return '\n';
  }
  if (c == '.' || isdigit ((int)c)) {
    char fl[255];
    fl[i++]=c;
    while (c=='.'||c=='x'||c=='X'||isxdigit((int)c)) fl[i++] = c = mygetchar();
    myungetc(c, input);
    yylval = malloc(sizeof(FLOAT));
    fl[i-1] = 0;
    if (fl[0]=='0' && (fl[1]=='x' || fl[1]=='X')) {
      unsigned int ui;
      sscanf (fl+2, "%x", &ui);
      *yylval = ui;
    } else
      sscanf (fl, "%lf", yylval);
    return NUM;
  }
  
  if (c == '%') {
    char str[255];
    c = mygetchar ();
    do {
      str[i++] = c;
      c = mygetchar ();
    } while (isalpha((int)c));
    myungetc (c, input);
    str[i] = 0;
    for (i=0; i<defnr; i++)
      if (!strcmp(define[i], str)) {
	if (deftyp[i]=='S') {
	  yylval = malloc(strlen(defval[i]) + 1);
	  strcpy((char *) yylval, defval[i]);
	  return STRNG;
	} else {
	  yylval = malloc(sizeof(FLOAT));
	  * (FLOAT *) yylval = atof(defval[i]);
	  return NUM;
	}
      }
    yylval =  (FLOAT *) "";
    return STRNG;
  }
  if (isalpha((int)c)) {
    char str[255];
    do {
      str[i++] = toupper(c);
      c = mygetchar ();
    } while (isalpha((int)c));
    myungetc (c, input);
    str[i] = 0;
    yylval = NULL;
    i = 0;
    while (func[i][0]!='q') {
      if (!strncasecmp(str,func[i],strlen(str))
	  || (str[0]=='H'&&str[1]=='M'&&
	      !strncasecmp(str+2,func[i],strlen(str+2)))) {
	if (yylval != NULL) {
	  printf("%s is ambigous.", str);
	  yylval = NULL;
	  return('\n');
	}
	yylval = (FLOAT *) func[i];
      }
      i++;
    }
    if (yylval==NULL) { yyerror("Command not found"); return '\n'; }; 
    if (!strcmp((char *) yylval,"QUIT")) return 0;
    if (!strcmp((char *) yylval,"EXIT")) return 0;  
    return COMMAND;
  }
  
  if (c == '"') {
    char str[255];
    i = 0;
    do {
      if (c != '"') str[i++] = c;
      c = mygetchar ();
    } while ((c != '"' && c != '\n' && i< 254));
    str[i] = 0;
    if (c == '\n') myungetc (c, input); 
    yylval = (FLOAT *) malloc(strlen(str) + 1);
    strcpy((char *) yylval, str);
    return STRNG;
  }  
  if (c == EOF) return 0;
  return c;
}

void help()
{
  int i=0;
  
  puts("__________________________________________________"
       "____________________________\nCommands:");
  while (func[i][0]!='q') {
    printf("%c%-18s", ((i-1)%4?' ':'\n'), func[i]); i++;
  }
  /*  printf("\n\n"
      printf("Example Session:\n\n");
      printf("HM>$ ls                     ! '$' starts shell command\n");
      printf("example.his\n");
      printf("HM> readfile \"example.his\"  ! read histogram file\n");
      printf("HM> list\n");
      printf("  0   First Histogram Title\n");
      printf("  1   Second Histogram Title\n");
      printf("HM> Zone 1 2 1\n");
      printf("HM> function 0; explain \"first histogram\"\n");
      printf("HM> same; function 1; explain \"second histogram\"\n");
      printf("HM> ^D\n");
  */
  puts("\n__________________________________________"
       "____________________________________");

  if (getenv("DISPLAY")) {
    if (!system(WWWBROWSER
		" -remote \"openURL(" DOCDIR "/hm.html)\"&>/dev/null"))
      printf("\nUsing Netscape on \"%s\"\n", getenv("DISPLAY"));
    else {
      printf("\nStart HTML browser? ([y]/n) ");
      if (getchar() != 'n') 
	if (system(WWWBROWSER " " DOCDIR "/hm.html &> /dev/null &")){};
    }
  }
}

void readfile(char *filename)
{
  FILE *fp;
  char sc;
  int i=0;
  if (!strcmp(filename,"-")) {
    fp = stdin;
  } else {
    char command[255];
    sprintf(command, GZIP " -c -d %s", filename);
    fp = popen(command,"r");
    if (!fp) {
      sprintf(command, "can't open gzcat pipe for %s",filename);
      yyerror(command);
      return;
    }
  }
  
  { HIST *t = HMFirst, *next;
    while (t) {
      if (t==HMLast) next = NULL; else next = t->next;
      free(t->data);
      if (t->xvalues) free(t->xvalues);
      free(t);
      t = next;
    }
    HMFirst = HMLast = NULL;
  }
 
  { HISTID *hid =firsthist;
    while (hid) {
      HISTID *next = hid->next;
      free(hid);
      hid = next;
    }
  }
  firsthist = lasthist = NULL;

  if (fscanf(fp,"HMBook %lf\n%c\n",&HMVersion, &sc)){};
  while (HMReadFilename(fp,filename)) addhist(i++, HMLast);
  if (!strcmp(filename,"-"))
    pclose(fp);
}

void appendfile(char *filename)
{
  FILE *fp;
  int i=0;
  HISTID *hid =firsthist;
  char sc, command[255];
  sprintf(command, GZIP " -c -d %s",filename);
  fp = popen(command,"r");
  if (!fp) {
    sprintf(command, "can't open gzcat pipe for %s", filename);
    yyerror(command);
    return;
  }
  
  while (hid) {
    if (hid->nr >= i) i = hid->nr+1;
    hid = hid->next;
  }
  if (fscanf(fp,"HMBook %lf\n%c\n",&HMVersion, &sc)){};
  while (HMReadFilename(fp,filename)) addhist(i++, HMLast);
  pclose(fp);
}

int main (int argc, char *argv[])
{
  char historyfile[255];
  char opt;
  int i;
  extern FILE* HMfp;
  input = stdin;  

  define = (char **) calloc(argc, sizeof(char *));
  defval = (char **) calloc(argc, sizeof(char *));
  deftyp = (char *)  calloc(argc, sizeof(char));
  HMfp = NULL;
  emptyhist = HMBook1("HISTOGRAM NOT DEFINED","","","","",2,0.0,1.0);
  HMFirst = HMLast = NULL;

  while ((opt = getopt(argc, argv, "r:o:hD:S:")) != EOF) {
    switch (opt) {
    case '?':
    case 'h':
      printf("\nUSAGE:      %s [Options] [macrofile [postscriptfile]]\n\n"
	     "Options:\n\n"
	     "-h            This help.\n"
	     "-o <file>     File for postscript output.\n"
	     "-r <file>     Read histogram file before interpreting macro.\n"
             "-S NAME=VALUE Define NAME as string VALUE.\n"
	     "-D NAME=VALUE Define NAME as VALUE.\n\n", argv[0]);
      exit(0);
      break;
    case 'D': 
    case 'S': 
      define[defnr] = (char *) malloc(strlen(optarg) + 1);
      defval[defnr] = (char *) malloc(strlen(optarg) + 1);
      deftyp[defnr] = opt;
      strcpy(define[defnr], optarg);
      * (char *) strchr(define[defnr],'=') = '\0';
      strcpy(defval[defnr], (char *) (strchr(optarg,'=')+1));
      for (i=0; defval[defnr][i]; i++) 
	if (defval[defnr][i] == '_') defval[defnr][i] = '-';
      defnr ++;
      break;
    case 'o': HMPlotFile(optarg); break;
    case 'r': if (firsthist) appendfile(optarg); else readfile(optarg); break;
    case ':': exit(-1); /* missing argument */
    }
  }

  if (optind < argc) { 
    char command[255];
    isstdin = 0;
    filename = argv[optind];
    input = fopen(filename, "r");
    if (!input) {perror(filename);exit(0);} 
    if (fscanf(input,"%s",command)){};
    if (strncmp(command,"#!",2))
      sprintf(command, GNUCPP" %s", filename); 
    else
      sprintf(command, "tail +2 %s | "GNUCPP" -", filename); 
    fclose(input);
    input = popen(command, "r");
    if (!input) {fprintf(stderr, "Can't open \"%s\"\n",argv[optind]);exit(0);} 
  } else { 
    filename = "stdin:";
    puts("------------------------------------------\n"
         "       Welcome to Histoman's World!       \n"
         "------------------------------------------\n"
         "Try 'help' for possible commands\n"); 
    if (getenv("HOME")) strcpy(historyfile, getenv("HOME"));
    else                strcpy(historyfile, ".");
    strcat(historyfile,"/.hm_history");
    using_history();
    read_history(historyfile);
  }
  if (optind + 1 < argc)    
    HMPlotFile(argv[optind+1]);
  else
    if (!HMfp) HMPipe();
  
  yyparse ();
  HMClose();  
  if (isstdin) {
    stifle_history(200);
    write_history(historyfile);
  }
  return 0;
}













