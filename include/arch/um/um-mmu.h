#ifndef _ARCH_UM_MMU_H
#define _ARCH_UM_MMU_H


#define PAGE_SIZE 4096

#define KERN_PHYSBASE 0x10000UL

#define VIRT_LOWLIMIT ((unsigned char *) 0x10000UL)
#define VIRT_HIGHLIMIT ((unsigned char *) 0x100000000UL)

extern void umMmRemapExecutable(void);
extern void umMmFlushMemoryMap(int flush_user, int flush_kern);
extern void umMmInitPhysicalMemory(void);


#endif
