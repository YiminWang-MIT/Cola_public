//								      -*-c++-*-
// COLA: Analysis and simulation for A1 experiments
//
// Copyright (c) 2004
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
// $Id: FibreDetector.cc 2502 2013-02-06 10:29:18Z patrick $
//

// complete set of online spectra for the fibre detector?
#define ONLINE_SPECTRA_FIBRE

// use leading and trailing edge of signals
#undef LEADING_TRAILING_ANALYSIS

#include "Aqua/AquaTree.h"
#include "FibreDetector.h"
#include "FibreEventDisplay.h"
#include <cmath>
#include <iostream>

using namespace std;

FibreDetector::FibreDetector(AquaTree *aquatree, AquaTree *outtree)
  : detectorBase(aquatree, outtree)
{
  ///////////////////////////// DEBUG FLAG /////////////////////////////
  // 0 : no output
  // 1 : information about initialisation
  // 2 : information about every event
  // 3 : detailed information about every event
  //////////////////////////////////////////////////////////////////////

  fibre_debug = 0;  // DEBUGGING OUTPUT {0|1|2|3}
  
  if (fibre_debug == 1)
  {  
    cout << " ->  FibreDetector::FibreDetector" << endl;
  }
  
  // RAW data

  // X plane
  AquaNode **usnx = (AquaNode **)xtime;
  atree -> multifind(usnx, 0, MAXHITS-1, "abcn.kaos.fibre.x.hit[%d].time");
  AquaNode **susnx = (AquaNode **)xchn;
  atree -> multifind(susnx, 0, MAXHITS-1,"abcn.kaos.fibre.x.hit[%d].channel");
	
  // theta plane
  AquaNode **usnth = (AquaNode **)thtime;
  atree -> multifind(usnth, 0, MAXHITS-1, "abcn.kaos.fibre.th.hit[%d].time");
  AquaNode **susnth = (AquaNode **)thchn;
  atree -> multifind(susnth, 0, MAXHITS-1,"abcn.kaos.fibre.th.hit[%d].channel");
  
  // reconstructed data

  x_leadingChannel =        (AquaNodeInt *)online -> find("online.kaos.scifi.x.hitLeadingChannel");
  x_leadingTime =           (AquaNodeDouble *)online -> find("online.kaos.scifi.x.hitLeadingTime");
  x_trailingTime =          (AquaNodeDouble *)online -> find("online.kaos.scifi.x.hitTrailingTime");
  x_minChannel =            (AquaNodeInt *)online -> find("online.kaos.scifi.x.hitMinChannel");
  x_maxChannel =            (AquaNodeInt *)online -> find("online.kaos.scifi.x.hitMaxChannel");
  x_hitMultiplicity =       (AquaNodeInt *)online -> find("online.kaos.scifi.x.hitMultiplicity");
  
  x_totalMultiplicity =     (AquaNodeInt *)online -> find("online.kaos.scifi.x.Multiplicity");
  x_numberClusters =        (AquaNodeInt *)online -> find("online.kaos.scifi.x.numberClusters");
  x_typeCluster =           (AquaNodeInt *)online -> find("online.kaos.scifi.x.typeCluster");
  x_typeAfterpulse =        (AquaNodeInt *)online -> find("online.kaos.scifi.x.typeAfterpulse");
  x_typeSingles =           (AquaNodeInt *)online -> find("online.kaos.scifi.x.typeSingle");

  x_channels =              (AquaNodeInt *)online -> find("online.kaos.scifi.x.channels");
  x_times =                 (AquaNodeDouble *)online -> find("online.kaos.scifi.x.times");

  x_timeMean=               (AquaNodeDouble *)online -> find("online.kaos.scifi.x.timeMean");
  x_timeRMS =               (AquaNodeDouble *)online -> find("online.kaos.scifi.x.timeRMS");
  x_lengthRMS =             (AquaNodeDouble *)online -> find("online.kaos.scifi.x.lengthRMS");
 
  theta_leadingChannel =    (AquaNodeInt *)online -> find("online.kaos.scifi.theta.hitLeadingChannel");
  theta_leadingTime =       (AquaNodeDouble *)online -> find("online.kaos.scifi.theta.hitLeadingTime");
  theta_trailingTime =      (AquaNodeDouble *)online -> find("online.kaos.scifi.theta.hitTrailingTime");
  theta_minChannel =        (AquaNodeInt *)online -> find("online.kaos.scifi.theta.hitMinChannel");
  theta_maxChannel =        (AquaNodeInt *)online -> find("online.kaos.scifi.theta.hitMaxChannel");
  theta_hitMultiplicity =   (AquaNodeInt *)online -> find("online.kaos.scifi.theta.hitMultiplicity");
  
  theta_totalMultiplicity = (AquaNodeInt *)online -> find("online.kaos.scifi.theta.Multiplicity");
  theta_numberClusters =    (AquaNodeInt *)online -> find("online.kaos.scifi.theta.numberClusters");
  theta_typeCluster =       (AquaNodeInt *)online -> find("online.kaos.scifi.theta.typeCluster");
  theta_typeAfterpulse =    (AquaNodeInt *)online -> find("online.kaos.scifi.theta.typeAfterpulse");
  theta_typeSingles =       (AquaNodeInt *)online -> find("online.kaos.scifi.theta.typeSingle");
  
  theta_channels =          (AquaNodeInt *)online -> find("online.kaos.scifi.theta.channels");
  theta_times =             (AquaNodeDouble *)online -> find("online.kaos.scifi.theta.times");
  theta_timeRMS =           (AquaNodeDouble *)online -> find("online.kaos.scifi.theta.timeRMS");
  theta_timeMean =          (AquaNodeDouble *)online -> find("online.kaos.scifi.theta.timeMean");
  theta_lengthRMS =         (AquaNodeDouble *)online -> find("online.kaos.scifi.theta.lengthRMS");

  track_number_tracks =     (AquaNodeInt *)online -> find("online.kaos.scifi.numberTracks");
  track_angle =             (AquaNodeDouble *)online -> find("online.kaos.scifi.angle");
  track_flightpath =        (AquaNodeDouble *)online -> find("online.kaos.scifi.flightpath");
  track_quality =           (AquaNodeDouble *)online -> find("online.kaos.scifi.quality");

  // online spectra

  AquaNode **x = (AquaNode **)x_TDC;
  online -> multifind(x, 0, XCHNS-1, "online.kaos.scifi.x.time[%d]");
	
  AquaNode **th = (AquaNode **)th_TDC;
  online -> multifind(th, 0, THCHNS-1, "online.kaos.scifi.theta.time[%d]");
	
  AquaNode **extn = (AquaNode **)ext_TDC;
  online -> multifind(extn, 0, EXTCHNS-1, "online.kaos.scifi.ext.time[%d]");
	
  char title[64];
  sprintf(title, "KAOS/FiX/Nr of Fibre X");
  FibreNrX = HMFind(title);
  sprintf(title, "KAOS/FiX/Time of Fibre X");
  FibreTimeX = HMFind(title);
  sprintf(title, "KAOS/FiX/Threshold Channel");
  ThresholdChannelX = HMFind(title);

  sprintf(title, "KAOS/FiTracks/FiX/Nr of Fibre X");
  TrackFibreNrX = HMFind(title);
  sprintf(title, "KAOS/FiTracks/FiX/Time of Fibre X");
  TrackFibreTimeX = HMFind(title);
  sprintf(title, "KAOS/FiTracks/FiX/Threshold Channel");
  TrackThresholdChannelX = HMFind(title);
 	
  sprintf(title, "KAOS/FiTheta/Nr of Fibre Theta");
  FibreNrTheta = HMFind(title);
  sprintf(title, "KAOS/FiTheta/Time of Fibre Theta");
  FibreTimeTheta = HMFind(title);
  sprintf(title, "KAOS/FiTheta/Threshold Channel");
  ThresholdChannelTheta = HMFind(title);

  sprintf(title, "KAOS/FiTracks/FiTheta/Nr of Fibre Theta");
  TrackFibreNrTheta = HMFind(title);
  sprintf(title, "KAOS/FiTracks/FiTheta/Time of Fibre Theta");
  TrackFibreTimeTheta = HMFind(title);
  sprintf(title, "KAOS/FiTracks/FiTheta/Threshold Channel");
  TrackThresholdChannelTheta = HMFind(title);
}

