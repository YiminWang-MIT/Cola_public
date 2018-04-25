//                                                                    -*-c++-*-
// 


#ifndef __spinTransfer__
#define __spinTransfer__  

#include "FourVector/FourVector.h"

/** @name Spin Transfer for the Focal Plane Polarimeter 

    @memo .
*/

//@{ 
/** Spin transfer matrix from target to the Carbon analyzer of the
 *  focal plane polarimeter in A
 *
 *  Targetspin fest im Spektrometer-Target-Koordinatensystem (x nach unten,
 *  z durch die Mitte des Quads, auch fuer Teilchen mit th/ph != 0!)
 *  Spin im Kohlenstoff im Teilchensystem (z in Teilchenflugrichtung, 
 *  x bei direkter Drehung ins HDC-System HDC-x-Richtung).
 *
 *  Dieses Modul beinhaltet eine Abbildungsmatrix, die durch Anfitten eines
 *  24242-Polynoms (dp, th, y0, ph, p_ref; 675 Parameter) an jede Kombination
 *  von den 3 Eingangs-Spin-Komponenten mit den 3 Ausgangs-Spin-Komponenten
 *  entstanden ist. 
 *  Als Stuetzpunkte dienten dabei der auf 1715 Tracks 
 *  (Binierung der Targetkoordinaten: 
 *      dp: [-5.7794, 0.0327, 5.6206, 11.0182, 16.2521] % 
 *      th: [-75, -50, -25, 0, 25, 50, 75] mrad 
 *      y0: [-30, -20, -10, 0, 10, 20, 30] mm 
 *      ph: [-105, -70, -35, 0, 35, 70, 105] mrad ) 
 *  mit dem Programm QSPIN fuer 6 verschiedene Referenzimpulse
 *  (p_ref: [480, 510, 540, 570, 600, 630] MeV/c) von Spektrometer A
 *  (das Gamma der Protonen aendert sich!) berechnete Spintransport.
 *  Die 3 x 10290 Stuetzpunkte finden sich getrennt nach Eingangs-Spin-
 *  Komponenten in den Dateien TI10290.spn.x/y/z. 
 *  
 *  Genauigkeit: Die Abweichung der Komponenten des Ausgangsspins liegt 
 *               im Vergleich zur QSPIN-Rechnung im Mittel bei etwa 0.25% 
 *               des Spin-Betrages.
 */

class SpinMatrix {
protected:
  struct {
    double Sx2x[361], Sx2y[316], Sx2z[361];
    double Sy2x[316], Sy2y[361], Sy2z[316];
    double Sz2x[361], Sz2y[316], Sz2z[361];
  } SP;
  
public:
  static const SpinMatrix matrix;
  /** Constructor */ SpinMatrix();
  
  /** calculate focal plane spin from target spin
      
      @param  target input spin at quadrupole
      @param  dp     Impulsablage zu p_ref in %
      @param  theta  theta at Target (positiv down) in mrad
      @param  y0     y0 at Target in mm
      @param  phi    ph at Target in mrad
      @param  pref   Reference momentum of spectrometer in MeV/c
      @return        spin in focal plane         
  */
  
  Momentum SpinTrans(const Momentum target,
		     const double dp, const double theta,
		     const double y0, const double phi,
		     const double pref) const;
};
//@}
#endif
