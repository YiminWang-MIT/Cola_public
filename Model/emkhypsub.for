c=======================================================================
c           Subroutines and functions for program  EMKHYP
c-----------------------------------------------------------------------
c    version: 22. 10. 2009
c    Petr Bydzovsky  bydz@ujf.cas.cz  
c-----------------------------------------------------------------------
c initial nucleon (proton,neutron) mass is 'rmnx' and can be off-shell
c which affects kinematics, exchanges of N7,N8, and N9, F_i calculations,
c and propagator of the s-channel Born term ('cmx');
c
c HADRONIC FORM FACTORS
c  Including the h.f.f. is controlled by 'ihff' parameter:
c
c    ihff = 0  NO h.f.f. is used
c    ihff = 1  a global h.f.f. is used
c    ihff = 2  Kaon-MAID model (Mart&Bennhold)
c    ihff = 3  Davidson and Workman, PRC63(2001)025210 (one cut-off)
c    ihff = 4  Davidson and Workman (independent cut-offs for background, 
c              i.e.Born, K*, K1,and Y* terms, and resonant, N*, terms)
c    ihff = 5  Gaussian shape, othervise as in ihff= 4
c
c WIDTH OF s-CHANNEL RESONANCES:
c    ihff = 2 (Kaon-MAID) energy dependence of N4,N6,N7, and N9 widths
c    constant width otherwise
c
c ELECTROMAGNETIC FORM FACTORS for the electroproduction
c  A form of the em.f.f. is controlled by 'iemff' parameter:
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
c INPUT VARIABLES 
c  Photoproduction:
c    px1 = Eg1 (E_gamma^lab), s, W
c    px2 = th0 (theta_K^cm), th1 (theta_K^lab), ct0 (cos(theta_K^cm)), t
c    units are GeV, GeV^2, deg
c
c  Electroproduction:
c    ivar=  1           2        3              4
c    -----------------------------------------------------
c    px1 =  W           W        W             Ebeam
c    px2 =  Q^2         Q^2      Q^2           E_e'
c    px3 =  epsilon     epsilon  epsilon       theta_e^lab
c    px4 =  theta_K^cm  t        cos(th_K^cm)  theta_K^cm
c    px5 =  phi_K       phi_K    phi_K         phi_K
c    -----------------------------------------------------
c    units are GeV (W,Ebeam,E_e'), (GeV/c)^2 (Q^2,t), and deg
c
c OUTPUT in field ths(i)
c  Photoproduction: CR -cross sections CMS(i=1), LAB(2), invariant(3)
c                       and  polarization(4) 
c                   SP -CMS cross section(i=1) and spin observables P(2),
c                       Sigma(3), T(4), Cx(5), Cz(6), Ox(7), Oz(8)
c        for iprint >1 -various amplitudes and t-matrices are 
c                       calculated and printed out   
c  Electroproduction: 
c   -cross sections:      sigma_T  (U),  sigma_L  (L), 
c                         sigma_TT (P),  sigma_LT (I) 
c      unseparated:  s_UL = s_T + eps * s_L 
c      full: s_K = s_T + eps * s_L + eps * cos(2*phi) * s_TT + 
c                  + sqrt(2*eps*(eps+1)) * cos(phi) * s_LT     
c      triple: s_eK = gflx * s_K
c
c  imod = 0  T(1), L(2), TT(3), LT(4), UL(5), and full(6) crs in CMS (nb/sr)
c       = 1  T(1), L(2), TT(3), LT(4), UL(5), and full(6) crs in LAB (nb/sr)
c       = 2  T(1), L(2), TT(3), LT(4), UL(5), and full(6) invariant crs(mub/GeV^2)
c       = 3 (for ivar=4 only) LAB T(1), L(2), and UL(3); CMS s_K(4), LAB s_K(5), 
c            and  LAB triple differential s_eK(6)  (nb/sr)
c       = 4  response functions (i= 1-18)
c       = 5  polarization(P)(1), photon (Sigma)(2) and target (T)(3) asymmetry
c       = 21 averaged crs(i=3)
c
c-----------------------------------------------------------------------
c
      subroutine mdinpt(model,iprnt)
c
c  model parameters and constants are prepared (read from model.xxx) 
c
      implicit none
      integer iprnt,ihff,iemff,iprint,nmbr,i,parita(16),
     +        spin(16),ispin(16),chres(16),ipar
      character*2 nck(16),vmd
      character*3 spn,isp
      character*16 model
      character*70 hlav
      real*8 pi,hc,alfa,rmp,rmn,rmnx,rml,rms0,rmsp,rmsm,rmhx,rmhd,rmk,
     +  rmk0,rmkx,rmres(16),rmks0,qnx,qkx,qhx,kpp,kpn,kpnx,kpl,kps0,
     +  kpsp,kpsm,kphx,kpls,rg1(16),rg2(16),axx,gkln,gks0n,gkhnx,gklsnx,
     +  g1(16),g2(16),g1x(16),g2x(16),hffct1,hffct2,hffa1,hffa2,emvp,
     +  emvn,emvk,emvks,emvk1
      complex*16 mp,mn,mnx,ml,ms0,msp,msm,mhx,mhd,mk,mk0,mkx,mres(16),
     +           mrs,cr

      common/param/ pi,hc,alfa,iprint
      common/mass2/ mp,mn,mnx,ml,ms0,msp,msm,mhx,mhd,mk,mk0,mkx,mres
      common/rmass/ rmp,rmn,rmnx,rml,rms0,rmsp,rmsm,rmhx,rmhd,rmk,rmk0,
     +              rmkx,rmres,rmks0
      common/resns/ parita,spin,ispin,chres,nck,nmbr,vmd
      common/emcpl/ qnx,qkx,qhx,kpp,kpn,kpnx,kpl,kps0,kpsp,kpsm,kphx,
     +              kpls,rg1,rg2,axx
      common/stcpl/ gkln,gks0n,gkhnx,gklsnx,g1,g2,g1x,g2x
      common/hdrff/ hffct1,hffct2,hffa1,hffa2,ihff
      common/elmff/ emvp,emvn,emvk,emvks,emvk1,iemff
c
c  values of pi, hc, and alfa  are from PDP tables
c
      pi = 3.141592653589793d0
      hc = 0.197326968d0
      alfa = 1.d0/dsqrt(137.03599911d0)
      iprint = iprnt
      cr= (1.d0,0.d0)
c
c  reading parameters of the model and writing out its description
c
      open(2,file=model,status='old')
      read(2,2000) hlav
      read(2,*)
       write(*,2000) hlav
       write(8,2000) hlav
      read(2,2000) hlav
       write(*,2000) hlav
       write(8,2000) hlav
      read(2,2000) hlav
      read(2,2012) vmd
       write(*,2015) vmd
       write(8,2015) vmd
      read(2,*)
      read(2,*)
      read(2,*)
      read(2,*)
      read(2,*)
      read(2,*) rmp,rmn
      read(2,*) rml,rms0,rmsp,rmsm
      read(2,*) rmk,rmk0
      read(2,*)
      read(2,*)
      read(2,*)
      read(2,*) kpp,kpn
      read(2,*) kpl,kps0,kpsp,kpsm,kpls
      read(2,*)
      read(2,*) gkln,gks0n
      read(2,*)
      read(2,*)
      read(2,*)  
      read(2,*) nmbr
      read(2,*) 
      read(2,*)
      do i = 1,nmbr
        read(2,2020)nck(i),mrs,spn,isp,ipar,g1(i),g2(i),rg1(i),rg2(i)
        rmres(i) = dble(mrs)
        mres(i) =  mrs*rmres(i)
        if(spn.eq.'1/2') then 
          spin(i)=1
        else if(spn.eq.'3/2') then 
          spin(i)=3
        else if(spn.eq.'5/2') then 
          spin(i)=5
        else if(spn.eq.' 1 ') then 
          spin(i)=2
        else
          write(*,100) spn
 100      format('#mdinpt: wrong resonance spin is given ',a3)
        end if
        if(isp.eq.'1/2') then 
          ispin(i)=1
        else if(isp.eq.'3/2') then 
          ispin(i)=3
        else if(isp.eq.' 0 ') then 
          ispin(i)=0
        else if(isp.eq.' 1 ') then 
          ispin(i)=2
        else
          write(*,200) isp
 200      format('#mdinpt: wrong resonance isospin is given ',a3)
        end if
        if(mod(spin(i),2).eq.0) then
c     t-channel resonances 
          chres(i)= 2
        else
          if(mod(ispin(i),2).eq.0) then
c     u-channel resonances
            chres(i)= 3
          else
c     s-channel resonances
            chres(i)= 1
          end if
        end if
        parita(i)= ipar
      end do
       write(8,2030)
       write(8,2032)
       do i = 1,nmbr
         write(8,2034)nck(i),mres(i)/rmres(i),spin(i),ispin(i),
     +                parita(i),chres(i),g1(i),g2(i),rg1(i),rg2(i)
       end do
      read(2,*)
      read(2,*)
      read(2,*) rmks0
       write(8,2036) rmks0
      read(2,*)
      read(2,*) axx
      read(2,*)
      read(2,*)
      read(2,*) ihff
      read(2,*)
      read(2,*) hffct1,hffct2
      read(2,*)
      read(2,*) hffa1,hffa2
      read(2,*)
      read(2,*)
      read(2,*) iemff
      read(2,*)
      read(2,*) emvp,emvn
      read(2,*)
      read(2,*) emvk,emvks,emvk1
 2000 format(a70)
 2010 format(a3)
 2012 format(a2)
 2015 format(1x,'--version of the model with ',a2,' in the final state')
 2020 format(1x,a2,1x,f7.5,1x,f7.4,1x,a3,1x,a3,1x,i2,2(1x,f9.6),
     1       2(1x,f7.4))
 2030 format(1x,'parameters of assumed resonances: ')
 2032 format(' nck   m     w    s  i  p  ch   g1     g2      r1     r2')
 2034 format(1x,a2,2x,f5.3,1x,f5.2,1x,4(i2,1x),1x,2(f6.3,1x),2(1x,f6.3))
 2036 format(1x,'mass of the neutral mode of K^* resonance: ',f8.5)
      close(2)

      mp=  cr*rmp**2
      mn=  cr*rmn**2
      ml=  cr*rml**2
      ms0= cr*rms0**2
      msp= cr*rmsp**2
      msm= cr*rmsm**2
      mk=  cr*rmk**2
      mk0= cr*rmk0**2
c
c  information about a form of the hadronic form factors used
c
      if(ihff.eq.0) then
        write(*,1000)
        write(8,1000)
 1000   format(1x,'--NO HADRONIC FORM FACTORS are used')      
      else if(ihff.eq.1) then
        write(*,1010)
        write(8,1010)
        write(8,1012)hffct1
 1010   format(1x,'--THE GLOBAL HADRONIC FORM FACTOR',
     +         ' by Bennhold et al, nucl-th/9703004')
 1012   format(1x,'--cutoff: ',f6.3,' GeV/c^2')
      else if(ihff.eq.2) then
        write(*,1020)
        write(8,1020)
        write(8,1022)hffct1,hffct2,hffa1,hffa2
 1020   format(1x,'--THE HADRONIC FORM FACTORS',
     +         ' by Bennhold and Mart (Kaon-MAID)')
 1022   format(1x,'--cutoff for nonresonant terms: ',f6.3,' GeV/c^2',/,
     +         1x,'--cutoff for resonant terms: ',f6.3,' GeV/c^2',/,
     +         1x,'--theta, phi: ',f7.2,3x,f7.2,' deg')
      else if(ihff.eq.3) then
        write(*,1030)
        write(8,1030)
        write(8,1032)hffct1
 1030   format(1x,'--HADRONIC FORM FACTOR by Davidson and Workman, ',
     +         'PRC63(2001)025210,')
 1032   format(1x,'--with cutoff: ',f6.3,' GeV/c^2', 'for all terms')
      else if(ihff.eq.4) then
        write(*,1040)
        write(8,1040)
        write(8,1042)hffct1,hffct2
        write(8,1043)
 1040   format(1x,'--HADRONIC FORM FACTOR by Davidson and Workman, ',
     +         'PRC63(2001)025210,')
 1042   format(1x,'--with cutoffs: ',f6.3,' and ',f6.3,' GeV/c^2') 
 1043   format(1x,'  for background and resonant terms, respectively')
      else if(ihff.eq.5) then
        write(*,1050)
        write(8,1050)
        write(8,1052)hffct1,hffct2
        write(8,1053)
 1050   format(1x,'--HADRONIC FORM FACTOR of Gaussian shape ')
 1052   format(1x,'--with scale factors ',f6.3,' and ',f6.3,' GeV/c^2')
 1053   format(1x,'  for background and resonant terms, respectively')
      else
        write(*,*)'#mdinpt: wrong value of ihff was given ',ihff
        write(*,*)'#mdinpt: ihff is set to zero'
        ihff=0
      end if
      write(*,1100)iemff
      write(8,1100)iemff
 1100 format(1x,'--ELECTROMAGNETIC FORM FACTOR: ',i2)
      return
      end
c
c-----------------------------------------------------------------------
c
      subroutine channel(ichnn,iflag)
c
c   Definition of channel parameters utilizing the isospin symmetry 
c   for the strong coupling constants provided those read from 
c   the input file are for the channels 1(L) and 3(S), e.g. 
c   gkln  is the coupling in  the K^+ Lambda proton  vertex and 
c   gks0n that in  K^+ Sigma^0 proton  one. In the electro-magnetic 
c   vertex, the ratio of coupling constants for neutral to charge  
c   modes of resonance, rg1 and rg2, given in model.*, are used too.
c
      implicit none
      integer ichnn,iflag,parita(16),spin(16),ispin(16),chres(16),nmbr,
     +        i
      character*2 nck(16),vmd
      real*8 rmp,rmn,rmnx,rml,rms0,rmsp,rmsm,rmhx,rmhd,rmk,rmk0,rmkx,
     +   rmres(16),rmks0,qnx,qkx,qhx,kpp,kpn,kpnx,kpl,kps0,kpsp,kpsm,
     +   kphx,kpls,rg1(16),rg2(16),axx,gkln,gks0n,gkhnx,gklsnx,g1(16),
     +   g2(16),g1x(16),g2x(16)
      complex*16 mp,mn,mnx,ml,ms0,msp,msm,mhx,mhd,mk,mk0,mkx,mres(16)

      common/mass2/ mp,mn,mnx,ml,ms0,msp,msm,mhx,mhd,mk,mk0,mkx,mres
      common/rmass/ rmp,rmn,rmnx,rml,rms0,rmsp,rmsm,rmhx,rmhd,rmk,rmk0,
     +              rmkx,rmres,rmks0
      common/resns/ parita,spin,ispin,chres,nck,nmbr,vmd
      common/emcpl/ qnx,qkx,qhx,kpp,kpn,kpnx,kpl,kps0,kpsp,kpsm,kphx,
     +              kpls,rg1,rg2,axx
      common/stcpl/ gkln,gks0n,gkhnx,gklsnx,g1,g2,g1x,g2x

      if(ichnn.eq.1) then
c
c  gamma  +  p ---> K^+  +  Lambda
c
        if(iflag.eq.3) then
          write(*,*)'--channel:  gamma  +  p ---> K^+  +  Lambda'
          write(8,*) ' channel:  gamma  +  p ---> K^+  +  Lambda'
        end if
        rmnx=  rmp
        mnx=   mp
        qnx=  1.d0
        kpnx= kpp
        rmhx=  rml
        mhx=   ml
        qhx=  0.d0
        kphx= kpl
        rmhd= rms0
        mhd=  ms0
        rmkx= rmk
        mkx=  mk
        qkx=  1.d0
        gkhnx= gkln
        gklsnx= gks0n
        do i=1,nmbr
          g1x(i)= g1(i)          
          g2x(i)= g2(i)
        end do
      else if(ichnn.eq.2) then
c
c  gamma  +  n ---> K^0  +  Lambda
c
        if(iflag.eq.3) then
          write(*,*)'--channel:  gamma  +  n ---> K^0  +  Lambda'
          write(8,*) ' channel:  gamma  +  n ---> K^0  +  Lambda'
        end if
        rmnx=  rmn
        mnx=   mn
        qnx=  0.d0
        kpnx= kpn
        rmhx=  rml
        mhx=   ml
        qhx=  0.d0
        kphx= kpl
        rmhd= rms0
        mhd=  ms0
        rmkx= rmk0 
        mkx=  mk0
        qkx=  0.d0
        gkhnx= gkln
        gklsnx= -gks0n
        do i=1,nmbr
          if(chres(i).eq.1) then
c       s-channel resonances (spin 1/2, 3/2, and 5/2)
            if(ispin(i).eq.1) then
c         Nucleon resonances
              g1x(i)= g1(i)*rg1(i)
              g2x(i)= g2(i)*rg2(i)
            else if(ispin(i).eq.3) then
c         Delta resonances
              g1x(i)= 0.d0
              g2x(i)= 0.d0
            end if
          else if(chres(i).eq.2) then
c       t-channel resonances (spin 1)
            g1x(i)= g1(i)*rg1(i)          
            g2x(i)= g2(i)*rg1(i)
          else if(chres(i).eq.3) then
c       u-channel resonances (spin 1/2)
            if(ispin(i).eq.0) then
c         Lambda resonances
              g1x(i)= g1(i)
            else if(ispin(i).eq.2) then
c         Sigma resonances
              g1x(i)= -g1(i)
            end if           
          end if
        end do        
      else if(ichnn.eq.3) then
c
c  gamma  +  p ---> K^+  +  Sigma^0
c
        if(iflag.eq.3) then
          write(*,*)'--channel:  gamma  +  p ---> K^+  +  Sigma^0'
          write(8,*) ' channel:  gamma  +  p ---> K^+  +  Sigma^0'
        end if
        rmnx=  rmp
        mnx=   mp
        qnx=  1.d0
        kpnx= kpp
        rmhx=  rms0
        mhx=   ms0
        qhx=  0.d0
        kphx= kps0
        rmhd= rml
        mhd=  ml
        rmkx= rmk
        mkx=  mk
        qkx=  1.d0
        gkhnx= gks0n
        gklsnx= gkln
        do i=1,nmbr
          g1x(i)= g1(i)          
          g2x(i)= g2(i)
        end do
      else if(ichnn.eq.4) then
c
c  gamma  +  n ---> K^0  +  Sigma^0
c
        if(iflag.eq.3) then
          write(*,*)'--channel:  gamma  +  n ---> K^0  +  Sigma^0'
          write(8,*) ' channel:  gamma  +  n ---> K^0  +  Sigma^0'
        end if
        rmnx=  rmn
        mnx=   mn
        qnx=  0.d0
        kpnx= kpn
        rmhx=  rms0
        mhx=   ms0
        qhx=  0.d0
        kphx= kps0
        rmhd= rml
        mhd=  ml
        rmkx= rmk0 
        mkx=  mk0
        qkx=  0.d0
        gkhnx= -gks0n
        gklsnx= gkln
        do i=1,nmbr
          if(chres(i).eq.1) then
c       s-channel resonances (spin 1/2, 3/2, and 5/2)
            if(ispin(i).eq.1) then
c         Nucleon resonances
              g1x(i)= -g1(i)*rg1(i)
              g2x(i)= -g2(i)*rg2(i)
            else if(ispin(i).eq.3) then
c         Delta resonances
              g1x(i)= g1(i)*rg1(i)
              g2x(i)= g2(i)*rg2(i)
            end if
          else if(chres(i).eq.2) then
c       t-channel resonances (spin 1)
            g1x(i)= -g1(i)*rg1(i)          
            g2x(i)= -g2(i)*rg1(i) 
          else if(chres(i).eq.3) then
c       u-channel resonances (spin 1/2)
            if(ispin(i).eq.0) then
c         Lambda resonances
              g1x(i)= g1(i)
            else if(ispin(i).eq.2) then
c         Sigma resonances
              g1x(i)= -g1(i)
            end if           
          end if
        end do
      else if(ichnn.eq.5) then
c
c  gamma  +  n ---> K^+  +  Sigma^-
c
        if(iflag.eq.3) then
          write(*,*)'--channel:  gamma  +  n ---> K^+  +  Sigma^-'
          write(8,*) ' channel:  gamma  +  n ---> K^+  +  Sigma^-'
        end if
        rmnx=  rmn
        mnx=   mn
        qnx=  0.d0
        kpnx= kpn
        rmhx=  rmsm
        mhx=   msm
        qhx=  -1.d0
        kphx= kpsm
        rmhd= 1.d0
        mhd=  (1.d0,0.d0)
        rmkx= rmk 
        mkx=  mk
        qkx=  1.d0
        gkhnx= gks0n*dsqrt(2.d0)
        gklsnx= 0.d0
        do i=1,nmbr
          if(chres(i).eq.1) then
c       s-channel resonances (spin 1/2, 3/2, and 5/2)
            if(ispin(i).eq.1) then
c         Nucleon resonances
              g1x(i)= g1(i)*rg1(i)*dsqrt(2.d0)
              g2x(i)= g2(i)*rg2(i)*dsqrt(2.d0)
            else if(ispin(i).eq.3) then
c         Delta resonances
              g1x(i)= g1(i)*rg1(i)/dsqrt(2.d0)
              g2x(i)= g2(i)*rg2(i)/dsqrt(2.d0)
            end if
          else if(chres(i).eq.2) then
c       t-channel resonances (spin 1)
            g1x(i)= g1(i)*dsqrt(2.d0)         
            g2x(i)= g2(i)*dsqrt(2.d0) 
          else if(chres(i).eq.3) then
c       u-channel resonances (spin 1/2)
            if(ispin(i).eq.0) then
c         Lambda resonances
              g1x(i)= 0.d0
            else if(ispin(i).eq.2) then
c         Sigma resonances
              g1x(i)= g1(i)*dsqrt(2.d0)
            end if           
          end if
        end do
      else if(ichnn.eq.6) then
c
c  gamma  +  p ---> K^0  +  Sigma^+
c
        if(iflag.eq.3) then
          write(*,*)'--channel:  gamma  +  p ---> K^0  +  Sigma^+'
          write(8,*) ' channel:  gamma  +  p ---> K^0  +  Sigma^+'
        end if
        rmnx=  rmp
        mnx=   mp
        qnx=  1.d0
        kpnx= kpp
        rmhx=  rmsp
        mhx=   msp
        qhx=  1.d0
        kphx= kpsp
        rmhd= 1.d0
        mhd=  (1.d0,0.d0)
        rmkx= rmk0 
        mkx=  mk0
        qkx=  0.d0
        gkhnx= gks0n*dsqrt(2.d0)
        gklsnx= 0.d0
        do i=1,nmbr
          if(chres(i).eq.1) then
c       s-channel resonances (spin 1/2, 3/2, and 5/2)
            if(ispin(i).eq.1) then
c         Nucleon resonances
              g1x(i)= g1(i)*dsqrt(2.d0)
              g2x(i)= g2(i)*dsqrt(2.d0)
            else if(ispin(i).eq.3) then
c         Delta resonances
              g1x(i)= -g1(i)/dsqrt(2.d0)
              g2x(i)= -g2(i)/dsqrt(2.d0)
            end if
          else if(chres(i).eq.2) then
c       t-channel resonances (spin 1)
            g1x(i)= g1(i)*rg1(i)*dsqrt(2.d0)         
            g2x(i)= g2(i)*rg1(i)*dsqrt(2.d0) 
          else if(chres(i).eq.3) then
c       u-channel resonances (spin 1/2)
            if(ispin(i).eq.0) then
c         Lambda resonances
              g1x(i)= 0.d0
            else if(ispin(i).eq.2) then
c         Sigma resonances
              g1x(i)= g1(i)*dsqrt(2.d0)
            end if           
          end if
        end do
      end if

      return
      end
c
c-----------------------------------------------------------------------
c
      subroutine invrm(px1,px2,a1,a2,a3,ths)
c
c  calculates cross section in the photoproduction evaluating directly 
c  the matrix element M_fi as a function of the Mandelstam variables, 
c  possibly also out of the physical region 
c
      implicit none
      character*3 a1,a2,a3
      real*8 px1,px2,ths(18),pi,hc,alfa,rmp,rmn,rmnx,rml,rms0,rmsp,rmsm,
     +    rmhx,rmhd,rmk,rmk0,rmkx,rmres(16),rmks0,qnx,qkx,qhx,kpp,kpn,
     +    kpnx,kpl,kps0,kpsp,kpsm,kphx,kpls,rg1(16),rg2(16),axx,rmnx2,
     +    rmkx2,rmhx2,ss,ts,us,matin,src,norm,mij(1:4,1:4),ws,qgcm,
     +    pkcm,ekcm,eglb,eklb,pklb,x0,x1,x2,qg2
      complex*16 aj(1:4),ai(1:6)
      integer i,j,iprint

      common/param/ pi,hc,alfa,iprint
      common/rmass/ rmp,rmn,rmnx,rml,rms0,rmsp,rmsm,rmhx,rmhd,rmk,rmk0,
     +              rmkx,rmres,rmks0
      common/emcpl/ qnx,qkx,qhx,kpp,kpn,kpnx,kpl,kps0,kpsp,kpsm,kphx,
     +              kpls,rg1,rg2,axx

      qg2 = 0.0d0
      rmnx2 = rmnx**2
      rmkx2 = rmkx**2
      rmhx2 = rmhx**2
c
c  kinematics for the s-channel  gamma + N --> K + Y (photoproduction)  
c   photon LAB energy (Eg1), c.m. energy (W), s= W^2
c   kaon c.m. angle (th0), its cosinus (ct0), kaon LAB angle (th1)
c   and Mandelstam variable t
c
      if(a1.eq.'Eg1') then
        ss = rmnx2 + 2.d0*px1*rmnx
      else if(a1.eq.'  s') then
        ss = px1
      else if(a1.eq.'  W') then
        ss = px1**2
      else
        write(*,*)'#invrm: wrong input kinematics ',a1
        ss = (rmkx + rmhx)**2
      end if
      ws = dsqrt(ss)
      qgcm = (ss - rmnx2)/2.d0/ws
      pkcm = dsqrt((ss + rmkx2 - rmhx2)**2 - 4.d0*ss*rmkx2)/2.d0/ws
      ekcm = (ss - rmhx2 + rmkx2)/2.d0/ws
      if(a2.eq.'th0') then
        ts = rmkx2 - 2.d0*qgcm*(ekcm - pkcm*dcos(px2/180.0d0*pi))
      else if(a2.eq.'ct0') then
        ts = rmkx2 - 2.d0*qgcm*(ekcm - pkcm*px2)
      else if(a2.eq.'th1') then
c
c kaon LAB angle; in the case there are two solutions for theta_K^cm 
c the input is ambiguous and solution of the forward CM angle is chosen
c initial (q) and final (k) CMS momenta
c
        x0 = dcos(px2/180.0d0*pi)
        x1 = ss + rmkx2 - rmhx2
        eglb = (ss - rmnx2)/2.d0/rmnx 
        x2 = ss + eglb**2*(1.0d0 - x0**2)
        pklb=((eglb+rmnx)*dsqrt(x1**2-4.d0*rmkx2*x2)+x1*eglb*x0)/2.d0/x2
        eklb = dsqrt(rmkx2 + pklb**2)
        ts = rmkx2 - 2.d0*eglb*(eklb - pklb*x0)  
      else if(a2.eq.'  t') then
        ts = px2
      else
        write(*,*)'#invrm: wrong input kinematics ',a2
        ts = rmkx2 - 2.d0*rmkx*qgcm
      end if
      us = rmnx2 + rmkx2 + rmhx2 - ss - ts

      call iampl(ss,qg2,ss,ts,us,ai)
c
c  changing our basis of the gauge-invariant operators to M^j
c  by  Williams, Ji, Cotanch, Phys.Rev.C46(1992)1617, eq.(6)  
c
      aj(1)= ai(1)
      aj(2)= (ai(2) + ai(3))/(ts - rmkx2)
      aj(3)= ai(4)
      aj(4)= ai(5)

      call opmat(ss,ts,us,rmnx,rmhx,mij)
c
c  martix element of the invariant amplitude is calculated
c
      matin = 0.d0
      do i = 1,4
        do j = 1,4
          matin = matin + dconjg(aj(j))*aj(i)*mij(j,i)
        end do
      end do
c
c  normalization and the factor due to averaging over proton spin 
c  and photon polarization
c
      matin = matin/4.d0
      norm = 10.d0*hc**2 * 1000.d0
      src = norm*matin*pkcm/qgcm/ss
      if(a3.eq.' S0') then
c
c  c.m. cross section in microbarn/sr
c
        ths(1) = src

      else if(a3.eq.'ICS') then
c        
c  invariant cross section in microbarn/GeV^2
c      
        ths(1) = src*pi/qgcm/pkcm
      else
        write(*,*)'#invrm: wrong output variable ',a3
      end if

      if(iprint.gt.1) then
c
c     writting results
c
        write(8,1000)ss,ts,us,qgcm,pkcm
        write(8,1010)src
 1000   format(1x,'s= ',f7.3,2x,'t= ',f7.3,2x,'u= ',f7.3,2x,'qgcm= ',
     1         f7.3,2x,'pkcm= ',f7.3)
 1010   format(1x,'cross section in c.m. = ',f12.6,' mub/sr')
      endif

      return
      end
c
c-----------------------------------------------------------------------
c
      subroutine thele(ivar,px1,px2,px3,px4,px5,imod,ths)
c
c  calculates response functions and cross sections for p(e,e'K)lambda 
c  from CGLN amplitudes
c
      implicit none
      complex*16 f(6),fc(6),a(6),ac(6),imgc
      integer ivar,imod,j,iprint
      real*8 px1,px2,px3,px4,px5,ths(18),pi,hc,alfa,rmp,rmn,rmnx,rml,
     +       rms0,rmsp,rmsm,rmhx,rmhd,rmk,rmk0,rmkx,rmres(16),rmks0,
     +       rmx2,rmkx2,rmhx2,w,qsq,eps,thkc,phi,ctc,stc,s,t,hgv2,
     +       egvcm,qgvcm,ekcm,pkcm,egv,qgv,ei,ef,pei,pef,me2,thel,gflx,
     +       ctl,pklab,thkl,epcm,r00t,r00l,r00tl,r00tt,rxd0tl,
     +       rxd0tt,ryd0t,ryd0l,ryd0tl,ryd0tt,rzd0tl,rzd0tt,r00tld,
     +       rxd0tld,rxd0ttd,ryd0tld,rzd0tld,rzd0ttd,rnrsp,q,k,snl,
     +       trfm,rn,ell,dsa,dsb,dsc,dsd,jacob,snn,crsl,ek,el,dsl,dsx
      common/param/ pi,hc,alfa,iprint
      common/rmass/ rmp,rmn,rmnx,rml,rms0,rmsp,rmsm,rmhx,rmhd,rmk,rmk0,
     +              rmkx,rmres,rmks0
c
c     w     = total CMS energy W in GeV 
c     qsq   = Q^2 > 0 in (GeV/c)^2
c     eps   = polarization of virtual photon epsilon
c     thkc  = kaon CMS polar angle theta_K^cm in deg (to photon)
c     phi   = kaon azimutal angle in deg
c     s, t  = Mandelstam variables
c     ctc/stc = cosine/sine of theta_K^cm 
c     qgv   = virtual photon LAB three-momentum
c     egv   = virtual photon LAB energy
c     qgvcm = virtual photon CMS three-momentum
c     egvcm = virtual photon CMS energy (can be negative- the photon 
c                  moves in opposite direction with positive energy)
c     hgv2  = virtual photon mass
c     pkcm  = kaon CMS three-momentum
c     thkl  = kaon LAB scattering angle (with respect to photon)
c     ctl   = cosinus of kaon LAB angle
c     pklab = kaon LAB three-momentum
c
      imgc = (0.0d0,-1.0d0)
      rmx2 = rmnx**2
      rmkx2 = rmkx**2
      rmhx2 = rmhx**2
      if(ivar.eq.1) then
c
c  W, Q^2, eps, thkc, phi
c
        w    = px1
        qsq  = px2
        eps  = px3
        thkc = px4
        phi  = px5
        ctc = dcos(pi*thkc/180.0d0)
        s = w**2
        hgv2 = -qsq
        egvcm = (s - rmx2 + hgv2)/w/2.0d0
        qgvcm = dsqrt(egvcm**2 - hgv2)
        pkcm = dsqrt((s - rmhx2 - rmkx2)**2 - 4.0d0*rmhx2*rmkx2)/w/2.0d0
        ekcm = dsqrt(pkcm**2 + rmkx2)
        t = hgv2 + rmkx2 - 2.0d0*(ekcm*egvcm - ctc*qgvcm*pkcm)
        egv = (s - hgv2 - rmx2)/rmnx/2.0d0
        qgv = dsqrt(egv*egv - hgv2)
      else if(ivar.eq.2) then
c
c  W, Q^2, eps, t, phi 
c
        w   = px1
        qsq = px2
        eps = px3
        t   = px4
        phi = px5
        s = w**2
        hgv2 = -qsq
        egvcm = (s - rmx2 + hgv2)/w/2.0d0
        qgvcm = dsqrt(egvcm**2 - hgv2)
        pkcm = dsqrt((s - rmhx2 - rmkx2)**2 - 4.0d0*rmhx2*rmkx2)/w/2.0d0
        ekcm = dsqrt(pkcm**2 + rmkx2)
        ctc = (t - rmkx2 - hgv2 + 2.0d0*ekcm*egvcm)/qgvcm/pkcm/2.0d0
        egv = (s - hgv2 - rmx2)/rmnx/2.0d0
        qgv= dsqrt(egv*egv - hgv2)
      else if(ivar.eq.3) then
c
c  W, Q^2, eps, cos(thkc), phi
c
        w   = px1
        qsq = px2
        eps = px3
        ctc = px4
        phi = px5
        s = w**2
        hgv2 = -qsq
        egvcm = (s - rmx2 + hgv2)/w/2.0d0
        qgvcm = dsqrt(egvcm**2 - hgv2)
        pkcm = dsqrt((s - rmhx2 - rmkx2)**2 - 4.0d0*rmhx2*rmkx2)/w/2.0d0
        ekcm = dsqrt(pkcm**2 + rmkx2)
        t = hgv2 + rmkx2 - 2.0d0*(ekcm*egvcm - ctc*qgvcm*pkcm)
        egv = (s -hgv2 -rmx2)/rmnx/2.0d0
        qgv = dsqrt(egv*egv - hgv2)
      else if(ivar.eq.4) then
c
c  Ebeam, E_e', theta_e, thkc, phi
c
c  URL is assumed for epsilon and virtual photon flux (gflx)
c
        me2 = 0.000511d0**2
        ei   = px1
        ef   = px2
        thel = px3*pi/180.d0
        thkc = px4
        phi  = px5
        ctc = dcos(pi*thkc/180.0d0)
        pei = dsqrt(ei**2-me2)
        pef = dsqrt(ef**2-me2)
        qgv = dsqrt((pei-pef*dcos(thel))**2+(pef*dsin(thel))**2)
        egv = ei - ef
        qsq = qgv**2 - egv**2
        s = rmx2 - qsq + 2.d0*egv*rmnx
        w = dsqrt(s)
        egvcm = (s - rmx2 - qsq)/w/2.0d0
        qgvcm = dsqrt(egvcm**2 + qsq)
        pkcm = dsqrt((rmkx2 + rmhx2 - s)**2 - 4.0d0*rmkx2*rmhx2)/w/2.0d0
        ekcm = dsqrt(pkcm**2 + rmkx2)
        t = rmkx2 - qsq - 2.0d0*(ekcm*egvcm - ctc*qgvcm*pkcm)
        eps = 1.d0/(1.d0 + 2.d0*(qgv*dtan(thel/2.d0))**2/qsq)
        gflx = (alfa/pi)**2*ef*(s-rmx2)/4.d0/rmnx/ei/(1.d0-eps)/qsq
      else
        write(*,*)'#thele: wrong choice of input kinematics: ',ivar
        qgv = 1.0d0
        egv = qgv
        ctc = 1.d0
      end if
c
c  check if theta_K^cm is in the physical region
c
      if(dabs(ctc).gt.1.d0) then
        if(dabs(ctc).gt.1.01d0) write(*,100) ctc
        ctc = ctc/dabs(ctc)
        stc = 0.0d0
      else
        stc = dsqrt(1.0d0-ctc**2)
      end if
 100  format('#thele: cos(theta_K^cm) is out of physical region:',
     +       1x,f8.5)
c
c  input:  qgv, egv, ctc
c  output: ctl, q, k, pklab, f(1:6), a(1:6)
c
      call cgnl(qgv,egv,ctc,ctl,q,k,pklab,f,a)
c
c  check if theta_K^lab is in the physical region
c
      if (dabs(ctl).gt.1.0d0) then
        if(dabs(ctl).gt.1.01d0) write(*,110) ctl
        ctl = ctl/dabs(ctl)
        snl = 0.0d0
      else
        snl = dsqrt(1.0d0 - ctl**2)
      end if
 110  format('#thele: cos(th_K^lab) is out of physical region',f8.2)

      do j = 1,6
        fc(j) = dconjg(f(j))
        ac(j) = dconjg(a(j))
      end do
c
c  beam and recoil polarization response functions in c.m. 
c  by Knoechlein, Drechsler and Tiator, 
c  Z. Phys. A 352 (1995) 327, App. C
c
c  cross sections:  sigma_U(T)  (r00t),  sigma_L     (r00l),
c                   sigma_P(TT) (r00tt), sigma_I(TL) (r00tl)
c
c  polarization:  P= ryd0t/r00t,   
c
c
      r00t=  dble(fc(1)*f(1)+fc(2)*f(2)-2.d0*ctc*fc(1)*f(2)+
     1       stc**2*((fc(3)*f(3)+fc(4)*f(4))/2.d0+
     2       fc(2)*f(3)+fc(1)*f(4)+ctc*fc(3)*f(4)))
      r00l=  dble(fc(5)*f(5)+fc(6)*f(6)+2.d0*ctc*fc(5)*f(6))
      r00tl= stc*dble(-fc(2)*f(5)-fc(3)*f(5)-fc(1)*f(6)-
     1       fc(4)*f(6)-ctc*(fc(4)*f(5)+fc(3)*f(6)))
      r00tt= stc**2*dble((fc(3)*f(3)+fc(4)*f(4))/2.d0+ 
     1       fc(2)*f(3)+fc(1)*f(4)+ctc*fc(3)*f(4))

      rxd0tl=dble(imgc*(-fc(2)*f(5)+fc(1)*f(6)+ctc*(fc(1)*f(5)-
     1       fc(2)*f(6))))
      rxd0tt=stc*dble(imgc*(fc(2)*f(3)-fc(1)*f(4)+
     1       ctc*(fc(2)*f(4)-fc(1)*f(3))))
      ryd0t= dble(imgc*(stc*(-2.d0*fc(1)*f(2)-fc(1)*f(3)+fc(2)*f(4)+
     1       ctc*(fc(2)*f(3)-fc(1)*f(4))+stc**2*fc(3)*f(4))))
      ryd0l= 2.d0*stc*dble(imgc*(fc(5)*f(6)))
      ryd0tl=-dble(imgc*(-fc(1)*f(5)+fc(2)*f(6)+ctc*(fc(2)*f(5)-
     1       fc(1)*f(6))+stc**2*(fc(3)*f(6)-fc(4)*f(5))))
      ryd0tt=-dble(imgc*(stc*(fc(1)*f(3)-fc(2)*f(4)+
     1       ctc*(fc(1)*f(4)-fc(2)*f(3))-stc**2*fc(3)*f(4)))) 
      rzd0tl=stc*dble(imgc*(fc(1)*f(5)+fc(2)*f(6)))
      rzd0tt=-stc**2*dble(imgc*(fc(1)*f(3)+fc(2)*f(4)))
      r00tld=-stc*dble(imgc*(fc(2)*f(5)+fc(3)*f(5)+fc(1)*f(6)+
     1        fc(4)*f(6)+ctc*(fc(4)*f(5)+fc(3)*f(6))))
      rxd0tld=dble(-fc(2)*f(5)+fc(1)*f(6)+ctc*(fc(1)*f(5)-
     1        fc(2)*f(6)))
      rxd0ttd=stc*dble(-fc(1)*f(1)+fc(2)*f(2)+fc(2)*f(3)-
     1        fc(1)*f(4)+ctc*(fc(2)*f(4)-fc(1)*f(3)))
      ryd0tld=-dble(fc(1)*f(5)-fc(2)*f(6)+ctc*(fc(1)*f(6)-
     1        fc(2)*f(5))+stc**2*(fc(4)*f(5)-fc(3)*f(6)))
      rzd0tld=stc*dble(fc(1)*f(5)+fc(2)*f(6))
      rzd0ttd=dble(-2.d0*fc(1)*f(2)+ctc*(fc(1)*f(1)+fc(2)*f(2))-
     1        stc**2*(fc(1)*f(3)+fc(2)*f(4)))
c
c  normalization of CGLN amplitudes is like in 'thpho'
c
      rn  = 1000.d0*pkcm/qgvcm
      ell = qsq/egvcm**2
      dsa = rn*r00t
      dsb = rn*r00l*ell
      dsc = rn*r00tt
      dsd = rn*r00tl*dsqrt(ell)
c
c  output
c
      thkc = dacos(ctc)*180.0d0/pi
      thkl = dacos(ctl)*180.0d0/pi
      epcm = dsqrt(qgvcm**2+rmx2)
      trfm = (egv+rmnx)*pkcm**2*qgvcm*(pkcm+qgvcm*ctc*ekcm/epcm)
     /       /rmnx/pklab**3/qgv
      if(iprint.gt.1) then
        write(8,*)
        write(8,200)w,qsq,eps,t,thkc,thkl,phi,qgvcm,
     +              egvcm,pkcm,qgv,egv,pklab
      end if
 200  format(1x,'W= ',f6.3,' GeV,',2x,'Q^2= ',f6.3,' GeV^2,',
     + 2x,'eps= ',f5.3,',',2x,'t= ',f7.4,' GeV^2',/,
     + 1x,'th_K^cm = ',f5.1,' deg,',6x,'th_K^lab= ',f5.1,' deg,',
     + 3x,'phi = ',f5.1,' deg',/,
     + 1x,'q_Gv^cm = ',f6.3,' GeV/c,',3x,'e_Gv^cm = ',f6.3,' GeV,',
     + 2x,'p_K^cm = ',f6.3,' GeV/c',/,
     + 1x,'q_Gv^lab= ',f6.3,' GeV/c,',3x,'e_Gv^lab= ',f6.3,' GeV,',
     + 2x,'p_K^lab= ',f6.3,' GeV/c')

      do j= 1,18
        ths(j) = 0.0d0
      end do
      phi = phi*pi/180.0d0
      snn = 1000.0d0
      if(imod.eq.0) then
c
c  center of mass frame;  cross sections are in nb/sr
c
        ths(1) = dsa*snn
        ths(2) = dsb*snn
        ths(3) = dsc*snn
        ths(4) = dsd*snn
        ths(5) = (dsa + eps*dsb)*snn      
        ths(6) = (dsa + eps*dsb + eps*dsc*dcos(2.d0*phi)+
     +            dsqrt(2.d0*eps*(1.d0+eps))*dsd*dcos(phi))*snn
        if(iprint.gt.1) then
          write(8,*)
          write(8,220)'F(1)=',f(1),'F(2)=',f(2)
          write(8,220)'F(3)=',f(3),'F(4)=',f(4)
          write(8,220)'F(5)=',f(5),'F(6)=',f(6)
        end if
 220    format(1x,2(a5,1x,2(d11.4,1x),4x))
      else if(imod.eq.1) then
c
c  laboratory frame;  cross sections are in nb/sr
c
        ths(1) = dsa*snn/trfm
        ths(2) = dsb*snn/trfm
        ths(3) = dsc*snn/trfm
        ths(4) = dsd*snn/trfm
        ths(5) = (dsa + eps*dsb)*snn/trfm
        ths(6) = (dsa + eps*dsb + eps*dsc*dcos(2.d0*phi)+
     +            dsqrt(2.d0*eps*(1.d0+eps))*dsd*dcos(phi))*snn/trfm
        if(iprint.gt.1) then
          crsl = dble(a(1)*ac(1) + a(2)*ac(2) - 2.0d0*ctl*ac(1)*a(2) + 
     +                snl**2*((a(3)*ac(3) + a(4)*ac(4))/2.0d0 +
     +                ac(1)*a(4) + ac(2)*a(3) + ac(3)*a(4)*ctl) )
          ek = dsqrt(pklab**2 + rmkx2)
          el = egv + rmnx - ek
          dsl= 1000.d0*pklab**2*s*ek*el/(pklab*el+ek*(pklab-qgv*ctl))
     +         *crsl
          dsx= qgv*pklab**2*rmnx/pkcm/qgvcm/(pklab*(el+ek)-qgv*ek*ctl)
     +         *dsa
          write(8,*)
          write(8,220)'A(1)=',a(1),'A(2)=',a(2)
          write(8,220)'A(3)=',a(3),'A(4)=',a(4)
          write(8,220)'A(5)=',a(5),'A(6)=',a(6)
          write(8,*)
          write(8,360) ths(1),snn*dsx,snn*dsl
        end if
 360    format(1x,'LAB crs: F-->CMS-->LAB(1,2)',2(f16.8,1x),/,
     +         9x,' A-->LAB',11x,f16.8)
      else if(imod.eq.2) then
c
c  invariant differential cross sections in mub/GeV^2
c
        jacob = pi/qgvcm/pkcm
        ths(1) = dsa*jacob
        ths(2) = dsb*jacob
        ths(3) = dsc*jacob
        ths(4) = dsd*jacob
        ths(5) = (dsa + eps*dsb)*jacob
        ths(6) = (dsa + eps*dsb + eps*dsc*dcos(2.d0*phi)+
     +            dsqrt(2.d0*eps*(1.d0+eps))*dsd*dcos(phi))*jacob
      else if(imod.eq.3 .and. ivar.eq.4) then
c
c  full cross sections in CMS, LAB, and triple-differential in LAB
c
        ths(1) = dsa*snn/trfm
        ths(2) = dsb*snn/trfm
        ths(3) = (dsa + eps*dsb)*snn/trfm
        ths(4) = (dsa + eps*dsb + eps*dsc*dcos(2.d0*phi)+
     +            dsqrt(2.d0*eps*(1.d0+eps))*dsd*dcos(phi))*snn
        ths(5) = ths(4)/trfm
        ths(6) = ths(5)*gflx
      else if(imod.eq.4) then
c
c  response functions in microbarns/sr  
c  they are related to cross sections by the factor pkcm/pgl  
c  where  pgl is the real-photon equivalent momentum in c.m.s.
c  [Knochlein etal, Z.Phys.A352(1995)327; Kelly etal, nucl-ex/0509004] 
c
        rnrsp  = 1000.d0*(s-rmx2)/2.d0/w/qgvcm
        ths(1) = rnrsp*r00t
        ths(2) = rnrsp*r00l
        ths(3) = rnrsp*r00tl
        ths(4) = rnrsp*r00tt
        ths(5) = rnrsp*r00tld

        ths(6) = rnrsp*rxd0tl
        ths(7) = rnrsp*rxd0tt
        ths(8) = rnrsp*rxd0tld
        ths(9) = rnrsp*rxd0ttd

        ths(10) = rnrsp*ryd0t
        ths(11) = rnrsp*ryd0l
        ths(12) = rnrsp*ryd0tl
        ths(13) = rnrsp*ryd0tt
        ths(14) = rnrsp*ryd0tld

        ths(15) = rnrsp*rzd0tl
        ths(16) = rnrsp*rzd0tt
        ths(17) = rnrsp*rzd0tld
        ths(18) = rnrsp*rzd0ttd
      else if(imod.eq.5) then
c
c  polarization(P)-1, photon(Sigma)-2 and target(T)-3 asymmetry
c
        ths(1) =   ryd0t/r00t
        ths(2) =   r00tt/r00t
        ths(3) = -ryd0tt/r00t
      else if(imod.eq.21) then
c
c  averaged full invariant differential cross section
c  (P.Brauel et al Z.Phys.C 3 (1979) 101, Tab. 8)
c  averaging is performed over phi from 120 to 240 deg  
c  units are microbarn/sr
c
        jacob = pi/qgvcm/pkcm  
        snn = 3.d0*dsqrt(3.d0)/2.d0/pi
        ths(1) = dsa*jacob
        ths(2) = dsb*jacob  
        ths(3) = jacob*(dsa+eps*dsb+eps*dsc*snn+
     +           dsqrt(2.d0*eps*(1.d0+eps))*dsd*(-snn))
      end if

      return
      end
c
c-----------------------------------------------------------------------
c
      subroutine thpho(px1,px2,a1,a2,a3,ths)
c
c  calculates cross section and spin observables in the photoproduction 
c  utilizing the CGLN and transversity amplitudes;
c  transformation of the amplitudes CMS-->LAB, 
c  spin-(non)flip amplitudes for 'S1'
c
      implicit none
      character*3 a1,a2,a3
      real*8 px1,px2,ths(18),pi,hc,alfa,rmp,rmn,rmnx,rml,rms0,rmsp,rmsm,
     +       rmhx,rmhd,rmk,rmk0,rmkx,rmres(16),rmks0,qnx,qkx,qhx,kpp,
     +       kpn,kpnx,kpl,kps0,kpsp,kpsm,kphx,kpls,rg1(16),rg2(16),axx,
     +       qg,eg,ctl,k,q,ds,pol,dsl,poll,snl,ctc,s,ek,el,pkl,dslp,
     +       rmx2,rmkx2,x0,x1,snc,x2,pklb,eklb,w,e0l,e0n,l11,l12,l21,
     +       l22,l23,l24,l33,l34,l41,l42,aln,rft0,rgt0,rgtp,rgtm,rfgt,
     +       thkl,dcn,bte,bsig,bpol,bds,cx,cz,ox,oz,thkcm,egthr,rmhx2
      complex*16 f(1:6),fc(1:6),a(1:6),ac(1:6),cds,cpol,bcn,acn,cdsl,
     +           cpoll,al1,al2,al3,al4,ft0,gt0,gtp,gtm,cbcn,cacn,ccn,
     +           bta(1:4),cbta(1:4),cb14,cb23,at1,at2,at3,at4,atnorm,
     +           attd1,attd2,attd3,attd4
      integer j,iprint
      common/param/ pi,hc,alfa,iprint
      common/rmass/ rmp,rmn,rmnx,rml,rms0,rmsp,rmsm,rmhx,rmhd,rmk,rmk0,
     +              rmkx,rmres,rmks0
      common/emcpl/ qnx,qkx,qhx,kpp,kpn,kpnx,kpl,kps0,kpsp,kpsm,kphx,
     +              kpls,rg1,rg2,axx
c
c  qg, eg - photon LAB momentum and energy in GeV
c  thkcm  - kaon CMS scattering angle (with respect to the photon)
c  ctc    - cosinus of kaon CMS angle
c  tkl    - LAB kaon scattering angle
c  ctl    - cosinus of kaon LAB angle
c  q, k   - initial and final CMS momenta
c  pkl, pklb - kaon LAB momentum
c
      rmx2 = rmnx**2
      rmkx2 = rmkx**2
      rmhx2 = rmhx**2
c
c   kinematics for gamma + N --> K + Y  (photoproduction)  
c   photon LAB energy (Eg1), c.m. energy (W), s= W^2
c   kaon c.m. angle (th0), its cosinus (ct0), kaon LAB angle (th1)
c   and Mandelstam variable t
c
      egthr = ((rmkx+rmhx)**2-rmx2)/2.d0/rmnx
      if(a1.eq.'Eg1') then
        qg = px1
      else if(a1.eq.'  s') then
        qg = (px1 - rmx2)/2.d0/rmnx
      else if(a1.eq.'  W') then
        qg = (px1**2 - rmx2)/2.d0/rmnx
      else
        write(*,*)'#thpho: wrong input kinematics ',a1
        qg = egthr
      end if
      if(qg.lt.egthr) then
        write(*,*)'#thpho: photon lab energy is below threshold ',qg
        qg = egthr *1.0001d0
      end if
      eg = qg
      s = rmx2 + 2.d0*qg*rmnx
      if(a2.eq.'th0') then
        ctc = dcos(px2*pi/180.d0)
      else if(a2.eq.'ct0') then
        ctc = px2
      else if(a2.eq.'th1') then
c
c in the case there are two solutions for th_K^cm input is ambiguous;
c solution of the forward CMS angle is chosen
c
        q = (s - rmx2)/2.d0/dsqrt(s)
        k = dsqrt(((rmkx2 + rmhx2 - s)**2 - 4.d0*rmkx2*rmhx2)/4.d0/s)
        x0 = dcos(px2/180.0d0*pi)
        x1 = s + rmkx2 - rmhx2
        x2 = s + qg**2*(1.0d0 - x0**2)
        pklb=((qg+rmnx)*dsqrt(x1**2-4.d0*rmkx2*x2)+x1*qg*x0)/2.d0/x2
        eklb = dsqrt(rmkx2 + pklb**2)
        ctc = (qg*(pklb*x0 - eklb) + q*dsqrt(k**2 + rmkx2))/q/k   
      else if(a2.eq.'  t') then
        q = (s - rmx2)/2.d0/dsqrt(s)
        k = dsqrt(((rmkx2 + rmhx2 - s)**2 - 4.d0*rmkx2*rmhx2)/4.d0/s)
        ctc = (px2 - rmkx2 + 2.d0*dsqrt(rmkx2 + k**2)*q)/2.d0/k/q
      else
        write(*,*)'#thpho: wrong input kinematics ',a2
        ctc = 1.d0
      end if
c
c  check if thkcm (theta_K^cm) is in the physical region
c
      if (dabs(ctc).gt.1.0d0) then
        if(dabs(ctc).gt.1.01d0) write(*,100) ctc
        ctc = ctc/dabs(ctc)
        snc = 0.0d0
      else
        snc = dsqrt(1.0d0 - ctc**2)
      end if
      thkcm= dacos(ctc)*180.d0/pi
 100  format('#thpho: cos(th_K^cm) is out of physical region',f8.2)
c
c  CGLN photoproduction amplitudes in CMS (f) and LAB (a) are
c  calculated together with laboratory kaon scattering angle
c  (ctl - cosinus of tkl), initial (q) and final (k) CMS momenta,  
c  and laboratory kaon momentum (pkl)
c
      call cgnl(qg,eg,ctc,ctl,q,k,pkl,f,a)
c
c  check if tkl (theta_K^LAB) is in the physical region
c
      if (dabs(ctl).gt.1.0d0) then
        if(dabs(ctl).gt.1.01d0) write(*,110) ctl
        ctl = ctl/dabs(ctl)
        snl = 0.0d0
      else
        snl = dsqrt(1.0d0 - ctl**2)
      end if
 110  format('#thpho: cos(th_K^lab) is out of physical region',f8.2)
c
c *** OUTPUT
c
      do j= 1,18
        ths(j)= 0.d0
      end do

      if(a3.eq.' SP') then
c
c  c.m. cross section and spin observables from transversity amplitudes
c  Adelseck, Saghai, Phys.Rev.C42(1990)108, eqs. 10
c
c  ###  v eq.6 je u F_2 opacne znamenko oproti Slavkove definici 
c       avsak v eqs. 10 je nutne vzit minus !!! nekonsistence ?!
c
        bcn = dcmplx(ctc,snc)
        acn = cdsqrt(bcn)
        cbcn= dconjg(bcn)
        cacn= dconjg(acn)
        ccn = (0.0d0,1.0d0)/dsqrt(2.0d0)
        dcn = snc/dsqrt(2.0d0)
        bta(1)= -ccn*(f(1)-f(2)*cbcn)*acn 
        bta(2)=  ccn*(f(1)-f(2)*bcn)*cacn
        bta(3)= -bta(1)-dcn*(f(3)+f(4)*cbcn)*acn
        bta(4)= -bta(2)-dcn*(f(3)+f(4)*bcn)*cacn
        do j = 1,4
          cbta(j) = dconjg(bta(j))
        end do
        bds=  1000.d0*k/q*dble(bta(1)*cbta(1)+bta(2)*cbta(2)+
     +        bta(3)*cbta(3)+bta(4)*cbta(4))/2.d0
        bpol= 1000.d0*k/q*dble(bta(1)*cbta(1)-bta(2)*cbta(2)+
     +        bta(3)*cbta(3)-bta(4)*cbta(4))/2.d0/bds
        bsig= 1000.d0*k/q*dble(bta(1)*cbta(1)+bta(2)*cbta(2)-
     -        bta(3)*cbta(3)-bta(4)*cbta(4))/2.d0/bds
        bte=  1000.d0*k/q*dble(bta(1)*cbta(1)-bta(2)*cbta(2)-
     -        bta(3)*cbta(3)+bta(4)*cbta(4))/2.d0/bds
        cb14= bta(1)*cbta(4)
        cb23= bta(2)*cbta(3)    
        cx=-1000.d0*k/q*dimag(cb14-cb23)/bds
        cz= 1000.d0*k/q*dble(cb14+cb23)/bds
        ox= 1000.d0*k/q*dble(cb14-cb23)/bds
        oz= 1000.d0*k/q*dimag(cb14+cb23)/bds
        ths(1) = bds
        ths(2) = bpol
        ths(3) = bsig
        ths(4) = bte
        ths(5) = cx
        ths(6) = cz
        ths(7) = ox
        ths(8) = oz
        if(iprint.gt.1) then
          write(8,*)
          write(8,1200) qg,q,k,thkcm
          write(8,1215)'bta(1)=',bta(1),'bta(3)=',bta(3)
          write(8,1215)'bta(2)=',bta(2),'bta(4)=',bta(4)
        end if
 1200   format(1x,'p_g^lab=',f6.3,3x,'p_g^cm=',f7.4,3x,'p_K^cm=',
     +         f7.4,'   GeV/c',3x,'th_K^cm=',f6.2,' deg')
 1215   format(1x,2(a7,1x,2(d11.4,1x),4x))

      else if(a3.eq.' CR') then
c
c  c.m., lab and invariant cross sections and polarizations
c
c  ds   - cross section in CMS in microbarn/sr
c  dsl  -               in LAB
c       - invariant cross section in microbarn/GeV^2
c  pol  - LAMBDA polarization in CMS
c
        do j = 1,6
          fc(j) = dconjg(f(j))
          ac(j) = dconjg(a(j))
        end do
        cds= f(1)*fc(1) + f(2)*fc(2) - 2.0d0*ctc*fc(1)*f(2) +
     +       (1.0d0-ctc**2)*( (f(3)*fc(3) + f(4)*fc(4))/2.0d0 +
     +       fc(1)*f(4) + fc(2)*f(3) + fc(3)*f(4)*ctc ) 
        cpol= dsqrt(1.0d0-ctc**2)*( -2.0d0*fc(1)*f(2) - fc(1)*f(3) +
     +       fc(2)*f(4) + (1.0d0-ctc**2)*fc(3)*f(4) +
     +       ctc*(fc(2)*f(3) - fc(1)*f(4)) )
        ds=  1000.0d0*k/q*dble(cds)
        pol= dimag(cpol)/dble(cds)
        cdsl= a(1)*ac(1) + a(2)*ac(2) - 2.0d0*ctl*ac(1)*a(2) +
     +        (1.0d0-ctl**2)*( (a(3)*ac(3) + a(4)*ac(4))/2.0d0 +
     +        ac(1)*a(4) + ac(2)*a(3) + ac(3)*a(4)*ctl )
        ek= dsqrt(rmkx2 + pkl**2)
        el= dsqrt(rmhx2 + qg**2 + pkl**2 - 2.0d0*qg*pkl*ctl)
        dsl=  1000.d0*pkl**2*s*ek*el/(pkl*el+ek*(pkl-qg*ctl))*dble(cdsl)
        ths(1) = ds
        ths(2) = dsl
        ths(3) = ds*pi/k/q
        ths(4) = pol
        if(iprint.gt.1) then
c
c  various amplitudes and t-matrices are evaluated
c
c    kinematics is printed out
c
          thkl= dacos(ctl)*180.d0/pi
          write(8,*)
          write(8,1177)
          write(8,1777)qg,thkl,q,pkl
          write(8,1177)
 1177     format(78('-'))
 1777     format(1x,'p_g^lab=',f6.3,3x,'th_K^lab=',f7.3,3x,
     +    'p_g^cm=',f6.3,3x,'p_K^lab=',f6.3,'  in GeV/c')
c
c    CGLN amplitudes are printed out
c
          write(8,*)
          write(8,1200) qg,q,k,thkcm
          write(8,1210)'F(1)=',f(1),'F(3)=',f(3)
          write(8,1210)'F(2)=',f(2),'F(4)=',f(4)
c
c    LAB cross section from the c.m. one and polarization in LAB 
c    are calculated for a check
c
          dslp= qg*pkl**2*rmnx/k/q/(pkl*(el+ek)-qg*ek*ctl)*ds
          cpoll= dsqrt(1.0d0-ctl**2)*( -2.0d0*ac(1)*a(2) -ac(1)*a(3) +
     +           ac(2)*a(4) + (1.0d0-ctl**2)*ac(3)*a(4) +
     +           ctl*(ac(2)*a(3) - ac(1)*a(4)) )
          poll= dimag(cpoll)/dble(cdsl)
          write(8,*)
          write(8,1220) pol,poll,pol-poll
1210      format(1x,2(a5,1x,2(d11.4,1x),4x))
1220      format(1x,'P from CMS and LAB amplitudes: ',f10.7,3x,f10.7,
     +           2x,'difference: ',d8.1)

c
c    transformation [Phys.Rev.43(1991)2753] of CMS amplitudes 
c    f(i) to LAB A(i) ones by T.Motoba,M.Sotona,K.Itonaga.     
c
        w= dsqrt(s)
        e0l= dsqrt(rmhx2 + k**2)
        e0n= dsqrt(rmx2 + q**2)
        l11=(w+rmnx)*(1.d0-(w-rmnx)*(qg-pkl*ctl)/(w+rmnx)/(el+rmhx))
        l12=(e0l+rmhx)*((w+rmnx)*(qg-pkl*ctl)-(w-rmnx)*(el+rmhx))/
     /      (el+rmhx)/k
        l21=-pkl*(w-rmnx)/(el+rmhx)
        l22=(w+rmnx)*(e0l+rmhx)*pkl/(el+rmhx)/k
        l23=(w+rmnx-qg*(w-rmnx)/(el+rmhx))*pkl/k
        l24=(e0l+rmhx)*(w-rmnx-qg*(w+rmnx)/(el+rmhx))*pkl/k**2
        l33=(w-rmnx)*pkl**2/(el+rmhx)/k
        l34=(w+rmnx)*(e0l+rmhx)*pkl**2/(el+rmhx)/k**2
        l41=(w-rmnx)*pkl/(el+rmhx)
        l42=-(w+rmnx)*(e0l+rmhx)*pkl/(el+rmhx)/k 
        aln=dsqrt(2.d0*qg*(el+rmhx)/ek/el/(e0l+rmhx)/(e0n+rmnx))/
     /      2.d0/(w-rmnx)/4.d0/pi**2
c
c    A(i) = ali  
c
        al1= aln*(l11*f(1)+l12*f(2))
        al2= aln*(l21*f(1)+l22*f(2)+l23*f(3)+l24*f(4))
        al3= aln*(l33*f(3)+l34*f(4))
        al4= aln*(l41*f(1)+l42*f(2))*(0.d0,-1.d0)
c
c   amplitudes Atild(i) = ati by T.Motoba 
c
        atnorm= (0.d0,1.d0)*dsqrt(1000.d0)*(2.d0*pi)**2/dsqrt(s) 
        at1= atnorm*al1
        at2= atnorm*al2
        at3= atnorm*al3
        at4= atnorm*al4
c
c   spin-flip and spin-non-flip amplitudes are calculated from 
c   Atild(i) using (2.12) in Prog.Theor.Phys.Suppl.117(1994)123
c
        ft0= at4*snl
        gt0= at1
        gtp= (-at1-at3*snl**2-(0.d0,1.d0)*snl*(at2+at3*ctl))/
     /       dsqrt(2.d0)    
        gtm= (at1+at3*snl**2-(0.d0,1.d0)*snl*(at2+at3*ctl))/
     /       dsqrt(2.d0)  
c
c   Toshio's amplitudes (attdi) directly from a(i)
c
        atnorm= (0.0d0,1.0d0)*dsqrt(1000.d0)
        attd1= atnorm*(a(1)-a(2)*ctl)
        attd2= atnorm*(a(2)+a(3))
        attd3= atnorm*a(4)
        attd4= atnorm*(0.0d0,1.0d0)*a(2)
c
c   the amplitudes are printed out
c
        write(8,177)' LAB amplitudes',' CMS amplitudes' 
        write(8,1122)'a(1)=',a(1),  'f(1)=',f(1) 
        write(8,1122)'a(2)=',a(2),  'f(2)=',f(2)
        write(8,1122)'a(3)=',a(3),  'f(3)=',f(3) 
        write(8,1122)'a(4)=',a(4),  'f(4)=',f(4)
 1122   format(1x,a10,f12.8,1x,f12.8,5x,a10,f12.8,1x,f12.8)
 177    format(1x,a25,15x,a25)
        write(8,77)
 77     format(1x,f12.8,1x,f12.8)
c
c   the LAB cross sections are printed as a check of consistency
c
        write(8,778)dslp 
        write(8,779)dsl 
 778    format(1x,'cross section:',2x,'f(i)-->cms-->lab:',f12.4,
     +         1x,'microbarn/sr') 
 779    format(1x,'cross section:',2x,'a(i)-------->lab:',f12.4,
     +         1x,'microbarn/sr')
        write(8,77)
        write(8,177)'f(i)-->A(i)'
        write(8,1122)'A(1)= ',al1  
        write(8,1122)'A(2)= ',al2  
        write(8,1122)'A(3)= ',al3  
        write(8,1122)'A(4)= ',al4 
        write(8,77)
c
c   A(i) are used to calculate the cross section in LAB  
c
        x0= al1*dconjg(al1)+al4*dconjg(al4)*snl**2/2.d0+
     +      snl**2*(al3*dconjg(al1+ctl*al2)+(al2*dconjg(al2)+
     +      al3*dconjg(al3))/2.d0)
        x1= (2.d0*pi)**4*pkl**2*ek*el/(pkl*(el+ek)-qg*ek*ctl)
        x2= x0*x1*1000.d0  
        write(8,1111)x2,x1
 1111   format(1x,'cross section: f(i)-->A(i)-->lab:',f9.4,
     1         1x,'microbarn/sr',3x,'norm.:',d12.5)
        write(8,77)
        write(8,179)'amplitudes by Toshio'
        write(8,177)' f(i)-->A(i)-->Atild(i)','a(i)-->Attd(i)'
        write(8,1122)'Atild(1)=',at1,  'Attd(1)=',attd1 
        write(8,1122)'Atild(2)=',at2,  'Attd(2)=',attd2 
        write(8,1122)'Atild(3)=',at3,  'Attd(3)=',attd3
        write(8,1122)'Atild(4)=',at4,  'Attd(4)=',attd4 
        write(8,77)
 179    format(1x,a20)
c
c   Atild(i) are used to calculate the cross section in LAB  
c
        x0= at1*dconjg(at1)+at4*dconjg(at4)*snl**2/2.d0+
     +      snl**2*(at3*dconjg(at1+ctl*at2)+(at2*dconjg(at2)+
     +      at3*dconjg(at3))/2.d0)
        x1= pkl**2*s*ek*el/(pkl*(el+ek)-qg*ek*ctl)
        x2= x0*x1 
        write(8,1112)x2,x1
 1112   format(1x,'cross section: Atild(i)-->lab:',f9.4,
     1         1x,'microbarn/sr',3x,'norm.:',d12.5)
c
        rft0= ft0*dconjg(ft0) 
        rgt0= gt0*dconjg(gt0) 
        rgtp= gtp*dconjg(gtp) 
        rgtm= gtm*dconjg(gtm) 
        rfgt= ft0*dconjg(gt0) 
        write(8,77) 
        write(8,178)'Atild(i)-->spin-flip, spin-non-flip'
        write(8,1122)'f0= ',ft0
        write(8,1122)'g0= ',gt0 
        write(8,1122)'gp= ',gtp 
        write(8,1122)'gm= ',gtm 
        write(8,77)
        write(8,1124)'|f0|^2= ',rft0  
        write(8,1124)'|g0|^2= ',rgt0  
        write(8,1124)'|gp|^2= ',rgtp  
        write(8,1124)'|gm|^2= ',rgtm
        write(8,1124)'Re(fg)= ',rfgt
        write(8,77)
 178    format(1x,a40)
 1124   format(1x,a10,d14.7)
c
c   calculating the LAB cross section from f and g's
c   units of x0 are: microbarns/sr
c
        x0= (rft0+rgt0+rgtp+rgtm)/2.0d0
        x1= pkl**2*s*ek*el/(pkl*(el+ek)-qg*ek*ctl)
        x0= x0*x1
        write(8,1123)x0,x1
 1123   format('cross section: Atild(i)-->f,g-->lab:',
     +         f9.4,1x,'microb/sr',3x,'norm.:',d12.5)
        write(8,*)

        end if
      else
        write(*,*)'#thpho: wrong output variable ',a3
      end if

      return
      end
c
c-----------------------------------------------------------------------
c
      subroutine cgnl(qg,og,ctc,ctl,ki,kf,pkl,cf,ca)
c
c  calculates CGLN amplitudes 
c  [Sotona, Frullani, Prog.Theor.Phys.Suppl.117(1994)151, eq.(2.9)] 
c  from the invariant amplitudes A_i, eqs.(2.8) (correspond to N_i basis)
c
      implicit none
      real*8 qg,og,ctl,ctc,kf,ki,pkl,rmp,rmn,rmnx,rml,rms0,rmsp,rmsm,
     +       rmhx,rmhd,rmk,rmk0,rmkx,rmres(16),rmks0,pi,hc,alfa,qg2,s,
     +       w,eg,ek,en,eh,t,ok,oh,u,ppl,ppn,n,rmnx2,rmhx2,rmkx2
      integer iprint
      complex*16 cf(1:6),ca(1:6),ai(6)
      common/param/ pi,hc,alfa,iprint
      common/rmass/ rmp,rmn,rmnx,rml,rms0,rmsp,rmsm,rmhx,rmhd,rmk,rmk0,
     +              rmkx,rmres,rmks0
c
c  qg, og - photon LAB momentum and energy (virtual-photon mass qg2 < 0)
c  ctc - cos(theta_K^cm)
c  cf, ca - CMS and LAB CGLN amplitudes
c  virtual-photon c.m. energy (eg) can be negative for large qg2
c  (the photon moves in opposite direction with positive energy)
c
      rmnx2 = rmnx**2
      rmhx2 = rmhx**2
      rmkx2 = rmkx**2
      qg2 = og**2 - qg**2
      s = qg2 + rmnx2 + 2.0d0*og*rmnx
      w = dsqrt(s)
      ki = dsqrt((s -rmnx2 -qg2)**2 - 4.0d0*rmnx2*qg2)/w/2.0d0
      eg= (s - rmnx2 + qg2)/2.d0/w
      en= dsqrt(ki**2 + rmnx2)
      kf = dsqrt((s -rmhx2 -rmkx2)**2 - 4.d0*rmhx2*rmkx2)/w/2.0d0
      ek= dsqrt(kf**2 + rmkx2)
      eh= dsqrt(kf**2 + rmhx2)
      t = qg2 + rmkx2 - 2.0d0*eg*ek + 2.0d0*ki*kf*ctc
      ok= (en*ek + ki*kf*ctc)/rmnx
      pkl = dsqrt(ok**2 - rmkx2)
      ctl = (og*ok - eg*ek + ki*kf*ctc)/qg/pkl
      oh = og + rmnx - ok
      u = qg2 + rmnx2 + rmhx2 + rmkx2 - s - t
      ppl = (rmhx2 + qg2 - u)/2.0d0
      ppn = (s - rmnx2 - qg2)/2.0d0
c
c  invariant amplitudes A_i  
c
      call iampl(s,qg2,s,t,u,ai)
c
c  CGLN amplitudes in CMS 
c
      n= dsqrt( 10.d0*(en+rmnx)*(eh+rmhx)*ki/2.0d0/w/(s-rmnx2) )*hc
      cf(1)= -n*(-(w-rmnx)*ai(1)+ppn*ai(4)+ppl*ai(5)-qg2*ai(6))
      cf(2)=  n*ki*kf/(en+rmnx)/(eh+rmhx)*(-(w+rmnx)*ai(1)-ppn*ai(4)-
     +        ppl*ai(5)+qg2*ai(6))
      cf(3)=  n*ki*kf/(en+rmnx)*(ai(3)+(w+rmnx)*ai(5))
      cf(4)= -n*kf**2/(eh+rmhx)*(ai(3)-(w-rmnx)*ai(5))
      if(dabs(qg2).lt.1.0d-10) then
c
c  real photon case
c
        cf(5)= (0.d0,0.d0)
        cf(6)= (0.d0,0.d0)

      else
        cf(5)= -n*ki**2/(en+rmnx)*(ai(1)-ai(2)-(ppn*ai(2)+
     +          ppl*ai(3))/qg2-(w+rmnx)*(ai(4)+ai(6)))
        cf(6)= -n*eg*ki*kf/(en+rmnx)/(eh+rmhx)*(ai(1)+
     +         (en+rmnx)/eg*(ai(2)+(ppn*ai(2)+ppl*ai(3))/qg2)-
     +         rmnx*ai(4)+ai(5)*ppl/eg-(w+rmnx)*ai(6))
      end if
      cf(5)= cf(1) + ctc*cf(3) + cf(5)
      cf(6)= cf(6) + ctc*cf(4)
c
c  CGLN amplitudes in LAB
c
      n= dsqrt( 10.d0*(oh+rmhx)*ki/w/qg/ok/oh/(s-rmnx2) )*hc
      ca(1)=  n*(-(og-qg**2/(oh+rmhx))*ai(1)+og*rmnx*ai(4)+ppl*ai(5)-
     +        qg2*ai(6))
      ca(2)=  n*qg*pkl/(oh+rmhx)*ai(1)
      ca(3)= -n*qg*pkl/(oh+rmhx)*(ai(3)+(oh+rmhx-og)*ai(5))
      ca(4)=  n*pkl**2/(oh+rmhx)*(ai(3)-og*ai(5))
      if(dabs(qg2).lt.1.0d-10) then
c
c  real photon case
c
        ca(5)= (0.d0,0.d0)
        ca(6)= (0.d0,0.d0)
      else
        ca(5)= n*qg**2/(oh+rmhx)*(-ai(1)+ai(3)-(rmnx*og*ai(2)+
     +         ppl*ai(3))/qg2+(oh+rmhx-og)*(ai(5)-ai(6)))
        ca(6)= n*qg*pkl/(oh+rmhx)*(-ai(3)+(rmnx*og*ai(2)+ppl*ai(3))/qg2+
     +         og*(ai(5)-ai(6)))
      end if
      ca(5)= ca(1) + ctl*ca(3) + ca(5)
      ca(6)= ca(6) + ctl*ca(4)

      return
      end
c
c-----------------------------------------------------------------------
c
      subroutine iampl(w2,qg2,s,t,u,ai)
c
c   Calculates invariant amplitudes A_i for the photo-production or 
c   radiative capture. The calculation is performed in the basis N^i 
c   Eq.6, in  Williams, Ji, Cotanch, Phys.Rev.C46(1992)1617. 
c  (N7 and N8 must be checked for crossing!)
c
c   virtual-photon mass squared qg2 < 0
c
      implicit none
      integer iprint,parita(16),spin(16),ispin(16),chres(16),nmbr,
     +        ihff,iemff,i
      character*2 nck(16),vmd
      real*8 w2,qg2,s,t,u,pi,hc,alfa,rmp,rmn,rmnx,rml,rms0,rmsp,rmsm,
     +    rmhx,rmhd,rmk,rmk0,rmkx,rmres(16),rmks0,qnx,qkx,qhx,kpp,kpn,
     +    kpnx,kpl,kps0,kpsp,kpsm,kphx,kpls,rg1(16),rg2(16),axx,gkln,
     +    gks0n,gkhnx,gklsnx,g1(16),g2(16),g1x(16),g2x(16),hffct1,
     +    hffct2,hffa1,hffa2,emvp,emvn,emvk,emvks,emvk1,w,ppl,ppn,fnx1,
     +    fnx2,fhx1,fhx2,fls2,xfp1,xfp2,xfn1,xfn2,aqhx,par,rmas,hffs,
     +    hfft,hffu,hffud,hfftld,hffr(16),hdrff0,hdrff1,hdrff2,ghf1,
     +    ghf2,rmksdl,ffc1,ffc2,ffn1,ffn2
      complex*16 mp,mn,mnx,ml,ms0,msp,msm,mhx,mhd,mk,mk0,mkx,mres(16),
     +         ai(6),fk,fn2(16),xfk,xfks,xfk1,gprop,mas,wdtt,ffm,fvmdm
      common/param/ pi,hc,alfa,iprint
      common/mass2/ mp,mn,mnx,ml,ms0,msp,msm,mhx,mhd,mk,mk0,mkx,mres
      common/rmass/ rmp,rmn,rmnx,rml,rms0,rmsp,rmsm,rmhx,rmhd,rmk,rmk0,
     +              rmkx,rmres,rmks0
      common/resns/ parita,spin,ispin,chres,nck,nmbr,vmd
      common/emcpl/ qnx,qkx,qhx,kpp,kpn,kpnx,kpl,kps0,kpsp,kpsm,kphx,
     +              kpls,rg1,rg2,axx
      common/stcpl/ gkln,gks0n,gkhnx,gklsnx,g1,g2,g1x,g2x
      common/hdrff/ hffct1,hffct2,hffa1,hffa2,ihff
      common/elmff/ emvp,emvn,emvk,emvks,emvk1,iemff
c
c  total c.m. energy of the system
c
      w = dsqrt(w2)
c  the sign shouldn't be changed since ppl-ppn cancel with t-rmkx**2
c  (see K* and K1 exchanges and the ai-->aj transformation)
c
      ppl = (rmhx**2 + qg2 - u)/2.0d0
      ppn = (s - rmnx**2 - qg2)/2.0d0
c
c  Electromagnetic form factors (normalized to unity)
c
      fnx1 = qnx
      fnx2 = 1.0d0
      fhx1 = qhx
      fhx2 = 1.0d0
      fls2 = 1.0d0
      fk = (1.0d0,0.0d0)
      do i= 1,nmbr
        fn2(i) = (1.0d0,0.0d0)
      end do
      if (dabs(qg2).gt.1.0d-10) then
c
c  electroproduction for iemff= 1,2,3, and 4 
c
        xfp1= 1.0d0
        xfp2= 1.0d0
        xfn1= 0.0d0
        xfn2= 1.0d0
        xfk =  (1.0d0,0.0d0)
        xfks = (1.0d0,0.0d0)
        xfk1 = (1.0d0,0.0d0)
        if(iemff.eq.1) then
c
c  GK92  W  A  A
c
          call fevmd(-qg2,xfp1,xfp2,xfn1,xfn2)
          xfk = fvmdm(qg2,0)
c
c   Adelsek-Wright PRC38(1988)1965.
c
          xfks = 1.0d0/(1.0d0 - qg2/emvks**2)
          xfk1 = 1.0d0/(1.0d0 - qg2/emvk1**2)
        else if(iemff.eq.2) then
c
c  GK92  W  W  W
c
          call fevmd(-qg2,xfp1,xfp2,xfn1,xfn2)
          xfk = fvmdm(qg2,0)
          xfks = fvmdm(qg2,1)
          xfk1 = fvmdm(qg2,1)
        else if(iemff.eq.3) then
c
c  GK92  W  W  W ; Meson width is assumed 
c
          call fevmd(-qg2,xfp1,xfp2,xfn1,xfn2)
          xfk = fvmdm(qg2,10)
          xfks = fvmdm(qg2,11)
          xfk1 = fvmdm(qg2,11)
        else if(iemff.eq.5) then
c
c  phenomenological, masses are read from the file 'model'
c  here appropriate mass of the proton is used
c
          xfp1 = ffc1(qg2,kpp,rmp,emvp)
          xfp2 = ffc2(qg2,rmp,emvp)
          xfn1 = ffn1(qg2,rmn,emvn)
          xfn2 = ffn2(qg2,rmn,emvn)
          xfk = ffm(qg2,emvk)
          xfks = ffm(qg2,emvks)
          xfk1 = ffm(qg2,emvk1)
        end if
        fnx1 = qnx*xfp1 + (1.0d0 - qnx)*xfn1
        fnx2 = qnx*xfp2 + (1.0d0 - qnx)*xfn2
        aqhx= dabs(qhx)
        fhx1 =  qhx*xfp1 + (1.0d0 - aqhx)*xfn1
        fhx2 = aqhx*xfp2 + (1.0d0 - aqhx)*xfn2
        fls2 = xfn2
        fk = xfk
        do i= 1,nmbr
          if(chres(i).eq.1) then
            fn2(i) = dcmplx(fnx2,0.0d0)
          else if(chres(i).eq.2) then
            if(parita(i).eq.-1) then
              fn2(i) = xfks
            else if(parita(i).eq.1) then
              fn2(i) = xfk1
            end if
          else if(chres(i).eq.3) then
            fn2(i) = dcmplx(fhx2,0.0d0)
          end if
        end do        
        if(iemff.eq.4) then
c
c  Model C4: R.A.Williams et al Phys.Rev.C46(1992)1617. 
c  !!!                          (GK92)   
c
          call fevmd(-qg2,xfp1,xfp2,xfn1,xfn2)
          xfk = fvmdm(qg2,10)
          xfks = fvmdm(qg2,11)
          xfk1 = fvmdm(qg2,11)
c
c    proton electric and magnetic f.f. ( qqg < 0 )
c
          fnx1 = xfp1 + kpp*xfp2*qg2/4.d0/rmp**2
          fnx2 = xfp1 + kpp*xfp2 - 1.0d0
          fhx1 = 0.0d0
          fhx2 = xfn2/(kpp+1.0d0)
          fls2 = fhx2
          fk = xfk
          do i= 1,nmbr
            if(chres(i).eq.1) then
              fn2(i) = dcmplx(fhx2,0.0d0)
            else if(chres(i).eq.2) then
              if(parita(i).eq.-1) then
                fn2(i) = xfks
              else if(parita(i).eq.1) then
                fn2(i) = xfk1
              end if
            else if(chres(i).eq.3) then
              fn2(i) = dcmplx(fhx2,0.0d0)
            end if
          end do        
        end if
      end if
c
c  Hadronic form factors
c
      if(ihff.eq.1) then
        hffs=  hdrff0(hffct1,t,rmkx)
        hfft=  hffs
        hffu=  hffs
        hffud= hffs
        hfftld=hffs
        do i=1,nmbr
          hffr(i)= hffs
        end do
      else if(ihff.eq.2) then
        hffs=  hdrff1(hffct1,s,rmnx)
        hfft=  hdrff1(hffct1,t,rmkx)
        hffu=  hdrff1(hffct1,u,rmhx)
        hffud= hdrff1(hffct1,u,rmhd)
        hfftld=dsin(hffa1*pi/180.d0)**2*(dcos(hffa2*pi/180.d0)**2*hffs+
     +     dsin(hffa2*pi/180.d0)**2*hffu)+dcos(hffa1*pi/180.d0)**2*hfft
        do i=1,nmbr
          if(chres(i).eq.1) then
            hffr(i)= hdrff1(hffct2,s,rmres(i))
          else if(chres(i).eq.2) then
            hffr(i)= hdrff1(hffct1,t,rmres(i))
          else if(chres(i).eq.3) then
            hffr(i)= hdrff1(hffct1,u,rmres(i))
          end if
        end do
      else if(ihff.eq.3) then
        hffs=  hdrff1(hffct1,s,rmnx)
        hfft=  hdrff1(hffct1,t,rmkx)
        hffu=  hdrff1(hffct1,u,rmhx)
        hffud= hdrff1(hffct1,u,rmhd)
        hfftld= hffs + hfft - hffs*hfft
        do i=1,nmbr
          if(chres(i).eq.1) then
            hffr(i)= hdrff1(hffct1,s,rmres(i))
          else if(chres(i).eq.2) then
            hffr(i)= hdrff1(hffct1,t,rmres(i))
          else if(chres(i).eq.3) then
            hffr(i)= hdrff1(hffct1,u,rmres(i))
          end if
        end do
      else if(ihff.eq.4) then
        hffs=  hdrff1(hffct1,s,rmnx)
        hfft=  hdrff1(hffct1,t,rmkx)
        hffu=  hdrff1(hffct1,u,rmhx)
        hffud= hdrff1(hffct1,u,rmhd)
        hfftld= hffs + hfft - hffs*hfft
        do i=1,nmbr
          if(chres(i).eq.1) then
            hffr(i)= hdrff1(hffct2,s,rmres(i))
          else if(chres(i).eq.2) then
            hffr(i)= hdrff1(hffct1,t,rmres(i))
          else if(chres(i).eq.3) then
            hffr(i)= hdrff1(hffct1,u,rmres(i))
          end if
        end do
      else if(ihff.eq.5) then
        hffs=  hdrff2(hffct1,s,rmnx)
        hfft=  hdrff2(hffct1,t,rmkx)
        hffu=  hdrff2(hffct1,u,rmhx)
        hffud= hdrff2(hffct1,u,rmhd)
        hfftld= hffs + hfft - hffs*hfft
        do i=1,nmbr
          if(chres(i).eq.1) then
            hffr(i)= hdrff2(hffct2,s,rmres(i))
          else if(chres(i).eq.2) then
            hffr(i)= hdrff2(hffct1,t,rmres(i))
          else if(chres(i).eq.3) then
            hffr(i)= hdrff2(hffct1,u,rmres(i))
          end if
        end do
      else
        hffs=  1.0d0
        hfft=  1.0d0
        hffu=  1.0d0
        hffud= 1.d0
        hfftld=1.0d0
        do i=1,nmbr
          hffr(i)= 1.d0
        end do
      end if

      do i=1,6
        ai(i)=(0.0d0,0.0d0)
      end do
c
c   BORN TERMS
c   Anomalous magnetic moment of particle 'x' is in units eh/2m_x
c
c     Nucleon (s-channel)
c
      gprop= alfa *gkhnx /(s-mnx)
      ai(1)=ai(1) + gprop *(fnx1 + kpnx*fnx2) *hffs 
      ai(2)=ai(2) + gprop *2.0d0*fnx1 *hfftld 
      ai(4)=ai(4) + gprop *kpnx*fnx2/rmnx *hffs
      ai(6)=ai(6) - gprop* kpnx*fnx2/rmnx/2.0d0 *hffs
c
c     Hyperon (u-channel)
c
      gprop= alfa *gkhnx /(u-mhx)
      ai(1)=ai(1) + gprop *(fhx1 + kphx*fhx2) *hffu
      ai(3)=ai(3) + gprop *2.d0*fhx1 *hfftld
      ai(5)=ai(5) + gprop *kphx*fhx2/rmhx *hffu
      ai(6)=ai(6) + gprop *kphx*fhx2/rmhx/2.0d0 *hffu
c
c     Kaon (t-channel)
c
      gprop= alfa *qkx*fk *gkhnx*hfftld /(t-mkx)
      ai(2)=ai(2) + gprop*2.0d0
      ai(3)=ai(3) - gprop*2.0d0
c
c     Lambda-Sigma (Transition magnetic moment is in eh/(m_L + m_S0))
c
      gprop= alfa *kpls*fls2 *gklsnx*hffud /(u-mhd)
      ai(1)=ai(1) + gprop
      ai(5)=ai(5) + gprop *2.0d0/(rml+rms0)
      ai(6)=ai(6) + gprop /(rml+rms0)
c
c   RESONANCES:
c
      do i= 1,nmbr
        if(chres(i).eq.1) then
c     s-channel Nucleon and Delta resonances
          par= 1.d0*parita(i)
          mas= mres(i)
          rmas= rmres(i)
          if(ihff.eq.2.and.ispin(i).eq.1) then
c          Width of the s-channel resonances (N4,N6,N7,N9) 
c          is modified in the case of the Kaon-MAID model
            mas=wdtt(nck(i),mas,rmas,s,rmnx,rmkx,rml)
          end if                    
          if(spin(i).eq.1) then
c       spin 1/2 
            ghf1= alfa *fn2(i) *g1x(i) *hffr(i)
            call nr1(s,ghf1,mas,par,rmas,rmnx,axx,ai)
          else if (spin(i).eq.3) then
c       spin 3/2  (crossing has to be still checked!)
c                  w is the total c.m. energy 
            ghf1= fn2(i) *hffr(i) *g1x(i)/rmas
            ghf2= fn2(i) *hffr(i) *g2x(i)/rmas
            call nr3(w,s,u,ghf1,ghf2,mas,par,qg2,rmnx,rmhx,rmkx,ai)
          else if (spin(i).eq.5) then
c       spin 5/2  (crossing has to be still checked!)
c                  w is the total c.m. energy
            ghf1= fn2(i) *hffr(i) *g1x(i)/rmas**3
            ghf2= fn2(i) *hffr(i) *g2x(i)/rmas**3
            call nr5(w,s,u,ghf1,ghf2,mas,par,qg2,rmnx,rmhx,rmkx,ai)
          end if
        else if(chres(i).eq.2) then
c     t-channel spin 1 resonances
          if(parita(i).eq.-1) then
c       K* 
            mas= mres(i)
            rmas= rmres(i)           
            if(qkx.eq.0.d0) then
c           mass is changed in K0 channels and h.f.f. is calculated again
              mas= (mas/rmas - rmas + rmks0)*rmks0
              if(ihff.gt.1.and.ihff.lt.5) then 
                hffr(i)= hdrff1(hffct1,t,rmks0)
              else if(ihff.eq.5) then
                hffr(i)= hdrff2(hffct1,t,rmks0)
              end if
            end if 
            gprop= fn2(i) *hffr(i)/(t-mas)/(rmhx+rmnx)
            ghf1= g1x(i)
            ghf2= g2x(i)
            ai(1)=ai(1) + gprop *(ghf1*(rmhx+rmnx)**2 + ghf2*t)
            ai(2)=ai(2) + gprop *ghf2*2.0d0*ppl
            ai(3)=ai(3) - gprop *ghf2*2.0d0*ppn
            ai(4)=ai(4) + gprop *(ghf1*(rmhx+rmnx) - ghf2*(rmhx-rmnx))
            ai(5)=ai(5) + gprop *(ghf1*(rmhx+rmnx) + ghf2*(rmhx-rmnx))
          else if(parita(i).eq.1) then
c       K1
            gprop= fn2(i) *hffr(i)/(t-mres(i))/(rmhx+rmnx)
            ghf1= g1x(i)
            ghf2= g2x(i)
            ai(2)=ai(2) - gprop *ghf2*2.0d0*ppl
            ai(3)=ai(3) + gprop *ghf2*2.0d0*ppn
            ai(4)=ai(4) + gprop *(ghf1*(rmhx+rmnx) + ghf2*(rmhx-rmnx))
            ai(5)=ai(5) - gprop *(ghf1*(rmhx+rmnx) + ghf2*(rmhx-rmnx))
          end if
        else if(chres(i).eq.3) then
c     u-channel Lambda and Sigma spin 1/2 resonances
          par= parita(i)
          mas= mres(i)
          rmas= rmres(i)
          ghf1= alfa *fn2(i) *g1x(i) *hffr(i)
          call yr1(u,ghf1,mas,par,rmas,rmhx,axx,ai)
        end if
      end do

      return
      end
c
c.......................................................................
c
      subroutine opmat(s,t,u,mx,my,mij)

      implicit none
      real*8 s,u,t,mx,my,mij(1:4,1:4),mx2,my2

      mx2=mx**2
      my2=my**2 
      mij(1,1)= -(s-mx2)*(u-my2)
      mij(1,2)= -0.5d0*(my2*(s-mx2)**2+mx2*(u-my2)**2-(s-mx2)*(u-my2)*
     +           (t-mx2-my2))
      mij(1,3)= -0.5d0*(s-mx2)*(my*(s-mx2)-mx*(u-my2))
      mij(1,4)= -0.5d0*(u-my2)*(mx*(u-my2)-my*(s-mx2))
      mij(2,2)= -(t-(mx-my)**2)*mij(1,2)
      mij(2,3)= -mx*mij(1,2)
      mij(2,4)= -my*mij(1,2)
      mij(3,3)= -0.25d0*(s-mx2)**2*(t-(mx+my)**2)
      mij(3,4)=  0.25d0*(my*(s-mx2)-mx*(u-my2))**2
      mij(4,4)= -0.25d0*(u-my2)**2*(t-(mx+my)**2)
      mij(2,1)= mij(1,2)
      mij(3,1)= mij(1,3)
      mij(4,1)= mij(1,4)
      mij(3,2)= mij(2,3)
      mij(4,2)= mij(2,4)
      mij(4,3)= mij(3,4)

      return
      end
c
c.......................................................................
c
      function hdrff0(cutoff,x,mi)
c
c    Global hadronic form factor by Bennhold, nucl-th/9703004 
c     
      real*8 hdrff0,cutoff,x,mi
      hdrff0=(cutoff**2-mi**2)/(cutoff**2-x)
      end
c
c.......................................................................
c
      function hdrff1(cutoff,x,mi)
c
c    Hadronic form factor Janssen et al, Phys.Rev.C65(2001)015201 
c    and Kaon-MAID by Bennhold and Mart (used in the computer code)
c     
      real*8 hdrff1,cutoff,x,mi
      hdrff1=cutoff**4/(cutoff**4+(x-mi**2)**2)
      end
c
c.......................................................................
c
      function hdrff2(cutoff,x,mi)
c
c    Gaussian shape; T.Corthals et al Phys.Rev.C73(2006)045207
c     
      real*8 hdrff2,cutoff,x,mi
      hdrff2= dexp( -(x-mi**2)**2/cutoff**4 )
      end
c
c.......................................................................
c
      function wdtt(nck,mr,rmr,s,rmnx,rmkx,rml)

c  Energy dependence of widthes of N* resonances;
c  (four decay channels assumed)
c  prescription by Mart&Bennhold (nucl-th/9901066); 
c  parameters are taken from the code by T.Mart 'mart.for'

      implicit none
      real*8 rmr,s,rmnx,rmkx,rml,xi(4,4),mi(4),mb(4),sl(4),
     +       al,al3,mr2,sum,mi2,mb2,qi,qsi,e
      integer nres,ir,i
      character*2 nck
      complex*16 wdtt,mr
      data xi/0.73, 0.00, 0.21, 0.10,
     +        0.22, 0.51, 0.75, 0.75,
     +        0.00, 0.32, 0.04, 0.14,
     +        0.05, 0.17, 0.01, 0.01/,
     +     mi/0.13957, 0.276,   0.5473,  0.0/,
     +     sl/0.0, 1.0, 1.0, 2.0/

      ir= 0
      if(nck.eq.'N4') ir=1
      if(nck.eq.'N6') ir=2
      if(nck.eq.'N7') ir=3
      if(nck.eq.'N9') ir=4
      if(ir.gt.0) then
        mi(4)=rmkx
        mb(1)=rmnx
        mb(2)=rmnx
        mb(3)=rmnx
        mb(4)=rml
        al=0.41d0
        al3=3.d0*al**2
        e=2*sl(ir)+1.d0
        mr2=rmr**2
        sum=0.d0
        do i=1,4   
          mi2=mi(i)**2
          mb2=mb(i)**2
          qi=dsqrt((s-mb2+mi2)**2/4.d0/s-mi2)
          qsi=dsqrt((mr2-mb2+mi2)**2/4.d0/mr2-mi2)    
          sum=sum+xi(ir,i)*(qi/qsi)**e*dexp((qsi**2-qi**2)/al3)
        end do
        wdtt= rmr**2 + (mr-rmr**2)*dsqrt(s)*sum/rmr
      else
        wdtt= mr
      end if
      return
      end
c
c.......................................................................
c
      subroutine nr1(s,gn,mn,par,rmn,rmnx,axx,ai)
c
c   N*(1/2+) or N*(1/2-) nucleonic resonances
c
      implicit none
      real*8 s,gn,par,rmn,rmnx,axx
      complex*16 ai(6),mn
      ai(1)=ai(1)+gn/(s-mn)*((rmn+par*rmnx)/(rmn+rmnx)-
     +      axx*(rmn-mn/rmn)/2.0d0/(rmn+rmnx))
      ai(4)=ai(4)+par*2.0d0*gn/(rmn+rmnx)/(s-mn)
      ai(6)=ai(6)-par*gn/(rmn+rmnx)/(s-mn)
      return
      end
c
c.......................................................................
c
      subroutine nr3(w,s,u,g1,g2,mn,par,qqg,rmnx,rmhx,rmkx,ai)
c
c   N*(3/2+) or N*(3/2-) nucleonic resonances 
c
      implicit none
      real*8 w,s,u,g1,g2,par,qqg,rmnx,rmhx,rmkx,ww,plpg,plq,pgpp,
     +       denm,avar,bvar
      complex*16 ai(6),mn,prop,ada2,ada6

      ww=par*w
      plpg= rmhx**2 + qqg - u
      plq= s + rmhx**2 - rmkx**2
      pgpp= s - qqg - rmnx**2
      denm= ww + rmnx
      avar= plq - ww*rmhx
      bvar= plq/2.d0 + ww*rmhx
      prop= 1.d0/(s-mn)
      ai(1)=ai(1)+par*prop*(g1*(rmhx-plpg/2.d0/denm+(3.d0*ww-rmnx)*
     +            avar/6.d0/s)-g2*(ww-rmnx)*bvar/6.d0/ww/denm+
     +            qqg*(g1*avar/6.d0/s/denm+g2*bvar/6.d0/ww/denm**2))
      ada2=prop*(-g1/denm+g2*(ww-rmnx)/2.d0/denm**2+qqg*(g1*avar/
     +     3.d0/s/denm/plpg-g2*(ww*rmhx*rmnx-plq*(rmnx-1.5d0*ww))/3.d0/
     +     s/denm**2/plpg))
      ai(2)=ai(2)+par*plpg*ada2
      ada6=prop*(g1*(avar*pgpp/3.d0/s/denm/plpg+1.d0/denm)-
     +   g2*pgpp*(ww*rmhx*rmnx-plq*(rmnx-1.5d0*ww))/3.d0/s/denm**2/plpg)
      ai(3)=ai(3)-par*(pgpp*ada2-qqg*ada6)
      ai(4)=ai(4)+par*prop*(g1*(rmhx/denm-rmnx*avar/3.d0/s/denm)+
     +            g2*(plpg/2.d0/denm**2-(ww-rmnx)*bvar/3.d0/ww/denm**2)-
     +            qqg*g2*avar/3.d0/s/denm**2)
      ai(5)=ai(5)+par*prop*(g1-g2*(ww-rmnx)/2.d0/denm+
     +            qqg*g2/2.d0/denm**2)
      ai(6)=ai(6)-par*prop*(-g1*avar/3.d0/s+g2*avar*pgpp/s/denm**2/6.d0)
      return
      end
c
c.......................................................................
c
      subroutine nr5(w,s,u,g1,g2,mn,par,qqg,rmnx,rmhx,rmkx,ai)
c
c   N*(5/2+) or N*(5/2-) nucleonic resonances 
c          for photo-production only (qqg=0) !
c
      implicit none
      real*8 w,s,u,g1,g2,par,qqg,rmnx,rmhx,rmkx,ww,plpg,plq,pgpp,
     +       wpl,wmi,wpl2,wmi2,p11,p12,p13,p14,p21,p22,p23,p24
      complex*16 ai(6),mn,prop 

      ww=par*w
      plpg= rmhx**2 + qqg - u
      plq= s + rmhx**2 - rmkx**2
      pgpp= s - qqg - rmnx**2
      wpl= ww + rmnx
      wmi= ww - rmnx
      wpl2=wpl*wpl
      wmi2=wmi*wmi
      prop=1.d0/(s-mn)/10.d0
      p11=(wmi2*(wpl*plq*(rmhx*ww*(2.d0*ww-rmnx)-(4.d0*ww+rmnx)*plq/
     +    2.d0)-rmhx**2*s*wpl2)+wmi*plpg*(s*plq*(7.d0*ww+4.d0*rmnx)-
     +  2.d0*rmhx*s*ww*(2.d0*ww-rmnx))-5.d0*plpg*plpg*s*s)/2.d0/wmi/s/s
      p12=(wmi*wpl*(2.d0*plq+rmhx*ww)-5.d0*plpg*s)/s/wmi
      p13=(wpl*wmi*plq*(plq*rmnx/2.d0-rmhx*ww*(2.d0*ww-rmnx))-rmhx**2*s*
     +    wpl*wmi2-2.d0*s*plpg*plq*rmnx+rmhx*ww*s*plpg*(5.d0*ww-rmnx))/
     +    wmi/s/s
      p14=(wpl*plq*(3.d0*ww-2.d0*rmnx)-5.d0*plpg*s-rmhx*ww*wpl2)/s

      p21=(wpl2*wmi*plq*(plq-2.d0*rmhx*ww)/4.d0+wpl*plpg*(rmhx*ww*s-
     +    s*plq/2.d0))/2.d0/wmi/ww/s
      p22=(wpl2*wmi*(-2.d0*plq-rmhx*ww)+5.d0*s*wpl*plpg)/2.d0/s/wmi2
      p23=(wpl2*wmi*(2.d0*rmhx*ww-plq)*plq/4.d0+wpl*plpg*(ww*plq*(2.d0*
     +   ww-rmnx)-rmhx*s*(3.d0*ww+rmnx)/2.d0))/ww/s/wmi2-5.d0*plpg*plpg/
     +   wmi2/2.d0
      p24=(5.d0*s*wpl*plpg-wpl2*plq*(3.d0*ww-2.d0*rmnx)+
     +    rmhx*ww*wpl2*wpl)/2.d0/s/wmi

      ai(1)=ai(1)+par*prop*(g1*p11+g2*p21)
      ai(2)=ai(2)+par*prop*plpg*(g1*p12+g2*p22)
      ai(3)=ai(3)-par*prop*pgpp*(g1*p12+g2*p22)
      ai(4)=ai(4)+par*prop*(g1*p13+g2*p23)
      ai(5)=ai(5)+par*prop*(g1*p14+g2*p24)
      return
      end
c
c.......................................................................
c
      subroutine yr1(u,gy,mas,par,rmas,rmhx,axx,ai)
c
c   Y*(1/2+-) hyperonic resonances Yx: Lx or Sx
c
      implicit none
      real*8 u,gy,par,rmas,rmhx,axx
      complex*16 ai(6),mas

      ai(1)=ai(1) + gy/(u-mas)*( (rmas+par*rmhx)/(rmas+rmhx)-
     +              axx*(rmas-mas/rmas)/2.0d0/(rmas+rmhx) )
      ai(5)=ai(5) + par*gy*2.0d0/(rmas+rmhx)/(u-mas)
      ai(6)=ai(6) + par*gy/(rmas+rmhx)/(u-mas)
      return
      end
c
c.......................................................................
c
      function ffc1(kk,kp,mn,mv)
      real*8 ffc1,kk,kp,mn,mv,ge,tau
      ge=  1.0d0/(1.0d0-kk/mv**2)**2
      tau= kk/4.0d0/mn**2
      ffc1=(1.0d0-kp*tau/(1.0d0-tau))*ge
      end
c
c.......................................................................
c
      function ffc2(kk,mn,mv)
      real*8 ffc2,kk,mn,mv,ge,tau
      ge=  1.0d0/(1.0d0-kk/mv**2)**2
      tau= kk/4.0d0/mn**2
      ffc2=1.0d0/(1.0d0-tau)*ge
      end
c
c.......................................................................
c
      function ffn1(kk,mn,mv)
      real*8 ffn1,kk,mn,mv,ge,tau,b
      b=   5.6d0
      ge=  1.0d0/(1.0d0-kk/mv**2)**2
      tau= kk/4.0d0/mn**2
      ffn1=(-1.0d0+1.0d0/(1.0d0-b*tau))*tau/(1.0d0-tau)*ge
      end
c
c.......................................................................
c
      function ffn2(kk,mn,mv)
      real*8 ffn2,kk,mn,mv,ge,tau,b
      b=   5.6d0
      ge=  1.0d0/(1.0d0-kk/mv**2)**2
      tau= kk/4.0d0/mn**2
      ffn2=(1.0d0-tau/(1.0d0-b*tau))/(1.0d0-tau)*ge
      end
c
c.......................................................................
c
      complex*16 function ffm(kk,mv)
      implicit none
      real*8 kk,mv
c  kk - photon mass squared (kk < 0)
      complex*16 cr
      cr= (1.0d0,0.0d0)
      ffm=cr*mv**2/(mv**2-kk)
      end
c
c.......................................................................
c
      complex*16 function fvmdm(kk,n)
c
c  Meson form factors by Williams et al, PRC46(1992)1617.
c  VMD for  kaon       (n=0) without vector meson width
c                      (n=10) vector meson width is assumed
c  EVMD for K* and K1  (n=1) without vector meson width
c                      (n=11) vector meson width is assumed 
c
      implicit none
      real*8 kk,mr,mo,mf,mf1,gr,go,gf,gf1,lm
      integer n
      complex*16 ca,ci
      ci= (0.0d0,1.0d0)
      mr= 0.7681d0
      gr= 0.1515d0
      mo= 0.78195d0
      go= 0.00843d0
      mf= 1.0194d0
      gf= 0.00443d0
      mf1=1.680d0
      gf1=0.150d0
      lm= 0.800d0
c
c  VMD form factor for K
c
      if(n.eq.0)then
        ca=0.5d0*mr**2/(mr**2-kk)+0.17d0*mo**2/(mo**2-kk)+
     +     0.33d0*mf**2/(mf**2-kk)
      endif
      if(n.eq.10)then
        ca=0.50d0*mr**2/(mr**2-kk-ci*mr*gr)+
     +     0.17d0*mo**2/(mo**2-kk-ci*mo*go)+
     +     0.33d0*mf**2/(mf**2-kk-ci*mf*gf)
      endif
c
c  EVMD for K* and K1
c
      if(n.eq.1)then
        ca=0.50d0*mr**2/(mr**2-kk)+0.17d0*mo**2/(mo**2-kk)+
     +     0.77d0*mf**2/(mf**2-kk)+0.63d0*mf1**2/(mf1**2-kk)-
     -     1.07d0*lm**2/(lm**2-kk)
      endif
      if(n.eq.11)then
        ca=0.50d0*mr**2/(mr**2-kk-ci*mr*gr)+
     +     0.17d0*mo**2/(mo**2-kk-ci*mo*go)+
     +     0.77d0*mf**2/(mf**2-kk-ci*mf*gf)+
     +     0.63d0*mf1**2/(mf1**2-kk-ci*mf1*gf1)-
     -     1.07d0*lm**2/(lm**2-kk)
      endif
      fvmdm=ca
      return
      end
c
c.......................................................................
c
      subroutine fevmd(q2,fp1,fp2,fn1,fn2)
c
c  Baryon form factors by Gari & Kruempelman, PR D45(1992)1817.
c  Extended Vector Meson Dominance Model
c
      implicit none
      real*8 q2,fp1,fp2,fn1,fn2,al2,ald,alq,al1,sio,hmo,zko,zio,
     +     sir,hmr,zkr,zir,eto,etr,qtl2,ffl2,fflo,ffld,ff1,ff2,
     +     ffd1,ffd2,fhmo,fhmr,fis1,fis2,fiv1,fiv2
      al2= 2.1d0
      ald= 1.21d0
      alq= 0.33d0
      al1= 0.863d0
      sio= 0.658d0
      hmo= 0.784d0
      zko= 0.4d0
      zio=-0.12d0
      sir= 0.631d0
      hmr= 0.776d0
      zkr= 3.3d0
      zir= 3.706d0
      hmo=hmo*hmo
      hmr=hmr*hmr
      eto=zko/zio
      etr=zkr/zir
      al2=al2*al2
      ald=ald*ald
      alq=alq*alq
      al1=al1*al1
      qtl2=q2*dlog((al2+q2)/alq)/dlog(al2/alq)
      ffl2=al2/(al2+qtl2)
      fflo=al1/(al1+qtl2)
      ffld=ald/(ald+qtl2)
      ff1=fflo*ffl2
      ff2=ff1*fflo
      ffd1=ffld*ffl2
      ffd2=ffd1*ffld
      fhmo=sio*hmo/(hmo+q2)
      fhmr=sir*hmr/(hmr+q2)
      fis1=fhmo*ff1+(1.0d0-sio)*ffd1
      fis2=eto*fhmo*ff2+(1.0d0-sio*eto)*ffd2
      fiv1=fhmr*ff1+(1.0d0-sir)*ffd1
      fiv2=etr*fhmr*ff2+(1.0d0-sir*etr)*ffd2
      fp1=0.5d0*(fis1+fiv1)
      fp2=(zio*fis2+zir*fiv2)/(zio+zir)
      fn1=0.5d0*(fis1-fiv1)
      fn2=(zio*fis2-zir*fiv2)/(zio-zir)
      return
      end
c
c.......................................................................
c
      SUBROUTINE GSET(AX,BX,NX,Z,W)
      IMPLICIT REAL*8(A-H,O-Z)
C    N-POINT GAUSS ZEROS AND WEIGHTS FOR THE INTERVAL (AX,BX) ARE
C    STORED IN  ARRAYS Z AND W RESPECTIVELY.
      DIMENSION A(273),X(273),KTAB(96)
      DIMENSION Z(*),W(*)
C
C    TABLE OF INITIAL SUBSCRIPTS FOR N=2(1)16(4)96
      DATA KTAB(2)/1/
      DATA KTAB(3)/2/
      DATA KTAB(4)/4/
      DATA KTAB(5)/6/
      DATA KTAB(6)/9/
      DATA KTAB(7)/12/
      DATA KTAB(8)/16/
      DATA KTAB(9)/20/
      DATA KTAB(10)/25/
      DATA KTAB(11)/30/
      DATA KTAB(12)/36/
      DATA KTAB(13)/42/
      DATA KTAB(14)/49/
      DATA KTAB(15)/56/
      DATA KTAB(16)/64/
      DATA KTAB(20)/72/
      DATA KTAB(24)/82/
      DATA KTAB(28)/82/
      DATA KTAB(32)/94/
      DATA KTAB(36)/94/
      DATA KTAB(40)/110/
      DATA KTAB(44)/110/
      DATA KTAB(48)/130/
      DATA KTAB(52)/130/
      DATA KTAB(56)/130/
      DATA KTAB(60)/130/
      DATA KTAB(64)/154/
      DATA KTAB(68)/154/
      DATA KTAB(72)/154/
      DATA KTAB(76)/154/
      DATA KTAB(80)/186/
      DATA KTAB(84)/186/
      DATA KTAB(88)/186/
      DATA KTAB(92)/186/
      DATA KTAB(96)/226/
C
C-----TABLE OF ABSCISSAE (X) AND WEIGHTS (A) FOR INTERVAL (-1,+1).
C
C-----N=2
      DATA X(1)/0.577350269189626D0/, A(1)/1.000000000000000D0/
C-----N=3
      DATA X(2)/0.774596669241483D0/, A(2)/0.555555555555556D0/
      DATA X(3)/0.000000000000000D0/, A(3)/0.888888888888889D0/
C-----N=4
      DATA X(4)/0.861136311594053D0/, A(4)/0.347854845137454D0/
      DATA X(5)/0.339981043584856D0/, A(5)/0.652145154862546D0/
C-----N=5
      DATA X(6)/0.906179845938664D0/, A(6)/0.236926885056189D0/
      DATA X(7)/0.538469310105683D0/, A(7)/0.478628670499366D0/
      DATA X(8)/0.000000000000000D0/, A(8)/0.568888888888889D0/
C-----N=6
      DATA X(9)/0.932469514203152D0/, A(9)/0.171324492379170D0/
      DATA X(10)/0.661209386466265D0 /, A(10)/0.360761573048139D0 /
      DATA X(11)/0.238619186083197D0 /, A(11)/0.467913934572691D0 /
C-----N=7
      DATA X(12)/0.949107912342759D0 /, A(12)/0.129484966168870D0 /
      DATA X(13)/0.741531185599394D0 /, A(13)/0.279705391489277D0 /
      DATA X(14)/0.405845151377397D0 /, A(14)/0.381830050505119D0 /
      DATA X(15)/0.000000000000000D0 /, A(15)/0.417959183673469D0 /
C-----N=8
      DATA X(16)/0.960289856497536D0 /, A(16)/0.101228536290376D0 /
      DATA X(18)/0.525532409916329D0 /, A(18)/0.313706645877887D0 /
      DATA X(17)/0.796666477413627D0 /, A(17)/0.222381034453374D0 /
C-----N=9
      DATA X(19)/0.183434642495650D0 /, A(19)/0.362683783378362D0 /
      DATA X(20)/0.968160239507626D0 /, A(20)/0.081274388361574D0 /
      DATA X(21)/0.836031107326636D0 /, A(21)/0.180648160694857D0 /
      DATA X(22)/0.613371432700590D0 /, A(22)/0.260610696402935D0 /
      DATA X(23)/0.324253423403809D0 /, A(23)/0.312347077040003D0 /
      DATA X(24)/0.000000000000000D0 /, A(24)/0.330239355001260D0 /
C-----N=10
      DATA X(25)/0.973906528517172D0 /, A(25)/0.066671344308688D0 /
      DATA X(26)/0.865063366688985D0 /, A(26)/0.149451349150581D0 /
      DATA X(27)/0.679409568299024D0 /, A(27)/0.219086362515982D0 /
      DATA X(28)/0.433395394129247D0 /, A(28)/0.269266719309996D0 /
      DATA X(29)/0.148874338981631D0 /, A(29)/0.295524224714753D0 /
C-----N=11
      DATA X(30)/0.978228658146057D0 /, A(30)/0.055668567116174D0 /
      DATA X(31)/0.887062599768095D0 /, A(31)/0.125580369464905D0 /
      DATA X(32)/0.730152005574049D0 /, A(32)/0.186290210927734D0 /
      DATA X(33)/0.519096129206812D0 /, A(33)/0.233193764591990D0 /
      DATA X(34)/0.269543155952345D0 /, A(34)/0.262804544510247D0 /
      DATA X(35)/0.000000000000000D0 /, A(35)/0.272925086777901D0 /
C-----N=12
      DATA X(36)/0.981560634246719D0 /, A(36)/0.047175336386512D0 /
      DATA X(37)/0.904117256370475D0 /, A(37)/0.106939325995318D0 /
      DATA X(38)/0.769902674194305D0 /, A(38)/0.160078328543346D0 /
      DATA X(39)/0.587317954286617D0 /, A(39)/0.203167426723066D0 /
      DATA X(40)/0.367831498998180D0 /, A(40)/0.233492536538355D0 /
      DATA X(41)/0.125233408511469D0 /, A(41)/0.249147045813403D0 /
C-----N=13
      DATA X(42)/0.984183054718588D0 /, A(42)/0.040484004765316D0 /
      DATA X(43)/0.917598399222978D0 /, A(43)/0.092121499837728D0 /
      DATA X(44)/0.801578090733310D0 /, A(44)/0.138873510219787D0 /
      DATA X(45)/0.642349339440340D0 /, A(45)/0.178145980761946D0 /
      DATA X(46)/0.448492751036447D0 /, A(46)/0.207816047536889D0 /
      DATA X(47)/0.230458315955135D0 /, A(47)/0.226283180262897D0 /
      DATA X(48)/0.000000000000000D0 /, A(48)/0.232551553230874D0 /
C-----N=14
      DATA X(49)/0.986283808696812D0 /, A(49)/0.035119460331752D0 /
      DATA X(50)/0.928434883663574D0 /, A(50)/0.080158087159760D0 /
      DATA X(51)/0.827201315069765D0 /, A(51)/0.121518570687903D0 /
      DATA X(52)/0.687292904811685D0 /, A(52)/0.157203167158194D0 /
      DATA X(53)/0.515248636358154D0 /, A(53)/0.185538397477938D0 /
      DATA X(54)/0.319112368927890D0 /, A(54)/0.205198463721296D0 /
      DATA X(55)/0.108054948707344D0 /, A(55)/0.215263853463158D0 /
C-----N=15
      DATA X(56)/0.987992518020485D0 /, A(56)/0.030753241996117D0 /
      DATA X(57)/0.937273392400706D0 /, A(57)/0.070366047488108D0 /
      DATA X(58)/0.848206583410427D0 /, A(58)/0.107159220467172D0 /
      DATA X(59)/0.724417731360170D0 /, A(59)/0.139570677926154D0 /
      DATA X(60)/0.570972172608539D0 /, A(60)/0.166269205816994D0 /
      DATA X(61)/0.394151347077563D0 /, A(61)/0.186161000015562D0 /
      DATA X(62)/0.201194093997435D0 /, A(62)/0.198431485327111D0 /
      DATA X(63)/0.000000000000000D0 /, A(63)/0.202578241925561D0 /
C-----N=16
      DATA X(64)/0.989400934991650D0 /, A(64)/0.027152459411754D0 /
      DATA X(65)/0.944575023073233D0 /, A(65)/0.062253523938648D0 /
      DATA X(66)/0.865631202387832D0 /, A(66)/0.095158511682493D0 /
      DATA X(67)/0.755404408355003D0 /, A(67)/0.124628971255534D0 /
      DATA X(68)/0.617876244402644D0 /, A(68)/0.149595988816577D0 /
      DATA X(69)/0.458016777657227D0 /, A(69)/0.169156519395003D0 /
      DATA X(70)/0.281603550779259D0 /, A(70)/0.182603415044924D0 /
      DATA X(71)/0.095012509837637D0 /, A(71)/0.189450610455069D0 /
C-----N=20
      DATA X(72)/0.993128599185094D0 /, A(72)/0.017614007139152D0 /
      DATA X(73)/0.963971927277913D0 /, A(73)/0.040601429800386D0 /
      DATA X(74)/0.912234428251325D0 /, A(74)/0.062672048334109D0 /
      DATA X(75)/0.839116971822218D0 /, A(75)/0.083276741576704D0 /
      DATA X(76)/0.746331906460150D0 /, A(76)/0.101930119817240D0 /
      DATA X(77)/0.636053680726515D0 /, A(77)/0.118194531961518D0 /
      DATA X(78)/0.510867001950827D0 /, A(78)/0.131688638449176D0 /
      DATA X(79)/0.373706088715419D0 /, A(79)/0.142096109318382D0 /
      DATA X(80)/0.227785851141645D0 /, A(80)/0.149172986472603D0 /
      DATA X(81)/0.076526521133497D0 /, A(81)/0.152753387130725D0 /
C-----N=24
      DATA X(82)/0.995187219997021D0 /, A(82)/0.012341229799987D0 /
      DATA X(83)/0.974728555971309D0 /, A(83)/0.028531388628933D0 /
      DATA X(84)/0.938274552002732D0 /, A(84)/0.044277438817419D0 /
      DATA X(85)/0.886415527004401D0 /, A(85)/0.059298584915436D0 /
      DATA X(86)/0.820001985973902D0 /, A(86)/0.073346481411080D0 /
      DATA X(87)/0.740124191578554D0 /, A(87)/0.086190161531953D0 /
      DATA X(88)/0.648093651936975D0 /, A(88)/0.097618652104113D0 /
      DATA X(89)/0.545421471388839D0 /, A(89)/0.107444270115965D0 /
      DATA X(90)/0.433793507626045D0 /, A(90)/0.115505668053725D0 /
      DATA X(91)/0.315042679696163D0 /, A(91)/0.121670472927803D0 /
      DATA X(92)/0.191118867473616D0 /, A(92)/0.125837456346828D0 /
      DATA X(93)/0.064056892862605D0 /, A(93)/0.127938195346752D0 /
C-----N=32
      DATA X(94)/0.997263861849481D0 /, A(94)/0.007018610009470D0 /
      DATA X(95)/0.985611511545268D0 /, A(95)/0.016274394730905D0 /
      DATA X(96)/0.964762255587506D0 /, A(96)/0.025392065309262D0 /
      DATA X(97)/0.934906075937739D0 /, A(97)/0.034273862913021D0 /
      DATA X(98)/0.896321155766052D0 /, A(98)/0.042835898022226D0 /
      DATA X(99)/0.849367613732569D0 /, A(99)/0.050998059262376D0 /
      DATA X(100)/0.794483795967942D0/, A(100)/0.058684093478535D0/
      DATA X(101)/0.732182118740289D0/, A(101)/0.065822222776361D0/
      DATA X(102)/0.663044266930215D0/, A(102)/0.072345794108848D0/
      DATA X(103)/0.587715757240762D0/, A(103)/0.078193895787070D0/
      DATA X(104)/0.506899908932229D0/, A(104)/0.083311924226946D0/
      DATA X(105)/0.421351276130635D0/, A(105)/0.087652093004403D0/
      DATA X(106)/0.331868602282127D0/, A(106)/0.091173878695763D0/
      DATA X(107)/0.239287362252137D0/, A(107)/0.093844399080804D0/
      DATA X(108)/0.144471961582796D0/, A(108)/0.095638720079274D0/
      DATA X(109)/0.048307665687738D0/, A(109)/0.096540088514727D0/
C-----N=40
      DATA X(110)/0.998237709710559D0/, A(110)/0.004521277098533D0/
      DATA X(111)/0.990726238699457D0/, A(111)/0.010498284531152D0/
      DATA X(112)/0.977259949983774D0/, A(112)/0.016421058381907D0/
      DATA X(113)/0.957916819213791D0/, A(113)/0.022245849194166D0/
      DATA X(114)/0.932812808278676D0/, A(114)/0.027937006980023D0/
      DATA X(115)/0.902098806968874D0/, A(115)/0.033460195282547D0/
      DATA X(116)/0.865959503212259D0/, A(116)/0.038782167974472D0/
      DATA X(117)/0.824612230833311D0/, A(117)/0.043870908185673D0/
      DATA X(118)/0.778305651426519D0/, A(118)/0.048695807635072D0/
      DATA X(119)/0.727318255189927D0/, A(119)/0.053227846983936D0/
      DATA X(120)/0.671956684614179D0/, A(120)/0.057439769099391D0/
      DATA X(121)/0.612553889667980D0/, A(121)/0.061306242492928D0/
      DATA X(122)/0.549467125095128D0/, A(122)/0.064804013456601D0/
      DATA X(123)/0.483075801686178D0/, A(123)/0.067912045815233D0/
      DATA X(124)/0.413779204371605D0/, A(124)/0.070611647391286D0/
      DATA X(125)/0.341994090825758D0/, A(125)/0.072886582395804D0/
      DATA X(126)/0.268152185007253D0/, A(126)/0.074723169057968D0/
      DATA X(127)/0.192697580701371D0/, A(127)/0.076110361900626D0/
      DATA X(128)/0.116084070675255D0/, A(128)/0.077039818164247D0/
      DATA X(129)/0.038772417506050D0/, A(129)/0.077505947978424D0/
C-----N=48
      DATA X(130)/0.998771007252426D0/, A(130)/0.003153346052305D0/
      DATA X(131)/0.993530172266350D0/, A(131)/0.007327553901276D0/
      DATA X(132)/0.984124583722826D0/, A(132)/0.011477234579234D0/
      DATA X(133)/0.970591592546247D0/, A(133)/0.015579315722943D0/
      DATA X(134)/0.952987703160430D0/, A(134)/0.019616160457355D0/
      DATA X(135)/0.931386690706554D0/, A(135)/0.023570760839324D0/
      DATA X(136)/0.905879136715569D0/, A(136)/0.027426509708356D0/
      DATA X(137)/0.876572020274247D0/, A(137)/0.031167227832798D0/
      DATA X(138)/0.843588261624393D0/, A(138)/0.034777222564770D0/
      DATA X(139)/0.807066204029442D0/, A(139)/0.038241351065830D0/
      DATA X(140)/0.767159032515740D0/, A(140)/0.041545082943464D0/
      DATA X(141)/0.724034130923814D0/, A(141)/0.044674560856694D0/
      DATA X(142)/0.677872379632663D0/, A(142)/0.047616658492490D0/
      DATA X(143)/0.628867396776513D0/, A(143)/0.050359035553854D0/
      DATA X(144)/0.577224726083972D0/, A(144)/0.052890189485193D0/
      DATA X(145)/0.523160974722233D0/, A(145)/0.055199503699984D0/
      DATA X(146)/0.466902904750958D0/, A(146)/0.057277292100403D0/
      DATA X(147)/0.408686481990716D0/, A(147)/0.059114839698395D0/
      DATA X(148)/0.348755886292160D0/, A(148)/0.060704439165893D0/
      DATA X(149)/0.287362487355455D0/, A(149)/0.062039423159892D0/
      DATA X(150)/0.224763790394689D0/, A(150)/0.063114192286254D0/
      DATA X(151)/0.161222356068891D0/, A(151)/0.063924238584648D0/
      DATA X(152)/0.097004699209462D0/, A(152)/0.064466164435950D0/
      DATA X(153)/0.032380170962869D0/, A(153)/0.064737696812683D0/
C-----N=64
      DATA X(154)/0.999305041735772D0/, A(154)/0.001783280721696D0/
      DATA X(155)/0.996340116771955D0/, A(155)/0.004147033260562D0/
      DATA X(156)/0.991013371476744D0/, A(156)/0.006504457968978D0/
      DATA X(157)/0.983336253884625D0/, A(157)/0.008846759826363D0/
      DATA X(158)/0.973326827789910D0/, A(158)/0.011168139460131D0/
      DATA X(159)/0.961008799652053D0/, A(159)/0.013463047896718D0/
      DATA X(160)/0.946411374858402D0/, A(160)/0.015726030476024D0/
      DATA X(161)/0.929569172131939D0/, A(161)/0.017951715775697D0/
      DATA X(162)/0.910522137078502D0/, A(162)/0.020134823153530D0/
      DATA X(163)/0.889315445995114D0/, A(163)/0.022270173808383D0/
      DATA X(164)/0.865999398154092D0/, A(164)/0.024352702568710D0/
      DATA X(165)/0.840629296252580D0/, A(165)/0.026377469715054D0/
      DATA X(166)/0.813265315122797D0/, A(166)/0.028339672614259D0/
      DATA X(167)/0.783972358943341D0/, A(167)/0.030234657072402D0/
      DATA X(168)/0.752819907260531D0/, A(168)/0.032057928354851D0/
      DATA X(169)/0.719881850171610D0/, A(169)/0.033805161837141D0/
      DATA X(170)/0.685236313054233D0/, A(170)/0.035472213256882D0/
      DATA X(171)/0.648965471254657D0/, A(171)/0.037055128540240D0/
      DATA X(172)/0.611155355172393D0/, A(172)/0.038550153178615D0/
      DATA X(173)/0.571895646202634D0/, A(173)/0.039953741132720D0/
      DATA X(174)/0.531279464019894D0/, A(174)/0.041262563242623D0/
      DATA X(175)/0.489403145707052D0/, A(175)/0.042473515123653D0/
      DATA X(176)/0.446366017253464D0/, A(176)/0.043583724529323D0/
      DATA X(177)/0.402270157963991D0/, A(177)/0.044590558163756D0/
      DATA X(178)/0.357220158337668D0/, A(178)/0.045491627927418D0/
      DATA X(179)/0.311322871990210D0/, A(179)/0.046284796581314D0/
      DATA X(180)/0.264687162208767D0/, A(180)/0.046968182816210D0/
      DATA X(181)/0.217423643740007D0/, A(181)/0.047540165714830D0/
      DATA X(182)/0.169644420423992D0/, A(182)/0.047999388596458D0/
      DATA X(183)/0.121462819296120D0/, A(183)/0.048344762234802D0/
      DATA X(184)/0.072993121787799D0/, A(184)/0.048575467441503D0/
      DATA X(185)/0.024350292663424D0/, A(185)/0.048690957009139D0/
C-----N=80
      DATA X(186)/0.999553822651630D0/, A(186)/0.001144950003186D0/
      DATA X(187)/0.997649864398237D0/, A(187)/0.002663533589512D0/
      DATA X(188)/0.994227540965688D0/, A(188)/0.004180313124694D0/
      DATA X(189)/0.989291302499755D0/, A(189)/0.005690922451403D0/
      DATA X(190)/0.982848572738629D0/, A(190)/0.007192904768117D0/
      DATA X(191)/0.974909140585727D0/, A(191)/0.008683945269260D0/
      DATA X(192)/0.965485089043799D0/, A(192)/0.010161766041103D0/
      DATA X(193)/0.954590766343634D0/, A(193)/0.011624114120797D0/
      DATA X(194)/0.942242761309872D0/, A(194)/0.013068761592401D0/
      DATA X(195)/0.928459877172445D0/, A(195)/0.014493508040509D0/
      DATA X(196)/0.913263102571757D0/, A(196)/0.015896183583725D0/
      DATA X(197)/0.896675579438770D0/, A(197)/0.017274652056269D0/
      DATA X(198)/0.878722567678213D0/, A(198)/0.018626814208299D0/
      DATA X(199)/0.859431406663111D0/, A(199)/0.019950610878141D0/
      DATA X(200)/0.838831473580255D0/, A(200)/0.021244026115782D0/
      DATA X(201)/0.816954138681463D0/, A(201)/0.022505090246332D0/
      DATA X(202)/0.793832717504605D0/, A(202)/0.023731882865930D0/
      DATA X(203)/0.769502420135041D0/, A(203)/0.024922535764115D0/
      DATA X(204)/0.744000297583597D0/, A(204)/0.026075235767565D0/
      DATA X(205)/0.717365185362099D0/, A(205)/0.027188227500486D0/
      DATA X(206)/0.689637644342027D0/, A(206)/0.028259816057276D0/
      DATA X(207)/0.660859898986119D0/, A(207)/0.029288369583267D0/
      DATA X(208)/0.631075773046871D0/, A(208)/0.030272321759557D0/
      DATA X(209)/0.600330622829751D0/, A(209)/0.031210174188114D0/
      DATA X(210)/0.568671268122709D0/, A(210)/0.032100498673487D0/
      DATA X(211)/0.536145920897131D0/, A(211)/0.032941939397645D0/
      DATA X(212)/0.502804111888784D0/, A(212)/0.033733214984611D0/
      DATA X(213)/0.468696615170544D0/, A(213)/0.034473120451753D0/
      DATA X(214)/0.433875370831756D0/, A(214)/0.035160529044747D0/
      DATA X(215)/0.398393405881969D0/, A(215)/0.035794393953416D0/
      DATA X(216)/0.362304753499487D0/, A(216)/0.036373749905835D0/
      DATA X(217)/0.325664370747701D0/, A(217)/0.036897714638276D0/
      DATA X(218)/0.288528054884511D0/, A(218)/0.037365490238730D0/
      DATA X(219)/0.250952358392272D0/, A(219)/0.037776364362001D0/
      DATA X(220)/0.212994502857666D0/, A(220)/0.038129711314477D0/
      DATA X(221)/0.174712291832646D0/, A(221)/0.038424993006959D0/
      DATA X(222)/0.136164022809143D0/, A(222)/0.038661759774076D0/
      DATA X(223)/0.097408398441584D0/, A(223)/0.038839651059051D0/
      DATA X(224)/0.058504437152420D0/, A(224)/0.038958395962769D0/
      DATA X(225)/0.019511383256793D0/, A(225)/0.039017813656306D0/
C-----N=96
      DATA X(226)/0.999689503883230D0/, A(226)/0.000796792065552D0/
      DATA X(227)/0.998364375863181D0/, A(227)/0.001853960788946D0/
      DATA X(228)/0.995981842987209D0/, A(228)/0.002910731817934D0/
      DATA X(229)/0.992543900323762D0/, A(229)/0.003964554338444D0/
      DATA X(230)/0.988054126329623D0/, A(230)/0.005014202742927D0/
      DATA X(231)/0.982517263563014D0/, A(231)/0.006058545504235D0/
      DATA X(232)/0.975939174585136D0/, A(232)/0.007096470791153D0/
      DATA X(233)/0.968326828463264D0/, A(233)/0.008126876925698D0/
      DATA X(234)/0.959688291448742D0/, A(234)/0.009148671230783D0/
      DATA X(235)/0.950032717784437D0/, A(235)/0.010160770535008D0/
      DATA X(236)/0.939370339752755D0/, A(236)/0.011162102099838D0/
      DATA X(237)/0.927712456722308D0/, A(237)/0.012151604671088D0/
      DATA X(238)/0.915071423120898D0/, A(238)/0.013128229566961D0/
      DATA X(239)/0.901460635315852D0/, A(239)/0.014090941772314D0/
      DATA X(240)/0.886894517402420D0/, A(240)/0.015038721026994D0/
      DATA X(241)/0.871388505909296D0/, A(241)/0.015970562902562D0/
      DATA X(242)/0.854959033434601D0/, A(242)/0.016885479864245D0/
      DATA X(243)/0.837623511228187D0/, A(243)/0.017782502316045D0/
      DATA X(244)/0.819400310737931D0/, A(244)/0.018660679627411D0/
      DATA X(245)/0.800308744139140D0/, A(245)/0.019519081140145D0/
      DATA X(246)/0.780369043867433D0/, A(246)/0.020356797154333D0/
      DATA X(247)/0.759602341176647D0/, A(247)/0.021172939892191D0/
      DATA X(248)/0.738030643744400D0/, A(248)/0.021966644438744D0/
      DATA X(249)/0.715676812348967D0/, A(249)/0.022737069658329D0/
      DATA X(250)/0.692564536642171D0/, A(250)/0.023483399085926D0/
      DATA X(251)/0.668718310043916D0/, A(251)/0.024204841792364D0/
      DATA X(252)/0.644163403784967D0/, A(252)/0.024900633222483D0/
      DATA X(253)/0.618925840125468D0/, A(253)/0.025570036005349D0/
      DATA X(254)/0.593032364777572D0/, A(254)/0.026212340735672D0/
      DATA X(255)/0.566510418561397D0/, A(255)/0.026826866725591D0/
      DATA X(256)/0.539388108324357D0/, A(256)/0.027412962726029D0/
      DATA X(257)/0.511694177154667D0/, A(257)/0.027970007616848D0/
      DATA X(258)/0.483457973920596D0/, A(258)/0.028497411065085D0/
      DATA X(259)/0.454709422167743D0/, A(259)/0.028994614150555D0/
      DATA X(260)/0.425478988407300D0/, A(260)/0.029461089958167D0/
      DATA X(261)/0.395797649828908D0/, A(261)/0.029896344136328D0/
      DATA X(262)/0.365696861472313D0/, A(262)/0.030299915420827D0/
      DATA X(263)/0.335208522892625D0/, A(263)/0.030671376123669D0/
      DATA X(264)/0.304364944354496D0/, A(264)/0.031010332586313D0/
      DATA X(265)/0.273198812591049D0/, A(265)/0.031316425596861D0/
      DATA X(266)/0.241743156163840D0/, A(266)/0.031589330770727D0/
      DATA X(267)/0.210031310460567D0/, A(267)/0.031828758894411D0/
      DATA X(268)/0.178096882367618D0/, A(268)/0.032034456231992D0/
      DATA X(269)/0.145973714654896D0/, A(269)/0.032206204794030D0/
      DATA X(270)/0.113695850110665D0/, A(270)/0.032343822568575D0/
      DATA X(271)/0.081297495464425D0/, A(271)/0.032447163714064D0/
      DATA X(272)/0.048812985136049D0/, A(272)/0.032516118713868D0/
      DATA X(273)/0.016276744849602D0/, A(273)/0.032550614492363D0/
C
C-----TEST N
      N=NX
      ALPHA=0.5D0*(BX+AX)
      BETA=0.5D0*(BX-AX)
      IF(N.LT.1) GO TO 100
      IF(N.NE.1) GO TO 1
      Z(1)=ALPHA
      W(1)=BX-AX
      RETURN
C
    1 IF(N.LE.16) GO TO 2
      IF(N.EQ.20) GO TO 2
      IF(N.EQ.24) GO TO 2
      IF(N.EQ.32) GO TO 2
      IF(N.EQ.40) GO TO 2
      IF(N.EQ.48) GO TO 2
      IF(N.EQ.64) GO TO 2
      IF(N.EQ.80) GO TO 2
      IF(N.EQ.96) GO TO 2
      GO TO 100
C
C----- SET K EQUAL TO INITIAL SUBSCRIPT AND STORE RESULTS
    2 K=KTAB(N)
      M=N/2
C
      DO 3 J=1,M
      JTAB=K-1+J
      WTEMP=BETA*A(JTAB)
      DELTA=BETA*X(JTAB)
      Z(J)=ALPHA-DELTA
      W(J)=WTEMP
      JP=N+1-J
      Z(JP)=ALPHA+DELTA
      W(JP)=WTEMP
    3 CONTINUE
C
      IF((N-M-M).EQ.0) RETURN
      Z(M+1)=ALPHA
      JMID=K+M
      W(M+1)=BETA*A(JMID)
      RETURN
C
  100 ZN=N
      PRINT 200,ZN
      RETURN
C
  200 FORMAT(  41H GSET ... N HAS THE NON-PERMISSIBLE VALUE,D11.3 )
      END
c
c=======================================================================
c                end of the file ppkhypsub.for
c=======================================================================
