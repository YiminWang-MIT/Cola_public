//
// Copyright (c) 1993 by   Michael O. Distler,
//                         Institut fuer Kernphysik,
//                         Johannes Gutenberg-Universitaet Mainz
//
// All rights reserved.
//
// $Header: /tmp/cvsroot/Cola/HBook/HBook.cc,v 1.23 2006-11-28 18:39:55 cola Exp $
//

extern "C" int MAIN__() { return 0;}; // bug in libf2c

static const char rcsid[] =
 "@(#)$Id: HBook.cc 2216 2008-06-13 21:13:47Z distler $";

#include <cstdio>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include "HBook.h"

extern "C" {
void hbook1_(int *, const char *, int *, float *, float *, float *, long);
void hbook2_(int *, const char *, int *, float *, float *,
	     int *, float *, float *, float *, long);
void hbookn_(int *, const char *, int *, const char *, int *, const char *, long , long , long);
void hcopy_(const int *, int *, const char *, long);
void hdelet_(int *);
long hexist_(int *);
int  hfreem_(long *);
void hff1_(int *, int **, float *, float *);
void hff2_(int *, int **, float *, float *, float *);
void hfn_(int *, float *);
void hgive_(int *, const char *, int *, float *, float *,
	     int *, float *, float *, int *, int *, long);
void hrin_(int *, int *, int *);
void hlimap_(int *, const char *, long);
void hlimit_(int *);
void hopera_(int *, const char *, const int *, int *, float *, float *, long);
void hrend_(const char *, long);
void hropen_(int *, const char *, const char *, const char *, int *, int *, long , long , long);
void hrout_(int *, int *, const char *, long);
void hunpak_(int *, float *, const char *, int *, long);
void hunpke_(int *, float *, const char *, int *, long);
};

int HBookBase::limit	= 10000;
int HBookBase::lastID	= -1;
int HBookBase::linkNr	= 0;
static float vmx	= 0.0;
static float Eins	=  1.0;

HBookBase::HBookBase(const char *inputfile, int pawc, const char *global)
{
    if (lastID >= 0) return;   // HBookBase wurde zum zweiten Mal initialisiert
    if (pawc > limit) limit = pawc;
    if (global != NULL)
	hlimap_(&limit, global, strlen(global));
    else
	hlimit_(&limit);
    lastID = 0;

    static char *chtop   = "HISTO";
    static int   lun     = 2;
    static int   recl    = 1024;
    static int   histoID = 0;
    static int   icycle	 = 0;
    static int   iofset	 = 0;
    static int	 istat;

    hropen_(&lun, chtop, inputfile, " ", &recl, &istat,
	    strlen(chtop), strlen(inputfile), 1L);
    if (!istat) {
	hrin_(&histoID, &icycle, &iofset);
	hrend_(chtop, strlen(chtop));
    }
}

HBookBase::HBookBase(int pawc, const char *global)
{
    if (lastID >= 0) return;   // HBookBase wurde zum zweiten Mal initialisiert
    if (pawc > limit) limit = pawc;
    if (global != NULL)
	hlimap_(&limit, global, strlen(global));
    else
	hlimit_(&limit);
    lastID = 0;
}

HBookBase::HBookBase(int Id, int Dim)
{
    if (lastID < 0)		// HBookBase Initialisierung wurde vergessen
	hlimit_(&limit);	// PAWC wird mit default angelegt

    if (Id > 0) {		// Id wurde angegeben
	id = Id;
	if (lastID < Id) lastID = Id;
    }
    else			// Id wird automatisch durch Inkrement erzeugt
	id = ++lastID;

    dim = Dim;
    if (dim > 0) {
	bin = new int[dim];
	val = new float[dim];
	min = new float[dim];
	max = new float[dim];
    }
    else {
	bin = NULL;
	val = min = max = NULL;
    }

    updateMode = 0;
    if (hexist_(&id) & 0x1) {
	char title[81];
	int nx, ny, nwt, loc;
	float xmi, xma, ymi, yma;

	memset(title,0,sizeof(title));
	hgive_(&id, title, &nx, &xmi, &xma, &ny, &ymi, &yma, &nwt, &loc, 80L);
	// hgive fuellt den title - String mit Blanks auf
	// std::cerr << "title[" << nwt << "] '" << title << "'" << std::endl;

	switch (dim) {
	case 2:
	    bin[1] = ny;
	    min[1] = ymi;
	    max[1] = yma;
	case 1:
	    bin[0] = nx;
	    min[0] = xmi;
	    max[0] = xma;
	default:
	    updateMode = 1;
	    break;
	}
    }		

    nid = NULL;
    linkNr++;		// Anzahl der vorhanden Histogramme
}

