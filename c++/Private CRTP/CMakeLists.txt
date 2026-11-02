cmake_minimum_required(VERSION 3.22.1)

project(CRTPFun
    VERSION 0.1
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(crtpf INTERFACE
    crtp.hpp thingers.hpp
)
target_include_directories(crtpf INTERFACE .)

add_executable(main)
target_sources(main
    PRIVATE
    main.cpp
)

target_compile_features(main PUBLIC cxx_std_20)
target_link_libraries(main PRIVATE crtpf)