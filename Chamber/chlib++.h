//								      -*-c++-*-
// Created 1993-2014 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Chamber/chlib++.h,v 2.25 2001-02-28 02:49:20 distler Exp $
//

#ifndef __chlib_plus_plus__
#define __chlib_plus_plus__

#define _DEFAULT_SOURCE 1

//#define LIKELIHOOD 1
#undef LIKELIHOOD

#include <stdio.h>
#include <math.h>

/**@name Chamber Library
 *
 * This document is a user's guide to the chamber library, a
 * library written in C++, used to calculate the coordinates
 * and angles of particle tracks in vertical and horizontal
 * drift chambers. These types of drift chambers are commonly
 * used as the focal plane detector of magnetic spectrometers,
 * e.g. at MAMI (Mainz Microtron, Germany) or MIT/Bates, USA.
 *
 * @memo     A drift chamber library
 * @version  2.15
 * @author   Michael O. Distler, Christian Kunz
 */

//@{
//@Include: chlib.dxx
//@Include: HDCoops.h
//@Include: ../Cindy/EventDispatch.h

#ifdef LIKELIHOOD
class VdcLikelihood;
#else
class VdcRegression;
#endif
class EventDispatch;

// ################ VdcPlane ################

const int ErrorCode0 = 0x0000;	// no error
const int ErrorCode1 = 0x0001;	// no wire
const int ErrorCode2 = 0x0002;	// two sign reversals
const int ErrorCode3 = 0x0004;	// more than two sign reversals
const int ErrorCode4 = 0x0008;	// removed single wire
const int ErrorCode5 = 0x0010;	// possible multi hit

struct chRawData {
  short wire;
  short time;
};

struct vdcPosOff {
  float *pos;
  short *off;
  short len;
};

/**This class has a large number of internal variables - most of
 * them are accessible by member functions. To understand these
 * functions you have to know that there are three types of wires:
 * \begin{enumerate}
 * \item Disabled wires: If you disable wires or the drift time
 *       is smaller then the defined minimum they are ignored
 *       completely and will not show up in this plane object.
 * \item Good wires are sorted by wire number and can be accessed
 *       at the beginning of the several internal vectors.
 * \item Noise: Hits that are recognized as noise are moved to
 *       the end of the internal vectors.
 * \end{enumerate}
 * You should understand now that the number of good wires is
 * always smaller or equal to the total number of wires.
 *
 * @memo VDC plane
 */
class VdcPlane {
public:
  /**Returns the number of wires, which have fired.
   */
  short NumberOfWires(void) { return num_of_wires; }

  /**Returns the number of wires, which are used for calculation.
   */
  short GoodWires(void) { return good_wires; }

  /**Returns a wire pattern. For each wire which has fired a bit in
   * the pattern is set. For example: wire numbers 48, 49, 50 and 52
   * give the pattern 10111b = 23.
   */
  unsigned int Pattern(void);

  /**Reset the internal memory after the calculation. It is very
   * {\bf important} to call this function at the end of your event loop.
   */
  void Reset(void);

  /**Return the error code. Each bit has a specific meaning:
   * \begin{verbatim}
   const int ErrorCode0 = 0x0000;	/ no error
   const int ErrorCode1 = 0x0001;	/ no wire
   const int ErrorCode2 = 0x0002;	/ two sign reversals
   const int ErrorCode3 = 0x0004;	/ more than two sign reversals
   const int ErrorCode4 = 0x0008;	/ removed single wire
   const int ErrorCode5 = 0x0010;	/ possible multi hit
   \end{verbatim}
   */
  int errorCode(void) { return error_code; }

  /**Returns the x coordinate. This is the intersection
   * of the track and the wire plane. [mm]
   */
  double x(void);
  /**Returns the slope of the track. To be more precise
   * the slope of the projection onto the x-z plane.
   */
  double slope(void);

  /**Returns the angle between the track and the z-axis. [mrad]
   */
  double angle(void);

  /**Returns the Chi-square of the linear regression fit.
   */
  double chi_sqr(void);

  /**Returns the error (sigma) of the fit for the x coordinate. [mm]
   */
  double sigma_x(void);

  /**Returns the error (sigma) of the fit for the slope.
   */
  double sigma_slope(void);

  /**Returns the name of the plane as given during the initialisation.
   */
  const char *NameOfPlane(void);

  /**Returns the total number of preamplifier cards.
   */
  int NumberOfCards(void) const;

  /**Returns the total number of signal wires.
   */
  short SignalWires(void) { return max_wire; }

  /**Returns the position of the first wire.
   */
  float getFirstPosition(void) { return position[0]; }

  /**Returns a pointer to a vector of short integers
   * containing the wire numbers of each wire that fired.
   * @memo Returns a vector of wire numbers
   */
  const short *WiresFired(void) { return wire; }

  /**Returns a pointer to a vector of short integers
   * containing	the raw drift times for each wire that fired.
   * @memo Returns a vector of drift times
   */
  const short *RawDriftTimes(void) { return time; }

