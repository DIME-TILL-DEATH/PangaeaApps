#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>

#include <QIcon>

#include <QDebug>
#include <QLoggingCategory>

#include <signal.h>

#include "core.h"
#include "netcore.h"
#include "presetlistmodel.h"

#include "uidesktopcore.h"
#include "uisettings.h"
#include "interfacecore.h"
#include "uiinterfacemanager.h"
#include "logger.h"

#include "devicedescription.h"

#include "threadcontroller.h"

#include "devicetypeenum.h"

void manageSegFailure(int signalCode);
Logger* Logger::currentHandler = nullptr;
Logger* appLogger_ptr;

int main(int argc, char *argv[])
{
    signal(SIGSEGV, manageSegFailure);
//    QGuiApplication app(argc, argv);
    QApplication app(argc, argv);
    app.setOrganizationName("AMT");
    app.setApplicationName("Pangaea");

#ifdef Q_OS_LINUX
    QApplication::setWindowIcon(QIcon(":/qml/Images/AMT.svg"));
    app.setApplicationVersion(VERSION_STRING);
#endif

    Logger log;
    log.setAsMessageHandlerForApp();
    appLogger_ptr = &log;

    Core* core = new Core();
    NetCore* netCore = new NetCore();
    InterfaceCore* interfaceManager = new InterfaceCore();

    PresetListModel presetListModel;

    ThreadController threadController(QThread::currentThread());
    core->moveToThread(threadController.backendThread());
    netCore->moveToThread(threadController.backendThread());

#if !defined(Q_OS_MACOS) && !defined(Q_OS_LINUX)
    interfaceManager->moveToThread(threadController.backendThread()); // On MAC BLE can work only on the main thread
                                                                        // In Linux BLE needs to work in separate thread from core
#endif
    QObject::connect(threadController.backendThread(), &QThread::finished, core, &QObject::deleteLater);
    QObject::connect(threadController.backendThread(), &QThread::finished, netCore, &QObject::deleteLater);
    QObject::connect(threadController.backendThread(), &QThread::finished, interfaceManager, &QObject::deleteLater);

    //-----------------------------------------------------------------
    // UI creation
    //----------------------------------------------------------------   
    UiInterfaceManager uiInterfaceManager;
    UiDesktopCore uiCore;
    UiSettings uiSettings;

    QQmlApplicationEngine engine;
    engine.addImportPath(":/qml");
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));

    qmlRegisterSingletonInstance("CppObjects", 1, 0, "UiCore", &uiCore);
    qmlRegisterSingletonInstance("CppObjects", 1, 0, "UiSettings", &uiSettings);
    qmlRegisterSingletonInstance("CppObjects", 1, 0, "InterfaceManager", &uiInterfaceManager);
    qmlRegisterSingletonInstance("CppObjects", 1, 0, "PresetListModel", &presetListModel);

    qRegisterMetaType<DeviceDescription>();
    qmlRegisterUncreatableType<DeviceDescription>("CppObjects", 1, 0, "DeviceDescription", "");
    qmlRegisterUncreatableType<DeviceTypeEnum>("CppEnums", 1, 0, "DeviceType", "Not creatable as it is an enum type");

    //-------------------------------------------------------------------------------
    // connections
    //-------------------------------------------------------------------------------   
    QObject::connect(&uiSettings, &UiSettings::sgTranslatorChanged, &engine, &QQmlApplicationEngine::retranslate);
    QObject::connect(&uiSettings, &UiSettings::sgApplicationStarted, netCore, &NetCore::requestAppUpdates);

    QObject::connect(&uiCore, &UiDesktopCore::sgReadAllParameters, core, &Core::readAllParameters);
    QObject::connect(&uiCore, &UiDesktopCore::sgSetParameter, core, &Core::setParameter);
    QObject::connect(&uiCore, &UiDesktopCore::sgRestoreValue, core, &Core::restoreValue);
    QObject::connect(&uiCore, &UiDesktopCore::sgSetImpuls, core, &Core::setImpulse);
    QObject::connect(&uiCore, &UiDesktopCore::sgSetFirmware, core, &Core::setFirmware, Qt::QueuedConnection);
    QObject::connect(&uiCore, &UiDesktopCore::sgEscImpuls, core, &Core::escImpulse);
    QObject::connect(&uiCore, &UiDesktopCore::sgExportPreset, core, &Core::exportPreset);
    QObject::connect(&uiCore, &UiDesktopCore::sgImportPreset, core, &Core::importPreset);
    QObject::connect(&uiCore, &UiDesktopCore::sgSw4Enable, core, &Core::sw4Enable);

    QObject::connect(core, &Core::sgSetUIParameter, &uiCore, &UiDesktopCore::sgSetUIParameter);
    QObject::connect(core, &Core::sgSetUIText, &uiCore, &UiDesktopCore::sgSetUIText);
    QObject::connect(core, &Core::sgPresetChangeStage, &uiCore, &UiDesktopCore::sgPresetChangeStage);
    QObject::connect(core, &Core::sgSetProgress, &uiCore, &UiDesktopCore::sgSetProgress);
    QObject::connect(core, &Core::sgFirmwareVersionInsufficient, &uiCore, &UiDesktopCore::slProposeOfflineFirmwareUpdate, Qt::QueuedConnection);

    QObject::connect(core, &Core::sgRefreshPresetList, &presetListModel, &PresetListModel::refreshModel, Qt::QueuedConnection);
    QObject::connect(core, &Core::sgUpdatePreset, &presetListModel, &PresetListModel::updatePreset, Qt::QueuedConnection);

    NetCore::connect(core, &Core::sgRequestNewestFirmware, netCore, &NetCore::requestNewestFirmware);

    NetCore::connect(netCore, &NetCore::sgNewFirmwareAvaliable, &uiCore, &UiDesktopCore::slProposeNetFirmwareUpdate, Qt::QueuedConnection);
    NetCore::connect(netCore, &NetCore::sgNewAppVersionAvaliable, &uiCore, &UiDesktopCore::slNewAppVersionAvaliable);
