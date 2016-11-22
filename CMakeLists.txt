cmake_minimum_required(VERSION 2.6)
project(pid)
set(PROJECT_VERSION 0.1)
set(CMAKE_BUILD_TYPE Debug)
configure_file(libpid.pc.in libpid.pc @ONLY)

if(MSVC)
  # Force to always compile with W4
  if(CMAKE_C_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
  else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUC)
  # Update if necessary
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -pedantic -std=c89")
endif()

include_directories(include)
include_directories(.)
IF(WIN32)
    file(GLOB SOURCES win32/*.c)
ELSEIF(UNIX)
    set(CMAKE_MACOSX_RPATH 1) 
    file(GLOB SOURCES posix/*.c)
ELSE()
    file(GLOB SOURCES *.c)
ENDIF(WIN32)
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Install pkg-config pc
install(FILES ${CMAKE_BINARY_DIR}/lib${PROJECT_NAME}.pc DESTINATION share/pkgconfig)

# Install library
install(TARGETS ${PROJECT_NAME} DESTINATION lib)

# Install library headers
file(GLOB HEADERS pid.h)
install(FILES ${HEADERS} DESTINATION include)

