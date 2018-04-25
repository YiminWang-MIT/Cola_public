#ifndef __EventDispatch
#define __EventDispatch
/*
 *	@(#)EventDispatch.h	1.2	KPH	94/06/15	94/06/15   (h.k)
 *
 */

#define _DEFAULT_SOURCE 1
#include <sys/types.h>

#ifndef NULL
#define NULL (0)
#endif

typedef int (* EvDpFun)(unsigned short *,int,char *);

/**A pure virtual base class. Defines methods that are common to
 * all classes that unpack event data and make the data available
 * for other objects.
 */
class EventDispatch {
public:
  virtual ~EventDispatch() { ; }
  /**Add a callback function.
   * @param name specifies the location where to place the callback function.
   * @param fun  pointer to the callback function. The type is defined as:
   *             {\tt typedef int (* EvDpFun)(unsigned short *,int,char *);}
   *             Wenn the callback function is called, the data, the number
   *             of short words, and the additional data is passed to it.
   * @param init additional data used by the callback function. Free format.
   */
  virtual void  addCallback(const char *name, EvDpFun fun, char *init) = 0;

  /**Function returns true, if a specific item was part of the event. The
   * parameter is the address of the item within the event structure.
   */
  virtual unsigned short itemOK(short *)  = 0;

  /**Function returns true, if a specific item was part of the event. The
   * parameter is the address of the item within the event structure.
   */
  virtual unsigned short itemOK(long *)   = 0;

  /**Function returns true, if a specific item was part of the event. The
   * parameter is the address of the item within the event structure.
   */
  virtual unsigned short itemOK(float *)  = 0;

  /**Function returns true, if a specific item was part of the event. The
   * parameter is the address of the item within the event structure.
   */
  virtual unsigned short itemOK(double *) = 0;

  /**Returns the address of an item within the event structure.
   * Parameter is the name of the item.
   */
  virtual caddr_t  addrOf (const char *)  = 0;
};

#endif
