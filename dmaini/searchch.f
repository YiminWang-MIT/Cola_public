	integer function searchchar(istart,istop,t,c)

	implicit none
	
	integer istart,istop,idx
	character c*1,t*(*)

	idx=index(t(istart:istop),c)
	if (idx.ne.0) then
	  searchchar=idx+istart-1
	else 
	  searchchar=0
	end if
	end
