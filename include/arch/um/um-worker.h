#ifndef _ARCH_UM_WORKER_H
#define _ARCH_UM_WORKER_H

extern umSySemaphore umKeMasterWorkerSemaphore;
extern umSySemaphore umKeSlaveWorkerSemaphore;

extern int umKeGetWorkerIdByPid(pid_t pid);
extern int umKeGetCurrentWorkerId(void);
extern void umKeRemoveWorker(int id);
extern void umKeKillAllWorkers(int sig);
extern void umKeWaitAllWorkers(void);
extern void umKeForkWorkers(void);


#endif
