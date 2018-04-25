c       ***************************************************************
c       ************************ HAUPTPROGRAMM ************************
c       ***************************************************************
c
c       NPOWMAX   : MAXIMALE ANZAHL DER MATRIXELEMENTE (POTENZEN)
c       NXMAX             : MAXIMALE ANZAHL DER STRAHLEN
c       NAMAX             : DIMENSIONIERUNG DES ZU INVERTIERENDEN MATRIXVEKTORS A
c       NPARA             : ANZAHL DER BILDEBENENKOORDINATEN X
c       NTERM             : ANZAHL DER ZU BESTIMMENDEN TARGETKOORDINATEN X0
c       NERNGERMAX : MAXIMALE ANZAHL DER VERSCHIEDENEN ENERGIEEN IM 
c                                       STRAHLENSATZ     
c

c       trimlen, searchchar           : interger*4 functions
c       small_letter                  : character*(*) function
c       F-Test 'invertiert', d.h. es wird mit allen Matrixelementen
c       angefangen und dann testweise jedes Matrixelement weggelassen
c       und ein F-Test ausgefuehrt 

	program dmain
c
c       VARIABLENDEKLARATION
c
	implicit none
c

	integer NPOWMAX,NXMAX,NAMAX,NPARA,NTERM,NENERGMAX
	parameter (NPOWMAX=320,NXMAX=6100,NAMAX=1952000,NPARA=4,NTERM=4)
	parameter (NENERGMAX=50)
	
	integer npow,pow,iterm,i,npow_save,pow_save,n,nx,i_choice,
     &          nenergy,nrpe,j,fnpow,fpow,ncommand,trimlen,unit,
     &          nrph,mode,ios
	dimension pow(NPARA,NPOWMAX,NTERM),pow_save(NPARA,NPOWMAX,NTERM),
     &            nrpe(NENERGMAX),fpow(NPARA,NPOWMAX,NTERM),nrph(NXMAX)

	real*8 A,x,x0,ko,lsq,lsq_old,ko_save,aux,xres,energ,sigma,
     &         deltasigma,mean,lsq_save,fko,xext,err,err_save

	dimension A(NAMAX),x(NPARA,NXMAX),x0(NTERM,NXMAX),ko(NPOWMAX),
     &          aux(2*NPOWMAX),ko_save(NPOWMAX),xres(NXMAX),
     &          sigma(NENERGMAX),energ(NENERGMAX),deltasigma(NENERGMAX),
     &          lsq_save(NPOWMAX),fko(NPOWMAX),xext(NPARA,2),
     &          mean(NENERGMAX),err(NPOWMAX*NPOWMAX),
     &          err_save(NPOWMAX*NPOWMAX)

	logical used,incr_pow,F_test,trace_on,prot,f_test_on,f_mode_up,
     &          first_F_test,data_in,pow_in,fused,legendre,up,fl,
     &          errors
	dimension used(NPOWMAX,NTERM),fused(NPOWMAX,NTERM)
	
	character*80 command(20),atw,comfile,mask
	character*80 small_letter
	character*6 cterm(4)
	
	common /text/ cterm
	common /trace/ trace_on,prot
	
	data i_choice,iterm,unit / 1,1,5 /
	data trace_on,prot /.true.,.false./
	data f_test_on,f_mode_up,first_F_test /.false.,.true.,.false./
	data data_in,pow_in,up / .false. , .false., .false. /
	data fko / NPOWMAX*0.0 /
	data legendre /.false./
	data errors /.true./

	data cterm / 'delta', 'theta0' , 'y0    ' , 'phi0  ' /

	write(6,*)'Dimensions : mrad and cm'
	write(6,*)'Maximale Anzahl der Datenpunkte    : ',NXMAX
	write(6,*)'Maximale Anzahl der Matrixelemente : ',NPOWMAX
