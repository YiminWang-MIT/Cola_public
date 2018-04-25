c
c~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
c
	subroutine fixpart(iterm,x0,nx,x,fko,fpow,fnpow,
     &                     NPOWMAX,NXMAX,NTERM,NPARA,
     &                     legendre,mode)

	implicit none
	integer NPOWMAX,NXMAX,NTERM,NPARA
	integer iterm,nx,i,j,k,fpow,fnpow,p,mode
	dimension fpow(NPARA,NPOWMAX,NTERM)
c
	real*8 x0,fko,fx0,prod,b,x,p_legendre
	dimension x0(NTERM,NXMAX),x(NPARA,NXMAX),fko(NPOWMAX)
c
	logical legendre

c
c       ZIEHE FIXEN ANTEIL AB (ADDIERE IHN WIEDER)
c
	do i=1,nx
c
c       SCHLEIFE "UBER DIE ANZAHL DER VERWENDETEN FIXEN POTENZEN
c
	  fx0=0.0
	  do j=1,fnpow
c           print*,j,fnpow,fko(j)
	    prod=1.                  ! Hilfsg"o"se
	    do k=1,4                 ! Schleife "uber die x,theta...
	      b=x(k,i)
	      p=fpow(k,j,iterm)
	      if (legendre) then     ! Legendre Polynome
		prod=prod*p_legendre(p,b)
	      else                   ! Polynome
		if ((b.ne.0.0).or.(p.ne.0)) then
		  prod=prod*b**p
		end if
	      end if
	    end do
	    fx0=fx0+prod*fko(j)
	  end do
	  if (mode.eq.1) then
	    x0(iterm,i)=x0(iterm,i)-fx0 ! ziehe ab
	  else if (mode.eq.0) then
	    x0(iterm,i)=x0(iterm,i)+fx0 ! addiere wieder
	  end if
	end do
	return
	end     

