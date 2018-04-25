
#include <math.h>
#include <stdio.h>
#include <iostream>
#include<stdlib.h>

// Routines to interpolate Arenhoevel D(e,e'p)n structure functions

const int
  MAXANG = 50,           // maximal number of angle's
  MAXENP = 50,           // maximal number of enp values
  MAXQCM = 50;           // maximal numer of qcm values

const double
  alpha   = 1.0/137.0,
  DEG2RAD = M_PI / 180.0,
  hbarc   = 197.33;


// --- muss besser werden: Gehoert in extra file -------

void sigaren(int& initflag, double k_i, double omega, double cos_th_e,
	     double q2, double qnu2, double jacobian,
	     double th_pq_cm_1, double ph_pq, double t_cm,
	     double q2_cm, int asym_flag, char nuc_type,
	     double& sig_mott,
	     double& v_l, double& v_t, double& v_lt, double& v_tt,
	     double& w_l, double& w_t, double& w_lt, double& w_tt);

void strfun_int(int& init, double thcm_v, double enp_v, double qcm_v,
		double& fl, double& ft, double& flt,
		double& ftt, double& fltp);

void read_data(char* filename);

void spline(double x[MAXANG+1], double y[MAXANG+1], int n,
	    double yp1, double ypn, double y2[MAXANG+1]);

void splint(double xa[MAXANG+1], double ya[MAXANG+1], double y2a[MAXANG+1],
	    int n, double x, double& y);

void splie2(double x1a[MAXENP+1], int mm, double x2a[MAXQCM+1], int mn,
	    double ya[MAXENP+1][MAXQCM+1], int m, int n,
	    double y2a[MAXENP+1][MAXQCM+1]);

void splin2(double x1a[MAXENP+1], int mm, double x2a[MAXQCM+1], int mn,
	    double ya[MAXENP+1][MAXQCM+1], double y2a[MAXENP+1][MAXQCM+1],
	    int m, int n, double x1, double x2, double& y);


// ============================================================================

// calculate everything to put arenhoevel cross section together
// parameters to pass:
//   k_i             :   initial electron energy, in MeV
//   omega           :   energy transfer, in MeV
//   cos_th_e        :   cosine of elektron angle
//   qlab2, qnu2     :   3-mom transfer lab squ'd/MeV^2 & Invariant Q^2/ MeV^2
//   th_pq_cm, ph_pq :   angles in rad
//   t_cm, q2_cm     :   kin. energy cm (MeV), mom. trf. cm (MeV^2)
//   jacobian        :   jacobian
// keep in mind that these parameters aren't all independent, if they
// belong to an D(e,e'p)n event!

