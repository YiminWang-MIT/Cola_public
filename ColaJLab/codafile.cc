//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaJLab/codafile.cc,v 1.3 1999-02-03 23:00:45 distler Exp $
//
// Classes to read coda files
//

#include "codafile.h"
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include <stdio.h>
#include <iomanip.h>
#include <netinet/in.h>

const int initread = 128*1024;

//
// Class name  : FileBase
// Constructor : FileBase(int ifd)
//
// Description : initialize object - allocate memory for buffer
//

FileBase::FileBase(int fdesc)
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

  char * args[3] = { NULL, NULL, NULL };
  // gzip compression method OR standard unix compress
  if (((initbuf[0] == 0x1f) && (initbuf[1] == 0x8b)) ||
      ((initbuf[0] == 0x1f) && (initbuf[1] == 0x9d))) {
    args[0] = GZIP; args[1] = "-dq";
  }
  // bzip2 compression method
  if ((initbuf[0] == 'B') && (initbuf[1] == 'Z') && (initbuf[2] == 'h') &&
      (initbuf[3] >= '1') && (initbuf[3] <= '9')) {
    args[0] = "bzip2"; args[1] = "-d";
  }

  if (args[0]) {
    if (pipe(&pipes.parent_in)) exit (errno);
    if (pipe(&pipes.child_in)) exit (errno);

    if ((pid = fork()) < 0) exit (errno);

    if (!pid) { /* child */

      close(pipes.parent_in);
      close(pipes.parent_out);

      if (0 != dup2(pipes.child_in, 0)) exit (errno);
      if (1 != dup2(pipes.child_out, 1)) exit (errno);

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
} // end of FileBase::FileBase(int)


//
// Class name  : FileBase
// Destructor  : ~FileBase()
//
// Description : free allocated memory
//

FileBase::~FileBase()
{
  if (buffer) delete buffer;
} // end of FileBase::~FileBase()


//
// Class name  : FileBase
// Method name : realignHeader(int count)
//

int
FileBase::realignHeader(int count)
{
  if (ofd >= 0 && (count != write(count))) {
    close(ofd);
    ofd = -1;
  }

  memmove(buffer, buffer+count, (bufptr-buffer)-count);
  bufptr -= count;

  return read(count);
} // end of FileBase::realignHeader(int)


//
// Class name  : FileBase
// Method name : read(int count)
//
// Description : read data into buffer
// Input       : count = number of chars to read
// Return      : number of chars read or -1 on error
//

int
FileBase::read(int count)
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
} // end of FileBase::read(int)


//
// Class name  : FileBase
// Method name : write(int count)
//
// Description : write data buffer to file
// Input       : count = number of chars to write
// Return      : number of chars written or -1 on error
//

int
FileBase::write(int count)
{
  int havewritten = 0;

  while (count > 0) {
    int w = ::write(ofd, buffer+havewritten, count);
    if (w < 0) return w;
    count -= w;
    havewritten += w;
  }

  return havewritten;
} // end of FileBase::write(int)


//
// Class name  : CodaFile
//

CodaFile::CodaFile(int ifd) : FileBase(ifd)
{
  tempbank = tempcont = NULL;
  putenv("TZ=EST5EDT"); tzset(); // Timezone US Eastern
} // end of CodaFile::CodaFile(int)

EventBase *
CodaFile::createEvent(struct BankHeader * bank)
{
  if (bank->num == 0xCC) {
    switch (ntohs(bank->tag)) {  // run control event
    case 16:  return new SyncEvent(bank); break;
    case 17:  return new PreStartEvent(bank); break;
    case 18:  return new GoEvent(bank); break;
    case 19:  return new PauseEvent(bank); break;
    case 20:  return new EndEvent(bank); break;
    default:  // standard physics event
      return new PhysicsEvent(bank); break;
    }
  }
  return new EventBase(bank);
}

int
CodaFile::handle()
{
  EventBase * event;
  if (tempbank) {
    event = createEvent(tempbank);
    event->handle();
    delete event;
    delete tempbank;
    tempbank = tempcont = NULL;
  }

  struct BankHeader * bankptr = bankstart;
  while (bankptr < bankend) {
    event = createEvent(bankptr);
    if (event->nextEvent() <= bankend) {
      bankptr = event->handle();
    } else {
      int tempsize = (char *)bankend-(char *)bankptr;
      tempbank = (struct BankHeader *) new int[ntohl(bankptr->length)+1];
      memcpy(tempbank, bankptr, tempsize);
      tempcont = (struct BankHeader *)((char *)tempbank + tempsize);
      bankptr = event->nextEvent();
    }
    delete event;
  }
  
  return 0;
} // end of CodaFile::handle()


//
// Class name  : CodaFile
// Method name : print(void)
//
// Description : print record information to stdout
// Input       : 
// Output      : 
// Return      : 
//

int
CodaFile::print()
{
  EventBase * event;
  if (tempbank) {
    event = createEvent(tempbank);
    event->print();
    delete event;
    delete tempbank;
    tempbank = tempcont = NULL;
  }

  cout << "Record " << ntohl(header->number)
       << " : len=" << ntohl(header->size) << endl;

  struct BankHeader * bankptr = bankstart;
  while (bankptr < bankend) {
    event = createEvent(bankptr);
    if (event->nextEvent() <= bankend) {
      bankptr = event->print();
    } else {
      int tempsize = (char *)bankend-(char *)bankptr;
      tempbank = (struct BankHeader *) new int[ntohl(bankptr->length)+1];
      memcpy(tempbank, bankptr, tempsize);
      tempcont = (struct BankHeader *)((char *)tempbank + tempsize);
      bankptr = event->nextEvent();
    }
    delete event;
  }
  
  return 0;
} // end of CodaFile::print()

