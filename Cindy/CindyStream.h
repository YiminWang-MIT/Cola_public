//								      -*-c++-*-
// Copyright (c) 1993-95 by	Michael O. Distler,
//				Institut fuer Kernphysik,
//				Johannes Gutenberg-Universitaet Mainz
//
// All rights reserved.
//
// $Header: /tmp/cvsroot/Cola/Cindy/CindyStream.h,v 4.12 2002-12-12 12:24:41 distler Exp $
//
// Header File fuer class CindyStream
//

#pragma interface
#ifndef __cindy_stream__
#define __cindy_stream__

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <cstdio>
#include <iostream>
#ifdef HAVE_ISTREAM
#include <istream>
#endif
#ifdef HAVE_OSTREAM
#include <ostream>
#endif
#include <fstream>
#include <iosfwd>
#include <ctime>
extern "C" {
#ifdef m68kph
#define time_t _G_time_t
#endif
#include "header.h"
}
#include <unistd.h>

typedef char	Int1;
typedef unsigned char	Uint1;
typedef short	Int2;
typedef unsigned short	Uint2;
typedef int	Int4;
typedef unsigned int	Uint4;

const int short_sizeof_behdr = sizeof(struct behdr) / sizeof(Uint2);

///Base class for all data input
class CindyInBase {
  /// pointer to the next data header
  char *bufptr;
  /// pointer to end of data
  char *bufend;
  /// pointer to this data header
  behdr *header;
  /// flag tells if bytes have to be swapped
  int havetoswap;
  /// flag tells if timeout has occured
  int timeOut;
  /// 
  virtual int Read();
  ///
  Uint2 swapIfNecessary(Uint2 data) {
    if (havetoswap>0)
      return (data<<8)|(data>>8);
    return data;
  }
  ///
  int checkHeader(behdr *head, int len);
protected:
  ///
  int bufferlen;
  ///
  int fd;
  ///
  char *mainbuf;
  ///
  void bufferReset(char *buf) { bufptr = bufend = buf; }
public:
  ///
  CindyInBase();
  ///
  virtual ~CindyInBase()        { ; }
  ///
  behdr *nextHeader();
  ///
  behdr *headerPtr()            { return header; }
  ///
  Uint2 *dataPtr()              { return (Uint2 *)(header+1); }
  ///
  Word	 dataLength()           { return header->b_len; }
  ///
  virtual int wasTimeOut()      { return timeOut; }
  ///
  virtual int isRewindable()    { return 0; }
  ///
  virtual int rewind()          { return -1; }
};


///
class CindyInStd : public CindyInBase {
public:
  CindyInStd() { fd = 0; }
  virtual ~CindyInStd() { ; }
};

class CindyInFile : public CindyInBase {
  int rewindable, isTape;
public:
  CindyInFile(const char *name);
  virtual ~CindyInFile() { if (fd > 0) close(fd); }
  virtual int isRewindable() { return rewindable; }
  virtual int rewind();
};

class CindyInTcpip : public CindyInBase {
public:
  CindyInTcpip(const char *port);
  virtual ~CindyInTcpip() { if (fd > 0) close(fd); }
};

class CindyInProc : public CindyInBase {
  char *com;
  FILE *proc;
public:
  CindyInProc(const char *command, const char *name=NULL);
  virtual ~CindyInProc() { pclose(proc); delete com; }
};

