#include "config.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include "um-common.h"




static_assert(NR_CPUS <= MAX_CPUS, "too many cpus");



void umKeKernelEntry(void)
{
    // turnoff output buffering
    setvbuf(stdout, NULL, _IONBF, 0);
    
    // avoid sbrk() in malloc()
    mallopt(M_MMAP_THRESHOLD, 0);
    
    // print banner
    fputs("\n", stdout);
    fputs("      " c_red c_bold "WELCOME TO USER-MODE AIM!" c_normal "\n", stdout);
    fputs("\n", stdout);
    fprintf(stdout, "  built on       : " __DATE__ ", " __TIME__ "\n");
    fprintf(stdout, "  compiler       : " BUILD_COMPILER "\n");
    fprintf(stdout, "  kernel base    : 0x%016lx\n", (unsigned long) KERN_BASE);
    fprintf(stdout, "  phyical memory : %d MB (%d KB)\n", MEM_SIZE / 1024, MEM_SIZE);
    fprintf(stdout, "  cpu cores      : %d\n", NR_CPUS);
    fputs("\n", stdout);
    fputs("\n", stdout);
    
    // init physical memory
    umMmInitPhysicalMemory();
    
    // remap kernel executable
    umMmRemapExecutable();
    
    // exec monitor
    umKdExecMonitor();
}