  /**Returns a pointer to a vector of floats
   * containing the drift path. [mm]
   * @memo Returns a vector of drift paths
   */
  const float *DriftPath(void) { return path; }

  /**Returns a pointer to a vector of floats containing
   * the drift path correction. [mm]
   * @memo Returns a vector of drift paths corrections
   */
  const float *DriftCorrection(void) { return corr; }

  /**Returns a pointer to a vector of shorts containing
   * the tdc offsets 
   * @memo Returns a vector of tdc offsets
   */
  const short *TdcOffset(void) { return offset; }

  /**Returns a pointer to a vector of shorts containing
   * the paddle offsets
   * @memo Returns a vector of paddle offsets
   */
  const short *PaddleOffset(void) { return paddle; }

  /**Returns a pointer to a vector of floats containing
   * the position of each wire that fired. [mm]
   * @memo Returns a vector of wire positions
   */
  const float *WireCoordinate(void) { return coord; }

  /**Used to disable single wires (parameter is the wire number)
   * or a number of successive wires (first parameter is the
   * starting wire, second is the number). Disabled wires will not
   * be used for any calculation.
   * @memo Disable (software!) certain wires
   */
  int disableWire(int wire, int count=1);

  /**Sets a temporary tdc offset. This offset is added to the {\bf
   * normal} tdc offset. The measured tdc count is subtracted from
   * the sum of both offsets and gives the uncorrected drift time.
   * The value of the temporary offset is set to zero if the
   * vdcPlane object is initialized or the \Ref{VdcPlane::Reset}
   * function is used.
   */
  void setTemporaryOffset(short toffs);

  /** Adjust paddle offset */
  void CorrectPaddleOffset(int nPaddle) {
    if (nPaddle >= 0 && nPaddle < 16)
      setTemporaryOffset(paddle[nPaddle]);
  }

protected:
  char   *disabledWire;
  int     error_code, default_slope;
  char	 *Name;
  float  *position, *coord, *path, *corr, *times, *path_diffs, min_drift;
  short  *offset, *wire, *time, tmpOffset, *paddle, *tmp_disable;
  short   first_wire, last_wire, max_wire, status, full_range;
  short   num_of_wires, good_wires, central_wire, num_of_sign_rev;
  double  x_, slope_, chisqr, sx, sslope, last_slope;
  double  time_to_length;
  void reverseSign(void);
  void CalcSums(void);
  void MemInit(const char *, int);
  void ignoreWire(short);
  void checkSingleWires(void);
  void checkSignRev(void);
  double corrPar[5];
  double driftTimeCorrFun(double a);
  int ignore_mode;
  float ignore_min, ignore_max, ignore_slope;
public:
  /**Copy Constructor. Allocates separate memory for the internal data.
   */
  VdcPlane(const VdcPlane &plane);

  /**Constructor. Specify the name and the maximum number of wires.
   */
  VdcPlane(const char *name, int size);

  /**Constructor. Specify the name and the maximum number of wires.
   * The first argument is the event dispatcher, e.g. a pointer
   * to a CindyTree object. Be sure that {\tt name} describes an
   * existing item.
   */
  VdcPlane(EventDispatch *evd, const char *name, int size);

  /**Callback function. This method is use to transfer the data
   * (wire numbers and drift times) to the object. The first
   * argument is a pointer to an array of short integers. {\tt
   * len} is the number of shorts (not wires).
   * \begin{verbatim}
     struct chRawData {
       short wire;
       short time;
     };
     \end{verbatim}
   */
  int FillTime(chRawData *cht, int len);

  /**Initialize the position for each wire and the TDC offset.
   * \begin{verbatim}
     struct vdcPosOff {
       float *pos;
       short *off;
       short len;
     };
     \end{verbatim}
   * @memo Initialization of position and TDC offset	
   */
  int InitPositionAndOffset(vdcPosOff *);

  /**Initialize the TDC offsets.
   */
  void InitOffset(short *offs);

  /**Initialize the TDC offsets. All channels get the same offset.
   */
  void InitOffset(short offs, short start = 0, short count = -1);

  /**Initialize the paddle offsets.
   */
  void InitPaddleOffset(short *offs);

  /**Initialize a single paddle offset.
   */
  void InitPaddleOffset(short offs, int nPaddle);

  /**Initialize the position (in mm) of all wires
   */
  void InitPosition(float *pos);

  /**Initialize the position (in mm) of a single wire.
   */
  void InitPosition(int wire, float pos);

  /**Initialize the conversion factor (TDC resolution * drift velocity).
   * A typical value is 0.75 ns/bin times 0.052 mm/ns.
   * @memo Initialize the conversion factor
   */
  void InitTimeToLength(double ttl) { time_to_length = ttl; }

  /**Initialize the TDC full range. A typical value is 512 bins.
   */
  void InitFullRange(short fr) { full_range = fr; }

