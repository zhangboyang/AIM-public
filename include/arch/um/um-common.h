#ifndef _ARCH_UM_COMMON_H
#define _ARCH_UM_COMMON_H

#ifdef COMPILING_LIBUM
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <assert.h>
#include <unistd.h>
#include <execinfo.h>
#include <malloc.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#endif

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


extern void umKeWorkerEntry(int wrkid);
extern void umKeKernelEntry(void);


#include "um-sync.h"
#include "um-terminal.h"
#include "um-mmu.h"
#include "um-debug.h"
#include "um-worker.h"

#endif