void sigaren(int& initflag, double k_i, double omega, double cos_th_e,
	     double q2, double qnu2, double jacobian,
	     double th_pq_cm, double ph_pq, double t_cm,
	     double q2_cm, int asym_flag, char nuc_type,
	     double& sig_mott,
	     double& v_l, double& v_t, double& v_lt, double& v_tt,
	     double& w_l, double& w_t, double& w_lt, double& w_tt)
{
#ifdef NEVEREVER
  std::cout << "SigAr: k_i = " << k_i
	    << ", omega = " << omega
	    << ", q = " << sqrt(q2)
	    << ", qnu = " << sqrt(qnu2)
	    << ", jac = " << jacobian
	    << ", t_cm = "<< t_cm
	    << ", q_cm = " << sqrt(q2_cm)
	    << "\n";
#endif

  double k_f  = k_i - omega;
  double q_cm = sqrt(q2_cm);

  // get interpolated structure functions a la Arenhoevel (in fm)
  double s00, s11, s01, s1_1, s01p;
  strfun_int(initflag, th_pq_cm, t_cm, q2_cm/(hbarc*hbarc),
	     s00, s11, s01, s1_1, s01p);

#ifdef NEVEREVER
  std::cout << "SigAr: s00 = " << s00
	    << ", s11 = "   << s11
	    << ", s01 = "   << s01
	    << ", s1_1 = "  << s1_1
	    << ", s01p = "  << s01p
	    << "\n";
#endif

    
  // calculate various kinematic factors
  double xi      = qnu2 / q2;
  double eta     = (1.0 - cos_th_e) / (1.0 + cos_th_e);     // = tan^2 (th_e/2)
  double beta_ah = sqrt(q2) / q_cm;

  // kinematic factors a la Arenhoevel (outcommented, since not yet needed)
  // double rho00  =  qnu2 * xi*xi/(2*eta) * beta_ah*beta_ah;
  // double rho11  =  0.5 * qnu2 * (1 + xi/(2*eta));
  // double rho01  =  qnu2 * xi/eta * sqrt((xi + eta)/8) * beta_ah;
  // double rho1_1 = -qnu2 * xi/(4*eta);
  // C a la Arenhoevel [1/MeV^2]
  // double c = alpha / ( 6 * M_PI*M_PI) / qnu2*qnu2 * k_f/k_i;

  // kinematic factors a la Donelly&Walecka
  v_l  =  xi*xi;
  v_t  =  qnu2 / (2.0*q2) + eta;
  v_lt =  xi * sqrt((xi + eta) / 2);
  v_tt = -xi/2;

  // Mott cross section a la Donelly&Walecka  [1/MeV^2]
  sig_mott = 2.0 * (1.0 + cos_th_e) * (alpha*k_f/qnu2)*(alpha*k_f/qnu2);

  // response functions in lab frame a la Donelly&Walecka, in fm
  w_l  = s00  * jacobian * beta_ah*beta_ah / (12 * alpha * M_PI*M_PI);
  w_t  = s11  * jacobian                   / (12 * alpha * M_PI*M_PI);
  w_lt = s01  * jacobian * beta_ah         / (12 * alpha * M_PI*M_PI);
  w_tt = s1_1 * jacobian                   / (12 * alpha * M_PI*M_PI);
}


//----------------------------------------------------------------------

static double
  th[MAXANG+1][MAXENP+1][MAXQCM+1],
  f00[MAXANG+1][MAXENP+1][MAXQCM+1],
  f11[MAXANG+1][MAXENP+1][MAXQCM+1],
  f01[MAXANG+1][MAXENP+1][MAXQCM+1],
  f_11[MAXANG+1][MAXENP+1][MAXQCM+1],
  f01p[MAXANG+1][MAXENP+1][MAXQCM+1];


static double
  enp[MAXENP+1],
  qcm[MAXQCM+1],
  d_enp[MAXENP+1],
  d_qcm[MAXQCM+1];

static int numdat[MAXENP+1][MAXQCM+1];

static int n_ang, n_enp, n_qcm;


// calculates the D(e,e'p)n response functions for a given enp, qcm^2
// and angle thcm interpolated using a cubic spline in 3 dimensions
//
// input: init  if true => read new file
//
//        thcm_v : angle between proton momentum and momentum transfer in
//                 pn-CM-System (= gD-CM-System). Has to be given in rad
//        enp_v  : Total kinetic energy in CM-System. In MeV
//        qcm_v  : (3-momentum transfer)^2 in the CM-system, in fm^-2
//
// output:  f... : response functions
//
// data file is read is init = true, after reading it init is set to false
//
// requires SPLINE, SPLINT, SPLIE2, SPLIN2
//
//----------------------------------------------------------------------

