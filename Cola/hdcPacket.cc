//
// Created 1997 by	T. Pospischil,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
//
/// Implementation of class HdcPacket
//

#include "hdcPacket.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

void
HdcPacket::MemInit()
{
  NameInData = new char[255]; 
}

HdcPacket::HdcPacket(AquaTree *atree, const char *name_in_data)
{
  char dumstr[255];

  MemInit();

  strcpy(NameInData, name_in_data);

  strcpy(dumstr, name_in_data);
  strcat(dumstr, ".u1");
  U1plane = new HdcPlane(atree->getShortSubSubTree(dumstr), "U1", HDC_WIRES);

  strcpy(dumstr, name_in_data);
  strcat(dumstr, ".v1");
  V1plane = new HdcPlane(atree->getShortSubSubTree(dumstr), "V1", HDC_WIRES);

  strcpy(dumstr, name_in_data);
  strcat(dumstr, ".u2");
  U2plane = new HdcPlane(atree->getShortSubSubTree(dumstr), "U2", HDC_WIRES);

  strcpy(dumstr, name_in_data);
  strcat(dumstr, ".v2");
  V2plane = new HdcPlane(atree->getShortSubSubTree(dumstr), "V2", HDC_WIRES);

}

void
HdcPacket::Reset()
{
  Ok = 0;
  if ( U1plane != NULL ) U1plane->Reset();
  if ( V1plane != NULL ) V1plane->Reset();
  if ( U2plane != NULL ) U2plane->Reset();
  if ( V2plane != NULL ) V2plane->Reset();
}


HdcPacket::~HdcPacket()
{
  delete U1plane;
  delete U2plane;
  delete V1plane;
  delete V2plane;
}

void
HdcPacket::AddToOffset(int AddOffset)
{
  U1plane->AddToOffset(AddOffset);
  V1plane->AddToOffset(AddOffset);
  U2plane->AddToOffset(AddOffset);
  V2plane->AddToOffset(AddOffset);
}

void 
HdcPacket::NewConfig(char *filename)    /// Reads new TDC offsets and  
{                                       /// ODD/Even setup (lim,side) from file
  U1plane->NewConfig(filename);
  V1plane->NewConfig(filename);
  U2plane->NewConfig(filename);
  V2plane->NewConfig(filename);
}

void 
HdcPacket::NewDriftParas(HDC aHDC)      /// Sets new driftpara. for all planes 
{
  U1plane->NewDriftParas(aHDC.u1);
  V1plane->NewDriftParas(aHDC.v1);
  U2plane->NewDriftParas(aHDC.u2);
  V2plane->NewDriftParas(aHDC.v2);
}

void 
HdcPacket::InitHDC(HDC aHDC)             /// does NewConfig and NewDriftparas
{
  NewConfig(aHDC.configfile);
  NewDriftParas(aHDC);
}

void                                    /// shifts OE limits of all wire planes
HdcPacket::AddToOElim(int lim_shift_U1, int lim_shift_V1,
		      int lim_shift_U2, int lim_shift_V2)    
{                                
  U1plane->AddToOElim(lim_shift_U1);
  V1plane->AddToOElim(lim_shift_V1);
  U2plane->AddToOElim(lim_shift_U2);
  V2plane->AddToOElim(lim_shift_V2);
}

int HdcPacket::handle()    
{                                
  return (U1plane->handle() || V1plane->handle() ||
	  U2plane->handle() || V2plane->handle());
}

int 
HdcPacket::FillOnline(AquaTree *out, struct online *onl)
{
  const double sin_gamma = sin(0.78539816);  // 45 Grad = 785.39816 mrad
  const double cos_gamma = cos(0.78539816);

  // NOT USED! MAKES RESOLUTION WORSE!:
  // correct driftpaths for tilted tracks:
  //   double ang;
  //   if ( online.ppolA.u1.ok && online.ppolA.u2.ok ){
  //     ang = atan((online.ppolA.u2.x-online.ppolA.u1.x)/DC_DIST);
  //     U1plane->corrDriftDistance(ang);
  //     U2plane->corrDriftDistance(ang);
  //   }
  //   if ( online.ppolA.v1.ok && online.ppolA.v2.ok ){
  //     ang = atan((online.ppolA.v2.x-online.ppolA.v1.x)/DC_DIST);
  //     V1plane->corrDriftDistance(ang);
  //     V2plane->corrDriftDistance(ang);
  //   }

  U1plane->FillOnline(out, onl->ppolA.u1);
  V1plane->FillOnline(out, onl->ppolA.v1);
  U2plane->FillOnline(out, onl->ppolA.u2);
  V2plane->FillOnline(out, onl->ppolA.v2);

  // Ok:
  if ( onl->ppolA.u1.ok && onl->ppolA.v1.ok &&
       onl->ppolA.u2.ok && onl->ppolA.v2.ok    ){
    Ok = onl->ppolA.u1.ok;
    if (onl->ppolA.v1.ok > Ok) Ok = onl->ppolA.v1.ok;
    if (onl->ppolA.u2.ok > Ok) Ok = onl->ppolA.u2.ok;
    if (onl->ppolA.v2.ok > Ok) Ok = onl->ppolA.v2.ok;
  }
  else Ok = 0;
  onl->ppolA.hdcOK = Ok;
  out->packEventData(&(onl->ppolA.hdcOK), 1);

  // HDC, calculation of coordinates:
  if (onl->ppolA.hdcOK){
    u1 = onl->ppolA.u1.x;
    v1 = onl->ppolA.v1.x;
    u2 = onl->ppolA.u2.x;
    v2 = onl->ppolA.v2.x;
    u2_u1 = u2 - u1; 
    v2_v1 = v2 - v1; 
    onl->ppolA.hdc.x  =  u1*cos_gamma + (v1-WP_DIST/DC_DIST*v2_v1)*sin_gamma;
    onl->ppolA.hdc.y  = -u1*sin_gamma + (v1-WP_DIST/DC_DIST*v2_v1)*cos_gamma;
    onl->ppolA.hdc.th = 1000 * atan ( ( u2_u1*cos_gamma + v2_v1*sin_gamma ) 
					/ DC_DIST );
    onl->ppolA.hdc.ph = 1000 * atan ( ( v2_v1*cos_gamma - u2_u1*sin_gamma )
					/ DC_DIST );

    out->packEventData(&onl->ppolA.hdc.x, 4);
  }

  return 0;
}
