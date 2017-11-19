#include <libpid.h>
#include <stdio.h>
#include "exit.h"

int main(int argc, char *argv[])
{
    int ret;
    (void) argc;

    //FIXME: should be done in CMakeLists.txt
#ifndef pid_byname
    (void) argv;
    ret = EXIT_NOSYS;
    puts("pid_byname not supported on this platform");
#else
    ret = EXIT_NOMATCH;
    while(*++argv) {
        pid_t pid = pid_byname(*argv);
        if (pid != -1) {
            printf("%s: %" PRIpid "\n", *argv, pid_byname(*argv));
            ret = EXIT_MATCHED;
        }
    }
#endif

    return ret;
}