FibreDetector::~FibreDetector()
{
  if (EventDisplay != NULL)
    delete EventDisplay;
}

int FibreDetector::pmt2thresholdChannel(int pmtCh)
{
  return(2 * (pmtCh / 8) + (pmtCh % 2));
}

int FibreDetector::handle()
{
  int N_x = 0;   // number of signals in x plane
  int N_th = 0;  // number of in signal theta plane
  int N_Tr = 0;  // number of trigger signals
  int N_ext = 0; // number of external signals

  list<hit> *hitListX = new list<hit>;
  list<hit> *hitListTh = new list<hit>;
  hit buffer, element;

  int    TriggerValue = 0;

  //  std::cout << "handling FibreDetector" << std::endl; 
    
  // finding trigger time (assuming the trigger channel is in the x plane)
  { // scope for int k
    int k = 0;
    while ( (N_Tr < 1) && k < MAXHITS)
      {
	//      std::cout << "looking for trigger time" << std::endl; 

	if (xtime[k] -> ok() && xchn[k] -> ok() && (*xchn[k] == TriggerChannel))
	  {
	    //	std::cout << "trigger time found in x" << std::endl; 
	    
	    TriggerValue = *xtime[k];
	    N_Tr++;
	  }
	k++;
      }
  }
  
  if (N_Tr < 1)
    { // no trigger reference found
      //    std::cout << "notrigger" << std::endl;
      return 0;
    }  
  //cout << "triggered"<< endl;
  
  /////////////////////////////////////////////////////////////////////////////
  // x-plane
  /////////////////////////////////////////////////////////////////////////////
  hitListX -> clear();
  for (int k = 0; k < MAXHITS; k++)
    {
      if (xtime[k] -> ok() && xchn[k] -> ok() && (*xchn[k] != TriggerChannel) )
	{   
	  // time difference in TDC clock ticks
	  double time_diff = *xtime[k] - TriggerValue; 
	  // in this case the TDC clock rolled back
	  if (time_diff > TDC_ROLL_LIMIT) time_diff -= TDC_ROLL; 

	  buffer.channel = *xchn[k];
	  buffer.time = time_diff * TDC_RESOLUTION - TriggerOffset;

	  if (fibre_debug == 2) cout << "TDC ->  " 
				     << buffer.channel
				     << " : " << buffer.time
				     << " ns " << endl;

	  if (*xchn[k] < XCHNS)
	    { 
	      
	      // TDC offsets from run.db
	      buffer.time += TDC_offset[buffer.channel];

	      buffer.type = 0; // hit in buffer is leading edge unless trailing edge is detected
	      buffer.trailingtime = buffer.time; // default unless trailing edge is detected
	      
	      // checking for trailing edges
	      if (hitListX -> size())
		{
		  element = hitListX -> back();
		  if ( element.channel == buffer.channel && 
		       abs(buffer.time - element.time) < MAX_PULSELENGTH )
		    {
		      if (buffer.time < element.time)
			{ 
			  buffer.trailingtime = element.time; 
			}
		      else
			{
			  hitListX -> back().trailingtime = buffer.time;
			  buffer.type = 99; // hit in buffer is trailing edge
			}
		    } // if ( element.channel ...
		} // if (hitListX -> size())
	      
	      if (buffer.type == 0) 
		{
		  hitListX -> push_back(buffer);
		  N_x++;
		  
		  // FILL RAW ONLINE HISTOGRAMS
		  if (FibreNrX) HMFill(FibreNrX, 
				       buffer.channel, 0, 1);
		  if (FibreTimeX) HMFill(FibreTimeX, 
					 buffer.channel, buffer.time, 1);
		  if (ThresholdChannelX) 
		    HMFill(ThresholdChannelX,
			   pmt2thresholdChannel(buffer.channel), 0, 1);
		} // buffer.type
	    } // if (*xchn[k] < XCHNS) ...
	  else if (*xchn[k] == HadronChannel)
	    {
	      if (fibre_debug == 2) cout << "Hadron chn  ->  " << *xchn[k] 
					 << " : " << time_diff 
					 << " (raw) cnts " << endl;
	      ext_TDC[0]->handle(time_diff);
	      N_ext++;
	    }
	  else if (*xchn[k] == ElectronChannel)
	    {
	      if (fibre_debug == 2) cout << "Electron chn  ->  " << *xchn[k] 
					 << " : " << time_diff 
					 << " (raw) cnts " << endl;
	      ext_TDC[1]->handle(time_diff);
	      N_ext++;
	    }
	} // if (xtime[k] -> ok() ...
    } // for (int k = 0; ...

  /////////////////////////////////////////////////////////////////////////////
  // theta-plane
  /////////////////////////////////////////////////////////////////////////////
  if (fibre_debug == 2) cout << endl << "-------------------" << endl 
			     << "--- <TDC hits> ---" << endl 
			     << "---<Theta Plane>---" << endl;
  hitListTh -> clear();

  for (int k = 0; k < MAXHITS; k++)
    {
      if (thtime[k] -> ok() && thchn[k] -> ok() && *thchn[k] != 2240)
	{
	  // time difference in TDC clock ticks
	  double time_diff = *thtime[k] - TriggerValue; 
	  // in this case the TDC clock rolled back
	  if (time_diff > TDC_ROLL_LIMIT) time_diff -= TDC_ROLL; 
	  
	  hit buffer;
	  buffer.channel = THCHNS - (*thchn[k] + 1);
	  buffer.time = time_diff * TDC_RESOLUTION - TriggerOffset; 
	  
	  if (fibre_debug == 2) cout << "TDC ->  " 
				     << buffer.channel
				     << " : " << buffer.time 
				     << " ns " << endl;
	  
	  if (*thchn[k] < THCHNS)
	    {
	      // TDC offsets from run.db
	      buffer.time += TDC_offset[buffer.channel + XCHNS];
	      
	      buffer.type = 0; // hit in buffer is leading edge unless trailing edge is detected
	      buffer.trailingtime = buffer.time; // default unless trailing edge is detected

	      // checking for trailing edges
	      if (hitListTh -> size())
		{
		  element = hitListTh -> back();
		  if ( element.channel == buffer.channel && 
		       abs(buffer.time - element.time) < MAX_PULSELENGTH )
		    {
		      if (buffer.time < element.time)
			{ buffer.trailingtime = element.time; }
		      else
			{
			  hitListTh -> back().trailingtime = buffer.time;
			  buffer.type = 99; // hit in buffer is trailing edge
			}
		    } // if ( element.channel ...
		} // if (hitListTh -> size())
	      
	      if (buffer.type == 0) 
		{
		  hitListTh -> push_back(buffer);
		  N_th++;
		  
		  // FILL RAW ONLINE HISTOGRAMS
		  if (FibreNrTheta) HMFill(FibreNrTheta, 
					   buffer.channel, 0, 1);
		  if (FibreTimeTheta) 
		    HMFill(FibreTimeTheta, buffer.channel, buffer.time, 1);
		  if (ThresholdChannelTheta) 
		    HMFill(ThresholdChannelTheta,
			   pmt2thresholdChannel(buffer.channel), 0, 1);
		} // buffer.type
	    } // *thchn[k] < THCHNS
	} // thtime[k] -> ok() ...
    } // (int k = 0; ...
  

  if (fibre_debug == 2) cout << "-------------------" << endl;
  if (fibre_debug == 2) cout << "x  ->  N:  "   << N_x << endl 
			     << "-------------------" << endl;
  if (fibre_debug == 2) cout << "th ->  N:  "   << N_th << endl 
			     << "-------------------" << endl;

  // cluster analysis
  if ((N_x != 0) || (N_th != 0)) clusterFind(hitListX, hitListTh);

  delete hitListX;
  delete hitListTh;

// #ifdef DRAW_FIBRE_EVENT_DISPLAY
  if (EventDisplay != NULL)
    EventDisplay -> drawPage();
// #endif

  return 0;
}

