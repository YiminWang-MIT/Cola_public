#include <iostream>
#include <list>
#include <cmath>
#include <cstdio>

#include "FibreEventDisplay.h"

#define STIME -600
#define ETIME 600
#define STEPS 100

using namespace std;

struct hit
{
  int channel;
  double time;
  int type;
} ;

struct track
{
  int channelX;
  int channelTh;
  double timeX;
  double timeTh;
  double quality;
} ;

struct clusters
{
  int multiplicity;
  int leadingChannel;
  int minChannel;
  int maxChannel;
  double minTime;
  double maxTime;
} ;

list<hit> *hitListX, *hitListTh; // List of all hits in one event
list<track> *trackList; // List of all possible trajectories
list<clusters> *clusterListX, *clusterListTh; // List of all closeups for one event

clusters *bestClusterX, *bestClusterTh;

FibreEventDisplay::FibreEventDisplay(int NoOfPages, double minTime, double maxTime, bool autoWriteHeader)
{
  fileOpen = false;
  outputFile = fopen("FibreEventDisplay.ps", "w");
  if (outputFile == NULL)
  {
    perror("FibreEventDisplay");
  }
  else
  {
    fileOpen = true;
  }
  pages = NoOfPages;
  if (autoWriteHeader)
  {
    writeHeader();
  }
  pageNumber = 1;
  pageUsed = false;
  hitListX = new list<hit>;
  hitListTh = new list<hit>;
  trackList = new list<track>;
  clusterListX = new list<clusters>;
  clusterListTh = new list<clusters>;
  bestClusterX = new clusters;
  bestClusterTh = new clusters;
  bestClusterX -> multiplicity = 0;
  bestClusterX -> leadingChannel = 0;
  bestClusterX -> minChannel = 0;
  bestClusterX -> maxChannel = 0;
  bestClusterX -> minTime = 0;
  bestClusterX -> maxTime = 0;
  bestClusterTh -> multiplicity = 0;
  bestClusterTh -> leadingChannel = 0;
  bestClusterTh -> minChannel = 0;
  bestClusterTh -> maxChannel = 0;
  bestClusterTh -> minTime = 0;
  bestClusterTh -> maxTime = 0;
  typeNumberX[0] = 0;
  typeNumberX[1] = 0;
  typeNumberX[2] = 0;
  typeNumberX[3] = 0;
  typeNumberTh[0] = 0;
  typeNumberTh[1] = 0;
  typeNumberTh[2] = 0;
  typeNumberTh[3] = 0;
  
	//clusterMinChannel = 0;
  //clusterMaxChannel = 287; /*   ändern!!!   */
}

FibreEventDisplay::~FibreEventDisplay()
{
  closeFile();
  delete hitListX;
  delete hitListTh;
  delete trackList;
  delete clusterListX;
  delete clusterListTh;
  delete bestClusterX;
  delete bestClusterTh;
}

void FibreEventDisplay::writeHeader()
{
  if (fileOpen)
  {
    fprintf(outputFile, "%%!PS-Adobe-2.0\n");
    fprintf(outputFile, "%%%%Creator: FibreDetector\n%%%%Pages: %d %d\n%%!EndComments\n", pages, pages);
    fprintf(outputFile, "/cross1 {newpath moveto 1 0 rlineto -2 0 rlineto 1 0 rlineto 0 1 rlineto 0 -2 rlineto 0 0 1 setrgbcolor 0.2 setlinewidth stroke} def\n");
    fprintf(outputFile, "/cross2 {newpath moveto 1 1 rlineto -2 -2 rlineto 1 1 rlineto -1 1 rlineto 2 -2 rlineto 0 1 0 setrgbcolor 0.2 setlinewidth stroke} def\n");
    fprintf(outputFile, "/cross3 {newpath moveto 1 1 rlineto -2 -2 rlineto 1 1 rlineto -1 1 rlineto 2 -2 rlineto 1 0 0 setrgbcolor 0.2 setlinewidth stroke} def\n");
    fprintf(outputFile, "/textL {0 0 0 setrgbcolor /Times findfont 6 scalefont setfont} def\n");
    fprintf(outputFile, "/textl {0 0 0 setrgbcolor /Times findfont 4.5 scalefont setfont} def\n");
    fprintf(outputFile, "/textS {0 0 0 setrgbcolor /Times findfont 3 scalefont setfont} def\n");
    fprintf(outputFile, "/texts {0 0 0 setrgbcolor /Times findfont 2 scalefont setfont} def\n");
    fprintf(outputFile, "/cshow {dup stringwidth pop 2 div neg 0 rmoveto show} def\n");
    fprintf(outputFile, "%%%%EndProlog\n\n");
    pageNumber = 1;
  }
}

