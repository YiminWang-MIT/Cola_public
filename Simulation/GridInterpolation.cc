#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include "GridInterpolation.h"


using namespace std;

int gridinterpolation::getindex(int th1, int ph1, int th2, int ph2, int T1) {
  int index = T1
    + ph2 * T1steps
    + th2 * ph2steps * T1steps
    + ph1 * th2steps * ph2steps * T1steps
    + th1 * ph1steps * th2steps * ph2steps * T1steps; 
  return index;
}

gridinterpolation::gridinterpolation() {
  //Datei öffnen
  if (getenv("He3eepnGRID")==NULL) 
    {
      cout << "Environment variable He3eepnGRID not given!\n"
	   << "This is needed for calculation.\n"
	   << "Use \"export He3eepnGRID=path\"." << endl;
      exit(-1);
    }
  char *gridfile = (char *) getenv("He3eepnGRID");
  ifstream inputfile (gridfile); 

  //Fehler wenn die Datei nicht geöffnet werden kann
  if (! inputfile.is_open()) {
    cout << "Error opening file" << endl; 
    exit (1); 
  }

  //Kommentare überspringen (#)  
  //Kommentare dürfen nur am Anfang der Datei stehen!
  char kommentar; 
  inputfile >> kommentar;
  while (kommentar == '#') {
    char dump[256];
    inputfile.getline(dump,256);
    inputfile >> kommentar;
  }

  //Letztes Zeichen wieder zurückgeben
  inputfile.putback(kommentar);

  //Ab jetzt dürfen keine Kommentare mehr in der Datei vorkommen!
  //Für die 5 Dimensionen werden jetzt die Werte eingelesen.
  //Notwendige Form:
  // -jede Dimension hat eine eigene Zeile
  // -Minimum Schrittgröße AnzahlPunkte

  inputfile >> gridT1min >> omega  >> q;
  inputfile >> th1min >> th1stepsize >> th1steps;
  inputfile >> ph1min >> ph1stepsize >> ph1steps;
  inputfile >> th2min >> th2stepsize >> th2steps;
  inputfile >> ph2min >> ph2stepsize >> ph2steps;
  inputfile >>  T1min >>  T1stepsize >>  T1steps;

  cout << "Grid information:\n"
       << "omega=" << omega <<"MeV, q=" << q << "MeV/c\n"
       << "Variable 1: " << th1min << "-" << th1min+th1stepsize*(th1steps-1) << " stepsize:" << th1stepsize << "\n"
       << "Variable 2: " << ph1min << "-" << ph1min+ph1stepsize*(ph1steps-1) << " stepsize:" << ph1stepsize << "\n"
       << "Variable 3: " << th2min << "-" << th2min+th2stepsize*(th2steps-1) << " stepsize:" << th2stepsize << "\n"
       << "Variable 4: " << ph2min << "-" << ph2min+ph2stepsize*(ph2steps-1) << " stepsize:" << ph2stepsize << "\n"
       << "Minimal kinetic energy for this grid: " << gridT1min << "MeV\n";

  if (getenv("GRID") != NULL) {
    CS = new punkt[th1steps*ph1steps*th2steps*ph2steps*T1steps];

    //Ab jetzt stehen nurnoch Wirkungsquerschnitte in der Datei

    while (! inputfile.eof()) {
      double varth1;
      double varph1;
      double varth2;
      double varph2;
      double varT1;
      double Wq;

      //Variablen und Wirkungsquerschnitt einlesen
      inputfile >> varth1 >> varph1 >> varth2 >> varph2 >> varT1 >> Wq;

      //Gitterkoordinaten errechnen
      int indexth1 = lround((varth1 - th1min) / th1stepsize);
      int indexph1 = lround((varph1 - ph1min) / ph1stepsize);
      int indexth2 = lround((varth2 - th2min) / th2stepsize);
      int indexph2 = lround((varph2 - ph2min) / ph2stepsize);
      int  indexT1 = lround(( varT1 - T1min)  / T1stepsize);

      //Wirkungsquerschnitt auf der errechneten Gitterposition abspeichern
      CS[getindex(indexth1, indexph1, indexth2, indexph2, indexT1)].y=log(Wq);
    }

    inputfile.close();

    calculate2ndderivativeth1(NATURAL);
    calculate2ndderivativeph1(NATURAL);
    calculate2ndderivativeth2(NATURAL);
    calculate2ndderivativeph2(NATURAL);
    calculate2ndderivativeT1(NATURAL);
  }
}

