//                                                                    -*-c++-*-
// $Id: hmview.cc 2638 2015-03-19 23:06:20Z schlimme $
//

#define _POSIX_C_SOURCE 2
#define _XOPEN_SOURCE 1
#define _XOPEN_SOURCE_EXTENDED
#include <config.h>
#include "../authors.h"
#include "hmbook.h"
#include "hmplot.h"
#include "hmdir.h"
#include "hmview.h"

#ifdef __hpux__
const char* prswitch = "-d";
#else
const char* prswitch = "-P";
#endif

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#define HELP_HTML DOCDIR "/hmview.html"
#define ICON      DOCDIR "/hmview.icon"



static const char *inithmviewps = 
  "%%!PS-Adobe-2.0\n"
  "%%%% Output as BITMAP\n"
  "/Palette <ffffff 000000> def\n"
  "[res 72 div 0 0 res 72 div 0 0] XSize YSize Palette makeimagedevice\n"
  "/Device exch def Device setdevice\n"
  "/copypage {\n"
  "  /scanline XSize 8 div cvi string def\n"
  "  YSize 1 sub -1 0 {Device exch scanline copyscanlines print}for\n"
  "} def\n"
  "/showpage {copypage erasepage initgraphics flush} def\n";

char *TmpNam(char *buffer) {
  strcpy(buffer,"hmview.XXXXXX");
  close(mkstemp(buffer));
  return buffer;
}

void lg(HIST *id) 
{ 
  HMSetOption(LOG_SCALE_Y,1);
  HMHistogram(id);
  HMSetOption(LOG_SCALE_Y,0);
}

int         wxEntry(int argc, char *argv[]);
static void choice(wxChoice&, wxEvent&);
static void plot(wxChoice& ch, wxEvent& event);

static int   resolutions[] = {50, 72, 100, 140, 200, 280};
static char* crs[] = {"50dpi","72dpi","100dpi","140dpi","200dpi","280dpi"};
static void (*style1d[])(HIST *id) = {HMHistogram, HMFunction,HMDataError,lg};
static char *c1d[] = {"Histogram", "Line", "Data/Error", "Logarithmic"};

static void (*style2d[])(HIST *id) = {HMGray, HMLego, HMScatter, HMBox,
                                      HMSurface, HMPlotMeanX, HMPlotMeanY}; 
static char *c2d[] = {"Grayscale", "Lego", "Scatter", "Box", "Surface",
		      "Mean x", "Mean y"};

static struct {
  char *name; 
  int dim; 
  double p[6];
} fits[] = {
  {"p[0] + p[1]*x",                                  2, {0,0,0,0,0,0}},
  {"p[0] + p[1]*x + p[2]*x^2",                       3, {0,0,0,0,0,0}},
  {"p[0] + p[1]*x + p[2]*x^2 + p[3]*x^3",            4, {0,0,0,0,0,0}},
  {"p[0] + p[1]*x + p[2]*x^2 + p[3]*x^3 + p[4]*x^4", 5, {0,0,0,0,0,0}},
  {"p[0]*gauss((x-p[1])*p[2])",                      3, {100,0,1,0,0,0}},
  {"p[0]*gauss((x-p[1])*p[2]) + p[3]",               4, {100,0,1,0,0,0}},
  {"p[0]*gauss((x-p[1])*p[2]) + p[3] + p[4]*x",      5, {100,0,1,0,0,0}},
  {"p[0]*gauss((x-p[1])*p[2]) + p[3] + p[4]*x + p[5]*x^2", 
   6, {100,0,1,0,0,0}},
  {"p[0] + p[1]*cos(x)",                                 2, {0,0,0,0,0,0}},
  {"p[0] + p[1]*cos(x) + p[2]*cos(x)^2",                 3, {0,0,0,0,0,0}},
  {"p[0] + p[1]*cos(x) + p[2]*cos(x)^2 + p[3]*cos(x)^3", 4, {0,0,0,0,0,0}},
  {"p[0]*exp(-x*p[1])",                              2, {100,1,0,0,0,0}},
  {NULL,0,{0,0,0,0,0,0}}
};

static int Nr_of_Frames = 0;
static Bool subprocess  = FALSE;
static wxMouseEvent      dummymouse;
static HMViewApp         HMViewApp;
static double start[255];

int gotSignal;

void
handleSignal(int sig)
{
    if ((sig == SIGPIPE)||(sig == SIGCHLD)) gotSignal = 1;
}

int HMLaunchView(int argc, char *argv[])
{ 
  subprocess = TRUE;
  int procid = fork(); 
  if (procid == 0) 
    wxEntry(argc, argv);
  return procid;
}

void HMLaunchViewWork(int argc, char *argv[], void (*work) (wxApp *wap))
{
  subprocess = TRUE; 
  HMViewApp.work_proc = work;
  wxEntry(argc, argv);
}

void (*HMMenuCallback)(int opt) = NULL; 