void strfun_int(int& init, double thcm_v, double enp_v, double qcm_v,
		double& fl, double& ft, double& flt,
		double& ftt, double& fltp)
{
  // calculated second derivatives with resp. to angles
  static double
    fl2[MAXANG+1][MAXENP+1][MAXQCM+1],
    ft2[MAXANG+1][MAXENP+1][MAXQCM+1],
    flt2[MAXANG+1][MAXENP+1][MAXQCM+1],
    ftt2[MAXANG+1][MAXENP+1][MAXQCM+1],
    fltp2[MAXANG+1][MAXENP+1][MAXQCM+1];

    // arrays of angle interpolated values
  static double
    da_fl[MAXENP+1][MAXQCM+1],
    da_flt[MAXENP+1][MAXQCM+1],
    da_ft[MAXENP+1][MAXQCM+1],
    da_ftt[MAXENP+1][MAXQCM+1],
    da_fltp[MAXENP+1][MAXQCM+1];

    // arrays of second derivatives of angle interpolated values
  static double
    da_fl2[MAXENP+1][MAXQCM+1],
    da_flt2[MAXENP+1][MAXQCM+1],
    da_ft2[MAXENP+1][MAXQCM+1],
    da_ftt2[MAXENP+1][MAXQCM+1],
    da_fltp2[MAXENP+1][MAXQCM+1];

  // local variables
  static int m = 0;

  double
    l_fl[MAXANG+1], l_fl2[MAXANG+1],
    l_ft[MAXANG+1], l_ft2[MAXANG+1],
    l_flt[MAXANG+1], l_flt2[MAXANG+1],
    l_ftt[MAXANG+1], l_ftt2[MAXANG+1],
    l_fltp[MAXANG+1], l_fltp2[MAXANG+1];

  static double theta[MAXANG+1];

  // ----------------------------------------------------------------------
  //  INITIALIZE
  // ----------------------------------------------------------------------

  if( init ) {
    read_data("response.dat");   // read data, dont print debug information
    init = 0;

    // calculate derivatives for the angles and store them in arrays. 
    // In future calls these derivatives dont have to be calculated anymore

    // ----------------------------------------------------------------------
    //  calculate the 1st derivative for the 1st and last point with respective
    //  to the angles

    //  use a simple linear estimate
    // ----------------------------------------------------------------------
    //  SPLINE
    // ----------------------------------------------------------------------

    for(int i=1; i<=n_ang; i++)
      theta[i] = th[i][1][1];

    for(int ie=1; ie<=n_enp; ie++) {
      for(int iq=1; iq<=n_qcm; iq++) {
	for(int i=1; i<=n_ang; i++) {
	  l_fl[i]   = f00[i][ie][iq];
	  l_ft[i]   = f11[i][ie][iq];
	  l_flt[i]  = f01[i][ie][iq];
	  l_ftt[i]  = f_11[i][ie][iq];
	  l_fltp[i] = f01p[i][ie][iq];
	}

	m = n_ang;
	double dth = theta[2] - theta[1];

	// initialize fl
	double dfl1 = (l_fl[2] - l_fl[1]) / dth;
	double dfl2 = (l_fl[m] - l_fl[m-1]) / dth;
	spline(theta, l_fl, m, dfl1, dfl2, l_fl2);

	// initialize ft
	double dft1 = (l_ft[2] - l_ft[1]) / dth;
	double dft2 = (l_ft[m] - l_ft[m-1]) / dth;
	spline(theta, l_ft, m, dft1, dft2, l_ft2);

	// initialize flt
	double dflt1 = (l_flt[2] - l_flt[1]) / dth;
	double dflt2 = (l_flt[m] - l_flt[m-1]) / dth;
	spline(theta, l_flt, m, dflt1, dflt2, l_flt2);

	// initialize ftt
	double dftt1 = (l_ftt[2] - l_ftt[1]) / dth;
	double dftt2 = (l_ftt[m] - l_ftt[m-1]) / dth;
	spline(theta, l_ftt, m, dftt1, dftt2, l_ftt2);

	// initialize fltp
	double dfltp1 = (l_fltp[2] - l_fltp[1]) / dth;
	double dfltp2 = (l_fltp[m] - l_fltp[m-1]) / dth;
	spline(theta, l_fltp, m, dfltp1, dfltp2, l_fltp2);

	for(int i=1; i<=n_ang; i++) {
	  fl2[i][ie][iq]   = l_fl2[i];
	  ft2[i][ie][iq]   = l_ft2[i];
	  flt2[i][ie][iq]  = l_flt2[i];
	  ftt2[i][ie][iq]  = l_ftt2[i];
	  fltp2[i][ie][iq] = l_fltp2[i];
	}
      }
    }
  }


  //
  // Here starts the part when initialisation isn't necessary anymore

#ifdef NEVEREVER
  for(int i=1; i<=n_ang; i++)
    for(int j=1; j<=n_enp; j++)
      for(int k=1; k<=n_qcm; k++)
	std::cout << i << " " << j << " " << k << " "
		  << th[i][j][k] << " "
		  << f00[i][j][k] << " "
		  << f11[i][j][k] << " "
		  << f01[i][j][k] << " "
		  << f_11[i][j][k] << " "
		  << f01p[i][j][k] << "\n";
#endif


  // ----------------------------------------------------------------------
  //  for each enp and qcm value calculate the angle interpolated values
  //    interpolate and use that the arrays are stored column 
  //    use spline interpolation:
  //
  // ----------------------------------------------------------------------
  //    SPLINT 
  // ----------------------------------------------------------------------

  for(int ie=1; ie<=n_enp; ie++) {
    for(int iq=1; iq<=n_qcm; iq++) {
      for(int i=1; i<=n_ang; i++) {
	l_fl[i]    = f00[i][ie][iq];
	l_fl2[i]   = fl2[i][ie][iq];
	l_ft[i]    = f11[i][ie][iq];  
	l_ft2[i]   = ft2[i][ie][iq];  
	l_flt[i]   = f01[i][ie][iq];  
	l_flt2[i]  = flt2[i][ie][iq];  
	l_ftt[i]   = f_11[i][ie][iq]; 
	l_ftt2[i]  = ftt2[i][ie][iq];  
	l_fltp[i]  = f01p[i][ie][iq]; 
	l_fltp2[i] = fltp2[i][ie][iq];  
      }

      splint(theta, l_fl,   l_fl2,   m, thcm_v, da_fl[ie][iq]);
      splint(theta, l_flt,  l_flt2,  m, thcm_v, da_flt[ie][iq]);
      splint(theta, l_ft,   l_ft2,   m, thcm_v, da_ft[ie][iq]);
      splint(theta, l_ftt,  l_ftt2,  m, thcm_v, da_ftt[ie][iq]);
      splint(theta, l_fltp, l_fltp2, m, thcm_v, da_fltp[ie][iq]);
    }
  }


#ifdef NEVEREVER
  for(int j=1; j<=n_enp; j++)
    for(int k=1; k<=n_qcm; k++)
	std::cout << j << " " << k << " "
		  << da_fl[j][k] << " "
		  << da_flt[j][k] << " "
		  << da_ft[j][k] << " "
		  << da_ftt[j][k] << " "
		  << da_fltp[j][k] << "\n";
#endif


  // the angle interpolated values are now stored in the 2-d arrays:
  // d_fl, d_flt etc. for all combinations of qcm and enp
  //
  // ----------------------------------------------------------------------
  //  2-d spline interpolation SPLIE2 on the above arrays
  // ----------------------------------------------------------------------

  splie2(d_enp, MAXENP, d_qcm, MAXQCM, da_fl, n_enp, n_qcm, da_fl2);
  splie2(d_enp, MAXENP, d_qcm, MAXQCM, da_ft, n_enp, n_qcm, da_ft2);
  splie2(d_enp, MAXENP, d_qcm, MAXQCM, da_flt, n_enp, n_qcm, da_flt2);
  splie2(d_enp, MAXENP, d_qcm, MAXQCM, da_ftt, n_enp, n_qcm, da_ftt2);
  splie2(d_enp, MAXENP, d_qcm, MAXQCM, da_fltp, n_enp, n_qcm, da_fltp2);


#ifdef NEVEREVER
  for(int j=1; j<=n_enp; j++)
    for(int k=1; k<=n_qcm; k++)
	std::cout << j << " " << k << " "
		  << da_fl2[j][k] << " "
		  << da_flt2[j][k] << " "
		  << da_ft2[j][k] << " "
		  << da_ftt2[j][k] << " "
		  << da_fltp2[j][k] << "\n";
#endif


  // ----------------------------------------------------------------------
  //  2-d spline interpolation SPLIN2 on the above arrays
  // ----------------------------------------------------------------------

  splin2(d_enp, MAXENP, d_qcm, MAXQCM, 
	 da_fl, da_fl2, n_enp, n_qcm, 
	 enp_v, qcm_v, fl);

  splin2(d_enp, MAXENP, d_qcm, MAXQCM, 
	 da_ft, da_ft2, n_enp, n_qcm, 
	 enp_v, qcm_v, ft);

  splin2(d_enp, MAXENP, d_qcm, MAXQCM, 
	 da_flt, da_flt2, n_enp, n_qcm, 
	 enp_v, qcm_v, flt);

  splin2(d_enp, MAXENP, d_qcm, MAXQCM, 
	 da_ftt, da_ftt2, n_enp, n_qcm, 
	 enp_v, qcm_v, ftt);

  splin2(d_enp, MAXENP, d_qcm, MAXQCM, 
	 da_fltp, da_fltp2, n_enp, n_qcm, 
	 enp_v, qcm_v, fltp);
}


