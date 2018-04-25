c
c~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
c
	subroutine fix_ko(iterm,fko,fnpow,fpow,fused,NPOWMAX,NPARA,NTERM,
     &                    ko,npow,pow,used,atw)

	implicit none
	integer NPOWMAX,NPARA,NTERM
c
c       ZUR DEFINITION DER FIXEN MATRIXELEMENTE F"UR DIE ABBILDUNG 
c       BILDEBENE-TARGET 
c
c       cterm: CHARACTER ARRAY MIT DEN BEZEICHNUNGEN DER TARGET-KOORD.
c       iterm: NUMMER DES ZU ERMITTELTEN TERMS AM TARGET
c              iterm=1 : DELTA
c              iterm=2 : THETA0
c              iterm=3 : Y0
c              iterm=4 : PHI0
c       fnpow: ANZAHL DER POTENZEN F"UR FIT
c       fpow : ARRAY MIT DEN POTENZEN F"UR X, THETA, Y UND PHI
c            fpow(Koord. Bild,Nummer,Term Target)
c       fko  : ARRAY MIT DEN FIXEN KOEFFIZIENTEN (MATRIXELEMENTE)
c                        fko(Nummer)
c       atw  : KOMMANDOSTRING MIT STEUERANWEISUNGEN F"UR SUBROUTINE
c
c       VARIABLENDEKLARATIONSTEIL
c
	integer fnpow,fpow,iterm,i,n,trimlen,ios,mode,pow,npow,j
	dimension fpow(NPARA,NPOWMAX,NTERM),pow(NPARA,NPOWMAX,NTERM)

	real*8 fko,ko
	dimension fko(NPOWMAX),ko(NPOWMAX)

	logical fused(NPOWMAX,NTERM),file_in,file_out,fl,store,add,
     &          used(NPOWMAX,NTERM),append,next
	character*6  cterm(4)
	character infile*80,outfile*80,term*6,atw*(*),powstring*80,
     &            mask*20,st*80
	common /text/ cterm

	term=cterm(iterm)            
	file_in=.false.
	file_out=.false.
	store=.false.
	add=.false.
	append=.false.
C
C       INTERPRETIEREN DES KOMMANDOS ATW
C
	mode=1
	mask='/file'
	st=' '
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

	mode=0
	mask='/store'
	st=' '
	call interpret(atw,mask,mode,st,store)
	
	mode=0
	mask='/add'
	st=' '
	call interpret(atw,mask,mode,st,add)
	
	mode=0
	mask='/append'
	st=' '
	call interpret(atw,mask,mode,st,append)

c       EINLESEN VON FILE
c
	if (file_in) then
	  open(50,file=infile,status='OLD',iostat=ios)
	  if (ios.gt.0) then
	    write(6,'(A)')' ERROR in subroutine FIX_KO: '//
     &                 ' FILE NOT FOUND '//infile(1:trimlen(infile))
	    return
	  end if
	  read(50,*)
	  read(50,*)
	  fnpow=0
	  do while (.true.)
	    fnpow=fnpow+1
	    read(50,*,err=20)fko(fnpow),fused(fnpow,iterm),
     &          fpow(1,fnpow,iterm),fpow(2,fnpow,iterm),
     &          fpow(3,fnpow,iterm),fpow(4,fnpow,iterm)
	  end do
20        continue
	  fnpow=fnpow-1
	  close(50)
	else if ((.not.store).and.(.not.add)) then

c
c       EINLESEN VON TASTATUR
c
	  write(6,*)'Define matrixelement and powers for X, '//
     &            'THETA, Y and PHI (D15.8,4I2).'
	  write(6,*)'Hit <RETURN> to stop.'
		fnpow=0
	   powstring='0.0,0,0,0,0,'
		do while ( powstring.ne.' ')
10                      continue
			write(6,'(A4,I2,A3)')'$No.',fnpow+1,':  '
	      read(5,'(A)')powstring
	      if (powstring.ne.' ') then
		fnpow=fnpow+1
			  read(powstring,'(D15.8,4I2)',err=10)fko(fnpow),
     &                               (fpow(i,fnpow,iterm), i=1,4)
			  fused(fnpow,iterm)=.true.