wxFrame *HMViewApp::OnInit(void)
{
  wx_class = "HMView";
  SetClassName("HMView");
  SetAppName("HMView Histogram Viewer");
  HMVFrame *frme = new HMVFrame(NULL, "Histogram View", 
				0, 0, 600, 570, wxSDI|wxDEFAULT_FRAME);
  if (argc == 2) {
    frme->readfile(argv[1]);
    if (frme->HMVhistogram) 
      frme->plothist(*(frme->choice1d), dummymouse);
  }
  return frme;
}

HMVFrame::~HMVFrame()
{ 
   _exit(0);
}

HMVFrame::HMVFrame(wxFrame *frme, char *title, 
		   int x, int y, int w, int h, long style)
 : wxFrame(frme, title, x, y, w, h, style)
{
  gspipein = NULL;
  gspipeout = NULL;
  gsprocid = 0;
  timer = new HMVTimer; 
  timer->frame = this;
  bit = NULL;
  res = 72;
  current_resolution = 3;
  sizex = 4 * res; 
  sizey = 3 * res;
  xold = -10; 
  yold = -10;
  backgroundBitmap = NULL;
  Nr_of_Frames ++;
  hv_maxx = NOMINIMUM; 
  hv_minx = NOMINIMUM;
  hv_maxy = NOMINIMUM;
  hv_miny = NOMINIMUM,
  hv_maxz = NOMINIMUM;
  hv_minz = NOMINIMUM;
  HMVhistogram = NULL;
  fitfunction = NULL;
  fitdimension = 0;
  hmoptions = "";
  SetIcon(new wxIcon(ICON));

  file_menu  = new wxMenu;
  histo_menu = new wxMenu;
  fit_menu   = new wxMenu;
  standardfit_menu = new wxMenu;
  macro_menu = new wxMenu;
  help_menu  = new wxMenu;
  zoom_menu  = new wxMenu;
  up_menu    = new wxMenu;

  //  static wxFont *myfont =
  //    wxTheFontList->FindOrCreateFont(16, wxSWISS, wxNORMAL, wxNORMAL);
  //  SetFont(myfont);

  int i;
  for (i = 0; i < 6; i++)
    zoom_menu->Append(HMVIEW_ZOOM+i+1,crs[i],NULL,TRUE); 
  zoom_menu->Check(HMVIEW_ZOOM+1+current_resolution,TRUE);
  
  if (!frme) {
    file_menu->Append(HMVIEW_NEW_FRAME,  "&New Frame");
    file_menu->AppendSeparator();
    if (!subprocess)
      file_menu->Append(HMVIEW_LOAD_FILE, "&Load Histogram File (*.his)");
    file_menu->Append(HMVIEW_SAVE_FILE,  "&Save Histogram File (*.his)");
    file_menu->Append(HMVIEW_EXPORT_FILE,  "E&xport Histogram File (*.hbook)");
    file_menu->AppendSeparator();
    file_menu->Append(HMVIEW_QUIT,  "&Quit");
  }
  else 
    file_menu->Append(HMVIEW_DELETE_FRAME,  "&Delete Frame");
  
  up_menu->Append(HMVIEW_STOPUPDATE, "Stop");
  up_menu->Append(HMVIEW_UPDATE01,   "1 sec.");
  up_menu->Append(HMVIEW_UPDATE05,   "5 sec.");
  up_menu->Append(HMVIEW_UPDATE10,   "10 sec.");
  up_menu->Append(HMVIEW_UPDATE20,   "20 sec.");
  up_menu->Append(HMVIEW_UPDATE60,   "60 sec.");
  
  histo_menu->Append(HMVIEW_ZOOM,       "&Zoom", zoom_menu);
  histo_menu->Append(HMVIEW_AUTOUPDATE, "&Update",up_menu);
  histo_menu->Append(HMVIEW_INFO,       "&Info");
  histo_menu->Append(HMVIEW_SAVE,       "&Save Current Histogram (*.eps)");
  histo_menu->Append(HMVIEW_SAVE_ASCII, "S&ave Current Histogram (*.ascii)");
  histo_menu->Append(HMVIEW_SAVE_ASCIITUPLE, "Save Current 1d/2d &Histogram gnuplot readable (*.gnudat)");
  histo_menu->Append(HMVIEW_GNUPLOTPLOT, "Plot actual 1d/2d Histogram with &gnuplot");
  histo_menu->Append(HMVIEW_PRINT,      "&Print Current Histogram");
  histo_menu->Append(HMVIEW_CLEAR,      "&Clear Current Histogram");
  
  fit_menu->Append(HMVIEW_FIT, "User defined fit");
  fit_menu->AppendSeparator();
  i = -1;
  while(fits[++i].name) 
    standardfit_menu->Append(HMVIEW_FITLIST+i,
			     fits[i].name);
  fit_menu->Append(HMVIEW_STANDARDFIT, "standard fit", standardfit_menu);
  fit_menu->AppendSeparator();
  fit_menu->Append(HMVIEW_NOFIT, "no fit");
  
  macro_menu->Append(HMVIEW_MACRO, "E&xecute Macro (*.hm)");
  help_menu->Append(HMVIEW_ABOUT,  "&About");
  help_menu->Append(HMVIEW_HELP,   "&Help");
  
  
  menu_bar = new wxMenuBar;
  menu_bar->Append(file_menu,  "&File");
  menu_bar->Append(histo_menu, "&Histogram");
  menu_bar->Append(fit_menu,   "F&it");
  menu_bar->Append(macro_menu, "&Macro");
  menu_bar->Append(help_menu,  "&Help");
  SetMenuBar(menu_bar);
  
  CreateStatusLine();

  Control = new wxPanel(this, 0,   0, 700, 100, wxBORDER);
  posx    = new wxMessage(Control, "x:",        10,  0);
  posy    = new wxMessage(Control, "y:",        10, 20);
  poscont = new wxMessage(Control, "Contents:", 10, 40);

  scrollLab = new wxMessage(Control, "Rebin: 1",          200,  0, 0);
  scrollReb = new wxScrollBar(Control,(wxFunction)& plot, 200, 20, 100, 15);
  scrollReb->SetViewLength(16);
  scrollReb->SetObjectLength(128);
  scrollReb->SetPageLength(4);
  scrollReb->SetValue(0);

  int xc,yc;
  choice1d=new wxChoice(Control,(wxFunction)& plot,"Style:",350,10,-1,-1,4,c1d);
  Control->GetCursor(&xc,&yc);
  choice2d=new wxChoice(Control,(wxFunction)& plot,"Style:",
			xc,yc,-1,-1,7,c2d);
  
  if (new wxButton(Control, (wxFunction)& plot,"update",10,60));
  histos  =new wxButton(Control, (wxFunction)&choice," Histogram List ");
  choice1d->Show(FALSE);
  scrollReb->Show(FALSE);
  scrollLab->Show(FALSE);
  choice2d->Show(FALSE);
  
  panel = new BitmapPanel(this,  0,  100, 580, 570, wxBORDER);
  panel->SetLabelPosition(wxVERTICAL);
  panel->SetBackground(new wxBrush("WHITE",wxSOLID));
  panel->Clear();

  counterPage = new HMVPage(this, "Counter Page", 0, 0, 500, 400,
			    wxDEFAULT_FRAME, "CounterPage");
  statPage = new HMVPage(this, "Statistic Page", 0, 0, 500, 400,
			 wxDEFAULT_FRAME, "StatisticPage");

  SetStatusText("");
  Show(TRUE);
}

