// Harald Merkel                                         -*-C++-*-
//               A1 Collaboration
//               Institut für Kernphysik
//               Universität Mainz

#include "Cola/Material.h"                // Definition of Materials
#include "Cola/QuasiRandom.h"             // Random Generator
#include "Cola/Masses.h"                  // Definition of Particles

/** @name Radiation Corrections.

  These subroutines provides a set of radiation corrections based on \\
  L.W. Mo and Y.S. Tsai, Rev. Mod. Phys 41 (1969) 205\\
  These approximations are known to be not very accurate and should
  not be used were you have to rely on the radiative corrections. For
  simple reactions like \TEX{$H(e,e'p)\pi^o$} the corrections are good
  enough to the \TEX{5\%} level (my personal feeling).  
*/

//@{
/** Spence Function.

 * This Function is defined by\\
 * \TEX{\begin{eqnarray*}
 *      \Phi(x) & = & \int_0^x\frac{-\ln |1-y|}{y}dy.
 *      \end{eqnarray*}}\\
 * Approximated here by\\
 * \TEX{\begin{eqnarray*}
 *   \Phi(x)  & \approx & \sum_{n=1}^{\infty} \frac{x^n}{n^2}
 *                        \mbox{, if  } |x| \le 1\\
 *   \Phi(1)  & = & \ \pi^2/6  \\
 *   \Phi(-1) & = & -\pi^2/12 \\
 *   \Phi(x)  & = & -\frac{1}{2}\ln^2|x| + \frac{1}{3}\pi^2-\Phi(1/x)
 *                \quad \mbox{if} \quad x \ge 1 \\ 
 *   \Phi(x)  & = & -\frac{1}{2}\ln^2|x| - \frac{1}{6}\pi^2-\Phi(1/x) 
 *              \quad \mbox{if} \quad x \le -1  
 *  \end{eqnarray*}}\\
 * Fast evaluation method due to t'Hooft and Veltman,
 * Scalar one-loop integrals, Nuc. Phys. B153 (1979) 365:\\
 * \TEX{\begin{eqnarray*}
 *  \Phi(x)  & \approx & \sum_{n=1}^{\infty} B_{n-1} \frac{z^n}{n!} \\
 *           &         & (z=-\ln(1-x), \quad B_i \ \mbox{Bernouilly numbers})
 *  \end{eqnarray*}}
 */
double Spence(const double x);

/** Generates energy loss by external Bremsstrahlung.
  
 * Y.S. Tsai, Rev. Mod. Phys 46 (1974) p.815\\
 * The distribution is given by\\
 * \TEX{\begin{eqnarray*}
 * I_e(E,E_1,t) & \sim & u^{bt-1}(1-u+\frac{3}{4}u^2) \\
 * u            & = & \frac{E-E_1}{E} \\
 * b            & = & \frac{4}{3}\left(1+\frac{1}{9}
 *                    \left[ \frac{Z+1}{Z+\xi} \right]
 *                    \ln^{-1}(183Z^{-\frac{1}{3}}) \right)\\
 * \xi        & = & \frac{\ln(1440Z^{-\frac{2}{3}})}
 *                         {\ln(183Z^{-\frac{1}{3}})}
 * \end{eqnarray*}}
 */

void externalBremsstrahlung(Particle& p, const Material *m,
			    double x, Uniform& h);

/** Generates energy loss by internal Bremsstrahlung for 
 *  a specified momentum transfer q2
  
 * Schwinger part (electron contributions) + "kinematical
 * recoil" correction (Mo and Tsai). The "dynamical" effect, i.e.
 * radiation from the hadronic system(s) is left out here for generality.
 * Let's hope we calculate some day exact Bremsstrahlung distributions
 * (also angular!) from all accelerated charges.
 */
void internalBremsstrahlung(Particle& p, const Material *m,
			    Uniform& h, double q2);

/** Generates a energy loss distribution by Ionization. 

 * The distribution is a Landau distribution for electrons 
 * (see QuasiRandom for generating this) of the parameter\\
 * \TEX{\begin{eqnarray*}
 *   \phi(\lambda) & = &\frac{1}\pi\int_0^\infty 
 *                       {e^{-\lambda r-r\ln r}sin(\pi r)}dr\\
 *  \lambda & =& \frac{\Delta E - \Delta E_{prob}}{\xi} \\
 *  \mbox{with most probable value} &&\\
 *   \Delta E_{prob} & = & \frac{At}{\beta^2}\left(B+0.891+
 *                         2 \ln\frac{P}{m_ec}+\ln\frac{At}{\beta^2}
 *                         -\beta^2-\delta\right)\\
 *   A               & = & 0.154 \frac{z_c^2 Z}{A_0} \\
 *   B               & = & \ln\left(\frac{m_ec^210^6}{I^2}\right)\\
 *   \xi           & = & 0.154 \frac{z_c^2 Zt}{\beta^2A_0}
 * \end{eqnarray*}
 * }
 @param  p      Particle to apply a generated  correction
 @param  m      Material of the radiator, usually just the target material
 @param  x      Pathlength in cm
 @param  r1     Uniformly distributed random generator
 @param  r2     Uniformly distributed random generator
 */

void LandauLoss(Particle& p, const Material* m,
		double x, Uniform& r1, Uniform& r2);

/** Returns the vertex correction and vaccum Polarization (exponentiated).
 *
 * L.W. Mo and Y.S. Tsai, Rev. Mod. Phys 41 (1969) 205\\
 * \TEX{\begin{eqnarray*}
 *  \mbox{Used correction: } && e^{\delta'}\\
 *  \mbox{with}\qquad 
 *  1 + \delta' & = & 1 + \frac{2\alpha}{\pi}\left[-\frac{14}{9}
 *                    + \frac{13}{12}\ln\frac{Q^2}{m_e^2}\right]\\
 *            &   & - \frac{\alpha}{2\pi}\ln\frac{E}{E'}\\
 *            &   & + \frac{\alpha}{\pi}\left[\frac 1 6 \pi^2 
 *                  - \Phi(cos^2\frac\theta 2)\right]
 *  \end{eqnarray*}}\\
 * \TEX{$\Phi(x)$} is the\Ref{Spence Function} .
 @param  q2    Photon virtuality \TEX{$q^2$} in \TEX{$GeV^2/c^2$}
 @param  E     Beam energy in GeV
 @param  Ep    Scattered energy in GeV
 @param  theta Scattering angle in rad
 *
 @return Dimensionless factor to multiply with cross section. For our energies
 *       and \TEX{$q^2$} a \TEX{$\approx 6\%$} increase in cross section.
 */   

double vertexCorrection (double q2, double E, double Ep, double theta);

/** Calculates the thickness of an equivalent radiator.
 *  This is not longer used by the standard Cola!
 *
 * L.W. Mo and Y.S. Tsai, Rev. Mod. Phys 41 (1969) 205\\
 * \TEX{\begin{eqnarray*}
 *  t_{eq} & = & \frac{\alpha}{b\pi}\left[\ln\frac{Q^2}{m_e^2}-1\right] \\
 *  b      & = & \frac{4}{3}\left(1+\frac{1}{9}\left[\frac{Z+1}{Z+\xi}\right]
 *               \ln^{-1}(183Z^{-\frac{1}{3}})\right)
 * \end{eqnarray*}}
 *
 @param  q2 Photon virtuality \TEX{$q^2$} in \TEX{$GeV^2/c^2$}.
 @return Length of an equivalent Radiator in cm.
 */

double equivalentRadiator (const Material* m, double q2);
//@}
