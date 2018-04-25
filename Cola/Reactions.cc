//                                                                    -*-c++-*-
// $Id: Reactions.cc 2435 2011-11-03 15:14:08Z patrick $
//
#include "Reactions.h"
#include "ReactionParser.parse.h"
#include <stdio.h>
#include <stdlib.h>

reaction::reaction(const char *Name, Particle *a, Particle *b, Particle *c,
		   Particle *d, Particle *o, Particle *mtarget, 
		   const Particle *M1, // Elastic scattering   if M1=0 
		   const Particle *M2, // Two body reaction    if M1,M2  given
		   const Particle *M3, // Isotropic threebody  if M1,M2,M3
		   const Particle *M4) // M2 decays in M3, M4  if M1,M2,M3,M4
{
  electronIn = electronOut = P_electron;
  name = Name;
  if ((M1 && !M2) || (M2 && !M1)) {
    fprintf(stderr, "Definition of reaction \"%s\" is not valid!\n",name); 
    exit(1);
  }
  A = a;  B = b;  C = c;  D = d;  O = o;
  target = *mtarget;
  m1 = M1;  m2 = M2;  m3 = M3;  m4 = M4;
  threshold = 0;
  if (m1) threshold += m1->getMass();
  if (m2) threshold += m2->getMass();
  if (m3 && !m4) threshold += m3->getMass();
  CMSPolarization = Momentum(0,0,0);
}

reaction::reaction(const char *Name)
{
  electronIn = electronOut = P_electron;
  Out1 = Out2 = Decay1 = Decay2 = Particle();
  name = Name;

  Particle *spec[11] = {NULL, NULL, NULL, NULL, NULL, 
			NULL, NULL, NULL, NULL, NULL, NULL}, 
	   *M[4]    = {NULL, NULL, NULL, NULL};

  reactionscan parser(Name);
  parser.yyparse(this, spec, M);

  // for (int i=0; i<9; i++)
  //  if (spec[i]) std::cerr << "spec[" << i << "]: " << *spec[i] << std::endl;

  A = spec[0];  B = spec[1];  C = spec[2];  D = spec[3];  O = spec[4];
  H3 = spec[5]; TOF = spec[6]; NPOL = spec[7]; NDET = spec[8]; SI = spec[9]; KAOS = spec[10];
  m1 = M[0];    m2 = M[1];  m3 = M[2];  m4 = M[3];

  threshold = 0;
  if (m1) threshold += m1->getMass();
  if (m2) threshold += m2->getMass();
  if (m3 && !m4) threshold += m3->getMass();
}











