cmake_minimum_required(VERSION 3.21.1)
project(pangaea-backend VERSION 1.0.0 LANGUAGES CXX)

message(VERBOSE "Architecture: ${ANDROID_TARGET_ARCH}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DCP16_FIRMWARE_VERSION="1.04.03")
add_definitions(-DCP16PA_FIRMWARE_VERSION="1.04.03")
add_definitions(-DCP100_FIRMWARE_VERSION="2.08.02")
add_definitions(-DCP100PA_FIRMWARE_VERSION="6.08.04")

find_package(Qt6 REQUIRED COMPONENTS Core)

qt_add_library(pangaea-backend STATIC
    cores/core.cpp cores/core.h
    cores/netcore.cpp cores/netcore.h
    cores/interfacecore.cpp cores/interfacecore.h

    interfaces/abstractinterface.cpp interfaces/abstractinterface.h
    interfaces/bleinterface.cpp interfaces/bleinterface.h
    interfaces/usbinterface.cpp interfaces/usbinterface.h
    interfaces/devicedescription.cpp interfaces/devicedescription.h

    device/answerworker.cpp device/answerworker.h
    device/defines.h
    device/device.cpp device/device.h
    device/deviceanswer.cpp device/deviceanswer.h
    device/firmware.cpp device/firmware.h
    device/preset.cpp device/preset.h
    device/deviceparameter.h device/deviceparameter.cpp

    models/presetlistmodel.cpp models/presetlistmodel.h
    models/deviceslistmodel.cpp models/deviceslistmodel.h

    moduls/abstractmodule.cpp moduls/abstractmodule.h
    moduls/eqband.cpp moduls/eqband.h
    moduls/eqresponse.cpp moduls/eqresponse.h

    utils/androidutils.cpp utils/androidutils.h
    utils/irworker.cpp utils/irworker.h
    utils/presetmanager.cpp utils/presetmanager.h
    utils/parser.cpp utils/parser.h

    uihelpers/controlvalue.cpp uihelpers/controlvalue.h
    uihelpers/devicetypeenum.h
)

target_include_directories(pangaea-backend PUBLIC
    cores
    device
    interfaces
    moduls
    models
    uihelpers
    utils
)

find_package(Qt6 REQUIRED COMPONENTS Bluetooth Network Positioning SerialPort)
target_link_libraries(pangaea-backend PUBLIC
    Qt6::Core
    Qt6::Bluetooth
    Qt6::Network
    Qt6::Positioning
    Qt6::SerialPort
)

if(ANDROID)
    target_link_libraries(pangaea-backend PUBLIC Qt::CorePrivate)
endif()