void FibreDetector::clusterFind(list<hit> * hitListX, list<hit> * hitListTh)
{
  // handle events in X plane
  vector<hit> hitsX;   
  // will contain all hits sorted by clusters
  list<cluster> *clustersX = new list<cluster>;  
  // will contain all information about clusters in list above

  hitsX.clear();   // just to be save
  clustersX -> clear();

  list<hit>::iterator it_hitListX;   // iterator in hitListX
  it_hitListX = hitListX -> begin();

  list<cluster>::iterator itX;       // iterator in clustersX
  itX = clustersX -> begin();

  // clustering algorithm
  unsigned int index = 0;
  while (!hitListX -> empty())
  {
    cluster buffer;                   // create a new cluster
    buffer.firstElement = hitsX.size();
    buffer.lastElement = 0;
    buffer.multiplicity = 0;
    buffer.leadingChannel = 0;
    buffer.centerTime = 0;
    buffer.center = 0;
    buffer.quality = 0;
    buffer.RMSlength = 0;
    buffer.width = 0;
    buffer.leadingTime = 0;
    buffer.leadingTimeChannel = 0;
    buffer.lowestChannel = 0;
    buffer.heighestChannel = 0;
    buffer.endTime = 0;
    buffer.RMSTime = 0;
    buffer.meanTime = 0;
    clustersX -> push_back(buffer);

    hitsX.push_back(hitListX -> front()); 
    // copy first Element from hitListX to hitsX ...
    hitListX -> pop_front();
    // ... and delete

    while (index < hitsX.size())
      {
	if (!hitListX -> empty())
	  {
	    it_hitListX = hitListX -> begin();
	    while(it_hitListX != hitListX -> end())
	      {
		if ( (abs(hitsX[index].channel - it_hitListX -> channel) 
		      < clusterSeparationChannels) &&
		     (abs(hitsX[index].time - it_hitListX -> time) 
		      < clusterSeparationTime) )
		  {
		    // the chosen element in hitListX 
		    // is near the chosen element in hitsX
		    hitsX.push_back(*it_hitListX); 
		    // copy first element from hitListX ho hitsX ...
		    it_hitListX = hitListX -> erase(it_hitListX); 
		    // ... and delete
		  }
		else
		  {
		    it_hitListX++;
		  }
	      }
	  } // end of if block
	index++;
      } // end of while block

    itX = clustersX -> end();
    itX--;
    itX -> lastElement = hitsX.size() - 1;
    itX -> multiplicity = itX -> lastElement - itX -> firstElement + 1;
  }

  // find borders of clustersX
  itX = clustersX -> begin();
  while(itX != clustersX -> end())
  {
    double minTime = 100000;
    double maxTime = -1000000;
    double RMSTime = 0, RMSLength = 0;
    double meanTime = 0, meanLength = 0;
    int minCh = 4000;
    int maxCh = -3;
    int Ndouble = 0;

    for (index = itX -> firstElement; 
	 index <= (unsigned int) itX -> lastElement; index++)
    {

      meanTime += hitsX[index].time;
      RMSTime  += pow( hitsX[index].time, 2);

      if (hitsX[index].trailingtime != hitsX[index].time)
	{
	  //  double pulselength = hitsX[index].trailingtime - hitsX[index].time;
	  //  meanLength += pulselength;
	  //  RMSLength  += pow( pulselength, 2);
	  RMSLength += pow( (hitsX[index].trailingtime), 2);
	  meanLength += (hitsX[index].trailingtime);
	  Ndouble ++;
	}

      if (minTime > hitsX[index].time)
	{
	  minTime = hitsX[index].time;
	  itX -> leadingTimeChannel = hitsX[index].channel;
	  itX -> trailingTime = hitsX[index].trailingtime;
	}
      if (maxTime < hitsX[index].time) {maxTime = hitsX[index].time;}
      if (minCh > hitsX[index].channel) {minCh = hitsX[index].channel;}
      if (maxCh < hitsX[index].channel) {maxCh = hitsX[index].channel;}

    } // for (index = itX -> firstElement ...

    itX -> leadingTime = minTime;
    itX -> lowestChannel = minCh;
    itX -> heighestChannel = maxCh;
    itX -> endTime = maxTime;
    itX -> meanTime = meanTime/(itX -> multiplicity);

    if (itX -> multiplicity > 1) 
      {
	itX -> RMSTime = sqrt( RMSTime/(itX -> multiplicity) 
			       - pow(itX -> meanTime,2) );
      }
    if (Ndouble > 1) 
      {
	itX -> RMSlength = sqrt( RMSLength/Ndouble 
				 - pow(meanLength/Ndouble,2) );
      }
    itX++;
  }

  // delete small clusters
  itX = clustersX -> begin();
  while (itX != clustersX -> end())
  {
    if ((itX -> multiplicity < minClusterSize)
          && (((itX -> lowestChannel % 32 != 0) && 
	       (itX -> heighestChannel % 32 != 31)) 
	      || (itX -> multiplicity < minClusterSize / 2))
       )
    {
       itX = clustersX -> erase(itX);
    }
    else
    {
      // assign hit type cluster
      for (index = itX -> firstElement; 
	   index <= (unsigned int) itX -> lastElement; index++)
      {
        hitsX[index].type = 1;
      }
      if (itX != clustersX -> end()) 
      {
        itX++;
      }
    }
  }

  // search for afterpulses
  int numberRelatedX = 0;
  int numberUnrelatedX = 0;
  for (index = 0; index < hitsX.size(); index++)
  {
    if (hitsX[index].type == 0)
    {
      itX = clustersX -> begin();
      bool related = false;
      while (itX != clustersX -> end())
      {
        if ((itX -> lowestChannel < hitsX[index].channel) && 
             (itX -> heighestChannel > hitsX[index].channel) &&
             (itX -> leadingTime < hitsX[index].time))
        {
          related = true;
        }
        itX++;
      }
      if (related)
      {
        hitsX[index].type = 2;
        numberRelatedX++;
      }
      else
      {
        hitsX[index].type = 3;
        numberUnrelatedX++;
     }
    }
  }
  
  // handle events in Theta plane
  vector<hit> hitsTh;   
  // will contain all hits sorted by clusters
  list<cluster> *clustersTh = new list<cluster>;  
  // will contain all information about clusters in list above

  hitsTh.clear();   // just to be save
  clustersTh -> clear();

  list<hit>::iterator it_hitListTh;   // iterator in hitListTh
  it_hitListTh = hitListTh -> begin();

  list<cluster>::iterator itTh;       // iterator in clustersTh
  itTh = clustersTh -> begin();

  // clustering algorithm
  index = 0;
  while (!hitListTh -> empty())
  {
    cluster buffer;                   // create a new cluster
    buffer.firstElement = hitsTh.size();
    buffer.lastElement = 0;
    buffer.multiplicity = 0;
    buffer.leadingChannel = 0;
    buffer.centerTime = 0;
    buffer.center = 0;
    buffer.quality = 0;
    buffer.RMSlength = 0;
    buffer.width = 0;
    buffer.leadingTime = 0;
    buffer.leadingTimeChannel = 0;
    buffer.lowestChannel = 0;
    buffer.heighestChannel = 0;
    buffer.endTime = 0;
    buffer.RMSTime = 0;
    buffer.meanTime = 0;
    clustersTh -> push_back(buffer);

    hitsTh.push_back(hitListTh -> front()); 
    // copy first Element from hitListTh ho hitsTh ...
    hitListTh -> pop_front();             
    // ... and delete

    while (index < hitsTh.size())
      {
	if (!hitListTh -> empty())
	  {
	    it_hitListTh = hitListTh -> begin();
	    while(it_hitListTh != hitListTh -> end())
	      {
		if ( (abs(hitsTh[index].channel - it_hitListTh -> channel) 
		      < clusterSeparationChannels ) &&
		     (abs(hitsTh[index].time - it_hitListTh -> time) 
		      < clusterSeparationTime))
		  {
		    // the chosen Element in hitListTh 
		    // is near the chosen Element in hitsTh
		    hitsTh.push_back(*it_hitListTh); 
		    // copy first Element from hitListTh ho hitsTh ...
		    it_hitListTh = hitListTh -> erase(it_hitListTh); 
		    // ... and delete
		  }
		else
		  {
		    it_hitListTh++;
		  }
	      }
	  }
	index++;
      }
    itTh = clustersTh -> end();
    itTh--;
    itTh -> lastElement = hitsTh.size() - 1;
    itTh -> multiplicity = itTh -> lastElement - itTh -> firstElement + 1;
  }
  
  // find borders of clusters
  itTh = clustersTh -> begin();
  while(itTh != clustersTh -> end())
  {
    double minTime = 100000;
    double maxTime = -1000000;
    double RMSTime = 0, RMSLength = 0;
    double meanTime = 0, meanLength = 0;
    int minCh = 4000;
    int maxCh = -3;
    int Ndouble = 0;

    for (index = itTh -> firstElement; 
	 index <= (unsigned int) itTh -> lastElement; index++)
    {
      meanTime += hitsTh[index].time;
      RMSTime += pow(hitsTh[index].time,2);

      if (hitsTh[index].trailingtime != hitsTh[index].time)
	{
	  //  double pulselength = hitsTh[index].trailingtime - hitsTh[index].time;
	  //  meanLength += pulselength;
	  //  RMSLength  += pow( pulselength, 2);
	  RMSLength += pow( (hitsTh[index].trailingtime), 2);
	  meanLength += (hitsTh[index].trailingtime);
	  Ndouble ++;
	}

      if (minTime > hitsTh[index].time)
      {
        minTime = hitsTh[index].time;
        itTh -> leadingTimeChannel = hitsTh[index].channel;
        itTh -> trailingTime = hitsTh[index].trailingtime;
      }
      if (maxTime < hitsTh[index].time) {maxTime = hitsTh[index].time;}
      if (minCh > hitsTh[index].channel) {minCh = hitsTh[index].channel;}
      if (maxCh < hitsTh[index].channel) {maxCh = hitsTh[index].channel;}
    }
    itTh -> leadingTime = minTime;
    itTh -> centerTime = minTime; 		// to be improved
    itTh -> lowestChannel = minCh;
    itTh -> heighestChannel = maxCh;
    itTh -> endTime = maxTime;
    itTh -> meanTime = meanTime/(itTh -> multiplicity);

    if (itTh -> multiplicity > 1) 
      {
	itTh -> RMSTime = sqrt( RMSTime/(itTh -> multiplicity) 
				- pow(itTh -> meanTime,2) );
      }
    if (Ndouble > 1) 
      {
	itTh -> RMSlength = sqrt( RMSLength/Ndouble 
				  - pow(meanLength/Ndouble,2) );
      }
    itTh++;
  }

  // delete small clusters
  itTh = clustersTh -> begin();
  while (itTh != clustersTh -> end())
  {
    if ((itTh -> multiplicity < minClusterSize)
          && (((itTh -> lowestChannel % 32 != 0) 
	       && (itTh -> heighestChannel % 32 != 31)) || 
	      (itTh -> multiplicity < minClusterSize / 2))
       )
    {
      itTh = clustersTh -> erase(itTh);
    }
    else
    {
      for (index = itTh -> firstElement; 
	   index <= (unsigned int) itTh -> lastElement; index++)
      {
        hitsTh[index].type = 1;
      }
      if (itTh != clustersTh -> end()) 
      {
        itTh++;
      }
    }
  }

  // search for afterpulses
  int numberRelatedTh = 0;
  int numberUnrelatedTh = 0;
  for (index = 0; index < hitsTh.size(); index++)
  {
    if (hitsTh[index].type == 0)
    {
      itTh = clustersTh -> begin();
      bool related = false;
      while (itTh != clustersTh -> end())
      {
        if ((itTh -> lowestChannel < hitsTh[index].channel) && 
             (itTh -> heighestChannel > hitsTh[index].channel) &&
             (itTh -> leadingTime < hitsTh[index].time))
        {
          related = true;
        }
        itTh++;
      }
      if (related)
      {
        hitsTh[index].type = 2;
        numberRelatedTh++;
      }
      else
      {
        hitsTh[index].type = 3;
        numberUnrelatedTh++;
      }
    }
  }

  // Find Tracks
  int numberTracks = 0;
  double bestangle = 360; 
  double bestflightPath = 0;
  double minQuality = -1000;
  double bestQuality = minQuality;

  list<cluster>::iterator bestClusterX = clustersX -> begin();
  list<cluster>::iterator bestClusterTh = clustersTh -> begin();
  
  itTh = clustersTh -> begin();
  while(itTh != clustersTh -> end())
  {
    itX = clustersX -> begin();
    while ( itX != clustersX -> end())
    {
      double quality = -1000;

      double angle = atan((THPITCH * itTh -> leadingTimeChannel 
			   - XPITCH * itX -> leadingTimeChannel + X_TH_OFFSET) 
			  / LAYER_DIST);

      double timeDifference = itTh -> leadingTime - itX -> leadingTime; // (ns)
      double timeOffset = (itTh -> leadingTime + itX -> leadingTime) / 2;

      double flightPath = LAYER_DIST / cos(angle); // (mm)
      double correctedTimeDifference = timeDifference - flightPath / LIGHTSPEED;

      angle *= degree;
      if ((angle > minAngle) && (angle < maxAngle) 
	  && (correctedTimeDifference > - clusterSeparationTime) 
	  && (correctedTimeDifference < clusterSeparationTime))
      {
	quality = - abs(timeOffset);
	numberTracks++;

        if (EventDisplay != NULL)
	  EventDisplay -> addTrack(itX -> leadingTimeChannel, 
				   itX -> leadingTime, 
				   itTh -> leadingTimeChannel, 
				   itTh -> leadingTime, quality);
      }
      
      if (quality > bestQuality) 
	{
	  bestClusterX = itX;
	  bestClusterTh = itTh;
	  bestQuality = quality;
	  bestflightPath = flightPath;
	  bestangle = angle;
	}
      itX++;
    }
    itTh++;
  }

  // FILL RAW ONLINE HISTOGRAMS
  if (clustersX -> size() > 0 && bestQuality > minQuality) {
    for (index = bestClusterX -> firstElement; 
	 index <= (unsigned int) bestClusterX -> lastElement; index++)
      {
	if (TrackFibreNrX) HMFill(TrackFibreNrX, 
				  hitsX[index].channel, 0, 1);
	if (TrackFibreTimeX) 
	  HMFill(TrackFibreTimeX, hitsX[index].channel, hitsX[index].time, 1);
	if (TrackThresholdChannelX) 
	  HMFill(TrackThresholdChannelX,
		 pmt2thresholdChannel(hitsX[index].channel), 0, 1);
      }
  }

  if (clustersTh -> size() > 0 && bestQuality > minQuality) {

    for (index = bestClusterTh -> firstElement; 
	 index <= (unsigned int) bestClusterTh -> lastElement; index++)
      {
	if (TrackFibreNrTheta) HMFill(TrackFibreNrTheta, 
				      hitsTh[index].channel, 0, 1);
	if (TrackFibreTimeTheta) 
	  HMFill(TrackFibreTimeTheta, 
		 hitsTh[index].channel, hitsTh[index].time, 1);
	if (TrackThresholdChannelTheta) 
	  HMFill(TrackThresholdChannelTheta,
		 pmt2thresholdChannel(hitsTh[index].channel), 0, 1);
      }
  }

  // event display
  if (EventDisplay != NULL)
  {
    for (index = 0; index < hitsX.size(); index++)
    {
      EventDisplay -> addHit(
        hitsX[index].channel,
        hitsX[index].time,
        0,
        hitsX[index].type);
    }
    if (clustersX -> size() > 0)
      EventDisplay -> addCluster(
        0,
        bestClusterX -> lowestChannel,
        bestClusterX -> heighestChannel,
        bestClusterX -> leadingTime,
        bestClusterX -> endTime);

    if (clustersX -> size() > 0)
    {
      EventDisplay -> clusterInfoX(
        bestClusterX -> multiplicity,
        bestClusterX -> leadingTimeChannel,
        bestClusterX -> lowestChannel,
        bestClusterX -> heighestChannel,
        bestClusterX -> leadingTime,
        bestClusterX -> endTime);
    }

    if (clustersTh -> size() > 0) 
      EventDisplay -> addCluster(
        1,
        bestClusterTh -> lowestChannel,
        bestClusterTh -> heighestChannel,
        bestClusterTh -> leadingTime,
        bestClusterTh -> endTime);
    for (index = 0; index < hitsTh.size(); index++)
    {
      EventDisplay -> addHit(
        hitsTh[index].channel,
        hitsTh[index].time,
        1,
        hitsTh[index].type);
    }
    if (clustersTh -> size() > 0)
    {
      EventDisplay -> clusterInfoTh(
        bestClusterTh -> multiplicity,
        bestClusterTh -> leadingTimeChannel,
        bestClusterTh -> lowestChannel,
        bestClusterTh -> heighestChannel,
        bestClusterTh -> leadingTime,
        bestClusterTh -> endTime);
    }
  }


  // return information
  if (clustersX -> size() > 0)
  {
    x_leadingChannel ->  handle(bestClusterX -> leadingTimeChannel);
    x_leadingTime ->     handle(bestClusterX -> leadingTime);
    x_trailingTime ->    handle(bestClusterX -> trailingTime);
    x_minChannel ->      handle(bestClusterX -> lowestChannel);
    x_maxChannel ->      handle(bestClusterX -> heighestChannel);
    x_hitMultiplicity -> handle(bestClusterX -> multiplicity);
    x_timeMean ->        handle(bestClusterX -> meanTime);
    x_timeRMS ->         handle(bestClusterX -> RMSTime); 
    x_lengthRMS ->       handle(bestClusterX -> RMSlength); 
  }

  x_totalMultiplicity -> handle(hitsX.size());
  x_numberClusters ->    handle(clustersX -> size());
  x_typeCluster ->       handle(hitsX.size() - numberRelatedX 
				- numberUnrelatedX);
  x_typeAfterpulse ->    handle(numberRelatedX);
  x_typeSingles ->       handle(numberUnrelatedX);

  if (clustersTh -> size() > 0)
  {
    theta_leadingChannel ->  handle(bestClusterTh -> leadingTimeChannel);
    theta_leadingTime ->     handle(bestClusterTh -> leadingTime);
    theta_trailingTime ->    handle(bestClusterTh -> trailingTime);
    theta_minChannel ->      handle(bestClusterTh -> lowestChannel);
    theta_maxChannel ->      handle(bestClusterTh -> heighestChannel);
    theta_hitMultiplicity -> handle(bestClusterTh -> multiplicity);
    theta_timeRMS ->         handle(bestClusterTh -> RMSTime);
    theta_timeMean ->        handle(bestClusterTh -> meanTime);
    theta_lengthRMS ->       handle(bestClusterTh -> RMSlength); 
  }

  theta_totalMultiplicity -> handle(hitsTh.size());
  theta_numberClusters ->    handle(clustersTh -> size());
  theta_typeCluster ->       handle(hitsTh.size() - numberRelatedTh 
				    - numberUnrelatedTh);
  theta_typeAfterpulse ->    handle(numberRelatedTh);
  theta_typeSingles ->       handle(numberUnrelatedTh);

  track_number_tracks ->     handle(numberTracks);
  track_angle ->             handle(bestangle);
  track_flightpath ->        handle(bestflightPath);
  track_quality ->           handle(bestQuality);
  
  delete clustersX;
  delete clustersTh;
  return;
}



