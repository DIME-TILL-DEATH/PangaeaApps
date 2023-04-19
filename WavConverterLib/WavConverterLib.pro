QT -= gui
QT += core

TEMPLATE = lib
CONFIG += c++17

DEFINES += WAVCONVERTERLIB_LIBRARY

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    resampler.cpp \
    wavutils.cpp

HEADERS += \
    resampler.h \
    wavutils.h

INCLUDEPATH += $${PWD}/../sox_lib/include/

includeFile = $${PWD}/resampler.h

win32-msvc {
   message("msvc2019 build")

    libsPath = $${PWD}/../sox_lib/lib.msvc2019/
    LIBS += -L$${libsPath} -llibsox
    LIBS += -L$${libsPath} -lliblpc10
    LIBS += -L$${libsPath} -llibgsm
    DEPENDPATH += libsPath
}

linux: !macx{
    !android{
        message("linux build")

        libsPath = $${PWD}/../sox_lib/lib.linux/

        LIBS += -L$$libsPath -lsox
        QMAKE_POST_LINK += ln -sf $${libsPath}/libsox.so $${libsPath}/libsox.so.3 $$escape_expand(\n\t)
    }
}

macx {
    message("mac build")

    libsPath = $${PWD}/sox_lib/lib.mac
    LIBS += -L$$libsPath -lsox
}