  /**Initialize the minimum drift path (in mm) that will be used
   * for the calculation. If the drift path is smaller then 1 mm the
   * information has a large systematic error and it might be better
   * to omit this wire. The minimum drift is set to 0 mm by default.
   * @memo Initialize the minimum drift path
   */
  void InitMinimumDrift(float min) { min_drift = min; }

  /**Initialize the criteria for ignoring some wire information.
   * If the mode is set different from zero, wires will be ignored
   * if their drift times do not have a correct difference to the
   * drift times of their neighbor wires, i.e. the difference does not
   * lie in the interval
   *   min+k*slope < abs(time[k]-time[k+-1]) < max+k*slope
   * mode= 1 means that at least one neighbor must lie in the interval
   *         (recommended for x-planes)
   * mode=-1 means that all neighbors must lie outside the interval
   *         (recommended for s-planes)
   * @memo Initialize wire ignoring criteria
   */
  void InitIgnoreCriteria(int mode, float min, float max, float slope) { 
    ignore_mode  = mode;
    ignore_min   = min;
    ignore_max   = max;
    ignore_slope = slope;
  }

  /**Initialize the drift time correction function.
   */
  void InitCorrection(const double *par);

  /**This method is used for debug purpose only. The name, the
   * number of wires that are used for the calculation, the number
   * of wires that have fired, as well as wire number and drift
   * time are printed to {\it stdout}.
   * @memo Print debug information
   */
  void printWireTime(void);

  /**This method is used for debug purpose only. The name, the
   * coordinate, the slope and the corresponding errors are
   * printed to {\it stdout}.
   * @memo Print debug information
   */
  void printCoordinates(void);

  /**Destructor. Cleans up the internal memory.
   */
  ~VdcPlane(void);

#ifdef LIKELIHOOD
  VdcLikelihood *fit;
#else
  VdcRegression *reg;
#endif
  double Regression(double sl = 0.0);
  friend double calcSlope(VdcPlane *, VdcPlane *, double);
  friend double recalcSlope(VdcPlane *, VdcPlane *, double);
};

// ################ VdcPacket ################

const int PACKET_STATUS_X1	= 0x0001;
const int PACKET_STATUS_S1	= 0x0002;
const int PACKET_STATUS_X2	= 0x0004;
const int PACKET_STATUS_S2	= 0x0008;
const int PACKET_STATUS_CHAMBER1= PACKET_STATUS_X1	+ PACKET_STATUS_S1;
const int PACKET_STATUS_CHAMBER2= PACKET_STATUS_X2	+ PACKET_STATUS_S2;
const int PACKET_STATUS_BOTH_X	= PACKET_STATUS_X1	+ PACKET_STATUS_X2;
const int PACKET_STATUS_BOTH_S	= PACKET_STATUS_S1	+ PACKET_STATUS_S2;
const int PACKET_STATUS_BOTH	= PACKET_STATUS_BOTH_X	+ PACKET_STATUS_BOTH_S;

struct vdcPosOffs {
  vdcPosOff x1, s1, x2, s2;
};

/**Each packet consists out of four planes. Allways remember that
 * most of the calculations are done as soon as the data is filled
 * into the plane objects. If coordinates or angles are requested
 * the information of the planes are combined.
 *
 * @memo VDC packet
 */
class VdcPacket {
public:
  /**Reset the internal memory after the calculation. It is very
   * {\bf important} to call this function at the end of your
   * event loop. This method calls the reset function for each of
   * the four planes.
   */
  void Reset(void);

  /**Return the error code. The error code is a bitwise {\it or}
   * of the error value of each of the four planes.
   */
  int errorCode(void);

  /**Return the error code of the specified plane. Valid values
   * for {\tt plane} are 0 to 3. See \Ref{VdcPlane::errorCode}
   * for more information on the return value.
   */
  int errorCode(int);

  /**Returns status information on the track calculation in each
   * plane. The flags for each individual plane are set, if
   * a minimum of 2 wires in each plane has fired
   * and could be used for the calculation.
   * If everything is {\it ok} and the track could be calculated,
   * this function returns {\tt PACKET\_STATUS\_BOTH}.
   * \begin{verbatim}
   const int PACKET_STATUS_X1 = 0x0001;
   const int PACKET_STATUS_S1 = 0x0002;
   const int PACKET_STATUS_X2 = 0x0004;
   const int PACKET_STATUS_S2 = 0x0008;
   const int PACKET_STATUS_CHAMBER1
           = PACKET_STATUS_X1 + PACKET_STATUS_S1;
   const int PACKET_STATUS_CHAMBER2
           = PACKET_STATUS_X2 + PACKET_STATUS_S2;
   const int PACKET_STATUS_BOTH_X
           = PACKET_STATUS_X1 + PACKET_STATUS_X2;
   const int PACKET_STATUS_BOTH_S
           = PACKET_STATUS_S1 + PACKET_STATUS_S2;
   const int PACKET_STATUS_BOTH
           = PACKET_STATUS_BOTH_X + PACKET_STATUS_BOTH_S;
   \end{verbatim}
   * See also \Ref{VdcPacket::ok1} and \Ref{VdcPacket::ok3} for
   * different operation modes.
   * @memo Returns true if a minimum of 2 wires are valid
   */
  int ok(void);

