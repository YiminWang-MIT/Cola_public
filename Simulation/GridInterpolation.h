class punkt {
 public:
  double y; //Funktionswert des Punktes
  double y2ndth1; //2. Ableitung des Punktes in th1-Richtung
  double y2ndph1; //2. Ableitung des Punktes in ph1-Richtung
  double y2ndth2; //2. Ableitung des Punktes in th2-Richtung
  double y2ndph2; //2. Ableitung des Punktes in ph2-Richtung
  double y2ndT1;  //2. Ableitung des Punktes in  T1-Richtung
};

class gridinterpolation {
 private:
  double omega, q, gridT1min;
  double th1min;
  double th1stepsize;
  int th1steps;

  double ph1min;
  double ph1stepsize;
  int ph1steps;

  double th2min;
  double th2stepsize;
  int th2steps;

  double ph2min;
  double ph2stepsize;
  int ph2steps;

  double T1min;
  double T1stepsize;
  int T1steps;

  int getindex (int th1, int ph1, int th2, int ph2, int T1);

  punkt* CS;

 public:
  enum boundaryCond {NATURAL};

  //Default constructor
  gridinterpolation();

  double getomega() {return omega;};
  double getq() {return q;};
  double getGridT1min() {return gridT1min;};

  double getth1min() {return th1min;};
  double getth1max() {return th1min+th1stepsize*(th1steps-1);};

  double getph1min() {return ph1min;};
  double getph1max() {return ph1min+ph1stepsize*(ph1steps-1);};

  double getth2min() {return th2min;};
  double getth2max() {return th2min+th2stepsize*(th2steps-1);};

  double getph2min() {return ph2min;};
  double getph2max() {return ph2min+ph2stepsize*(ph2steps-1);};

  double getT1min() {return T1min;};
  double getT1max() {return T1min+T1stepsize*(T1steps-1);};
  double getT1stepsize() {return T1stepsize;};
  int getT1steps() {return T1steps;};

  void calculate2ndderivativeth1(boundaryCond boundary);
  void calculate2ndderivativeph1(boundaryCond boundary);
  void calculate2ndderivativeth2(boundaryCond boundary);
  void calculate2ndderivativeph2(boundaryCond boundary);
  void calculate2ndderivativeT1(boundaryCond boundary);

  double getCS(double th1, double ph1, double th2, double ph2, double T1);

  //Linear interpolation of the tabulated crosssections
  double linearinterpolation (double x[]);
  ~gridinterpolation() {
    delete[] CS;
  }
};
