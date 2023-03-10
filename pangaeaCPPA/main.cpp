#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QDebug>

#include <signal.h>

#include "core.h"
#include "uidesktopcore.h"
#include "interfacemanager.h"
#include "uiinterfacemanager.h"
#include "logger.h"

#include "devicedescription.h"

#include "threadcontroller.h"

void manageSegFailure(int signalCode);
Logger* Logger::currentHandler = nullptr;
Logger* appLogger_ptr;

int main(int argc, char *argv[])
{
    signal(SIGSEGV, manageSegFailure);
    QGuiApplication app(argc, argv);
    app.setOrganizationName("AMT");
    app.setApplicationName("Pangaea");

    Logger log;
    log.setAsMessageHandlerForApp();
    appLogger_ptr = &log;

    Core* core = new Core();
    InterfaceManager* interfaceManager = new InterfaceManager();

    ThreadController threadController(QThread::currentThread());
    core->moveToThread(threadController.backendThread());
#ifndef Q_OS_MACOS
    interfaceManager->moveToThread(threadController.backendThread()); // On MAC BLE can work only on the main thread
#endif
    QObject::connect(threadController.backendThread(), &QThread::finished, core, &QObject::deleteLater);
    QObject::connect(threadController.backendThread(), &QThread::finished, interfaceManager, &QObject::deleteLater);

    //-----------------------------------------------------------------
    // UI creation
    //----------------------------------------------------------------   
    UiInterfaceManager uiInterfaceManager;
    UiDesktopCore uiCore;

    QQmlApplicationEngine engine;

    qmlRegisterSingletonInstance("CppObjects", 1, 0, "UiCore", &uiCore);
    qmlRegisterSingletonInstance("CppObjects", 1, 0, "InterfaceManager", &uiInterfaceManager);

    qRegisterMetaType<DeviceDescription>();
    qmlRegisterUncreatableType<DeviceDescription>("CppObjects", 1, 0, "DeviceDescription", "");

    //-------------------------------------------------------------------------------
    // connections
    //-------------------------------------------------------------------------------
    QObject::connect(&uiCore, &UiDesktopCore::sgTranslatorChanged, &engine, &QQmlApplicationEngine::retranslate);
    QObject::connect(&uiCore, &UiDesktopCore::sgReadAllParameters, core, &Core::readAllParameters);
    QObject::connect(&uiCore, &UiDesktopCore::sgSetParameter, core, &Core::setParameter);
    QObject::connect(&uiCore, &UiDesktopCore::sgRestoreValue, core, &Core::restoreValue);
    QObject::connect(&uiCore, &UiDesktopCore::sgSetImpuls, core, &Core::setImpulse);
    QObject::connect(&uiCore, &UiDesktopCore::sgSetFirmware, core, &Core::setFirmware, Qt::QueuedConnection);
    QObject::connect(&uiCore, &UiDesktopCore::sgEscImpuls, core, &Core::escImpulse);
    QObject::connect(&uiCore, &UiDesktopCore::sgTranslatorChanged, core, &Core::pushReadPresetCommands);
    QObject::connect(&uiCore, &UiDesktopCore::sgExportPreset, core, &Core::exportPreset);
    QObject::connect(&uiCore, &UiDesktopCore::sgImportPreset, core, &Core::importPreset);
    QObject::connect(&uiCore, &UiDesktopCore::sgSw4Enable, core, &Core::sw4Enable);

    QObject::connect(core, &Core::sgSetUIParameter, &uiCore, &UiDesktopCore::sgSetUIParameter);
    QObject::connect(core, &Core::sgSetUIText, &uiCore, &UiDesktopCore::sgSetUIText);
    QObject::connect(core, &Core::sgPresetChangeStage, &uiCore, &UiDesktopCore::sgPresetChangeStage);
    QObject::connect(core, &Core::sgSetProgress, &uiCore, &UiDesktopCore::sgSetProgress);
    QObject::connect(core, &Core::sgFirmwareVersionInsufficient, &uiCore, &UiDesktopCore::slProposeOfflineFirmwareUpdate, Qt::QueuedConnection);

//    QObject::connect(&uiCore, &UICore::sgDoOnlineFirmwareUpdate, &netCore, &NetCore::requestFirmwareFile);
//    QObject::connect(&core, &Core::sgRequestNewestFirmware, &netCore, &NetCore::requestNewestFirmware);
//    QObject::connect(&netCore, &NetCore::sgFirmwareDownloaded, &core, &Core::uploadFirmware);
//    QObject::connect(&netCore, &NetCore::sgNewFirmwareAvaliable, &uiCore, &UICore::slProposeNetFirmwareUpdate, Qt::QueuedConnection);
//    QObject::connect(&netCore, &NetCore::sgDownloadProgress, &uiCore, &UICore::sgDownloadProgress, Qt::QueuedConnection);

    UiInterfaceManager::connect(&uiInterfaceManager, &UiInterfaceManager::sgStartScanning, interfaceManager, &InterfaceManager::startScanning, Qt::QueuedConnection);
    UiInterfaceManager::connect(&uiInterfaceManager, &UiInterfaceManager::sgConnectToDevice, interfaceManager, &InterfaceManager::connectToDevice, Qt::QueuedConnection);
    UiInterfaceManager::connect(&uiInterfaceManager, &UiInterfaceManager::sgDisconnectFromDevice, interfaceManager, &InterfaceManager::disconnectFromDevice, Qt::QueuedConnection);

    InterfaceManager::connect(interfaceManager, &InterfaceManager::sgDeviceListUpdated, &uiInterfaceManager, &UiInterfaceManager::updateDevicesList, Qt::QueuedConnection);
    InterfaceManager::connect(interfaceManager, &InterfaceManager::sgConnectionStarted, &uiInterfaceManager, &UiInterfaceManager::sgConnectionStarted, Qt::QueuedConnection);
    InterfaceManager::connect(interfaceManager, &InterfaceManager::sgInterfaceConnected, &uiInterfaceManager, &UiInterfaceManager::sgInterfaceConnected, Qt::QueuedConnection);
    InterfaceManager::connect(interfaceManager, &InterfaceManager::sgInterfaceError, &uiInterfaceManager, &UiInterfaceManager::sgInterfaceError, Qt::QueuedConnection);
    InterfaceManager::connect(interfaceManager, &InterfaceManager::sgInterfaceUnavaliable, &uiInterfaceManager, &UiInterfaceManager::slInterfaceUnavaliable, Qt::QueuedConnection);
    InterfaceManager::connect(interfaceManager, &InterfaceManager::sgDeviceUnavaliable, &uiInterfaceManager, &UiInterfaceManager::sgDeviceUnavaliable, Qt::QueuedConnection);
    InterfaceManager::connect(interfaceManager, &InterfaceManager::sgInterfaceDisconnected, &uiInterfaceManager, &UiInterfaceManager::sgInterfaceDisconnected, Qt::QueuedConnection);

    Core::connect(interfaceManager, &InterfaceManager::sgNewData, core, &Core::parseInputData);
    Core::connect(interfaceManager, &InterfaceManager::sgInterfaceConnected, core, &Core::readAllParameters);
    Core::connect(core, &Core::sgWriteToInterface, interfaceManager, &InterfaceManager::writeToDevice);
    Core::connect(core, &Core::sgExchangeError, interfaceManager, &InterfaceManager::disconnectFromDevice);
    Core::connect(core, &Core::sgExchangeError, &uiInterfaceManager, &UiInterfaceManager::sgExchangeError);
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
