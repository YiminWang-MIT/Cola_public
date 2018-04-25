//
// Hadron3 Tracking in Cola++
//
// @(#) Hadron3
// @(#) Pablo Barneo
// @(#) NIKHEF, Amsterdam
//
// $Id: H3Tracking.cc 2354 2009-11-13 13:58:04Z merkel $
//
#include "rundatabase.h"       // Run Database
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cmath>
#include "H3Tracking.h"

////////////////////////////////////////////////////////////////////////////
//  TRACK STRUCTURE ROUTINES
//
TRACK *TRACK::head = NULL;
//TRACK *TRACK::tail = NULL;
//
// Constructor of TRACK
TRACK::TRACK(TRACK *previous_track)
{
  if (!head) head = this;

  if (previous_track) {
    previous_track->nextgood=previous_track->nextbad=previous_track->next=this;
  }

  element = -1;
  procran = 0;
  tlayer = -999;
  rawflag = procflg = 0;
  trkh1 = trkh2 = trkl1 = trkl2 = trkl3 = trkl4 = trkl5 = trkl6 = -1;
  light = ttime = tpbf = tpb0 = tptf = tpt0 = ldiff = theta = phi = 
    phiraw = imp_angle = l1adc = -999.0;
  next = nextgood = nextbad = NULL;
  previous  = previous_track;
}

// Destructor of TRACK
TRACK::~TRACK()
{
  if (next) delete next;
  next = NULL;
  nextgood = nextbad = NULL;
}

void TRACK::print(int mode, int event_nr)
{
  //-----------------------------------------------------
  //	Print the track structure.
  //	Mode = 0 --> Total track structure
  //	Mode = 1 --> Only the "good" tracks
  //    Mode = 2 --> Only the "bad" tracks
  //-----------------------------------------------------

  TRACK *track = this;
  char *empty = "      "; // 6 blanks
  char *layer = new char[3];
  int nl;
  
  switch (mode) {
  case 0:
    std::cerr << "Hadron3 Track: TRACK output for event number: "
	      << event_nr << std::endl;
    break;
  case 1:
    std::cerr << "Hadron3 Track: GOOD TRACK output for event number: "
	      << event_nr << std::endl;
    break;
  case 2:
    std::cerr << "Hadron3 Track: BAD TRACK output for event number: "
	      << event_nr << std::endl;
    break;
  default: 
    std::cerr << "Hadron3 Track: In print routine, illegal mode: "
	      << mode << std::endl;
    delete layer;
    return; // Hadron3 failure
  }

  std::cerr << "      elm rawf prcf tlay pr h1 h2 l1 l2 l3 l4 l5 l6   light   ttime  tpbf   tptf   tpt0  ldiff    theta      phi   phiraw   impangle l1adc\n";

  do {
    nl = track->tlayer + 1;
    if (nl < 2) sprintf(layer, "%s%1d", "H", nl+1);
    else sprintf(layer, "%s%1d", "L", nl-1);
    
    if ( (track->element >= dbH.layer.first[nl]) && 
	 (track->element < dbH.layer.first[nl+1])   ) {
      std::cerr << layer << '-';
      sprintf(layer, "%02d",track->element-(int)dbH.layer.first[nl]+1);
      std::cerr << layer << ' ';
    } else std::cerr << empty;

    char str[256];
    sprintf(str, "%3d %4x %4x %4d %2d %2d %2d %2d %2d %2d %2d %2d %2d %8.1e %6.1f %6.1f %6.1f %6.1f %6.1f %8.1e %8.1e %8.1e %8.1e %6.1f\n",
	    track->element, track->rawflag, track->procflg, track->tlayer,
	    track->procran, track->trkh1, track->trkh2, track->trkl1,
	    track->trkl2, track->trkl3, track->trkl4, track->trkl5,
	    track->trkl6, track->light, track->ttime, track->tpbf, track->tptf,
	    track->tpt0, track->ldiff, track->theta, track->phi, track->phiraw,
	    track->imp_angle, track->l1adc);
    std::cerr << str;
    switch (mode) {
    case 0:
      track=track->next;
      break;
    case 1:
      track=track->nextgood;
      break;
    case 2:
      track=track->nextbad;
      break;
    default:
      std::cerr << "Hadron3 Track: In printing routine, illegal mode: "
	   << mode << std::endl;
      track = NULL;
      delete layer;
      return; // Hadron3 failure
    }
  } while (track);

  std::cerr << std::endl;
  track = NULL;
  delete layer;
  return;
}

void TRACK::flgstpl()
{
  //----------------------------------------------------------------------
  // This function takes a track tree structure, walks it and
  // flags the layers where tracks are stopped. each new call will
  // update the flags to the new situation.
  //
  // parameter:
  // tracki	pointer to the track tree structure
  //
  // local variables:
  // last	pointer to the element just before the current
  // track	local copy of tracki
  // mask	bit 0: stoppinglayer bit
  //----------------------------------------------------------------------

  TRACK	*track, *last;
  int  layer;
	
  last = track = this;
  
  // while (track) {
  do {
    //---------------------------------------
    //  Check if end of this track.
    //---------------------------------------
    while (track->element != -1) {
      //--------------------------------------------------------------------
      //  No, clear stoppinglayer bit, and try next scintillator in track.
      //--------------------------------------------------------------------
      track->procflg = ( track->procflg & (unsigned short)0 );
      last = track;
      track = track->next;
    }
    //---------------------------------------------------------------------
    // Yes, so the scintillator before this must be the stopping layer.
    // Set stopping layer bit (but check first for valid scintillator).
    //---------------------------------------------------------------------
    if (last->element != -1 ) {
      last->procflg = (last->procflg | (unsigned short)1);
      layer = last->tlayer;
      track->tlayer = layer;
      if ((layer > 0) && (layer <= NLS)) 
	track->procflg = (track->procflg | (1 << (layer-1)));
    }
    last=track;
    track=track->next;
  } while (track);
  return;
}

