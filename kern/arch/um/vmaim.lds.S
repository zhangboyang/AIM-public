/* Copyright (C) 2016 David Gao <davidgao1001@gmail.com>
 *
 * This file is part of AIM.
 *
 * AIM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AIM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#define __LDSCRIPT__

/*
 * Using the C preprocessor, we allow includes and macro expansion in this
 * linker script.
 */


// 我写 ldscript 的过程是这样的：
//   while (不正常) 瞎折腾;
// 再也不想看到 ld 了，心累！ by ZBY 20170412

ENTRY(_start)

SECTIONS
{
    // make sure KERN_BASE % align_in_readelf == 0, or VMA will be 0
    // make sure add SIZEOF_HEADERS, otherwise PHDR (i.e. FILEHDR) will be at wrong VMA
	. = KERN_BASE + SIZEOF_HEADERS;

	.text : {
		*(.text);
	}

	.data : {
		*(.data);
	}

	.bss : {
		*(.bss);
		. = ALIGN(4096);
		PROVIDE(kstack = .);
		. += KSTACKSIZE;
		PROVIDE(kstack_top = .);
	}
	
	
	// p.s. use "gcc a.c -Wl,-verbose" to show default linker script
	
	// fix undefined symbol __init_array_start and __init_array_end
	.init_array : {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);
    }
    
    // fix "./kern/vmaim.elf: error while loading shared libraries: unexpected PLT reloc type 0x06"
    .rela.plt : {
      *(.rela.plt)
      PROVIDE_HIDDEN (__rela_iplt_start = .);
      *(.rela.iplt)
      PROVIDE_HIDDEN (__rela_iplt_end = .);
    }
}

