TARGET   = PangaeaCPPA
TEMPLATE = app

QT += quick
QT += quickcontrols2
QT += qml
QT += svg
QT += serialport
QT += bluetooth
QT += widgets

win32 {
    VERSION = 1.0.1725.1

    QMAKE_TARGET_COMPANY = AMT electronics
    QMAKE_TARGET_COPYRIGHT = AMT electronics

    RC_ICONS = icons/pangaea_amt.ico
}
else: VERSION = 1.0.1725

macx {
    QMAKE_INFO_PLIST = $$PWD/MacOS/Info.plist
    ICON = icons/pangaea.icns
}

linux {
    QMAKE_INFO_PLIST = $$PWD/MacOS/Info.plist
    ICON = icons/pangaea.icns

    soxLibsPath = $${PWD}/../sox_lib/lib.linux
    LIBS += -L$${soxLibsPath} -lsox

    DEPENDPATH += $${soxLibsPath}
}

INCLUDEPATH += $${PWD}/../WavConverterLib

DEPENDPATH += $${PWD}/../WavConverterLib
DEPENDPATH += $${PWD}/../pangaea-backend

win32{
    CONFIG(debug, debug|release){
        wavConverterLibPath = $${OUT_PWD}/../WavConverterLib/debug/

        LIBS += -L$${OUT_PWD}/../pangaea-backend/debug/ -lpangaea-backend
        PRE_TARGETDEPS += $${OUT_PWD}/../pangaea-backend/debug/pangaea-backend.lib
    }
    CONFIG(release, debug|release){
        wavConverterLibPath = $${OUT_PWD}/../WavConverterLib/release/

        LIBS += -L$${OUT_PWD}/../pangaea-backend/release/ -lpangaea-backend
        PRE_TARGETDEPS += $${OUT_PWD}/../pangaea-backend/release/pangaea-backend.lib
    }
    LIBS += -L$${wavConverterLibPath} -lWavConverterLib
}

unix{
    LIBS += -L$${OUT_PWD}/../WavConverterLib/ -lWavConverterLib
    LIBS += -L$$$${OUT_PWD}/../pangaea-backend/ -lpangaea-backend

    PRE_TARGETDEPS += $${OUT_PWD}/../pangaea-backend/libpangaea-backend.a
    PRE_TARGETDEPS += $${soxLibsPath}/libsox.so.3
}

DEFINES += VERSION_STRING=\\\"$${VERSION}\\\"

CONFIG += c++11

SOURCES += main.cpp \
    threadcontroller.cpp \
    ui/uidesktopcore.cpp \
    ui/uiinterfacemanager.cpp \
    ui/uisettings.cpp \
    utils/logger.cpp \

HEADERS += \
    threadcontroller.h \
    ui/uidesktopcore.h \
    ui/uiinterfacemanager.h \
    ui/uisettings.h \
    utils/logger.h \

INCLUDEPATH += ui
INCLUDEPATH += utils
INCLUDEPATH += models

INCLUDEPATH += $${PWD}/../pangaea-backend/cores
INCLUDEPATH += $${PWD}/../pangaea-backend/device
INCLUDEPATH += $${PWD}/../pangaea-backend/interfaces
INCLUDEPATH += $${PWD}/../pangaea-backend/cores
INCLUDEPATH += $${PWD}/../pangaea-backend/models
INCLUDEPATH += $${PWD}/../pangaea-backend/utils
INCLUDEPATH += $${PWD}/../pangaea-backend/qmlEnums/

TRANSLATIONS += translations/PangaeaCPPA_en.ts
TRANSLATIONS += translations/PangaeaCPPA_ru.ts
TRANSLATIONS += translations/PangaeaCPPA_de.ts
TRANSLATIONS += translations/PangaeaCPPA_it.ts


QML_IMPORT_PATH += $$PWD/qml/
QML2_IMPORT_PATH += $$PWD/qml/

RESOURCES += qml.qrc \
    translations.qrc

DISTFILES += \
    MacOS/Info.plist \
    icons/pangaea_amt.ico \