//
// Class name  : CodaFile
// Method name : nextRecord(void)
//
// Description : get next physical record
//

int
CodaFile::nextRecord()
{
  int r;

  bufptr = buffer;
  if (sizeof(struct RecordHeader) != (r = read(sizeof(struct RecordHeader)))) {
    if (r < 0) { // error
      return -1;
    } else { // end of file
      return -1;
    }
  }

  header = (struct RecordHeader *) buffer;
  int havetoread =
    sizeof(unsigned int)*ntohl(header->size) - sizeof(struct RecordHeader);
  if (havetoread != (r = read(havetoread))) {
    if (r < 0) { // error
      return -1;
    } else { // end of file
      return -1;
    }
  }

  if (ofd >= 0) {
    int havetowrite = sizeof(struct RecordHeader)+havetoread;
    if (havetowrite != write(havetowrite)) {
      close(ofd);
      ofd = -1;
    }
  }

  bankstart = (struct BankHeader *) ((int *)buffer + htonl(header->start));
  bankend   = (struct BankHeader *) ((int *)buffer + htonl(header->used));

  if (tempbank && tempcont)
    memcpy(tempcont, header+1, sizeof(unsigned int)*htonl(header->start)
	   - sizeof(struct RecordHeader));

  return 0;
} // end of CodaFile::print()

EventBase::EventBase(struct BankHeader * bank)
{
  buffer = bank;
  bufend = (struct BankHeader *)((int *)buffer + ntohl(buffer->length) + 1);
}

struct BankHeader *
EventBase::print()
{
  cout << "  Event: len=" << ntohl(buffer->length)
       << " tag=" << buffer->tag << " type=" << (int)buffer->dtype
       << " num=" << (int)buffer->num << endl;

  return bufend;
}

struct BankHeader *
EventBase::handle()
{
  return bufend;
}

struct BankHeader *
SyncEvent::print()
{
  unsigned int * ev = (unsigned int *)buffer;
  time_t unixTime = ntohl(ev[2]);
  cout << "  Sync Event:  events=" << ntohl(ev[3])
       << "(" << ntohl(ev[4]) << ")" << "  status=" << ntohl(ev[5])
       << "  " << ctime(&unixTime);

  return bufend;
}

struct BankHeader *
PreStartEvent::print()
{
  unsigned int * ev = (unsigned int *)buffer;
  time_t unixTime = ntohl(ev[2]);
  cout << "  PreStart Event:  run=" << ntohl(ev[3])
       << "  type=" << ntohl(ev[4])
       << "  " << ctime(&unixTime);

  return bufend;
}

struct BankHeader *
GoEvent::print()
{
  unsigned int * ev = (unsigned int *)buffer;
  time_t unixTime = ntohl(ev[2]);
  cout << "  Go Event:  events=" << ntohl(ev[4])
       << "  " << ctime(&unixTime);

  return bufend;
}

struct BankHeader *
PauseEvent::print()
{
  unsigned int * ev = (unsigned int *)buffer;
  time_t unixTime = ntohl(ev[2]);
  cout << "  Pause Event:  events=" << ntohl(ev[4])
       << "  " << ctime(&unixTime);

  return bufend;
}

struct BankHeader *
EndEvent::print()
{
  unsigned int * ev = (unsigned int *)buffer;
  time_t unixTime = ntohl(ev[2]);
  cout << "  End Event:  events=" << ntohl(ev[4])
       << "  " << ctime(&unixTime);

  return bufend;
}

struct BankHeader *
PhysicsEvent::print()
{
  cout << "  Physics Event:  tag=" << ntohs(buffer->tag)
       << "  len=" << ntohl(buffer->length) << endl;
  BankBase * bank;
  struct BankHeader * bankptr = buffer + 1;
  while (bankptr < bufend) {
    bank = createBank(bankptr);
    bankptr = bank->print();
    delete bank;
  }
  return bufend;
}

struct BankHeader *
PhysicsEvent::handle()
{
  BankBase * bank;
  struct BankHeader * bankptr = buffer + 1;
  while (bankptr < bufend) {
    bank = createBank(bankptr);
    bankptr = bank->handle();
    delete bank;
  }
  return bufend;
}

BankBase *
PhysicsEvent::createBank(struct BankHeader * bank)
{
  if (ntohs(bank->tag) == 0xC000)
    return new EventIdBank(bank);

  if (ntohs(bank->tag) < 32)
    return new ROCDataBank(bank);

  return new BankBase(bank);
}

BankBase::BankBase(struct BankHeader * bank)
{
  buffer = bank;
  bufend = (struct BankHeader *)((int *)buffer + ntohl(buffer->length) + 1);
}

struct BankHeader *
BankBase::print()
{
  cout << "    Bank: len=" << ntohl(buffer->length)
       << " tag=" << ntohs(buffer->tag) << " type=" << (int)buffer->dtype
       << " num=" << (int)buffer->num << endl;

  return bufend;
}

struct BankHeader *
BankBase::handle()
{
  return bufend;
}

struct BankHeader *
EventIdBank::print()
{
  unsigned int * data = (unsigned int *)buffer;
  cout << "    Event Id Bank:  number=" << ntohl(data[2]) << "  class="
       << ntohs(data[3]) << "  status=" << ntohl(data[4]) << endl;

  return bufend;
}

struct BankHeader *
ROCDataBank::print()
{
  cout << "    ROC Data Bank:  ROC=" << ntohs(buffer->tag)
       << "  counter=" << (int)buffer->num << endl;

  return bufend;
}
