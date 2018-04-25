/*                                                                      -*-c-*-
   Raw data structure for TJNAF Hall A HRS spectrometers 
*/

#ifndef __HALL_A_HRS__
#define __HALL_A_HRS__

#define VDC_MAXWORD     450  /*  maximum number of data words for a plane */
#define SCINT_MAXWORD    15  /*  maximum number of data words for a plane */
#define AERO_MAXWORD     30  /*  maximum number of data words for a plane */
#define GAS_MAXWORD      15  /*  maximum number of data words for a plane */
#define SHWR_MAXWORD    100  /*  maximum number of data words for a plane */
#define FPP_MAXWORD      35  /*  maximum number of data words for a plane */
#define HELICITY_MAXWORD 10  /*  maximum number of data words for a plane */
#define SPARE_MAXWORD    20  /*  maximum number of data words for a */
                             /*   spare detector plane */
#define TRGGR_MAXWORD    10 
#define SHMULT_MAXWORD   16  /*  maximum number of data words for the */
                             /*     multiplexer of preshower/shower */

#define BPM_MAXWORD      16  /*  max number data words for BPM info  */
#define RASTER_MAXWORD   16  /*  max number data words for raster  */

struct DATA   {  short data,   opt; };
struct WIRE   {  short wire,   opt; };
struct PAD    {  short pad,    opt; };
struct MIRROR {  short mirror, opt; };
struct BLOCK  {  short block,  opt; };
struct STRAW  {  short straw,  opt; };
/*                             opt      optional information word
                                        for instance, in the case of the
                                        LeCroy 1875 TDC, it contains the
                                        range value: 0 ==> 'LOW'
                                                     1 ==> 'HIGH'
                                        info is set equal to -1 when not used.
*/

struct VDC {
  struct WIRE u1[VDC_MAXWORD];
  struct WIRE v1[VDC_MAXWORD];
  struct WIRE u2[VDC_MAXWORD];
  struct WIRE v2[VDC_MAXWORD];
};

struct SCINT_PLANE {
  struct PAD adc[SCINT_MAXWORD];
  struct PAD tdc[SCINT_MAXWORD];
};

struct SCINT_E {
  struct SCINT_PLANE s[2];
};

struct SCINT_H {
  struct SCINT_PLANE s[3];
};

struct AERO {
  struct MIRROR tdc[AERO_MAXWORD];
  struct MIRROR adc[AERO_MAXWORD];
};

struct GAS {
  struct MIRROR tdc[GAS_MAXWORD];
  struct MIRROR adc[GAS_MAXWORD];
};

struct SHOWER {
  struct DATA shwr[SHWR_MAXWORD];    
};

struct HRSE {
  struct VDC     vdc;
  struct SCINT_E scint;
  struct AERO    aero;
  struct GAS     gas;
  struct SHOWER  shower[2];
  struct DATA trigger[TRGGR_MAXWORD];
};

struct HRSH {
  struct VDC     vdc;
  struct SCINT_H scint;
  struct AERO    aero;
  struct GAS     gas;
  struct DATA trigger[TRGGR_MAXWORD];
};

struct hrs {
  struct HRSE e;
  struct HRSE h;
  int    eventnr;
  int    length;
  struct DATA helicity[HELICITY_MAXWORD];
  struct DATA shmult1[SHMULT_MAXWORD]; /* data for preshower multiplex */
  struct DATA shmult2[SHMULT_MAXWORD]; /* data for shower multiplex    */
  struct DATA bpm[BPM_MAXWORD];
  struct DATA raster[BPM_MAXWORD];

  struct DATA spare1[SPARE_MAXWORD];
  struct DATA spare2[SPARE_MAXWORD];
  struct DATA spare3[SPARE_MAXWORD];
  struct DATA spare4[SPARE_MAXWORD];
  struct DATA spare5[SPARE_MAXWORD];
  struct DATA spare6[SPARE_MAXWORD];
  struct DATA spare7[SPARE_MAXWORD];
  struct DATA spare8[SPARE_MAXWORD];
  struct DATA spare9[SPARE_MAXWORD];
  struct DATA spare10[SPARE_MAXWORD];
};

