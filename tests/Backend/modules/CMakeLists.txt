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
    ${PANGAEA_BACKEND_DIR}/modules/abstractmodule.cpp
    ${PANGAEA_BACKEND_DIR}/modules/abstractmodule.h
    ${PANGAEA_BACKEND_DIR}/device/deviceparameter.cpp
    ${PANGAEA_BACKEND_DIR}/device/deviceparameter.h
)

target_link_libraries(TestAbstractModule
    PRIVATE Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Core
)

add_test(NAME TestAbstractModule COMMAND TestAbstractModule)
