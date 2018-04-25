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
// $Header: /tmp/cvsroot/Cola/ColaMIT/OOPStape.cc,v 2.2 2002-12-12 12:24:12 distler Exp $
//
// Classes to read Q tape (with OOPS Data)
//

#define _DEFAULT_SOURCE    1
#include "OOPStape.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <utime.h>
#include <errno.h>
#include <iostream>
#include <stdlib.h>

struct event3   & OOPSevent3::data    = oops1.clear;
struct event5   & OOPSevent4::data    = oops1.old_scaler;
struct event5   & OOPSevent5::data    = oops1.old_scaler;
struct event6   & OOPSevent6::data    = oops1.scaler;
struct event8   & OOPSevent7::data    = oops1.det;
struct event8   & OOPSevent8::data    = oops1.det;
struct event10  & OOPSevent10::data   = oops1.beam;
struct event13  & OOPSevent13::data   = oops1.target;
struct moller5  & MOLLERevent5::data  = oops1.moller.coinc;
struct moller16 & MOLLERevent16::data = oops1.moller.quad;

int OOPSdataRecord::isMollerRun = 0;
int OOPSdataRecord::concatBeamEvents = 0;
int OOPSdataRecord::saveLen = 0;
unsigned short * OOPSdataRecord::saveData = NULL;

//
// Class name  : OOPStape
//


OOPStape::OOPStape(CindyTree * ctree, int ifd, int copyrun) : Qtape(ifd)
{
  ct = ctree;
  copyRun = copyrun;
  *runname = 0;
  childPID = -1;
  dataTaking = 0;
  mask = 0xffffffff;
}


//
// Class name  : OOPStape
// Method name : forkGzip(const char * filename)
//
// Description : create a new record object
// Input       : rTyp  = record type
//               data  = pointer to data
//               words = number of data words
// Return      : pointer to new QrecordBase object
//

