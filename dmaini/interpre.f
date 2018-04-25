	subroutine interpret(atw,mask,mode,st,fl)
C
C       INTERPRETIEREN DES KOMMANDOS ATW
C
C       atw : Kommandostring
C       mask: Maske nach der gesucht werden soll. 
C             mask hat die Form '/KENNUNG' (mode=0) oder 
C             '/KENNUNG="STRING"' (mode=1)
C       mode: 
C            a) mode=0 
C               Suche nach der Maske mask im string atw. Wird mask
C               gefunden, so ist fl=.true. bei der Rueckgabe.
C            b) mode=1
C               Wie mode=0, jedoch wird zus"atzlich nach dem '='
C               ein String erwartet. Dieser String beginnt nach dem 
C               ersten " das auf das = folgt und endet beim zweiten ".
C               Wird das zweite " nicht angegeben, so endet der einge-
C               lesene String mit dem Ende von atw.
C               Werden keine "" angegeben, so beginnt der String nach 
C               dem = und endet beim n"achsten / oder dem Ende von atw.
C       st  : Eingelesener String
C       fl  : Flagge die anzeigt, ob die angegebene Maske gefunden 
C             wurde und die Syntax eingehalten wurde.
	
	implicit none

	integer mode,smask,lmask,i,k,l,m,n,trimlen,searchchar,slashpos
	character atw*(*),st*(*),mask*(*),msk*4
	logical fl
	
	fl=.false.
	lmask=trimlen(mask)
	smask=slashpos(lmask,mask)
	msk(1:3)=mask((smask+1):(smask+3))

	k=index(atw(1:len(atw)),msk(1:3))
	if (k.ne.0) then
	  if (mode.eq.0) then
	    fl=.true.
	    return
	  else if (mode.eq.1) then
	    i=k+lmask+2
	    l=searchchar(k,i,atw,'=')+1
	    if (l.gt.1) then
	      fl=.true.
	      m=searchchar(l,trimlen(atw),atw,'/')-1
	      if (m.le.0) m=trimlen(atw)
	      n=searchchar(l,m,atw,'"')
	      if (n.gt.0) then
		l=n+1
		n=searchchar(l,trimlen(atw),atw,'"')
		if (n.gt.0) then
		  m=n-1
		else
		  m=trimlen(atw)
		end if
	      end if
	      st=atw(l:m)
	    else 
	      st=' '
	    end if        
	  end if
	else 
	  fl=.false.
	end if
	return
	end