c
c                       PR"UFE,OB KOMBINATION SCHON VORHANDEN
c
		do n=1,fnpow-1
		  if ((fpow(1,n,iterm).eq.fpow(1,fnpow,iterm)).and.
     &              (fpow(2,n,iterm).eq.fpow(2,fnpow,iterm)).and.
     &              (fpow(3,n,iterm).eq.fpow(3,fnpow,iterm)).and.
     &              (fpow(4,n,iterm).eq.fpow(4,fnpow,iterm))) then
		    write(6,'(A,I2,A)')'<< input identical '//
     &                                      ' with No.',n,'>>'
		    fnpow=fnpow-1
		    goto 10
		  end if
		end do
	      end if
	   end do
	end if
c
c       SPEICHERN DER DATEN IM MEMORY
c
	if (store) then
	  if (add) then
	    do i=1,npow
	      next=.true.
	      do j=1,fnpow
		if ((pow(1,i,iterm).eq.fpow(1,j,iterm)).and.
     &              (pow(2,i,iterm).eq.fpow(2,j,iterm)).and.
     &              (pow(3,i,iterm).eq.fpow(3,j,iterm)).and.
     &              (pow(4,i,iterm).eq.fpow(4,j,iterm)).and.next) then
		  fko(j)=fko(j)+ko(i)
		  next=.false.
		end if
	      end do
	      if (next) then
		fko(fnpow+1)=ko(i)
		do j=1,NPARA
		  fpow(j,fnpow+1,iterm)=pow(j,i,iterm)
		end do
		fused(fnpow+1,iterm)=used(i,iterm)
		fnpow=fnpow+1
	      end if
	    end do
	  else if (append) then
	    do i=1,npow
	      fko(fnpow+i)=ko(i)
	      do j=1,NPARA
		fpow(j,fnpow+i,iterm)=pow(j,i,iterm)
	      end do
	      fused(fnpow+i,iterm)=used(i,iterm)
	    end do
	    fnpow=fnpow+npow
	  else
	    do i=1,npow
	      fko(i)=ko(i)
	      do j=1,NPARA
		fpow(j,i,iterm)=pow(j,i,iterm)
	      end do
	      fused(i,iterm)=used(i,iterm)
	    end do
	    fnpow=npow
	  end if
	end if
c
c               SPEICHERN DER DATEN AUF FILE
c
	if (file_out) then
	  open(66,file=outfile,status='UNKNOWN',iostat=ios)
	  if (ios.gt.0) then
	    write(6,'(A)')' ERROR in subroutine FIX_KO :'//
     &     'can not write on file '//outfile(1:trimlen(outfile))
	    return
	  end if
c       
c       KOMMENTIERTE AUSGABE DER GEFITTETEN MATRIXELEMENTE
c
	  write(66,'(1x,A)')'Fix matrixelements for focal plane '//term
	  write(66,*)' '
	  do n=1,fnpow  
	    write(66,120)fko(n),fused(n,iterm),
     &            fpow(1,n,iterm),fpow(2,n,iterm),fpow(3,n,iterm),
     &       fpow(4,n,iterm),'Nr.',n,': <  x^',
     &        fpow(1,n,iterm),' * theta^',fpow(2,n,iterm),' * y^',
     &        fpow(3,n,iterm),' * phi^',fpow(4,n,iterm),' >'
	  end do
	  write(66,*)' '
	  close(66)
      end if    
	  
      write(6,*)
      write(6,*)'FIX_KO :'
      if (file_in) then
	  write(6,*)'data read from file  '//infile(1:trimlen(infile))
      end if
      if (file_out) then
	  write(6,*)'data written to file '//outfile(1:trimlen(outfile))
      end if
      if (store) then
	if (add) then
	  write(6,*)'data added in memory'
	else
	  write(6,*)'data stored in memory'
	end if
      end if
      write(6,'(1x,A,I3)')term,fnpow
      do n=1,fnpow    
	  write(6,120)fko(n),fused(n,iterm),
     &            fpow(1,n,iterm),fpow(2,n,iterm),fpow(3,n,iterm),
     &       fpow(4,n,iterm),'Nr.',n,': <  x^',
     &        fpow(1,n,iterm),' * theta^',fpow(2,n,iterm),' * y^',
     &        fpow(3,n,iterm),' * phi^',fpow(4,n,iterm),' >'
      end do
120   format(1x,E15.8,1x,L1,1x,4I2,4x,A3,I3,A7,I2,A9,I2,A5,I2,A7,
     &       I2,A2)
c130   format(1x,4I1,1x,E15.8)
      return
      end