// ===========================================================================

// read structure function file and store values in common
//
// these data files are usually created by the programs: short_edit,
// short_edit1 from files in the directory h2dat/deep/

void read_data(char* filename)
{
  double enp_data, qcm_data;
  int    numdat_l, jset_l;
      
  char pot[80];
  FILE* ah_data = fopen(filename, "r");

  if( ah_data == NULL ) {
    fprintf(stderr, "Cannot open file %s, exiting ...\n", filename);
    exit(-1);
  }

  double
    enp_old = 0.0,
    qcm_old = 0.0;

  int
    i_enp = 0,
    i_qcm = 0;

  while( 1 ) {
    if( fscanf(ah_data, "%d", &numdat_l) == EOF ) // number of data per set
      break;

    if (fscanf(ah_data, "%lf %lf", &enp_data, &qcm_data));

    // update indices: this works only for sorted input sequence!
    //                 => use shell scripts to sort input files

    if( enp_data != enp_old ) {
      i_enp        = i_enp + 1;
      enp_old      = enp_data;
      enp[i_enp]   = enp_data;
      d_enp[i_enp] = enp_data;
    }

    if( qcm_data != qcm_old ) {
      i_qcm        = i_qcm + 1;
      i_enp        = 1;                    // reset enp counter
      qcm_old      = qcm_data;
      qcm[i_qcm]   = qcm_data;
      d_qcm[i_qcm] = qcm_data;
    }

    // ----------------------------------------------------------------------

    numdat[i_enp][i_qcm] = numdat_l;

    // potential name and number, jset is ignored 
    if (fscanf(ah_data, "%d %s %*s %*s", &jset_l, pot));  // scheisse

    // ----------------------------------------------------------------------

    for(int i_ang = 1; i_ang <= numdat_l; i_ang++) {
      if (fscanf(ah_data, "%lf %lf %lf %lf %lf %lf",
		 &(th[i_ang][i_enp][i_qcm]),
		 &(f00[i_ang][i_enp][i_qcm]),
		 &(f11[i_ang][i_enp][i_qcm]),
		 &(f01[i_ang][i_enp][i_qcm]),
		 &(f_11[i_ang][i_enp][i_qcm]),
		 &(f01p[i_ang][i_enp][i_qcm]) ));

      // Arenhoevels angles are given in degree, convert to radian
      th[i_ang][i_enp][i_qcm] *= DEG2RAD;
    }
  }

  fclose(ah_data);
      
  n_ang = numdat_l;
  n_qcm = i_qcm;
  n_enp = i_enp;
}


