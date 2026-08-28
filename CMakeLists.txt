cmake_minimum_required(VERSION 3.20)

project(WAF CXX)

# Enforcing standard C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch Catch2 from github for tests
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG v3.16.0  # Stable modern Catch2 v3 release
)
FetchContent_MakeAvailable(Catch2)

# List of source files for the project
set(SOURCE_FILES
    src/main.cpp
    src/WafSocket.cpp
)

# Define production executable name and source files used to build it
add_executable(WAF_Server ${SOURCE_FILES})
# Add include folder to compiler's search path for header files
target_include_directories(WAF_Server PRIVATE include)

# Build test runner using Catch2's pre-compiled 'int main()'
set(TEST_SOURCES
    tests/test_main.cpp
    src/WafSocket.cpp
)
# Define Test executable with source files used
add_executable(WAF_Tests ${TEST_SOURCES})
target_include_directories(WAF_Tests PRIVATE include)

# Injects Catch2's main engine into Waf_Tests for easy testing
target_link_libraries(WAF_Tests PRIVATE Catch2::Catch2WithMain)