#include <unistd.h>
#define LIBPID_NO_MACRO 1
#include "libpid.h"

pid_t pid_self(void)
#if HAS_WEAK_ALIAS
__attribute__((weak_import, alias("getpid")));
#else
{
    return getpid();
}
#endif

pid_t pid_parent(void)
#if HAS_WEAK_ALIAS
__attribute__((weak_import, alias("getppid")));
#else
{
    return getppid();
}
#endif

