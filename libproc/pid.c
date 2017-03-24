#include "libpid.h"

#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include <libproc.h>

pid_t
pid_byfileref(const char *file)
{
        pid_t pid = -1;
        proc_listpidspath(PROC_ALL_PIDS, 0, file, 0, &pid, sizeof pid);
        return pid;
}

pid_t
pid_byname(const char *name)
{
    pid_t pid = -1;

    int pidbufsz = proc_listpids(PROC_ALL_PIDS, 0, NULL, 0);
    pid_t *pids = calloc(1, pidbufsz);
    int pidcnt = proc_listpids(PROC_ALL_PIDS, 0, pids, pidbufsz) / sizeof (pid_t);

    char *pidname = malloc(PATH_MAX);

    for (int i = 0; i < pidcnt; i++) {
        proc_name(pids[i], pidname, PATH_MAX);
        if (strncmp(pidname, name, PATH_MAX) == 0)
        {
            pid = pids[i];
            break;
        }
    }

    free(pidname);
    free(pids);
    
    return pid;
}


struct pid_snapshot {
    int len;
    int i;
    int flags;
    size_t namelen;
    pid_t pids[];
};

struct pid_snapshot *pid_snapshot_new(int flags) {
    int sz = proc_listpids(PROC_ALL_PIDS, 0, NULL, 0);
    struct pid_snapshot *snapshot = calloc(1, sizeof *snapshot + sz);
    if (!snapshot) return NULL;
    snapshot->len = proc_listpids(PROC_ALL_PIDS, 0, snapshot->pids, sz) / sizeof(pid_t);
    snapshot->flags = flags;
    return snapshot;
}
void pid_snapshot_del(struct pid_snapshot *snapshot) {
    free(snapshot);
}


int pid_snapshot_first(struct pid_snapshot *snapshot, struct pid_snapshot_entry *entry) {
    snapshot->i = 0;
    return pid_snapshot_next(snapshot, entry);
}
int pid_snapshot_next(struct pid_snapshot *snapshot, struct pid_snapshot_entry *entry) {
    if (snapshot->i == snapshot->len)
        return 1;
    entry->pid = snapshot->pids[snapshot->i];
    if (snapshot->flags & PID_SNAPSHOT_NAME) {
        size_t namelen = PID_SNAPSHOT_NAMELEN;
        if (snapshot->flags & PID_SNAPSHOT_CUSTOM_NAMELEN) {
            size_t before = (char*)&entry->sysentry.name - (char*)entry;
            size_t after = sizeof *entry - sizeof entry->sysentry.name - before;
            namelen = entry->size - before - after;
        }
        proc_name(snapshot->pids[snapshot->i], entry->sysentry.name, namelen);
        entry->name = entry->sysentry.name;
    }

    if (++snapshot->i == snapshot->len)
        return 1;

    return 0;
}