c
c       DEFINIERE MOEGLICHE KOMMANDOS
c
	command(1)='select'
	command(2)='data'
	command(3)='defpow'
	command(4)='fixmat'
	command(5)='fit'
	command(6)='outko'
	command(7)='printko'
	command(8)='back'
	command(9)='dcl'         
	command(10)='help'
	command(11)='plane'       ! imageplane 
	command(12)='scan'        ! image
	command(13)='project'     ! image
	command(14)='continue'
	command(15)='throwaway'
	command(16)='change'
	command(17)='execute'
	command(18)='exit'
	command(19)='continue' !oder <RETURN> Taste
	ncommand=19
c
c       KOMMANDO ABFRAGEN UND AUSFUEHREN
c
	do while (i_choice.ne.(ncommand-1))
	  write(6,'(A,$)')'DMAIN>'
	  read(unit,'(A)',err=10,end=10)atw
	  if (unit.ne.5) write(6,'(A)')atw(1:trimlen(atw))
	  goto 20
10        continue
	  close(unit)
	  unit=5
	  write(6,*)
20        continue
c
c         EINGABE IN KLEINBUCHSTABEN UMWANDELN
c
	  atw=small_letter(atw)
c
c         NUMMER DES KOMMANDOS ERMITTELN
c
	  i_choice=0
	  mode=0
	  do i=1,ncommand
	    mask=command(i)
	    mask=mask(1:trimlen(mask))
	    if (index(mask,atw(1:3)).ne.0) then
	      i_choice=i
	    end if
	  end do
	  
	  if (i_choice.eq.0) then 
	    write(6,'(A)')' ERROR : unknown expression '//atw
	    atw=' '
	  end if
c
c         KOMMANDO AUSFUEHREN
c
	  if (i_choice.eq.1) then
	    call select(iterm,cterm,f_test_on,f_mode_up,trace_on,prot,
     &                  legendre,errors,atw)
	  end if

	  if (i_choice.eq.2) then
c
c           EINLESEN DER KOORDINATENDATEN
c
	    call read_koord(x0,x,nrph,nx,nenergy,nrpe,NPARA,NTERM,NXMAX,
     &                      NENERGMAX,atw)
	    data_in=.true.
	  end if
	  if (i_choice.eq.3) then
c
c           EINGABE DER ZU VERWENDENDEN MATRIXELEMENTE
c
	    call define_powers(iterm,npow,pow,used,NPOWMAX,NPARA,NTERM,
     &                         atw)
	    pow_in=.true.
	  end if
	  if (i_choice.eq.4) then
c
c           EINGABE DER FESTZUHALTENDEN KOEFFIZIENTEN
c
	    call fix_ko(iterm,fko,fnpow,fpow,fused,NPOWMAX,NPARA,NTERM,
     &                  ko,npow,pow,used,atw)
	  end if
	  if (i_choice.eq.5) then
c
c          AUSF"UHREN DER MATRIXELEMENTBERECHNUNG
c
	   if (.not.data_in) then
	     write(6,'(1x,A)')'NO DATA HAS BEEN ENTERED!'
	     call enter_return()
	     goto 100 ! ans Ende
	   end if
	   if (.not.pow_in) then
	     write(6,'(1x,A)')'NO POWERS HAVE BEEN ENTERED!'
	     goto 100 ! ans Ende
	   end if
c   
c         FALLS F-TEST GEW"UNSCHT
c       
	   if (f_test_on) then
c
c           RETTE DEFINIERTE POTENZEN
c
	    call save_powers(iterm,npow,pow,npow_save,pow_save,
     &                        NPOWMAX,NPARA,NTERM)
