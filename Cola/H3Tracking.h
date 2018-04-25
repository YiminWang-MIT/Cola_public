//
// Hadron3 Tracking classes in Cola++, header file
//
// @(#) Hadron3
// @(#) Pablo Barneo
// @(#) NIKHEF, Amsterdam
//
// $Id: H3Tracking.h 2216 2008-06-13 21:13:47Z distler $
//

#define dbH rundb.H

enum { Ho1, Ho2, L1, L2, L3, L4, L5, L6};
enum { Ll1, Ll2, Ll3, Ll4, Ll5, Ll6};

////////////////////////////////////////////////////////////////////////////
//  TRACK STRUCTURE
//  this structure type contains a tree like structure in which the
//  information for all the tracks in thea detector is stored.
//  It is organized as follows
//
//  organization:
//  -------------
//
//  the data are stored in 25 structure variables. The variables element
//  and next work together to define the track. the array element contains
//  the number of the scintillator (see common events), and next
//  points to the next structure where you find
//  the next scintillator, and so on until element becomes -1.
//  This indicates the end of the this track. next points now to
//  the first structure of the next track. The last track is indicated
//  by a null pointer (next .eq. 0)
//
//  example:
//  --------
//
//  strucnr |   element	  |	tlayer...ldiff		  | next points to
//  --------+---------------+-------------------------------+------------
//  1	    |	8	  |	scintillator data	  |	2
//  2	    |	21	  |	scintillator data	  |	3
//  3	    |	31	  |	scintillator data	  |	4
//  4	    |	37	  |	scintillator data	  |	5
//  5	    |	-1	  |	track data		  |	6
//  6	    |	4	  |	scintillator data	  |	7
//  7	    |	21	  |	scintillator data	  |	8
//  8	    |	28	  |	scintillator data	  |	9
//  9	    |	-1	  |	track data		  |	10
//  10	    |	-1	  |	irrelevant		  |	0
//
//  element	the scintillator number (or -1)
//  tlayer	the layer where this element can be found
//              (in the track data: stopping layer)
//  rawflag	copy of flags in events common block (see there)
//  procflg	flags set by track routines:
//  bit 0:	flags stopping layer
//
//  procran flags set by track routines: flags last layer with light
//  light	raw adc converted to light
//  ttime	raw tdc corrected for conversion gain and offset
//  tptf	light value converted to energy deposited in
//		scintillator
//	        (in track data: measured total energy)
//  ldiff	loss value calculated from the estimated total
//		energy obtained from adc in stopping layer
//		and range before this layer
//  next	points to next valid data (see above)
//

class TRACK {
public:
  unsigned short rawflag, procflg;
  int element, tlayer, procran;
  int trkh1, trkh2, trkl1, trkl2, trkl3, trkl4, trkl5, trkl6;
  double light, ttime, tpb0, tpbf, tptf, tpt0, ldiff, theta;
  double phi, phiraw, imp_angle, l1adc;
  TRACK *next, *nextgood, *nextbad, *previous;
  static TRACK *head;

  TRACK(TRACK *);
  ~TRACK();
  void print(int mode, int event_nr);
  void flgstpl();
  int counttrack(int);
  void etot(double, int);
};
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// GLOBAL TRACK STRUCTURE
// This structure returns the results of the track routines
// to the main program. Next points to the next trackresult structure.
// (In the same way as for h3_track.h. ??)
//
//  NSS	      dimension of scintillator related data array's
//            (max number of scintillators (see also ev_dim
//	      in common/events))
//  trflags   flags pertinent to this particular track
//            (see procflg in common/track/)
//  trrange   flags the range (=last layer) within the
//	      candidate array in which light was detected
//  trtime    time converted to nsec, between timestop (which
//	      is the signal to the coincidence unit) and the
//	      signal in the element of l1, which belongs in
//	      this track [nsec]
//  trangle   impact angle
//  toten     total energy deposited in the detector by this
//            track [MeV]
//  trtheta   (vertical) angle of track [radians]
//  trphi     (horizontal) angle [radians]
//  trpoint   pointer to begin of track
//

class TRGLOB {
public:
  unsigned short trflags, trh1flag, trh2flag;
  int trkh1, trkh2, trkl1, trkl2, trkl3, trkl4, trkl5, trkl6, trrange;
  double trtime, toten, tralpha, trtheta, trphi, trphiraw;
  double tbox, t0, trl1adc;
  double trlifetime, trlifetime_err;
  TRACK *trpoint;
  TRGLOB *next;
  static TRGLOB *head;

  TRGLOB();
  ~TRGLOB();
  void print(int);
};
//////////////////////////////////////////////////////////////////
