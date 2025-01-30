cmake_minimum_required (VERSION 3.21.1)

#пока только под андроид
project(mobile-bundle)

add_subdirectory (WavConverterLib)
add_subdirectory (PangaeaBackend)
if(ANDROID OR IOS)
add_subdirectory (pangaea-mobile)
else()

endif()
