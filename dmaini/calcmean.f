	subroutine calcmean(iterm,x0,x,xres,nenergy,nrpe,energ,mean,
     &                    NTERM,NPARA,NXMAX,NENERGMAX,up)
	implicit none

	integer NTERM,NPARA,NXMAX,NENERGMAX,iterm,nenergy,nrpe,i,j,k
	dimension nrpe(NENERGMAX)

	real*8 x0,x,xres,energ,mea,mean
	dimension x0(NTERM,NXMAX),xres(NXMAX),energ(NENERGMAX),
     &          mean(NENERGMAX),x(NPARA,NXMAX)
	logical up

	write(6,*)'CALCMEAN : '
	write(6,*)'           nenergy = ',nenergy 
	do i=1,nenergy
	  write(6,*)'           nrpe  = ',i,nrpe(i)
	end do

	k=0
	do i=1,nenergy
	  mea=0.0
	  do j=1,nrpe(i)
	    k=k+1
	    mea=mea+x0(iterm,k)-xres(k)
	  end do
	  mean(i)=mea/nrpe(i)
	  if (up) then
	    energ(i)=x(1,k)
	  else
	    energ(i)=x0(1,k)
	  end if
	end do
	do i=1,nenergy
	  write(6,*)'            energ = ',i,energ
	end do
	return
	end
