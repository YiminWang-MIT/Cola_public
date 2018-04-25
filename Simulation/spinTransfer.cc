//                                                                    -*-c++-*-
// 
//  Spintransport der Protonen vom Target zum Kohlenstoffanalysator in Spek A
//
//  Targetspin fest im Spektrometer-Target-Koordinatensystem (x nach unten,
//  z durch die Mitte des Quads, auch fuer Teilchen mit th/ph != 0!)
//  Spin im Kohlenstoff im Teilchensystem (z in Teilchenflugrichtung, 
//  x bei direkter Drehung ins HDC-System HDC-x-Richtung).
//
//  Dieses Modul beinhaltet eine Abbildungsmatrix, die durch Anfitten eines
//  24242-Polynoms (dp, th, y0, ph, p_ref; 675 Parameter) an jede Kombination
//  von den 3 Eingangs-Spin-Komponenten mit den 3 Ausgangs-Spin-Komponenten
//  entstanden ist. 
//  Als Stuetzpunkte dienten dabei der auf 1715 Tracks 
//  (Binierung der Targetkoordinaten: 
//      dp: [-5.7794, 0.0327, 5.6206, 11.0182, 16.2521] %
//      th: [-75, -50, -25, 0, 25, 50, 75] mrad 
//      y0: [-30, -20, -10, 0, 10, 20, 30] mm 
//      ph: [-105, -70, -35, 0, 35, 70, 105] mrad ) 
//  mit dem Programm QSPIN fuer 6 verschiedene Referenzimpulse
//  (p_ref: [480, 510, 540, 570, 600, 630] MeV/c) von Spektrometer A
//  (das Gamma der Protonen aendert sich!) berechnete Spintransport.
//  Die 3 x 10290 Stuetzpunkte finden sich getrennt nach Eingangs-Spin-
//  Komponenten in den Dateien TI10290.spn.x/y/z. 
//  
//  Genauigkeit: Die Abweichung der Komponenten des Ausgangsspins liegt 
//               im Vergleich zur QSPIN-Rechnung im Mittel bei etwa 0.25% 
//               des Spin-Betrages.
//  --------------------------------------------------------------------------
//  Benutzung: Durch einmaligen Aufruf der Routine InitSpinMatrix muss
//             das Parameterfeld am Anfang einaml initialisiert werden.
//             Danach bekommt man von der Funktion SpinTrans die Drehung 
//             eines beliebigen Spins fuer ein beliebiges 
//             (dp, th, y0, ph, pref) (Gueltigkeitsbereich siehe oben 
//             angegebene Stuezpunkte) vom Target zum Kohlenstoff berechnet.

#include <string.h>
#include "spinTransfer.h"

SpinMatrix::SpinMatrix()
{
 #include "spinInit.h"
}

Momentum
SpinMatrix::SpinTrans(const Momentum TG,
		      const double dp, const double th, 
		      const double y0, const double ph,
		      const double pr) const
{
  double P315[316];
  double P360[361];
  int i;

  double dp2, th2, y02, ph2, pr2;
  double th3, ph3;
  double th4, ph4;

  double Sxx, Sxy, Sxz;
  double Syx, Syy, Syz;
  double Szx, Szy, Szz;

  dp2 = dp * dp;
  th2 = th * th; th3 = th * th2; th4 = th * th3;
  y02 = y0 * y0;
  ph2 = ph * ph; 
  ph3 = ph * ph2; 
  ph4 = ph * ph3;
  pr2 = pr * pr;

# include "spinPolynom1.h"
# include "spinPolynom2.h"

  Sxx = 0.0; if (TG[0]) for(i=1; i<=360; i++) Sxx += SP.Sx2x[i] * P360[i];
  Sxy = 0.0; if (TG[0]) for(i=1; i<=315; i++) Sxy += SP.Sx2y[i] * P315[i];
  Sxz = 0.0; if (TG[0]) for(i=1; i<=360; i++) Sxz += SP.Sx2z[i] * P360[i];

  Syx = 0.0; if (TG[1]) for(i=1; i<=315; i++) Syx += SP.Sy2x[i] * P315[i];
  Syy = 0.0; if (TG[1]) for(i=1; i<=360; i++) Syy += SP.Sy2y[i] * P360[i];
  Syz = 0.0; if (TG[1]) for(i=1; i<=315; i++) Syz += SP.Sy2z[i] * P315[i];

  Szx = 0.0; if (TG[2]) for(i=1; i<=360; i++) Szx += SP.Sz2x[i] * P360[i];
  Szy = 0.0; if (TG[2]) for(i=1; i<=315; i++) Szy += SP.Sz2y[i] * P315[i];
  Szz = 0.0; if (TG[2]) for(i=1; i<=360; i++) Szz += SP.Sz2z[i] * P360[i];
 
  return Momentum(Sxx * TG[0] + Syx * TG[1] + Szx * TG[2],
		  Sxy * TG[0] + Syy * TG[1] + Szy * TG[2],
		  Sxz * TG[0] + Syz * TG[1] + Szz * TG[2]);
}

const SpinMatrix SpinMatrix::matrix;
