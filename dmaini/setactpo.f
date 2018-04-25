c
c~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
c
	subroutine setactpow(iterm,npow,pow,npow_save,pow_save,used,
     &                                   NPOWMAX,NPARA,NTERM)

	implicit none
	integer NPOWMAX,NPARA,NTERM
c
c       BESETZT pow BIS ZUM npow-ten ELEMENT MIT DEN POTENZEN, DIE
c       BEIM FIT ZU VERWENDEN SIND. IN pow_save STEHEN ALLE IN DEFINIERTEN
c       POTENZEN. FALLS used=.true. WERDEN SIE IN pow "UBERNOMMEN.
c
	integer iterm,npow,pow,npow_save,pow_save,i,j,n
	dimension pow(NPARA,NPOWMAX,NTERM)
	dimension pow_save(NPARA,NPOWMAX,NTERM)
	
	logical used
	dimension used(NPOWMAX,NTERM)

	i=1
	n=1
	do while ((i.le.npow).and.(n.le.npow_save))
		if (used(n,iterm)) then
			do j=1,NPARA
				pow(j,i,iterm)=pow_save(j,n,iterm)
			end do
			i=i+1
		end if
		n=n+1
	end do
	npow=i-1 ! 14.5.91 MK
	return
	end

