cmake_minimum_required(VERSION 3.21)

project(TestCores LANGUAGES CXX)

enable_testing()

find_package(Qt6 REQUIRED COMPONENTS Test Core Bluetooth SerialPort Network Positioning)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Test for InterfaceManager
add_executable(TestInterfaceManager
    tst_interfacemanager.cpp
    # ${PANGAEA_BACKEND_DIR}/cores/interfacemanager.cpp
    # ${PANGAEA_BACKEND_DIR}/cores/interfacemanager.h
    # ${PANGAEA_BACKEND_DIR}/interfaces/abstractinterface.cpp
    # ${PANGAEA_BACKEND_DIR}/interfaces/abstractinterface.h
)

target_include_directories(TestInterfaceManager PUBLIC
    ${PANGAEA_BACKEND_DIR}/
    ${PANGAEA_BACKEND_DIR}/cores/
    ${PANGAEA_BACKEND_DIR}/device/
    ${PANGAEA_BACKEND_DIR}/interfaces/
    ${PANGAEA_BACKEND_DIR}/mock/
    ${PANGAEA_BACKEND_DIR}/utils/
    ${PANGAEA_BACKEND_DIR}/qmlEnums/
    ${PANGAEA_BACKEND_DIR}/models/
    ${PANGAEA_BACKEND_DIR}/uihelpers/
)

target_link_libraries(TestInterfaceManager PRIVATE
    Qt6::Test
    Qt6::Core
    Qt6::Bluetooth
    Qt6::Network
    Qt6::Positioning
    Qt6::SerialPort

    PangaeaBackend
    converter-lib
)

add_test(NAME TestInterfaceManager COMMAND TestInterfaceManager)
