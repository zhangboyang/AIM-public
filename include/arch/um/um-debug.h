#ifndef _ARCH_UM_DEBUG_H
#define _ARCH_UM_DEBUG_H

extern void umKdPrintMemoryMap(void);
extern void umKdPrintStackBacktrace(void);
extern void umKdKernelPanic(const char *msg);
extern void umKdExecMonitor(void);

#endif
