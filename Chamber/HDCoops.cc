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
// $Header: /tmp/cvsroot/Cola/Chamber/HDCoops.cc,v 2.11 2001-03-03 00:05:25 cola Exp $
//
// Classes to analyse OOPS horizontal driftchambers (HDCs)
//

static char rcsid[] =
"@(#)$Id: HDCoops.cc 2216 2008-06-13 21:13:47Z distler $";

#include "Cindy/EventDispatch.h"
#include "HDCoops.h"
#include "vdcRegression.h"
#include <stdio.h>

inline double square(double x) { return x*x; }


//
// Function name : scheduleHdc(unsigned short *data, int len, char *ptr)
//
// Description   : callback function used to pass
//                 hdc data to the plane object
//

int
scheduleHdc(unsigned short *data, int len, char *ptr)
{
  return ((HdcPlane *) ptr)->FillData(data, len);
} // end of scheduleHdc(unsigned short *, int, char *)



//
// Class name  : HdcPlane
//

HdcPlane::HdcPlane(EventDispatch *evd, const char *name)
{
  if (*rcsid == '@') MemInit(name); Reset();
  evd->addCallback(name, scheduleHdc, (char *)this);
} // end of HdcPlane::HdcPlane(EventDispatch *, const char *)


HdcPlane::HdcPlane(const char *name)
{
  MemInit(name);
  Reset();
} // end of HdcPlane::HdcPlane(const char *)


HdcPlane::~HdcPlane()
{
  ;
} // end of HdcPlane::~HdcPlane()


void
HdcPlane::MemInit(const char *name)
{
  // initialize the name
  Name = new char[strlen(name)+1];
  strcpy(Name, name);

  // initialize the constants to default values
  int i, entries = 2;
  oeSign=1;
  badplane = 0;
  float ftmp, *dtt = new float[entries];
  for (i=0, ftmp=0.0; i<entries; i++, ftmp += 1.0/(entries-1)) dtt[i] = ftmp;

  def.bintons[0]      =
  def.bintons[1]      =    0.21;
  def.wire0offset     =    0.0;
  def.delayLineUnit   =    0.28;
  def.driftTimeOffset =   80.0;
  def.adcLRinfo[0]    =  450.0;
  def.adcLRinfo[1]    =  700.0;
  def.adcLRinfo[2]    = 1200.0;
  def.adcLRinfo[3]    = 1500.0;
  def.adcOEflip       = 0;
  def.zPosition       =
  def.xOffset         =
  def.rotation        =    0.0;
  def.lowDrifttime    =   12.0;
  def.highDrifttime   =  221.0;
  def.entries         = entries;
  def.drifttimeTable  = dtt;

  sinRot = 0.0;
  cosRot = 1.0;

} // end of HdcPlane::MemInit(const char *)


void
HdcPlane::flipOESign(int doflip)
{
  oeSign = (doflip == 0) ? 1 : -1;
} // end of HdcPlane::flipOESign()


void
HdcPlane::Init(const char *initfile)
{
  hdcInit * init = new hdcInit(initfile);
  delete def.drifttimeTable;
  init->planeInit(&def);
  flipOESign(def.adcOEflip);
  sinRot = sin(def.rotation*0.001);
  cosRot = cos(def.rotation*0.001);
  delete init;
} // end of HdcPlane::Init(const char *)


void
HdcPlane::Reset()
{
  adc = right = left = -1;
  goodness = 0;
  nofit = 0;
  coord2Ch = 0;
  error = ErrorCodeNoADC + ErrorCodeNoTDCright + ErrorCodeNoTDCleft;
} // end of HdcPlane::Reset()


