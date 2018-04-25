//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaJLab/HallAfile.cc,v 1.3 2002-12-12 12:24:53 distler Exp $
//
// Classes to read coda files
//

#include "HallAfile.h"
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include <stdio.h>
#include <iostream>
#include <netinet/in.h>

struct BankHeader *
EventIdBankHallA::handle()
{
  return bufend;
}  // end of EventIdBankHallA::handle()

ROCDataBankHallA::ROCDataBankHallA(CindyTree * ctree, struct BankHeader * bank)
  : ROCDataBank(bank)
{
  ct = ctree;
  digit = new (DigitBase **)[16];
  memset(digit, 0, 16*sizeof(DigitBase **));

  digit[1] = new (DigitBase *)[32];
  memset(digit[1], 0, 32*sizeof(DigitBase *));
  digit[1][4]  = new Digit1877();  digit[1][5]  = new Digit1877();
  digit[1][6]  = new Digit1877();  digit[1][7]  = new Digit1877();
  digit[1][8]  = new Digit1877();  digit[1][9]  = new Digit1877();
  digit[1][10] = new Digit1877();  digit[1][11] = new Digit1877();
  digit[1][12] = new Digit1877();  digit[1][13] = new Digit1877();
  digit[1][14] = new Digit1877();  digit[1][15] = new Digit1877();
  digit[1][16] = new Digit1877();  digit[1][17] = new Digit1877();
  digit[1][18] = new Digit1877();  digit[1][19] = new Digit1877();
  digit[1][20] = new Digit1875();  digit[1][21] = new Digit1875();
  digit[1][22] = new Digit1881();  digit[1][23] = new Digit1881();
  digit[1][24] = new Digit1881();  digit[1][25] = new Digit1881();

  digit[2] = new (DigitBase *)[32];
  memset(digit[2], 0, 32*sizeof(DigitBase *));
  digit[2][6]  = new Digit1877();  digit[2][7]  = new Digit1877();
  digit[2][8]  = new Digit1877();  digit[2][9]  = new Digit1877();
  digit[2][10] = new Digit1877();  digit[2][11] = new Digit1877();
  digit[2][12] = new Digit1877();  digit[2][13] = new Digit1877();
  digit[2][14] = new Digit1877();  digit[2][15] = new Digit1877();
  digit[2][16] = new Digit1877();  digit[2][17] = new Digit1877();
  digit[2][18] = new Digit1877();  digit[2][19] = new Digit1877();
  digit[2][20] = new Digit1877();  digit[2][21] = new Digit1877();
  digit[2][22] = new Digit1877();  digit[2][23] = new Digit1875();
  digit[2][25] = new Digit1881();

  digit[3] = new (DigitBase *)[16];
  memset(digit[3], 0, 16*sizeof(DigitBase *));
  digit[3][4]  = new Digit1877();  digit[3][5]  = new Digit1877();
  digit[3][6]  = new Digit1877();  digit[3][7]  = new Digit1877();
  digit[3][8]  = new Digit1877();  digit[3][9]  = new Digit1877();
  digit[3][10] = new Digit1877();

  digit[14] = new (DigitBase *)[4];
  memset(digit[14], 0, 4*sizeof(DigitBase *));
  digit[14][1] = new Digit1182();  digit[14][1] = new Digit3123();
}  // end of ROCDataBankHallA::ROCDataBankHallA(CindyTree *, BankHeader *)

struct BankHeader *
ROCDataBankHallA::handle()
{
  return bufend;
}  // end of ROCDataBankHallA::handle()

struct BankHeader *
ROCDataBankHallA::print()
{
  ROCDataBank::print();

  unsigned int * code = (unsigned int *)buffer+2;
  unsigned int * cend = (unsigned int *)bufend;

  while (code < cend) {
    unsigned int data = ntohl(*code);
    unsigned short tag = ntohs(buffer->tag);
    if (digit[tag]) digit[tag][data>>27]->print(data);
    code++;
  }

  return bufend;
}  // end of ROCDataBankHallA::print()

struct BankHeader *
SyncEventHallA::handle()
{
  return bufend;
}  // end of SyncEventHallA::handle()

struct BankHeader *
PreStartEventHallA::handle()
{
  return bufend;
}  // end of PreStartEventHallA::handle()

struct BankHeader *
GoEventHallA::handle()
{
  return bufend;
}  // end of GoEventHallA::handle()

struct BankHeader *
PauseEventHallA::handle()
{
  return bufend;
}  // end of PauseEventHallA::handle()

struct BankHeader *
EndEventHallA::handle()
{
  return bufend;
}  // end of EndEventHallA

BankBase *
PhysicsEventHallA::createBank(struct BankHeader * bank)
{
  if (ntohs(bank->tag) == 0xC000)
    return new EventIdBankHallA(ct, bank);

  if (ntohs(bank->tag) < 32)
    return new ROCDataBankHallA(ct, bank);

  return new BankBase(bank);
}  // end of PhysicsEventHallA::createBank(struct BankHeader *)

EventBase *
HallAfile::createEvent(struct BankHeader * bank)
{
  if (bank->num == 0xCC) {
    switch (ntohs(bank->tag)) {  // run control event
    case 16:  return new SyncEventHallA(ct, bank); break;
    case 17:  return new PreStartEventHallA(ct, bank); break;
    case 18:  return new GoEventHallA(ct, bank); break;
    case 19:  return new PauseEventHallA(ct, bank); break;
    case 20:  return new EndEventHallA(ct, bank); break;
    default:  // standard physics event
      return new PhysicsEventHallA(ct, bank); break;
    }
  }
  return new EventBase(bank);
}  // end of HallAfile::createEvent(struct BankHeader *)

HallAfile::HallAfile(CindyTree * ctree, int ifd, int copyrun) : CodaFile(ifd)
{
  ct = ctree;
  copyRun = copyrun;
  *runname = 0;
  childPID = -1;
  dataTaking = 0;
}

//
// Class name  : HallAfile
// Method name : forkGzip(const char * filename)
//
// Description : create a new record object
// Input       : rTyp  = record type
//               data  = pointer to data
//               words = number of data words
// Return      : pointer to new QrecordBase object
//

int
HallAfile::forkGzip(const char * filename)
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
} // end of HallAfile::forkGzip(const char *)
