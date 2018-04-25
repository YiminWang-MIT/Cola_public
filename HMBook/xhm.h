class XHMApp: public wxApp
{
  public:
    wxFrame *OnInit(void);
};

// Define a new frame
class XHMFrame: public wxFrame
{
  public:
    wxMenuBar *menu_bar;
    wxPanel   *panel;
    wxMenu *file_menu;
    wxMenu *help_menu;

    ~XHMFrame();
    XHMFrame(wxFrame *frame, 
	     char *title, int x, int y, int w, int h, long style);
    Bool OnClose(void);
    void OnSize(int w, int h);
    void OnMenuCommand(int id);
};


#define XHM_QUIT       105
#define XHM_ABOUT      501
#define XHM_HELP       502


