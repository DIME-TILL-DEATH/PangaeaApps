cmake_minimum_required(VERSION 3.16)
project(PangaeaBackend VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_QML_GENERATE_QMLLS_INI ON)

find_package(Qt6 REQUIRED COMPONENTS Core Quick)

if(NOT IOS)
    set(USB_INTERFACE_SOURCES
        interfaces/usbinterface.cpp interfaces/usbinterface.h
    )
endif()

if(ANDROID)
    set(PLATFORM_SPECIFIC_SOURCES
        utils/androidutils.cpp utils/androidutils.h
    )
endif()

qt_add_library(PangaeaBackend STATIC
    cores/core.cpp cores/core.h
    cores/netcore.cpp cores/netcore.h
    cores/interfacemanager.cpp cores/interfacemanager.h

    interfaces/abstractinterface.cpp interfaces/abstractinterface.h
    interfaces/bleinterface.cpp interfaces/bleinterface.h
    # interfaces/usbinterface.cpp interfaces/usbinterface.h
    interfaces/offlineinterface.h interfaces/offlineinterface.cpp

    device/defines.h

    device/abstractdevice.h device/abstractdevice.cpp
    device/deviceanswer.cpp device/deviceanswer.h
    device/deviceparameter.h device/deviceparameter.cpp
    device/firmware.cpp device/firmware.h
    device/presetabstract.h device/presetabstract.cpp
    device/irfile.h device/irfile.cpp
    device/cp/presetlegacy.cpp device/cp/presetlegacy.h
    device/cp/presetmodern.h device/cp/presetmodern.cpp
    device/cp/cplegacy.h device/cp/cplegacy.cpp
    device/cp/cpmodern.h device/cp/cpmodern.cpp
    device/cp/lapreamp.h device/cp/lapreamp.cpp
    device/cp100fx/cp100fx.h device/cp100fx/cp100fx.cpp
    device/cp100fx/presetfx.h device/cp100fx/presetfx.cpp
    device/cp100fx/fswfx.h device/cp100fx/fswfx.cpp
    device/cp100fx/systemsettingsfx.h device/cp100fx/systemsettingsfx.cpp
    device/cp100fx/controllerfx.h device/cp100fx/controllerfx.cpp

    device/cp/hardwareclassicpreset.h
    device/cp/hardwareclassicpreset.cpp
    device/cp100fx/hardwarefxpreset.h

    models/presetlistmodel.cpp models/presetlistmodel.h
    models/deviceslistmodel.cpp models/deviceslistmodel.h
    models/moduleslistmodel.h models/moduleslistmodel.cpp
    models/filebrowsermodel.h models/filebrowsermodel.cpp

    utils/irworker.cpp utils/irworker.h
    utils/presetmanager.cpp utils/presetmanager.h
    utils/maskedparser.cpp utils/maskedparser.h
    utils/parser.h utils/parser.cpp

    mock/abstractmockdevice.h mock/abstractmockdevice.cpp
    mock/mockcp16legacy.h mock/mockcp16legacy.cpp
    mock/mockcp16modern.h mock/mockcp16modern.cpp
    mock/mockla3.h mock/mockla3.cpp
    mock/mockcp100fx.h mock/mockcp100fx.cpp

    ${USB_INTERFACE_SOURCES}
    ${PLATFORM_SPECIFIC_SOURCES}
)

qt_add_qml_module(PangaeaBackend
    URI PangaeaBackend
    VERSION 1.0
    RESOURCE_PREFIX /

    SOURCES

        uihelpers/devicetypeenum.h
        uihelpers/deviceerrorenum.h
        uihelpers/devicemessageenum.h
        uihelpers/filtertypeenum.h
        uihelpers/moduletypeenum.h
        uihelpers/presetstateenum.h
        uihelpers/uimessagetype.h

        interfaces/devicedescription.cpp interfaces/devicedescription.h

        modules/abstractmodule.cpp modules/abstractmodule.h
        modules/cabsim.h modules/cabsim.cpp
        modules/chorus.h modules/chorus.cpp
        modules/compressor.h modules/compressor.cpp
        modules/controlvalue.cpp modules/controlvalue.h
        modules/controlqlegacy.h modules/controlqlegacy.cpp
        modules/delay.h modules/delay.cpp
        modules/dualcabsim.h modules/dualcabsim.cpp
        modules/earlyreflections.h modules/earlyreflections.cpp
        modules/eqband.cpp modules/eqband.h
        modules/eqparametric.cpp modules/eqparametric.h
        modules/flanger.cpp modules/flanger.h
        modules/hipassfilter.h modules/hipassfilter.cpp
        modules/lowpassfilter.h modules/lowpassfilter.cpp
        modules/noisegate.h modules/noisegate.cpp
        modules/phaser.h modules/phaser.cpp
        modules/poweramp.cpp modules/poweramp.h
        modules/preamp.h modules/preamp.cpp
        modules/presence.h modules/presence.cpp
        modules/reverb.h modules/reverb.cpp
        modules/resonancefilter.h modules/resonancefilter.cpp
        modules/tremolo.h modules/tremolo.cpp
        modules/volume.h modules/volume.cpp
        modules/mastereq.h modules/mastereq.cpp
        modules/tuner.h modules/tuner.cpp
)


target_include_directories(PangaeaBackend PUBLIC
    cores
    device
    device/cp100fx
    device/cp
    interfaces
    mock
    modules
    models
    uihelpers
    utils

    ../WavConverterLib/
)


if(NOT IOS)
    find_package(Qt6 REQUIRED COMPONENTS Bluetooth Network Positioning SerialPort Concurrent)
    target_link_libraries(PangaeaBackend PUBLIC
        Qt6::Core
        Qt6::Bluetooth
        Qt6::Network
        Qt6::Positioning
        Qt6::SerialPort
        Qt6::Concurrent
        Qt6::Quick
    )
else()
    find_package(Qt6 REQUIRED COMPONENTS Bluetooth Network Positioning Concurrent)
    target_link_libraries(PangaeaBackend PUBLIC
        Qt6::Core
        Qt6::Bluetooth
        Qt6::Network
        Qt6::Positioning
        Qt6::Concurrent
        Qt6::Quick
    )
endif()

if(ANDROID)
    target_link_libraries(PangaeaBackend PUBLIC Qt::CorePrivate)
endif()
