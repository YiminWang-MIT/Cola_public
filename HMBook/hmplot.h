#if defined(__linux__) && !defined(_POSIX_SOURCE)
#define _POSIX_SOURCE
#endif
#include <stdio.h>

#if !defined(FLOAT)
# define FLOAT double
#endif

#define	SIZE_X			1
#define	SIZE_Y			2
#define	MRG_LEFT		3
#define	MRG_LOW			4
#define	MRG_TITLE		5
#define	WIN_X			6
#define	WIN_Y			7
#define	WIN_TITLE		8
#define	LAB_X			9
#define	LAB_Y			10
#define	AXI_X			11
#define	AXI_Y			12
#define	FONTSIZE_AXIS		13
#define	FONTSIZE_LABEL		14
#define	FONTSIZE_COMMENT       	15
#define	FONTSIZE_WINTITLE	16
#define	FONTSIZE_TITLE		17
#define ON_SCALE_X		18
#define ON_SCALE_Y		19
#define ON_SCALE_Z		20
#define LOG_SCALE_X             21
#define LOG_SCALE_Y             22
#define LOG_SCALE_Z             23
#define	USE_GHOSTVIEW       	24
#define TIGHT_GS                25
#define GS_RESOLUTION           26
#define NO_OPEN                 27
#define CENTER_SCALE_X          28
#define FILL_RED                29
#define FILL_GREEN              30
#define FILL_BLUE               31
#define FIT_ERROR               32
#define COLOR_LABEL             33
#define COLOR_WINTITLE          34
#define COLOR_TITLE             35
#define LINE_COLOR              36
#define LINE_WIDTH              37
#define LINE_STYLE              38
#define FILL_COLOR              39
#define DATA_MARK               40
#define FORMAT_AXIS_X           41
#define FORMAT_AXIS_Y           42
#define FORMAT_AXIS_Z           43
#define TEXT_ANGLE              44
#define SHADOW_EFFECT           45
#define DATA_SIZE               46
#define TIGHT                   47
#define FILL_ZERO               48
#define SCALE_TICKMARKS         49
#define LAB_Z                   50
#define DELAY_TICKS             51
#define LAST_OPTION             52

#define NOMINIMUM		32343.0

#define ST_A4          1
#define ST_LETTER      2
#define ST_OVERHEAD    3
#define ST_PSFIG       4
#define ST_PHYSREVLETT 5

#if defined(__cplusplus)
extern "C"
{
#endif

/** @name Plot file operations */
/*@{*/
/** Open plot file for output */
void	HMPlotFile	(const char *filename);
/** Close plot file */
void	HMClose		();
/** Set file pointer */
void    HMFilePointer   (FILE *);
/** Create a ghostscript pipe for output*/
FILE    *HMPipe		();
/** Flush plot file buffer */
void    HMFlush         ();
/** Start WXWindows histogram viewer*/
int     HMLaunchView    (int argc, char *argv[]);
/*@}*/

/** @name Plotting figures */
/*@{*/
/** Plot data points with error bars */
void	HMPlotError	(int nnx, FLOAT *x, FLOAT *y, FLOAT *xerr, FLOAT *yerr,
			 const char *title, const char *xlab, const char *ylab,
			 const char *xunit, const char *yunit);
/** Set additiona ticks on error bar, eg. to denote stat/sys error */
void    HMSetStatError(FLOAT *err);
/** Plot a line (function) */
void	HMPlotFunction	(int nnx, FLOAT *x, FLOAT *y, 
			const char *title, const char *xlab, const char *ylab,
			const char *xunit, const char *yunit);
/** Plot a box diagram */
void	HMPlotBox	(int nnx, int nny, FLOAT x0, FLOAT y0,
			 FLOAT dx, FLOAT dy, FLOAT *x,
			 const char *title, const char *xlab, const char *ylab,
			 const char *xunit, const char *yunit,const char type);
/** Plot a grayscale diagram */
void	HMPlotGray	(int nnx, int nny, FLOAT *x,
			 const char *title, const char *xlab, const char *ylab,
			 const char *xunit, const char *yunit);
/** PLot a scatter plot */
void	HMPlotScatter	(int entries,
			 int nnx, int nny, FLOAT *x,
			 const char *title, const char *xlab, const char *ylab,
			 const char *xunit, const char *yunit);
/** plot a three dimensional figure */
void	HMPlot3D	(int	nx, int ny, FLOAT *z, 
			const char *title,
			const char *xlab, const char *ylab, const char *zlab,
			const char *xunit, const char *yunit, 
			const char *zunit,
			int 	surfacetype);
/*@}*/

/** @name Setting options and page structure*/
/*@{*/
/** Set an option*/
void	HMSetOption	(int option, FLOAT value);
/** Return the value of an option*/
FLOAT	HMGetOption	(int option);
/** Set an option by name*/
void	HMSetOptionName	(const char *option, FLOAT value);
/** Return the value of an option by name*/
FLOAT	HMGetOptionName	(const char *option);
/** Divide apge into parts*/
void	HMZone		(int nrx, int nry, int nrnext);
/** Print next Histogram in same figure*/
void	HMSame		();
/** New page*/
void    HMNewPage       ();
/** Set minimum for x axis */
void	HMMinimumX	(FLOAT min);
/** Set minimum for y axis */
void	HMMinimumY	(FLOAT min);
/** Set minimum for z axis */
void	HMMinimumZ	(FLOAT min);
/** Set maximum for x axis */
void	HMMaximumX	(FLOAT max);
/** Set maximum for y axis */
void	HMMaximumY	(FLOAT max);
/** Set maximum for z axis */
void	HMMaximumZ	(FLOAT max);
/** Set color for filled area */
void    HMSetFillRGB(FLOAT r, FLOAT g, FLOAT b);
/*@}*/

/** @name Comments and titles */
/*@{*/
/** The title of the page*/
void	HMTitle		(const char *title);
/** Mark the last ploted histogram with a comment*/
void	HMExplain	(const char *comment);
/** Mark the last ploted histogram with a comment*/
void    HMExplainOffset (const char *comment,double offsetx, double offsety);
/** Print a comment*/
void    HMComment       (const char *comment, int line);
/** Print a text at an arbitrary position within the histogram/plot window*/
void    HMText          (FLOAT x, FLOAT y, const char *text);
/** Print a text at an arbitrary position on the whole page with given angle and align parameter*/
void    HMGlobalText    (FLOAT x, FLOAT y, FLOAT angle, FLOAT align ,const char *text);
/** Draw a line between two points*/
void    HMLine          (FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2);
/** Draw a rectangle given by lower left and upper right corner */
void    HMRectangle     (FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2);
/** Mark a line directly on the line*/
void    HMMarkFunction(int where,  FLOAT *x, FLOAT *y, const char *mark);
/** Include a postscript file */
void    HMIncludePS     (const char *filename, double x,double y, double zoom);
/*@}*/

FLOAT   HMXPixel        (FLOAT px);
FLOAT   HMYPixel        (FLOAT py);
#if defined(__cplusplus)
}
#endif















