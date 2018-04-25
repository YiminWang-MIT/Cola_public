	real*8 function p_legendre(n,x)

	implicit none

	integer n,i
	real*8 x,p0,p1,p2
	data p2/0.0/
	
	if (n.eq.0) then 
	  p_legendre=1.0
	else
	  if (n.eq.1) then
	    p_legendre=x
	  else
	    p0=1.0
	    p1=x
	    do i=2,n
		   p2=((2*i-1)*x*p1-(i-1)*p0)/i
	      p0=p1
	      p1=p2
	   end do
	   p_legendre=p2
	  end if
	end if
	return
	end

