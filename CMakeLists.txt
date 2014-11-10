cmake_minimum_required(VERSION 2.8)
project(parcomp CXX C)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)

set(SHARED_FLAGS "-DDEBUG -Wall -Wextra -g -fcolor-diagnostics -Wno-missing-braces")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SHARED_FLAGS} -stdlib=libc++ -std=c++11 -fpic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SHARED_FLAGS} -std=c11")

include_directories(include)
if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
    include_directories(/usr/include/mpich)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

add_library(par SHARED src/mpi.cxx src/prefix.cxx)
target_link_libraries(par mpi)

add_executable(prefix_tests test/prefix.cxx)
target_link_libraries(prefix_tests par)

configure_file(run_prefix_tests.pl run_prefix_tests.pl COPYONLY)
configure_file(mpi_hosts mpi_hosts COPYONLY)
