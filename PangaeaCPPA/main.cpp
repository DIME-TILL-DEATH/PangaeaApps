#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>

#include <QIcon>

#include <QDebug>
#include <QLoggingCategory>

#include <signal.h>

#include <QQuickStyle>

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

#include "eqband.h"
#include "eqresponse.h"

void manageSegFailure(int signalCode);
Logger* Logger::currentHandler = nullptr;
Logger* appLogger_ptr;

int main(int argc, char *argv[])
{
    signal(SIGSEGV, manageSegFailure);
//    signal(SIGFPE, manageSegFailure);
//    signal(SIGILL, manageSegFailure);

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

    EqResponse eqResponse;

    PresetListModel presetListModel;

    ThreadController threadController(QThread::currentThread());
    core->moveToThread(threadController.backendThread());
    netCore->moveToThread(threadController.backendThread());

    interfaceManager->moveToThread(threadController.connectionsThread());

    QObject::connect(threadController.backendThread(), &QThread::finished, core, &QObject::deleteLater);
    QObject::connect(threadController.backendThread(), &QThread::finished, netCore, &QObject::deleteLater);
    QObject::connect(threadController.backendThread(), &QThread::finished, interfaceManager, &QObject::deleteLater);

    //-----------------------------------------------------------------
    // UI creation
    //----------------------------------------------------------------
    UiInterfaceManager uiInterfaceManager; // TODO: move to backend, либо common(Общий для всех)
    UiDesktopCore uiCore;
    UiSettings uiSettings;

    QQmlApplicationEngine engine;
    engine.addImportPath(":/qml");
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
#ifdef Q_OS_MACOS
    QQuickStyle::setStyle("Basic");
#endif


    qmlRegisterSingletonInstance("CppObjects", 1, 0, "UiCore", &uiCore);
    qmlRegisterSingletonInstance("CppObjects", 1, 0, "UiSettings", &uiSettings);
    qmlRegisterSingletonInstance("CppObjects", 1, 0, "InterfaceManager", &uiInterfaceManager);
    qmlRegisterSingletonInstance("CppObjects", 1, 0, "PresetListModel", &presetListModel);

    qmlRegisterSingletonInstance("CppObjects", 1, 0, "EqResponse", &eqResponse);

    qmlRegisterUncreatableType<DeviceParameter>("CppObjects", 1, 0, "DeviceParameter", "Cannot create DeviceParameter in QML");
    qmlRegisterUncreatableType<DeviceDescription>("CppObjects", 1, 0, "DeviceDescription", "");
    qmlRegisterUncreatableType<DeviceTypeEnum>("CppEnums", 1, 0, "DeviceType", "Not creatable as it is an enum type");
    qmlRegisterUncreatableType<EqBand>("CppObjects", 1, 0, "EqBand", "Cannot create EqBand in QML");

    //-------------------------------------------------------------------------------
    // connections
    //-------------------------------------------------------------------------------   
    QObject::connect(&uiSettings, &UiSettings::sgTranslatorChanged, &engine, &QQmlApplicationEngine::retranslate);
    QObject::connect(&uiSettings, &UiSettings::sgApplicationStarted, netCore, &NetCore::requestAppUpdates);

    QObject::connect(&uiCore, &UiDesktopCore::sgReadAllParameters, core, &Core::readAllParameters);
    QObject::connect(&uiCore, &UiDesktopCore::sgSetParameter, core, &Core::setParameter);
    QObject::connect(&uiCore, &UiDesktopCore::sgSetDeviceParameter, core, &Core::setDeviceParameter);
    QObject::connect(&uiCore, &UiDesktopCore::sgSetDeviceParameter, &eqResponse, &EqResponse::slFilterParamChanged);
    QObject::connect(&uiCore, &UiDesktopCore::sgRestoreValue, core, &Core::restoreValue);
    QObject::connect(&uiCore, &UiDesktopCore::sgSetImpuls, core, &Core::setImpulse);
    QObject::connect(&uiCore, &UiDesktopCore::sgSetFirmware, core, &Core::setFirmware, Qt::QueuedConnection);
    QObject::connect(&uiCore, &UiDesktopCore::sgEscImpuls, core, &Core::escImpulse);
    QObject::connect(&uiCore, &UiDesktopCore::sgExportPreset, core, &Core::exportPreset);
    QObject::connect(&uiCore, &UiDesktopCore::sgImportPreset, core, &Core::importPreset);
    QObject::connect(&uiCore, &UiDesktopCore::sgSw4Enable, core, &Core::sw4Enable);

    QObject::connect(&eqResponse, &EqResponse::setDeviceParameter, core, &Core::setDeviceParameter);

    QObject::connect(core, &Core::sgSetUIParameter, &uiCore, &UiDesktopCore::sgSetUIParameter);
    QObject::connect(core, &Core::sgSetUiDeviceParameter, &uiCore, &UiDesktopCore::sgSetUiDeviceParameter);
    QObject::connect(core, &Core::sgSetUiDeviceParameter, &eqResponse, &EqResponse::slFilterParamChanged);
    QObject::connect(core, &Core::sgSetUIText, &uiCore, &UiDesktopCore::sgSetUIText);
    QObject::connect(core, &Core::sgPresetChangeStage, &uiCore, &UiDesktopCore::sgPresetChangeStage);
    QObject::connect(core, &Core::sgSetProgress, &uiCore, &UiDesktopCore::sgSetProgress);
    QObject::connect(core, &Core::sgFirmwareVersionInsufficient, &uiCore, &UiDesktopCore::slProposeOfflineFirmwareUpdate, Qt::QueuedConnection);  

    QObject::connect(core, &Core::sgRefreshPresetList, &presetListModel, &PresetListModel::refreshModel, Qt::QueuedConnection);
    QObject::connect(core, &Core::sgUpdatePreset, &presetListModel, &PresetListModel::updatePreset, Qt::QueuedConnection);

    NetCore::connect(core, &Core::sgRequestNewestFirmware, netCore, &NetCore::requestNewestFirmware);

    NetCore::connect(netCore, &NetCore::sgNewFirmwareAvaliable, &uiCore, &UiDesktopCore::slProposeNetFirmwareUpdate, Qt::QueuedConnection);
    NetCore::connect(netCore, &NetCore::sgNewAppVersionAvaliable, &uiCore, &UiDesktopCore::slNewAppVersionAvaliable);

    Core::connect(interfaceManager, &InterfaceCore::sgNewData, core, &Core::parseInputData, Qt::QueuedConnection);
    Core::connect(interfaceManager, &InterfaceCore::sgInterfaceConnected, core, &Core::readAllParameters, Qt::QueuedConnection);
//#if !defined(Q_OS_MACOS)
    Core::connect(core, &Core::sgWriteToInterface, interfaceManager, &InterfaceCore::writeToDevice, Qt::BlockingQueuedConnection);
    Core::connect(core, &Core::sgSilentWriteToInterface, interfaceManager, &InterfaceCore::silentWriteToDevice, Qt::BlockingQueuedConnection);
//#else
//    Core::connect(core, &Core::sgWriteToInterface, interfaceManager, &InterfaceCore::writeToDevice);
//#endif
    Core::connect(core, &Core::sgExchangeError, &uiInterfaceManager, &UiInterfaceManager::sgExchangeError, Qt::QueuedConnection);

    UiInterfaceManager::connect(&uiInterfaceManager, &UiInterfaceManager::sgStartScanning, interfaceManager, &InterfaceCore::startScanning, Qt::QueuedConnection);
    UiInterfaceManager::connect(&uiInterfaceManager, &UiInterfaceManager::sgConnectToDevice, interfaceManager, &InterfaceCore::connectToDevice, Qt::QueuedConnection);
    UiInterfaceManager::connect(&uiInterfaceManager, &UiInterfaceManager::sgDisconnectFromDevice, interfaceManager, &InterfaceCore::disconnectFromDevice, Qt::QueuedConnection);
    Core::connect(core, &Core::sgImmediatelyDisconnect, interfaceManager, &InterfaceCore::disconnectFromDevice, Qt::QueuedConnection);

    InterfaceCore::connect(interfaceManager, &InterfaceCore::sgDeviceListUpdated, &uiInterfaceManager, &UiInterfaceManager::updateDevicesList, Qt::BlockingQueuedConnection);
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
