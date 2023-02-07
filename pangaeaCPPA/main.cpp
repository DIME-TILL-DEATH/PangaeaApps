#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QDebug>

#include <signal.h>

#include "core.h"
#include "uidesktopcore.h"
#include "logger.h"

#include "threadcontroller.h"

void manageSegFailure(int signalCode);
Logger* Logger::currentHandler = nullptr;
Logger* appLogger_ptr;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    signal(SIGSEGV, manageSegFailure);

    app.setOrganizationName("AMT");
    app.setApplicationName("Pangaea");

    Logger log;
    log.setAsMessageHandlerForApp();
    appLogger_ptr = &log;

    QQmlApplicationEngine engine;
    Core core;
    UiDesktopCore uiCore;

    qmlRegisterSingletonInstance("CppObjects", 1, 0, "UiCore", &uiCore);
    core.registerQmlObjects();

    ThreadController threadController(QThread::currentThread());
    core.moveToThread(threadController.backendThread());

    //-------------------------------------------------------------------------------
    // connections
    //-------------------------------------------------------------------------------
    QObject::connect(threadController.backendThread(), &QThread::finished, &core, &Core::stopCore);

    QObject::connect(&uiCore, &UiDesktopCore::sgTranslatorChanged, &engine, &QQmlApplicationEngine::retranslate);

    QObject::connect(&uiCore, &UiDesktopCore::sgReadAll, &core, &Core::readAllParameters);
    QObject::connect(&uiCore, &UiDesktopCore::sgSetParameter, &core, &Core::setParameter);
    QObject::connect(&uiCore, &UiDesktopCore::sgRestoreValue, &core, &Core::restoreValue);
    QObject::connect(&uiCore, &UiDesktopCore::sgSetImpuls, &core, &Core::setImpulse);
    QObject::connect(&uiCore, &UiDesktopCore::sgSetFirmware, &core, &Core::setFirmware, Qt::QueuedConnection);
    QObject::connect(&uiCore, &UiDesktopCore::sgEscImpuls, &core, &Core::escImpulse);
    QObject::connect(&uiCore, &UiDesktopCore::sgTranslatorChanged, &core, &Core::pushReadPresetCommands);
    QObject::connect(&uiCore, &UiDesktopCore::sgExportPreset, &core, &Core::exportPreset);
    QObject::connect(&uiCore, &UiDesktopCore::sgImportPreset, &core, &Core::importPreset);
    QObject::connect(&uiCore, &UiDesktopCore::sgSw4Enable, &core, &Core::sw4Enable);
    QObject::connect(&uiCore, &UiDesktopCore::sgUiClosing, &core, &Core::stopCore);

//    QObject::connect(&uiCore, &UICore::sgModuleNameChanged, &core, &Core::setModuleName);
//    QObject::connect(&uiCore, &UICore::sgModuleNameChanged, &bleConnection, &BluetoothleUART::setModuleName);

//    QObject::connect(&uiCore, &UICore::sgDoOnlineFirmwareUpdate, &netCore, &NetCore::requestFirmwareFile);

    QObject::connect(&core, &Core::sgSetUIParameter, &uiCore, &UiDesktopCore::sgSetUIParameter);
    QObject::connect(&core, &Core::sgSetUIText, &uiCore, &UiDesktopCore::sgSetUIText);
//    QObject::connect(&core, &Core::sgUpdateBLEDevicesList, &uiCore, &UiDesktopCore::sgUpdateBLEDevicesList);
//    QObject::connect(&core, &Core::sgConnectReady, &uiCore, &UiDesktopCore::sgConnectReady);
    QObject::connect(&core, &Core::sgPresetChangeStage, &uiCore, &UiDesktopCore::sgPresetChangeStage);
    QObject::connect(&core, &Core::sgSetProgress, &uiCore, &UiDesktopCore::sgSetProgress);
    QObject::connect(&core, &Core::sgModuleNameUpdated, &uiCore, &UiDesktopCore::setModuleName);
    QObject::connect(&core, &Core::sgFirmwareVersionInsufficient, &uiCore, &UiDesktopCore::slProposeOfflineFirmwareUpdate, Qt::QueuedConnection);

//    QObject::connect(&core, &Core::sgRequestNewestFirmware, &netCore, &NetCore::requestNewestFirmware);

//    QObject::connect(&netCore, &NetCore::sgFirmwareDownloaded, &core, &Core::uploadFirmware);
//    QObject::connect(&netCore, &NetCore::sgNewFirmwareAvaliable, &uiCore, &UICore::slProposeNetFirmwareUpdate, Qt::QueuedConnection);
//    QObject::connect(&netCore, &NetCore::sgDownloadProgress, &uiCore, &UICore::sgDownloadProgress, Qt::QueuedConnection);
    //----------------------------------------------------------------


    engine.addImportPath(":/qml");
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    engine.load(url);
    return app.exec();
}

void manageSegFailure(int signalCode)
{
    qWarning() << "Segmentation fault! Code:" << signalCode;
    appLogger_ptr->~Logger();

    signal(signalCode, SIG_DFL);
    QGuiApplication::exit(3);
}
