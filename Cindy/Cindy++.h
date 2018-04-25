//								      -*-c++-*-
// Created 1993-96 by  Michael O. Distler,
//                     Institut fuer Kernphysik,
//                     Johannes Gutenberg-Universitaet Mainz
// Updated 1997 by     Michael O. Distler,
//                     M.I.T., Laboratory for Nuclear Science
//
// $Header: /tmp/cvsroot/Cola/Cindy/Cindy++.h,v 4.15 2003-08-05 09:36:29 distler Exp $
//

#ifndef __cindy_plus_plus__
#define __cindy_plus_plus__

/**@name Cindy++ Compiler and Library
 *
 * This document is a user's guide to the cindy++ compiler and
 * the cindy library, a library written in C++, used to decode
 * data taken in nuclear physics experiments, e.g. at MAMI
 * (Mainz Microtron, Germany) or MIT/Bates, USA. The library
 * constitutes an esential part of online and offline analysis
 * programs used at the mentioned laboratories.
 *
 * @memo     The cindy++ library
 * @version  4.8
 * @author   Michael O. Distler
 */

//@{
//@Include: cindy.dxx

//#include "Cindy/EventDispatch.h"
#include "EventDispatch.h"
#include <cstring>
#include <ctime>
#ifdef m68kph
#define time_t _G_time_t
#endif

const int verboseFlag		= 0x0001;
const int zipFlag		= 0x0002;
const int specialFlag		= 0x0004;
const int configFlag		= 0x0008;
const int identFlag		= 0x0010;
const int noDecodeFlag		= 0x0020;
const int debugFlag		= 0x0040;
const int outputFlag		= 0x0080;
const int updateHistoFlag	= 0x0100;
const int clearHistoFlag	= 0x0200;
const int gnutarFlag		= 0x0400;
const int countFlag		= 0x0800;
const int printExpFlag		= 0x1000;
const int ignoreExpFlag		= 0x2000;
const int noForkFlag		= 0x4000;

class CindyNode;
class CindyStream;
class CindyOutStream;

/**
 *
 */
class CindyItem {
    CindyNode *item;
public:
    /**
     *
     */
    CindyItem() { item = NULL; }
    /**
     *
     */
    CindyItem(CindyNode *node) { item = node; }
    /**
     *
     */
    void init(CindyNode *node) { item = node; }
    /**
     *
     */
    unsigned short ok();
    /**
     *
     */
    int operator()();		// falls item vom Integer-Typ ist,
				// liefert () den Wert.
    /**
     *
     */
    double scalar();		// ansonsten verwandelt scalar
				// den Wert in ein double
    /**
     *
     */
    int valid() { return (item != NULL); }
    /**
     *
     */
    int operator()(short *s);
    /**
     *
     */
    int operator()(unsigned short *us);
    /**
     *
     */
    int operator()(int *i);
    /**
     *
     */
    int operator()(unsigned int *ui);
    /**
     *
     */
    int operator()(long *l);
    /**
     *
     */
    int operator()(unsigned long *ul);
    /**
     *
     */
    int operator()(float *f);
    /**
     *
     */
    int operator()(double *d);
    /**
     *
     */
    ~CindyItem() { ; }
};

/**
 *
 */
class CindyBase : public EventDispatch {
protected:
  CindyNode	**tree, **base;
  int  		 *level, maxlevel, datasize;
  int             longestName, configSize, fastClear;
  unsigned short *ok_flags;
  char		 *dataroot, *configuration, *configDate;
public:
  /**
   *
   */
  enum	 item_type  { SHORT  = 1, INT    = 2, LONG  = 3,
		      USHORT = 4, UINT   = 5, ULONG = 6,
		      FLOAT  = 7, DOUBLE = 8, UNKNOWN = 9 };
  /**
   *
   */
  CindyBase(int);
  /**
   *
   */
  virtual ~CindyBase();
  /**
   *
   */
  void alloc(int);
  /**
   *
   */
  void alloc(int, int);

  /**Use this methode to add an analysis function to the configuration
   * tree. The location is specified by {\em node}. The name of the
   * experiment can be omited. In this case {\em node} has to start
   * with a period. During the unpacking of an event, the function
   * {\em fun} is called. The callback function {\em fun} will then
   * have a pointer to the raw data, the length of the data (number
   * of 16bit words), and the pointer to {\em data} as parameters.
   * Callback functions are used by the chamber library.
   * @memo schedule a callback function
   */
  void addCallback(const char *node, EvDpFun fun, char *data);

