//                                                                    -*-c++-*-
// authors:
//
// Christian Kunz                            mailto:kunz@mit.edu
// MIT, Lab for Nuclear Science, 26-567      tel.   (617) 253-1521
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// Michael O. Distler                        mailto:distler@mit.edu
// MIT, Lab for Nuclear Science, 26-402b     tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/Chamber/HDCoops.h,v 2.5 1999-02-22 17:32:01 cola Exp $
//
// Classes to analyse OOPS horizontal driftchambers (HDCs)
//

#ifndef __HDC_OOPS_H__
#define __HDC_OOPS_H__

#include <math.h>
#include <string.h>
#include "../Chamber/hdcInit.h"

const int ErrorCodeNoError    = 0x0000;
const int ErrorCodeNoADC      = 0x0001;
const int ErrorCodeNoTDCright = 0x0002;
const int ErrorCodeNoTDCleft  = 0x0004;
const int ErrorCodeBadADC     = 0x0001;

const int GoodnessAdcBetween  = 0x0001;
const int GoodnessAdcLow      = 0x0002;
const int GoodnessAdcHigh     = 0x0004;
const int GoodnessDriftHigh   = 0x0008;
const int GoodnessDriftLow    = 0x0010;
const int GoodnessBadWire     = 0x0020;

const float wireSpacing = 8.128;  //  mm (from Christian)
const float chamSpacing = 10; // cm

class EventDispatch;
class VdcRegression;


/**Represents one plane of the OOPS horizontal driftchambers.
 */
class HdcPlane {
private:
  char * Name;
  void MemInit(const char *name);
protected:
  PlaneDefault def;
  int adc, right, left;
  int oeSign;
  int error;
  int goodness;
  int nofit;
  int badplane;
  float coord2Ch;
  float sinRot, cosRot;
public:
  /**Constructor.
   */
  HdcPlane(const char *name);

  /**Constructor.
   */
  HdcPlane(EventDispatch *evd, const char *name);

  /**Destructor.
   */
  ~HdcPlane();

  /**Initializes drift time tables and additional calibration constants.
   * Function creates a \Ref{hdcInit} parser object which scans the initfile.
   */
  void Init(const char *initfile);

  /**Pass the hdc data to the plane object. The data has to have the COLA
   * format which is a number of header and actual adc and tdc data. The
   * length is len short words in total.
   */
  int FillData(unsigned short data[], int len);

  /**Reset the internal variables to default values.
   * This function has to be called after every event.
   */
  void Reset(void);

  /**Calculate the difference of the two tdc values.
   * Result is in ns.
   */
  float diff_ns(void);

  /**Calculate the difference of the two tdc values.
   * Result is calibrated to give the wire number.
   */
  float diff_wire(void);

  /**Returns the wire number calculated from the difference of the
   * two tdc values.
   */
  int wire(void);

  /**Calculate the drift time using the sum of the tdc values.
   */
  float dtime(void);

  /**Looks up drift time and returns normalize drift distance.
   * The return value ranges between 0 and 1.
   */
  float drift(void);

  /**Return the error code.
   */
  int errorCode();

  /**Determines sign for odd/even wires.
   */
  int sign(void);

  /**Flip the sign of the odd-even information. For OOPS
   * chambers the sign of the is different for x- and y-planes.
   * The sign is set in the initialization file.
   * doflip can be either true (0) or false (1)
   */
  void flipOESign(int doflip); 

  /**Determines overall sign for drift correction
   */
  int oddEven(void);

  /**Calculate the intersection point
   * of the particle track and the wire plane.
   * flip is either 1 or -1
   * Returns a fitted coordinate if nofit bit is set for this plane
   */
  float coord(int flip = 1);

  /**Calculate the intersection point
   * of the particle track and the wire plane.
   * flip is either 1 or -1
   */
  float realCoord(int flip = 1);

  /**Calculate the intersection point
   * of the particle track and the wire plane without offsets
   * flip is either 1 or -1
   */
  float rawcoord(int flip = 1);

  /**Calculates a statistical weight according to ADC value.
   * flip is either 1 or -1
   */
  float weight(int flip=1);

  /**Return the z position [mm] of the plane.
   * Used by the packet to calculate the track.
   */
  float z(void);

  /**Returns the rotation angle of the plane in rad.
   */
  float rotation(void);

  /**Returns the sin of the rotation angle.
   */
  float sinRotation(void);

  /**Returns the cos of the rotation angle.
   */
  float cosRotation(void);

  /**Returns the offset of the plane in mm.
   */
  float offset(void);

