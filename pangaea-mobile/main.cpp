#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include <signal.h>

#include <QDir>

#include <QDebug>

#include "core.h"
#include "uicore.h"
#include "uiinterfacemanager.h"
#include "netcore.h"

#include "threadcontroller.h"
#include "logger.h"

//#include "bluetoothleuart.h"
#include "bleinterface.h"
#include "presetlistmodel.h"

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

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    app.setOrganizationName("AMT");
    app.setOrganizationDomain("amtelectronics.com");
    app.setApplicationName("pangaea mobile");

    Logger log;
    log.setAsMessageHandlerForApp();
    appLogger_ptr = &log;

    Core* core = new Core;
    NetCore* netCore = new NetCore;
    BleInterface* bleInterface = new BleInterface;

    PresetListModel presetListModel;

    ThreadController threadController(QThread::currentThread());
    core->moveToThread(threadController.backendThread());
    netCore->moveToThread(threadController.backendThread());
//    bleInterface->moveToThread(threadController.backendThread());

    QObject::connect(threadController.backendThread(), &QThread::finished, core, &QObject::deleteLater);
    QObject::connect(threadController.backendThread(), &QThread::finished, netCore, &QObject::deleteLater);
    QObject::connect(threadController.backendThread(), &QThread::finished, bleInterface, &QObject::deleteLater);

    //-----------------------------------------------------------------
    // UI creation
    //----------------------------------------------------------------
    UICore uiCore;
    UiInterfaceManager uiInterfaceManager;

    QQmlApplicationEngine engine;
    engine.addImportPath(":/qml");
    engine.addImportPath(":/firmwares");
    engine.addImportPath(":/translations");
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));

    qmlRegisterSingletonInstance("CppObjects", 1, 0, "UiCore", &uiCore);
    qmlRegisterSingletonInstance("CppObjects", 1, 0, "InterfaceManager", &uiInterfaceManager);
    qmlRegisterSingletonInstance("CppObjects", 1, 0, "PresetListModel", &presetListModel);  

    qRegisterMetaType<DeviceDescription>();
    qmlRegisterUncreatableType<DeviceDescription>("CppObjects", 1, 0, "DeviceDescription", "");
    qmlRegisterUncreatableType<DeviceTypeEnum>("CppEnums", 1, 0, "DeviceType", "Not creatable as it is an enum type");
    //-------------------------------------------------------------------------------
    // connections
    //-------------------------------------------------------------------------------
    UICore::connect(&uiCore, &UICore::sgTranslatorChanged, &engine, &QQmlApplicationEngine::retranslate);

    QObject::connect(&uiCore, &UICore::sgReadAllParameters, core, &Core::readAllParameters);
    QObject::connect(&uiCore, &UICore::sgSetParameter, core, &Core::setParameter);
    QObject::connect(&uiCore, &UICore::sgRestoreValue, core, &Core::restoreValue);
    QObject::connect(&uiCore, &UICore::sgSetImpuls, core, &Core::setImpulse);
    QObject::connect(&uiCore, &UICore::sgSetFirmware, core, &Core::setFirmware, Qt::QueuedConnection);
    QObject::connect(&uiCore, &UICore::sgEscImpuls, core, &Core::escImpulse);
    QObject::connect(&uiCore, &UICore::sgExportPreset, core, &Core::exportPreset);
    QObject::connect(&uiCore, &UICore::sgImportPreset, core, &Core::importPreset);
    QObject::connect(&uiCore, &UICore::sgSw4Enable, core, &Core::sw4Enable);



    // TODO
    QObject::connect(&uiCore, &UICore::sgDoOnlineFirmwareUpdate, netCore, &NetCore::requestFirmwareFile);
    //

    QObject::connect(core, &Core::sgSetUIParameter, &uiCore, &UICore::sgSetUIParameter);
    QObject::connect(core, &Core::sgSetUIText, &uiCore, &UICore::sgSetUIText);
    QObject::connect(core, &Core::sgPresetChangeStage, &uiCore, &UICore::sgPresetChangeStage);
    QObject::connect(core, &Core::sgSetProgress, &uiCore, &UICore::sgSetProgress);
    QObject::connect(core, &Core::sgFirmwareVersionInsufficient, &uiCore, &UICore::slProposeOfflineFirmwareUpdate, Qt::QueuedConnection);

    QObject::connect(core, &Core::sgRefreshPresetList, &presetListModel, &PresetListModel::refreshModel, Qt::QueuedConnection);
    QObject::connect(core, &Core::sgUpdatePreset, &presetListModel, &PresetListModel::updatePreset, Qt::QueuedConnection);

    QObject::connect(core, &Core::sgRequestNewestFirmware, netCore, &NetCore::requestNewestFirmware);

    QObject::connect(netCore, &NetCore::sgFirmwareDownloaded, core, &Core::uploadFirmware);
    QObject::connect(netCore, &NetCore::sgNewFirmwareAvaliable, &uiCore, &UICore::slProposeNetFirmwareUpdate, Qt::QueuedConnection);
    QObject::connect(netCore, &NetCore::sgDownloadProgress, &uiCore, &UICore::sgDownloadProgress, Qt::QueuedConnection);

    Core::connect(bleInterface, &BleInterface::sgNewData, core, &Core::parseInputData);
    Core::connect(bleInterface, &BleInterface::sgInterfaceConnected, core, &Core::readAllParameters);
    Core::connect(core, &Core::sgWriteToInterface, bleInterface, &BleInterface::write);
    Core::connect(core, &Core::sgExchangeError, bleInterface, &BleInterface::disconnectFromDevice);
    Core::connect(core, &Core::sgReadyTodisconnect, bleInterface, &BleInterface::disconnectFromDevice);
    Core::connect(core, &Core::sgExchangeError, &uiInterfaceManager, &UiInterfaceManager::sgExchangeError);


    UiInterfaceManager::connect(&uiInterfaceManager, &UiInterfaceManager::sgStartScanning, bleInterface, &BleInterface::startScan, Qt::QueuedConnection);
    UiInterfaceManager::connect(&uiInterfaceManager, &UiInterfaceManager::sgConnectToDevice, bleInterface, &BleInterface::connect, Qt::QueuedConnection);
    UiInterfaceManager::connect(&uiInterfaceManager, &UiInterfaceManager::sgDisconnectFromDevice, bleInterface, &BleInterface::disconnectFromDevice, Qt::QueuedConnection);

    QObject::connect(&uiCore, &UICore::sgModuleNameChanged, bleInterface, &BleInterface::setModuleName);
    QObject::connect(bleInterface, &BleInterface::sgModuleNameUpdated, &uiCore, &UICore::setModuleName);

    QObject::connect(bleInterface, &BleInterface::sgDeviceListUpdated, &uiInterfaceManager, &UiInterfaceManager::updateDevicesList);
    QObject::connect(bleInterface, &BleInterface::sgConnectionStarted, &uiInterfaceManager, &UiInterfaceManager::sgConnectionStarted);
    QObject::connect(bleInterface, &BleInterface::sgInterfaceConnected, &uiInterfaceManager, &UiInterfaceManager::sgInterfaceConnected);
    QObject::connect(bleInterface, &BleInterface::sgInterfaceError, &uiInterfaceManager, &UiInterfaceManager::sgInterfaceError);
    QObject::connect(bleInterface, &BleInterface::sgInterfaceUnavaliable, &uiInterfaceManager, &UiInterfaceManager::slInterfaceUnavaliable);
    QObject::connect(bleInterface, &BleInterface::sgDeviceUnavaliable, &uiInterfaceManager, &UiInterfaceManager::sgDeviceUnavaliable);
    QObject::connect(bleInterface, &BleInterface::sgInterfaceDisconnected, &uiInterfaceManager, &UiInterfaceManager::sgInterfaceDisconnected);
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