c            
c           f_mode_up=.true. : F-TEST WIRD MIT STEIGENDER ZAHL VON 
c                              MATRIXELEMENTEN AUSGEF"UHRT.
c           npow=1,..,npow_save
c
	    if (f_mode_up) then
	     npow=1
	     call setactpow(iterm,npow,pow,npow_save,pow_save,used,
     &                      NPOWMAX,NPARA,NTERM) 
	     call setup_matrix(iterm,A,npow,pow,nx,x,NAMAX,NPOWMAX,NXMAX,
     &                         NPARA,NTERM,legendre)
	     call fixpart(iterm,x0,nx,x,fko,fpow,fnpow,NPOWMAX,NXMAX,
     &                    NTERM,NPARA,legendre,1)
	     call weights(iterm,A,x0,nrph,npow,nx,NAMAX,NXMAX,NTERM,1)
	     call fitko(iterm,A,x0,ko_save,npow,nx,aux,
     &                  NPOWMAX,NXMAX,NAMAX,NTERM,err_save,errors)
	     lsq_old=aux(1)
	     call weights(iterm,A,x0,nrph,npow,nx,NAMAX,NXMAX,NTERM,0)
	     call fixpart(iterm,x0,nx,x,fko,fpow,fnpow,NPOWMAX,NXMAX,
     &                    NTERM,NPARA,legendre,0)
	     lsq_save(1)=lsq_old
c             
c           SCHLEIFE UEBER ALLE EINGEGEBENEN MATRIXELEMENTE
c
	     npow=2
	     do n=2,npow_save
	      write(6,*)'Number of elements =',npow_save
	      write(6,*)'Processed elements =',n 
	      write(6,*)'Accepted elements  =',npow

c
c           MATRIXELEMENT n HINZUNEHMEN (TESTEN)
c
	      write(6,*)'TESTING MATRIXELEMENT :'
	      used(n,iterm)=.true.
	      write(6,'(1x,4I2)')(pow_save(j,n,iterm),j=1,4)
c
c           SETZE AKTUELLE POTENZEN F"UR FIT
c                       
	      call setactpow(iterm,npow,pow,npow_save,pow_save,used,
     &                       NPOWMAX,NPARA,NTERM)       
c
c           BESETZE MATRIX MITTELS AKTUELLEN POTENZEN
c
	     call setup_matrix(iterm,A,npow,pow,nx,x,NAMAX,NPOWMAX,
     &                          NXMAX,
     &                          NPARA,NTERM,legendre)
	      call fixpart(iterm,x0,nx,x,fko,fpow,fnpow,NPOWMAX,NXMAX,
     &                     NTERM,NPARA,legendre,1)
	      call weights(iterm,A,x0,nrph,npow,nx,NAMAX,NXMAX,NTERM,1)
