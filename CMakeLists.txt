cmake_minimum_required(VERSION 3.22)

# Project settings
project(
        Sage
        VERSION 1.0.0
        DESCRIPTION ""
        LANGUAGES CXX C
)

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

enable_testing()
add_subdirectory(vendor)
add_subdirectory(test_sage)