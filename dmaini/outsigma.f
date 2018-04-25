	subroutine outsigma(iterm,nenergy,energ,sigma,deltasigma,
     &                    NENERGMAX,atw)

	implicit none

	integer NENERGMAX,iterm,nenergy,trimlen,i,mode

	real*8 energ,sigma,deltasigma
	dimension energ(NENERGMAX),sigma(NENERGMAX),deltasigma(NENERGMAX)

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
		 write(77,*)'DELTA          SIGMA          DELTASIGMA'
	    do i=1,nenergy
			write(77,'(3E15.8)')energ(i),sigma(i),deltasigma(i)
	    end do
	end if
	write(6,*)
	write(6,*)'OUTSIGMA :'
	write(6,*)'  RMS resolution vs delta for '//cterm(iterm)
	write(6,*)'DELTA          SIGMA          DELTASIGMA'
	do i=1,nenergy
	  write(6,'(3E15.8)')energ(i),sigma(i),deltasigma(i)
	end do
	write(6,*)
	if (file_out) then
	  write(6,*)' data written on file '//outfile(1:trimlen(outfile))
	  write(6,*)
	end if
	close(77)
	return
	end
