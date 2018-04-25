//initialize variables
//double EMSX = 0;
//double ElossMSX1 = 0;
//double ElossMSX2 = 0;    
//double ElossMSX3 = 0;
//double ElossMSX4 = 0;

//Coefficients for Bethe_Bloch polynomial (300-1000 um SiI; 1000.1000 um Si)
//y=a0*exp(-x/a1) + a2*exp(-x/a3) + a4
//const double a300_tot[5] = {0.00271, 0.00383, 0.00273, 0.01622, 0.00058};
//const double a1000_tot[5] = {0.00610, 0.02284, 0.00447, 0.0068, 0.00158}; <<------ ovo mi treba
//const double atot_300[5] = {0.004768, 0.00112, 0.19793, 0.00029, 0.0058}; //to get initial energy from dE(300 um)
//const double atot_1000[5] = {0.05697, 0.00199, 0.05697, 0.00195, 0.01208}; //to get initial energy from dE(1000 um)

//y=p0 + p1*x + p2*x*x + p3*x*x*x + p4*x*x*x*x
//const double a300_300[5] = {0.00603, -0.90683, 110.19072, -9553.14875, 414814.03480}; //dE(300) from dE(300)
//const double a300_1000[5] = {0.00601, -0.87076, 89.63393, -5387.74749, 136500.00916}; //dE(300) from dE(1000)
//const double a1000_1000[5] = {0.01215, -0.92714, 53.99710, -2191.39968, 44909.52417}; //dE(1000) from dE(1000)
//const double a1000_300[5] = {0.01216, -0.94015, 61.40304, -3684.06714, 144679.35793}; //dE(1000) from dE(300)
//const double a_sci_fadcsum[3] = {349.875, -0.00435833, 0.000000015}; //dE(scint) from fadc.sum

//const double a1003_tot[5] = {1.260937e-02,-1.023376e+00,5.511402e+01,-1.576923e+03,1.759602e+04}; //dE(1003) from E rest
const double dE1003_Erest[5] = {1.213329e-02,-9.092758e-01,4.700306e+01,-1.411101e+03,1.805033e+04}; //dE(1003) from E rest
const double Erest_dE1003[5] = {9.174875e-02,-3.012581e+01,4.250451e+03,-2.869751e+05,7.415611e+06}; //E rest from dE(1003) 

//EMSX = EnergyfADC[1]+EnergyfADC[2]+EnergyfADC[3]+EnergyfADC[4];
//ElossMSX1 = a1000_tot[0]*exp(-EMSX/a1000_tot[1]) + a1000_tot[2]*exp(-EMSX/a1000_tot[3]) + a1000_tot[4];

//EMSX = EnergyfADC[2]+EnergyfADC[3]+EnergyfADC[4];
//ElossMSX2 = a1000_tot[0]*exp(-EMSX/a1000_tot[1]) + a1000_tot[2]*exp(-EMSX/a1000_tot[3]) + a1000_tot[4];

//EMSX = EnergyfADC[3]+EnergyfADC[4];
//ElossMSX3 = a1000_tot[0]*exp(-EMSX/a1000_tot[1]) + a1000_tot[2]*exp(-EMSX/a1000_tot[3]) + a1000_tot[4];

//EMSX = EnergyfADC[4];
//ElossMSX4 = a1000_tot[0]*exp(-EMSX/a1000_tot[1]) + a1000_tot[2]*exp(-EMSX/a1000_tot[3]) + a1000_tot[4];

//oni koje se zaustavljaju u MSX1
//bool STOP1=-(*maximum[1])<500;

//oni koje se zaustavljaju u MSX2
//bool STOP2=-(*maximum[2])<500 && -(*maximum[1])>=500;

//oni koje se zaustavljaju u MSX3
//bool STOP3=-(*maximum[3])<500 && -(*maximum[2])>=500;

//oni koje se zaustavljaju u MSX4
//bool STOP4=-(*maximum[4])<500 && -(*maximum[3])>=500;