// Berechne die zweite Ableitung in jede der 5 möglichen Richtungen
// für jeden Punkt. Dazu wird nacheinander für jeden Punkt ein
// tridiagonales Gleichungssystem der Form
//
// | 4/3  1   0   0 ...  0   0  |   |y''_1  |   |   y_2 - 2 y_1 + y_0   |   
// |  1  4/3  1   0 ...  0   0  |   |y''_2  |   |   y_3 - 2 y_2 + y_1   |   
// |  0   1  4/3  1 ...  0   0  |   |y''_3  |   |   y_4 - 2 y_3 + y_2   |   
// |  .   .   .   . ...  .   .  | * |   .   | = |           .           |   
// |  .   .   .   . ...  .   .  |   |   .   |   |           .           |   
// |  0   0   0   0 ... 3/4  1  |   |y''_n-2|   |y_n-1 - 2 y_n-2 + y_n-3|   
// |  0   0   0   0 ...  1  3/4 |   |y''_n-1|   | y_n - 2 y_n-1 + y_n-2 |   
//
// gelöst werden. 
//Quelle des Algorithmus: Numerical Recipes in C, §3.4

// Bestimmung der zweiten Ableitung in th1-Richtung für jeden Punkt
void gridinterpolation::calculate2ndderivativeth1(boundaryCond boundary) {
  for (int i = 0; i < ph1steps; i++) {
    for (int j = 0; j < th2steps; j++) {
      for (int k = 0; k < ph2steps; k++) {
	for (int l = 0; l < T1steps; l++) {
	  // Randbedingungen
	  // NATURAL: Zweite Ableitung am Rand gleich 0 f"(min)==f"(max)==0

	  double solution[th1steps];
	  switch (boundary) {
	  case NATURAL:  
	    CS[getindex(0,i,j,k,l)].y2ndth1 = 
	      CS[getindex(th1steps-1,i,j,k,l)].y2ndth1 = 0.0;
	    solution[0]=0.0;
	    break;
	  }

	  for (int m = 1; m <= th1steps - 2; m++) {
	    double sig = 1.0 / 2.0;
	    double p = sig * CS[getindex(m-1,i,j,k,l)].y2ndth1 + 2.0;
	    CS[getindex(m,i,j,k,l)].y2ndth1 = (sig - 1.0)/p;

	    solution[m] = CS[getindex(m-1,i,j,k,l)].y 
	      - 2 * CS[getindex(m,i,j,k,l)].y 
	      + CS[getindex(m+1,i,j,k,l)].y;

	    solution[m] = (6.0 * solution[m] / 2.0 - sig * solution[m-1]) / p;
	  }

	  for (int m = th1steps - 2; m > 0; m--) 
	    CS[getindex(m,i,j,k,l)].y2ndth1 = CS[getindex(m,i,j,k,l)].y2ndth1 
	      * CS[getindex(m+1,i,j,k,l)].y2ndth1 + solution[m];
	}
      }
    }
  }
}

// Bestimmung der zweiten Ableitung in ph1-Richtung für jeden Punkt
void gridinterpolation::calculate2ndderivativeph1(boundaryCond boundary) {
  for (int i = 0; i < th1steps; i++) {
    for (int j = 0; j < th2steps; j++) {
      for (int k = 0; k < ph2steps; k++) {
	for (int l = 0; l < T1steps; l++) {
	  // Randbedingungen
	  // NATURAL: Zweite Ableitung am Rand gleich 0 f"(min)==f"(max)==0

	  double solution[ph1steps];
	  switch (boundary) {
	  case NATURAL:  
	    CS[getindex(i,0,j,k,l)].y2ndph1 = 
	      CS[getindex(i,ph1steps-1,j,k,l)].y2ndph1 = 0.0;
	    solution[0]=0.0;
	    break;
	  }

	  for (int m = 1; m <= ph1steps - 2; m++) {
	    double sig = 1.0 / 2.0;
	    double p = sig * CS[getindex(i,m-1,j,k,l)].y2ndph1 + 2.0;
	    CS[getindex(i,m,j,k,l)].y2ndph1 = (sig - 1.0)/p;

	    solution[m] = CS[getindex(i,m-1,j,k,l)].y 
	      - 2 * CS[getindex(i,m,j,k,l)].y 
	      + CS[getindex(i,m+1,j,k,l)].y;

	    solution[m] = (6.0 * solution[m] / 2.0 - sig * solution[m-1]) / p;
	  }

	  for (int m = ph1steps - 2; m > 0; m--) 
	    CS[getindex(i,m,j,k,l)].y2ndph1 = CS[getindex(i,m,j,k,l)].y2ndph1 
	      * CS[getindex(i,m+1,j,k,l)].y2ndph1 + solution[m];
	}
      }
    }
  }
}

