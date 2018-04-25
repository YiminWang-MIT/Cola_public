//
// Created 1993-95 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Cindy/CindyStream.cc,v 4.18 2002-12-12 12:24:41 distler Exp $
//
// Implementation of class CindyStream
//

#pragma implementation
#define  _DEFAULT_SOURCE 1
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#ifdef HAVE_BSTRING_H
#include <bstring.h>
#endif
#include <cstring>
#include <cstdlib>
#include <signal.h>
#include <sys/param.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mtio.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <limits.h>

#include "CindyStream.h"
#include "CindyXDR.h"

const int   MAXBUFLEN		= 65*1024;
const Uint2 format_swapped	= (Uint2)(MECDASFORMAT<<8)
				| (Uint2)((MECDASFORMAT>>8) & 0xff);

#ifdef __alpha__
void
aXDRcpy(int *i, char *c)
{
  char *I = (char *)i;

  I[0] = c[3];
  I[1] = c[2];
  I[2] = c[1];
  I[3] = c[0];
}
#endif


CindyInBase::CindyInBase()
{
  bufferlen = MAXBUFLEN;
  bufferReset(mainbuf = new char[bufferlen]);
  havetoswap = fd = -1;
}


int
CindyInBase::Read()
{
  if (fd < 0) return (timeOut = 0);

  int readSum, readPart, haveToRead = bufferlen - (bufend - mainbuf);
  fd_set fdset;
  struct timeval timeout;

  for (readSum = 0; haveToRead > 0; ) {
    timeout.tv_sec  = 0;
    timeout.tv_usec = 10000;
    FD_ZERO(&fdset);
    FD_SET(fd, &fdset);

    if (1 == select(fd+1, (FDSETPTR)&fdset, NULL, NULL, &timeout)) {
      timeOut = 0;
      if ((readPart = read(fd, bufend, haveToRead)) <= 0) {
	if (readPart < 0) return 0;
	break;
      }
      haveToRead -= readPart;
      bufend     += readPart;
      readSum    += readPart;
    } else {
      timeOut = 1;
      break;
    }
  }

  return readSum;
}


int
CindyInBase::checkHeader(behdr *head, int len)
{
  if (!havetoswap) return 0;

  if (havetoswap<0) {	// -1: muss erst feststellen, ob ge'swap't werden muss
    if (head->b_format == MECDASFORMAT) {
      havetoswap = 0;
      return 0;
    }
    else if (head->b_format == format_swapped) {
      havetoswap = 1;
    }
    else {
      std::cerr << "bad header" << std::endl;
      return -1;
    }
  }

  if (short_sizeof_behdr+swapIfNecessary(head->b_len) > len) return 0;

  mem_swap((Uint2 *)head, (Uint2 *)head, short_sizeof_behdr);
  if (!(head->b_flags & BEnoswap))
    mem_swap((Uint2 *)(head+1), (Uint2 *)(head+1), head->b_len);

  return 1;
}


behdr *
CindyInBase::nextHeader()
{
  if (mainbuf == bufend) Read(); // buffer is empty

  unsigned int datalen = bufend-bufptr;
  if (datalen < sizeof(struct behdr)) { // remaining data < header size
    if (bufptr != mainbuf) { // copy remaining data to begining of buffer
      memcpy(mainbuf, bufptr, datalen);
      bufptr = mainbuf;
      bufend = mainbuf+datalen;
    }
    Read();
  }

  datalen = bufend-bufptr;
  if (datalen < sizeof(struct behdr)) return NULL;
  header = (struct behdr *) bufptr;
  checkHeader(header, 0);
  // remaining data < header+event
  if (datalen < (sizeof(struct behdr)+
		 swapIfNecessary(header->b_len)*sizeof(Uint2))) {
    if (bufptr != mainbuf) { // copy remaining data to begining of buffer
      memcpy(mainbuf, bufptr, datalen);
      bufptr = mainbuf;
      bufend = mainbuf+datalen;
      header = (struct behdr *) bufptr;
    }
    Read();
  }

  datalen = bufend-bufptr;
  if (datalen < (sizeof(struct behdr)+
		 swapIfNecessary(header->b_len)*sizeof(Uint2))) return NULL;

  checkHeader(header, datalen);
  bufptr = (char *)(header+1) + header->b_len*sizeof(Uint2);

  return header;
}