  /**Returns status information on the track calculation in each
   * plane. The flags for both corresponding planes (x or s) are
   * set, if a minimum of 3 wires has fired in both
   * corresponding planes (x and s respectively). This is the
   * enhanced operation and gives the best efficiency.
   * See \Ref{VdcPacket::ok} for description of the flags.
   * @memo Returns true if 3 wires in 2 planes are valid
   */
  int ok1(void);

  /**Returns status information on the track calculation in each
   * plane. The flags for each individual plane are set,if
   * a minimum of 3 wires in each plane has fired
   * and could be used for the calculation. This is the secure
   * operation and should be used if accuracy is more important
   * than efficiency.
   * See \Ref{VdcPacket::ok} for description of the flags.
   * @memo Returns true if a minimum of 3 wires are valid
   */
  int ok3(void);

  /**Returns the s coordinate in mm. Please notice that the angle
   * between the x- and s-wires is not 90 but 40 degrees. Moreover
   * the distance between s- and focal plane is 24mm.
   */
  double s(void);

  /**Returns the x coordinate in mm of the intersection of
   * the particle track and the first (focal) plane.
   * @memo Returns the x coordinate in mm
   */
  double x(void);

  /**Returns the y coordinate in mm of the intersection of
   * the particle track and the first (focal) plane.
   * @memo Returns the y coordinate in mm
   */
  double y(void);

  /**Returns the angle in mrad between the vertikal and the
   * particle track (projected onto the s-z-plane).
   * @memo Returns an angle
   */
  double theta_s(void);

  /**Returns the angle in mrad between the vertikal and the
   * particle track (projected onto the x-z-plane).
   * @memo Returns the dispersive angle
   */
  double theta(void);

  /**Returns the angle in mrad between the vertikal and the
   * particle track (projected onto the y-z-plane).
   * @memo Returns the non dispersive angle
   */
  double phi(void);

  /**Returns the error (sigma) of the fit for the x coordinate in mm.
   */
  double dx(void);

  /**Returns the error (sigma) of the fit for the y coordinate in mm.
   */
  double dy(void);

  /**Returns the error (sigma) of the fit for the angle theta in mrad.
   */
  double dtheta(void);

  /**Returns the error (sigma) of the fit for the angle phi in mrad.
   */
  double dphi(void);

  /**Returns the pointer to the x1 plane.
   */
  VdcPlane* x1Plane() { return x1; }

  /**Returns the pointer to the s1 plane.
   */
  VdcPlane* s1Plane() { return s1; }

  /**Returns the pointer to the x2 plane.
   */
  VdcPlane* x2Plane() { return x2; }

  /**Returns the pointer to the s2 plane.
   */
  VdcPlane* s2Plane() { return s2; }

  /**Set the temporary tdc offset for each plane.
   * See \Ref{VdcPlane::setTemporaryOffset}.
   */
  void setTemporaryOffset(short toffs);

  /** The ultimate function to circumvent the callback junk
   *  (see above)
   */
  void DoTheLinearRegressionAgainWithCorrectOffset(double);

  /** Some hypervirtual friends of this function
   */
  void DoTheLinearRegressionAgainWithX1S1Only();
  void DoTheLinearRegressionAgainWithX2S2Only();
  double DoTheLinearRegressionSeparately();
  
  /** Apply the relative tdc offsets for each paddle
   */
  void CorrectPaddleOffset(int nPaddle);

protected:
  double x_, y_, s_, theta_, theta_s_, phi_, slope_x, slope_s;
  double dx_, dy_, dtheta_, dphi_, dsl_x, dsl_s;
  double sin_x_s, sec_x_s, cos_x_s, csc_x_s, tan_x_s, cot_x_s, ccos_x_s;
  int status, statusX, statusY;
#ifdef LIKELIHOOD
  VdcLikelihood *fitX, *fitS;
#else
  VdcRegression *regX, *regS;
#endif
  char *ident;
  void MemInit(void);
  virtual void CalcCoord(int bitmask);
  VdcPlane *x1, *s1, *x2, *s2;
  int destroyPlanes;
public:

  /**Copy Constructor.
   */
  VdcPacket(void);

  /**Constructor. Used to combine 4 planes.
   */
  VdcPacket(VdcPlane *x_1, VdcPlane *s_1, VdcPlane *x_2, VdcPlane *s_2);

  /**Constructor. Parameters are event dispatcher (CINDY), the name of
   * the packet (something like ".a.det.vdc") and the number of channels
   * (wires) in each plane. The planes have to have the names x1, s1,
   * x2 and s2 respectively. If {\tt uv != 0} the names have to be
   * u1, v1, u2 and v2. Used in \Ref{VdcOHIPSpacket}.
   */
  VdcPacket(EventDispatch *evd, const char *path,
	    int nx1, int ns1, int nx2, int ns2, int uv=0);

