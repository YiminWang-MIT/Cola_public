	subroutine outko(iterm,ko,npow,pow,lsq,nx,used,npow_save,
     &                   pow_save,NPOWMAX,NPARA,NTERM,legendre,
     &                   err,up,atw)

	implicit none
	integer NPOWMAX,NPARA,NTERM
c
	integer iterm,npow,pow,nx,npow_save,pow_save,j,n,nfree,mode
	dimension pow(NPARA,NPOWMAX,NTERM),pow_save(NPARA,NPOWMAX,NTERM)

	logical used,all_out,legendre,fl,up
	dimension used(NPOWMAX,NTERM)
	
	real*8 ko,lsq,ko_out,err,err_out
	dimension ko(NPOWMAX),err(NPOWMAX*NPOWMAX)

	character*6  cterm(4)
	character    term*6,atw*(*),mask*20,st*80
	common /text/ cterm
c
	data all_out / .false. /
	
	term=cterm(iterm)
	all_out=.false.
C
C       INTERPRETIEREN DES KOMMANDOS ATW
C
	mode=0
	mask='/all_out'
	call interpret(atw,mask,mode,st,fl)
	if (fl) then
	  all_out=.true.
	end if
c
c       PROGRAMMKOPF
c
	write(6,*)'OUTKO :'
	write(6,*)'Matrixelements for '//term
	write(6,*)npow
	if (up) then
	  write(6,*)'< delta   theta0   y0   phi0 >'
	else
	  write(6,*)'< x       theta    y    phi >'
	end if
	write(6,*)' '
c       
c       AUSGABE DER GEFITTETEN MATRIXELEMENTE
c
	nfree=nx-npow-1
	do n=1,npow     
	  err_out=dsqrt(abs(err(n+(n-1)*npow))*lsq/nfree)
	  write(6,110)ko(n),err_out,.true.,
     &      'Nr.',n,': < ',pow(1,n,iterm),' ',pow(2,n,iterm),
     &      ' ',pow(3,n,iterm),' ',pow(4,n,iterm),' >'
	end do
	write(6,*)' '
	write(6,*)'up        = ',up
	write(6,*)'legendre  = ',legendre
	write(6,'(1x,A,I6)')'nx        =',nx
	write(6,'(1x,A,I6)')'npow      =',npow
	write(6,'(1x,A,E12.5)')'LSQ       =',lsq
	write(6,'(1x,A,E12.5)')'LSQ/nfree =',lsq/nfree
	write(6,*)' '
	write(6,*)'Errors have been normalized to lsq/nfree := 1'
	write(6,*)' '
	if (all_out) then
	  j=1
	  do n=1,npow_save
	    if (used(n,iterm)) then
	      ko_out=ko(j)
	      err_out=dsqrt(abs(err(j+(j-1)*npow))*lsq/nfree)
	      j=j+1
	    else
	      ko_out=0.0
	      err_out=0.0
	    end if
	    write(6,110)ko_out,err_out,used(n,iterm),
     &        'Nr.',n,': < ',pow_save(1,n,iterm),' ',
     &         pow_save(2,n,iterm),' ',pow_save(3,n,iterm),
     &         ' ',pow_save(4,n,iterm),' >'
	  end do
	  write(6,*)' '
	  call enter_return()
	end if
110     format(1x,E15.8,1x,E15.8,1x,L1,3x,A3,I3,A3,I2,A1,I2,A1,I2,A1,
     &         I2,A2)
	return
	end