int
HdcPlane::FillData(unsigned short data[], int len)
{
  if (len == 3) {
    if ((data[0] == 1)&&(data[1] == 1)) {
      adc = data[2];
      error &= ~ErrorCodeNoADC;
    } else {
      adc = data[0];
      right = data[1];
      left  = data[2];
      error = ErrorCodeNoError;
    }
  } else {
    int i = 0;
    len /= 2;
    while (len-- > 0) {
      switch (data[i++]) {
      case 1:  adc   = data[i++]; error &= ~ErrorCodeNoADC;      break;
      case 2:  right = data[i++]; error &= ~ErrorCodeNoTDCright; break;
      case 3:  left  = data[i++]; error &= ~ErrorCodeNoTDCleft;  break;
      }
    }
  }

  // test on TDC overflow
  //  if (left > 2050) error |= ErrorCodeNoTDCleft; 
  //  if (right > 2050) error |= ErrorCodeNoTDCright; 
  if (left > 1025) error |= ErrorCodeNoTDCleft;
  if (right > 1025) error |= ErrorCodeNoTDCright; 

  // check Drift time range
  if(dtime()<def.lowDrifttime) goodness |= GoodnessDriftLow;  
  if(dtime()>def.highDrifttime) goodness |= GoodnessDriftHigh;


  //Do no fitting if TDC missing or drift out of range
  if(error >= ErrorCodeNoTDCright || goodness > GoodnessAdcHigh)
      setNoFit();

  return 0;
} // end of HdcPlane::FillData(unsigned short *, int)


float
HdcPlane::drift()
{ 
  float range, tab, res, slope;
  float drifttime = dtime();
  int n;

  // check range
  if(drifttime<def.lowDrifttime) {
    goodness |= GoodnessDriftLow;
    return 0;
  }
  if(drifttime>def.highDrifttime) {
    goodness |= GoodnessDriftHigh; 
    return oddEven();
  }

  // find table entry
  range=def.highDrifttime-def.lowDrifttime;
  tab=(drifttime-def.lowDrifttime)/range*def.entries;
  n=(int)floor(tab);

  // do linear fit between entries
  res=tab-n;
  slope=def.drifttimeTable[n+1]-def.drifttimeTable[n];

  return (def.drifttimeTable[n]+res*slope)*oddEven();
} // end of HdcPlane::drift()


int
HdcPlane::sign()
{
  if (wire()%2)
    return -oeSign;

  return oeSign;
} // end of HdcPlane::sign()


int
HdcPlane::oddEven()
{ 
  if ((def.adcLRinfo[0]<adc) && (adc<def.adcLRinfo[1])) return sign();

  if ((def.adcLRinfo[2]<adc) && (adc<def.adcLRinfo[3])) return -sign();

  // some code to deal with ambiguous o-e information
  if (adc<def.adcLRinfo[0]) {
    goodness |= GoodnessAdcLow;
    return sign();
  }

  if (def.adcLRinfo[3]<adc) {
    goodness |= GoodnessAdcHigh;
    return -sign();
  }

  float middle = (def.adcLRinfo[1] + def.adcLRinfo[2]) * 0.5;
  goodness |= GoodnessAdcBetween;

  if ( middle > adc ) return sign();

  return -sign();

} // end of HdcPlane::oddEven()


float 
HdcPlane::weight(int flip)
{
  float w = 0.5;

  if ((adc<def.adcLRinfo[0]) || (def.adcLRinfo[3]<adc)) w = 0.0;

  if ((def.adcLRinfo[1]<adc) && (adc<def.adcLRinfo[2])) {
    w = fabs(((def.adcLRinfo[1] + def.adcLRinfo[2]) * 0.5 - adc)/
	     (def.adcLRinfo[1] - def.adcLRinfo[2]));
  }

  // -mod- return 0.55+0.9*w*flip;
  return 0.5+0.99*w*flip;
} // end of HdcPlane::weight(int)


float
HdcPlane::diff_ns()
{
  return def.bintons[0]*right - def.bintons[1]*left;
} // end of HdcPlane::diff_ns()


float
HdcPlane::diff_wire()
{
  return diff_ns()*def.delayLineUnit+def.wire0offset;
} // end of HdcPlane::diff_wire()


int
HdcPlane::wire()
{
  return (int)floor(diff_wire()+0.5);
} // end of HdcPlane::wire()