/*  Allocation of memory for focal plane polarimeter
           record /raw_unit/ fpple_v1(FPP_MAXWORD)  v-plane #1;  lead edge time
           record /raw_unit/ fppte_v1(FPP_MAXWORD)  v-plane #1;  trail edge time
           record /raw_unit/ fpple_v2(FPP_MAXWORD)  v-plane #2;  lead edge time
           record /raw_unit/ fppte_v2(FPP_MAXWORD)  v-plane #2;  trail edge time
           record /raw_unit/ fpple_v3(FPP_MAXWORD)  v-plane #3;  lead edge time
           record /raw_unit/ fppte_v3(FPP_MAXWORD)  v-plane #3;  trail edge time
           record /raw_unit/ fpple_u1(FPP_MAXWORD)  u-plane #1;  lead edge time
           record /raw_unit/ fppte_u1(FPP_MAXWORD)  u-plane #1;  trail edge time
           record /raw_unit/ fpple_u2(FPP_MAXWORD)  u-plane #2;  lead edge time
           record /raw_unit/ fppte_u2(FPP_MAXWORD)  u-plane #2;  trail edge time
           record /raw_unit/ fpple_u3(FPP_MAXWORD)  u-plane #3;  lead edge time
           record /raw_unit/ fppte_u3(FPP_MAXWORD)  u-plane #3;  trail edge time
           record /raw_unit/ fpple_v4(FPP_MAXWORD)  v-plane #4;  lead edge time
           record /raw_unit/ fppte_v4(FPP_MAXWORD)  v-plane #4;  trail edge time
           record /raw_unit/ fpple_v5(FPP_MAXWORD)  v-plane #5;  lead edge time
           record /raw_unit/ fppte_v5(FPP_MAXWORD)  v-plane #5;  trail edge time
           record /raw_unit/ fpple_v6(FPP_MAXWORD)  v-plane #6;  lead edge time
           record /raw_unit/ fppte_v6(FPP_MAXWORD)  v-plane #6;  trail edge time
           record /raw_unit/ fpple_u4(FPP_MAXWORD)  u-plane #4;  lead edge time
           record /raw_unit/ fppte_u4(FPP_MAXWORD)  u-plane #4;  trail edge time
           record /raw_unit/ fpple_u5(FPP_MAXWORD)  u-plane #5;  lead edge time
           record /raw_unit/ fppte_u5(FPP_MAXWORD)  u-plane #5;  trail edge time
           record /raw_unit/ fpple_u6(FPP_MAXWORD)  u-plane #6;  lead edge time
           record /raw_unit/ fppte_u6(FPP_MAXWORD)  u-plane #6;  trail edge time
           record /raw_unit/ fpple_u7(FPP_MAXWORD)  u-plane #7;  lead edge time
           record /raw_unit/ fppte_u7(FPP_MAXWORD)  u-plane #7;  trail edge time
           record /raw_unit/ fpple_u8(FPP_MAXWORD)  u-plane #8;  lead edge time
           record /raw_unit/ fppte_u8(FPP_MAXWORD)  u-plane #8;  trail edge time
           record /raw_unit/ fpple_v7(FPP_MAXWORD)  v-plane #7;  lead edge time
           record /raw_unit/ fppte_v7(FPP_MAXWORD)  v-plane #7;  trail edge time
           record /raw_unit/ fpple_v8(FPP_MAXWORD)  v-plane #8;  lead edge time
           record /raw_unit/ fppte_v8(FPP_MAXWORD)  v-plane #8;  trail edge time
           record /raw_unit/ fpple_x1(FPP_MAXWORD)  x-plane #1;  lead edge time
           record /raw_unit/ fppte_x1(FPP_MAXWORD)  x-plane #1;  trail edge time
           record /raw_unit/ fpple_x2(FPP_MAXWORD)  x-plane #2;  lead edge time
           record /raw_unit/ fppte_x2(FPP_MAXWORD)  x-plane #2;  trail edge time
           record /raw_unit/ fpple_u9(FPP_MAXWORD)  u-plane #9;  lead edge time
           record /raw_unit/ fppte_u9(FPP_MAXWORD)  u-plane #9;  trail edge time
           record /raw_unit/ fpple_u10(FPP_MAXWORD)  u-plane #10; lead edge time
           record /raw_unit/ fppte_u10(FPP_MAXWORD)  u-plane #10; trail edge time
           record /raw_unit/ fpple_u11(FPP_MAXWORD)  u-plane #11; lead edge time
           record /raw_unit/ fppte_u11(FPP_MAXWORD)  u-plane #11; trail edge time
           record /raw_unit/ fpple_v9(FPP_MAXWORD)  v-plane #9;  lead edge time
           record /raw_unit/ fppte_v9(FPP_MAXWORD)  v-plane #9;  trail edge time
           record /raw_unit/ fpple_v10(FPP_MAXWORD)  v-plane #10; lead edge time
           record /raw_unit/ fppte_v10(FPP_MAXWORD)  v-plane #10; trail edge time
           record /raw_unit/ fpple_v11(FPP_MAXWORD)  v-plane #11; lead edge time
           record /raw_unit/ fppte_v11(FPP_MAXWORD)  v-plane #11; trail edge time

	   */

extern struct hrs hrs;

#endif /* __HALL_A_HRS__ */
