//
// $Id: KAOSfield.cc 2348 2009-10-09 10:07:29Z merkel $
//

#include "KAOSfield.h"

using namespace std;

int KAOSField::getindex(int x, int y, int z) const
{
  return x + anzx*y + anzx*anzy_sp*z;
}




void KAOSField::cal2ndderivatex(feldpunkt *fp)
{
  double sig = 1.0 / 2.0;
  for (int i=0;i<anzy_sp;i++) {
    for (int j=0;j<anzz;j++) {
      double *solution = new double[anzx];
      fp[getindex(0,i,j)].d2x=0.0;
      fp[getindex(anzx-1,i,j)].d2x=0.0;                       
      solution[0]=0.0;
      for (int m=1;m<=anzx-2;m++) {
        double p = sig * fp[getindex(m-1,i,j)].d2x + 2.0;
        fp[getindex(m,i,j)].d2x = (sig - 1.0)/p;
        solution[m] = fp[getindex(m-1,i,j)].wert
            - 2.0 * fp[getindex(m,i,j)].wert
            + fp[getindex(m+1,i,j)].wert;
        solution[m]=(6.0*solution[m]/2.0-sig*solution[m-1])/p;
      } 
      for (int m=anzx-2;m>0;m--) {
        fp[getindex(m,i,j)].d2x=fp[getindex(m,i,j)].d2x
            *fp[getindex(m+1,i,j)].d2x+solution[m];
      } 
    } 
  } 
}

void KAOSField::cal2ndderivatey(feldpunkt *fp)
{
  double sig = 1.0 / 2.0;
  for (int i=0;i<anzx;i++) {
    for (int j=0;j<anzz;j++) {
      double *solution = new double[anzy_sp];
      fp[getindex(i,0,j)].d2y=0.0;
      fp[getindex(i,anzy_sp-1,j)].d2y=0.0;                    
      solution[0]=0.0;
      for (int m=1;m<=anzy_sp-2;m++) {
        double p = sig * fp[getindex(i,m-1,j)].d2y + 2.0;
        fp[getindex(i,m,j)].d2y = (sig - 1.0)/p;
        solution[m] = fp[getindex(i,m-1,j)].wert
            - 2.0 * fp[getindex(i,m,j)].wert
            + fp[getindex(i,m+1,j)].wert;
        solution[m]=(6.0*solution[m]/2.0-sig*solution[m-1])/p;
      } 
      for (int m=anzy_sp-2;m>0;m--) {
        fp[getindex(i,m,j)].d2y=fp[getindex(i,m,j)].d2y
            *fp[getindex(i,m+1,j)].d2y+solution[m];
      }  
    } 
  } 
}

void KAOSField::cal2ndderivatez(feldpunkt *fp)
{
  double sig = 1.0 / 2.0;
  for (int i=0;i<anzx;i++) {
    for (int j=0;j<anzy_sp;j++) {
      double *solution = new double[anzz];
      fp[getindex(i,j,0)].d2z=0.0;
      fp[getindex(i,j,anzz-1)].d2z=0.0;                       
      solution[0]=0.0;
      for (int m=1;m<=anzz-2;m++) {
        double p = sig * fp[getindex(i,j,m-1)].d2z + 2.0;
        fp[getindex(i,j,m)].d2z = (sig - 1.0)/p;
        solution[m] = fp[getindex(i,j,m-1)].wert
            - 2.0 * fp[getindex(i,j,m)].wert
            + fp[getindex(i,j,m+1)].wert;
        solution[m]=(6.0*solution[m]/2.0-sig*solution[m-1])/p;
      } 
      for (int m=anzz-2;m>0;m--) {
        fp[getindex(i,j,m)].d2z=fp[getindex(i,j,m)].d2z
            *fp[getindex(i,j,m+1)].d2z+solution[m];
      } 
    } 
  } 
}

