c
c	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
c
	subroutine enter_return()

	implicit none
	character*1 cdum

	write(6,'(1x,A)')'< RETURN >'
	read(5,'(A)')cdum
	return
	end

