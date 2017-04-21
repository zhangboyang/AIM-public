#include "um-common.h"

static pid_t worker[MAX_CPUS];

int umKeGetWorkerIdByPid(pid_t pid)
{
    for (int i = 0; i < NR_CPUS; i++) {
        if (worker[i] == pid) {
            return i;
        }
    }
    return -1;
}

int umKeGetCurrentWorkerId(void)
{
    return umKeGetWorkerIdByPid(getpid());
}

void umKeKillAllWorkers(int sig)
{
    for (int i = 0; i < NR_CPUS; i++) {
        if (worker[i] > 0) {
            kill(worker[i], sig);
        }
    }
}

void umKeRemoveWorker(int id)
{
    worker[id] = 0;
}

void umKeWaitAllWorkers(void)
{
    for (int i = 0; i < NR_CPUS; i++) {
        if (worker[i] > 0) {
            waitpid(worker[i], NULL, 0);
        }
    }
}






umSySemaphore umKeMasterWorkerSemaphore;
umSySemaphore umKeSlaveWorkerSemaphore;

void umKeForkWorkers(void)
{
    fprintf(stdout, "  " c_green "forking workers ..." c_normal "\n");

    // init semaphore
    umSyInitSemaphore(&umKeMasterWorkerSemaphore, 0);
    umSyInitSemaphore(&umKeSlaveWorkerSemaphore, 0);
    
    // do fork
    for (int i = 0; i < NR_CPUS; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            umKeWorkerEntry(i);
            exit(0);
        }
        if (pid != -1) {
            fprintf(stdout, "    worker[%d] = %d\n", i, (int) pid);
            worker[i] = pid;
        } else {
            umKdKernelPanic("unable to fork worker.");
        }
    }
    
    fprintf(stdout, "  " c_green "[ OK ]" c_normal "\n\n");
    
    // release semaphore for master
    umSySignalSemaphore(&umKeMasterWorkerSemaphore);
}

