//                                                                    -*-c++-*-
// $Id: fpu.h 2216 2008-06-13 21:13:47Z distler $
//
// This file handles the very machine and operating sys dependend settings of
// the floating point unit.
//

#if defined(__i386__)
#if defined(__linux__)
#include <fpu_control.h>  
/// Enable floating point exceptions.
inline void enableFPE() { 
  long cw = _FPU_IEEE-_FPU_MASK_ZM-_FPU_MASK_OM-_FPU_MASK_UM-_FPU_MASK_IM;
  _FPU_SETCW(cw); 

#ifdef __SSE__
  //The SSE Controlword MXCSR has to be set in addition to the 387 cw
  //
  //FZ	bit 15	Flush To Zero
  //R+	bit 14	Round Positive
  //R-	bit 13	Round Negative
  //RZ	bits 13 and 14	Round To Zero
  //RN	bits 13 and 14 are 0	Round To Nearest
  //PM	bit 12	Precision Mask
  //UM	bit 11	Underflow Mask
  //OM	bit 10	Overflow Mask
  //ZM	bit 9	Divide By Zero Mask
  //DM	bit 8	Denormal Mask
  //IM	bit 7	Invalid Operation Mask
  //DAZ	bit 6	Denormals Are Zero
  //PE	bit 5	Precision Flag
  //UE	bit 4	Underflow Flag
  //OE	bit 3	Overflow Flag
  //ZE	bit 2	Divide By Zero Flag
  //DE	bit 1	Denormal Flag
  //IE	bit 0	Invalid Operation Flag

  __asm__ ("stmxcsr %0" : "=m" (*&cw));   // read SSE Controlword
  cw &= 0xF17F;                           // delete UM,OM,ZM,IM masks
  __asm__ ("ldmxcsr %0" : : "m" (*&cw));  // write SSE Controlword
#endif
}
#endif
#if defined(__FreeBSD__)
#include <floatingpoint.h>
inline void enableFPE() { 
  fpsetprec(0x03);
  fpsetmask(0x1D);
}
#endif
#else
inline void enableFPE() {} 
#endif