// Bestimmung der zweiten Ableitung in th2-Richtung für jeden Punkt
void gridinterpolation::calculate2ndderivativeth2(boundaryCond boundary) {
  for (int i = 0; i < th1steps; i++) {
    for (int j = 0; j < ph1steps; j++) {
      for (int k = 0; k < ph2steps; k++) {
	for (int l = 0; l < T1steps; l++) {
	  // Randbedingungen
	  // NATURAL: Zweite Ableitung am Rand gleich 0 f"(min)==f"(max)==0

	  double solution[th2steps];
	  switch (boundary) {
	  case NATURAL:  
	    CS[getindex(i,j,0,k,l)].y2ndth2 = 
	      CS[getindex(i,j,th2steps-1,k,l)].y2ndth2 = 0.0;
	    solution[0]=0.0;
	    break;
	  }

	  for (int m = 1; m <= th2steps - 2; m++) {
	    double sig = 1.0 / 2.0;
	    double p = sig * CS[getindex(i,j,m-1,k,l)].y2ndth2 + 2.0;
	    CS[getindex(i,j,m,k,l)].y2ndth2 = (sig - 1.0)/p;

	    solution[m] = CS[getindex(i,j,m-1,k,l)].y 
	      - 2 * CS[getindex(i,j,m,k,l)].y 
	      + CS[getindex(i,j,m+1,k,l)].y;

	    solution[m] = (6.0 * solution[m] / 2.0 - sig * solution[m-1]) / p;
	  }

	  for (int m = th2steps - 2; m > 0; m--) 
	    CS[getindex(i,j,m,k,l)].y2ndth2 = CS[getindex(i,j,m,k,l)].y2ndth2 
	      * CS[getindex(i,j,m+1,k,l)].y2ndth2 + solution[m];
	}
      }
    }
  }
}

// Bestimmung der zweiten Ableitung in ph2-Richtung für jeden Punkt
void gridinterpolation::calculate2ndderivativeph2(boundaryCond boundary) {
  for (int i = 0; i < th1steps; i++) {
    for (int j = 0; j < ph1steps; j++) {
      for (int k = 0; k < th2steps; k++) {
	for (int l = 0; l < T1steps; l++) {
	  // Randbedingungen
	  // NATURAL: Zweite Ableitung am Rand gleich 0 f"(min)==f"(max)==0

	  double solution[ph2steps];
	  switch (boundary) {
	  case NATURAL:  
	    CS[getindex(i,j,k,0,l)].y2ndph2 = 
	      CS[getindex(i,j,k,ph2steps-1,l)].y2ndph2 = 0.0;
	    solution[0]=0.0;
	    break;
	  }

	  for (int m = 1; m <= ph2steps - 2; m++) {
	    double sig = 1.0 / 2.0;
	    double p = sig * CS[getindex(i,j,k,m-1,l)].y2ndph2 + 2.0;
	    CS[getindex(i,j,k,m,l)].y2ndph2 = (sig - 1.0)/p;

	    solution[m] = CS[getindex(i,j,k,m-1,l)].y 
	      - 2 * CS[getindex(i,j,k,m,l)].y 
	      + CS[getindex(i,j,k,m+1,l)].y;

	    solution[m] = (6.0 * solution[m] / 2.0 - sig * solution[m-1]) / p;
	  }

	  for (int m = ph2steps - 2; m > 0; m--) 
	    CS[getindex(i,j,k,m,l)].y2ndph2 = CS[getindex(i,j,k,m,l)].y2ndph2 
	      * CS[getindex(i,j,k,m+1,l)].y2ndph2 + solution[m];
	}
      }
    }
  }
}

