cmake_minimum_required (VERSION 3.21.1)

project(PangaeaApp)

add_subdirectory (WavConverterLib)
if(NOT ANDROID AND NOT IOS)
    add_subdirectory (WavConverterShell)
    set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Desktop/qml)
else()
    set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Mobile/qml)
endif()
add_subdirectory (PangaeaBackend)
add_subdirectory (PangaeaFrontend)

# option(BUILD_BACKEND_TESTS "Build tests" OFF)
# if(BUILD_BACKEND_TESTS)
#     add_subdirectory(tests/Backend)
# endif()
