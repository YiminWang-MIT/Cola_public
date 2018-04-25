C      PROGRAM KAON_MAID
C      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
C      PI=3.14159265358979D0
C      FINE=137.03604D0
C      ichan=1
C     ---------------------------------------------------------------
C     Kinematics, must be checked and rechecked before used!!
C     Ei  = initial electron energy
C     Ef  = final electron energy
C     The = Electron scatt angle
C     THRAD = Kaon c.m. scatt angle in radian
C     ---------------------------------------------------------------
c      Ei=
c      Eo=
c      THe=
c      EGLab=Ei-Eo
c      Q2=4d0*Ei*Eo*DSIN(THe/2d0)*DSIN(THe/2d0)
c      QI2=EGLab**2-Q2
c      EPS=1d0/(1d0+2d0*QI2*DTAN(The/2)*DTAN(The/2)/Q2)
c      GAMMA=0.5d0/(PI*PI*FINE)*(Eo/Ei)*EGLab/(Q2*(1d0-EPS))
C      Q2=0.5D6
C      W = 1.8d3
C     ---------------------------------------------------------------
C      DO I=1,37
C         TH=(I-1)*180d0/36d0
C         THRAD=TH*PI/180d0
C         call K_MAID(ICHAN,Q2,W,THRAD,ST,SL,STL,STT,STLP)
c         TOTAL=ST+EPS*SL+EPS*STT*DCOS(2d0*PHIK)+
c     .         DSQRT(0.5d0*EPS*(1d0+EPS))*STL*DCOS(PHIK)
C         write (*,100) TH,ST,SL,STT,STL,STLP
C      ENDDO
C 100  FORMAT(1X,F6.0,1X,5(F8.3,1X))
C      STOP
C      END


C     ---------------------------------------------------------------
C        This subroutine calculates A1-A6,F1-F6, and H1-H6 
C        for photo- and electroproduction of kaon
C     ---------------------------------------------------------------
C
C     I've changed the name from k_maid to kmaid, since old compilers
C     will complain 
C
      SUBROUTINE KMAID(ICHAN,QP2,W,TH,ST,SL,STL,STT,STLP)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      COMPLEX*16 A,A1,A2,A3,A4,A5,A6,ZERO,H1,H2,H3,H4,H5,H6,F1,F2,F3,
     .		 F4,F5,F6,F5X,F6X,AFH(6),BB1,BB2,BB3,BB4,BB5,BB6,FFK
      COMMON /MASSS/ XMN,XMK,XMY,XMPI,XMETA,XMZETA,XML
      DATA XMP,XMNN,XML,XMS0,XMSP,XMSM,XMK0,XMKP,XMKSP,XMKS0,XMQ1/
     .     938.27231,939.56563,1115.63,1192.55,1189.37,1197.43,497.671,
     .	   493.646,891.59,896.10,1270.0/,XMN1,XMN2,XMN3,XMN4,XMN5,XMN6,
     .	   XMN7,XMY1,XMY2,XMY3,XMY4,XMY5,XMY6,XMY7/1440.,1520.,1535.,
     .	   1650.,1700.,1710.,1720.,1405.,1600.,1670.,1800.,1810.,1660.,
     .	   1750./,XMD1,XMD2,XMD3,XMDH1,XMDH2,XMDH3/1232.,1700.,1920.,
     .	   1620.,1900.,1910./,
     .	   WKS,WKS0,WQ1,WN1,WN2,WN3,WN4,WN5,WN6,WN7,WY1,WY2,WY3,WY4,
     .	   WY5,WY6,WY7/49.8,50.5,90.,200.,120.,150.,150.,100.,100.,
     .     150.,50.,150.,35.,300.,150.,100.,90./,WD1,WD2,WD3,WDH1,
     .     WDH2,WDH3/120.,300.,200.,150.,200.,250./,
     .	   XKP,XKNN,XKL,XKS0,XKSP,XKSM,AF/1.79284739D0,-1.9130427D0,
     .	   -0.7289D0,1.2157D0,2.068D0,-0.48D0,137.0359895D0/,
     .	   XBM,XMPI,XMETA,XMZETA/1000.,139.57,547.3,276./,
     .     S11STATE,P11STATE,P13STATE,D13STATE/0.0,1.0,1.0,2.0/,
     .     S11PIN,S11ZN,S11EN,S11KL/0.73,0.22,0.00,0.05/,
     .     P11PIN,P11ZN,P11EN,P11KL/0.00,0.51,0.32,0.17/,
     .     P13PIN,P13ZN,P13EN,P13KL/0.21,0.75,0.04,0.01/,
     .     D13PIN,D13ZN,D13EN,D13KL/0.10,0.75,0.14,0.01/
      ZERO=DCMPLX(0.D0,0.D0)
      XE=1.D0/DSQRT(AF)
      PI=3.14159265358979D0
C     -----------------------INITIALIZATION-----------------------------
      GL=0.D0
      GS=0.D0
      GV=0.D0
      GT=0.D0
      GQV=0.D0
      GQT=0.D0
      GN1=0.D0
      GN2=0.D0
      GN3=0.D0
      GN4=0.D0
      GN51=0.D0
      GN52=0.D0
      GN53=0.D0
      GN6=0.D0
      GN71=0.D0
      GN72=0.D0
      GN73=0.D0
      GDH2=0.D0
      GDH2X=0.D0
      GDH3=0.D0
      GDH3X=0.D0
      THG=0.D0
      PHG=0.D0
      IGAM=0
      MOD_FFN =1
      MOD_FFL =1
      MOD_FFS =1
      MOD_FFK =1
      MOD_FFKS=1
      MOD_FFK0=1
      MOD_WIL =1
C     ---------------------------------------------------------------
      IHA     =3
      IGAM    =1
      IRECIPES=1
      LET     =0
      MN2080  =1
C
      IF(ICHAN.EQ.1 .OR. ICHAN.EQ.2) THEN
         GS   =  1.2000*1.956D0  
         GL   = -3.8000          
         GV   = -0.7867          
         GT   = -2.6279          
         GN4  = -0.1332          
C         GN4X = 1.2297           
         GN4X = 0.  
         GN6  = -0.2575          
C         GN6X = -0.1830          
         GN6X = 0.
         GDH2 = 0.D0
         GDH2X= 0.D0
         GDH3 = 0.D0
         GDH3X= 0.D0
         GQV  = 3.8104           
         GQT  = -2.4078          
         GN51 = 1.1013           
         GN52 = 0.6345           
C         GN53 = 10.3313          
         GN53 = 0.          
         GN71 = 0.0463           
         GN72 = 0.6140           
C         GN73 = -8.9999          
         GN73 = 0.          
         cut_born=0.6366*1000.D0
         cut_res =1.0340*1000.D0
         XMN5FIT=1.8953*1000.D0
         WN5FIT=0.3722*1000.D0
         THGWU=PI*90.D0/180.D0
         PHGWU=PI*180.D0/180.D0
      ELSE
         GL   = -3.8000*1.956D0  
         GS   =  1.2000          
         GV   = -0.0310          
         GT   = -0.1717          
         GN4  = -0.0277          
         GN4X = -1.2694          
         GN6  =  0.1610          
         GN6X = 12.9388          
         GDH2 = 0.1060           
         GDH2X= 0.5074           
         GDH3 = 0.3215           
         GDH3X= -1.1892          
         GQV  = -1.3639          
         GQT  = -0.5176          
         GN51 = 0.0000
         GN52 = 0.0000
         GN53 = 0.0000
         GN71 = -0.0563          
         GN72 = 0.2361           
         GN73 = 4.9762           
         cut_born=0.8238*1000.D0
         cut_res =1.2603*1000.D0
         THGWU=PI*90.033/180.D0
         PHGWU=PI*0.1553/180.D0
      ENDIF
      R_K1    =-0.4474
      CUT_N4  =0.8629
      CUT_N4X =0.4945
      CUT_N6  =1.0886
      CUT_N6X =0.5098
      CUT_NP31=1.1773
      CUT_NP32=0.8915
      CUT_NP33=0.7051
      CUT_ND31=0.3468
      CUT_ND32=0.5828
      CUT_ND33=0.4795
      CUT_D2  =0.3970
      CUT_D2X =0.7844
      CUT_D3  =1.9997
      CUT_D3X =1.2860
      CUT_KS  =1.5053
      CUT_K1  =0.6689
C
C     ---------------------------------------------------------------
C           		PARAMETERS FOR EACH PROCESS
C     ---------------------------------------------------------------
      CIS=1.D0
      GOTO (10,20,30,40,50,60), ICHAN
