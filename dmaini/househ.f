      SUBROUTINE TLS (A,B,AUX,IPIV,EPS,X,NA,NB,NX,NIPIV)   
C   
C CERN PROGLIB# E230    TLS             .VERSION KERNFOR  2.06  740511  
C ORIG. 11/05/74 W.HART+W.MATT  
C   
C.  
C.  SUBROUTINE TLS          LINEAR LEAST SQUARES          HART/MATT   
C.        HOUSEHOLDER ROTATIONS ARE USED TO ROTATE A TO TRIANGULAR  
C.        FORM THEN X OBTAINED THRO BACK SUBSTITUTION.  
C.        IDENTICAL IN OPERATION TO LIB ROUTINE HLS BUT WITH TC ARRAY   
C.        DEFINITIONS. SEE HLS WRITE UP (D520) FOR FURTHER DETAILS. 
C.  ARGUMENTS   
C.        A    M BY N CONSTRAINT/COEFFICIENT MTX (DESTROYED),   
C.             CONSTRAINTS FIRST.   
C.        B    M BY L R.H.S. MTX (DESTROYED)    
C.        AUX  AUXILIARY STORAGE ARRAY OF DIM=MAX(N,L)+N    
C.             ON RETURN THE FIRST L LOCS CONTAIN THE RESULTING 
C.             SUM OF SQUARES   
C.        IPIV INTEGER VECTOR (DIM=N) WHICH RETURNS THE COLUMN  
C.             INTERCHANGES IN MTX A    
C.        EPS  INPUT PARAM SPECIFYING A TOLERANCE FOR DETERMINING   
C.             THE RANK OF MTX A    
C.        X    N BY L SOLUTION MTX  
C.        THE FOLLOWING ARE TRANSMITTED THRO COMMON /TLSDIM/  ...   
C.        M1   NUMBER OF CONSTRAINT EQUATIONS = 0   
C.        M    TOTAL NUMBER OF EQUATIONS    
C.        N    NUMBER OF UNKNOWNS   
C.        L    NUMBER OF SYSTEMS TO BE SOLVED   
C.        IER  OUTPUT PARAM GIVING RANK OF MTX A    
C.  
C.-------------------------------------------------------------------   
C   
      IMPLICIT  REAL*8 (A-H,O-Z)     
      COMMON /TLSDIM/ M1,M,N,L,IER  
c*****
c      COMMON /SLATE/ BETA,F,G,H,I,IB,ID,IEND,II,IST,J,JA,JB,JD,JK,JL    
c     +              ,TOL,JX,J1,K,KPIV,KR,KST,KT,K1,K2,K3,LV,NK,NR,N1    
c     +              ,PIV,PIVT,SIG,JST,DUM(5)    
c*****      
      DIMENSION A(NA),B(NB),X(NX),IPIV(NIPIV),AUX(2*NX)   

      write(6,*)' jumped into tls'

C   
C     ERROR TEST    
      kpiv=0
      write(6,*)'M=',M,'N=',N
      IF(M-N.lt.0) goto 31 
      IF(M-N.ge.0) goto 1 
C   
C     CALCN OF INITIAL VECTORS S(K),T(K) IN LOCS AUX(1),AUX(K1) 
    1 PIV=0.    
      K1 = MAX0(N,L)    
      IST = 0   
      K2 = K1 + 1   
      DO 4 K=1,N    
      IPIV(K)=K 
      IST = IST + 1 
      IF (M .EQ. 1) GO TO 40    
      CALL TLPIV(A,IST,B,N,L,M,H,G,NA,NB)    
      GO TO 41  
   40 G=A(IST)*B(1) 
      H=A(IST)*A(IST)   
   41 AUX(K)=H  
      AUX(K2) = G   
      PIVT = G*G/H  
      IF(PIVT-PIV.le.0) goto 4
      PIV = PIVT    
      KPIV=K    
    4 K2 = K2 + 1   
      IF (M .EQ. 1) GO TO 2 
      CALL TLSMSQ(B,1,L,M,F,NB)   
      GO TO 7   
    2 F=B(1)*B(1)   
C   
C     ERROR TEST    
    7 IF(PIV.le.0) goto 31    
      IF(PIV.gt.0) goto 5    
