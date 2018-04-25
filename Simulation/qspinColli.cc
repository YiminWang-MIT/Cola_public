#include "Colli.h"
#include <stdio.h>

OHIPS_qspin::OHIPS_qspin(double hor, double ver, double dist) {
  if ((hor==OHIPS_SHORT_HORIZONTAL) && (ver==OHIPS_SHORT_VERTICAL) &&
      (dist==OHIPS_SHORT_DISTANCE)) {
    name = "OHIPS++short";
  } else if ((hor==OHIPS_LONG_HORIZONTAL) && (ver==OHIPS_LONG_VERTICAL) &&
      (dist==OHIPS_LONG_DISTANCE)) {
    name = "OHIPS++long";
  } else sprintf(name = new char[21] ,"OHIPS++%.2fmsr", hor*ver*4000);

  verPlane = new colliPlane(dist, 0, dist*tan(ver));
  horPlane = new colliPlane(dist, dist*tan(hor), 0);
  init = 1;
}

int OHIPS_qspin::accepted(double x0[3], FourVector p,
			  double theta, double oop, double refmom)
{
  if (init) {
    init = 0;
    refMom = refMom;
    verPlane->init(theta, oop);
    horPlane->init(theta, oop);
  }
  return (verPlane->checkVertical(x0, p) && horPlane->checkHorizontal(x0, p));
}

OOPS_qspin::OOPS_qspin(double hor, double ver, double dist) {
  if ((hor==OOPS_SHORT_HORIZONTAL) && (ver==OOPS_SHORT_VERTICAL) &&
      (ver==OOPS_SHORT_DISTANCE)) {
    name = "OOPS++short";
  } else if ((hor==OOPS_LONG_HORIZONTAL) && (ver==OOPS_LONG_VERTICAL) &&
      (ver==OOPS_LONG_DISTANCE)) {
    name = "OOPS++long";
  } else sprintf(name = new char[21] ,"OOPS++%.2fmsr", hor*ver*4000);
  
  verPlane = new colliPlane(dist, 0, dist*tan(ver));
  horPlane = new colliPlane(dist, dist*tan(hor), 0);
  init = 1;
}

int OOPS_qspin::accepted(double x0[3], FourVector p,
			  double theta, double oop, double refmom)
{
  if (init) {
    init = 0;
    refMom = refMom;
    verPlane->init(theta, oop);
    horPlane->init(theta, oop);
  }
  return (verPlane->checkVertical(x0, p) && horPlane->checkHorizontal(x0, p));
}
