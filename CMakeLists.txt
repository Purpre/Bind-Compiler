cmake_minimum_required(VERSION 3.20)

project(Bind)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)

add_executable(Bind ${SOURCES})

target_include_directories(Bind PRIVATE
    include
)