void HMVFrame::create_gs_pipe()
{
  res = resolutions[current_resolution];
  sizex = 4*res;
  sizey = 3*res;

 
  char *argv[]={GHOSTSCRIPT, "-dNODISPLAY", new char[32], new char[32], 
		new char[32], "-q", "-dNOPAUSE", "-", NULL};
  
  sprintf(argv[2], "-dres=%d", res);
  sprintf(argv[3], "-dXSize=%d",sizex);
  sprintf(argv[4], "-dYSize=%d",sizey);
  
  int pipetogs[2], pipefromgs[2];
  if (pipe(pipetogs  )) perror("HMView: Can't open pipe to ghostscript");
  if (pipe(pipefromgs)) perror("HMView: Can't open pipe from ghostscript");
  gsprocid = fork();
  if (gsprocid==-1) perror("HMView: Can't spawn subprocess for ghostscript.");
  if (gsprocid==0) {
    if (dup2(pipetogs[0], 0))
      perror("HMView: Can't dup2 to");
    dup2(pipefromgs[1], 1);
    int fd = open("/dev/null", O_RDWR);
    dup2(fd, 2);
    for (fd = sysconf(_SC_OPEN_MAX)-1; fd > 2; fd--) close(fd);
    errno = 0; // probably got set to EBADF from a close
    if (execvp(argv[0], argv) == -1) 
      perror("HMView Can't execute gs.");
  } else {
    close(pipefromgs[1]);
    gspipein  = fdopen(pipefromgs[0], "r");
    if (!gspipein) 
      perror("HMView: Can't read from subprocess");
    close(pipetogs[0]);
    gspipeout = fdopen(pipetogs[1],   "w");
    if (!gspipeout) 
      perror("HMView: Can't write to subprocess");

    gotSignal = 0;
    signal(SIGPIPE,  handleSignal);
    signal(SIGCHLD,  handleSignal);

    fprintf(gspipeout,inithmviewps,0); fflush(gspipeout);
    HMFilePointer(gspipeout);
    HMPipe();
    HMSetOption(MRG_TITLE, 0.0);
    HMSetOption(MRG_LEFT,  0.5);
    HMSetOption(MRG_LOW,   0.5);
    HMSetOption(SIZE_X,    4.0 * 2.54 - 1.0);
    HMSetOption(SIZE_Y,    3.0 * 2.54 - 1.0);
    fflush(gspipeout);
  }
}  

void HMVFrame::close_gs_pipe()
{
  if (gspipeout) {
    fprintf(gspipeout,"0 0 moveto 100 100 lineto stroke\n");
    fprintf(gspipeout,"showpage\n"); 
    fflush(gspipeout);
    char *bit = new char[(sizex/8)*sizey];
    if(fread(bit, sizeof(char), (sizex/8)*sizey, gspipein)){};
    fprintf(gspipeout,"quit\n");
    fflush(gspipeout);
    int status;
    waitpid(gsprocid, &status, 0 );
    if (fclose(gspipeout)) perror("HMV: Can't close pipe to gs");
  }
  
  if (gspipein) 
    if (fclose(gspipein))  
      perror("HMV: Can't close pipe from gs");
  
  gspipeout = gspipein = NULL;
}

