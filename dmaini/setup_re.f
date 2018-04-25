	subroutine setup_res(iterm,xres,x,nx,npow,pow,ko,fnpow,fpow,fko,
     &                     NPOWMAX,NXMAX,NPARA,NTERM,legendre)

	implicit none
	integer NPOWMAX,NXMAX,NPARA,NTERM
c
c       BESETZT DEN VEKTOR XRES  
c
c       iterm: NUMMER DES ZU ERMITTELTEN TERMS AM TARGET
c              iterm=1 : DELTA
c              iterm=2 : THETA0
c              iterm=3 : Y0
c              iterm=4 : PHI0
c       npow : ANZAHL DER POTENZEN
c       pow  : ARRAY MIT DEN POTENZEN F"UR X, THETA, Y UND PHI
c            pow(term Bild,Nummer,term Target)
c       nx   : ANZAHL DER KOORDINATEN
c       x    : KOORDINATEN IN DER ME"SEBENEN x(term Bild,Nummer)
c       xres : VEKTOR MIT BERECHNETER TARGETKOORDINATE
c       ko   : KOEFFIZIENTENVEKTOR
c       fnpow: ZAHL DER FIXE POTENZEN
c       fpow : FIXE POTENZEN
c       fko  : FIXE KOEFFIZIENTEN
c       legendre : logical, entscheidet, ob Potenzen oder Legendre Pol.
c       p_legendre : real*8 Funktion, Legendre Polynome
c       VARIABLENDEKLARATION
c
	integer npow,pow,i,j,k,iterm,p,nx,fnpow,fpow
	dimension pow(NPARA,NPOWMAX,NTERM),fpow(NPARA,NPOWMAX,NTERM)
	real*8 x,prod,b,sum,xres,ko,fko,p_legendre
	dimension x(NPARA,NXMAX),xres(NXMAX),ko(NPOWMAX),fko(NPOWMAX)
	logical legendre

c
c       BERECHNE BEITRAG DER FIXEN KOEFFIZIENTEN
c
c
c       SCHLEIFE "UBER DIE ANZAHL DER KOORDINATENPUNKTE nx
c
	do i=1,nx
c
c               SCHLEIFE "UBER DIE ANZAHL DER VERWENDETEN FIXEN POTENZEN
c
	  sum=0.0
	  do j=1,fnpow
	    prod=1.          ! Hilfsg"o"se
	    do k=1,4         ! Schleife "uber die x,theta...
	      b=x(k,i)
	      p=fpow(k,j,iterm)
	      if (legendre) then         ! Legendre Polynome
		prod=prod*p_legendre(p,b)
	      else                       ! Polynome
		if ((b.ne.0.0).or.(p.ne.0)) then
		  prod=prod*b**p
		end if
	      end if
	    end do
	    sum=sum+prod*fko(j)
	  end do
	  xres(i)=sum
	end do
c
c       BERECHNE BEITRAG DER OPTIMIERTEN KOEFFIZIENTEN
c
c
c       SCHLEIFE "UBER DIE ANZAHL DER KOORDINATENPUNKTE nx
c
	do i=1,nx
c
c               SCHLEIFE "UBER DIE ANZAHL DER VERWENDETEN POTENZEN
c
	  sum=0.0
	  do j=1,npow
	    prod=1.             ! Hilfsg"o"se
	    do k=1,4            ! Schleife "uber die x,theta...
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
	    sum=sum+prod*ko(j)
	  end do
	  xres(i)=xres(i)+sum
	end do
c
c       Debug
c
c	do i=1,nx
c          write(6,*)i,xres(i)
c	end do
	return
	end

