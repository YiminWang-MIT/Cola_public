	subroutine weights(iterm,A,x0,nrph,npow,nx,NAMAX,NXMAX,
     &                     NTERM,mode)

	implicit none
	integer NXMAX,NAMAX,NTERM
c
c       BRINGT GEWICHTSFAKTOREN AN DEN VEKTOR A UND X0 AN 
c
c       mode : 1 : mit Gewichten multiplizieren
c              0 : durch Gewichte dividieren
c
c       iterm: NUMMER DES ZU ERMITTELTEN TERMS AM TARGET
c              iterm=1 : DELTA
c              iterm=2 : THETA0
c              iterm=3 : Y0
c              iterm=4 : PHI0
c       A    : KOORDINATENVEKTOR (MATRIX) 
c       nx   : ANZAHL DER KOORDINATEN
c       
c
c       VARIABLENDEKLARATION
c
	integer npow,i,j,m,n,iterm,nx,nrph,mode
	dimension nrph(NXMAX)
	real*8 A,sigma,x0,weight
	dimension A(NAMAX),sigma(4),x0(NTERM,NXMAX)
c
c       Schaetzwerte fuer Sigmas Target        
	data sigma /0.05,1.5,0.15,1.5/
c
c       
c
c       SCHLEIFE "UBER DIE ANZAHL DER KOORDINATENPUNKTE nx
c
	do i=1,nx
	  n=i-1
c
c         SCHLEIFE "UBER DIE ANZAHL DER VERWENDETEN POTENZEN
c
	  weight=dsqrt(dble(nrph(i)))/sigma(iterm)
	  if (mode.eq.0) then
	    if (weight.ne.0) then
	      weight=1/weight
	    else
	      weight=1.0
	    end if
	  end if

	  x0(iterm,i)=x0(iterm,i)*weight

	  do j=1,npow
	    m=j+n*npow        ! Index f"ur A-Vektor
	    A(m)=A(m)*weight
	  end do
	end do
	return
	end


