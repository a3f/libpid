cmake_minimum_required(VERSION 2.8.0)
project(pid C)
set(PROJECT_VERSION 0.16)
set(API_VERSION 0)
INCLUDE (GNUInstallDirs)
configure_file(libpid.pc.in libpid.pc @ONLY)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)


option(WITH_PIC "Compile static library as position-independent code" OFF) # Shared library is always PIC
option(BUILD_MACOS_FATLIB "Build universal library for both i386 and x86_64 on macOS" ON)

IF(BUILD_MACOS_FATLIB)
    IF (CMAKE_OSX_ARCHITECTURES)
        MESSAGE(FATAL_ERROR "User supplied -DCMAKE_OSX_ARCHITECTURES overrides BUILD_MACOS_FATLIB=ON")
    ELSE()
        SET(CMAKE_OSX_ARCHITECTURES "x86_64;i386")
    ENDIF()
ENDIF()

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
elseif (("${CMAKE_C_COMPILER_ID}" MATCHES ".*Clang") OR ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU") OR ("${CMAKE_C_COMPILER_ID}" STREQUAL "Intel"))
  # Update if necessary
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -std=c99")
endif()

include_directories(include)
include_directories(.)
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_MACOSX_RPATH 1)
    file(GLOB SOURCES libproc/*.c posix/*.c)

ELSEIF(UNIX)
    file(GLOB SOURCES posix/*.c)

ELSEIF(WIN32)
    file(GLOB SOURCES win32/*.c)


ELSE()
    file(GLOB SOURCES *.c)

ENDIF()
add_library(${PROJECT_NAME} STATIC ${SOURCES})
#set_target_properties(${PROJECT_NAME} PROPERTIES
#    VERSION   ${PROJECT_VERSION}
#    SOVERSION ${API_VERSION}
#)

file(GLOB mypid_src tools/mypid.c)
add_executable(mypid ${mypid_src})
target_link_libraries(mypid ${PROJECT_NAME})
set_target_properties(mypid PROPERTIES OUTPUT_NAME "mypid")

file(GLOB pidof_src tools/pidof.c)
add_executable(pidof ${pidof_src})
target_link_libraries(pidof ${PROJECT_NAME})
set_target_properties(pidof PROPERTIES OUTPUT_NAME "pidof")

file(GLOB ps_src tools/ps.c)
add_executable(ps ${ps_src})
target_link_libraries(ps ${PROJECT_NAME})
set_target_properties(ps PROPERTIES OUTPUT_NAME "ps")

# Install pkg-config pc
install(FILES ${CMAKE_BINARY_DIR}/lib${PROJECT_NAME}.pc DESTINATION "share/pkgconfig")

# Install library
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})

# Install library headers
file(GLOB HEADERS libpid.h)
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Testing
enable_testing()
add_test(MYPID ${CMAKE_CURRENT_BINARY_DIR}/mypid)
add_test(PIDOF ${CMAKE_CURRENT_BINARY_DIR}/pidof pidof)
add_test(PS ${CMAKE_CURRENT_BINARY_DIR}/ps)

SET(CPACK_PACKAGE_NAME "lib${PROJECT_NAME}")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Multi-Platform memory reading/writing")
SET(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
SET(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/README.md")
SET(CPACK_RESOURCE_FILE_WELCOME "${PROJECT_SOURCE_DIR}/README.md")
SET(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
SET(CPACK_GENERATOR "ZIP;TGZ") # Remove this, if you want the NSIS installer on Windows
include(CPack)
