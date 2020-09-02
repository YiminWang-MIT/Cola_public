
// the guy to blame:
//
// Jan C. Bernauer
// A1 Collaboration
// J.G. University Mainz
// mailto: bernauer@kph.uni-mainz.de


#ifndef __baseTM_h__
#define __baseTM_h__

///Define the structure containing the results.
struct TargetCo  { 
  float dp, th, y0, ph, len; 
};


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

class baseTM {
private:
public:
  /// Contructor of the class. Reads the matrix file and prepares evaluation
  baseTM(const char *filename);
  /// Destructor
  virtual ~baseTM();
  /// Method for evaluation
   virtual struct TargetCo * run(double x, double th, double y, double ph); 
  /** Set offsets. Overwrite initialization by matrix.
   *  The units are cm and mrad.
   */
  virtual int setOffsets(double x_offs, double th_offs, double y_offs, double ph_offs);
  /// Variable containing the results of evaluation
  TargetCo result;
};


#endif