  /**Initialize the position for each wire, the TDC offset and
   * the angle between the wires in the x- and s-plane.
   * @memo Initialize position and offset
   */
  int InitPositionAndOffset(vdcPosOffs *init, double angle=40.0);

  /**Initialize the TDC offsets. The data is NOT copied, therefor you
   * must not delete the vectors.
   */
  void InitOffset(short *x1offs, short *s1offs, short *x2offs, short *s2offs);

  /**Initialize the TDC offsets. All channels in one plane get the
   * same offset.
   */
  void InitOffset(short x1offs, short s1offs, short x2offs, short s2offs);

  /**Print a postscript file with a picture of the track through the 
   * packet.
   */
  void drawTrack(char *fname, char *comment);
  
  /**Initialize a conversion factor. (TDC resolution * drift velocity).
   * A typical value is 0.75 ns/bin times 0.052 mm/ns.
   * This call initializes all four planes.
   */
  void InitTimeToLength(double);
  
  /**Initialize the conversion factor for all four planes individually.
   */
  void InitTimeToLength(double, double, double, double);

  /**Initialize the TDC full range. A typical value is 512 bins.
   * This call initializes all four planes.
   */
  void InitFullRange(short);

  /**Initialize the minimum drift path (in mm) that will be used for
   * the calculation. If the drift path is smaller then 1 mm the information
   * has a large systematic error and it might be better to omit this
   * wire. The minimum drift is set to 0 mm by default.
   * This call initializes all four planes.
   * @memo Initialize the minimum drift path
   */
  void InitMinimumDrift(float min);

  /**Initialize wire ignoring criteria, separately for x/s-planes.
   * @memo Initialize wire ignoring criteria
   */
  void InitIgnoreCriteria(int, float, float, float, 
			  int, float, float, float);

  /**Destructor. Cleans up the internal memory.
   */
  virtual ~VdcPacket(void);
};

/**Inheritance is used to derive packet classes for each
 * spectrometer. The new constructors initialize the packet
 * objects with specific data. This includes the position and
 * offset information for each channel, the angle between the
 * wires in the {\it x-} and {\it s-}plane, the drift velocity
 * (typically 50 km/s) and the TDC full range (384 ns or 16384 ns
 * depending on the TDC System) and resolution (0.75 ns/bin or
 * 0.25 ns/bin).
 *
 * @memo VDC packet for spectrometer A
 */
class VdcApacket : public VdcPacket {
  void Init(void);
public:
  /**Constructor. Used to combine 4 planes.
   */
  VdcApacket(VdcPlane *x_1, VdcPlane *s_1, VdcPlane *x_2, VdcPlane *s_2);

  /**Constructor. Parameters are event dispatcher (CINDY), the name of
   * the packet (something like ".a.det.vdc") and the number of channels
   * (wires) in each plane. The planes have to have the names x1, s1,
   * x2 and s2 respectively.
   */
  VdcApacket(EventDispatch *, const char *, int, int, int, int);
};

/**Inheritance is used to derive packet classes for each
 * spectrometer. The new constructors initialize the packet
 * objects with specific data. This includes the position and
 * offset information for each channel, the angle between the
 * wires in the {\it x-} and {\it s-}plane, the drift velocity
 * (typically 50 km/s) and the TDC full range (384 ns or 16384 ns
 * depending on the TDC System) and resolution (0.75 ns/bin or
 * 0.25 ns/bin).
 *
 * @memo VDC packet for spectrometer B
 */
class VdcBpacket : public VdcPacket {
  void Init(void);
public:
  /**Constructor. Used to combine 4 planes.
   */
  VdcBpacket(VdcPlane *x_1, VdcPlane *s_1, VdcPlane *x_2, VdcPlane *s_2);

  /**Constructor. Parameters are event dispatcher (CINDY), the name of
   * the packet (something like ".b.det.vdc") and the number of channels
   * (wires) in each plane. The planes have to have the names x1, s1,
   * x2 and s2 respectively.
   */
  VdcBpacket(EventDispatch *, const char *, int, int, int, int);
};

/**Inheritance is used to derive packet classes for each
 * spectrometer. The new constructors initialize the packet
 * objects with specific data. This includes the position and
 * offset information for each channel, the angle between the
 * wires in the {\it x-} and {\it s-}plane, the drift velocity
 * (typically 50 km/s) and the TDC full range (384 ns or 16384 ns
 * depending on the TDC System) and resolution (0.75 ns/bin or
 * 0.25 ns/bin).
 *
 * @memo VDC packet for spectrometer C
 */
class VdcCpacket : public VdcPacket {
  void Init(void);
public:
  /**Constructor. Used to combine 4 planes.
   */
  VdcCpacket(VdcPlane *x_1, VdcPlane *s_1, VdcPlane *x_2, VdcPlane *s_2);

  /**Constructor. Parameters are event dispatcher (CINDY), the name of
   * the packet (something like ".c.det.vdc") and the number of channels
   * (wires) in each plane. The planes have to have the names x1, s1,
   * x2 and s2 respectively.
   */
  VdcCpacket(EventDispatch *, const char *, int, int, int, int);
};

