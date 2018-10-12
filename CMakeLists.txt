cmake_minimum_required(VERSION 3.9)
project(kscript)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin CACHE INTERNAL "" FORCE)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib CACHE INTERNAL "" FORCE)
set(TESTS_OUTPUT_DIR ${EXECUTABLE_OUTPUT_PATH}/tests CACHE INTERNAL "" FORCE)

if(NOT DEFINED CMAKE_BUILD_TYPE_INIT)
    set(CMAKE_BUILD_TYPE_INIT "Release")
endif()

set(kscript_VERSION_MAJOR 1)
set(kscript_VERSION_MINOR 0)
set(kscript_VERSION_FULL "${kscript_VERSION_MAJOR}.${kscript_VERSION_MINOR}")

include(cmake/kscript-options.cmake)
include(cmake/kscript-dev-tools.cmake)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    include(cmake/clang-cxx-dev-tools.cmake)
endif ()

include(include/CMakeLists.txt)
include(src/CMakeLists.txt)

if (kscript_VERBOSE)
    kscript_info("Building kscript version ${kscript_VERSION_FULL}")
endif ()