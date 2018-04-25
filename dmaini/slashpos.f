	integer function slashpos(istart,t)

	implicit none
	character t*(*)
	integer istart

	do slashpos=istart,1,-1
		if (t(slashpos:slashpos).eq.'/' ) return
	end do
	slashpos=0
	return
	end
