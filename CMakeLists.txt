cmake_minimum_required(VERSION 2.6)
project(pid)
set(CMAKE_BUILD_TYPE Release)

file (GLOB SOURCES *.c)
include_directories(include)
include_directories(.)
IF(WIN32)
    file(GLOB SOURCES win32/*.c)
ELSEIF(UNIX)
    file(GLOB SOURCES posix/*.c)
    #add_definitions(-DHAS_WEAK_ALIAS)
ENDIF(WIN32)
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Install library
install(TARGETS ${PROJECT_NAME} DESTINATION lib)

# Install library headers
#file(GLOB HEADERS include/*.h)
file(GLOB HEADERS pid.h)
install(FILES ${HEADERS} DESTINATION include)
