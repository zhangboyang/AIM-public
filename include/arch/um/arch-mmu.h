#ifndef _ARCH_MMU_H
#define _ARCH_MMU_H

/* addresses before and after early MMU mapping */
#define __premap_addr(kva)	(ULCAST(kva) - KERN_BASE)
#define __postmap_addr(pa)	(ULCAST(pa) + KERN_BASE)

/* kernel virtual address and physical address conversion */
#define kva2pa(kva)		(ULCAST(kva) - KERN_BASE)
#define pa2kva(pa)		(PTRCAST(pa) + KERN_BASE)

#ifndef __ASSEMBLER__

typedef unsigned long pgindex_t;

#endif /* !__ASSEMBLER__ */

#endif /* !_ARCH_MMU_H */

