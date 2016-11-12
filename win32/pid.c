#include "pid.h"

#ifdef LINT
#define dllimport
#define __stdcall
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

pid_t pid_self(void)
{
    return GetCurrentProcessId();
}