float
HdcPlane::dtime()
{
  return def.bintons[0]*right + def.bintons[1]*left - def.driftTimeOffset;
} // end of HdcPlane::dtime()


int
HdcPlane::errorCode()
{
  return error;
} // end of HdcPlane::errorCode()


float
HdcPlane::coord(int flip)
{
  if(noFit()) {
    return coord2Ch;
  }
  else
    return wireSpacing*(wire()+drift()*flip*0.5)+offset();
} // end of HdcPlane::coord(int)

float
HdcPlane::realCoord(int flip)
{
  return wireSpacing*(wire()+drift()*flip*0.5)+offset();
} // end of HdcPlane::coord(int)


float
HdcPlane::z()
{
  return def.zPosition;
} // end of HdcPlane::z()


float
HdcPlane::rotation()
{
  return def.rotation*0.001;
} // end of HdcPlane::rotation()


float
HdcPlane::sinRotation()
{
  return sinRot;
} // end of HdcPlane::sinRotation()


float
HdcPlane::cosRotation()
{
  return cosRot;
} // end of HdcPlane::cosRotation()


float
HdcPlane::offset()
{
  return def.xOffset;
} // end of HdcPlane::offset()


int
HdcPlane::goodCheck()
{
  //careful make sure plane has been processed when calling goodCheck()
  return goodness;
} // end of HdcPlane::goodCheck()

int
HdcPlane::noFit()
{
  return nofit;
} // end of HdcPlane::noFit()

void
HdcPlane::setNoFit()
{
  nofit =  1;
} // end of HdcPlane::setNoFit()

int
HdcPlane::isBad()
{
  return badplane;
} // end of HdcPlane::isBad()

void
HdcPlane::markBad()
{
  badplane =  1;
} // end of HdcPlane::markBad()

void
HdcPlane::setCoord2Ch(float x)
{
  //  printf("setting plane to %f \n", x);
  coord2Ch =  x;
} // end of HdcPlane::setCoord2Ch()



//
// Class name  : HdcChamber
//

void
HdcChamber::meminit()
{
  x = y = NULL;
} // end of HdcChamber::meminit()


HdcChamber::HdcChamber(HdcPlane *X, HdcPlane *Y)
{
  meminit();
  x = X;
  y = Y;
} // end of HdcChamber::HdcChamber(HdcPlane *, HdcPlane *)


HdcChamber::HdcChamber(EventDispatch *evd, const char *path)
{
  meminit();

  char *ppath = new char[strlen(path)+3];
  strcpy(ppath, path);

  strcat(ppath, ".x");
  x = new HdcPlane(evd, ppath);

  ppath[strlen(ppath)-1] = 'y';
  y = new HdcPlane(evd, ppath);

  delete ppath;
} // end of HdcChamber::HdcChamber(EventDispatch *, const char *)


HdcChamber::~HdcChamber()
{
  ;
} // end of HdcChamber::~HdcChamber()


void
HdcChamber::Init(const char * fileX, const char * fileY)
{
  if (x) x->Init(fileX);
  if (y) y->Init(fileY);
} // end of HdcChamber::Init(const char *, const char *)


void
HdcChamber::Reset(void)  {
  if (x) x->Reset();
  if (y) y->Reset();
} // end of HdcChamber::Reset(void)


int
HdcChamber::getCoord(int pattern,
		     float& X, float& Xweight, float& Y, float& Yweight)
{
  float xtmp, ytmp;

  xtmp    = x->coord((pattern & 1)*2-1);
  Xweight = x->weight((pattern & 1)*2-1);
  ytmp    = y->coord((pattern & 2)-1);
  Yweight = y->weight((pattern & 2)-1);

  //  cout << "tmp:= " << xtmp << '\t' << Xweight << endl;

  // should be changed to small angle approx. at some point
  // both planes are rotated with the same angle

  X = xtmp * x->cosRotation() + ytmp * x->sinRotation();
  Y = ytmp * x->cosRotation() - xtmp * x->sinRotation();

  //  cout << "fin:= " << X << '\t' << Y << endl;

  return 0;
} // end of HdcChamber::getCoord(int, float&, float&, float&, float&)