//oni koje se zaustavljaju u MSX5
//bool STOP5=-(*maximum[5])<300 && -(*maximum[4])>=500;


// --- Coeffiecients for energy correction ---
//const double c[5] = {400, 80, 200, 250, 350}; //for lower beam currents
//const double c[5] = {450, 150, 400, 350, 300}; //for higher beam currents

bool CORR1_=(EnergyfADC[1]<0.001 && EnergyfADC[0]<26.4569*EnergyfADC[1]*EnergyfADC[1]-0.8293*EnergyfADC[1]+0.012359-0.00075) || \
	    (EnergyfADC[1]<0.0002 && EnergyfADC[0]>26.4569*EnergyfADC[1]*EnergyfADC[1]-0.8293*EnergyfADC[1]+0.012359-0.00075) || \
	    (EnergyfADC[1]<0.0005 && EnergyfADC[0]>26.4569*EnergyfADC[1]*EnergyfADC[1]-0.8293*EnergyfADC[1]+0.012359+0.00075);

bool STOP1 = CORR1_;

bool CORR2_=(EnergyfADC[2]<0.001 && EnergyfADC[1]<26.4569*EnergyfADC[2]*EnergyfADC[2]-0.8293*EnergyfADC[2]+0.012359-0.00075) || \
	    (EnergyfADC[2]<0.0002 && EnergyfADC[1]>26.4569*EnergyfADC[2]*EnergyfADC[2]-0.8293*EnergyfADC[2]+0.012359-0.00075) || \
	    (EnergyfADC[2]<0.0005 && EnergyfADC[1]>26.4569*EnergyfADC[2]*EnergyfADC[2]-0.8293*EnergyfADC[2]+0.012359+0.00075);
bool STOP2 = CORR2_ && !CORR1_;

bool CORR3_=(EnergyfADC[3]<0.001 && EnergyfADC[2]<26.4569*EnergyfADC[3]*EnergyfADC[3]-0.8293*EnergyfADC[3]+0.012359-0.00075) || \
	    (EnergyfADC[3]<0.0002 && EnergyfADC[2]>26.4569*EnergyfADC[3]*EnergyfADC[3]-0.8293*EnergyfADC[3]+0.012359-0.00075) || \
	    (EnergyfADC[3]<0.0005 && EnergyfADC[2]>26.4569*EnergyfADC[3]*EnergyfADC[3]-0.8293*EnergyfADC[3]+0.012359+0.00075);
bool STOP3 = CORR3_ && !CORR2_;

bool CORR4_=(EnergyfADC[4]<0.001 && EnergyfADC[3]<26.4569*EnergyfADC[4]*EnergyfADC[4]-0.8293*EnergyfADC[4]+0.012359-0.00075) || \
	    (EnergyfADC[4]<0.0002 && EnergyfADC[3]>26.4569*EnergyfADC[4]*EnergyfADC[4]-0.8293*EnergyfADC[4]+0.012359-0.00075) || \
	    (EnergyfADC[4]<0.0005 && EnergyfADC[3]>26.4569*EnergyfADC[4]*EnergyfADC[4]-0.8293*EnergyfADC[4]+0.012359+0.00075);
bool STOP4 = CORR4_ && !CORR3_;

bool CORR5_=-(*maximum[5])<100;
bool STOP5= CORR5_ && !CORR4_;

bool CORR6_=-(*maximum[5])<1000 && -(*maximum[4])>2100;
bool STOP6= CORR6_ && !CORR5_;


#ifdef FADC_ENERGY_CORRECTION

long double calc1=0;
long double calc2=0;
long double calc3=0;
long double calc4=0;