void HMVFrame::kill_gs_pipe()
{
  close_gs_pipe();
  kill(gsprocid, 9);
  puts("done");
}

void HMVTimer::Notify()
{
  frame->plothist(*(frame->choice1d),dummymouse);
}

void HMVFrame::readfile(char *filename)
{
  HMReadAll(filename);
  HMVhistogram = HMFirst;
}

void HMVFrame::print_current()
{
  if (!HMVhistogram) return;

  HMSetOption(MRG_TITLE, 0.0);
  HMSetOption(MRG_LEFT, 2.5);
  HMSetOption(MRG_LOW,  15.0);
  HMSetOption(SIZE_X,   16.0);
  HMSetOption(SIZE_Y,   10.0);

  char *prcom;
  char *prenv = getenv("PRINTER");

  if (prenv) {
    prcom = new char[7+strlen(prenv)];
    strcpy(prcom, "lpr ");
    strcat(prcom, prswitch);
    strcat(prcom, prenv);
  } else {
    prcom = new char[4];
    strcpy(prcom, "lpr");
  }

  FILE *printer = popen(prcom, "w");
  HMFilePointer(printer);  
  HMPipe();

  HMMinimumX(hv_minx);  HMMaximumX(hv_maxx);
  HMMinimumY(hv_miny);  HMMaximumY(hv_maxy);
  HMMinimumZ(hv_minz);  HMMaximumZ(hv_maxz);
  if (HMVhistogram->ny)
    style2d[choice2d->GetSelection()](HMVhistogram);
  else
    style1d[choice1d->GetSelection()](HMVhistogram);
  HMClose();
  
  HMFilePointer(gspipeout);
  HMSetOption(MRG_TITLE,0.0);
  HMSetOption(MRG_LEFT,0.5);
  HMSetOption(MRG_LOW,0.5);
  HMSetOption(SIZE_X, 4.0*2.54-1.0);
  HMSetOption(SIZE_Y, 3.0*2.54-1.0);
  plothist(*choice1d, dummymouse);
  delete prcom;
}

void
HMVFrame::clear_current()
{
  if (!HMVhistogram) return;

  if (HMVhistogram->ny) {
    memset(HMVhistogram->data, 0,
	   HMVhistogram->nx*HMVhistogram->ny*sizeof(FLOAT));
  } else {
    memset(HMVhistogram->data, 0, HMVhistogram->nx*sizeof(FLOAT));
  }
  HMVhistogram->entries=0;
}

void HMVFrame::save_current()
{
  if (!HMVhistogram) return;

  HMSetOption(MRG_TITLE, 0.0);
  HMSetOption(MRG_LEFT,  2.5);
  HMSetOption(MRG_LOW,  15.0);
  HMSetOption(SIZE_X,   16.0);
  HMSetOption(SIZE_Y,   10.0);
  char *s = wxFileSelector("Save Postscript File", NULL, NULL, NULL, "*.eps");
  if (!s) return;
  HMPlotFile(s);
  HMMinimumX(hv_minx);  HMMaximumX(hv_maxx);
  HMMinimumY(hv_miny);  HMMaximumY(hv_maxy);
  HMMinimumZ(hv_minz);  HMMaximumZ(hv_maxz);
  if (HMVhistogram->ny)
    style2d[choice2d->GetSelection()](HMVhistogram);
  else
    style1d[choice1d->GetSelection()](HMVhistogram);
  HMClose();
  
  HMFilePointer(gspipeout);
  HMSetOption(MRG_TITLE,0.0);
  HMSetOption(MRG_LEFT,0.5);
  HMSetOption(MRG_LOW,0.5);
  HMSetOption(SIZE_X, 4.0*2.54-1.0);
  HMSetOption(SIZE_Y, 3.0*2.54-1.0);
  plothist(*choice1d, dummymouse);
}

void HMVFrame::save_current_ascii()
{
  if (!HMVhistogram) return;
  char *s = wxFileSelector("Save ASCII Dump File", NULL,NULL,NULL, "*.ascii");

  if (!s) return;
  FILE *fp = fopen(s,"w");
  if (!fp) return; 
  HMWriteAscii(fp, HMVhistogram);
  fclose(fp);
}

void HMVFrame::save_current_ascii_tuple()
{
  if (!HMVhistogram) return;
  char *s = wxFileSelector("Save ASCII Dump File", NULL,NULL,NULL, "*.dat");

  if (!s) return;
  FILE *fp = fopen(s,"w");
  if (!fp) return; 
  HMWriteAsciiTuple(fp, HMVhistogram);
  fclose(fp);
}