C     ---------------------------------------------------------------
C     		      GAMMA + PROTON --> K+ + LAMBDA
C     ---------------------------------------------------------------
 10   GY=GL
      GYP=GS
      XKN=XKP
      XKY=XKL
      XMN=XMP
      XMY=XML
      XMK=XMKP
      XMKS=XMKSP
      XMYP=XMS0
      CDEL=0.D0
      CIS=1.D0
      QN=1.D0
      QY=0.D0
      QK=1.D0
      GOTO 70
C     ---------------------------------------------------------------
C     		     GAMMA + NEUTRON --> K0 + LAMBDA 
C     ---------------------------------------------------------------
 20   GY=GL
      GYP=-GS
      XKN=XKNN
      XKY=XKL
      XMN=XMNN
      XMK=XMK0
      XMKS=XMKS0
      XMY=XML
      XMYP=XMS0
      CDEL=0.D0
      CIS=1.D0
      QN=0.D0
      QY=0.D0
      QK=0.D0
      GOTO 70
C     ---------------------------------------------------------------
C     		     GAMMA + PROTON --> K+ + SIGMA0 
C     ---------------------------------------------------------------
 30   GY=GS
      GYP=GL
      XKN=XKP
      XKY=XKS0
      XMN=XMP
      XMY=XMS0
      XMYP=XML
      XMK=XMKP
      XMKS=XMKSP
      CDEL=1.D0
      CIS=1.D0
      QN=1.D0
      QY=0.D0
      QK=1.D0
      GOTO 70
C     ---------------------------------------------------------------
C     		     GAMMA + PROTON --> K0 + SIGMA+ 
C     ---------------------------------------------------------------
 40   GY=DSQRT(2.D0)*GS
      GYP=0.D0
      XKN=XKP
      XKY=XKSP
      XMN=XMP
      XMY=XMSP
      XMK=XMK0
      XMKS=XMKS0
      CDEL=-1.D0/DSQRT(2.D0)
      CIS=DSQRT(2.D0)
      QN=1.D0
      QY=1.D0
      QK=0.D0
      GOTO 70
C     ---------------------------------------------------------------
C     		     GAMMA + NEUTRON --> K+ + SIGMA- 
C     ---------------------------------------------------------------
 50   GY=DSQRT(2.D0)*GS
      GYP=0.D0
      XKN=XKNN
      XKY=XKSM
      XMN=XMNN
      XMK=XMKP
      XMKS=XMKSP
      XMY=XMSM
      CDEL=1.D0/DSQRT(2.D0)
      CIS=DSQRT(2.D0)
      QN=0.D0
      QY=-1.D0
      QK=1.D0
      GOTO 70
C     ---------------------------------------------------------------
C     		     GAMMA + NEUTRON --> K0 + SIGMA0
C     ---------------------------------------------------------------
 60   GY=-GS
      GYP=GL
      XKN=XKNN
      XKY=XKS0
      XMN=XMNN
      XMY=XMS0
      XMK=XMK0
      XMKS=XMKS0
      XMYP=XML
      CDEL=1.D0
      CIS=-1.D0
      QN=0.D0
      QY=0.D0
      QK=0.D0
 70   CONTINUE
C     ---------------------------------------------------------------
C     				 KINEMATICS 
C     ---------------------------------------------------------------
      S=W*W
      XK2=-QP2
      EN=(S+XMN*XMN-XK2)/2.D0/W
      XKI2=EN*EN-XMN*XMN
      XK0=DSQRT(XK2+XKI2)
      XKI=DSQRT(XKI2)
      EY=(S+XMY*XMY-XMK*XMK)/2.D0/W
      XQI2=EY*EY-XMY*XMY
      XQ0=DSQRT(XQI2+XMK*XMK)
      XQI=DSQRT(XQI2)
      QIDKI=XQI*XKI*DCOS(TH)
      QDK=XQ0*XK0-QIDKI
      T=XMK*XMK+XK2-2.D0*QDK
      U=XMN*XMN+XMK*XMK-2.D0*(EN*XQ0+QIDKI)
      SMN2=S-XMN*XMN
      TMK2=T-XMK*XMK
      UMY2=U-XMY*XMY
      EGLAB=(SMN2-XK2)/2.D0/XMN
      EGCM =XK0
      Q_K  =XQI
      CTH  =DCOS(TH)
C     ==================================================================
C                       Calling the form factors
C     ==================================================================
C
C     ---------------------------------------------------------------
C                      Nucleon's form factors
C     ---------------------------------------------------------------
      IF (MOD_FFN .EQ. 1) THEN
         CALL FF_PROTON1(-XK2,FFN1,FFN2,FFNEUT)
      ELSE IF (MOD_FFN .EQ. 2) THEN
         CALL FF_PROTON2(-XK2,FFN1,FFN2,FFNEUT)
      ELSE
C         WRITE(11,*) 'Error in the model of nucleon form factors'
         STOP
      ENDIF
C
      IF(ICHAN.EQ.2 .OR. ICHAN.EQ.5 .OR. ICHAN.EQ.6) THEN
         FFN1=0.D0
         FFN2=FFNEUT
      ENDIF
C     ---------------------------------------------------------------
C                        Kaon's form factors
C     ---------------------------------------------------------------
      IF(ICHAN.EQ.1 .OR. ICHAN.EQ.3 .OR. ICHAN.EQ.5) THEN
         IF (MOD_FFK .EQ. 1) THEN
            CALL FF_KAON1(-XK2,FFK)
         ELSE IF (MOD_FFK .EQ. 2) THEN
            CALL FF_KAON2(-XK2,FFK)
         ELSE
C            WRITE(11,*) 'Error in the model of the K+ form factor'
            STOP
         ENDIF
      ELSE IF(ICHAN.EQ.2 .OR. ICHAN.EQ.4 .OR. ICHAN.EQ.6) THEN
         CALL FF_KZERO(-XK2,MOD_FFK0,FFK)
      ELSE
C         WRITE(11,*) 'Error in the channel number at K0 FF loop'
         STOP
      ENDIF
C     ---------------------------------------------------------------
C                  Lambda's and transition form factors
C     ---------------------------------------------------------------
      IF(ICHAN.EQ.1 .OR. ICHAN.EQ.2 .OR. ICHAN.EQ.3 .OR. ICHAN.EQ.6) 
     .     THEN
         IF (MOD_FFL .EQ. 1) THEN
            CALL FF_LAMBDA1(MOD_WIL,-XK2,F2LAMBDA,F2TRANS)
         ELSE IF (MOD_FFL .EQ. 2) THEN
            CALL FF_LAMBDA2(-XK2,F2LAMBDA,F2TRANS)
         ELSE
C            WRITE(11,*) 'Error in the model of the lambda form factor'
            STOP
         ENDIF
      ENDIF
      IF(ICHAN.EQ.1 .OR. ICHAN.EQ.2) THEN
         FFY1=0.D0
         FFY2=F2LAMBDA
         FFTRS=F2TRANS
      ELSE IF(ICHAN.EQ.3 .OR. ICHAN.EQ.6) THEN
         CALL FF_SIGMA0(-XK2,FFSIG0)
         FFY1=0.D0
         FFY2=FFSIG0
         FFTRS=F2TRANS
      ELSE IF(ICHAN.EQ.4) THEN
         CALL FF_SIGPLUS(-XK2,FFSIGP1,FFSIGP2)
         FFY1=FFSIGP1
         FFY2=FFSIGP2
         FFTRS=0.D0
      ELSE IF(ICHAN.EQ.5) THEN
         CALL FF_SIGMINS(-XK2,FFSIGM1,FFSIGM2)
         FFY1=FFSIGM1
         FFY2=FFSIGM2
         FFTRS=0.D0
      ELSE
C         WRITE(11,*) 'Error in the channel number at Lambda FF loop'
         STOP
      ENDIF
C     ---------------------------------------------------------------
C	    Electromagnetic form factors for resonant particles
C     ---------------------------------------------------------------
C    -----N*(1650) -----
      Q2=-XK2/1.D6
C      CUT_1650=1.178D3
C      CALL FF_GREGOR(-XK2,CUT_1650,FFN4)
      FFN4  = 1.D0/(1.D0+Q2/CUT_N4**2)**2
      FFN4X = 1.D0/(1.D0+Q2/CUT_N4X**2)**2
C	----- N*(1720) -----
      FFN6  = 1.D0/(1.D0+Q2/CUT_N6**2)**2
      FFN6X = 1.D0/(1.D0+Q2/CUT_N6X**2)**2
C    ----- Delta(1900) -----
      FDEL2  = 1.D0/(1.D0+Q2/CUT_D2**2)**2
      FDEL2X = 1.D0/(1.D0+Q2/CUT_D2X**2)**2
C    ----- Delta(1910) -----
      FDEL3  = 1.D0/(1.D0+Q2/CUT_D3**2)**2
      FDEL3X = 1.D0/(1.D0+Q2/CUT_D3X**2)**2
