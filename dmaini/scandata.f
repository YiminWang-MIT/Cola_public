	subroutine scanimage(x,xext,nx,NPARA,NXMAX)
c
c       sucht Extremwerte der Bildebenenkoordinaten
c
c       x       : Bildebenenebenenkooordinaten
c       xext    : Extremwerte von x 
c       NPARA   : Zahl der Bildebenenkoordinaten

	implicit none

	integer NPARA,NXMAX,i,j,nx

	real*8 x,xext
	dimension x(NPARA,NXMAX),xext(NPARA,2)

	do i=1,NPARA
	  xext(i,1)=x(i,1)
	  xext(i,2)=x(i,nx)
	  do j=1,nx
	    if (x(i,j).lt.xext(i,1)) xext(i,1)=x(i,j)
	    if (x(i,j).gt.xext(i,2)) xext(i,2)=x(i,j)
	  end do
	end do
	write(6,*)
	write(6,*)'SCANIMAGE :'
	write(6,*)'  Image plane extreme values'
	do i=1,NPARA
	  write(6,'(3x,D15.8,2x,D15.8)')xext(i,1),xext(i,2)
	end do
	return
	end
c 
c
	subroutine scaleimagedown(x,xext,nx,NPARA,NXMAX)
c
c       bildet Bildebenenkoordinaten auf Intervall [-1,1] ab
c       fuer Legendrepolynomfit
c
	implicit none

	integer NPARA,NXMAX,i,j,nx

	real*8 x,xext,a,b,c,d
	dimension x(NPARA,NXMAX),xext(NPARA,2)

	do i=1,NPARA
	  a=xext(i,1)
	  b=xext(i,2)
	  if (a.eq.b) then
	    write(6,*)' << ERROR in SCALEIMAGEDOWN >>'
	    write(6,*)'    Interval length is zero.'
	    return
	  else
	  c=1/(b-a)
	    d=-b-a
	    do j=1,nx
	      x(i,j)=(x(i,j)*2+d)*c
	    end do
	  end if
	end do
	write(6,*)
	write(6,*)'SCALEIMAGEDOWN :'
	write(6,*)'  Image plane coordinates scaled to interval [-1,1].'
	write(6,*)'  Linear and constant transformation coefficients :'
	do i=1,NPARA
	  a=xext(i,1)
	  b=xext(i,2)
	  c=1/(b-a)
	  d=-b-a
	  write(6,'(3x,D15.8,2x,D15.8)')c,d
	end do
	return
	end