c
c           BESTIMME KOEFFIZIENTEN BILDEBENE-TARGET MIT FIT
c
	      call fitko(iterm,A,x0,ko,npow,nx,aux,
     &                   NPOWMAX,NXMAX,NAMAX,NTERM,err,errors)
	      lsq=aux(1)
	      call weights(iterm,A,x0,nrph,npow,nx,NAMAX,NXMAX,NTERM,0)
	      call fixpart(iterm,x0,nx,x,fko,fpow,fnpow,NPOWMAX,NXMAX,
     &                     NTERM,NPARA,legendre,0)
	      lsq_save(n)=lsq

	      if (trace_on) then
	       call outko(iterm,ko,npow,pow,lsq,nx,used,npow_save,
     &              pow_save,NPOWMAX,NPARA,NTERM,legendre,err,up,atw)
	       write(6,*)'SETUP_RES :'
	       call setup_res(iterm,xres,x,nx,npow,pow,ko,fnpow,fpow,fko,
     &                        NPOWMAX,NXMAX,NPARA,NTERM,legendre)
	       write(6,*)'CALCSIGMA :'
	       call calcsigma(iterm,x0,x,xres,nenergy,nrpe,energ,sigma,
     &                        deltasigma,NTERM,NPARA,NXMAX,NENERGMAX,up)
	       call outsigma(iterm,nenergy,energ,sigma,deltasigma,
     &                       NENERGMAX,atw)
	      end if

	      incr_pow=F_test(lsq,lsq_old,npow,nx,first_F_test)
	      if (incr_pow) then
	       used(n,iterm)=.true.
	       do i=1,npow
		ko_save(i)=ko(i)
	       end do
	       do i=1,npow*npow
		err_save(i)=err(i)
	       end do
	       lsq_old=lsq
	       npow=npow+1
	      else
	       used(n,iterm)=.false.
	      end if
	     end do
	     npow=npow-1
	     lsq=lsq_old
	     do i=1,npow
	      ko(i)=ko_save(i)
	      print*, ko(i)
	     end do
	     do i=1,npow*npow
	      err(i)=err_save(i)
	     end do
	     call setactpow(iterm,npow,pow,npow_save,pow_save,used,
     &                      NPOWMAX,NPARA,NTERM)       
	    
	     call outko(iterm,ko,npow,pow,lsq,nx,used,npow_save,
     &            pow_save,NPOWMAX,NPARA,NTERM,legendre,err,up,atw)
	     write(6,'(A)')'*********** OVERVIEW *************'
	     write(6,*)' '
	     write(6,'(A)')' Number, powers and lsq:'
	     do i=1,npow_save
	      write(6,'(1x,I3,4I2,D15.8)')i,(pow_save(j,i,iterm),j=1,4),
     &                                    lsq_save(i)
	     end do
	    
	    else
	     npow=npow_save
	     call setactpow(iterm,npow,pow,npow_save,pow_save,used,
     &                      NPOWMAX,NPARA,NTERM) 
	     call setup_matrix(iterm,A,npow,pow,nx,x,NAMAX,NPOWMAX,NXMAX,
     &                         NPARA,NTERM,legendre)
	     call fixpart(iterm,x0,nx,x,fko,fpow,fnpow,NPOWMAX,NXMAX,
     &                    NTERM,NPARA,legendre,1)
	     call weights(iterm,A,x0,nrph,npow,nx,NAMAX,NXMAX,NTERM,1)
	     call fitko(iterm,A,x0,ko_save,npow,nx,aux,
     &                  NPOWMAX,NXMAX,NAMAX,NTERM,err_save,errors)
	     lsq_old=aux(1)
	     call weights(iterm,A,x0,nrph,npow,nx,NAMAX,NXMAX,NTERM,0)
	     call fixpart(iterm,x0,nx,x,fko,fpow,fnpow,NPOWMAX,NXMAX,
     &                    NTERM,NPARA,legendre,0)
	     npow=npow_save-1
c             
c           SCHLEIFE UEBER ALLE EINGEGEBENEN MATRIXELEMENTE
c
	     do n=npow_save,1,-1
	      write(6,*)'Number of elements =',npow_save
	      write(6,*)'Processed elements =',n 
	      write(6,*)'Accepted elements  =',npow

c
c           MATRIXELEMENT n AUSMASKIEREN (TESTEN)
c
	      write(6,*)'TESTING MATRIXELEMENT :'
	      used(n,iterm)=.false.
	      write(6,'(1x,4I2)')(pow_save(j,n,iterm),j=1,4)
c
c           SETZE AKTUELLE POTENZEN F"UR FIT
c                       
	      call setactpow(iterm,npow,pow,npow_save,pow_save,used,
     &                       NPOWMAX,NPARA,NTERM)       
c
c           BESETZE MATRIX MITTELS AKTUELLEN POTENZEN
c
	      call setup_matrix(iterm,A,npow,pow,nx,x,NAMAX,NPOWMAX,
     &                          NXMAX,
     &                          NPARA,NTERM,legendre)
	      call fixpart(iterm,x0,nx,x,fko,fpow,fnpow,NPOWMAX,
     &                     NXMAX,
     &                     NTERM,NPARA,legendre,1)
	      call weights(iterm,A,x0,nrph,npow,nx,NAMAX,NXMAX,NTERM,1)
