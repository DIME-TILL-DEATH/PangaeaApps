cmake_minimum_required(VERSION 3.21)

project(TestModules LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test Core)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Test for AbstractModule
add_executable(TestAbstractModule
    tst_abstractmodule.cpp
    # ${PANGAEA_BACKEND_DIR}/modules/abstractmodule.cpp
    # ${PANGAEA_BACKEND_DIR}/modules/abstractmodule.h
    # ${PANGAEA_BACKEND_DIR}/device/deviceparameter.cpp
    # ${PANGAEA_BACKEND_DIR}/device/deviceparameter.h
)

target_include_directories(TestAbstractModule PUBLIC
     ${PANGAEA_BACKEND_DIR}/
     ${PANGAEA_BACKEND_DIR}/cores
     ${PANGAEA_BACKEND_DIR}/device
     ${PANGAEA_BACKEND_DIR}/interfaces/
     ${PANGAEA_BACKEND_DIR}/utils
     ${PANGAEA_BACKEND_DIR}/qmlEnums
     ${PANGAEA_BACKEND_DIR}/models/
     ${PANGAEA_BACKEND_DIR}/uihelpers/
     ${PANGAEA_BACKEND_DIR}/utils/
)

target_link_libraries(TestAbstractModule
    Qt6::Test
    Qt6::Core

    PangaeaBackend
    converter-lib
)

add_test(NAME TestAbstractModule COMMAND TestAbstractModule)