  /**This methode cancels the callback function {\em fun} that was
   * scheduled for the {\em node} within the configuration tree.
   * @memo Cancel a callback function
   */
  void cancelCallback(const char *, EvDpFun);
  /**
   *
   */
  void add(int, int, int, const char *);
  /**
   *
   */
  void add(int, int, int, const char *, short *);
  /**
   *
   */
  void add(int, int, int, const char *, unsigned short *);
  /**
   *
   */
  void add(int, int, int, const char *, int *);
  /**
   *
   */
  void add(int, int, int, const char *, unsigned int *);
  /**
   *
   */
  void add(int, int, int, const char *, long *);
  /**
   *
   */
  void add(int, int, int, const char *, unsigned long *);
  /**
   *
   */
  void add(int, int, int, const char *, float *);
  /**
   *
   */
  void add(int, int, int, const char *, double *);
  /**
   *
   */
  void *getroot() { return (void *) dataroot; };
  /**
   *
   */
  void setroot(char *, int, int);
  /**
   *
   */
  void setconf(char *cdate, char *conf, int size)
    { configDate = cdate; configuration = conf; configSize = size; }
  /**
   *
   */
  void clearData()
  { if (!fastClear) memset(dataroot,0,datasize); memset(ok_flags,0,datasize); }
  /**
   *
   */
  caddr_t   addrOf(const char *nam);
  /**
   *
   */
  caddr_t   addrOf(const char *nam,int dmsg);
  /**
   *
   */
  item_type typeOf(const char *nam,int dmsg=1);
  /**
   *
   */
  CindyItem item(const char *nam,int dmsg=1);
  /**
   *
   */
  unsigned short itemOK(short *ptr)
    { return ok_flags[((char *)ptr-dataroot)>>1]; }
  /**
   *
   */
  unsigned short itemOK(unsigned short *ptr)
    { return ok_flags[((char *)ptr-dataroot)>>1]; }
  /**
   *
   */
  unsigned short itemOK(int *ptr)
    { return ok_flags[((char *)ptr-dataroot)>>1]; }
  /**
   *
   */
  unsigned short itemOK(unsigned int *ptr)
    { return ok_flags[((char *)ptr-dataroot)>>1]; }
  /**
   *
   */
  unsigned short itemOK(long *ptr)
    { return ok_flags[((char *)ptr-dataroot)>>1]; }
  /**
   *
   */
  unsigned short itemOK(unsigned long *ptr)
    { return ok_flags[((char *)ptr-dataroot)>>1]; }
  /**
   *
   */
  unsigned short itemOK(float *ptr)
    { return ok_flags[((char *)ptr-dataroot)>>1]; }
  /**
   *
   */
  unsigned short itemOK(double *ptr)
    { return ok_flags[((char *)ptr-dataroot)>>1]; }
};

/**CindyTree is a container class that supplies an interface to
 * decode the experiment data.
 */
class CindyTree : public CindyBase {
public:
  /**
   *
   */
  enum	 histo_mode { CREATE_HISTO = 0, UPDATE_HISTO = 1, CLEAR_HISTO = 2 };
  /**
   *
   */
  CindyTree(int);
  /**
   *
   */
  virtual ~CindyTree();
  /**
   *
   */
  char  *input;
  /**
   *
   */
  char  *output;

  /**Currently, this methode is only used in the general {\em Decode}
   * program. The experiment configuration is extracted from the data
   * stream. Then the configuration is compiled and the tree that
   * represents the event structure is constructed. That way the
   * decoding of data is prepared.
   * @memo Extract and compile the event configuration
   */
  int	 nextArchiveUnpackAndCompile();

  /**Repeat the analysis of the last event in the data stream. The
   * data is unpacked again and all the scheduled callback functions
   * are executed. A return value not equal zero represents an error
   * condition, e.g. the end of data. Note that events which are
   * scheduled for output will not be written until the next call
   * of \Ref{CindyTree::analyseNextEvent}. Even if the event is
   * marked several times it will only be written once.
   */
  int	 analyseLastEvent();

  /**Analyse the next event in the data stream.
   *
   */
  int	 analyseNextEvent();
  /**
   *
   */
  int	 analyseGetOpt(int argc, char *argv[], CindyTree *cx=NULL);
  /**
   *
   */
  int	 decodeNextEvent();
  /**
   *
   */
  int	 decodeGetOpt(int, char **);
  /**
   *
   */
  unsigned short eventCount();
  /**
   *
   */
  unsigned short eventCode();
  /**
   *
   */
  time_t getTimeOfDataTaking();
  /**
   *
   */
  const	char *getInputFilename();
  /**
   *
   */
  const	char *getOutputFilename();
  /**
   *
   */
  const char *getExperimentName();
  /**
   *
   */
  const char *getRunName();
  /**
   *
   */
  int	 putOpt(int flag, char *in=NULL, char *out=NULL, char *run=NULL);
  /**
   *
   */
  int    closeIO();
  /**
   *
   */
  int	 putEventData();
  /**
   *
   */
  int	 packEventData(void *ptr, int len=1);
  /**
   *
   */
  long	 openShmKeyFile(long key);
  /**
   *
   */
  int	 checkDebugFlag() { return (flags & debugFlag); }
  /**
   *
   */
  histo_mode histoMode()  { return histomode; }
  /**
   *
   */
  int	rewind(int events=0);
  /**
   *
   */
  int	putPackedData();
  /**
   *
   */
  int	putPackedData(int count, int code);
  /**
   *
   */
  int   putIdent(time_t t, int cfg=0);
  /**
   *
   */
  int   putComment(const char * str);
  /**
   *
   */
  int   putOldIdent(const char * str, time_t t);
  /**
   *
   */
  int   putRunName(const char * runName);
private:
  int		  flags, dataReady;
  histo_mode	  histomode;
  CindyStream	 *in;
  CindyOutStream *out;
  CindyTree	 *ctx;
  void	analyseHelp(char *);
  void	decodeHelp(char *);
  int	dataIsReady() { return (dataReady != 0); }
  int	iStream(int events=0);
  int	oStream(char *run=NULL);
};

extern char *SharedMemoryKeyFile;
extern CindyTree *cindyTreeAlloc();
extern CindyTree *cindyTreeAlloc(void *datastructure);
extern CindyTree *cindyOutTreeAlloc();
extern CindyTree *cindyOutTreeAlloc(void *datastructure);

//@}

#endif
