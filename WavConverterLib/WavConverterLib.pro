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

INCLUDEPATH += sox_lib/include/


linux-g++: LIBS += -lsox$${LIB_SUFFIX}


destIncludeDir = $${PWD}/../shared_libs/include
includeFile = $${PWD}/resampler.h

win32-g++ {
    message("MinGW 32 compile")
    libsPath = $${PWD}/sox_lib/lib.mingw32    
    LIBS += -L$${libsPath} -lsox-3

    DESTDIR = $$PWD/../shared_libs/lib.mingw32
}

win32-msvc {
   message("msvc2019 compile")

    libsPath = $${PWD}/sox_lib/lib.msvc2019/
    LIBS += -L$${libsPath} -llibsox
    LIBS += -L$${libsPath} -lliblpc10
    LIBS += -L$${libsPath} -llibgsm
    DEPENDPATH += libsPath

    DESTDIR = $$PWD/../shared_libs/lib.msvc2019
}

win32 {
    destLibDir = $${DESTDIR}

    includeFile ~= s,/,\\,g
    libsPath ~= s,/,\\,g
    destIncludeDir ~= s,/,\\,g
    destLibDir ~= s,/,\\,g

    !exists($${destIncludeDir}) {QMAKE_POST_LINK += $$QMAKE_MKDIR $$shell_quote($${destIncludeDir}) $$escape_expand(\\n\\t)}
    QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($${includeFile}) $$shell_quote($${destIncludeDir}) $$escape_expand(\\n\\t)
    win32-g++: QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($${libsPath}) $$shell_quote($${destLibDir}) $$escape_expand(\\n\\t)
}

mac {
    libsPath = $${PWD}/sox_lib/lib.mac

    DESTDIR = $$PWD/../shared_libs/lib.mac
    LIBS += -L$$libsPath -lsox

    QMAKE_POST_LINK += mkdir -p $${destIncludeDir} $$escape_expand(\n\t)
    QMAKE_POST_LINK += cp $${includeFile} $${destIncludeDir} $$escape_expand(\n\t)
}


