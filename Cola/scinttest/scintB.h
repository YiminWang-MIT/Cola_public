//
//	SZINTILLATOR B
//

online.B.dE.paddle = -1;
online.B.ToF.paddle = -1;
online.B.ToF.Energy.left = online.B.ToF.Energy.right = 0;
online.B.dE.Energy.left = online.B.dE.Energy.right = 0;
scBpattern=0;
if (online.B.trigger) 
{ sumB_dE = 0;
  cnt = i = 0;
  
  scBpattern = spekB.det.trigger.scint.de.pattern;
  
  while (scBpattern != 0) 
  { if (scBpattern & 1) 
    { online.B.dE.paddle = i+1;
      if (ct->itemOK(&spekB.det.trigger.scint.de.pad[i].energy)) {
	sumB_dE += spekB.det.trigger.scint.de.pad[i].energy;
      }
      cnt++;
    }
    scBpattern >>= 1;
    i++;
  };
  if (BlaserOK) sumB_dE /= cnt;
  
  cnt = i = 0;
  scBpattern = spekB.det.trigger.scint.tof.pattern;
  
  sumB_ToF = 0;
  while (scBpattern != 0) 
  { if (scBpattern & 1) 
    { online.B.ToF.paddle = i+1;
      if ((ct->itemOK(&spekB.det.trigger.scint.tof.pad[i].left.energy)) &&
	  (ct->itemOK(&spekB.det.trigger.scint.tof.pad[i].right.energy))) 
      { sumB_ToF += spekB.det.trigger.scint.tof.pad[i].left.energy;
	sumB_ToF += spekB.det.trigger.scint.tof.pad[i].right.energy;
	online.B.ToF.Energy.left += 
	  spekB.det.trigger.scint.tof.pad[i].left.energy;
	online.B.ToF.Energy.right += 
	  spekB.det.trigger.scint.tof.pad[i].right.energy;
      }
      cnt++;
    }
    scBpattern >>= 1;
    i++;
  };
  sumB_ToF /= cnt;
  
  scBpattern = spekB.det.trigger.scint.tof.pattern;
  online.B.dE.Energy.left = online.B.dE.Energy.right = sumB_dE;
  if ( ct->itemOK(&spekB.det.trigger.scint.de_tof_time)
       && online.B.dE.paddle >=0  && online.B.ToF.paddle >=0 )     
  { online.B.dE_ToF_raw = spekB.det.trigger.scint.de_tof_time - 
                         rundb.B.scint.dE[online.B.dE.paddle] +
                         rundb.B.scint.ToF[online.B.ToF.paddle];
    online.B.dE_ToF = online.B.dE_ToF_raw +
              ( rundb.B.scint.walk[0]
              + rundb.B.scint.walk[1]/sqrt(online.B.ToF.Energy.right)
              - rundb.B.scint.walk[2]/sqrt(online.B.dE.Energy.right )
              )/rundb.tdc_resolution;
   }
}





