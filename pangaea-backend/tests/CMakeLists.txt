cmake_minimum_required(VERSION 3.16)

project(TestLegacyDevice LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(TestLegacyDevice
    tst_legacydevice.cpp

    ../threadcontroller.cpp ../threadcontroller.h

    ../cores/core.cpp ../cores/core.h
    # ../cores/netcore.cpp ../cores/netcore.h
    # ../cores/interfacecore.cpp ../cores/interfacecore.h

    # ../interfaces/abstractinterface.cpp ../interfaces/abstractinterface.h
    # ../interfaces/bleinterface.cpp ../interfaces/bleinterface.h
    # ../interfaces/usbinterface.cpp ../interfaces/usbinterface.h
    ../interfaces/devicedescription.cpp ../interfaces/devicedescription.h
    # ../interfaces/offlineinterface.h ../interfaces/offlineinterface.cpp


    ../device/defines.h

    ../device/deviceanswer.cpp ../device/deviceanswer.h
    ../device/firmware.cpp ../device/firmware.h
    ../device/preset.cpp ../device/preset.h
    ../device/deviceparameter.h ../device/deviceparameter.cpp
    ../device/abstractdevice.h ../device/abstractdevice.cpp
    ../device/cplegacy.h ../device/cplegacy.cpp
    ../device/cpmodern.h ../device/cpmodern.cpp
    ../device/lapreamp.h ../device/lapreamp.cpp

    ../models/presetlistmodel.cpp ../models/presetlistmodel.h
    ../models/deviceslistmodel.cpp ../models/deviceslistmodel.h
    ../models/moduleslistmodel.h ../models/moduleslistmodel.cpp

    ../modules/controlvalue.cpp ../modules/controlvalue.h
    ../modules/abstractmodule.cpp ../modules/abstractmodule.h
    ../modules/presetvolume.h ../modules/presetvolume.cpp
    ../modules/noisegate.h ../modules/noisegate.cpp
    ../modules/compressor.h ../modules/compressor.cpp
    ../modules/preamp.h ../modules/preamp.cpp
    ../modules/poweramp.cpp ../modules/poweramp.h
    ../modules/cabsim.h ../modules/cabsim.cpp
    ../modules/hipassfilter.h ../modules/hipassfilter.cpp
    ../modules/eqband.cpp ../modules/eqband.h
    ../modules/eqparametric.cpp ../modules/eqparametric.h
    ../modules/lowpassfilter.h ../modules/lowpassfilter.cpp
    ../modules/earlyreflections.h ../modules/earlyreflections.cpp
    ../modules/controlqlegacy.h ../modules/controlqlegacy.cpp

    # ../utils/androidutils.cpp ../utils/androidutils.h
    ../utils/irworker.cpp ../utils/irworker.h
    ../utils/presetmanager.cpp ../utils/presetmanager.h
    ../utils/maskedparser.cpp ../utils/maskedparser.h
    ../utils/parser.h ../utils/parser.cpp

    ../uihelpers/devicetypeenum.h
    ../uihelpers/deviceerrorenum.h
    ../uihelpers/devicemessageenum.h
    ../uihelpers/moduletypeenum.h
    ../uihelpers/presetstateenum.h
    ../uihelpers/deviceclassenum.h

    ../mock/abstractmockdevice.h ../mock/abstractmockdevice.cpp
    ../mock/mockcp16legacy.h ../mock/mockcp16legacy.cpp
    ../mock/mockcp16modern.h ../mock/mockcp16modern.cpp
)
add_test(NAME LegacyDevice COMMAND LegacyDevice)

target_include_directories(TestLegacyDevice PUBLIC
    ../
    ../cores
    ../device
    ../interfaces
    ../mock
    ../modules
    ../models
    ../uihelpers
    ../utils
)

find_package(Qt6 REQUIRED COMPONENTS Bluetooth Gui Network Positioning Quick QuickControls2 LinguistTools Svg SerialPort)
target_link_libraries(TestLegacyDevice PRIVATE
    Qt6::Test
    Qt6::Core
    Qt6::Bluetooth
    Qt6::SerialPort
    Qt6::Gui
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Svg
    Qt6::Positioning
)

if(ANDROID)
    target_link_libraries(TestLegacyDevice PUBLIC Qt::CorePrivate)

    set_target_properties(TestLegacyDevice PROPERTIES
        QT_ANDROID_VERSION_NAME ${PROJECT_VERSION}
        QT_ANDROID_VERSION_CODE ${VERSION_CODE}
        QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
    )
endif()
