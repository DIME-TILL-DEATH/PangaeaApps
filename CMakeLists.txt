cmake_minimum_required (VERSION 3.21.1)

project(PangaeaApp)

add_subdirectory (WavConverterLib)
if(NOT ANDROID AND NOT IOS)
add_subdirectory (WavConverterShell)
endif()
add_subdirectory (PangaeaBackend)
add_subdirectory (PangaeaFrontend)

