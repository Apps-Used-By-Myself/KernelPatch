#include "module.h"
#include <stdio.h>
#include <string.h>

#define ENOEXEC 333

#define elf_check_arch(x) ((x)->e_machine == EM_AARCH64)

/* Parse tag=value strings from .modinfo section */
static char *next_string(char *string, unsigned long *secsize)
{
    /* Skip non-zero chars */
    while (string[0]) {
        string++;
        if ((*secsize)-- <= 1)
            return NULL;
    }

    /* Skip any zero padding. */
    while (!string[0]) {
        string++;
        if ((*secsize)-- <= 1)
            return NULL;
    }
    return string;
}

static char *get_next_modinfo(const struct load_info *info, const char *tag, char *prev)
{
    char *p;
    unsigned int taglen = strlen(tag);
    Elf_Shdr *infosec = &info->sechdrs[info->index.info];
    unsigned long size = infosec->sh_size;

    /*
	 * get_modinfo() calls made before rewrite_section_headers()
	 * must use sh_offset, as sh_addr isn't set!
	 */
    char *modinfo = (char *)info->hdr + infosec->sh_offset;

    if (prev) {
        size -= prev - modinfo;
        modinfo = next_string(prev, &size);
    }

    for (p = modinfo; p; p = next_string(p, &size)) {
        if (strncmp(p, tag, taglen) == 0 && p[taglen] == '=')
            return p + taglen + 1;
    }
    return NULL;
}

static char *get_modinfo(const struct load_info *info, const char *tag)
{
    return get_next_modinfo(info, tag, NULL);
}

/* Find a module section: 0 means not found. */
static unsigned int find_sec(const struct load_info *info, const char *name)
{
    unsigned int i;

    for (i = 1; i < info->hdr->e_shnum; i++) {
        Elf_Shdr *shdr = &info->sechdrs[i];
        /* Alloc bit cleared means "ignore it." */
        if ((shdr->sh_flags & SHF_ALLOC) && strcmp(info->secstrings + shdr->sh_name, name) == 0)
            return i;
    }
    return 0;
}

static int setup_load_info(struct load_info *info, int flags)
{
    unsigned int i;

    /* Set up the convenience variables */
    info->sechdrs = (void *)info->hdr + info->hdr->e_shoff;
    info->secstrings = (void *)info->hdr + info->sechdrs[info->hdr->e_shstrndx].sh_offset;

    /* Try to find a name early so we can log errors with a module name */
    info->index.info = find_sec(info, ".kpm.info");
    if (!info->index.info) {
        printf("Not found .kpm.info section\n");
        return -1;
    }
    info->name = get_modinfo(info, "name");

    /* Find internal symbols and strings. */
    for (i = 1; i < info->hdr->e_shnum; i++) {
        if (info->sechdrs[i].sh_type == SHT_SYMTAB) {
            info->index.sym = i;
            info->index.str = info->sechdrs[i].sh_link;
            info->strtab = (char *)info->hdr + info->sechdrs[info->index.str].sh_offset;
            break;
        }
    }

    if (info->index.sym == 0) {
        printf("%s: module has no symbols (stripped?)\n", info->name);
        return -ENOEXEC;
    }

    info->index.mod = find_sec(info, ".gnu.linkonce.this_module");
    if (!info->index.mod) {
        printf("%s: No module found in object\n", info->name ?: "(missing .modinfo name field)");
        return -ENOEXEC;
    }
    /* This is temporary: point mod into copy of data. */
    info->mod = (void *)info->hdr + info->sechdrs[info->index.mod].sh_offset;

    /*
	 * If we didn't load the .modinfo 'name' field earlier, fall back to
	 * on-disk struct mod 'name' field.
	 */
    // if (!info->name)
    //     info->name = info->mod->name;

    // if (flags & MODULE_INIT_IGNORE_MODVERSIONS)
    //     info->index.vers = 0; /* Pretend no __versions section! */
    // else
    //     info->index.vers = find_sec(info, "__versions");

    // info->index.pcpu = find_pcpusec(info);

    return 0;
}

static int elf_header_check(struct load_info *info)
{
    if (info->len < sizeof(*(info->hdr)))
        return -ENOEXEC;

    if (memcmp(info->hdr->e_ident, ELFMAG, SELFMAG) != 0 || info->hdr->e_type != ET_REL || !elf_check_arch(info->hdr) ||
        info->hdr->e_shentsize != sizeof(Elf_Shdr))
        return -ENOEXEC;

    if (info->hdr->e_shoff >= info->len || (info->hdr->e_shnum * sizeof(Elf_Shdr) > info->len - info->hdr->e_shoff))
        return -ENOEXEC;

    return 0;
}

static int load_module(struct load_info *info, const char *uargs, int flags)
{
    int rc = 0;
    rc = elf_header_check(info);
    rc = setup_load_info(info, 0);
    printf("rc %d\n", rc);
    return 0;
}

long init_module(void *umod, unsigned long len, const char *uargs)
{
    struct load_info info = {};

    // void *modd = memdup_user(umod, len);
    void *modd = umod;
    // if (!modd) {
    //     return -1;
    // }
    info.len = len;
    info.hdr = modd;

    load_module(&info, uargs, 0);

    return 0;
}