QT -= gui
QT += core

TEMPLATE = lib
DEFINES += WAVCONVERTERLIB_LIBRARY

CONFIG += c++17

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    resampler.cpp \
    wavutils.cpp

HEADERS += \
    resampler.h \
    wavutils.h

INCLUDEPATH += sox_lib/include/


linux-g++: LIBS += -lsox$${LIB_SUFFIX}


destIncludeDir = $${PWD}/../shared_libs/include
includeFile = $${PWD}/resampler.h

win32 {
    libsPath = $${PWD}/sox_lib/lib.win32_mingw
    DESTDIR = $$PWD/../shared_libs/lib.win32
    LIBS += -L$$libsPath -lsox-3

    destLibDir = $${DESTDIR}

    includeFile ~= s,/,\\,g
    libsPath ~= s,/,\\,g
    destIncludeDir ~= s,/,\\,g
    destLibDir ~= s,/,\\,g

    !exists($${destIncludeDir}) {QMAKE_POST_LINK += $$QMAKE_MKDIR $$shell_quote($${destIncludeDir}) $$escape_expand(\\n\\t)}
    QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($${includeFile}) $$shell_quote($${destIncludeDir}) $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($${libsPath}) $$shell_quote($${destLibDir}) $$escape_expand(\\n\\t)
}

mac {
    libsPath = $${PWD}/sox_lib/lib.mac

    DESTDIR = $$PWD/../shared_libs/lib.mac
    LIBS += -L$$libsPath -lsox

    QMAKE_POST_LINK += mkdir -p $${destIncludeDir} $$escape_expand(\n\t)
    QMAKE_POST_LINK += cp $${includeFile} $${destIncludeDir} $$escape_expand(\n\t)
}
