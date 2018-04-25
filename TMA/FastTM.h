//                                                             -*-c++-*-
//
// Small Additions by Jan C. Bernauer to enable Spline-System
//
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mit.edu
// MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// former version   Jan Friedrich 5/97 
//                  A1 Collaboration
// copyright        Institut fuer Kernphysik Mainz
//
// $Header: /tmp/cvsroot/Cola/TMA/FastTM.h,v 1.8 2004-01-20 14:33:11 bernauer Exp $
//

#ifndef __FastTM_h__
#define __FastTM_h__

#include "baseTM.h"

///Forward declaration for class handling matrix entries.
class ftmEntry;

/** Basic TMA class.

  This class handles the information and methods for the matrix
  backtracing formalism of the magnetic spectrometers. The target
  coordinates \TEX{$t_i$} (\TEX{$t_{(1-4)}$} are momentum \TEX{$\Delta
  p$}, dispersive angle \TEX{$\theta_0$}, vertex coordinate \TEX{$y_0$}),
  non-dispersive angle \TEX{$\phi_0$}, and the path length len are
  reconstructed by the measured detector coordinates \TEX{$d_j$} via \\
  \TEX{
   \begin{eqnarray*}
     t_i & = & \sum_{ijkl} a_{ijkl} x^i\theta^jy^k\phi^l 
   \end{eqnarray*}
  }\\
  The coefficents \TEX{$a_{ijkl}$} are given in a file in a list of 
  the form \\
  totalNumberOfLines\\
  i j k l a(dp) a(theta) a(y0) a(phi) a(len)\\
  ...\\
  x-offset:  \TEX{$x_0$}\\
  th-offset: \TEX{$\theta_0$} ...\\
  y-offset:  \TEX{$y_0$} ...\\
  ph-offset: \TEX{$\phi_0$} ...\\
  */

class TMAC :public baseTM{
private:
  /// List containing all the matrix elements
  ftmEntry *elements;
  /** Vectors containing specific non-zero matrix elements.
   *  Index (0-4) stands for dp, th0, y0, ph0, len respectively.
   */
  ftmEntry **node[5];

  /// Offsets on the detector coordinates (x-dependent)
  double *offs[4];
  /// The maximum x-power of the detector coordinate offsets.
  int power[4];
public:
  /// Contructor of the class. Reads the matrix file and prepares evaluation
  TMAC(const char *filename);
  /// Destructor
  ~TMAC();
  /// Method for evaluation
    struct TargetCo * run(double x, double th, double y, double ph);
  /** Set offsets. Overwrite initialization by matrix.
   *  The units are cm and mrad.
   */
    int setOffsets(double x_offs, double th_offs, double y_offs, double ph_offs);
};


#endif
