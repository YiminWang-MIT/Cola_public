	
	subroutine printko(iterm,ko,npow,pow,lsq,nx,used,npow_save,
     &                     pow_save,NPOWMAX,NPARA,NTERM,legendre,
     &                     err,up,atw)

	implicit none
	integer NPOWMAX,NPARA,NTERM
c
	integer iterm,npow,pow,nx,npow_save,pow_save,j,n,
     &          mode,trimlen,nfree
	dimension pow(NPARA,NPOWMAX,NTERM),pow_save(NPARA,NPOWMAX,NTERM)

	logical used,all_out,legendre,fl,up
	dimension used(NPOWMAX,NTERM)
	
	real*8 ko,lsq,ko_out,err,err_out
	dimension ko(NPOWMAX),err(NPOWMAX**2)

	character*6  cterm(4)
	character    outfile*80,term*6,atw*(*),mask*20,st*80
	common /text/ cterm
c
	term=cterm(iterm)
	all_out=.false.
	outfile='matrix.mx'
C
C       INTERPRETIEREN DES KOMMANDOS ATW
C
	mode=1
	mask='/save'
	st=' '
	call interpret(atw,mask,mode,st,fl)
	if (fl) then
	  outfile=st
	end if

	mode=0
	mask='/all_out'
	call interpret(atw,mask,mode,st,fl)
	if (fl) then
	  all_out=.true.
	end if

	open(66,file=outfile,status='UNKNOWN')
	write(6,*)' '
c 
c       
c       KOMMENTIERTE AUSGABE DER GEFITTETEN MATRIXELEMENTE
c
	write(66,'(1x,A)')'Matrixelements for '//term
	write(66,'(I3)')npow
	nfree=nx-npow-1           
	do n=1,npow   
	  err_out=dsqrt(abs(err(n+(n-1)*npow))*lsq/nfree)
	  write(66,120)ko(n),.true.,
     &    pow(1,n,iterm),pow(2,n,iterm),pow(3,n,iterm),pow(4,n,iterm),
     &    err_out,
     &    'Nr.',n,': < ',pow(1,n,iterm),' ',pow(2,n,iterm),' ',
     &                   pow(3,n,iterm),' ',pow(4,n,iterm),' >'
	end do
	write(66,*)'@'
	write(66,*)'Errors have been normalized to lsq/nfree:=1!'
	write(66,*)'up         =',up
	write(66,*)'legendre   =',legendre
	write(66,'(1x,A,I6)')'nx        =',nx
	write(66,'(1x,A,I6)')'npow      =',npow
	write(66,'(1x,A,E12.5)')'LSQ       =',lsq
	write(66,'(1x,A,E12.5)')'LSQ/nfree =',lsq/nfree
	write(66,*)' '
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
	    write(66,120)ko_out,used(n,iterm),
     &        pow_save(1,n,iterm),pow_save(2,n,iterm),
     &        pow_save(3,n,iterm),pow_save(4,n,iterm),
     &        err_out,
     &        'Nr.',n,': < ',
     &        pow_save(1,n,iterm),' ',pow_save(2,n,iterm),' ',
     &        pow_save(3,n,iterm),' ',pow_save(4,n,iterm),' >'
	  end do
	  write(66,*)' '
	end if    ! (all_out)
	write(6,*)'PRINTKO :'
	write(6,*)'Matrix written on :'//outfile(1:trimlen(outfile))
	write(6,*)'up         = ',up
	write(6,*)'all_out    = ',all_out
	write(6,*)'legendre   = ',legendre
120     format(1x,E15.8,1x,L2,1x,4I2,3x,E15.8,4x,A3,I3,A4,I2,A1,I2,A1,
     &         I2,A1,I2,A2)
c130     format(1x,4I1,1x,E15.8)
	close(66)
	return
	end

