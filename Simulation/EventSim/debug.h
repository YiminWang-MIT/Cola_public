#ifndef __DEBUG_H_
#define __DEBUG_H_


//#define EVDEBUGFLAG
//#define EVLOGFLAG








#ifdef EVDEBUGFLAG
#define EVDEBUG(arg) std::cerr<<FILE<<arg<<std::endl;
#else
#define EVDEBUG(arg) 
#endif
#ifdef EVLOGFLAG
#define EVLOG(arg) std::cerr<<FILE<<arg<<std::endl;
#else
#define EVLOG(arg) 
#endif

#endif
