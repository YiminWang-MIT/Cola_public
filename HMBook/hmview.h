// -*-C++-*-

#include "wx.h"
#include "wx_main.h"
#include "wx_timer.h"
#include "wx_scrol.h"

void        HMLaunchViewWork(int arc, char *arv[], void (*work) (wxApp *wap));
extern void (*HMMenuCallback) (int id);

class HMViewApp: public wxApp
{
  public:
    wxFrame *OnInit(void);
};

class BitmapPanel: public wxPanel
{
  public:
    class HMVFrame *frame;
    wxMessage *posx;
    wxMessage *posy;
    wxMessage *poscont;
    BitmapPanel(class HMVFrame *frme, int x, int y, int w, int h, long style);
    void OnPaint(void);
    void OnEvent(wxMouseEvent& ev); 
};

class MyCanvas: public wxCanvas
{
  public:
    MyCanvas(wxWindow *parent, int x, int y, int w, int h, long style = wxBORDER):
      wxCanvas(parent, x, y, w, h, style)
    {
    }
    void OnPaint(void)
    {
      wxDC *dc = GetDC();
      dc->SetPen(wxBLACK_PEN);
      dc->SetBrush(wxGREEN_BRUSH);
      dc->DrawRoundedRectangle(20, 20, 100, 100);
    }
};

// Define a new frame
class HMVFrame: public wxFrame
{
  public:
    FILE  *gspipein, *gspipeout;
    int    gsprocid;
    wxMenuBar *menu_bar;
    wxBitmap  *backgroundBitmap;
    char      *bit;
    wxMessage *posx, *posy, *poscont, *scrollLab;
    wxButton  *histos;
    wxScrollBar *scrollReb;
    wxChoice  *choice1d, *choice2d;
    wxPanel   *panel, *Control;
    int    res, sizex, sizey, xold, yold;
    float  hv_maxx, hv_minx, hv_maxy, hv_miny, hv_maxz, hv_minz;
    int    current_resolution;
    wxMenu *zoom_menu;
    wxMenu *file_menu;
    wxMenu *histo_menu;
    wxMenu *macro_menu;
    wxMenu *help_menu;
    wxMenu *fit_menu;
    wxMenu *standardfit_menu;
    wxMenu *up_menu;
    HIST   *HMVhistogram;
    class  HMVTimer *timer;
    char   *fitfunction;
    int    fitdimension;
    char   *hmoptions;
    class  HMVPage *counterPage, *statPage;

    ~HMVFrame();
    HMVFrame(wxFrame *fr, char *title, int x, int y, int w, int h, long style);
    void create_gs_pipe();
    void close_gs_pipe();
    void readfile(char *filename);
    void print_current();
    void clear_current();
    void save_current();
    void save_current_ascii();
    void save_current_ascii_tuple();
    void gnuplotplot();
    char *read_bitmap();
    void plothist(wxChoice& ch, wxEvent& event);
    void infobox();
    void kill_gs_pipe();
    void executemacro(char *s);
    Bool OnClose(void);
    void OnSize(int w, int h);
    void OnMenuCommand(int id);
};

class HMVTimer: public wxTimer
{ private: 
  public:
    HMVFrame *frame;
    void Notify();
};

class HMVPage : public wxFrame {
protected:
  wxMenuBar *menu_bar;
  wxMenu    *file_menu;
  wxMenu    *help_menu;
  Bool       frozen;
public:
  wxTextWindow * textWindow;

  HMVPage(wxFrame *parent, char *title,
	  int x, int y, int width, int height, long style, char *name);
  ~HMVPage(void);

  Bool OnClose(void);
  void OnSize(int w, int h);
  void OnMenuCommand(int commandId);

  Bool IsFrozen();
};

const int HMVIEW_NEW_FRAME    = 101;
const int HMVIEW_LOAD_FILE    = 102;
const int HMVIEW_SAVE_FILE    = 103;
const int HMVIEW_EXPORT_FILE  = 104;
const int HMVIEW_QUIT         = 105;
const int HMVIEW_DELETE_FRAME = 106;

const int HMVIEW_ZOOM         = 200;

const int HMVIEW_SAVE         = 301;
const int HMVIEW_PRINT        = 302;
const int HMVIEW_INFO         = 303 ;
const int HMVIEW_AUTOUPDATE   = 304;
const int HMVIEW_STOPUPDATE   = 305;
const int HMVIEW_UPDATE01     = 306;
const int HMVIEW_UPDATE05     = 307;
const int HMVIEW_UPDATE10     = 308;
const int HMVIEW_UPDATE20     = 309;
const int HMVIEW_UPDATE60     = 310;
const int HMVIEW_SAVE_ASCII   = 311;
const int HMVIEW_CLEAR        = 312;
const int HMVIEW_SAVE_ASCIITUPLE = 313;
const int HMVIEW_GNUPLOTPLOT  = 314;

const int HMVIEW_MACRO        = 401;

const int HMVIEW_ABOUT        = 501;
const int HMVIEW_HELP         = 502;
const int HMVIEW_FIT          = 600;
const int HMVIEW_STANDARDFIT  = 601;
const int HMVIEW_NOFIT        = 602;
const int HMVIEW_FITLIST      = 700;

const int HMVPAGE_FREEZE = 1000;
const int HMVPAGE_SAVE   = 1001;
const int HMVPAGE_PRINT  = 1002;
const int HMVPAGE_CLOSE  = 1003;
const int HMVPAGE_ABOUT  = 1900;
