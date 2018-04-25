c
c~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
c
	subroutine fitko(iterm,A,x0,ko,npow,nx,aux,
     &                   NPOWMAX,NXMAX,NAMAX,NTERM,err,errors)

	implicit none
	integer NPOWMAX,NXMAX,NAMAX,NTERM,NIPIVMAX,NXX0MAX
c
	parameter (NIPIVMAX=320)
	integer iterm,npow,nx,M1,M,N,L,IERR,IPIV,i
	dimension IPIV(NIPIVMAX)
c
	parameter (NXX0MAX=6100)
	real*8 A,x0,ko,epsi,aux(2*NPOWMAX),xx0,err
	dimension A(NAMAX),x0(NTERM,NXMAX),ko(NPOWMAX),xx0(NXX0MAX),
     &            err(NPOWMAX*NPOWMAX)
	logical errors
c

	common /TLSDIM/ M1,M,N,L,IERR
	data M1,L,epsi /0,1,0.0/

c
c       PR"UFE DIMENSIONIERUNG VON IPIV
c
	if (NIPIVMAX.ne.NPOWMAX) then
	  if (NIPIVMAX.gt.NPOWMAX) then
	    print*,'<< WARNING FROM FITKO : NIPIVMAX > NPOWMAX >>'
	  else
	    print*,'<< ERROR IN FITKO : NIPIVMAX .ne. NPOWMAX >>'
	    print*,'-> EDIT FITKO'
	    stop
	  end if
	end if
c
	if (NXX0MAX.ne.NXMAX) then
	  if (NXX0MAX.gt.NXMAX) then
	    print*,'<< WARNING FROM FITKO : NXX0MAX > NXMAX >>'
	  else
	    print*,'<< ERROR IN FITKO : NXX0MAX .ne. NXMAX >>'
	    print*,'-> EDIT FITKO'
	    stop
	  end if
	end if
c
c       PR"UFE DIMENSIONIERUNG VON xx0
c
	if (NXX0MAX.lt.nx) then
	  print*,'<< ERROR IN FITKO : ARRAY xx0 TO SMALL >>'
	  print*,'-> EDIT DMAINI'
	  stop
	end if
c
c       BESETZE VEKTOR MIT DEN ZU FITTENDEN DATEN (xx0 wird zerstoert)
c
	do i=1,nx
	  xx0(i)=x0(iterm,i)
	end do
c
c       F"UHRE FIT AUS
c
	M=nx                          ! ZAHL DER DATENPUNKTE
	N=npow                        ! ZAHL DER ZU FITTENDEN PARAMETER
	call tls(A,xx0,aux,ipiv,epsi,ko,NAMAX,NXX0MAX,NPOWMAX,NIPIVMAX)
c
c       FALLS FEHLER AUFTRITT : STOP
c
	if (IERR.lt.0) then
	  print*,'<< ERROR IN FITKO : ERROR IN TLS  >>'
	  stop
	end if
	print*,'TLS COMPLETED SUCCESSFULLY'
	
c
c       FEHLER BERECHNEN, FALLS errors=.true., ANDERENFALLS AUF 0.0
c       SETZEN
c
	if (errors) then
	  call tlerr(A,err,aux,ipiv,NAMAX,NXMAX,NIPIVMAX)
	  print*,'TLERR COMPLETED SUCCESSFULLY'
	else
	  do i=1,NPOWMAX*NPOWMAX
	    err(i)=0.0
	  end do
	end if
	return
	end     

