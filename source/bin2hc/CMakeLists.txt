cmake_minimum_required(VERSION 4.1)

project(bin2hc)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
if (MSVC)
    add_compile_options("/utf-8")
endif()


add_executable(bin2hc)
target_sources(bin2hc PRIVATE main.cpp)
target_compile_features(bin2hc PRIVATE cxx_std_17)

if (MINGW)
    target_link_options(bin2hc PRIVATE -static)
endif()