c
c           BESTIMME KOEFFIZIENTEN BILDEBENE-TARGET MIT FIT
c
	      call fitko(iterm,A,x0,ko,npow,nx,aux,
     &                   NPOWMAX,NXMAX,NAMAX,NTERM,err,errors)
	      lsq=aux(1)
	      call weights(iterm,A,x0,nrph,npow,nx,NAMAX,NXMAX,NTERM,0)
	      call fixpart(iterm,x0,nx,x,fko,fpow,fnpow,NPOWMAX,NXMAX,
     &                     NTERM,NPARA,legendre,0)
	      lsq_save(n)=lsq

	      if (trace_on) then
	       call outko(iterm,ko,npow,pow,lsq,nx,used,npow_save,
     &              pow_save,NPOWMAX,NPARA,NTERM,legendre,err,up,atw)
	       write(6,*)'SETUP_RES :'
	       call setup_res(iterm,xres,x,nx,npow,pow,ko,fnpow,fpow,fko,
     &                        NPOWMAX,NXMAX,NPARA,NTERM,legendre)
	       write(6,*)'CALCSIGMA :'
	       call calcsigma(iterm,x0,x,xres,nenergy,nrpe,energ,sigma,
     &                        deltasigma,NTERM,NPARA,NXMAX,NENERGMAX,up)
	       call outsigma(iterm,nenergy,energ,sigma,deltasigma,
     &                       NENERGMAX,atw)
	      end if

	      incr_pow=F_test(lsq_old,lsq,npow,nx,first_F_test)
	      if (incr_pow) then
	       used(n,iterm)=.true.
	      else
	       do i=1,npow
		ko_save(i)=ko(i)
	       end do
	       do i=1,npow*npow
		err_save(i)=err(i)
	       end do
	       npow=npow-1 
	       lsq_old=lsq
	      end if
	     end do
	     npow=npow+1
	     lsq=lsq_old
	     do i=1,npow
	      ko(i)=ko_save(i)
	      print*, ko(i)
	     end do
	     do i=1,npow*npow
	      err(i)=err_save(i)
	     end do
	     call setactpow(iterm,npow,pow,npow_save,pow_save,used,
     &                      NPOWMAX,NPARA,NTERM)       
	    
	     call outko(iterm,ko,npow,pow,lsq,nx,used,npow_save,
     &            pow_save,NPOWMAX,NPARA,NTERM,legendre,err,up,atw)
	     write(6,'(A)')'*********** OVERVIEW *************'
	     write(6,*)' '
	     write(6,'(A)')' Number, powers and lsq:'
	     do i=1,npow_save
	      write(6,'(1x,I3,4I2,D15.8)')i,(pow_save(j,i,iterm),j=1,4),
     &                                    lsq_save(i)
	     end do
	    end if
	   else
c
c     FALLS KEIN F-TEST GEW"UNSCHT
c
	    call save_powers(iterm,npow,pow,npow_save,pow_save,
     &                       NPOWMAX,NPARA,NTERM)
	    call setactpow(iterm,npow,pow,npow_save,pow_save,used,
     &                     NPOWMAX,NPARA,NTERM) 
	    call setup_matrix(iterm,A,npow,pow,nx,x,NAMAX,NPOWMAX,NXMAX,
     &                        NPARA,NTERM,legendre)
	    call fixpart(iterm,x0,nx,x,fko,fpow,fnpow,NPOWMAX,NXMAX,
     &                   NTERM,NPARA,legendre,1)
	    call weights(iterm,A,x0,nrph,npow,nx,NAMAX,NXMAX,NTERM,1)
	    call fitko(iterm,A,x0,ko,npow,nx,aux,
     &                 NPOWMAX,NXMAX,NAMAX,NTERM,err,errors)
	    lsq=aux(1)
	    call weights(iterm,A,x0,nrph,npow,nx,NAMAX,NXMAX,NTERM,0)
	    call fixpart(iterm,x0,nx,x,fko,fpow,fnpow,NPOWMAX,NXMAX,
     &                   NTERM,NPARA,legendre,0)
	    call outko(iterm,ko,npow,pow,lsq,nx,used,npow_save,
     &         pow_save,NPOWMAX,NPARA,NTERM,legendre,err,up,atw)
	   end if