C   
C     DEFINE TOLERANCE FOR CHECKING RANK OF A   
    5 TOL = EPS*EPS 
      IER = 1   
C   
C   
C     DECOMPOSITION LOOP    
      IST = -N  
      JB = 1 - L    
      DO 21 K=1,N   
      IF(EPS.EQ.0.) GO TO 12    
      IF(EPS.GT.0.) GO TO 11    
      IF(PIV.GT.TOL) GO TO 12   
      GO TO 34  
   11 IF(F.GT.TOL*dble(M-K+1)) GO TO 12    
   34 IF(K.NE.1) GO TO 32   
      TOL = 0.  
      IER = -1  
   12 F = F - PIV   
      IST = IST + N + 1 
      JB = JB + L   
      LV = M-K+1    
      I=KPIV-K  
      IF(I.le.0) goto 8    
      IF(I.gt.0) goto 6    
C   
C     INTERCHANGE K-TH COLUMN OF A WITH KPIV-TH IN CASE KPIV.GT.K   
    6 H=AUX(K)  
      AUX(K)=AUX(KPIV)  
      AUX(KPIV)=H   
      K2 = K1 + K   
      K3 = K1 + KPIV    
      G = AUX(K2)   
      AUX(K2) = AUX(K3) 
      AUX(K3) = G   
      JA = IST  
      JD = IST + I  
      NR = M - K + 1    
      CALL TLSWOP(A,IST,A,JD,N,NR,NA,NA)    
C   
C     COMPUTATION OF PARAMETER SIG  
C     GENERATION OF VECTOR UK IN K-TH COLUMN OF MATRIX A AND OF 
C     PARAMETER BETA    
    8 CALL TLUK(A,IST,N,LV,SIG,BETA,NA)   
C   
      J = K1 + K    
      AUX(J)=-SIG   
C   
C     SAVE INTERCHANGE INFORMATION  
      IPIV(KPIV)=IPIV(K)    
      IPIV(K)=KPIV  
      IF(K-N.ge.0) goto 19   
C   
C     TRANSFORMATION OF MATRIX A    
      NK = N - K    
      CALL TLSTEP(A,IST,A,IST+1,N,N,LV,NK,BETA,NA,NA)   
C   
C     TRANSFORMATION OF RIGHT HAND SIDE MATRIX B    
   19 IB = (K-1) * L +1 
      CALL TLSTEP(A,IST,B,IB,N,L,LV,L,BETA,NA,NB)   
      IF(K-N.ge.0) goto 21   
C   
C     UPDATING OF S(K),T(K) ELEMENTS STORED IN AUX  
      PIV = 0.  
      KPIV = K + 1  
      J1 = KPIV 
      ID = IST  
      K2 = K1 + J1  
      DO 18 J=J1,N  
      ID = ID+1 
      H=AUX(J) - A(ID) * A(ID)  
      AUX(J)=H  
      G = AUX(K2) - A(ID) * B(JB)   
      AUX(K2) = G   
      PIVT = G*G/H  
      IF(PIVT-PIV.le.0) goto 18  
      PIV = PIVT    
      KPIV=J    
   18 K2 = K2 + 1   
   21 CONTINUE  
      GO TO 20  
C     END OF DECOMPOSITION LOOP 
C   
C   
C     RANK OF MATRIX LESS THAN N, ZERO X,S AND BACK SUBSTITUTE  
   32 KR = K - 1    
      KT = KR   
      IER = KR  
      JK = KR * L + 1   
      JL = N * L    
      DO 15 JX=JK,JL    
   15 X(JX) = 0.    
      GO TO 16  
C   
C     BACK SUBSTITUTION AND BACK INTERCHANGE    
   20 IER = N * IER 
      KT = N    
      JK = (N-1) * L    
      K = K1 + N    
      PIV=1./AUX(K) 
      DO 22 K=1,L   
      JK = JK + 1   
   22 X(JK) = PIV * B(JK)   
      KR = N - 1    