int
OOPStape::forkGzip(const char * filename)
{
  int fd;
  struct {
    int child;
    int parent;
  } pipes;

  if (pipe(&pipes.child)) exit(errno);

  if ((childPID = fork()) < 0) exit(errno);

  if (!childPID) { /* child */

    close(pipes.parent);
    if (0 != dup2(pipes.child, 0)) exit (errno);

    fd = creat(filename, S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
    if (1 != dup2(fd, 1)) exit (errno);

    char * args[2] = { GZIP, NULL };
    if (execvp(args[0], args)) exit (errno);

  } else { /* parent */

    close(pipes.child);

  }

  return pipes.parent;
} // end of OOPStape::forkGzip(const char *)


//
// Class name  : OOPStape
// Method name : newRecord(RecordType rTyp, unsigned short *data, int words)
//
// Description : create a new record object
// Input       : rTyp  = record type
//               data  = pointer to data
//               words = number of data words
// Return      : pointer to new QrecordBase object
//

QrecordBase *
OOPStape::newRecord(RecordType rTyp, unsigned short *data, int words)
{
  QrecordBase * qrec = NULL;
  char datetime[32];
  struct tm * tmptr;
  struct utimbuf tbuf;
  time_t t;

  switch (rTyp) {
  case BoR:
    qrec = new OOPSbeginningOfRunRecord(ct, data, words);
    // if (ofd >= 0) { close(ofd); ofd = -1; }
    if (ofd < 0) {
      t = qrec->dateTime();
      tmptr = localtime(&t);
      switch (copyRun) {
      case copyRunUncompressed:
	sprintf(runname, "run%d.dat", qrec->runNumber());
	ofd = creat(runname, S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
	break;
      case copyRunCompressed:
	sprintf(runname, "run%d.dat.gz", qrec->runNumber());
	ofd = forkGzip(runname);
	break;
      case linkRunUncompressed:
	sprintf(runname, "run%d.dat", qrec->runNumber());
	ofd = creat(runname, S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
	sprintf(datetime, "run_%02d%02d%02d%02d%02d%02d",
		tmptr->tm_year, tmptr->tm_mon+1, tmptr->tm_mday,
		tmptr->tm_hour, tmptr->tm_min, tmptr->tm_sec);
	while (access(runname, F_OK)) sleep(0);
	unlink(datetime);
	link(runname, datetime);
	break;
      case linkRunCompressed:
	sprintf(runname, "run%d.dat.gz", qrec->runNumber());
	ofd = forkGzip(runname);
	sprintf(datetime, "run_%02d%02d%02d%02d%02d%02d.gz",
		tmptr->tm_year, tmptr->tm_mon+1, tmptr->tm_mday,
		tmptr->tm_hour, tmptr->tm_min, tmptr->tm_sec);
	while (access(runname, F_OK)) sleep(0);
	unlink(datetime);
	link(runname, datetime);
	break;
      }
      write(sizeof(Qhead)+2*words);
    }
    // if (ofd >= 0) write(sizeof(Qhead)+2*words);
    break;
  case Data:
    qrec = new OOPSdataRecord(ct, data, words, mask);
    break;
  case Comment:
    qrec = new OOPScommentRecord(ct, data, words);
    break;
  case EoR:
    qrec = new OOPSendOfRunRecord(ct, data, words);
  case EoF:
    tbuf.actime = tbuf.modtime = dataTaking;
    if (ofd >= 0) { close(ofd); ofd = -1; }
    if (childPID > 0) {
      wait(NULL);
      childPID = -1;
    }
    sync();
    sleep(0);
    sync();
    sleep(0);
    sync();
    utime(runname, &tbuf);
    break;
  default:
    qrec = NULL;
    break;
  }
  if (qrec) dataTaking = qrec->dateTime();
    
  return qrec;
} // end of OOPStape::newRecord(RecordType, unsigned short *, int)


int
OOPStape::setMask(unsigned int newMask)
{
  mask = newMask;
  return 0;
} // end of OOPStape::setMask(unsigned int)


//
// Class name  : OOPSdataRecord
// Method name : newEvent(unsigned short *data, int words)
//
// Description : create a new event object
// Input       : data = pointer to data
// Output      : words = number of data words
// Return      : pointer to new Qevent object
//

Qevent *
OOPSdataRecord::newEvent(unsigned short *data, int words)
{
  Dptr head = (Dptr) data;
  Qevent * event = NULL;

  if (1<<(head->eventNumber-1) & ~mask) return event;

  switch (head->eventNumber) {
  case  3:
    event = new OOPSevent3(ct, data+2, words-2);
    break;
  case  4:
    event = new OOPSevent4(ct, data+2, words-2);
    break;
  case  5:
    if (isMollerRun) {
      event = new MOLLERevent5(ct, data+2, words-2);
    } else {
      event = new OOPSevent5(ct, data+2, words-2);
    }
    break;
  case  6:
    event = new OOPSevent6(ct, data+2, words-2);
    break;
  case  7:
    event = new OOPSevent7(ct, data+2, words-2);
    break;
  case  8:
    if (saveData && concatBeamEvents) {
      event = new OOPSevent10(ct, saveData, saveLen, 0);
      event->handle();
      delete event; delete saveData; saveData = NULL; saveLen = 0;
    }
    event = new OOPSevent8(ct, data+2, words-2);
    break;
  case 10:
    if (concatBeamEvents) {
      delete saveData;
      saveLen = words-2;
      saveData = (unsigned short *)memcpy(new unsigned short[saveLen], 
					  data+2, saveLen*sizeof(short));
    } else event = new OOPSevent10(ct, data+2, words-2);
    break;
  case 13:
    event = new OOPSevent13(ct, data+2, words-2);
    break;
  case 16:
    event = new MOLLERevent16(ct, data+2, words-2);
    break;
  case 17:
    if (ct->checkDebugFlag())
      printf("OOPSdataRecord::newEvent() moller run detected\n");
    isMollerRun = 1;
  default:
    event = NULL;
    break;
  }

  if (event) event->setDateTime(dataTaking);

  return event;
} // end of OOPSdataRecord::newEvent()


//
// Class name  : OOPSdataRecord
// Method name : handle(void)
//
// Description : handle data record
// Return      :
//

int
OOPSdataRecord::handle()
{
  ct->putIdent(dataTaking);
  if (ct->checkDebugFlag()) printf("OOPSdataRecord::handle()\n");

  return QdataRecord::handle();
} // end of OOPSdataRecord::handle()


//
// Class name  : OOPScommentRecord
// Method name : handle(void)
//
// Description : handle comment record
// Return      :
//

int
OOPScommentRecord::handle()
{
  ct->putIdent(dateTime());
  ct->putComment(str);

  if (ct->checkDebugFlag()) printf("OOPScommentRecord::handle()\n");
  return 0;
} // end of OOPScommentRecord::handle()


//
// Class name  : OOPSbeginningOfRunRecord
// Method name : handle(void)
//
// Description : handle BeginningOfRun record
// Return      :
//

int
OOPSbeginningOfRunRecord::handle()
{
  Qptr header = ((Qptr)buffer)-1;
  char runName[16];
  sprintf(runName, "%02d%02d%02d%02d%02d%02d%c",
	  swapByte(header->date) & 0x007f,
	  (swapByte(header->date) & 0x0780) >> 7,
	  (swapByte(header->date) & 0xf800) >> 11,
	  header->hours, header->minutes, header->seconds, 0);
  ct->putRunName(runName);

  time_t dt = dateTime();
  char oldIdent[64];
  char *cdt = ctime(&dt);
  cdt[strlen(cdt)-1] = 0;
  sprintf(oldIdent, "run%d - %s (Q)", swapByte(header->runNumber), cdt);
  ct->putOldIdent(oldIdent, dt);

  if (str) ct->putComment(str);

  ct->putIdent(dt, putConfigurationAndIdent);

  if (ct->checkDebugFlag()) printf("OOPSbeginningOfRunRecord::handle()\n");
  return 0;
} // end of OOPSbeginningOfRunRecord::handle()


//
// Class name  : OOPSendOfRunRecord
// Method name : handle(void)
//
// Description : handle endOfRun record
// Return      :
//

int
OOPSendOfRunRecord::handle()
{
  time_t dt = dateTime();

  ct->putIdent(dt);
  memcpy(oops1.event, buffer-3, 32*sizeof(Uword32));

  for (int i=0; i<32; i++)
    if (oops1.event[i]) ct->packEventData(&oops1.event[i]);
  ct->putPackedData();

  Qptr header = ((Qptr)buffer)-1;
  char oldIdent[64];
  char *cdt = ctime(&dt);
  cdt[strlen(cdt)-1] = 0;
  sprintf(oldIdent, "run%d - %s (Q)", swapByte(header->runNumber), cdt);
  ct->putOldIdent(oldIdent, dt);

  if (ct->checkDebugFlag()) printf("OOPSendOfRunRecord::handle()\n");
  return 0;
} // end of OOPSendOfRunRecord::handle()


//
// Class name  : OOPSevent3
// Method name : handle(void)
//
// Description : handle event 3
//

int
OOPSevent3::handle()
{
  if (ct->checkDebugFlag()) printf("OOPSevent3::handle()\n");
  data.scaler = 1;
  ct->packEventData(&data.scaler);
  ct->putPackedData();

  return 0;
} // end of OOPSevent3::handle()


//
// Class name  : OOPSevent4
// Method name : handle(void)
//
// Description : handle event 4
//

int
OOPSevent4::handle()
{
  int index;
  unsigned short *us = buffer;

  for (index=1; index<=35; index++) packData(ct, &us, &data.ohips.sca[index]);

  if (ct->checkDebugFlag()) printf("OOPSevent4::handle()\n");
  ct->putPackedData();

  return 0;
} // end of OOPSevent4::handle()


//
// Class name  : OOPSevent5
// Method name : handle(void)
//
// Description : handle event 5
//

int
OOPSevent5::handle()
{
  int index;
  unsigned short * us = buffer;

  packData(ct, &us, &data.bt3bic);
  packData(ct, &us, &data.oopsA.scint[1].left);
  packData(ct, &us, &data.oopsA.scint[1].right);
  packData(ct, &us, &data.oopsA.scint[2].left);
  packData(ct, &us, &data.oopsA.scint[2].right);
  packData(ct, &us, &data.oopsA.scint[3].left);
  packData(ct, &us, &data.oopsA.scint[3].right);
  packData(ct, &us, &data.oopsA.scint[2].low.left);
  packData(ct, &us, &data.oopsA.scint[2].low.right);
  packData(ct, &us, &data.oopsA.scint[1].left_AND_right);
  packData(ct, &us, &data.oopsA.scint[2].left_AND_right);
  packData(ct, &us, &data.oopsA.scint[3].left_AND_right);
  packData(ct, &us, &data.oopsA.prescaler);
  packData(ct, &us, &data.oops.prescalOR);
  packData(ct, &us, &data.oops.retimingOR);
  packData(ct, &us, &data.oops.live);
  packData(ct, &us, &data.computer_busy);
  packData(ct, &us, &data.oopsB.scint[1].left);
  packData(ct, &us, &data.oopsB.scint[1].right);
  packData(ct, &us, &data.oopsB.scint[2].left);
  packData(ct, &us, &data.oopsB.scint[2].right);
  packData(ct, &us, &data.oopsB.scint[3].left);
  packData(ct, &us, &data.oopsB.scint[3].right);
  packData(ct, &us, &data.oopsB.scint[2].low.left);
  packData(ct, &us, &data.oopsB.scint[2].low.right);
  packData(ct, &us, &data.oopsB.scint[1].left_AND_right);
  packData(ct, &us, &data.oopsB.scint[2].left_AND_right);
  packData(ct, &us, &data.oopsB.scint[3].left_AND_right);
  packData(ct, &us, &data.oopsB.prescaler);
  packData(ct, &us, &data.event_5);
  packData(ct, &us, &data.scaler31);
  packData(ct, &us, &data.busclc);
  packData(ct, &us, &data.oops.trigger);
  packData(ct, &us, &data.oopsC.scint[1].left);
  packData(ct, &us, &data.oopsC.scint[1].right);
  packData(ct, &us, &data.oopsC.scint[2].left);
  packData(ct, &us, &data.oopsC.scint[2].right);
  packData(ct, &us, &data.oopsC.scint[3].left);
  packData(ct, &us, &data.oopsC.scint[3].right);
  packData(ct, &us, &data.oopsC.scint[2].low.left);
  packData(ct, &us, &data.oopsC.scint[2].low.right);
  packData(ct, &us, &data.oopsC.scint[1].left_AND_right);
  packData(ct, &us, &data.oopsC.scint[2].left_AND_right);
  packData(ct, &us, &data.oopsC.scint[3].left_AND_right);
  packData(ct, &us, &data.oopsC.prescaler);
  packData(ct, &us, &data.oopsA.and_123);
  packData(ct, &us, &data.oopsA.and_12);
  packData(ct, &us, &data.oopsB.and_123);
  packData(ct, &us, &data.oopsB.and_12);
  packData(ct, &us, &data.oopsC.and_123);
  packData(ct, &us, &data.oopsC.and_12);
  packData(ct, &us, &data.coincidence);
  packData(ct, &us, &data.ohips.scaler53);
  packData(ct, &us, &data.ohips.live54);
  packData(ct, &us, &data.ohips.trigger);
  packData(ct, &us, &data.final_trigger);
  packData(ct, &us, &data.scaler57);
  packData(ct, &us, &data.oopsA.hdc[1].x.top);
  packData(ct, &us, &data.oopsA.hdc[1].x.bottom);
  packData(ct, &us, &data.oopsA.hdc[1].y.left);
  packData(ct, &us, &data.oopsA.hdc[1].y.right);
  packData(ct, &us, &data.oopsA.hdc[2].x.top);
  packData(ct, &us, &data.oopsA.hdc[2].x.bottom);
  packData(ct, &us, &data.oopsA.hdc[2].y.left);
  packData(ct, &us, &data.oopsA.hdc[2].y.right);
  packData(ct, &us, &data.oopsA.hdc[3].x.top);
  packData(ct, &us, &data.oopsA.hdc[3].x.bottom);
  packData(ct, &us, &data.oopsA.hdc[3].y.left);
  packData(ct, &us, &data.oopsA.hdc[3].y.right);
  packData(ct, &us, &data.oopsB.hdc[1].x.top);
  packData(ct, &us, &data.oopsB.hdc[1].x.bottom);
  packData(ct, &us, &data.oopsB.hdc[1].y.left);
  packData(ct, &us, &data.oopsB.hdc[1].y.right);
  packData(ct, &us, &data.oopsB.hdc[2].x.top);
  packData(ct, &us, &data.oopsB.hdc[2].x.bottom);
  packData(ct, &us, &data.oopsB.hdc[2].y.left);
  packData(ct, &us, &data.oopsB.hdc[2].y.right);
  packData(ct, &us, &data.oopsB.hdc[3].x.top);
  packData(ct, &us, &data.oopsB.hdc[3].x.bottom);
  packData(ct, &us, &data.oopsB.hdc[3].y.left);
  packData(ct, &us, &data.oopsB.hdc[3].y.right);
  packData(ct, &us, &data.oopsC.hdc[1].x.top);
  packData(ct, &us, &data.oopsC.hdc[1].x.bottom);
  packData(ct, &us, &data.oopsC.hdc[1].y.left);
  packData(ct, &us, &data.oopsC.hdc[1].y.right);
  packData(ct, &us, &data.oopsC.hdc[2].x.top);
  packData(ct, &us, &data.oopsC.hdc[2].x.bottom);
  packData(ct, &us, &data.oopsC.hdc[2].y.left);
  packData(ct, &us, &data.oopsC.hdc[2].y.right);
  packData(ct, &us, &data.oopsC.hdc[3].x.top);
  packData(ct, &us, &data.oopsC.hdc[3].x.bottom);
  packData(ct, &us, &data.oopsC.hdc[3].y.left);
  packData(ct, &us, &data.oopsC.hdc[3].y.right);
  packData(ct, &us, &data.clock);
  packData(ct, &us, &data.oopsA.sincb);
  packData(ct, &us, &data.sincb);
  packData(ct, &us, &data.retcb);
  packData(ct, &us, &data.live);
  packData(ct, &us, &data.cb);
  packData(ct, &us, &data.oopsB.sincb);
  packData(ct, &us, &data.trig);
  packData(ct, &us, &data.oopsC.sincb);
  packData(ct, &us, &data.oopsA.fired);
  packData(ct, &us, &data.oopsB.fired);
  packData(ct, &us, &data.oopsC.fired);

  if (us < buffer+buflen) { 

    packData(ct, &us, &data.ohips.scaler106);
    packData(ct, &us, &data.ohips.prescal);
    packData(ct, &us, &data.ohips.coincidence);
    packData(ct, &us, &data.ohips.live109);
    packData(ct, &us, &data.ohips.AND_oopsA);
    packData(ct, &us, &data.ohips.AND_oopsB);
    packData(ct, &us, &data.ohips.AND_oopsC);
    packData(ct, &us, &data.ohips.scaler113);
    packData(ct, &us, &data.ohips.scaler114);
    packData(ct, &us, &data.ohips.scaler115);
    packData(ct, &us, &data.ohips.scaler116);
    packData(ct, &us, &data.ohips.scaler117);
    for (index=1; index<=35; index++)
      packData(ct, &us, &data.ohips.sca[index]);
    packData(ct, &us, &data.ohips.event5, 0);
    packData(ct, &us, &data.ohips.bb);
    packData(ct, &us, &data.ohips.bbcb);
    packData(ct, &us, &data.heli_negative.oopsA_prescal);
    packData(ct, &us, &data.heli_negative.oopsB_prescal);
    packData(ct, &us, &data.heli_negative.oopsC_prescal);
    packData(ct, &us, &data.heli_negative.ohips_prescal);
    packData(ct, &us, &data.heli_negative.oops_prescal_OR);
    packData(ct, &us, &data.heli_negative.coincidence);
    packData(ct, &us, &data.heli_negative.oops_live);
    packData(ct, &us, &data.heli_negative.ohips_live);
    packData(ct, &us, &data.heli_negative.oops_trigger);
    packData(ct, &us, &data.heli_negative.ohips_trigger);
    packData(ct, &us, &data.heli_negative.master_trigger);
    packData(ct, &us, &data.heli_positive.oopsA_prescal);
    packData(ct, &us, &data.heli_positive.oopsB_prescal);
    packData(ct, &us, &data.heli_positive.oopsC_prescal);
    packData(ct, &us, &data.heli_positive.ohips_prescal);
    packData(ct, &us, &data.heli_positive.oops_prescal_OR);
    packData(ct, &us, &data.heli_positive.coincidence);
    packData(ct, &us, &data.heli_positive.oops_live);
    packData(ct, &us, &data.heli_positive.ohips_live);
    packData(ct, &us, &data.heli_positive.oops_trigger);
    packData(ct, &us, &data.heli_positive.ohips_trigger);
    packData(ct, &us, &data.heli_positive.master_trigger);
    for (index=36; index<=42; index++)
      packData(ct, &us, &data.ohips.sca[index]);

  }

  if (us < buffer+buflen) { 
    packData(ct, &us, &data.df.scint1);
    packData(ct, &us, &data.df.scint2);
    packData(ct, &us, &data.df.coinc12);
    packData(ct, &us, &data.df.timers);
    packData(ct, &us, &data.df.coincAB);
  }
  
  if (ct->checkDebugFlag()) printf("OOPSevent5::handle()\n");
  ct->putPackedData();

  return 0;
} // end of OOPevent5::handle() 



//
// Class name  : OOPSevent6
// Method name : handle(void)
//
// Description : handle event 6
//
// Scaler configuration for runs later than May 2000
//

int
OOPSevent6::handle()
{
  int index;
  unsigned short * us = buffer;

  packData(ct, &us, &data.oopsA.scint[1].left);
  packData(ct, &us, &data.oopsA.scint[1].right);
  packData(ct, &us, &data.oopsA.scint[2].low.left);
  packData(ct, &us, &data.oopsA.scint[2].left);
  packData(ct, &us, &data.oopsA.scint[2].right);
  packData(ct, &us, &data.oopsA.scint[2].low.right);
  packData(ct, &us, &data.oopsA.scint[3].left);
  packData(ct, &us, &data.oopsA.scint[3].right);
  packData(ct, &us, &data.oopsA.scint[1].left_AND_right);
  packData(ct, &us, &data.oopsA.scint[2].left_AND_right);
  packData(ct, &us, &data.oopsA.scint[3].left_AND_right); //12

  packData(ct, &us, &data.oopsB.scint[1].left);
  packData(ct, &us, &data.oopsB.scint[1].right);
  packData(ct, &us, &data.oopsB.scint[2].low.left);
  packData(ct, &us, &data.oopsB.scint[2].left);
  packData(ct, &us, &data.oopsB.scint[2].right);
  packData(ct, &us, &data.oopsB.scint[2].low.right);
  packData(ct, &us, &data.oopsB.scint[3].left);
  packData(ct, &us, &data.oopsB.scint[3].right);
  packData(ct, &us, &data.oopsB.scint[1].left_AND_right);
  packData(ct, &us, &data.oopsB.scint[2].left_AND_right);
  packData(ct, &us, &data.oopsB.scint[3].left_AND_right); //23

  packData(ct, &us, &data.oopsC.scint[1].left);
  packData(ct, &us, &data.oopsC.scint[1].right);
  packData(ct, &us, &data.oopsC.scint[2].low.left);
  packData(ct, &us, &data.oopsC.scint[2].left);
  packData(ct, &us, &data.oopsC.scint[2].right);
  packData(ct, &us, &data.oopsC.scint[2].low.right);
  packData(ct, &us, &data.oopsC.scint[3].left);
  packData(ct, &us, &data.oopsC.scint[3].right);
  packData(ct, &us, &data.oopsC.scint[1].left_AND_right);
  packData(ct, &us, &data.oopsC.scint[2].left_AND_right);
  packData(ct, &us, &data.oopsC.scint[3].left_AND_right); //34

  packData(ct, &us, &data.oopsD.scint[1].left);
  packData(ct, &us, &data.oopsD.scint[1].right);
  packData(ct, &us, &data.oopsD.scint[2].low.left);
  packData(ct, &us, &data.oopsD.scint[2].left);
  packData(ct, &us, &data.oopsD.scint[2].right);
  packData(ct, &us, &data.oopsD.scint[2].low.right);
  packData(ct, &us, &data.oopsD.scint[3].left);
  packData(ct, &us, &data.oopsD.scint[3].right);
  packData(ct, &us, &data.oopsD.scint[1].left_AND_right);
  packData(ct, &us, &data.oopsD.scint[2].left_AND_right);
  packData(ct, &us, &data.oopsD.scint[3].left_AND_right); //45

  packData(ct, &us, &data.oopsA.and_123);
  packData(ct, &us, &data.oopsA.and_12);
  packData(ct, &us, &data.oopsB.and_123);
  packData(ct, &us, &data.oopsB.and_12);
  packData(ct, &us, &data.oopsC.and_123);
  packData(ct, &us, &data.oopsC.and_12);
  packData(ct, &us, &data.oopsD.and_123);
  packData(ct, &us, &data.oopsD.and_12);
  packData(ct, &us, &data.ohips.AND_oopsA);
  packData(ct, &us, &data.ohips.AND_oopsB);
  packData(ct, &us, &data.ohips.AND_oopsC);
  packData(ct, &us, &data.ohips.AND_oopsD); //57

  packData(ct, &us, &data.oopsA.and_123cb);
  packData(ct, &us, &data.oopsA.and_12cb);
  packData(ct, &us, &data.oopsB.and_123cb);
  packData(ct, &us, &data.oopsB.and_12cb);
  packData(ct, &us, &data.oopsC.and_123cb);
  packData(ct, &us, &data.oopsC.and_12cb);
  packData(ct, &us, &data.oopsD.and_123cb);
  packData(ct, &us, &data.oopsD.and_12cb);
  packData(ct, &us, &data.ohips.AND_oopsAcb);
  packData(ct, &us, &data.ohips.AND_oopsBcb);
  packData(ct, &us, &data.ohips.AND_oopsCcb);
  packData(ct, &us, &data.ohips.AND_oopsDcb); //69

  packData(ct, &us, &data.oopsA.prescaler);
  packData(ct, &us, &data.oopsB.prescaler);
  packData(ct, &us, &data.oopsC.prescaler);
  packData(ct, &us, &data.oopsD.prescaler);
  packData(ct, &us, &data.ohips.pit74);
  packData(ct, &us, &data.ohips.coincidence); //75

  packData(ct, &us, &data.oopsA.prescalercb);
  packData(ct, &us, &data.oopsB.prescalercb);
  packData(ct, &us, &data.oopsC.prescalercb);
  packData(ct, &us, &data.oopsD.prescalercb);
  packData(ct, &us, &data.ohips.pit80cb);
  packData(ct, &us, &data.ohips.coincb); //81

  packData(ct, &us, &data.oops.retimingORcb);
  packData(ct, &us, &data.oops.singleORcb);
  packData(ct, &us, &data.oops.livecb);
  packData(ct, &us, &data.ohips.livecb);
  packData(ct, &us, &data.oops.triggercb);
  packData(ct, &us, &data.ohips.triggercb);
  packData(ct, &us, &data.final_triggercb);
  packData(ct, &us, &data.coincidencecb);
  packData(ct, &us, &data.ohips.pit90cb);
  packData(ct, &us, &data.ohips.prescalcb);

// spare1 and spare2 are utilized for 'ohips read' and 'ohips read (cb)'
// after Aug.17,2000

  packData(ct, &us, &data.ohips.spare1); // 92 Two spare variables to
  packData(ct, &us, &data.ohips.spare2); // 93 accomodate scaler93

  packData(ct, &us, &data.oops.retimingOR);
  packData(ct, &us, &data.oops.singleOR);
  packData(ct, &us, &data.oops.live);
  packData(ct, &us, &data.ohips.live);
  packData(ct, &us, &data.oops.trigger);
  packData(ct, &us, &data.ohips.trigger);
  packData(ct, &us, &data.final_trigger);
  packData(ct, &us, &data.coincidence);
  packData(ct, &us, &data.ohips.pit102); 
  packData(ct, &us, &data.ohips.prescal); //103

  packData(ct, &us, &data.sync);
  packData(ct, &us, &data.synccb);
  packData(ct, &us, &data.cbinh);
  packData(ct, &us, &data.event_6, 0);
  packData(ct, &us, &data.gun);
  packData(ct, &us, &data.guncb);  //108

  // not used additional scaler channels

  packData(ct, &us, &data.oopsA.hdc[1].x.top);  //126
  packData(ct, &us, &data.oopsA.hdc[1].x.bottom);
  packData(ct, &us, &data.oopsA.hdc[1].y.left);
  packData(ct, &us, &data.oopsA.hdc[1].y.right);
  packData(ct, &us, &data.oopsA.hdc[2].x.top);
  packData(ct, &us, &data.oopsA.hdc[2].x.bottom);
  packData(ct, &us, &data.oopsA.hdc[2].y.left);
  packData(ct, &us, &data.oopsA.hdc[2].y.right);
  packData(ct, &us, &data.oopsA.hdc[3].x.top);
  packData(ct, &us, &data.oopsA.hdc[3].x.bottom);
  packData(ct, &us, &data.oopsA.hdc[3].y.left);
  packData(ct, &us, &data.oopsA.hdc[3].y.right); //137

  packData(ct, &us, &data.oopsB.hdc[1].x.top);
  packData(ct, &us, &data.oopsB.hdc[1].x.bottom);
  packData(ct, &us, &data.oopsB.hdc[1].y.left);
  packData(ct, &us, &data.oopsB.hdc[1].y.right);
  packData(ct, &us, &data.oopsB.hdc[2].x.top);
  packData(ct, &us, &data.oopsB.hdc[2].x.bottom);
  packData(ct, &us, &data.oopsB.hdc[2].y.left);
  packData(ct, &us, &data.oopsB.hdc[2].y.right);
  packData(ct, &us, &data.oopsB.hdc[3].x.top);
  packData(ct, &us, &data.oopsB.hdc[3].x.bottom);
  packData(ct, &us, &data.oopsB.hdc[3].y.left);
  packData(ct, &us, &data.oopsB.hdc[3].y.right); //149

  packData(ct, &us, &data.oopsC.hdc[1].x.top);
  packData(ct, &us, &data.oopsC.hdc[1].x.bottom);
  packData(ct, &us, &data.oopsC.hdc[1].y.left);
  packData(ct, &us, &data.oopsC.hdc[1].y.right);
  packData(ct, &us, &data.oopsC.hdc[2].x.top);
  packData(ct, &us, &data.oopsC.hdc[2].x.bottom);
  packData(ct, &us, &data.oopsC.hdc[2].y.left);
  packData(ct, &us, &data.oopsC.hdc[2].y.right);
  packData(ct, &us, &data.oopsC.hdc[3].x.top);
  packData(ct, &us, &data.oopsC.hdc[3].x.bottom);
  packData(ct, &us, &data.oopsC.hdc[3].y.left);
  packData(ct, &us, &data.oopsC.hdc[3].y.right); //161

  packData(ct, &us, &data.oopsD.hdc[1].x.top);
  packData(ct, &us, &data.oopsD.hdc[1].x.bottom);
  packData(ct, &us, &data.oopsD.hdc[1].y.left);
  packData(ct, &us, &data.oopsD.hdc[1].y.right);
  packData(ct, &us, &data.oopsD.hdc[2].x.top);
  packData(ct, &us, &data.oopsD.hdc[2].x.bottom);
  packData(ct, &us, &data.oopsD.hdc[2].y.left);
  packData(ct, &us, &data.oopsD.hdc[2].y.right);
  packData(ct, &us, &data.oopsD.hdc[3].x.top);
  packData(ct, &us, &data.oopsD.hdc[3].x.bottom);
  packData(ct, &us, &data.oopsD.hdc[3].y.left);
  packData(ct, &us, &data.oopsD.hdc[3].y.right); //161

  for (index=1; index<=64; index++)
    packData(ct, &us, &data.ohips.sca[index]);

  for (index=1; index<=17; index++)   // counting room scalers
    packData(ct, &us, &data.crm[index]);

  if (dateTime > 960512052) {   
  for (index=18; index<=24; index++)  
    packData(ct, &us, &data.crm[index]);
  }   

  if (ct->checkDebugFlag()) printf("OOPSevent6::handle()\n");
  ct->putPackedData();

  return 0;
} // end of OOPSevent6::handle()



//
// Function : handleDCOS
//
// Description : handle DCOS data
//

static int
handleDCOS(CindyTree *ct, unsigned short **usp, struct ohips_data & ohips)
{
  int expectedEndFlags = 2;

   if (**usp != 0) {
    // no lam from the dcos controler indicates no data
    if (**usp != 077776)
      fprintf(stderr, "event 8 : (dcos) wrong flag %o\n", **usp);
    return expectedEndFlags;
  }

  //static count = 0;
  //fprintf(stderr, "count : %5d  null : %o  dcos : %2d\n",
  //	  count++, (*usp)[0], (*usp)[1]);
  ++*usp;

  // dcos data length
  packData(ct, usp, &ohips.chamber.dcos);

  unsigned short dcos, subaddress, wire, modul = 0;

  // comment out, because dateTime is not available here for some reason
  if (ohips.chamber.dcos > 60) {
    dcos = 60;
    expectedEndFlags = 0;
  } else {
    if (ohips.chamber.dcos == 60) expectedEndFlags = 1;
    dcos = ohips.chamber.dcos - 1;
  }
    
  for ( ; (dcos > 0) && (**usp != 077776) ; dcos--, ++*usp ) {
    if (**usp & 0x8000) { // data
      subaddress = (**usp & 0x7c00) >> 10;
      if (modul <= 8) { // chamber 1
	if (subaddress < 16) {
	  wire = 16*modul+16-(modul<=4 ? subaddress+1 : 16-subaddress);
	  if (wire >= 160) fprintf(stderr, "u1 wire : %d\n", wire);
	  ohips.chamber.u1[wire] = **usp & 0x3ff;
	  ct->packEventData(&ohips.chamber.u1[wire]);
	} else {
	  wire = 16*modul+(modul>4 ? 31-subaddress : subaddress-16);
	  if (wire >= 160) fprintf(stderr, "v1 wire : %d\n", wire);
	  ohips.chamber.v1[wire] = **usp & 0x3ff;
	  ct->packEventData(&ohips.chamber.v1[wire]);
	}
      } else { // chamber 2
	if (subaddress < 16) {
	  wire = 16*modul+subaddress-128;
	  if (wire >= 224) fprintf(stderr, "u2 wire : %d\n", wire);
	  ohips.chamber.u2[wire] = **usp & 0x3ff;
	  ct->packEventData(&ohips.chamber.u2[wire]);
	} else {
	  wire=16*modul+subaddress-144;
	  if (wire >= 224) fprintf(stderr, "v2 wire : %d\n", wire);
	  ohips.chamber.v2[wire] = **usp & 0x3ff;
	  ct->packEventData(&ohips.chamber.v2[wire]);
	}
      }
    } else { // modul address
      modul = (**usp & 0x7c00) >> 10;
      // fprintf(stderr, "module : %2d\n", modul);
    }
  } 

  return expectedEndFlags;
}


//
// Function : handleDCOS2000
//
// Description : handle DCOS data (after Y2000)
//

static int
handleDCOS2000(CindyTree *ct, unsigned short **usp, struct ohips_data & ohips)
{
  if (**usp != 0) {
    // no lam from the dcos controler indicates no data
    if (((*usp)[0] == 044444) && ((*usp)[1] == 0) && ((*usp)[2] == 055555)) {
      *usp += 3;
    } else fprintf(stderr, "event 8 : (dcos2k) wrong flag %o\n", **usp);
    return 0;
  }
  ++*usp; // skip LAM

  // dcos data length
  packData(ct, usp, &ohips.chamber.dcos);

  unsigned short dcos, subaddress, wire, modul = 0;

  if (ohips.chamber.dcos >= 90) {
    //    fprintf(stderr, "event 8 : (ohips) Hit wires %3d > 90\n",
    //	    ohips.chamber.dcos);
    dcos = 89;
  } else dcos = ohips.chamber.dcos;

  // Chamber 1
  for ( ; (dcos > 0) && (**usp != 077776) ; dcos--, ++*usp) {
    if (**usp & 0x8000) { // data
      if (**usp & 0x200) fprintf(stderr, "DCOS : Overflow in Chamber1\n");
      subaddress = (**usp & 0x7c00) >> 10;
      if (subaddress < 16) { // plane 1
	if (modul <  5) {
	  wire = 16*modul+subaddress+1;
	} else wire = 16*modul+16-subaddress;
	if (wire >= 177) fprintf(stderr, "DCOS : u1 wire number = %d\n", wire);
	ohips.chamber.u1[wire] = **usp & 0x1ff;
	if ( 10 < ohips.chamber.u1[wire] )
	  ct->packEventData(&ohips.chamber.u1[wire]);
      } else { // subaddress >= 16 : plane 2
	if (modul < 5) {
	  wire = 16*modul+subaddress-15;
	} else wire = 16*modul+32-subaddress;
	if (wire >= 177) fprintf(stderr, "DCOS : v1 wire number = %d\n", wire);
	ohips.chamber.v1[wire] = **usp & 0x1ff;
	if ( 10 < ohips.chamber.v1[wire] )
	  ct->packEventData(&ohips.chamber.v1[wire]);
      }
    } else { // modul address
      modul = (**usp & 0x7c00) >> 10;
      if (modul < 16) {
	//	if (~modul & 0x0001)
	//	  fprintf(stderr, "DCOS : Wrong Module # = %3d\n", modul);
	modul = (modul - 1) / 2;
      } else {
	//	if (modul > 19)
	//	  fprintf(stderr, "DCOS : Wrong Module # = %3d\n", modul);
	modul = modul - 9;
      }
    }
  }
  if (dcos>0) { dcos--; ++*usp; }

  // Chamber 2
  for ( ; (dcos > 0) && (**usp != 077776) ; dcos--, ++*usp) {
    if (**usp & 0x8000) { // data
      if ( **usp & 0x200 ) fprintf(stderr, "DCOS : Overflow in Chamber2\n");
      subaddress = (**usp & 0x7c00) >> 10;
      if (subaddress < 16) { // plane 1
	wire = 16*modul+subaddress+1;
	if (wire >= 224) fprintf(stderr, "DCOS : u2 wire number = %d\n", wire);
	if (10 < **usp & 0x1ff) {
	  ohips.chamber.u2[wire] = **usp & 0x1ff;
	  if (10 < ohips.chamber.u2[wire])
	    ct->packEventData(&ohips.chamber.u2[wire]); 
	}
      } else { // subaddress >= 16 : plane 2
	wire = 16*modul+subaddress-15;
	if (wire >= 224) fprintf(stderr, "DCOS : v2 wire number = %d\n", wire);
	if (10 < **usp & 0x1ff) {
	  ohips.chamber.v2[wire] = **usp & 0x1ff;
	  if (10 < ohips.chamber.v2[wire])
	    ct->packEventData(&ohips.chamber.v2[wire]);
	}
      }
    } else { // modul address
      modul = (**usp & 0x7c00) >> 10;
      if (modul < 16) {
	//	if (~modul & 0x0001) 
	//	  fprintf(stderr, "DCOS : Wrong Module # = %3d\n", modul);
	modul = (modul - 1) / 2;
      } else {
	//	if (modul > 20) 
	//	  fprintf(stderr, "DCOS : Wrong Module # = %3d\n", modul); 
	modul = modul - 9; 
      }
    }
  }
  if ((*usp)[0] == 077776) {
    if ((*usp)[1] == 0) ++*usp;
    ++*usp;
  }
  if ((**usp == 044444) || (**usp == 055555)) {
    ++*usp;
  } else fprintf(stderr, "event 8 : (dcos2k) wrong end flag %o\n", **usp);

  return 0;
}

//
// Class name  : OOPSevent7
// Method name : handle(void)
//
// Description : handle event 7
//
// event 7 history:
//
// Wed Jul 17 09:43:36 1996 : OHIPS commissioning starts
//                            only dcos (VDC) data is read out
// Sat Jul 27 15:39:00 1996 : scintillator, cerenkov, and leadglas
//                            counters are added
// Sat Aug 24 11:52:00 1996 : meantimer for scintillators added
//
// Sun Aug 25 21:55:23 1996 : OHIPS commissioning ends
//

int
OOPSevent7::handle()
{
  int index;
  unsigned short **usp = &buffer;

  if (dateTime > 838496340) {  // after Sat Jul 27 15:39:00 1996
    
    packData(ct, usp, &data.ohips.noLAM, 0);
    
    for (index=1; index<=3; index++) {
      packData(ct, usp, &data.ohips.scint[index].adc.left, 0xfff);
      packData(ct, usp, &data.ohips.scint[index].adc.right, 0xfff);
    }
    
    for (index=1; index<=3; index++)
      packData(ct, usp, &data.ohips.cerenkov[index].adc, 0xfff);
    
    for (index=1; index<=14; index++)
      packData(ct, usp, &data.ohips.leadglas[index].adc, 0xfff);
    
    packData(ct, usp, &data.ohips.sum.cerenkov, 0xfff);
    packData(ct, usp, &data.ohips.sum.leadglas, 0xfff);
    
    for (index=1; index<=3; index++) {
      packData(ct, usp, &data.ohips.scint[index].tdc.left, 0xfff);
      packData(ct, usp, &data.ohips.scint[index].tdc.right, 0xfff);
    }
    
    for (index=1; index<=3; index++)
      packData(ct, usp, &data.ohips.cerenkov[index].tdc, 0xfff);
    
    for (index=1; index<=14; index++)
      packData(ct, usp, &data.ohips.leadglas[index].tdc, 0xfff);
    
    // if (dateTime > 840901920) {  // after Sat Aug 24 11:52:00 1996
    if (dateTime > 840013260) {  // after Wed Aug 14 05:01:00 1996
      for (index=1; index<=3; index++)
	packData(ct, usp, &data.ohips.scint[index].meantime, 0xfff);
    }
    
  }
  
  int expectedEndFlags = handleDCOS(ct, usp, data.ohips);
  
  // skip second dcos packet
  // how can this happen anyway?
  if ((expectedEndFlags == 2) && ((*usp)[0] == 077776) &&
      isValid(*usp+(*usp)[1]+1)) *usp += (*usp)[1]+1;
  
  for ( ; expectedEndFlags>0 && isValid(*usp) ; expectedEndFlags-- ) {
    if (**usp != 077776)
      fprintf(stderr, "event 7 : (ohips) wrong end flag %o\n", **usp);
    ++*usp;
  }

  if (ct->checkDebugFlag()) printf("OOPSevent7::handle()\n");
  ct->putPackedData();
  
  return 0;
} // end of OOPSevent7::handle()


//
// Class name  : OOPSevent8
// Method name : packRCOIN(unsigned short ** usp)
//
// Description : handle event 8 (RCOIN subroutine)
//

void
OOPSevent8::packRCOIN(unsigned short ** usp)
{
  int index;

  packData(ct, usp, &data.coincidence.noLAM, 0);

  for (index=1; index<=8; index++)
    packData(ct, usp, &data.coincidence.tdc[index]);

  if (**usp != 074)
    fprintf(stderr, "event 8 : (coincidence) wrong end flag %o\n", **usp);
  ++*usp;

} // end of OOPSevent8::packRCOIN(unsigned short **)


//
// Class name  : OOPSevent8
// Method name : packREADOOPS(unsigned short ** usp, int endFlag)
//
// Description : handle event 8 (READA, READB or READC subroutine)
//

void
OOPSevent8::packREADOOPS(unsigned short ** usp, int endFlag)
{
  struct oops_data * dptr = &data.oopsA;
  int index;

  switch (endFlag) {
  case 012:
    dptr = &data.oopsA;
    break;
  case 024:
    dptr = &data.oopsB;
    break;
  case 036:
    dptr = &data.oopsC;
    break;
  case 050:
    dptr = &data.oopsD;
    break;
  default:
    fprintf(stderr, "event 8 : wrong end flag %o configured\n", endFlag);
    break;
  }

  packData(ct, usp, &(dptr->noLAM), 0);

  for (index=1; index<=3; index++) {
    packData(ct, usp, &(dptr->scint[index].adc.left), 0xfff);
    packData(ct, usp, &(dptr->scint[index].adc.right), 0xfff);
  }

  for (index=1; index<=3; index++) {
    packData(ct, usp, &(dptr->chamber[index].y.adc), 0xfff);
    packData(ct, usp, &(dptr->chamber[index].x.adc), 0xfff);
  }
  
  for (index=1; index<=3; index++) {
    packData(ct, usp, &(dptr->scint[index].tdc.left), 0xfff);
    packData(ct, usp, &(dptr->scint[index].tdc.right), 0xfff);
  }
  packData(ct, usp, &(dptr->scint[2].low.left), 0xfff);
  packData(ct, usp, &(dptr->scint[2].low.right), 0xfff);

  // -mod- 2000/06/15
  // fprintf(stderr, "scint[2].low.right %d\n", dptr->scint[2].low.right);
  // -mod-

  for (index=1; index<=3; index++) {
    packData(ct, usp, &(dptr->chamber[index].y.left), 0xfff);
    packData(ct, usp, &(dptr->chamber[index].y.right), 0xfff);
    packData(ct, usp, &(dptr->chamber[index].x.top), 0xfff);
    packData(ct, usp, &(dptr->chamber[index].x.bottom), 0xfff);
  }
  
  if (**usp != endFlag)
    fprintf(stderr, "event 8 : (oops) wrong end flag %o\n", **usp);
  ++*usp;

  if (**usp == 0177000+endFlag) {
    ++*usp;
    switch (endFlag) {
    case 024:
      for (index=1; index<=6; index++) {
	packData(ct, usp, &(data.leadglas.block[index].adc), 0xfff);
      }
      for (index=1; index<=6; index++) {
	packData(ct, usp, &(data.leadglas.block[index].tdc), 0xfff);
      }
      if (**usp != endFlag)
	fprintf(stderr, "event 8 : (oops) wrong end flag %o\n", **usp);
      ++*usp;
      break;
    case 036:
      for (index=1; index<=3; index++) {
	packData(ct, usp, &(dptr->leadglas[index].adc), 0xfff);
      }
      if (**usp != endFlag)
	fprintf(stderr, "event 8 : (oops) wrong end flag %o\n", **usp);
      ++*usp;
      break;
    }
  }
} // end of OOPSevent8::packREADOOPS(unsigned short **, int)


//
// Class name  : OOPSevent8
// Method name : packREADMISC(unsigned short ** usp)
//
// Description : handle event 8 (READMISC subroutine)
//

void
OOPSevent8::packREADMISC(unsigned short ** usp)
{
  packData(ct, usp, &data.misc.helicity, 0);
  packData(ct, usp, &data.misc.latch2);
  packData(ct, usp, &data.misc.notUsed1);
  packData(ct, usp, &data.misc.notUsed2);

  if (**usp != 0106)
    fprintf(stderr, "event 8 : (misc) wrong end flag %o\n", **usp);
  ++*usp;
} // end of OOPSevent8::packREADMISC(unsigned short **)


//
// Class name  : OOPSevent8
// Method name : packREADTestBPM(unsigned short ** usp)
//
// Description : handle event 8 (READTestBPM subroutine)
//

void
OOPSevent8::packREADTestBPM(unsigned short ** usp)
{
  // readout data only before Jun 8, 2000. Else do nothing.
  if (dateTime < 960512052 ) {
    int index;

    for (index=1; index<=5; index++)
      packData(ct, usp, &data.beam.BPM[index]);

    packData(ct, usp, &data.beam.BeamTAG);

    if (**usp != 0106)
      fprintf(stderr, "event 8 : (beam) wrong end flag %o\n", **usp);
    ++*usp;
  }
} // end of OOPSevent8::packREADTestBPM(unsigned short **)


//
// Class name  : OOPSevent8
// Method name : packREADBEAM(unsigned short ** usp)
//
// Description : handle event 8 (READBEAM subroutine)
//

void
OOPSevent8::packREADBEAM(unsigned short ** usp)
{
  int index;
  // readout data only after Jun 8, 2000. Else do nothing.
  if (dateTime > 960512052 ) {

    packData(ct, usp, &data.LATCHtdc.noLAM, 0);

    for (index=1; index<=4; index++) 
      packData(ct, usp, &data.beam.BPM[index]);

      packData(ct, usp, &data.beam.BPM[5], 0xfff);

    for (index=1; index<=3; index++) 
      packData(ct, usp, &data.beam.BPM2[index], 0xfff);

    if (dateTime <968336415) 
    packData(ct, usp, &data.beam.BeamTAG);
  
    packData(ct, usp, &data.latch2);

    packData(ct, usp, &data.LATCHtdc.oopsA, 0xfff);
    packData(ct, usp, &data.LATCHtdc.oopsB, 0xfff);
    packData(ct, usp, &data.LATCHtdc.oopsC, 0xfff);
    packData(ct, usp, &data.LATCHtdc.oopsD, 0xfff);
    packData(ct, usp, &data.LATCHtdc.ohips, 0xfff);
    packData(ct, usp, &data.LATCHtdc.coin, 0xfff);
    packData(ct, usp, &data.LATCHtdc.sync, 0xfff);
    packData(ct, usp, &data.LATCHtdc.CB, 0xfff);
    
    if (**usp != 0106) 
      fprintf(stderr, "event 8 : (beam) wrong end flag %o\n", **usp);
    ++*usp;
 }
} // end of OOPSevent8::packREADBEAM(unsigned short **)


//
// Class name  : OOPSevent8
// Method name : packREADO(unsigned short ** usp)
//
// Description : handle event 8 (READO subroutine)
//

void
OOPSevent8::packREADO(unsigned short ** usp)
{
  int index;

  packData(ct, usp, &data.ohips.noLAM, 0);

  if (dateTime < 953657798) {  // before Tue Mar 21 11:56:38 2000
    
    for (index=1; index<=3; index++) {
      packData(ct, usp, &data.ohips.scint[index].adc.left, 0xfff);
      packData(ct, usp, &data.ohips.scint[index].adc.right, 0xfff);
    }
    
    for (index=1; index<=3; index++)
      packData(ct, usp, &data.ohips.cerenkov[index].adc, 0xfff);
    
    if (dateTime < 849762000) { // before Dec 12, 1996 read leadglas
      for (index=1; index<=14; index++)
	packData(ct, usp, &data.ohips.leadglas[index].adc, 0xfff);
    }
    
    packData(ct, usp, &data.ohips.sum.cerenkov, 0xfff);
    packData(ct, usp, &data.ohips.sum.leadglas, 0xfff);
    
    for (index=1; index<=3; index++) {
      packData(ct, usp, &data.ohips.scint[index].tdc.left, 0xfff);
      packData(ct, usp, &data.ohips.scint[index].tdc.right, 0xfff);
    }
    
    for (index=1; index<=3; index++)
      packData(ct, usp, &data.ohips.cerenkov[index].tdc, 0xfff);
    
    if (dateTime < 849762000) { // before Dec 12, 1996 read leadglas
      for (index=1; index<=14; index++)
	packData(ct, usp, &data.ohips.leadglas[index].tdc, 0xfff);
      for (index=1; index<=3; index++)
	packData(ct, usp, &data.ohips.scint[index].meantime, 0xfff);
    } else {
      packData(ct, usp, &data.ohips.scint[2].meantime, 0xfff);
    }
    
    if (**usp == 0177123) {
      ++*usp;
      for (index=1; index<=14; index++)
	packData(ct, usp, &data.ohips.leadglas[index].adc, 0xfff);
      for (index=1; index<=14; index++)
	packData(ct, usp, &data.ohips.leadglas[index].tdc, 0xfff);
    }
    
    // checking ohips end flag is a pain
    // every valid dcos event should have TWO end flags
    // but there are exceptions - a lot of exceptions
    // to have some sort of check I require only one - somewhere
    
    int expectedEndFlags = handleDCOS(ct, usp, data.ohips);
    
    while (isValid(*usp) && (expectedEndFlags>0)) {
      if (**usp == 077776) expectedEndFlags = 0;
      ++*usp;
    }
    
    if (expectedEndFlags)
      fprintf(stderr, "event 8 : (ohips) wrong end flag\n");
    
  }  else { // after  Tue Mar 21 11:56:38 2000
    
    // ADCs
    
    for (index=1; index<=4; index++) {
      packData(ct, usp, &data.ohips.scint1[index].adc.left, 0xfff);
      packData(ct, usp, &data.ohips.scint1[index].adc.right, 0xfff);
    }
    
    for (index=2; index<=3; index++) {
      packData(ct, usp, &data.ohips.scint[index].adc.left, 0xfff);
      packData(ct, usp, &data.ohips.scint[index].adc.right, 0xfff);
    }
    
    for (index=1; index<=3; index++)
      packData(ct, usp, &data.ohips.cerenkov[index].adc, 0xfff);
    
    for (index=1; index<=18; index++)
      packData(ct, usp, &data.ohips.leadglas[index].adc, 0xfff);
    
    packData(ct, usp, &data.ohips.sum.cerenkov, 0xfff);
    packData(ct, usp, &data.ohips.sum.leadglas, 0xfff);
    
    // TDCs
    
    for (index=1; index<=4; index++) {
      packData(ct, usp, &data.ohips.scint1[index].tdc.left, 0xfff);
      packData(ct, usp, &data.ohips.scint1[index].tdc.right, 0xfff);
    }
    
    for (index=2; index<=3; index++) {
      packData(ct, usp, &data.ohips.scint[index].tdc.left, 0xfff);
      packData(ct, usp, &data.ohips.scint[index].tdc.right, 0xfff);
    }

    for (index=1; index<=3; index++)
      packData(ct, usp, &data.ohips.cerenkov[index].tdc, 0xfff);
    
    for (index=1; index<=18; index++)
      packData(ct, usp, &data.ohips.leadglas[index].tdc, 0xfff);

    handleDCOS2000(ct, usp, data.ohips);

  } // after  Tue Mar 21 11:56:38 2000
  
  
} // end of OOPSevent8::packREADO(unsigned short **)


//
// Class name  : OOPSevent8
// Method name : handle(void)
//
// Description : handle event 8
//

int
OOPSevent8::handle()
{
  unsigned short * us = buffer;

  //static count = 0;
  //fprintf(stderr, "count : %5d  buflen : %3d\n", count++, buflen);

  // BeamTAG scaler is read here after September 07, 2000 
  if (dateTime > 968336415) 
    packData(ct, &us, &data.BeamTAGshort);

  packData(ct, &us, &data.latch1, 0);
  packData(ct, &us, &data.flag, 0);

  switch (data.flag) {
  case 0101:
    packREADOOPS(&us, 012);
    packRCOIN(&us);
    packREADMISC(&us);
    packREADO(&us);
    break;
  case 0102:
    packREADOOPS(&us, 024);
    packRCOIN(&us);
    packREADMISC(&us);
    packREADO(&us);
    break;
  case 0103:
    packREADOOPS(&us, 036);
    packRCOIN(&us);
    packREADMISC(&us);
    packREADO(&us);
    break;
  case 05:		// July/August '96 OOPS commissioning
  case 0105:
    packREADOOPS(&us, 012);
    packREADMISC(&us);
    break;
  case 06:		// July/August '96 OOPS commissioning
  case 0106:
    packREADOOPS(&us, 024);
    packREADMISC(&us);
    break;
  case 07:		// July/August '96 OOPS commissioning
  case 0107:
    packREADOOPS(&us, 036);
    packREADMISC(&us);
    break;
  case 0111:
    packREADMISC(&us);
    packREADO(&us);
    break;
  case 0112:
    packREADOOPS(&us, 012);
    packREADOOPS(&us, 024);
    packRCOIN(&us);
    packREADMISC(&us);
    packREADO(&us);
    break;
  case 0113:
    packREADOOPS(&us, 012);
    packREADOOPS(&us, 036);
    packRCOIN(&us);
    packREADMISC(&us);
    packREADO(&us);
    break;
  case 0115:
    packREADOOPS(&us, 024);
    packREADOOPS(&us, 036);
    packRCOIN(&us);
    packREADMISC(&us);
    packREADO(&us);
    break;
  case 0120:
    packREADOOPS(&us, 012);
    packREADOOPS(&us, 024);
    packREADOOPS(&us, 036);
    packRCOIN(&us);
    packREADMISC(&us);
    packREADO(&us);
    break;
  case 025:		// July/August '96 OOPS commissioning
  case 0125:
    packREADOOPS(&us, 012);
    packREADOOPS(&us, 024);
    packREADMISC(&us);
    break;
  case 026:		// July/August '96 OOPS commissioning
  case 0126:
    packREADOOPS(&us, 012);
    packREADOOPS(&us, 036);
    packREADMISC(&us);
    break;
  case 030:		// July/August '96 OOPS commissioning
  case 0130:
    packREADOOPS(&us, 024);
    packREADOOPS(&us, 036);
    packREADMISC(&us);
    break;
  case 033:		// July/August '96 OOPS commissioning
  case 0133:
    packREADOOPS(&us, 012);
    packREADOOPS(&us, 024);
    packREADOOPS(&us, 036);
    packREADMISC(&us);
    break;

// Data type flags for runs later than May 2000

  case 0201:
    packREADOOPS(&us, 012);
    packRCOIN(&us);
    packREADO(&us);
    packREADBEAM(&us);
    break;
  case 0202:
    packREADOOPS(&us, 024);
    packRCOIN(&us);
    packREADO(&us);
    packREADBEAM(&us);
    break;
  case 0203:
    packREADOOPS(&us, 036);
    packRCOIN(&us);
    packREADO(&us);
    packREADBEAM(&us);
    break;
  case 0204:
    packREADOOPS(&us, 050);
    packRCOIN(&us);
    packREADO(&us);
    packREADBEAM(&us);
    break;
  case 0205:
    packREADOOPS(&us, 012);
    packREADBEAM(&us);
    break;
  case 0206:
    packREADOOPS(&us, 024);
    packREADBEAM(&us);
    break;
  case 0207:
    packREADOOPS(&us, 036);
    packREADBEAM(&us);
    break;
  case 0210:
    packREADOOPS(&us, 050);
    packREADBEAM(&us);
    break;
  case 0211:
    packREADTestBPM(&us);
    packREADO(&us);
    packREADBEAM(&us);
    break;
  case 0212:
    packREADOOPS(&us, 012);
    packREADOOPS(&us, 024);
    packRCOIN(&us);
    packREADO(&us);
    packREADBEAM(&us);
    break;
  case 0213:
    packREADOOPS(&us, 012);
    packREADOOPS(&us, 036);
    packRCOIN(&us);
    packREADO(&us);
    packREADBEAM(&us);
    break;
  case 0214:
    packREADOOPS(&us, 012);
    packREADOOPS(&us, 050);
    packRCOIN(&us);
    packREADO(&us);
    packREADBEAM(&us);
    break;
  case 0215:
    packREADOOPS(&us, 024);
    packREADOOPS(&us, 036);
    packRCOIN(&us);
    packREADO(&us);
    packREADBEAM(&us);
    break;
  case 0216:
    packREADOOPS(&us, 024);
    packREADOOPS(&us, 050);
    packRCOIN(&us);
    packREADO(&us);
    packREADBEAM(&us);
    break;
  case 0217:
    packREADOOPS(&us, 036);
    packREADOOPS(&us, 050);
    packRCOIN(&us);
    packREADO(&us);
    packREADBEAM(&us);
    break;
  case 0220:
    packREADOOPS(&us, 012);
    packREADOOPS(&us, 024);
    packREADOOPS(&us, 036);
    packRCOIN(&us);
    packREADO(&us);
    packREADBEAM(&us);
    break;
  case 0221:
    packREADOOPS(&us, 012);
    packREADOOPS(&us, 024);
    packREADOOPS(&us, 050);
    packRCOIN(&us);
    packREADO(&us);
    packREADBEAM(&us);
    break;
  case 0222:
    packREADOOPS(&us, 012);
    packREADOOPS(&us, 036);
    packREADOOPS(&us, 050);
    packRCOIN(&us);
    packREADO(&us);
    packREADBEAM(&us);
    break;
  case 0223:
    packREADOOPS(&us, 024);
    packREADOOPS(&us, 036);
    packREADOOPS(&us, 050);
    packRCOIN(&us);
    packREADO(&us);
    packREADBEAM(&us);
    break;
  case 0224:
    packREADOOPS(&us, 012);
    packREADOOPS(&us, 024);
    packREADOOPS(&us, 036);
    packREADOOPS(&us, 050);
    packRCOIN(&us);
    packREADO(&us);
    packREADBEAM(&us);
    break;
  case 0225:
    packREADOOPS(&us, 012);
    packREADOOPS(&us, 024);
    packREADBEAM(&us);
    break;
  case 0226:
    packREADOOPS(&us, 012);
    packREADOOPS(&us, 036);
    packREADBEAM(&us);
    break;
  case 0227:
    packREADOOPS(&us, 012);
    packREADOOPS(&us, 050);
    packREADBEAM(&us);
    break;
  case 0230:
    packREADOOPS(&us, 024);
    packREADOOPS(&us, 036);
    packREADBEAM(&us);
    break;
  case 0231:
    packREADOOPS(&us, 024);
    packREADOOPS(&us, 050);
    packREADBEAM(&us);
    break;
  case 0232:
    packREADOOPS(&us, 036);
    packREADOOPS(&us, 050);
    packREADBEAM(&us);
    break;
  case 0233:
    packREADOOPS(&us, 012);
    packREADOOPS(&us, 024);
    packREADOOPS(&us, 036);
    packREADBEAM(&us);
    break;
  case 0234:
    packREADOOPS(&us, 012);
    packREADOOPS(&us, 024);
    packREADOOPS(&us, 050);
    packREADBEAM(&us);
    break;
  case 0235:
    packREADOOPS(&us, 012);
    packREADOOPS(&us, 036);
    packREADOOPS(&us, 050);
    packREADBEAM(&us);
    break;
  case 0236:
    packREADOOPS(&us, 024);
    packREADOOPS(&us, 036);
    packREADOOPS(&us, 050);
    packREADBEAM(&us);
    break;
  case 0237:
    packREADOOPS(&us, 012);
    packREADOOPS(&us, 024);
    packREADOOPS(&us, 036);
    packREADOOPS(&us, 050);
    packREADBEAM(&us);
    break;
  case 0177777:
    packREADMISC(&us);
    break;
  case 0177770:  // Junk Event Flag for OOPS2000 unpolarized series
    packREADBEAM(&us);
    break;

  default:
    fprintf(stderr, "unknown event8 flag : %o\n", data.flag);
    break;
  }

  if (ct->checkDebugFlag()) printf("OOPSevent8::handle()\n");
  ct->putPackedData();

  return 0;
} // end of OOPSevent8::handle()


//
// Class name  : OOPSevent10
// Method name : handle(void)
//
// Description : handle event 10
//

int
OOPSevent10::handle()
{
  unsigned short * us = (unsigned short *) buffer;
  
  packData(ct, &us, &data.flag);
  packData(ct, &us, &data.helicity, 0);
  packData(ct, &us, &data.timeslot, 0);
  //if (us[0] || us[1]) {
  data.toroid.bt1 = us[0]; data.toroid.bt2 = us[1];
  ct->packEventData(&data.toroid.bt1, 2);
  //}
  us += 2;
  if (us[0] || us[1] || us[2] || us[3]) {
    data.position.molx = us[0]; data.position.moly = us[1];
    data.position.tgtx = us[2]; data.position.tgty = us[3];
    ct->packEventData(&data.position.molx, 4);
  }
  us += 4;
  if (us[0] || us[1]) {
    data.halo.halo1 = us[0]; data.halo.halo2 = us[1];
    ct->packEventData(&data.halo.halo1, 2);
  }
  us += 2;
  
  if (us < buffer+buflen) {
    
    // begin of bit24
    packData(ct, &us, &data.bit24.oapre);
    packData(ct, &us, &data.bit24.obpre);
    packData(ct, &us, &data.bit24.ocpre);
    packData(ct, &us, &data.bit24.hpre);
    packData(ct, &us, &data.bit24.opreor);
    packData(ct, &us, &data.bit24.coin10);
    packData(ct, &us, &data.bit24.oliv);
    packData(ct, &us, &data.bit24.hliv);
    packData(ct, &us, &data.bit24.otrig);
    packData(ct, &us, &data.bit24.htrig);
    packData(ct, &us, &data.bit24.mtrig);
    // end of bit24
    
    if (us[0] || us[1] || us[2] || us[3]) {
      data.reserved.scaler12 = us[0]; data.reserved.scaler13 = us[1];
      data.reserved.scaler14 = us[2]; data.reserved.scaler15 = us[3];
      ct->packEventData(&data.reserved.scaler12, 4);
    }
    us += 4;
    
  }
  
  if (ct->checkDebugFlag()) printf("OOPSevent10::handle()\n");
  if (doPackData) ct->putPackedData();
  
  return 0;
} // end of OOPSevent10::handle()


//
// Class name  : OOPSevent13
// Method name : handle(void)
//
// Description : handle event 13
//

int
OOPSevent13::handle()
{
  char str[256], am_pm;
  int i, year, month, day, hour, minute, second;

  for (i=0; i<buflen && buffer[i]!=0xffff; i++) str[i] = buffer[i];
  str[i] = 0;

  if ((12 == sscanf(str, "%d/%d %d:%d:%d %cM %f %f %f %f %f %f",
		    &month, &day, &hour, &minute, &second, &am_pm,
		    &data.top.pressure, &data.bottom.pressure,
		    &data.top.cellTemp, &data.top.exchTemp,
		    &data.bottom.cellTemp, &data.bottom.exchTemp)) ||
      (12 == sscanf(str, "%d/%d/ %d:%d:%d %cM %f %f %f %f %f %f",
		    &month, &day, &hour, &minute, &second, &am_pm,
		    &data.top.pressure, &data.bottom.pressure,
		    &data.top.cellTemp, &data.top.exchTemp,
		    &data.bottom.cellTemp, &data.bottom.exchTemp)) ||
      (13 == sscanf(str, "%d/%d/%d %d:%d:%d %cM %f %f %f %f %f %f",
		    &month, &day, &year, &hour, &minute, &second, &am_pm,
		    &data.top.pressure, &data.bottom.pressure,
		    &data.top.cellTemp, &data.top.exchTemp,
		    &data.bottom.cellTemp, &data.bottom.exchTemp))) {
    data.date.month  = month;
    data.date.day    = day;
    data.date.hour   = hour;
    data.date.minute = minute;
    data.date.second = second;
    if (am_pm == 'P') data.date.hour += 12;
    if (ct->checkDebugFlag()) printf("event13 : %s\n", str);

    ct->packEventData(&data.date, 5);
    ct->packEventData(&data.top.pressure, 3);
    ct->packEventData(&data.bottom.pressure, 3);
    ct->putPackedData();
  }
  
  return 0;
} // end of OOPSevent13::handle()


//
// Class name  : MOLLERevent5
// Method name : handle(void)
//
// Description : handle moller event 5
//

int
MOLLERevent5::handle()
{
  int i;
  unsigned short * us = (unsigned short *) buffer;
  
  packData(ct, &us, &data.helicity);
  packData(ct, &us, &data.timeslot, 0);
  packData(ct, &us, &data.qshunt);
  packData(ct, &us, &data.scaler.cerenkov1);
  packData(ct, &us, &data.scaler.cerenkov2);
  packData(ct, &us, &data.scaler.early);
  packData(ct, &us, &data.scaler.ontime);
  packData(ct, &us, &data.scaler.late);
  packData(ct, &us, &data.scaler.spare);
  packData(ct, &us, &data.spare);
  packData(ct, &us, &data.clock);
  packData(ct, &us, &data.flag);
  packData(ct, &us, &data.intCer1);
  packData(ct, &us, &data.intCer2);
  packData(ct, &us, &data.intBT1);
  packData(ct, &us, &data.intBT2);
  if (us[0] || us[1] || us[2] || us[3]) {
    data.position.molx = us[0]; data.position.moly = us[1];
    data.position.tgtx = us[2]; data.position.tgty = us[3];
    ct->packEventData(&data.position.molx, 4);
  }
  us += 4;
  if (us[0] || us[1]) {
    data.halo.moller = us[0]; data.halo.target = us[1];
    ct->packEventData(&data.halo.moller, 2);
  }
  us += 2;
  for (i=0; i<9; i++) packData(ct, &us, &data.tdc[i]);
  packData(ct, &us, &data.event5);
  packData(ct, &us, &data.prescaled5);

  if (ct->checkDebugFlag()) printf("MOLLERevent5::handle()\n");
  ct->putPackedData();

  return 0;
} // end of MOLLERevent5::handle()


//
// Class name  : MOLLERevent16
// Method name : handle(void)
//
// Description : handle moller event 16
//

int
MOLLERevent16::handle()
{
  unsigned short * us = (unsigned short *) buffer;
  
  packData(ct, &us, &data.qshunt);
  packData(ct, &us, &data.intvdt);
  packData(ct, &us, &data.current);
  packData(ct, &us, &data.scaler[1]);
  packData(ct, &us, &data.scaler[2]);
  packData(ct, &us, &data.scaler[3]);
  packData(ct, &us, &data.scaler[4]);
  packData(ct, &us, &data.scaler[5]);
  packData(ct, &us, &data.scaler[6]);
  packData(ct, &us, &data.scaler[19]);
  packData(ct, &us, &data.scaler[20]);

  if (ct->checkDebugFlag()) printf("MOLLERevent16::handle()\n");
  ct->putPackedData();

  return 0;
} // end of MOLLERevent16::handle()
