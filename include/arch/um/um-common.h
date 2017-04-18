#ifndef _ARCH_UM_COMMON_H
#define _ARCH_UM_COMMON_H

extern unsigned char ktext_start[];
extern unsigned char ktext_end[];


extern void umKeKernelEntry();


#include "um-terminal.h"
#include "um-mmu.h"
#include "um-debug.h"

#endif
