cmake_minimum_required(VERSION 3.20)

project(WAF)

set(SOURCE_FILES
    src/main.cpp
    src/WafSocket.cpp
)

add_executable(WAF ${SOURCE_FILES})

target_include_directories(WAF PRIVATE include)