void HMVFrame::gnuplotplot()
{
  fprintf(stderr, "\n\BOOM BOOM BANANA\n");
  if (!HMVhistogram) return;
  char *gnudat = TmpNam(new char[255]);
  FILE *fp = fopen(gnudat,"w");
  if (!fp) return; 
  HMWriteAsciiTuple(fp, HMVhistogram); 
  char command[2555];
  char style[32];
  if (HMVhistogram->ny==0)
    sprintf(style, "wi histeps");
  else 
    sprintf(style, "wi image");
  //  sprintf(command, "(gnuplot -e \"set palette model CMY rgbformulae 7,5,15; set xlabel '%s [%s]'; set ylabel '%s [%s]'; plot '%s' %s title '%s'; pause mouse close\"; rm -f %s) &", HMVhistogram->labelx, HMVhistogram->unitx, HMVhistogram->labely, HMVhistogram->unity, gnudat, style, HMVhistogram->title, gnudat);
  sprintf(command, "(gnuplot -e \"set palette model CMY rgbformulae 7,5,15; set xlabel \\\"%s [%s]\\\"; set ylabel \\\"%s [%s]\\\"; plot '%s' %s title \\\"%s\\\"; pause mouse close\"; rm -f %s) &", HMVhistogram->labelx, HMVhistogram->unitx, HMVhistogram->labely, HMVhistogram->unity, gnudat, style, HMVhistogram->title, gnudat);
  if (system(command)){};
  delete gnudat; 
}


static void plot(wxChoice& ch, wxEvent& event)
{
  HMVFrame *fr = (HMVFrame*) (ch.GetParent()->GetParent());
  fr->plothist(ch,event);
}

wxButton *ok, *cancel;
wxText *ms1, *ms2, *ms3;
 
void getfunc(wxButton& d, wxEvent&)
{
  wxWindow *dialog = d.GetParent();
  HMVFrame *fr = (HMVFrame *) dialog->GetParent();
  fr->fitfunction=NULL; 
  if (&d == ok) {
    fr->fitfunction = new char[strlen(ms1->GetValue())];
    strcpy(fr->fitfunction, ms1->GetValue());;
    fr->fitdimension = atoi(ms2->GetValue());
    sscanf (ms3->GetValue(),"%lf%lf%lf%lf%lf%lf",
	    &start[0],&start[1],&start[2],&start[3],&start[4],&start[5]);
  }
  dialog->Show(FALSE);
}

char *getfunctionstr(wxWindow *frame)
{
  wxDialogBox *dialog = new wxDialogBox(frame,"Function Definition",TRUE,
					-1,-1,400,250);
  ms1    = new wxText(dialog, NULL, "f(x) = ", 
		      ((HMVFrame *)frame)->fitfunction,-1,-1,350,-1);
  dialog->NewLine();
  char def[255];
  sprintf (def,"%d",((HMVFrame *)frame)->fitdimension);
  ms2    = new wxText(dialog, NULL, "# of parameter = ", def,-1,-1,200,-1);
  dialog->NewLine();
  sprintf (def,"%f %f %f %f %f %f",
	   start[0],start[1],start[2],start[3],start[4],start[5]);
  ms3    = new wxText(dialog, NULL, "start parameter = ", def,-1,-1,300,-1);
  dialog->NewLine();
  dialog->Center();
  ok     = new wxButton(dialog,(wxFunction)&getfunc, "   OK   ",150);
  wxButton(dialog,(wxFunction)&getfunc, " Cancel ");
  dialog->Show(TRUE);
  return NULL;
}


static void choice(wxChoice& ch, wxEvent& event)
{
  if (!HMLast) return;

  HistDirectory *root;
  HistDirectory *branch;

  HMVFrame *fr = (HMVFrame*) (ch.GetParent()->GetParent());
  root = new HistDirectory("/");
  root->CreateHistoTree();
  branch = root->FindBaseDir(ch.GetLabel());

  char *names[4096];
  HIST *ids[4096];
  
  for(;;) {
    branch->newscan();
    HistDirEntry *he = branch->next();
    int lines = 0; 
    if (strcmp(branch->getName(), "/")) {
      names[0] = new char[5];
      strcpy(names[0], "[..]");
      lines++;
    }
    while (he) {
      if (he->DirAddr()) {
	ids[lines] = NULL;
	names[lines] = new char[strlen(he->getName())+3];
	sprintf(names[lines], "[%s]", he->getName());
      } else {
	ids[lines] = ((HistEntry *) he)->Histogram();
	names[lines] = new char[strlen(he->getName())+6];
	sprintf(names[lines], "%1dD:  %s",
		(ids[lines]->ny>0 ? 2 : 1), he->getName());
      }
      lines++;
      he = branch->next();
    }

    if (!lines) return;
    char *title = "Histogram List";
    if (branch != root) title = (char *)branch->getName();
    int choose=wxGetSingleChoiceIndex(title, "Histograms",
				      lines, names,
				      NULL, -1, -1, TRUE, 400, 300);
    // Cancel button pressed
    if (choose<0) {
      while (lines > 0) delete names[--lines];
      delete root;
      return;
    }

    // Test if we have a histogram entry
    if (names[choose][0]!='[' && names[choose][1]=='D') {
      fr->HMVhistogram=ids[choose];
      while (lines > 0) delete names[--lines];
      break;
    }

    // Test if level up has been choosen
    if (!strcmp(names[choose],"[..]")) {
      branch = branch->cd("..");
      if (!strcmp(branch->getName(),"/")) branch = root;
    } 
    else if (names[choose][0]=='[') {
      names[choose][strlen(names[choose])-1]=0;
      branch = branch->cd(&names[choose][1]); 
    }
    while (lines > 0) delete names[--lines];
  }
  fr->histos->SetLabel((char *)(fr->HMVhistogram->title));
  fr->hv_maxx = fr->hv_minx =
    fr->hv_maxy = fr->hv_miny = 
    fr->hv_maxz = fr->hv_minz = NOMINIMUM;
  fr->plothist(ch, event);

  delete root;
}   

