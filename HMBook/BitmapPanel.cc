#include <config.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include <stdio.h>
#include <math.h>

#include "hmbook.h"
#include "hmplot.h"
#include "hmview.h"

BitmapPanel::BitmapPanel(HMVFrame *frme, int x, int y, int w, int h, 
			 long style):
  wxPanel(frme, x, y, w, h, style)
{ frame   = frme;
  posx    = frme->posx;
  posy    = frme->posy;
  poscont = frme->poscont;
}

void BitmapPanel::OnEvent(wxMouseEvent& me)
{ if (!frame->HMVhistogram) return;
  float x,y;

  SetLogicalFunction(wxXOR);
  CrossHair(frame->xold,frame->yold);

  if (me.Entering())
  { static HMVFrame* oldframe = NULL;
    if (oldframe && oldframe != frame) frame->plothist(*(frame->choice1d), me);
    oldframe = frame;
    return;
  }
  if (me.Leaving())
  { frame->xold = frame->yold = -1;
    return;
  }

  me.Position(&x,&y);
  float xf = HMXPixel(                x  * 2.54/frame->res),
	yf = HMYPixel((frame->sizey - y) * 2.54/frame->res);

  static int printmouseposition = (getenv("HMMOUSEPOSITION") !=NULL);
  if (printmouseposition)
  { static float xdown = 0, ydown = 0;
    if (me.LeftUp() && xdown == x && ydown == y)
    { printf("%g %g\n", xf, yf);     
      return;
    }
    if (me.LeftDown())
    { xdown = x;
      ydown = y;
      return; 
    }
  }

  frame->xold = (int) x; 
  frame->yold = (int) y;
  CrossHair(frame->xold,frame->yold);
  char mes[200];
                 
  HIST *his= frame->HMVhistogram;
  sprintf(mes,"%5s: %.7g",his->labelx, xf);posx->SetLabel(mes);
  sprintf(mes,"%5s: %.7g",his->labely, yf);posy->SetLabel(mes);
  sprintf(mes,"Bin: %.7g",(his->ny==0? 
			  HMExtract1(his,xf):HMExtract2(his,xf,yf)));
  poscont->SetLabel(mes);

  posx->Refresh();
  posy->Refresh();
  poscont->Refresh();
  if (me.LeftDown()) 
  { frame->hv_minx = xf; 
    if (frame->hv_minx<his->minx) frame->hv_minx=his->minx;
    CrossHair(frame->xold,0);
    if (HMDimension(his)==2)
    { frame->hv_miny = yf; 
      if (frame->hv_miny<his->miny) frame->hv_miny=his->miny;
      CrossHair(0,frame->yold);
    }
  }
  if (me.LeftUp())   
  { frame->hv_maxx = xf;
    if (frame->hv_maxx<frame->hv_minx) 
    { frame->hv_maxx = frame->hv_minx;
      frame->hv_minx = xf;
    }
    if (frame->hv_maxx>his->maxx) frame->hv_maxx = his->maxx;
    if (frame->hv_maxx - frame->hv_minx < 1e-9)  
      frame->hv_maxx = frame->hv_minx = NOMINIMUM;
    if (HMDimension(his)==2)
    { frame->hv_maxy = yf;
    if (frame->hv_maxy<frame->hv_miny) 
    { frame->hv_maxy = frame->hv_miny;
      frame->hv_miny = yf;
    }
    if (frame->hv_maxy>his->maxy) frame->hv_maxy = his->maxy;
    if (frame->hv_maxy - frame->hv_miny < 1e-9)  
      frame->hv_maxy = frame->hv_miny = NOMINIMUM;
    }
    frame->plothist(*(frame->choice1d),me);
  }

  if (me.RightDown()) 
  { frame->hv_miny = yf; 
    CrossHair(0,frame->yold);
  }

  if (me.RightUp())   
  { frame->hv_maxy = yf;
    if (fabs(frame->hv_maxy - frame->hv_miny) < 1e-9)  
    { if (yf>0) frame->hv_maxy = yf;
      else  frame->hv_maxy = NOMINIMUM;
      frame->hv_miny = NOMINIMUM;
    } 
    else
      if (frame->hv_maxy < frame->hv_miny) 
      { frame->hv_maxy = frame->hv_miny;
        frame->hv_miny = yf;
      }

    frame->plothist(*(frame->choice1d),me);
  }

  if (me.MiddleUp()) 
  { frame->hv_maxx = frame->hv_minx = NOMINIMUM;
    frame->hv_maxy = frame->hv_miny = NOMINIMUM;
    frame->hv_maxz = frame->hv_minz = NOMINIMUM;
    frame->plothist(*(frame->choice1d),me);
  }
}

void BitmapPanel::OnPaint(void)
{ int w=0,h=0;
  frame->GetSize(&w,&h);

  wxPanel::OnPaint();
  SetLogicalFunction(wxSET);
  Clear();
  SetBrush(wxWHITE_BRUSH);
  SetPen(wxWHITE_PEN);
  //  DrawRectangle(0,0,w/2,h);
  // DrawRectangle(0,0,w/2,h);
  SetBrush(wxBLACK_BRUSH);
  SetBackground(wxWHITE_BRUSH);
 
  wxDC *dc = GetDC();
  dc->SetLogicalFunction(wxCLEAR);
  dc->SetBrush(wxWHITE_BRUSH);
  dc->SetPen(wxWHITE_PEN);
  dc->SetBackground(wxWHITE_BRUSH);
  dc->Clear();
  dc->DrawRectangle(0,0,w,h);
  dc->SetPen(wxBLACK_PEN);
  if (!frame->backgroundBitmap) return;
  wxMemoryDC memDC;

  memDC.SelectObject(frame->backgroundBitmap);
  if (frame->backgroundBitmap->GetColourMap())
  { memDC.SetColourMap(frame->backgroundBitmap->GetColourMap());
    dc->SetColourMap(frame->backgroundBitmap->GetColourMap());
  }
    
  dc->Blit(0, 0, w, h, &memDC, 0, 0);
  memDC.SelectObject(NULL);
  SetLogicalFunction(wxXOR);
  CrossHair(frame->xold, frame->yold);
}
















