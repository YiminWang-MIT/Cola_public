/*
 * pass1.h: Definition der Output - Konfiguration
 */

struct Focal {
  float x, th, y, ph;
};

struct Target {
  float dp, th, y0, ph, len;
};

struct Spec {
  struct Focal focal;
  struct Target target;
};

struct pass1 {
  struct Spec a, b;
};

extern struct pass1 pass1;
