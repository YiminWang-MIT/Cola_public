//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-456      tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaMIT/HDCraw.cc,v 2.1 2002-03-05 03:49:49 distler Exp $
//
// initialize OOPS HDC and make raw histograms
//

#include "HDCraw.h"

//
// Function name : fillplane(HdcPlane *plane, struct Hdc *onl)
//
// Description   : 
//

void
fillplane(Aqua_online *out, HdcPlane *plane, struct HDCchamber *ch)
{
  ch->error = plane->errorCode();
  out->packEventData(&ch->error);
  if (!(ch->error & (ErrorCodeNoTDCright+ErrorCodeNoTDCleft))) {
    ch->wire      = plane->wire();
    out->packEventData(&ch->wire);
    ch->difftime  = plane->diff_ns();
    out->packEventData(&ch->difftime);
    ch->diffwire  = plane->diff_wire();
    out->packEventData(&ch->diffwire);
    ch->drifttime = plane->dtime();
    out->packEventData(&ch->drifttime);
    ch->goodness  = plane->goodCheck();
    out->packEventData(&ch->goodness);
    ch->rawcoord     = plane->realCoord();
    out->packEventData(&ch->rawcoord);
  }
} // end of fillplane(Aqua_online *, HdcPlane *, struct Hdc *)


//
// Function name : hdccoord(struct FocalCoord *focal,
//                          double x, double th, double y, double ph)
//
// Description   : 
// Input         : x     = 
//                 th    = 
//                 y     = 
//                 ph    = 
// Output        : focal = 
//

void
hdccoord(Aqua_online *out, struct FocalCoord *focal,
	 double x, double th, double y, double ph)
{
    focal->x  = x;
    focal->th = th;
    focal->y  = y;
    focal->ph = ph;
    out->packEventData(&focal->x, 4);

} // end of hdccoord(struct FocalCoord *, double, double, double, double)


//
// Function name : hdcinit(HdcPacket& hdc, struct OOPS& rdb)
//

void
hdcinit(HdcPacket& hdc, struct OOPS& rdb)
{
  if (rdb.hdc.x[1].initfile) hdc.getXplane(1)->Init(rdb.hdc.x[1].initfile);
  if (rdb.hdc.y[1].initfile) hdc.getYplane(1)->Init(rdb.hdc.y[1].initfile);
  if (rdb.hdc.x[2].initfile) hdc.getXplane(2)->Init(rdb.hdc.x[2].initfile);
  if (rdb.hdc.y[2].initfile) hdc.getYplane(2)->Init(rdb.hdc.y[2].initfile);
  if (rdb.hdc.x[3].initfile) hdc.getXplane(3)->Init(rdb.hdc.x[3].initfile);
  if (rdb.hdc.y[3].initfile) hdc.getYplane(3)->Init(rdb.hdc.y[3].initfile);


  if (rdb.bad_planes[0]) hdc.getXplane(1)->markBad();
  if (rdb.bad_planes[1]) hdc.getYplane(1)->markBad();
  if (rdb.bad_planes[2]) hdc.getXplane(2)->markBad();
  if (rdb.bad_planes[3]) hdc.getYplane(2)->markBad();
  if (rdb.bad_planes[4]) hdc.getXplane(3)->markBad();
  if (rdb.bad_planes[5]) hdc.getYplane(3)->markBad();

} // end of hdcinit(HdcPacket&, struct OOPS&)


//
// Function name : hdcraw(HdcPacket& hdc, struct SpectrometerOOPS *onl)
//
// Description   : 
// Input         : 
// Output        : 
// Return        : 
//

void
hdcraw(Aqua_online *out, HdcPacket& hdc, struct SpectrometerOOPS *onl)
{
  int i;

  onl->x.multi   = onl->y.multi   = 0;
  onl->x.pattern = onl->y.pattern = 0;
  onl->x.FitPattern = onl->y.FitPattern = 0;
    
  for (i=1; i<=3; i++) {

    fillplane(out, hdc.getXplane(i), &onl->x.chamber[i]);
    if (!onl->x.chamber[i].error) {
      onl->x.multi += 1;
      onl->x.pattern |= (1<<(i-1));
      out->packEventData(&onl->x.multi, 2);

      if(!hdc.noTrack()) {
	onl->x.OEpattern = hdc.bestPat();
	out->packEventData(&onl->x.OEpattern);

	onl->x.chamber[i].coord = hdc.bestX(i);
	out->packEventData(&onl->x.chamber[i].coord);

	onl->x.chamber[i].OEbit = hdc.bestPat()>>((i-1)*2) & 1;
	out->packEventData(&onl->x.chamber[i].OEbit);

	onl->x.chamber[i].driftdist = hdc.getXplane(i)->drift() 
	  * hdc.getXplane(i)->sign() * hdc.patSign((i*2)-2);
	out->packEventData(&onl->x.chamber[i].driftdist);
      }
    }

    onl->x.FitPattern |= (hdc.getXplane(i)->noFit()<<(i-1));
    out->packEventData(&onl->x.FitPattern);

    fillplane(out, hdc.getYplane(i), &onl->y.chamber[i]);
    if (!onl->y.chamber[i].error) {
      onl->y.multi += 1;
      onl->y.pattern |= (1<<(i-1));
      out->packEventData(&onl->y.multi, 2);

      if(!hdc.noTrack()) {
	onl->y.OEpattern = hdc.bestPat();
	out->packEventData(&onl->y.OEpattern);

	onl->y.chamber[i].coord = hdc.bestY(i);
	out->packEventData(&onl->y.chamber[i].coord);

	onl->y.chamber[i].OEbit = (hdc.bestPat()>>((i-1)*2) & 2)/2;
	out->packEventData(&onl->y.chamber[i].OEbit);

	onl->y.chamber[i].driftdist = hdc.getYplane(i)->drift() 
	  * hdc.getYplane(i)->sign() * hdc.patSign((i*2)-1);
	out->packEventData(&onl->y.chamber[i].driftdist);
      }
    }
    onl->y.FitPattern |= (hdc.getYplane(i)->noFit()<<(i-1));
    out->packEventData(&onl->y.FitPattern);
  }

  //  printf("no track: %d \n", hdc.noTrack());
  if(!hdc.noTrack()) {
    onl->x.x       = hdc.x();
    onl->x.slope   = hdc.slopeX();
    onl->x.chi_sqr = hdc.chisqrX();
    out->packEventData(&(onl->x.x),3);

    onl->y.x       = hdc.x();
    onl->y.slope   = hdc.slopeX();
    onl->y.chi_sqr = hdc.chisqrY();
    out->packEventData(&(onl->y.x),3);
  }
} // end of hdcraw(HdcPacket&, struct SpectrometerOOPS *)
