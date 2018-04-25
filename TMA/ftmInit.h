//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mit.edu
// MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/TMA/ftmInit.h,v 1.3 1998-12-07 06:39:18 distler Exp $
//
// initialize the Fast Trace Machine
//

#ifndef __ftmInit_h__
#define __ftmInit_h__

#include <errno.h>
#include "ftmInitYACC.h"
#include "ftmInitLEX.h"

/**Scanner class that is used to read in matrix elements.
 */
class ftmInit : public ftmInitYACC {
  ///File containing the matrix elements.
  FILE *file;
  ///Scanner error variable.
  int yerrs;
  ///Lexical analyzer.
  ftmInitLEX * lex;
  ///Lexical analyzer main function.
  virtual int  yylex();
  ///Function is called in case of an error.
  virtual void yyerror(char * m);
  ///Extract offsets from scanner class.
  int getOffset(double **offs, Number *numbers);
public:
  /**Constructor. Scans the initfile and stores the
   * matrix element entries and the offsets.
   */
  ftmInit(const char * initfile);

  /**Destructor. Cleans up internal memory.
   */
  virtual ~ftmInit();

  /**Get the matrix elements from the parser. This function allocates
   * memory for the exponents and the matrix elements and copies the
   * entries to the memory. The return value is the number of entries.
   * In the case of an error, a default matrix is returned.
   * The user is responsible for deleting the memory after use.
   */
  int getEntries(int **ix, int **ith, int **iy, int **iph, double **dp,
		 double **th, double **y0, double **ph, double **len);
  ///Same thing as above. Returns a list of objects instead of vectors.
  int getEntries(ftmEntry **);

  /**Get the x offsets from the parser. This function allocates
   * memory for the offsets and copies the offsets to the memory.
   * The return value is the number of offsets.
   * The user is responsible for deleting the memory after use.
   */
  int getXoffset(double **offs);

  /**Get the theta offsets from the parser. This function allocates
   * memory for the offsets and copies the offsets to the memory.
   * The return value is the number of offsets.
   * The user is responsible for deleting the memory after use.
   */
  int getToffset(double **offs);

  /**Get the y offsets from the parser. This function allocates
   * memory for the offsets and copies the offsets to the memory.
   * The return value is the number of offsets.
   * The user is responsible for deleting the memory after use.
   */
  int getYoffset(double **offs);

  /**Get the phi offsets from the parser. This function allocates
   * memory for the offsets and copies the offsets to the memory.
   * The return value is the number of offsets.
   * The user is responsible for deleting the memory after use.
   */
  int getPoffset(double **offs);
};

#endif /* __ftmInit_h__ */