void FibreEventDisplay::drawPage()
{
  if ((fileOpen) && (pageUsed))
  {
    fprintf(outputFile, "%%%%Page: %i %i\n2.8328611898 2.8328611898 scale %% switch to mm\n", pageNumber, pageNumber);
    fprintf(outputFile, "10 10 translate %% 1 cm border\n0 0 0 setrgbcolor\n0.2 setlinewidth\n\n");
    
    ////////////////////////////////////////////////////////
    // Draw Infobox for Event in X
    ////////////////////////////////////////////////////////
    
    fprintf(outputFile, "%% Infobox X\nnewpath 0 277 moveto 93 0 rlineto 0 -30 rlineto -93 0 rlineto closepath stroke\n");
    fprintf(outputFile, "textL\n3 270 moveto (Fibre X-Plane) show\ntextS\n");
    fprintf(outputFile, "3 265 moveto (Signal Multiplicity: %i) show\n", (int) hitListX -> size()); 
    if ((int) hitListX -> size() > 0)
    {
      fprintf(outputFile, "3 260 moveto (No. of Clusters: %i) show\n", (int) clusterListX -> size());
      fprintf(outputFile, "60 270 moveto (Signals in Clusters: %i) show\n", typeNumberX[1]);
      fprintf(outputFile, "60 265 moveto (Afterpulses:  %i) show\n", typeNumberX[2]);
      fprintf(outputFile, "60 260 moveto (Single Pulses: %i) show\n", typeNumberX[3]);
      fprintf(outputFile, "newpath 57 270 moveto 2 0 rlineto 0 2 rlineto -2 0 rlineto closepath 0 0 1 setrgbcolor fill stroke\n");
      fprintf(outputFile, "newpath 57 265 moveto 2 0 rlineto 0 2 rlineto -2 0 rlineto closepath 0 1 0 setrgbcolor fill stroke\n");
      fprintf(outputFile, "newpath 57 260 moveto 2 0 rlineto 0 2 rlineto -2 0 rlineto closepath 1 0 0 setrgbcolor fill stroke\n");
    }
    double frac1, frac2, frac3;
    if (hitListX -> size() == 0)
    {
      frac1 = 0;
    } else {
      frac1 = 87 * ((double) typeNumberX[1]) / ((double) hitListX -> size());
    }
    if (frac1 > 0.0001) fprintf(outputFile, "newpath 3 250 moveto %.1f 0 rlineto 0 5 rlineto -%.1f 0 rlineto closepath 0 0 1 setrgbcolor fill stroke\n", frac1, frac1);
    
    if (hitListX -> size() == 0)
    {
      frac2 = 0;
    } else {
      frac2 = 87 * ((double) typeNumberX[2]) / ((double) hitListX -> size());
    }
    if (frac2 > 0.0001) fprintf(outputFile, "newpath %.1f 250 moveto %.1f 0 rlineto 0 5 rlineto -%.1f 0 rlineto closepath 0 1 0 setrgbcolor fill stroke\n", 3 + frac1, frac2, frac2);
    
    if (hitListX -> size() == 0)
    {
      frac3 = 0;
    } else {
      frac3 = 87 * ((double) typeNumberX[3]) / ((double) hitListX -> size());
    }
    if (frac3 > 0.0001) fprintf(outputFile, "newpath %.1f 250 moveto %.1f 0 rlineto 0 5 rlineto -%.1f 0 rlineto closepath 1 0 0 setrgbcolor fill stroke\n", 3 + frac1 + frac2, frac3, frac3);
    fprintf(outputFile, "0 0 0 setrgbcolor\n\n");
    
    ////////////////////////////////////////////////////////
    // Draw Infobox for Event in Th
    ////////////////////////////////////////////////////////
    
    fprintf(outputFile, "%% Infobox Th\nnewpath 97 277 moveto 93 0 rlineto 0 -30 rlineto -93 0 rlineto closepath stroke\n");
    fprintf(outputFile, "textL\n100 270 moveto (Fibre Th-Plane) show\ntextS\n");
    fprintf(outputFile, "100 265 moveto (Signal Multiplicity: %i) show\n", (int) hitListTh -> size());
    if ((int) hitListTh -> size() > 0)
    {
      fprintf(outputFile, "100 260 moveto (No. of Clusters: %i) show\n", (int) clusterListTh -> size());
      fprintf(outputFile, "157 270 moveto (Signals in Clusters: %i) show\n", typeNumberTh[1]);
      fprintf(outputFile, "157 265 moveto (Afterpulses:  %i) show\n", typeNumberTh[2]);
      fprintf(outputFile, "157 260 moveto (Single Pulses: %i) show\n", typeNumberTh[3]);
      fprintf(outputFile, "newpath 154 270 moveto 2 0 rlineto 0 2 rlineto -2 0 rlineto closepath 0 0 1 setrgbcolor fill stroke\n");
      fprintf(outputFile, "newpath 154 265 moveto 2 0 rlineto 0 2 rlineto -2 0 rlineto closepath 0 1 0 setrgbcolor fill stroke\n");
      fprintf(outputFile, "newpath 154 260 moveto 2 0 rlineto 0 2 rlineto -2 0 rlineto closepath 1 0 0 setrgbcolor fill stroke\n");
    }
    if (hitListTh -> size() == 0)
    {
      frac1 = 0;
    } else {
      frac1 = 87 * ((double) typeNumberTh[1]) / ((double) hitListTh -> size());
    }
    if (frac1 > 0.0001) fprintf(outputFile, "newpath 100 250 moveto %.1f 0 rlineto 0 5 rlineto -%.1f 0 rlineto closepath 0 0 1 setrgbcolor fill stroke\n", frac1, frac1);
    
    if (hitListTh -> size() == 0)
    {
      frac2 = 0;
    } else {
      frac2 = 87 * ((double) typeNumberTh[2]) / ((double) hitListTh -> size());
    }
    if (frac2 > 0.0001) fprintf(outputFile, "newpath %.1f 250 moveto %.1f 0 rlineto 0 5 rlineto -%.1f 0 rlineto closepath 0 1 0 setrgbcolor fill stroke\n", 100 + frac1, frac2, frac2);

    if (hitListTh -> size() == 0)
    {
      frac3 = 0;
    } else {
      frac3 = 87 * ((double) typeNumberTh[3]) / ((double) hitListTh -> size());
    }
    if (frac3 > 0.0001) fprintf(outputFile, "newpath %.1f 250 moveto %.1f 0 rlineto 0 5 rlineto -%.1f 0 rlineto closepath 1 0 0 setrgbcolor fill stroke\n", 100 + frac1 + frac2, frac3, frac3);
    fprintf(outputFile, "0 0 0 setrgbcolor\n\n");
    
    ////////////////////////////////////////////////////////
    // Draw overview over all hits
    ////////////////////////////////////////////////////////
    
    // channel numbers X
    fprintf(outputFile, "%% Overview\nnewpath 0 243 moveto 88 0 rlineto 0 -80 rlineto -88 0 rlineto closepath stroke\n");
    fprintf(outputFile, "newpath 102 243 moveto 88 0 rlineto 0 -80 rlineto -88 0 rlineto closepath stroke\n");
    fprintf(outputFile, "textS\n-0.8 159 moveto (0) show\n17 159 moveto (500) show\n35.4 159 moveto (1000) show\n");
    fprintf(outputFile, "54.5 159 moveto (1500) show\n73.6 159 moveto (2000) show\n81.2 159 moveto (Channel) show\n");
    fprintf(outputFile, "newpath 0 163 moveto 0 -1.5 rlineto stroke\nnewpath 19.1 163 moveto 0 -1.5 rlineto stroke\nnewpath 38.2 163 moveto 0 -1.5 rlineto stroke\n");
    fprintf(outputFile, "newpath 57.3 163 moveto 0 -1.5 rlineto stroke\nnewpath 76.4 163 moveto 0 -1.5 rlineto stroke\nnewpath 88 163 moveto 0 -1.5 rlineto stroke\n");
    // channel numbers Theta
    fprintf(outputFile, "101.2 159 moveto (0) show\n119 159 moveto (500) show\n137.4 159 moveto (1000) show\n");
    fprintf(outputFile, "156.5 159 moveto (1500) show\n175.6 159 moveto (2000) show\n183.2 159 moveto (Channel) show\n");
    fprintf(outputFile, "newpath 102 163 moveto 0 -1.5 rlineto stroke\nnewpath 121.1 163 moveto 0 -1.5 rlineto stroke\nnewpath 140.2 163 moveto 0 -1.5 rlineto stroke\n");
    fprintf(outputFile, "newpath 159.3 163 moveto 0 -1.5 rlineto stroke\nnewpath 178.4 163 moveto 0 -1.5 rlineto stroke\nnewpath 190 163 moveto 0 -1.5 rlineto stroke\n");
    // Time koordinates
    fprintf(outputFile, "95 240.5 moveto (Time [ns]) cshow\n");
    for (int i = STIME / STEPS; i <= ETIME / STEPS; i += 1)
    {
      if (80 * (STEPS * i - STIME) / (ETIME - STIME) < 76.5)
      {
        fprintf(outputFile, "95 %d moveto (%d) cshow\n", 162 + 80 * (STEPS * i - STIME) / (ETIME - STIME), i * STEPS);
      }
      fprintf(outputFile, "newpath 88 %.1f moveto 1 0 rlineto stroke\nnewpath 102 %.1f moveto -1 0 rlineto stroke\n", 163 + 80 * (STEPS * i - STIME) / (double)(ETIME - STIME), 163 + 80 * (STEPS * i - STIME) / (double)(ETIME - STIME));
    }
    if (-STIME / (double)(ETIME -STIME) > 0 && -STIME / (double)(ETIME -STIME) < 1)
    {
      fprintf(outputFile, "newpath 0 %.1f moveto 88 0 rlineto stroke\nnewpath 102 %.1f moveto 88 0 rlineto stroke\n", 163 - 80 * STIME / (double)(ETIME -STIME), 163 - 80 * STIME / (double)(ETIME -STIME));
    }
    
    
    
    list<hit>::iterator it;
    // Draw all hits on X layer
    fprintf(outputFile, "gsave\nnewpath 0 243 moveto 88 0 rlineto 0 -80 rlineto -88 0 rlineto closepath clip\n");
    for (it = hitListX -> begin(); it != hitListX -> end(); it++)
    {
      if (it->type > 0 && it -> type < 4)
      {
        fprintf(outputFile, "%.2f %.2f cross%d\n", 0.03819 * it -> channel, 163 + 80 * (it -> time - STIME) / (double)(ETIME - STIME), it -> type);
      }
    }
    
    // Mark clusters in X layer
    //fprintf(outputFile, "0.5 0.5 0.5 setrgbcolor\n");
    list<clusters>::iterator it_clusters;
    for (it_clusters = clusterListX -> begin(); it_clusters != clusterListX -> end(); it_clusters++)
    {
      fprintf(outputFile, "newpath %.2f %.2f moveto %.2f 0 rlineto 0 %.2f rlineto -%.2f 0 rlineto closepath 0.5 0.5 0.5 setrgbcolor stroke\n",
             0.03819 * it_clusters -> minChannel - 1.5,
             163 + 80 * (it_clusters -> minTime - STIME) / (double)(ETIME - STIME) - 1.5,
             0.03819 * (it_clusters -> maxChannel - it_clusters -> minChannel) + 3,
             80 * (it_clusters -> maxTime - it_clusters -> minTime) / (double)(ETIME - STIME) + 3,
             0.03819 * (it_clusters -> maxChannel - it_clusters -> minChannel) + 3);
    }
    fprintf(outputFile, "grestore\n");
    
    // Draw all hits on Th layer
    fprintf(outputFile, "gsave\nnewpath 102 243 moveto 88 0 rlineto 0 -80 rlineto -88 0 rlineto closepath clip\n");
    for (it = hitListTh -> begin(); it != hitListTh -> end(); it++)
    {
      if (it -> type > 0 && it -> type < 4)
      {
        fprintf(outputFile, "%.2f %.2f cross%d\n", 102 + 0.03819 * it -> channel, 163 + 80 * (it -> time - STIME) / (double)(ETIME - STIME), it -> type);
      }
    }
    
    // Mark clusters in Th layer
    //fprintf(outputFile, "\n");
    //list<clusters>::iterator it_clusters;
    for (it_clusters = clusterListTh -> begin(); it_clusters != clusterListTh -> end(); it_clusters++)
    {
      fprintf(outputFile, "newpath %.2f %.2f moveto %.2f 0 rlineto 0 %.2f rlineto -%.2f 0 rlineto closepath 0.5 0.5 0.5 setrgbcolor stroke\n",
              102 + 0.03819 * it_clusters -> minChannel - 1.5,
              163 + 80 * (it_clusters -> minTime - STIME) / (double)(ETIME - STIME) - 1.5,
              0.03819 * (it_clusters -> maxChannel - it_clusters -> minChannel) + 3,
              80 * (it_clusters -> maxTime - it_clusters -> minTime) / (double)(ETIME - STIME) + 3,
              0.03819 * (it_clusters -> maxChannel - it_clusters -> minChannel) + 3);
    }
    fprintf(outputFile, "grestore\n\n");
    
    ////////////////////////////////////////////////////////
    // Draw track display box
    ////////////////////////////////////////////////////////
    
    fprintf(outputFile, "%% Tracks\nnewpath 0 157 moveto 190 0 rlineto 0 -53 rlineto -190 0 rlineto closepath stroke\n");
    fprintf(outputFile, "gsave\nnewpath 0 157 moveto 190 0 rlineto 0 -53 rlineto -190 0 rlineto closepath clip\n");
    fprintf(outputFile, "1 0 0 setcolor\n");
    
    // Draw single pulses
    for (it = hitListX->begin(); it != hitListX->end(); it++)
    {
      if (it->type == 3)
      {
          fprintf(outputFile, "newpath %.2f 148 moveto 0 2 rlineto 0.0651 0 rlineto 0 -2 rlineto fill stroke\n", 32 + it -> channel / 24.0);
      }
    }
    for (it = hitListTh->begin(); it != hitListTh->end(); it++)
    {
      if (it->type == 3)
      {
        fprintf(outputFile, "newpath %.2f 135 moveto 0 -2 rlineto 0.0651 0 rlineto 0 2 rlineto fill stroke\n", 62 + it -> channel / 24.0); // 35 + 0.0651 * it -> channel
      }
    }
    
    // Draw Afterpulses
    fprintf(outputFile, "0 1 0 setcolor\n");
    for (it = hitListX->begin(); it != hitListX->end(); it++)
    {
      if (it->type == 2)
      {
        fprintf(outputFile, "newpath %.2f 148 moveto 0 2 rlineto 0.0651 0 rlineto 0 -2 rlineto fill stroke\n", 32 + it -> channel / 24.0);
      }
    }
    for (it = hitListTh->begin(); it != hitListTh->end(); it++)
    {
      if (it->type == 2)
      {
        fprintf(outputFile, "newpath %.2f 135 moveto 0 -2 rlineto 0.0651 0 rlineto 0 2 rlineto fill stroke\n", 62 + it -> channel / 24.0);
      }
    }
    
    // Draw pulses in Clusters
    fprintf(outputFile, "0 0 1 setcolor\n");
    for (it = hitListX->begin(); it != hitListX->end(); it++)
    {
      if (it->type == 1)
      {
        fprintf(outputFile, "newpath %.2f 148 moveto 0 2 rlineto 0.0651 0 rlineto 0 -2 rlineto fill stroke\n", 32 + it -> channel / 24.0);
      }
    }
    for (it = hitListTh->begin(); it != hitListTh->end(); it++)
    {
      if (it->type == 1)
      {
        fprintf(outputFile, "newpath %.2f 135 moveto 0 -2 rlineto 0.0651 0 rlineto 0 2 rlineto fill stroke\n", 62 + it -> channel / 24.0);
      }
    }
    
    // Draw actual Tracks
    //fprintf(outputFile, "0 0 0 setcolor\nnewpath 5 148 moveto 150 0 rlineto stroke\nnewpath 35 135 moveto 150 0 rlineto stroke\n");
    fprintf(outputFile, "0 0 0 setcolor\nnewpath 32 148 moveto 96 0 rlineto stroke\nnewpath 62 135 moveto 96 0 rlineto stroke\n");
    // Tracks Info
    list<track>::iterator it_tracks = trackList -> begin();
    track buffer;
    buffer.quality = -1e100;
    buffer.channelX = 0;
    buffer.channelTh = 0;
    buffer.timeX = 0;
    buffer.timeTh = 0;
    track bestTracks[5];
    bestTracks[0] = buffer;
    bestTracks[1] = buffer;
    bestTracks[2] = buffer;
    bestTracks[3] = buffer;
    bestTracks[4] = buffer;
//     list<track>::iterator it_t1 = NULL;
//     list<track>::iterator it_t2 = NULL;
//     list<track>::iterator it_t3 = NULL;
//     list<track>::iterator it_t4 = NULL;
//     list<track>::iterator it_t5 = NULL;
    //int counter = 0;
    while (it_tracks != trackList -> end())
    {
      
      if (it_tracks -> quality > bestTracks[0].quality)
      {
        bestTracks[4] = bestTracks[3];
        bestTracks[3] = bestTracks[2];
        bestTracks[2] = bestTracks[1];
        bestTracks[1] = bestTracks[0];
        bestTracks[0] = *it_tracks;
      } else {
        if (it_tracks -> quality > bestTracks[1].quality)
        {
          bestTracks[4] = bestTracks[3];
          bestTracks[3] = bestTracks[2];
          bestTracks[2] = bestTracks[1];
          bestTracks[1] = *it_tracks;
        } else {
          if (it_tracks -> quality > bestTracks[2].quality)
          {
            bestTracks[4] = bestTracks[3];
            bestTracks[3] = bestTracks[2];
            bestTracks[2] = *it_tracks;
          } else {
            if (it_tracks -> quality > bestTracks[3].quality)
            {
              bestTracks[4] = bestTracks[3];
              bestTracks[3] = *it_tracks;
            } else {
              if (it_tracks -> quality > bestTracks[4].quality)
              {
                bestTracks[4] = *it_tracks;
              }
            }
          }
        }
      }
      it_tracks++;
    }
    if (bestTracks[0].quality > -1e99)
    {
      fprintf(outputFile, "textS\n60 125 moveto (Channel X) cshow\n80 125 moveto (Channel Th) cshow\n 100 125 moveto (Angle [deg]) cshow\n");
      fprintf(outputFile, "120 125 moveto (Flight Time[ns]) cshow\n140 125 moveto (Time Offset[ns]) cshow\n160 125 moveto (Quality) cshow\n");
      fprintf(outputFile, "newpath 50 124 moveto 170 124 lineto 0.5 0.5 0.5 setrgbcolor stroke\n");

      double angle = 180 * atan((0.83 * bestTracks[0].channelTh - 0.83 * bestTracks[0].channelX + 602.7) / 259.1) / 3.1415926;
      fprintf(outputFile,"textl\n60 120 moveto (%d) cshow\n80 120 moveto (%d) cshow\n100 120 moveto (%.2f) cshow\n120 120 moveto (%.2f) cshow\n140 120 moveto (%.2f) cshow\n160 120 moveto (%.2f) cshow\n",
             bestTracks[0].channelX,
             bestTracks[0].channelTh,
             angle,
             bestTracks[0].timeTh - bestTracks[0].timeX,
             (bestTracks[0].timeX + bestTracks[0].timeTh) / 2,
             bestTracks[0].quality);
    }
    if (bestTracks[1].quality > -1e99)
    {
      double angle = 180 * atan((0.83 * bestTracks[1].channelTh - 0.83 * bestTracks[1].channelX + 602.7) / 259.1) / 3.1415926;
      fprintf(outputFile,"textS\n60 116 moveto (%d) cshow\n80 116 moveto (%d) cshow\n100 116 moveto (%.2f) cshow\n120 116 moveto (%.2f) cshow\n140 116 moveto (%.2f) cshow\n160 116 moveto (%.2f) cshow\n",
              bestTracks[1].channelX,
              bestTracks[1].channelTh,
              angle,
              bestTracks[1].timeTh - bestTracks[1].timeX,
              (bestTracks[1].timeX + bestTracks[1].timeTh) / 2,
               bestTracks[1].quality);
    }
    if (bestTracks[2].quality > -1e99)
    {
      double angle = 180 * atan((0.83 * bestTracks[2].channelTh - 0.83 * bestTracks[2].channelX + 602.7) / 259.1) / 3.1415926;
      fprintf(outputFile,"60 113 moveto (%d) cshow\n80 113 moveto (%d) cshow\n100 113 moveto (%.2f) cshow\n120 113 moveto (%.2f) cshow\n140 113 moveto (%.2f) cshow\n160 113 moveto (%.2f) cshow\n",
              bestTracks[2].channelX,
              bestTracks[2].channelTh,
              angle,
              bestTracks[2].timeTh - bestTracks[2].timeX,
              (bestTracks[2].timeX + bestTracks[2].timeTh) / 2,
               bestTracks[2].quality);
    }
    if (bestTracks[3].quality > -1e99)
    {
      double angle = 180 * atan((0.83 * bestTracks[3].channelTh - 0.83 * bestTracks[3].channelX + 602.7) / 259.1) / 3.1415926;
      fprintf(outputFile,"60 110 moveto (%d) cshow\n80 110 moveto (%d) cshow\n100 110 moveto (%.2f) cshow\n120 110 moveto (%.2f) cshow\n140 110 moveto (%.2f) cshow\n160 110 moveto (%.2f) cshow\n",
              bestTracks[3].channelX,
              bestTracks[3].channelTh,
              angle,
              bestTracks[3].timeTh - bestTracks[3].timeX,
              (bestTracks[3].timeX + bestTracks[3].timeTh) / 2,
               bestTracks[3].quality);
    }
    if (bestTracks[4].quality > -1e99)
    {
      double angle = 180 * atan((0.83 * bestTracks[4].channelTh - 0.83 * bestTracks[4].channelX + 602.7) / 259.1) / 3.1415926;
      fprintf(outputFile,"60 107 moveto (%d) cshow\n80 107 moveto (%d) cshow\n100 107 moveto (%.2f) cshow\n120 107 moveto (%.2f) cshow\n140 107 moveto (%.2f) cshow\n160 107 moveto (%.2f) cshow\n",
              bestTracks[4].channelX,
              bestTracks[4].channelTh,
              angle,
              bestTracks[4].timeTh - bestTracks[4].timeX,
              (bestTracks[4].timeX + bestTracks[4].timeTh) / 2,
               bestTracks[4].quality);
    }
    fprintf(outputFile, "0.5 0.5 0.5 setrgbcolor\n");
    for (it_tracks = trackList -> begin(); it_tracks != trackList -> end(); it_tracks++)
    {
      //fprintf(outputFile, "newpath %.2f 148 moveto %.2f 135 lineto stroke\n", 5 + 150 * it_tracks -> channelX / 2304.0, 35 + 150 * it_tracks -> channelTh / 2304.0);
      fprintf(outputFile, "newpath %.2f 148 moveto %.2f 135 lineto stroke\n", 32 + it_tracks -> channelX / 24.0, 62 + it_tracks -> channelTh / 24.0);
    }
    if (bestTracks[0].quality > -1e99)
    {
      //fprintf(outputFile, "0 0 0 setrgbcolor\nnewpath %.2f 154.5 moveto %.2f 128.5 lineto stroke\n", 5 + 75 * (3 * bestTracks[0].channelX - bestTracks[0].channelTh - 460.8)/ 2304.0, 35 + 75 * (3 * bestTracks[0].channelTh - bestTracks[0].channelX + 460.8) / 2304.0);
      fprintf(outputFile, "0 0 0 setrgbcolor\nnewpath %.2f 154.5 moveto %.2f 128.5 lineto stroke\n",
              32 + (3 * bestTracks[0].channelX - bestTracks[0].channelTh - 720) / 48.0,
                        62 + (3 * bestTracks[0].channelTh - bestTracks[0].channelX + 720) / 48.0);
    }

    
    fprintf(outputFile, "textl\n133 147 moveto (X-Plane) show\n41 134 moveto (Th-Plane) show\n3 115 moveto (Number of) show\n3 110 moveto (Tracks: %d) show\n", (int) trackList -> size());
    fprintf(outputFile, "grestore\n\n");
    
    ////////////////////////////////////////////////////////
    // Draw best cluster info for X layer
    ////////////////////////////////////////////////////////
    
    fprintf(outputFile, "%% Cluster Info X\nnewpath 0 100 moveto 93 0 rlineto 0 -24 rlineto -93 0 rlineto closepath stroke\n");
    fprintf(outputFile, "textl\n3 94 moveto (Best Cluster X Info) show\ntextS\n");
    fprintf(outputFile, "3 88 moveto (Multiplicity: %d) show\n", bestClusterX -> multiplicity);
    if (bestClusterX -> multiplicity > 0)
    {
      fprintf(outputFile, "3 84 moveto (Central Position: %.1f mm) show\n", 0.5 * (bestClusterX -> maxChannel + bestClusterX -> minChannel) * 0.83);
      fprintf(outputFile, "3 80 moveto (Leading Time: %.1f ns) show\n", bestClusterX -> minTime);
      fprintf(outputFile, "50 88 moveto (Leading Channel: %d) show\n", bestClusterX -> leadingChannel);
      fprintf(outputFile, "50 84 moveto (Width: %d channels) show\n", bestClusterX -> maxChannel - bestClusterX -> minChannel + 1);
      fprintf(outputFile, "50 80 moveto (Length: %.1f ns) show\n", bestClusterX -> maxTime - bestClusterX -> minTime);
    }
    fprintf(outputFile, "\n");
    
    ////////////////////////////////////////////////////////
    // Draw best cluster info for Th layer
    ////////////////////////////////////////////////////////
    
    fprintf(outputFile, "%% Cluster Info Th\nnewpath 97 100 moveto 93 0 rlineto 0 -24 rlineto -93 0 rlineto closepath stroke\n");
    fprintf(outputFile, "textl\n100 94 moveto (Best Cluster Th Info) show\ntextS\n");
    fprintf(outputFile, "100 88 moveto (Multiplicity: %d) show\n", bestClusterTh -> multiplicity);
    if (bestClusterTh -> multiplicity > 0)
    {
      fprintf(outputFile, "100 84 moveto (Central Position: %.1f mm) show\n", 0.5 * (bestClusterTh -> minChannel + bestClusterTh -> maxChannel) * 0.83);
      fprintf(outputFile, "100 80 moveto (Leading Time: %.1f ns) show\n", bestClusterTh -> minTime);
      fprintf(outputFile, "147 88 moveto (Leading Channel: %d) show\n", bestClusterTh -> leadingChannel);
      fprintf(outputFile, "147 84 moveto (Width: %d channels) show\n", bestClusterTh -> maxChannel - bestClusterTh -> minChannel + 1);
      fprintf(outputFile, "147 80 moveto (Length: %.1f ns) show\n", bestClusterTh -> maxTime - bestClusterTh -> minTime);
    }
    fprintf(outputFile, "\n");
    
    ////////////////////////////////////////////////////////
    // Draw X closeup
    ////////////////////////////////////////////////////////
    
    fprintf(outputFile, "%% Closeup X\nnewpath 10 72 moveto 83 0 rlineto 0 -68 rlineto -83 0 rlineto closepath stroke\ntextS\n");
    // channel numbers
    fprintf(outputFile, "95 0 moveto (Channel) cshow\n");
    if (bestClusterX -> multiplicity > 0 && bestClusterX -> minChannel <= bestClusterX -> maxChannel) // to be save
    {
      if (bestClusterX -> maxChannel - bestClusterX -> minChannel > 100)
      {
        for (int i = bestClusterX -> minChannel - 1; i <= bestClusterX -> maxChannel + 0; i++)
        {
          if (i % 20 == 0)
          {
            fprintf(outputFile, "newpath %.1f 4 moveto 0 2 rlineto stroke\n", 10 + (83.0 * (i - bestClusterX -> minChannel + 1)) / (bestClusterX -> maxChannel - bestClusterX -> minChannel + 2));
            fprintf(outputFile, "%.1f 0 moveto (%d) cshow\n", 10 + (83.0 * (i - bestClusterX -> minChannel + 1)) / (bestClusterX -> maxChannel - bestClusterX -> minChannel + 2), i);
          }
          else
          {
            fprintf(outputFile, "newpath %.1f 4 moveto 0 1 rlineto stroke\n", 10 + (83.0 * (i - bestClusterX -> minChannel + 1)) / (bestClusterX -> maxChannel - bestClusterX -> minChannel + 2));
          }
        }
      }
      else
      {
        if (bestClusterX -> maxChannel - bestClusterX -> minChannel > 10)
        {
          for (int i = bestClusterX -> minChannel - 1; i <= bestClusterX -> maxChannel + 0; i++)
          {
            if (i % 5 == 0)
            {
              fprintf(outputFile, "newpath %.1f 4 moveto 0 2 rlineto stroke\n", 10 + (83.0 * (i - bestClusterX -> minChannel + 1)) / (bestClusterX -> maxChannel - bestClusterX -> minChannel + 2));
              fprintf(outputFile, "%.1f 0 moveto (%d) cshow\n", 10 + (83.0 * (i - bestClusterX -> minChannel + 1)) / (bestClusterX -> maxChannel - bestClusterX -> minChannel + 2), i);
            }
            else
            {
              fprintf(outputFile, "newpath %.1f 4 moveto 0 1 rlineto stroke\n", 10 + (83.0 * (i - bestClusterX -> minChannel + 1)) / (bestClusterX -> maxChannel - bestClusterX -> minChannel + 2));
            }
          }
        }
        else
        {
//           if (bestClusterX -> maxChannel - bestClusterX -> minChannel > 1)
//           {
            for (int i = bestClusterX -> minChannel - 1; i <= bestClusterX -> maxChannel + 0; i++)
            {
              fprintf(outputFile, "newpath %.1f 4 moveto 0 2 rlineto stroke\n", 10 + (83.0 * (i - bestClusterX -> minChannel + 1)) / (bestClusterX -> maxChannel - bestClusterX -> minChannel + 2));
              fprintf(outputFile, "%.1f 0 moveto (%d) cshow\n", 10 + (83.0 * (i - bestClusterX -> minChannel + 1)) / (bestClusterX -> maxChannel - bestClusterX -> minChannel + 2), i);
            }
//           }
        }
      }     
    }
    
    // time coordinates
    fprintf(outputFile, "0 71 moveto (Time) show\n0 68 moveto ([ns]) show\n");
    if (bestClusterX -> multiplicity > 0 && bestClusterX -> maxTime - bestClusterX -> minTime > 0)
    {
      if (bestClusterX -> maxTime - bestClusterX -> minTime > 10)
      {
        for (int i = (int) bestClusterX -> minTime - 1; i <= (int) bestClusterX -> maxTime + 1; i++)
        {
          if (i % 5 == 0)
          {
            fprintf(outputFile, "newpath 10 %.1f moveto 2 0 rlineto stroke\n", 4 + 68.0 * (i - (int) bestClusterX -> minTime + 2) / ((int) bestClusterX -> maxTime - (int) bestClusterX -> minTime + 4));
            fprintf(outputFile, "0 %.1f moveto (%d) show\n", 3.5 + (68.0 * (i - (int) bestClusterX -> minTime + 2)) / ((int) bestClusterX -> maxTime - (int) bestClusterX -> minTime + 4), i);
          }
          else
          {
            fprintf(outputFile, "newpath 10 %.1f moveto 1 0 rlineto stroke\n", 4 + 68.0 * (i - (int) bestClusterX -> minTime + 2) / ((int) bestClusterX -> maxTime - (int) bestClusterX -> minTime + 4));
          }
        }
      }
      else
      {
        for (int i = (int) bestClusterX -> minTime - 2; i <= (int) bestClusterX -> maxTime + 1; i++)
        {
          fprintf(outputFile, "newpath 10 %.1f moveto 2 0 rlineto stroke\n", 4 + 68.0 * (i - (int) bestClusterX -> minTime + 2) / ((int) bestClusterX -> maxTime - (int) bestClusterX -> minTime + 4));
          fprintf(outputFile, "0 %.1f moveto (%d) show\n", 3.5 + (68.0 * (i - (int) bestClusterX -> minTime + 2)) / ((int) bestClusterX -> maxTime - (int) bestClusterX -> minTime + 4), i);
        }
      }
    }
    // Draw signals
    fprintf(outputFile, "gsave\nnewpath 10 4 moveto 83 0 rlineto 0 68 rlineto -83 0 rlineto closepath clip\n");
    for (it = hitListX -> begin(); it != hitListX -> end(); it++)
    {
      if (it->type > 0 && it -> type < 4)
      {
        fprintf(outputFile, "%.2f %.2f cross%d\n", 10 + (83.0 * (it -> channel - bestClusterX -> minChannel + 1)) / (bestClusterX -> maxChannel - bestClusterX -> minChannel + 2),
          4 + 68.0 * (it -> time - (int) bestClusterX -> minTime + 2) / ((int) bestClusterX -> maxTime - (int) bestClusterX -> minTime + 4),
          it -> type);
      }
    }
    fprintf(outputFile, "grestore\n\n");
    
    ////////////////////////////////////////////////////////
    // Draw Th closeup
    ////////////////////////////////////////////////////////
    
    fprintf(outputFile, "%% Closeup Th\nnewpath 97 72 moveto 83 0 rlineto 0 -68 rlineto -83 0 rlineto closepath stroke\ntextS\n");
    // channel numbers
    if (bestClusterTh -> multiplicity > 0 && bestClusterTh -> minChannel <= bestClusterTh -> maxChannel) // to be save TODO use multiplicity
    {
      if (bestClusterTh -> maxChannel - bestClusterTh -> minChannel > 100)
      {
        for (int i = bestClusterTh -> minChannel - 0; i <= bestClusterTh -> maxChannel + 1; i++)
        {
          if (i % 20 == 0)
          {
            fprintf(outputFile, "newpath %.1f 4 moveto 0 2 rlineto stroke\n", 97 + (83.0 * (i - bestClusterTh -> minChannel + 1)) / (bestClusterTh -> maxChannel - bestClusterTh -> minChannel + 2));
            fprintf(outputFile, "%.1f 0 moveto (%d) cshow\n", 97 + (83.0 * (i - bestClusterTh -> minChannel + 1)) / (bestClusterTh -> maxChannel - bestClusterTh -> minChannel + 2), i);
          }
          else
          {
            fprintf(outputFile, "newpath %.1f 4 moveto 0 1 rlineto stroke\n", 97 + (83.0 * (i - bestClusterTh -> minChannel + 1)) / (bestClusterTh -> maxChannel - bestClusterTh -> minChannel + 2));
          }
        }
      }
      else
      {
        if (bestClusterTh -> maxChannel - bestClusterTh -> minChannel > 10)
        {
          for (int i = bestClusterTh -> minChannel - 0; i <= bestClusterTh -> maxChannel + 1; i++)
          {
            if (i % 5 == 0)
            {
              fprintf(outputFile, "newpath %.1f 4 moveto 0 2 rlineto stroke\n", 97 + (83.0 * (i - bestClusterTh -> minChannel + 1)) / (bestClusterTh -> maxChannel - bestClusterTh -> minChannel + 2));
              fprintf(outputFile, "%.1f 0 moveto (%d) cshow\n", 97 + (83.0 * (i - bestClusterTh -> minChannel + 1)) / (bestClusterTh -> maxChannel - bestClusterTh -> minChannel + 2), i);
            }
            else
            {
              fprintf(outputFile, "newpath %.1f 4 moveto 0 1 rlineto stroke\n", 97 + (83.0 * (i - bestClusterTh -> minChannel + 1)) / (bestClusterTh -> maxChannel - bestClusterTh -> minChannel + 2));
            }
          }
        }
        else
        {
          if (bestClusterTh -> maxChannel - bestClusterTh -> minChannel > 1)
          {
            for (int i = bestClusterTh -> minChannel - 0; i <= bestClusterTh -> maxChannel + 1; i++)
            {
              if (i % 1 == 0)
              {
                fprintf(outputFile, "newpath %.1f 4 moveto 0 2 rlineto stroke\n", 97 + (83.0 * (i - bestClusterTh -> minChannel + 1)) / (bestClusterTh -> maxChannel - bestClusterTh -> minChannel + 2));
                fprintf(outputFile, "%.1f 0 moveto (%d) cshow\n", 97 + (83.0 * (i - bestClusterTh -> minChannel + 1)) / (bestClusterTh -> maxChannel - bestClusterTh -> minChannel + 2), i);
              }
              else
              {
                fprintf(outputFile, "newpath %.1f 4 moveto 0 1 rlineto stroke\n", 97 + (83.0 * (i - bestClusterTh -> minChannel + 1)) / (bestClusterTh -> maxChannel - bestClusterTh -> minChannel + 2));
              }
            }
          }
        }
      }     
    }
    // time coordinates
    
    fprintf(outputFile, "182 71 moveto (Time) show\n182 68 moveto ([ns]) show\n");
    
    if (bestClusterTh -> multiplicity > 0 && bestClusterTh -> maxTime - bestClusterTh -> minTime > 0)
    {
      if (bestClusterTh -> maxTime - bestClusterTh -> minTime > 10)
      {
        for (int i = (int) bestClusterTh -> minTime - 1; i <= (int) bestClusterTh -> maxTime + 1; i++)
        {
          if (i % 5 == 0)
          {
            fprintf(outputFile, "newpath 180 %.1f moveto -2 0 rlineto stroke\n", 4 + 68.0 * (i - (int) bestClusterTh -> minTime + 2) / ((int) bestClusterTh -> maxTime - (int) bestClusterTh -> minTime + 4));
            fprintf(outputFile, "182 %.1f moveto (%d) show\n", 3.5 + (68.0 * (i - (int) bestClusterTh -> minTime + 2)) / ((int) bestClusterTh -> maxTime - (int) bestClusterTh -> minTime + 4), i);
          }
          else
          {
            fprintf(outputFile, "newpath 180 %.1f moveto -1 0 rlineto stroke\n", 4 + 68.0 * (i - (int) bestClusterTh -> minTime + 2) / ((int) bestClusterTh -> maxTime - (int) bestClusterTh -> minTime + 4));
          }
        }
      }
      else
      {
        for (int i = (int) bestClusterTh -> minTime - 2; i <= (int) bestClusterTh -> maxTime + 1; i++)
        {
          fprintf(outputFile, "newpath 180 %.1f moveto -2 0 rlineto stroke\n", 4 + 68.0 * (i - (int) bestClusterTh -> minTime + 2) / ((int) bestClusterTh -> maxTime - (int) bestClusterTh -> minTime + 4));
          fprintf(outputFile, "182 %.1f moveto (%d) show\n", 3.5 + (68.0 * (i - (int) bestClusterTh -> minTime + 2)) / ((int) bestClusterTh -> maxTime - (int) bestClusterTh -> minTime + 4), i);
        }
      }
    }
    // Draw signals
    fprintf(outputFile, "gsave\nnewpath 97 4 moveto 83 0 rlineto 0 68 rlineto -83 0 rlineto closepath clip\n");
    for (it = hitListTh -> begin(); it != hitListTh -> end(); it++)
    {
      if (it->type > 0 && it -> type < 4)
      {
        fprintf(outputFile, "%.2f %.2f cross%d\n", 97 + (83.0 * (it -> channel - bestClusterTh -> minChannel + 1)) / (bestClusterTh -> maxChannel - bestClusterTh -> minChannel + 2),
                4 + 68.0 * (it -> time - (int) bestClusterTh -> minTime + 2) / ((int) bestClusterTh -> maxTime - (int) bestClusterTh -> minTime + 4),
                            it -> type);
      }
    }
    fprintf(outputFile, "grestore\n");
    fprintf(outputFile, "showpage\n\n");
    
    pageNumber += 1;
    pageUsed = false;
    hitListX->clear();
    hitListTh->clear();
    trackList->clear();
    clusterListX->clear();
    clusterListTh->clear();
    bestClusterX -> multiplicity = 0;
    bestClusterX -> leadingChannel = 0;
    bestClusterX -> minChannel = 0;
    bestClusterX -> maxChannel = 0;
    bestClusterX -> minTime = 0;
    bestClusterX -> maxTime = 0;
    bestClusterTh -> multiplicity = 0;
    bestClusterTh -> leadingChannel = 0;
    bestClusterTh -> minChannel = 0;
    bestClusterTh -> maxChannel = 0;
    bestClusterTh -> minTime = 0;
    bestClusterTh -> maxTime = 0;
    typeNumberX[0] = 0;
    typeNumberX[1] = 0;
    typeNumberX[2] = 0;
    typeNumberX[3] = 0;
    typeNumberTh[0] = 0;
    typeNumberTh[1] = 0;
    typeNumberTh[2] = 0;
    typeNumberTh[3] = 0;
    if (pageNumber > pages)
    {
      closeFile();
    }
  }
}	

