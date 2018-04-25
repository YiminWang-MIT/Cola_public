#include <math.h>
#include <float.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <getopt.h>
 
#include "Simulation/Generate.h"
#include <FourVector/FourVector.h>
#include <HMBook/hmbook.h>
#include "Model/ChPT_VCS.h"
#include "Masses.h"

// Ntuple stuff, just for linkage => ignore
#include "Cola/ColaNtuple.h"

#define Ebeam 0.855;

struct {  int hmemor[HBOOKMEM];} pawc_;
char *ntvarn[MAX_NT_VAR];   // Scalar name related to ntvar
char *ntdes[MAX_NT_VAR];    // NTvariable name
char *ntbck[MAX_NT_VAR];    // Block name for each ntvar
char *nttitle;              // Ntuple Title
int  ntiden, ntnvar;
int  iselastic=false;
SobolSequence sobol;
int  verbose =1;
//////////


using namespace std;

extern "C" void   invert(double *in, double *out, int n);

extern "C" double simplex(double (* func)(double[]), double x[], int dim,
			  double scale[], double toleranz);


double ElasticCrossSection(const FourVector &e_in, 
			   const FourVector &e_out,
			   const FourVector &q_out,
			   const double helicity,
			   const unsigned short a,
			   class Momentum *P_Spin,
			   class Momentum *P_Spin_CM);
 
class DATA { 
public: 
  double euler[3], phi, A, var;
  double electron[3], photon[3];
  double cs;

  FourVector eIN;
  FourVector eOUT;
  FourVector qOUT;    //Real Photon
  FourVector qphoton; //Virtual Photon

  double BHpol[3];    //BH+B Polarizations
  double Mn;          //Normalization
  double M_square;    
};

inline double s(double phi, double A) { return 1 - A * sin(phi); }

//
// Rotation of the vector in by the three euler angles to the vector out.
//
void EulerRotate(const double in[3], double out[3], const double euler[3])
{
  double cx = cos(euler[0]), sx = sin(euler[0]);
  double cy = cos(euler[1]), sy = sin(euler[1]);
  double cz = cos(euler[2]), sz = sin(euler[2]);
  out[0]= in[0]*( cy*cz)          + in[1]*(-cy*sz         ) + in[2]*(-sy   );
  out[1]= in[0]*(-sx*sy*cz+cx*sz) + in[1]*( sx*sy*sz+cx*cz) + in[2]*(-sx*cy);
  out[2]= in[0]*( cx*sy*cz+sx*sz) + in[1]*(-cx*sy*sz+sx*cz) + in[2]*( cx*cy);
}

class maxLikelihood {
public:
  static DATA *data;
  static int N;
  
  maxLikelihood(DATA *d, int n) {
    data = d;
    N = n;
  }
   
  //Connection between Polarizations and Polarizabilities
  static void polChPT(double P[3], double pol[3] , DATA d)
  {

    //Needed by M0,Mx,My,Mz
    FourVector p_in    = FourVector(m_proton, 0, 0, 0);  
    FourVector CM      = p_in + d.eIN - d.eOUT;
    FourVector e_inCM  = d.eIN.Lorentz(-CM).rotate(CM);
    FourVector e_outCM = d.eOUT.Lorentz(-CM).rotate(CM);
    
    FourVector q_outCM = d.qOUT.Lorentz(-CM).rotate(CM);
    FourVector p_inCM  = p_in.Lorentz(-CM).rotate(CM);    
    FourVector q_p     = e_inCM - e_outCM;    
    
    double hel = d.A / fabs(d.A);                          //Helicity
    double eps   = epsilon(e_inCM,e_outCM);                //Polarization
    double thgg  = atan2(-q_outCM[1], q_outCM[3]);         //Theta gammagamma
    double phigg = asin( q_outCM[2] / q_outCM.momentum() );//Phi gammagamma
    
    ChPT_VCS vcs;

    double PP[6] = {1, 1, P[0], P[1], P[2], 1};

    ChPT_VCS::setScale(PP);
    PP[0] = vcs.P01010ScaleBoundaryCondition(FourVector(0,0,0,0.6), 23.7, 0.62);
    PP[1] = vcs.P11110ScaleBoundaryCondition(FourVector(0,0,0,0.6), -5.0);
    ChPT_VCS::setScale(PP);
    
    double M0 = vcs.M0(q_outCM,p_inCM,e_inCM,e_outCM,q_p,thgg,phigg,eps);
    double Mx = vcs.Mx(q_outCM,p_inCM,e_inCM,e_outCM,q_p,thgg,phigg,eps,hel);
    double My = vcs.My(q_outCM,p_inCM,e_inCM,e_outCM,q_p,thgg,phigg,eps,hel);
    double Mz = vcs.Mz(q_outCM,p_inCM,e_inCM,e_outCM,q_p,thgg,phigg,eps,hel);
    
    //VCS Polarizations (CM or LAB ?????)
    pol[0] = ( (d.BHpol[0]/d.Mn)*d.M_square + Mx ) / (d.M_square + 2*M0);
    pol[1] = ( (d.BHpol[1]/d.Mn)*d.M_square + My ) / (d.M_square + 2*M0);
    pol[2] = ( (d.BHpol[2]/d.Mn)*d.M_square + Mz ) / (d.M_square + 2*M0);

    //pol[0] = d.BHpol[0]*hel;
    //pol[1] = d.BHpol[1]*hel;
    //pol[2] = d.BHpol[2]*hel;

    //cout << "From CalcPol: " << pol[0]*hel << " " << pol[1]*hel << " " << pol[2]*hel << " " <<endl;
    //<< "eps=" << eps << " q=" << q_p.momentum() << " "
    //<< "Vphoton=" << q_outCM.momentum() << endl;

  }
  
