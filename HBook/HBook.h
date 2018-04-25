//								      -*-c++-*-
// Copyright (c) 1993 by   Michael O. Distler,
//                         Institut fuer Kernphysik,
//                         Johannes Gutenberg-Universitaet Mainz
//
// All rights reserved.
//
// $Header: /tmp/cvsroot/Cola/HBook/HBook.h,v 1.17 1997-08-13 22:24:00 distler Exp $
//

#ifndef __HBook__
#define __HBook__

///
class HBookBase {
protected:
    /// id of the last booked histogram
    static int lastID;
    /// number of histograms
    static int linkNr;
    /** memory allocation
      * > 0 :  size of common /PAWC/
      * < 0 :  size of global */
    static int limit;
    ///
    int id, dim, *bin, *nid, updateMode;
    ///
    float *val, *min, *max, weight;
    ///
    HBookBase(int Id, int Dim);
public:
    ///
    HBookBase(const char *inputfile, int pawc, const char *global=0);
    ///
    HBookBase(int pawc, const char *global=0);
    ///
    HBookBase& operator*=(const float factor);
    ///
    int scale(const float factor);	// scales by factor all values and
					/// errors in all bins of the histogram
    int write(const char *name, int Id = 0);
					// stores the histogram in a file
					// named 'name'. If Id=0, all
					/// histograms in memory are stored.
    int printTitle(int Id);
    ///
    int printContents(int Id, const char *pre, const char *post);
    ///
    ~HBookBase();
};

///
class HBook1D : public HBookBase {
public:
    ///
    HBook1D(const char *chtitl, const HBook1D &histo);
    ///
    HBook1D(int Id2, const char *chtitl, const HBook1D &histo);
    ///
    HBook1D(const char *chtitl, int nx, float xmi, float xma);
    ///
    HBook1D(int Id, const char *chtitl, int nx, float xmi, float xma);
    ///
    int inc(float x, float wgt=1.0);
    ///
    HBook1D& operator+=(const HBook1D &histo);
    ///
    HBook1D& operator-=(const HBook1D &histo);
    ///
    HBook1D& operator/=(const HBook1D &histo);
    ///
    ~HBook1D();
};

///
class HBook1Darray {
    ///
    HBook1D **p;
    ///
    int length;
public:
    ///
    HBook1Darray(int len, const char *chtitl, int nx, float xmi, float xma);
    ///
    ~HBook1Darray();
    ///
    HBook1D& operator[] (int i) const { return *p[i]; }
};

///
class HBook2D : public HBookBase {
public:
    ///
    HBook2D(const char *chtitl, const HBook2D &histo);
    ///
    HBook2D(int Id2, const char *chtitl, const HBook2D &histo);
    ///
    HBook2D(const char *chtitl, int nx, float xmi, float xma,
	    int ny, float ymi, float yma);
    ///
    HBook2D(int Id, const char *chtitl, int nx, float xmi, float xma,
	    int ny, float ymi, float yma);
    ///
    int inc(float x, float y, float wgt=1.0);
    ///
    HBook2D& operator+=(const HBook2D &histo);
    ///
    HBook2D& operator-=(const HBook2D &histo);
    ///
    HBook2D& operator/=(const HBook2D &histo);
    ///
    ~HBook2D();
};

///
class HBook2Darray {
    ///
    HBook2D **p;
    ///
    int length;
public:
    ///
    HBook2Darray(int len, const char *chtitl,
		 int nx, float xmi, float xma, int ny, float ymi, float yma);
    ///
    ~HBook2Darray();
    ///
    HBook2D& operator[] (int i) const { return *p[i]; }
};

///
class HBookN : public HBookBase {
    ///
    int init(int Id, const char *title, const char *name, int ncol, const char **chtags);
public:
    ///
    HBookN(const char *title, const char *name, int ncol, const char **chtags);
    ///
    HBookN(int Id, const char *title, const char *name, int ncol, const char **chtags);
    ///
    int fill(float *xtuple);
    ///
    ~HBookN();
};

//
//	for compatibility only
//	class HBook and class Ntuple may be removed in future releases
//	use HBook1D, HBook2D, HBookN instead
///

class HBook {
  ///
  enum Dim { OneDim, TwoDim, noDim };
  ///
  Dim dim;
  ///
  int id, *nid, chan, chan2;
  ///
  float min, max, min2, max2;
public:
  ///
  HBook(int pawc, int Id = 0);
  ///
  HBook(const char *name, int  ch, float  Min, float  Max, int Id = 0);
  ///
  HBook(const char *name, int xch, float xMin, float xMax,
		    int ych, float yMin, float yMax, int Id = 0);
  ///
  int inc(float val);
  ///
  int inc(float xval, float yval);
  ///
  int incw(float val, float wgt);
  ///
  int incw(float xval, float yval, float wgt);
  ///
  int write(const char *name);
};

///
class NTuple {
  ///
  int id, ncolum;
public:
  ///
  NTuple(int pawc, int ntID = 0);
  ///
  NTuple(const char *title, const char *name, int ncol, const char **chtags);
  ///
  int book(float *xtuple);
  ///
  ~NTuple();
};

#endif
