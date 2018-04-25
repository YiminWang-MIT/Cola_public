C ------------------------ M A I D 2000 NEW ----------------------------
C          Pion Photo and Electroproduction on the Nucleon
C          with Born + Vector mesons: Omega, Rho
C          and Resonances: P33(M1+,E1+,L1+), P11(M1-,L1-)
C                          D13, D33 (M2-,E2-,L2-), 2xS11(E0+,L0+), 
C                          F15(M3-,E3-,L3-)
C    *****        Program Source: 01/b  (20.12.00)         *****
C ----------------------------------------------------------------------
      SUBROUTINE MAID(ISO,WCM,Q20,X,QPI,EGCM,EGLAB,
     & F1,F2,F3,F4,F5,F6,A1,A2,A3,A4,A5,A6,H1,H2,H3,H4,H5,H6,
     & IMULT,LMAX,ACH,AIS,APN3,
     & BORN,VEC,OMEGA,RHO,P33,P11,D13,S11F,S11S,F15,D33)
      IMPLICIT REAL*8 (A-H,O-Z)
      COMPLEX*16 F1,F2,F3,F4,F5,F6,MULTI,ACH,AIS,APN3,FPL,FMN,F0
      COMPLEX*16 H1,H2,H3,H4,H5,H6,A1,A2,A3,A4,A5,A6
      COMPLEX*16 F_back(6)
      REAL*8 KAPPAP,KAPPAN,MPI,MPI0,MPIP,MP,MN,M,M1,M2,M1P,M1M
      INTEGER GAUGE
      DIMENSION MULTI(10,8)
      DIMENSION ACH(10,8,4),AIS(10,8,3),APN3(10,8,3)
      COMMON/COUPL/ IPVPS,IFSI
      COMMON/GAUSS/EX(100),WX(100),NX
      COMMON/FORMF/ xlampi,xlamax,mode,gauge
      COMMON/bl/ PI,PIH,HQC,F2PI,E,KAPPAP,KAPPAN,CSUC,CT,COME,CRHO,
     & CBORN,CVEC,ALAMBDA,MPI,MPI0,MPIP,MP,MN,AVM,M,M1,M2,IPV,IFORM,
     & CP33,CP11,CD13,CD33,CS11F,CS11S,CF15
      common/newres/ S31
       COMMON /E0CORR/ XE,XS,XMIX
      DIMENSION OBS(10)
      DATA IFST/0/
      SAVE IFST
c *************  INPUT ***************
c  ISO :  1  for pi0 + proton   :  2  for pi0 + neutron
c      :  3  for pi+ + neutron  :  4  for pi- + proton
c  WCM :  Total pi N c.m. energy (in 1/fm) (WCM<2.0 GeV)
c  Q2  :  Square of virtual photon 4-mometum (in 1/fm/fm)
c      :  (0=< Q2 < 3 (GeV/c)^2)
c  X   : cos(Theta_pi) in the piN c.m. frame
c**************  OUTPUT  *************
c   F1, ....,F6  :  CGLN amplitudes (in fm). Convention as in
c                : ref. D. Drechsel and L. Tiator,
c                : J.Phys.G: Nucl. Phys. 18 (1992) 449.
c                : F5=F8(Berends)*w/q ,  F6=F7(Berends)*w/q
c    EGCM        : virtual photon energy (in 1/fm). It is useful
c                : to use this value for the Lorentz tranformations of
c                : of the longitudinal amplitudes F5, F6 in order
c                : to avoid spurious singularities
c ************   PARAMETERS ***************************************
c   CBORN,CVEC,COME,CRHO  :  1 or 0 :  with or without Born, Vect-mesons
c   ISO              :  1  for pi0 + proton   :  2  for pi0 + neutron
c                    :  3  for pi+ + neutron  :  4  for pi- + proton
c   MODE             :  0  use Sachs form factors in Dipole form (std)
c                    :  1  all Dirac form factors = 1
c                    :  3  all Dirac form factors in Dipole form (diff from 0)
c                    :  2  Fpi and Fax different F1 and F2 as in MODE=0
c  !!!!!!!!  Please, use option MODE=0  and Gauge=1 or 2  !!!!!!!!!!
c   GAUGE            :  1  Coulomb Gauge (only Jz is used)
c                    :  2  Lorentz Gauge
c                    :  3  Time (axial)  (only Rho is used)
c   IPVPS            :  0   PV coupling in the Born terms
c                    :  1   PS coupling in the Born terms
c                    :  2   mixed coupling (realistic)
c   IFSI             :  0   Non-unitarized E0+
c                    :  1   unitarized E0+ (realistic)
c CP33,....,CF15     :  1 or 0 with or without corresponding resonances
c *****************************************************************************
c   FTNORM           :  arbitrary renormalization, not used in MAID2000
C   IFORM=JFORM      :  0 or 1 without or with hadronic ff (pi,omega,rho)
C                    :  =0 in MAID2000
c******************************************************************************

      IF (IFST.EQ.1) GO TO 999
c ***********  Interpolation of phases and inelasticities  *******************
      open (2, file = piNphases, status = 'old')
      CALL SPLINES()
       IFST=1
c      CLOSE (2)
999   CONTINUE
C *********  Problem at Q2=0 ***********
       Q2=Q20
       IF (Q20.LE.1.E-6) Q2=1.E-6
C******************************************************************************
	HQC=197.3285D0
	MP=938.2723D0/HQC
        MN=939.5653D0/HQC
        AVM = (MP + MN) / 2.0
	MPIP=139.5685D0/HQC
        MPI0=134.9642D0/HQC
	F2PI=0.079D0
C         f2pi=0.079  yields  g2pi=14.28
c         PI=4.D0*ATAN(1.D0)
        PI=3.1415926536D0
        PIH=PI/2.D0
        E=DSQRT(4.D0*PI/137.D0)
	KAPPAP=1.7928
	KAPPAN=-1.9130
c ****************************
        MODE=0
        GAUGE=2.
        FTNORM=1.
c ****************************
        IPVPS=2
        IFSI=0
	IF (DABS(XMIX).LE.1.E-06) IPVPS=1
	IF (DABS(XMIX).GE.1000.) IPVPS=0
C ****************************
        COME=OMEGA
        CRHO=RHO
        CBORN=BORN
        CVEC=VEC
        CP33=P33
        CP11=P11
        CD13=D13
        CD33=D33
        CS11F=S11F
        CS11S=S11S
        CF15=F15
C	LMAX=3
        LMAX1=LMAX+1
c ************************
        XLAMPI=643.0
        XLAMAX=1010.0

c ***********************
       IF (ISO.EQ.1) THEN
         M1=MP
         M2=MP
         MPI=MPI0
       ELSE IF (ISO.EQ.2) THEN
         M1=MN
         M2=MN
         MPI=MPI0
       ELSE IF (ISO.EQ.3) THEN
         M1=MP
         M2=MN
         MPI=MPIP
       ELSE IF (ISO.EQ.4) THEN
         M1=MN
         M2=MP
         MPI=MPIP
       ENDIF
C *********  N E W  20.09.98  **********
         M1=(M1+M2)/2.
         M2=M1
C **************************************
      CALL KINMAV(WCM,Q2,M1,MPI,M2,CK0,CK,Q1,W1)
      EGCM=CK0
      QPI=Q1
      EGLAB=(WCM**2-M1**2+Q2)/2./M1
        IF (W1.EQ.0) THEN
         WTHR=M2+MPI
         WRITE(1,81) WCM*HQC,Q2GEV,WTHR*HQC
   81    FORMAT(' Kinematik: wcm,q2 ',F7.1,F7.4,' below threshold: ',
     &        ' wthr =',F7.2)
        ENDIF
C ************************
      F1=(0.D0,0.D0)
      F2=(0.D0,0.D0)
      F3=(0.D0,0.D0)
      F4=(0.D0,0.D0)
      F5=(0.D0,0.D0)
      F6=(0.D0,0.D0)
      H1=(0.D0,0.D0)
      H2=(0.D0,0.D0)
      H3=(0.D0,0.D0)
      H4=(0.D0,0.D0)
      H5=(0.D0,0.D0)
      H6=(0.D0,0.D0)


        DO 99  L1=1,10
        DO 99  K=1,8
        DO 98  I=1,3
        APN3(L1,K,I)=(0.D0,0.D0)
        AIS(L1,K,I)=(0.D0,0.D0)
 98     ACH(L1,K,I)=(0.D0,0.D0)
 99     ACH(L1,K,4)=(0.D0,0.D0)
      IF (W1.EQ.0) RETURN
C ****************************************************************
C   unitarization phases, CGLN amplitudes from background and res
c   additional terms for background unitarization
c ****************************************************************
      CALL UNI(WCM, Q2, m1, MPI, lmax1)
      CALL CGLN(ISO,WCM,Q2,CK0,X,F1,F2,F3,F4,F5,F6)
      DO 101 K=1,6
 101  F_back(K)=(0.D0,0.D0)
      IUNI=IDINT(P33+P11+D13+D33+S11F+S11S+F15+S31)
      IF  (IUNI.NE.0)
     &  CALL CGLN_BACK(ISO, WCM, Q2, X, m1, MPI, F_back)
      F1 = F1 + F_back(1)
      F2 = F2 + F_back(2)
      F3 = F3 + F_back(3)
      F4 = F4 + F_back(4)
      F5 = F5 + F_back(5)
      F6 = F6 + F_back(6)
C ***********   HELICITY AMPLITUDES ***********
      XX=X
      YY=DSQRT(1-XX**2)
      X2Y=XX*XX-YY*YY
      SQ2=DSQRT(2.D0)
c *** old spin amplitudes of Knoechlein et al.   *********
c *** recoil polarization along the incoming photon  *****
      A1=-YY*(F3+F4*XX)/SQ2
      A2=-(2.*F1-2.*F2*XX+F4*YY*YY)/SQ2
      A3=-YY**2*F4/SQ2
      A4=YY*(2.*F2+F3+F4*XX)/SQ2
      A5=F5+F6*XX
      A6=F6*YY
      XX2=DSQRT((1+XX)/2.D0)
      YY2=DSQRT((1-XX)/2.D0)
c *** new helicity amplitudes (21.3.99)  ****************
c *** recoil polarization along the outgoing pion  ******
      H1=-1/SQ2*YY*XX2*(F3+F4)
      H2=SQ2*XX2*(F2-F1+(F3-F4)*(1-XX)/2.D0)
      H3=1/SQ2*YY*YY2*(F3-F4)
      H4=SQ2*YY2*(F1+F2+(F3+F4)*(1+XX)/2.D0)
      H5= XX2*(F5+F6)
      H6=-YY2*(F5-F6)
c ***********************************************
        IF (IMULT.NE.1) RETURN
        NX=32
        CALL GAUSSP(NX,EX,WX)

       DO 255 ISO1=1,4
      CALL MULTIPOL(ISO1,WCM,Q2,CK0,CK,LMAX,IUNI,MULTI)
       DO 255 L1=1,LMAX1
       DO 255 K=1,8
 255  ACH(L1,K,ISO1)=MULTI(L1,K)*1000.*MPIP

       DO 256 K=1,8
       DO 256 L1=1,LMAX1
       FPL = (ACH(L1,K,1)+ACH(L1,K,2))/2.
       FMN = (ACH(L1,K,3)-ACH(L1,K,4))/2./DSQRT(2.D0)
       F0  = (ACH(L1,K,1)-ACH(L1,K,2))/2.
       AIS(L1,K,1) = FPL + 2.*FMN
       AIS(L1,K,3) = FPL - FMN
       AIS(L1,K,2) = F0
       APN3(L1,K,1)= F0 + AIS(L1,K,1)/3.
       APN3(L1,K,2)= F0 - AIS(L1,K,1)/3.
       APN3(L1,K,3)=  AIS(L1,K,3)
 256   CONTINUE

      RETURN
      END
C

      SUBROUTINE CGLN(ISO,WCM,Q2,WGCM,X,F1,F2,F3,F4,F5,F6)
C******************************************************************************
C     normalization of CGLN amplitudes calculated in Subroutine MAINZ
C******************************************************************************
      IMPLICIT REAL*8 (A-H,O-Z)
      COMPLEX*16 F1,F2,F3,F4,F5,F6
      REAL*8 KAPPAP,KAPPAN,MPIP,MPI0,MPI,MP,MN,M,M1,M2
      COMMON/bl/ PI,PIH,HQC,F2PI,E,KAPPAP,KAPPAN,CSUC,CT,COME,CRHO,
     & CBORN,CVEC,ALAMBDA,MPI,MPI0,MPIP,MP,MN,M,AVM,M1,M2,IPV,IFORM,
     & CP33,CP11,CD13,CD33,CS11F,CS11S,CF15
        INTEGER IBORN,IVEC,IOME,IRHO,JFORM,IP33,IP11,ID13,IS11,IF15
        INTEGER GAUGE, IS11F, IS11S, ID33
      real*8 M1FM,M2FM,MPIFM
        COMMON /FORMF/ xlampi,xlamax,mode,gauge
        COMMON /LINK1/ M1FM,M2FM,MPIFM,IBORN,IVEC,IOME,IRHO,JFORM,
     &                IP33,IP11,ID13,ID33,IS11F,IS11S,IF15
C
      IBORN=INT(CBORN+0.1)
      IVEC=INT(CVEC+0.1)
      IOME=INT(COME+0.1)
      IRHO=INT(CRHO+0.1)
      IP33=INT(CP33+0.1)
      IP11=INT(CP11+0.1)
      ID13=INT(CD13+0.1)
      ID33=INT(CD33+0.1)
      IS11F=INT(CS11F+0.1)
      IS11S=INT(CS11S+0.1)
      IF15=INT(CF15+0.1)
      JFORM = 0
c      JFORM=IFORM
      M1FM=M1
      M2FM=M2
      MPIFM=MPI
c        FNORM=MPIP
c  the units of CGLN amplitudes are : 1/mpip
      CALL MAINZ(ISO,WCM,Q2,WGCM,X,F1,F2,F3,F4,F5,F6)
      F1=F1/MPIP
      F2=F2/MPIP
      F3=F3/MPIP
      F4=F4/MPIP
      F5=F5/MPIP
      F6=F6/MPIP
      RETURN
      END
C
      SUBROUTINE KINMAV(WCM,Q2,XM1,XMPI,XM2,CK0,CK,CKP,ECPI)
C******************************************************************************
C     pion cm momentum and energy
C******************************************************************************
      IMPLICIT REAL*8 (A-H,O-Z)
	HQC=197.3285D0
      S=WCM**2
      CK0=(S-Q2-XM1**2)/(2*WCM)
      CK=SQRT(CK0**2+Q2)
      IF (WCM.GT.XM2+XMPI) THEN
        ECPI=(S+XMPI*XMPI-XM2*XM2)/(2*WCM)
        CKP=SQRT(ECPI*ECPI-XMPI*XMPI)
      ELSE
c        PRINT *,' W=',WCM*HQC,' below threshold '
        WRITE(6,100) WCM*HQC
 100    FORMAT(' W=',F10.4,' below threshold ')
        ECPI=0
        CKP=0
      ENDIF
      RETURN
      END

      SUBROUTINE THRESH(XM1,XMPI,XM2,XK)
C******************************************************************************
C     threshold equivalent photon lab energy
C******************************************************************************
      IMPLICIT REAL*8 (A-H,O-Z)
      XK=(XM2+XMPI)**2/(2*XM1) - XM1/2
      RETURN
      END
c
        SUBROUTINE MAINZ(ISO,WCM,Q2FM,WGCM,XX,F1C,F2C,F3C,F4C,F5C,F6C)
C******************************************************************************
C   combines the CGLN amplitudes from Born terms, Vector mesons and Resonances
C   evaluation of charged amplitudes from isospin amplitudes
C******************************************************************************
        IMPLICIT NONE
        INTEGER ISO,MODE,GAUGE, I33, IS11F, IS11S, IS31
	COMMON/newres/ S31
        DOUBLE PRECISION XLAMPI,XLAMAX,S31
        COMMON /FORMF/ xlampi,xlamax,mode,gauge
        DOUBLE PRECISION WCM,Q2FM,XX,WGCM
        COMPLEX*16 F1C,F2C,F3C,F4C,F5C,F6C
        INTEGER IBORN,IVEC,IOME,IRHO,JFORM,IP33,IP11,ID13,ID33,IS11,IF15
        DOUBLE PRECISION M1FM,M2FM,MPIFM
        COMMON /LINK1/ M1FM,M2FM,MPIFM,IBORN,IVEC,IOME,IRHO,JFORM,
     &                IP33,IP11,ID13,ID33,IS11F,IS11S,IF15
    	DOUBLE PRECISION OmegL, Q2, W, qcm, kcm, Q2G, wGacm
c	COMMON /KinVar/ OmegL, Q2, W, qcm, kcm
	COMMON /KinVar/ OmegL, Q2, W, wGacm, kcm
    	DOUBLE PRECISION mi, mf, mPi
	COMMON /Mass/ mi, mf, mPi
        COMMON/COUPL/ IPVPS,IFSI
	INTEGER Reaction, Unq, a, l, IPVPS,IFSI
	DOUBLE PRECISION F1, F2, F3, F4, F5, F6
        DOUBLE PRECISION F1b,F2b,F3b,F4b,F5b,F6b,F7b,F8b
        DOUBLE PRECISION F1v,F2v,F3v,F4v,F5v,F6v,ReM2MN,ImM2MN
	DIMENSION F1(2),F2(2),F3(2),F4(2),F5(2),F6(2)
	DIMENSION F1b(2),F2b(2),F3b(2),F4b(2),F5b(2),ReM3MN(3),
     * F6b(2),F7b(2),F8b(2),ReM2MN(3),ImM2MN(3),ImM3MN(3)
	DIMENSION F1v(2),F2v(2),F3v(2),F4v(2),F5v(2),F6v(2)
	DOUBLE PRECISION ReC(6), ImC(6), ReF(6,2),ImF(6,2)
       DOUBLE PRECISION ReM1P, ImM1P, ReM1M, ImM1M, ReM2M, ImM2M,
     * ReM1MN,ImM1MN,ReM0PL,ImM0PL,ReS31(3),ImS31(3)
       DIMENSION ReM1P(3),ImM1P(3),ReM1M(3),ImM1M(3),
     * ReM2M(3),ImM2M(3),ReM1MN(3), ImM1MN(3), ReM0PL(3),ImM0pl(3)
	DOUBLE PRECISION ReM0P,ImM0P,ReM3M,ImM3M,ReM3MN,ImM3MN
	DIMENSION ReM0P(3),ImM0P(3),ReM3M(3),ImM3M(3)
	DIMENSION ReM0PL2(3),ImM0PL2(3),ReM1PL(3),ImM1PL(3)
	DOUBLE PRECISION ReM0PL2,ImM0PL2,ReM1PL,ImM1PL
        DOUBLE PRECISION ReD33M(3), ImD33M(3),ReD33(3), ImD33(3)
	DOUBLE PRECISION PLM2,PLM,PL,PLP,PLP2,d(6),Pi,x,y,hqc,Theta
        DOUBLE PRECISION S,P2S,P3S,P3SS,kgcm,GMED,WRMED,XCM
       COMMON /MEDIUM/ GMED,WRMED,XCM
	PARAMETER ( Pi = 3.1415926536D0, hqc = 197.3285)
c
        Reaction=1+MOD(ISO+1,4)
        W=WCM*hqc
        XCM=XX
        Q2=Q2fm*hqc**2
c  same nucleon mass is necessary for gauge invariance
        mi=(m1fm+m2fm)/2.*hqc
        mf=mi
        mPi=mpifm*hqc
	OmegL = (W*W+Q2-mi*mi)/2./mi
	kcm = SQRT(((W*W+mPi*mPi-mf*mf)/(2*W))**2-mPi*mPi)
       kgcm = (W*W-mi*mi)/2./W
        wGacm=WGCM*hqc
        qcm=sqrt(wGacm**2+Q2)
c ********************************
        Unq=1
c ********************************
        DO 109 l=1,2
        F1b(l)=0
        F2b(l)=0
        F3b(l)=0
        F4b(l)=0
        F5b(l)=0
        F6b(l)=0
        F7b(l)=0
        F8b(l)=0
        F1v(l)=0
        F2v(l)=0
        F3v(l)=0
        F4v(l)=0
        F5v(l)=0
        F6v(l)=0
  109   CONTINUE
        DO 110 l=1,3
        ReM0P(l)=0
        ImM0P(l)=0
        ReM1P(l)=0
        ImM1P(l)=0
        ReM1M(l)=0
        ImM1M(l)=0
        ReM2M(l)=0
        ImM2M(l)=0
        ReM3M(l)=0
        ImM3M(l)=0
        ReD33M(l)=0
        ImD33M(l)=0
        ReM0PL(l)=0
        ImM0PL(l)=0
        ReM0PL2(l)=0
        ImM0PL2(l)=0
	ReS31(l)=0.
	ImS31(l)=0.
  110   CONTINUE
c *****************************************************************
c *******     resonance contributions from 7 resonances     *******
c *****************************************************************
        IF (IP33.EQ.1) CALL P33NEW(Reaction,ReM1P,ImM1P)
        IF (IP11.EQ.1) CALL P11NEW(Reaction,Unq,ReM1M,ImM1M)
        IF (ID13.EQ.1) CALL D13NEW(Reaction,Unq,ReM2M,ImM2M)
        IF (ID33.EQ.1) CALL D33NEW(Unq,ReD33M,ImD33M)
        IF (IS11F.EQ.1) CALL S11FST(Reaction,ReM0PL,ImM0PL)
        IF (IS11S.EQ.1) CALL S11SEC(Reaction,ReM0PL2,ImM0PL2)
        IF (IF15.EQ.1) CALL F15NEW(Reaction,Unq,ReM3M,ImM3M)
	IS31=INT(S31+0.1)
        IF (IS31.EQ.1) CALL S31NEW(Unq,ReS31,ImS31)
c
        DO 156 L=1,3
        REM0P(L)=REM0PL(L)+REM0PL2(L)
156     IMM0P(L)=IMM0PL(L)+IMM0PL2(L)

        x = XX
	P2S = 3.D0*x
	P3S = 15.D0/2.D0*x*x-3.D0/2.D0
	P3SS = 15.D0*x
C ************************************************************************
C         standard Born terms (PS-PV) and vector meson contributions
C         without unitarization
C ************************************************************************
       IF(IBORN.EQ.1) CALL BORNEW(Reaction,x,F1b,F2b,F3b,F4b,F5b,F6b)
       IF(IVEC.EQ.1)  CALL VECTNEW(Reaction,x,F1v,F2v,F3v,F4v,F5v,F6v)
C ************************************************************************

        DO 108 l=1,2
        F1(l)=F1b(l)+F1v(l)
        F2(l)=F2b(l)+F2v(l)
        F3(l)=F3b(l)+F3v(l)
        F4(l)=F4b(l)+F4v(l)
        F5(l)=F5b(l)+F5v(l)
        F6(l)=F6b(l)+F6v(l)
  108   CONTINUE
c *******************  E0FSI(p,n,) are included (Maid 98)****************
	ReF(1,1) = F1(1)+ReM0P(1)+3.D0*ReM2M(2)+ReM2M(1)
     *  +(4.D0*ReM3M(2)+ReM3M(1))*P2S
	ImF(1,1) = ImM0P(1)+3.D0*ImM2M(2)+ImM2M(1)
     *	 +(4.D0*ImM3M(2)+ImM3M(1))*P2S
c ***********************************************************************
        ReF(2,1) = F2(1)+ReM1M(2)+2.D0*ReM2M(2)*P2S+3.D0*ReM3M(2)*P3S
        ImF(2,1) = ImM1M(2)+2.D0*ImM2M(2)*P2S+3.D0*ImM3M(2)*P3S
	ReF(3,1) = F3(1)+(ReM3M(1)+ReM3M(2))*3.D0
	ImF(3,1) = (ImM3M(1)+ImM3M(2))*3.D0
	ReF(4,1) = F4(1)-3.D0*(ReM2M(1)+ReM2M(2))-(ReM3M(1)
     * +ReM3M(2))*P3SS
	ImF(4,1) = -3.D0*(ImM2M(1)+ImM2M(2))-(ImM3M(1)+ImM3M(2))*P3SS