HdcPlane *
HdcChamber::getX()
{
  return x;
} // end of HdcChamber::getX()


HdcPlane *
HdcChamber::getY()
{
  return y;
} // end of HdcChamber::getY()



//
// Class name  : HdcPacket
//

void
HdcPacket::meminit() {
  ch[0] = ch[1] = ch[2] = NULL;
  trackinfoX = new VdcRegression;
  trackinfoY = new VdcRegression;
  status = 0;
  Best = 0;
  pattern = 63; // take everything as is
  notrack=0;

} // end of HdcPacket::meminit()


HdcPacket::HdcPacket(EventDispatch *evd, const char *path)
{
  meminit();

  char *cpath = new char[strlen(path)+4];

  strcpy(cpath, path);
  strcat(cpath, "[0]");

  cpath[strlen(cpath)-2] = '1';
  ch[0] = new HdcChamber(evd, cpath);

  cpath[strlen(cpath)-2] = '2';
  ch[1] = new HdcChamber(evd, cpath);

  cpath[strlen(cpath)-2] = '3';
  ch[2] = new HdcChamber(evd, cpath);

  delete cpath;
} // end of HdcPacket::HdcPacket(EventDispatch *, const char *)


HdcPacket::HdcPacket(HdcChamber *ch1, HdcChamber *ch2, HdcChamber *ch3)
{
  meminit();
  ch[0] = ch1;
  ch[1] = ch2;
  ch[2] = ch3;
} // end of HdcPacket::HdcPacket(HdcChamber *, HdcChamber *, HdcChamber *)


HdcPacket::~HdcPacket()
{
  delete trackinfoX;
  delete trackinfoY;
} // end of HdcPacket::~HdcPacket()


HdcPlane *
HdcPacket::getXplane(int pl)
{
  return ch[pl-1]->getX();
} // end of HdcPacket::getXplane(int)


HdcPlane *
HdcPacket::getYplane(int pl)
{
  return ch[pl-1]->getY();
} // end of HdcPacket::getYplane(int)


int
HdcPacket::shuffle(void)
{
  double minChi2 = 1.0e+20;
  int minPattern = 63, fixpattern =0, nofitX = 0, nofitY = 0;
  int i, power = 1;

  //Test if Odd-Even ADC is in peak region (weight >0.99) 
  //then set fixpattern flag to exclude from shuffling
  // bit chamber
  //  1   X1
  //  2   Y1
  //  4   X2
  //  8   Y2
  // 16   X3
  // 32   Y3

  for(i=0; i<3; i++){
      if(ch[i]->getX()->noFit()){
	// if a rejected chamber is not marked as bad do not continue tracking
	if(!ch[i]->getX()->isBad()) nofitX = 3;
	nofitX++;
	fixpattern |= 21; // no X shuffling for only two chambers
      }
      
      if (ch[i]->getX()->weight(1) > 0.99) fixpattern |= power;
      power*=2;

      if(ch[i]->getY()->noFit()){
	// if a rejected chamber is not marked as bad do not continue tracking
	if(!ch[i]->getY()->isBad()) nofitY = 3;
	nofitY++;
	fixpattern |= 42; // no Y shuffling for only two chambers
      }

      if (ch[i]->getY()->weight(1) > 0.99) fixpattern |= power;
      power*=2;
  }


  //if one plane has only 1 good chamber exit and return error
  if(nofitX > 1 || nofitY > 1) {
    Best = 1;
    notrack = 1;
    return 1;
  }

  //Loop over all plausible combinations

  for (pattern=0; pattern<64; pattern++) {
    status=0;
    if ((pattern & fixpattern) == fixpattern) {
      if ( chisqrX()+chisqrY() < minChi2 ) {
	minChi2 = chisqrX()+chisqrY();
	minPattern = pattern;
      }
    }
  }

  Best++;
  pattern = minPattern;
  // pattern = 63; /* to switch of shuffling */
  FillTrack();

#ifdef NEVER
  char bitstr[8]; // make a friendly looking bitstring
  int i, bp;

  for (i=0, bp = pattern; i<6; i++, bp /= 2) bitstr[i] = bp&1 ? '+' : '-';
  bitstr[6] = 0; // chop off the tail

  cout << "Best fit X " << bitstr << " " << pattern
       << '\t' << chisqrX() << '\t' << x() << endl;

  cout << "Best fit Y " << bitstr << " " << pattern
       << '\t' << chisqrY() << '\t' << y() << endl;

  for(int i=0; i<3; i++) {
    cout << "O-E sign X" << ch[i]->getX()->oddEven() << '\t' 
         << ch[i]->getX()->goodCheck() << endl;
    cout << "O-E sign Y" << ch[i]->getY()->oddEven() << '\t' 
         << ch[i]->getY()->goodCheck() << endl;
  }
#endif

  return 0;   
} // end of HdcPacket::shuffle(void)


