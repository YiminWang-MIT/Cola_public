/*								        -*-c-*-
 * AQUA: data AcQUisition for A1 experiments
 *
 * Copyright (c) 2003
 *
 * Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
 * 55099 Mainz, Germany				fax  +49 6131 39-22964
 *
 * $Id: abcn.h 2682 2016-12-02 11:58:53Z aesser $
 */

#define uword32 unsigned int
#define uword16 unsigned short

#include "a.h"
#include "b.h"
#include "c.h"
#include "d.h"
#include "ndet.h"
#include "hadron3.h"
#include "tofa1.h"
#include "npolraw.h"
#include "silicon.h"
#include "spekk.h"
#include "parity.h"
#include "trb.h"

struct abcn {
  struct MpsDataA	a;
  struct MpsDataB	b;
  struct MpsDataC	c;
  struct MpsDataN	ndet;
  struct hadron3	hadron3;
  struct TOFA1_t	tof;
  struct Rnpolraw	npol;
  struct DataD		d;
  struct Silicon_t      si;
  struct MpsDataKAOS	kaos;
  struct MpsDataParity  parity;
  struct DataNdetTrb    n;
};