HBookBase& 
HBookBase::operator*=(const float factor)
{
    if (!hexist_(&id)) return *this;

    val[0] = factor;
    hopera_(&id, "+E", &id, &id, val, &vmx, 2L);

    return *this;
}

int
HBookBase::scale(float factor)
{
    if (!hexist_(&id)) return -1;

    val[0] = factor;
    hopera_(&id, "+E", &id, &id, val, &vmx, 2L);

    return 0;
}

int
HBookBase::write(const char *name, int Id)
{
    static char *chtop   = "HISTO";
    static int   lun     = 2;
    static int   recl    = 1024;
    static int   histoID = Id;
    static int   istat, icycle;

    hropen_(&lun, chtop, name, "N", &recl, &istat,
	    strlen(chtop), strlen(name), 1L);
    hrout_(&histoID, &icycle, " ", 1L);
    hrend_(chtop, strlen(chtop));

    return 0;
}

int
HBookBase::printTitle(int Id)
{
    if (!(hexist_(&Id) & 0x1))
	return -1;

    char title[81];
    int nx, ny, nwt, loc;
    float xmi, xma, ymi, yma;

    memset(title,0,sizeof(title));
    hgive_(&Id, title, &nx, &xmi, &xma, &ny, &ymi, &yma, &nwt, &loc, 80L);

    int i=80;
    do { title[i--] = 0; } while ((i>=0) && (title[i]==' '));

    std::cout << title << std::endl;
    
    return 0;
}

int
HBookBase::printContents(int Id, const char *pre, const char *post)
{
    if (!(hexist_(&Id) & 0x1))
	return -1;

    char title[81];
    int nx, ny, nwt, loc, Nx, Ny;
    float xmi, xma, ymi, yma, xbin, ybin, *con, *err;
    static int eins = 1;

    memset(title,0,sizeof(title));
    hgive_(&Id, title, &nx, &xmi, &xma, &ny, &ymi, &yma, &nwt, &loc, 80L);

    xbin = (xma-xmi)/(float)nx;
    xmi += 0.5 * xbin;
    if (ny==0) {
	// 1d case
	con = new float[nx];
	err = new float[nx];
	hunpak_(&Id, con, " ", &eins, 1L);
	hunpke_(&Id, err, " ", &eins, 1L);
	for (Nx=0; Nx<nx; Nx++) {
	    if (pre) std::cout << pre;
	    std::cout << xmi+xbin*Nx << "\t0.0\t"
		      << con[Nx] << '\t' << err[Nx];
	    if (post) std::cout << post;
	    std::cout << std::endl;
	}
    }
    else {
	// 2d case
	ybin = (yma-ymi)/(float)ny;
	ymi += 0.5 * ybin;
	con = new float[nx*ny];
	err = new float[nx*ny];
	hunpak_(&Id, con, "HIST", &eins, 4L);
	hunpke_(&Id, err, "HIST", &eins, 4L);
	for (Nx=0; Nx<nx; Nx++)
	  for (Ny=0; Ny<ny; Ny++) {
	      if (pre) std::cout << pre;
	      std::cout << xmi+xbin*Nx << '\t' << ymi+ybin*Ny << '\t'
			<< con[Ny*nx+Nx] << '\t' << err[Ny*nx+Nx];
	      if (post) std::cout << post;
	      std::cout << std::endl;
	  }
    }

    return 0;
}

HBookBase::~HBookBase()
{
    if (val) delete val;
    if (bin) delete bin;
    if (min) delete min;
    if (max) delete max;

    if (linkNr>0) {
	linkNr--;
    }
    else {
	if (limit < 0) {
	    hfreem_(NULL);
	    limit=0;
	}
    }
}

