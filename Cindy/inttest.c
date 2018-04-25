/*
 * Created 1994-95 by	Michael O. Distler,
 *			Institut fuer Kernphysik,
 *			Johannes Gutenberg-Universitaet Mainz
 *
 * $Header: /tmp/cvsroot/Cola/Cindy/inttest.c,v 4.6 1997-07-09 20:39:45 distler Exp $
 */

static char rcsid[] =
 "@(#)$Id: inttest.c 2216 2008-06-13 21:13:47Z distler $";

#include <stdio.h>

struct { char ch; char	c2; } cc;
struct { char ch; short sh; } cs;
struct { char ch; int	in; } ci;
struct { char ch; long	lo; } cl;
struct { char ch; float	fl; } cf;
struct { char ch; double d; } cd;

int
main()
{
  /* Are we little or big endian?  From Harbison&Steele.  */
  union
  {
    long l;
    char c[sizeof (long)];
  } u;

#ifdef INTTEST
  int i;
  long l[2];
  unsigned char *c;
  float f;
  double d;

  i = 287454020;
  l[0] = 287454020;
  l[1] = 0;
  d = 3.14159265358979323846;
  f = d;

  c = (unsigned char *) &i;
  printf("i: %02x %02x %02x %02x\n", c[0], c[1], c[2], c[3]);

  c = (unsigned char *) l;
  printf("l: %02x %02x %02x %02x %02x %02x %02x %02x\n",
	 c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7]);

  c = (unsigned char *) &f;
  printf("f: %02x %02x %02x %02x\n", c[0], c[1], c[2], c[3]);

  c = (unsigned char *) &d;
  printf("d: %02x %02x %02x %02x %02x %02x %02x %02x\n\n",
	 c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7]);
#endif

  if (*rcsid != '@') return(0);

  u.l = 1;
  if (u.c[sizeof (long) - 1] == 1)
    printf("#define WORDS_BIGENDIAN 1\n\n");
  else
    printf("#undef  WORDS_BIGENDIAN\n\n");

  printf("const int CHAR_SIZE   = %d;\n", sizeof(char));
  printf("const int SHORT_SIZE  = %d;\n", sizeof(short));
  printf("const int INT_SIZE    = %d;\n", sizeof(int));
  printf("const int LONG_SIZE   = %d;\n", sizeof(long));
  printf("const int FLOAT_SIZE  = %d;\n", sizeof(float));
  printf("const int DOUBLE_SIZE = %d;\n\n", sizeof(double));

  printf("const int CHAR_ALIGN   = %ld;\n", (long)&cc.c2 - (long)&cc.ch);
  printf("const int SHORT_ALIGN  = %ld;\n", (long)&cs.sh - (long)&cs.ch);
  printf("const int INT_ALIGN    = %ld;\n", (long)&ci.in - (long)&ci.ch);
  printf("const int LONG_ALIGN   = %ld;\n", (long)&cl.lo - (long)&cl.ch);
  printf("const int FLOAT_ALIGN  = %ld;\n", (long)&cf.fl - (long)&cf.ch);
  printf("const int DOUBLE_ALIGN = %ld;\n", (long)&cd.d  - (long)&cd.ch);

  return(0);
}
