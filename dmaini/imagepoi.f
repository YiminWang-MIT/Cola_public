	subroutine imagepoint(x,theta,nray,NRAYMAX,xs,zs,fileout)
	
	implicit none

	integer nray,NRAYMAX,i,j,k,NRAY_SAVE

	parameter (NRAY_SAVE=5000)

	real*8 x,theta,xa,ttha,xb,tthb,xs,zs,xssum,zssum,xs_save,
     &         zs_save,sigxs,sigzs
	dimension x(NRAYMAX),theta(NRAYMAX),xs_save(NRAY_SAVE),
     &            zs_save(NRAY_SAVE)
	logical fileout

	if (nray.gt.NRAY_SAVE) then
	  write(6,*)'IMAGEPOINT :'
	  write(6,*)'             NRAY_SAVE too small!!'
	  return
	end if

	xssum=0.0
	zssum=0.0
	k=0
	do i=1,NRAY_SAVE
	  xs_save(i)=0.0
	  zs_save(i)=0.0
	end do
	do i=1,nray-1
	  xa=x(i)
	  ttha=tan((theta(i))/1000.)   ! mrad
	  do j=i+1,nray
	    xb=x(j)
	    tthb=tan((theta(j))/1000.) ! mrad
c
c       event is rejected if angles differ less then two mrad
c
	    if (abs(theta(i)-theta(j)).gt.2.0) then
	      xs=(tthb*xa-ttha*xb)/(tthb-ttha)
	      xssum=xssum+xs
	      zs=(xs-xb)/tthb
	      zssum=zssum+zs
	      k=k+1
	      xs_save(k)=xs
	      zs_save(k)=zs
c              print*,k,xs_save(k),zs_save(k)
c              write(6,'(I3,6F9.3)')k,xs,zs,xa,theta(i),xb,theta(j)
	    end if
	  end do
	end do
	if (k.gt.1) then
	  xs=xssum/k
	  zs=zssum/k
	  sigxs=0.0
	  sigzs=0.0
	  do i=1,k
	    sigxs=sigxs+(xs_save(i)-xs)**2 ! Sigma der Verteilung der 
	    sigzs=sigzs+(zs_save(i)-zs)**2 ! Schnittpunkte
c            print*,i,xs_save(i),xs,zs_save(i),zs
	  end do
	  sigxs=dsqrt(sigxs/(k-1))/dsqrt(dble(k)) ! Fehler am Mittelwert
	  sigzs=dsqrt(sigzs/(k-1))/dsqrt(dble(k))
	else
	   sigxs=0.0
	   sigzs=0.0
	end if
	write(6,'(1x,4F8.3,I4)')xs,zs,sigxs,sigzs,k
	if (fileout) then
	  write(77,'(1x,4F8.3,I4)')xs,zs,sigxs,sigzs,k
	end if
	return
	end
	    
