	subroutine read_koord(x0,x,nrph,nx,nenergy,nrpe,NPARA,NTERM,        
     &                        NXMAX,NENERGMAX,atw)
c
c       Einlesen der Koordinatenfiles mit Inhalt
c         delta, x0, th0, y0, ph0, x, th ,y, ph, nh
c
c       Syntax: "data/qualifier"
c       Moegliche qualifier : 
c       a) init
c          initialisiert Datenfelder
c       b) file=<NAME>
c          Angabe des Filenamens.
c          Bei jedem Aufruf wird die Variable "nenergy" um 1 erhoeht und
c          im Feld "nrpe" an der Stelle "nrpe(nenergy)" die Zahl der ver-
c          schiedenen Strahlen abgespeichert.
c       c) list
c          zeigt eingelesene Daten auf STDOUT an
c       d) accu
c          akkumuliert das spezifizierte Datenfile zum vorher ein-
c          gegebenen Datensatz gleicher Impulsablage. Insbesondere wird
c          "nenergy" nicht erhoeht. Ist in dem zu akkumulierenden 
c          Datensatz die Anzahl der Strahlen hoeher als in dem zuvor ab-
c          gespeicherten Datensatz gleicher Energie, so erscheint die
c          Warnung "accu : not found". Die "ueberzaehligen" Strahlen 
c          werden ignoriert.
c
	implicit none
	integer NXMAX,NPARA,NTERM,NENERGMAX

	integer nx,nenergy,nrpe,mode,ios,i,j,nrph
	dimension nrpe(NENERGMAX),nrph(NXMAX)

	real*8 x0,x
	dimension x0(NTERM,NXMAX),x(NPARA,NXMAX)

	character mask*20,st*80,infile*80
	character atw*(*)

	logical fl,accu,list

C       INTERPRETIEREN DES KOMMANDOSTRINGS atw
	mode=0
	mask='/init'
	call interpret(atw,mask,mode,st,fl)
	if (fl) then
	  nx=0
	  do i=1,NTERM
	    do j=1,NXMAX
	      x0(i,j)=0.0
	    end do
	  end do
	  do i=1,NPARA
	    do j=1,NXMAX
	      x(i,j)=0.0
	    end do
	  end do
	  nenergy=0
	  do i=1,NENERGMAX
	    nrpe(i)=0
	  end do
	end if
	mode=0
	mask='/accumulate'
	call interpret(atw,mask,mode,st,accu)
	mode=0
	mask='/list'
	call interpret(atw,mask,mode,st,list)
	mode=1
	mask='/file'
	call interpret(atw,mask,mode,infile,fl)
	if (fl) then
	  open(70,file=infile,status='OLD',iostat=ios)
	  if (ios.gt.0) then
	    write(6,'(1x,A)')'ERROR in READ_KOORD : FILE NOT FOUND'
	  else
	    call get_koord(70,x0,x,nrph,nx,nenergy,nrpe,accu,list,
     &                     NPARA,NTERM,NXMAX,NENERGMAX)
	    close(70)
	  end if
	end if
	write(6,*)'READ_KOORD :'
	write(6,*)'        nx = ',nx
	write(6,*)'   nenergy = ',nenergy
	write(6,*)
	return
	end

	subroutine get_koord(unit,x0,x,nrph,nx,nenergy,nrpe,accu,
     &                       list,NPARA,NTERM,NXMAX,NENERGMAX)