//    QObject::connect(&uiCore, &UICore::sgDoOnlineFirmwareUpdate, &netCore, &NetCore::requestFirmwareFile);
//    QObject::connect(&netCore, &NetCore::sgFirmwareDownloaded, &core, &Core::uploadFirmware);
//    QObject::connect(&netCore, &NetCore::sgDownloadProgress, &uiCore, &UICore::sgDownloadProgress, Qt::QueuedConnection);

    Core::connect(interfaceManager, &InterfaceCore::sgNewData, core, &Core::parseInputData);
    Core::connect(interfaceManager, &InterfaceCore::sgInterfaceConnected, core, &Core::readAllParameters);
    Core::connect(core, &Core::sgWriteToInterface, interfaceManager, &InterfaceCore::writeToDevice);
//    Core::connect(core, &Core::sgExchangeError, interfaceManager, &InterfaceCore::disconnectFromDevice);
    Core::connect(core, &Core::sgExchangeError, &uiInterfaceManager, &UiInterfaceManager::sgExchangeError);

    UiInterfaceManager::connect(&uiInterfaceManager, &UiInterfaceManager::sgStartScanning, interfaceManager, &InterfaceCore::startScanning, Qt::QueuedConnection);
    UiInterfaceManager::connect(&uiInterfaceManager, &UiInterfaceManager::sgConnectToDevice, interfaceManager, &InterfaceCore::connectToDevice, Qt::QueuedConnection);
    UiInterfaceManager::connect(&uiInterfaceManager, &UiInterfaceManager::sgDisconnectFromDevice, interfaceManager, &InterfaceCore::disconnectFromDevice, Qt::QueuedConnection);

    InterfaceCore::connect(interfaceManager, &InterfaceCore::sgDeviceListUpdated, &uiInterfaceManager, &UiInterfaceManager::updateDevicesList, Qt::QueuedConnection);
    InterfaceCore::connect(interfaceManager, &InterfaceCore::sgConnectionStarted, &uiInterfaceManager, &UiInterfaceManager::sgConnectionStarted, Qt::QueuedConnection);
    InterfaceCore::connect(interfaceManager, &InterfaceCore::sgInterfaceConnected, &uiInterfaceManager, &UiInterfaceManager::sgInterfaceConnected, Qt::QueuedConnection);
    InterfaceCore::connect(interfaceManager, &InterfaceCore::sgInterfaceError, &uiInterfaceManager, &UiInterfaceManager::sgInterfaceError, Qt::QueuedConnection);
    InterfaceCore::connect(interfaceManager, &InterfaceCore::sgInterfaceUnavaliable, &uiInterfaceManager, &UiInterfaceManager::slInterfaceUnavaliable, Qt::QueuedConnection);
    InterfaceCore::connect(interfaceManager, &InterfaceCore::sgDeviceUnavaliable, &uiInterfaceManager, &UiInterfaceManager::sgDeviceUnavaliable, Qt::QueuedConnection);
    InterfaceCore::connect(interfaceManager, &InterfaceCore::sgInterfaceDisconnected, &uiInterfaceManager, &UiInterfaceManager::sgInterfaceDisconnected, Qt::QueuedConnection);
    //----------------------------------------------------------------

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
