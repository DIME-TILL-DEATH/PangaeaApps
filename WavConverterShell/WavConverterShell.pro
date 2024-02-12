TEMPLATE = app

QT += quick
QT += quickcontrols2
#QT += widgets

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

INCLUDEPATH += $${PWD}/../WavConverterLib
DEPENDPATH += $${PWD}/../WavConverterLib

win32:CONFIG(release, debug|release): LIBS += -L$${OUT_PWD}/../WavConverterLib/release/ -lWavConverterLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$${OUT_PWD}/../WavConverterLib/debug/ -lWavConverterLib
else:unix: LIBS += -L$${OUT_PWD}/../WavConverterLib/ -lWavConverterLib

win32{
    DESTDIR = $${PWD}\output_bin\

    RC_ICONS = $${PWD}\icons\spirit.ico

    DISTFILES += \
        icons/spirit.ico \
        icons/suhr.ico

# нужен ли???
    libsPath = $${PWD}/../shared_libs/lib.msvc2019
}

mac{

    DISTFILES += \
        icons/spirit.icns \
        icons/suhr.icns \

    ICON = icons/spirit.icns

    libsPath = $${OUT_PWD}/../WavConverterLib/
    LIBS += -L$${OUT_PWD}/../WavConverterLib/ -lWavConverterLib

    QMAKE_POST_LINK += macdeployqt $${DESTDIR}$${TARGET}.app -qmldir=$${PWD}/ -libpath=$${libsPath} $$escape_expand(\\n\\t)
}

linux{
    libsPath = $${PWD}/../sox_lib/lib.linux/
    LIBS += -L$${libsPath} -lsox

    destDir = $${PWD}/output_bin/

    QMAKE_POST_LINK += mkdir -p $${destDir} $$escape_expand(\n\t)
    QMAKE_POST_LINK += cp -pr $${OUT_PWD}/$${TARGET} $${destDir} $$escape_expand(\n\t)
}
