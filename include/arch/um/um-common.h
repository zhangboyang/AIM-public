#ifndef _ARCH_UM_COMMON_H
#define _ARCH_UM_COMMON_H



/*
    function naming convention:
      umXyFunc(...)
      um       - usermode
          Func - function name
        Mm     - mmu
        Kd     - kernel debuging
        Sy     - sync (spin lock)
        Tr     - trap (context)
        Io     - input/output
        Rtl    - run-time library
        Ke     - general
*/
#define MAXLINE 4096

#define BUILD_COMPILER "gcc " __VERSION__



#include "util.h"

extern unsigned char kstart[];
extern unsigned char kend[];
extern unsigned char ktext_start[];
extern unsigned char ktext_end[];



extern void umKeKernelEntry();


#include "um-terminal.h"
#include "um-mmu.h"
#include "um-debug.h"

#endif