void HMVFrame::plothist(wxChoice&, wxEvent&)
{
  if (!HMVhistogram) {
    panel->Clear();
    return;
  }
  
  wxBeginBusyCursor();
  
  if (gspipeout && res != resolutions[current_resolution]) {
    close_gs_pipe();
    if (bit) delete bit;
    bit = NULL;
  }
  if (!gspipeout) 
    create_gs_pipe();
  
  scrollReb->Show(HMVhistogram->ny==0);
  scrollLab->Show(HMVhistogram->ny==0);
  choice1d->Show(HMVhistogram->ny==0);
  choice2d->Show(HMVhistogram->ny>0);

  HMFilePointer(gspipeout);
  HMMinimumX(hv_minx);  HMMaximumX(hv_maxx);
  HMMinimumY(hv_miny);  HMMaximumY(hv_maxy);
  HMMinimumZ(hv_minz);  HMMaximumZ(hv_maxz);
  HMSetRebin(scrollReb->GetValue()+1);
  {
    char lab[30]; 
    sprintf(lab,"Rebin: %d",scrollReb->GetValue()+1);
    scrollLab->SetLabel(lab);
  }
  if (HMVhistogram->ny)
    style2d[choice2d->GetSelection()](HMVhistogram);
  else
    style1d[choice1d->GetSelection()](HMVhistogram);

  static double par[255] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  if(fitfunction) {
    if (!par[0]) for(int i=0;i<fitdimension;i++) par[i] = start[i];
    if (HMScanFunction(fitfunction, par, fitdimension))
      fprintf(stderr,"Can't scan function \"%s\"\n", fitfunction);
    else {
      HMFit1(HMVhistogram, HMFit1Function, par, fitdimension); 
      char fitt[255];
      sprintf(fitt,"f(x) = %s",fitfunction);
      // HMComment(fitt, 0);
    }
  }
  else 
    for(int i=0;i<fitdimension;i++) 
      par[i] = 0;
  HMNewPage();
  fflush(gspipeout);   
  
  wxBitmap *bitmap = new wxBitmap(read_bitmap(), sizex, sizey);

  if (bitmap) {
    if (backgroundBitmap) delete backgroundBitmap;
    backgroundBitmap = bitmap;
  }
  
  panel->EnableScrolling(TRUE,TRUE);
  panel->SetScrollbars(20,20,sizex/20,sizey/20,4,4);
  wxCursor *crs = new wxCursor(wxCURSOR_CROSS);
  panel->SetCursor(crs);
  panel->Refresh();
  wxEndBusyCursor();
}

char *
HMVFrame::read_bitmap()
{
  if (!bit) bit = new char[(sizex/8)*sizey];
  int nr = fread(bit, sizeof(char), (sizex/8)*sizey, gspipein);
 if (nr != (sizex/8)*sizey) {
    fprintf(stderr, "\n!!! CONGRATULATIONS !!! You crashed ghostscript !!!\n");
    fprintf(stderr, "\ntrying to kill zombie process ... ");
    kill_gs_pipe();
    fprintf(stderr, "\n\tpress middle mouse button to resume\n");
  }
  for(int s = 0; s < (sizex/8) * sizey; s++) {
    char bt; 
    for(int i=bt=0 ; i < 8; i++) 
      bt += (bit[s] >> i & 1) << (7 - i);
    bit[s]=bt;
  }
  return bit;
}

void HMVFrame::executemacro(char *s)
{
  char *psout    = TmpNam(new char[255]);
  char *macrolog = TmpNam(new char[255]);
  char command[255];
  sprintf(command, "hm -r- %s %s %s > %s", hmoptions, s, psout, macrolog);
  HMWriteAllPipe(command);
  sprintf(command, "sh -c \"("GHOSTVIEW" %s;rm -f %s %s;) &\"",
	  psout, psout, macrolog);
  if (system(command)){};
  delete psout; 
  delete macrolog;
}
///////////////////////////////////////////////////////////////////////////////
//  Event Handler
///////////////////////////////////////////////////////////////////////////////

