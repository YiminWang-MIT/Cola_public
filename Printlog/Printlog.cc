#define _POSIX_C_SOURCE 2
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

extern "C" void _exit(int); 
class ggoodbye { public:   ggoodbye(){};~ggoodbye() {_exit(0);};};
ggoodbye ggdbye; // that's ugly, but avoids core dump by wxWindows

#include "wx.h"
#include "wx_main.h"
#include "wx_timer.h"
#include <ctime>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include "HMBook/hmbook.h"
#include "HMBook/hmview.h"

#define GCC_VERSION (__GNUC__*10000+__GNUC_MINOR__*100+__GNUC_PATCHLEVEL__)
#if !defined(__sgi) && GCC_VERSION < 30200
extern "C" int putenv(const char *string); // This should be POSIX standard!
#endif

#define env_def(name,var,default) char *name = (char *)getenv(var);\
                                  if (!name) name= default;

static char timeunit[255];
static FILE *fp; 
static char *histfile = NULL;
static time_t start_t=0,
              end_t=0;
static int isonline=0;

class parameter 
{ 
  private:
  public:
    static parameter *first;
    static parameter *last;
    parameter *next;
    HIST  *hist;
    char  *name;
    int   nr;
    FLOAT *x,*y;
    int   n, nmax;
    parameter(char *name, int nr);
    static parameter *find(char *name, int nr);
    parameter(char *nme, int number, char *aliasname, char *unit);
    parameter(char *nme, int number, char *aliasname, char *label, char *unit);
    void fill(FLOAT x, FLOAT y);
};

parameter *
parameter::first = NULL;
parameter *
parameter::last  = NULL;

parameter::parameter(char *nme, int number)
{ next = NULL;
  if (!first)  first = this; 
  else         last->next = this;
  last = this;

  name = new char[strlen(nme)+1];
  strcpy(name,nme);

  nr = number;
  char *title=new char[255];
  sprintf(title,"%s",name);
  hist = HMBook1(title,"t","",timeunit,"",100,0,1);
  //  printf("Histogram [%s] generated\n",title);
  nmax = 8192;
  x = new FLOAT[nmax];
  y = new FLOAT[nmax];
  x[0] = y[0] = 0.0;
  n = 1;
}

parameter::parameter(char *nme, int number, char *aliasname, char *unt)
{ next = NULL;
  if (!first)  first = this; 
  else         last->next = this;
  last = this;

  name = new char[strlen(nme)+1];
  strcpy(name,nme);
  nr = number;
  if (strcmp(aliasname,"omit"))
  { hist = HMBook1(aliasname,"t","",timeunit,unt,100,0,1);
  nmax = 8192;
    x = new FLOAT[nmax];
    y = new FLOAT[nmax];
    x[0] = y[0] = 0.0;
    n = 1;
  }
  else
    hist = NULL;
}

parameter *
parameter::find(char *nme, int n)
{ parameter *result = first;
  while(result!=NULL && (strcmp(nme,result->name) || result->nr!=n)) 
    result=result->next;
  if (!result) result = new parameter(nme, n);
  return result;
}

void 
parameter::fill(FLOAT xx, FLOAT yy)
{ if (!hist) return;
  if (n >= nmax - 2)
  { nmax = (int) (nmax * 2);
    FLOAT *xn = new FLOAT[nmax];
    FLOAT *yn = new FLOAT[nmax];
    int i; for (i=0; i<n;i++) { xn[i] =x[i]; yn[i]=y[i];}
    delete x;
    delete y;
    x = xn;
    y = yn;
    //    printf("%s -> %d\n", name, nmax);
  }

  if (xx <= 0.0) {
    x[1]=0.0;
    y[1]=yy;
    if (n < 2)
      n = 2;
    HMSetArray(hist,n,x,y);
  } else {
    x[n]=xx;
    y[n]=y[n - 1];
    n++;
    HMSetArray(hist,n,x,y);

    x[n]=xx;
    y[n]=yy;
    n++;
    HMSetArray(hist,n,x,y);
  }
}

void HMLaunchViewWork(int arc, char *arv[], void (*work) (wxApp *wap));
int  work (wxApp *app);
static int argc; 
static char ** argv;