// ------------------------------------------------------------------------
//  necessary interpolation routines. They all have in common that the
// last argument gets modified.
// ------------------------------------------------------------------------

// Read only: x, y, n, yp1, ypn. Result: y2

const int NMAX = 100;

void spline(double x[MAXANG+1], double y[MAXANG+1], int n,
	    double yp1, double ypn, double y2[MAXANG+1])
{
  double u[NMAX+1];
    
  if( yp1 > .99e30 ) {
    y2[1] = 0.0;
    u[1]  = 0.0;
  }
  else {
    y2[1] = -0.5;
    u[1]  = (3.0 / (x[2] - x[1])) * ((y[2] - y[1]) / (x[2] - x[1]) - yp1);
  }

  for(int i=2; i<=(n-1); i++) {
    double sig = (x[i] - x[i-1]) / (x[i+1] - x[i-1]);
    double p   = sig*y2[i-1] + 2.0;
    y2[i] = (sig - 1.0) / p;
    u[i]  = (6. * ((y[i+1] - y[i]) / (x[i+1] - x[i]) - (y[i] - y[i-1])
		   / (x[i] - x[i-1])) / (x[i+1] - x[i-1]) - sig*u[i-1]) / p;
  }

  double qn, un;

  if( ypn > .99e30 ) {
    qn = 0.;
    un = 0.;
  }
  else {
    qn = 0.5;
    un = (3. / (x[n] - x[n-1])) * (ypn - (y[n] - y[n-1]) / (x[n] - x[n-1]));
  }

  y2[n] = (un - qn*u[n-1]) / (qn*y2[n-1] + 1.0);

  for(int k=(n-1); k>=1; k--)
    y2[k] = y2[k]*y2[k+1] + u[k];
}


