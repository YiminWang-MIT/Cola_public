//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mit.edu
// MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaMIT/qtape.cc,v 2.1 2002-03-05 03:49:50 distler Exp $
//
// Classes to read Q tape (or any files that contain Bates Data)
//

#define _DEFAULT_SOURCE    1
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#include <unistd.h>
#include <errno.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#include "qtape.h"

const int initread = 32 * 1024;

//
// Class name  : QtapeBase
// Constructor : QtapeBase(int ifd)
//
// Description : initialize object
//		 allocate memory for buffer
// Input       : 
//

QtapeBase::QtapeBase(int fdesc)
{
  fd_set inSet;
  fd_set errSet;

  ifd = fdesc;
  ofd = -1;

  initbuf = initptr = new unsigned char[initread];
  initlen = 0;
  totallen = 0;
  totaltmp = 0;
  FD_ZERO(&inSet);
  FD_ZERO(&errSet);
  FD_SET(ifd, &inSet);
  FD_SET(ifd, &errSet);
  if (0 < select(ifd+1,(FDSETPTR)&inSet,NULL,(FDSETPTR)&errSet,NULL)) {
    if (FD_ISSET(ifd, &errSet) && !FD_ISSET(ifd, &inSet)) return;
    initlen = ::read(ifd, initbuf, initread);
  }

  if (initlen < 2) return;

  // gzip compression method OR standard unix compress
  if (((initbuf[0] == 0x1f) && (initbuf[1] == 0x8b)) ||
      ((initbuf[0] == 0x1f) && (initbuf[1] == 0x9d))) {

    if (pipe(&pipes.parent_in)) exit (errno);
    if (pipe(&pipes.child_in)) exit (errno);

    if ((pid = fork()) < 0) exit (errno);

    if (!pid) { /* child */

      close(pipes.parent_in);
      close(pipes.parent_out);

      if (0 != dup2(pipes.child_in, 0)) exit (errno);
      if (1 != dup2(pipes.child_out, 1)) exit (errno);

      char * args[3] = { GZIP, "-dq", NULL };
      if (execvp(args[0], args)) exit (errno);

    } else { /* parent */

      close(pipes.child_in);
      close(pipes.child_out);

      ::write(pipes.parent_out, initbuf, initlen);
      initlen = 0;
    }
  } else { // no compression
    pipes.parent_out = -1;
    pipes.parent_in = fdesc;
  }

  buffer = bufptr = new unsigned char[maxBuffer];  
} // end of QtapeBase::QtapeBase(int)


//
// Class name  : QtapeBase
// Destructor  : ~QtapeBase()
//
// Description : free allocated memory
//

QtapeBase::~QtapeBase()
{
  if (buffer) delete buffer;
} // end of QtapeBase::~QtapeBase()


//
// Class name  : QtapeBase
// Method name : realignHeader(int count)
//

int
QtapeBase::realignHeader(int count)
{
  if (ofd >= 0 && (count != write(count))) {
    close(ofd);
    ofd = -1;
  }

  memmove(buffer, buffer+count, (bufptr-buffer)-count);
  bufptr -= count;

  return read(count);
} // end of QtapeBase::realignHeader(int)


//
// Class name  : QtapeBase
// Method name : read(int count)
//
// Description : read data into buffer
// Input       : count = number of chars to read
// Output      : 
// Return      : number of chars read or -1 on error
//

