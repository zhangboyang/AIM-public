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
    int wrkid = umKeGetCurrentWorkerId();
    
    fputs("\n", stdout);
    fputs("\n", stdout);
    fputs("      " c_red c_bold "==== KERNEL PANIC ====" c_normal "\n", stdout);
    fputs("\n", stdout);

    umKdPrintMemoryMap();
    umKdPrintStackBacktrace();

    fprintf(stdout, "  " c_green "message: " c_blue "(worker = %d, pid = %d)" c_normal "\n", wrkid, (int) getpid());
    fprintf(stdout, "    " c_red c_bold "%s" c_normal "\n", msg);
    fputs("\n", stdout);

    if (wrkid >= 0) kill(getppid(), SIGUSR1);
    while (1);
}





static volatile sig_atomic_t monitor_action;
static volatile pid_t child_pid;
static volatile int child_status;


enum {
    MONITOR_ACT_PANIC = 1,
    MONITOR_ACT_QUIT,
    MONITOR_ACT_CHILDEXIT,
};

static void monitor_sigaction(int signo, siginfo_t *info, void *context)
{
    int sv_errno = errno;
    switch (signo) {
    case SIGINT: monitor_action = MONITOR_ACT_QUIT; break;
    case SIGUSR1: monitor_action = MONITOR_ACT_PANIC; break;
    case SIGCHLD:
        monitor_action = MONITOR_ACT_CHILDEXIT;
        while (1) {
            int status;
            pid_t pid = waitpid(-1, &status, WNOHANG);
            if (pid == 0) break;
            child_pid = pid;
            child_status = status;
        }
    }
    //printf("signo=%d\n", signo);
    errno = sv_errno;
}

void umKdExecMonitor(void)
{
    // register handler
    struct sigaction act;
    act.sa_sigaction = monitor_sigaction;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGUSR1, &act, NULL);
    act.sa_flags = SA_NOCLDSTOP;
    sigaction(SIGCHLD, &act, NULL);

    // wait signal
    while (1) {
        sigset_t zeromask, newmask;
        sigemptyset(&zeromask);
        sigemptyset(&newmask);
        sigaddset(&newmask, SIGINT);
        sigaddset(&newmask, SIGUSR1);
        sigaddset(&newmask, SIGCHLD);
        
        sigprocmask(SIG_BLOCK, &newmask, NULL);
        monitor_action = 0;
        while (monitor_action == 0) {
            sigsuspend(&zeromask);
            //printf("monitor_action=%d\n", monitor_action);
        }
        
        switch (monitor_action) {
        case MONITOR_ACT_CHILDEXIT:
            fprintf(stdout, "\n  " c_purple "worker %d (pid = %d) exits, ", umKeGetWorkerIdByPid(child_pid), (int) child_pid);
            if (WIFSIGNALED(child_status)) fprintf(stdout, "%s, ", strsignal(WTERMSIG(child_status)));
            fprintf(stdout, "stopping all workers ..." c_normal "\n");
            umKeKillAllWorkers(SIGSTOP);
            break;
        case MONITOR_ACT_PANIC:
            fprintf(stdout, "\n  " c_purple "kernel panic occured, stopping all workers ..." c_normal "\n");
            umKeKillAllWorkers(SIGSTOP);
            break;
        case MONITOR_ACT_QUIT:
            fprintf(stdout, "\n  " c_purple "sigint received, quitting ..." c_normal "\n");
            umKeKillAllWorkers(SIGKILL);
            umKeWaitAllWorkers();
            exit(1);
        }
    }
}