// Bestimmung der zweiten Ableitung in T1-Richtung für jeden Punkt
void gridinterpolation::calculate2ndderivativeT1(boundaryCond boundary) {
  for (int i = 0; i < th1steps; i++) {
    for (int j = 0; j < ph1steps; j++) {
      for (int k = 0; k < th2steps; k++) {
	for (int l = 0; l < ph2steps; l++) {
	  // Randbedingungen
	  // NATURAL: Zweite Ableitung am Rand gleich 0 f"(min)==f"(max)==0

	  double solution[T1steps];
	  switch (boundary) {
	  case NATURAL:  
	    CS[getindex(i,j,k,l,0)].y2ndT1 = 
	      CS[getindex(i,j,k,l,T1steps-1)].y2ndT1 = 0.0;
	    solution[0]=0.0;
	    break;
	  }

	  for (int m = 1; m <= T1steps - 2; m++) {
	    double sig = 1.0 / 2.0;
	    double p = sig * CS[getindex(i,j,k,l,m-1)].y2ndT1 + 2.0;
	    CS[getindex(i,j,k,l,m)].y2ndT1 = (sig - 1.0)/p;

	    solution[m] = CS[getindex(i,j,k,l,m-1)].y 
	      - 2 * CS[getindex(i,j,k,l,m)].y 
	      + CS[getindex(i,j,k,l,m+1)].y;

	    solution[m] = (6.0 * solution[m] / 2.0 - sig * solution[m-1]) / p;
	  }

	  for (int m = T1steps - 2; m > 0; m--) 
	    CS[getindex(i,j,k,l,m)].y2ndT1 = CS[getindex(i,j,k,l,m)].y2ndT1 
	      * CS[getindex(i,j,k,l,m+1)].y2ndT1 + solution[m];
	}
      }
    }
  }
}

/*
 Um einen Punkt zu berechnen, muss jeder Parameter einzeln bestimmt werden.
 Am Anfang hat man für jeden Gitterpunkt den Funktionswert und den Wert der
 2. Ableitung in alle 5 Gitterrichtungen.
 Als erstes wird in jeder Dimension der Gitterpunkt vor dem eingegebenen Punkt,
 der Abstand zu diesem Gitterpunkt und der Abstand zum nächsten Gitterpunkt
 bestimmt.
 Der Funktionswert für die th1-Richtung wird dann per Kubischem-Spline für den 
 gegebenen th1-Wert bestimmt. Die 2. Ableitungen in den anderen 4 Richtungen 
 an diesem Punkt durch lineare Interpolation bestimmt. Dies geschieht insgesamt 
 2^4=16 mal, jeweils in den 4 restlichen Dimensionen für den Gitterpunkt vor und
 hinter dem gegebenen Punkt.
 So werden nacheinander alle 5 Dimensionen abgearbeitet, bis am Schluss nur noch 
 eine kubische Splineinterpolation übrig bleibt.
 Die Formel für Die kubische Splineinterpolation lautet (in einer Dimension):
 
 y(x) = A * y_i + B * y_{i+1} * (A * A * A - A) * y''_i + (B * B * B - B) * y''_{i+1}

 wobei die Gitterkonstante gleich 1 ist,
 i <= x < i+1,
 A = x - i + 1,
 B = A - 1 = x - i.
*/