void
HdcPacket::FillTrack()
{ 
  float xrot, yrot, xweight, yweight;
  int i, signs;


  // If one chamber has a bad plane do 2 point fit to fill it first
  //but only once per shuffling 2 plane events are not shuffled
  for (i=0; i<3; i++){
    if((ch[i]->getX()->noFit() && !ch[i]->getX()->coord()) ||
	    (ch[i]->getY()->noFit() && !ch[i]->getY()->coord())) {
      twoChamberFit();
      break;
    }
  }


  status=0;
  trackinfoX->reset();
  trackinfoY->reset();


  for (i=0; i<3; i++) {
    if (ch[i]->getX() && ch[i]->getY()) {
      signs = pattern>>(i*2) & 3;
      ch[i]->getCoord(signs, xrot, xweight, yrot, yweight);
      //      printf("***Ch x %f  y %f fit %d %d\n", xrot, yrot, 
      //	     ch[i]->getX()->noFit(), ch[i]->getY()->noFit());
      trackinfoX->addw(ch[i]->getX()->z(), xrot, xweight );
      trackinfoY->addw(ch[i]->getY()->z(), yrot, yweight );
      status++;
    }
  }

  if (status > 2) {
    trackinfoX->calc();
    trackinfoY->calc();
  }

} // end of HdcPacket::FillTrack(void)



void
HdcPacket::twoChamberFit()
{ 
  float xrot, yrot, xweight, yweight;
  int i, signs;

  trackinfoX->reset();
  trackinfoY->reset();

  //  printf("Doing two point fit \n");
  // Do a 2 point fit
  for (i=0; i<3; i++) {
    signs = pattern>>(i*2) & 3;
    ch[i]->getCoord(signs, xrot, xweight, yrot, yweight);
    //    printf("2Ch x %f  y %f fit %d %d\n", xrot, yrot, 
    //	   ch[i]->getX()->noFit(),ch[i]->getY()->noFit() );
    if(!ch[i]->getX()->noFit()) 
      trackinfoX->addw(ch[i]->getX()->z(), xrot, 1 );
    if(!ch[i]->getY()->noFit()) 
      trackinfoY->addw(ch[i]->getY()->z(), yrot, 1 );
  }


    trackinfoX->calc();
    trackinfoY->calc();
//    trackinfoX->print("Regression X:");
//    trackinfoY->print("Regression Y:");

    // Fill result from 2 point fit into missing chamber
  for (i=0; i<3; i++) {
    if(ch[i]->getX()->noFit()){
      ch[i]->getX()->setCoord2Ch(trackinfoX->y0()+
				 trackinfoX->sl0()*ch[i]->getX()->z());
      //      printf("Set X plane %d to %f \n", i,trackinfoX->y0()+
      //	     trackinfoX->sl0()*ch[i]->getX()->z()); 
    }

    if(ch[i]->getY()->noFit()){
      ch[i]->getY()->setCoord2Ch(trackinfoY->y0()+
				 trackinfoY->sl0()*ch[i]->getY()->z());
      //      printf("Set X plane %d to %f \n", i,trackinfoY->y0()+
      //	     trackinfoY->sl0()*ch[i]->getY()->z()); 
    }
  }
}// end of HdcPacket::twoChamberFit(void)



