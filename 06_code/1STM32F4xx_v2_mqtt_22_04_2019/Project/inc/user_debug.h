#ifndef	__USER_DEBUG_H
#define	__USER_DEBUG_H

//#define	USER_DEBUG

#ifdef USER_DEBUG
	#define DBG(fmt,...) printf(fmt,##__VA_ARGS__)
#else
  #define DBG(fmt, args...)
#endif

#endif
