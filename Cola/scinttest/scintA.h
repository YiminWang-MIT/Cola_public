//
//	SZINTILLATOR A
//

#define A_TOF spekA.det.trigger.scint.tof
#define A_DE  spekA.det.trigger.scint.de

scApattern = 0;

if (online.A.trigger) 
{
  online.A.dE.Energy.left  = online.A.dE.Energy.right = 
  online.A.ToF.Energy.left = online.A.ToF.Energy.right = 0;
  cnt = 0;
  number = 1;
  for (i=0; i<15; i++) 
  { if (ct->itemOK(&A_TOF.pad[i].left.energy)
     && ct->itemOK(&A_TOF.pad[i].right.energy))
    { online.A.ToF.Energy.left  += A_TOF.pad[i].left.energy;
      online.A.ToF.Energy.right += A_TOF.pad[i].right.energy;
      online.A.ToF.paddle       =  i+1;
      cnt++;
      if (!rundb.A.scint.use_dE) scApattern |= number;
    }
    number <<= 1;
  }
  if (cnt)
  { online.A.ToF.Energy.left  /= cnt;
    online.A.ToF.Energy.right /= cnt;
  }

  cnt = 0;
  number = 1;
  for (i=0; i<15; i++) 
  { if (ct->itemOK(&A_DE.pad[i].left.energy) &&
	ct->itemOK(&A_DE.pad[i].right.energy)) 
    { online.A.dE.Energy.left  += A_DE.pad[i].left.energy;
      online.A.dE.Energy.right += A_DE.pad[i].right.energy;
      online.A.dE.paddle        = i+1;
      cnt++;
      if (rundb.A.scint.use_dE)  scApattern |= number;
    }
    number <<= 1;
  }

  if (cnt)
  { online.A.dE.Energy.left  /= cnt;
    online.A.dE.Energy.right /= cnt; 
  }

  if ( ct->itemOK(&spekA.det.trigger.scint.de_tof_time)
       && online.A.dE.paddle >=0 && online.A.ToF.paddle >=0 )
  { online.A.dE_ToF_raw = spekA.det.trigger.scint.de_tof_time - 
                         rundb.A.scint.dE[online.A.dE.paddle] +
                         rundb.A.scint.ToF[online.A.ToF.paddle];
    online.A.dE_ToF = online.A.dE_ToF_raw +
              ( rundb.A.scint.walk[0]
              - rundb.A.scint.walk[1]/sqrt(online.A.ToF.Energy.right)
              + rundb.A.scint.walk[2]/sqrt(online.A.dE.Energy.right )
              )/rundb.tdc_resolution;
  }
}
















