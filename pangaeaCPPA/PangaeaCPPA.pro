TARGET   = PangaeaCPPA
TEMPLATE = app

QT += quick
QT += quickcontrols2
QT += qml quick svg
QT += serialport

win32: {
    VERSION = 1.0.1708.1

    QMAKE_TARGET_COMPANY = AMT electronics
    QMAKE_TARGET_COPYRIGHT = AMT electronics

    RC_ICONS = icons/pangaea_amt.ico
}
else: VERSION = 1.0.1708

CONFIG += c++11

SOURCES += main.cpp \
    cores/core.cpp \
    cores/uidesktopcore.cpp \
    device/answerworker.cpp \
    device/device.cpp \
    device/deviceanswer.cpp \
    device/devicecontrols.cpp \
    device/deviceparameter.cpp \
    device/firmware.cpp \
    device/preset.cpp \
    device/presetlistmodel.cpp \
    hardware/devicedescription.cpp \
    interfaces/abstractinterface.cpp \
    interfaces/usbinterface.cpp \
    threadcontroller.cpp \
    utils/irworker.cpp \
    utils/logger.cpp \
    utils/parser.cpp \
    utils/presetmanager.cpp

HEADERS += \
    cores/core.h \
    cores/uidesktopcore.h \
    device/answerworker.h \
    device/defines.h \
    device/device.h \
    device/deviceanswer.h \
    device/devicecontrols.h \
    device/deviceparameter.h \
    device/firmware.h \
    device/preset.h \
    device/presetlistmodel.h \
    hardware/devicedescription.h \
    interfaces/abstractinterface.h \
    interfaces/usbinterface.h \
    threadcontroller.h \
    utils/irworker.h \
    utils/logger.h \
    utils/parser.h \
    utils/presetmanager.h

INCLUDEPATH += cores
INCLUDEPATH += utils
INCLUDEPATH += interfaces
INCLUDEPATH += hardware
INCLUDEPATH += device

macx {
    QMAKE_INFO_PLIST = $$PWD/MacOS/Info.plist
    ICON = icons/pangaea.icns

    libsPath = $${PWD}/../shared_libs/lib.mac
}

win32 {
    libsPath = $${PWD}/../shared_libs/lib.win32
}

INCLUDEPATH += $${PWD}/../shared_libs/include
LIBS += -L$${libsPath} -lWavConverterLib

DEFINES += CP16_FIRMWARE_VERSION=\\\"1.04.00\\\"
DEFINES += CP16PA_FIRMWARE_VERSION=\\\"1.04.00\\\"
DEFINES += CP100_FIRMWARE_VERSION=\\\"2.08.02\\\"
DEFINES += CP100PA_FIRMWARE_VERSION=\\\"6.08.04\\\"

QML_IMPORT_PATH += $$PWD/qml/
QML2_IMPORT_PATH += $$PWD/qml/

RESOURCES += qml.qrc

DISTFILES += \
    MacOS/Info.plist \
    icons/pangaea_amt.ico \

dirDocs = $${PWD}/docs/

CONFIG(release, debug|release) {
    win32 {
        appBinaryFile = release/$${TARGET}.exe
        converterBinaryFile = $${PWD}/../WavConverterShell/output_bin/IrConverter.exe
        dirDeploy = $${PWD}/../desktop_bundle_output
        dirDeployRelease = $${dirDeploy}/release

        # replace slashes in source path for Windows
        appBinaryFile ~= s,/,\\,g
        converterBinaryFile ~= s,/,\\,g
        libsPath ~= s,/,\\,g
        dirDocs ~= s,/,\\,g
        dirDeploy ~= s,/,\\,g
        dirDeployRelease ~= s,/,\\,g


        QMAKE_POST_LINK += windeployqt.exe  release/$${TARGET}.exe -qmldir=$${PWD}/qml/ --dir $$shell_quote($$dirDeployRelease) $$escape_expand(\\n\\t)# $$RETURN
        QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($${appBinaryFile}) $$shell_quote($${dirDeployRelease}) $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($${converterBinaryFile}) $$shell_quote($${dirDeployRelease}) $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($${libsPath}) $$shell_quote($${dirDeployRelease}) $$escape_expand(\\n\\t)
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
}
