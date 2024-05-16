#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include <signal.h>

#include <QDir>

#include <QDebug>

#include "core.h"
#include "deviceproperties.h"
#include "uicore.h"
#include "uiinterfacemanager.h"
#include "netcore.h"

#include "threadcontroller.h"
#include "logger.h"

#include "interfacecore.h"
#include "presetlistmodel.h"

#include "eqband.h"
#include "eqresponse.h"

#ifdef Q_OS_ANDROID
#include <QtCore/private/qandroidextras_p.h>

using QtJniObject = QJniObject;

inline QtJniObject qtAndroidContext()
{
    return QJniObject(QCoreApplication::instance()
                          ->nativeInterface<QNativeInterface::QAndroidApplication>()
                          ->context());
}

inline int qtAndroidSdkVersion()
{
    return QCoreApplication::instance()
        ->nativeInterface<QNativeInterface::QAndroidApplication>()
        ->sdkVersion();
}

inline void qt5RunOnAndroidMainThread(const std::function<void()> &runnable)
{
    QCoreApplication::instance()
        ->nativeInterface<QNativeInterface::QAndroidApplication>()
        ->runOnAndroidMainThread([runnable]() {runnable(); return
QVariant();});
}
#endif

void manageSegFailure(int signalCode);
Logger* Logger::currentHandler = nullptr;
Logger* appLogger_ptr;