/**Inheritance is used to derive packet classes for each
 * spectrometer. The new constructors initialize the packet
 * objects with specific data. This includes the position and
 * offset information for each channel, the angle between the
 * wires in the {\it x-} and {\it s-}plane, the drift velocity
 * (typically 50 km/s) and the TDC full range (384 ns or 16384 ns
 * depending on the TDC System) and resolution (0.75 ns/bin or
 * 0.25 ns/bin).
 *
 * @memo prototyp VDC packet
 */
class VdcProtoPacket : public VdcPacket {
  void Init(void);
public:
  /**Constructor. Used to combine 2 planes.
   */
  VdcProtoPacket(VdcPlane *x, VdcPlane *s);
};

// the following derived classes do specific initialization
// for the chambers of the MIT/Bates OHIPS

/**Inheritance is used to derive packet classes for each
 * spectrometer. The new constructors initialize the packet
 * objects with specific data. This includes the position and
 * offset information for each channel, the angle between the
 * wires in the {\it x-} and {\it s-}plane, the drift velocity
 * (typically 50 km/s) and the TDC full range (384 ns or 16384 ns
 * depending on the TDC System) and resolution (0.75 ns/bin or
 * 0.25 ns/bin).
 *
 * @memo VDC packet for OHIPS (MIT/Bates)
 */
class VdcOHIPSpacket : public VdcPacket {
  void Init(void);
protected:
  double dist_u1_u2;
  double dist_v1_v2;
  double dist_u1_v1;
  virtual void CalcCoord(int bitmask);
public:
  /**Constructor. Used to combine 4 planes.
   */
  VdcOHIPSpacket(VdcPlane *u1, VdcPlane *v1, VdcPlane *u2, VdcPlane *v2);

  /**Constructor. Parameters are event dispatcher (CINDY), the name of
   * the packet (something like ".det.ohips.chamber") and the number of
   * channels (wires) in each plane. The planes have to have the names
   * x1, s1, x2 and s2, respectively.
   */
  VdcOHIPSpacket(EventDispatch *, const char *, int, int, int, int);

  /**Initialize chamber with geometrical constants apropriate for
   * pre Y2K runs, especially the N->Delta runs in 1998.
   * At that time a different first chamber was used.
   */
  void Init98(void);

  /**Returns the pointer to the u1 plane.
   */
  VdcPlane * u1Plane() { return x1; }

  /**Returns the pointer to the v1 plane.
   */
  VdcPlane * v1Plane() { return s1; }

  /**Returns the pointer to the u1 plane.
   */
  VdcPlane * u2Plane() { return x2; }

  /**Returns the pointer to the v1 plane.
   */
  VdcPlane * v2Plane() { return s2; }
};

// ################ VdcSetup ################

/**A setup consists out of up to three packets. This object has only one
 * member function to reset the whole setup after an event is analysed.
 *
 * @memo 3 spectrometer (VDC) setup
 */
class VdcSetup {
  VdcPacket *speca, *specb, *specc;
public:
  /**Constructor. If you don't have three packets
   * (spectrometers) in use, use the {\it NULL} pointer.
   */
  VdcSetup(VdcPacket *a, VdcPacket *b, VdcPacket *c);

  /**Reset the internal memory after the calculation.
   * It is very {\bf important} to call this function
   * at the end of your event loop. This method calls
   * the reset function for each of the three packets.
   */
  void Reset(void);

  /**Destructor. Cleans up the internal memory.
   */
  ~VdcSetup(void);
};

// ################ VdcPlane ################

const short PLANE_STATUS_X	= 0x01;
const short PLANE_STATUS_dX	= 0x02;
const short PLANE_STATUS_SLOPE	= 0x04;
const short PLANE_STATUS_dSLOPE	= 0x08;
const short PLANE_STATUS_CHISQR	= 0x10;
const short PLANE_STATUS_ALL	= 0x1F;

inline double
VdcPlane::x()
{
  if (!(status & PLANE_STATUS_X)) CalcSums();
  return x_;
}

inline double
VdcPlane::slope()
{
  if (!(status & PLANE_STATUS_SLOPE)) CalcSums();
  return slope_;
}

inline double
VdcPlane::chi_sqr()
{
  if (!(status & PLANE_STATUS_CHISQR)) CalcSums();
  return chisqr;
}

inline double
VdcPlane::sigma_x()
{
  if (!(status & PLANE_STATUS_dX)) CalcSums();
  return sx;
}

inline double
VdcPlane::sigma_slope()
{
  if (!(status & PLANE_STATUS_dSLOPE)) CalcSums();
  return sslope;
}

inline void
VdcPlane::setTemporaryOffset(short toffs)
{
  tmpOffset = toffs;
}

// ################ VdcPacket ################

