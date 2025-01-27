QT -= gui
QT += core
QT += serialport
QT += bluetooth
QT += positioning
QT += concurrent

android{
    QT += core-private
}

CONFIG += c++17

TEMPLATE = lib
CONFIG += staticlib

DEFINES += PANGAEA_LIBRARY
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += CP16_FIRMWARE_VERSION=\\\"RV.1.04.03\\\"
DEFINES += CP16PA_FIRMWARE_VERSION=\\\"PA.1.04.03\\\"
DEFINES += CP100_FIRMWARE_VERSION=\\\"RV.2.08.02\\\"
DEFINES += CP100PA_FIRMWARE_VERSION=\\\"PA.6.09.04\\\"

SOURCES += \
    cores/core.cpp \
    cores/netcore.cpp \
    cores/interfacecore.cpp \
    interfaces/abstractinterface.cpp \
    interfaces/bleinterface.cpp \
    interfaces/usbinterface.cpp \
    interfaces/devicedescription.cpp \
    interfaces/offlineinterface.cpp \
    device/deviceanswer.cpp \
    device/firmware.cpp \
    device/presetlegacy.cpp \
    device/presetmodern.cpp \
    device/presetabstract.cpp \
    device/irfile.cpp \
    device/deviceparameter.cpp \
    device/abstractdevice.cpp \
    device/cplegacy.cpp \
    device/cpmodern.cpp \
    device/lapreamp.cpp \
    device/hardwarepreset.cpp \
    models/presetlistmodel.cpp \
    models/deviceslistmodel.cpp \
    models/moduleslistmodel.cpp \
    modules/controlvalue.cpp \
    modules/controlqlegacy.cpp \
    modules/abstractmodule.cpp \
    modules/presetvolume.cpp \
    modules/noisegate.cpp \
    modules/compressor.cpp \
    modules/preamp.cpp \
    modules/poweramp.cpp \
    modules/presence.cpp \
    modules/hipassfilter.cpp \
    modules/eqband.cpp \
    modules/eqparametric.cpp \
    modules/lowpassfilter.cpp \
    modules/cabsim.cpp \
    modules/earlyreflections.cpp \
    modules/delay.cpp \
    modules/tremolo.cpp \
    modules/chorus.cpp \
    modules/phaser.cpp \
    utils/irworker.cpp \
    utils/presetmanager.cpp \
    utils/maskedparser.cpp \
    utils/parser.cpp \
    mock/abstractmockdevice.cpp \
    mock/mockcp16legacy.cpp \
    mock/mockcp16modern.cpp \
    mock/mockla3.cpp

HEADERS += \
    cores/core.h \
    cores/netcore.h \
    cores/interfacecore.h \
    interfaces/abstractinterface.h \
    interfaces/bleinterface.h \
    interfaces/usbinterface.h \
    interfaces/devicedescription.h \
    interfaces/offlineinterface.h \
    device/defines.h \
    device/deviceanswer.h \
    device/firmware.h \
    device/presetlegacy.h \
    device/presetmodern.h \
    device/presetabstract.h \
    device/irfile.h \
    device/deviceparameter.h \
    device/abstractdevice.h \
    device/cplegacy.h \
    device/cpmodern.h \
    device/lapreamp.h \
    device/hardwarepreset.h \
    models/presetlistmodel.h \
    models/deviceslistmodel.h \
    models/moduleslistmodel.h \
    modules/controlvalue.h \
    modules/controlqlegacy.h \
    modules/abstractmodule.h \
    modules/presetvolume.h \
    modules/noisegate.h \
    modules/compressor.h \
    modules/preamp.h \
    modules/poweramp.h \
    modules/presence.h \
    modules/hipassfilter.h \
    modules/eqband.h \
    modules/eqparametric.h \
    modules/lowpassfilter.h \
    modules/cabsim.h \
    modules/earlyreflections.h \
    modules/delay.h \
    modules/tremolo.h \
    modules/chorus.h \
    modules/phaser.h \
    utils/irworker.h \
    utils/presetmanager.h \
    utils/maskedparser.h \
    utils/parser.h \
    uihelpers/devicetypeenum.h \
    uihelpers/deviceerrorenum.h \
    uihelpers/devicemessageenum.h \
    uihelpers/filtertypeenum.h \
    uihelpers/moduletypeenum.h \
    uihelpers/presetstateenum.h \
    uihelpers/uimessagetype.h \
    mock/abstractmockdevice.h \
    mock/mockcp16legacy.h \
    mock/mockcp16modern.h \
    mock/mockla3.h

INCLUDEPATH += cores
INCLUDEPATH += device
INCLUDEPATH += interfaces
INCLUDEPATH += mock
INCLUDEPATH += modules
INCLUDEPATH += models
INCLUDEPATH += uihelpers
INCLUDEPATH += utils

#win32-g++ {
#    message("MinGW 32 build")
#    DESTDIR = $$PWD/../shared_libs/lib.mingw32
#}

#win32-msvc {
#    message("msvc2019 build")
#    DESTDIR = $$PWD/../shared_libs/lib.msvc2019
#}

#linux: !macx{
#    !android{
#        message("linux build")

#        DESTDIR = $$PWD/../shared_libs/lib.linux
#    }

#    android {
#        message("android build")

#        equals(ANDROID_TARGET_ARCH, armeabi-v7a) {
#            DESTDIR = $$PWD/../shared_libs/lib.android/armeabi-v7a
#        }
#        equals(ANDROID_TARGET_ARCH, arm64-v8a) {
#            DESTDIR = $$PWD/../shared_libs/lib.android/arm64-v8a
#        }
#    }
#}

#macx {
#    message("mac build")

#    DESTDIR = $$PWD/../shared_libs/lib.mac
#}

## TODO спрятать внутреннюю структуру backend, уменьшить количество заголовочных наружу
#headersDataFiles.path = $${PWD}/../shared_libs/include
#headersDataFiles.files = $$files($${PWD}/*.h, true) #$$PWD/*/*.h
#win32: headersDataFiles.files ~= s,/,\\,g
#INSTALLS += headersDataFiles # add make install step!!!!