CindyInFile::CindyInFile(const char *name)
{
    struct stat sbuf;

    if (stat(name, &sbuf) == -1) {
	perror("CindyInFile stat");
    } else {
	rewindable = S_ISREG(sbuf.st_mode);
    }
    if ((fd = open(name,O_RDONLY)) == -1) {
	perror("CindyInFile open");
    } else {
	struct mtget mt_status;
	isTape = (S_ISCHR(sbuf.st_mode) &&
		  (ioctl(fd, MTIOCGET, (char *)&mt_status) != -1));
	if (isTape) rewindable = 1;
    }
}

int
CindyInFile::rewind()
{
    if (rewindable) {
	bufferReset(mainbuf);
	if (isTape) {
	    struct mtop mt_com;

	    mt_com.mt_op = MTBSF;
	    mt_com.mt_count = 1;
	    if (ioctl(fd, (int)MTIOCTOP, (char *)&mt_com) == -1) {
		mt_com.mt_op = MTREW; // error maybe first file
	    } else {
		mt_com.mt_op = MTFSF;
	    }
	    mt_com.mt_count = 1;
	    return ioctl(fd, (int)MTIOCTOP, (char *)&mt_com);
	} else {
	    return (lseek(fd,0,SEEK_SET) == -1) ? -1 : 0;
	}
    }

    return -1;
}


CindyInTcpip::CindyInTcpip(const char *port)
{
  if (port) {
    char * target = new char[MAXHOSTNAMELEN];
    unsigned short portnumber;
    if (*port == ':') { // localhost is target
      if (gethostname(target, MAXHOSTNAMELEN) < 0) {
	perror("gethostname"); return;
      }
      if (1 != sscanf(port+1, "%hd", &portnumber)) return;
    } else {
      if (2 != sscanf(port, "%[^:]:%hd", target, &portnumber)) return;
    }

    struct hostent * targethost;
    if (NULL == (targethost = gethostbyname(target))) {
      perror("gethostbyname"); return;
    }

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("socket"); return;
    }

    struct sockaddr_in outsockaddr;
    memset(outsockaddr.sin_zero, 0, 8);
    outsockaddr.sin_family = targethost->h_addrtype;
    outsockaddr.sin_port   = htons(portnumber);
    memcpy(&outsockaddr.sin_addr, targethost->h_addr, targethost->h_length);
    if (connect(fd, (struct sockaddr *)&outsockaddr, sizeof(outsockaddr))) {
      perror("connect"); fd = -1; return;
    }
  }
}


CindyInProc::CindyInProc(const char *command, const char *name)
{
  if (name) {
    com = new char[strlen(command)+strlen(name)+1];
    strcpy(com, command); strcat(com, name);
  } else {
    com = new char[strlen(command)+1];
    strcpy(com, command);
  }
  proc = popen(com, "r");
  fd = fileno(proc);
}


int
modWrite(int fd,char *data, int len)
{
  const int block = 10240;

  while (len > 0) {
    if (len>block) {
      if (block != write(fd,data,block)) return(-1);
    }
    else {
      if (len != write(fd,data,len)) return(-1);
    }
    data += block;
    len -= block;
  }

  return 0;
}

int
modWrite(FILE *fs, char *data, int len)
{
  const int block = 10240;

  while (len > 0) {
    if (len>block)
      fwrite(data,block,1,fs);
    else
      fwrite(data,len,1,fs);
    data += block;
    len -= block;
  }

  return 0;
}

CindyStream::CindyStream(const char *file, const char *proc)
{
  name = ident = runName = experiment = NULL;
  timeOfDataTaking = oldTimeOfDataTaking = 0;
  debug_flag = 0;
  remainingEvents = INT_MAX;
  name = new char[strlen(file)+1];
  strcpy(name, file);
  if (index(name, ':')) {
    in = new CindyInTcpip(file);
  } else {
    if (0 == strcmp(name, "-")) {
      if (proc)
	in = new CindyInProc(proc,file);
      else
	in = new CindyInStd();
    } else {
      if (proc)
	in = new CindyInProc(proc,file);
      else
	in = new CindyInFile(file);
    }
  }
}

int
CindyStream::rewind()
{
    if (!in || (in->rewind() == -1))
	return -1;

    remainingEvents = INT_MAX;
    return 0;
}

int
CindyStream::getHeader()
{
  if (NULL == in->nextHeader()) return (in->wasTimeOut() ? -1 : 0);

  if (remainingEvents <= 0) return 0;

  header = in->headerPtr();
  data = in->dataPtr();

  if (normalEventData()) remainingEvents--;
  
  return 1;
}