C
C       LIEST VOM FILE MIT DER UNIT unit DIE KOORDINATEN AM TARGET
C       UND IN DER BILDEBENEN.
C       BEI JEDEM AUFRUF WIRD NENERGY ( ANZAHL DER ENERGIEN ) UM EINS
C       ERHOEHT. DIE ZAHL DER STRAHLEN DIESER ENERGIE WIRD IM VEKTOR
C       nrpe (number of rays per energy)  ABGELEGT.     
C       DIE TOTALE ANZAHL DER STRAHLEN nx WIRD UM nrpe ERHOEHT.
C
C       Die Variable nrph bedeutet nUMBER of rAYS pER hOLE und ist
C       die Zahl der Ereignisse, die das entsprechende Loch passiert
C       haben (genauer : Ereignisse mit gleichen Startkoordinaten)
C
C       accu ist eine logische Variable und ist wahr, falls der aktuell
C       eingelesene Datensatz zu einem schon eingelesenen Datensatz 
C       gleicher Energie hinzugenommen werden soll
C
C       list ist ebenfalls eine logische Variable und ist wahr, falls
C       die eingelesenen Daten angezeigt werden sollen
C
	implicit none
	integer NXMAX,NPARA,NTERM,NENERGMAX

	integer nx,nenergy,nrpe,unit,nxe,nrph,i,n
	dimension nrpe(NENERGMAX),nrph(NXMAX)

	real*8 x0,x,dum,delta,th0,y0,ph0,xf,thf,yf,phf
	dimension x0(NTERM,NXMAX),x(NPARA,NXMAX)

	logical accu,found,list

	if (accu) then
	  do while (.true.)
	    found=.false.
	    read(unit,*,err=20)delta,dum,th0,y0,ph0,xf,thf,yf,phf,n
	    if (list) then
	      write(6,100)delta,dum,th0,y0,ph0,xf,thf,yf,phf,n
	    end if
	    i=1
	    do while ((i.le.nx).and.(.not.found))
	      if ((abs(delta-x0(1,i)).lt.0.001).and.  ! 1E-5 
     &            (abs(th0-x0(2,i)).lt.0.05).and.     ! 5/100 mrad
     &            (abs(y0-x0(3,i)).lt.0.005).and.     ! 5/1000 cm
     &            (abs(ph0-x0(4,i)).lt.0.05)) then    ! 5/100 mrad
		if (list) then
		  write(6,'(A)')' +'
		  write(6,100)x0(1,i),dum,x0(2,i),x0(3,i),x0(4,i),
     &                        x(1,i),x(2,i),x(3,i),x(4,i),nrph(i)
		end if
		x0(1,i)=(x0(1,i)*nrph(i)+delta*n)/(nrph(i)+n)
		x0(2,i)=(x0(2,i)*nrph(i)+th0*n)/(nrph(i)+n)
		x0(3,i)=(x0(3,i)*nrph(i)+y0*n)/(nrph(i)+n)
		x0(4,i)=(x0(4,i)*nrph(i)+ph0*n)/(nrph(i)+n)
		x(1,i)=(x(1,i)*nrph(i)+xf*n)/(nrph(i)+n)
		x(2,i)=(x(2,i)*nrph(i)+thf*n)/(nrph(i)+n)
		x(3,i)=(x(3,i)*nrph(i)+yf*n)/(nrph(i)+n)
		x(4,i)=(x(4,i)*nrph(i)+phf*n)/(nrph(i)+n)
		nrph(i)=nrph(i)+n
		if (list) then
		  write(6,'(A)') ' ='
		  write(6,100)x0(1,i),dum,x0(2,i),x0(3,i),x0(4,i),
     &                        x(1,i),x(2,i),x(3,i),x(4,i),nrph(i)
		  write(6,*)'--------------------------------------'
		end if
		found=.true.
	      end if
	      i=i+1
	    end do
	    if ((i.gt.nx).and.(.not.found)) then
	      write(6,*)'WARNING READ_KO, ACCU : not found'
	    end if
	  end do
20        continue
	else
	  nenergy=nenergy+1
	  nxe=0
	  do while (.true.)
	    nx=nx+1
c                  delta    x0  th0      y0       ph0   
	    read(unit,*,err=10,end=11)
     &             x0(1,nx),dum,x0(2,nx),x0(3,nx),x0(4,nx),
c                  x       th      y       ph      Haeufigkeit
     &             x(1,nx),x(2,nx),x(3,nx),x(4,nx),nrph(nx)
	    if (list) then
	      write(6,100)x0(1,nx),dum,x0(2,nx),x0(3,nx),x0(4,nx),
     &                    x(1,nx),x(2,nx),x(3,nx),x(4,nx),nrph(nx)
	    end if
	    nxe=nxe+1
	  end do
	  goto 11
10        continue
	  write(6,*)'GET_KOORD  : READ ERROR AFTER LINE ',nxe
11        continue	  
	  nx=nx-1
	  nrpe(nenergy)=nxe
	  write(6,*)'GET_KOORD  : NUMBER OF RAYS READ : ',nxe
	end if
100     format(F8.4,1x,F7.3,1x,F7.2,1x,F6.2,1x,F7.2,1x,F7.3,1x,F7.2,
     &         1x,F7.3,1x,F7.2,1x,I5)
	return
	end