int
HdcPacket::ok()
{
  int result = 0;

  result |= ch[0]->getX()->errorCode() ? 0 : 1;
  result |= ch[0]->getY()->errorCode() ? 0 : 2;
  result |= ch[1]->getX()->errorCode() ? 0 : 4;
  result |= ch[1]->getY()->errorCode() ? 0 : 8;
  result |= ch[2]->getX()->errorCode() ? 0 : 16;
  result |= ch[2]->getY()->errorCode() ? 0 : 32;

  // deal with 2 Plane fits
  // allow ignoring a plane only if marked as bad

  if(!notrack){
  if(ch[0]->getX()->isBad()) result |= ch[0]->getX()->noFit() ? 1 : 0;
  if(ch[0]->getY()->isBad()) result |= ch[0]->getY()->noFit() ? 2 : 0;
  if(ch[1]->getX()->isBad()) result |= ch[1]->getX()->noFit() ? 4 : 0;
  if(ch[1]->getY()->isBad()) result |= ch[1]->getY()->noFit() ? 8 : 0;
  if(ch[2]->getX()->isBad()) result |= ch[2]->getX()->noFit() ? 16 : 0;
  if(ch[2]->getY()->isBad()) result |= ch[2]->getY()->noFit() ? 32 : 0;
  }
  else
    result =0; // do not make a track

  return result;
} // end of HdcPacket::ok(void)


double
HdcPacket::x()
{
  if (!Best) shuffle();

  return trackinfoX->y0();
} // end of HdcPacket::x(void)


double
HdcPacket::y()
{
  if (!Best) shuffle();

  return trackinfoY->y0();
} // end of HdcPacket::y(void)


double
HdcPacket::slopeX()
{
  if (!Best) shuffle();

  return trackinfoX->sl0();
} // end of HdcPacket::slopeX(void)


double
HdcPacket::slopeY()
{
  if (!Best) shuffle();

  return trackinfoY->sl0();
} // end of HdcPacket::slopeY(void)


double
HdcPacket::theta()
{
  if (!Best) shuffle();

  return 1E3*atan(trackinfoX->sl0());
} // end of HdcPacket::theta(void)


double
HdcPacket::phi()
{
  if (!Best) shuffle();

  return 1E3*atan(trackinfoY->sl0());
} // end of HdcPacket::phi(void)


double
HdcPacket::dx()
{
  if (!Best) shuffle();

  return trackinfoX->dy0();
} // end of HdcPacket::dx(void)


double
HdcPacket::dy()
{
  if (!Best) shuffle();

  return trackinfoY->dy0();
} // end of HdcPacket::dy(void)


double
HdcPacket::dtheta()
{
  if (!Best) shuffle();

  return 1E3*trackinfoX->dsl0()/(1.0+square(trackinfoX->sl0()));
} // end of HdcPacket::dtheta(void)


double
HdcPacket::dphi()
{
  if (!Best) shuffle();

  return 1E3*trackinfoY->dsl0()/(1.0+square(trackinfoY->sl0()));
} // end of HdcPacket::dphi(void)


double
HdcPacket::chisqrX(void)
{
  if (notrack) return FLT_MAX;

  if (!status) FillTrack();
  return trackinfoX->chisqr();
} // end of HdcPacket::chisqrX(void)


double
HdcPacket::chisqrY(void)
{
  if (notrack) return FLT_MAX;

  if (!status) FillTrack();
  return trackinfoY->chisqr();
} // end of HdcPacket::chisqr(void)


