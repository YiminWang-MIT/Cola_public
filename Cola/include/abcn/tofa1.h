/*--------------------------------------------------------------------------
 *
 *	TOF Data Structs for Mecdas
 *	Hardware-independent typedefs of CERN ROOT used
 *	UShort_t	unsigned 2-byte integer
 *	Short_t		signed 2-byte integer
 *	UInt_t		unsigned 4-byte integer
 *	Int_t		signed 4-byte integer
 *
 *	J.R.M. Annand	28th July 1999
 *
 *--------------------------------------------------------------------------
 */

#ifndef __TOFA1_H__
#define __TOFA1_H__

/*	Single vertical bar of plastic scintillator
 *	read out by 2 photomultiplier tubes
 *	one at each end
 */

struct Bar_t {
  unsigned short QDCtop;
  unsigned short QDCbottom;
  unsigned short TDCtop;
  unsigned short TDCbottom;
};

/*	One stand of TOF...
 *	1 small veto bar (close to target)
 *	1 frame (layer) of veto/delta E
 *	3 frames of neutron or charged-particle-stopping detector
 *	Pin-diode amplitudes...monitor stability of light flasher
 *	Scalers
 *	numbering for bars is 1..8
 */

struct TOFStand_t{
  struct Bar_t SVeto;		/* single small veto */
  struct Bar_t Veto[9];		/* veto frame */
  struct Bar_t F1[9];		/* 1st n-frame */
  struct Bar_t F2[9];		/* 2nd n-frame */
  struct Bar_t F3[9];		/* 3rd n-frame */
  unsigned short Hits[2];	/* Hit pattern */
  unsigned short QDCPin[4];	/* 1 Pin diode/frame */
  unsigned int SVScaler;	/* Small-veto scaler */
  unsigned int VScaler[9];	/* Veto scalers */
  unsigned int F1Scaler[9];	/* A-frame scalers */
  unsigned int F2Scaler[9];	/* B-frame scalers */
  unsigned int F3Scaler[9];	/* C-frame scalers */
};

/*	Complete TOF Data as used for (e,e'pn) in A1
 *	3 TOF stands
 *	Hit Pattern
 *	Some miscellaneous ADC's
 *	Trigger Scalers
 *	numbering for stands is 1..4
 */

struct TOFA1_t {
  unsigned short label;
  unsigned short Trigger[4];	/* trigger patterns */
  unsigned int TriggerScaler[16];
  struct TOFStand_t Stand1;
  struct TOFStand_t Stand2;
  struct TOFStand_t Stand3;
  unsigned int clockticks;
  unsigned short BHtime;
  struct Bar_t HB;
};

#endif /* __TOFA1_H__ */
