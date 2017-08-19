#include <libpid.h>
#include <stdio.h>
#include "exit.h"

int main(void)
{
    //FIXME: should be done in CMakeLists.txt
#ifndef pid_snapshot_next
    puts("pid_snapshot_next not supported on this platform");
    return EXIT_INTERNAL_ERR;
#else

    struct pid_snapshot_entry entry;
    pid_snapshot *snapshot = pid_snapshot_new(PID_SNAPSHOT_NAME);

    if (!snapshot) {
        printf("Failed to create snapshot\n");
        return EXIT_NOMATCH;
    }


    int done;
    for (done = pid_snapshot_first(snapshot, &entry);
        !done;
         done = pid_snapshot_next(snapshot, &entry))
    {
        printf("%" FMTpid "\t%s\n", entry.pid, entry.name);
    }
    if (done != 1) {
        printf("Failed with error code = %d\n", done);
        return EXIT_NOMATCH;
    }
    for (done = pid_snapshot_first(snapshot, &entry);
        !done;
         done = pid_snapshot_next(snapshot, &entry))
    {
        printf("%" FMTpid "\t%s\n", entry.pid, entry.name);
    }
    if (done != 1) {
        printf("Failed with error code = %d\n", done);
        return EXIT_NOMATCH;
    }

    pid_snapshot_del(snapshot);
#endif

    return EXIT_MATCHED;
}