c	ReF(5,1) = F5(1) + ReM0PL(3) - 2*ReM2M(3)-9*x*ReM3M(3)
	ReF(5,1) = F5(1) + ReM0P(3) - 2*ReM2M(3)-9*x*ReM3M(3)
c	ImF(5,1) = ImM0PL(3) - 2*ImM2M(3)-9*x*ImM3M(3)
	ImF(5,1) = ImM0P(3) - 2*ImM2M(3)-9*x*ImM3M(3)
	ReF(6,1) = F6(1)+ReM1M(3)+6*x*ReM2M(3)+1.5*(15*x*x-3)*ReM3M(3)
	ImF(6,1) = ImM1M(3) + 6*x*ImM2M(3)+1.5*(15*x*x-3)*ImM3M(3)
C ***************** E0FSI(3/2) (Maid 98) and D33 are included **********
	ReF(1,2) = F1(2)+(ReM1P(1)+ReM1P(2))*P2S + ReS31(1)
     *   + 3.D0*ReD33M(2)+ReD33M(1)
	ImF(1,2) = (ImM1P(1)+ImM1P(2))*P2S + ImS31(1)
     *   + 3.D0*ImD33M(2)+ImD33M(1)
	ReF(2,2) = F2(2)+2.D0*ReM1P(1) + 2.D0*ReD33M(2)*P2S
	ImF(2,2) = 2.D0*ImM1P(1) + 2.D0*ImD33M(2)*P2S
	ReF(3,2) = F3(2)+3.D0*(ReM1P(2)-ReM1P(1))
	ImF(3,2) = 3.D0*(ImM1P(2)-ImM1P(1))
	ReF(4,2) = F4(2)-3.D0*(ReD33M(1)+ReD33M(2))
	ImF(4,2) = -3.D0*(ImD33M(1)+ImD33M(2))
c **********************************************************************
	ReF(5,2) = F5(2)+2.D0*ReM1P(3)*P2S + ReS31(3) - 2*ReD33M(3)
	ImF(5,2) = 2.D0*ImM1P(3)*P2S + ImS31(3) - 2*ImD33M(3)
	ReF(6,2) = F6(2)-2.D0*ReM1P(3) + 6*x*ReD33M(3)
        ImF(6,2) = -2.D0*ImM1P(3) + 6*x*ImD33M(3)
	DO 111 l = 1, 6
          IF (Reaction.EQ.1) THEN
	    ReC(l) = -ReF(l,2)/SQRT(3.D0)-ReF(l,1)*SQRT(2.D0/3.D0)
	    ImC(l) = -ImF(l,2)/SQRT(3.D0)-ImF(l,1)*SQRT(2.D0/3.D0)
          ELSE IF (Reaction.EQ.2) THEN
	    ReC(l) = ReF(l,2)/SQRT(3.D0)-ReF(l,1)*SQRT(2.D0/3.D0)
	    ImC(l) = ImF(l,2)/SQRT(3.D0)-ImF(l,1)*SQRT(2.D0/3.D0)
          ELSE IF (Reaction.EQ.3) THEN
	    ReC(l) = ReF(l,2)*SQRT(2.D0/3.D0)-ReF(l,1)/SQRT(3.D0)
	    ImC(l) = ImF(l,2)*SQRT(2.D0/3.D0)-ImF(l,1)/SQRT(3.D0)
          ELSE IF (Reaction.EQ.4) THEN
	    ReC(l) = ReF(l,2)*SQRT(2.D0/3.D0)+ReF(l,1)/SQRT(3.D0)
	    ImC(l) = ImF(l,2)*SQRT(2.D0/3.D0)+ImF(l,1)/SQRT(3.D0)
	  END IF
 111	CONTINUE
        F1C=CMPLX(ReC(1),ImC(1))
        F2C=CMPLX(ReC(2),ImC(2))
        F3C=CMPLX(ReC(3),ImC(3))
        F4C=CMPLX(ReC(4),ImC(4))
        F5C=CMPLX(ReC(5),ImC(5))
        F6C=CMPLX(ReC(6),ImC(6))

        RETURN
    	END



 	SUBROUTINE BORNEW (Reaction, CosTh, F1, F2, F3, F4, F5, F6)
C******************************************************************************
C         Born terms with e.m. form factors and hadronic formfactors
C         IPVPS=0,1,2  for PV, PS and mixed
C         IFSI=1  unitarized S-waves
C******************************************************************************
	IMPLICIT NONE
        INTEGER IBORN,IVEC,IOME,IRHO,JFORM,IP33,IP11,ID13,IS11,IF15
	INTEGER i,a,Reaction, MODE,GAUGE,IPVPS,IFSI,IS11F,IS11S,ID33
        COMMON/COUPL/ IPVPS,IFSI
        COMMON /E0CORR/ XA, XB, XMIX
        DOUBLE PRECISION M1FM,M2FM,MPIFM,apvps,kappl,kap0,mup,mun
        COMMON /LINK1/ M1FM,M2FM,MPIFM,IBORN,IVEC,IOME,IRHO,JFORM,
     &                IP33,IP11,ID13,ID33,IS11F,IS11S,IF15
        DOUBLE PRECISION XLAMPI,XLAMAX,fpvps, EFIP
        COMMON /FORMF/ xlampi,xlamax,mode,gauge
	DOUBLE PRECISION mi, mf, mPi, mn, apvps0, XA, XB, XMIX
	COMMON /Mass/ mi, mf, mPi
    	DOUBLE PRECISION OmegL, Q2, W, qcm, kcm, kt,wGacm
	COMMON /KinVar/ OmegL, Q2, W, wGacm, kcm
    	DOUBLE PRECISION wPicm, CosTh, U, T, DU, DT,KQV, KQ
	DOUBLE PRECISION Ei, Ef, WPP, WPM, WMP, WMM, WM, WP
	DOUBLE PRECISION F(6,2),F1(2),F2(2),F3(2),F4(2),F5(2),
     * F6(2),G(3,8)
	DOUBLE PRECISION D,F1S,F1V,F2S,F2V,FPI,
     * FAX,FPiNN,FOmNN, FRhoNN
	DOUBLE PRECISION Pi, eLadung, Kopplung, C0, C1, C2, C3
	PARAMETER (Pi=3.1415926536D0,eLadung=.3028619D0,
     * Kopplung=0.996365D0)
       DOUBLE PRECISION TMP1, TMP2, TMP3, TMP4
	DOUBLE PRECISION mOm, mRho, DTOm, DTRho, LamPi, LamOm, LamRho
       PARAMETER(mOm=783.0D0,mRho=770.D0,LamPi=1.D3,
     * LamOm=1.4D3,LamRho=1.8D3)
	PARAMETER ( mup = 2.7928, mun = -1.913, apvps0=450.)

	mn = (mi+mf)/2.D0
	apvps=apvps0*XMIX
c ***********************************
        qcm=sqrt(wGacm**2+Q2)
c **************************************
	wPicm = SQRT(mPi*mPi+kcm*kcm)
	Ei = SQRT(mi*mi+qcm**2)
	Ef = SQRT(mf*mf+kcm**2)
	KQV = kcm*qcm*CosTh
	KQ = 2.*(wGacm*wPicm-KQV)
	kt = kcm*kcm+qcm*qcm-2.*KQV
	U = mi*mi+mPi*mPi-2.*(Ei*wPicm+KQV)
	DU = 1./(U-mf*mf)
	T = mPi*mPi-Q2-2.*(wGacm*wPicm-KQV)
        DT = 1./(T-mPi*mPi)

	WP = W+mn
	WM = W-mn
	WPP = SQRT((Ef+mf)*(Ei+mi))/(2.*W)
	WPM = SQRT((Ef+mf)*(Ei-mi))/(2.*W)
	WMP = SQRT((Ef-mf)*(Ei+mi))/(2.*W)
	WMM = SQRT((Ef-mf)*(Ei-mi))/(2.*W)

	CALL FORM (D, F1S, F1V, F2S, F2V, FPI, FAX)
c ***********************************************
       FPiNN = LamPi*LamPi/(LamPi*LamPi+kcm*kcm)
c ******************************
C       FPiNN = (LamPi*LamPi-mPi*mPi)/(LamPi*LamPi+kcm*kcm)
c *********************************
        IF (JFORM.EQ.0) THEN
         FPiNN=1
        ENDIF

	C1 = IBORN*eLadung*Kopplung/(8.*Pi)*FPiNN

        G(1,1) = C1*WPP*(F1V*(-WM/WP+DU*(KQ-WM*WM))-
     A                   F2V*(-WM*(1./mn-1./WP)+DU*(WM*WM-KQ)))
        G(2,1) = C1*WPP*(F1S*(-WM/WP+DU*(KQ-WM*WM))-
     A                   F2S*(-WM*(1./mn-1./WP)+DU*(WM*WM-KQ)))
