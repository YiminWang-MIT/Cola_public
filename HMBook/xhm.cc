
#include "wx.h"
#include "wx_main.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "xhm.h"


#define IconSpaceH 90
#define IconSpaceV 90

#define ICON      HMBOOK_ROOT "xhm.icon"

static XHMApp XHMApp;

wxFrame *XHMApp::OnInit(void)
{
  XHMFrame   *frme = new XHMFrame(NULL, "Histogram View", 
				  0, 0, 600, 570, wxSDI|wxDEFAULT_FRAME);
  return frme;
}

XHMFrame::~XHMFrame()
{
}

XHMFrame::XHMFrame(wxFrame *frme, char *title, 
		   int x, int y, int w, int h, long style)
 : wxFrame(frme, title, x, y, w, h, style)
{
//  SetIcon(new wxIcon(ICON));


  file_menu  = new wxMenu;
  help_menu  = new wxMenu;
  wxFont *myfont = wxTheFontList->FindOrCreateFont(14, wxSWISS, 
						   wxNORMAL, wxNORMAL);
  SetFont(myfont);

  file_menu->Append(XHM_QUIT,  "&Quit");
  help_menu->Append(XHM_ABOUT, "&About");
  help_menu->Append(XHM_HELP,  "&Help");

  menu_bar = new wxMenuBar;
  menu_bar->Append(file_menu, "&File");
  menu_bar->Append(help_menu, "&Help");
  SetMenuBar(menu_bar);

  CreateStatusLine();

  panel = new wxPanel(this,0,0,600);
  panel->SetLabelFont(myfont);
  panel->SetButtonFont(myfont);

  Show(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
//  Event Handler
///////////////////////////////////////////////////////////////////////////////

void XHMFrame::OnMenuCommand(int id)
{
    switch (id)
    {
       case XHM_QUIT:         { OnClose(); delete this; exit(0); }
       default: break;
    }
}

Bool XHMFrame::OnClose(void)
{ Show(FALSE);
  return TRUE; 
}

void XHMFrame::OnSize(int w, int h)
{ wxFrame::OnSize(w, h);
  wxIcon *Icon1 = new wxIcon("folder.xbm",wxBITMAP_TYPE_XBM );
  panel->Clear();
  int i;
  int x=0, y=0;
  panel->SetSize(w,h);
  for(i=0;i<20;i++)
    { wxButton *butt = new wxButton(panel, (wxFunction) NULL,
				    Icon1,x+IconSpaceH/2-25,y+20);
      
      new wxButton(panel, (wxFunction) NULL, 
		   "label",x+4,y+75,IconSpaceH);
      x += IconSpaceH;
      if (x+IconSpaceH > w) {x=0; y+=IconSpaceV;} 
    }


  panel->SetSize(w,h-165);
//  panel->SetScrollbars(18,18,sizex/18,sizey/18,4,4);
}