  static double logLikelihood(double P[3]) {
    double res = 0;
    double angle,n;
    for (int i=0; i<N; i++) {
      double tr[3];
      double in[3];
      
      polChPT(P,in,data[i]);

      EulerRotate(in, tr, data[i].euler);

      angle = atan2(tr[1], tr[0]);
      n     = sqrt(tr[0] * tr[0] + tr[1] * tr[1]);

      //Constraint-----------------------------------------------------------------
      static ChPT_VCS vcs;

      FourVector qcm(0,0,0,0.6);//q=600MeV

      //Constraints at fixed momentum
      double s1 = vcs.P_LL(qcm) - vcs.P_TT(qcm)/0.62;
      double s2 = vcs.P_LT(qcm);

      //double s1 = vcs.P_LL(data[i].qphoton) - vcs.P_TT(data[i].qphoton)/0.64;
      //double s2 = vcs.P_LT(data[i].qphoton);

      //cout << s1 << "  " << s2 << " at q=" << qcm.momentum() << endl;
      
      
      if (false && i==0){
	cout << "P(01,01)0=" << vcs.P01010(data[i].qphoton)*1e3
	     << "  P(11,11)0=" << vcs.P11110(data[i].qphoton)*1e3
	     << "  P(11,11)1=" << vcs.P11111(data[i].qphoton)*1e3
	     << "  P(01,01)1=" << vcs.P01011(data[i].qphoton)*1e3
	     << "  P(01,12)1=" << vcs.P01121(data[i].qphoton)*1e3 
	     << "  P(11,02)1=" << vcs.P11021(data[i].qphoton)*1e3 << endl;//"\r" << flush;

	FourVector x(0,0,0,0.6);//q=600MeV
	cout << "---> " << vcs.P_LL(x) - vcs.P_TT(x)/0.62 << "  " 
	     << vcs.P_LT(x) << " at q=0.6" 
	     << " and C1 = " << s1-23.7 << " C2 = " << s2+5 << endl;

      } 
      
      //---------------------------------------------------------------------------

      //The likelihood
      if (s(data[i].phi - angle, data[i].A * n) > 0){
	
	res += log(s(data[i].phi - angle, data[i].A * n));
	//	      + (- 0.5*pow((s1-23.7)/2.2,2) - 0.5*pow((s2+5)/0.8,2))/N; 
      }
      else { cerr << "Event "<<i<<": L zero or negative" << endl;}
    }
    return res;
  }
   
 
   
