	subroutine setup_matrix(iterm,A,npow,pow,nx,x,NAMAX,NPOWMAX,
     &                                   NXMAX,NPARA,NTERM,legendre)

	implicit none
	integer NPOWMAX,NXMAX,NAMAX,NPARA,NTERM
c
c       BESETZT DEN VEKTOR A (MATRIX) 
c
c       iterm: NUMMER DES ZU ERMITTELTEN TERMS AM TARGET
c              iterm=1 : DELTA
c              iterm=2 : THETA0
c              iterm=3 : Y0
c              iterm=4 : PHI0
c       A    : KOORDINATENVEKTOR (MATRIX) 
c       npow : ANZAHL DER POTENZEN
c       pow  : ARRAY MIT DEN POTENZEN F"UR X, THETA, Y UND PHI
c              pow(term Bild,Nummer,term Target)
c       nx   : ANZAHL DER KOORDINATEN
c       x    : KOORDINATEN IN DER ME"SEBENEN x(term Bild,Nummer)
c       
c       legendre   : logical, gibt an, ob Polynome oder Legendre Polynome
c                    verwendet werden
c       p_legendre : Legendre Polynome, real*8 function
c
c       VARIABLENDEKLARATION
c
	integer npow,pow,i,j,k,m,n,iterm,p,nx
	dimension pow(NPARA,NPOWMAX,NTERM)
	real*8 A,x,prod,b,p_legendre
	dimension A(NAMAX),x(NPARA,NXMAX)
	logical legendre
c
c       SCHLEIFE "UBER DIE ANZAHL DER KOORDINATENPUNKTE nx
c
	do i=1,nx
	  n=i-1
c
c         SCHLEIFE "UBER DIE ANZAHL DER VERWENDETEN POTENZEN
c
	  do j=1,npow
	    m=j+n*npow        ! Index f"ur A-Vektor
	    prod=1.           ! Hilfsg"o"se
	    do k=1,4          ! Schleife "uber die x,theta...
	      b=x(k,i)
	      p=pow(k,j,iterm)
	      if (legendre) then         ! Legendre Polynome
		prod=prod*p_legendre(p,b)
	      else                       ! Polynome
		if ((b.ne.0.0).or.(p.ne.0)) then
		  prod=prod*b**p
		end if
	      end if
	    end do
	    A(m)=prod
c                 print*,'A=',A(m),m
	  end do
	end do
	return
	end