int main(int argc, char *argv[])
{
    signal(SIGSEGV, manageSegFailure);
    signal(SIGFPE, manageSegFailure);
    signal(SIGILL, manageSegFailure);

    QGuiApplication app(argc, argv);
//    QApplication app(argc, argv);

    app.setOrganizationName("AMT");
    app.setOrganizationDomain("amtelectronics.com");
    app.setApplicationName("pangaea mobile");

    Logger log;
    log.setAsMessageHandlerForApp();
    appLogger_ptr = &log;

    Core* core = new Core;
    NetCore* netCore = new NetCore;
    InterfaceCore* interfaceManager = new InterfaceCore;

    EqResponse eqResponse;

    PresetListModel presetListModel;

    ThreadController threadController(QThread::currentThread());
    core->moveToThread(threadController.backendThread());
    netCore->moveToThread(threadController.backendThread());
#if !defined(Q_OS_MACOS)
    interfaceManager->moveToThread(threadController.connectionsThread()); // On MAC BLE can work only on the main thread
#endif

    QObject::connect(threadController.backendThread(), &QThread::finished, core, &QObject::deleteLater);
    QObject::connect(threadController.backendThread(), &QThread::finished, netCore, &QObject::deleteLater);
    QObject::connect(threadController.backendThread(), &QThread::finished, interfaceManager, &QObject::deleteLater);

    //-----------------------------------------------------------------
    // UI creation
    //----------------------------------------------------------------
    UiCore uiCore;
    UiInterfaceManager uiInterfaceManager;
    DeviceProperties deviceProperties;

    QQmlApplicationEngine engine;
    engine.addImportPath(":/qml");
    engine.addImportPath(":/firmwares");
    engine.addImportPath(":/translations");
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));


    qmlRegisterSingletonInstance("CppObjects", 1, 0, "UiCore", &uiCore);
    qmlRegisterSingletonInstance("CppObjects", 1, 0, "DeviceProperties", &deviceProperties);
    qmlRegisterSingletonInstance("CppObjects", 1, 0, "InterfaceManager", &uiInterfaceManager);
    qmlRegisterSingletonInstance("CppObjects", 1, 0, "PresetListModel", &presetListModel);

    qmlRegisterSingletonInstance("CppObjects", 1, 0, "EqResponse", &eqResponse);

    qmlRegisterUncreatableType<Core>("CppObjects", 1, 0, "AppParameter", "Cannot create AppParameter in QML");
    qmlRegisterUncreatableType<DeviceParameter>("CppObjects", 1, 0, "DeviceParameter", "Cannot create DeviceParameter in QML");
    qmlRegisterUncreatableType<DeviceDescription>("CppObjects", 1, 0, "DeviceDescription", "");
    qmlRegisterUncreatableType<DeviceTypeEnum>("CppEnums", 1, 0, "DeviceType", "Not creatable as it is an enum type");

    qmlRegisterUncreatableType<AbstractModule>("CppObjects", 1, 0, "Module", "Cannot create Module in QML");
    qmlRegisterUncreatableType<EqBand>("CppObjects", 1, 0, "EqBand", "Cannot create EqBand in QML");
    qmlRegisterUncreatableType<ControlValue>("CppObjects", 1, 0, "ControlValue", "Cannot create ControlValue in QML");
    //-------------------------------------------------------------------------------
    // connections
    //-------------------------------------------------------------------------------
    UiCore::connect(&uiCore, &UiCore::sgTranslatorChanged, &engine, &QQmlApplicationEngine::retranslate);

    QObject::connect(&uiCore, &UiCore::sgReadAllParameters, core, &Core::readAllParameters);
    QObject::connect(&uiCore, &UiCore::sgSetParameter, core, &Core::setParameter);
    QObject::connect(&uiCore, &UiCore::sgSetDeviceParameter, core, &Core::slSetDeviceParameter);

    QObject::connect(&uiCore, &UiCore::sgRestoreValue, core, &Core::restoreValue);
    QObject::connect(&uiCore, &UiCore::sgSetImpuls, core, &Core::setImpulse);
    QObject::connect(&uiCore, &UiCore::sgSetFirmware, core, &Core::setFirmware, Qt::QueuedConnection);
    QObject::connect(&uiCore, &UiCore::sgEscImpuls, core, &Core::escImpulse);
    QObject::connect(&uiCore, &UiCore::sgExportPreset, core, &Core::exportPreset);
    QObject::connect(&uiCore, &UiCore::sgImportPreset, core, &Core::importPreset);
    QObject::connect(&uiCore, &UiCore::sgSw4Enable, core, &Core::sw4Enable);

    QObject::connect(&eqResponse, &EqResponse::sgSetDeviceParameter, core, &::Core::slSetDeviceParameter);

    // QObject::connect(core, &Core::sgSetAppParameter, &uiCore, &UiCore::slSetAppParameter);
    QObject::connect(core, &Core::sgSetUIParameter, &uiCore, &UiCore::sgSetUIParameter);
    QObject::connect(core, &Core::sgSetAppParameter, &eqResponse, &EqResponse::sgSetAppParameter);
    QObject::connect(core, &Core::sgRecieveDeviceParameter, &uiCore, &UiCore::sgSetUiDeviceParameter);
    QObject::connect(core, &Core::sgSetAppParameter, &deviceProperties, &DeviceProperties::slSetAppParameter);
    QObject::connect(core, &Core::sgRecieveDeviceParameter, &deviceProperties, &DeviceProperties::slSetUiDeviceParameter);
    QObject::connect(core, &Core::sgRecieveDeviceParameter, &eqResponse, &EqResponse::slSetUiDeviceParameter);
    QObject::connect(core, &Core::sgSetUIText, &uiCore, &UiCore::sgSetUIText);
    QObject::connect(core, &Core::sgSetProgress, &uiCore, &UiCore::sgSetProgress);
    QObject::connect(core, &Core::sgFirmwareVersionInsufficient, &uiCore, &UiCore::slProposeOfflineFirmwareUpdate, Qt::QueuedConnection);

    QObject::connect(&deviceProperties, &DeviceProperties::sendAppAction, core, &Core::slRecieveAppAction);

    QObject::connect(core, &Core::sgRefreshPresetList, &presetListModel, &PresetListModel::refreshModel, Qt::QueuedConnection);
    QObject::connect(core, &Core::sgUpdatePreset, &presetListModel, &PresetListModel::updatePreset, Qt::QueuedConnection);

    QObject::connect(core, &Core::sgRequestNewestFirmware, netCore, &NetCore::requestNewestFirmware);

    QObject::connect(netCore, &NetCore::sgNewFirmwareAvaliable, &uiCore, &UiCore::slProposeNetFirmwareUpdate, Qt::QueuedConnection);