  //
  // second derivative of the log Likelihood sum, needed for error calculation 
  //
  static double d2logLikelihood(double in[3], int x, int y)
  {
    double hx = pow(DBL_EPSILON,1.0/3);// see numerical recipies 5.7
    double hy = pow(DBL_EPSILON,1.0/3);
    double i[3] = {in[0], in[1], in[2]};
    double tx = i[x]+hx;
    double ty = i[y]+hy;
    double fxy = logLikelihood(i);
    hx = tx-in[x]; // to ensure exact machine accuracy for i[x]+hx
    hy = ty-in[y];

    if (x!=y) {
      i[x] += hx;
      double fxp = logLikelihood(i);
      i[x] = in[x];
      i[y] += hy;
      double fyp = logLikelihood(i);
      return ((fxp - fxy) - (fyp - fxy)) / hx / hy;
    } else {
      i[x] = in[x]+hx;
      double fxp = logLikelihood(i);
      i[x] = in[x]-hx;
      double fxm = logLikelihood(i);
      return (fxp - 2*fxy + fxm) / hx / hx;
    }
  }
  
  // "minfunc" is the function to be minimized. It has to be static to 
  // be minimized by the simplex subroutine (and so logLikelihood has
  // to be static, too).
  
  static double minfunc(double in[3]) {
    if (fabs(in[0])>5.5 || fabs(in[1])>5.5 || fabs(in[2])>5.5) return N;

    static int cnt = 0;   
    double res =  - logLikelihood(in);
    cout << "\r" 
	 <<setw(3) << ++cnt << " " << setprecision(3) <<showpoint<<fixed
	 << setw(7) <<  in[0] << " "
	 << setw(7) <<  in[1] << " "
	 << setw(7) <<  in[2] << " " 
    	 << setprecision(4) << setw(9)  << " " << res << endl;//"\r" << flush;
    return res;
  }
  
  void calcmin(double result[3]) {
    double scale[3] = {0.01, 0.01, 0.01};
    simplex(minfunc, result, 3, scale, 0.00001);
  }
  
  void calcerr(double result[3], double error[3]) {
    double m[3][3], n[3][3];
    for (int i=0;i<3;i++) 
      for (int j=0;j<3;j++)
  	m[i][j] = - d2logLikelihood(result, i, j);

    cout << endl;
    cout << "Hessian Matrix:" << endl;
    for (int i=0;i<3;i++){
      for (int j=0;j<3;j++)
  	cout<<setprecision(4)<<"  ("<<i<<","<<j<<")= " << m[i][j];
      cout << endl;
    }

    invert((double *) m,(double *) n, 3);
    
    /*cout << endl << setprecision(4)
	 <<setw(8)<< n[0][0] <<setw(8)<< n[0][1] <<setw(8)<< n[0][2] << endl
	 <<setw(8)<< n[1][0] <<setw(8)<< n[1][1] <<setw(8)<< n[1][1] << endl
	 <<setw(8)<< n[2][2] <<setw(8)<< n[2][1] <<setw(8)<< n[2][2] << endl
	 <<setw(8)<< n[3][0] <<setw(8)<< n[3][1] <<setw(8)<< n[3][2] << endl
	 <<setw(8)<< n[4][0] <<setw(8)<< n[4][1] <<setw(8)<< n[4][2] << endl
	 <<setw(8)<< n[5][0] <<setw(8)<< n[5][1] <<setw(8)<< n[5][2] << endl;*/
    
    cout << endl;
    cout << "Covariances:" << endl;
    for (int i=0;i<3;i++){
      cout << endl;
      for (int j=0;j<3;j++)
  	cout<<setw(13)<< setprecision(10)<<", " << n[i][j];
    }
    cout << endl;

    error[0] = sqrt(n[0][0]);
    error[1] = sqrt(n[1][1]);
    error[2] = sqrt(n[2][2]);
  }
};

DATA * maxLikelihood::data;
int    maxLikelihood::N;

