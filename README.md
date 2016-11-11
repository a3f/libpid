# This is work in progress
## libpid: The portable process identification library

Need a portable way to get a process' id? By name? execution path? Regex-matching Window names? libpid to the rescue!

Works for Windows and POSIX (Linux, macOS, probably more).

### But what use is a portably obtained pid, you might ask?
* You can print it!
* You can compare for equality
* You can use it in cross platform code (e.g. both [lade] and [libvas] use libpid)
* libpid also has basic support for inter-process signals, waiting on process state change and creating pid files

