	
	character*(*) function small_letter(t)

	implicit none
	
	integer l,j

	character*(*) t, tab(2)*26
	data tab /'abcdefghijklmnopqrstuvwxyz',
     &              'ABCDEFGHIJKLMNOPQRSTUVWXYZ'/


	do l=1,len(t)
		j=index(tab(2),t(l:l))
		if (j.eq.0) then
			small_letter(l:l)=t(l:l)
		else
			small_letter(l:l)=tab(1)(j:j)
		end if
	end do
	end
	

	