int FibreDetector::init(
	double * TDC_offset_, 
	double * trigger_, 
	double * analysisParameters_,
	double * eventDisplay_)
{
  if (fibre_debug >= 1) {
    cout << " ->  FibreDetector::init" << endl;
    cout << " ->  check FIBRECHNS in Cola.cc" << endl;
    cout << " ->  check TDC fields in rundatabase.h" << endl;
  }

  // load number of trigger channel and time offset
  TriggerChannel = (int) (trigger_[0]);
  TriggerOffset  = trigger_[1];
  HadronChannel = trigger_[2];
  ElectronChannel = trigger_[3];

  if (fibre_debug >= 1) {
    cout << "Trigger channel: " << TriggerChannel << endl;
    cout << "Trigger time offset: " << TriggerOffset << " ns" << endl;
    cout << "Hadron channel: " << HadronChannel << endl;
    cout << "Electron channel: " << ElectronChannel << endl;
  }

  // load analysis parameters
  minClusterSize            = (int) (analysisParameters_[0]);
  clusterSeparationChannels = (int) (analysisParameters_[1]);
  clusterSeparationTime     = analysisParameters_[2];
  minAngle                  = analysisParameters_[3];
  maxAngle                  = analysisParameters_[4];

  if (fibre_debug >= 1) {
    cout << "Analysis parameters" << endl;
    cout << "Minimum cluster size: " << minClusterSize << endl;
    cout << "Cluster separation channels: " << clusterSeparationChannels << endl;
    cout << "Cluster separation time: " << clusterSeparationTime << endl;
    cout << "Minimum angle: " << minAngle << endl;
    cout << "Maximum angle: " << maxAngle << endl;
  }

  // load event display parameters
  eventDisplayPages   = (int) (eventDisplay_[0]);
  eventDisplayMinTime = eventDisplay_[1];
  eventDisplayMaxTime = eventDisplay_[2];

  if (fibre_debug >= 1) {
    cout << "No of EventDisplay pages: " << eventDisplayPages  << endl;
    cout << "EventDisplay minimum time: " << eventDisplayMinTime  << endl;
    cout << "EventDisplay maximum time: " << eventDisplayMaxTime  << endl;
  }

  // init event display
  if (eventDisplayPages > 0)
    EventDisplay = new FibreEventDisplay(eventDisplayPages, eventDisplayMinTime, eventDisplayMaxTime, true);

  // load TDC offsets
  for (int i=0; i<FIBRECHNS; i++)
  {
    TDC_offset[i]=  TDC_offset_[i];
  }


  return 0;
}


