#ifndef LIBPID_PID_H_
#define LIBPID_PID_H_

#include <sys/types.h>

pid_t pid_self(void);
pid_t pid_parent(void);
pid_t pid_search();

int pid_kill(pid_t, int);
int pid_wait(pid_t, ...);

int pid_file(pid_t);

struct pid_query_t {
    const char *cmdline;
};
int pid_query(pid_t, struct pid_query_t*);

#endif