int
CindyStream::unpackConfigData(const char *path)
{
  char cwd[MAXPATHLEN+2];
  char *sysStr = new char[strlen(path)+32];

  sprintf(sysStr, "mkdir %s 2>/dev/null", path);
  if (getcwd(cwd,MAXPATHLEN+2));
  if (system(sysStr));
  if (chdir(path));
  delete sysStr;

  int len = sizeof(Uint2) * getLengthOfDataField();
  FILE *pb;

  if (header->b_flags & BEtar) {
    if (debug_flag)
      std::cerr << "found configuration (tar: "
		<< len << " Bytes)" << std::endl;
    if (!(pb = popen("tar xf - >/dev/null 2>&1", "w"))) return -1;
  }
  else if (header->b_flags & BEshar) {
    if (debug_flag)
      std::cerr << "found configuration (shar: "
		<< len << " Bytes)" << std::endl;
    if (!(pb = popen("sh - >/dev/null 2>&1", "w"))) return -1;
  }
  else return -1;

  fwrite(data, 1, len, pb);
  pclose(pb);

  if (chdir(cwd));
  return 0;
}

int
CindyStream::unpackSpecialData(const char *path)
{
  char cwd[MAXPATHLEN+2];
  char *sysStr= new char[strlen(path)+32];

  sprintf(sysStr, "mkdir %s 2>/dev/null", path);
  if (getcwd(cwd,MAXPATHLEN+2));
  if (system(sysStr));
  if (chdir(path));
  delete sysStr;

  int len = sizeof(Uint2) * getLengthOfDataField();

  char specialName[16];

#ifdef __alpha__
  sprintf(specialName,"%d",timeOfDataTaking);
#else
  sprintf(specialName,"%ld",timeOfDataTaking);
#endif

  if (debug_flag)
    std::cerr << "found special data (" << len << " Bytes,"
	      << getTimeOfDataTakingString() << ')' << std::endl;

  FILE *out;
  out = fopen(specialName,"w");
  modWrite(out,(char *)data,len);
  fclose(out);

  if (chdir(cwd));
  return 0;
}

Uint2 *
CindyStream::getRunName()
{
  if (!runName) {
    runName = new char[strlen((char *)data)+1];
    strcpy(runName, (char *)data);
  }

  return data;
}

Uint2 *
CindyStream::getIdent()
{
  char  *bufchar = (char *)data;

  if (!ident) {
    ident = new char[strlen(bufchar)+1];
    strcpy(ident, bufchar);
  }
  if (!experiment) {
    experiment = new char[strlen(ident)+1];
    char *exp = ident;
    int index=0;
    for ( ; *exp != ' ' ; index++)
      experiment[index] = *exp++;
    experiment[index] = 0;
  }

  if (debug_flag) std::cerr << "found ident: " << bufchar << std::endl;

  bufchar += strlen(bufchar)+1;
#ifdef __alpha
  aXDRcpy(&timeOfDataTaking,bufchar);
#else
  XDRcpy(&timeOfDataTaking,bufchar);
#endif

  if (debug_flag)
    std::cerr << "experiment: " << experiment << ' '
	      << timeOfDataTaking << std::endl;

  return data;
}

Uint2 *
CindyStream::getOldIdent()
{
  char  *bufchar = (char *)data;
  char *oldExp = NULL;

  if (debug_flag) {
    std::cerr << "found old ident: " << bufchar << std::endl;
    oldExp = new char[strlen(bufchar)+1];
    char *exp = bufchar;
    int index = 0;
    for ( ; *exp != ' ' ; index++)
      oldExp[index] = *exp++;
    oldExp[index] = 0;
  }

  bufchar += strlen(bufchar)+1;
#ifdef __alpha
  aXDRcpy(&oldTimeOfDataTaking,bufchar);
#else
  XDRcpy(&oldTimeOfDataTaking,bufchar);
#endif

  if (!ident) timeOfDataTaking = oldTimeOfDataTaking;

  if (debug_flag) {
    std::cerr << "experiment: " << oldExp << ' '
	      << oldTimeOfDataTaking << std::endl;
    delete oldExp;
  }

  return data;
}

CindyStream::~CindyStream()
{
  delete name;
  delete ident;
  delete runName;
  delete experiment;
  delete in;
}

CindyOutStream::CindyOutStream(const char *file,const char *proc)
{
  tag	= none;
  name	= NULL;
  out	= NULL;
  eventCount = 0;
  if (file) {
    name = new char[strlen(file)+1];
    strcpy(name, file);
    if (index(name, '@')) {
      tag = Pipe;
      std::cerr << "Pipes are not yet supported" << std::endl;
    } else if (index(name, ':')) {
      tag = TCPIP;
      std::cerr << "TCPIP connections are not yet supported" << std::endl;
    } else if (0 == strcmp(name, "-")) {
      if (proc) {
	tag = Proc;
	out = popen(proc, "w");
      }
      else {
	tag = Stdio;
	out = stdout;
      }
    } else {
      if (proc) {
	tag = Proc;
	char *command = new char[strlen(proc)+strlen(file)+4];
	sprintf(command, "%s > %s", proc, file);
	out = popen(command, "w");
	delete command;
      }
      else {
	tag = Stdio;
	out = fopen(file,"w");
      }
    }
  }
}

