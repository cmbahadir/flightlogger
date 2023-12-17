# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.12.0)

# Project's name
project(flightlogger)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBPQXX REQUIRED libpqxx)

# Include directories
include_directories(${LIBPQXX_INCLUDE_DIRS})

# Link directories
link_directories(${LIBPQXX_LIBRARY_DIRS})

# Add executable to be built from the project files
add_executable(${PROJECT_NAME} flightlogger.cpp flightlogger.hpp)

# Link the executable to the necessary libraries
target_link_libraries(${PROJECT_NAME} PRIVATE pthread ${LIBPQXX_LIBRARIES})

# Add an option to enable or disable the building of tests
option(BUILD_TESTS "Build tests" OFF)

# Conditionally add the 'test' directory to the build
if(BUILD_TESTS)
    add_subdirectory(test)
endif()