int
HdcPacket::patSign(int bit)
{
  if (!Best) shuffle();

  return ((pattern>>bit) & 1)*2-1;

} // end of HdcPacket::patSign(int)


int
HdcPacket::bestPat()
{
  if (!Best) shuffle();

  return pattern;
} // end of HdcPacket::bestPat()

int
HdcPacket::noTrack()
{
  return notrack;
} // end of HdcPacket::noTrack()


float
HdcPacket::bestX(int pl)
{
  float xrot, xweight, yrot, yweight;
  int signs;

  if (!Best) shuffle();

  signs = pattern>>((pl-1)*2) & 3;
  ch[pl-1]->getCoord(signs, xrot, xweight, yrot, yweight);

  return xrot;
} // end of HdcPacket::bestX(int)


float
HdcPacket::bestY(int pl)
{
  float xrot, xweight, yrot, yweight;
  int signs;

  if (!Best) shuffle();

  signs = pattern>>((pl-1)*2) & 3;
  ch[pl-1]->getCoord(signs, xrot, xweight, yrot, yweight);

  return yrot;
} // end of HdcPacket::bestY(int)


void
HdcPacket::Reset()
{
  if (ch[0]) ch[0]->Reset();
  if (ch[1]) ch[1]->Reset();
  if (ch[2]) ch[2]->Reset();

  status = Best = notrack = 0;

  if (trackinfoX) trackinfoX->reset();
  if (trackinfoY) trackinfoY->reset();
} // end of HdcPacket::Reset(int)



//
// Class name  : HdcApacket
//

HdcApacket::HdcApacket(EventDispatch *evd, const char *path)
  : HdcPacket(evd, path)
{
  //    x->Init((float *)&HdcAplane1X, 1);
  //    x->Init((float *)&HdcAplane2X, 2);
  //    x->Init((float *)&HdcAplane3X, 3);
  //    y->Init((float *)&HdcAplane1Y, 1);
  //    y->Init((float *)&HdcAplane2Y, 2);
  //    y->Init((float *)&HdcAplane3Y, 3);
} // end of HdcApacket::HdcApacket(EventDispatch *, const char *)



//
// Class name  : HdcBpacket
//

HdcBpacket::HdcBpacket(EventDispatch *evd, const char *path)
  : HdcPacket(evd, path)
{
  //    x->Init((float *)&HdcBplane1X, 1);
  //    x->Init((float *)&HdcBplane2X, 2);
  //    x->Init((float *)&HdcBplane3X, 3);
  //    y->Init((float *)&HdcBplane1Y, 1);
  //    y->Init((float *)&HdcBplane2Y, 2);
  //    y->Init((float *)&HdcBplane3Y, 3);
} // end of HdcBpacket::HdcBpacket(EventDispatch *, const char *)



//
// Class name  : HdcCpacket
//

HdcCpacket::HdcCpacket(EventDispatch *evd, const char *path)
  : HdcPacket(evd, path)
{
  //    x->Init((float *)&HdcCplane1X, 1);
  //    x->Init((float *)&HdcCplane2X, 2);
  //    x->Init((float *)&HdcCplane3X, 3);
  //    y->Init((float *)&HdcCplane1Y, 1);
  //    y->Init((float *)&HdcCplane2Y, 2);
  //    y->Init((float *)&HdcCplane3Y, 3);
} // end of HdcCpacket::HdcCpacket(EventDispatch *, const char *)



//
// Class name  : HdcDpacket
//

HdcDpacket::HdcDpacket(EventDispatch *evd, const char *path)
  : HdcPacket(evd, path)
{
  //    x->Init((float *)&HdcDplane1X, 1);
  //    x->Init((float *)&HdcDplane2X, 2);
  //    x->Init((float *)&HdcDplane3X, 3);
  //    y->Init((float *)&HdcDplane1Y, 1);
  //    y->Init((float *)&HdcDplane2Y, 2);
  //    y->Init((float *)&HdcDplane3Y, 3);
} // end of HdcDpacket::HdcDpacket(EventDispatch *, const char *)
