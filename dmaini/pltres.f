	subroutine pltres(iterm,x0,xres,nx,NTERM,NXMAX)
	implicit none

	integer NTERM,NXMAX,iterm,nx,i

	real*8 x0,xres
	dimension x0(NTERM,NXMAX),xres(NXMAX)

	open(54,file='outres.lis',status='UNKNOWN',err=10)
	do i=1,nx
	  write(54,'(5(1x,E15.8))')x0(iterm,i),xres(i),
     &    x0(iterm,i)-xres(i),x0(2,i),x0(4,i)
c         write(6,'(E15.8,1x,E15.8)')x0(iterm,i),xres(i)
	end do
	close(54)
	goto 20
10      write(6,*)'ERROR OPENING FILE outres.lis '
20      continue
	return
	end
