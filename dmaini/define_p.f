	subroutine define_powers(iterm,npow,pow,used,NPOWMAX,NPARA,NTERM,
     &                         atw)

	implicit none
	integer NPOWMAX,NPARA,NTERM
c
c       ZUR DEFINITION DER POTENZEN F"UR DIE ABBILDUNG BILDEBENE-TARGET 
c
c       cterm: CHARACTER ARRAY MIT DEN BEZEICHNUNGEN DER TARGET-KOORD.
c       iterm: NUMMER DES ZU ERMITTELTEN TERMS AM TARGET
c              iterm=1 : DELTA
c              iterm=2 : THETA0
c              iterm=3 : Y0
c              iterm=4 : PHI0
c       npow : ANZAHL DER POTENZEN F"UR FIT
c       pow  : ARRAY MIT DEN POTENZEN F"UR X, THETA, Y UND PHI
c            pow(Koord. Bild,Nummer,Term Target)
c       used : LOGICAL ARRAY, BESTIMMT OB DER ENTSPRECHENDE TERM BEIM FIT
c                   OPTIMIERT WERDEN SOLL. MOMENTAN OHNE BEDEUTUNG.
c
c       atw  : KOMMANDOSTRING MIT STEUERANWEISUNGEN F"UR SUBROUTINE
c
c       VARIABLENDEKLARATIONSTEIL
c
	integer npow,pow,iterm,i,n,ios,mode,trimlen
	dimension pow(NPARA,NPOWMAX,NTERM)
	logical used(NPOWMAX,NTERM),file_in,file_out,fl
	character*6  cterm(4)
	character infile*80,outfile*80,term*6,term_old*6,atw*(*),
     &          powstring*80,mask*20,st*80
	common /text/ cterm
c
c       VOREINSTELLUNGSWERTE F"UR EINIGE VARIABLEN
c
	term=cterm(iterm)
	file_in=.false.
	file_out=.false.
	st=' '
C
C       INTERPRETIEREN DES KOMMANDOS ATW
C
	mode=1
	mask='/file'
	call interpret(atw,mask,mode,st,fl)
	if (fl) then
	  infile=st
	  file_in=.true.
	end if
	
	mode=1
	mask='/save'
	st=' '
	call interpret(atw,mask,mode,st,fl)
	if (fl) then
	  outfile=st
	  file_out=.true.
	end if
	
	if (file_in) then
	  open(50,file=infile,status='OLD',iostat=ios)
	  if (ios.gt.0) then
	    write(6,'(A)')' ERROR in subroutine DEFINE_POWERS: '//
     &                 ' FILE NOT FOUND '//infile(1:trimlen(infile))
	    return
	  end if
	
	  read(50,'(A,I3)')term_old,npow
	  write(6,'(1x,A,I3)')term_old,npow
	  if (term_old.ne.term) then
	    write(6,*)'<<< WARNING : WRONG COORDINATE IDENTIFIER >>>'
	  end if
	  do n=1,npow
	    read(50,'(4I2,L1)')(pow(i,n,iterm),i=1,4),used(n,iterm)
	  end do
	  close(50)
	else

c
c               EINLESEN VON TASTATUR
c
	  write(6,*)'Define powers for X, THETA, Y and PHI (4I2). '//
     &        'Hit <RETURN> to stop.'
	  npow=0
	  powstring='0,0,0,0'
	  do while ( powstring.ne.' ' )
10          continue
	    powstring=' '
	    write(6,'(A4,I3,A3,$)')'No.',npow+1,':  '
	    read(5,'(A)')powstring
		 if (powstring.ne.' ') then
	      npow=npow+1
	      read(powstring,'(4I2)',iostat=ios)(pow(i,npow,iterm), 
     &              i=1,4)
		   if (ios.gt.0) then
		     npow=npow-1
		write(6,*)'SYNTAX ERROR'
		     goto 10
	      end if
	      used(npow,iterm)=.true.
c
c                       PR"UFE,OB KOMBINATION SCHON VORHANDEN
c
	      do n=1,npow-1
		if ((pow(1,n,iterm).eq.pow(1,npow,iterm)).and.
     &            (pow(2,n,iterm).eq.pow(2,npow,iterm)).and.
     &            (pow(3,n,iterm).eq.pow(3,npow,iterm)).and.
     &            (pow(4,n,iterm).eq.pow(4,npow,iterm))) then
		  write(6,'(A,I3,A)')' << input identical with No.',n,'>>'
		  npow=npow-1
		  goto 10
		end if
	      end do
	    end if
	  end do
	end if
c
c               SPEICHERN DER DATEN AUF FILE
c
	if (file_out) then      
	  open(unit=60,file=outfile,status='UNKNOWN',iostat=ios)
	  if (ios.gt.0) then
	    write(6,'(A)')' ERROR in subroutine DEFINE POWERS :'//
     &     'can not write on file '//outfile(1:trimlen(outfile))
	    return
	  end if
	  write(60,'(A,I3)')term,npow
	  do n=1,npow
			write(60,110)(pow(i,n,iterm),i=1,4),used(n,iterm),
     &     'Nr.',n,': <  x^',pow(1,n,iterm),' * theta^',pow(2,n,iterm),
     &              ' * y^',pow(3,n,iterm),' * phi^',pow(4,n,iterm),' >'
	  end do
	  close(60)
	end if
	write(6,*)
	write(6,*)'DEFINE POWERS :'
	if (file_in) then
	  write(6,*)'data read from file  '//infile(1:trimlen(infile))
	end if
	if (file_out) then
	  write(6,*)'data written to file '//outfile(1:trimlen(outfile))
	end if
	write(6,'(1x,A,I3)')term,npow
	do n=1,npow
	  write(6,120)(pow(i,n,iterm),i=1,4),used(n,iterm),
     &   'Nr.',n,': <  x^',pow(1,n,iterm),' * theta^',pow(2,n,iterm),
     &             ' * y^',pow(3,n,iterm),' * phi^',pow(4,n,iterm),' >'
	  end do
110     format(4I2,L1,10x,A3,I3,A7,I2,A9,I2,A5,I2,A7,I2,A2)
120     format(1x,4I2,L1,10x,A3,I3,A7,I2,A9,I2,A5,I2,A7,I2,A2)
	return
	end