C    ----- P13(1720) ----
      FFNP31= 1.D0/(1.D0+Q2/CUT_NP31**2)**2
      FFNP32= 1.D0/(1.D0+Q2/CUT_NP32**2)**2
      FFNP33= 1.D0/(1.D0+Q2/CUT_NP33**2)**2
C    ----- D13(1895) ----
      FFND31= 1.D0/(1.D0+Q2/CUT_ND31**2)**2
      FFND32= 1.D0/(1.D0+Q2/CUT_ND32**2)**2
      FFND33= 1.D0/(1.D0+Q2/CUT_ND33**2)**2
C    ----- K* and K1 ----
      FFKS  = 1.D0/(1.D0+Q2/CUT_KS**2)
      FFK1  = 1.D0/(1.D0+Q2/CUT_K1**2)
C
      FFYS=FFY2
      FFNS2=FFN2
      FDEL1=FFN2
C     ---------------------------------------------------------------
C                      Reset the coefficient functions
C     ---------------------------------------------------------------
      A1=ZERO
      A2=ZERO
      A3=ZERO
      A4=ZERO
      A5=ZERO
      A6=ZERO
C     ----------------------------------------------------------------
C                  Prepare the Helmut Haberzettl's Constants
C     ----------------------------------------------------------------
      AX1=(DSIN(THGWU)*DCOS(PHGWU))**2
      AX2=(DSIN(THGWU)*DSIN(PHGWU))**2
      AX3=(DCOS(THGWU))**2
C      IF(IPV.NE.0) WRITE(11,*) 'PSEUDOVECTOR IS NOT CONSIDERED'
c     ------------------------------------------------------------------
c     Some combinations of hadronic form factors
c     ------------------------------------------------------------------
      ffshh = hff(cut_born,xmn,s,IHA)
      ffuhh = hff(cut_born,xmy,u,IHA)
      ffthh = hff(cut_born,xmk,t,IHA)
      ffsig = hff(cut_born,xmyp,u,IHA)
      ffsut = ax1*ffshh+ax2*ffuhh+ax3*ffthh
      dfshh = ffsut-ffshh
      dfuhh = ffsut-ffuhh
      dfthh = ffsut-ffthh
      xgmhh = xe*gy/(xmn+xmy)
      chh_bar=1.d0-chh
      if (LET .eq. 0) then
         chh=0.d0
         chh_bar=0.d0
      endif
      hhl_bar=0.d0
      betahh=0.d0
C     ==================================================================
C     The BORN TERMS are reconstructed for including Haberzettl's recipe
C     ------------------------------------------------------------------
      A1 = -xe*ffshh*gy*(qn*ffn1+xkn*ffn2*(xmn-xmy)/2.d0/xmn)/smn2
     .     -xe*ffuhh*gy*(qy*ffy1+xky*ffy2*(xmy-xmn)/2.d0/xmy)/umy2
     .     -chh_bar*xgmhh*(ffshh*xkn*ffn2/xmn+ffuhh*xky*ffy2/xmy)/2.d0
     .     -betahh*xe*gy*(dfshh*qn/smn2+dfuhh*qy/umy2)
     .     -(1.D0-DABS(QY))*XE*ffsig*
     .      GYP/(U-XMYP*XMYP)*(XMYP-XMN)/(XMYP+XMY)*FFTRS
C     ==================================================================
C        Ohta Method (first case) does not have the hadr. FF in A2
C     ------------------------------------------------------------------
      IF ((irecipes.EQ.0) .OR. ((iha.EQ.0) .OR. (iha.EQ.1))) THEN
        A2=xe*gy*2.d0*(qn*ffn1/smn2+qy*ffy1/umy2)/tmk2
     .      -chh*xgmhh*( ffshh*xkn*ffn2/xmn
     .                   +ffuhh*xky*ffy2/xmy )/tmk2
      ELSE
        A2=xe*gy*2.d0*ffsut*( qn/smn2+qy/umy2 )/tmk2
     .     +xe*gy*2.d0*( qn*ffshh*(ffn1-1.d0)/smn2
     .                  +qy*ffuhh*(ffy1-1.d0)/umy2
     .                  -chh*( ffshh*xkn*ffn2/xmn/2.d0
     .                         +ffuhh*xky*ffy2/xmy/2.d0)
     .                  /(xmn+xmy))/tmk2
      ENDIF
C     ==================================================================
      A3 = xe*ffshh*gy*xkn*ffn2/smn2/2.d0/xmn
     .     -xe*gy*ffuhh*xky*ffy2/umy2/2.d0/xmy
     .	   -(1.D0-DABS(QY))*XE*ffsig*GYP*FFTRS/
     .      (U-XMYP*XMYP)/(XMYP+XMY)
     .     -betahh*xgmhh*(dfshh*qn/smn2-dfuhh*qy/umy2
     .                     +2.d0*dfthh*qk/tmk2)
c
C     ==================================================================
      A4 = xe*ffshh*gy*xkn*ffn2/smn2/2.d0/xmn
     .     +xe*ffuhh*gy*xky*ffy2/umy2/2.d0/xmy
     .	   +(1.D0-DABS(QY))*XE*ffsig*GYP*
     .       FFTRS/(U-XMYP*XMYP)/(XMYP+XMY)
     .     -betahh*xgmhh*(dfshh*qn/smn2+dfuhh*qy/umy2)
C     ==================================================================
C     Photoproduction doesn't like A5 and A6
      IF(DABS(XK2).LT.1.D-6) GOTO 400
C     ==================================================================
      A5 = xe*gy*ffsut*( qn/smn2-qy/umy2 )/tmk2
     .     +xe*gy/tmk2*( qn*ffshh*(ffn1-1.d0)/smn2
     .                  -qy*ffuhh*(ffy1-1.d0)/umy2
     .                  -chh*( ffshh*xkn*ffn2/2.d0/xmn
     .                         -ffuhh*xky*ffy2/2.d0/xmy )/(xmn+xmy)
     .                  +chh*( smn2*ffshh*xkn*ffn2/xmn
     .                         -umy2*ffuhh*xky*ffy2/xmy )/(xmn+xmy)/xk2
     .                  +2.d0/xk2*( ffthh*qk*(ffk-1.d0)
     .                             -ffshh*qn*(ffn1-1.d0)
     .                             +ffuhh*qy*(ffy1-1.d0) ) )
C     ==================================================================
      A6 = -hhl_bar*xgmhh/xk2*
     .      ( ffshh*qn*(ffn1-1.d0)-ffuhh*qy*(ffy1-1.d0) )
     .     -chh*xgmhh*(xmn-xmy)/xk2*
     .      ( ffshh*xkn*ffn2/2.d0/xmn+ffuhh*xky*ffy2/2.d0/xmy )
     .     -betahh*xgmhh*dfthh*qk/tmk2
C     ==================================================================
 400  CONTINUE
C     ---------------------------------------------------------------
C      FIRST PARTS ARE RESONANCES WITH PARITY(+) EXCEPT FOR MESONS
C     ---------------------------------------------------------------
C     			 NUCLEON RESONANCES-SPIN 1/2
C         values of CNS6 and CNS4 were 0.45 and -0.40
C     FFN4X and FFN6X are FF/k^2 and therefore shouldn't have singu-
C	larities. Only N4 and N6 are used in this code!
C     ---------------------------------------------------------------
      CNS4=1.D0
      CNS6=1.D0
C	-----------N6(1710)-----------Par(+)
      IF(IGAM.EQ.1) WN6=WIDTH(P11STATE,XMN6,P11PIN,P11ZN,P11EN,P11KL,
     .                  S,100.D0)
      IF(ICHAN.EQ.2.OR.ICHAN.EQ.5.OR.ICHAN.EQ.6) CNS6=-0.22D0
      A = XE*HFF(cut_res,XMN6,S,IHA)*CIS*CNS6*GN6*FFN6/DCMPLX(S-
     .    XMN6*XMN6,XMN6*WN6)
      AX = XE*HFF(cut_res,XMN6,S,IHA)*CIS*GN6X*FFN6X/DCMPLX(S-XMN6*
     .     XMN6,XMN6*WN6)/1.D6
      A1 = A1-A*DCMPLX(XMN6-XMY,-WN6/2.D0)/(XMN6+XMN)-AX*XK2
      A2 = A2+AX*2.D0*XK2/TMK2
      A3 = A3+A/(XMN6+XMN)
      A4 = A4+A/(XMN6+XMN)
      A5 = A5+AX*(XK2-2.D0*SMN2)/TMK2
      A6 = A6+AX*DCMPLX(XMN-XMN6,WN6/2.D0)
