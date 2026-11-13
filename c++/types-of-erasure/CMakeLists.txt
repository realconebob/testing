cmake_minimum_required(VERSION 3.22.1)

project(types-of-erasure
    VERSION 0.1
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# add_library(crtpf INTERFACE
#     crtp.hpp thingers.hpp
# )
# target_include_directories(crtpf INTERFACE .)

add_executable(standard)
target_sources(standard
    PRIVATE
    standard.cpp
)

add_executable(half-template)
target_sources(half-template
    PRIVATE
    half-template.cpp
)

add_executable(full-crtp)
target_sources(full-crtp
    PRIVATE
    full-crtp.cpp
)

target_compile_features(standard PUBLIC cxx_std_20)
target_compile_features(half-template PUBLIC cxx_std_20)
target_compile_features(full-crtp PUBLIC cxx_std_20)
# target_link_libraries(main PRIVATE crtpf)