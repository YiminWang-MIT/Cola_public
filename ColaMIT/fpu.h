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
