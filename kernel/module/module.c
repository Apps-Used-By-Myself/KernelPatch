#include "module.h"
#include <stdio.h>
#include <string.h>

#define ENOEXEC 333

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
    printf("rc %d\n", rc);
    return 0;
}

long init_module(void *umod, unsigned long len, const char *uargs)
{
    struct load_info info = {};

    // void *modd = memdup_user(umod, len);
    void *modd = 0;
    // if (!modd) {
    //     return -1;
    // }
    info.len = len;
    info.hdr = modd;

    load_module(&info, uargs, 0);

    return 0;
}