//    NetCore::connect(netCore, &NetCore::sgNewAppVersionAvaliable, &uiCore, &UiDesktopCore::slNewAppVersionAvaliable);
    QObject::connect(&uiCore, &UiCore::sgDoOnlineFirmwareUpdate, netCore, &NetCore::requestFirmwareFile);
    QObject::connect(netCore, &NetCore::sgFirmwareDownloaded, core, &Core::uploadFirmware);
    QObject::connect(netCore, &NetCore::sgDownloadProgress, &uiCore, &UiCore::sgDownloadProgress, Qt::QueuedConnection);

    Core::connect(interfaceManager, &InterfaceCore::sgNewData, core, &Core::parseInputData);
    Core::connect(interfaceManager, &InterfaceCore::sgInterfaceConnected, core, &Core::readAllParameters);
    Core::connect(core, &Core::sgWriteToInterface, interfaceManager, &InterfaceCore::writeToDevice);
    Core::connect(core, &Core::sgSilentWriteToInterface, interfaceManager, &InterfaceCore::silentWriteToDevice);
    Core::connect(core, &Core::sgExchangeError, interfaceManager, &InterfaceCore::disconnectFromDevice);
    Core::connect(core, &Core::sgReadyTodisconnect, interfaceManager, &InterfaceCore::disconnectFromDevice);
    Core::connect(core, &Core::sgExchangeError, &uiInterfaceManager, &UiInterfaceManager::sgExchangeError);

    UiInterfaceManager::connect(&uiInterfaceManager, &UiInterfaceManager::sgStartScanning, interfaceManager, &InterfaceCore::startScanning, Qt::QueuedConnection);
    UiInterfaceManager::connect(&uiInterfaceManager, &UiInterfaceManager::sgConnectToDevice, interfaceManager, &InterfaceCore::connectToDevice, Qt::QueuedConnection);
    UiInterfaceManager::connect(&uiInterfaceManager, &UiInterfaceManager::sgDisconnectFromDevice, interfaceManager, &InterfaceCore::disconnectFromDevice, Qt::QueuedConnection);
//    Core::connect(core, &Core::sgImmediatelyDisconnect, interfaceManager, &InterfaceCore::disconnectFromDevice, Qt::QueuedConnection);

    QObject::connect(&uiCore, &UiCore::sgModuleNameChanged, interfaceManager, &InterfaceCore::setModuleName);
    QObject::connect(interfaceManager, &InterfaceCore::sgModuleNameUpdated, &uiCore, &UiCore::setModuleName);

    QObject::connect(interfaceManager, &InterfaceCore::sgDeviceListUpdated, &uiInterfaceManager, &UiInterfaceManager::updateDevicesList);
    QObject::connect(interfaceManager, &InterfaceCore::sgConnectionStarted, &uiInterfaceManager, &UiInterfaceManager::sgConnectionStarted);
    QObject::connect(interfaceManager, &InterfaceCore::sgInterfaceConnected, &uiInterfaceManager, &UiInterfaceManager::sgInterfaceConnected);
    QObject::connect(interfaceManager, &InterfaceCore::sgInterfaceError, &uiInterfaceManager, &UiInterfaceManager::sgInterfaceError);
    QObject::connect(interfaceManager, &InterfaceCore::sgInterfaceUnavaliable, &uiInterfaceManager, &UiInterfaceManager::slInterfaceUnavaliable);
    QObject::connect(interfaceManager, &InterfaceCore::sgDeviceUnavaliable, &uiInterfaceManager, &UiInterfaceManager::sgDeviceUnavaliable);
    QObject::connect(interfaceManager, &InterfaceCore::sgInterfaceDisconnected, &uiInterfaceManager, &UiInterfaceManager::sgInterfaceDisconnected);
    //----------------------------------------------------------------

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

    //-----------------------------------------------------------------------
    // keep screen always on
    // also WAKE_LOCK permision in manifest
#ifdef Q_OS_ANDROID
        qt5RunOnAndroidMainThread([]
        {
            QJniObject activity = QNativeInterface::QAndroidApplication::context();
            if (activity.isValid())
            {
                QJniObject window = activity.callObjectMethod("getWindow", "()Landroid/view/Window;");

                if (window.isValid())
                {
                    const int FLAG_KEEP_SCREEN_ON = 128;
                    window.callMethod<void>("addFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
                }

                QJniEnvironment env;
                if (env->ExceptionCheck())
                {
                    env->ExceptionClear();
                }
            }
        });
#endif
    //----------------------------------------------------------------------
    return app.exec();
}

void manageSegFailure(int signalCode)
{
    qWarning() << "Critical fault! Code:" << signalCode;
    appLogger_ptr->~Logger();

    signal(signalCode, SIG_DFL);
    QGuiApplication::exit(3);
}