// Set Magnetic Field to fraction (factor Fract) of nominal Field:
int KAOSField::ScaleField(double Fract)  
{
  B_Act = Fract*B_Nom;
  return 0;
}



KAOSField::KAOSField(double Bfield)
{
  cout<<"Start"<<endl;
  B_Nom = Bfield; // maximum dipole field
  B_Act = B_Nom;
  char *pfad = "dipmap2.dat";
  cout << "<KAOSField::KAOSField>: Lade Feldkarte aus: " << pfad << endl;
  ifstream in(pfad);
  
  feld = new messpunkt*** [300];
  for(int i = 0; i<300; i++) feld[i] = new messpunkt** [20];
  for(int i = 0; i<300; i++){
    for(int j = 0; j<20; j++) feld[i][j] = new messpunkt*[231];
  }
  for(int i = 0; i<300; i++){
    for(int j = 0; j<20; j++){
      for(int k = 0; k<231; k++) feld[i][j][k] = new messpunkt[3];
    }
  }
 
  cout<<"Creating!"<<endl;
  if (in.good())
  {
    in >> zmin >> zmax >> z0
        >> xmin >> xmax >> x0
        >> ymin >> ymax >> y0;
      
      
    anzx=int(x0)+(2*Splinerand);
    anzy=int(y0)+(2*Splinerand);
    anzy_sp=int(y0*2-1)+(2*Splinerand);
    anzz=int(z0)+(2*Splinerand);
    ymin_sp=-ymax;
    int x,y,z,k;

    for(k=0;k<3;k++) {
      splinefeld[k]=(feldpunkt *)calloc(anzx*anzy_sp*anzz,sizeof(feldpunkt));
    }
    
    kdx=(xmax*cm-xmin*cm)/(x0-1);
    kdy=(ymax*cm-ymin*cm)/(y0-1);
    kdz=(zmax*cm-zmin*cm)/(z0-1);
    cout << "Gittergroesse: " << kdx << " " << kdy << " " << kdz << endl;

    for(x=0; x<(anzx-(2*Splinerand)); x++) {
      for(y=0; y<(anzy-(2*Splinerand)); y++) {
        for(z=0; z<(anzz-(2*Splinerand)); z++) {
          int zelle;
          for(k=0;k<3;k++) {
            feldpunkt *fp;
            in >> feld[x][y][z][k].f;
            fp=splinefeld[k];
            zelle=getindex(x+Splinerand,anzy-1-Splinerand+y,z+Splinerand);
            fp[zelle].wert=feld[x][y][z][k].f;
            zelle=getindex(x+Splinerand,anzy-1-Splinerand-y,z+Splinerand);
            if (k==1) {
              fp[zelle].wert=feld[x][y][z][k].f;                                                      
            } else {
              if (y==0) {
                fp[zelle].wert=0;                                              
              } else {
                fp[zelle].wert=-feld[x][y][z][k].f;                                                     
              }
            }
          } 
        } 
      } 
    } 
    cout<<"Loaded!"<<endl;
    for(k=0;k<3;k++) {
      feldpunkt *fp;
      fp=splinefeld[k];
      for(x=0; x<(anzx); x++) {
        for(y=0; y<(anzy_sp); y++) {
          int zelle=0;
          for (int i=0;i<Splinerand;i++) {
            zelle=getindex(x,y,i);                          
            fp[zelle].wert=0.0;
            zelle=getindex(x,y,anzz-1-i);                           
            fp[zelle].wert=0.0;
          }
        }
      }   
                
      for(x=0; x<(anzx); x++) {
        for(z=0; z<(anzz); z++) {
          int zelle=0;
          for (int i=0;i<Splinerand;i++) {
            zelle=getindex(x,i,z);                          
            fp[zelle].wert=0.0;
            zelle=getindex(x,anzy_sp-1-i,z);                                
            fp[zelle].wert=0.0;
          }
        }
      }               
      for(y=0; y<(anzy_sp); y++) {
        for(z=0; z<(anzz); z++) {
          int zelle=0;
          for (int i=0;i<Splinerand;i++) {
            zelle=getindex(i,y,z);                          
            fp[zelle].wert=0.0;
            zelle=getindex(anzx-1-i,y,z);                           
            fp[zelle].wert=0.0;
          }
        }
      }               
    }
    cout<<"Loaded 2!"<<endl;  
    for(k=0;k<3;k++) {
      cal2ndderivatex(splinefeld[k]);
      cal2ndderivatey(splinefeld[k]);
      cal2ndderivatez(splinefeld[k]);
    }   
    cout<<"Loaded 3!"<<endl;              
  } 
  else
  {
    cerr << "<KAOSField::KAOSField> --- Error" << endl;
    cerr << "Kann Feldkarte >" << pfad << "< nicht oeffnen." << endl;
  }

  in.close();
  cout << "gelesen: ";
  cout << "anz x:"<< anzx << " y:" << anzy << " y_sp:" << anzy_sp << " z:" << anzz << endl; 
  cout << "Min x:"<< xmin << " y:" << ymin << " z:" << zmin << endl; 
  cout << "Max x:"<< xmax << " y:" << ymax << " z:" << zmax << endl; 
  
}

