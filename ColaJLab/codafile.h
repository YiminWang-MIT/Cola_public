//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaJLab/codafile.h,v 1.1.1.1 1999-01-24 21:44:07 distler Exp $
//
// Classes to read coda files
//

#ifndef __CODAFILE_H__
#define __CODAFILE_H__

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <time.h>

/**The header of a physical record. According to the CODA User's Manual 1.4
 * the last two fields are reserved. It turns out that in reality they
 * contain the total number of events and a magic word, respectively.
 */
struct RecordHeader {
  unsigned int size;
  unsigned int number;
  unsigned int len;
  unsigned int start;
  unsigned int used;
  unsigned int version;
  unsigned int reserved;
  unsigned int magic;
};

/**The header of a bank. This definition of a two longword header is shared
 * by all bank and event headers.
 */
struct BankHeader {
  unsigned int length;
  unsigned short tag;
  unsigned char  dtype;
  unsigned char  num;
};

const int maxBuffer = 256*1028;

/**Base class definition.
 */
class BankBase {
protected:
  struct BankHeader * buffer;
  struct BankHeader * bufend;
public:
  ///Constructor. Only parameter is a pointer to the bank header.
  BankBase(struct BankHeader * bank);
  ///Virtual destructor. Nothing to be done.
  virtual ~BankBase() { ; }
  /**Print the header information to stdout. Function evaluates the
   * length field of the header and returns a pointer to the next bank.
   */
  virtual struct BankHeader * print();
  /**Virtual function for handling the bank data.
   * Returns a pointer to the next bank.
   */
  virtual struct BankHeader * handle();
}; // end of class BankBase

/**This class handles the event id bank, created by the event builder.
 * The bank data contains the event number, a event classification,
 * and a status summary.
 */
class EventIdBank : public BankBase {
public:
  ///Constructor. Only parameter is a pointer to the bank header.
  EventIdBank(struct BankHeader * bank) : BankBase(bank) { ; }
  ///Virtual destructor. Nothing to be done.
  virtual ~EventIdBank() { ; }
  /**Print the header information to stdout. Function evaluates the
   * length field of the header and returns a pointer to the next bank.
   */
  virtual struct BankHeader * print();
}; // end of class EventIdBank

/**This class handles the Readout Controller data banks.
 */
class ROCDataBank : public BankBase {
public:
  ///Constructor. Only parameter is a pointer to the bank header.
  ROCDataBank(struct BankHeader * bank) : BankBase(bank) { ; }
  ///Virtual destructor. Nothing to be done.
  virtual ~ROCDataBank() { ; }
  /**Print the header information to stdout. Function evaluates the
   * length field of the header and returns a pointer to the next bank.
   */
  virtual struct BankHeader * print();
}; // end of class ROCDataBank

/**Base class definition.
 */
class EventBase {
protected:
  struct BankHeader * buffer;
  struct BankHeader * bufend;
public:
  ///Constructor. Only parameter is a pointer to the event header.
  EventBase(struct BankHeader * bank);
  ///Virtual destructor. Nothing to be done.
  virtual ~EventBase() { ; }
  /**Print the header information to stdout. Function evaluates the
   * length field of the header and returns a pointer to the next event.
   */
  virtual struct BankHeader * print();
  /**Virtual function for handling the event data.
   * Returns a pointer to the next event.
   */
  virtual struct BankHeader * handle();
  ///Function returns a pointer to the next event header.
  struct BankHeader * nextEvent() { return bufend; }
}; // end of class EventBase

/**This class handles synchronization events. These events are generated
 * automatically by the trigger supervisor, or upon operator command.
 * Sync events contain a time stamp in unix format, event numbers, and
 * status information.
 */
class SyncEvent : public EventBase {
public:
  ///Constructor. Only parameter is a pointer to the event header.
  SyncEvent(struct BankHeader * bank) : EventBase(bank) { ; }
  ///Virtual destructor. Nothing to be done.
  virtual ~SyncEvent() { ; }
  /**Print the header information to stdout. Function evaluates the
   * length field of the header and returns a pointer to the next event.
   */
  virtual struct BankHeader * print();
}; // end of class SyncEvent

/**This class handles pre start events. These events are generated
 * during the transition from the downloaded state to the prestarted state.
 * PreStart events contain a time stamp in unix format, the run number,
 * and the run type.
 */
