	
	logical function F_test(lsq,lsq_old,npow,nx,firstcall)
c
c       MACHT EINEN F_TEST
c
	integer npow,nx,nfree
	real*8 fvalue
	real*8 lsq,lsq_old,Fx,F
	logical firstcall,trace_on,prot
	common /trace/ trace_on,prot

	nfree=nx-npow-1
	F=fvalue(nfree)
	if (firstcall) then 
	  F_test=.true.
	  firstcall=.false.
	else
	  if (lsq.gt.0.) then
	    Fx=(lsq_old/lsq-1.)*nfree
	    if (Fx.ge.F) then
	      F_test=.true.
	    else
	      F_test=.false.
	    end if
	  else 
	    F_test=.false.                              
	  end if
	end if
	if (trace_on) then
	  write(6,'(1x,A)')'Subroutine F_TEST : '
	  write(6,'(1x,A,E12.5)')'Fx=',Fx
	  write(6,'(1x,A,E12.5)')'F =',F 
	end if
	if (F_test) then
	  write(6,*)'F-Test positive , parameter accepted.'
	else
	  write(6,*)'F-Test negative , parameter rejected.'
	end if
	return
	end

c
c~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
c
	real*8 function fvalue(nfree)
c
c       BERECHNET DURCH INTERPOLATION ZWISCHEN TABELLIERTEN F-WERTEN
c       DEN AKTUELLEN F-WERT FUER ANGEGEBENEN FREIHEITSGRAD
c       DIE HIER TABELLIERTEN F-WERTE SIND BEVINGTON ENTNOMMEN UND
c       SIND FUER EINE WAHRSCHEINLICHKEIT VON x % GROESSERE F-WERTE
c       ZU BEOBACHTEN, FALLS DIESER LETZTE PARAMETER NICHT GEBRAUCHT 
c       WIRD. 
	implicit none
c
c       npts     : Anzahl der Datenpunkte                      
c       nterms   : Anzahl der Terme fuer Interpolation (z.B.3:2te Ord.)
c       n_tab    : tabellierte Freiheitsgrade                  
c       F_tab    : tabellierte F-Werte                         
c       result   : interpolierte Datenpunkte                   
c
c
c
c
	integer npts,nterms
	parameter (npts=20,nterms=2)
	integer nfree
	real*8 n_tab(npts),F_tab(npts),result

	data n_tab / 1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,11.,12.,15.,20.,24.,
     &            30.,40.,60.,120.,1000000./

	include 'f_test.inc'

	call interp(n_tab,F_tab,npts,nterms,dble(nfree),result)
	fvalue = result
c
	return
	end



	subroutine interp(x,y,npts,nterms,xin,yout)

c        purpose
c          interpolate between data points to evaluate a function
c          (Bevington p.266)
c        x      - array of data points for independent variable
c        y      - array of data points for dependant variable
c        npts   - number of pairs of data points
c        nterms - number of terms in fitting polynamial
c        xin    - input value of x
c        yout   - interpolated value
c
	implicit none

	integer npts,nterms,i,i1,i2,ix,imax,ixmax,j,k
	
	real*8 x,y,xin,yout,denom
	dimension x(npts),y(npts)

	real*8 deltax,delta,a,prod,sum
	dimension delta(10),a(10)

c
c       search for appropriate value of x(1)
c
	do 19 i=1,npts
	if (xin-x(i).lt.0) goto 13
	if (xin-x(i).eq.0) goto 17
	if (xin-x(i).gt.0) goto 19
13      i1=i-nterms/2
	if (i1.lt.0) goto 15
	if (i1.eq.0) goto 15
	if (i1.gt.0) goto 21
15      i1=1
	goto 21
17      yout=y(i)
	goto 61
19      continue
	i1=npts-nterms+1
21      i2=i1+nterms-1
	if (npts-i2.lt.0) goto 23
	if (npts-i2.eq.0) goto 31
	if (npts-i2.gt.0) goto 31
23      i2=npts
	i1=i2-nterms+1
	if(i1.lt.0) goto 26
	if(i1.eq.0) goto 26
	if(i1.gt.0) goto 31
26      i1=1
	nterms=i2-i1+1
c
c       evaluate deviations delta
c
31      denom=x(i1+1)-x(i1)
	deltax=(xin-x(i1))/denom
	do 35 i=1,nterms
	ix=i1+i-1
35      delta(i)=(x(ix)-x(i1))/denom
c
c       accumulate coefficients a
c
	a(1)=y(i1)
	do 50 k=2,nterms
	prod=1.
	sum=0.
	imax=k-1
	ixmax=i1+imax
	do 49 i=1,imax
	j=k-i
	prod=prod*(delta(k)-delta(j))
49      sum=sum-a(j)/prod
50      a(k)=sum+y(ixmax)/prod
c
c       accumulate sum of expansion
c
	sum=a(1)
	do 57 j=2,nterms
	prod=1.
	imax=j-1
	do 56 i=1,imax
56      prod=prod*(deltax-delta(i))
57      sum=sum+a(j)*prod
	yout=sum
61      return
	end
