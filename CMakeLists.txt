cmake_minimum_required(VERSION 3.20)
project(sync_files)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_EXE main.cpp)

include_directories(folder_sync_lib)

add_executable(sync_files_con ${SOURCE_EXE})

add_subdirectory(folder_sync_lib)

target_link_libraries(sync_files_con folder_sync_lib)