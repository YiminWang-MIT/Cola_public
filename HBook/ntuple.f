C
C
C     Copyright (c) 1991 by	Michael O. Distler,
C				Institut fuer Kernphysik,
C				Johannes Gutenberg-Universitaet Mainz
C
C     All rights reserved.
C
C     @(#)hbook.f	1.1	KPH	91/03/06	93/02/19
C
C
      BLOCK DATA
      CHARACTER*144 COPYRIGHT
      CHARACTER*64 SCCSID
      COMMON /SCCSBLOCK/COPYRIGHT,SCCSID
      DATA COPYRIGHT/'@(#)Copyright (c) 1991 by
     - Michael O. Distler,
     - Institut fuer Kernphysik,
     - Johannes Gutenberg-Universitaet Mainz.
     - All rights reserved.>'/
      DATA SCCSID/'@(#)hbook.f	1.1	KPH	91/03/06	93/02/19>'/
      END
C
      PROGRAM NTUPLE
      COMMON/PAWC/H(70000)
      INTEGER NCOLUM,NPRIME,LRECL,ISTAT,NEVENTS,I,ICYCLE
      PARAMETER (NCOLUM=20)
      REAL H,XTUPLE(NCOLUM)
      CHARACTER*8 CHTAGS(NCOLUM)
      CHARACTER*80 FNAME
      DATA NPRIME/8192/
      CALL HLIMIT(70000)
      LRECL=1024
      READ '(A)',FNAME
      CALL HROPEN(2,'NTUPLE',FNAME,'N',LRECL,ISTAT)
      READ *,NEVENTS
      I=0
 11   CONTINUE
      I=I+1
      READ '(A)',CHTAGS(I)
      IF (I.LT.NCOLUM) GOTO 11
      CALL HBOOKN(10,'Eta_Meson',NCOLUM,'NTUPLE',NPRIME,CHTAGS)
      I=0
 10   CONTINUE
      READ *,XTUPLE
      CALL HFN(10,XTUPLE)
      I=I+1
      IF (I.LT.NEVENTS) GOTO 10
      CALL HROUT(0,ICYCLE,' ')
      CALL HREND('NTUPLE')
      END  