C   
   16 IF(KR.le.0) goto 26    
      JST = KR * (N+1) + 2  
      DO 25 J=1,KR  
      JST = JST - N - 1 
      IEND = (KR-J+1) * N   
      K = K1 + KR - J + 1   
      PIV=1./AUX(K) 
      KST=K-K1  
      ID=IPIV(KST)-KST  
      KST = (KR-J) * L  
      DO 25 K=1,L   
      KST = KST + 1 
      H=B(KST)  
      II = KST  
      DO 24 I = JST,IEND    
      II = II + L   
   24 H = H - A(I) * X(II)  
      II = KST + ID * L 
      X(KST) = X(II)    
      X(II)=PIV*H   
   25 CONTINUE  
C   
C     COMPUTATION OF LEAST SQUARES  
   26 IST = KT * L  
      N1 = KT + 1   
      DO 29 J=1,L   
      IST = IST + 1 
      H=0.  
      JA = IST  
      IF(M-KT.le.0) goto 29  

      NR = M - N1 + 1   
      IF(NR.EQ.1) GO TO 28  
      CALL TLSMSQ(B,IST,L,NR,H,NB)    
      GO TO 29  
   28 H = B(IST)*B(IST) 
C   
   29 AUX(J)=H  
      RETURN    
C   
C     ERROR RETURN IN CASE OF ZERO-MATRIX A OR M.LT.N   
   31 IER = -1001   
      RETURN    
      END   
      
      
      
      SUBROUTINE TLSMSQ (B,IB,L,M,F,NB)   
C   
C CERN PROGLIB# E230    TLSMSQ          .VERSION KERNFOR  2.06  740511  
C ORIG. 11/05/74 WH+WM  
C   
C.  SUBROUTINE TLSMSQ (B,L,M,F) 
C.  
C.       CALCULATES THE SUM OF SQUARES OF RESIDUALS.    
C.  
C.-------------------------------------------------------------------   
C   
C   
C
      IMPLICIT REAL*8 (A-H,O-Z)     
c*****
c      real*4 DUMMY1
c      COMMON /SLATE/ DUMMY1(51),I,JB   
c*****      
      DIMENSION      B(NB)   
c      dimension B(1) ! MK
C   
      F  = 0.   
      JB = 1    
      DO           10        I=1,M  
      F  = F + B(JB+(IB-1))*B(JB+(IB-1))  
   10 JB = JB + L   
C   
      RETURN    
      END   
      SUBROUTINE TLPIV(A,IA,B,IASEP,IBSEP,M,H,G,NA,NB)   
C   
C CERN PROGLIB# E230    TLPIV           .VERSION KERNFOR  2.06  740511  
C ORIG. 11/05/74 WH+WM  
C   
C.  SUBROUTINE TLPIV (A,B,IASEP,IBSEP,M,H,G)    
C.  
C.        CALCULATES PIVOTING FACTORS FOR TLS   
C.  
C.-------------------------------------------------------------------   
C   
C   
C
      IMPLICIT REAL*8 (A-H,O-Z)     
c*****
c      real*4 DUMMY2
c      COMMON /SLATE/ DUMMY2(50),I,JA,JB    
c*****      
      DIMENSION A(NA),B(NB)   
c      dimension A(1),B(1) ! MK
C   
      H=0.0 
      G=0.0 
      JA=1  
      JB=1  
      DO 2 I=1,M    
      G=G+A(JA+(IA-1))*B(JB)   
      H=H+A(JA+(IA-1))*A(JA+(IA-1))   
      JA=JA+IASEP   
    2 JB=JB+IBSEP   
      RETURN    
      END   
      SUBROUTINE TLSTEP (A,I1,B,I2,IASEP,IBSEP,NR,NC,BETA,NA,NB)    
C   
C CERN PROGLIB# E230    TLSTEP          .VERSION KERNFOR  2.06  740511  
C ORIG. 11/05/74 WH+WM  
C   
C.  SUBROUTINE TLSTEP (A,B,IASEP,IBSEP,NR,NC,BETA)  
C.  
C.       PERFORMS AN ORTHOGONAL TRANSFORMATION. 
C.  
C.-------------------------------------------------------------------   
C   
C   
C
	IMPLICIT REAL*8 (A-H,O-Z)     
c        real*4 DUMMY3
c      COMMON /SLATE/ DUMMY3(46),H,I,IB,J,JA,JB 
      DIMENSION      A(NA), B(NB) 
