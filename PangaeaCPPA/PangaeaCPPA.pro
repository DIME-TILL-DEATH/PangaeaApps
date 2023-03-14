TARGET   = PangaeaCPPA
TEMPLATE = app

QT += quick
QT += quickcontrols2
QT += qml
QT += svg
QT += serialport
QT += bluetooth
QT += widgets

DEFINES += PANGAEA_DESKTOP

win32: {
    VERSION = 1.0.1714.1

    QMAKE_TARGET_COMPANY = AMT electronics
    QMAKE_TARGET_COPYRIGHT = AMT electronics

    RC_ICONS = icons/pangaea_amt.ico
}
else: VERSION = 1.0.1714

CONFIG += c++11

SOURCES += main.cpp \
    cores/core.cpp \
    cores/interfacecore.cpp \
    cores/netcore.cpp \
    device/answerworker.cpp \
    device/device.cpp \
    device/deviceanswer.cpp \
    device/devicecontrols.cpp \
    device/deviceparameter.cpp \
    device/firmware.cpp \
    device/preset.cpp \
    models/interfacelistmodel.cpp \
    models/presetlistmodel.cpp \
    interfaces/abstractinterface.cpp \
    interfaces/devicedescription.cpp \
    interfaces/bleinterface.cpp \
    interfaces/usbinterface.cpp \
    threadcontroller.cpp \
    ui/uidesktopcore.cpp \
    ui/uiinterfacemanager.cpp \
    ui/uisettings.cpp \
    utils/irworker.cpp \
    utils/logger.cpp \
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
    device/devicecontrols.h \
    device/deviceparameter.h \
    device/firmware.h \
    device/preset.h \
    models/interfacelistmodel.h \
    models/presetlistmodel.h \
    interfaces/abstractinterface.h \
    interfaces/devicedescription.h \
    interfaces/bleinterface.h \
    interfaces/usbinterface.h \
    qmlEnums/devicetypeenum.h \
    threadcontroller.h \
    ui/uidesktopcore.h \
    ui/uiinterfacemanager.h \
    ui/uisettings.h \
    utils/irworker.h \
    utils/logger.h \
    utils/parser.h \
    utils/presetmanager.h

INCLUDEPATH += cores
INCLUDEPATH += device
INCLUDEPATH += interfaces
INCLUDEPATH += models
INCLUDEPATH += qmlEnums
INCLUDEPATH += ui
INCLUDEPATH += utils

TRANSLATIONS += translations/PangaeaCPPA_en.ts
TRANSLATIONS += translations/PangaeaCPPA_ru.ts
TRANSLATIONS += translations/PangaeaCPPA_de.ts
TRANSLATIONS += translations/PangaeaCPPA_it.ts

macx {
    QMAKE_INFO_PLIST = $$PWD/MacOS/Info.plist
    ICON = icons/pangaea.icns

    libsPath = $${PWD}/../shared_libs/lib.mac
}

win32-g++: libsPath = $${PWD}/../shared_libs/lib.mingw32
win32-msvc: libsPath = $${PWD}/../shared_libs/lib.msvc2019

linux {
    QMAKE_INFO_PLIST = $$PWD/MacOS/Info.plist
    ICON = icons/pangaea.icns

    libsPath = $${PWD}/../shared_libs/lib.linux
    LIBS += -L$${libsPath} -lsox
}

INCLUDEPATH += $${PWD}/../shared_libs/include
LIBS += -L$${libsPath} -lWavConverterLib

DEFINES += CP16_FIRMWARE_VERSION=\\\"1.04.01\\\"
DEFINES += CP16PA_FIRMWARE_VERSION=\\\"1.04.01\\\"
DEFINES += CP100_FIRMWARE_VERSION=\\\"2.08.02\\\"
DEFINES += CP100PA_FIRMWARE_VERSION=\\\"6.08.04\\\"

QML_IMPORT_PATH += $$PWD/qml/
QML2_IMPORT_PATH += $$PWD/qml/

RESOURCES += qml.qrc \
    translations.qrc

DISTFILES += \
    MacOS/Info.plist \
    icons/pangaea_amt.ico \

dirDocs = $${PWD}/docs/

CONFIG(release, debug|release) {
    win32 {
        appBinaryFile = release/$${TARGET}.exe
        converterBinaryFile = $${PWD}/../WavConverterShell/output_bin/IrConverter.exe
        dirDeploy = $${PWD}/../deploy_windows
        dirDeployRelease = $${dirDeploy}/release

        # replace slashes in source path for Windows
        appBinaryFile ~= s,/,\\,g
        converterBinaryFile ~= s,/,\\,g
        libsPath ~= s,/,\\,g
        dirDocs ~= s,/,\\,g
        dirDeploy ~= s,/,\\,g
        dirDeployRelease ~= s,/,\\,g

        QMAKE_POST_LINK += $$[QT_INSTALL_BINS]/windeployqt.exe --force release/$${TARGET}.exe --qmldir=$${PWD}/qml/ --dir $$shell_quote($$dirDeployRelease) $$escape_expand(\\n\\t)# $$RETURN
        QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($${appBinaryFile}) $$shell_quote($${dirDeployRelease}) $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($${converterBinaryFile}) $$shell_quote($${dirDeployRelease}) $$escape_expand(\\n\\t)
        win32-g++: QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($${libsPath}) $$shell_quote($${dirDeployRelease}) $$escape_expand(\\n\\t)
        win32-msvc: QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($${libsPath}\\WavConverterLib.dll) $$shell_quote($${dirDeployRelease}) $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($${dirDocs}) $$shell_quote($${dirDeployRelease}\\docs\\) $$escape_expand(\\n\\t)

        #place version in template file and create Inno setup installer
        QMAKE_POST_LINK += powershell.exe $$shell_quote(((Get-Content $${dirDeploy}/template_script.iss) -replace  \'MyAppVersionREPLACE\',  \'MyAppVersion \"$${VERSION}\"\' | Out-file $${dirDeploy}/installer_script.iss -encoding ascii)) $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += ISCC.exe /O$${dirDeploy}/bin $${dirDeploy}/installer_script.iss $$escape_expand(\\n\\t)
    }

    macx {
        converterBinary = $${PWD}/../WavConverterShell/output_bin/IrConverter.app/Contents/MacOS/IrConverter

        QMAKE_POST_LINK += cp -r $${converterBinary} PangaeaCPPA.app/Contents/MacOS/ $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += cp -r $${dirDocs} PangaeaCPPA.app/Contents/MacOS/docs $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += macdeployqt $${DESTDIR}$${TARGET}.app -qmldir=$${PWD}/qml/ -libpath=$${libsPath} -dmg $$escape_expand(\\n\\t)
    }

    linux{
        appBinaryFile = $${TARGET}
        converterBinaryFile = $${PWD}/../WavConverterShell/output_bin/IrConverter
        dirDeploy = $${PWD}/../deploy_linux

        QMAKE_POST_LINK += mkdir -p $${dirDeploy}/bin/ $$escape_expand(\n\t)
        QMAKE_POST_LINK += cp -r $${converterBinaryFile} $${dirDeploy}/bin/ $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += cp -r $${dirDocs} $${dirDeploy}/ $$escape_expand(\\n\\t)

        # using cqtdeployer, installed from snap
        QMAKE_POST_LINK += cqtdeployer -bin PangaeaCPPA -targetDir $${dirDeploy} -libDir $${libsPath} -qmlDir $${PWD}/qml/ -qmake ~/Qt/6.4.2/gcc_64/bin/qmake $$escape_expand(\\n\\t)
    }
}