class PreStartEvent : public EventBase {
public:
  ///Constructor. Only parameter is a pointer to the event header.
  PreStartEvent(struct BankHeader * bank) : EventBase(bank) { ; }
  ///Virtual destructor. Nothing to be done.
  virtual ~PreStartEvent() { ; }
  /**Print the header information to stdout. Function evaluates the
   * length field of the header and returns a pointer to the next event.
   */
  virtual struct BankHeader * print();
}; // end of class PreStartEvent

/**This class handles go events. These events are generated
 * for each start or resume command. Go events contain a time stamp
 * in unix format and the number of events in the run so far.
 */
class GoEvent : public EventBase {
public:
  ///Constructor. Only parameter is a pointer to the event header.
  GoEvent(struct BankHeader * bank) : EventBase(bank) { ; }
  ///Virtual destructor. Nothing to be done.
  virtual ~GoEvent() { ; }
  /**Print the header information to stdout. Function evaluates the
   * length field of the header and returns a pointer to the next event.
   */
  virtual struct BankHeader * print();
}; // end of class GoEvent

/**This class handles pause events. These events are generated
 * for each transition from the active state to the paused state.
 * Pause events contain a time stamp in unix format and the
 * number of events in the run so far.
 */
class PauseEvent : public EventBase {
public:
  ///Constructor. Only parameter is a pointer to the event header.
  PauseEvent(struct BankHeader * bank) : EventBase(bank) { ; }
  ///Virtual destructor. Nothing to be done.
  virtual ~PauseEvent() { ; }
  /**Print the header information to stdout. Function evaluates the
   * length field of the header and returns a pointer to the next event.
   */
  virtual struct BankHeader * print();
}; // end of class PauseEvent

/**This class handles end events. These events are generated
 * for each time a run is ended. End events contain a time stamp
 * in unix format and the number of events in the run.
 */
class EndEvent : public EventBase {
public:
  ///Constructor. Only parameter is a pointer to the event header.
  EndEvent(struct BankHeader * bank) : EventBase(bank) { ; }
  ///Virtual destructor. Nothing to be done.
  virtual ~EndEvent() { ; }
  /**Print the header information to stdout. Function evaluates the
   * length field of the header and returns a pointer to the next event.
   */
  virtual struct BankHeader * print();
}; // end of class EndEvent

/**This class handles all standard physics events.
 * These events consist of an outermost bank with a tag equal to
 * the event type and a number of data banks.
 */
class PhysicsEvent : public EventBase {
protected:
  virtual BankBase * createBank(struct BankHeader * bank);
public:
  ///Constructor. Only parameter is a pointer to the event header.
  PhysicsEvent(struct BankHeader * bank) : EventBase(bank) { ; }
  ///Virtual destructor. Nothing to be done.
  virtual ~PhysicsEvent() { ; }
  /**Print the header information to stdout. Function evaluates the
   * length field of the header and returns a pointer to the next event.
   */
  virtual struct BankHeader * print();
  /**Virtual function for handling the event data. Function will recursively
   * call the handle function for all banks contained.
   * Returns a pointer to the next event.
   */
  virtual struct BankHeader * handle();
}; // end of class PhysicsEvent

/**Structure holding file decriptors.
 * Used when a subprocess for decompression is created.
 */
struct pipes {
    int parent_in;
    int child_out;
    int child_in;
    int parent_out;
};

/**Base class definition. Handles general file input/output.
 * It recognizes compressed files and automatically creates a
 * subprocess to do the decompression.
 */
class FileBase {
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
  ///Constructor. Only parameter is a file descriptor.
  FileBase(int fdesc);
  ///Virtual destructor. Cleans up the memory. Deletes dynamic variables.
  virtual ~FileBase();
}; // end of class FileBase

/**Class definition.
 */
class CodaFile : public FileBase {
protected:
  struct RecordHeader * header;
  struct BankHeader * bankstart;
  struct BankHeader * bankend;
  struct BankHeader * tempbank;
  struct BankHeader * tempcont;
  virtual EventBase * createEvent(struct BankHeader * bank);
public:
  /// Constructor. Initialize object.
  CodaFile(int ifd);

  /// Destructor. Free memory.
  virtual ~CodaFile() { ; }

  /**Get next physical record. Returns -1 if end of file is reached or
   * if some error occured
   */ 
  virtual int nextRecord();

  /**Print the header information to stdout.
   * The function will recursively call the print function of the
   * events and banks that are contained in the actual record.
   */
  virtual int print();

  /**Handle the record data.
   * The function will recursively call the handle function of the
   * events and banks that are contained in the actual record.
   */
  virtual int handle();

}; // end of class CodaFile


#endif /* __CODAFILE_H__ */
