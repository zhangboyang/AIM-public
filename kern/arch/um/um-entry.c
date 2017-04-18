#include "config.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "um-common.h"


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


static_assert(NR_CPUS <= MAX_CPUS, "too many cpus");



void umKeKernelEntry(void)
{
    // print banner
    fputs("\n", stdout);
    fputs("      " c_red c_bold "WELCOME TO USER-MODE AIM!" c_normal "\n", stdout);
    fputs("\n", stdout);
    fprintf(stdout, "  built on       : " __DATE__ ", " __TIME__ "\n");
    fprintf(stdout, "  compiler       : " BUILD_COMPILER "\n");
    fprintf(stdout, "  kernel base    : 0x%016lX\n", (unsigned long) KERN_BASE);
    fprintf(stdout, "  phyical memory : %d MB (%d KB)\n", MEM_SIZE / 1024, MEM_SIZE);
    fprintf(stdout, "  cpu cores      : %d\n", NR_CPUS);
    fputs("\n", stdout);
    fputs("\n", stdout);


    // print memory map
    fputs("  " c_green "initial memory map:" c_normal "\n", stdout);
    char cmdbuf[MAXLINE];
    snprintf(cmdbuf, sizeof(cmdbuf), "pmap %d | sed 's/^/    /'", getpid());
    system(cmdbuf);
    
    
    // init physical memory
    umMmInitPhysicalMemory();
    
    
    // exec monitor
    umKdExecMonitor();
}