KAOSField::~KAOSField()
{
  for(int i = 0; i<300; i++){
    for(int j = 0; j<20; j++){
      for(int k = 0; k<231; k++) delete[] feld[i][j][k];
    }
  }
  for(int i = 0; i<300; i++){
    for(int j = 0; j<20; j++) delete[] feld[i][j]; 
  }
  for(int i = 0; i<300; i++) delete[] feld[i];
  delete[] feld;
  
  cout << "<KAOSField~> Terminiert" << endl; 
}

void KAOSField::BerechneDipolFeld(double xl[3], double *Bfield)
{
  double dsx, dsy, dsz;
  double dx, dy, dz;
  double wx2,wy2,wz2,wx,wy,wz,rwx,rwy,rwz;
  int ix,iy,iz;
  float b1,b2,b3,b4,b5,b6,b7,b8;
  int k;

  dsx = xl[0]/cm-xmin;
  dsy = xl[1]/cm-ymin;
  dsz = xl[2]/cm-zmin;

  dx=fabs(dsx);
  dy=fabs(dsy);
  dz=fabs(dsz);

  wx2=dx - ((int)(dx/2)*2.0);
  wy2=dy - ((int)(dy/2)*2.0);
  wz2=dz - ((int)(dz/2)*2.0);

  wx=wx2*0.5;
  wy=wy2*0.5;
  wz=wz2*0.5;
  rwx=1-wx;
  rwy=1-wy;
  rwz=1-wz;

  ix=(int)((dx-wx2)/2);
  iy=(int)((dy-wy2)/2);
  iz=(int)((dz-wz2)/2);

  for (k=0;k<3;k++) {
    feldpunkt *fp=splinefeld[k];
    b1=fp[getindex(ix+Splinerand,  iy+Splinerand,  iz+Splinerand)].wert;
    b2=fp[getindex(ix+1+Splinerand,iy+Splinerand,  iz+Splinerand)].wert;
    b3=fp[getindex(ix+Splinerand,  iy+1+Splinerand,iz+Splinerand)].wert;
    b4=fp[getindex(ix+1+Splinerand,iy+1+Splinerand,iz+Splinerand)].wert;
    b5=fp[getindex(ix+Splinerand,  iy+Splinerand,  iz+1+Splinerand)].wert;
    b6=fp[getindex(ix+1+Splinerand,iy+Splinerand,  iz+1+Splinerand)].wert;
    b7=fp[getindex(ix+Splinerand,  iy+1+Splinerand,iz+1+Splinerand)].wert;
    b8=fp[getindex(ix+1+Splinerand,iy+1+Splinerand,iz+1+Splinerand)].wert;
    Bfield[k] = b1*rwx*rwy*rwz +
        b2*wx*rwy*rwz +
        b3*rwx*wy*rwz +
        b4*wx*wy*rwz +
        b5*rwx*rwy*wz +
        b6*wx*rwy*wz +
        b7*rwx*wy*wz +
        b8*wx*wy*wz;
  }

  if (dsy<0)
  {
    Bfield[0]=-Bfield[0];
    Bfield[2]=-Bfield[2];
  }
  
  // In the end we fix field units
  Bfield[0] = -0.000056497*B_Act*Bfield[0]/10000.0;
  Bfield[1] = -0.000056497*B_Act*Bfield[1]/10000.0;
  Bfield[2] = -0.000056497*B_Act*Bfield[2]/10000.0;
}




