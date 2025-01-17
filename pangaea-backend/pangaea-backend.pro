QT -= gui
QT += core
QT += serialport
QT += bluetooth

android{
    QT += positioning
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
    cores/interfacecore.cpp \
    cores/netcore.cpp \
    device/answerworker.cpp \
    device/device.cpp \
    device/deviceanswer.cpp \
    device/deviceparameter.cpp \
    device/firmware.cpp \
    device/preset.cpp \
    interfaces/abstractinterface.cpp \
    interfaces/devicedescription.cpp \
    interfaces/bleinterface.cpp \
    interfaces/usbinterface.cpp \
    models/deviceslistmodel.cpp \
    models/presetlistmodel.cpp \
    moduls/abstractmodule.cpp \
    moduls/eqband.cpp \
    moduls/eqresponse.cpp \
    moduls/poweramp.cpp \
    uihelpers/controlvalue.cpp \
    uihelpers/appproperties.cpp \
    uihelpers/deviceproperties.cpp \
    utils/irworker.cpp \
    utils/parser.cpp \
    utils/presetmanager.cpp

HEADERS += \
    cores/core.h \
    cores/interfacecore.h \
    cores/netcore.h \
    device/answerworker.h \
    device/defines.h \
    device/device.h \
    device/deviceanswer.h \
    device/deviceparameter.h \
    device/firmware.h \
    device/preset.h \
    interfaces/abstractinterface.h \
    interfaces/devicedescription.h \
    interfaces/bleinterface.h \
    interfaces/usbinterface.h \
    models/deviceslistmodel.h \
    models/presetlistmodel.h \
    moduls/abstractmodule.h \
    moduls/eqband.h \
    moduls/eqresponse.h \
    moduls/poweramp.h \
    uihelpers/appproperties.h \
    uihelpers/controlvalue.h \
    uihelpers/deviceproperties.h \
    uihelpers/devicetypeenum.h \
    utils/irworker.h \
    utils/parser.h \
    utils/presetmanager.h

INCLUDEPATH += cores
INCLUDEPATH += device
INCLUDEPATH += interfaces
INCLUDEPATH += models
INCLUDEPATH += moduls
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