C        G(3,1) = C1*WPP*(F1V*(-WM/WP+DU*(WM*WM-KQ)+2.)-
	G(3,1) = C1*WPP*(F1V*(-WM/WP+DU*(WM*WM-KQ)) + 2.*FAX -
     A                   F2V*(WM/WP+DU*(KQ-WM*WM)))

	G(1,2) = C1*WMM*(F1V*(-WP/WM+DU*(KQ-WP*WP))-
     A                   F2V*(WP*(1./mn+1./WM)+DU*(WP*WP-KQ)))
	G(2,2) = C1*WMM*(F1S*(-WP/WM+DU*(KQ-WP*WP))-
     A   	        F2S*(WP*(1./mn+1./WM)+DU*(WP*WP-KQ)))
c        G(3,2) = C1*WMM*(F1V*(-WP/WM+DU*(WP*WP-KQ)+2.)-
	G(3,2) = C1*WMM*(F1V*(-WP/WM+DU*(WP*WP-KQ)) + 2.*FAX -
     A   	        F2V*(WP/WM+DU*(KQ-WP*WP)))

	G(1,3) = C1*WMM*(F1V*4.*mn*(Ef+mf)*DU+F2V*2.*(Ef+mf)*WP*DU)
	G(2,3) = C1*WMM*(F1S*4.*mn*(Ef+mf)*DU+F2S*2.*(Ef+mf)*WP*DU)
C        G(3,3) = C1*WMM*(F1V*(-4.*mn*(Ef+mf)*DU-8.*(Ef+mf)*mn*DT)-
	G(3,3) = C1*WMM*(F1V*(-4.*mn*(Ef+mf)*DU)-
     A  FPI*8.*(Ef+mf)*mn*DT - F2V*2.*(Ef+mf)*WP*DU)

	G(1,4) = C1*WPP*(Ef-mf)*(-F1V*4.*mn*DU+F2V*2.*WM*DU)
	G(2,4) = C1*WPP*(Ef-mf)*(-F1S*4.*mn*DU+F2S*2.*WM*DU)
C        G(3,4)=C1*WPP*(Ef-mf)*(F1V*4.*mn*(DU+2.*DT)-F2V*2.*WM*DU)
        G(3,4)=C1*WPP*(Ef-mf)*(F1V*4.*mn*DU+FPI*8.*mn*DT-
     A  F2V*2.*WM*DU)

	TMP1 = -WM/WP+DU*(KQ+4.*mn*KQV/(Ei+mi)-WM*WM)
        TMP2 = wGacm*(1./WP-1./mn)+DU*(wGacm*WM-KQ-
     A  2.*KQV*WP/(Ei+mi))
	G(1,5) = C1*WPP*(F1V*TMP1-F2V*TMP2)
	G(2,5) = C1*WPP*(F1S*TMP1-F2S*TMP2)
C        TMP1 = -WM/WP+DU*(WM*WM-KQ-4.*KQV*mn/(Ei+mi))+2.+
C      A         4.*mn*DT*(Ei-mn-2.*KQV/(Ei+mi))
	TMP1 = -WM/WP+DU*(WM*WM-KQ-4.*KQV*mn/(Ei+mi))
	TMP2 = wGacm/WP+DU*(KQ+2.*KQV*WP/(Ei+mi)-wGacm*WM)
        TMP3 = 2.
        TMP4 = 4.*mn*DT*(Ei-mn-2.*KQV/(Ei+mi))
C        G(3,5) = C1*WPP*(F1V*TMP1-F2V*TMP2)
	G(3,5) = C1*WPP*(F1V*TMP1-F2V*TMP2+FAX*TMP3+FPI*TMP4)

	TMP1 = -WP/WM+DU*(KQ-WP*WP-4.*mn*KQV/(Ei-mi))
	TMP2 = wGacm*(1./mn+1./WM)+DU*(wGacm*WP-KQ-
     A  2.*KQV*WM/(Ei-mi))
	G(1,6) = C1*WMM*(F1V*TMP1-F2V*TMP2)
	G(2,6) = C1*WMM*(F1S*TMP1-F2S*TMP2)
C        TMP1 = -WP/WM+DU*(WP*WP+4.*mn*KQV/(Ei-mi)-KQ)+2.-
C        A         4.*mn*DT*(Ei+mn-2.*KQV/(Ei-mi))
	TMP1 = -WP/WM+DU*(WP*WP+4.*mn*KQV/(Ei-mi)-KQ)
	TMP2 = wGacm/WM+DU*(KQ+2.*KQV*WM/(Ei-mi)-wGacm*WP)
        TMP3 = 2.
        TMP4 = -4.*mn*DT*(Ei+mn-2.*KQV/(Ei-mi))
C        G(3,6) = C1*WMM*(F1V*TMP1-F2V*TMP2)
	G(3,6) = C1*WMM*(F1V*TMP1-F2V*TMP2+FAX*TMP3+FPI*TMP4)

C new F7 and F8
	TMP1 = WP/WM+DU*(W*W+2.*(Ef-wPicm)*mn+mn*mn-KQ)
	TMP2 = (Ei-mn)*(1./mn+1./WM)+DU*(KQ+(Ef-wPicm+mn)*WM-
     A   wGacm*WP)

	G(1,7) = C1*WMP*(F1V*TMP1-F2V*TMP2)
	G(2,7) = C1*WMP*(F1S*TMP1-F2S*TMP2)
	TMP1 = WP/WM+DU*(KQ-W*W-2.*(Ef-wPicm)*mn-mn*mn)
	TMP2 = (Ei-mn)/WM+DU*(wGacm*WP-KQ-(Ef-wPicm+mn)*WM)
        TMP3 = -2.
        TMP4 = +4.*mn*DT*(2.*wPicm-wGacm)

	G(3,7) = C1*WMP*(F1V*TMP1-F2V*TMP2 + FAX*TMP3 + FPI*TMP4)
	TMP1 = WM/WP+DU*(W*W+2.*(wPicm-Ef)*mn+mn*mn-KQ)
        TMP2 = (Ei+mn)*(1./WP-1./mn)
     A       +DU*(KQ+(Ei+mn)*(Ef-mn)-wPicm*(wGacm+W+mn))
	G(1,8) = C1*WPM*(F1V*TMP1-F2V*TMP2)
	G(2,8) = C1*WPM*(F1S*TMP1-F2S*TMP2)
	TMP1 = WM/WP+DU*(KQ-W*W-2.*(wPicm-Ef)*mn-mn*mn)
        TMP3 = -2.
        TMP4 = -4.*mn*DT*(2.*wPicm-wGacm)
	TMP2 = (Ei+mn)/WP+DU*(wPicm*(wGacm+W+mn)-(Ei+mn)*(Ef-mn)-KQ)
	G(3,8) = C1*WPM*(F1V*TMP1-F2V*TMP2 + FAX*TMP3 + FPI*TMP4)

C  gauge fixing for non-conserved currents (GAUGE=2)
C  GAUGE=1: Coulomb  =2: Lorentz  =3: Time (axial)
           DO 190 I=1,3
         IF (GAUGE-2) 190,191,192
 191       IF (Q2 .NE. 0) THEN
c           print *,i,G(i,5), qcm*(qcm*G(i,5)-wGacm*G(i,8))/Q2
c           print *,i,G(i,6), qcm*(qcm*G(i,6)-wGacm*G(i,7))/Q2
            G(i,5)=G(i,5)-qcm*(qcm*G(i,5)-wGacm*G(i,8))/Q2
            G(i,6)=G(i,6)-qcm*(qcm*G(i,6)-wGacm*G(i,7))/Q2
           ENDIF
           GOTO 190
 192       G(i,5)=wGacm/qcm*G(i,8)
           G(i,6)=wGacm/qcm*G(i,7)

 190     CONTINUE
c ******************  PS and MIXED couplings ************
        if (ipvps.eq.0) go to 1000
        fpvps=1.-apvps**2/(apvps**2+kcm**2)
        IF(IPVPS.EQ.1) FPVPS=1.
        EFIP=DSQRT((Ei+mi)*(Ef+mf))
	C0 =fpvps*eLadung*Kopplung/(8.*Pi*W*mi)
        G(1,1)=G(1,1)-F2V*C0*(W-mi)*EFIP/2.
        G(1,2)=G(1,2)+F2V*C0*(W+mi)*qcm*kcm/EFIP/2.
        G(2,1)=G(2,1)-F2S*C0*(W-mi)*EFIP/2.
        G(2,2)=G(2,2)+F2S*C0*(W+mi)*qcm*kcm/EFIP/2.
        G(1,5)=G(1,5)-F2V*C0*wGacm*EFIP/2.
        G(1,6)=G(1,6)+F2V*C0*(Ei-mi)*wGacm*kcm/qcm/2.
     *   * DSQRT((Ei+mi)/(Ef+mf))
        G(2,5)=G(2,5)-F2S*C0*wGacm*EFIP/2.
        G(2,6)=G(2,6)+F2S*C0*(Ei-mi)*wGacm*kcm/qcm/2.
     *   * DSQRT((Ei+mi)/(Ef+mf))
c *************************************************************
1000    continue
	DO 200 i = 1, 6
	  IF (Reaction.EQ.1 .OR. Reaction.EQ.3) THEN
	    F(i,1) = -SQRT(1./3.)*(G(1,i)+2.*G(3,i))-SQRT(3.)*G(2,i)
	    F(i,2) = SQRT(2./3.)*(G(1,i)-G(3,i))
	  ELSE IF (Reaction.EQ.2 .OR. Reaction.EQ.4) THEN
	    F(i,1) = SQRT(1./3.)*(G(1,i)+2.*G(3,i))-SQRT(3.)*G(2,i)
	    F(i,2) = SQRT(2./3.)*(G(1,i)-G(3,i))
	  END IF

 200	CONTINUE
	DO a = 1, 2
          F1(a) = F(1,a)
	  F2(a) = F(2,a)
	  F3(a) = F(3,a)
	  F4(a) = F(4,a)
          F5(a) = F(5,a)
	  F6(a) = F(6,a)
	END DO
        return
        END

 	SUBROUTINE VECTNEW (Reaction, CosTh, F1, F2, F3, F4, F5, F6)
C******************************************************************************
C     vector mesons omega and rho  JFORM=0 : no hadronic ff
C******************************************************************************
	IMPLICIT NONE
        INTEGER IBORN,IVEC,IOME,IRHO,JFORM,IP33,IP11,ID13,IS11,IF15
        DOUBLE PRECISION M1FM,M2FM,MPIFM
	INTEGER i, a, Reaction, MODE,GAUGE, IS11F, IS11S, ID33
        COMMON /LINK1/ M1FM,M2FM,MPIFM,IBORN,IVEC,IOME,IRHO,JFORM,
     &                IP33,IP11,ID13,ID33,IS11F,IS11S,IF15
        DOUBLE PRECISION XLAMPI,XLAMAX
        COMMON /FORMF/ xlampi,xlamax,mode,gauge
	DOUBLE PRECISION mi, mf, mPi, mn
	COMMON /Mass/ mi, mf, mPi
    	DOUBLE PRECISION OmegL, Q2, W, qcm, kcm, kt, wGacm
	COMMON /KinVar/ OmegL, Q2, W, wGacm, kcm
    	DOUBLE PRECISION wPicm, CosTh, U, T, DU, DT,KQV, KQ
	DOUBLE PRECISION Ei, Ef, WPP, WPM, WMP, WMM, WM, WP
	DOUBLE PRECISION F(6,2),F1(2),F2(2),F3(2),F4(2)
        DOUBLE PRECISION F5(2),F6(2),G(3,6)
	DOUBLE PRECISION D,F1S,F1V,F2S,F2V,FPI,FAX,FPiNN,FOmNN,FRhoNN
	DOUBLE PRECISION Pi, eLadung, Kopplung, C1, C2, C3
	PARAMETER (Pi=3.1415926536D0,eLadung=.3028619D0,
     A     Kopplung=0.996365D0)
	DOUBLE PRECISION TMP1, TMP2, TMP3, VEK1, VEK2
       DOUBLE PRECISION ggpo, ggpr, gom1,gom2, grho1, grho2
	DOUBLE PRECISION mOm,mRho,DTOm,DTRho,LamPi,LamOm, LamRho
	PARAMETER(mOm=783.0D0,mRho=770.D0,LamPi=0.6D3,
     * LamOm=1.2D3,LamRho=1.5D3)

	mn = (mi+mf)/2.D0
c ****************   NEW **************
        qcm=sqrt(wGacm**2+Q2)
c **************************************
	wPicm = SQRT(mPi*mPi+kcm*kcm)
	Ei = SQRT(mi*mi+qcm**2)
	Ef = SQRT(mf*mf+kcm**2)
	KQV = kcm*qcm*CosTh
	KQ = 2.*(wGacm*wPicm-KQV)
	kt = kcm*kcm+qcm*qcm-2.*KQV
	U = mi*mi+mPi*mPi-2.*(Ei*wPicm+KQV)
	DU = 1./(U-mf*mf)
	T = mPi*mPi-Q2-2.*(wGacm*wPicm-KQV)
	DTOm = 1./(T-mOm*mOm)
	DTRho = 1./(T-mRho*mRho)

c      VZ geaendert am 19.5.95
c **************   omega *****************
      ggpo = -IVEC*IOME*0.314*eLadung
      FOmNN = LamOm*LamOm/(LamOm*LamOm+kt)
      gom1 = 21.*FOmNN
      gom2=-12.*FOmNN
c **************   rho ******************
	ggpr = -IVEC*IRHO*0.103*eLadung
      grho1 = SQRT(4.*Pi*0.84)
      FRhoNN =LamRho*LamRho/(LamRho*LamRho+kt)
      grho1 = 2.*FRhoNN
      grho2 = 13.*FRhoNN
c *************************************
       WP = W+mn
	WM = W-mn
	WPP = SQRT((Ef+mf)*(Ei+mi))/(2.*W)
	WPM = SQRT((Ef+mf)*(Ei-mi))/(2.*W)
	WMP = SQRT((Ef-mf)*(Ei+mi))/(2.*W)
	WMM = SQRT((Ef-mf)*(Ei-mi))/(2.*W)

	CALL FORM (D, F1S, F1V, F2S, F2V, FPI, FAX)

      FOmNN = LamOm*LamOm/(LamOm*LamOm+kt)
      FOmNN=1.
      FRhoNN =LamRho*LamRho/(LamRho*LamRho+kt)

c      FOmNN = (LamOm*LamOm-mOm*mOm)/(LamOm*LamOm+kt)
c      FRhoNN = (LamRho*LamRho-mRho*mRho)/(LamRho*LamRho+kt)

c       FOmNN = ((LamOm*LamOm-mOm*mOm)/(LamOm*LamOm+kt))**2
c       FRhoNN = ((LamRho*LamRho-mRho*mRho)/(LamRho*LamRho+kt))**2

        IF (JFORM.EQ.0) THEN
         FOmNN=1
        FRhoNN=1
        ENDIF

	C2 = ggpo/4./Pi
	C3 = ggpr/4./Pi

        VEK1 = WM*WM-KQ/2.
	VEK2 = VEK1+  W/mn*((Ef-mf)*(Ei-mi)-KQV)
        G(1,1) = C2*WPP*D*DTOm*(gom1*VEK1+gom2*VEK2)
        G(2,1) = C3*WPP*D*DTRho*(grho1*VEK1+grho2*VEK2)
        G(3,1) = 0

	VEK1 = WP*WP-KQ/2.
	VEK2 = VEK1+  W/mn*(KQV-(Ef+mf)*(Ei+mi))
       G(1,2) = C2*WMM*D*DTOm*(gom1*VEK1+gom2*VEK2)
	G(2,2) = C3*WMM*D*DTRho*(grho1*VEK1+grho2*VEK2)
        G(3,2) = 0

	VEK1 = -(Ef+mn)*WP
	VEK2 = VEK1+  W/mn*(Ef+mf)*(Ei+mi)
       G(1,3) = C2*WMM*D*DTOm*(gom1*VEK1+gom2*VEK2)
	G(2,3) = C3*WMM*D*DTRho*(grho1*VEK1+grho2*VEK2)
        G(3,3) = 0

	VEK1 = -WM*(Ef-mn)
	VEK2 = VEK1-  W/mn*(Ef-mf)*(Ei-mi)
       G(1,4) = C2*WPP*D*DTOm*(gom1*VEK1+gom2*VEK2)
	G(2,4) = C3*WPP*D*DTRho*(grho1*VEK1+grho2*VEK2)
        G(3,4) = 0

	VEK1 = wGacm*WM-KQ/2.-WP*KQV/(Ei+mn)
	VEK2 = VEK1
       G(1,5) = C2*WPP*D*DTOm*(gom1*VEK1+gom2*VEK2)
	G(2,5) = C3*WPP*D*DTRho*(grho1*VEK1+grho2*VEK2)
        G(3,5) = 0

	VEK1 = wGacm*WP-KQ/2.-WM*KQV/(Ei-mn)
	VEK2 = VEK1
       G(1,6) = C2*WMM*D*DTOm*(gom1*VEK1+gom2*VEK2)
	G(2,6) = C3*WMM*D*DTRho*(grho1*VEK1+grho2*VEK2)
        G(3,6) = 0

	DO 200 i = 1, 6

	  IF (Reaction.EQ.1 .OR. Reaction.EQ.3) THEN
	    F(i,1) = -SQRT(1./3.)*(G(1,i)+2.*G(3,i))-SQRT(3.)*G(2,i)
	    F(i,2) = SQRT(2./3.)*(G(1,i)-G(3,i))
	  ELSE IF (Reaction.EQ.2 .OR. Reaction.EQ.4) THEN
	    F(i,1) = SQRT(1./3.)*(G(1,i)+2.*G(3,i))-SQRT(3.)*G(2,i)
	    F(i,2) = SQRT(2./3.)*(G(1,i)-G(3,i))
	  END IF

 200	CONTINUE

	DO a = 1, 2
          F1(a) = F(1,a)
	  F2(a) = F(2,a)
	  F3(a) = F(3,a)
	  F4(a) = F(4,a)
          F5(a) = F(5,a)
	  F6(a) = F(6,a)
	END DO
        return
        END


	SUBROUTINE FORM (F, F1S, F1V, F2S, F2V, FPI, FAX)
C******************************************************************************
C     electromagnetic form factors
C******************************************************************************
	IMPLICIT NONE
        INTEGER MODE,GAUGE
        DOUBLE PRECISION XLAMPI,XLAMAX, FPI,FAX
	DOUBLE PRECISION F,mg2,tau,GEP,GEN,GMN,GMP,F1P,F1N,F2P,F2N
	DOUBLE PRECISION F1S, F1V, F2S, F2V
	DOUBLE PRECISION mup, mun
	DOUBLE PRECISION mi, mf, mPi, mn
	COMMON /Mass/ mi, mf, mPi
	DOUBLE PRECISION OmegL, Q2, W, wGacm, kcm
	COMMON /KinVar/ OmegL, Q2, W, wGacm, kcm
        COMMON /FORMF/ xlampi,xlamax,mode,gauge
	PARAMETER (mg2 = .71D6, mup = 2.7928, mun = -1.913)

        mn = (mi+mf)/2.
	tau = Q2/(4.*mn*mn)
	F = 1./(1.+Q2/mg2)**2
	GEP = F
	GMN = mun*F
	GEN = (-tau)/(1.+4.*tau)*GMN
	GMP = mup*F
	F1P = (GEP+tau*GMP)/(1.+tau)
	F1N = (GEN+tau*GMN)/(1.+tau)
	F2P = (GMP-GEP)/((mup-1.)*(1.+tau))
	F2N = (GMN-GEN)/(mun*(1.+tau))
	F1S = F1P+F1N
	F1V = F1P-F1N
	F2S = (mup-1.)*F2P+mun*F2N
	F2V = (mup-1.)*F2P-mun*F2N
        FPI = F1V
        FAX = F1V
c   MODE  :  0  use Sachs form factors in Dipole form (std)
c         :  1  all Dirac form factors = 1
c         :  3  all Dirac form factors in Dipole form (diff from 0)
c         :  2  Fpi and Fax different F1 and F2 as in MODE=0

        IF (MODE.EQ.0) RETURN
        IF (MODE.EQ.1) GOTO 100
        IF (MODE.EQ.2) GOTO 200
        IF (MODE.EQ.3) GOTO 300
        F=0
  100   F=1
  300   F1S=F
        F1V=F
        F2S=(mup-1+mun)*F
        F2V=(mup-1-mun)*F
        FPI = F
        FAX = F
        RETURN
  200   FPI = 1.0/(1.0+Q2/XLAMPI**2)
        FAX = 1.0/(1.0+Q2/XLAMAX**2)**2
        RETURN
	END

       SUBROUTINE P33NEW(Reaction,ReM1P,ImM1P)
C******************************************************************************
C     M1+, E1+, L1+  for P33(1232) with fit parameters  RM, RE, RL
C******************************************************************************
       IMPLICIT NONE
c ***************************************************************+
       COMMON/param3/ XMP33,XEP33,XSP33,XES31,XSS31,XED33,XMD33,XSD33
       DOUBLE PRECISION XMP33,XEP33,XSP33,XES31,XSS31,XED33,XMD33,XSD33
c *****************************************************************
       DOUBLE PRECISION fke,fpi,DE,DM,Fak,GaGa,GaPi,GaT
       DOUBLE PRECISION mi,mf,mPION,OmegL,Q2,Q2G,W,qcm,kcm,qcm0,kgcm
       DOUBLE PRECISION W0,Ga0,X,fkm,kglb,FIM,FIE,FIL
       DOUBLE PRECISION Phi(3),kcm0,Ga,Gpi,ReM1P(3),ImM1P(3),mPi
       DOUBLE PRECISION Pi,mn,mdip,wGacm,kx,ReM1(3),ImM1(3),WX
       DOUBLE PRECISION FMQ,FEQ,FLQ,Fq,GMED,GNR,WRMED,XCM
       COMMON /MEDIUM/ GMED,WRMED,XCM
       DOUBLE PRECISION del_R, phi_c, phi_s11,phi_s31,phi_p11,phi_p31
       DOUBLE PRECISION phi_p13, phi_p33, phi_d13, phi_d33
       DOUBLE PRECISION phi_d15, phi_d35, phi_f15, phi_f35
       DOUBLE PRECISION phi_f17, phi_f37
       DOUBLE PRECISION RM,RE,RL, DS
       COMMON /PHASES/ phi_s11, phi_s31, phi_p11, phi_p31,
     *                phi_p13, phi_p33, phi_d13, phi_d33,
     *                phi_d15, phi_d35, phi_f15, phi_f35,
     *                phi_f17, phi_f37
       COMMON /PARP33/ RM,RE,RL, DM, DE, DS
       INTEGER i, Reaction
          COMMON /Mass/ mi, mf, mPION
          COMMON /KinVar/ OmegL, Q2, W, wGacm, kcm
       PARAMETER (Pi=3.14159265358979D0, mdip=0.71)
       PARAMETER(W0=1232.,Ga0=130., X=500.)
	mn = (mi+mf)/2.
        mPi=139.5685
	kcm0 = SQRT(((W0*W0+mPi*mPi-mf*mf)/(2*W0))**2-mPi*mPi)
        qcm0 = SQRT(((W0*W0+mi*mi)/(2*W0))**2-mi*mi)
        kgcm = (W*W-mi*mi)/2./W
        kglb=(W*W-mi*mi)/2./mi
        qcm=sqrt(wGacm**2+Q2)
	Q2G = Q2/1.D6
C **********************************************
	fkm = (kgcm/qcm0)**2*(qcm0*qcm0+X*X)/(kgcm*kgcm+X*X)
	fke = (qcm0/kgcm)*(qcm0*qcm0+X*X)/(kgcm*kgcm+X*X)
	fpi =(kcm/kcm0)**3*((kcm0*kcm0+X*X)/(kcm*kcm+X*X))
c        Fq=dexp(-0.24*Q2G)/(1+Q2G/mdip)**2*(qcm/kgcm)
c ********  This is new form factor which explain new Frolov data *****
        Fq=dexp(-0.21*Q2G)/(1+Q2G/mdip)**2*(qcm/kgcm)
C *****************************************************************
C     default values for M1+, E1+, S1+
C     default form factors Fq identical for all 3 multipoles
C******************************************************************************
            DM= 300.*Fq*XMP33
            DE=-6.5*Fq*XEP33
c*(1.-Q2G/3.7)
            DS=-3.*6.5*Fq*XSP33
c*(1.+Q2G**3/60.)

c *************************************************
	GaGa = kgcm*qcm/Pi*mn/W0/4./1.D9
        GaPi=Ga0*fpi*W0/W
	GaT =GaPi

 	Fak = W0/((W0*W0-W*W)**2+W0*W0*GaT*GaT)
     * *SQRT(GaGa*GaPi/qcm/kcm)

	  ReM1(1) = Fak*(W0*W0-W*W)*DM*mPi*fkm
	  ImM1(1) = Fak*W0*GaT*DM*mPi*fkm
	  ReM1(2) = Fak*(W0*W0-W*W)*DE*mPi*fke
	  ImM1(2) = FaK*W0*GaT*DE*mPi*fke
	  ReM1(3) = wGacm/qcm*Fak*(W0*W0-W*W)*DS*mPi*fke
	  ImM1(3) = wGacm/qcm*FaK*W0*GaT*DS*mPi*fke
C******************************************************************************
C                (1), (2), (3)  for  M1+, E1+, L1+
c******************************************************************************
c resonance phase is the same for all 3 multipoles
c it must be calculated for an amplitude .ne. 0.0
c******************************************************************************
          if (ReM1(1).ne.0.0) then
           del_R = datan2(ImM1(1),ReM1(1))
          elseif (ReM1(2).ne.0.0) then
           del_R = datan2(ImM1(2),ReM1(2))
          elseif (ReM1(3).ne.0.0) then
           del_R = datan2(ImM1(3),ReM1(3))
          else
	    del_R = pi/2.
          endif
          IF (del_R.LT.0.0) del_R = del_R + pi
          phi_c = phi_p33 - del_R
          Phi(1) = phi_c
          Phi(2) = phi_c
          Phi(3) = phi_c
c*************************************************************
          DO 111 I=1,3
       ReM1P(i) = ReM1(i)*DCOS(Phi(i))-ImM1(i)*DSIN(Phi(i))
       ImM1P(i) = ReM1(i)*DSIN(Phi(i))+ImM1(i)*DCOS(Phi(i))
 111      CONTINUE
c	write (1,367) ReM1P(1), ImM1P(1)
c 367    format(1x,2(e10.4,2x))
       RETURN
	END


	SUBROUTINE P11NEW(Reaction,Unq,ReM1M,ImM1M)
C******************************************************************************
C     multipoles  M1- and L1-  for P11(1440)  with fit parameters RM and RL
C******************************************************************************
	IMPLICIT NONE
c ***************************************************************+
       COMMON/par11p/ XMP11p,XSP11p,XES11p,XSS11p
       COMMON/par11n/ XMP11n,XSP11n,XES11n,XSS11n
       DOUBLE PRECISION XMP11p,XSP11p,XES11p,XSS11p
       DOUBLE PRECISION XMP11n,XSP11n,XES11n,XSS11n
c *****************************************************************
       DOUBLE PRECISION fk,fpi,DE,DM,DS,Fak,GaGa,GaPi,GaT
       DOUBLE PRECISION mi,mf,mPION,OmegL,Q2,Q2G,W,qcm,kcm,qcm0,kgcm
       DOUBLE PRECISION W0, Ga0, X, S1, FQP, FQN, FQ, WX
       DOUBLE PRECISION Phi(3),kcm0,Ga,Gpi,ReM1M(3),ImM1M(3),mPi
       DOUBLE PRECISION Pi,mn,mdip,wGacm,kx,ReM1(3),ImM1(3)
       DOUBLE PRECISION alp,q2EVF0,q2EVF,gam0,gam,EXP0
	DOUBLE PRECISION k2pi0,k2pi,w2thr,finel
       INTEGER i, Reaction, Unq
       DOUBLE PRECISION del_R, phi_c, phi_s11,phi_s31,phi_p11,phi_p31
       DOUBLE PRECISION phi_p13, phi_p33, phi_d13, phi_d33
       DOUBLE PRECISION phi_d15, phi_d35, phi_f15, phi_f35
       DOUBLE PRECISION phi_f17, phi_f37
       COMMON /PHASES/ phi_s11, phi_s31, phi_p11, phi_p31,
     *                phi_p13, phi_p33, phi_d13, phi_d33,
     *                phi_d15, phi_d35, phi_f15, phi_f35,
     *                phi_f17, phi_f37
          COMMON /Mass/ mi, mf, mPION
          COMMON /KinVar/ OmegL, Q2, W, wGacm, kcm
       PARAMETER (Pi=3.14159265358979D0, mdip=0.71, alp=370.)
       PARAMETER(W0=1440.,Ga0=350., X=470.)

	mn = (mi+mf)/2.
c **************************************
        MPI=139.5685
        qcm=sqrt(wGacm**2+Q2)
c *******************************************************
	kcm0 = SQRT(((W0*W0+mPi*mPi-mf*mf)/(2*W0))**2-mPi*mPi)
	qcm0 = SQRT(((W0*W0+Q2+mi*mi)/2./W0)**2-mi*mi)
	k2pi0=SQRT(((W0*W0+4.*mPi*mPi-mf*mf)/(2*W0))**2-4.*mPi*mPi)
        w2thr=mf+2.*mPi
        k2pi=0.
        if (W.gt.w2thr)
     *	k2pi=SQRT(((W*W+4.*mPi*mPi-mf*mf)/(2*W))**2-4.*mPi*mPi)
	kgcm = (W*W-mi*mi)/2./W
C ******************************************************
        FQ=(qcm/kgcm)*EXP(-Q2/6./alp**2)
c        FQ=1.0
c ******************************************************
	fk = (qcm/qcm0)*((qcm0*qcm0+X*X)/(qcm*qcm+X*X))
	fpi = (kcm/kcm0)**3*(kcm0*kcm0+X*X)/(kcm*kcm+X*X)
	finel=(k2pi/k2pi0)**6*((k2pi0*k2pi0+X*X)/
     /  (k2pi*k2pi+X*X))**3
	Q2G = Q2/1.D6
c ******************************************************
        FQP=1.d0
        FQN=1.d0
        IF (Unq.eq.1)  FQP=(1.+9.9768*Q2G)/(1.+2.3842*Q2G)
        IF (Unq.eq.1)  FQN=(1.+9.9768*Q2G)/(1.+2.3842*Q2G)
c ******************************************************
c      default values for proton M1-, S1-
c ******************************************************
	  IF (Reaction.EQ.1 .OR. Reaction.EQ.3) THEN
c          DM = -fk*75.*FQ
          DM = -85.*FQ
	  DS = -40.*FQ
c*************************************************************
C      default values for neutron M1-,  S1-
C*******************************************************************
	  ELSE IF (Reaction.EQ.2 .OR. Reaction.EQ.4) THEN
      	  DM = 66.*FQ
	  DS = 40.*FQ
	  END IF
c ************************************************
	GaGa = kgcm*qcm/2./Pi*mn/W0/1.D9
        GaPi=0.70*Ga0*fpi*W0/W
	GaT =GaPi+0.30*Ga0*finel
        Fak=W0/((W0*W0-W*W)**2+W0*W0*GaT*GaT)
     * *SQRT(GaGa*GaPi/qcm/kcm)
c ******************************************************
	  ReM1M(1) = 0
	  ImM1M(1) = 0
	  ReM1(2) = Fak* (W0*W0-W*W)*DM*mPi*fk
	  ImM1(2) = Fak* W0*GaT*DM*mPi*fk
	  ReM1(3) = Fak* (W0*W0-W*W)*DS*mPi*wGacm/qcm*fk
	  ImM1(3) = Fak* W0*GaT*mPi*DS*wGacm/qcm*fk

c******************************************************************************
c resonance phase is the same for all 2 multipoles
c it must be calculated for an amplitude .ne. 0.0
c******************************************************************************
          if (ReM1(2).ne.0.0) then
           del_R = datan2(ImM1(2),ReM1(2))
          elseif (ReM1(3).ne.0.0) then
           del_R = datan2(ImM1(3),ReM1(3))
          else
	    del_R = pi/2.
          endif
          IF (del_R.LT.0.0) del_R = del_R + pi
          phi_c = phi_p11 - del_R
          Phi(1) = phi_c
          Phi(2) = phi_c
          Phi(3) = phi_c
c*************************************************************
          ReM1M(1)= 0.
          ImM1M(1)= 0.
	  IF (Reaction.EQ.1 .OR. Reaction.EQ.3) THEN
       ReM1M(2)=XMP11p*(ReM1(2)*DCOS(Phi(2))-ImM1(2)*DSIN(Phi(2)))
       ImM1M(2)=XMP11p*(ReM1(2)*DSIN(Phi(2))+ImM1(2)*DCOS(Phi(2)))
       ReM1M(3)=XSP11p*(ReM1(3)*DCOS(Phi(3))-ImM1(3)*DSIN(Phi(3)))
       ImM1M(3)=XSP11p*(ReM1(3)*DSIN(Phi(3))+ImM1(3)*DCOS(Phi(3)))
	  ELSE IF (Reaction.EQ.2 .OR. Reaction.EQ.4) THEN
       ReM1M(2)=XMP11n*(ReM1(2)*DCOS(Phi(2))-ImM1(2)*DSIN(Phi(2)))
       ImM1M(2)=XMP11n*(ReM1(2)*DSIN(Phi(2))+ImM1(2)*DCOS(Phi(2)))
       ReM1M(3)=XSP11n*(ReM1(3)*DCOS(Phi(3))-ImM1(3)*DSIN(Phi(3)))
       ImM1M(3)=XSP11n*(ReM1(3)*DSIN(Phi(3))+ImM1(3)*DCOS(Phi(3)))
	  END IF
       return
       END


	SUBROUTINE D13NEW(Reaction,Unq,ReM2M,ImM2M)
C******************************************************************************
C      multipoles M2- and E2- for D13(1520) with fixed param.  no L2-
C******************************************************************************
	IMPLICIT NONE
c ***************************************************************+
       COMMON/parDFp/ XED13p,XMD13p,XSD13p,XEF15p,XMF15p,XSF15p
       COMMON/parDFn/ XED13n,XMD13n,XSD13n,XEF15n,XMF15n,XSF15n
       DOUBLE PRECISION XED13p,XMD13p,XSD13p,XEF15p,XMF15p,XSF15p
       DOUBLE PRECISION XED13n,XMD13n,XSD13n,XEF15n,XMF15n,XSF15n
       DOUBLE PRECISION  XE,XM,XS
c *****************************************************************
       DOUBLE PRECISION fk,fpi,DE,DM,Fak,GaGa,GaPi,GaT,Ga0,X,W0
       DOUBLE PRECISION mi,mf,mPION,OmegL,Q2,Q2G,W,qcm,kcm,qcm0,kgcm
       DOUBLE PRECISION Phi(3),kcm0,Ga,Gpi,ReM2M(3),ImM2M(3),mPi
       DOUBLE PRECISION Pi,mn,mdip,wGacm,kx,ReM2(3),ImM2(3)
       DOUBLE PRECISION TE,TM,SE,SM,FQE,FQM
       DOUBLE PRECISION k2pi0,k2pi,w2thr,finel
       INTEGER i, Reaction, Unq
          COMMON /Mass/ mi, mf, mPION
          COMMON /KinVar/ OmegL, Q2, W, wGacm, kcm
       PARAMETER (Pi=3.14159265358979D0, mdip=0.71)
       PARAMETER(W0=1520.,Ga0=130., X=500.)


	mn = (mi+mf)/2.
c **************************************
        MPI=139.5685
        qcm=sqrt(wGacm**2+Q2)
c **************************************
	kcm0 = SQRT(((W0*W0+mPi*mPi-mf*mf)/(2*W0))**2-mPi*mPi)
	kgcm = (W*W-mi*mi)/2./W
	qcm0 = SQRT(((W0*W0+Q2+mi*mi)/2./W0)**2-mi*mi)
	k2pi0=SQRT(((W0*W0+4.*mPi*mPi-mf*mf)/(2*W0))**2-4.*mPi*mPi)
        w2thr=mf+2.*mPi
        k2pi=0.
        if (W.gt.w2thr)
     *	k2pi=SQRT(((W*W+4.*mPi*mPi-mf*mf)/(2*W))**2-4.*mPi*mPi)
c *********************************************************
	fk = (qcm/qcm0)**2*((qcm0*qcm0+X*X)/(qcm*qcm+X*X))
	fpi = (kcm/kcm0)**5*((kcm0*kcm0+X*X)/(kcm*kcm+X*X))**2
	finel=(k2pi/k2pi0)**8*((k2pi0*k2pi0+X*X)/
     /  (k2pi*k2pi+X*X))**4
        GaPi=0.60*Ga0*fpi*W0/W
	GaT =GaPi+0.40*Ga0*finel
	GaGa = kgcm*qcm/Pi*mn/W0/4./1.D9
 	Fak = W0/((W0*W0-W*W)**2+W0*W0*GaT*GaT)
     * *SQRT(GaGa*GaPi/qcm/kcm)
c *********************************************************
            CALL AQRES(1,Reaction,Q2,TE,TM,SE,SM)
	Q2G = Q2/1.D6
        FQE=1.d0
        FQM=1.d0
        IF (Unq.EQ.1) FQE=(1.+15.17*Q2G)/(1.+21.98*Q2G)
        IF (Unq.EQ.1) FQM=(1.+1.5626*Q2G)/(1.+5.2015*Q2G)
c ******************************************************
	  IF (Reaction.EQ.1 .OR. Reaction.EQ.3) THEN
        Phi(1) = 23*Pi/180.*FQE
        Phi(2) = 35*Pi/180.*FQM
        Phi(3) = Phi(1)
        DE=TE*fk/DCOS(Phi(1))
        DM=TM*fk*(qcm/qcm0)**3/DCOS(Phi(2))
	XE=XED13p
	XM=XMD13p
	XS=XSD13p
	  ELSE IF (Reaction.EQ.2 .OR. Reaction.EQ.4) THEN
        Phi(1) = 23.*Pi/180.*FQE
        Phi(2) = 35.*Pi/180.*FQM
        Phi(3) = Phi(1)
        DE=1.1*TE*fk/DCOS(Phi(1))
        DM=TM*fk*(qcm/qcm0)**3/DCOS(Phi(2))
	XE=XED13n
	XM=XMD13n
	XS=XSD13n

	  END IF
c **************************************************

        ReM2(1) =Fak* (W0*W0-W*W)*DE*mPi*XE
	ImM2(1) =Fak* W0*GaT*DE*mPi*XE
	ReM2(2) =Fak*(W0*W0-W*W)*DM*mPi*XM
	ImM2(2) =Fak* W0*GaT*DM*mPi*XM
	ReM2(3) =-wGacm/qcm*Fak* (W0*W0-W*W)*DE*mPi*XS/2.
	ImM2(3) =-wGacm/qcm*Fak* W0*GaT*DE*mPi*XS/2.

          DO 111 i=1,3
       ReM2M(i) = ReM2(i)*DCOS(Phi(i))-ImM2(i)*DSIN(Phi(i))
       ImM2M(i) = ReM2(i)*DSIN(Phi(i))+ImM2(i)*DCOS(Phi(i))
 111      CONTINUE
       RETURN
	END
C
       SUBROUTINE D33NEW(Unq,ReD33,ImD33)
C******************************************************************************
C      multipoles M2- and E2-  for D33(1700) with fixed param.  no L2-
C******************************************************************************
       IMPLICIT NONE
c ***************************************************************+
       COMMON/param3/ XMP33,XEP33,XSP33,XES31,XSS31,XED33,XMD33,XSD33
       DOUBLE PRECISION XMP33,XEP33,XSP33,XES31,XSS31,XED33,XMD33,XSD33
c *****************************************************************
       DOUBLE PRECISION fk,fpi,DE,DM,Fak,GaGa,GaPi,GaT
       DOUBLE PRECISION mi,mf,mPION,OmegL,Q2,Q2G,W,qcm,kcm,qcm0,kgcm
       DOUBLE PRECISION W0,Ga0,X,k2pi0,k2pi,w2thr,finel
       DOUBLE PRECISION Phi(3),kcm0,Ga,Gpi,ReD33(3),ImD33(3),mPi
       DOUBLE PRECISION Pi,mn,mdip,wGacm,kx,ReD3(3),ImD3(3)
       DOUBLE PRECISION TE,TM,SE,SM,FQE,FQM
       INTEGER i, Reaction, Unq
          COMMON /Mass/ mi, mf, mPION
          COMMON /KinVar/ OmegL, Q2, W, wGacm, kcm
       PARAMETER (Pi=3.14159265358979D0, mdip=0.71)
       PARAMETER(W0=1740.,Ga0=450., X=500.)

	mn = (mi+mf)/2.
        mPi=139.5685
	kcm0 = SQRT(((W0*W0+mPi*mPi-mf*mf)/(2*W0))**2-mPi*mPi)
        qcm0 = SQRT(((W0*W0+Q2+mi*mi)/(2*W0))**2-mi*mi)
        kgcm = (W*W-mi*mi)/2./W
        qcm=sqrt(wGacm**2+Q2)
	Q2G = Q2/1.D6

	k2pi0=SQRT(((W0*W0+4.*mPi*mPi-mf*mf)/(2*W0))**2-4.*mPi*mPi)
        w2thr=mf+2.*mPi
        k2pi=0.
        if (W.gt.w2thr)
     *	k2pi=SQRT(((W*W+4.*mPi*mPi-mf*mf)/(2*W))**2-4.*mPi*mPi)
C **********************************************
	fk = (qcm/qcm0)**2*(qcm0*qcm0+X*X)/(qcm*qcm+X*X)
	fpi = (kcm/kcm0)**5*((kcm0*kcm0+X*X)/(kcm*kcm+X*X))**2
	finel=(k2pi/k2pi0)**8*((k2pi0*k2pi0+X*X)/
     /  (k2pi*k2pi+X*X))**4
        GaPi=0.15*Ga0*fpi*W0/W
	GaT =GaPi+0.85*Ga0*finel
	GaGa = kgcm*qcm/Pi*mn/W0/4./1.D9
 	Fak = W0/((W0*W0-W*W)**2+W0*W0*GaT*GaT)
     * *SQRT(GaGa*GaPi/qcm/kcm)
c **************************************************
            CALL AQRES(4,1,Q2,TE,TM,SE,SM)
	Q2G = Q2/1.D6
        FQE=1.d0
        FQM=1.d0
c *********************************************************
        Phi(1) = 61.*Pi/180.*FQE
        Phi(2) = 61.*Pi/180.*FQM
        Phi(3) = Phi(1)
        DE=TE*fk*(qcm/qcm0)**2/DCOS(Phi(1))
        DM=TM*fk*(qcm/qcm0)**2/DCOS(Phi(2))
c *************************************************

	  ReD3(1) = Fak* (W0*W0-W*W)*DE*mPi*XED33
	  ImD3(1) = Fak* W0*GaT*DE*mPi*XED33
	  ReD3(2) = Fak* (W0*W0-W*W)*DM*mPi*XMD33
	  ImD3(2) = Fak* W0*GaT*DM*mPi*XMD33
          ReD3(3)=- wGacm/qcm*Fak* (W0*W0-W*W)*DE*mPi*XSD33
          ImD3(3)=- wGacm/qcm*Fak* W0*GaT*DE*mPi*XSD33
          DO 111 I=1,3
       ReD33(i) = ReD3(i)*DCOS(Phi(i))-ImD3(i)*DSIN(Phi(i))
       ImD33(i) = ReD3(i)*DSIN(Phi(i))+ImD3(i)*DCOS(Phi(i))
  111     CONTINUE

       RETURN
	END
c
       SUBROUTINE S11fst(Reaction,ReM0P,ImM0P)
C******************************************************************************
C      multipoles E0+ and L0+  for S11(1535) with fit parameters RE, RL
C      RL = 0  currently
C******************************************************************************
       IMPLICIT NONE
c ********************************************************************
       COMMON/par11p/ XMP11p,XSP11p,XES11p,XSS11p
       COMMON/par11n/ XMP11n,XSP11n,XES11n,XSS11n
       DOUBLE PRECISION XMP11p,XSP11p,XES11p,XSS11p
       DOUBLE PRECISION XMP11n,XSP11n,XES11n,XSS11n
       DOUBLE PRECISION XE, XS
c *****************************************************************
       DOUBLE PRECISION fk,fpi,DE,GaGa,GaPi,GaT
       DOUBLE PRECISION mi,mf,mPION,OmegL,Q2,Q2G,W,qcm,kcm,qcm0,kgcm
       DOUBLE PRECISION ALPHE, ALPHM, W0, Ga0, X
       DOUBLE PRECISION Phi(3),kcm0,Ga,Gpi,ReM0P(3),ImM0P(3),mPi
       DOUBLE PRECISION Pi,mn,mdip,wGacm,Fak,kx,ReM0(3),ImM0(3)
       DOUBLE PRECISION k2pi0,k2pi,w2thr,finel
       DOUBLE PRECISION qj, qj0, GaEta, mEta, Dis
       DOUBLE PRECISION TE,TM,SE,SM
       DOUBLE PRECISION del_R, phi_c, phi_s11,phi_s31,phi_p11,phi_p31
       DOUBLE PRECISION phi_p13, phi_p33, phi_d13, phi_d33
       DOUBLE PRECISION phi_d15, phi_d35, phi_f15, phi_f35
       DOUBLE PRECISION phi_f17, phi_f37
       COMMON /PHASES/ phi_s11, phi_s31, phi_p11, phi_p31,
     *                phi_p13, phi_p33, phi_d13, phi_d33,
     *                phi_d15, phi_d35, phi_f15, phi_f35,
     *                phi_f17, phi_f37
       INTEGER i, Reaction
       COMMON /Mass/ mi, mf, mPION
       COMMON /KinVar/ OmegL, Q2, W, wGacm, kcm
       PARAMETER (Pi=3.14159265358979D0, mdip=0.71)
       PARAMETER(W0=1520.,Ga0=80., X=500.,mEta=549.)
c
        mn = (mi+mf)/2.
        mPi=139.5685
        qcm=sqrt(wGacm**2+Q2)
c **************************************
        kcm0 = SQRT(((W0*W0+mPi*mPi-mf*mf)/(2*W0))**2-mPi*mPi)
        qcm0 = SQRT(((W0*W0+Q2+mi*mi)/2./W0)**2-mi*mi)
        k2pi0=SQRT(((W0*W0+4.*mPi*mPi-mf*mf)/(2*W0))**2-4.*mPi*mPi)
        w2thr=mf+2.*mPi
        k2pi=0.
        if (W.gt.w2thr)
     *  k2pi=SQRT(((W*W+4.*mPi*mPi-mf*mf)/(2*W))**2-4.*mPi*mPi)
        finel=(k2pi/k2pi0)**4*((k2pi0*k2pi0+X*X)/
     /  (k2pi*k2pi+X*X))**2
c ********************************************************
        fk = (qcm/qcm0)**2*(qcm0*qcm0+X*X)/(qcm*qcm+X*X)
        kgcm = (W*W-mi*mi)/2./W

        GaPi = 0.40*Ga0*kcm/kcm0*W0/W

        Dis = ((W*W-mEta*mEta+mf*mf)/2./W)**2-mf*mf
        IF (Dis.LE.0.) THEN
          qj = 0.
        ELSE IF (Dis.GT.0.) THEN
          qj = SQRT(Dis)
        END IF
        qj0 = SQRT(((W0*W0-mEta*mEta+mf*mf)/2./W0)**2-mf*mf)
        GaEta = 0.50*Ga0*qj/qj0*W0/W
        GaT = GaPi+GaEta+0.10*Ga0*finel

        GaGa = kgcm*qcm/Pi*mn/W0/2.D9
        Fak=W0/((W0*W0-W*W)**2+W0*W0*GaT*GaT)
     *  *SQRT(GaGa*GaPi/qcm/kcm)
c *************************************************************
        CALL AQRES(2,Reaction,Q2,TE,TM,SE,SM)
        Phi(1) = 0.D0
        Phi(2) = 0.D0
        Phi(3) = 0.D0

        IF (Reaction.EQ.1 .OR. Reaction.EQ.3) THEN
        DE=TE*fk
	XE=XES11p
	XS=XSS11p
        ELSE IF (Reaction.EQ.2 .OR. Reaction.EQ.4) THEN
        DE=0.7*TE*fk
	XE=XES11n
	XS=XSS11n

	END IF
c *************************************************************
          ReM0(1) = Fak*(W0*W0-W*W)*DE*mPi
          ImM0(1) = Fak*W0*GaT*DE*mPi
c *************************************************************
          del_R=pi/2.
          IF (DABS(ReM0(1)).GT.1.E-6)
     &     del_R = datan2(ImM0(1),ReM0(1))
          IF (del_R.LT.0.0) del_R = del_R + pi
          Phi(1) = phi_s11 - del_R
         IF (W.GT.1300.) Phi(1)=0.143
c          IF (W.GT.1300.) Phi(1)=0.
c*************************************************************
          ReM0P(1)=XE*(DCOS(Phi(1))*ReM0(1)-DSIN(Phi(1))*ImM0(1))
          ImM0P(1)=XE*(DSIN(Phi(1))*ReM0(1)+DCOS(Phi(1))*ImM0(1))
          ReM0P(2) = 0.
          ImM0P(2) = 0.
          ReM0P(3)=XS*(DCOS(Phi(1))*ReM0(1)-DSIN(Phi(1))*ImM0(1))*
     &    (wGacm/qcm)
          ImM0P(3)=XS*(DSIN(Phi(1))*ReM0(1)+DCOS(Phi(1))*ImM0(1))*
     &	  (wGacm/qcm)

        RETURN
        END
c
       SUBROUTINE S11sec(Reaction,ReM0P,ImM0P)
C******************************************************************************
C      multipoles E0+ for S11(1650)  with fixed parameters
C      L0+ = 0
C******************************************************************************
       IMPLICIT NONE
c *****************************************************
       COMMON/secS11/ XES11p,XSS11p, XES11n,XSS11n
       DOUBLE PRECISION XES11p,XSS11p,XES11n,XSS11n, XE, XS
c *****************************************************
       DOUBLE PRECISION fk,fpi,DE,Fak,GaGa,GaPi,GaT
       DOUBLE PRECISION mi,mf,mPION,OmegL,Q2,Q2G,W,qcm,kcm,qcm0,kgcm
       DOUBLE PRECISION ALPHE, ALPHM, W0, Ga0, X
       DOUBLE PRECISION Phi(3),kcm0,Ga,Gpi,ReM0P(3),ImM0P(3),mPi
       DOUBLE PRECISION Pi,mn,mdip,wGacm,kx,ReM0(3),ImM0(3)
       DOUBLE PRECISION k2pi0,k2pi,w2thr,finel
       DOUBLE PRECISION TE,TM,SE,SM
       DOUBLE PRECISION del_R, phi_c, phi_s11,phi_s31,phi_p11,phi_p31
       DOUBLE PRECISION phi_p13, phi_p33, phi_d13, phi_d33
       DOUBLE PRECISION phi_d15, phi_d35, phi_f15, phi_f35
       DOUBLE PRECISION phi_f17, phi_f37
       COMMON /PHASES/ phi_s11, phi_s31, phi_p11, phi_p31,
     *                phi_p13, phi_p33, phi_d13, phi_d33,
     *                phi_d15, phi_d35, phi_f15, phi_f35,
     *                phi_f17, phi_f37
       INTEGER i, Reaction
       COMMON /Mass/ mi, mf, mPION
       COMMON /KinVar/ OmegL, Q2, W, wGacm, kcm
       PARAMETER (Pi=3.14159265358979D0, mdip=0.71)
C       PARAMETER(W0=1650.,Ga0=100., X=500.)
       PARAMETER(W0=1690.,Ga0=100., X=500.)
c
        mn = (mi+mf)/2.
        mPi=139.5685
        qcm=sqrt(wGacm**2+Q2)
        kcm0 = SQRT(((W0*W0+mPi*mPi-mf*mf)/(2*W0))**2-mPi*mPi)
        qcm0 = SQRT(((W0*W0+Q2+mi*mi)/2./W0)**2-mi*mi)
        k2pi0=SQRT(((W0*W0+4.*mPi*mPi-mf*mf)/(2*W0))**2-4.*mPi*mPi)
        w2thr=mf+2.*mPi
        k2pi=0.
        if (W.gt.w2thr)
     *  k2pi=SQRT(((W*W+4.*mPi*mPi-mf*mf)/(2*W))**2-4.*mPi*mPi)
        kgcm = (W*W-mi*mi)/2./W
c ************************************************************
        fk = (qcm/qcm0)**4*(qcm0*qcm0+X*X)/(qcm*qcm+X*X)
        finel=(k2pi/k2pi0)**4*((k2pi0*k2pi0+X*X)/
     /  (k2pi*k2pi+X*X))**2
c
        GaPi = 0.85*Ga0*kcm/kcm0*W0/W
        GaT =GaPi+0.15*Ga0*finel
        GaGa = kgcm*qcm/Pi*mn/W0/2.D9
        Fak=W0/((W0*W0-W*W)**2+W0*W0*GaT*GaT)
     *  *SQRT(GaGa*GaPi/qcm/kcm)
c *************************************************************
        CALL AQRES(2,Reaction,Q2,TE,TM,SE,SM)
C ****************************************************
          IF (Reaction.EQ.1 .OR. Reaction.EQ.3) THEN
        Phi(1) = 0.*Pi/180.
        Phi(2) = 0.D0
        Phi(3) = 0.D0
	XE=XES11p
	XS=XSS11p

          ELSE IF (Reaction.EQ.2 .OR. Reaction.EQ.4) THEN
        Phi(1) = 0.*Pi/180.
        Phi(2) = 0.D0
        Phi(3) = 0.D0
	XE=XES11n
	XS=XSS11n
          END IF

        DE=TE*fk*DTAN(30.*Pi/180.)/DCOS(Phi(1))
c
        IF (Reaction.EQ.2 .OR. Reaction.EQ.4) DE=0.7*DE
c *************************************************************
          ReM0(1) = Fak*(W0*W0-W*W)*DE*mPi
          ImM0(1) = Fak*W0*GaT*DE*mPi
c*************************************************************
          del_R=pi/2.
          IF (DABS(ReM0(1)).GT.1.E-6)
     &     del_R = datan2(ImM0(1),ReM0(1))
          IF (del_R.LT.0.0) del_R = del_R + pi
          Phi(1) = phi_s11 - del_R
         IF (W.GT.1300.) Phi(1)=0.1215
c          IF (W.GT.1300.) Phi(1)=0.
c*************************************************************
          ReM0P(1)=XE*(DCOS(Phi(1))*ReM0(1)-DSIN(Phi(1))*ImM0(1))
          ImM0P(1)=XE*(DSIN(Phi(1))*ReM0(1)+DCOS(Phi(1))*ImM0(1))
          ReM0P(2) = 0.
          ImM0P(2) = 0.
          ReM0P(3)=XS*(DCOS(Phi(1))*ReM0(1)-DSIN(Phi(1))*ImM0(1))*
     &    (wGacm/qcm)
          ImM0P(3)=XS*(DSIN(Phi(1))*ReM0(1)+DCOS(Phi(1))*ImM0(1))*
     &	  (wGacm/qcm)
c	   write (6,267) XS,ReM0P(3),ImM0P(3)
c 267       format(1x,3(e10.4,2x))
c ***************************************************************
        RETURN
        END
C
       SUBROUTINE S31NEW(Unq,ReS31,ImS31)
       IMPLICIT NONE
c ****************************************************************
       COMMON/param3/ XMP33,XEP33,XSP33,XES31,XSS31,XED33,XMD33,XSD33
       DOUBLE PRECISION XMP33,XEP33,XSP33,XES31,XSS31,XED33,XMD33,XSD33
c *****************************************************************
       DOUBLE PRECISION fk,fpi,DE,DM,Fak,GaGa,GaPi,GaT
       DOUBLE PRECISION mi,mf,mPION,OmegL,Q2,Q2G,W,qcm,kcm,qcm0,kgcm
       DOUBLE PRECISION W0,Ga0,X,k2pi0,k2pi,w2thr,finel
       DOUBLE PRECISION Phi(3),kcm0,Ga,Gpi,ReS31(3),ImS31(3),mPi
       DOUBLE PRECISION Pi,mn,mdip,wGacm,kx,ReS3(3),ImS3(3)
       DOUBLE PRECISION TE,TM,SE,SM,FQE,FQM,TE0,TM0,SE0,SM0
       INTEGER i, Reaction, Unq
       DOUBLE PRECISION del_R, phi_c, phi_s11,phi_s31,phi_p11,phi_p31
       DOUBLE PRECISION phi_p13, phi_p33, phi_d13, phi_d33
       DOUBLE PRECISION phi_d15, phi_d35, phi_f15, phi_f35
       DOUBLE PRECISION phi_f17, phi_f37
       COMMON /PHASES/ phi_s11, phi_s31, phi_p11, phi_p31,
     *                phi_p13, phi_p33, phi_d13, phi_d33,
     *                phi_d15, phi_d35, phi_f15, phi_f35,
     *                phi_f17, phi_f37
          COMMON /Mass/ mi, mf, mPION
          COMMON /KinVar/ OmegL, Q2, W, wGacm, kcm
       PARAMETER (Pi=3.14159265358979D0, mdip=0.71)
       PARAMETER(W0=1620.,Ga0=150., X=470.)

	mn = (mi+mf)/2.
        mPi=139.5685
	kcm0 = SQRT(((W0*W0+mPi*mPi-mf*mf)/(2*W0))**2-mPi*mPi)
        qcm0 = SQRT(((W0*W0+Q2+mi*mi)/(2*W0))**2-mi*mi)
        kgcm = (W*W-mi*mi)/2./W
        qcm=sqrt(wGacm**2+Q2)
	Q2G = Q2/1.D6

	k2pi0=SQRT(((W0*W0+4.*mPi*mPi-mf*mf)/(2*W0))**2-4.*mPi*mPi)
        w2thr=mf+2.*mPi
        k2pi=0.
        if (W.gt.w2thr)
     *	k2pi=SQRT(((W*W+4.*mPi*mPi-mf*mf)/(2*W))**2-4.*mPi*mPi)
C **********************************************
	fk = (qcm/qcm0)*(qcm0*qcm0+X*X)/(qcm*qcm+X*X)
	fpi = kcm/kcm0
	finel=(k2pi/k2pi0)**4*((k2pi0*k2pi0+X*X)/
     /  (k2pi*k2pi+X*X))**2
        GaPi=0.25*Ga0*fpi*W0/W
	GaT =GaPi+0.75*Ga0*finel
	GaGa = kgcm*qcm/Pi*mn/W0/4./1.D9
 	Fak = W0/((W0*W0-W*W)**2+W0*W0*GaT*GaT)
     * *SQRT(GaGa*GaPi/qcm/kcm)
c **************************************************
            CALL AQRES(2,1,Q2,TE,TM,SE,SM)
            CALL AQRES(2,1,0.D0,TE0,TM0,SE0,SM0)
        TE =120.*fk *TE/TE0
	Q2G = Q2/1.D6
        FQE=1.d0
c *********************************************************
        Phi(1) = 0.0
        Phi(2) = 0.0
        Phi(3) = 0.D0
c *************************************************

	  ReS3(1) = Fak* (W0*W0-W*W)*TE*mPi
	  ImS3(1) = Fak* W0*GaT*TE*mPi
          ReS3(2)=0.D0
          ImS3(2)=0.D0
          ReS3(3)=(wGacm/qcm)*ReS3(1)
          ImS3(3)=(wGacm/qcm)*ImS3(1)

c*************************************************************
          del_R = atan(ImS3(1)/ReS3(1))
          IF (del_R.LT.0.0) del_R = del_R + pi
          phi_c = phi_S31 - del_R
          Phi(1) = phi_c
c*************************************************************
          ReS31(2)=0.D0
          ImS31(2)=0.D0
       ReS31(1)=XES31*(ReS3(1)*DCOS(Phi(1))-ImS3(1)*DSIN(Phi(1)))
       ImS31(1)=XES31*(ReS3(1)*DSIN(Phi(1))+ImS3(1)*DCOS(Phi(1)))
       ReS31(3)=XSS31*(ReS3(3)*DCOS(Phi(1))-ImS3(3)*DSIN(Phi(1)))
       ImS31(3)=XSS31*(ReS3(3)*DSIN(Phi(1))+ImS3(3)*DCOS(Phi(1)))

       RETURN
	END

	SUBROUTINE F15NEW(Reaction,Unq,ReM3M,ImM3M)
C******************************************************************************
C      multipoles E3- and M3-  for F15(1680)  with fixed parameters
C      L3- = 0
C******************************************************************************
	IMPLICIT NONE
c ***************************************************************+
       COMMON/parDFp/ XED13p,XMD13p,XSD13p,XEF15p,XMF15p,XSF15p
       COMMON/parDFn/ XED13n,XMD13n,XSD13n,XEF15n,XMF15n,XSF15n
       DOUBLE PRECISION XED13p,XMD13p,XSD13p,XEF15p,XMF15p,XSF15p
       DOUBLE PRECISION XED13n,XMD13n,XSD13n,XEF15n,XMF15n,XSF15n
       DOUBLE PRECISION XE, XM, XS
c *****************************************************************
       DOUBLE PRECISION fk,fpi,DE,DM,Fak,GaGa,GaPi,GaT,W0, Ga0, X
       DOUBLE PRECISION mi,mf,mPION,OmegL,Q2,Q2G,W,qcm,kcm,qcm0,kgcm
       DOUBLE PRECISION Phi(3),kcm0,Ga,Gpi,ReM3M(3),ImM3M(3),mPi
       DOUBLE PRECISION Pi,mn,mdip,wGacm,kx,ReM3(3),ImM3(3)
       DOUBLE PRECISION k2pi0,k2pi,w2thr,finel
       DOUBLE PRECISION TE,TM,SE,SM,FQE,FQM
       INTEGER i, Reaction, Unq
          COMMON /Mass/ mi, mf, mPION
          COMMON /KinVar/ OmegL, Q2, W, wGacm, kcm
       PARAMETER (Pi=3.14159265358979D0, mdip=0.71)
       PARAMETER(W0=1680.,Ga0=135., X=500.)

	mn = (mi+mf)/2.
c **************************************
        mPi=139.5685
        qcm=sqrt(wGacm**2+Q2)
c **************************************
	kcm0 = SQRT(((W0*W0+mPi*mPi-mf*mf)/(2*W0))**2-mPi*mPi)
	kgcm = (W*W-mi*mi)/2./W
	qcm0 = SQRT(((W0*W0+Q2+mi*mi)/2./W0)**2-mi*mi)
	k2pi0=SQRT(((W0*W0+4.*mPi*mPi-mf*mf)/(2*W0))**2-4.*mPi*mPi)
        w2thr=mf+2.*mPi
        k2pi=0.
        if (W.gt.w2thr)
     *	k2pi=SQRT(((W*W+4.*mPi*mPi-mf*mf)/(2*W))**2-4.*mPi*mPi)
c ***************************************************************
	fk = (qcm/qcm0)**3*((qcm0*qcm0+X*X)/(qcm*qcm+X*X))
	fpi = (kcm/kcm0)**7*((kcm0*kcm0+X*X)/(kcm*kcm+X*X))**3
	finel=(k2pi/k2pi0)**10*((k2pi0*k2pi0+X*X)/
     /  (k2pi*k2pi+X*X))**5
        GaPi=0.70*Ga0*fpi*W0/W
	GaT =GaPi+0.30*Ga0*finel
	GaGa = kgcm*qcm/Pi*mn/W0/6./1.D9
	Fak = W0/((W0*W0-W*W)**2+W0*W0*GaT*GaT)
     * *SQRT(GaGa*GaPi/qcm/kcm)
c ***************************************************************
        CALL AQRES(3,Reaction,Q2,TE,TM,SE,SM)

	Q2G = Q2/1.D6
        FQE=1.d0
        FQM=1.d0
        IF (Unq.EQ.1) FQM=(1.-0.27538*Q2G)/(1.+3.5535*Q2G)
        IF (Unq.EQ.1) FQE=(1.-0.06975*Q2G)/(1.+3.7657*Q2G)
c ***************************************************************
	IF (Reaction.EQ.1 .OR. Reaction.EQ.3) THEN
        Phi(1) = 15*Pi/180.*FQE
        Phi(2) = 15*Pi/180.*FQM
        Phi(3) = Phi(1)
        DE=TE*fk/DCOS(Phi(1))
        DM=TM*fk*qcm/qcm0/DCOS(Phi(2))
	XE=XEF15p
	XM=XMF15p
	XS=XSF15p

	ELSE IF (Reaction.EQ.2 .OR. Reaction.EQ.4) THEN

        Phi(1) = 15.*Pi/180.*FQE
        Phi(2) = 15.*Pi/180.*FQM
        Phi(3) = Phi(1)

        DE=0.9*TE*fk/DCOS(Phi(1))
        DM=0.9*TM*fk*qcm/qcm0/DCOS(Phi(2))
	XE=XEF15n
	XM=XMF15n
	XS=XSF15n

	END IF
c ******************************************************

	ReM3(1)=Fak* (W0*W0-W*W)*DE*mPi*XE
	ImM3(1)=Fak* W0*GaT*DE*mPi*XE
	ReM3(2)=Fak* (W0*W0-W*W)*DM*mPi*XM
	ImM3(2)=Fak* W0*GaT*DM*mPi*XM
	ReM3(3) =-wGacm/qcm*Fak* (W0*W0-W*W)*DE*mPi*XS/3.
	ImM3(3) =-wGacm/qcm*Fak* W0*GaT*DE*mPi*XS/3.

          DO 111 i=1,3
       ReM3M(i) = ReM3(i)*DCOS(Phi(i))-ImM3(i)*DSIN(Phi(i))
       ImM3M(i) = ReM3(i)*DSIN(Phi(i))+ImM3(i)*DCOS(Phi(i))
 111      CONTINUE

        RETURN
	END


        SUBROUTINE AQRES(NRES,IR,Q2MEV,TE,TM,SE,SM)
c *****************************************************************************
c     e.m. couplings and form factors of resonance excitation (except P33)
c     from quark model     for: S11, D13, D33, F15
c *****************************************************************************
        IMPLICIT REAL*8 (A-H,O-Z)
        DOUBLE PRECISION m11, m12, m22, m23, mn, mPi, mf, mi
        PARAMETER ( Pi = 3.1415926536D0, hqc = 197.3285)
        PARAMETER ( mn = 938.2723D0, mPi=139.5685D0)

         Q2=Q2MEV

         TE=0.D0
         TM=0.D0
         SE=0.D0
         SM=0.D0

         GO TO (1,2,3,4), NRES

c ****************************   D13 ****************************************
 1      W0=1520.
	qevf=((W0*W0-mn*mn)**2/4./W0/mn+Q2*(W0+mn)**2/4./W0/mn)/1.D6
	Fq = 1./(1.+qevf/0.71)**2

	  e11 = 2.56*Fq
c          if (qevf.gt.1.) e11=(1.97+0.59*qevf)*Fq
	  m11 = (1.80-3.93*qevf)*Fq
	  m12 = (-0.42+5.82*qevf)*Fq
	  IF (qevf.GT.1.) m12 = (5.84-0.44*qevf)*Fq
c          write (1,123) qevf, e11/fq, m11/fq, m12/fq
c 123      format(1x,4(f8.2,1x))

	  IF (IR.EQ.1.OR.IR.EQ.3) THEN
	    G11 = SQRT(2./3.)*e11+SQRT(1./3.)*m11
	    G12 = m12
	  ELSE IF (IR.EQ.2.OR.IR.EQ.4) THEN
	    G11 = -SQRT(2./3.)*e11-SQRT(1./27.)*m11
	    G12 = -m12/3.
	  END IF

	  A1 = -1.D3*SQRT(Pi/137.*1.D9/mn/(W0*W0-mn*mn))
     *	      *(SQRT(3./4.)*G12-SQRT(1./4.)*G11)
	  A2 = 1.D3*SQRT(Pi/137.*1.D9/mn/(W0*W0-mn*mn))
     *	      *(SQRT(1./4.)*G12+SQRT(3./4.)*G11)

         TE=-0.5*(A1+A2*SQRT(3.))
         TM=0.5*(A1-A2/SQRT(3.))

          RETURN
c ******************************   S11  *************************************
  2      W0=1520.
	qevf=((W0*W0-mn*mn)**2/4./W0/mn+Q2*(W0+mn)**2/4./W0/mn)/1.D6
	Fq = 1./(1.+qevf/0.71)**2

	  e11 = 2.56*Fq
c          if (qevf.gt.1.) e11=(1.97+0.59*qevf)*Fq
	  m11 = (1.80-3.93*qevf)*Fq

	  IF (IR.EQ.1.OR.IR.EQ.3) THEN
	    G11 = SQRT(1./3.)*e11-SQRT(2./3.)*m11
	  ELSE IF (IR.EQ.2.OR.IR.EQ.4) THEN
	    G11 = -SQRT(1./3.)*e11+SQRT(2./27.)*m11
	  END IF

	  A1 = 1.D3*SQRT(Pi/137.*1.D9/mn/(W0*W0-mn*mn))*G11

          TE=-A1

          RETURN
c ******************************** F15 ************************************
 3     W0=1680.
       qevf=((W0*W0-mn*mn)**2/4./W0/mn+Q2*(W0+mn)**2/4./W0/mn)/1.D6
       qevf0=(W0*W0-mn*mn)**2/4./W0/mn/1.d6
       Fq = 1./(1.+qevf/0.71)**2
       Fq0 = 1./(1.+qevf0/0.71)**2

	  e22 = 4.35*DSQRT(qevf)*Fq
	  m22 = (3.823-4.172*qevf)*DSQRT(qevf)*Fq
	  IF (qevf.LE.1.) m23 =(0.42+5.1*qevf)*Fq*DSQRT(qevf)
	  IF (qevf.GT.1.) m23 =(6.31-0.79*qevf)*Fq*DSQRT(qevf)

	  IF (IR.EQ.1.OR.IR.EQ.3) THEN
	    G12 = -SQRT(3./5.)*e22-SQRT(2./5.)*m22
	    G13 = -m23
	  ELSE IF (IR.EQ.2.OR.IR.EQ.4) THEN
	    G12 = SQRT(8./45.)*m22
	    G13 = 2.*m23/3.
	  END IF

	  A1 = 1.D3*SQRT(Pi/137.*1.D9/mn/(W0*W0-mn*mn))
     *	      *(SQRT(2./3.)*G13-SQRT(1./3.)*G12)
	  A2 = -1.D3*SQRT(Pi/137.*1.D9/mn/(W0*W0-mn*mn))
     *	      *(SQRT(1./3.)*G13+SQRT(2./3.)*G12)

         TE=-(A1+A2*SQRT(2.))/3.
         TM=(A1-A2/SQRT(2.))/3.
          RETURN
c ****************************   D33 *******************************
c 4       W0=1740.
 4       W0=1520.
	qevf=((W0*W0-mn*mn)**2/4./W0/mn+Q2*(W0+mn)**2/4./W0/mn)/1.D6
	Fq = 1./(1.+qevf/0.71)**2

	  e11 = 2.56*Fq
c          if (qevf.gt.1.) e11=(1.97+0.59*qevf)*Fq
	  m11 = (1.80-3.93*qevf)*Fq
	  m12 = (-0.42+5.82*qevf)*Fq
	  IF (qevf.GT.1.) m12 = (5.84-0.44*qevf)*Fq

	  IF (IR.EQ.1.OR.IR.EQ.3) THEN
	    G11 = SQRT(2./3.)*e11-SQRT(1./27.)*m11
	    G12 = -m12/3
	  ELSE IF (IR.EQ.2.OR.IR.EQ.4) THEN
	    G11 = SQRT(2./3.)*e11-SQRT(1./27.)*m11
	    G12 = -m12/3.
	  END IF

	  A1 = -1.D3*SQRT(Pi/137.*1.D9/mn/(W0*W0-mn*mn))
     *	      *(SQRT(3./4.)*G12-SQRT(1./4.)*G11)
	  A2 = 1.D3*SQRT(Pi/137.*1.D9/mn/(W0*W0-mn*mn))
     *	      *(SQRT(1./4.)*G12+SQRT(3./4.)*G11)

         TE=-0.5*(A1+A2*SQRT(3.))
         TM=0.5*(A1-A2/SQRT(3.))

        RETURN
        END



      SUBROUTINE GAUSSP(N,E,W)
      IMPLICIT REAL*8 (A-H,O-Z)
C     GAUSSP BERECHNET ABSZISSEN UND GEWICHTE FUER GAUSSINTEGRATION
C     IM INTERVALL "-1,1@ IN AUFSTEIGENDER REIHENFOLGE
C     REFERENZ:   ABRAMOWITZ AND STEGUN
C                 HANDBOOK OF MATHEMATICAL FUNCTIONS
C                 IM FOLGENDEN ABGEKUERZT A+S
C     DEFINITION VON ABSZISSEN UND GEWICHTEN: A+S(25.4.29)
C     METHODE:
C             BESTIMMUNG DER NULLSTELLEN VON PN DURCH NEWTONSCHE
C             ITERATION
C     GENAUIGKEIT:     1.E-15
C     N:     ANZAHL DER PUNKTE
C     E      RESULTAT:   ABSZISSEN
C     W      RESULTAT:   GEWICHTE
      DIMENSION E(1),W(1)
      DATA PI/3.141592653589793238462643D0/,EPS/1.D-16/
      M=(N+1)/2
      DN=N
      DO 1000 I=1,M
      DI=I
C     STARTWERTE A+S(22.16.6)
      X=PI*(4.D0*(DN-DI)+3.D0)/(4.D0*DN+2.D0)
      XN=(1.D0-(DN-1.D0)/(8.D0*DN*DN*DN))*DCOS(X)
      IF(I.GT.N/2) XN=0
      DO 100  ITER=1,10
      X=XN
C     BERECHNUNG VON PN(X) DURCH ITERATION A+S(8.5.3)
      Y1=1.D0
      Y=X
      IF(N.LT.2) GOTO 250
      DO 200 J=2,N
      DJ=J
      Y2=Y1
      Y1=Y
200   Y=((2.D0*DJ-1.D0)*X*Y1-(DJ-1.D0)*Y2)/DJ
250   CONTINUE
C     BERECHNUNG DER ABLEITUNG A+S(8.5.4)
      YS=DN*(X*Y-Y1)/(X*X-1.D0)
C     NEWTONITERATION
      H=-Y/YS
      XN=X+H
      IF (DABS(H) .LT. EPS) GOTO 110
  100 CONTINUE
  110 E(I)=X
      E(N-I+1)=-X
      GEW=2.D0/((1.D0-X*X)*YS*YS)
      W(I)=GEW
      W(N-I+1)=GEW
1000  CONTINUE
      RETURN
      END


      SUBROUTINE MULTIPOL(ISO,WCM,Q2,CK0,CK,LMAX,IUNI,MULTI)
c *****************************************************************************
c     calculation of multipoles from unitarized background amplitudes
c     by numerical integration
c *****************************************************************************
      IMPLICIT REAL*8 (A-H,O-Z)
      COMPLEX*16 F1,F2,F3,F4,F5,F6,MULTI,D
      COMPLEX*16 F_back(6)
      REAL*8 KAPPAP,KAPPAN,MPI,MPI0,MPIP,MP,MN,M,M1,M2
      DIMENSION MULTI(10,8),D(6)
      COMMON/GAUSS/EX(100),WX(100),NX
      COMMON/bl/ PI,PIH,HQC,F2PI,E,KAPPAP,KAPPAN,CSUC,CT,COME,CRHO,
     & CBORN,CVEC,ALAMBDA,MPI,MPI0,MPIP,MP,MN,AVM,M,M1,M2,IPV,IFORM,
     & CP33,CP11,CD13,CD33,CS11F,CS11S,CF15

        DO  L=0,LMAX
         DO  K=1,8
          MULTI(L+1,K)=0
         END DO
        END DO
c  integration loop
        DO 2 J=1,NX
        X=EX(J)
        CALL CGLN(ISO,WCM,Q2,CK0,X,F1,F2,F3,F4,F5,F6)
	DO 101 K=1,6
 101    F_back(K)=(0.D0,0.D0)
	IF (IUNI.NE.0)
     &   CALL CGLN_BACK(ISO, WCM, Q2, X, AVM, MPI, F_back)
        F1 = F1 + F_back(1)
        F2 = F2 + F_back(2)
        F3 = F3 + F_back(3)
        F4 = F4 + F_back(4)
        F5 = F5 + F_back(5)
        F6 = F6 + F_back(6)
C ******  TEST *************
c      F1 = F1 - F_back(1)
c      F2 = F2 - F_back(2)
c      F3 = F3 - F_back(3)
c      F4 = F4 - F_back(4)
c      F5 = F5 - F_back(5)
c      F6 = F6 - F_back(6)
C ***************************
C  loop ueber Drehimpulse
        PLM2=0
        PLM=0
        PL=1
        PLP=X
        DO 3 L=0,LMAX
        L1=L+1
        LD=2*L+1
        PLP2=((2*L+3)*X*PLP-L1*PL)/(L+2.D0)
        D(1)=PL*F1/(2.D0*L1)-PLP*F2/(2.D0*L1)+(PLM-PLP)*F3*L/FLOAT(LD)
     &       /(2.D0*L1)+(PL-PLP2)*F4/(4*L+6.D0)
        D(5)=(PL*F5+PLP*F6)/(2.D0*L1)
        IF (L.LT.2) THEN
           D(2)=0
        ELSE
        D(2)=PL*F1/(2.D0*L)-PLM*F2/(2.D0*L)+(PLP-PLM)*F3*L1/FLOAT(LD)
     &       /(2.D0*L)+(PL-PLM2)*F4/(4*L-2.D0)
        ENDIF
        IF (L.EQ.0) THEN
           D(3)=0
           D(4)=0
           D(6)=0
        ELSE
        D(3)=PL*F1/(2.D0*L1)-PLP*F2/(2.D0*L1)+(PLP-PLM)*F3/FLOAT(LD)/
     &       (2.D0*L1)
        D(4)=-PL*F1/(2.D0*L)+PLM*F2/(2.D0*L)+(PLM-PLP)*F3/FLOAT(LD)/
     &       (2.D0*L)
        D(6)=(PL*F5+PLM*F6)/(2.D0*L)
        ENDIF
        DO  K=1,6
         MULTI(L+1,K)=MULTI(L+1,K)+D(K)*WX(J)
        END DO
        PLM2=PLM
        PLM=PL
        PL=PLP
        PLP=PLP2
  3     CONTINUE
  2     CONTINUE
        DO L=0,LMAX
        DO K=7,8
         MULTI(L+1,K)=MULTI(L+1,K-2)*CK/CK0
        ENDDO
        ENDDO
        RETURN
        END


      SUBROUTINE CGLN_BACK(ISO, W, Q2, x, MN, MPI, F_back)
c *****************************************************************************
c     CGLN amplitudes F_background(i) from multipoles
c     F_unitarized = F_Born+Vector  +  F_background
c *****************************************************************************
      IMPLICIT REAL*8(A-H,O-Z)
      COMMON /MULTB/ MULT(6,10,3)
      REAL*8 MN, MPI
      DIMENSION P_S(10), P_SS(10)
      COMPLEX*16 MULT, F_iso(6, 3), F_back(6)
      PARAMETER (pi = 3.1415926536D0)

      l_max = 4
c      l_max=2

      HQC = 197.3285


      P_S(1) = 0.0
      P_S(2) = 1.0
      P_S(3) = 3.0 * x
      P_S(4) = (15.0 * x * x - 3.0) / 2.0
      P_S(5) = (35.0 * x * x * x - 15.0 * x) / 2.0
      P_SS(1) = 0.0
      P_SS(2) = 0.0
      P_SS(3) = 3.0
      P_SS(4) = 15.0 * x
      P_SS(5) = 105.0 / 2.0 * x * x - 7.5

      DO 6000 k = 1, 6
      f_back(k)=(0.d0,0.d0)
         DO 7000 j = 1, 3
            F_iso(k, j) = (0.0, 0.0)
 7000    CONTINUE
 6000 CONTINUE

      DO 4000 l = 1, l_max
          DO 5000 j = 1, 3
             F_iso(1, j) = F_iso(1, j)
     *                + ((REAL(l) - 1.0) * MULT(3, l, j)
     *                     + MULT(1, l, j)) * P_S(l + 1)
             F_iso(5, j) = F_iso(5, j)
     *                + REAL(l) * MULT(5, l, j) * P_S(l + 1)
             IF (l.GT.1) THEN
                F_iso(2, j) = F_iso(2, j)
     *                   + (REAL(l) * MULT(3, l, j)
     *                   + (REAL(l) - 1.0) * MULT(4, l, j)) * P_S(l)
                F_iso(3, j) = F_iso(3, j)
     *                   + (MULT(1, l, j)
     *                       - MULT(3, l, j)) * P_SS(l + 1)
     *                   + (MULT(2, l, j)
     *                       + MULT(4, l, j)) * P_SS(l - 1)
                F_iso(6, j) = F_iso(6, j)
     *                   + ((REAL(l) - 1.0) * MULT(6, l, j)
     *                       - REAL(l) * MULT(5, l, j)) * P_S(l)
             END IF
             IF (l.GT.2) THEN
                F_iso(1, j) = F_iso(1, j)
     *                   + (REAL(l) * MULT(4, l, j)
     *                   + MULT(2, l, j)) * P_S(l - 1)
                F_iso(4, j) = F_iso(4, j)
     *                   + (MULT(3, l, j) - MULT(1, l, j)
     *                   - MULT(4, l, j) - MULT(2, l, j)) * P_SS(l)
                F_iso(5, j) = F_iso(5, j)
     *                 - (REAL(l) - 1.0) * MULT(6, l, j) * P_S(l - 1)
             END IF
 5000     CONTINUE
 4000 CONTINUE

      IF (ISO.EQ.1) THEN
         DO 4500 k = 1, 6
            F_back(k) = F_iso(k, 2) + F_iso(k, 1) / 3.0
     *                 + 2.0 * F_iso(k, 3) / 3.0
 4500       CONTINUE
      ELSE IF (ISO.EQ.2) THEN
         DO 4600 k = 1, 6
            F_back(k) = - F_iso(k, 2) + F_iso(k, 1) / 3.0
     *                 + 2.0 * F_iso(k, 3) / 3.0
 4600       CONTINUE
      ELSE IF (ISO.EQ.3) THEN
         DO 4700 k = 1, 6
            F_back(k) = SQRT(2.0) * (F_iso(k, 2) + F_iso(k, 1) / 3.0
     *                 - F_iso(k, 3) / 3.0)
 4700       CONTINUE
      ELSE IF (ISO.EQ.4) THEN
         DO 4800 k = 1, 6
            F_back(k) = SQRT(2.0) * (F_iso(k, 2) - F_iso(k, 1) / 3.0
     *                 + F_iso(k, 3) / 3.0)
 4800       CONTINUE
      END IF
      DO 4900 k = 1, 6
         F_back(k) = F_back(k) * HQC
 4900 CONTINUE

      END

      SUBROUTINE LET_CORR(W,Q2,MM,MMPI,Epl,Spl,E0_corr,S0_corr,xpi)
      IMPLICIT REAL*8(A-H,O-Z)
      REAL*8 MM, MMPI, kcm
      COMMON /KinVar/ OmegL, Q2MEV, WWW, wGacm, kcm
      COMMON /E0CORR/ XE, XS, XMIX
c ***********************************
      HQC = 197.3285
      ampi=139.4999/hqc
      amn=939.5/hqc
      apiN=0.124/ampi
      qpi_fm = 0.d0
      W_MEV=W*HQC
      W0=1090./hqc
      qpi0 = SQRT(((W0*W0+ampi**2-amn**2)/(2*W0))**2-ampi**2)
      IF (W_MeV.GE.1079.)
     & qpi_fm = SQRT(((W*W+ampi**2-amn**2)/(2*W))**2-ampi**2)
      xpi=qpi_fm/qpi0
c ******  from fit of Fucs data  ********
c      Ae=0.135E-4
c      Be=0.68
c      Ce=-0.14668
c *******  from fit of new Schmidt data *****
      Ae=0.14415E-4
      Be=0.70868
      Ce=0.
c *******************************************
      qpi = SQRT(((W*W+MMPI**2-MM**2)/(2*W))**2-MMPI**2)
        	egcm=(W**2-MM**2-Q2)/2./W
                qcm=sqrt(egcm**2+Q2)
	        Q2GEV=Q2*(197.337/1000.)**2
	        FDIP = 1./(1.+Q2GEV/0.71)**2
            CALL FNFPI(Q2,MM,F1S,F1V,F2S,F2V,FPI)
      E0_corr = Ae*(1.+Ce*qpi**2)/((1.+Be**2*qpi**2)**2)*F2V/3.7058
      S0_corr = E0_corr * egcm/qcm
      E0_corr= E0_corr*XE
      S0_corr= S0_corr*XS
c **********  cusp effects ***********
       qpi_eff=0.
      IF (W_MeV.LT.1079.)
     & qpi_eff = SQRT(-((W*W+ampi**2-amn**2)/(2*W))**2+ampi**2)
      IF (W_MeV.LT.1079.) E0_corr = E0_corr - Epl*apiN*qpi_eff
      IF (W_MeV.LT.1079.) S0_corr = S0_corr - Spl*apiN*qpi_eff
c *******************************************************************

      RETURN
      END

      SUBROUTINE UNI(W, Q2, MM, MMPI, l_max)
c *****************************************************************************
c     calculates unitarization phases and unitarized background multipoles
c     l=0, 1, 2, 3
c     1+iT = (PiN + 1)/2   or  (piN-1)/2 when non-unitarized bg is subtracted
c     S11, P11 prepared for alternative unitarization (commented)
c     no background unitarization for D13, D33 and F15
c *****************************************************************************
      IMPLICIT REAL*8(A-H,O-Z)
      COMMON /MULTB/MULT(6,10,3)
      REAL*8 KAPPAP,KAPPAN,MPI,MPI0,MPIP,MP,MN,M,M1,M2,M1P,M1M
      REAL*8 m_N, m_pi, MM, MMPI, m_pip
      REAL*8 MV_p, MV_m, LV_p, LV_m
      COMPLEX*16 piN, tmp, phase
      COMPLEX*16 MULT
      COMMON/COUPL/ IPVPS,IFSI
      DIMENSION FB(6,10,3), FV(6, 10, 2)
      DIMENSION W_sp(93)
      DIMENSION del_s11(93),del_s11_ss(93),del_s31(93),del_s31_ss(93)
      DIMENSION del_p11(93),del_p11_ss(93),del_p31(93),del_p31_ss(93)
      DIMENSION del_p13(93),del_p13_ss(93),del_p33(93),del_p33_ss(93)
      DIMENSION del_d13(93),del_d13_ss(93),del_d33(93),del_d33_ss(93)
      DIMENSION del_d15(93),del_d15_ss(93),del_d35(93),del_d35_ss(93)
      DIMENSION del_f15(93),del_f15_ss(93),del_f35(93),del_f35_ss(93)
      DIMENSION del_f17(93),del_f17_ss(93),del_f37(93),del_f37_ss(93)
      DIMENSION eta_s11(93),eta_s11_ss(93),eta_s31(93),eta_s31_ss(93)
      DIMENSION eta_p11(93),eta_p11_ss(93),eta_p31(93),eta_p31_ss(93)
      DIMENSION eta_p13(93),eta_p13_ss(93),eta_p33(93),eta_p33_ss(93)
      DIMENSION eta_d13(93),eta_d13_ss(93),eta_d33(93),eta_d33_ss(93)
      DIMENSION eta_d15(93),eta_d15_ss(93),eta_d35(93),eta_d35_ss(93)
      DIMENSION eta_f15(93),eta_f15_ss(93),eta_f35(93),eta_f35_ss(93)
      DIMENSION eta_f17(93),eta_f17_ss(93),eta_f37(93),eta_f37_ss(93)
      COMMON /SPL/ W_sp,del_s11,del_s11_ss,del_s31,del_s31_ss,
     *      del_p11,del_p11_ss,del_p31,del_p31_ss,
     *      del_p13,del_p13_ss,del_p33,del_p33_ss,
     *      del_d13,del_d13_ss,del_d33,del_d33_ss,
     *      del_d15,del_d15_ss,del_d35,del_d35_ss,
     *      del_f15,del_f15_ss,del_f35,del_f35_ss,
     *      del_f17,del_f17_ss,del_f37,del_f37_ss,
     *      eta_s11,eta_s11_ss,eta_s31,eta_s31_ss,
     *      eta_p11,eta_p11_ss,eta_p31,eta_p31_ss,
     *      eta_p13,eta_p13_ss,eta_p33,eta_p33_ss,
     *      eta_d13,eta_d13_ss,eta_d33,eta_d33_ss,
     *      eta_d15,eta_d15_ss,eta_d35,eta_d35_ss,
     *      eta_f15,eta_f15_ss,eta_f35,eta_f35_ss,
     *      eta_f17,eta_f17_ss,eta_f37,eta_f37_ss
      COMMON /PHASES/ phi_s11, phi_s31, phi_p11, phi_p31,
     *                phi_p13, phi_p33, phi_d13, phi_d33,
     *                phi_d15, phi_d35, phi_f15, phi_f35,
     *                phi_f17, phi_f37
      COMMON/bl/ PI,PIH,HQC,F2PI,E,KAPPAP,KAPPAN,CSUC,CT,COME,CRHO,
     & CBORN,CVEC,ALAMBDA,MPI,MPI0,MPIP,MP,MN,AVM,M,M1,M2,IPV,IFORM,
     & CP33,CP11,CD13,CD33,CS11F,CS11S,CF15

      HQC = 197.3285
      W_MeV = HQC * W
      Q2_MeV = HQC * HQC * Q2
      m_N = HQC * MM
      m_pi = HQC * MMPI
      m_pip=139.5685
      CNORM=m_pi/m_pip

      DO 99 I=1,6
      DO 99 J=1,4
      DO 99 K=1,3
      FB(I,J,K)=(0.D0,0.D0)
99    MULT(I,J,K)=(0.D0,0.D0)

      IF (CBORN.EQ.1) THEN
c ****************************  Born multipoles  ******************************
         CALL BORN_MUL(IPVPS,W,Q2,MM,MMPI,l_max,FB)
         DO 1234 i = 1, 6
            DO 1235 j = 1, 4
               DO 1236 k = 1, 3
                  FB(i, j, k) = FB(i, j, k) / 1000.0 /m_pip
 1236          CONTINUE
 1235       CONTINUE
 1234    CONTINUE
      ELSE
         DO 1237 i = 1, 6
            DO 1238 j = 1, 4
               DO 1239 k = 1, 3
                  FB(i, j, k) = 0.0
 1239          CONTINUE
 1238       CONTINUE
 1237    CONTINUE
      END IF
c ************************  vector mesons: omega and rho  *********************
      IF (COME.EQ.1) THEN
         DO 2100 l = 1, l_max + 1
            CALL EL(W_MeV, Q2_MeV, m_N, m_pi, l - 1, 1, 1, EV_p)
            CALL EL(W_MeV, Q2_MeV, m_N, m_pi, l - 1, 2, 1, EV_m)
            CALL ML(W_MeV, Q2_MeV, m_N, m_pi, l - 1, 1, 1, MV_p)
            CALL ML(W_MeV, Q2_MeV, m_N, m_pi, l - 1, 2, 1, MV_m)
            CALL LL(W_MeV, Q2_MeV, m_N, m_pi, l - 1, 1, 1, LV_p)
            CALL LL(W_MeV, Q2_MeV, m_N, m_pi, l - 1, 2, 1, LV_m)
            FV(1, l, 1) = EV_p*CNORM
            FV(2, l, 1) = EV_m*CNORM
            FV(3, l, 1) = MV_p*CNORM
            FV(4, l, 1) = MV_m*CNORM
            FV(5, l, 1) = LV_p*CNORM
            FV(6, l, 1) = LV_m*CNORM
 2100    CONTINUE
      ELSE
         DO 2300 l = 1, l_max + 1
            FV(1, l, 1) = 0.0
            FV(2, l, 1) = 0.0
            FV(3, l, 1) = 0.0
            FV(4, l, 1) = 0.0
            FV(5, l, 1) = 0.0
            FV(6, l, 1) = 0.0
 2300    CONTINUE
      END IF

      IF (CRHO.EQ.1) THEN
         DO 2200 l = 1, l_max + 1
            CALL EL(W_MeV, Q2_MeV, m_N, m_pi, l - 1, 1, 2, EV_p)
            CALL EL(W_MeV, Q2_MeV, m_N, m_pi, l - 1, 2, 2, EV_m)
            CALL ML(W_MeV, Q2_MeV, m_N, m_pi, l - 1, 1, 2, MV_p)
            CALL ML(W_MeV, Q2_MeV, m_N, m_pi, l - 1, 2, 2, MV_m)
            CALL LL(W_MeV, Q2_MeV, m_N, m_pi, l - 1, 1, 2, LV_p)
            CALL LL(W_MeV, Q2_MeV, m_N, m_pi, l - 1, 2, 2, LV_m)
            FV(1, l, 2) = EV_p*CNORM
            FV(2, l, 2) = EV_m*CNORM
            FV(3, l, 2) = MV_p*CNORM
            FV(4, l, 2) = MV_m*CNORM
            FV(5, l, 2) = LV_p*CNORM
            FV(6, l, 2) = LV_m*CNORM
 2200    CONTINUE
      ELSE
         DO 2400 l = 1, l_max + 1
            FV(1, l, 2) = 0.0
            FV(2, l, 2) = 0.0
            FV(3, l, 2) = 0.0
            FV(4, l, 2) = 0.0
            FV(5, l, 2) = 0.0
            FV(6, l, 2) = 0.0
 2400    CONTINUE
      END IF

C -----------------------------------  S11  -----------------------------------
c ********  LET correction, 22.10.2000 ***********
      EplB = sqrt(2.)*(FB(1,1,2)+(FB(1,1,1)-FB(1,1,3))/3)
      EplV = sqrt(2.)*FV(1,1,2)
      Epl = EplB + EplV
      SplB = sqrt(2.)*(FB(5,1,2)+(FB(5,1,1)-FB(5,1,3))/3)
      SplV = sqrt(2.)*FV(5,1,2)
      Spl = SplB + SplV

      CALL LET_CORR(W,Q2,MM,MMPI,Epl,Spl,E0_corr,S0_corr,xpi)
C ************************************************
      CALL SPLINT(W_sp, del_s11, del_s11_ss, 93, W_MeV, del)
      CALL SPLINT(W_sp, eta_s11, eta_s11_ss, 93, W_MeV, eta)
      IF (W_MeV.LT.1090.) del=3.83*PI*xpi/180.
      IF (W_MeV.LT.1079.) del=0.00001
      IF (W_MeV.LT.1079.) eta=1.
      piN = eta * (COS(2.0 * del) + (0.0, 1.0) * SIN(2.0 * del))
      tmp = (piN + 1.0) / 2.0
      ARG= DIMAG(tmp) /DREAL(tmp)
      phi_tmp = DATAN(ARG)
      phi_s11 = DATAN((1.0 - eta * COS(2.0 * del))
     *        / eta / SIN(2.0 * del))
c      phi_s11=phi_tmp
      IF (phi_s11.LT.0.0) phi_s11 = phi_s11 + pi
      del_phi = phi_s11 - phi_tmp
      phase = COS(del_phi) + (0.0, 1.0) * SIN(del_phi)
      MULT(1, 1, 1) = (FB(1, 1, 1) + FV(1, 1, 1))
c     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
     *              *  (piN - 1.0) / 2.0  + E0_corr*tmp
      MULT(1, 1, 2) = (FB(1, 1, 2) + FV(1, 1, 2))
c     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
     *              *  (piN - 1.0) / 2.0
      MULT(5, 1, 1) = (FB(5, 1, 1) + FV(5, 1, 1))
c     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
     *              *  (piN - 1.0) / 2.0   + S0_corr*tmp
      MULT(5, 1, 2) = (FB(5, 1, 2) + FV(5, 1, 2))
c     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
     *              *  (piN - 1.0) / 2.0
C
C -----------------------------------  S31  -----------------------------------
      CALL SPLINT(W_sp, del_s31, del_s31_ss, 93, W_MeV, del)
      CALL SPLINT(W_sp, eta_s31, eta_s31_ss, 93, W_MeV, eta)
      IF (W_MeV.LT.1090.) del=-2.08*PI*xpi/180.
      IF (W_MeV.LT.1079.) del=-0.00001
      IF (W_MeV.LT.1079.) eta=1.
      piN = eta * (COS(2.0 * del) + (0.0, 1.0) * SIN(2.0 * del))
      tmp = (piN + 1.0) / 2.0
      ARG= DIMAG(tmp) /DREAL(tmp)
      phi_tmp = DATAN(ARG)
      phi_s31 = DATAN((1.0 - eta * COS(2.0 * del))
     *        / eta / SIN(2.0 * del))
c      phi_s31=phi_tmp
      IF (phi_s31.LT.0.0) phi_s31 = phi_s31 + pi
      del_phi = phi_s31 - phi_tmp
      phase = COS(del_phi) + (0.0, 1.0) * SIN(del_phi)
      MULT(1, 1, 3) = (FB(1, 1, 3) + FV(1, 1, 1))
c     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
     *              *   (piN - 1.0) / 2.0  + E0_corr*tmp
c      write (6,367) w_mev, mult(1,1,3),e0_corr, DEL,ETA
c 367  format(1x,f8.2,2x,3(2(e10.4,1x),1x))
      MULT(5, 1, 3) = (FB(5, 1, 3) + FV(5, 1, 1))
c     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
     *              *   (piN - 1.0) / 2.0  + S0_corr*tmp

C
C -----------------------------------  P11  -----------------------------------
      CALL SPLINT(W_sp, del_p11, del_p11_ss, 93, W_MeV, del)
      CALL SPLINT(W_sp, eta_p11, eta_p11_ss, 93, W_MeV, eta)
      IF (W_MeV.LT.1079.) del=0.00001
      IF (W_MeV.LT.1079.) eta=1.
      piN = eta * (COS(2.0 * del) + (0.0, 1.0) * SIN(2.0 * del))
      tmp = (piN + 1.0) / 2.0
      ARG= DIMAG(tmp) /DREAL(tmp)
      phi_tmp = DATAN(ARG)
      phi_p11 = DATAN((1.0 - eta * COS(2.0 * del))
     *        / eta / SIN(2.0 * del))
      IF (W_MeV.GT.1400.AND.phi_p11.LT.0.0) phi_p11 = phi_p11 + pi
c      IF (phi_p11.LT.0.0) phi_p11 = phi_p11 + pi
      del_phi = phi_p11 - phi_tmp
      phase = COS(del_phi) + (0.0, 1.0) * SIN(del_phi)
      MULT(4, 2, 1) = (FB(4, 2, 1) + FV(4, 2, 1))
C     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
     *              *  (piN - 1.0) / 2.0
      MULT(4, 2, 2) = (FB(4, 2, 2) + FV(4, 2, 2))
C     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
     *              *  (piN - 1.0) / 2.0
      MULT(6, 2, 1) = (FB(6, 2, 1) + FV(6, 2, 1))
C     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
     *              *  (piN - 1.0) / 2.0
      MULT(6, 2, 2) = (FB(6, 2, 2) + FV(6, 2, 2))
C     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
     *              *  (piN - 1.0) / 2.0
C
C -----------------------------------  P31  -----------------------------------
      CALL SPLINT(W_sp, del_p31, del_p31_ss, 93, W_MeV, del)
      CALL SPLINT(W_sp, eta_p31, eta_p31_ss, 93, W_MeV, eta)
      IF (W_MeV.LT.1079.) del=0.00001
      IF (W_MeV.LT.1079.) eta=1.
      piN = eta * (COS(2.0 * del) + (0.0, 1.0) * SIN(2.0 * del))
      MULT(4, 2, 3) = (FB(4, 2, 3) + FV(4, 2, 1))
     *              *   (piN - 1.0) / 2.0
      MULT(6, 2, 3) = (FB(6, 2, 3) + FV(6, 2, 1))
     *              *   (piN - 1.0) / 2.0
C
C -----------------------------------  P13  -----------------------------------
      CALL SPLINT(W_sp, del_p13, del_p13_ss, 93, W_MeV, del)
      CALL SPLINT(W_sp, eta_p13, eta_p13_ss, 93, W_MeV, eta)
      IF (W_MeV.LT.1079.) del=0.00001
      IF (W_MeV.LT.1079.) eta=1.
      piN = eta * (COS(2.0 * del) + (0.0, 1.0) * SIN(2.0 * del))
      MULT(1, 2, 1) = (FB(1, 2, 1) + FV(1, 2, 1))
     *              *  (piN - 1.0) / 2.0
      MULT(1, 2, 2) = (FB(1, 2, 2) + FV(1, 2, 2))
     *              *  (piN - 1.0) / 2.0
      MULT(3, 2, 1) = (FB(3, 2, 1) + FV(3, 2, 1))
     *              *  (piN - 1.0) / 2.0
      MULT(3, 2, 2) = (FB(3, 2, 2) + FV(3, 2, 2))
     *              *  (piN - 1.0) / 2.0
      MULT(5, 2, 1) = (FB(5, 2, 1) + FV(5, 2, 1))
     *              *  (piN - 1.0) / 2.0
      MULT(5, 2, 2) = (FB(5, 2, 2) + FV(5, 2, 2))
     *              *  (piN - 1.0) / 2.0
C
C -----------------------------------  P33  -----------------------------------
      CALL SPLINT(W_sp, del_p33, del_p33_ss, 93, W_MeV, del)
      CALL SPLINT(W_sp, eta_p33, eta_p33_ss, 93, W_MeV, eta)
      IF (W_MeV.LT.1079.) del=0.
      IF (W_MeV.LT.1079.) eta=1.
      piN = eta * (COS(2.0 * del) + (0.0, 1.0) * SIN(2.0 * del))
      tmp = (piN + 1.0) / 2.0
      ARG= DIMAG(tmp) /DREAL(tmp)
      phi_p33 = DATAN(ARG)
      IF (phi_p33.LT.0.0) phi_p33 = phi_p33 + pi
      IF (W_MeV.GT.1400.AND.phi_p33.LT.PI/2.0) phi_p33 = phi_p33 + pi
      IF (W_MeV.LT.1300.) phi_p33=del
      MULT(1, 2, 3) = (FB(1, 2, 3) + FV(1, 2, 1))
     *              *   (piN - 1.0) / 2.0
      MULT(3, 2, 3) = (FB(3, 2, 3) + FV(3, 2, 1))
     *              *   (piN - 1.0) / 2.0
      MULT(5, 2, 3) = (FB(5, 2, 3) + FV(5, 2, 1))
     *              *   (piN - 1.0) / 2.0
C
C -----------------------------------  D13  -----------------------------------
      CALL SPLINT(W_sp, del_d13, del_d13_ss, 93, W_MeV, del)
      CALL SPLINT(W_sp, eta_d13, eta_d13_ss, 93, W_MeV, eta)
      IF (W_MeV.LT.1079.) del=0.00001
      IF (W_MeV.LT.1079.) eta=1.
      piN = eta * (COS(2.0 * del) + (0.0, 1.0) * SIN(2.0 * del))
      IF (del.EQ.0.0) THEN
         phi_d13 = 0.0
         phase = (1.0, 0.0)
      ELSE
         tmp = (piN + 1.0) / 2.0
      ARG= DIMAG(tmp) /DREAL(tmp)
      phi_tmp = DATAN(ARG)
         phi_d13 =DATAN((1.0 - eta * COS(2.0 * del))
     *           / eta / SIN(2.0 * del))
c         IF (W_MeV.GT.1400.AND.phi_d13.LT.0.0) phi_d13 = phi_d13 + pi
         IF (phi_d13.LT.0.0) phi_d13 = phi_d13 + pi
         del_phi = phi_d13 - phi_tmp
         phase = COS(del_phi) + (0.0, 1.0) * SIN(del_phi)
      END IF
C      MULT(2, 3, 1) = (FB(2, 3, 1) + FV(2, 3, 1))
c     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
C     *              *  (piN - 1.0) / 2.0
C      MULT(2, 3, 2) = (FB(2, 3, 2) + FV(2, 3, 2))
c     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
C     *              *  (piN - 1.0) / 2.0
C      MULT(4, 3, 1) = (FB(4, 3, 1) + FV(4, 3, 1))
c     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
C     *              *  (piN - 1.0) / 2.0
C      MULT(4, 3, 2) = (FB(4, 3, 2) + FV(4, 3, 2))
c     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
C     *              *  (piN - 1.0) / 2.0
C      MULT(6, 3, 1) = (FB(6, 3, 1) + FV(6, 3, 1))
c     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
C     *              *  (piN - 1.0) / 2.0
C      MULT(6, 3, 2) = (FB(6, 3, 2) + FV(6, 3, 2))
c     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
C     *              *  (piN - 1.0) / 2.0
C
C -----------------------------------  D33  -----------------------------------
      CALL SPLINT(W_sp, del_d33, del_d33_ss, 93, W_MeV, del)
      CALL SPLINT(W_sp, eta_d33, eta_d33_ss, 93, W_MeV, eta)
      IF (W_MeV.LT.1079.) del=0.00001
      IF (W_MeV.LT.1079.) eta=1.
      piN = eta * (COS(2.0 * del) + (0.0, 1.0) * SIN(2.0 * del))
      IF (del.EQ.0.0) THEN
         phi_d33 = 0.0
         phase = (1.0, 0.0)
      ELSE
         tmp = (piN + 1.0) / 2.0
      ARG= DIMAG(tmp) /DREAL(tmp)
      phi_tmp = DATAN(ARG)
         phi_d33 = DATAN((1.0 - eta * COS(2.0 * del))
     *           / eta / SIN(2.0 * del))
c         IF (W_MeV.GT.1400.AND.phi_d33.LT.0.0) phi_d33 = phi_d33 + pi
         IF (phi_d33.LT.0.0) phi_d33 = phi_d33 + pi
         del_phi = phi_d33 - phi_tmp
         phase = COS(del_phi) + (0.0, 1.0) * SIN(del_phi)
      END IF
C      MULT(2, 3, 3) = (FB(2, 3, 3) + FV(2, 3, 1))
C     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
C     *              *   (piN - 1.0) / 2.0
C      MULT(4, 3, 3) = (FB(4, 3, 3) + FV(4, 3, 1))
C     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
C     *              *   (piN - 1.0) / 2.0
C      MULT(6, 3, 3) = (FB(6, 3, 3) + FV(6, 3, 1))
C     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
C     *              *   (piN - 1.0) / 2.0
C
C -----------------------------------  D15  -----------------------------------
      CALL SPLINT(W_sp, del_d15, del_d15_ss, 93, W_MeV, del)
      CALL SPLINT(W_sp, eta_d15, eta_d15_ss, 93, W_MeV, eta)
      IF (W_MeV.LT.1079.) del=0.00001
      IF (W_MeV.LT.1079.) eta=1.
      piN = eta * (COS(2.0 * del) + (0.0, 1.0) * SIN(2.0 * del))
      IF (del.EQ.0.0) THEN
         phi_d15 = 0.0
         phase = (1.0, 0.0)
      ELSE
         tmp = (piN + 1.0) / 2.0
      ARG= DIMAG(tmp) /DREAL(tmp)
      phi_tmp = DATAN(ARG)
         phi_d15 = DATAN((1.0 - eta * COS(2.0 * del))
     *           / eta / SIN(2.0 * del))
c         IF (W_MeV.GT.1400.AND.phi_d15.LT.0.0) phi_d15 = phi_d15 + pi
         IF (phi_d15.LT.0.0) phi_d15 = phi_d15 + pi
         del_phi = phi_d15 - phi_tmp
         phase = COS(del_phi) + (0.0, 1.0) * SIN(del_phi)
      END IF
      MULT(1, 3, 1) = (FB(1, 3, 1) + FV(1, 3, 1))
C     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
     *              *  (piN - 1.0) / 2.0
      MULT(1, 3, 2) = (FB(1, 3, 2) + FV(1, 3, 2))
C     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
     *              *  (piN - 1.0) / 2.0
      MULT(3, 3, 1) = (FB(3, 3, 1) + FV(3, 3, 1))
C     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
     *              *  (piN - 1.0) / 2.0
      MULT(3, 3, 2) = (FB(3, 3, 2) + FV(3, 3, 2))
C     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
     *              *  (piN - 1.0) / 2.0
      MULT(5, 3, 1) = (FB(5, 3, 1) + FV(5, 3, 1))
C     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
     *              *  (piN - 1.0) / 2.0
      MULT(5, 3, 2) = (FB(5, 3, 2) + FV(5, 3, 2))
C     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
     *              *  (piN - 1.0) / 2.0
C
C -----------------------------------  D35  -----------------------------------
      CALL SPLINT(W_sp, del_d35, del_d35_ss, 93, W_MeV, del)
      CALL SPLINT(W_sp, eta_d35, eta_d35_ss, 93, W_MeV, eta)
      IF (W_MeV.LT.1079.) del=0.00001
      IF (W_MeV.LT.1079.) eta=1.
      piN = eta * (COS(2.0 * del) + (0.0, 1.0) * SIN(2.0 * del))
      MULT(1, 3, 3) = (FB(1, 3, 3) + FV(1, 3, 1))
     *              *   (piN - 1.0) / 2.0
      MULT(3, 3, 3) = (FB(3, 3, 3) + FV(3, 3, 1))
     *              *   (piN - 1.0) / 2.0
      MULT(5, 3, 3) = (FB(5, 3, 3) + FV(5, 3, 1))
     *              *   (piN - 1.0) / 2.0
C
C -----------------------------------  F15  -----------------------------------
      CALL SPLINT(W_sp, del_f15, del_f15_ss, 93, W_MeV, del)
      CALL SPLINT(W_sp, eta_f15, eta_f15_ss, 93, W_MeV, eta)
      IF (W_MeV.LT.1079.) del=0.00001
      IF (W_MeV.LT.1079.) eta=1.
      piN = eta * (COS(2.0 * del) + (0.0, 1.0) * SIN(2.0 * del))
      tmp = (piN + 1.0) / 2.0
      IF (del.EQ.0.0) THEN
         phi_f15 = 0.0
         phase = (1.0, 0.0)
      ElSE
      ARG= DIMAG(tmp) /DREAL(tmp)
      phi_tmp = DATAN(ARG)
         phi_f15 = DATAN((1.0 - eta * COS(2.0 * del))
     *           / eta / SIN(2.0 * del))
c         IF (W_MeV.GT.1400.AND.phi_15.LT.0.0) phi_f15 = phi_f15 + pi
         IF (phi_f15.LT.0.0) phi_f15 = phi_f15 + pi
         del_phi = phi_f15 - phi_tmp
         phase = COS(del_phi) + (0.0, 1.0) * SIN(del_phi)
      END IF
C      MULT(2, 4, 1) = (FB(2, 4, 1) + FV(2, 4, 1))
C     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
C     *              *  (piN - 1.0) / 2.0
C      MULT(2, 4, 2) = (FB(2, 4, 2) + FV(2, 4, 2))
C     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
C     *              *  (piN - 1.0) / 2.0
C      MULT(4, 4, 1) = (FB(4, 4, 1) + FV(4, 4, 1))
C     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
C     *              *  (piN - 1.0) / 2.0
C      MULT(4, 4, 2) = (FB(4, 4, 2) + FV(4, 4, 2))
C     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
C     *              *  (piN - 1.0) / 2.0
C      MULT(6, 4, 1) = (FB(6, 4, 1) + FV(6, 4, 1))
C     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
C     *              *  (piN - 1.0) / 2.0
C      MULT(6, 4, 2) = (FB(6, 4, 2) + FV(6, 4, 2))
C     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
C     *              *  (piN - 1.0) / 2.0
C
C -----------------------------------  F35  -----------------------------------
      CALL SPLINT(W_sp, del_f35, del_f35_ss, 93, W_MeV, del)
      CALL SPLINT(W_sp, eta_f35, eta_f35_ss, 93, W_MeV, eta)
      IF (W_MeV.LT.1079.) del=0.00001
      IF (W_MeV.LT.1079.) eta=1.
      piN = eta * (COS(2.0 * del) + (0.0, 1.0) * SIN(2.0 * del))
      IF (del.EQ.0.0) THEN
         phi_f35 = 0.0
         phase = (1.0, 0.0)
      ELSE
         tmp = (piN + 1.0) / 2.0
      ARG= DIMAG(tmp) /DREAL(tmp)
      phi_tmp = DATAN(ARG)
         phi_f35 = DATAN((1.0 - eta * COS(2.0 * del))
     *           / eta / SIN(2.0 * del))
c         IF (W_MeV.GT.1400.AND.phi_f35.LT.0.0) phi_f35 = phi_f35 + pi
         IF (phi_f35.LT.0.0) phi_f35 = phi_f35 + pi
         del_phi = phi_f35 - phi_tmp
         phase = COS(del_phi) + (0.0, 1.0) * SIN(del_phi)
       END IF
      MULT(2, 4, 3) = (FB(2, 4, 3) + FV(2, 4, 1))
C     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
     *              *   (piN - 1.0) / 2.0
      MULT(4, 4, 3) = (FB(4, 4, 3) + FV(4, 4, 1))
C     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
     *              *   (piN - 1.0) / 2.0
      MULT(6, 4, 3) = (FB(6, 4, 3) + FV(6, 4, 1))
C     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
     *              *   (piN - 1.0) / 2.0
C
C -----------------------------------  F17  -----------------------------------
      CALL SPLINT(W_sp, del_f17, del_f17_ss, 93, W_MeV, del)
      CALL SPLINT(W_sp, eta_f17, eta_f17_ss, 93, W_MeV, eta)
      IF (W_MeV.LT.1079.) del=0.00001
      IF (W_MeV.LT.1079.) eta=1.
      piN = eta * (COS(2.0 * del) + (0.0, 1.0) * SIN(2.0 * del))
      MULT(1, 4, 1) = (FB(1, 4, 1) + FV(1, 4, 1))
     *              *  (piN - 1.0) / 2.0
      MULT(1, 4, 2) = (FB(1, 4, 2) + FV(1, 4, 2))
     *              *  (piN - 1.0) / 2.0
      MULT(3, 4, 1) = (FB(3, 4, 1) + FV(3, 4, 1))
     *              *  (piN - 1.0) / 2.0
      MULT(3, 4, 2) = (FB(3, 4, 2) + FV(3, 4, 2))
     *              *  (piN - 1.0) / 2.0
      MULT(5, 4, 1) = (FB(5, 4, 1) + FV(5, 4, 1))
     *              *  (piN - 1.0) / 2.0
      MULT(5, 4, 2) = (FB(5, 4, 2) + FV(5, 4, 2))
     *              *  (piN - 1.0) / 2.0
C
C -----------------------------------  F37  -----------------------------------
      CALL SPLINT(W_sp, del_f37, del_f37_ss, 93, W_MeV, del)
      CALL SPLINT(W_sp, eta_f37, eta_f37_ss, 93, W_MeV, eta)
      IF (W_MeV.LT.1079.) del=0.00001
      IF (W_MeV.LT.1079.) eta=1.
      piN = eta * (COS(2.0 * del) + (0.0, 1.0) * SIN(2.0 * del))
      IF (del.EQ.0.0) THEN
         phi_f37 = 0.0
         phase = (1.0, 0.0)
      ELSE
         tmp = (piN + 1.0) / 2.0
      ARG= DIMAG(tmp) /DREAL(tmp)
      phi_tmp = DATAN(ARG)
         phi_f37 = DATAN((1.0 - eta * COS(2.0 * del))
     *           / eta / SIN(2.0 * del))
c         IF (W_MeV.GT.1400.AND.phi_f37.LT.0.0) phi_f37 = phi_f37 + pi
         IF (phi_f37.LT.0.0) phi_f37 = phi_f37 + pi
         del_phi = phi_f37 - phi_tmp
         phase = COS(del_phi) + (0.0, 1.0) * SIN(del_phi)
       END IF
      MULT(1, 4, 3) = (FB(1, 4, 3) + FV(1, 4, 1))
C     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
     *              *   (piN - 1.0) / 2.0
      MULT(3, 4, 3) = (FB(3, 4, 3) + FV(3, 4, 1))
C     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
     *              *   (piN - 1.0) / 2.0
      MULT(5, 4, 3) = (FB(5, 4, 3) + FV(5, 4, 1))
C     *              *  ((piN + 1.0) / 2.0 * phase - 1.0)
     *              *   (piN - 1.0) / 2.0

C ***************  test **********
c      DO 1001 K=1,6
c      DO 1001 J=1,4
c      DO 1001 I=1,3
c      II=I
c      IF (I.EQ.3) II=1
c1001  MULT(K,J,I)=FB(K,J,I)
C1001  MULT(K,J,I)=FB(K,J,I)+FV(K,J,II)
C ********************************
      RETURN
      END

      SUBROUTINE SPLINES()
      IMPLICIT NONE
      INTEGER i
      REAL*8 a
      REAL*8 W_sp(93)
      REAL*8 del_s11(93),del_s11_ss(93),del_s31(93),del_s31_ss(93)
      REAL*8 del_p11(93),del_p11_ss(93),del_p31(93),del_p31_ss(93)
      REAL*8 del_p13(93),del_p13_ss(93),del_p33(93),del_p33_ss(93)
      REAL*8 del_d13(93),del_d13_ss(93),del_d33(93),del_d33_ss(93)
      REAL*8 del_d15(93),del_d15_ss(93),del_d35(93),del_d35_ss(93)
      REAL*8 del_f15(93),del_f15_ss(93),del_f35(93),del_f35_ss(93)
      REAL*8 del_f17(93),del_f17_ss(93),del_f37(93),del_f37_ss(93)
      REAL*8 eta_s11(93),eta_s11_ss(93),eta_s31(93),eta_s31_ss(93)
      REAL*8 eta_p11(93),eta_p11_ss(93),eta_p31(93),eta_p31_ss(93)
      REAL*8 eta_p13(93),eta_p13_ss(93),eta_p33(93),eta_p33_ss(93)
      REAL*8 eta_d13(93),eta_d13_ss(93),eta_d33(93),eta_d33_ss(93)
      REAL*8 eta_d15(93),eta_d15_ss(93),eta_d35(93),eta_d35_ss(93)
      REAL*8 eta_f15(93),eta_f15_ss(93),eta_f35(93),eta_f35_ss(93)
      REAL*8 eta_f17(93),eta_f17_ss(93),eta_f37(93),eta_f37_ss(93)
      COMMON /SPL/ W_sp,del_s11,del_s11_ss,del_s31,del_s31_ss,
     *      del_p11,del_p11_ss,del_p31,del_p31_ss,
     *      del_p13,del_p13_ss,del_p33,del_p33_ss,
     *      del_d13,del_d13_ss,del_d33,del_d33_ss,
     *      del_d15,del_d15_ss,del_d35,del_d35_ss,
     *      del_f15,del_f15_ss,del_f35,del_f35_ss,
     *      del_f17,del_f17_ss,del_f37,del_f37_ss,
     *      eta_s11,eta_s11_ss,eta_s31,eta_s31_ss,
     *      eta_p11,eta_p11_ss,eta_p31,eta_p31_ss,
     *      eta_p13,eta_p13_ss,eta_p33,eta_p33_ss,
     *      eta_d13,eta_d13_ss,eta_d33,eta_d33_ss,
     *      eta_d15,eta_d15_ss,eta_d35,eta_d35_ss,
     *      eta_f15,eta_f15_ss,eta_f35,eta_f35_ss,
     *      eta_f17,eta_f17_ss,eta_f37,eta_f37_ss
      REAL*8 yp1, ypn,pi
      PARAMETER(yp1=1.0E30, ypn = 1.0E30,pi = 3.1415926536D0)

            READ(2, 2111)
            READ(2, 2111)
 2111        FORMAT(20X)
      DO 2112 i = 1, 93
         READ(2, *) W_sp(i), del_s11(i),a,eta_s11(i)
         del_s11(i) = del_s11(i) * pi / 180.0
         eta_s11(i) = SQRT(1.0 - eta_s11(i))
 2112  CONTINUE
      CALL SPLINE(W_sp, del_s11, 93, yp1, ypn, del_s11_ss)
      CALL SPLINE(W_sp, eta_s11, 93, yp1, ypn, eta_s11_ss)

            READ(2, 2211)
            READ(2, 2211)
            READ(2, 2211)
 2211        FORMAT(20X)
      DO 2212 i = 1, 93
         READ(2, *) W_sp(i), del_s31(i),a,eta_s31(i)
         del_s31(i) = del_s31(i) * pi / 180.0
         eta_s31(i) = SQRT(1.0 - eta_s31(i))
 2212  CONTINUE
      CALL SPLINE(W_sp, del_s31, 93, yp1, ypn, del_s31_ss)
      CALL SPLINE(W_sp, eta_s31, 93, yp1, ypn, eta_s31_ss)

            READ(2, 2311)
            READ(2, 2311)
            READ(2, 2311)
 2311        FORMAT(20X)
      DO 2312 i = 1, 93
         READ(2, *) W_sp(i), del_p11(i),a,eta_p11(i)
         del_p11(i) = del_p11(i) * pi / 180.0
         eta_p11(i) = SQRT(1.0 - eta_p11(i))
 2312  CONTINUE
      CALL SPLINE(W_sp, del_p11, 93, yp1, ypn, del_p11_ss)
      CALL SPLINE(W_sp, eta_p11, 93, yp1, ypn, eta_p11_ss)

            READ(2, 2411)
            READ(2, 2411)
            READ(2, 2411)
 2411        FORMAT(20X)
      DO 2412 i = 1, 93
         READ(2, *) W_sp(i), del_p31(i),a,eta_p31(i)
         del_p31(i) = del_p31(i) * pi / 180.0
         eta_p31(i) = SQRT(1.0 - eta_p31(i))
 2412  CONTINUE
      CALL SPLINE(W_sp, del_p31, 93, yp1, ypn, del_p31_ss)
      CALL SPLINE(W_sp, eta_p31, 93, yp1, ypn, eta_p31_ss)

            READ(2, 2511)
            READ(2, 2511)
            READ(2, 2511)
 2511        FORMAT(20X)
      DO 2512 i = 1, 93
         READ(2, *) W_sp(i), del_p13(i),a,eta_p13(i)
         del_p13(i) = del_p13(i) * pi / 180.0
         eta_p13(i) = SQRT(1.0 - eta_p13(i))
 2512  CONTINUE
      CALL SPLINE(W_sp, del_p13, 93, yp1, ypn, del_p13_ss)
      CALL SPLINE(W_sp, eta_p13, 93, yp1, ypn, eta_p13_ss)

            READ(2, 2611)
            READ(2, 2611)
            READ(2, 2611)
 2611        FORMAT(20X)
      DO 2612 i = 1, 93
         READ(2, *) W_sp(i), del_p33(i),a,eta_p33(i)
         del_p33(i) = del_p33(i) * pi / 180.0
         IF (del_p33(i).LT.0.0.OR.W_sp(i).GT.1800.0)
     *       del_p33(i) = del_p33(i) + pi
         eta_p33(i) = SQRT(1.0 - eta_p33(i))
 2612  CONTINUE
      CALL SPLINE(W_sp, del_p33, 93, yp1, ypn, del_p33_ss)
      CALL SPLINE(W_sp, eta_p33, 93, yp1, ypn, eta_p33_ss)

            READ(2, 2811)
            READ(2, 2811)
            READ(2, 2811)
 2811        FORMAT(20X)
      DO 2812 i = 1, 93
         READ(2, *) W_sp(i), del_d13(i),a,eta_d13(i)
         del_d13(i) = del_d13(i) * pi / 180.0
         eta_d13(i) = SQRT(1.0 - eta_d13(i))
 2812  CONTINUE
      CALL SPLINE(W_sp, del_d13, 93, yp1, ypn, del_d13_ss)
      CALL SPLINE(W_sp, eta_d13, 93, yp1, ypn, eta_d13_ss)

            READ(2, 2911)
            READ(2, 2911)
            READ(2, 2911)
 2911        FORMAT(20X)
      DO 2912 i = 1, 93
         READ(2, *) W_sp(i), del_d33(i),a,eta_d33(i)
         del_d33(i) = del_d33(i) * pi / 180.0
         eta_d33(i) = SQRT(1.0 - eta_d33(i))
 2912  CONTINUE
      CALL SPLINE(W_sp, del_d33, 93, yp1, ypn, del_d33_ss)
      CALL SPLINE(W_sp, eta_d33, 93, yp1, ypn, eta_d33_ss)

            READ(2, 3011)
            READ(2, 3011)
            READ(2, 3011)
 3011        FORMAT(20X)
      DO 3012 i = 1, 93
         READ(2, *) W_sp(i), del_d15(i),a,eta_d15(i)
         del_d15(i) = del_d15(i) * pi / 180.0
         eta_d15(i) = SQRT(1.0 - eta_d15(i))
 3012  CONTINUE
      CALL SPLINE(W_sp, del_d15, 93, yp1, ypn, del_d15_ss)
      CALL SPLINE(W_sp, eta_d15, 93, yp1, ypn, eta_d15_ss)

            READ(2, 3111)
            READ(2, 3111)
            READ(2, 3111)
 3111        FORMAT(20X)
      DO 3112 i = 1, 93
         READ(2, *) W_sp(i), del_d35(i),a,eta_d35(i)
         del_d35(i) = del_d35(i) * pi / 180.0
         eta_d35(i) = SQRT(1.0 - eta_d35(i))
 3112  CONTINUE
      CALL SPLINE(W_sp, del_d35, 93, yp1, ypn, del_d35_ss)
      CALL SPLINE(W_sp, eta_d35, 93, yp1, ypn, eta_d35_ss)

            READ(2, 3211)
            READ(2, 3211)
            READ(2, 3211)
 3211        FORMAT(20X)
      DO 3212 i = 1, 93
         READ(2, *) W_sp(i), del_f15(i),a,eta_f15(i)
         del_f15(i) = del_f15(i) * pi / 180.0
         eta_f15(i) = SQRT(1.0 - eta_f15(i))
 3212  CONTINUE
      CALL SPLINE(W_sp, del_f15, 93, yp1, ypn, del_f15_ss)
      CALL SPLINE(W_sp, eta_f15, 93, yp1, ypn, eta_f15_ss)

            READ(2, 3311)
            READ(2, 3311)
            READ(2, 3311)
 3311        FORMAT(20X)
      DO 3312 i = 1, 93
         READ(2, *) W_sp(i), del_f35(i),a,eta_f35(i)
         del_f35(i) = del_f35(i) * pi / 180.0
         eta_f35(i) = SQRT(1.0 - eta_f35(i))
 3312  CONTINUE
      CALL SPLINE(W_sp, del_f35, 93, yp1, ypn, del_f35_ss)
      CALL SPLINE(W_sp, eta_f35, 93, yp1, ypn, eta_f35_ss)

            READ(2, 3411)
            READ(2, 3411)
            READ(2, 3411)
 3411        FORMAT(20X)
      DO 3412 i = 1, 93
         READ(2, *) W_sp(i), del_f17(i),a,eta_f17(i)
         del_f17(i) = del_f17(i) * pi / 180.0
         eta_f17(i) = SQRT(1.0 - eta_f17(i))
 3412  CONTINUE
      CALL SPLINE(W_sp, del_f17, 93, yp1, ypn, del_f17_ss)
      CALL SPLINE(W_sp, eta_f17, 93, yp1, ypn, eta_f17_ss)

            READ(2, 3511)
            READ(2, 3511)
            READ(2, 3511)
 3511        FORMAT(20X)
      DO 3512 i = 1, 93
         READ(2, *) W_sp(i), del_f37(i),a,eta_f37(i)
         del_f37(i) = del_f37(i) * pi / 180.0
         eta_f37(i) = SQRT(1.0 - eta_f37(i))
 3512  CONTINUE
      CALL SPLINE(W_sp, del_f37, 93, yp1, ypn, del_f37_ss)
      CALL SPLINE(W_sp, eta_f37, 93, yp1, ypn, eta_f37_ss)

      END


      SUBROUTINE BORN_MUL(IPVPS,W,Q2,M,MPI,LMAX,FB)
c *****************************************************************************
c     Born multipoles l=0,..,lmax
c *****************************************************************************
      IMPLICIT REAL*8(A-H,O-Z)
      REAL*8 M,MPI,MPIP,K0
      DIMENSION FPOL(6,10,3),AEP(3),AEM(3),AMP(3),AMM(3)
      DIMENSION FB(6,10,3),ALP(3),ALM(3)
c   IPVPS            :  0   PV coupling in the Born terms
c                    :  1   PS coupling in the Born terms
c                    :  2   mixed coupling (realistic)
      LMAX1=LMAX+1
      HQC=197.3285
      MPIP=139.5685/HQC
      K0=(W**2-M**2-Q2)/2./W

      CALL FPOLE(W,Q2,M,MPI,LMAX1,FPOL)
      CALL FNFPI(Q2,M,F1S,F1V,F2S,F2V,FPI)

      DO 1001 L1=1,LMAX1
      L=L1-1

      DO 99 ISO=1,3
      AEP(ISO)=0.
      AEM(ISO)=0.
      AMP(ISO)=0.
 99   AMM(ISO)=0.

      CL1=2*L1/MPIP/1000.
      CL=2*L/MPIP/1000.

      DO 1 ISO=1,3
      AEP(ISO)=FPOL(1,1,ISO)/CL1
      ALP(ISO)=K0*FPOL(5,L1,ISO)*MPIP*1000./L1
      IF (L1.EQ.1) GO TO 1
      AEP(ISO)=(FPOL(1,L1,ISO)-L*FPOL(3,L1,ISO))/CL1
      AMP(ISO)=(FPOL(1,L1,ISO)+(L+2)*FPOL(3,L1,ISO))/CL1
      AMM(ISO)=-FPOL(2,1,ISO)/CL1
      ALM(ISO)=K0*FPOL(6,L,ISO)*MPIP*1000./L
      IF (L.LT.1) GO TO 1
      AMM(ISO)=(-FPOL(2,L,ISO)+(L-1)*FPOL(4,L,ISO))/CL
       IF (L.LT.2) GO TO 1
      AEM(ISO)=(FPOL(2,L,ISO)+(L+1)*FPOL(4,L,ISO))/CL
 1    CONTINUE

c *********  PV-PS couplings *************
      DF1=0.
      DF2=0.
      DF5=0.
      DF6=0.

      IF (L.GT.1) GO TO 111
      CALL DFPVPS(IPVPS,W,Q2,M,MPI,DF1,DF2,DF5,DF6)
      IF (L.EQ.1) GO TO 222
      AEP(1)=AEP(1)+F2V*DF1*1000.
      AEP(2)=AEP(2)+F2S*DF1*1000.
      ALP(1)=ALP(1)+F2V*DF5*1000.
      ALP(2)=ALP(2)+F2S*DF5*1000.
      GO TO 111
 222  AMM(1)=AMM(1)-F2V*DF2*1000.
      AMM(2)=AMM(2)-F2S*DF2*1000.
      ALM(1)=ALM(1)-F2V*DF6*1000.
      ALM(2)=ALM(2)-F2S*DF6*1000.
 111  CONTINUE
C **************************************
        FB(1,L1,1)=AEP(1)+2.*AEP(3)
        FB(2,L1,1)=AEM(1)+2.*AEM(3)
        FB(3,L1,1)=AMP(1)+2.*AMP(3)
        FB(4,L1,1)=AMM(1)+2.*AMM(3)
        FB(5,L1,1)=ALP(1)+2.*ALP(3)
        FB(6,L1,1)=ALM(1)+2.*ALM(3)

        FB(1,L1,3)=AEP(1)-AEP(3)
        FB(2,L1,3)=AEM(1)-AEM(3)
        FB(3,L1,3)=AMP(1)-AMP(3)
        FB(4,L1,3)=AMM(1)-AMM(3)
        FB(5,L1,3)=ALP(1)-ALP(3)
        FB(6,L1,3)=ALM(1)-ALM(3)

        FB(1,L1,2)=AEP(2)
        FB(2,L1,2)=AEM(2)
        FB(3,L1,2)=AMP(2)
        FB(4,L1,2)=AMM(2)
        FB(5,L1,2)=ALP(2)
        FB(6,L1,2)=ALM(2)

 1001  CONTINUE
       RETURN
       END


      SUBROUTINE DFPVPS(IPVPS,WFM,LAM2,M,MPI,DF1,DF2,DF5,DF6)
c *******************  PS-PV mixing  ******************************************
      IMPLICIT REAL*8(A-H,O-Z)
      COMMON /E0CORR/ XA, XB, XMIX
      REAL*8 M,MPI,LAM2,K0,K2,KF,NU,Kopplung
      PARAMETER (Pi=3.1415926536D0,eLadung=.3028619D0,
     * Kopplung=0.996365D0, apvps0=450.,HQC=197.3285 )

       DF1=0.D0
       DF2=0.D0
       DF5=0.d0
       DF6=0.D0

       if (ipvps.eq.1) return

      apvps=apvps0*XMIX
      W=WFM
      SIG=W**2-M**2
      BET=SIG+LAM2/2.
      K0=(SIG-LAM2)/2./W
      K2=K0**2+LAM2
      KF=DSQRT(K2)
      Q0=(SIG+MPI**2)/2./W
      Q2=Q0**2-MPI**2
      Q=DSQRT(Q2)
      E1=DSQRT(M**2+K2)
      E2=DSQRT(M**2+Q2)

        fpvps=apvps**2/(apvps**2+Q2*hqc**2)
        IF(IPVPS.EQ.0) FPVPS=1.

        EFIP=DSQRT((E1+M)*(E2+M))
	C0 =fpvps*eLadung*Kopplung/(8.*Pi*W*M)
        DF1=C0*(W-M)*EFIP/2.
        DF2=C0*(W+M)*Q*KF/EFIP/2.
        DF5=C0*K0*EFIP/2.
        DF6=C0*(E1-M)*K0*Q/KF/2.* DSQRT((E1+M)/(E2+M))

	RETURN
	END

      SUBROUTINE FPOLE(WFM,LAM2,M,MPI,LMAX1,FPOL)
      IMPLICIT REAL*8(A-H,O-Z)
      REAL*8 M,MPI,LAM2
      DIMENSION HH1(10,3),HH3(10,3),HH5(10,3),FPOL(6,10,3)
      DIMENSION HH1M(10,3),HH3M(10,3),HH5M(10,3)

      CALL ODDH(WFM,LAM2,M,MPI,LMAX1+1,HH1,HH3,HH5)
      CALL ODDH(-WFM,LAM2,M,MPI,LMAX1+1,HH1M,HH3M,HH5M)

      DO 1 ISO=1,3
      DO 1 L1=1,LMAX1

      FPOL(1,L1,ISO) = HH1(L1,ISO) + HH1M(L1+1,ISO)
      FPOL(2,L1,ISO) = HH1M(L1,ISO) + HH1(L1+1,ISO)
      FPOL(3,L1,ISO) = HH3(L1,ISO) - HH3M(L1+1,ISO)
      FPOL(4,L1,ISO) =- HH3M(L1,ISO) + HH3(L1+1,ISO)
      FPOL(5,L1,ISO) = HH5(L1,ISO) + HH5M(L1+1,ISO)
      FPOL(6,L1,ISO) = HH5M(L1,ISO) + HH5(L1+1,ISO)
 1    CONTINUE

      RETURN
      END

      SUBROUTINE ODDH(WFM,LAM2,M,MPI,LMAX1,HH1,HH3,HH5)
c ***************  helicity multipoles  ****************************
      IMPLICIT REAL*8(A-H,O-Z)
      REAL*8 M,MPI,LAM2,K0,K2,KF,NU,MPIP
      DIMENSION HM(6),FIJ(6,10),NEPS(3)
      DIMENSION QLZ(10),QLY(10),HH1(10,3),HH3(10,3),HH5(10,3)
      DATA NEPS/1,1,-1/
      PARAMETER (Pi=3.1415926536D0,er=.3028619D0,fr=0.996365D0)

      W=WFM
      COF=8.*PI*W
      SIG=W**2-M**2
      BET=SIG+LAM2/2.
      K0=(SIG-LAM2)/2./W
      K2=K0**2+LAM2
      KF=DSQRT(K2)
      Q0=(SIG+MPI**2)/2./W
      Q2=Q0**2-MPI**2
      Q=DSQRT(Q2)
      NU=-(W+M)
      OM=W-M
      E1=W+M-K0
      E2=W+M-Q0
      G1=-W+M+K0
      G2=-W+M+Q0

      MPIP=139.5685/197.3285
      GPIN=2.*M/MPIP*fr
      QKF2=2*Q*KF
      Z=(2.*Q0*K0+LAM2)/QKF2
      Y=(2.*Q0*K0-SIG)/QKF2

      CALL FNFPI(LAM2,M,F1S,F1V,F2S,F2V,FPI)
      CALL HFMATR(W,LAM2,M,MPI,LMAX1+1,HM,FIJ)
      CALL QLEG(Z,LMAX1+1,QLZ)
      CALL QLEG(Y,LMAX1+1,QLY)

      DO 1 ISO=1,3

      F1I=F1V*er
      F2I=F2V*er/2./M
      FPII=0.5*er*(1.-NEPS(ISO))*FPI
      IF (ISO.NE.2) GO TO 111
      F1I=F1S*er
      F2I=F2S*er/2./M
 111  CONTINUE

      A1 = GPIN/K2 * FPII * (LAM2+2.*NU*(Q0-K0)) / QKF2
      B1 = GPIN/2/K2 * NEPS(ISO) * ( (SIG+2*NU*Q0)*F1I +
     + ((NU+2*K0)*SIG + 2*Q0*LAM2) * F2I ) / QKF2
      C1 = GPIN/2/K2* ((F1I-OM*F2I)*2*E1/NU + 2*FPII +
     + NEPS(ISO)*(F1I + (NU+2*K0)*F2I))

      A3 = -2*GPIN * FPII/QKF2
      B3 = -GPIN * NEPS(ISO)*(F1I-NU*F2I)/QKF2

      A5 = GPIN/K2 *FPII*(2*Q0-K0)/QKF2
      B5 = - GPIN/K2/2. * NEPS(ISO) * ((OM-2*Q0)*F1I +
     + ((2*Q0-K0)*NU+SIG+LAM2)*F2I)/QKF2
      C5 =  GPIN/K2/2.*((2*FPII-(1-NEPS(ISO))*F1I)*K0/LAM2 +
     + (F1I+(E1-NEPS(ISO)*NU)*F2I)/NU)

      DL=1.
      RLZ=0.
      RLY=0.

      DO 2 L1=1,LMAX1
      IF (L1.EQ.1) GO TO 222
      DL=0.
      RLZ=(QLZ(L1-1)-QLZ(L1+1))/(2*L1-1)
      RLY=(QLY(L1-1)-QLY(L1+1))/(2*L1-1)
 222  HH1(L1,ISO)=HM(1)*(A1*QLZ(L1)+B1*QLY(L1)+C1*DL)
      HH3(L1,ISO)=HM(3)*(A3*RLZ + B3*RLY)
      HH5(L1,ISO)=HM(5)*(A5*QLZ(L1)+B5*QLY(L1)+C5*DL)

  2   CONTINUE
  1   CONTINUE

      RETURN
      END


      SUBROUTINE QLEG(Z,LMAX1,QL)
      IMPLICIT REAL*8(A-H,O-Z)
      DIMENSION QL(1)


      DO 1 L1=1,LMAX1
      L=L1-1
1     QL(L1)=QLEGY(Z,L)

      RETURN
      END

      FUNCTION QLEGY(ZZ,L)
      IMPLICIT REAL*8(A-H,O-Z)
C
      DIMENSION DURALT(11),DALT(11),DNEU(11),C(11)
C     WRITE(6,2)L,Z
    2 FORMAT(' INPUT OF QLEG L,Z=',I2,D20.8)
      Z=ZZ
      SIGN=Z/DABS(Z)
      IF(Z .LT. 0.0) Z=-Z
      IF((Z-1.)*(L+1).LT.2.) GO TO 100
      MMAX=20
      EPS=2.D-7
      LG=L+1
      X=1./Z
      XX=X*X
      SUM=0
      TERM=1.0
      M=0
      C(1)=1.0
      IF(L.EQ.0) GO TO 5
      DO 4 N=2,LG
      XN=N-2
4     C(N)=(XN+1.)/(2.*XN+3.)*C(N-1)
5     SUM=SUM+C(L+1)*TERM
      NALT=1
      DALT(1)=SUM
      GO TO 14
10    NURALT=NALT
      DO 11 N=1,NURALT
11    DURALT(N)=DALT(N)
      NALT=NNEU
      DO 12 N=1,NALT
12    DALT(N)=DNEU(N)
14    M=M+1
      XM=M-1
      TERM=XX*TERM
      C(1)=1./(2.*XM+3.)
      IF(L.EQ.0) GO TO 16
      DO 15 N=2,LG
      XN=N-1
15    C(N)=((2.*XM+XN+1.)*C(N)+XN*C(N-1))/(2.*(XN+XM)+3.)
16    SUM=SUM+C(L+1)*TERM
      NNEU=M/2+1
      DNEU(1)=SUM
      IF(M.LE.1) GO TO 10
      Y1=DNEU(1)-DALT(1)
      Y2=DURALT(1)-DALT(1)
      Z1=1.D99
      IF(Y1.NE.0) Z1=1./Y1
      Z2=1.D99
      IF(Y2.NE.0) Z2=1./Y2
      DNEU(2)=DALT(1)+1./(Z1+Z2)
      IF(NNEU.LE.2) GO TO 20
      DO 18 N=3,NNEU
      Y1=DNEU(N-1)-DALT(N-1)
      Y2=DURALT(N-1)-DALT(N-1)
      Y3=DURALT(N-2)-DALT(N-1)
      Z1=1.D99
      IF(Y1.NE.0) Z1=1./Y1
      Z2=1.D99
      IF(Y2.NE.0) Z2=1./Y2
      Z3=1.D99
      IF(Y3.NE.0) Z3=1./Y3
      DNEU(N)=DALT(N-1)+1./(Z1+Z2-Z3)
18    CONTINUE
20    CONTINUE
      IF(M.GE.MMAX) GO TO 30
      IF(ABS(DNEU(NNEU)-DNEU(NNEU-1)).LT.EPS*ABS(DNEU(NNEU))) GO TO 40
      GO TO 10
30    WRITE(6,31) MMAX,EPS,L,Z
31    FORMAT(////10X,'WARNUNG :'/10X,'IN QLEG KONNTE NACH',I3/10X,
     *       'SCHRITTEN DIE GENAUIGKEIT VON ',E8.2,/10X,'FUER Q',I2,
     *       1H(,E12.6,') NICHT ERREICHT WERDEN'/)
40    QLEGY=X**(L+1)*DNEU(NNEU)
      QLEGY=SIGN**(L+1)*QLEGY
C     PRINT 2,L,Z,QLEG
      RETURN
100   Y=0.5*DLOG((Z+1.D0)/(Z-1.D0))
      IF(L.EQ.0) GO TO 120
      YALT=Y
      Y=Z*Y-1.0
      IF(L.EQ.1) GO TO 120
      DO 110 N=2,L
      XN=N-1
      YURALT=YALT
      YALT=Y
110   Y=((2.*XN+1.)*Z*YALT-XN*YURALT)/(XN+1.)
120   QLEGY=Y
      QLEGY=SIGN**(L+1)*QLEGY

      RETURN
      END


      SUBROUTINE FNFPI(lam2,MFM,F1S,F1V,F2S,F2V,FPI)
c  ****************** e.m. form  factors *********************+*
      IMPLICIT REAL*8(A-H,O-Z)
      REAL*8 mn,lam2,mg2,mup,mun,MFM
      PARAMETER (mg2 =.71D6,mup=2.7928,mun= -1.913,HQC=197.3285)

        Q2=lam2*HQC**2
        mn=MFM*HQC

	tau = Q2/(4.*mn*mn)
	F = 1./(1.+Q2/mg2)**2
	GEP = F
	GMN = mun*F
	GEN = (-tau)/(1.+4.*tau)*GMN
	GMP = mup*F
	F1P = (GEP+tau*GMP)/(1.+tau)
	F1N = (GEN+tau*GMN)/(1.+tau)
	F2P = (GMP-GEP)/((mup-1.)*(1.+tau))
	F2N = (GMN-GEN)/(mun*(1.+tau))
	F1S =F1P+F1N
	F1V =F1P-F1N
	F2S = (mup-1.)*F2P+mun*F2N
	F2V = (mup-1.)*F2P-mun*F2N
        FPI = F1V

      RETURN
      END


      SUBROUTINE HFMATR(WFM,LAM2,M,MPI,LMAX1,HM,FIJ)
c  ***************  helicity multipoles  *****************************
      IMPLICIT REAL*8(A-H,O-Z)
      REAL*8 M,MPI,LAM2,K0,K2,KF,NU
      DIMENSION HM(6),FIJ(6,10)
      DATA PI/3.1415926536/

      W=WFM
      COF=8.*PI*W
      SIG=W**2-M**2
      BET=SIG+LAM2/2.
      K0=(SIG-LAM2)/2./W
      K2=K0**2+LAM2
      KF=DSQRT(K2)
      Q0=(SIG+MPI**2)/2./W
      Q2=Q0**2-MPI**2
      Q=DSQRT(Q2)
      NU=-(W+M)
      OM=W-M
      E1=W+M-K0
      E2=W+M-Q0
      G1=-W+M+K0
      G2=-W+M+Q0

      HM(1)=G1*DSQRT(E1*E2)/COF
      HM(2)=-E1*DSQRT(G1*G2)/COF
      HM(3)=E2*DSQRT(G1*G2)/COF
      HM(4)=G2*DSQRT(E1*E2)/COF
      HM(5)=-HM(1)
      HM(6)=-HM(2)

      DO 1 L1=1,LMAX1
      L=L1-1
      COF1=(Q*KF)**L*W
      COF2=(Q*KF)**(L-1)
      DO 1 I=1,6
      C=COF1
      IF (I.EQ.3) C=COF2
      IF (I.EQ.4) C=COF2
      FIJ(I,L1)=C*HM(I)
 1    CONTINUE

      RETURN
      END

      SUBROUTINE EL(W, Q2_MeV, m_N, m_pi, l, spin, iso, Mul)
      IMPLICIT REAL*8(A-H,O-Z)
      INTEGER spin
      REAL*8 Mul
      DIMENSION Q_l(20)
      REAL*8 m_N, m_pi, m_V
      REAL*8 l_V, l_om, l_rho
      REAL*8 k, l_1, l_2, l_3
      PARAMETER (pi = 3.1415926536D0, mg2 = 0.71D6)
      PARAMETER (m_om = 783.0, m_rho = 770.0, l_om = 0.314,
     *           gV_om = 21.0, gT_om = -0.5714, cut_om = 1.2E3,
     *           l_rho = 0.103, gV_rho = 2.0, gT_rho = 6.5,
     *           cut_rho = 1.5E3, el_ch = 0.3028619)

      dip = mg2

      IF (iso.EQ.1) THEN
      m_V = m_om
      l_V = el_ch * l_om / m_pi
      gV_V = gV_om
      gT_V = gT_om * gV_V / 2.0 / m_N
      cut_V = cut_om
      ELSE IF (iso.EQ.2) THEN
      m_V = m_rho
      l_V = el_ch * l_rho / m_pi
      gV_V = gV_rho
      gT_V = gT_rho * gV_V / 2.0 / m_N
      cut_V = cut_rho
      ENDIF

      W_m = W - m_N
      W_p = W + m_N
      sigma = W_m * W_p
      om_gam = (sigma - Q2_MeV) / 2.0 / W
      q = SQRT(om_gam**2 + Q2_MeV)
      om_pi = (sigma + m_pi**2) / 2.0 / W
      k = SQRT(om_pi**2 - m_pi**2)
      E_i = W - om_gam
      D_i = SQRT(E_i + m_N)
      E_f = W - om_pi
      D_f = SQRT(E_f + m_N)
      arg_Q = (m_V**2 - m_pi**2 + 2.0 * om_gam * om_pi
     *      + Q2_MeV) / 2.0 / k / q

      CALL QLEG(arg_Q, l + 5, Q_l)

      IF (l.LT.2.AND.spin.EQ.2) GO TO 333

      IF (spin.EQ.1) THEN
         l_1 = 1.0 / (real(l) + 1)
         l_2 = real(l)
         l_3 = (real(l) + 1.0) / (2.0 * real(l) + 3.0)
         Q_2 = Q_l(l + 2)
         Q_4 = Q_l(l + 1) - Q_l(l + 3)

      ELSE IF (spin.EQ.2) THEN
         l_1 = 1.0 / real(l)
         l_2 = - (real(l) + 1.0)
         l_3 = real(l) / (2.0 * real(l) - 1.0)
         Q_2 = Q_l(l)
         Q_4 = Q_l(l + 1) - Q_l(l - 1)
      ENDIF
      delta = 0.0
      IF (l.EQ.0) delta = 1.0

      F_V = 1.0 / (1.0 + Q2_MeV / mg2)**2
     *    * cut_V**2 / (cut_V**2 + k * k + q * q)
      Q_1 = Q_l(l + 1)
      IF (l.GT.0) THEN
         Q_3 = (Q_l(l) - Q_l(l + 2)) / (2.0 * real(l) + 1.0)
      ELSE IF (l.EQ.0) THEN
         Q_3 = 0
      END IF

      Mul = l_1 * l_V * F_V / 16.0 / pi / W * D_f / D_i
     *     * (D_i**2 / k / q * Q_1 * ((W_m**2
     *     + (m_V**2 - m_pi**2  + Q2_MeV) / 2.0) * gV_V
     *     - W_m * m_V**2 * gT_V)
     *     - Q_2 / D_f**2 * ((W_p**2
     *     + (m_V**2 - m_pi**2 + Q2_MeV) / 2.0) * gV_V
     *     + W_p * m_V**2 * gT_V)
     *     - l_2 * Q_3 * (W_p * gV_V - (W_p * W_m + Q2_MeV) * gT_V)
     *     - l_3 * Q_4 * k / q * D_i**2 / D_f**2
     *     * (W_m * gV_V + (W_p * W_m + Q2_MeV) * gT_V)
     *     + delta * (-gV_V + 2.0 * W_m * gT_V) * D_i**2)
c '-' in last line before gV_V needs further confirmation

      GO TO 334

 333  Mul = 0.0
 334  CONTINUE

      END

      SUBROUTINE ML(W, Q2_MeV, m_N, m_pi, l, spin, iso, Mul)
      IMPLICIT REAL*8(A-H,O-Z)
      INTEGER spin
      REAL*8 Mul
      DIMENSION Q_l(20)
      REAL*8 m_N, m_pi, m_V
      REAL*8 l_V, l_om, l_rho
      REAL*8 k, l_1
      PARAMETER (pi = 3.1415926536D0, mg2 = 0.71D6)
      PARAMETER (m_om = 783.0, m_rho = 770.0, l_om = 0.314,
     *           gV_om = 21.0, gT_om = -0.5714, cut_om = 1.2E3,
     *           l_rho = 0.103, gV_rho = 2.0, gT_rho = 6.5,
     *           cut_rho = 1.5E3, el_ch = 0.3028619)

      dip = mg2

      IF (iso.EQ.1) THEN
      m_V = m_om
      l_V = el_ch * l_om / m_pi
      gV_V = gV_om
      gT_V = gT_om * gV_V / 2.0 / m_N
      cut_V = cut_om
      ELSE IF (iso.EQ.2) THEN
      m_V = m_rho
      l_V = el_ch * l_rho / m_pi
      gV_V = gV_rho
      gT_V = gT_rho * gV_V / 2.0 / m_N
      cut_V = cut_rho
      ENDIF

      W_m = W - m_N
      W_p = W + m_N
      sigma = W_m * W_p
      om_gam = (sigma - Q2_MeV) / 2.0 / W
      q = SQRT(om_gam**2 + Q2_MeV)
      om_pi = (sigma + m_pi**2) / 2.0 / W
      k = SQRT(om_pi**2 - m_pi**2)
      E_i = W - om_gam
      D_i = SQRT(E_i + m_N)
      E_f = W - om_pi
      D_f = SQRT(E_f + m_N)
      arg_Q = (m_V**2 - m_pi**2 + 2.0 * om_gam * om_pi
     *      + Q2_MeV) / 2.0 / k / q

      CALL QLEG(arg_Q, l + 5, Q_l)

      IF (l.LT.1) GO TO 335

      delta = 0.0

      IF (spin.EQ.1) THEN
         l_1 = 1.0 / (real(l) + 1)
         Q_2 = Q_l(l + 2)

      ELSE IF (spin.EQ.2) THEN
         l_1 = - 1.0 / real(l)
         Q_2 = Q_l(l)
         IF (l.EQ.1) delta = 1.0
      ENDIF

      F_V = 1.0 / (1.0 + Q2_MeV / mg2)**2
     *    * cut_V**2 / (cut_V**2 + k * K + q * q)
      Q_1 = Q_l(l + 1)
      Q_3 = (Q_l(l) - Q_l(l+2)) / (2.0 * real(l) + 1.0)

      Mul = l_1 * l_V * F_V / 16.0 / pi / W * D_f / D_i *
     *    (Q_1 * D_i**2 / k / q * ((W_m**2
     *  + (m_V**2 - m_pi**2 + Q2_MeV) / 2.0) * gV_V
     *  - W_m * m_V**2 * gT_V)
     *  - Q_2 / D_f**2 * ((W_p**2
     *  + (m_V**2 - m_pi**2 + Q2_MeV) / 2.0) * gV_V
     *  + W_p * m_V**2 * gT_V)
     *  + Q_3 * (W_p * gV_V - (W_m * W_p + Q2_MeV) * gT_V)
     *  + delta * k * q / D_f**2 * (gV_V + 2.0 * W_p * gT_V))

      GO TO 336

 335  Mul = 0.0

 336  CONTINUE

      END

      SUBROUTINE LL(W, Q2_MeV, m_N, m_pi, l, spin, iso, Mul)
      IMPLICIT REAL*8(A-H,O-Z)
      INTEGER spin
      REAL*8 Mul
      DIMENSION Q_l(20)
      REAL*8 m_N, m_pi, m_V
      REAL*8 l_V, l_om, l_rho
      REAL*8 k, l_1
      PARAMETER (pi = 3.1415926536D0, mg2 = 0.71D6)
      PARAMETER (m_om = 783.0, m_rho = 770.0, l_om = 0.314,
     *           gV_om = 21.0, gT_om = -0.5714, cut_om = 1.2E3,
     *           l_rho = 0.103, gV_rho = 2.0, gT_rho = 6.5,
     *           cut_rho = 1.5E3, el_ch = 0.3028619)


      dip = mg2

      IF (iso.EQ.1) THEN
      m_V = m_om
      l_V = el_ch * l_om / m_pi
      gV_V = gV_om
      gT_V = gT_om * gV_V / 2.0 / m_N
      cut_V = cut_om
      ELSE IF (iso.EQ.2) THEN
      m_V = m_rho
      l_V = el_ch * l_rho / m_pi
      gV_V = gV_rho
      gT_V = gT_rho * gV_V / 2.0 / m_N
      cut_V = cut_rho
      ENDIF

      W_m = W - m_N
      W_p = W + m_N
      sigma = W_m * W_p
      om_gam = (sigma - Q2_MeV) / 2.0 / W
      q = SQRT(om_gam**2 + Q2_MeV)
      om_pi = (sigma + m_pi**2) / 2.0 / W
      k = SQRT(om_pi**2 - m_pi**2)
      E_i = W - om_gam
      D_i = SQRT(E_i + m_N)
      E_f = W - om_pi
      D_f = SQRT(E_f + m_N)
      arg_Q = (m_V**2 - m_pi**2 + 2.0 * om_gam * om_pi
     *      + Q2_MeV) / 2.0 / k / q

      CALL QLEG(arg_Q, l + 5, Q_l)

      delta_1 = 0.0
      delta_2 = 0.0
      IF (l.LT.1.AND.spin.EQ.2) GO TO 337

      IF (spin.EQ.1) THEN
         l_1 = 1.0 / (real(l) + 1)
         Q_2 = Q_l(l + 2)
         Q_4 = Q_l(l + 2)
         IF (l.EQ.0) delta_1 = 1.0

      ELSE IF (spin.EQ.2) THEN
         l_1 = 1.0 / real(l)
         Q_2 = Q_l(l)
         Q_4 = Q_l(l)
         IF (l.EQ.1) delta_2 = 1.0
      ENDIF

      F_V = 1.0 / (1.0 + Q2_MeV / dip)**2
     *    * cut_V**2 / (cut_V**2 + k * K + q * q)
      Q_1 = Q_l(l + 1)
      Q_3 = Q_l(l + 1)

      Mul = l_1 * l_V * F_V / 16.0 / pi / W * D_f / D_i
     *    * (((-D_i**2 * (m_pi**2 - Q2_MeV - 2.0 * om_gam * om_pi)
     *    - 2.0 * E_f * (om_gam**2 + Q2_MeV))* gT_V
     *    + (E_f - m_N) * (E_i + m_N) * gV_V) * Q_1 / k
     *    + D_i**2 / D_f**2 * (((E_i - m_N)
     *    * (m_pi**2 - Q2_MeV - 2.0 * om_gam * om_pi)
     *    + 2.0 * E_f * (om_gam**2 + Q2_MeV)) * gT_V
     *    + (E_i - m_N) * (E_f + m_N) * gV_V) * Q_2 / q
     *    + (2.0 * m_N * gT_V + gV_V) * (q * delta_1 + k * delta_2
     *    - arg_Q * (q * Q_3 + k * D_i**2 / D_f **2 * Q_4)))
     *    * om_gam / q

      GO TO 338

 337  Mul = 0.0

 338  CONTINUE

      END


      SUBROUTINE spline(x,y,n,yp1,ypn,y2)
      INTEGER n,NMAX
      REAL*8 yp1,ypn,x, y, y2
      dimension x(100),y(100),y2(100)
      PARAMETER (NMAX=500)
      INTEGER i,k
      REAL*8 p,qn,sig,un,u
      dimension u(500)

      if (yp1.gt..99e30) then
        y2(1)=0.
        u(1)=0.
      else
        y2(1)=-0.5
        u(1)=(3./(x(2)-x(1)))*((y(2)-y(1))/(x(2)-x(1))-yp1)
      endif
      do 11 i=2,n-1
        sig=(x(i)-x(i-1))/(x(i+1)-x(i-1))
        p=sig*y2(i-1)+2.
        y2(i)=(sig-1.)/p
        u(i)=(6.*((y(i+1)-y(i))/(x(i+
     *1)-x(i))-(y(i)-y(i-1))/(x(i)-x(i-1)))/(x(i+1)-x(i-1))-sig*
     *u(i-1))/p
11    continue
      if (ypn.gt..99e30) then
        qn=0.
        un=0.
      else
        qn=0.5
        un=(3./(x(n)-x(n-1)))*(ypn-(y(n)-y(n-1))/(x(n)-x(n-1)))
      endif
      y2(n)=(un-qn*u(n-1))/(qn*y2(n-1)+1.)
      do 12 k=n-1,1,-1
        y2(k)=y2(k)*y2(k+1)+u(k)
C        PRINT *, k, y2(k)
12    continue
      return
      END
C  (C) Copr. 1986-92 Numerical Recipes Software VsXz&2+L.0(9p+.

      SUBROUTINE splint(xa,ya,y2a,n,x,y)
      INTEGER n
      REAL*8 x,y,xa(n),y2a(n),ya(n)
      INTEGER k,khi,klo
      REAL*8 a,b,h
      klo=1
      khi=n
1     if (khi-klo.gt.1) then
        k=(khi+klo)/2
        if(xa(k).gt.x)then
          khi=k
        else
          klo=k
        endif
      goto 1
      endif
      h=xa(khi)-xa(klo)
c      if (h.eq.0.) pause 'bad xa input in splint'
      a=(xa(khi)-x)/h
      b=(x-xa(klo))/h
      y=a*ya(klo)+b*ya(khi)+((a**3-a)*y2a(klo)+(b**3-b)*y2a(khi))*(h**
     *2)/6.
      return
      END
C  (C) Copr. 1986-92 Numerical Recipes Software VsXz&2+L.0(9p+.
