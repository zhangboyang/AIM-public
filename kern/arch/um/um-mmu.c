#include "config.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "um-common.h"

static int zerofd = -1;
static int physfd = -1;

static char *random_shmname(char *buf, size_t bufsz, const char *prefix)
{
    unsigned long long r;
    FILE *fp = fopen("/dev/urandom", "r");
    fread(&r, sizeof(r), 1, fp);
    fclose(fp);
    snprintf(buf, bufsz, "%s-%016llx", prefix, r);
    return buf;
}
static int create_shmfd(const char *name, size_t size)
{
    int fd;
    int ret;
    fd = shm_open(name, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) return -1;
    ret = ftruncate(fd, size);
    if (ret == -1) return -1;
    ret = shm_unlink(name);
    if (ret == -1) return -1;
    return fd;
}

void umMmRemapExecutable(void)
{
    assert(physfd >= 0);
    fprintf(stdout, "  " c_green "remapping kernel executable ..." c_normal "\n");
    fprintf(stdout, "    kstart = 0x%016lx\n", (unsigned long) kstart);
    fprintf(stdout, "    ktop   = 0x%016lx\n", (unsigned long) kend);
    size_t kernsz = kend - kstart;
    fprintf(stdout, "    kernsz = 0x%016lx\n", (unsigned long) kernsz);

    // create temp mapping and copy content
    void *tmp = mmap(NULL, kernsz, PROT_READ | PROT_WRITE, MAP_SHARED, physfd, KERN_PHYSBASE);
    //fprintf(stdout, "    tmp    = 0x%016lx\n", (unsigned long) tmp);
    memcpy(tmp, kstart, kernsz);
    munmap(tmp, kernsz);
    
    // overwrite current mapping
    if (mmap(kstart, kernsz, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED | MAP_FIXED, physfd, KERN_PHYSBASE) == MAP_FAILED) {
        umKdKernelPanic("remap failed!");
    }
    
    fprintf(stdout, "  " c_green "[ OK ]" c_normal "\n\n");
}

void umMmFlushMemoryMap(int flush_user, int flush_kern)
{
    assert(PTR_IS_ALIGNED(kstart, PAGE_SIZE));
    assert(PTR_IS_ALIGNED(kend, PAGE_SIZE));
    if (flush_user) {
        if (mmap(VIRT_LOWLIMIT, kstart - VIRT_LOWLIMIT, PROT_NONE, MAP_SHARED | MAP_FIXED, zerofd, 0) == MAP_FAILED) {
            umKdKernelPanic("can't flush user mapping.");
        }
    }
    if (flush_kern) {
        if (mmap(kend, VIRT_HIGHLIMIT - kend, PROT_NONE, MAP_SHARED | MAP_FIXED, zerofd, 0) == MAP_FAILED) {
            umKdKernelPanic("can't flush kern mapping.");
        }
    }
}

void umMmInitPhysicalMemory(void)
{
    fprintf(stdout, "  " c_green "creating physical memory ..." c_normal "\n");
    
    // create posix shared memory file descriptor
    char shmname[MAXLINE];
    random_shmname(shmname, sizeof(shmname), "/umaim-physmem");
    fprintf(stdout, "    shm    = %s\n", shmname);
    physfd = create_shmfd(shmname, MEM_SIZE * 1024);
    if (physfd == -1) umKdKernelPanic("can't create physmem fd.");
    fprintf(stdout, "    physfd = %d\n", physfd);
    
    // create zero file descriptor
    zerofd = open("/dev/zero", O_RDONLY);
    if (zerofd == -1) umKdKernelPanic("can't create zero fd.");
    fprintf(stdout, "    zerofd = %d\n", zerofd);
    
    // reserve virtual address space
    umMmFlushMemoryMap(1, 1);
    
    
    fprintf(stdout, "  " c_green "[ OK ]" c_normal "\n\n");
}

