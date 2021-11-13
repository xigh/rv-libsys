cmake_minimum_required(VERSION 2.80)

project(libsys)

add_library(libsys src/file.c)

target_include_directories(libsys PUBLIC include)
