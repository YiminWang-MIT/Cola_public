C
C     Copyright (c) 1993 by	Michael O. Distler,
C				Institut fuer Kernphysik,
C				Johannes Gutenberg-Universitaet Mainz
C
C     All rights reserved.
C
C     $Header: /tmp/cvsroot/Cola/HBook/hbook.f,v 1.16 1997-06-11 11:56:33 distler Exp $
C
      PROGRAM HBOOK
      COMMON/PAWC/H(10000)
      INTEGER ISTAT,ICYCLE,NID
      REAL H
      CALL HLIMIT(10000)
      CALL HBOOK1(10,'Test Histo',100,0.,100.,0.)
      NID=0
      CALL HFF1(10,NID,10.,1.)
      CALL HFF1(10,NID,20.,1.)
      CALL HFF1(10,NID,30.,1.)
      CALL HFF1(10,NID,40.,1.)
      CALL HFF1(10,NID,50.,1.)
      CALL HROPEN(2,'HISTO','histo.dat','N',1024,ISTAT)
      CALL HROUT(0,ICYCLE,' ')
      CALL HREND('HISTO')
      END  