void HMVFrame::OnMenuCommand(int id)
{
  if (id > HMVIEW_ZOOM && id < HMVIEW_ZOOM + 100) {  
    zoom_menu->Check(HMVIEW_ZOOM + 1 + current_resolution, FALSE);
    current_resolution = id - HMVIEW_ZOOM - 1;
    zoom_menu->Check(HMVIEW_ZOOM + 1 + current_resolution, TRUE);
    plothist(*choice1d, dummymouse);
    return;
  }
  
  if (id >= HMVIEW_FITLIST && id < HMVIEW_FITLIST + 99) {
    fitfunction = fits[id - HMVIEW_FITLIST].name;
    fitdimension = fits[id - HMVIEW_FITLIST].dim;
    for(int i=0; i<6; i++)
      start[i] = fits[id - HMVIEW_FITLIST].p[i];
    plothist(*choice1d, dummymouse);
    return;
  }
  
  char *s;
  switch (id) {
  case HMVIEW_FIT: 
    getfunctionstr(this);
    plothist(*choice1d, dummymouse);
    break;
  case HMVIEW_NOFIT: 
    fitfunction = NULL;
    fitdimension = 0;
    plothist(*choice1d, dummymouse);
    break;
  case HMVIEW_NEW_FRAME: 
    if (new HMVFrame(this, "Histogram View (Spawned)", 
		     0, 0, 600, 570, wxSDI|wxDEFAULT_FRAME));
    break;
  case HMVIEW_LOAD_FILE:
    s = wxFileSelector("Load Histogram File", 
		       NULL, NULL, NULL, "*.his");
    if (s) {
      readfile(s);
      histos->SetLabel((char *)HMVhistogram->title);
      plothist(*choice1d, dummymouse);
    }
    break;
  case HMVIEW_SAVE_FILE:
    s = wxFileSelector("Save Histogram File", 
		       NULL, NULL, NULL, "*.his");
    if (s) HMWriteAll(s);
    break;
  case HMVIEW_EXPORT_FILE:
    s = wxFileSelector("Export HBook File", 
		       NULL, NULL, NULL, "*.hbook");
    if (s) {
      char *histfile = TmpNam(new char[255]);
      HMWriteAll(histfile);
      char command[255];
      sprintf(command,
	      "sh -c \"hm2hbook %s soeinmist; mv soeinmist %s;rm %s\"",
	      histfile, s, histfile);	  
      if (system(command)){};
      delete histfile;
    }
    break;
  case HMVIEW_MACRO:   
    if (opendir("hm"))
      s = wxFileSelector("Execute Macro File","hm",NULL,NULL,"*.hm");
    else
      s = wxFileSelector("Execute Macro File",NULL,NULL,NULL,"*.hm");
    if (s) executemacro(s);
    break;
  case HMVIEW_UPDATE05:  
    timer->Stop();
    timer->Start( 5000); 
    break;
  case HMVIEW_UPDATE01:  
    timer->Stop();
    timer->Start( 1000); 
    break;
  case HMVIEW_UPDATE10:  
    timer->Stop();
    timer->Start(10000); 
    break;
  case HMVIEW_UPDATE20:  
    timer->Stop();
    timer->Start(20000); 
    break;
  case HMVIEW_UPDATE60: 
    timer->Stop();
    timer->Start(60000);
    break;
  case HMVIEW_STOPUPDATE: 
    timer->Stop();
    break; 
  case HMVIEW_DELETE_FRAME:
    timer->Stop();
    Show(FALSE);
    Nr_of_Frames--;
    return; 
  case HMVIEW_QUIT:
    timer->Stop();
    Show(FALSE);
    close_gs_pipe();
    Nr_of_Frames--;
    wxPostDelete(this);
    break; 
  case HMVIEW_INFO:
    infobox();
    break; 
  case HMVIEW_PRINT:
    print_current();
    break; 
  case HMVIEW_CLEAR:
    clear_current();
    break; 
  case HMVIEW_SAVE:
    save_current();
    break; 
  case HMVIEW_SAVE_ASCII:
    save_current_ascii();
    break; 
  case HMVIEW_SAVE_ASCIITUPLE:
    save_current_ascii_tuple();
    break; 
  case HMVIEW_GNUPLOTPLOT:
    gnuplotplot();
    break; 
  case HMVIEW_HELP:
    char helpcom[255];
    sprintf(helpcom,"sh -c \"%s %s &\"",
	    WWWBROWSER,
	    HELP_HTML);
    if (system(helpcom)){};
    break;
  case HMVIEW_ABOUT:
    wxMessageBox(authors, "About",wxOK);
    break;
  default:
    if (Nr_of_Frames != 1)  
      wxMessageBox("Please close all spawned\n"
		   "frames before doing this! "
		   "Otherwise\n"
		   "I might have to plot undefined Histograms."
		   "\n","Warning");
    else 
      if (HMMenuCallback) HMMenuCallback(id); 
  }
}

