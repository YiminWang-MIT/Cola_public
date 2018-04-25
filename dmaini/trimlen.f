	integer function trimlen(t)

	implicit none
	character t*(*)
	
	do trimlen=len(t),1,-1
		if (t(trimlen:trimlen).ne.' ' ) return
	end do
	trimlen=0
	return
	end
