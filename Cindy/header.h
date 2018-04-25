/*
 *
 *	Copyright (c) 1989, 1990, 1991, 1992, 1993, 1994 by
 *	Johannes Gutenberg-Universitaet Mainz,
 *	Institut fuer Kernphysik, K.W. Krygier
 *
 *	All rights reserved.
 *
 *	%W% KPH (K.W. Krygier) %E% %D%
 *
 */

#ifndef _FORMAT_H_
#define _FORMAT_H_

typedef	unsigned long	Ulong;
typedef long		Long;
typedef unsigned short	Uword;
typedef short		Word;
typedef unsigned char	Ubyte;
typedef char		Byte;

#define	MECDASFORMAT	30000
#define	BEHDRSIZE	sizeof (struct behdr)

#define BEtype		(0xff << 8)
#define	BEnormal	0		/* normal event data */
#define BEadmin		(1 << 8)	/* administration info in event form. */
#define BEident		(2 << 8)	/* experiment identifier */
#define BEconfig	(3 << 8)	/* experiment configuration */
#define BEspecial	(4 << 8)	/* special data */
#define BEany		(5 << 8)
#define BEruninfo	(6 << 8)
#define BEoldident	(7 << 8)	/* old unvalid experiment identifier */
#define BEbad		(8 << 8)	/* bad event data */
#define BEraw		(9 << 8)	/* event data in raw format */
#define BEcomment	(10 << 8)	/* run comment (ASCII) */
#define BEname		(11 << 8)	/* run name (ASCII) */
#define BEmsg		(12 << 8)	/* error or debug message (ASCII) */

#define BEerror		(1 << 0)	/* error in this buffer element */
#define	BEnoswap	(1 << 1)	/* data must not be swapped */
#define	BEtar		(1 << 2)	/* file archive in tar format */
#define	BEshar		(1 << 3)	/* file archive in shar format */

struct	behdr {				/* buffer element header */
	Uword	b_len;			/* length of data field */
	Uword	b_null;			/* 0 for little endian, ffff for big */
	Uword	b_format;		/* format version */
	Uword	b_flags;		/* additional information */
	Uword	b_events;		/* event counter */
	Uword	b_code;			/* event code */
};

#endif