int main(int ac, char *av[])
{ argc=ac;
  argv=av;

  char *filename  = NULL;
  char *begintime = NULL;
  char *endtime   = NULL;

  //  putenv("TZ=MET-1METDST");  tzset(); // Timezone Central Europe required!
  putenv("TZ=Europe/Berlin");  tzset(); // Timezone Central Europe required!
  int opt, compress=0;
  while (EOF != (opt = getopt(ac, av, "b:cde:f:ghino:r:svz?"))) 
  { switch (opt) 
    { case 'b' : strcpy(begintime = new char[255], optarg); break;
      case 'e' : strcpy(endtime = new char[255], optarg); break;
      case 'z' : compress=1; break;
      case 'f' : strcpy(filename = new char[255], optarg); break;    
      case 'o' : strcpy(histfile = new char[255], optarg); break;    
      case 'h' : puts("Usage: Printlog++ [options]");
                 puts(" ");    
                 puts("no options        use current dbman logfile ");    
                 puts("-f filename       use 'filename' as inputfile");
                 puts("-f -              use 'stdin' as inputfile");
                 puts("-b yymmdd[hhmmss] begin at this time");
                 puts("-b yymmdd[hhmmss] begin at this time");
                 puts("-b today          begin this morning 00:00:00");
                 puts("-e yymmdd[hhmmss] end at this time");
                 puts("-z                decompress inputfile");
                 break;           
      default:  fprintf(stderr,"Unkown option '%c'\n",opt);
    }
  }

  //// open file or pipe /////////////////////

  if (filename==NULL) {
      fprintf(stderr,"Invalid logfile\n");
      return 1;
  }

  if (!strcmp(filename,"-")) fp=stdin;
  else
    if (compress)
    { char command[255];
      sprintf(command, GZIP " -c -d %s 2>/dev/null", filename);
      if (!(fp = popen(command, "r")))
      { fprintf(stderr, "Can't open zip pipe for %s",filename);
        exit(-1);
      }
    }
    else
      if (!(fp = fopen(filename,"r")))
      { fprintf(stderr, "Can't open %s",filename);
        exit(-1);
      }

  //// calculate start and stop time ///////////////

  struct tm       start;
  time_t now;  
  time(&now); 
  memcpy(&start, localtime(&now), sizeof(start));

  if (begintime)
  { int l = strlen(begintime);

    if(!strcmp(begintime, "today"))
      start.tm_hour=start.tm_min=start.tm_sec=0;
    else
    { start.tm_mon=start.tm_mday=start.tm_hour=start.tm_min=start.tm_sec=0;
      start.tm_year=100+(begintime[ 0]-'0')*10 + begintime[ 1]-'0';
      if (l> 3) start.tm_mon =(begintime[ 2]-'0')*10 + begintime[ 3]-'0' - 1;
      if (l> 5) start.tm_mday=(begintime[ 4]-'0')*10 + begintime[ 5]-'0';
      if (l> 7) start.tm_hour=(begintime[ 6]-'0')*10 + begintime[ 7]-'0';
      if (l> 9) start.tm_min =(begintime[ 8]-'0')*10 + begintime[ 9]-'0';
      if (l>11) start.tm_sec =(begintime[10]-'0')*10 + begintime[11]-'0';
      start_t = mktime(&start);
    }
  }

  if (endtime)
  { int l = strlen(endtime);
    start.tm_mon=start.tm_mday=start.tm_hour=start.tm_min=start.tm_sec=0;
    start.tm_year=100+(endtime[ 0]-'0')*10 + endtime[ 1]-'0';
    if (l> 3) start.tm_mon =(endtime[ 2]-'0')*10 + endtime[ 3]-'0' - 1;
    if (l> 5) start.tm_mday=(endtime[ 4]-'0')*10 + endtime[ 5]-'0';
    if (l> 7) start.tm_hour=(endtime[ 6]-'0')*10 + endtime[ 7]-'0';
    if (l> 9) start.tm_min =(endtime[ 8]-'0')*10 + endtime[ 9]-'0';
    if (l>11) start.tm_sec =(endtime[10]-'0')*10 + endtime[11]-'0';
    end_t = mktime(&start);
  }

  char start_time[32];
  char end_time[32];
  if (start_t) 
  { strncpy(start_time, ctime(&start_t)+4, 12);
    start_time[12]=0;
    sprintf(timeunit,"min. since %s",start_time);
    printf("Starttime: %s \n",start_time);
  }
  else
    sprintf(timeunit,"min.");
  if (end_t)
  { strncpy(end_time, ctime(&end_t)+4, 12);
    end_time[12]=0;
    printf("Endtime:   %s \n",end_time);
  }

  if (getenv("DISPLAY")) HMLaunchViewWork(0, argv, (void (*)(wxApp *))work);
  else    while (!work((wxApp *) NULL));
  exit(0);
  return 0;
}

static int lastpos=0;
static int WAIT = 0;

class PrintlogTimer: public wxTimer
{ private: 
  public:
    wxApp *app;
    HMVFrame *frame;
    void Notify();
};

void PrintlogTimer::Notify()
{ Stop();
  frame->SetStatusText("Wakeup");  
//   app->work_proc = (void (*)(wxApp *)) work;

   while (!work(app));
}

static PrintlogTimer *timer = new PrintlogTimer;

int work (wxApp *app)
{
  static char device[255];
  static time_t date;
  static double value;
  static HMVFrame *frame;
  static int init = 1;
  static int line = 0;

  if (WAIT)
  { if (lastpos) fseek(fp, lastpos, 0);
    WAIT = 0;
  }

  if (init)
    { init=0;
      if (app)
      { frame = (HMVFrame *) app->GetTopWindow();
        frame->choice1d->SetSelection(1);
        frame->choice1d->Refresh();
        timer->app = app;
        timer->frame = frame;
      }
  }

  char status[255];

  if (fscanf(fp,"%ld",(long *)&date));
  if ( feof(fp)) 
  { if (histfile) { HMWriteAll(histfile); exit(0); }
    if (isonline)
    { WAIT = -1; 
      sprintf(status,"%d lines read, sleeping for 10 sec",line);
      if (app)
      { frame->SetStatusText(status);  
        timer->Start(10000);
      }
    }
    return -1;
  }
  line++;

  if (!(line%5000))
  {
    sprintf(status,"%d lines read",line);
    if (app) frame->SetStatusText(status);
  }  
  if (!start_t) start_t = date;
  if (end_t==0 || end_t>date)
  { int d1,d2,d3;
    if (fscanf(fp,"%s%d%d%d%lf",device,&d1,&d2,&d3,&value));
    if (!strchr(device, '@')) {
      parameter *p = parameter::find(device, d1);
      p->fill((date-start_t)/60.0, value);
    }
  }
  while(fgetc(fp)!='\n'&&!feof(fp));  

  lastpos=ftell(fp);

  return 0;
}








