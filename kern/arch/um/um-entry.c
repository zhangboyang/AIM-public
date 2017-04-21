#include "um-common.h"

static_assert(NR_CPUS <= MAX_CPUS, "too many cpus");

void umKeWorkerEntry(int wrkid)
{
    if (wrkid == 0) { // master
        umSyWaitSemaphore(&umKeMasterWorkerSemaphore);
        fprintf(stdout, "  " c_green "master worker %d initialized." c_normal "\n", wrkid);
    } else { // slave
        umSyWaitSemaphore(&umKeSlaveWorkerSemaphore);
        fprintf(stdout, "  " c_green "slave worker %d initialized." c_normal "\n", wrkid);
//if(wrkid==3)umKdKernelPanic("hahaha");
if(wrkid==3)*(int*)0=0;
//if(wrkid==3)exit(0);
    }
    umSySignalSemaphore(&umKeSlaveWorkerSemaphore); // send semaphore to slave
while(1);
}

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
    
    // fork workers
    umKeForkWorkers();
    
    // exec monitor
    umKdExecMonitor();
    exit(0);
}

