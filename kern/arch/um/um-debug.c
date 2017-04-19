#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <execinfo.h>
#include "um-common.h"

void umKdPrintMemoryMap(void)
{
    fputs("  " c_green "memory map:" c_normal "\n", stdout);
    char cmdbuf[MAXLINE];
    snprintf(cmdbuf, sizeof(cmdbuf), "pmap -p %d | sed 's/^/    /'", getpid());
    system(cmdbuf);
    fputs("\n", stdout);
}

void umKdPrintStackBacktrace(void)
{
#define MAX_STACK_BACKTRACE 1024
    static void *buf[MAX_STACK_BACKTRACE];
    int size = backtrace(buf, MAX_STACK_BACKTRACE);
    char **bt = backtrace_symbols(buf, size);
    fputs("  " c_green "stack backtrace:" c_normal "\n", stdout);
    for (int i = 0; i < size; i++) {
        unsigned char *fn = (unsigned char *) buf[i];
        int important = ktext_start <= fn && fn < ktext_end;
        fprintf(stdout, important ? "    " c_red "%s" c_normal "\n" : "    %s\n", bt[i]);
    }
    fputs("\n", stdout);
}

void umKdKernelPanic(const char *msg)
{
    fputs("\n", stdout);
    fputs("\n", stdout);
    fputs("      " c_red c_bold "==== KERNEL PANIC ====" c_normal "\n", stdout);
    fputs("\n", stdout);
    fputs("  " c_green "message:" c_normal "\n", stdout);
    fprintf(stdout, "    " c_red "%s" c_normal "\n", msg);
    fputs("\n", stdout);
    umKdPrintStackBacktrace();
    umKdPrintMemoryMap();
    fprintf(stdout, "  " c_blue "waiting for debugger (pid = %d) ..." c_normal "\n", getpid());
    while (1) pause();
}

void umKdExecMonitor(void)
{
    umKdKernelPanic("monitor");
}
