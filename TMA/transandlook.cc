// the guy to blame:
// 
// Jan C. Bernauer             mailto:bernauer@kph.uni-mainz.de
// A1 Collaboration 
// Nov. of 2003 
//

#include "transandlook.h"
using namespace std;

#include <math.h>
TL::TL()
{
    DEF_GRIDSIZE[0]=DEF_0;
    DEF_GRIDSIZE[1]=DEF_1;
    DEF_GRIDSIZE[2]=DEF_2;
    DEF_GRIDSIZE[3]=DEF_3;
    DEF_N=(DEF_0+4)*(DEF_1+4)*(DEF_2+4)*(DEF_3+4);
}

inline double spline(int i, double x)
{
    double t=x-floor(x);
    switch (i) {
        case 0:
	    double temp;
	    temp=1-t;
	    return temp*temp*temp/6.0f;
	    break;
        case 1:
	    return ((0.5f*t-1)*t*t+1.0f/1.5f);
	    break;
        case 2:
	    return ((-0.5f*t+0.5f)*t+0.5f)*t+1.0f/6.0f;
	    break;
        case 3:
	    return t*t*t/6.0f;
	    break;
    }
    return 0;
}

#define OVERFLOW_FLAG
void TL::lookup(double *src,double *tgt)
{
    int i;

    int base[DEF_DIMIN];
    double rel[DEF_DIMIN];
    double temp;

    int c[DEF_DIMIN];
    double s[DEF_DIMIN];
    int p[DEF_DIMIN];
    //mangling source

#ifdef OVERFLOW_FAST
    for (i=0;i<DEF_DIMIN;i++) {
	temp=DEF_GRIDSIZE[i]*((src[i]-m_centerSrc[i])/m_lengthSrc[i]+0.5f);
	if (temp<0) {
	    base[i]=0; rel[i]=0; cerr<<"Underflow: "<<i<<endl;
	} else
	    if (temp>DEF_GRIDSIZE[i]) {
		base[i]=DEF_GRIDSIZE[i]; rel[i]=0; cerr<<"Overflow: "<<i<<endl;
	    }else {
		base[i]=(int) floor(temp);
		rel[i]=temp-base[i];}
	
    }
#endif
#ifdef OVERFLOW_SENS
    for (i=0;i<DEF_DIMIN;i++) {
	temp=DEF_GRIDSIZE[i]*((src[i]-m_centerSrc[i])/m_lengthSrc[i]+0.5f);
	if (temp<0) {
	    base[i]=0; rel[i]=temp;
	} else
	    if (temp>DEF_GRIDSIZE[i]) {
		base[i]=DEF_GRIDSIZE[i]; rel[i]=temp-DEF_GRIDSIZE[i];
	    }else {
		base[i]=(int) floor(temp);
		rel[i]=temp-base[i];}
    }
#endif
#ifdef OVERFLOW_FLAG
for (i=0;i<DEF_DIMIN;i++) {
	temp=DEF_GRIDSIZE[i]*((src[i]-m_centerSrc[i])/m_lengthSrc[i]+0.5f);
	if ((temp<0) || (temp>DEF_GRIDSIZE[i]))
	{
	    for (int j=0;j<DEF_DIMOUT;j++) tgt[j]=1e8;
	    return;
	}
	base[i]=(int) floor(temp);
	rel[i]=temp-base[i];
}
#endif


    //Setup of output
    for(i=0;i<DEF_DIMOUT;i++) tgt[i]=m_centerTgt[i];

    //And now the tricky part ;)
    for (c[3]=0;c[3]<4;c[3]++) {
	s[3]=spline(c[3],rel[3]);
	p[3]=c[3]+base[3];
	for (c[2]=0;c[2]<4;c[2]++) {
	    s[2]=s[3]*spline(c[2],rel[2]);
	    p[2]=p[3]*(DEF_GRIDSIZE[2]+4)+c[2]+base[2];
	    for (c[1]=0;c[1]<4;c[1]++) {
		s[1]=s[2]*spline(c[1],rel[1]);	
		p[1]=p[2]*(DEF_GRIDSIZE[1]+4)+c[1]+base[1];
		for (c[0]=0;c[0]<4;c[0]++) {
		    s[0]=s[1]*spline(c[0],rel[0]);
		    p[0]=p[1]*(DEF_GRIDSIZE[0]+4)+c[0]+base[0];
		    for (i=0;i<DEF_DIMOUT;i++) 
			tgt[i]+=s[0]*solution[p[0]][i];
		}
	    }
	}
    }
}





void TL::LoadSolution(const char *name)
{
ifstream *in=new ifstream(name);
int i,j;
 cerr<<"# of vertices"<<endl;
 DEF_N=1;
for (i=0;i<DEF_DIMIN;i++) {(*in)>>DEF_GRIDSIZE[i];DEF_N*=(DEF_GRIDSIZE[i]+4);cerr<<DEF_GRIDSIZE[i]<<"\t";}
 cerr <<"->"<<DEF_N<<endl; 
 cerr <<"Center and length of source space:"<<endl;
for (i=0;i<DEF_DIMIN;i++) {(*in)>>m_centerSrc[i]>>m_lengthSrc[i];cerr<<m_centerSrc[i]<<" "<<m_lengthSrc[i]<<"   ";}
cerr<<endl<<endl<<"Center of target space:"<<endl;
for (i=0;i<DEF_DIMOUT;i++) {(*in)>>m_centerTgt[i]; cerr<<m_centerTgt[i]<<"    ";}
cout <<endl;
for (j=0;j<DEF_DIMOUT;j++) 
    for (i=0;i<DEF_N;i++) (*in) >>solution[i][j];
cerr << "Solution read in"<<endl;    

in->close();
};


void TL::LoadFirstOrder(const char *name)
{
    ifstream *in=new ifstream(name);
    int i,j,k;
    for (i=0;i<DEF_DIMIN;i++) *in >>firstorderconst[i];
    for (j=0;j<DEF_DIMIN;j++)
	for (i=0;i<DEF_DIMIN;i++) *in >>firstorderlin[j][i];
    for (k=0;k<DEF_DIMIN;k++)
	for (j=0;j<DEF_DIMIN;j++)
	    for (i=0;i<DEF_DIMIN;i++) *in >>firstorderquad[k][j][i];
    in->close();
}

/*
  !   0.0176991    0.           0.           0.        !
  !   0.0293730  - 0.5319149    0.           0.        !
  !   0.           0.         - 0.3991458    1.576626  !
  !   0.           0.         - 0.6516056    0.0738420 !
*/


void TL::firstorderreconstruct(double *v)
{
    double t[DEF_DIMIN];
    int i,j,k;
    for (i=0;i<DEF_DIMIN;i++)
    {
	t[i]=v[i];
	v[i]=firstorderconst[i];
    }
    for (j=0;j<DEF_DIMIN;j++)    
	for (i=0;i<DEF_DIMIN;i++) v[j]+=t[i]*firstorderlin[j][i];
    for (k=0;k<DEF_DIMIN;k++)  
	for (j=0;j<DEF_DIMIN;j++)    
	    for (i=0;i<DEF_DIMIN;i++) v[k]+=t[j]*t[i]*firstorderquad[k][j][i];
}
