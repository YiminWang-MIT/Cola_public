C
C     Created 1994 by	Michael O. Distler,
C			Institut fuer Kernphysik,
C			Johannes Gutenberg-Universitaet Mainz
C
C     from HBOOK manual
C
C     $Header: /tmp/cvsroot/Cola/HBook/hserver.f,v 1.16 1997-06-11 11:56:33 distler Exp $
C
      PROGRAM HSERVER
      INTEGER NWORDS,I
      REAL A,B
      PARAMETER(NWORDS=50000)
      CALL HLIMAP(NWORDS,'TEST')
      CALL HBOOK1(1,'test1',100,-3.,3.,0.)
      CALL HCOPY(1,2,'test2')
      CALL HCOPY(1,3,'test3')
      DO 10 I=1,100000000
         CALL RANNOR(A,B)
         CALL HF1(1,A,1.)
         CALL HF1(2,B,1.)
         CALL HF1(3,A**2+B**2,1.)
         IF (MOD(I,100000).EQ.0)
     X        PRINT *,' hserver in loop index ',I
 10   CONTINUE
      CALL HFREEM(0)
      END
