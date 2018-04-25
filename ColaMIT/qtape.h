//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-456      tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaMIT/qtape.h,v 2.1 2002-03-05 03:49:54 distler Exp $
//
// Classes to read Q tape (or any files that contain Bates Data)
//

#ifndef __QTAPE_H__
#define __QTAPE_H__

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <time.h>

typedef struct Qheader {
  unsigned short recordNumber;
  unsigned short date;
  unsigned char  hours;
  unsigned char  minutes;
  unsigned char  seconds;
  unsigned char  ticks;
  unsigned char  recordType;
  unsigned char  formatID;
  unsigned short runNumber;
  unsigned short number1;
  unsigned short number2;
  unsigned short number3;
} Qhead, *Qptr;

typedef struct Dheader {
  unsigned short wordCount;
  unsigned char  eventNumber;
  unsigned char  statusFlag;
} Dhead, *Dptr;

const int maxBuffer = 65554;


inline unsigned short
swapByte(unsigned short us) {
#if defined(__sparc)
  return (us >> 8) | (us << 8);
#else
  return us;
#endif
}


//
// Class name  : Qevent
//
// Description : 
//

class Qevent {
protected:
  int buflen;
  unsigned short * buffer;
  unsigned short * bufend;
  time_t dateTime;
public:
  Qevent(unsigned short * buf, int len);
  virtual ~Qevent() { ; }
  virtual int print(void);
  virtual int handle(void);
  void setDateTime(time_t dt) { dateTime = dt; }
  int isValid(unsigned short * addr) { return (addr < bufend); }
}; // end of class Qevent


//
// Class name  : QrecordBase
//
// Description : handle record data
//

class QrecordBase {
protected:
  int buflen;
  unsigned short * buffer;
public:
  QrecordBase(unsigned short * buf, int len) { buffer = buf; buflen = len; }
  virtual ~QrecordBase() { ; }
  time_t dateTime(void);
  virtual int print(unsigned long offs) = 0;
  virtual int handle(void) = 0;
  virtual Qevent * newEvent(unsigned short * data, int words) = 0;
  virtual unsigned short runNumber() {
    return swapByte(((Qptr)buffer-1)->runNumber);
  }
}; // end of class QrecordBase


//
// Class name  : Qrecord
//
// Description : handle record data
//               supplies default print function
//

class Qrecord : public QrecordBase {
public:
  Qrecord(unsigned short * buf, int len) : QrecordBase(buf, len) { ; }
  virtual ~Qrecord() { ; }
  virtual int print(unsigned long offs);
  virtual int handle(void) { return 0; }
  virtual Qevent * newEvent(unsigned short * data, int words)
  { if (data && words); return NULL; }
}; // end of class QrecordBase


//
// Class name  : QbeginningOfTapeRecord
//
// Description : handle beginningOfTape record
//

class QbeginningOfTapeRecord : public Qrecord {
public:
  QbeginningOfTapeRecord(unsigned short * buf, int len)
    : Qrecord(buf, len) { ; }
  virtual ~QbeginningOfTapeRecord() { ; }
  virtual int print(unsigned long offs);
}; // end of class QbeginningOfTapeRecord


//
// Class name  : QdiskFileErrorRecord
//
// Description : handle diskFileError record
//

class QdiskFileErrorRecord : public Qrecord {
public:
  QdiskFileErrorRecord(unsigned short * buf, int len)
    : Qrecord(buf, len) { ; }
  virtual ~QdiskFileErrorRecord() { ; }
  virtual int print(unsigned long offs);
}; // end of class QdiskFileErrorRecord


//
// Class name  : QbeginningOfRunRecord
//
// Description : handle beginningOfRun record
//

class QbeginningOfRunRecord : public Qrecord {
protected:
  char * str;
public:
  QbeginningOfRunRecord(unsigned short * buf, int len);
  virtual ~QbeginningOfRunRecord() { if (str) delete str; }
  virtual int print(unsigned long offs);
}; // end of class QbeginningOfRunRecord


//
// Class name  : QdataRecord
//
// Description : handle data record
//

class QdataRecord : public QrecordBase {
public:
  QdataRecord(unsigned short * buf, int len) : QrecordBase(buf, len) { ; }
  virtual ~QdataRecord() { ; }
  virtual int print(unsigned long offs);
  virtual int handle(void);
  virtual Qevent * newEvent(unsigned short * data, int words);
}; // end of class QdataRecord


//
// Class name  : QcommentRecord
//
// Description : handle comment record
//

class QcommentRecord : public Qrecord {
protected:
  char * str;
public:
  QcommentRecord(unsigned short * buf, int len);
  virtual ~QcommentRecord() { if (str) delete str; }
  virtual int print(unsigned long offs);
}; // end of class QcommentRecord


//
// Class name  : QendOfRunRecord
//
// Description : handle endOfRun record
//

class QendOfRunRecord : public Qrecord {
public:
  QendOfRunRecord(unsigned short * buf, int len) : Qrecord(buf, len) { ; }
  virtual ~QendOfRunRecord() { ; }
  virtual int print(unsigned long offs);
}; // end of class QendOfRunRecord


struct pipes {
    int parent_in;
    int child_out;
    int child_in;
    int parent_out;
};


//
// Class name  : QtapeBase
//
// Description : read file handle
//		 print structure of Q data
//

class QtapeBase {
  struct pipes pipes;
  unsigned char * initbuf;
  unsigned char * initptr;
  int initlen;
  int pid;
protected:
  int ifd;		// input  file descriptor
  int ofd;		// output file descriptor
  unsigned char * buffer;
  unsigned char * bufptr;
  unsigned long totallen;
  unsigned long totaltmp;
  int read(int count);
  int write(int count);
  int realignHeader(int count);
public:
  QtapeBase(int fdesc);
  virtual ~QtapeBase();
  enum RecordType { None=0, BoT=1, BoR=2, Data=3,
		    EoR=4, EoT=5, Comment=6, Error=7, EoF=8 };
}; // end of class QtapeBase


//
// Class name  : Qtape
//
// Description : read file handle
//		 print structure of Q data
//

class Qtape : public QtapeBase {
protected:
  QrecordBase * record;
  int runNumber;
  int recNumber;
  int dataCorrupt;
public:
  /// Constructor. Initialize object.
  Qtape(int ifd);

  /// Destructor. Free memory.
  virtual ~Qtape();

  /// Get next data record. Read a full data record from file.
  RecordType nextRecord(void);

  ///
  virtual int print(void);

  ///
  virtual int handle(void);

  /**Create a new record object.
   * The data header information is evaluated and the remaining
   * data is passed to the new object.
   */
  virtual QrecordBase * newRecord(RecordType rTyp,
				  unsigned short *data, int words);

  ///
  virtual QrecordBase * getRecord(void) { return record; }

  ///
  virtual int putRecord(int fdesc);
}; // end of class Qtape


#endif /* __QTAPE_H__ */
