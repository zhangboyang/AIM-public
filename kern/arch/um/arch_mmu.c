#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <sys/types.h>
#include <aim/early_kmmap.h>
#include <aim/mmu.h>

bool early_mapping_valid(struct early_mapping *entry)
{
	return true;
}
