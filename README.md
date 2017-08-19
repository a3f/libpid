# This is work in progress
## libpid: The portable process identification library

Need a portable way to get a process' id? By name? execution path? Window name? libpid to the rescue!

Works for Windows and macOS, Linux to follow.

[![Build Status](https://travis-ci.org/a3f/libpid.svg?branch=master)](https://travis-ci.org/a3f/libpid)
[![Build status](https://ci.appveyor.com/api/projects/status/uf517cinlto5onjl/branch/master?svg=true)](https://ci.appveyor.com/project/a3f/libpid/branch/master)

### install

    git clone https://github.com/a3f/libpid.git && cd libpid
    mkdir build && cd build
    cmake ..
    make install

## But what use is a portably obtained pid, you might ask?
* You can print it!
* You can compare for equality
* You can use it in cross platform code (See [lade](http://github.com/a3f/lade) for an example)
* Eventually, there will be also basic support for inter-process signals, waiting on process state change and creating pid files