const int PACKET_STATUS_X	= 0x0001;
const int PACKET_STATUS_Y	= 0x0002;
const int PACKET_STATUS_THETA	= 0x0004;
const int PACKET_STATUS_PHI	= 0x0008;
const int PACKET_STATUS_dX	= 0x0010;
const int PACKET_STATUS_dY	= 0x0020;
const int PACKET_STATUS_dTHETA	= 0x0040;
const int PACKET_STATUS_dPHI	= 0x0080;
const int PACKET_STATUS_SLOPE_X	= 0x0100;
const int PACKET_STATUS_SLOPE_S	= 0x0200;
const int PACKET_STATUS_S	= 0x0400;
const int PACKET_STATUS_THETA_S	= 0x0800;

inline int
VdcPacket::ok()
{
  return ((x1 && x1->GoodWires() > 1) ? PACKET_STATUS_X1 : 0) +
         ((s1 && s1->GoodWires() > 1) ? PACKET_STATUS_S1 : 0) +
	 ((x2 && x2->GoodWires() > 1) ? PACKET_STATUS_X2 : 0) +
         ((s2 && s2->GoodWires() > 1) ? PACKET_STATUS_S2 : 0);
}

inline int
VdcPacket::ok3()
{
  return ((x1 && x1->GoodWires() > 2) ? PACKET_STATUS_X1 : 0) +
         ((s1 && s1->GoodWires() > 2) ? PACKET_STATUS_S1 : 0) +
	 ((x2 && x2->GoodWires() > 2) ? PACKET_STATUS_X2 : 0) +
         ((s2 && s2->GoodWires() > 2) ? PACKET_STATUS_S2 : 0);
}

inline int
VdcPacket::ok1()
{
  return ((x1->GoodWires() + x2->GoodWires() > 2) ?
	  PACKET_STATUS_BOTH_X : 0) +
         ((s1->GoodWires() + s2->GoodWires() > 2) ?
	  PACKET_STATUS_BOTH_S : 0);
}

inline int
VdcPacket::errorCode()
{
  return x1->errorCode()|s1->errorCode()|x2->errorCode()|s2->errorCode();
}

inline int
VdcPacket::errorCode(int plane)
{
  int err = ~ErrorCode0;
  switch (plane) {
  case 0:	err = x1->errorCode(); break;
  case 1:	err = s1->errorCode(); break;
  case 2:	err = x2->errorCode(); break;
  case 3:	err = s2->errorCode(); break;
  }
  return err;
}

inline double
VdcPacket::x()
{
  if (!(status & PACKET_STATUS_X)) CalcCoord(PACKET_STATUS_X);
  return x_;
}

inline double
VdcPacket::dx()
{
  if (!(status & PACKET_STATUS_dX)) CalcCoord(PACKET_STATUS_dX);
  return dx_;
}

inline double
VdcPacket::y()
{
  if (!(status & PACKET_STATUS_Y)) CalcCoord(PACKET_STATUS_Y);
  return y_;
}

inline double
VdcPacket::dy()
{
  if (!(status & PACKET_STATUS_dY)) CalcCoord(PACKET_STATUS_dY);
  return dy_;
}

inline double
VdcPacket::theta()
{
  if (!(status & PACKET_STATUS_THETA)) CalcCoord(PACKET_STATUS_THETA);
  return theta_;
}

inline double
VdcPacket::dtheta()
{
  if (!(status & PACKET_STATUS_dTHETA)) CalcCoord(PACKET_STATUS_dTHETA);
  return dtheta_;
}

inline double
VdcPacket::phi()
{
  if (!(status & PACKET_STATUS_PHI)) CalcCoord(PACKET_STATUS_PHI);
  return phi_;
}

inline double
VdcPacket::dphi()
{
  if (!(status & PACKET_STATUS_dPHI)) CalcCoord(PACKET_STATUS_dPHI);
  return dphi_;
}

inline double
VdcPacket::s()
{
  if (!(status & PACKET_STATUS_S)) CalcCoord(PACKET_STATUS_S);
  return s_;
}

inline double
VdcPacket::theta_s()
{
  if (!(status & PACKET_STATUS_THETA_S)) CalcCoord(PACKET_STATUS_THETA_S);
  return theta_s_;
}

inline void
VdcPacket::InitOffset(short *x1offs, short *s1offs,
		      short *x2offs, short *s2offs)
{
    if (x1) x1->InitOffset(x1offs);
    if (s1) s1->InitOffset(s1offs);
    if (x2) x2->InitOffset(x2offs);
    if (s2) s2->InitOffset(s2offs);
}

inline void
VdcPacket::InitOffset(short x1offs, short s1offs,
		      short x2offs, short s2offs)
{
    if (x1) x1->InitOffset(x1offs);
    if (s1) s1->InitOffset(s1offs);
    if (x2) x2->InitOffset(x2offs);
    if (s2) s2->InitOffset(s2offs);
}

inline void VdcPacket::InitTimeToLength(double ttl)
{
    InitTimeToLength(ttl, ttl, ttl, ttl);
}