class CindyStream {
  int debug_flag, remainingEvents;
  behdr *header;
  Uint2 *data;
  CindyInBase *in;
  char *name, *ident, *runName, *experiment;
  time_t timeOfDataTaking, oldTimeOfDataTaking;
public:
  CindyStream(const char *file,const char *proc=NULL);
  int	      getHeader();
  behdr      *getHeaderPtr() { return header; }
  Uint2      *getData() { return data; }
  Uint2      *getRunName();
  Uint2      *getIdent();
  Uint2      *getOldIdent();
  Uint2       getLengthOfDataField() { return header->b_len; }
  Uint2       getEventCounter() { return header->b_events; }
  Uint2       getEventCode() { return header->b_code; }
  char       *getTimeOfDataTakingString()
    { return ctime(&timeOfDataTaking); }
  time_t      getTimeOfDataTaking()
    { return timeOfDataTaking; }
  char       *getOldTimeOfDataTakingString()
    { return ctime(&oldTimeOfDataTaking); }
  time_t      getOldTimeOfDataTaking()
    { return oldTimeOfDataTaking; }
  const char *getIdentificationString() { return ident; }
  const char *getRunNameString() { return runName; }
  const char *getExperimentString() { return experiment; }
  const char *getFilename() { return (name ? name : "-"); }
  int         normalEventData()
    { return ((header->b_flags & BEtype) == BEnormal)
	  && (header->b_code != 0xffff); }
  int         badEventData()
    { return ((header->b_flags & BEtype) == BEnormal)
	  && (header->b_code == 0xffff); }
  int         specialEventData()
    { return (header->b_flags & BEtype) == BEspecial; }
  int         identificationData()
    { return (header->b_flags & BEtype) == BEident; }
  int         oldIdentData()
    { return (header->b_flags & BEtype) == BEoldident; }
  int         configurationData()
    { return (header->b_flags & BEtype) == BEconfig; }
  int         runCommentData()
    { return (header->b_flags & BEtype) == BEcomment; }
  int         runNameData()
    { return (header->b_flags & BEtype) == BEname; }
  int         errorMessageData()
    { return (header->b_flags & BEtype) == BEmsg; }
  int	      setRemainingEvents(int num)
    { return (remainingEvents=num); }
  int	      unpackConfigData(const char *);
  int	      unpackSpecialData(const char *);
  void	      debugOn() { debug_flag = 1; }
  int	      isRewindable() { return ( in ? in->isRewindable() : 0 ); }
  int	      rewind();

  ~CindyStream(void);
};

class CindyOutStream {
public:
  enum StreamTag { Stdio, File, TCPIP, Pipe, Proc, none };
private:
  StreamTag	tag;
  char         *name;
  FILE	       *out;
  FILE         *outz;
  std::streampos	filepos;
  Word		eventCount;
public:
  CindyOutStream(const char *file, const char *proc);
  virtual ~CindyOutStream();
  StreamTag   getTag() { return tag; }
  const char *getFilename() { return (name ? name : "-"); }
  int	      putData(behdr *, const Uint2 *);
  int	      putData(const short *, int);
  int	      putData(const short *, int, int, int);
  int	      putIdent(char *id, time_t dataT=0);
  int	      putIdentX(char *id, time_t dataT=0);
  int	      putOldIdent(const Uint2 *, Uint2);
  int	      putTar(const char *, int);
  int	      putComment(const char *);
  int	      putRunName(const char *);
  //     copy and swap time to BIGENDIAN
  void        mem_swap(char * to, time_t * from);
};

inline void
CindyOutStream::mem_swap(char * to, time_t * from)
{
  char *t = (char *)from;

//	the date behind the id is written by the E6 frontend
//	the date is always BIGENDIAN

#ifdef WORDS_BIGENDIAN
  *to++ = *t++; *to++ = *t++; *to++ = *t++; *to++ = *t++;
#else
  *to++ = t[3]; *to++ = t[2]; *to++ = t[1]; *to++ = t[0];
#endif
}

void mem_swap(Uint2 *, Uint2 *, Uint2);
void mem_swap(Uint4 *, Uint4 *, Uint2);

inline void
mem_swap(Uint2 *to, Uint2 *from)
{
  *to = (*from << 8) + (*from >> 8);
}

inline void
mem_swap(Uint4 *to, Uint4 *from)
{
  *to++ = (*from << 24) + ((*from << 8) & 0xff0000) +
    ((*from >> 8) & 0xff00) + (*from >> 24);
}

inline void
mem_copy(Uint2 *to, Uint2 *from, int n)
{
  while ( n-- )
    *to++ = *from++;
}

#endif