100        continue 
	  end if
	  if (i_choice.eq.6) call outko(iterm,ko,npow,pow,lsq,nx,
     &         used,npow_save,pow_save,NPOWMAX,NPARA,NTERM,
     &         legendre,err,up,atw)
	  if (i_choice.eq.7) call printko(iterm,ko,npow,pow,lsq,nx,used,
     &         npow_save,pow_save,NPOWMAX,NPARA,NTERM,legendre,err,
     &         up,atw)
	  if (i_choice.eq.8) then
	    call read_ko(iterm,ko,npow,pow,used,NPARA,NPOWMAX,NTERM,
     &                   atw)
	    if (npow.gt.0) then
	      call setup_res(iterm,xres,x,nx,npow,pow,ko,fnpow,fpow,fko,
     &                       NPOWMAX,NXMAX,NPARA,NTERM,legendre)
	      call calcmean(iterm,x0,x,xres,nenergy,nrpe,energ,mean,
     &                      NTERM,NPARA,NXMAX,NENERGMAX,up)
c              call plotmean(iterm,energ,mean,nenergy,NENERGMAX)
	      call outmean(iterm,nenergy,energ,mean,NENERGMAX,atw)
	      call calcsigma(iterm,x0,x,xres,nenergy,nrpe,energ,sigma,
     &                       deltasigma,NTERM,NPARA,NXMAX,NENERGMAX,up)
c             call plotsigma(iterm,energ,sigma,nenergy,NENERGMAX)
	      call outsigma(iterm,nenergy,energ,sigma,deltasigma,
     &                     NENERGMAX,atw)
	      call pltres(iterm,x0,xres,nx,NTERM,NXMAX)
	    end if
	  end if
          if (i_choice.eq.9) then 
            call dcl
          end if
	  if (i_choice.eq.10) then 
	     write(6,*)'THERE IS NO HELP.'
	  end if
	 if (i_choice.eq.11) then
	   call imageplane(x0,x,nenergy,nrpe,NTERM,NPARA,NXMAX,
     &                     NENERGMAX,atw)
	 end if
	  if (i_choice.eq.12) call scanimage(x,xext,nx,NPARA,NXMAX)
	  if (i_choice.eq.13) call scaleimagedown(x,xext,nx,NPARA,NXMAX)
	  
	  if (i_choice.eq.15) then 
	    errors=.true.
	    call throwaway(iterm,ko,npow,used,
     &                         NPOWMAX,NTERM,nx,lsq,err,npow_save)
	    call setactpow(iterm,npow,pow,npow_save,pow_save,used,
     &                     NPOWMAX,NPARA,NTERM) 
	    if (npow.ne.0) then
	    call setup_matrix(iterm,A,npow,pow,nx,x,NAMAX,NPOWMAX,NXMAX,
     &                        NPARA,NTERM,legendre)
	    call fixpart(iterm,x0,nx,x,fko,fpow,fnpow,NPOWMAX,NXMAX,
     &                   NTERM,NPARA,legendre,1)
	    call weights(iterm,A,x0,nrph,npow,nx,NAMAX,NXMAX,NTERM,1)
	    call fitko(iterm,A,x0,ko,npow,nx,aux,
     &                 NPOWMAX,NXMAX,NAMAX,NTERM,err,errors)
	    lsq=aux(1)
	    call weights(iterm,A,x0,nrph,npow,nx,NAMAX,NXMAX,NTERM,0)
	    call fixpart(iterm,x0,nx,x,fko,fpow,fnpow,NPOWMAX,NXMAX,
     &                   NTERM,NPARA,legendre,0)
	    call outko(iterm,ko,npow,pow,lsq,nx,used,npow_save,
     &         pow_save,NPOWMAX,NPARA,NTERM,legendre,err,up,atw)
	    end if
	  end if
	  if (i_choice.eq.16) then
	    call exchange(x0,x,nx,NTERM,NPARA,NXMAX,up)
	  end if
	  if (i_choice.eq.17) then
	    mask='/file='
	    mode=1
	    call interpret(atw,mask,mode,comfile,fl)
	    if (fl) then
	      unit=65
	      open(unit=unit,file=comfile,status='OLD',iostat=ios)
	      if (ios.gt.0) then
		write(6,*)' EXECUTE : FILE NOT FOUND'
		unit=5
	      end if
	    else
	      write(6,*)' EXECUTE : SYNTAX ERROR - FILE NOT SPECIFIED'
	    end if
	  end if
	end do
	stop
	end

