	subroutine calcsigma(iterm,x0,x,xres,nenergy,nrpe,energ,sigma,
     &                     deltasigma,NTERM,NPARA,NXMAX,NENERGMAX,up)
	implicit none

	integer NTERM,NPARA,NXMAX,NENERGMAX,iterm,nenergy,nrpe,i,j,k
	dimension nrpe(NENERGMAX)

	real*8 x0,x,xres,energ,sig,diff,sigma,deltasigma
	dimension x0(NTERM,NXMAX),xres(NXMAX),energ(NENERGMAX),
     &          sigma(NENERGMAX),deltasigma(NENERGMAX),x(NPARA,NXMAX)
	logical up

	k=0
	do i=1,nenergy
	  sig=0.0
	  do j=1,nrpe(i)
	    k=k+1
	    diff=xres(k)-x0(iterm,k)
	    sig=diff**2+sig
	  end do
	  sig=dsqrt(sig/dble(nrpe(i)))
	  sigma(i)=sig
	  deltasigma(i)=sig/dsqrt(dble(nrpe(i)))
	  if (up) then
	    energ(i)=x(1,k)
	  else
	    energ(i)=x0(1,k)
	  end if
	end do
	return
	end