void HMVFrame::infobox()
{
  if (!HMVhistogram) return;
  HIST *ids;
  int   nx, ny;
  FLOAT minx,miny,maxx,maxy;
  FLOAT maxi, mini, maxipos, minipos, rms,
    inte, fwhm, fwhmpos, mean;
  FLOAT regInt, regRMS, regMean;
  char infos[512];
  
  ids = HMVhistogram;
  HMBookParams(ids,&nx,&ny,&minx,&maxx,&miny,&maxy);

  if (HMDimension(ids)==1) {
    HMInfo1D(ids,&maxi,&maxipos,&mini,&minipos,
	     &fwhm,&fwhmpos,&rms,&mean,&inte);
    HMMinimumX(hv_minx);  HMMaximumX(hv_maxx);
    regInt = HMIntegralRegion(ids,&regRMS,&regMean);
    sprintf(infos,
	    "1D - HISTOGRAM INFO\n\n"
	    "Title:      ""%s""\n"
	    "Entries:    %d\n"
	    "Range:      %g <x< %g (%d Bins)\n"
	    "Minimum:    %g at %g\n"
	    "Maximum:    %g at %g\n"
	    "FWHM:       %g at %g\n"
	    "RMS:        %g\n"
	    "Mean:       %g\n"
	    "Integral:   %g\n"
	    "Region:\n"
	    "  RMS:      %g\n"
	    "  Mean:     %g\n"
	    "  Integral: %g",
	    ids->title,ids->entries,
	    minx,maxx,nx,
	    mini,minipos,maxi,maxipos,fwhm,fwhmpos, rms,mean,inte,
	    regRMS, regMean, regInt);
  } else
    sprintf(infos,
	    "2D - HISTOGRAM INFO\n\n"
	    "Title:    ""%s""\n"
	    "Entries:  %d\n"
	    "Range X:  %g <x< %g (%d Bins)\n"
	    "Range Y:  %g <y< %g (%d Bins)",
	    ids->title,ids->entries,
	    minx,maxx,nx, miny,maxy,ny);
  
  wxMessageBox(infos, "Histogram Info", wxOK);  
}

Bool HMVFrame::OnClose(void)
{
  return TRUE; 
}

void HMVFrame::OnSize(int w, int h)
{
  wxFrame::OnSize(w, h);
  Control->SetSize(w, 100);
  panel->SetSize(w, h - 165);
  panel->SetScrollbars(18, 18, sizex/18, sizey/18, 4, 4);
}


HMVPage::HMVPage(wxFrame *parent, char *title,
		 int x, int y, int width, int height, long style, char *name):
  wxFrame(parent, title, x, y, width, height, style, name)
{
  char * twname = new char[strlen(name)+12];
  strcpy(twname, name);
  strcat(twname, "TextWindow");

  // Create child subwindows.
  textWindow = new wxTextWindow(this, 0, 0, width, height,
				wxREADONLY|wxHSCROLL, twname);
  delete[] twname;
  frozen = 1;

  file_menu  = new wxMenu;
  file_menu->Append(HMVPAGE_FREEZE, "Free&ze",
		    "Toggle Flag: Update/Freeze Display", TRUE);
  file_menu->Append(HMVPAGE_SAVE,   "&Save", "Save window in file");
  file_menu->Append(HMVPAGE_PRINT,  "&Print", "Send window to printer");
  file_menu->AppendSeparator();
  file_menu->Append(HMVPAGE_CLOSE,  "&Close", "Close window");
  file_menu->Check(HMVPAGE_FREEZE, frozen);

  help_menu  = new wxMenu;
  help_menu->Append(HMVPAGE_ABOUT,  "&About", "About Stat Page");

  menu_bar = new wxMenuBar;
  menu_bar->Append(file_menu, "&File");
  menu_bar->Append(help_menu, "&Help");

  SetMenuBar(menu_bar);
  // CreateStatusLine();

  // Ensure the subwindows get resized o.k.
  OnSize(width, height);

  // Centre frame on the screen.
  Centre(wxBOTH);

  // Show the frame.
  Show(FALSE);
}

HMVPage::~HMVPage(void)
{
  delete menu_bar;
  delete help_menu;
  delete file_menu;
  delete textWindow;
}

Bool
HMVPage::OnClose(void)
{
  Show(FALSE);
  return FALSE;
}

void
HMVPage::OnSize(int, int)
{
  // Return if subwindows not yet initialized.
  if (!textWindow) return;

  int clientW, clientH;
  GetClientSize(&clientW, &clientH);

  textWindow->SetSize(0, 0, clientW, clientH);
}

void
HMVPage::OnMenuCommand(int commandId)
{
  char *selection;
  char *prcom;
  char *prenv;
  FILE *printer = NULL;

  switch (commandId) {
  case HMVPAGE_FREEZE:
    frozen = !frozen;
    break;
  case HMVPAGE_SAVE:
    selection = wxFileSelector("Save Counter Page", NULL, NULL, NULL, "*.cnt");
    if (selection) textWindow->SaveFile(selection);
    break;
  case HMVPAGE_PRINT:
    prenv = getenv("PRINTER");
    if (prenv) {
      prcom = new char[7+strlen(prenv)];
      strcpy(prcom, "lpr ");
      strcat(prcom, prswitch);
      strcat(prcom, prenv);
    } else {
      prcom = new char[4];
      strcpy(prcom, "lpr");
    }
    printer = popen(prcom, "w");
    if (printer) {
      char *contents = textWindow->GetContents();
      fputs(contents, printer);
      fclose(printer);
      printer = NULL;
      delete contents;
    }
    delete prcom;
    break;
  case HMVPAGE_ABOUT:
    break;
  case HMVPAGE_CLOSE:
  default:
    Show(FALSE);
    break;
  }
}

Bool
HMVPage::IsFrozen()
{
  if (!textWindow) return TRUE;

  return frozen; // file_menu->Checked(HMVPAGE_FREEZE);
}
