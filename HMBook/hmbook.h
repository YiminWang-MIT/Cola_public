#ifndef _HMBOOK_H_
#define _HMBOOK_H_
#if defined(__linux__) && !defined(_POSIX_SOURCE)
#define _POSIX_SOURCE
#endif
#ifndef _STDIO_H_
#include <stdio.h>
#endif

#if defined(__cplusplus)
extern "C" {
#endif

  // #define FLOAT float
#define FLOAT double

extern FLOAT HMVersion;
  
/** @name Datastructure and Booking */ /*@{*/
/** Histogram data structure */
struct hist {
  /** Bins in x */           int    nx;
  /** Bins in y */           int    ny;
  /** Entries */             int    entries;
  /** Ranges */              FLOAT  minx, maxx, miny, maxy;
  /** Normalized integral */ FLOAT  *random;
  /** Name */                const char *name;
  /** Title  */              const char  *title;
  /** Label */               const char *labelx, *labely, *labelz;  
  /** Unit */                const char *unitx, *unity, *unitz;
  /** chained list */        struct hist *next;
  /** Normalization */       struct hist *divide;
  /** global scaling */      FLOAT  scale;
  /** nonequdistant bins */  FLOAT  *xvalues;
  /** data array */          FLOAT  *data;
  /** data square */         FLOAT  *datasqr;
  /** entries per bin */     FLOAT  *nbin;
  /** error array */         FLOAT  *error;
};

/** Histogram data type */
typedef struct hist HIST;

/** Anchor of chained list of histograms */
extern HIST *HMFirst;
/** End of chained list of histograms */
extern HIST *HMLast;

/** Book a 1-dimensional histogram.

  @param title  title of the histogram
  @param labelx label of the x axis
  @param labely label of the y axis
  @param unitx  unit of the x axis
  @param unity  unit of the y axis
  @param nx     number of bins
  @param minx   left border of range
  @param maxx   right border of range
  @return Pointer to the new allocated histogram structure
 */
HIST  *HMBook1       (const char *title, 
		      const char *labelx, const char *labely, 
		      const char *unitx, const char *unity,
		      int nx, FLOAT minx, FLOAT maxx);

/** Book a 2-dimendsional histogram.

  @param title  title of the histogram
  @param labelx label of the x axis
  @param labely label of the y axis
  @param labelz label of the z axis
  @param unitx  unit of the x axis
  @param unity  unit of the y axis
  @param unitz  unit of the z axis
  @param nx     number of x bins
  @param minx   left border of x range
  @param maxx   right border of x range
  @param ny     number of y bins
  @param miny   left border of y range
  @param maxy   right border of y range
  @return Pointer to the new allocated histogram structure
 */
HIST  *HMBook2	     (const char *title, 
		      const char *labelx,const char *labely,const char *labelz,
		      const char *unitx, const char *unity, const char *unitz,
		      int nx, FLOAT minx, FLOAT maxx,
		      int ny, FLOAT miny, FLOAT maxy);
/** Delete histogram*/
void  HMDelete       (HIST *id);
/** Return the parameter of call to HMBook* */
void  HMBookParams(HIST *id, int *nx, int *ny, FLOAT *minx, FLOAT *maxx, 
		  FLOAT *miny, FLOAT *maxy);
/** set arrays for nonequidistant binning */
void  HMSetArray(HIST *id, int n, FLOAT *x, FLOAT *y);
/*@}*/
HIST  *HMCounter     (char *title, char * unit);
void  HMCount        (HIST *id, FLOAT weight);
/** @name IO operations */ /*@{*/
/** Write a histogram to file */
void  HMWrite        (HIST *id, FILE *fp, char *name);
/** Read a histogram from file with filename*/
HIST *HMReadFilename(FILE *fp, char *filename);
/** Read a histogram from file */
HIST  *HMRead        (FILE *id);
/** Read a histogram from file WITHOUT adding to chain*/
HIST  *HMReadIsolated(FILE *id);
void  HMWriteSingleHistogram(char * filename, HIST *id);
/** Write all histograms to file*/
void  HMWriteAll     (char * filename);
/** Write all histograms uncompressed to pipe*/
void HMWriteAllPipe(char *command);
/** Open file and read just the header */
FILE *HMOpenHisFile(const char *filename);
/** Read all histograms from file*/
int   HMReadAll      (const char * filename);
/** Write histogram as ascii dump*/
void  HMWriteAscii(FILE *fp, HIST *id);
/** Write 2d histogram as ascii dump with format x1\ty1\nx2\ty2\t or 3d histogram in gnuplots "with image" style*/
void  HMWriteAsciiTuple(FILE *fp, HIST *id);
/** just read one histogram from file */
HIST  *HMScanFile    (const char *filename, char *histogram);
/** Find histogram by name, usualy name==title */
HIST  *HMFind        (char *name);
/** Find histogram by title */
HIST  *HMFindTitle   (char *name);
/*@}*//** @name Filling histograms*//*@{*/
/** Fill histogram*/
void  HMFill         (HIST *id, FLOAT x, FLOAT y, FLOAT weight);
/** Fill a 1d-histogram with a given function */
void  HMFillFunction1(HIST *id, FLOAT (*f)(FLOAT x));
/** Fill a 2d-histogram with a given function */
void  HMFillFunction2(HIST *id, FLOAT (*f)(FLOAT x, FLOAT y));
/** extract value at a given position*/
FLOAT HMExtract1     (HIST *id, FLOAT x);
/** extract error at a given position*/
FLOAT HMExtractError (HIST *id, FLOAT x);
/** extract value at a given position*/
FLOAT HMExtract2     (HIST *id, FLOAT x, FLOAT y);
/** extract value at a given position by linear interpolation*/
FLOAT HMInterpolate  (HIST *id, FLOAT x);
/** extract value at a given position by linear interpolation*/
  FLOAT HMInterpolate2D  (HIST *id, FLOAT x, FLOAT y);
/** extract error at a given position by linear interpolation*/
FLOAT HMInterpolateError  (HIST *id, FLOAT x);
/** Return contents of bin i */
FLOAT HMBinX         (HIST *id, int i);
/** Return contents of bin i */
FLOAT HMBinY         (HIST *id, int i);
/** Return dimension of histogram (1 or 2) */
int   HMDimension(HIST *id);
/** Return the minimum bin contents */
FLOAT HMMin          (HIST *id);
/** Return the maximum bin contents */
FLOAT HMMax          (HIST *id);
/** Book Error treatment for weighted histograms */
void HMBookWeightError(HIST *id);
/** Book Variance */
void HMBookVariance(HIST *id);
/** Book Error per bin */
void HMFillError(HIST *id);
/** Set error per bin to zero*/
void HMClearError(HIST *id);
/*@}*/
/** @name Plotting histograms*/ 
/*@{*/
/** Set a predifined style for plotting.

  @param style see hmplot.h for possible styles
*/
void  HMSetPredefinedStyle(int style);
/** Plot 1d-histogram as histogram */
void  HMHistogram    (HIST *id);
/** Plot 1d-histogram as datapoints with error bars */
void  HMDataError    (HIST *id);
/** Plot 1d-histogram as datapoints with error bars as defined in a seperate histogram*/
void  HMDataAndError (HIST *id, HIST *id2);
/** Plot 2d-histogram as gray scale picture  */
void  HMGray         (HIST *id);
/** Plot 2d-histogram as color scale picture  */
void  HMColor        (HIST *id);
/** Plot 2d-histogram as mean in x direction  */
void  HMPlotMeanX    (HIST *id);
/** Plot 2d-histogram as mean in y direction  */
void  HMPlotMeanY    (HIST *id);
/** Plot 2d-histogram as box picture  */
void  HMBox          (HIST *id);
/** Plot 2d-histogram as lego plot (manhattan plot)  */
void  HMLego         (HIST *id);
/** Plot 2d-histogram as surface  */
void  HMSurface      (HIST *id);
/** Plot 2d-histogram as scatter plot (Kaffeesatz-plot)  */
void  HMScatter      (HIST *id);
/** Plot 1d-histogram as line  */
void  HMFunction     (HIST *id);
/** Mark last histogram with a comment  */
void  HMMarkHist     (HIST *id, int where, char *txt);
/*@}*/
/** @name Arithmetics with histograms*/ 
/*@{*/
/** Perform an arithmetic operation with two histograms 
    @param oper one of '+','-','/','*'
    @return a new allocate histogram with the result
*/
HIST  *HMOperation   (HIST *id1, char oper,
                      HIST *id2, const char *title, 
		      const char *labz, const char *unitz);
/** Scale histogram */
void  HMSetScale     (HIST *id, FLOAT scale);
/** Normalize histogram by an other histogram. */
void  HMSetDivide    (HIST*id, HIST *divide);
/** Scalar operation on histogram */
HIST  *HMScalar      (HIST *id1, char oper, FLOAT scalar);
/** Smooth histogram by averaging over "bins" bins*/
HIST  *HMSmooth      (HIST *id, int bins);
/** Convolution of two histograms */
HIST *HMConvolute(HIST *id1, HIST *id2);
/** Deconvolution of two histograms */
HIST *HMDeconvolute(HIST *id1, HIST *id2);
/** Sum of n bins for output */
void  HMSetRebin     (int rebinfactor);
/** Return statistical information for this histogram */
void  HMInfo1D(HIST *id, FLOAT *Maximum, FLOAT *Maxposition,
	                FLOAT *Minimum, FLOAT *Minposition,
	                FLOAT *FWHM, FLOAT *FWHMposition,
	                FLOAT *RMS, FLOAT *Mean,
	                FLOAT *Integral);
/** Return integral over region defined by HMMinimum, HMMaximum.
    If pointers RMS and Mean or not NULL, output RMS and Mean for the region */
FLOAT HMIntegralRegion(HIST *id, FLOAT *RMS, FLOAT *Mean);
/*@}*/
/** @name Simple random generators */ 
/*@{*/
/** Generate random number according a deviation given by a 1d-histogram*/
FLOAT HMRandom1      (HIST *id);
/** Generate random number according a deviation given by a 2d-histogram*/
void  HMRandom2      (HIST *id, FLOAT *xreturn, FLOAT *yreturn);
/** normal distributed random number */
void  norran         (FLOAT *value);
/** uniform distributed random number */
FLOAT rndm           ();
/*@}*/
/** @name Least Square Fitting */
/*@{*/ 
/** least square fit of function to a set of data.

    @param ndata number of data points
    @param x array of data points
    @param y array of funtion values at data points
    @param erry error of funtion
    @param func function depending on x and parameterset
    @param parameter array of parameter to vary
    @param dim the dimension of the problem: number of parameter    
*/
FLOAT HMFitData      (int ndata, FLOAT x[], FLOAT y[], FLOAT erry[],
	              double (*func)(double x, double par[]), 
		      FLOAT parameter[], int dim);
/** Fit function to histogram. */
void  HMFit1(HIST *id, double (*function)(double [],double x),
	     double *par,int params);
/** Plot the result of the last fit */
void  HMPlotFit(double *par, double *kov, 
                double minx, double maxx, int points);
/** Parse function given as string */
int    HMScanFunction(char *func, double p[], int n);
/** that's the parsed function */
double HMFit1Function(double p[], double x);
/**  */
double HMParsedFunction(double x, double y);
/*@}*/
void   HMServer(FILE *in, FILE *out, int (* work)(void *));

#if defined(__cplusplus)
}
#endif
#endif










