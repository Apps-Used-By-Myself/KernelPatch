#ifndef _KP_MODULE_H_
#define _KP_MODULE_H_

#include "elf-em.h"
#include "elf.h"

struct load_info {
	const char *name;
	/* pointer to module in temporary copy, freed at end of load_module() */
	struct module *mod;
	Elf_Ehdr *hdr;
	unsigned long len;
	Elf_Shdr *sechdrs;
	char *secstrings, *strtab;
	unsigned long symoffs, stroffs, init_typeoffs, core_typeoffs;
	struct _ddebug *debug;
	unsigned int num_debug;
	bool sig_ok;
	unsigned long mod_kallsyms_init_off;

	struct {
		unsigned int sym, str, mod, vers, info, pcpu;
	} index;
};


#endif