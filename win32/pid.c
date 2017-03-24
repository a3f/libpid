#include "libpid.h"

#ifdef LINT
#define dllimport
#define __stdcall
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string.h>
#include <TlHelp32.h>
#include <stdio.h>

pid_t pid_self(void)
{
    return GetCurrentProcessId();
}

pid_t pid_bywindow(const char *class, const char *title)
{
    DWORD ProcessId;
    HWND hWnd;

    if (!(hWnd = FindWindowA(class, title))) return -1;
    if (!GetWindowThreadProcessId(hWnd, &ProcessId)) return -1;

    return ProcessId;
}

pid_t pid_byname(const char *name)
{
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof entry;
    char *ext = strrchr(name, '.');
    ptrdiff_t basenamelen = ext - name;

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    for (BOOL more = Process32First(snapshot, &entry); more; more = Process32Next(snapshot, &entry)) {
        if (strnicmp(name, entry.szExeFile, basenamelen) == 0
        && (!ext || stricmp(ext, entry.szExeFile + basenamelen) == 0)) {
            return entry.th32ProcessID;
        }
    }

    CloseHandle(snapshot);

    return -1;
}

struct pid_snapshot {
    HANDLE handle;
};

struct pid_snapshot *pid_snapshot_new(int flags) {
    (void)flags;
    HANDLE *handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (handle == INVALID_HANDLE_VALUE)
        return NULL;
    return (struct pid_snapshot*)handle;
}
void pid_snapshot_del(pid_snapshot *snapshot) {
    CloseHandle((HANDLE)snapshot);
}

int pid_snapshot_first(struct pid_snapshot *snapshot, struct pid_snapshot_entry *entry) {
    entry->sysentry.dwSize = sizeof entry->sysentry;
    if (!Process32First((HANDLE)snapshot, &entry->sysentry))
        return GetLastError() == ERROR_NO_MORE_FILES ? 1 : -1;

    entry->pid = entry->sysentry.th32ProcessID;
    entry->name = entry->sysentry.szExeFile;
    return 0;
}
int pid_snapshot_next(struct pid_snapshot *snapshot, struct pid_snapshot_entry *entry) {
    entry->sysentry.dwSize = sizeof entry->sysentry;
    if (!Process32Next((HANDLE)snapshot, &entry->sysentry))
        return GetLastError() == ERROR_NO_MORE_FILES ? 1 : -1;

    entry->pid = entry->sysentry.th32ProcessID;
    entry->name = entry->sysentry.szExeFile;
    return 0;
}