int
CindyOutStream::putData(behdr *header, const Uint2 *data)
{
  std::streampos havewritten = 0;

#ifdef WORDS_BIGENDIAN
  header->b_null = 0xffff;
#else
  header->b_null = 0x0;
#endif
  switch (tag) {
  case Stdio:
  case File:
  case Proc:
    havewritten = fwrite(header,1,sizeof(behdr),out);
    filepos += havewritten;
    havewritten = fwrite(data,1,sizeof(Uint2) * header->b_len,out);
    filepos += havewritten;
    break;
  case Pipe:
  case TCPIP:
  case none:
    break;
  }
  return
    havewritten == (std::streampos)(sizeof(behdr)+sizeof(Uint2)*header->b_len);
}

int
CindyOutStream::putData(const short *data, int size)
{
  behdr header;

  if (!size) return 0;

  header.b_len = size;
  header.b_format = MECDASFORMAT;
  header.b_flags  = BEnormal;
  header.b_events = ++eventCount;
  header.b_code   = 0;

  return putData(&header,(Uint2 *)data);
}

int
CindyOutStream::putData(const short *data, int size, int cnt, int code)
{
  behdr header;

  if (!size) return 0;

  header.b_len = size;
  header.b_format = MECDASFORMAT;
  header.b_flags  = BEnormal;
  header.b_events = 0xffff & (eventCount = cnt);
  header.b_code   = code;

  return putData(&header,(Uint2 *)data);
}

int
CindyOutStream::putIdentX(char *id, time_t dataT)
{
  return (eventCount % 100) ? 0 : putIdent(id,dataT);
}

int
CindyOutStream::putIdent(char *id, time_t dataT)
{
  behdr header;
  int tpos = strlen(id)+1;

  header.b_len = (tpos+5)/2;
  header.b_format = MECDASFORMAT;
  header.b_flags  = BEident | BEnoswap;
  header.b_events = 0;
  header.b_code   = 0;

  if (!dataT) time(&dataT);
  mem_swap(id+tpos, &dataT);

  return putData(&header,(Uint2 *)id);
}

int
CindyOutStream::putOldIdent(const Uint2 *id, Uint2 size)
{
  behdr header;

  header.b_len = size;
  header.b_format = MECDASFORMAT;
  header.b_flags  = BEoldident | BEnoswap;
  header.b_events = 0;
  header.b_code   = 0;

  return putData(&header,id);
}

int
CindyOutStream::putTar(const char *data, int size)
{
  behdr header;

  header.b_len = size / 2;
  header.b_format = MECDASFORMAT;
  header.b_flags  = BEconfig | BEtar | BEnoswap;
  header.b_events = 0;
  header.b_code   = 0;

  return putData(&header,(Uint2 *)data);
}

int
CindyOutStream::putComment(const char *data)
{
  behdr header;

  header.b_len = strlen(data) / 2 + 1;
  header.b_format = MECDASFORMAT;
  header.b_flags  = BEcomment | BEnoswap;
  header.b_events = 0;
  header.b_code   = 0;

  return putData(&header,(Uint2 *)data);
}

int
CindyOutStream::putRunName(const char *data)
{
  behdr header;

  header.b_len = strlen(data) / 2 + 1;
  header.b_format = MECDASFORMAT;
  header.b_flags  = BEname | BEnoswap;
  header.b_events = 0;
  header.b_code   = 0;

  return putData(&header,(Uint2 *)data);
}

CindyOutStream::~CindyOutStream()
{
  if (name)	delete name;
  if (out) {
    if (tag == Proc) {
      pclose(out);
    } else fclose(out);
  }
}

void
mem_swap(Uint2 *to, Uint2 *from, Uint2 n)
{
  while ( n-- ) {
    *to++ = (*from << 8) + (*from >> 8);
    from++;
  }
}

void
mem_swap(Uint4 *to, Uint4 *from, Uint2 n)
{
  while ( n-- ) {
    *to++ = (*from << 24) + ((*from << 8) & 0xff0000) +
      ((*from >> 8) & 0xff00) + (*from >> 24);
    from++;
  }
}