C
C	-----------N4(1650)-----------Par(-)
      IF(IGAM.EQ.1) WN4=WIDTH(S11STATE,XMN4,S11PIN,S11ZN,S11EN,S11KL,
     .                  S,150.D0)
      IF(ICHAN.EQ.2.OR.ICHAN.EQ.5.OR.ICHAN.EQ.6) CNS4=-0.28D0
      A = XE*HFF(cut_res,XMN4,S,IHA)*CIS*CNS4*GN4*FFN4/DCMPLX(S-XMN4*
     .    XMN4,XMN4*WN4)
      AX = XE*HFF(cut_res,XMN4,S,IHA)*CIS*GN4X*FFN4X/DCMPLX(S-XMN4*
     .     XMN4,XMN4*WN4)/1.D6 
      A1 = A1-A*DCMPLX(XMN4+XMY,-WN4/2.D0)/(XMN4+XMN)-AX*XK2
      A2 = A2+AX*2.D0*XK2/TMK2
      A3 = A3-A/(XMN4+XMN)
      A4 = A4-A/(XMN4+XMN)
      A5 = A5+AX*(XK2-2.D0*SMN2)/TMK2
      A6 = A6+AX*DCMPLX(XMN+XMN4,-WN4/2.D0)
C     ---------------------------------------------------------------
C       		MESON RESONANCE Parity (-)
C     ---------------------------------------------------------------
      CKS1=1.D0
      IF(ICHAN.EQ.2.OR.ICHAN.EQ.4.OR.ICHAN.EQ.6) CKS1=-1.53D0
      A = CIS*HFF(CUT_BORN,XMKS,T,IHA)*CKS1*FFKS/(XBM*DCMPLX(T-XMKS*
     .    XMKS,XMKS*WKS)*(XMN+XMY)) 
      A1 = A1-A*GT*T
      A2 = A2+A*GT*(1.D0+XK2/TMK2)
      A3 = A3+A*GT*(XMN-XMY)
      A4 = A4+A*GV*(XMN+XMY)
      A5 = A5+A*GT*(UMY2-SMN2)/2.D0/TMK2
      A6 = A6+A*GT*(XMN-XMY)
C     ---------------------------------------------------------------
C       		MESON RESONANCE Parity (+)
C     ---------------------------------------------------------------
      CKS2=1.D0
      IF(ICHAN.EQ.2.OR.ICHAN.EQ.4.OR.ICHAN.EQ.6) CKS2=R_K1
      A = CIS*HFF(CUT_BORN,XMQ1,T,IHA)*CKS2*FFK1/(XBM*DCMPLX(T-XMQ1*
     .    XMQ1,XMQ1*WQ1)*(XMN+XMY))
      A2 = A2-A*GQT*(1.D0+XK2/TMK2)
      A3 = A3+A*(GQV*(XMY+XMN)+GQT*(XMY-XMN))
      A5 = A5-A*GQT*(UMY2-SMN2)/2.D0/TMK2
      A6 = A6+A*(GQV*(XMY+XMN)+GQT*(XMY-XMN))
C     ---------------------------------------------------------------
C     			 SPIN 1/2 DELTA RESONANCES 
C	Only D2(1900) and D3(1910) are used in this model. D1(1620) 
C	has not yet modified.
C     ---------------------------------------------------------------
C	------D3(1910)------
      A = CDEL*XE*HFF(cut_res,XMDH3,S,IHA)*GDH3*FDEL3/
     .    DCMPLX(S-XMDH3*XMDH3,XMDH3*WDH3)
      AX = CDEL*XE*HFF(cut_res,XMDH3,S,IHA)*GDH3X*FDEL3X/
     .     DCMPLX(S-XMDH3*XMDH3,XMDH3*WDH3)/1.D6
      A1 = A1-A*DCMPLX(XMDH3-XMY,-WDH3/2.D0)/(XMDH3+XMN)-AX*XK2
      A2 = A2+AX*2.D0*XK2/TMK2
      A3 = A3+A/(XMDH3+XMN)
      A4 = A4+A/(XMDH3+XMN)
      A5 = A5+AX*(XK2-2.D0*SMN2)/TMK2
      A6 = A6+AX*DCMPLX(XMN-XMDH3,WDH3/2.D0)
C
C	------D2(1900)------
      A = CDEL*XE*HFF(cut_res,XMDH2,S,IHA)*GDH2*FDEL2/
     .    DCMPLX(S-XMDH2*XMDH2,XMDH2*WDH2)
      AX = CDEL*XE*HFF(cut_res,XMDH2,S,IHA)*GDH2X*FDEL2X/
     .     DCMPLX(S-XMDH2*XMDH2,XMDH2*WDH2)/1.D6
      A1 = A1-A*DCMPLX(XMDH2+XMY,-WDH2/2.D0)/(XMDH2+XMN)-AX*XK2
      A2 = A2+AX*2.D0*XK2/TMK2
      A3 = A3-A/(XMDH2+XMN)
      A4 = A4-A/(XMDH2+XMN)
      A5 = A5+AX*(XK2-2.D0*SMN2)/TMK2
      A6 = A6+AX*DCMPLX(XMN+XMDH2,-WDH2/2.D0)
C     ---------------------------------------------------------------
C     		    SPIN 3/2  NUCLEON RESONANCES 
C     ---------------------------------------------------------------
C     P_13(1720) resonance
C     --------------------
      SQS=DSQRT(S)
      SQSP=SQS+XMN
      SQSN=SQS-XMN
      IF(IGAM.EQ.1) WN7=WIDTH(P13STATE,XMN7,P13PIN,P13ZN,P13EN,P13KL,
     .                  S,150.D0)
      IF(ICHAN.EQ.2.OR.ICHAN.EQ.5.OR.ICHAN.EQ.6) THEN
        CNS71=-2.24
        CNS72=0.42
        CNS73=1.D0
      ELSE
        CNS71=1.D0
        CNS72=1.D0
        CNS73=1.D0
      ENDIF
      A = CIS*HFF(cut_res,XMN7,S,IHA)/(3.D0*XMN7*DCMPLX(S-XMN7*XMN7,
     .    XMN7*WN7)*SQSP)
      AX = A/SQSP
      A1 = A1+A*(GN71*FFNP31*CNS71/2.D0*(3.D0*T-XK2-XMK*XMK-2.D0*XMN*
     .     XMN-2.D0*XMY*XMY+(XMK*XMK-XMY*XMY)/S*((SMN2+XK2)+(XMN+XMY)*
     .     SQSP)+(XMN+XMY)*(2.D0*SQSP+3.D0*XMY+(S+XMN*XMY-XMK*XMK+XMY*
     .     XMY)/SQS)-4.D0*XMY*SQSP+(XMY*(SMN2+XK2)-2.D0*XMN*(S-XMK*XMK+
     .     XMY*XMY))/SQS)+GN72*FFNP32*CNS72/SQSP*((XMN+XMY)/2.D0*
     .     ((3.D0*T-S+3.D0*XK2-XMY*XMY-2.D0*XMK*XMK)/2.D0-XMY*SQSN+XMY*
     .     XK2/SQS)+(S-XMK*XMK+XMY*XMY)/4.D0/SQS*((XMN+XMY)*(XMN-2.D0*
     .     XK2/SQS)+(SMN2-XK2))+XMY/2.D0*(SMN2-XK2)))
     .     +AX*GN73*FFNP33*CNS73*XK2*((S-XMK*XMK-XMY*XMN)/(2.D0*SQS)
     .     +(XMN+XMY)*(XMY*XMY-XMK*XMK)/(2.D0*S)-XMN)
      A2 = A2+A*(GN71*FFNP31*CNS71*(-3.D0+XK2/TMK2*(1.D0-(XMK*XMK-
     .     XMY*XMY+XMY*SQS)/S))+GN72*FFNP32*CNS72/SQSP*(XK2/2.D0/TMK2*
     .     (3.D0*SQSN-2.D0/SQS*((S-XMK*XMK+XMY*XMY)*(1.5D0-XMN/SQS)+
     .     XMN*XMY))+1.5D0*SQSN))
     .     +AX*GN73*FFNP33*CNS73*(-XK2/S)*(SQSN/TMK2)*(2.D0*S-XMY*XMY
     .     +XMK*XMK+XMY*SQS)
      A3 = A3+A*SQSP*(GN71*FFNP31*CNS71*(-2.D0+0.5D0/SQSP*(3.D0*XMY+
     .     (S+XMN*XMY-XMK*XMK+XMY*XMY)/SQS)+(XMK*XMK-XMY*XMY)/2.D0/S)+
     .     GN72*FFNP32*CNS72/SQSP/SQSP*(0.25D0*(5.D0*S+3.D0*T-6.D0*
     .     XMN*XMN-2.D0*XMK*XMK-XMY*XMY-3.D0*XK2)-0.5D0*XMY*SQSN+(S-
     .     XMK*XMK+XMY*XMY)*(XMN/4.D0/SQS-XK2/2.D0/S)+XMY/2.D0/SQS*XK2))
     .     +AX*GN73*FFNP33*CNS73*XK2*(2.D0+(XMY*XMY-XMK*XMK)/(2.D0*S)
     .     -XMY/(2.D0*SQS))
      A4 = A4+A*SQSP*(GN71*FFNP31*CNS71*(1.D0+0.5D0/SQSP*(3.D0*XMY+
     .     (S+XMN*XMY-XMK*XMK+XMY*XMY)/SQS)+(XMK*XMK-XMY*XMY)/2.D0/S)+
     .     GN72*FFNP32*CNS72/SQSP/SQSP*(-0.25D0*(S-3.D0*T+2.D0*XMK*
     .     XMK+XMY*XMY-3.D0*XK2)-0.5D0*XMY*SQSN+(S-XMK*XMK+XMY*XMY)*
     .     (XMN/4.D0/SQS-XK2/2.D0/S)+XMY/2.D0/SQS*XK2))
     .     +AX*GN73*FFNP33*CNS73*XK2*(-1.D0+(XMY*XMY-XMK*XMK)/(2.D0*S)
     .     +XMY/(2.D0*SQS))
      A5 = A5+A*(GN71*FFNP31*CNS71/2.D0*(1.D0+(XK2-2.D0*SMN2)/TMK2*
     .     (1+(XMY*XMY-XMK*XMK-XMY*SQS)/S))+GN72*FFNP32*CNS72/SQSP*
     .     ((2.D0*SMN2-XK2)/TMK2*(-0.75D0*SQSN+((S-XMK*XMK+XMY*XMY)*
     .     (0.75D0-XMN/2.D0/SQS)+XMN*XMY/2.D0)/SQS)-0.75D0*SQSN))
     .     +AX*GN73*FFNP33*CNS73*(SQSN/2.D0/TMK2)*(3.D0*(XK2-2.D0*UMY2)
     .     +(XK2-2.D0*SMN2)*(S-XMK*XMK+XMY*XMY-XMY*SQS)/S)
      A6 = A6+A*SQSP*(GN71*FFNP31*CNS71/2.D0*(-(XMK*XMK-XMY*XMY)/S+
     .     (3.D0*XMY-2.D0*SQSP*(1.D0+XMY/SQS)+(S+XMN*XMY-XMK*XMK+XMY*
     .     XMY)/SQS)/SQSP)+GN72*FFNP32*CNS72/SQSP/SQSP*(0.75D0*(S+T-
     .     2.D0*XMN*XMN-XMY*XMY-XK2)+XMY*XMN/2.D0*(1.D0-XMN/SQS)+(S-
     .     XMK*XMK+XMY*XMY)*(XMN/4.D0/SQS+XK2/2.D0/S)))
     .     +AX*GN73*FFNP33*CNS73*(1.5D0*UMY2+XMY*SQSP*SQSP/(2.D0*SQS)
     .     +(S-XMK*XMK+XMY*XMY)*(SQS*SQSP-SMN2)/(2.D0*S))
