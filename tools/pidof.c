#include <libpid.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    (void) argc;

    //FIXME: should be done in CMakeLists.txt
#ifndef pid_bywindow
    puts("pid_by_window not supported on this platform");
#else
    while(*++argv)
        printf("%s: %d\n", *argv, pid_byname(*argv));
#endif


    return 0;
}

