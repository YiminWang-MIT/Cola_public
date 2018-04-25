//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-456      tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaMIT/qpsql.cc,v 2.1 2002-03-05 03:49:50 distler Exp $
//
// test program for class Qtape
//

#include <stdio.h>
#include <string.h>
#include "qtape.h"

const char *
myctime(time_t *t)
{
  static char tstr[32];
  strcpy(tstr, ctime(t));
  tstr[strlen(tstr)-1] = 0;
  return tstr;
}

class PSQL {
  int runnr;
  int event[32];
  int datatype[256];
  time_t start, stop, suspend;
  int suspended;
  char *comment;
public:
  PSQL();
  virtual ~PSQL() { delete comment; }
  int initDT(time_t dt, unsigned short *buf = NULL);
  int incDataFlag(unsigned short i) { return ++datatype[i&255]; }
  int incEvent(unsigned short i) { return ++event[i&31]; }
  int setRunNr(unsigned short nr) { return runnr = nr; }
  void markSuspend() { suspend = stop; }
  void markResume() { if (suspend) suspended += stop-suspend; suspend = 0; }
  int print();
}; // end of class PSQL

PSQL::PSQL()
{
  int i;
  runnr = suspended = 0;
  for (i=0; i<32; i++) event[i] = 0;
  for (i=0; i<256; i++) datatype[i] = 0;
  start = stop = suspend = 0;
  comment = NULL;
}

int
PSQL::initDT(time_t dt, unsigned short * buf)
{
  stop = dt;
  if (!start) start = dt;
  if (!comment) {
    if (buf) {
      int slen = buf[-1];
      if (slen) {
	comment = new char[slen+2];
	memset(comment, 0, slen+2);
	memcpy(comment, buf, slen);
      }
    } else {
      comment = new char[4];
      memset(comment, 0, 4);
    }      
  }
  return 0;
}

int
PSQL::print()
{
  printf("INSERT INTO run (\n");
  printf("    number,\n");
  printf("    startTime,\n");
  printf("    stopTime,\n");
  printf("    suspended,\n");
  printf("    event5, event8, event10, event13,\n");
  printf("    junk, coincAO, coincBO, coincCO,\n");
  printf("    singleA, singleB, singleC, singleO,\n");
  printf("    coincABO, coincACO, coincBCO, coincABCO,\n");
  printf("    coincAB, coincAC, coincBC, coincABC,\n");
  printf("    initialComment\n");
  printf(") VALUES (\n");
  printf("    %d,\n", runnr);
  printf("    '%s'::datetime,\n", myctime(&start));
  printf("    '%s'::datetime,\n", myctime(&stop));
  printf("    '%d sec'::timespan,\n", suspended);
  printf("    %d, %d, %d, %d,\n", event[5], event[8], event[10], event[13]);
  printf("    %d, %d, %d, %d,\n",
	 datatype[255], datatype[65], datatype[66], datatype[67]);
  printf("    %d, %d, %d, %d,\n",
	 datatype[69], datatype[70], datatype[71], datatype[73]);
  printf("    %d, %d, %d, %d,\n",
	 datatype[74], datatype[75], datatype[77], datatype[80]);
  printf("    %d, %d, %d, %d,\n",
	 datatype[85], datatype[86], datatype[88], datatype[91]);
  printf("    '%s'\n);\n", comment);

  return 0;
}

class PSQLevent8 : public Qevent {
  PSQL * psql;
public:
  PSQLevent8(PSQL * p, unsigned short * buf, int len)
    : Qevent(buf, len) { psql = p; }
  virtual ~PSQLevent8() { ; }
  virtual int handle(void) { psql->incDataFlag(buffer[1]); return 0; }
}; // end of class PSQLevent8

class PSQLdataRecord : public QdataRecord {
  PSQL * psql;
public:
  PSQLdataRecord(PSQL * p, unsigned short * buf, int len)
    : QdataRecord(buf, len) { psql = p; }
  virtual ~PSQLdataRecord() { ; }
  virtual Qevent * newEvent(unsigned short * data, int words);
}; // end of class PSQLdataRecord

Qevent *
PSQLdataRecord::newEvent(unsigned short *data, int words)
{
  Dptr head = (Dptr) data;
  Qevent * event = NULL;

  psql->incEvent(head->eventNumber);
  if (head->eventNumber == 27) psql->markSuspend();
  if (head->eventNumber == 26) psql->markResume();
  if (head->eventNumber == 8) event = new PSQLevent8(psql, data+2, words-2);

  return event;
} // end of PSQLdataRecord::newEvent()

class PSQLtape : public Qtape {
protected:
  PSQL * psql;
public:
  PSQLtape(PSQL * p, int ifd) : Qtape(ifd) { psql = p; }
  virtual ~PSQLtape() { ; }
  virtual QrecordBase * newRecord(RecordType rTyp,
				  unsigned short *data, int words);
}; // end of class PSQLtape

QrecordBase *
PSQLtape::newRecord(RecordType rTyp, unsigned short *data, int words)
{
  QrecordBase * qrec = NULL;

  switch (rTyp) {
  case BoR:
    qrec = new QbeginningOfRunRecord(data, words);
    psql->initDT(qrec->dateTime(), data-1);
    psql->setRunNr(qrec->runNumber());
    break;
  case Data:
    qrec = new PSQLdataRecord(psql, data, words);
    psql->initDT(qrec->dateTime());
    break;
  default:
    qrec = new Qrecord(data, words);
    psql->initDT(qrec->dateTime());
    break;
  }
    
  return qrec;
} // end of PSQLtape::newRecord(RecordType, unsigned short *, int)

int
main(int argc, char *argv[])
{
  PSQL psql;
  PSQLtape pt(&psql, 0);

  if (argc > 1) printf("usage: %s\n", argv[0]);

  while (Qtape::EoF != pt.nextRecord()) pt.handle();

  psql.print();

  return 0;
} // end of main()
