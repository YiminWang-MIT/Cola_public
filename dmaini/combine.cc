//								      -*-c++-*-
// Copyright (c) 2002
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
// $Id: combine.cc 2216 2008-06-13 21:13:47Z distler $
//

#include <iostream>
#include <iomanip>
#include <iosfwd>
#include <fstream>

class Element {
  static int maxpower;
  int x, t, y, p;
  float dp, th, y0, ph, len;
  Element *next;
  int meminit();
  Element * find(int X, int T, int Y, int P);
  Element(int X, int T, int Y, int P);
public:
  Element();
  ~Element();
  void insert(int X, int T, int Y, int P, int column, float val);
  void print();
};

int
Element::meminit()
{
  x = t = y = p = 0;
  dp = th = y0 = ph = len = 0.0;
  next = NULL;
  return 0;
}

Element *
Element::find(int X, int T, int Y, int P)
{
  if (X+T+Y+P == 0) return this;

  Element * prev = this;
  Element * elem = next;

  if (maxpower<=X) maxpower = X+1;
  if (maxpower<=T) maxpower = T+1;
  if (maxpower<=Y) maxpower = Y+1;
  if (maxpower<=P) maxpower = P+1;

  while (elem != NULL) {
    int idx = P + maxpower * (Y + maxpower * (T + maxpower * X));
    int eidx = elem->p +
      maxpower * (elem->y + maxpower * (elem->t + maxpower * elem->x));

    if (idx == eidx) return elem;

    if (idx < eidx) {
      prev->next = new Element(X, T, Y, P);
      prev->next->next = elem;
      return prev->next;
    }

    prev = elem;
    elem = elem->next;
  }

  if (elem == NULL) prev->next = elem = new Element(X, T, Y, P);

  return elem;
}

Element::Element(int X, int T, int Y, int P)
{
  meminit();
  x = X; t = T; y = Y; p = P;
}

Element::Element()
{
  meminit();
}

Element::~Element()
{
  delete next;
}

void
Element::insert(int X, int T, int Y, int P, int column, float val)
{
  Element * e = find(X, T, Y, P);
  switch (column) {
  case 1:  e->dp  = val; break;
  case 2:  e->th  = val; break;
  case 3:  e->y0  = val; break;
  case 4:  e->ph  = val; break;
  case 5:  e->len = val; break;
  }
}

void
Element::print()
{
  Element* e = this;

  while (e) {
    std::cout.setf(std::ios::scientific, std::ios::floatfield);
    std::cout << e->x << ' '
	 << e->t << ' '
	 << e->y << ' '
	 << e->p << std::setw(15)
	 << e->dp << std::setw(14)
	 << e->th << std::setw(14)
	 << e->y0 << std::setw(14)
	 << e->ph << std::setw(14)
	 << e->len << std::endl;
    e = e->next;
  }
}

int Element::maxpower = 1;

void
usage(char *prog)
{
  std::cerr << "Program to combine mx-files produced by dmaini." << std::endl;
  std::cerr << "Usage:" << std::endl;
  std::cerr << '\t' << prog << " [delta] [theta] [y] [phi] [len]" << std::endl;
  std::cerr << '\t' << "(use a single '-' if any mx.file does not exist)"
	    << std::endl;
}

int
main(int argc, char *argv[])
{
  if (argc != 6) {
    usage(argv[0]);
    return 0;
  }

  Element first;
  std::ifstream input;
  int i;
  for (i=1; i<6; i++) {
    char buffer[1024];
    int lines;
    // std::cout << "opening : " << argv[i] << std::endl;
    input.open(argv[i]);
    if (!input) continue;
    input.get(buffer, 1024, '\n');
    input >> lines;
    while ((lines>0) && !input.eof()) {
      float elem;
      char c;
      int x, t, y, p;
      input >> elem >> c >> x >> t >> y >> p;
      // std::cout << x << ' ' << t << ' ' << y << ' ' << p << ' ' << elem << std::endl;
      first.insert(x, t, y, p, i, elem);
      input.get(buffer, 1024, '\n');
      lines--;
    }
    input.close();
  }
  first.print();

  return 0;
}