int
QtapeBase::read(int count)
{
  int haveread = 0;

  while (count > 0) {
    int r = 0;
    if (initlen > 0) {
      r = (count < initlen) ? count : initlen;
      memcpy(bufptr, initptr, r);
      initlen -= r; initptr += r;
    } else {
      fd_set inSet;
      struct timeval timeout;

      if (pipes.parent_out < 0) { // no decompression

	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	FD_ZERO(&inSet);
	FD_SET(pipes.parent_in, &inSet);
	if (1 == select(pipes.parent_in+1,
			(FDSETPTR)&inSet,NULL,NULL,&timeout)) {
	  initlen = ::read(pipes.parent_in, initptr=initbuf, initread);
	  if (initlen < 0) return initlen;
	  if (initlen == 0) return haveread;
	}

      } else {

	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	FD_ZERO(&inSet);
	FD_SET(pipes.parent_in, &inSet);
	if (1 == select(pipes.parent_in+1,
			(FDSETPTR)&inSet,NULL,NULL,&timeout)) {
	  r = ::read(pipes.parent_in, bufptr, count);
	  if (r < 0) return r;
	  if (!r) break;
	}

	fd_set outSet;
	FD_ZERO(&inSet);
	FD_SET(ifd, &inSet);
	FD_ZERO(&outSet);
	FD_SET(pipes.parent_out, &outSet);
	if (2 == select((ifd > pipes.parent_out) ? ifd+1 : pipes.parent_out+1,
			(FDSETPTR)&inSet,(FDSETPTR)&outSet,NULL,&timeout)) {
	  initlen = ::read(ifd, initbuf, (initread>1024) ? 1024 : initread);
	  if (initlen >= 0) {
	    if (initlen == 0) {
	      close(pipes.parent_out);
	      pipes.parent_out = -1;
	    } else {
	      ::write(pipes.parent_out, initbuf, initlen);
	      initlen = 0;
	    }
	  }
	}
      }
    }
    bufptr += r;
    count -= r;
    haveread += r;
  }

  return haveread;
} // end of QtapeBase::read(int)


//
// Class name  : QtapeBase
// Method name : write(int count)
//
// Description : write data buffer to file
// Input       : count = number of chars to write
// Return      : number of chars written or -1 on error
//

int
QtapeBase::write(int count)
{
  int havewritten = 0;

  while (count > 0) {
    int w = ::write(ofd, buffer+havewritten, count);
    if (w < 0) return w;
    count -= w;
    havewritten += w;
  }

  return havewritten;
} // end of QtapeBase::write(int)


//
// Class name  : Qtape
//

Qtape::Qtape(int ifd) : QtapeBase(ifd)
{
  record = NULL;
  runNumber = recNumber = -1;
  dataCorrupt = 0;
  putenv("TZ=EST5EDT"); tzset(); // Timezone US Eastern
} // end of Qtape::Qtape(int)

Qtape::~Qtape()
{
  delete record;
} // end of Qtape::~Qtape()

QrecordBase *
Qtape::newRecord(RecordType rTyp, unsigned short *data, int words)
{
  QrecordBase * qrec;

  switch (rTyp) {
  case BoT:
    qrec = new QbeginningOfTapeRecord(data, words);
    break;
  case BoR:
    qrec = new QbeginningOfRunRecord(data, words);
    break;
  case Data:
    qrec = new QdataRecord(data, words);
    break;
  case Comment:
    qrec = new QcommentRecord(data, words);
    break;
  case EoR:
    qrec = new QendOfRunRecord(data, words);
    break;
  case Error:
    qrec = new QdiskFileErrorRecord(data, words);
    break;
  default:
    qrec = new Qrecord(data, words);
    break;
  }
    
  return qrec;
} // end of Qtape::newRecord(RecordType, unsigned short *, int)

int
Qtape::handle()
{
  return record ? record->handle() : 0;
} // end of Qtape::handle()

