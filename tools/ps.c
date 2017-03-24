#include <libpid.h>
#include <stdio.h>
void pid_ls(void);
int main(void)
{
    //FIXME: should be done in CMakeLists.txt
#ifndef pid_snapshot_next
    puts("pid_snapshot_next not supported on this platform");
#else

    pid_snapshot *snapshot = pid_snapshot_new(PID_SNAPSHOT_NAME);

    struct pid_snapshot_entry entry;

    int done;
    for (done = pid_snapshot_first(snapshot, &entry);
        !done;
         done = pid_snapshot_next(snapshot, &entry))
    {
        printf("%d\t%s\n", entry.pid, entry.name);
    }
    if (done != 1) {
        printf("Failed with error code = %d\n", done);
    }
    for (done = pid_snapshot_first(snapshot, &entry);
        !done;
         done = pid_snapshot_next(snapshot, &entry))
    {
        printf("%d\t%s\n", entry.pid, entry.name);
    }
    if (done != 1) {
        printf("Failed with error code = %d\n", done);
    }

    pid_snapshot_del(snapshot);
#endif

    return 0;
}


