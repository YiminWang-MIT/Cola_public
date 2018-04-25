	subroutine outmean(iterm,nenergy,energ,mean,NENERGMAX,atw)

	implicit none

	integer NENERGMAX,iterm,nenergy,trimlen,i,mode

	real*8 energ,mean
	dimension energ(NENERGMAX),mean(NENERGMAX)

	character atw*(*),outfile*80,mask*20,st*80
	character*6 cterm(4)
	common /text/ cterm

	logical file_out,fl

	file_out=.false.
C
C       INTERPRETIEREN DES KOMMANDOS ATW
C
	mode=1
	mask='/save'
	st=' '
	call interpret(atw,mask,mode,st,fl)
	if (fl) then
	  outfile=st
	  file_out=.true.
	end if  

	if (file_out) then 
	  open(77,file=outfile,status='UNKNOWN')
		 write(77,*)'RMS resolution vs delta for '//cterm(iterm)
		 write(77,*)'DELTA           MEAN'
	    do i=1,nenergy
			write(77,'(1E15.8,1x,1E15.8)')energ(i),mean(i)
	    end do
	end if
	write(6,*)
	write(6,*)'OUTMEAN :'
	write(6,*)'  RMS resolution vs delta for '//cterm(iterm)
	write(6,*)'DELTA           MEAN'
	do i=1,nenergy
	  write(6,'(1E15.8,1x,1E15.8)')energ(i),mean(i)
	end do
	write(6,*)
	if (file_out) then
	  write(6,*)' data written on file '//outfile(1:trimlen(outfile))
	  write(6,*)
	end if
	close(77)
	return
	end