//korekcija za fadc2
if (STOP1==false){
    //calc1=1.56018e-10 * exp(1.06893e+03 * EnergyfADC[0]) + 8.77733e-04 * EnergyfADC[0]; //korekcija za 0.7 um Al
    //if (calc1>0.000075) calc1=0.000075;

    //calc1=3.37704e-08 * exp(6.61308e+02 * EnergyfADC[0]) + 1.42193e-04 * EnergyfADC[0]; //korekcija za 1 um Al
    //if (calc1>0.000125) calc1=0.000125;
   
    //calc1=1.12499e-09 * exp(9.75264e+02 * EnergyfADC[0]) + 1.50739e-03 * EnergyfADC[0]; //korekcija za 1.5 um Al
    //if (calc1>0.000175) calc1=0.000175;

    calc1=5.96507e-10 * exp(1.05499e+03 * EnergyfADC[0]) + 2.17676e-03 * EnergyfADC[0]; //korekcija za 2.0 um Al
    if (calc1>0.000230) calc1=0.000230; 
    
    fadc_E_corr += calc1;
}

//korekcija za fadc3
if (STOP1==false && STOP2==false){
    //calc2=1.56018e-10 * exp(1.06893e+03 * EnergyfADC[1]) + 8.77733e-04 * EnergyfADC[1]; //korekcija za 0.7 um Al
    //if (calc2>0.000075) calc2=0.000075;

    //calc2=3.37704e-08 * exp(6.61308e+02 * EnergyfADC[1]) + 1.42193e-04 * EnergyfADC[1]; //korekcija za 1 um Al
    //if (calc2>0.000125) calc2=0.000125;

    //calc2=1.12499e-09 * exp(9.75264e+02 * EnergyfADC[1]) + 1.50739e-03 * EnergyfADC[1]; //korekcija za 1.5 um Al
    //if (calc2>0.000175) calc2=0.000175;

    calc2=5.96507e-10 * exp(1.05499e+03 * EnergyfADC[1]) + 2.17676e-03 * EnergyfADC[1]; //korekcija za 2.0 um Al
    if (calc2>0.000230) calc2=0.000230;

    fadc_E_corr += calc2;
}

//korekcija za fadc4
if (STOP1==false && STOP2==false && STOP3==false){
    //calc3=1.56018e-10 * exp(1.06893e+03 * EnergyfADC[2]) + 8.77733e-04 * EnergyfADC[2]; //korekcija za 0.7 um Al
    //if (calc3>0.000075) calc3=0.000075;
    
    //calc3=3.37704e-08 * exp(6.61308e+02 * EnergyfADC[2]) + 1.42193e-04 * EnergyfADC[2]; //korekcija za 1 um Al
    //if (calc3>0.000125) calc3=0.000125;
    
    //calc3=1.12499e-09 * exp(9.75264e+02 * EnergyfADC[2]) + 1.50739e-03 * EnergyfADC[2]; //korekcija za 1.5 um Al
    //if (calc3>0.000175) calc3=0.000175;

    calc3=5.96507e-10 * exp(1.05499e+03 * EnergyfADC[2]) + 2.17676e-03 * EnergyfADC[2]; //korekcija za 2.0 um Al
    if (calc3>0.000230) calc3=0.000230;

    fadc_E_corr += calc3;
}

//korekcija sa fadc5
if (STOP1==false && STOP2==false && STOP3==false && STOP4==false){
    //calc4=1.56018e-10 * exp(1.06893e+03 * EnergyfADC[3]) + 8.77733e-04 * EnergyfADC[3]; //korekcija za 0.7 um Al
    //if (calc4>0.000075) calc4=0.000075;
    
    //calc4=3.37704e-08 * exp(6.61308e+02 * EnergyfADC[3]) + 1.42193e-04 * EnergyfADC[3]; //korekcija za 1 um Al
    //if (calc3>0.000125) calc3=0.000125;
    
    //calc4=1.12499e-09 * exp(9.75264e+02 * EnergyfADC[3]) + 1.50739e-03 * EnergyfADC[3]; //korekcija za 1.5 um Al
    //if (calc3>0.000175) calc3=0.000175;

    calc4=5.96507e-10 * exp(1.05499e+03 * EnergyfADC[3]) + 2.17676e-03 * EnergyfADC[3]; //korekcija za 2.0 um Al
    if (calc4>0.000230) calc4=0.000230;

    fadc_E_corr += calc4;
}

#endif //FADC_ENERGY_CORRECTION



