//								      -*-c++-*-
// Copyright (c) 1995 by	Michael O. Distler,
//				Institut für Kernphysik,
//				Johannes Gutenberg-Universität Mainz
//
// All rights reserved.
//
// $Header: /tmp/cvsroot/Cola/ColaMIT/magTape.h,v 2.1 2002-03-05 03:49:49 distler Exp $
//

#ifndef __magTape_h__
#define __magTape_h__

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mtio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

class magTape {
  int mtfd;
  char *tape;
  struct mtget mt_status;
  int command(int com, int count=1) {
    struct mtop mt_com;
    mt_com.mt_op = com;
    mt_com.mt_count = count;
    if ((mtfd < 0) && (open() < 0)) return errno;
    if (ioctl(mtfd, MTIOCTOP, (char *)&mt_com) < 0) return errno;
    close();
    return 0;
  }
public:
  magTape(char *mt) {
    tape = new char[strlen(mt)+1];
    strcpy(tape, mt);  mtfd = -1;
  }
  ~magTape()             { if (tape) delete tape; close(); }
  int open()             { return (mtfd = ::open(tape, O_RDONLY)); }
  int close()            { if (mtfd >= 0) ::close(mtfd); mtfd = -1; return 0; }
  int fdesc()            { return mtfd; }
  int rewind()           { return command(MTREW); }
  int fskip()            { return command(MTFSF,1); }
  int offline()          { return command(MTOFFL); }
#ifdef MTSETBLK
  int setblk(int size)   { return command(MTSETBLK, size); }
#else
  int setblk(int)        { return 0; }
#endif
  struct mtget *status() {
    int was_closed = (mtfd < 0);
    if ((was_closed) && (open() < 0)) return NULL;
    if (ioctl(mtfd, MTIOCGET, (char *)&mt_status) < 0) return NULL;
    if (was_closed) close();
    return &mt_status;
  }
  int isEOT() {
    struct mtget * st = status();
#ifdef GMT_EOT
    return ((st) ? GMT_EOT(st->mt_gstat) : 0);
#else
#ifdef MT_EOT
    return ((st->mt_dposn & MT_EOT) == MT_EOT);
#else
    return 0;
#endif
#endif
  }
  void printStatus() {
    struct mtget * st = status();
    if (!st) return;
    printf("type  : %8ld 0x%08lx\n", (long)st->mt_type,  (long)st->mt_type);
    printf("resid : %8ld 0x%08lx\n", (long)st->mt_resid, (long)st->mt_resid);
#ifdef _INCLUDE_HPUX_SOURCE
    printf("dsreg1: %8ld 0x%08lx\n", (long)st->mt_dsreg1, (long)st->mt_dsreg1);
    printf("dsreg2: %8ld 0x%08lx\n", (long)st->mt_dsreg2, (long)st->mt_dsreg2);
#else
    printf("dsreg : %8ld 0x%08lx\n", (long)st->mt_dsreg, (long)st->mt_dsreg);
#endif
    printf("erreg : %8ld 0x%08lx\n", (long)st->mt_erreg, (long)st->mt_erreg);
#ifdef GMT_EOF
    printf("gstat : %8ld 0x%08lx\n", (long)st->mt_gstat, (long)st->mt_gstat);
    if (GMT_EOF(st->mt_gstat))       printf("EOF ");
    if (GMT_BOT(st->mt_gstat))       printf("BOT ");
    if (GMT_EOT(st->mt_gstat))       printf("EOT ");
    if (GMT_SM(st->mt_gstat))        printf("SM ");
    if (GMT_EOD(st->mt_gstat))       printf("EOD ");
    if (GMT_WR_PROT(st->mt_gstat))   printf("WR_PROT ");
    if (GMT_ONLINE(st->mt_gstat))    printf("ONLINE ");
    if (GMT_D_6250(st->mt_gstat))    printf("D_6250 ");
    if (GMT_D_1600(st->mt_gstat))    printf("D_1600 ");
    if (GMT_D_800(st->mt_gstat))     printf("D_800 ");
    if (GMT_DR_OPEN(st->mt_gstat))   printf("DR_OPEN ");
    if (GMT_IM_REP_EN(st->mt_gstat)) printf("IM_REP_EN ");
#else
#ifdef MT_EOT
    printf("dposn : %8ld 0x%08lx\n", (long)st->mt_dposn, (long)st->mt_dposn);
    if (st->mt_dposn & MT_EOT)    printf("EOT ");
    if (st->mt_dposn & MT_BOT)    printf("BOT ");
    if (st->mt_dposn & MT_WPROT)  printf("WPROT ");
    if (st->mt_dposn & MT_ONL)    printf("ONL ");
    if (st->mt_dposn & MT_EOD)    printf("EOD ");
    if (st->mt_dposn & MT_FMK)    printf("FMK ");
    if (st->mt_dposn & MT_QIC24)  printf("QIC24 ");
    if (st->mt_dposn & MT_QIC120) printf("QIC120 ");
#endif
#endif
    printf("\n");
  }
};

#endif /* __magTape_h__ */
