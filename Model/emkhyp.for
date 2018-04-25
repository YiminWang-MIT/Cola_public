c=======================================================================
c                      Program   EMKHYP
c          (Electro-Magnetic production of Kaon-HYPeron)
c-----------------------------------------------------------------------
c  Calculates cross sections, spin observables, and response functions 
c  for the photo- and electroproduction of Lambda and Sigma hyperons 
c  off nucleons using isobaric models (Kaon-MAID, Saclay-Lyon,..)
c    version: 30. 11. 2009 
c    Petr Bydzovsky  bydz@ujf.cas.cz  
c-----------------------------------------------------------------------
c  CHANNELS (controlled by 'ichnn'):
c 
c    ichnn = 1 :    gamma  +  p ---> K^+  +  Lambda
c    ichnn = 2 :    gamma  +  n ---> K^0  +  Lambda
c    ichnn = 3 :    gamma  +  p ---> K^+  +  Sigma^0
c    ichnn = 4 :    gamma  +  n ---> K^0  +  Sigma^0
c    ichnn = 5 :    gamma  +  n ---> K^+  +  Sigma^-
c    ichnn = 6 :    gamma  +  p ---> K^0  +  Sigma^+
c
c  Note: Coupling constants read from files 'model.*' are 
c        supposed to be those for the channel 1 -L (Lambda) version  
c        and 3 -S (Sigma) version. Isospin SU(2) symmetry and ratia 
c        of the electro-magnetic couplings for the neutral to charged 
c        modes of resonances are used to gain the coupling constants 
c        for the other channels. The ratia are read from 'model.*'.
c
c  HADRONIC FORM FACTORS  (controlled by 'ihff'):
c
c    ihff = 0  NO h.f.f. is used
c    ihff = 1  a global h.f.f. is used
c    ihff = 2  Bennhold et al, nucl-th/9901066
c    ihff = 3  Davidson and Workman, PRC63(2001)025210 (one cut-off)
c    ihff = 4  Davidson and Workman (independent cut-offs for background, 
c              i.e.Born, K*, K1,and Y* terms, and resonant, N*, terms)
c    ihff = 5  Gaussian shape, othervise as in ihff= 4
c
c  ELECTROMAGNETIC FORM FACTORS (controlled by 'iemff'):
c
c    iemff = 0  no electromagnetic form factors
c    iemff = 1  GK92 W A A (model SL, SLA)
c    iemff = 2  GK92 W W W no meson widths assumed
c    iemff = 3  GK92 W W W meson widths assumed
c    iemff = 4  model by WJC Phys.Rev.C46(1992)1617 
c              (questionable definition of the em.f.f.!)
c    iemff = 5 phenomenological, masses read from files 'model.xxx'
c
c  Note: GK92 is for Extended Vector Meson Dominance Model by 
c        Gari&Kruempelman, Phys.Rev.D45(1992)1817.
c        W is for VMD (kaon) and EVMD (K*,K1) by Williams et al;
c        Phys.Rev.C46(1992)1617.
c        A is for em.f.f by Adelsek-Wright Phys.Rev.C38(1988)1965. 
c
c  INPUT VARIABLES (controlled by 'idata', 'irun', and 'ivar'):
c    idata= 1  following variables can vary:
c              irun = 1  E_g^lab, s, W                    (photo, phtot)
c                     2  th_K^cm, cos(th_K^cm), th_K^lab, t      (photo)
c                     3  Q^2                         (electroproduction)
c                     4  W                           (electroproduction)
c                     5  th_K^cm, t, cos(th_K^cm)    (electroproduction)
c                     6  Ei, Ef,th_e^lb,th_k^cm, phi (electroproduction)
c                      (ivar= 4; particular data points with given number)
c    idata= 2  individual kinematics is read from kin.input 
c
c  Notes: for irun=2 and kaon lab angle - if there are two solutions 
c         for theta_K^cm (two kaon lab momenta) the input is ambiguous 
c         and solution of the forward c.m. kaon angle is chosen.
c         for irun= 3,4,5   ivar= 1 - thkcm,  2 - t, and  3 - cos(thkcm)
c         for irun= 6, ivar= 4 and imod= 3; calculation at given data points
c  
c  Photoproduction:
c    px1 = Eg1 (E_g^lab), s, W
c    px2 = th0 (th_K^cm), ct0 (cos(th_K^cm)), th1 (th_K^lab), t
c    units are GeV, GeV^2, deg
c
c  Electroproduction (in 'thele' kinematics is controlled by 'ivar'):
c    ivar=  1           2        3              4
c    -----------------------------------------------------
c    px1 =  W           W        W             Ebeam
c    px2 =  Q^2         Q^2      Q^2           E_e'
c    px3 =  epsilon     epsilon  epsilon       theta_e^lab
c    px4 =  theta_K^cm  t        cos(th_K^cm)  theta_K^cm
c    px5 =  phi_K       phi_K    phi_K         phi_K
c    -----------------------------------------------------
c    units are GeV (W, Ebeam, E_e'), (GeV/c)^2 (Q^2, t), and deg
c
c OUTPUT (controlled by 'iprint', 'a3' and 'imod')
c  iprint = 0  only basic information from 'mdinpt'
c           1  emkhyp.out1 and emkhyp.out2(data for figures)
c           2  emkhyp.out1(with more details) and emkhyp.out2  
c 
c  Photoproduction -permissible values of 'a3' (proc= photo): 
c     -cross section in CMS (S0), LAB (S1), and invariant (ICS)
c     -polarization (P), beam (Sg) and target (T) asymmetry
c     -CMS cross section and spin observables (SP)
c     -CMS, LAB, and ICS cross sections and polarization (CR)
c     -total cross section (proc= phtot)
c     -various amplitudes and t-matrices are printed for iprint >1
c
c  Electroproduction (irun= 3-6): 
c     -cross sections:      sigma_U(T),  sigma_L, 
c                           sigma_P(TT), sigma_I(LT) 
c      unseparated:  s_UL = s_U + eps * s_L 
c      full: s_K = s_U + eps * s_L + eps * cos(2*phi) * s_P + 
c                  + sqrt(2*eps*(eps+1)) * cos(phi) * s_I     
c      triple: s_eK = gflx * s_K
c
c   imod = 0  T, L, TT, LT, UL, and s_K crs in CMS  (nb/sr)
c        = 1  T, L, TT, LT, UL, and s_K crs in LAB  (nb/sr)
c        = 2  T, L, TT, LT, UL, and s_K invariant crs  (mub/GeV^2)
c        = 3 (only for ivar= 4) s_K and s_eK  crs in LAB (nb/sr)
c        = 4  response functions (1-18)
c        = 5  polarization, photon(Sigma) and target(T) asymmetry
c-----------------------------------------------------------------
      implicit none
      integer iprint,idata
      character*16 versn,inptd,model,kindat
      character*17 ax

      versn='30. 11. 2009'
      write(*,*)'  *******************'
      write(*,*)'  * program  EMKHYP *'
      write(*,*)'  *******************'
      write(*,*)'--version: ',versn
c
c  reading input
c
      inptd= 'emkhyp.in'
      write(*,100) inptd
 100  format(1x,'--input from ',a16)
      open(1,file=inptd,status='old')
      read(1,*)
      read(1,*)
      read(1,52)ax,model
      read(1,54)ax,iprint
      read(1,54)ax,idata
      read(1,52)ax,kindat
      read(1,*)
 52   format(a17,1x,a16)
 54   format(a17,i10)
      if(iprint.lt.0) iprint=0
c
c  output files
c
      open( 8,file='emkhyp.out1',status='unknown')
      open( 9,file='emkhyp.out2',status='unknown')

      write(8,200)
      write(8,210)versn
      write(8,220)
 200  format(78(1h-))
 210  format(1x,'Program EMKHYP, version: ',a16)
 220  format(1x,36(1h-))
c
c  defining model parameters and writting out model description
c
      call mdinpt(model,iprint)
c
c  calculating observables
c
      if(idata.eq.1) then 
        call crsct
        write(*,*)'--calculation was successfully terminated'
      else if(idata.eq.2) then
        call fcn0(kindat)
        write(*,*)'--calculation was successfully terminated'
      else
        write(*,*)'#main: wrong control parameter idata: ',idata
        write(*,*)'#main: calculation terminated'
      endif

      close(9)
      close(8)
      close(1)
      STOP
      END
c
c-----------------------------------------------------------------------
c
      subroutine crsct
      implicit none
      integer iprint,parita(16),spin(16),ispin(16),chres(16),nmbr,npnt,
     +        mpoi,ichnn,iflag,i,irun,nx,nvr,ieg,iint,imod,ivar,iobs
      character*2 nck(16),vmd
      character*5 proc,akom,arun
      character*3 a1,a2,a3,aa
      character*9 ctr
      character*16 exdata
      character*60 comm
      real*8 pi,hc,alfa,rmp,rmn,rmnx,rml,rms0,rmsp,rmsm,rmhx,rmhd,rmk,
     +   rmk0,rmkx,rmres(16),rmks0,egl,egli,xmin,xmax,xinc,q2,w,thkcm,
     +   thkcmi,eps,phi,ths(18),sum,xcos(24),wght(24),px2,thkx,ebeam,
     +   efin,thte1,thtk0

      common/param/ pi,hc,alfa,iprint
      common/rmass/ rmp,rmn,rmnx,rml,rms0,rmsp,rmsm,rmhx,rmhd,rmk,rmk0,
     +              rmkx,rmres,rmks0
      common/resns/ parita,spin,ispin,chres,nck,nmbr,vmd

      npnt= 24
      mpoi= 200
c
c  channel is chosen 
c
      read(1,1000) akom,ichnn
 1000 format(a5,i3)
      if(ichnn.lt.1.or.ichnn.gt.6) then
        write(*,*)'#crsct: wrong value of the channel number: ',ichnn
        ichnn= 1
	write(*,*)'##channel set to: ',ichnn
      end if
      if(vmd.eq.' L'.and.ichnn.gt.2) then
        write(*,1010) ichnn,vmd
	ichnn = 1
	write(*,*)'##channel set to: ',ichnn
      end if
      if(vmd.eq.' S'.and.ichnn.lt.3) then
        write(*,1010) ichnn,vmd
	ichnn = 3
	write(*,*)'##channel set to: ',ichnn
      end if
 1010 format(1x,'#crsct: the channel number inconsistent with',
     +       ' the model version ',i4,2x,a2)
c
c  channel parameters are prepared
c
      iflag= 3
      call channel(ichnn,iflag)
c
c---------------------------------------------------------------
c
c  determination of kinematics and calculation
c
      read(1,1000) akom,irun
      read(1,*)
      do i= 1,18
        ths(i) = 0.d0
      end do
c---------------------------------------------------------------
      if(irun.eq.1) then
c
c  case  E_gamma^lab (Eg1), s (s) or W (W)  varies (photo,phtot)
c  output: invariant, c.m., and LAB cross sections (ICS, S0, S1)
c          polarization (P) and other spin observables (SP)
c
        read(1,2100) proc,a1,xmin,a2,thkcm,xmax,xinc,a3
        aa= a3
        iobs= 1
        if(a3.eq.' S0') then
          aa=' SP'
          iobs= 1
        else if(a3.eq.'  P') then
          aa=' SP'
          iobs= 2
        else if(a3.eq.' Sg') then 
          aa=' SP'
          iobs= 3
        else if(a3.eq.'  T') then
          aa=' SP'
          iobs= 4
        else if(a3.eq.' S1') then
          aa=' CR'
          iobs= 2
        else if(a3.eq.'ICS') then
          aa=' CR'
          iobs= 3
        end if
	if(xmin.lt.((rmkx+rmhx)**2-rmnx**2)/2.d0/rmnx) goto 9999
        if(xmin.gt.xmax .or. xinc.lt.0.d0) goto 9999
        if(xinc.gt.1.d-7) then
          nx = ifix(sngl((xmax - xmin)/xinc)) + 1
          nvr = min(nx,mpoi)
	  if(nvr.lt.nx) write(*,*)'#crsct: increment is small '
        else
          write(*,*)'#crsct: increment is too small: eginc=',xinc
          nvr= 1
        end if
        if(iprint.eq.1 .and. a3.eq.' SP') write(8,2105) a1
        if(iprint.eq.1 .and. a3.eq.' CR') write(8,2107) a1
        do ieg = 1,nvr
          egli = xmin + xinc*(ieg - 1)
          if(proc.eq.'photo') then 
            call thpho(egli,thkcm,a1,a2,aa,ths)
          else if(proc.eq.'phtot') then
            call gset(-1.0d0,1.0d0,npnt,xcos,wght)
            a2= 'ct0'
            aa= ' SP'
	    iprint = 1
            sum= 0.d0
            do iint= 1,npnt
              px2= xcos(iint)
              call invrm(egli,px2,a1,a2,aa,ths)
              sum = sum + wght(iint)*ths(1)
            end do
            sum= sum*2.d0*pi
          end if
          if(iprint.gt.0) then
            if(proc.eq.'photo') then
              if(a3.eq.' SP') then
                if(iprint.gt.1) write(8,2105) a1
                write(8,2110) egli,(ths(i),i=1,8)
                write(9,2110) egli,(ths(i),i=1,8)
              else if(a3.eq.' CR') then
                if(iprint.gt.1) write(8,2107) a1
                write(8,2110) egli,(ths(i),i=1,4)
                write(9,2110) egli,(ths(i),i=1,4)
              else               
                write(8,2120)a1,egli,a2,thkcm,a3,ths(iobs)
                write(9,2130)egli,ths(iobs)
              end if
            else if(proc.eq.'phtot') then
              write(8,2140)a1,egli,sum
              write(9,2130)egli,sum
            end if
          end if
        end do 
 2100   format(a5,a5,f10.0,a5,3f10.0,a5)
 2105   format(3x,a3,7x,'ds',8x,'P',8x,'Sig',8x,'T',8x,'Cx',10x,'Cz',
     +         10x,'Ox',10x,'Oz')
 2107   format(3x,a3,7x,'ds',7x,'dsl',7x,'ICS',8x,'P')
 2110   format(1x,f6.3,1x,4(f9.5,1x),4(d11.4,1x))
 2120   format(1x,a3,'=',f10.3,2x,a3,'=',f8.3,4x,a3,'=',f12.5)
 2130   format(1x,f10.4,2x,3(f10.4,2x))
 2140   format(1x,a3,'=',f7.3,5x,'total cross section =',
     +           f10.4,' mub')
 2200   format(1x,f6.2,1x,4(f9.5,1x),4(d11.4,1x))
c---------------------------------------------------------------
      else if(irun.eq.2) then
c
c  case theta_K^cm, cos(theta_K^cm), theta_K^lab, t  varies (photo)
c
        read(1,2100) proc,a1,egl,a2,xmin,xmax,xinc,a3
        aa=a3
        iobs= 1
        if(a3.eq.' S0') then
          aa=' SP'
          iobs= 1
        else if(a3.eq.'  P') then
          aa=' SP'
          iobs= 2
        else if(a3.eq.' Sg') then 
          aa=' SP'
          iobs= 3
        else if(a3.eq.'  T') then
          aa=' SP'
          iobs= 4
        else if(a3.eq.' S1') then
          aa=' CR'
          iobs= 2
        else if(a3.eq.'ICS') then
          aa=' CR'
          iobs= 3
        end if
	if(egl.lt.((rmkx+rmhx)**2-rmnx**2)/2.d0/rmnx) goto 9999
        if(xmin.gt.xmax .or. xinc.lt.0.d0) goto 9999
        if(xinc.gt.1.d-7) then
          nx= ifix(sngl((xmax - xmin)/xinc)) + 1
          nvr= min(nx,mpoi)
	  if(nvr.lt.nx) write(*,*)'#crsct: increment is small '
        else
          write(*,*)'#crsct: increment is too small: thkinc=',xinc
          nvr= 1
        end if
        if(iprint.eq.1 .and. a3.eq.' SP') write(8,2105) a2
        if(iprint.eq.1 .and. a3.eq.' CR') write(8,2107) a2
        do ieg= 1,nvr
          thkcmi= xmin + xinc*(ieg-1)
          if(proc.eq.'photo') then
            call thpho(egl,thkcmi,a1,a2,aa,ths)
          end if
          if(iprint.gt.0) then
            if(a3.eq.' SP') then
              if(iprint.gt.1) write(8,2105) a2
              write(8,2200) thkcmi,(ths(i),i=1,8)
              write(9,2200) thkcmi,(ths(i),i=1,8)
            else if(a3.eq.' CR') then
              if(iprint.gt.1) write(8,2107) a2
              write(8,2110) thkcmi,(ths(i),i=1,4)
              write(9,2110) thkcmi,(ths(i),i=1,4)
            else 
              write(8,2120) a1,egl,a2,thkcmi,a3,ths(iobs)
              write(9,2130) thkcmi,ths(iobs)
            end if
          end if
        end do
c---------------------------------------------------------------
      else if(irun.eq.3) then
c
c  case  Q^2  varies (electroproduction)
c
        read(1,*) ivar,xmin,w,thkx,eps,phi,xmax,xinc,imod
        if(w.lt.rmhx+rmkx) goto 9999
        if(xmin.gt.xmax .or. xinc.lt.0.d0) goto 9999
        if(ivar.lt.1.or.ivar.gt.3) goto 9999
        if(imod.eq.3) goto 9999
        if(xinc.gt.1.d-7) then
          nx = (xmax - xmin)/xinc + 1
          nvr = min(nx,mpoi)
	  if(nvr.lt.nx) write(*,*)'#crsct: increment is small '
        else
          write(*,*)'#crsct: increment is too small: q2inc=',xinc
          nvr = 1
        end if
        do ieg = 1,nvr
          q2 = xmin + xinc*(ieg - 1)
          call thele(ivar,w,q2,eps,thkx,phi,imod,ths)
          if(iprint.gt.0) then
            write(8,*)
            if(ivar.eq.1)write(8,2301) w, q2, eps, thkx, phi
            if(ivar.eq.2)write(8,2302) w, q2, eps, thkx, phi
            if(ivar.eq.3)write(8,2303) w, q2, eps, thkx, phi
            if(imod.eq.0) write(8,2310) (ths(i),i=1,6)
            if(imod.eq.1) write(8,2320) (ths(i),i=1,6)
            if(imod.eq.2) write(8,2330) (ths(i),i=1,6)
            if(imod.eq.4) then
              write(8,2350)
              write(8,2360) (ths(i),i=1,5)
              write(8,2370)
              write(8,2360) (ths(i),i=6,9)
              write(8,2380)
              write(8,2360) (ths(i),i=10,14)
              write(8,2390)
              write(8,2360) (ths(i),i=15,18)
            end if
            if(imod.eq.5) write(8,2400) (ths(i),i=1,3)
            write(8,2399)
            if(imod.eq.4) then
	      write(9,2395) q2,ths
	    else if(imod.eq.5) then
	      write(9,2395) q2,(ths(i),i=1,3)
            else
	      write(9,2395) q2,(ths(i),i=1,6)
	    end if
          end if
        end do
 2301   format('W=',f6.3,' GeV',3x,'Q^2=',f6.3,' GeV^2',3x,
     +       'eps=',f5.3,2x,'thKcm=',f5.1,2x,'phi= ',f5.1,' deg')
 2302   format('W=',f6.3,' GeV',3x,'Q^2=',f6.3,' GeV^2',3x,
     +       'eps=',f5.3,2x,'t=',f6.3,' GeV^2',2x,'phi= ',f5.1,' deg')
 2303   format('W=',f6.3,' GeV',3x,'Q^2=',f6.4,' GeV^2',3x,
     +       'eps=',f5.3,2x,'cos(thKc)=',f6.4,2x,'phi= ',f5.1,' deg')
 2310 format(1x,'CMS:  s_T= ',d12.6,3x,'s_L= ',d12.6,
     +       2x,'s_TT= ',d12.6,/,6x,'s_LT= ',d12.6,2x,
     +       's_UL= ',d12.6,3x,'s_K= ',d12.6,2x,'nb/sr')
 2320 format(1x,'LAB:  s_T= ',d12.6,3x,'s_L= ',d12.6,
     +       2x,'s_TT= ',d12.6,/,6x,'s_LT= ',d12.6,2x,
     +       's_UL= ',d12.6,3x,'s_K= ',d12.6,2x,'nb/sr')
 2330 format(1x,'INV:  s_T= ',d12.6,3x,'s_L= ',d12.6,
     +       2x,'s_TT= ',d12.6,/,6x,'s_LT= ',d12.6,2x,
     +       's_UL= ',d12.6,3x,'s_K= ',d12.6,2x,'mub/GeV^2')
 2350 format(4x,'  r00t',8x,'  r00l',7x,'  r00tl',7x,'  r00tt',
     +       7x,' r00tld')
 2360 format(1x,5(d12.6,2x))
 2370 format(4x,'rxd0tl',8x,'rxd0tt',7x,'rxd0tld',7x,'rxd0ttd')
 2380 format(4x,' ryd0t',8x,' ryd0l',7x,' ryd0tl',7x,' ryd0tt',
     +       7x,'ryd0tld')
 2390 format(4x,'rzd0tl',8x,'rzd0tt',7x,'rzd0tld',7x,'rzd0ttd')
 2395 format(1x,f8.4,1x,18(d11.4,1x))
 2399 format(72(1h-))
 2400 format(1x,'P= ',f8.5,2x,'photon and target asymmetry: ',
     +       'Sigma= ',f8.5,2x,'T= ',f8.5)
c---------------------------------------------------------------
      else if(irun.eq.4) then
c
c  case  W  varies (electroproduction)
c
        read(1,*) ivar,q2,xmin,thkx,eps,phi,xmax,xinc,imod
        if(xmin.lt.rmhx+rmkx) goto 9999
        if(xmin.gt.xmax .or. xinc.lt.0.d0) goto 9999
        if(ivar.lt.1.or.ivar.gt.3) goto 9999
        if(imod.eq.3) goto 9999
        if(xinc.gt.1.d-7) then
          nx = (xmax - xmin)/xinc + 1
          nvr = min(nx,mpoi)
	  if(nvr.lt.nx) write(*,*)'#crsct: increment is small '
        else
          write(*,*)'#crsct: increment is too small: winc=',xinc
          nvr = 1
        end if
        do ieg = 1,nvr
          w = xmin + xinc*(ieg - 1)
          call thele(ivar,w,q2,eps,thkx,phi,imod,ths)
          if(iprint.gt.0) then
            write(8,*)
            if(ivar.eq.1)write(8,2301) w, q2, eps, thkx, phi
            if(ivar.eq.2)write(8,2302) w, q2, eps, thkx, phi
            if(ivar.eq.3)write(8,2303) w, q2, eps, thkx, phi
            if(imod.eq.0) write(8,2310) (ths(i),i=1,6)
            if(imod.eq.1) write(8,2320) (ths(i),i=1,6)
            if(imod.eq.2) write(8,2330) (ths(i),i=1,6)
            if(imod.eq.4) then
              write(8,2350)
              write(8,2360) (ths(i),i=1,5)
              write(8,2370)
              write(8,2360) (ths(i),i=6,9)
              write(8,2380)
              write(8,2360) (ths(i),i=10,14)
              write(8,2390)
              write(8,2360) (ths(i),i=15,18)
            end if
            if(imod.eq.5) write(8,2400) (ths(i),i=1,3)
            write(8,2399)
            if(imod.eq.4) then
	      write(9,2395) w,ths
	    else if(imod.eq.5) then
              write(9,2395) w,(ths(i),i=1,3)
            else
	      write(9,2395) w,(ths(i),i=1,6)
	    end if
          end if
        end do
c---------------------------------------------------------------
      else if(irun.eq.5) then
c
c  case thkcm varies (electroproduction)
c
        read(1,*) ivar,q2,w,xmin,eps,phi,xmax,xinc,imod
        if(w.lt.rmhx+rmkx) goto 9999
        if(xmin.gt.xmax.or.xinc.lt.0.d0) goto 9999
        if(ivar.lt.1.or.ivar.gt.3) goto 9999
        if(imod.eq.3) goto 9999
        if(xinc.gt.1.d-7) then
          nx = (xmax - xmin)/xinc + 1
          nvr = min(nx,mpoi)
	  if(nvr.lt.nx) write(*,*)'#crsct: increment is small '
        else
          write(*,*)'#crsct: increment is too small: thkinc=',xinc
          nvr = 1
        end if
        do ieg = 1,nvr
          thkx = xmin + xinc*(ieg - 1)
          call thele(ivar,w,q2,eps,thkx,phi,imod,ths)
          if(iprint.gt.0) then
            write(8,*)
            if(ivar.eq.1)write(8,2301) w, q2, eps, thkx, phi
            if(ivar.eq.2)write(8,2302) w, q2, eps, thkx, phi
            if(ivar.eq.3)write(8,2303) w, q2, eps, thkx, phi
            if(imod.eq.0) write(8,2310) (ths(i),i=1,6)
            if(imod.eq.1) write(8,2320) (ths(i),i=1,6)
            if(imod.eq.2) write(8,2330) (ths(i),i=1,6)
            if(imod.eq.4) then
              write(8,2350)
              write(8,2360) (ths(i),i=1,5)
              write(8,2370)
              write(8,2360) (ths(i),i=6,9)
              write(8,2380)
              write(8,2360) (ths(i),i=10,14)
              write(8,2390)
              write(8,2360) (ths(i),i=15,18)
            end if
            if(imod.eq.5) write(8,2400) (ths(i),i=1,3)
            write(8,2399)
            if(imod.eq.4) then
	      write(9,2395) thkx,ths
	    else if(imod.eq.5) then
              write(9,2395) thkx,(ths(i),i=1,3)
            else
	      write(9,2395) thkx,(ths(i),i=1,6)
	    end if
          end if
        end do
c---------------------------------------------------------------
      else if(irun.eq.6) then
c
c  case more general electron kinematics is given (ivar= 4)
c
        read(1,*) nvr
	ivar= 4
	imod= 3
        write(8,*)
        write(8,*)'Tripple-differential cross section in Lab s_eK (nb)'
        if(iprint.eq.1) write(8,2338)
        do ieg = 1,nvr
          read(1,*)       ebeam,efin,thte1,thtk0,phi
          call thele(ivar,ebeam,efin,thte1,thtk0,phi,imod,ths)
          if(iprint.gt.0) then
            if(iprint.gt.1) write(8,2338)
            write(8,2340) ebeam,efin,thte1,thtk0,phi,ths(5),ths(6)
            write(9,2340) ebeam,efin,thte1,thtk0,phi,ths(5),ths(6)
          end if
        end do
 2338   format(2x,'Ebeam',2x,'Efin',2x,'th_e^lb',2x,'th_K^cm',3x,'phi',
     1         7x,'s_K',7x,'s_eK')
 2340   format(1x,2(f6.3,1x),3(f7.3,1x),f11.4,1x,f9.4)
c---------------------------------------------------------------
      else
        write(*,*)'#crsct: wrong value of irun: ',irun
        write(*,*)'#crsct: calculation terminated'
      end if

      return
c---------------------------------------------------------------
 9999 write(*,*)'#crsct: inconsistent input kinematics'
      write(*,*)'#crsct: calculation terminated'

      return
      end
c
c-----------------------------------------------------------------------
c
      subroutine fcn0(kindat)
c
c  deviations of theoretical values from given data points and 
c  chi^2 are calculated
c
      implicit none
      real*8 pi,hc,alfa,rmp,rmn,rmnx,rml,rms0,rmsp,rmsm,rmhx,rmhd,
     +       rmk,rmk0,rmkx,rmres(16),rmks0,ths(18),sum,xcos(24),
     +       wght(24),fprev,rnmf,chi,dtot,chdp,dat,dstat,txx,rcon,
     +       eg1,eg2,dstep,px1,px2,px3,px4,px5,rkk1
      integer parita(16),spin(16),ispin(16),chres(16),nmbr,npnt,
     +        fitun,ndf,ndp,nn,nint,in,ichnn,imod,iobs,ivar,iprint,
     +        icount,iflag
      character*2 nck(16),vmd
      character*3 a1,a2,a3,aa
      character*5 proc
      character*9 ctr
      character*16 kindat
      character*60 comm

      common/param/ pi,hc,alfa,iprint
      common/rmass/ rmp,rmn,rmnx,rml,rms0,rmsp,rmsm,rmhx,rmhd,rmk,rmk0,
     +              rmkx,rmres,rmks0
      common/resns/ parita,spin,ispin,chres,nck,nmbr,vmd

      npnt= 24
      fitun= 1
      rnmf= 1.0d0
      dtot= 0.0d0
      chi=  0.0d0
      ndf= 0
      open(3,file=kindat,status='old')
 100  continue
c
c---------------------------------------------------------------
c  choosing the process and channel
c
      read(3,1100) proc,ichnn,ndp
      read(3,1110) comm
      if(icount.eq.1) then
        if(ichnn.lt.1.or.ichnn.gt.6) then
          write(*,*)'#fcn: wrong channel number: ',ichnn
          ichnn = 1
        end if
        if(vmd.eq.' L'.and.ichnn.gt.2) then
          write(*,1120) ichnn,vmd
	  ichnn = 1
	  write(*,*)'#fcn: channel number set to: ',ichnn
        end if
        if(vmd.eq.' S'.and.ichnn.lt.3) then
          write(*,1120) ichnn,vmd
	  ichnn = 3
	  write(*,*)'#fcn: channel number set to: ',ichnn
        end if
      end if
 1100 format(a5,i3,i5)
 1110 format(a60) 
 1120 format(1x,'#fcn: channel number inconsistent with model version ',
     +       i4,2x,a2)
c
c  channel parameters are prepared
c
      iflag= 3
      call channel(ichnn,iflag)
c
c  informing about data file (kinematics and observables)
c
      write(8,1130) proc,comm
 1130 format(1x,a5,'-data from: ',a60)

      ndf= ndf + ndp
      if(proc.eq.'elect') then
        read(3,*)
      end if
c---------------------------------------------------------------
      if(proc.eq.'photo') then
c
c  photoproduction: differential cross section or spin observables
c
      do nn = 1, ndp
        read(3,1400)a1,px1,a2,px2,a3,dat,dstat,dtot
        iobs= 1
        aa=' SP'
        if(a3.eq.'  P') iobs= 2
        if(a3.eq.' Sg') iobs= 3
        if(a3.eq.'  T') iobs= 4
        if(a3.eq.' Cx') iobs= 5
        if(a3.eq.' Cz') iobs= 6
        if(a3.eq.' Ox') iobs= 7
        if(a3.eq.' Oz') iobs= 8
        if(a3.eq.' S1') then
          aa= ' CR'
          iobs= 2
        else if(a3.eq.'ICS') then
          aa= ' CR'
          iobs= 3
        end if     
        call thpho(px1,px2,a1,a2,aa,ths)
        txx= ths(iobs)
        call dvtn(fitun,txx,dat,dstat,dtot,rnmf,chdp)
        write(8,1410)a1,px1,a2,px2,a3,txx,chdp
        chi= chi + chdp**2
      end do
 1400 format(a3,1x,f10.0,1x,a3,1x,f10.0,1x,a3,1x,3(f7.0,1x))
 1410 format(1x,a3,'=',f8.3,4x,a3,'=',f8.3,3x,a3,'=',f10.5,4x,
     +       'dev= ',f10.4)
c---------------------------------------------------------------
      else if(proc.eq.'phtot') then
c
c  photoproduction: total cross section 
c
        call gset(-1.0d0,1.0d0,npnt,xcos,wght)
      do nn = 1, ndp
        read(3,1400)a1,px1,a2,px2,a3,dat,dstat,dtot
        a2='ct0'
        aa=' SP'
        sum=0.d0
        do in=1,npnt
          px2=xcos(in)
          call thpho(px1,px2,a1,a2,aa,ths)
          sum=sum+wght(in)*ths(1)
        end do
        txx= sum*2.d0*pi
        call dvtn(fitun,txx,dat,dstat,dtot,rnmf,chdp)
        write(8,1430)a1,px1,txx,chdp
        chi= chi + chdp**2
      end do
 1430 format(1x,a3,'=',f8.3,12x,'tot crs =',f10.4,4x,'dev= ',
     +       f10.4)
c---------------------------------------------------------------
      else if(proc.eq.'phav1') then
c
c  photoproduction: averaging over the energy interval (Eg1,Eg2); 
c  integration is performed using the trapezoidal method
c
      do nn = 1, ndp
        read(3,1440)a1,eg1,eg2,nint,a2,px2,a3,dat,dstat,dtot
        iobs= 1
        aa=' SP'
        if(a3.eq.'  P') iobs= 2
        if(a3.eq.' Sg') iobs= 3
        if(a3.eq.'  T') iobs= 4
        if(a3.eq.' Cx') iobs= 5
        if(a3.eq.' Cz') iobs= 6
        if(a3.eq.' Ox') iobs= 7
        if(a3.eq.' Oz') iobs= 8
        if(a3.eq.' S1') then
          aa= ' CR'
          iobs= 2
        else if(a3.eq.'ICS') then
          aa= ' CR'
          iobs= 3
        end if     
        dstep=(eg2-eg1)/(nint-1)
        sum=0.0d0
        do in=1,nint
          px1=eg1+(in-1)*dstep
          call thpho(px1,px2,a1,a2,aa,ths)
          if(in.eq.1.or.in.eq.nint) then
            rcon=1.0d0
          else
            rcon=2.0d0
          end if
          sum=sum+rcon*ths(iobs)
        end do
        txx=sum/2.0d0/(nint-1)
c
c  'txx' should be still multiplied by 'eg2-eg1' 
c   but due to averaging also divided by 'eg2-eg1'! 
c
        call dvtn(fitun,txx,dat,dstat,dtot,rnmf,chdp)
        write(8,1450)eg1,eg2,a2,px2,a3,txx,chdp
        chi= chi + chdp**2
      end do
 1440 format(a3,1x,2(f7.0,1x),i3,2x,a3,1x,f7.0,1x,a3,1x,3(f7.0,1x))
 1450 format('eg1,eg2=',2f8.3,2x,a3,'=',f8.3,2x,a3,'=',f9.4,3x,
     +       'dev= ',f10.4)
c---------------------------------------------------------------
      else if(proc.eq.'phav2') then
c
c  photoproduction: angular averaging  
c  integration is performed using the trapezoidal method
c
      do nn = 1, ndp
        read(3,1460)a1,px1,a2,eg1,eg2,nint,a3,dat,dstat,dtot
        iobs= 1
        aa=' SP'
        if(a3.eq.'  P') iobs= 2
        if(a3.eq.' Sg') iobs= 3
        if(a3.eq.'  T') iobs= 4
        if(a3.eq.' Cx') iobs= 5
        if(a3.eq.' Cz') iobs= 6
        if(a3.eq.' Ox') iobs= 7
        if(a3.eq.' Oz') iobs= 8
        if(a3.eq.' S1') then
          aa= ' CR'
          iobs= 2
        else if(a3.eq.'ICS') then
          aa= ' CR'
          iobs= 3
        end if     
        dstep=(eg2-eg1)/(nint-1)
        sum=0.0d0
        do in=1,nint
          px2=eg1+(in-1)*dstep
          call thpho(px1,px2,a1,a2,aa,ths)
          if(in.eq.1.or.in.eq.nint) then
            rcon=1.0d0
          else
            rcon=2.0d0
          end if
          sum=sum+rcon*ths(iobs)
        end do
        txx=sum/2.0d0/(nint-1)
c
c  'txx' should be still multiplied by 'eg2-eg1' 
c   but due to averaging also divided by 'eg2-eg1'! 
c
        call dvtn(fitun,txx,dat,dstat,dtot,rnmf,chdp)
        write(8,1470)a1,px1,a2,eg1,eg2,a3,txx,chdp 
        chi= chi + chdp**2
      end do
 1460 format(a3,1x,f7.0,1x,a3,1x,2(f7.0,1x),i3,2x,a3,1x,3(f7.0,1x))
 1470 format(1x,a3,'=',f8.3,2x,a3,':',2f8.3,4x,a3,'=',f10.5,3x,
     +       'dev= ',f10.4)
c---------------------------------------------------------------
      else if(proc.eq.'elect') then
c
c  electroproduction: cross sections and response functions
c
      do nn = 1, ndp
        read(3,*)ivar,px1,px2,px3,px4,px5,imod,iobs,dat,dstat,dtot
        call thele(ivar,px1,px2,px3,px4,px5,imod,ths)
	if(imod.eq.0.or.imod.eq.1.or.imod.eq.2) then
          if(iobs.lt.1.or.iobs.gt.6) then
	    iobs= 1
	    if(icount.eq.1) write(*,*)'#fcn: wrong iobs is given ',
     +	                              iobs,' for data point ',nn
          end if
          txx= ths(iobs)
	else if(imod.eq.3) then
	  txx= ths(6)
	else if(imod.eq.4) then
          if(iobs.lt.1.or.iobs.gt.18) then
	    iobs= 1
	    if(icount.eq.1) write(*,*)'#fcn: wrong iobs is given ',
     +	                              iobs,' for data point ',nn
          end if
	  txx= ths(iobs)
	else if(imod.eq.5) then
	  if(iobs.lt.1.or.iobs.gt.3) then
	    iobs= 1
	    if(icount.eq.1) write(*,*)'#fcn: wrong iobs is given ',
     +	                              iobs,' for data point ',nn
          end if
          txx= ths(iobs)
	else if(imod.eq.21) then
	  txx= ths(3)
	end if
        call dvtn(fitun,txx,dat,dstat,dtot,rnmf,chdp)
          if(ivar.eq.1) write(8,1490)px1,px2,px3,px4,px5
          if(ivar.eq.2) write(8,1500)px1,px2,px3,px4,px5
          if(ivar.eq.3) write(8,1510)px1,px2,px3,px4,px5
          if(ivar.eq.4) write(8,1520)px1,px2,px3,px4,px5
          if(imod.eq.0) write(8,1530)txx,chdp 
          if(imod.eq.1) write(8,1540)txx,chdp 
          if(imod.eq.2) write(8,1550)txx,chdp 
          if(imod.eq.21)write(8,1560)txx,chdp 
          if(imod.eq.3.and.ivar.eq.4) write(8,1570)ths(4),ths(5),txx
	  if(imod.eq.4) write(8,1580)iobs,txx,chdp
	  if(imod.eq.5) write(8,1590)iobs,txx,chdp
          write(8,*)
        chi= chi + chdp**2
      end do
 1490 format('W=',f6.3,' GeV',3x,'Q^2=',f6.3,' GeV^2',3x,'eps=',
     +       f5.3,2x,'th_K^cm=',f5.1,2x,'ph_K=',f6.1,' deg')
 1500 format('W=',f6.3,' GeV',2x,'Q^2=',f6.3,' GeV^2',2x,'eps=',
     +        f5.3,2x,'t=',f7.3,' GeV^2',2x,'ph_K=',f6.1,' deg')
 1510 format('W=',f6.3,' GeV',2x,'Q^2=',f6.3,' GeV^2',2x,'eps=',
     +        f5.3,2x,'cos(th_K)=',f7.4,2x,'ph_K=',f6.1,' deg')
 1520 format('E_e=',f6.3,' GeV',2x,'E_e`=',f6.3,' GeV',2x,
     +       'th_e^lab=',f5.1,' deg',/,'th_K^cm=',f5.1,
     +       ' deg',2x,'phi_K=',f5.1,' deg')
 1530 format(1x,'in CMS: crsect=',f12.4,' nb/sr',2x,'dev= ',f12.4)
 1540 format(1x,'in LAB: crsect=',f12.4,' nb/sr',2x,'dev= ',f12.4)
 1550 format(1x,'crsect=',f12.4,' mub/GeV^2',2x,'dev= ',f12.4)
 1560 format(1x,'averaged crs=',f12.4,' mub/GeV^2',2x,'dev= ',f12.4)
 1570 format('s_K(CMS)= ',d11.5,2x,'s_K(LAB)= ',d11.5,
     +       2x,'s_eK(LAB)= ',d11.5,' mub/sr')
 1580 format(1x,'response func. ',i1,'=',f12.5,2x,'dev= ',f12.4,
     +       2x,'mub/sr')
 1590 format(1x,'spin observable ',i1,'=',f12.5,2x,'dev= ',f12.4)
c---------------------------------------------------------------
      else
c
c  for the sake of 'proc' has a wrong value
c
        write(*,*)'#fcn: proc is wrong:  ',proc
        ndf= ndf - ndp
      end if  
      write(8,1900)
 1900 format(74(1h-))
c---------------------------------------------------------------   
c
c  calculation is performed until the end of the data set
c
      read(3,2000) ctr
 2000 format(a9)
      if(ctr.eq.'continued') go to 100

      write(8,5000) chi, chi/ndf
 5000 format(1x,'total chi^2 and chi^2/n.d.p. :',f10.4,2x,f10.5)

      close(3)
      return
      end
c
c-----------------------------------------------------------------------
c
      subroutine dvtn(fitun,txx,dat,dstat,dtot,rnmf,chdp)
      implicit none
      integer fitun
      real*8 txx,dat,dstat,dtot,rnmf,chdp
c
c  choosing a fitting method (definition of chi^2)
c
      if(fitun.eq.1) then
        chdp= (dat - txx) / dstat 
      else if(fitun.eq.2) then
        chdp= (dat - txx) / dtot 
      else if(fitun.eq.3) then
        chdp= (rnmf*dat - txx) / dstat
      end if
      return
      end
c=======================================================================
c                end of the file emkhyp.for
c=======================================================================
