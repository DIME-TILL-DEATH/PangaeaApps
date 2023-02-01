QT += quick

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = IrConverter


QMAKE_TARGET_COMPANY = AMT electronics
QMAKE_TARGET_COPYRIGHT = AMT electronics

SOURCES += \
        core.cpp \
        main.cpp

HEADERS += \
    core.h

RESOURCES += qml.qrc



win32:{
    DESTDIR = $${PWD}\..\output_bin\

    RC_ICONS = $${PWD}\icons\spirit.ico

    DISTFILES += \
        icons/spirit.ico \
        icons/suhr.ico

    libsPath = $${PWD}/../../shared_libs/lib.win32
}

mac:{

    DISTFILES += \
        icons/spirit.icns \
        icons/suhr.icns \

    ICON = icons/spirit.icns

    libsPath = $${PWD}/../../shared_libs/lib.mac

    destDir = $${PWD}/../output_bin/
    # DESTDIR does not allow to run program without copying QT librarys
#    QMAKE_POST_LINK += macdeployqt $${DESTDIR}$${TARGET}.app -qmldir=$${PWD}/ -libpath=$${libsPath} $$escape_expand(\\n\\t)

    QMAKE_POST_LINK += mkdir -p $${destDir} $$escape_expand(\n\t)
    QMAKE_POST_LINK += cp -pr $${OUT_PWD}/$${TARGET}.app $${destDir} $$escape_expand(\n\t)

}

INCLUDEPATH += $${PWD}/../../shared_libs/include
LIBS += -L$${libsPath} -lWavConverterLib