double gridinterpolation::getCS(double th1, double ph1, double th2, double ph2, double T1) {

  //Variablen auf die Gittergröße umrechnen
  double th1grid = (th1 - th1min) / th1stepsize;
  double ph1grid = (ph1 - ph1min) / ph1stepsize;
  double th2grid = (th2 - th2min) / th2stepsize;
  double ph2grid = (ph2 - ph2min) / ph2stepsize;
  double  T1grid = ( T1 -  T1min) /  T1stepsize;

  //Gitterpunkt vor der Variable berechnen
  int o = (int) (trunc(th1grid));
  int p = (int) (trunc(ph1grid));
  int q = (int) (trunc(th2grid));
  int r = (int) (trunc(ph2grid));
  int s = (int) (trunc( T1grid));
 
  //Abstand vom vorhergehenden Gitterpunkt zum gegebenen Punkt
  double om = th1grid - o;
  double pm = ph1grid - p;
  double qm = th2grid - q;
  double rm = ph2grid - r;
  double sm =  T1grid - s;

  //Abstand vom gegebenen Punkt zum nächsten Gitterpunkt
  double op = 1 - om;
  double pp = 1 - pm;
  double qp = 1 - qm;
  double rp = 1 - rm;
  double sp = 1 - sm;

  double help = 1.0 / 6.0;

  //th1 festlegen
  punkt firststep[2][2][2][2];
  double C = help * (op * op * op - op);
  double D = help * (om * om * om - om);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {
	for (int l = 0; l < 2; l++) {
	  int indexo = getindex(o,p+i,q+j,r+k,s+l);
	  int indexopo = getindex(o+1,p+i,q+j,r+k,s+l);
	  firststep[i][j][k][l].y = CS[indexo].y * op
	    + CS[indexopo].y * om
	    + CS[indexo].y2ndth1 * C
	    + CS[indexopo].y2ndth1 * D;

	  firststep[i][j][k][l].y2ndph1 = CS[indexo].y2ndph1 * op
	    + CS[indexopo].y2ndph1 * om;

	  firststep[i][j][k][l].y2ndth2 = CS[indexo].y2ndth2 * op
	    + CS[indexopo].y2ndth2 * om;

	  firststep[i][j][k][l].y2ndph2 = CS[indexo].y2ndph2 * op
	    + CS[indexopo].y2ndph2 * om;

	  firststep[i][j][k][l].y2ndT1 = CS[indexo].y2ndT1 * op
	    + CS[indexopo].y2ndT1 * om;
	}
      }
    }
  }

  //ph1 festlegen
  punkt secondstep[2][2][2];
  C = help * (pp * pp * pp - pp);
  D = help * (pm * pm * pm - pm);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2 ; k++) {
	secondstep[i][j][k].y = firststep[0][i][j][k].y * pp
	  + firststep[1][i][j][k].y * pm
	  + firststep[0][i][j][k].y2ndph1 * C
	  + firststep[1][i][j][k].y2ndph1 * D;

	secondstep[i][j][k].y2ndth2 = firststep[0][i][j][k].y2ndth2 * pp
	  + firststep[1][i][j][k].y2ndth2 * pm;

	secondstep[i][j][k].y2ndph2 = firststep[0][i][j][k].y2ndph2 * pp
	  + firststep[1][i][j][k].y2ndph2 * pm;

	secondstep[i][j][k].y2ndT1 = firststep[0][i][j][k].y2ndT1 * pp
	  + firststep[1][i][j][k].y2ndT1 * pm;
      }
    }
  }

  //th2 festlegen
  punkt thirdstep[2][2];
  C = help * (qp * qp * qp - qp);
  D = help * (qm * qm * qm - qm);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      thirdstep[i][j].y = secondstep[0][i][j].y *qp
	+ secondstep[1][i][j].y * qm
	+ secondstep[0][i][j].y2ndth2 * C
	+ secondstep[1][i][j].y2ndth2 * D;

      thirdstep[i][j].y2ndph2 = secondstep[0][i][j].y2ndph2 * qp
	+ secondstep[1][i][j].y2ndph2 * qm; 

      thirdstep[i][j].y2ndT1 = secondstep[0][i][j].y2ndT1 * qp
	+ secondstep[1][i][j].y2ndT1 * qm;
    }
  }

  //ph2 festlegen
  punkt fourthstep[2];
  C = help * (rp * rp * rp - rp);
  D = help * (rm * rm * rm - rm);
  for (int i = 0; i < 2; i++) {
    fourthstep[i].y = thirdstep[0][i].y * rp
      + thirdstep[1][i].y * rm
      + thirdstep[0][i].y2ndph2 * C
      + thirdstep[1][i].y2ndph2 * D;

    fourthstep[i].y2ndT1 = thirdstep[0][i].y2ndT1 * rp
      + thirdstep[1][i].y2ndT1 * rm;
  }

  double result = fourthstep[0].y * sp
      + fourthstep[1].y * sm
      + fourthstep[0].y2ndT1 * help * (sp * sp * sp - sp)
      + fourthstep[1].y2ndT1 * help * (sm * sm * sm - sm);


  return exp(result); 
}


