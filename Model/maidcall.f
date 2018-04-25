C                                                                 -*-Fortran-*-
C     $Id: maidcall.f 2216 2008-06-13 21:13:47Z distler $
C
C     This subroutine provides an interface to c++ 
C     for the maid2000 fortran code
C
      SUBROUTINE MYMAID(ISO, W, Q2, COSTHETA, H, F, EP,EM,MP,MM,LP,LM)
      
      IMPLICIT REAL*8 (A-H,O-Z)
      
      INTEGER       ISO
      REAL*8        W, Q2, COSTHETA
      COMPLEX*16    H, F, EP,EM,MP,MM,LP,LM
      DIMENSION     H(6), F(6)
      DIMENSION     EP(10),EM(10),MP(10),MM(10),LP(10),LM(10)

      COMPLEX*16 MULTI,ACH,AIS,APN3,FPL,FMN,F0
      COMPLEX*16 A1,A2,A3,A4,A5,A6
      COMPLEX*16 F_back(6)
      REAL*8 KAPPAP,KAPPAN,MPI,MPI0,MPIP,MPP,MN,M,M1,M2,M1P,M1M
      INTEGER GAUGE
      DIMENSION MULTI(10,8)
      DIMENSION ACH(10,8,4),AIS(10,8,3),APN3(10,8,3)

      COMMON/HELIC/ SIG1, SIG3, STL
      COMMON /E0CORR/ XEC,XSC,XMIX
      COMMON/param3/ XMP33,XEP33,XSP33,XES31,XSS31,XED33,XMD33,XSD33
      COMMON/parDFp/ XED13p,XMD13p,XSD13p,XEF15p,XMF15p,XSF15p
      COMMON/parDFn/ XED13n,XMD13n,XSD13n,XEF15n,XMF15n,XSF15n
      COMMON/par11p/ XMP11p,XSP11p,XES11p,XSS11p
      COMMON/par11n/ XMP11n,XSP11n,XES11n,XSS11n
      COMMON/secS11/ XES2p,XSS2p,XES2n,XSS2n
      COMMON/newres/ S31

      XMIX=1
      XEC=1
      XSC=1
      BORN  = 1
      VEC   = 1
      RHO   = 1
      OMEGA = 1
      P33   = 1
      P11   = 1
      S11F  = 1
      S11S  = 1
      D13   = 1
      D33   = 1
      F15   = 1
      S31   = 1
      XMP11p=1
      XSP11p=1
      XES11p=1
      XSS11p=0
      XMD13p=1
      XED13p=1
      XSD13p=0
      XMF15p=1
      XEF15p=1
      XSF15p=0
      XMP11n=1
      XSP11n=1
      XES11n=1
      XSS11n=0
      XMD13n=1
      XED13n=1
      XSD13n=0
      XMF15n=1
      XEF15n=1
      XSF15n=0
      XES2p=1
      XES2n=1
      XSS2p=0
      XSS2n=0
      
      XMP33 = 1
      XEP33 = 1
      XSP33 = 1
      XES31 = 0
      XSS31 = 0
      XED33 = 1
      XMD33 = 1
      XSD33 = 0

      CALL MAID(ISO,W/0.197327053,Q2/0.038937966,COSTHETA,
     *     QPI,EGCM,EGLAB,
     *     F(1),F(2),F(3),F(4),F(5),F(6), 
     *     A1,A2,A3,A4,A5,A6, 
     *     H(1),H(2),H(3),H(4),H(5),H(6),
     *     1,3,ACH,AIS,APN3,
     *     BORN,VEC,OMEGA,RHO,P33,P11,D13,S11F,S11S,F15,D33)
      
c     ---------------------------------------------------------------------
      DO 10 L=1,10
         EP(L) = ACH(L, 1, ISO)
         EM(L) = ACH(L, 2, ISO)
         MP(L) = ACH(L, 3, ISO)
         MM(L) = ACH(L, 4, ISO)
         LP(L) = ACH(L, 5, ISO)
         LM(L) = ACH(L, 6, ISO)
 10   CONTINUE

      DO 20 L=1,6
         H(L) = H(L)/197.327053
         F(L) = F(L)/197.327053
 20   CONTINUE
      
      RETURN
      END