C     ---------------------------------------------------------------------
C     D_13(1700) resonance
C     --------------------
C     This is only for KLambda channel !!!
      IF(ICHAN.EQ.1 .OR. ICHAN.EQ.2) THEN
      IF(MN2080.EQ.1) THEN
         XMNX=XMN5FIT
         WN5TP=WN5FIT
      ELSE
         XMNX=XMN5
         WN5TP=WN5
      ENDIF
      IF(IGAM.EQ.1) THEN
         WN5X=WIDTH(D13STATE,XMNX,D13PIN,D13ZN,D13EN,D13KL,S,WN5TP)
      ELSE
         WN5X=WN5TP
      ENDIF
      IF(ICHAN.EQ.2.OR.ICHAN.EQ.5.OR.ICHAN.EQ.6) THEN
        CNS51=-0.103
        CNS52=-0.055
        CNS53=1.D0
      ELSE
        CNS51=1.D0
        CNS52=1.D0
        CNS53=1.D0
      ENDIF
      A = CIS*HFF(cut_res,XMNX,S,IHA)/(3.D0*XMNX*DCMPLX(S-XMNX*XMNX,
     .    XMNX*WN5X)*SQSN) 
      AX = A/SQSN
      A1 = A1+A*(GN51*FFND31*CNS51/2.D0*(3.D0*T-XK2-XMK*XMK-2.D0*XMN*
     .	   XMN-2.D0*XMY*XMY+(XMK*XMK-XMY*XMY)/S*((SMN2+XK2)-(XMN+XMY)*
     .	   SQSN)-(XMN+XMY)*(2.D0*SQSN-3.D0*XMY+(S+XMN*XMY-XMK*XMK+XMY*
     .     XMY)/SQS)+4.D0*XMY*SQSN-(XMY*(SMN2+XK2)-2.D0*XMN*(S-XMK*XMK+
     .	   XMY*XMY))/SQS)+GN52*FFND32*CNS52/SQSN*(-(XMN+XMY)/2.D0*
     .	   ((3.D0*T-S+3.D0*XK2-XMY*XMY-2.D0*XMK*XMK)/2.D0+XMY*SQSP-
     .	   XMY*XK2/SQS)+(S-XMK*XMK+XMY*XMY)/4.D0/SQS*((XMN+XMY)*
     .	   (XMN+2.D0*XK2/SQS)+(SMN2-XK2))-XMY*(SMN2-XK2)/2.D0))
     .     +AX*GN53*FFND33*XK2*((S-XMK*XMK-XMY*XMN)/(2.D0*SQS)-(XMN+XMY)
     .     *(XMY*XMY-XMK*XMK)/(2.D0*S)+XMN)
      A2 = A2+A*(GN51*FFND31*CNS51*(-3.D0+XK2/TMK2*(1.D0-(XMK*XMK-XMY*
     .	   XMY-XMY*SQS)/S))+GN52*FFND32*CNS52/SQSN*(XK2/2.D0/TMK2*
     .	   (3.D0*SQSP-2.D0/SQS*((S-XMK*XMK+XMY*XMY)*(1.5D0+XMN/SQS)+
     .     XMN*XMY))+1.5D0*SQSP))
     .     +AX*GN53*FFND33*CNS53*(-XK2/S)*(SQSP/TMK2)*(2.D0*S-XMY*XMY
     .     +XMK*XMK-XMY*SQS)
      A3 = A3+A*SQSN*(GN51*FFND31*CNS51*(2.D0+0.5D0/SQSN*(3.D0*XMY-
     .	   (S+XMN*XMY-XMK*XMK+XMY*XMY)/SQS)-(XMK*XMK-XMY*XMY)/2.D0/S)+
     .	   GN52*FFND32*CNS52/SQSN/SQSN*(-0.25D0*(5.D0*S+3.D0*T-6.D0*
     .	   XMN*XMN-2.D0*XMK*XMK-XMY*XMY-3.D0*XK2)-0.5D0*XMY*SQSP+(S-
     .     XMK*XMK+XMY*XMY)*(XMN/4.D0/SQS+XK2/2.D0/S)+XMY/2.D0/SQS*
     .     XK2))+AX*GN53*FFND33*CNS53*XK2*(-2.D0-(XMY*XMY-XMK*XMK)/
     .     (2.D0*S)-XMY/(2.D0*SQS))
      A4 = A4+A*SQSN*(GN51*FFND31*CNS51*(-1.D0+0.5D0/SQSN*(3.D0*XMY-
     .	   (S+XMN*XMY-XMK*XMK+XMY*XMY)/SQS)-(XMK*XMK-XMY*XMY)/2.D0/S)+
     .	   GN52*FFND32*CNS52/SQSN/SQSN*(0.25D0*(S-3.D0*T+2.D0*XMK*XMK+
     .	   XMY*XMY-3.D0*XK2)-0.5D0*XMY*SQSP+(S-XMK*XMK+XMY*XMY)*(XMN/
     .	   4.D0/SQS+XK2/2.D0/S)+XMY/2.D0/SQS*XK2))
     .     +AX*GN53*FFND33*CNS53*XK2*(1.D0-(XMY*XMY-XMK*XMK)/(2.D0*S)
     .     -XMY/(2.D0*SQS))
      A5 = A5+A*(GN51*FFND31*CNS51/2.D0*(1.D0+(XK2-2.D0*SMN2)/TMK2*
     .	   (1+(XMY*XMY-XMK*XMK+XMY*SQS)/S))+GN52*FFND32*CNS52/SQSN*
     .	   ((2.D0*SMN2-XK2)/TMK2*(-0.75D0*SQSP+((S-XMK*XMK+XMY*XMY)*
     .	   (0.75D0+XMN/2.D0/SQS)+XMN*XMY/2.D0)/SQS)-0.75D0*SQSP))
     .     +AX*GN53*FFND33*CNS53*(SQSP/2.D0/TMK2)*(3.D0*(XK2-2.D0*UMY2)
     .     +(XK2-2.D0*SMN2)*(S-XMK*XMK+XMY*XMY+XMY*SQS)/S)
      A6 = A6+A*SQSN*(GN51*FFND31*CNS51/2.D0*((XMK*XMK-XMY*XMY)/S+
     .	   (3.D0*XMY+2.D0*SQSN*(1.D0-XMY/SQS)-(S+XMN*XMY-XMK*XMK+
     .	   XMY*XMY)/SQS)/SQSN)+GN52*FFND32*CNS52/SQSN/SQSN*(-0.75D0*
     .	   (S+T-2.D0*XMN*XMN-XMY*XMY-XK2)+XMY*XMN/2.D0*(-1.D0-
     .	   XMN/SQS)+(S-XMK*XMK+XMY*XMY)*(XMN/4.D0/SQS-XK2/2.D0/S)))
     .     +AX*GN53*FFND33*CNS53*(-1.5D0*UMY2+XMY*SQSP*SQSP/(2.D0*SQS)
     .     + (S-XMK*XMK+XMY*XMY)*(-SQS*SQSP-SMN2)/(2.D0*S))
      ENDIF