//
//	HBook1D
//

HBook1D::HBook1D(const char *chtitl, const HBook1D &histo) : HBookBase(0,1)
{
    if (!updateMode) {
	bin[0] = histo.bin[0];
	min[0] = histo.min[0];
	max[0] = histo.max[0];
	hcopy_(&histo.id, &id, chtitl, strlen(chtitl));
    }
}

HBook1D::HBook1D(int Id2, const char *chtitl, const HBook1D &histo)
    : HBookBase(Id2,1)
{
    if (!updateMode) {
	bin[0] = histo.bin[0];
	min[0] = histo.min[0];
	max[0] = histo.max[0];
	hcopy_(&histo.id, &id, chtitl, strlen(chtitl));
    }
}

HBook1D::HBook1D(const char *chtitl, int nx, float xmi, float xma) : HBookBase(0,1)
{
    if (updateMode) {
	if ((dim != 1) || (bin[0] != nx) ||
	    (min[0] != xmi) || (max[0] != xma)) {
	    std::cerr << "Histogram '" << chtitl
		      << "'was declared with inconsistent parameters."
		      << std::endl;
	    exit(1);
	}
    }
    else {
	bin[0] = nx;
	min[0] = xmi;
	max[0] = xma;
	hbook1_(&id, chtitl, bin, min, max, &vmx, strlen(chtitl));
    }
}

HBook1D::HBook1D(int Id, const char *chtitl, int nx, float xmi, float xma)
    : HBookBase(Id,1)
{
    if (updateMode) {
	if ((dim != 1) || (bin[0] != nx) ||
	    (min[0] != xmi) || (max[0] != xma)) {
	    std::cerr << "Histogram '" << chtitl
		      << "'was declared with inconsistent parameters."
		      << std::endl;
	    exit(1);
	}
    }
    else {
	bin[0] = nx;
	min[0] = xmi;
	max[0] = xma;
	hbook1_(&id, chtitl, bin, min, max, &vmx, strlen(chtitl));
    }
}

int
HBook1D::inc(float x, float wgt)
{
    val[0] = x; weight = wgt;
    hff1_(&id, &nid, val, &weight);
    return 0;
}

HBook1D&
HBook1D::operator+=(const HBook1D &histo)
{
    if (bin[0] != histo.bin[0]) return *this;
    hopera_(&id, "+E", &histo.id, &id, &Eins, &Eins, 2L);
    return *this;
}

HBook1D&
HBook1D::operator-=(const HBook1D &histo)
{
    if (bin[0] != histo.bin[0]) return *this;
    hopera_(&id, "-E", &histo.id, &id, &Eins, &Eins, 2L);
    return *this;
}

HBook1D&
HBook1D::operator/=(const HBook1D &histo)
{
    if (bin[0] != histo.bin[0]) return *this;
    hopera_(&id, "/E", &histo.id, &id, &Eins, &Eins, 2L);
    return *this;
}

HBook1D::~HBook1D()
{
    if (id != 0) hdelet_(&id);
}

//
//	HBook1Darray
//

HBook1Darray::HBook1Darray(int len, const char *chtitl,
			   int nx, float xmi, float xma)
{
  int i;
  length = len;
  p = new HBook1D*[len];

  for (i=0; i<len ; i++) {
    char *str = new char[strlen(chtitl)+16];
    sprintf(str, "%s.%d", chtitl, i);
    p[i] = new HBook1D(str, nx, xmi, xma);
    delete str;
  }
}

HBook1Darray::~HBook1Darray()
{
    while (length--) delete p[length];
    delete p;
}
	
//
//	HBook2D
//

HBook2D::HBook2D(const char *chtitl, const HBook2D &histo) : HBookBase(0,2)
{
    if (!updateMode) {
	bin[0] = histo.bin[0]; bin[1] = histo.bin[1];
	min[0] = histo.min[0]; min[1] = histo.min[1];
	max[0] = histo.max[0]; max[1] = histo.max[1];
	hcopy_(&histo.id, &id, chtitl, strlen(chtitl));
    }
}