int TRACK::counttrack(int mode)
{
  //--------------------------------------------------------
  //  Count the number of tracks
  //  Mode = 0 --> All tracks
  //  Mode = 1 --> Only the "good" tracks
  //  Mode = 2 --> Only the "bad" tracks
  //-------------------------------------------------------

  TRACK	*tracki = this;
  int ntrk = 0;
  
  while ( tracki && (tracki->element != -1)) {
    ntrk++;
    switch (mode) {
    case 0:
      while (tracki && (tracki->element != -1))	tracki=tracki->next;
      if (tracki) tracki=tracki->next;
      break;
    case 1:
      while (tracki && (tracki->element != -1))	tracki=tracki->nextgood;
      if (tracki) tracki=tracki->nextgood;
      break;
    case 2:
      while (tracki && (tracki->element != -1))	tracki=tracki->nextbad;
      if (tracki) tracki=tracki->nextbad;
      break;
    default:
      std::cerr << "Hadron3: Error counting tracks. Illegal mode: "<<mode<< std::endl;
      return -1;
    }
  }
  return ntrk;
}

void TRACK::etot(double light, int stplr)
{
  //----------------------------------------------------------------------
  //  estimates total energy from light value from stopping layer
  //
  //  uses a 3rd degree polynomial approximation, obtained from 
  //  data from the program plop. (march '90, plop made by L. Kester)
  //
  //  parameters:
  //  lite		input light value
  //  stplr		input stopping layer number
  //
  //  local variables:
  //  istrt	first value of loop counter in the evaluation of the polynomial
  //  Ltpb_pol	array containing the coefficients for energy in box for protons
  //  Ltpt_pol	array containing the coefficients for energy 
  //            at target for protons
  //  Ltdb_pol	array containing the coefficients for energy 
  //            in box for deuterons
  //  Ltdt_pol	array containing the coefficients for energy 
  //            at target for deuterons
  //----------------------------------------------------------------------

  int istrt;

  istrt = stplr-1;

  if ( (istrt < 0) || (istrt >= NLS-2) ) {
    if (dbH.swt.debug)
      std::cerr << "Hadron3: In etot, stoplayer should be more than 1, but is: "
	   << istrt+1 << std::endl;
    return;
  }

  //---------------------------------------------------------
  //	Calculation of T_p in box
  //---------------------------------------------------------

  tpb0 = dbH.Llayer.tpb[istrt][0] + dbH.Llayer.tpb[istrt][1]*light +
    dbH.Llayer.tpb[istrt][2]*light*light + 
    dbH.Llayer.tpb[istrt][3]*light*light*light;

  tpbf = tpb0/cos(dbH.Llayer.e_cos_fac_box[istrt] * imp_angle);

  //--------------------------------------------------------
  //   Calculation of T_p at target
  //--------------------------------------------------------

  tpt0 = dbH.Llayer.tpt[istrt][0] + dbH.Llayer.tpt[istrt][1]*light +
    dbH.Llayer.tpt[istrt][2]*light*light + 
    dbH.Llayer.tpt[istrt][3]*light*light*light;

  tptf = tpt0/cos(dbH.Llayer.e_cos_fac_tar[istrt] * imp_angle);

  return;
}
//
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// GLOBAL TRACK STRUCTURE ROUTINES
//
TRGLOB *TRGLOB::head = NULL;
//TRGLOB *TRGLOB::tail = NULL;
//
// Constructor TRGLOB
TRGLOB::TRGLOB()  
{
  if (!head) head = this;

  trflags = trh1flag = trh2flag = 0;
  trrange = 0;
  trlifetime_err = 0.0;
  trtime = toten = tralpha = trtheta = trphi = trphiraw = tbox = t0 = 
    trl1adc  = -999.0;
  trlifetime = 1.0;
  trkh1 = trkh2 = trkl1 = trkl2 = trkl3 = trkl4 = trkl5 = trkl6 = -1;
  next = NULL;
  trpoint = NULL;
}

// Destructor TRGLOB
TRGLOB::~TRGLOB() 
{
  if (next) delete next;
  next = NULL;
  trpoint = NULL;
}

void TRGLOB::print(int event_nr)
{
  //----------------------------------------------
  //	Print the tracksum structure.
  //----------------------------------------------
  
  TRGLOB *track = this;

  std::cerr << "TRACKSUM output for event number: " << event_nr << std::endl;
  std::cerr << "flag h1fl h2fl      time  toten  alpha  theta    phi  phirw   tbox     t0 l1adc lifetime  error  range trh1 trh2 trl1 trl2 trl3 trl4 trl5 trl6\n";
  do {
    char str[256];
    sprintf(str, "%4x %4x %4x %9.4f %5.2f %6.3f %6.3f %6.3f %6.3f %5.2f %5.2f %4.1f %7.4f %6.3f %4d %4d %4d %4d %4d %4d %4d %4d %4d\n\n", track->trflags,
	    track->trh1flag, track->trh2flag, track->trtime, track->toten,
	    track->tralpha, track->trtheta, track->trphi, track->trphiraw,
	    track->tbox, track->t0, track->trl1adc, track->trlifetime,
	    track->trlifetime_err, track->trrange, track->trkh1, track->trkh2,
	    track->trkl1, track->trkl2, track->trkl3, track->trkl4,
	    track->trkl5, track->trkl6);
    std::cerr << str << std::endl;
    track->trpoint->print(0, event_nr);
    track=track->next;
  } while (track);
  track = NULL;
  return;
}
//
//////////////////////////////////////////////////////////////////
