/*                                                                      -*-c-*-
 * the guy to blame:
 *
 * Michael O. Distler                        mailto:distler@mitlns.mit.edu
 * MIT, Lab for Nuclear Science, 26-456      tel.   (617) 253-6997
 * Cambridge, MA 02139                       fax    (617) 258-5440
 *
 * $Header: /tmp/cvsroot/Cola/ColaIASA/include/iasa0.h,v 1.1.1.1 1998-08-25 23:57:09 distler Exp $
 *
 * simple iasa data structure
 *
 */

#ifndef __IASA_0_H__
#define __IASA_0_H__

struct iasa0 {
  int timestamp;
  float bias;
  struct {
    float v;
    float i;
  } filament;
  float ppul;
  float hv;
  float cur;
  float vac[2];
  float dos[3];
};

extern struct iasa0 iasa0;

#endif /* __iasa_0_H__ */
