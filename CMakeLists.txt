cmake_minimum_required(VERSION 2.6)
project(pid C)
set(PROJECT_VERSION 0.1)
set(CMAKE_BUILD_TYPE Debug)
configure_file(libpid.pc.in libpid.pc @ONLY)

IF(WIN32)
    include(CheckIncludeFile)
    check_include_file("TlHelp32.h" HAVE_TLHELP32_H)

    IF(NOT HAVE_TLHELP32_H)
        #MESSAGE(FATAL_ERROR "Windows XP or newer required.")
    ENDIF()
ENDIF()

if(MSVC)
  # Force to always compile with W4
  if(CMAKE_C_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
  else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUC)
  # Update if necessary
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -pedantic -std=c99")
endif()

include_directories(include)
include_directories(.)
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_MACOSX_RPATH 1) 
    file(GLOB SOURCES libproc/*.c)

ELSEIF(UNIX)
    file(GLOB SOURCES posix/*.c)

ELSEIF(WIN32)
    file(GLOB SOURCES win32/*.c)


ELSE()
    file(GLOB SOURCES *.c)

ENDIF()
add_library(${PROJECT_NAME} SHARED ${SOURCES})

file(GLOB pidof_src tools/pidof.c)
add_executable(pidof ${pidof_src})
target_link_libraries(pidof ${PROJECT_NAME})
set_target_properties(pidof PROPERTIES OUTPUT_NAME "pidof")

file(GLOB ps_src tools/ps.c)
add_executable(ps ${ps_src})
target_link_libraries(ps ${PROJECT_NAME})
set_target_properties(ps PROPERTIES OUTPUT_NAME "ps")

# Install pkg-config pc
install(FILES ${CMAKE_BINARY_DIR}/lib${PROJECT_NAME}.pc DESTINATION share/pkgconfig)

# Install library
install(TARGETS ${PROJECT_NAME} DESTINATION lib)

# Install library headers
file(GLOB HEADERS libpid.h)
install(FILES ${HEADERS} DESTINATION include)

