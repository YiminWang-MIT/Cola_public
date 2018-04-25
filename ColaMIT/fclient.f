C                                                                 -*-fortran-*-
C     the guy to blame:
C
C     Michael O. Distler                        mailto:distler@mitlns.mit.edu
C     MIT, Lab for Nuclear Science, 26-456      tel.   (617) 253-6997
C     Cambridge, MA 02139                       fax    (617) 258-5440
C
C     $Header: /tmp/cvsroot/Cola/ColaMIT/fclient.f,v 2.1 2002-03-05 03:49:49 distler Exp $
C
C     test program for TCP/IP library
C
      PROGRAM FCLIENT
      IMPLICIT NONE
      INTEGER FD, ERROR, I
      INTEGER TCPIP_OPEN
      INTEGER TCPIP_WRITE
      INTEGER TCPIP_CLOSE
      INTEGER*2 BUFFER

      OPEN(unit=42,file='run4065.dat',status='OLD',
     &     form='UNFORMATTED',access='DIRECT')

      FD = TCPIP_OPEN('mod.mit.edu', 8000)

      I = 1
 100  READ(42,REC=I,ERR=200) BUFFER
      ERROR = TCPIP_WRITE(FD, BUFFER, 2)
      I = I + 1
      GOTO 100

 200  CLOSE(42)
      ERROR = TCPIP_CLOSE(FD)

      END