  /**Returns the goodness of the plane data.
   */
  int goodCheck(void);

  /**Returns the Fitting bit
   */
  int noFit(void);

  /**Sets the no Fitting bit
   */
  void setNoFit(void);

  /**Set a coordinate extracted from the two other planes
   */
  void setCoord2Ch(float);

  /**Mark this plane as bad meaning tracks can be extracted without this plane
   */
  void markBad();

  /**return bad flag of a plane
   */
  int isBad();

}; // end of class HdcPlane


/**Represents a horizontal drift chamber of OOPS.
 * Consists of a x- and a y-plane.
 */
class HdcChamber {
private:
  HdcPlane *x, *y;
  void meminit();
public:
  /**Constructor. Uses x and y planes as input.
   */
  HdcChamber(HdcPlane *X, HdcPlane *Y);

  /**Constructor.
   */
  HdcChamber(EventDispatch *evd, const char *path);

  /**Destructor. Free memory.
   */
  ~HdcChamber();

  /**Initialize drift time tables and calibration constants.
   */
  void Init(const char * fileX, const char * fileY);

  /**Reset internal memory in order to get ready for the next event.
   * @memo Reset internal memory
   */
  void Reset(void);

  /**Calculates x and y coordinates. 
   * pattern gives the sign of the drift.
   * 3 (11) = +y+x, 2 (10) = +y-x, 1 (01) = -y+x, 0 (00) = -y-x
   */
  int getCoord(int pattern,
	       float& x, float& xWeight, float& y, float& yWeight);

  /**Return a pointer to the x plane.
   */
  HdcPlane * getX();

  /**Return a pointer to the y plane.
   */
  HdcPlane * getY();

}; // end of class HdcChamber


/**Represents a detector packet of OOPS.
 * Consists of three horizontal drift chambers.
 */
class HdcPacket {
protected:
  HdcChamber *ch[3];
  VdcRegression* trackinfoX;
  VdcRegression* trackinfoY;
  int status, Best, pattern;
  int notrack;
  int shuffle();
  void meminit();
  void FillTrack();
  void twoChamberFit();
public:
  /**Constructor.
   */
  HdcPacket(HdcChamber *ch1, HdcChamber *ch2, HdcChamber *ch3);

  /**Constructor.
   */
  HdcPacket(EventDispatch *evd, const char *path);

  /**Destructor.
   */
  ~HdcPacket();

  /**Reset internal memory in order to get ready for the next event.
   * @memo Reset internal memory
   */
  void Reset();

  /**Returns a pointer to the x plane.
   * Planes (pl) are numbered 1, 2, and 3.
   */
  HdcPlane *getXplane(int pl);

  /**Returns a pointer to the y plane.
   * Planes (pl) are numbered 1, 2, and 3.
   */
  HdcPlane *getYplane(int pl);

  /**
   */
  int ok();

  /**
   */
  double x();

  /**
   */
  double y();

  /**
   */
  double slopeX();

  /**
   */
  double slopeY();

  /**
   */
  double chisqrX();

  /**
   */
  double chisqrY();

  /**
   */
  double theta();

  /**
   */
  double phi();

  /**
   */
  double dx();

  /**
   */
  double dy();

  /**
   */
  double dtheta();

  /**
   */
  double dphi();

  /**Calculates coordinates for every chamber according to optimized pattern.
   */
  float bestX(int pl);

  /**Calculates coordinates for every chamber according to optimized pattern.
   */
  float bestY(int pl);

  /**Returns the sign for a plane from the optimized pattern.
   */
  int patSign(int bit);

  /**
   */
  int bestPat(void);

  /**
   */
  int noTrack(void);

}; // end of class HdcPacket


/**
 *
 */
class HdcApacket : public HdcPacket {
public:
  /**Constructor.
   */
  HdcApacket(EventDispatch *evd, const char *path);
}; // end of class HdcApacket


/**
 *
 */
class HdcBpacket : public HdcPacket {
public:
  /**Constructor.
   */
  HdcBpacket(EventDispatch *evd, const char *path);
}; // end of class HdcBpacket


/**
 *
 */
class HdcCpacket : public HdcPacket {
public:
  /**Constructor.
   */
  HdcCpacket(EventDispatch *evd, const char *path);
}; // end of class HdcCpacket


/**
 *
 */
class HdcDpacket : public HdcPacket {
public:
  /**Constructor.
   */
  HdcDpacket(EventDispatch *evd, const char *path);
}; // end of class HdcDpacket

#endif /* __HDC_OOPS_H__ */
