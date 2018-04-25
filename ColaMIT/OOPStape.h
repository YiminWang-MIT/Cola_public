//                                                                    -*-c++-*-
// the guys to blame:
//
// Michael O. Distler                        mailto:distler@kph.uni-mainz.de
// Insitut fuer Kernphysik, Univ. Mainz      +49 6131 39 22956
// Mainz, FRG                                +49 6131 39 22964
//
// Itaru Nakagawa                            mailto:itaru@mitlns.mit.edu
// Simon Sirca                               mailto:sirca@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-402      tel.   (617) 253-3051 (Itaru)
// Cambridge, MA 02139                       tel.   (617) 258-5438 (Simon)
//                                           fax.   (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaMIT/OOPStape.h,v 2.1 2002-03-05 03:49:49 distler Exp $
//
// Classes to read Q tape (with OOPS Data)
//

#ifndef __OOPSTAPE_H__
#define __OOPSTAPE_H__

#include "Cindy/Cindy++.h"
#include "qtape.h"
#include "oops1.h"

// constants for use with CindyTree::putIdent(time_t, int)
const int putIdentOnly             = 0;
const int putConfigurationAndIdent = 1;

// constants for use with OOPStape::OOPStape(CindyTree *, int, int)
const int donotCopyRun             = 0;
const int copyRunUncompressed      = 1;
const int copyRunCompressed        = 2;
const int linkRunUncompressed      = 3;
const int linkRunCompressed        = 4;

/** class handles event 3.
 *  This event indicates that the scalers are cleared.
 *  no action necessary.
 */
class OOPSevent3 : public Qevent {
protected:
  CindyTree * ct;
  static struct event3 & data;
public:
  ///
  OOPSevent3(CindyTree * ctree, unsigned short * buf, int len)
    : Qevent(buf, len) { ct = ctree; }

  ///
  virtual ~OOPSevent3() { ; }

  ///
  virtual int handle(void);
}; // end of class OOPSevent3


/** class handles event 4.
 *  This event type has been used during the OHIPS commissioning.
 */
class OOPSevent4 : public Qevent {
protected:
  CindyTree * ct;
  static struct event5 & data;
public:
  ///
  OOPSevent4(CindyTree * ctree, unsigned short * buf, int len)
    : Qevent(buf, len) { ct = ctree; }

  ///
  virtual ~OOPSevent4() { ; }

  ///
  virtual int handle(void);
}; // end of class OOPSevent4


/** class handles event 5.
 *  Before the year 2000 runs, this used to be the scaler event.
 */
class OOPSevent5 : public Qevent {
protected:
  CindyTree * ct;
  static struct event5 & data;
public:
  ///
  OOPSevent5(CindyTree * ctree, unsigned short * buf, int len)
    : Qevent(buf, len) { ct = ctree; }

  ///
  virtual ~OOPSevent5() { ; }

  ///
  virtual int handle(void);
}; // end of class OOPSevent5


/** class handles event 6.
 *  Scaler event for runs later than May 2000.
 */
class OOPSevent6 : public Qevent {
protected:
  CindyTree * ct;
  static struct event6 & data;
public:
  ///
  OOPSevent6(CindyTree * ctree, unsigned short * buf, int len)
    : Qevent(buf, len) { ct = ctree; }

  ///
  virtual ~OOPSevent6() { ; }

  ///
  virtual int handle(void);
}; // end of class OOPSevent6


/** class handles event 7.
 */
class OOPSevent7 : public Qevent {
protected:
  CindyTree * ct;
  static struct event8 & data;
public:
  ///
  OOPSevent7(CindyTree * ctree, unsigned short * buf, int len)
    : Qevent(buf, len) { ct = ctree; }

  ///
  virtual ~OOPSevent7() { ; }

  ///
  virtual int handle(void);
}; // end of class OOPSevent7


/** class handles event 8.
 */
class OOPSevent8 : public Qevent {
  void packRCOIN(unsigned short ** usp);
  void packREADOOPS(unsigned short ** usp, int endFlag);
  void packREADMISC(unsigned short ** usp);
  void packREADO(unsigned short ** usp);
  void packREADTestBPM(unsigned short ** usp);
  void packREADBEAM(unsigned short ** usp);
protected:
  CindyTree * ct;
  static struct event8 & data;
public:
  ///
  OOPSevent8(CindyTree * ctree, unsigned short * buf, int len)
    : Qevent(buf, len) { ct = ctree; }

  ///
  virtual ~OOPSevent8() { ; }

  ///
  virtual int handle(void);
}; // end of class OOPSevent8


/** class handles event 10.
 */
class OOPSevent10 : public Qevent {
protected:
  CindyTree * ct;
  int doPackData;
  static struct event10 & data;
public:
  ///
  OOPSevent10(CindyTree * ctree, unsigned short * buf, int len, int doPack=1)
    : Qevent(buf, len) { ct = ctree; doPackData = doPack; }

  ///
  virtual ~OOPSevent10() { ; }

  ///
  virtual int handle(void);
}; // end of class OOPSevent10


/** class handles event 13.
 */
class OOPSevent13 : public Qevent {
protected:
  CindyTree * ct;
  static struct event13 & data;
public:
  OOPSevent13(CindyTree * ctree, unsigned short * buf, int len)
    : Qevent(buf, len) { ct = ctree; }
  virtual ~OOPSevent13() { ; }
  virtual int handle(void);
}; // end of class OOPSevent13