Qtape::RecordType
Qtape::nextRecord(void)
{
  RecordType result = None;
  int r;

  bufptr = buffer;

  if (record) {
    delete record;
    record = NULL;
  }

  if (sizeof(Qhead) != (r = read(sizeof(Qhead)))) {
    if (r < 0) {
      return None;
    } else {
      return EoF;
    }
  }

  if (dataCorrupt) return None;

  Qptr header = (Qptr)buffer;
  if ((runNumber == -1) && (recNumber == -1)) {
    // beginning of tape (run)
    runNumber = swapByte(header->runNumber);
    recNumber = swapByte(header->recordNumber);
  } else if ((runNumber == swapByte(header->runNumber)) &&
	     (((header->recordNumber == 0) && (recNumber == 0xffff)) ||
	      (recNumber == swapByte(header->recordNumber)-1))) {
    // this is what I expect
    recNumber = swapByte(header->recordNumber);
  } else if ((header->recordType == BoR) &&
	     (recNumber == swapByte(header->recordNumber)-1)) {
    // BoT header have wrong runNumber
    runNumber = swapByte(header->runNumber);
    recNumber = swapByte(header->recordNumber);
#define QRU_TWICE 0
#if QRU_TWICE
  } else if ((header->recordType == BoT) &&
	     (swapByte(header->recordNumber) == 1)) {
    // use this to concatenate 2 runs produced by typing qru twice
    runNumber = swapByte(header->runNumber);
    recNumber = swapByte(header->recordNumber);
#endif
  } else if ((runNumber == swapByte(header->number1)) &&
	     (recNumber == swapByte(header->date)-1)) {
    // this takes care of a Q bug:
    // comment records written to tape are sometimes one word to short
    // realign the header information
    fprintf(stderr, "********* realign Header *********\n");
    recNumber = swapByte(header->date);
    if (sizeof(short) == (r = realignHeader(sizeof(short)))) {
      totallen  = totaltmp;
      totaltmp += sizeof(short);
    } else if (r < 0) {
      return None;
    } else {
      return EoF;
    }
  } else {
    // this data is corrupt ( not Q format )
    fprintf(stderr, "********** corrupt data **********\n");
    if (ofd >= 0 && (sizeof(Qhead) != write(sizeof(Qhead)))) {
      close(ofd);
      ofd = -1;
    }
    totallen  = totaltmp;
    totaltmp += sizeof(Qhead);
    dataCorrupt++;
    return None;
  }

  int havetoread = 0;
  switch (header->recordType) {
  case BoT:
    result = BoT;
    break;
  case BoR:
    result = BoR;
    havetoread = 158;
    break;
  case Data:
    result = Data;
    havetoread = swapByte(header->number3);
    break;
  case EoR:
    result = EoR;
    havetoread = 122;
    break;
  case EoT:
    result = EoT;
    break;
  case Comment:
    result = Comment;
    // havetoread = (swapByte(header->number2)/2 - 1)*2;
    havetoread = swapByte(header->number2) - 2 - swapByte(header->number2)%2;
    break;
  case Error:
    result = Error;
    break;
  default:
    result = None;
    break;
  }

  if (havetoread != (r = read(havetoread))) {
    if (r < 0) {
      return None;
    } else {
      return EoF;
    }
  }

  if (ofd >= 0) {
    int havetowrite = sizeof(Qhead)+havetoread;
    if (havetowrite != write(havetowrite)) {
      close(ofd);
      ofd = -1;
    }
  }

  totallen  = totaltmp;
  totaltmp += sizeof(Qhead)+havetoread;

  record = newRecord(result, (unsigned short *)(header+1), havetoread/2);

  return result;
} // end of Qtape::nextRecord()


//
// Class name  : Qtape
// Method name : putRecord(int fdesc)
//
// Description : write this record to file and initialize ofd
// Input       : 
// Output      : 
// Return      : bytes written
//

int
Qtape::putRecord(int fdesc)
{
  ofd = fdesc;

  Qptr header = (Qptr)buffer;
  int written = 0;
  int havetowrite = sizeof(Qhead);
  switch (header->recordType) {
  case BoR:
    havetowrite += 158;
    break;
  case Data:
    havetowrite += swapByte(header->number3);
    break;
  case EoR:
    havetowrite += 122;
    break;
  case Comment:
    // havetowrite += ((swapByte(header->number2) - 3)/2)*2;
    havetowrite += swapByte(header->number2) - 2 - swapByte(header->number2)%2;
  default:
    break;
  }

  if ((ofd >= 0) && (havetowrite != (written = write(havetowrite)))) {
    close(ofd);
    ofd = -1;
  }

  return written;
} // end of Qtape::putRecord()