C
      IF(XK2.NE.0.D0) GOTO 500
      A5=ZERO
      A6=ZERO
 500  CONTINUE
      IF(IHA.NE.1) GOTO 505
      FFHA=CUT_BORN**2/DSQRT(CUT_BORN**4+(T-XMK*XMK)**2)
      A1=A1*FFHA
      A2=A2*FFHA
      A3=A3*FFHA
      A4=A4*FFHA
      A5=A5*FFHA
      A6=A6*FFHA
 505  CONTINUE
C
c     BB1=-A1+(XMN+XMY)*A3
c     BB2=(0.5D0*XK2-UMY2)*A2+XK2*A5
c     BB3=-(0.5D0*XK2+SMN2)*A2-XK2*A5
c     BB4=A3+A4
c     BB5=-A3+A4
c     BB6=-A3+A6
C
      PI8W=2.D0*W
      SQ2=DSQRT(2.D0)
      ENP=EN+XMN
      ENM=EN-XMN
      EYP=EY+XMY
      EYM=EY-XMY
      WNP=W+XMN
      WNM=W-XMN
      WYP=W+XMY
      WYM=W-XMY
C
      F1=(DSQRT(ENP*EYP)*(WNM*A1+QDK*(A3-A4)+WNM*WYM*A4-XK2*A6))/PI8W
      F2=(DSQRT(ENM*EYM)*(-WNP*A1+QDK*(A3-A4)+WNP*WYP*A4-XK2*A6))/PI8W
      F3=XQI*XKI*DSQRT(EYP/ENP)*(SMN2*A2-XK2*(A2-2.D0*A5)/2.D0+WNP*
     .	 (A3-A4))/PI8W
      F4=XQI*XKI*DSQRT(EYM/ENM)*(-SMN2*A2+XK2*(A2-2.D0*A5)/2.D0+WNM*
     .	 (A3-A4))/PI8W
C      F6=XK0*DSQRT(ENP*EYP)*(A1+WYM*A4-WNM*A6-(W*(XKI2-2.D0*QIDKI)*A2
C     .	 +(QDK*XK0-XQ0*XK2)*(A5-3.D0*A2/2.D0)-(XQ0*WNP-QDK)*(A3-A4))/
C     .	 ENP)/PI8W
C      F5=XK0*DSQRT(ENM*EYM)*(-A1+WYP*A4-WNP*A6-(-W*(XKI2-2*QIDKI)*A2-
C     .	 (QDK*XK0-XQ0*XK2)*(A5-3.D0*A2/2.D0)-(XQ0*WNM-QDK)*(A3-A4))/
C     .	 ENM)/PI8W
C ---- F5 AND F6 INTERCHANGED ACCORDING TO PETR BYDCOVSKY ----
      F5=XK0*DSQRT(ENP*EYP)*(A1+WYM*A4-WNM*A6-(W*(XKI2-2.D0*QIDKI)*A2
     .	 +(QDK*XK0-XQ0*XK2)*(A5-3.D0*A2/2.D0)-(XQ0*WNP-QDK)*(A3-A4))/
     .	 ENP)/PI8W
      F6=XK0*DSQRT(ENM*EYM)*(-A1+WYP*A4-WNP*A6-(-W*(XKI2-2*QIDKI)*A2-
     .	 (QDK*XK0-XQ0*XK2)*(A5-3.D0*A2/2.D0)-(XQ0*WNM-QDK)*(A3-A4))/
     .	 ENM)/PI8W
C
      H1=-DSIN(TH)*DCOS(TH/2.D0)*(F3+F4)/SQ2
      H3=DSIN(TH)*DSIN(TH/2.D0)*(F3-F4)/SQ2
      H2=-DCOS(TH/2.D0)*(F1-F2)*SQ2+H3
      H4=DSIN(TH/2.D0)*(F1+F2)*SQ2-H1
      H5=DCOS(TH/2.D0)*(F5+F6)
      H6=-DSIN(TH/2.D0)*(F5-F6)
C
      EGEQ=(W*W-XMN*XMN)/2.D0/W
      FACT=Q_K/EGEQ*19732.7053D0*19732.7053D0
      SQ2=DSQRT(2.D0)
C
      RT  = DREAL(H1*DCONJG(H1)+H2*DCONJG(H2)+H3*DCONJG(H3)
     .      +H4*DCONJG(H4))/2.D0
      ST  = FACT*RT
C
      RL  = DREAL(H5*DCONJG(H5)+H6*DCONJG(H6))
      SL  = FACT*RL*QP2/(EGCM**2) 
C     
      RTL = DREAL((H1-H4)*DCONJG(H5)+(H2+H3)*DCONJG(H6))/SQ2
      STL = FACT*RTL*DSQRT(QP2)/EGCM
C
      RTLP=-DIMAG((H1-H4)*DCONJG(H5)+(H2+H3)*DCONJG(H6))/SQ2
      STLP= FACT*RTLP*DSQRT(QP2)/EGCM 
C
      RTT = DREAL(H3*DCONJG(H2)-H4*DCONJG(H1))
      STT = FACT*RTT
C
      RETURN
      END
C     ---------------------------------------------------------------
C                       HADRONIC FORM FACTORS
C     ---------------------------------------------------------------
      FUNCTION HFF(PAR,XMASS,STU,IHA)
      IMPLICIT REAL*8 (A-H,O-Z)
      COMMON /MASSS/ XMN,XMK,XMY,XMPI,XMETA,XMZETA,XML
      IF(IHA.EQ.0) GOTO 100
      GOTO (10,20,30,40,50), IHA
 10   HFF=1.D0
      RETURN
 20   HFF=(PAR*PAR-XMASS*XMASS)/(PAR*PAR-STU)
      RETURN
 30   HFF=PAR**4/(PAR**4+(STU-XMASS*XMASS)**2)
      RETURN
 40   XTO=2.D0*(XMN*XMN+XMK*XMK+XMY*XMY)
      XNN=4.D0*PAR**4+(XMN*XMN+XMY*XMY-XMK*XMK)**2-
     .    4.D0*XMY*XMY*XMN*XMN
      HFF=XNN/(XNN+(STU-XMASS**2)*(STU+3.D0*XMASS**2-XTO))
      RETURN
 50   HFF=DEXP(-(STU-XMASS*XMASS)**2/PAR**4)
      RETURN
 100  HFF=1.D0
      RETURN
      END
C     ---------------------------------------------------------------
C                RESONANCE WIDTHS ARE NOT CONSTANT
C     ---------------------------------------------------------------
      FUNCTION WIDTH(STATE,XMRES,FRACPIN,FRACZN,FRACEN,FRACKL,SS,RWIDTH)
      IMPLICIT REAL*8 (A-H,O-Z)
      COMMON /MASSS/ XMN,XMK,XMY,XMPI,XMETA,XMZETA,XML
      WIDTH=RWIDTH*DSQRT(SS)/XMRES*(FRACPIN*RATE(STATE,SS,XMRES,
     .       XMN,XMPI)+FRACZN*RATE(STATE,SS,XMRES,XMN,XMZETA)+
     .       FRACEN*RATE(STATE,SS,XMRES,XMN,XMETA)+FRACKL*RATE(STATE,
     .       SS,XMRES,XML,XMK))
      RETURN
      END
