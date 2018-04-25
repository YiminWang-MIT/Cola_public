//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mit.edu
// MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaMIT/StripChart.cc,v 2.3 2004-01-12 10:02:29 distler Exp $
//
// Main Part of the Cindy analysis program to check the helicity pattern
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */
#include "wx.h"
#include "wx_main.h"
#include "wx_timer.h"
#include <time.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include <signal.h>
#include "Aqua_oops1.h"
#include "Aqua/AquaUnzip.h"
#include "Aqua/CindyReader.h"
#include "Aqua/BatesReader.h"
#include "HMBook/hmbook.h"
#include "HMBook/hmview.h"
#include "StripChart.parse.h"
#include "StripChart.lex.h"
#include <errno.h>
#if defined(__GNU_LIBRARY__)
#  include <getopt.h>
#endif

extern "C" {
  char *getenv(const char *);
}

struct oops1 oops1;
AquaTree *atree = NULL;
AquaReader *reader = NULL;

class StripChartInit : public StripChartYACC {
  FILE *file;
  int yerrs;
  StripChartLEX *stripChartLEX;
  virtual int yylex();
  virtual void yyerror(char *);
public:
  static time_t startTime;
  StripChartInit(char *, AquaTree *);
  virtual ~StripChartInit();
};

void HMLaunchViewWork(int arc, char *arv[], void (*work) (wxApp *wap));
int  work (wxApp *app);

/////////////////////////////////////////

int
main(int argc, char *argv[])
{
  int cindyFormat = 0;
  char opt, *input = NULL;

#if defined(__GNU_LIBRARY__)
  struct option lo[] = {
    {"cindy",   0, NULL, 'c'},
    {"input",   1, NULL, 'f'},
    {"help",    0, NULL, 'h'},
    {NULL, 0, NULL, 0}};
  while ((opt = getopt_long(argc, argv, "cf:h", lo, NULL)) != EOF) {
#else
  while ((opt = getopt(argc, argv, "cf:h")) != EOF) {
#endif
    switch (opt) {
    case '?':                    // unknown option
    case ':':                    // missing argument
    case 'h':             
      cout << 
	"\nUsage:   " << argv[0] <<" [options]\n\n"
	"         StripChart display of ColaMIT.\n\n" 
	"Options:\n\n" 
	"-h      --help        This help.\n"
	"-f <f>  --input=<f>   Input filename.\n"
	"-c      --cindy       Read file in cindy format.\n";
      return 0;
    case 'f': input = optarg; break;
    case 'c': cindyFormat = 1; break;
    }
  }

  // parse input parameter (if any)
  AquaIO *aquaIO = NULL;
  if (!input || (*input=='-')) {
    aquaIO = new AquaUnzip(0);
  } else aquaIO = new AquaUnzip(input);

  atree = new Aqua_oops1(&oops1);
  reader = (cindyFormat ?
	    (AquaReader *)new CindyReader(aquaIO) :
	    (AquaReader *)new BatesReader(aquaIO));
  reader->initTree(atree);

  putenv("TZ=EST5EDT"); tzset(); // Timezone US Eastern

  if (getenv("DISPLAY")) {
    HMLaunchViewWork(0, argv, (void (*)(wxApp *))work);
  } else {
    while (!work((wxApp *) NULL));
  }

  exit(0);
  return 0;
}

class StripChartTimer : public wxTimer
{
private: 
public:
  wxApp *app;
  HMVFrame *frame;
  void Notify();
};

void
StripChartTimer::Notify()
{
  Stop();
  frame->SetStatusText("Wakeup");  

  while (!work(app));
}

StripChartTimer *timer = new StripChartTimer;
StripChartInit *init = NULL;
HMVFrame *frame;
int statusTime = 0;

int
work (wxApp *app)
{
  if (!init) {
    init = new StripChartInit("StripChart.items", atree);
    if (app) {
      frame = (HMVFrame *) app->GetTopWindow();
      frame->choice1d->SetSelection(1);
      frame->choice1d->Refresh();
      timer->app = app;
      timer->frame = frame;
    }
  }

  switch (reader->nextEvent(atree)) {
  case 1: // timeOut
    return 0;
    break;
  case -1: // finished: clean up and write files
    {
      char *stripchart_his = getenv("STRIPCHART_HIS");
      if (!stripchart_his) stripchart_his = "StripChart.his";
      HMWriteAll(stripchart_his);
    }
    return 1;  // This ends the X11 Callback loop!
    break;
  default: // next event
    if (!StripChart::startTime)
      StripChart::startTime = atree->getTimestamp();
    StripChart::handle(atree->getTimestamp());
    time_t now = atree->getTimestamp();
    if (app && (now >= statusTime)) {
      char sline[256];
      sprintf(sline, "display time : %s", ctime(&now));
      sline[39] = 0;
      frame->SetStatusText(sline);
      statusTime = now + 10;
    }
    break;
  }

  return 0;
}

StripChartInit::StripChartInit(char *rundb, AquaTree *aquatree)
{
  if (NULL == (file = fopen(rundb, "r"))) {
    char tmpbuf[256];
    sprintf(tmpbuf, "StripChartInit: '%s'", rundb);
    perror(tmpbuf);
    exit(errno);
  }
  stripChartLEX = new StripChartLEX(file);
  yerrs = 0;
  atree = aquatree;

  yyparse();
}

int
StripChartInit::yylex()
{
  yylloc.first_line = stripChartLEX->lineno;
  int token	    = stripChartLEX->yylex(&yylval);
  yylloc.last_line  = stripChartLEX->lineno;
  yylloc.text	    = (char *)stripChartLEX->yytext;

  return token;
}

void
StripChartInit::yyerror(char *m)
{
  yerrs++;
  std::cerr << " - line " << yylloc.last_line << ": "
    << m << " at token \'" << yylloc.text << '\'' << std::endl;

  if (yerrs>=20) exit(errno);
}

StripChartInit::~StripChartInit()
{
  if (file) fclose(file);
}
