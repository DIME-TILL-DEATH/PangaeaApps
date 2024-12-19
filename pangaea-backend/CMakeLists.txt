cmake_minimum_required(VERSION 3.16)
project(pangaea-backend VERSION 1.0.0 LANGUAGES CXX)

message(VERBOSE "Architecture: ${ANDROID_TARGET_ARCH}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_QML_GENERATE_QMLLS_INI ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

qt_add_library(pangaea-backend STATIC
    cores/core.cpp cores/core.h
    cores/netcore.cpp cores/netcore.h
    cores/interfacecore.cpp cores/interfacecore.h

    interfaces/abstractinterface.cpp interfaces/abstractinterface.h
    interfaces/bleinterface.cpp interfaces/bleinterface.h
    interfaces/usbinterface.cpp interfaces/usbinterface.h
    interfaces/devicedescription.cpp interfaces/devicedescription.h
    interfaces/offlineinterface.h interfaces/offlineinterface.cpp


    device/defines.h

    device/deviceanswer.cpp device/deviceanswer.h
    device/firmware.cpp device/firmware.h
    device/presetlegacy.cpp device/presetlegacy.h
    device/presetmodern.h device/presetmodern.cpp
    device/presetabstract.h device/presetabstract.cpp
    device/irfile.h device/irfile.cpp
    device/deviceparameter.h device/deviceparameter.cpp
    device/abstractdevice.h device/abstractdevice.cpp
    device/cplegacy.h device/cplegacy.cpp
    device/cpmodern.h device/cpmodern.cpp
    device/lapreamp.h device/lapreamp.cpp

    device/hardwarepreset.h
    device/hardwarepreset.cpp

    models/presetlistmodel.cpp models/presetlistmodel.h
    models/deviceslistmodel.cpp models/deviceslistmodel.h
    models/moduleslistmodel.h models/moduleslistmodel.cpp

    modules/controlvalue.cpp modules/controlvalue.h
    modules/controlqlegacy.h modules/controlqlegacy.cpp
    modules/abstractmodule.cpp modules/abstractmodule.h
    modules/presetvolume.h modules/presetvolume.cpp
    modules/noisegate.h modules/noisegate.cpp
    modules/compressor.h modules/compressor.cpp
    modules/preamp.h modules/preamp.cpp
    modules/poweramp.cpp modules/poweramp.h
    modules/presence.h modules/presence.cpp
    modules/hipassfilter.h modules/hipassfilter.cpp
    modules/eqband.cpp modules/eqband.h
    modules/eqparametric.cpp modules/eqparametric.h
    modules/lowpassfilter.h modules/lowpassfilter.cpp
    modules/cabsim.h modules/cabsim.cpp
    modules/earlyreflections.h modules/earlyreflections.cpp

    utils/androidutils.cpp utils/androidutils.h
    utils/irworker.cpp utils/irworker.h
    utils/presetmanager.cpp utils/presetmanager.h
    utils/maskedparser.cpp utils/maskedparser.h
    utils/parser.h utils/parser.cpp

    uihelpers/deviceclassenum.h
    uihelpers/devicetypeenum.h
    uihelpers/deviceerrorenum.h
    uihelpers/devicemessageenum.h
    uihelpers/filtertypeenum.h
    uihelpers/moduletypeenum.h
    uihelpers/presetstateenum.h
    uihelpers/uimessagetype.h

    mock/abstractmockdevice.h mock/abstractmockdevice.cpp
    mock/mockcp16legacy.h mock/mockcp16legacy.cpp
    mock/mockcp16modern.h mock/mockcp16modern.cpp

)

target_include_directories(pangaea-backend PUBLIC
    cores
    device
    interfaces
    mock
    modules
    models
    uihelpers
    utils
)

find_package(Qt6 REQUIRED COMPONENTS Bluetooth Network Positioning SerialPort Concurrent)
target_link_libraries(pangaea-backend PUBLIC
    Qt6::Core
    Qt6::Bluetooth
    Qt6::Network
    Qt6::Positioning
    Qt6::SerialPort
    Qt6::Concurrent
)

if(ANDROID)
    target_link_libraries(pangaea-backend PUBLIC Qt::CorePrivate)
endif()
