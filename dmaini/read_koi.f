	
	subroutine read_ko(iterm,ko,npow,pow,used,NPARA,
     &              NPOWMAX,NTERM,atw)

	implicit none
	integer NPARA,NPOWMAX,NTERM
	integer iterm,npow,pow,trimlen,ios,mode
	dimension pow(NPARA,NPOWMAX,NTERM)

	real*8 ko
	dimension ko(NPOWMAX)

	logical used,file_in,fl
	dimension used(NPOWMAX,NTERM)
	
	character infile*80,dum*1,atw*(*),mask*20,st*80

	file_in=.false.

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

	write(6,*)' '
	write(6,*)'READ_KO :'

	if (file_in) then
	  open(88,file=infile,status='OLD',iostat=ios)
	  if (ios.gt.0) then
	    write(6,'(A)')' ERROR in subroutine READ_KO: '//
     &                 ' FILE NOT FOUND '//infile(1:trimlen(infile))
	    npow=0
	    return
	  end if

	  read(88,'(A)')dum
	  read(88,'(A)')dum
	  npow=0
	  do while (.true.)
		npow=npow+1
		read(88,*,err=20)ko(npow),used(npow,iterm),
     &          pow(1,npow,iterm),pow(2,npow,iterm),
     &          pow(3,npow,iterm),pow(4,npow,iterm)
		write(6,120,err=20)npow,ko(npow),used(npow,iterm),
     &          pow(1,npow,iterm),pow(2,npow,iterm),
     &          pow(3,npow,iterm),pow(4,npow,iterm)
	  end do
20        continue
	  npow=npow-1
	  close(88)
	else
	  write(6,*)'take actual matrixelements for backtracing'
	end if
120     format(1x,I3,1x,D15.8,1x,L1,1x,4I2)
	return
	end