c      dimension A(1),B(1) ! MK
C   
      IB = 0    
      DO           30        J=1,NC 
      IB = IB + 1   
      H  = 0.   
      JA = 1    
      JB = IB   
C   
      DO           10        I=1,NR 
      H  = H + A(JA+(I1-1)) * B(JB+(I2-1))    
      JA = JA +IASEP    
   10 JB = JB + IBSEP   
C   
      H  = H * BETA 
      JA = 1    
      JB = IB   
      DO           20        I=1,NR 
      B(JB+(I2-1)) = B(JB+(I2-1)) - A(JA+(I1-1)) * H 
      JA = JA +IASEP    
   20 JB = JB + IBSEP   
   30 CONTINUE  
C   
      RETURN    
      END   
      SUBROUTINE TLSWOP (A,IA,AD,IAD,N,NR,NA,NAD) 
C   
C CERN PROGLIB# E230    TLSWOP          .VERSION KERNFOR  2.06  740511  
C ORIG. 11/05/74 WH+WM  
C   
C.  SUBROUTINE TLSWOP (A,AD,N,NR)   
C.  
C.       INTERCHANGES THE FIRST ROW  OF MATRICES A AND AD.  
C.  
C.-------------------------------------------------------------------   
C   
C   
C
	IMPLICIT REAL*8 (A-H,O-Z)     
c*****
c      real*4 DUMMY4
c      COMMON /SLATE/ DUMMY4(49),H,I,JA 
c*****      
      DIMENSION      A(NA), AD(NAD)    
c      dimension A(1),AD(1) ! MK
C   
      JA = 1    
      DO           10        I=1,NR 
      H  = A(JA+(IA-1))    
      A(JA+(IA-1))  = AD(JA+(IAD-1))   
      AD(JA+(IAD-1)) = H    
   10 JA = JA + N   
C   
      RETURN    
      END   
      SUBROUTINE TLUK (A,IA,IASEP,NR,SIG,BETA,NA) 
C   
C CERN PROGLIB# E230    TLUK            .VERSION KERNFOR  2.06  740511  
C ORIG. 11/05/74 WH+WM  
C   
C.  SUBROUTINE TLUK (A,IASEP,NR,SIG,BETA)   
C.  
C.       COMPUTE TRANSFORMATION QUANTITIES. 
C.       TLUK HAS BEEN MODIFIED FOR KINEMATICS. 
C.  
C.-------------------------------------------------------------------   
C   
C   
C
	IMPLICIT REAL*8 (A-H,O-Z)     
c*****
c      real*4 DUMMY5
c      COMMON /SLATE/ DUMMY5(50),I,JA,LL    
c*****      
      DIMENSION A(NA)    
c      dimension A(1) ! MK
C-- 
C   
C--      COMPUTE MODULUS OF A GIVEN ROW IN A MATRIX AND FIND LAST   
C--      NON-ZERO ELEMENT IN THAT ROW.  
C   
      SIG= 0.   
      JA = 1    
      LL = 0    
C   
      DO           10        I=1,NR 
      IF     (A(JA+(IA-1)).EQ.0.)             GO TO     10 
      LL = I    
      SIG= SIG + A(JA+(IA-1))* A(JA+(IA-1))   
   10 JA = JA + IASEP   
C   
C--      FOR A ZERO ROW RETURN. 
C   
      NR = LL   
      IF     (NR.EQ.0)                 RETURN   
C-- 
C--      OTHERWISE TAKE THE MODULUS WITH SIGN OF FIRST ELEMENT OF THAT  
C--      ROW. REDEFINE THAT FIRST ARGUMENT AND VALUE OF BETA.   
C   
      SIG  = SIGN (SQRT (SIG),A(IA)) 
      BETA = A(IA) + SIG 
      A(IA) = BETA   
      BETA = 1. / (SIG * BETA)  
      RETURN    
      END   

	       
      SUBROUTINE TLERR (A,X,AUX,IPIV,NA,NX,NIPIV)   
