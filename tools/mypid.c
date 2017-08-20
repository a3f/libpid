#include <libpid.h>
#include <stdio.h>
#include "exit.h"

int main(void)
{
    pid_t pid = pid_self();
    if (pid < 0)
        return EXIT_INTERNAL_ERR;

    printf("%" PRIpid "\n", pid_self());

    return EXIT_MATCHED;
}