inline void VdcPacket::InitTimeToLength(double ttlx1, double ttls1,
					double ttlx2, double ttls2)
{
    if (x1) x1->InitTimeToLength(ttlx1);
    if (s1) s1->InitTimeToLength(ttls1);
    if (x2) x2->InitTimeToLength(ttlx2);
    if (s2) s2->InitTimeToLength(ttls2);
}

inline void
VdcPacket::InitIgnoreCriteria(int xmode, float xmin, float xmax, float xslope,
			      int smode, float smin, float smax, float sslope)
{
    if (x1) x1->InitIgnoreCriteria(xmode, xmin, xmax, xslope);
    if (s1) s1->InitIgnoreCriteria(smode, smin, smax, sslope);
    if (x2) x2->InitIgnoreCriteria(xmode, xmin, xmax, xslope);
    if (s2) s2->InitIgnoreCriteria(smode, smin, smax, sslope);
}

inline void
VdcPacket::InitFullRange(short fr)
{
    if (x1) x1->InitFullRange(fr);
    if (s1) s1->InitFullRange(fr);
    if (x2) x2->InitFullRange(fr);
    if (s2) s2->InitFullRange(fr);
}

inline void
VdcPacket::InitMinimumDrift(float min)
{
    if (x1) x1->InitMinimumDrift(min);
    if (s1) s1->InitMinimumDrift(min);
    if (x2) x2->InitMinimumDrift(min);
    if (s2) s2->InitMinimumDrift(min);
}

inline void
VdcPacket::setTemporaryOffset(short toffs)
{
    if (x1) x1->setTemporaryOffset(toffs);
    if (s1) s1->setTemporaryOffset(toffs);
    if (x2) x2->setTemporaryOffset(toffs);
    if (s2) s2->setTemporaryOffset(toffs);
}

inline void
VdcPacket::DoTheLinearRegressionAgainWithCorrectOffset(double offs)
{
  static chRawData chtemp[4][15]; // Fuck events with more than 15 wires/plane
  int i;
  int lx1=x1->NumberOfWires(); if (lx1>15) lx1=15;
  int ls1=s1->NumberOfWires(); if (ls1>15) ls1=15;
  int lx2=x2->NumberOfWires(); if (lx2>15) lx2=15;
  int ls2=s2->NumberOfWires(); if (ls2>15) ls2=15;
  for (i=0; i < lx1; i++){
    chtemp[0][i].wire = x1->WiresFired()[i]+1;
    chtemp[0][i].time = x1->RawDriftTimes()[i] + (short)offs;
  }
  for (i=0; i < ls1; i++){
    chtemp[1][i].wire = s1->WiresFired()[i]+1;
    chtemp[1][i].time = s1->RawDriftTimes()[i] + (short)offs;
  }
  for (i=0; i < lx2; i++){
    chtemp[2][i].wire = x2->WiresFired()[i]+1;
    chtemp[2][i].time = x2->RawDriftTimes()[i] + (short)offs;
  }
  for (i=0; i < ls2; i++){
    chtemp[3][i].wire = s2->WiresFired()[i]+1;
    chtemp[3][i].time = s2->RawDriftTimes()[i] + (short)offs;
  }
  Reset();
  x1->FillTime(chtemp[0], 2*lx1);
  s1->FillTime(chtemp[1], 2*ls1);
  x2->FillTime(chtemp[2], 2*lx2);
  s2->FillTime(chtemp[3], 2*ls2);
}

inline void 
VdcPacket::DoTheLinearRegressionAgainWithX1S1Only(){
  static chRawData chtemp[2][15];  
  int i;
  int lx1=x1->NumberOfWires(); if (lx1>15) lx1=15;
  int ls1=s1->NumberOfWires(); if (ls1>15) ls1=15;
  for (i=0; i < lx1; i++){
    chtemp[0][i].wire = x1->WiresFired()[i]+1;
    chtemp[0][i].time = x1->RawDriftTimes()[i];
  }
  for (i=0; i < ls1; i++){
    chtemp[1][i].wire = s1->WiresFired()[i]+1;
    chtemp[1][i].time = s1->RawDriftTimes()[i];
  }
  Reset();
  x1->FillTime(chtemp[0], 2*lx1);
  s1->FillTime(chtemp[1], 2*ls1);
}

inline void 
VdcPacket::DoTheLinearRegressionAgainWithX2S2Only(){
  static chRawData chtemp[2][15];  
  int i;
  int lx2=x2->NumberOfWires(); if (lx2>15) lx2=15;
  int ls2=s2->NumberOfWires(); if (ls2>15) ls2=15;
  for (i=0; i < lx2; i++){
    chtemp[0][i].wire = x2->WiresFired()[i]+1;
    chtemp[0][i].time = x2->RawDriftTimes()[i];
  }
  for (i=0; i < ls2; i++){
    chtemp[1][i].wire = s2->WiresFired()[i]+1;
    chtemp[1][i].time = s2->RawDriftTimes()[i];
  }
  Reset();
  x2->FillTime(chtemp[0], 2*lx2);
  s2->FillTime(chtemp[1], 2*ls2);
}

//@}

#endif
