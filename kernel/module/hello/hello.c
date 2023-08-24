#include <hook.h>
#include <log.h>

int add(int a, int b)
{
    return a + b;
}

int (*backup_add)(int, int) = 0;

int replace_add(int a, int b)
{
    return a - b;
}

int init()
{
    int a = 20;
    int b = 10;
    logkd("before hook add(a,b)=%d\n", add(a,b));

    hook((void*)add, (void*)replace_add, (void**)&backup_add);
    
    logkd("after hook add(a,b)=%d\n", add(a,b));
    return 0;
}