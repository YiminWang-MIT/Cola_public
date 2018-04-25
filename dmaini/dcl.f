	subroutine dcl

	integer system,estat
	character*50 command
	logical quit
	external system

	quit=.false.
	do while (.not.quit)
C		write(6,'(A,$)')'DCL>'
C		read(5,'(A)')command
C		if (index(command,'quit').eq.0) then
C	                estat=system(command)
C		else
C			quit=.true.
C		end if
	 write(6,'(A)')'DCL> "Dec Command Language" not implemented'
	 write(6,'(A)')'DCL> (BTW: This is UNIX - in case you are puzzled)'
	 write(6,'(A)')'DCL> quit'
	 quit=.true.
	end do
	return
	end
