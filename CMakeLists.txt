cmake_minimum_required(VERSION 3.22)

# Project settings
project(
        Sage
        VERSION 1.0.0
        DESCRIPTION ""
        LANGUAGES CXX C
)

# Project options
option(SAGE_BUILD_TESTS "Build test programs" OFF)
option(SAGE_BUILD_EXAMPLES "Build example programs" OFF)

# CMake C++ standards
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Who doesnt like to know the compilation commands...
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Disable the ZERO_CHECK project generation
set(CMAKE_SUPPRESS_REGENERATION true)

add_subdirectory(sage)

if(SAGE_BUILD_EXAMPLES)
        add_subdirectory(examples)
endif(SAGE_BUILD_EXAMPLES)

enable_testing()

if(SAGE_BUILD_TESTS)
        add_subdirectory(vendor)
        add_subdirectory(test_sage)
endif(SAGE_BUILD_TESTS)