#ifndef _KP_STDDEF_H_
#define _KP_STDDEF_H_

#define NULL 0

#define RET_VOID ((void)0)

#define offsetof(TYPE, MEMBER) ((size_t) & ((TYPE *)0)->MEMBER)

#endif