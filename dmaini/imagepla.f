	subroutine imageplane(x0,x,nenergy,nrpe,NTERM,NPARA,NXMAX,
     &                        NENERGMAX,atw)

	implicit none

	integer MAX,PPEMAX
	parameter (MAX=30)     ! maximale Zahl der Bildpunkte
	parameter (PPEMAX=200)! maximale Zahl der Strahlen pro Bildpkt
	integer NTERM,NPARA,NXMAX,NENERGMAX,nenergy,nrpe,i,j,k,l,m
	dimension nrpe(NENERGMAX)

	real*8 x0,x,xt,tht,xs,zs,sumx,sumz,sumxz,sumx2,denom,a,b
	dimension x(NPARA,NXMAX),zs(MAX),xs(MAX),xt(PPEMAX),tht(PPEMAX),
     &            x0(NTERM,NXMAX)
c
c       Variablen fuer Kommandointerpreter
c
	character atw*(*)
	character mask*20,st*80,outfile*80
	integer mode,ios
	real*8 angle
	logical fl,fileout
	data fl /.false./

	mode=1
	mask='/angle'
	call interpret(atw,mask,mode,st,fl)
	if (.not.fl) then
	  write(6,*)'ERROR IN IMAGEPLANE : central ray angle not'//
     &              ' specified'
	  return
	else
	  read(st,'(F6.2)')angle 
	  write(6,*)angle
	end if
	mode=1
	mask='/save'
	call interpret(atw,mask,mode,outfile,fl)
	if (fl) then
	  open(77,file=outfile,status='UNKNOWN',iostat=ios)
	  if (ios.ge.1) then
	    write(6,*)'ERROR IN IMAGEPLANE : can not open file'
	    return
	  else
	    fileout=.true.
	  end if
	else
	  fileout=.false.
	end if

	k=1
	l=0
	sumxz=0.0
	sumx=0.0
	sumz=0.0
	sumx2=0.0
	write(6,*)'IMAGEPOINTS:'
	write(6,*)'xfoc    zfoc    sigx    sigz    npts'
	if (fileout) write(77,*)'xfoc    zfoc    sigx    sigz   npts'
	do i=1,nenergy
	  l=l+nrpe(i)
	  m=0
	  do j=k,l
	   if ((abs(x0(4,j)).le.10.0).and.(abs(x0(2,j)).le.55.0)) then 
c            print*,x0(4,j),x0(2,j)
	     m=m+1                       
	     xt(m)=x(1,j)
	     tht(m)=x(2,j)+angle
	   end if
	  end do
	  k=l+1
	  if (m.ge.2) then
	    if (m.gt.PPEMAX) then
	      write(6,*)'ERROR in subroutine IMAGEPLANE :'
	      write(6,*)' too much rays per imagepoint.'
	    end if
	    call imagepoint(xt,tht,m,PPEMAX,xs(i),zs(i),fileout)
	    sumxz=sumxz+xs(i)*zs(i)
	    sumx=sumx+xs(i)
	    sumz=sumz+zs(i)
	    sumx2=sumx2+xs(i)**2
	  end if
	end do
	if (fileout) close(77)
c        print*,nenergy,sumx2,sumx**2
	denom=nenergy*sumx2-sumx**2
	if (denom.ne.0.0) then
	  a=(nenergy*sumxz-sumx*sumz)/denom !Steigung
	  b=(sumx2*sumz-sumx*sumxz)/denom   !Achsenabschnitt
c         print*,a,b
	  write(6,*)'IMAGEPLANE :'
	  write(6,*)' angle [mrad]     :',atan(a)*1000
	  write(6,*)' z position [cm]  :',b
	  if (fileout) write(6,*)' filename        :',outfile 
	  write(6,*)
	  write(6,*)' WARNING :'
	  write(6,*)' the values are NOT used in transformation'
	  write(6,*)' the result is complete nonsens if phi is not small'
	  write(6,*)
	else
	  write(6,*)'IMAGEPLANE :'
	  write(6,*)' WARNING : too few rays for subroutine IMAGEPLANE'
	  write(6,*)
	end if
	return
	end
