#ifndef LIBPID_PID_H_
#define LIBPID_PID_H_

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <TlHelp32.h>
    #ifdef _MSC_VER
        #ifndef _PID_T_
            #define	_PID_T_
            #ifndef _WIN64
                typedef int	pid_t;
            #else
                typedef __int64	pid_t;
            #endif
        #endif
    #endif
#endif

#include <sys/types.h>

#ifndef PID_SNAPSHOT_NAMELEN
#define PID_SNAPSHOT_NAMELEN 1024
#endif


#if defined __APPLE__ && defined __MACH__
    #include <sys/param.h>
    #if defined BSD
        #define LIPPID_ON_UNIX 1
        #define LIBPID_ON_DARWIN 1
    #endif
#elif defined(__unix__)
    #define LIBPID_ON_UNIX 1
#endif

/**
 * \returns process id
 *
 * Retrieves process id of caller
 */
pid_t pid_self(void);
#if defined LIBPID_ON_UNIX && !defined LIBPID_NO_MACRO
#define pid_self getpid
pid_t getpid(void);
#endif

#if 0
/**
 * \param pid         process id
 * \param level [in]  pointer to 0 = parent, 1 = grandparent and so on
 * \param level [out] Maximum level traversed
 * \returns oldest parent  or parent of specified level, whichever's reached first
 *          If pointer to level doesn't change
 *
 * Retrieves the specified process' parent of level X
 */
pid_t pid_parent(pid_t pid, int *level);
#define pid_parent pid_parent
#endif

typedef struct pid_snapshot pid_snapshot;

enum { PID_SNAPSHOT_PID = 0, PID_SNAPSHOT_NAME = 1, PID_SNAPSHOT_CUSTOM_NAMELEN = 2 };
pid_snapshot *pid_snapshot_new(int flags);

void pid_snapshot_del(pid_snapshot *);

struct pid_snapshot_entry {
    size_t size;

    pid_t pid;
    char *name;

#ifdef _WIN32
    PROCESSENTRY32 sysentry;
#elif defined __APPLE__ && defined __MACH__
    struct {
        char name[PID_SNAPSHOT_NAMELEN];
    } sysentry;
#endif
};

int pid_snapshot_first(struct pid_snapshot *snapshot, struct pid_snapshot_entry *entry);
int pid_snapshot_next(struct pid_snapshot *snapshot, struct pid_snapshot_entry *entry);

#if (defined(LIBPID_ON_DARWIN) || defined(_WIN32)) && !defined LIBPID_NO_MACRO
#define pid_snapshot pid_snapshot
#define pid_snapshot_entry pid_snapshot_entry

#define pid_snapshot_new pid_snapshot_new
#define pid_snapshot_del pid_snapshot_del
#define pid_snapshot_first pid_snapshot_first
#define pid_snapshot_next pid_snapshot_next
#endif

/* TODO: provide more complete functions */
#if (LIBPID_ON_DARWIN || _WIN32) && !defined LIBPID_NO_MACRO
pid_t   pid_byname(const char *name);
#define pid_byname pid_byname
#endif

#if LIBPID_ON_DARWIN && !defined LIBPID_NO_MACRO
pid_t   pid_byfileref(const char *name);
#define pid_byfileref pid_byfileref
#endif

#if _WIN32 && !defined LIBPID_NO_MACRO
pid_t   pid_bywindow(const char *title, const char *);
#define pid_bywindow pid_bywindow
#endif

#ifndef LIBPID_NO_FMT

#if defined _WIN64
    #include <inttypes.h>
    #define FMTpid PRId64
#else
    #define FMTpid "d"
#endif

#define PRIpid FMTpid
#define SCNpid FMTpid

#endif

#if 0
pid_snapshot *pid_search(void);



int pid_kill(pid_t, int);
int pid_wait(pid_t, ...);

int pid_file(pid_t);

struct pid_query_t {
    const char *cmdline;
};
int pid_query(pid_t, struct pid_query_t*);
#endif

#endif