/** class handles moller event 5.
 */
class MOLLERevent5 : public Qevent {
protected:
  CindyTree * ct;
  static struct moller5 & data;
public:
  ///
  MOLLERevent5(CindyTree * ctree, unsigned short * buf, int len)
    : Qevent(buf, len) { ct = ctree; }

  ///
  virtual ~MOLLERevent5() { ; }

  ///
  virtual int handle(void);
}; // end of class MOLLERevent5


/** class handles moller event 16.
 */
class MOLLERevent16 : public Qevent {
protected:
  CindyTree * ct;
  static struct moller16 & data;
public:
  ///
  MOLLERevent16(CindyTree * ctree, unsigned short * buf, int len)
    : Qevent(buf, len) { ct = ctree; }

  ///
  virtual ~MOLLERevent16() { ; }

  ///
  virtual int handle(void);
}; // end of class MOLLERevent16


//
// Class name  : OOPSbeginningOfRunRecord
//
// Description : handle BeginningOfRun record
//               write configuration event
//

class OOPSbeginningOfRunRecord : public QbeginningOfRunRecord {
protected:
  CindyTree * ct;
public:
  OOPSbeginningOfRunRecord(CindyTree * ctree, unsigned short * buf, int len)
    : QbeginningOfRunRecord(buf, len) { ct = ctree; }
  virtual ~OOPSbeginningOfRunRecord() { ; }
  virtual int handle(void);
}; // end of class OOPSbeginningOfRunRecord


//
// Class name  : OOPSdataRecord
//
// Description : handle data record
//

class OOPSdataRecord : public QdataRecord {
protected:
  static int isMollerRun;
  static int saveLen;
  static unsigned short * saveData;
  CindyTree * ct;
  time_t dataTaking;
  unsigned int mask;
public:
  OOPSdataRecord(CindyTree * ctree, unsigned short * buf, int len,
		 unsigned int msk=0xffff) : QdataRecord(buf, len)
  { ct = ctree; dataTaking = dateTime(); mask = msk; }
  virtual ~OOPSdataRecord() { ; }
  virtual int handle(void);
  virtual Qevent * newEvent(unsigned short * data, int words);
  static int concatBeamEvents;
}; // end of class OOPSdataRecord


//
// Class name  : OOPScommentRecord
//
// Description : handle comment record
//

class OOPScommentRecord : public QcommentRecord {
protected:
  CindyTree * ct;
public:
  OOPScommentRecord(CindyTree * ctree, unsigned short * buf, int len)
    : QcommentRecord(buf, len) { ct = ctree; }
  virtual ~OOPScommentRecord() { ; }
  virtual int handle(void);
}; // end of class OOPScommentRecord


//
// Class name  : OOPSendOfRunRecord
//
// Description : handle endOfRun record
//

class OOPSendOfRunRecord : public QendOfRunRecord {
protected:
  CindyTree * ct;
public:
  OOPSendOfRunRecord(CindyTree * ctree, unsigned short * buf, int len)
    : QendOfRunRecord(buf, len) { ct = ctree; }
  virtual ~OOPSendOfRunRecord() { ; }
  virtual int handle(void);
}; // end of class OOPSendOfRunRecord


//
// Class name  : OOPStape
//
// Description : read file handle
//		 print structure of Q data
//

class OOPStape : public Qtape {
  int  copyRun;
  int  childPID;
  char runname[32];
  time_t dataTaking;
  unsigned int mask;
protected:
  CindyTree * ct;
  int forkGzip(const char * filename);
public:
  OOPStape(CindyTree * ctree, int ifd, int copyrun=donotCopyRun);
  virtual ~OOPStape() { ; }
  virtual QrecordBase * newRecord(RecordType rTyp,
				  unsigned short *data, int words);
  int setMask(unsigned int newMask);
}; // end of class OOPStape


// little endian to host byte order
inline unsigned short ltohs(unsigned short *buf)
{
  unsigned char *uch = (unsigned char *)buf;
  return (unsigned short)uch[0]+((unsigned short)uch[1]<<8);
}

// little endian to host byte order
inline unsigned int ltohl(unsigned short *buf)
{
  unsigned char *uch = (unsigned char *)buf;
  return (unsigned short)uch[0]+((unsigned short)uch[1]<<8)
    +((unsigned short)uch[2]<<16)+((unsigned short)uch[3]<<24);
}

inline void
packData(CindyTree * ct, unsigned short **src,
	 short * dest, unsigned short mask=0xffff)
{
  if (mask) {
    *dest = (short)(ltohs(*src) & mask);
    if (*dest) ct->packEventData(dest);
  } else {
    *dest = (short)ltohs(*src);
    ct->packEventData(dest);
  }
  (*src)++;
}

inline void
packData(CindyTree * ct, unsigned short **src,
	 unsigned short * dest, unsigned short mask=0xffff)
{
  if (mask) {
    *dest = ltohs(*src) & mask;
    if (*dest) ct->packEventData(dest);
  } else {
    *dest = ltohs(*src);
    ct->packEventData(dest);
  }
  (*src)++;
}

inline void
packData(CindyTree * ct, unsigned short **src,
	 unsigned int * dest, unsigned int mask=0xffffffff)
{
  *dest = ltohl(*src);
  if (mask) {
    *dest &= mask;
    if (*dest) ct->packEventData(dest);
  } else {
    ct->packEventData(dest);
  }
  *src += 2;
}

#endif /* __OOPSTAPE_H__ */