HBook2D::HBook2D(int Id2, const char *chtitl, const HBook2D &histo)
    : HBookBase(Id2,2)
{
    if (!updateMode) {
	bin[0] = histo.bin[0]; bin[1] = histo.bin[1];
	min[0] = histo.min[0]; min[1] = histo.min[1];
	max[0] = histo.max[0]; max[1] = histo.max[1];
	hcopy_(&histo.id, &id, chtitl, strlen(chtitl));
    }
}

HBook2D::HBook2D(const char *chtitl, int nx, float xmi, float xma,
		 int ny, float ymi, float yma) : HBookBase(0,2)
{
    if (updateMode) {
	if ((dim != 2) ||
	    (bin[0] != nx) || (min[0] != xmi) || (max[0] != xma) ||
	    (bin[1] != ny) || (min[1] != ymi) || (max[1] != yma)) {
	    std::cerr << "Histogram '" << chtitl
		      << "'was declared with inconsistent parameters."
		      << std::endl;
	    exit(1);
	}
    }
    else {
	bin[0] = nx;  bin[1] = ny;
	min[0] = xmi; min[1] = ymi;
	max[0] = xma; max[1] = yma;
	hbook2_(&id, chtitl, bin, min, max,
		bin+1, min+1, max+1, &vmx, strlen(chtitl));
    }
}

HBook2D::HBook2D(int Id, const char *chtitl, int nx, float xmi, float xma,
		 int ny, float ymi, float yma) : HBookBase(Id,2)
{
    if (updateMode) {
	if ((dim != 2) ||
	    (bin[0] != nx) || (min[0] != xmi) || (max[0] != xma) ||
	    (bin[1] != ny) || (min[1] != ymi) || (max[1] != yma)) {
	    std::cerr << "Histogram '" << chtitl
		      << "'was declared with inconsistent parameters."
		      << std::endl;
	    exit(1);
	}
    }
    else {
	bin[0] = nx;  bin[1] = ny;
	min[0] = xmi; min[1] = ymi;
	max[0] = xma; max[1] = yma;
	hbook2_(&id, chtitl, bin, min, max,
		bin+1, min+1, max+1, &vmx, strlen(chtitl));
    }
}

int
HBook2D::inc(float x, float y, float wgt)
{
    val[0] = x; val[1] = y; weight = wgt;
    hff2_(&id, &nid, val, val+1, &weight);
    return 0;
}

HBook2D&
HBook2D::operator+=(const HBook2D &histo)
{
    if (bin[0] != histo.bin[0]) return *this;
    hopera_(&id, "+E", &histo.id, &id, &Eins, &Eins, 2L);
    return *this;
}

HBook2D&
HBook2D::operator-=(const HBook2D &histo)
{
    if (bin[0] != histo.bin[0]) return *this;
    hopera_(&id, "-E", &histo.id, &id, &Eins, &Eins, 2L);
    return *this;
}

HBook2D&
HBook2D::operator/=(const HBook2D &histo)
{
    if (bin[0] != histo.bin[0]) return *this;
    hopera_(&id, "/E", &histo.id, &id, &Eins, &Eins, 2L);
    return *this;
}

HBook2D::~HBook2D()
{
    if (id != 0) hdelet_(&id);
}

//
//	HBook2Darray
//

HBook2Darray::HBook2Darray(int len, const char *chtitl,
			   int nx, float xmi, float xma,
			   int ny, float ymi, float yma)
{
  int i;
  length = len;
  p = new HBook2D*[len];

  for (i=0; i<len ; i++) {
    char *str = new char[strlen(chtitl)+16];
    sprintf(str, "%s.%d", chtitl, i);
    p[i] = new HBook2D(str, nx, xmi, xma, ny, ymi, yma);
    delete str;
  }
}

HBook2Darray::~HBook2Darray()
{
    while (length--) delete p[length];
    delete p;
}

//
//	HBookN
//

HBookN::HBookN(const char *, const char *, int, const char **)
    : HBookBase(0,0)
{
}

HBookN::HBookN(int Id, const char *, const char *, int, const char **)
    : HBookBase(Id,0)
{
}

int
HBookN::fill(float *)
{
    return 0;
}

int
HBookN::init(int, const char *, const char *, int, const char **)
{
    return 0;
}

HBookN::~HBookN()
{
}