###### Post-build ###############

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
        wavConverterLibPath ~= s,/,\\,g
        dirDocs ~= s,/,\\,g
        dirDeploy ~= s,/,\\,g
        dirDeployRelease ~= s,/,\\,g

        QMAKE_POST_LINK += $$[QT_INSTALL_BINS]/windeployqt.exe --force release/$${TARGET}.exe --qmldir=$${PWD}/qml/ --dir $$shell_quote($$dirDeployRelease) $$escape_expand(\\n\\t)# $$RETURN
        QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($${appBinaryFile}) $$shell_quote($${dirDeployRelease}) $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($${converterBinaryFile}) $$shell_quote($${dirDeployRelease}) $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($${wavConverterLibPath}\\WavConverterLib.dll) $$shell_quote($${dirDeployRelease}) $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($${dirDocs}) $$shell_quote($${dirDeployRelease}\\docs\\) $$escape_expand(\\n\\t)

        #place version in template file and create Inno setup installer
        QMAKE_POST_LINK += powershell.exe $$shell_quote(((Get-Content $${dirDeploy}/template_script.iss) -replace  \'MyAppVersionREPLACE\',  \'MyAppVersion \"$${VERSION}\"\' | Out-file $${dirDeploy}/installer_script.iss -encoding ascii)) $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += ISCC.exe /O$${dirDeploy}/bin $${dirDeploy}/installer_script.iss $$escape_expand(\\n\\t)
    }

    macx {
        converterBinary = $${PWD}/../WavConverterShell/output_bin/IrConverter.app/Contents/MacOS/IrConverter

        QMAKE_POST_LINK += cp -r $${converterBinary} PangaeaCPPA.app/Contents/MacOS/ $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += cp -r $${dirDocs} PangaeaCPPA.app/Contents/MacOS/docs $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += macdeployqt $${DESTDIR}$${TARGET}.app -qmldir=$${PWD}/qml/ -libpath=$${soxLibsPath} -dmg $$escape_expand(\\n\\t)
    }

    linux{
        appBinaryFile = $${TARGET}
        converterBinaryFile = $${PWD}/../WavConverterShell/output_bin/IrConverter
        dirDeploy = $${PWD}/../deploy_linux
        dirApp = $${dirDeploy}/app

        QMAKE_POST_LINK += mkdir -p $${dirApp}/bin/ $$escape_expand(\n\t)
        QMAKE_POST_LINK += cp -r $${converterBinaryFile} $${dirApp}/bin/ $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += cp -r $${dirDocs} $${dirApp}/docs $$escape_expand(\\n\\t)

        # using cqtdeployer, installed from snap
        QMAKE_POST_LINK += cqtdeployer -bin PangaeaCPPA -targetDir $${dirApp} -libDir $${soxLibsPath} -qmlDir $${PWD}/qml/ -qmake ~/Qt/$${QT_VERSION}/gcc_64/bin/qmake $$escape_expand(\\n\\t)

        QMAKE_POST_LINK += cp -r $${OUT_PWD}/../WavConverterLib/libWavConverterLib.* $${dirApp}/lib/ $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += cp -r $${soxLibsPath}/libsox.* $${dirApp}/lib/ $$escape_expand(\\n\\t)

        # make tarball archive
        QMAKE_POST_LINK += tar -czvf $${dirDeploy}/PangaeaCPPA.tar.gz $${dirApp}/ $$escape_expand(\\n\\t)

        # make .deb package
        QMAKE_POST_LINK += mkdir -p $${dirDeploy}/debian_deploy/PangaeaCPPA/usr/local/PangaeaCPPA/ $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += cp -r $${dirApp}/. $${dirDeploy}/debian_deploy/PangaeaCPPA/usr/local/PangaeaCPPA/ $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += cp -r $${dirDeploy}/DEBIAN/ $${dirDeploy}/debian_deploy/PangaeaCPPA/ $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += sed -i $$shell_quote(s/APPVERSION/$${VERSION}/) $${dirDeploy}/debian_deploy/PangaeaCPPA/DEBIAN/control $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += cp -r $${dirDeploy}/usr/ $${dirDeploy}/debian_deploy/PangaeaCPPA/ $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += dpkg-deb --build $${dirDeploy}/debian_deploy/PangaeaCPPA/ $$escape_expand(\\n\\t)
    }
}