// ------------------------------------------------------------------------

// Read only: xa, ya, y2a, n, x. Result: y

void splint(double xa[MAXANG+1], double ya[MAXANG+1], double y2a[MAXANG+1],
	    int n, double x, double& y)
{
  int klo = 1, khi = n;

  while( (khi - klo) > 1 ) {
    int k = (khi + klo) / 2;

    if( xa[k] > x )
      khi = k;
    else
      klo = k;
  }

  double h = xa[khi] - xa[klo];

  if( h == 0.0 ) {
    fprintf(stderr, "bad xa input\n");
    exit(-1);
  }

  double a = (xa[khi] - x) / h;
  double b = (x - xa[klo]) / h;

  y = a*ya[klo] + b*ya[khi] +
      ((a*a*a - a)*(y2a[klo]) + (b*b*b - b)*(y2a[khi])) * (h*h)/6.0 ;
}


// ------------------------------------------------------------------------

// Dummy: x1a, mm, mn. Read only: x2a, ya, m, n. Result: y2a

const int NN = 100;

void splie2(double x1a[MAXENP+1], int mm, double x2a[MAXQCM+1], int mn,
	    double ya[MAXENP+1][MAXQCM+1], int m, int n,
	    double y2a[MAXENP+1][MAXQCM+1])
{
  double ytmp[NN+1], y2tmp[NN+1];

  for(int j=1; j<=m; j++) {
    for(int k=1; k<=n; k++)
      ytmp[k] = ya[j][k];

    spline(x2a, ytmp, n, 1.0e30, 1.0e30, y2tmp);

    for(int k=1; k<=n; k++)	
      y2a[j][k] = y2tmp[k];
  }
}


// ------------------------------------------------------------------------

// Dummy: mm, mn. Read only: x1a, x2a, ya, y2a, m, n, x1, x2. Result: y

void splin2(double x1a[MAXENP+1], int mm, double x2a[MAXQCM+1], int mn,
	    double ya[MAXENP+1][MAXQCM+1], double y2a[MAXENP+1][MAXQCM+1],
	    int m, int n, double x1, double x2, double& y)
{      
  double ytmp[NN+1], y2tmp[NN+1], yytmp[NN+1];

  for(int j=1; j<=m; j++) {
    for(int k=1; k<=n; k++) {
      ytmp[k]  = ya[j][k];
      y2tmp[k] = y2a[j][k];
    }

    splint(x2a, ytmp, y2tmp, n, x2, yytmp[j]);
  }

  spline(x1a, yytmp, m, 1.0e30, 1.0e30, y2tmp);
  splint(x1a, yytmp, y2tmp, m, x1, y);
}
