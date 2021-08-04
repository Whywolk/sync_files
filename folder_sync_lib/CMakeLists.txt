cmake_minimum_required(VERSION 3.20)
project(folder_sync_lib)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_LIB folder_sync.cpp)

add_library(folder_sync_lib STATIC ${SOURCE_LIB})