double  KAOSField::interpolate(feldpunkt *fp, double x, double y, double z) const
{
  double xgrid=((x-xmin*cm) / kdx)+Splinerand;
  double ygrid=((y-ymin_sp*cm) / kdy)+Splinerand;
  double zgrid=((z-zmin*cm) / kdz)+Splinerand;
        
  int o = (int) (trunc(xgrid));
  int p = (int) (trunc(ygrid));
  int q = (int) (trunc(zgrid));

  if ((o<0) || (o>anzx)) {
    cerr << "o zu gross: \a" << o << endl;
  }
  if ((p<0) || (p>anzy_sp)) {
    cerr << "p zu gross: \a" << p << endl;
    
  }
  if ((q<0) || (q>anzz)) {
    cerr << "q zu gross: \a" << q << endl;
  }
                
  double om = xgrid - o;
  double pm = ygrid - p;
  double qm = zgrid - q;
        
  double op = 1 - om;
  double pp = 1 - pm;
  double qp = 1 - qm;
        
  double help = 1.0/6.0;
        
  feldpunkt firststep[2][2];
  double C = help * (op*op*op-op);
  double D = help * (om*om*om-om);
        
  for (int i=0;i<2;i++) {
    for (int j=0;j<2;j++) {
      int indexo = getindex(o,p+i,q+j);
      int indexopo = getindex(o+1,p+i,q+j);
      firststep[i][j].wert = fp[indexo].wert * op
          + fp[indexopo].wert * om
          + fp[indexo].d2x * C
          + fp[indexopo].d2x * D;
      firststep[i][j].d2y = fp[indexo].d2y * op
          +fp[indexopo].d2y * om;
      firststep[i][j].d2z = fp[indexo].d2z * op
          +fp[indexopo].d2z * om;
    }
  }


  feldpunkt secondstep[2];
  C = help * (pp*pp*pp-pp);
  D = help * (pm*pm*pm-pm);
        
  for (int i=0;i<2;i++) {
    secondstep[i].wert = firststep[0][i].wert * pp
        + firststep[1][i].wert * pm
        + firststep[0][i].d2y * C
        + firststep[1][i].d2y * D;
    secondstep[i].d2z = firststep[0][i].d2z * pp
        +firststep[1][i].d2z * pm;
  }
  double result = secondstep[0].wert * qp
        + secondstep[1].wert * qm
        + secondstep[0].d2z * help * (qp*qp*qp-qp)
        + secondstep[1].d2z * help * (qm*qm*qm-qm);
  return result;
}

void KAOSField::BerechneDipolFeldGlatt(double xl[3], double *Bfield)
{
  //  double dsx, dsy, dsz;
  double dx, dy, dz;

  dx=xl[0];
  dy=xl[1];
  dz=xl[2];
  for (int k=0;k<3;k++) {
    Bfield[k] = interpolate(splinefeld[k],dx,dy,dz);
  }
  
  // In the end we fix field units
  Bfield[0] = -0.000056497*B_Act*Bfield[0];
  Bfield[1] = -0.000056497*B_Act*Bfield[1];
  Bfield[2] = -0.000056497*B_Act*Bfield[2];
 
}