//
//	for compatibility only
//	class HBook and class Ntuple may be removed in future releases
//	use HBook1D, HBook2D, HBookN instead
//

static int IDtop = -1;

HBook::HBook(int pawc, int Id)
{
  static int limit;

  limit = pawc;
  hlimit_(&limit);
  IDtop = (Id>0) ? Id-1 : 0;
  dim = noDim;
}

HBook::HBook(const char *name, int ch, float Min, float Max, int Id)
{
  dim = OneDim;
  id  = (Id>IDtop) ? (IDtop=Id) : ++IDtop;
  chan = ch; min = Min; max = Max;
  hbook1_(&id, name, &chan, &min, &max, &vmx, strlen(name));
}

HBook::HBook(const char *name, int xch, float xMin, float xMax,
	     int ych, float yMin, float yMax, int Id)
{
  dim = TwoDim;
  id  = (Id>IDtop) ? (IDtop=Id) : ++IDtop;
  chan  = xch; min  = xMin; max  = xMax;
  chan2 = ych; min2 = yMin; max2 = yMax;
  hbook2_(&id, name, &chan, &min, &max,
	  &chan2, &min2, &max2, &vmx, strlen(name));
}

int
HBook::inc(float val)
{
  if (dim != OneDim) return -1;

  static float value;
  value = val;
  hff1_(&id, &nid, &value, &Eins);
  
  return 0;
}

int
HBook::incw(float val, float wgt)
{
  if (dim != OneDim) return -1;

  static float value, weight;
  value = val;
  weight = wgt;
  hff1_(&id, &nid, &value, &weight);
  
  return 0;
}

int
HBook::inc(float xval, float yval)
{
  if (dim != TwoDim) return -1;

  static float xvalue, yvalue;
  xvalue = xval;
  yvalue = yval;
  hff2_(&id, &nid, &xvalue, &yvalue, &Eins);
  
  return 0;
}

int
HBook::incw(float xval, float yval, float wgt)
{
  if (dim != TwoDim) return -1;

  static float xvalue, yvalue, weight;
  xvalue = xval;
  yvalue = yval;
  weight = wgt;
  hff2_(&id, &nid, &xvalue, &yvalue, &weight);

  return 0;
}

int
HBook::write(const char *name)
{
  static char *chtop   = "HISTO";
  static int   lun     = 2;
  static int   recl    = 1024;
  static int   histoID = 0;
  static int   istat, icycle;

  hropen_(&lun, chtop, name, "N", &recl, &istat,
	  strlen(chtop), strlen(name), 1L);
  hrout_(&histoID, &icycle, " ", 1L);
  hrend_(chtop, strlen(chtop));
  
  return 0;
}

NTuple::NTuple(int pawc, int ntID)
{
  static int limit;

  limit = pawc;
  hlimit_(&limit);
  IDtop = (ntID>0) ? ntID-1 : 0;
}

NTuple::NTuple(const char *title, const char *name, int ncol, const char **chtags)
{
  static char *chtop   = "NTUPLE";
  static int   lun     = 2;
  static int   recl    = 1024;
  static int   nprime  = 8192;
  static int   istat, i;
  static long  lmax;
  static char *tags;

  id = ++IDtop;
  ncolum = ncol;
  hropen_(&lun, chtop, name, "N", &recl, &istat,
	  strlen(chtop), strlen(name), 1L);
  for (i=0; i<ncol; i++)
    if (lmax < (long) strlen(chtags[i]))
      lmax = strlen(chtags[i]);
  if (lmax>8) lmax = 8;
  tags = new char[ncol*lmax];
  memset(tags, ' ', ncol*lmax);
  for (i=0;i<ncol;i++) strncpy(tags+i*lmax, chtags[i], lmax);
  hbookn_(&id, title, &ncolum, chtop, &nprime, tags,
	  strlen(title), strlen(chtop), lmax);
}

int
NTuple::book(float *xtuple)
{
  hfn_(&id, xtuple);

  return 0;
}

NTuple::~NTuple()
{
  static char *chtop   = "NTUPLE";
  static int   icycle;
  static int   lun     = 0;

  hrout_(&lun, &icycle, " ", 1L);
  hrend_(chtop, strlen(chtop));
}
