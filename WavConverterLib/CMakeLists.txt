cmake_minimum_required(VERSION 3.21.1)
project(converter-lib VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

qt_add_library(converter-lib SHARED
    resampler.cpp
    wavutils.cpp
)

target_link_libraries(converter-lib PUBLIC
    Qt6::Core

    ${CMAKE_CURRENT_SOURCE_DIR}/../sox_lib/lib.android/${CMAKE_ANDROID_ARCH_ABI}/libsox.so
)

target_include_directories(converter-lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../sox_lib/include/
)
