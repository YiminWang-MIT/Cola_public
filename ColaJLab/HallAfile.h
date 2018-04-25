//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaJLab/HallAfile.h,v 1.2 1999-02-03 23:00:44 distler Exp $
//
// Classes to read hall A coda files
//

#ifndef __HALLAFILE_H__
#define __HALLAFILE_H__

#include "Cindy/Cindy++.h"
#include "Digitizer.h"
#include "codafile.h"
#include "hrs.h"

// constants for use with CindyTree::putIdent(time_t, int)
const int putIdentOnly             = 0;
const int putConfigurationAndIdent = 1;

// constants for use with OOPStape::OOPStape(CindyTree *, int, int)
const int donotCopyRun             = 0;
const int copyRunUncompressed      = 1;
const int copyRunCompressed        = 2;
const int linkRunUncompressed      = 3;
const int linkRunCompressed        = 4;

/**This class handles the event id bank, created by the event builder.
 * The bank data contains the event number, a event classification,
 * and a status summary.
 */
class EventIdBankHallA : public EventIdBank {
protected:
  CindyTree * ct;
public:
  ///Constructor. Only parameter is a pointer to the bank header.
  EventIdBankHallA(CindyTree * ctree, struct BankHeader * bank)
    : EventIdBank(bank) { ct = ctree; }
  ///Virtual destructor. Nothing to be done.
  virtual ~EventIdBankHallA() { ; }
  /**Handle the bank data. Function evaluates the length field
   * of the header and returns a pointer to the next bank.
   */
  virtual struct BankHeader * handle();
}; // end of class EventIdBankHallA

/**This class handles the Readout Controller data banks.
 */
class ROCDataBankHallA : public ROCDataBank {
  DigitBase ***digit;
protected:
  CindyTree * ct;
public:
  ///Constructor. Only parameter is a pointer to the bank header.
  ROCDataBankHallA(CindyTree * ctree, struct BankHeader * bank);
  ///Virtual destructor. Nothing to be done.
  virtual ~ROCDataBankHallA() { ; }
  /**Handle the bank data. Function evaluates the length field
   * of the header and returns a pointer to the next bank.
   */
  virtual struct BankHeader * handle();
  /**Print the data to stdout. Function evaluates the
   * length field of the header and returns a pointer to the next bank.
   */
  virtual struct BankHeader * print();
}; // end of class ROCDataBankHallA

/**This class handles synchronization events. These events are generated
 * automatically by the trigger supervisor, or upon operator command.
 * Sync events contain a time stamp in unix format, event numbers, and
 * status information.
 */
class SyncEventHallA : public SyncEvent {
protected:
  CindyTree * ct;
public:
  ///Constructor. Only parameter is a pointer to the event header.
  SyncEventHallA(CindyTree * ctree, struct BankHeader * bank)
    : SyncEvent(bank) { ct = ctree; }
  ///Virtual destructor. Nothing to be done.
  virtual ~SyncEventHallA() { ; }
  /**Handle the bank data. Function evaluates the length field
   * of the header and returns a pointer to the next event.
   */
  virtual struct BankHeader * handle();
}; // end of class SyncEventHallA

/**This class handles pre start events. These events are generated
 * during the transition from the downloaded state to the prestarted state.
 * PreStart events contain a time stamp in unix format, the run number,
 * and the run type.
 */
class PreStartEventHallA : public PreStartEvent {
protected:
  CindyTree * ct;
public:
  ///Constructor. Only parameter is a pointer to the event header.
  PreStartEventHallA(CindyTree * ctree, struct BankHeader * bank)
    : PreStartEvent(bank) { ct = ctree; }
  ///Virtual destructor. Nothing to be done.
  virtual ~PreStartEventHallA() { ; }
  /**Handle the bank data. Function evaluates the length field
   * of the header and returns a pointer to the next event.
   */
  virtual struct BankHeader * handle();
}; // end of class PreStartEventHallA

/**This class handles go events. These events are generated
 * for each start or resume command. Go events contain a time stamp
 * in unix format and the number of events in the run so far.
 */
class GoEventHallA : public GoEvent {
protected:
  CindyTree * ct;
public:
  ///Constructor. Only parameter is a pointer to the event header.
  GoEventHallA(CindyTree * ctree, struct BankHeader * bank)
    : GoEvent(bank) { ct = ctree; }
  ///Virtual destructor. Nothing to be done.
  virtual ~GoEventHallA() { ; }
  /**Handle the bank data. Function evaluates the length field
   * of the header and returns a pointer to the next event.
   */
  virtual struct BankHeader * handle();
}; // end of class GoEventHallA

/**This class handles pause events. These events are generated
 * for each transition from the active state to the paused state.
 * Pause events contain a time stamp in unix format and the
 * number of events in the run so far.
 */
class PauseEventHallA : public PauseEvent {
protected:
  CindyTree * ct;
public:
  ///Constructor. Only parameter is a pointer to the event header.
  PauseEventHallA(CindyTree * ctree, struct BankHeader * bank)
    : PauseEvent(bank) { ct = ctree; }
  ///Virtual destructor. Nothing to be done.
  virtual ~PauseEventHallA() { ; }
  /**Handle the bank data. Function evaluates the length field
   * of the header and returns a pointer to the next event.
   */
  virtual struct BankHeader * handle();
}; // end of class PauseEventHallA

/**This class handles end events. These events are generated
 * for each time a run is ended. End events contain a time stamp
 * in unix format and the number of events in the run.
 */
class EndEventHallA : public EndEvent {
protected:
  CindyTree * ct;
public:
  ///Constructor. Only parameter is a pointer to the event header.
  EndEventHallA(CindyTree * ctree, struct BankHeader * bank)
    : EndEvent(bank) { ct = ctree; }
  ///Virtual destructor. Nothing to be done.
  virtual ~EndEventHallA() { ; }
  /**Handle the bank data. Function evaluates the length field
   * of the header and returns a pointer to the next event.
   */
  virtual struct BankHeader * handle();
}; // end of class EndEventHallA

/**This class handles all standard physics events.
 * These events consist of an outermost bank with a tag equal to
 * the event type and a number of data banks.
 */
class PhysicsEventHallA : public PhysicsEvent {
protected:
  CindyTree * ct;
  virtual BankBase * createBank(struct BankHeader * bank);
public:
  ///Constructor. Only parameter is a pointer to the event header.
  PhysicsEventHallA(CindyTree * ctree, struct BankHeader * bank)
    : PhysicsEvent(bank) { ct = ctree; }
  ///Virtual destructor. Nothing to be done.
  virtual ~PhysicsEventHallA() { ; }
}; // end of class PhysicsEventHallA

/**Class definition.
 */
class HallAfile : public CodaFile {
  int  copyRun;
  int  childPID;
  char runname[32];
  time_t dataTaking;
protected:
  CindyTree * ct;
  int forkGzip(const char * filename);
  virtual EventBase * createEvent(struct BankHeader * bank);
public:
  /// Constructor. Initialize object.
  HallAfile(CindyTree * ctree, int ifd, int copyrun=donotCopyRun);
  /// Destructor. Free memory.
  virtual ~HallAfile() { ; }
}; // end of class HallAfile

#endif /* __CODAFILE_H__ */
