#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <unistd.h>

#define sqr(x) ((x)*(x))

FILE *in1;

double f,t;
double sgl_p,sgl_m;
double coinc_p,coinc_m;
double acc_p,acc_m;

char *infile;

int main (int argc, char *argv[]) 
{
  if (argc<2) {
    std::cerr << "Usage:\t  "<<argv[0]<<" filename {options}" << std::endl;
    std::cerr << "options:" << std::endl;
    std::cerr << "-B\t   use analyzing strength for 855 MeV" << std::endl;
    std::cerr << "-C\t   use analyzing strength for 1508 MeV" << std::endl;
    std::cerr << "-d\t   use dilution factor 1.067 +/- 0.015" << std::endl;
    return -1;
  }
  printf("\n*******************************************\n");
  printf("%s                      *\n",argv[1]);
  printf("*******************************************\n");

  infile = argv[1];


  in1 = fopen(infile,"r");
  
  //bss  printf("scanning data\n");
  if (fscanf(in1,"%lf\n",&f)); 
  if (fscanf(in1,"%lf\n",&t)); 
  if (fscanf(in1,"%lf\n",&sgl_p)); 
  if (fscanf(in1,"%lf\n",&sgl_m)); 
  if (fscanf(in1,"%lf\n",&coinc_p)); 
  if (fscanf(in1,"%lf\n",&coinc_m)); 
  if (fscanf(in1,"%lf\n",&acc_p)); 
  if (fscanf(in1,"%lf\n",&acc_m)); 
  //bss  printf("done\n");

  // Fehler
  double dstat_sgl_p = sqrt(sgl_p);
  double dstat_sgl_m = sqrt(sgl_m);
  double d_sgl_p = dstat_sgl_p;
  double d_sgl_m = dstat_sgl_m;

  double dstat_coinc_p = sqrt(coinc_p);
  double dstat_coinc_m = sqrt(coinc_m);
  double d_coinc_p = dstat_coinc_p;
  double d_coinc_m = dstat_coinc_m;

  double dstat_acc_p = sqrt(acc_p);
  double dstat_acc_m = sqrt(acc_m);
  double d_acc_p = dstat_acc_p;
  double d_acc_m = dstat_acc_m;

  // korrigierte Koinzidenzen
  double true_p = (coinc_p-acc_p);
  double true_m = (coinc_m-acc_m);
  double d_true_p = sqrt(d_coinc_p*d_coinc_p + d_acc_p*d_acc_p);
  double d_true_m = sqrt(d_coinc_m*d_coinc_m + d_acc_m*d_acc_m);
 
  // Ladungsnormierte Koinzidenzen
  double norm_p = true_p/sgl_p;
  double norm_m = true_m/sgl_m;
  double d_norm_p = fabs(norm_p)*sqrt(sqr(d_true_p/true_p)+sqr(d_sgl_p/sgl_p));
  double d_norm_m = fabs(norm_m)*sqrt(sqr(d_true_m/true_m)+sqr(d_sgl_m/sgl_m));
  
  // Asymmetrie
  double minus = norm_p-norm_m;
  double dminus = sqrt(sqr(d_norm_p)+sqr(d_norm_m));
  double plus = norm_p+norm_m;
  double dplus = dminus;

  double eps = minus/plus;
  double deps = fabs(eps)*sqrt(sqr(dminus/minus)+sqr(dplus/plus));
  
  // Ladungsasymmetrie
  double lasym = (sgl_p - sgl_m)/(sgl_p+sgl_m);
  double dlasym = sqrt(sgl_m*sgl_p / pow((sgl_p+sgl_m),3));

  // Polarisation
  // P = c * A / (pt * azz)
  // mit gemessener Asymmetrie A = eps, Targetpolarisation pt, 
  // gemittelter Analysierstaerke azz, und "Verduennungsfaktor" c 
  const double pt        =  0.0806;
  const double dpt       =  0.0005;  // 0.6 % rel
  const double azz_855   = -0.7712;  //855 MeV  (mult)
  const double azz_1508  = -0.7688;  //1508 MeV (mult)
  const double dazz_855  =  0.0013;  // 0.2 % rel (this is NOT 0.2%!!)
  const double dazz_1508 =  0.0015;  // 0.2 % rel


  double c    =  1.0; //default, no dilution factor
  double dc   =  0.0;   
  //double c = 1.067; //only temp. A4 not to confuse
  //double dc = 0.015;
  //  double azz  =  azz_1508; //default 1508 MeV electron energy
  double azz  =  azz_1508; //default 855 MeV electron energy
  double dazz = dazz_1508;

  char opt;
  while ((opt = getopt(argc, argv, "dfBC")) != EOF) {
    switch (opt) {
    case 'd': c = 1.067; dc = 0.015; break;
    case 'f': c = 1.0;   dc = 0.0;   break;  
    case 'B': azz = azz_855;  dazz = dazz_855;  break; //855  MeV beam energy
    case 'C': azz = azz_1508; dazz = dazz_1508; break; //1508 MeV beam energy
    }
  }

  double p = c * eps / (azz * pt);
  double dpstat = fabs(c * deps /(azz * pt)); // Fehler nur stat
  double dpsyst = sqrt( // Fehler system.
   (sqr(dc/azz/pt)+
    sqr(c*dpt/azz/pt/pt)+
    sqr(c*dazz/pt/azz/azz)) * sqr(eps));

  printf("time  = %.0f s (@ %.0f Hz)\n", t, f);
  printf("sgl + = (%9.3f ± %4.3f) k   (%5.1f kHz)\n", 
	 sgl_p/1000, dstat_sgl_p/1000, sgl_p/1000/t);
  printf("sgl - = (%9.3f ± %4.3f) k   (%5.1f kHz)\n", 
	 sgl_m/1000, dstat_sgl_m/1000, sgl_m/1000/t);
  printf("                                (%5.1f kHz)\n", 
	 (sgl_p + sgl_m)/1000/t);  
  printf("& +   = (%9.3f ± %4.3f) k   (%5.1f kHz)\n", 
	 coinc_p/1000, dstat_coinc_p/1000, coinc_p/1000/t);
  printf("& -   = (%9.3f ± %4.3f) k   (%5.1f kHz)\n", 
	 coinc_m/1000, dstat_coinc_m/1000, coinc_m/1000/t);
  printf("                                (%5.1f kHz)\n", 
	 (coinc_p + coinc_m)/1000/t);  
  printf("acc + = (%9.3f ± %4.3f) k   (%5.1f kHz)\n", 
	 acc_p/1000, dstat_acc_p/1000, acc_p/1000/t);
  printf("acc - = (%9.3f ± %4.3f) k   (%5.1f kHz)\n", 
	 acc_m/1000, dstat_acc_m/1000, acc_m/1000/t);
  printf("                                (%5.1f kHz)\n", 
	 (acc_p + acc_m)/1000/t);  

  printf("azz   = (%9.4f ± %5.4f)", azz, dazz);
  if (azz == azz_1508) printf("    ( 1508 MeV)");
  else if (azz == azz_855)  printf("    (  855 MeV)");
  printf("\n");
  printf("d.f.  = (%9.4f ± %5.4f)\n\n", c, dc);

  printf("(Lp-Lm)/(Lp+Lm) = (%.2f ± %.2f)%%\n", 100*lasym, 100*dlasym);
  printf("asymmetry    = (%.2f ± %.2f)%%\n", 100*eps, 100*deps);
  //  printf("charge asym  = (%.2f ± %.2f)%% (Lp-Lm)/(Lp+Lm)\n", 
  //	 100*lasym, 100*dlasym);
  printf("polarization = (%.1f ± %.1fstat ± %.1fsyst)%%\n"
	 , 100*p, 100*dpstat,100*dpsyst);
  printf("*******************************************\n\n");
  fclose(in1);
};