int 
main(int argc, char *argv[]) 
{
  char   *infn  = "polarization.dat", *outfn = "polarization.his";
  char   *label = "", *unit ="", opt;
  double min = 0., max = 1.;
  int    dx = 10;

  struct option lo[] = {{"help",    0, NULL, 'h'}, {"input",   1, NULL, 'i'},
			{"output",  1, NULL, 'o'}, {"label",   1, NULL, 'l'},
			{"unit",    1, NULL, 'u'}, {"minimum", 1, NULL, 'm'},
			{"maximum", 1, NULL, 'M'}, {"bins",    1, NULL, 'b'},
			{NULL,      0, NULL,  0 }};
  while ((opt=getopt_long(argc, argv, "hi:o:l:u:m:M:b:", lo, NULL))!=EOF) {
    switch (opt) {
    case 'h': cout << "USAGE: " << argv[0] << " [options]" << endl
		   << "-i --input filename"                << endl
		   << "-o --output filename"               << endl
		   << "-l --label label_of_x_axis"""       << endl
		   << "-u --unit unit_of_x_axis"""         << endl
		   << "-m --minimum minimum_of_x_axis"""   << endl
		   << "-M --maximum maximum_of_x_axis"""   << endl
		   << "-b --bins Number_of_bins"           << endl; exit(0);
    case 'i': infn  = optarg;       break;
    case 'o': outfn = optarg;       break;
    case 'l': label = optarg;       break;
    case 'u': unit  = optarg;       break;
    case 'm': min   = atof(optarg); break;
    case 'M': max   = atof(optarg); break;
    case 'b': dx    = atoi(optarg); break;
    default: cerr << "unknown option " << opt << endl; exit(-3);
    }
  }

  DATA **data = new DATA*[dx];
  int *N = new int[dx];

  for (int j=0; j<dx; j++) N[j] = 0;

  {
    ifstream in(infn);
    if (!in.good()) {cerr << "Can't open file " << infn << endl; return -2; }
    while (!in.eof()) {
      if (in.get() == '#') in.ignore(1024,'\n'); // skip comment lines
      else {
	in.unget();
	double var;
	in >> var;
	int j = (int)((var - min) / (max - min) * dx);
	if (j>=0 && j<dx)	N[j]++;
      }
      in.ignore(1024,'\n');                       // skip line end
    }
    for (int j=0; j<dx; j++) N[j]--;
    for (int j=0; j<dx; j++) data[j] = new DATA[N[j]];
    
    in.close();
  }
  
   
  int *k = new int[dx];
  for (int j=0; j<dx; j++) k[j] = 0;

  int z=0;


  //Fill the data class
  {  ifstream in(infn);
  while (!in.eof()) {
    if (in.get() == '#') in.ignore(1024,'\n'); // skip comment lines
    else {
      in.unget();
      double var;
      in >> var;
      int j = (int) ((var - min) / (max - min) * dx);
      if (j>=0 && j<dx) {
	data[j][k[j]].var = var;
	in >> data[j][k[j]].electron[0]
	   >> data[j][k[j]].electron[1]
	   >> data[j][k[j]].electron[2]
	   >> data[j][k[j]].photon[0]
	   >> data[j][k[j]].photon[1]
	   >> data[j][k[j]].photon[2]
	   >> data[j][k[j]].A  
	   >> data[j][k[j]].phi 
	   >> data[j][k[j]].euler[0]
	   >> data[j][k[j]].euler[1]
	   >> data[j][k[j]].euler[2];

	double E0 = 0.855; //Beam Energy
	data[j][k[j]].eIN  = FourVector(E0, 0, 0, sqrt(E0*E0 - m_e_sqr));
	data[j][k[j]].eOUT = FourVector(sqrt(pow(data[j][k[j]].electron[0],2)+
					   pow(data[j][k[j]].electron[1],2)+
					   pow(data[j][k[j]].electron[2],2)+m_e_sqr),
        data[j][k[j]].electron[0],data[j][k[j]].electron[1],data[j][k[j]].electron[2]);  
	
	
	data[j][k[j]].qOUT = FourVector(sqrt(pow(data[j][k[j]].photon[0],2)+
					   pow(data[j][k[j]].photon[1],2)+
					   pow(data[j][k[j]].photon[2],2)),
	data[j][k[j]].photon[0],data[j][k[j]].photon[1],data[j][k[j]].photon[2]);
	
	//----------------------------------------------------    
	FourVector p_in    = FourVector(m_proton, 0, 0, 0);  
	FourVector CM      = p_in + data[j][k[j]].eIN - data[j][k[j]].eOUT;
	FourVector e_inCM  = data[j][k[j]].eIN.Lorentz(-CM).rotate(CM);
        FourVector e_outCM = data[j][k[j]].eOUT.Lorentz(-CM).rotate(CM);
	
	//Virtual photon in CM: needed by constraints
	data[j][k[j]].qphoton = e_inCM - e_outCM;
        //----------------------------------------------------
	
	//Helicity  
	double hel = data[j][k[j]].A / fabs(data[j][k[j]].A);
	
	Momentum P_Spin,P_SpinCM;
	double Norm = 1;
	double Msqr = 1;

	//Calculate the BH+B Contribution to Polarizations
	if (data[j][k[j]].qOUT.energy() != 0)
	  ElasticCrossSection(data[j][k[j]].eIN,
			      data[j][k[j]].eOUT,
			      data[j][k[j]].qOUT,hel,
			      0,
			      &P_Spin,
			      &P_SpinCM);

	data[j][k[j]].Mn = Norm;
	data[j][k[j]].M_square = Msqr;
	data[j][k[j]].BHpol[0] = P_Spin[0];
	data[j][k[j]].BHpol[1] = P_Spin[1];
	data[j][k[j]].BHpol[2] = P_Spin[2];

	if (!(++z % 100)) cout << "\rRead data " << z << "\r" << flush;

	k[j]++;
      }
    }
    in.ignore(1024,'\n');                       // skip line end
  }
  }
  
    


  /*HIST   *id[] = { HMBook1("", label, "P_x!", unit, "", dx, min, max),
    HMBook1("", label, "P_y!", unit, "", dx, min, max),
    HMBook1("", label, "P_z!", unit, "", dx, min, max),
    HMBook1("", label, "P_x!", unit, "", dx, min, max),
    HMBook1("", label, "P_y!", unit, "", dx, min, max),
    HMBook1("", label, "P_z!", unit, "", dx, min, max)};*/
  
  HIST   *id[] = { HMBook1("", label, "P(01,01)0", unit, "", dx, min, max),
		   HMBook1("", label, "P(11,11)0", unit, "", dx, min, max),
		   HMBook1("", label, "P(11,11)1", unit, "", dx, min, max),
		   HMBook1("", label, "P(01,01)1", unit, "", dx, min, max),
		   HMBook1("", label, "P(01,12)1", unit, "", dx, min, max),
		   HMBook1("", label, "P(11,02)1", unit, "", dx, min, max)};
  


  for (int j=0; j<6; j++) HMFillError(id[j]);

  for (int j=0;j<dx;j++) {
    
    double result[3] = {2.0, 0.5, 0.5};
    double error[3]  = {0, 0, 0};

    maxLikelihood ml(data[j], N[j]);
    ml.calcmin(result);
    ml.calcerr(result, error);

    ChPT_VCS vcs;
    FourVector q(0,0,0,0.6);

    cout << endl;
    cout << "P(01,01)0 = " << vcs.P01010(q)*1e3 << " scale = Bound.Cond." << endl;
    cout << "P(11,11)0 = " << vcs.P11110(q)*1e3 << " scale = Bound.Cond." << endl;
    cout << "P(11,11)1 = " << vcs.P11111(q)*1e3 << " scale = " << result[0] << "  +/- " << error[0] << endl;
    cout << "P(01,01)1 = " << vcs.P01011(q)*1e3 << " scale = " << result[1] << "  +/- " << error[1] << endl;
    cout << "P(01,12)1 = " << vcs.P01121(q)*1e3 << " scale = " << result[2] << "  +/- " << error[2] << endl;
    cout << "P(11,02)1 = " << vcs.P11021(q)*1e3 << " scale = 1 " << endl;
	 
    id[0]->data[j] = result[0];  id[0]->error[j] = error[0];
    id[1]->data[j] = result[1];  id[1]->error[j] = error[1];
    id[2]->data[j] = result[2];  id[2]->error[j] = error[2];
  }

  /*for (int j=0;j<dx;j++) {
    double result[3] = {0,0,0};
    double error[3] = {0,0,0};
    maxLikelihood ml(data[j], N[j]);
    ml.calcmin2(result);
    ml.calcerr2(result, error);
    
    cout << endl 
	 << setprecision(4)<<setw(7)<<result[0]<< " +/- " << error[0] << endl
	 << setprecision(4)<<setw(7)<<result[1]<< " +/- " << error[1] << endl
	 << setprecision(4)<<setw(7)<<result[2]<< " +/- " << error[2] << endl;
    
    id[3]->data[j] = result[0];  id[3]->error[j] = error[0];
    id[4]->data[j] = result[1];  id[4]->error[j] = error[1];
    id[5]->data[j] = result[2];  id[5]->error[j] = error[2];
    }*/

  HMWriteAll(outfn);
  return 0;
}