C   
C CERN PROGLIB# E230    TLERR           .VERSION KERNFOR  2.06  740511  
C ORIG. 11/05/74 WH+WM  
C   
C.  SUBROUTINE TLERR          L.S. ERROR MATRIX             HART/MATT   
C.  
C.        CALCULATES INVERSE OF (A-TRANSPOSED*A) DIRECTLY FROM THE  
C.        TRIANGULARISED TRANSFORM OF A.    
C.  ARGUMENTS   
C.        A,X,AUX,IPIV,(M1,M,N,L,IER) DEFINED AS FOR TLSC WITH X THE    
C.        COVARIANCE MATRIX. MATRIX X MAY OVERWRITE MATRIX A.   
C.  REMARKS 
C.        CONSTRAINED AND UNCONSTRAINED VERSION COMBINED    
C.  
C.-------------------------------------------------------------------   
C   
C   
C
      IMPLICIT REAL*8 (A-H,O-Z)     

      COMMON /TLSDIM/ M1,M,N,L,IER  

c****** 
c      real*4 DUMMY6
c      COMMON /SLATE/ BETA,H,I,IA,ID,IEND,II,IK,IL,IST,J,JA,JD,JK,JL 
c     +              ,K,KN,KS,K1,LV,NK,PIV,SIG,DUMMY6(26)   
c******      

      DIMENSION      A(NA), AUX(2*NX), IPIV(NIPIV), X(NX*NX)    
C   
      K1 = MAX0 (N,L)   
      IF     (IABS (IER).EQ.N)         GO TO     5  
C-- 
C--      COMPLETE HOUSEHOLDER TRANSFORMATION IF IER LESS THAN N.    
C   
      IST = IER * (N+1) + 1 
      KS  = IER + 1 
C   
      DO           4         K=KS,N 
      LV = M - K + 1    
C   
C--      GENERATE VECTOR UK AND TRANSFORMATION PARAMETER BETA.  
C   
      CALL TLUK (A,IST,N,LV,SIG,BETA,NA)  
      J = K1 + K    
      AUX(J) = -SIG 
      IF     (K.EQ.N)        GO TO     4    
C   
C--      TRANSFORMATION OF MATRIX A.    
C-- 
      NK = N - K    
      IF (LV.EQ.1)                     GO TO     2  
      CALL TLSTEP (A,IST,A,IST+1,N,N,LV,NK,BETA,NA,NA)  
      GO TO        4    
    2 DO           3         J=1,NK 
      JST = IST + J 
    3 A(JST) = A(JST)*(1.-BETA*A(IST)**2)   
      IST = IST + N + 1 
    4 IPIV(K) = K   
C   
C   
C--      COMPUTE X FROM A AND DIAGONAL ELEMENTS OF A-TRANSPOSED.    
C   
    5 DO           40        K=1,N  
      KN = N-K+1    
      IA = (KN-1)*( N+1)+1  
      IK = KN *N    
      IL = N*N - K + 1  
      II = KN + K1  
      PIV=1./AUX(II)    
      ID = IPIV(KN)-KN  
      JA = IA+1 
      JK = IK   
      JL = IL   
C   
      DO           20        J=1,K  
      H=0.  
      IF     (J.EQ.K .AND. J.LE.N-M1)            H = PIV    
      IF     (K.EQ.1)                            GO TO     15   
      II = JK   
C   
      DO           10        I=JA,IK    
      II=II+N   
   10 H = H-A(I)*X(II)  
C   
   15 H = H*PIV 
      X(JL) = H 
      JK = JK - 1   
   20 JL = JL - N   
C   
C--      COMPLETE SYMMETRIC PART.   
C   
      IF     (K.EQ.1)                  GO TO     40 
      JL = IA   
      DO           25        J=JA,IK    
      JL = JL + N   
   25 X(J) = X(JL)  
C   
C--      INTERCHANGE OF ROWS AND COLUMNS ALREADY FINISHED.  
C   
      IF     (ID.EQ.0)                 GO TO     40 
      DO           30        J=IA,IL,N  
      II = J + ID   
      H  = X(II)    
      X(II) = X(J)  
   30 X(J)  = H 
C   
      ID = ID*N 
      DO           35        J=IA,IK    
      II = J + ID   
      H  = X(II)    
      X(II) = X(J)  
   35 X(J)  = H 
C   
   40 CONTINUE  
C   
      RETURN    
      END   

