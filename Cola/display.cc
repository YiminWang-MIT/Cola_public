//								      -*-c++-*-
// Copyright (c) 2002
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
// $Id: display.cc 2354 2009-11-13 13:58:04Z merkel $
//

#include <cstdio>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include "display.h"

extern  pdcD  *PdcD; //that's dirty, I know...

EventDisplay::EventDisplay(AquaTree *atree, AquaTree *onl) 
{
  nr = (AquaNodeInt *)onl->find("online.nr");
  A_dp     = (AquaNodeDouble *) onl->find("online.A.target.dp");
  A_y0     = (AquaNodeDouble *) onl->find("online.A.target.y0");
  A_theta0 = (AquaNodeDouble *) onl->find("online.A.target.th");
  A_phi0   = (AquaNodeDouble *) onl->find("online.A.target.ph");
  out = NULL;
  isfirst = -1;
}

void 
EventDisplay::doDisplay()
{
  //std::cout << "displaying event " << *nr << " ..." << std::flush;
  static int i=0;
  i++;
  char *argv[]={"/usr/lib/java/bin/java", "eventdisplay", NULL};

  if (isfirst) {
    isfirst = 0;
    if (pipe(pipeto  )) perror("Can't open pipe to Java Eventdisplay");
    if (pipe(pipefrom)) perror("Can't open pipe from Java Eventdisplay");
    procid = fork();
    
    if (procid==-1) perror("Can't spawn subprocess Java Eventdisplay");
    if (procid==0) {
      if (dup2(pipeto[0], 0)) perror("Can't dup2 to");
      dup2(pipefrom[1], 1);
      int fd = open("/dev/null", O_RDWR);
      dup2(fd, 2);
      for (fd = sysconf(_SC_OPEN_MAX)-1; fd > 2; fd--) close(fd);
      errno = 0; // probably got set to EBADF from a close
      if (execvp(argv[0], argv) == -1) 
	perror("Can't execute Java Eventdisplay.");
    } else {
      close(pipefrom[1]);
      pipein  = fdopen(pipefrom[0], "r");
      if (!pipein) perror("Can't read from subprocess");
      close(pipeto[0]);
      pipeout = fdopen(pipeto[1],   "w");
      if (!pipeout) 
	perror("Can't write to subprocess");
      
      //    gotSignal = 0;
      //    signal(SIGPIPE,  handleSignal);
      //    signal(SIGCHLD,  handleSignal);
    }
  }
      
  fprintf(pipeout, "Event %d\n",i);
  if (A_dp->ok())     fprintf(pipeout, "Adp     %f\n", (double) *A_dp);
  if (A_theta0->ok()) fprintf(pipeout, "Atheta0 %f\n", (double) *A_theta0);
  if (A_y0->ok())     fprintf(pipeout, "Ay0     %f\n", (double) *A_y0);
  if (A_phi0->ok())   fprintf(pipeout, "Aphi0   %f\n", (double) *A_phi0);
  fprintf(pipeout, "display\n");

  fflush(pipeout);
  char line[255];
  if (fscanf(pipein,"%s", line));

  /*  if (out==NULL) {
#if GCC_VERSION < 30200
    out = new std::ofstream(fileno(popen(
    "gs -q -r72 -g850x450 -dNOPAUSE -dquiet 1>> /dev/null","w")));
#else
    out = new std::ofstream("/dev/null");
#endif
    *out 
      <<"/Helvetica findfont dup length dict /newdict exch def\n"
      <<"{1 index /FID ne {newdict 3 1 roll put} {pop pop} ifelse } forall\n"
      <<"newdict /Encoding ISOLatin1Encoding put\n"
      <<"/Helvetica newdict definefont pop\n"
      << "/Helvetica findfont 0.6 scalefont setfont\n"
      <<"/m {moveto} def\n"
      <<"/l {lineto} def\n"
      <<"/rm {rmoveto} def\n"
      <<"/rl {rlineto} def\n";
  }
  *out
    << "erasepage initgraphics flush 72 2.54 div dup scale\n"
    << "0.01 setlinewidth 0.5 1.5 translate\n";

  planeX = PdcD->getPlaneX();
  planeY = PdcD->getPlaneY();
  int multix, multiy;

  struct wiretime *planex = planeX->getWireTime(multix);
  struct wiretime *planey = planeY->getWireTime(multiy);
  double *lengthx = planeX->getLength(multix);
  double *lengthy = planeY->getLength(multiy);

  *out<<"gsave 0 7 translate\n"
      << "0 0 m 28.8 0 rl 0 5.5 rl 0 5.5 l closepath stroke\n";
  for (int cell=0;cell<8;cell++) {
    *out<<"gsave "<<cell * 1.84 * 2<<" 1 translate\n";
    for (int w = 0; w<8; w++) {
      *out <<"1.8 "<<w*0.508<< " m 0.1 0 rm 1.8 "
	   <<w*0.508<< " 0.1 0 360 arc\n";
      int wire = cell*8+w;
      double length = -1;
      for (int k=0;k<multix; k++)
	if(planex[k].wire == wire) length = lengthx[k]/10;
      if (length != -1)
	*out << 1.84-length<<" "<<w*0.508
	     << " fill m " << 2*length << " 0 rl\n";
     *out<< " stroke ";
    }
    *out<<" grestore ";
  }
  *out<<" grestore ";

  *out <<"gsave 10.8 0 translate\n"
       << "0 0 m 7.2 0 rl 0 5.5 rl -7.2 0 rl closepath stroke\n";
  for (int cell=0;cell<2;cell++) {
    *out<<"gsave "<<cell*1.84*2<<" 1 translate\n";
    for (int w = 0; w<8; w++) {
      *out <<"1.8 "<<w*0.508<< " m 0.1 0 rm 1.8 "
	   <<w*0.508<< " 0.1 0 360 arc\n";
      int wire = cell*8+w;
      double length = -1;
      for (int k=0;k<multiy; k++)
	if(planey[k].wire == wire) length = lengthy[k]/10;
      if (length != -1)
	*out << 1.84-length<<" "<<w*0.508<<" fill m " << 2*length << " 0 rl\n";
     *out<< " stroke ";
    }
    *out<<" grestore ";
  }
  *out<<" grestore ";

  double  xa, xb, xchisqr, ya, yb, ychisqr;
  int xn, xtime, yn, ytime;

  xa      = planeX->a;
  xb      = planeX->b;
  xchisqr = planeX->chisqr;
  xn      = planeX->n;
  xtime   = planeX->time;

  double xa2     = planeX->l22 * xn - planeX->l12 * planeX->l12;
  double ch_dx   = sqrt(xchisqr * planeY->l22 / xa2);
  double ch_dth  = sqrt(xchisqr * xn / xa2) / (1+xb*xb) * 1000;

  ya      = planeY->a;
  yb      = planeY->b;
  ychisqr = planeY->chisqr;
  yn      = planeY->n;
  ytime   = planeY->time;

  double ch_x  = -147.2 + xa - 113.72 * xb; //mm
  double ch_th = atan(xb)*1000.0;           //mrad
  double ch_y  =  -36.8 + ya -  36.72 * yb; //mm
  double ch_ph = atan(yb)*1000.0;           //mrad

  double ya2   = planeY->l22 * planeY->n - planeY->l12 * planeY->l12;
  double ch_dy = sqrt(ychisqr * planeY->l22 / ya2);
  double ch_dphi = sqrt(ychisqr * yn / ya2) / (1+yb*yb) * 1000;

  if (xchisqr<1e5)
    *out <<"gsave 0 7 translate\n"
	 << (xa + xb * -10)/10 << " 0 m " 
	 << (xa + xb *  45)/10 << " 5.5 l stroke grestore\n";
  if (ychisqr<1e5)
    *out <<"gsave 10.8 0 translate\n"
         << (ya + yb * -10)/10 << " 0 m " 
         << (ya + yb *  45)/10 << " 5.5 l stroke grestore\n";
 
 *out
    << "0 4.8 moveto (Event #"<< *nr<<") show\n"
    << "19 6.0 moveto (x       = " << ch_x              << " mm) show\n"
    << "19 5.2 moveto (theta  = " << ch_th/3141.59*180 << "°  ) show\n"
    << "19 4.4 moveto (y       = " << ch_y              << " mm) show\n"
    << "19 3.6 moveto (phi     = " << ch_ph/3141.59*180 << "°  ) show\n"
    << "19 2.4 moveto (dx      = " << ch_dx             << " mm) show\n"
    << "19 1.6 moveto (dth     = " << ch_dth/3141.59*180 << "°  ) show\n"
    << "19 0.8 moveto (dy      = " << ch_dy              << " mm) show\n"
    << "19 0.0 moveto (dphi    = " << ch_dphi/3141.59*180 << "°  ) show\n"
    << "0 3.2 moveto (x: Chi²/n = "<< xchisqr <<"/" << xn << ") show\n"
    << "0 2.4 moveto (   time =   "<< xtime   <<" µs) show\n"
    << "0 1.6 moveto (y: Chi²/n = "<< ychisqr <<"/" << yn << ") show\n"
    << "0 0.8 moveto (   time =   "<<ytime<<" µs) show\n"
    << " flush \n"<< std::flush;
  std::cout << "press return to continue";
  getchar();
  */
}