void FibreEventDisplay::addHit(int channelNumber, double channelTime, int plane, int type)
{
  if (fileOpen)
  {
    hit buffer;
    buffer.channel = channelNumber;
    buffer.time = channelTime;
    if (0 < type && 5 > type)
    {
      buffer.type = type;
    } else {
      buffer.type = 0;
    }
    if (plane == 0)
    {
      hitListX->push_back(buffer);
      typeNumberX[buffer.type] += 1;
    } else {
      hitListTh->push_back(buffer);
      typeNumberTh[buffer.type] += 1;
    }
    pageUsed = true;
  }
}

void FibreEventDisplay::addTrack(int channelX, double timeX, int channelTh, double timeTh, double quality)
{
  if (fileOpen)
  {
    track buffer;
    buffer.channelX = channelX;
    buffer.channelTh = channelTh;
    buffer.timeX = timeX;
    buffer.timeTh = timeTh;
    buffer.quality = quality;
    trackList->push_back(buffer);
  }
}

void FibreEventDisplay::addCluster(int plane, int minChannel, int maxChannel, double minTime, double maxTime)
{
  clusters buffer;
  buffer.multiplicity = 0;
  buffer.leadingChannel = 0;
  buffer.minChannel = minChannel;
  buffer.maxChannel = maxChannel;
  buffer.minTime = minTime;
  buffer.maxTime = maxTime;
  if (plane == 0) clusterListX -> push_back(buffer);
  if (plane == 1) clusterListTh -> push_back(buffer);
}

void FibreEventDisplay::clusterInfoX(int multi, int leadingChannel, int minChannel, int maxChannel, double minTime, double maxTime)
{
  bestClusterX -> multiplicity = multi;
  bestClusterX -> leadingChannel = leadingChannel;
  bestClusterX -> minChannel = minChannel;
  bestClusterX -> maxChannel = maxChannel;
  bestClusterX -> minTime = minTime;
  bestClusterX -> maxTime = maxTime;
}

void FibreEventDisplay::clusterInfoTh(int multi, int leadingChannel, int minChannel, int maxChannel, double minTime, double maxTime)
{
  bestClusterTh -> multiplicity = multi;
  bestClusterTh -> leadingChannel = leadingChannel;
  bestClusterTh -> minChannel = minChannel;
  bestClusterTh -> maxChannel = maxChannel;
  bestClusterTh -> minTime = minTime;
  bestClusterTh -> maxTime = maxTime;
}

void FibreEventDisplay::closeFile()
{
  if (fileOpen)
  {
    fprintf(outputFile, "%%%%Trailer\n%%%%EOF\n");
    fclose(outputFile);
    fileOpen = false;
  }
}
