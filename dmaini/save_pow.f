	
	subroutine save_powers(iterm,npow,pow,npow_save,pow_save,NPOWMAX,
     &                              NPARA,NTERM)

	implicit none
	integer NPOWMAX,NPARA,NTERM
c
c       RETTET npow UND pow AUF npow_save UND pow_save
c
	integer iterm,npow,pow,npow_save,pow_save,i,j
	dimension pow(NPARA,NPOWMAX,NTERM),pow_save(NPARA,NPOWMAX,NTERM)

	npow_save=npow
	do i=1,npow
		do j=1,NPARA
			pow_save(j,i,iterm)=pow(j,i,iterm)
		end do
	end do
	return
	end