C
      FUNCTION RATE(STATE,SS,XMRE,XMNU,XMME)
      IMPLICIT REAL*8 (A-H,O-Z)
      ALPHO=410.D0
      ABSQIR=DSQRT((XMRE**2-XMNU**2+XMME**2)**2/(4.D0*XMRE**2)-XMME**2)
      ABSQI=DSQRT((SS-XMNU**2+XMME**2)**2/(4.D0*SS)-XMME**2)
      RATE=(ABSQI/ABSQIR)**(2.D0*STATE+1.D0)*DEXP(-ABSQI**2/(3.D0
     .     *ALPHO**2))/DEXP(-ABSQIR**2/(3.D0*ALPHO**2))
      RETURN
      END
C     ---------------------------------------------------------------
C     			FUNCTION DIPOLE AND POLE 
C     	      BEWARE, XMASS IS NOT SQUARED IN FUNCTION DIPOLE
C     ---------------------------------------------------------------
      FUNCTION DIPOLE(XLAMBDA,XK2,XMASS)
      DOUBLE PRECISION DIPOLE,XMASS,XLAMBDA,XK2
      DIPOLE=1.D0/(1.D0-XLAMBDA*XK2/XMASS)
      RETURN
      END
C
      FUNCTION POLE(XLAMBDA,XMASS,XK2)
      DOUBLE PRECISION POLE,XMASS,XLAMBDA,XK2
      POLE=(XLAMBDA**2-XMASS**2)/(XLAMBDA**2-XK2)
      RETURN
      END

C     ---------------------------------------------------------------
C                 Galery of electromagnetic form factors
C     Note that the form factors use positif QQ2 instead of neg. XK2!
C     ---------------------------------------------------------------
C
C     ---------------------------------------------------------------
C                    dipole fit proton form factor  (1)
C     ---------------------------------------------------------------
      SUBROUTINE FF_PROTON1(QQ2,FFN1,FFN2,FFNEUT)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DATA XMP,XMNN,XKP,XKN /938.27231,939.56563,1.79284739D0,-1.913D0/
      GEP=(1.D0/(1.D0+QQ2/0.71D6))**2
      FFN1=GEP*((1.D0+QQ2/4.D0/XMP/XMP*(1.D0+XKP))/(1.D0+QQ2/4.D0/XMP/
     .	   XMP))
      FFN2=GEP*(1.D0/(1.D0+QQ2/(4.D0*XMP*XMP)))
      FFNEUT=GEP
      RETURN
      END

C     ---------------------------------------------------------------
C                 Gari & Krumpellman proton form factor   (2) 
C     ---------------------------------------------------------------
      SUBROUTINE FF_PROTON2(QQ2,FFN1,FFN2,FFNEUT)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DATA XMP,XMRHO,XMOME,XKP,XKN,XK_IV,XK_IS,GRHO_FRHO,XK_RHO,
     .     GOME_FOME,XK_OME,XLA_1,XLAD_1,XLA_2,XLA_QCD / 938.27231,
     .     770.D0,781.94D0,1.793D0,-1.913D0,3.706D0,-0.12D0,0.631D0,
     .     3.3D0,0.658D0,0.4D0,0.863D3,1.21D3,2.1D3,0.33D3 /
      QTIL2=QQ2*DLOG((XLA_2**2+QQ2)/XLA_QCD**2)/DLOG(XLA_2**2/
     .      XLA_QCD**2)
      F1_ALP=(XLA_1**2/(XLA_1**2+QTIL2))*(XLA_2**2/(XLA_2**2+QTIL2))
      F2_ALP=((XLA_1**2/(XLA_1**2+QTIL2))**2)
     .       *(XLA_2**2/(XLA_2**2+QTIL2))
      F1_D=(XLAD_1**2/(XLAD_1**2+QTIL2))*(XLA_2**2/(XLA_2**2+QTIL2))
      F2_D=((XLAD_1**2/(XLAD_1**2+QTIL2))**2)
     .       *(XLA_2**2/(XLA_2**2+QTIL2))
      F1_IV=GRHO_FRHO*(XMRHO**2/(XMRHO**2+QQ2))*F1_ALP+(1-GRHO_FRHO)*
     .      F1_D
      F1_IS=GOME_FOME*(XMOME**2/(XMOME**2+QQ2))*F1_ALP+(1-GOME_FOME)*
     .      F1_D
      F2_IV=(XK_RHO*GRHO_FRHO*(XMRHO**2/(XMRHO**2+QQ2))*F2_ALP+
     .      (XK_IV-XK_RHO*GRHO_FRHO)*F2_D)/XK_IV
      F2_IS=(XK_OME*GOME_FOME*(XMOME**2/(XMOME**2+QQ2))*F2_ALP+
     .      (XK_IS-XK_OME*GOME_FOME)*F2_D)/XK_IS
      FFN1=(F1_IS+F1_IV)/2.D0
      FFN2=(XK_IS*F2_IS+XK_IV*F2_IV)/2.D0/XKP
      FFNEUT=(XK_IS*F2_IS-XK_IV*F2_IV)/2.D0/XKN
      RETURN
      END

C     ---------------------------------------------------------------
C                      williams kaon form factor  (1)
C     ---------------------------------------------------------------
      SUBROUTINE FF_KAON1(QQ2,FFKP)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      COMPLEX*16 FFKP
      DATA XMRHO,XMOME,XMPHI,WRHO,WOME,WPHI / 770.D0,781.94D0, 
     .     1019.413D0,151.5D0,8.43D0,4.43D0 /
      FFKP=0.5D0*XMRHO**2/DCMPLX(XMRHO**2+QQ2,-XMRHO*WRHO)+
     .     0.17D0*XMOME**2/DCMPLX(XMOME**2+QQ2,-XMOME*WOME)+
     .     0.33D0*XMPHI**2/DCMPLX(XMPHI**2+QQ2,-XMPHI*WPHI)
      RETURN
      END 

C     ---------------------------------------------------------------
C                       cardareli kaon form factor   (2)
C     ---------------------------------------------------------------
      SUBROUTINE FF_KAON2(QQ2,FFKP)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      COMPLEX*16 FFKP
      FFKP1=0.398D0/(1.D0+QQ2/0.642D3**2)
     .      +0.602D0/(1.D0+QQ2/1.386D3**2)**2
      FFKP=DCMPLX(FFKP1,0.D0)
      RETURN
      END

C     ---------------------------------------------------------------
C             K*+K+ form factor (1)Williams, (2) Muenz
C     ---------------------------------------------------------------
      SUBROUTINE FF_KSTARP(QQ2,MOD_FFKS,FFKS)
      IMPLICIT REAL*8 (A-H,O-Z)
      IF (MOD_FFKS .EQ. 1) THEN
         FFKS=1.D0/(1.D0+QQ2/1.D6/(0.8344D0*0.8344D0))**1.3D0
      ELSE IF (MOD_FFKS .EQ. 2) THEN
         FFKS=DABS(QQ2/1.D6-2.68D0)/2.68D0/(1.D0+QQ2/1.D6/
     .            (1.5321D0*1.5321D0))**3.75D0
      ELSE
C         WRITE(11,*) 'Error in the model of the K* and K1 form factors'
         STOP
      ENDIF
      RETURN
      END

C     ---------------------------------------------------------------
C             K*0K0 form factor (1)Williams, (2) Muenz
C     ---------------------------------------------------------------
      SUBROUTINE FF_KSTAR0(QQ2,MOD_FFKS,FFKS)
      IMPLICIT REAL*8 (A-H,O-Z)
      IF (MOD_FFKS .EQ. 1) THEN
         FFKS=1.D0/(1.D0+(QQ2/1.D6)/(0.8046D0*0.8046D0))**0.9D0
      ELSE IF (MOD_FFKS .EQ. 2) THEN
         FFKS=1.D0/(1.D0+(QQ2/1.D6)/(1.1945D0*1.1945D0))**2.05D0
      ELSE
C         WRITE(11,*) 'Error in the model of the K* and K1 form factors'
         STOP
      ENDIF
      RETURN
      END

C     --------------------------------------------------------------
C                 Lambda and Transition Form Factors 
C     Williams & Small, Phys. Rev. C55, 882 (1997); C53, 1580 (1996)
C                 Last modified July 3th. 1999. TM.
C     --------------------------------------------------------------
      SUBROUTINE FF_LAMBDA1(MODEL,QQ2,F2LAM,F2TRS)
      IMPLICIT REAL*8 (A-H,O-Z)
      DATA XMRHO,XMOME,XMPHI,XMPHIS/776.,784.,1020.,1680./,
     .     WRHO,WOME,WPHI,WPHIS/153.,10.,4.,150./,XLAM/2000./
