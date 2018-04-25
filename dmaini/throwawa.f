      
	subroutine throwaway(iterm,ko,npow,used,NPOWMAX,NTERM,
     &                       nx,lsq,err,npow_save)
c
c       eliminiert Matrixelemente deren Fehler gr"osser ist als ihr
c       Betrag
c
	implicit none
	integer NPOWMAX,NTERM
c
	integer iterm,npow,nfree,n,nx,npow_save,j

	logical used
	dimension used(NPOWMAX,NTERM)

	
	real*8 ko,lsq,err,err_out
	dimension ko(NPOWMAX),err(NPOWMAX*NPOWMAX)

	nfree=nx-npow-1
	j=1
	do n=1,npow_save
	  if (used(n,iterm)) then
	    err_out=dsqrt(dabs(err(j+(j-1)*npow))*lsq/nfree)
	    if (err_out.ge.abs(ko(j))) then
	      used(n,iterm)=.false.
	    end if
	    j=j+1
	  end if
	end do
	return
	end