//
// Class name  : QrecordBase
// Method name : dateTime(void)
//
// Description : calculate date and time of data taking in unix format
// Input       : 
// Output      : 
// Return      : 
//

time_t
QrecordBase::dateTime(void)
{
  struct tm tms;
  memset(&tms, 0, sizeof(struct tm));

  Qptr header            = ((Qptr)buffer)-1;
  unsigned short hdrdate = swapByte(header->date);

  tms.tm_year  = (hdrdate & 0x007f);
  tms.tm_mon   = ((hdrdate & 0x0780) >> 7) - 1;
  tms.tm_mday  = (hdrdate & 0xf800) >> 11;
  tms.tm_hour  = header->hours;
  tms.tm_min   = header->minutes;
  tms.tm_sec   = header->seconds;
  tms.tm_isdst = -1;

  return mktime(&tms);
} // end of QrecordBase::dateTime(void)


//
// Class name  : Qrecord
// Method name : print(unsigned long offs)
//
// Description : print record information to stdout
// Input       : 
// Output      : 
// Return      : 
//

int
Qrecord::print(unsigned long offs)
{
  Qptr header = ((Qptr)buffer)-1;
  time_t t = dateTime();
  printf("run %5d record %5d : type %2d offset %9ld  %s",
	 swapByte(header->runNumber), swapByte(header->recordNumber),
	 header->recordType, offs, ctime(&t));

  return 0;
} // end of Qrecord::print(unsigned long)


//
// Class name  : Qtape
// Method name : print(void)
//
// Description : print record information to stdout
// Input       : 
// Output      : 
// Return      : 
//

int
Qtape::print(void)
{
  if (record) record->print(totallen);

  return 0;
} // end of Qtape::print(void)


//
// Class name  : QdataRecord
// Method name : newEvent(unsigned short *data, int words)
//
// Description : create a new event object
// Input       : data = pointer to data
// Output      : words = number of data words
// Return      : pointer to new Qevent object
//

Qevent *
QdataRecord::newEvent(unsigned short *data, int words)
{
  return new Qevent(data+2, words-2);
} // end of QdataRecord::newEvent()


//
// Class name  : QdataRecord
// Method name : print(unsigned long offs)
//
// Description : print data record statistics
//

int
QdataRecord::print(unsigned long offs)
{
  Qptr header = ((Qptr)buffer)-1;
  time_t t = dateTime();
  printf("run %5d record %5d : type %2d offset %9ld  %s",
	 swapByte(header->runNumber), swapByte(header->recordNumber),
	 header->recordType, offs, ctime(&t));

  unsigned short * bufptr = buffer;
  while (bufptr < (buffer+buflen)) {
    Dptr head = (Dptr) bufptr;
    if (swapByte(head->wordCount) == 0) break;
    Qevent * event = newEvent(bufptr, swapByte(head->wordCount));
    if (event) {
      event->print();
      delete event;
    }
    bufptr += swapByte(head->wordCount);
  }

  return 0;
} // end of QdataRecord::print(unsigned long)


//
// Class name  : QdataRecord
// Method name : handle(void)
//
// Description : handle dataRecord data
//

int
QdataRecord::handle()
{
  unsigned short * bufptr = buffer;

  while (bufptr < (buffer+buflen)) {
    Dptr head = (Dptr) bufptr;
    if (swapByte(head->wordCount) == 0) break;
    Qevent * event = newEvent(bufptr, swapByte(head->wordCount));
    if (event) {
      event->handle();
      delete event;
    }
    bufptr += swapByte(head->wordCount);
  }

  return 0;
} // end of QdataRecord::handle()


//
// Class name  : QcommentRecord
// Constructor : QcommentRecord(unsigned short * buf, int len)
//
// Description : initialize object
//		 rearrange pointers so they fit
//               comment record definition
// Input       : 
//