C
      XMRHO2=XMRHO*XMRHO
      WRHO2=WRHO*WRHO 
      GOTO (10,20,30), MODEL
 10   ALPHA=0.5D0
      BETA=0.5D0
      GAMMA=-0.5D0
      GOTO 40
 20   ALPHA=0.D0
      BETA=1.D0
      GAMMA=1.D0
      GOTO 40
 30   ALPHA=0.D0
      BETA=-3.3D0
      GAMMA=1.6D0
 40   CONTINUE
      F2GM=DSQRT(XMRHO2*(XMRHO2+WRHO2)/((-QQ2-XMRHO2)**2+
     .     (XMRHO*WRHO)**2))*(XLAM*XLAM/(XLAM*XLAM+DABS(QQ2)))**2
      F2LAM=F2GM*(1.D0-ALPHA*QQ2/(XMOME*XMOME+QQ2)+BETA*(-QQ2/(XMPHI*
     .      XMPHI+QQ2)))
      F2TRS=F2GM*(1.D0-GAMMA*QQ2/(XMRHO2+QQ2))
      RETURN
      END

C     --------------------------------------------------------------
C         Lambda and Transition Form Factors, from Kim & Goeke
C                      Phys.Rev. D53, 4013 (1996)
C     --------------------------------------------------------------
      SUBROUTINE FF_LAMBDA2(QQ2,F2LAM,F2TRS)
      IMPLICIT REAL*8 (A-H,O-Z)
      F2LAM=DEXP(-0.2872D0*DSQRT(QQ2/1.D6))/((1.D0+QQ2/1.D6/1.1509D0*
     .      1.1509D0)**3.0235D0)
      F2TRS=F2LAM
      RETURN
      END

C     --------------------------------------------------------------
C                Sigma0 Form Factor, from Kim & Goeke
C     --------------------------------------------------------------
      SUBROUTINE FF_SIGMA0(QQ2,FFS0)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      FFS0=DEXP(-0.2344D0*DSQRT(QQ2/1.D6))/(1.D0+QQ2/1.D6/
     .         (1.1155D0*1.1155D0))**3.0043D0
      RETURN
      END

C     --------------------------------------------------------------
C                Sigma+ Form Factors, from Kim & Goeke
C     --------------------------------------------------------------
      SUBROUTINE FF_SIGPLUS(QQ2,FFSP1,FFSP2)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      XMY=1189.37D0
      GESP=1.D0/(1+(QQ2/1.D6)/(0.948D0*0.948D0))**2.8478D0
      GMSP=2.432D0*DEXP(-0.2229D0*DSQRT(QQ2/1.D6))/(1.D0+QQ2/1.D6/
     .     (1.0929D0*1.0929D0))**2.9398D0
      FFSP1=(GESP+QQ2/(4.D0*XMY*XMY)*GMSP)/(1.D0+QQ2/(4.D0*XMY*XMY))
      FFSP2=(GMSP-GESP)/1.432D0/(1.D0+QQ2/(4.D0*XMY*XMY))
      RETURN
      END

C     --------------------------------------------------------------
C                Sigma- Form Factors, from Kim & Goeke
C     --------------------------------------------------------------
      SUBROUTINE FF_SIGMINS(QQ2,FFSM1,FFSM2)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      XMY=1189.37D0
      GESM=1.D0/(1+(QQ2/1.D6)/(0.8583D0*0.8583D0))**2.1525D0
      GMSM=0.9507D0/(1.D0+QQ2/1.D6/(0.8809D0*0.8809D0))**2.3817D0
      FFSM1=(GESM+QQ2/(4.D0*XMY*XMY)*GMSM)/(1.D0+QQ2/(4.D0*XMY*XMY))
      FFSM2=(GMSM-GESM)/(-0.0493D0)/(1.D0+QQ2/(4.D0*XMY*XMY))
      RETURN
      END

      SUBROUTINE FF_KZERO(QQ2,MOD_FFK0,FFKP)
      PARAMETER(NY=51)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      COMPLEX*16 FFKP
      DOUBLE PRECISION RY(NY),FINP(NY),FINZ(NY),Q2FINP(NY),Q2FINZ(NY)
      DATA XMRHO,XMOME,XMPHI,XMPHIS /768.1D0,781.95D0,1019.413D0,
     .                               1680.D0/
      IF (MOD_FFK0 .EQ. 1) THEN
         XITO=4.5D3
         FFK=(-XMOME*XMOME/(XMOME*XMOME+QQ2)+XMPHI*XMPHI/(XMPHI*
     .        XMPHI+QQ2))/3.D0*XITO*XITO/(XITO*XITO+QQ2)
      ELSE IF (MOD_FFK0 .EQ. 2) THEN
         OPEN(UNIT=9,FILE='kaon3.dat',STATUS='UNKNOWN')
         READ(9,320) LINES
 320     FORMAT(///)
         DO I=1,NY
            READ(9,*) RY(I),FINP(I),FINZ(I),Q2FINP(I),Q2FINZ(I)
         ENDDO
         CLOSE(9)
         CALL INT1D(NY,RY,FINZ,QQ2/1.D6,FFKOUT)
         FFK=FFKOUT
      ELSE
C         WRITE(11,*) 'Error in the model of the kaon form factor'
         STOP
      ENDIF
      FFKP=DCMPLX(FFK,0.D0)
      RETURN
      END

C     --------------------------------------------------------------
C     Subroutine for the interpolation of the K-Zero FF from H. Ito
C     Data are in kaon3.dat
C     **  May 11, 1995 TM
C     --------------------------------------------------------------
      SUBROUTINE INT1D(NY,RY,FIN,Z,FOUT)
      IMPLICIT REAL*8(A-H,O-Z)
      DIMENSION RY(NY),FIN(NY),DY(4),SY(4)
      IC=NY+1
C
      FOUT=0.D0
      DO 10 N10=1,NY
         IF(IC.NE.NY+1)GO TO 10
         X=RY(N10)
         IF(X.GE.Z)IC=N10
 10   CONTINUE
      IF(IC.EQ.NY+1)GO TO 99
C     
      NL=IC-2
      NW=4
      IF(IC.EQ.1)NL=1
      IF(IC.EQ.2)NL=1
      NU=NL+3
C
      IF(IC.EQ.NY)NL=NY-3
      IF(IC.EQ.NY)NU=NY
C
      DO N20=1,4
         DY(N20)=0.D0
         SY(N20)=0.D0
      ENDDO
C     
      DO N30=NL,NU
         DY(N30-NL+1)=RY(N30)
         SY(N30-NL+1)=FIN(N30)
      ENDDO
C     
      CALL SFTP24(NW,DY,SY,Z,FTP24)
      FOUT=FTP24
   99 CONTINUE
      RETURN
      END
C     ----------------------------------
      SUBROUTINE SFTP24(NW,D,S,X,FTP24)
      IMPLICIT REAL*8(A-H,O-Z)
      DIMENSION D(4),S(4)
C
      XZ1=D(1)
      XZ2=D(2)
      YZ1=S(1)
      YZ2=S(2)
      FTP24=(  (X-XZ2)*YZ1 - (X-XZ1)*YZ2  )/(XZ1-XZ2)
      IF(NW.EQ.2)GO TO 99
C
      X01=X-D(1)
      X02=X-D(2)
      X03=X-D(3)
      X04=X-D(4)
C
      X12=D(1)-D(2)
      X13=D(1)-D(3)
      X14=D(1)-D(4)
C
      X23=D(2)-D(3)
      X24=D(2)-D(4)
C
      X34=D(3)-D(4)
C
      Y1=S(1)
      Y2=S(2)
      Y3=S(3)
      Y4=S(4)
C
      Y=Y1*X02*X03*X04/X12/X13/X14 -Y2*X01*X03*X04/X12/X23/X24
     . +Y3*X01*X02*X04/X13/X23/X34 -Y4*X01*X02*X03/X14/X24/X34
      FTP24=Y
   99 RETURN
      END
C
C     --------------------------------------------------------------
C           Woods Saxon's Form Factors for the resonances
C     --------------------------------------------------------------
      FUNCTION FF_WOODS_SAXON(QQ2,Q02,AW2)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      FF_WOODS_SAXON=(1.D0+DEXP(-Q02/AW2))/
     .               (1.D0+DEXP((QQ2/1.D6-Q02)/AW2))
      RETURN
      END
C
C     --------------------------------------------------------------
C                The N* Form Factors from Gregor Penner
C     --------------------------------------------------------------
      SUBROUTINE FF_GREGOR(QQ2,CUT,FFN)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DATA XMRHO,XMOMEGA /768.1D0,781.95D0/
      XMV=(XMRHO+XMOMEGA)/2.D0
      F_CUT = (CUT**4+XMV**4/4.D0)/(CUT**4+(QQ2+XMV*XMV/2.D0)**2)
      FFN = F_CUT*(1.D0-0.91D0*QQ2/(5.01D0*0.718D0*(XMRHO**2+QQ2))-
     .      0.03D0*0.91D0*QQ2/(16.98D0*0.718D0*(XMOMEGA**2+QQ2)))
      RETURN
      END
