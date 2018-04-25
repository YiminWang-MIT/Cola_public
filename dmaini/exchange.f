	subroutine exchange(x0,x,nx,NTERM,NPARA,NXMAX,up)

	implicit none

	integer nx,NTERM,NPARA,NXMAX,i,j

	real*8 x0,x,save
	dimension x0(NTERM,NXMAX),x(NPARA,NXMAX)
	logical up
	common /text/ cterm
	character*6 cterm(4),cterm_up(4),cterm_down(4)
	data cterm_up / 'x     ','theta ','y     ','phi   '/
	data cterm_down / 'delta ','theta0','y0    ','phi0  '/


	if (NPARA.ne.NTERM) then
	  write(6,*)'Warning from subroutine EXCHANGE :'
	  write(6,*)'NPARA .ne. NTERM'
	  return
	end if
	do j=1,NTERM
	  do i=1,nx
	    save=x0(j,i)
	    x0(j,i)=x(j,i)
	    x(j,i)=save
	  end do
	end do
	if (up) then 
	  up=.false.
	  do i=1,4
	    cterm(i)=cterm_down(i)
	  end do
	else
	  up=.true.
	  do i=1,4
	    cterm(i)=cterm_up(i)
	  end do
	end if
	return
	end
