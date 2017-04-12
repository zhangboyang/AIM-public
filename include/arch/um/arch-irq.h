#ifndef _ARCH_IRQ_H
#define _ARCH_IRQ_H

#ifndef __ASSEMBLER__

static inline void local_irq_enable()
{
}

static inline void local_irq_disable()
{

}

#define local_irq_save(flags) \
	do { \
	} while (0)

#define local_irq_restore(flags) \
	do { \
	} while (0)

#endif /* !__ASSEMBLER__ */

#endif /* _ARCH_IRQ_H */

