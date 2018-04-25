//
//	SZINTILLATOR C
//

online.C.dE.paddle = -1;  
online.C.ToF.paddle = -1;
online.C.ToF.Energy.left = online.C.ToF.Energy.right = 0;
online.C.dE.Energy.left = online.C.dE.Energy.right = 0;

scCpattern = 0;

if (online.C.trigger) 
{ sumC_ToF = sumC_dE = 0;
  cnt = i = j = 0;
  number = 1;

  for (i=0; i<15; i++) 
  { if ((ct->itemOK(&spekC.det.trigger.scint.tof.pad[i].left.energy)) &&
	(ct->itemOK(&spekC.det.trigger.scint.tof.pad[i].right.energy))) 
    { sumC_ToF += spekC.det.trigger.scint.tof.pad[i].left.energy;
      sumC_ToF += spekC.det.trigger.scint.tof.pad[i].right.energy;
      online.C.ToF.Energy.left += 
	spekC.det.trigger.scint.tof.pad[i].left.energy;
      online.C.ToF.Energy.right += 
	spekC.det.trigger.scint.tof.pad[i].right.energy;
      online.C.ToF.paddle = i+1;
      cnt++; j = i;
      scCpattern |= number;
    }
    number <<= 1;
  }
  if (ClaserOK && cnt) 
  { sumC_ToF /= cnt;
    online.C.ToF.Energy.left /= cnt;
    online.C.ToF.Energy.right /= cnt;
  }
  sumC_dE = cnt = j = 0;
  for (i=0; i<15; i++) 
  { if ((ct->itemOK(&spekC.det.trigger.scint.de.pad[i].left.energy)) &&
	(ct->itemOK(&spekC.det.trigger.scint.de.pad[i].right.energy))) 
    { sumC_dE += spekC.det.trigger.scint.de.pad[i].left.energy;
      sumC_dE += spekC.det.trigger.scint.de.pad[i].right.energy;
      online.C.dE.Energy.left+=
	spekC.det.trigger.scint.de.pad[i].left.energy;
      online.C.dE.Energy.right+=
	spekC.det.trigger.scint.de.pad[i].right.energy;
      online.C.dE.paddle = i+1;
      cnt++; j = i;
    }
  }
  if (ClaserOK && cnt) 
  { sumC_dE /= cnt;
    online.C.dE.Energy.left/=cnt;
    online.C.dE.Energy.right/=cnt;
  }
}






