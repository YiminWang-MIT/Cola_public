//
// Created 1994 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Chamber/vdcDispatch.cc,v 2.15 2003-03-14 13:28:56 merkel Exp $
//
// Implementation of EventDispatch callback functions
//

static char rcsid[] =
 "@(#)$Id: vdcDispatch.cc 2216 2008-06-13 21:13:47Z distler $";

#include "../Cindy/EventDispatch.h"
#include "chlib++.h"
#include <string.h>

int
scheduleVdc(unsigned short *data, int len, char *ptr)
{
  return ((VdcPlane *) ptr) -> FillTime((chRawData *)data,len);
}

VdcPlane::VdcPlane(EventDispatch *evd, const char *name, int size)
{
  if (*rcsid == '@') MemInit(name, size);
  if (evd) evd->addCallback(name, scheduleVdc, (char *)this);
}

VdcPacket::VdcPacket(EventDispatch *evd, const char *path,
		     int nx1, int ns1, int nx2, int ns2, int uv)
{
  int plen = strlen(path);
  char *buf = new char[plen+4];

  strcpy(buf, path);
  strcat(buf, ".x1");

  buf[plen+1] = (uv ? 'u' : 'x');
  x1 = new VdcPlane(evd, buf, nx1);

  buf[plen+1] = (uv ? 'v' : 's');
  s1 = new VdcPlane(evd, buf, ns1);

  buf[plen+2] = '2';
  buf[plen+1] = (uv ? 'u' : 'x');
  x2 = new VdcPlane(evd, buf, nx2);

  buf[plen+1] = (uv ? 'v' : 's');
  s2 = new VdcPlane(evd, buf, ns2);

  MemInit();
  destroyPlanes = 1;
  delete [] buf;
}

VdcApacket::VdcApacket(EventDispatch *evd, const char *path,
		       int nx1, int ns1, int nx2, int ns2)
  : VdcPacket(evd, path, nx1, ns1, nx2, ns2)
{
  Init();
}

VdcBpacket::VdcBpacket(EventDispatch *evd, const char *path,
		       int nx1, int ns1, int nx2, int ns2)
  : VdcPacket(evd, path, nx1, ns1, nx2, ns2)
{
  Init();
}

VdcCpacket::VdcCpacket(EventDispatch *evd, const char *path,
		       int nx1, int ns1, int nx2, int ns2)
  : VdcPacket(evd, path, nx1, ns1, nx2, ns2)
{
  Init();
}

VdcOHIPSpacket::VdcOHIPSpacket(EventDispatch *evd, const char *path,
			       int nu1, int nv1, int nu2, int nv2)
  : VdcPacket(evd, path, nu1, nv1, nu2, nv2, 1)
{
  Init();
}