QcommentRecord::QcommentRecord(unsigned short * buf, int len)
  : Qrecord(buf, len)
{
  int slen = buf[-2];
  str = new char[slen+2];
  memset(str, 0, slen+2);
  memcpy(str, buf-1, slen-slen%2);
  str[strlen(str)] = '\n';
} // end of QcommentRecord::QcommentRecord(unsigned short *, int)


//
// Class name  : QcommentRecord
// Method name : print(unsigned long offs)
//
// Description : print data record statistics
//

int
QcommentRecord::print(unsigned long offs)
{
  Qrecord::print(offs);
  printf("               comment : %s", str);

  return 0;
} // end of QcommentRecord::print(unsigned long)


//
// Class name  : QbeginningOfTapeRecord
// Method name : print(unsigned long offs)
//
// Description : print data record statistics
//

int
QbeginningOfTapeRecord::print(unsigned long offs)
{
  Qptr header = ((Qptr)buffer)-1;
  printf("tape %d\n", swapByte(header->number1));

  return Qrecord::print(offs);
} // end of QbeginningOfTapeRecord::print(unsigned long)


//
// Class name  : QdiskFileErrorRecord
// Method name : print(unsigned long offs)
//
// Description : print data record statistics
//

int
QdiskFileErrorRecord::print(unsigned long offs)
{
  Qrecord::print(offs);
  
  Qptr header = ((Qptr)buffer)-1;
  printf("           error class : %d = 0%o = 0x%x\n",
	 swapByte(header->number1), swapByte(header->number1),
	 swapByte(header->number1));

  return 0;
} // end of QdiskFileErrorRecord::print(unsigned long)


//
// Class name  : QbeginningOfRunRecord
// Constructor : QbeginningOfRunRecord(unsigned short * buf, int len)
//
// Description : initialize object
// Input       : 
//

QbeginningOfRunRecord::QbeginningOfRunRecord(unsigned short * buf, int len)
  : Qrecord(buf, len)
{
  str = NULL;
  int slen = buf[-2];
  if (slen) {
    str = new char[slen+2];
    memset(str, 0, slen+2);
    memcpy(str, buf-1, slen);
    str[strlen(str)] = '\n';
  }
} // end of QbeginningOfRunRecord::QbeginningOfRunRecord(unsigned short *, int)


//
// Class name  : QbeginningOfRunRecord
// Method name : print(unsigned long offs)
//
// Description : print data record statistics
//

int
QbeginningOfRunRecord::print(unsigned long offs)
{
  Qrecord::print(offs);
  if (str) printf("               comment : %s", str);

  return 0;
} // end of QbeginningOfRunRecord::print(unsigned long)


//
// Class name  : QendOfRunRecord
// Method name : print(unsigned long offs)
//
// Description : print data record statistics
//

int
QendOfRunRecord::print(unsigned long offs)
{
  Qrecord::print(offs);

  unsigned int counter[32];
  memcpy(counter, buffer-3, 32*sizeof(unsigned int));

  for (int i=0; i<32; i++) {
    printf("            counter %2d :%8d\n", i, counter[i]);
  }      

  return 0;
} // end of QendOfRunRecord::print(unsigned long)


//
// Class name  : Qevent
// Constructor : Qevent(unsigned short * buf, int len)
//
// Description : initialize event object
// Input       : buf = pointer to event data
//               len = length of event record (in words)
//

Qevent::Qevent(unsigned short * buf, int len)
{
  buffer = buf;
  buflen = len;
  bufend = buf+len;
  dateTime = 0;
} // end of Qevent::Qevent()


//
// Class name  : Qevent
// Method name : print(void)
//
// Description : print event statistics
//

int
Qevent::print()
{
  Dptr head = (Dptr) (buffer-2);
  printf("           event %5d : len %3d status %d\n",
	 head->eventNumber, swapByte(head->wordCount), head->statusFlag);

  return 0;
} // end of Qevent::print()


//
// Class name  : Qevent
// Method name : handle(void)
//
// Description : handle event
//

int
Qevent::handle()
{
  return 0;
} // end of Qevent::handle()
