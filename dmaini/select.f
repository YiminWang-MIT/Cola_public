
	subroutine select(iterm,cterm,ftest,fmode,trace,prot,legendre,
     &                    errors,atw)

	implicit none

	integer i,iterm,trimlen,iterm_save,
     &          mode
	character cterm*6,atw*(*),mask*20,st*80
	dimension cterm(4)
	logical ftest,fmode,trace,prot,legendre,fl,errors
C
C       INTERPRETIEREN DES KOMMANDOS ATW
C
	iterm_save=iterm
	iterm=0

	mode=1
	mask='/coordinate'
	call interpret(atw,mask,mode,st,fl)
	if (fl) then
	  do i=1,4
	    if (index(cterm(i),st(1:trimlen(st))).ne.0) then
	      iterm=i
	    end if
	  end do
	  if (iterm.eq.0) then
	    write(6,'(A)')' ERROR in subroutine SELECT: '//
     &                    'SYNTAX ERROR coordinate'
	    iterm=iterm_save
	    return
	  end if
	end if

	mode=1
	mask='/ftest'
	call interpret(atw,mask,mode,st,fl)
	if (fl) then
	  if (st(1:trimlen(st)).eq.'on') then
	    ftest=.true.
	  else if (st(1:trimlen(st)).eq.'off') then
	    ftest=.false.
	  else
	    write(6,'(A)')' ERROR in subroutine SELECT: '//
     &                    'SYNTAX ERROR ftest'
	      
	   end if
	end if

	mode=1
	mask='/fmode'
	call interpret(atw,mask,mode,st,fl)
	if (fl) then
	  if (st(1:trimlen(st)).eq.'up') then
	    fmode=.true.
	  else if (st(1:trimlen(st)).eq.'down') then
	    fmode=.false.
	  else
	    write(6,'(A)')' ERROR in subroutine SELECT: '//
     &                    'SYNTAX ERROR fmode'
	      
	   end if
	end if

	mode=1
	mask='/legendre'
	call interpret(atw,mask,mode,st,fl)
	if (fl) then
	  if (st(1:trimlen(st)).eq.'on') then
	    legendre=.true.
	  else if (st(1:trimlen(st)).eq.'off') then 
	    legendre=.false.
	  else
	    write(6,'(A)')' ERROR in subroutine SELECT: '//
     &                    'SYNTAX ERROR legendre'
	      
	   end if
	end if

	mode=1
	mask='/trace'
	call interpret(atw,mask,mode,st,fl)
	if (fl) then
	  if (st(1:trimlen(st)).eq.'on') then
	    trace=.true.
	  else if (st(1:trimlen(st)).eq.'off') then 
	    trace=.false.
	  else
	    write(6,'(A)')' ERROR in subroutine SELECT: '//
     &                    'SYNTAX ERROR trace'
	      
	   end if
	end if

	mode=1
	mask='/prot'
	call interpret(atw,mask,mode,st,fl)
	if (fl) then
	  write(6,*)'SELECT : prot currently not available'
	  if (st(1:trimlen(st)).eq.'on') then
	    prot=.true.
	  else if (st(1:trimlen(st)).eq.'off') then
	    prot=.false.
	  else
	    write(6,'(A)')' ERROR in subroutine SELECT: '//
     &                    'SYNTAX ERROR prot'
	      
	   end if
	end if
	
	mode=1
	mask='/errors'
	call interpret(atw,mask,mode,st,fl)
	if (fl) then
	  if (st(1:trimlen(st)).eq.'on') then
	    errors=.true.
	  else if (st(1:trimlen(st)).eq.'off') then 
	    errors=.false.
	  else
	    write(6,'(A)')' ERROR in subroutine SELECT: '//
     &                    'SYNTAX ERROR errors'
	      
	   end if
	end if

	if (iterm.eq.0) iterm=iterm_save
	write(6,'(A)')' SELECT :'
	write(6,'(A)')'          coordinate : '//cterm(iterm)
	write(6,'(A,L1)')'          ftest    = ',ftest
	write(6,'(A,L1)')'          f_mode_up= ',fmode
	write(6,'(A,L1)')'          trace    = ',trace
	write(6,'(A,L1)')'          prot     = ',prot
	write(6,'(A,L1)')'          legendre = ',legendre
	write(6,'(A,L1)')'          errors   = ',errors
	return
	end
