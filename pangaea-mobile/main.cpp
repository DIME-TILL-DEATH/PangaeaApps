#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include <signal.h>

#include <QDir>

#include <QDebug>

#include "core.h"
#include "uiinterfacemanager.h"
#include "netcore.h"

#include "threadcontroller.h"
#include "logger.h"

#include "interfacecore.h"

#include "filtertypeenum.h"
#include "deviceerrorenum.h"
#include "devicemessageenum.h"
#include "uimessagetype.h"

#include "uicore.h"
#include "uisettings.h"


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

#ifdef Q_OS_WINDOWS
    QApplication  app(argc, argv);
#else
    QGuiApplication app(argc, argv);
#endif

    app.setOrganizationName("AMT");
    app.setOrganizationDomain("amtelectronics.com");
    app.setApplicationName("pangaea mobile");

// #ifdef Q_OS_LINUX
//     QApplication::setWindowIcon(QIcon(":/qml/Images/AMT.svg"));
//     app.setApplicationVersion(VERSION_STRING);
// #endif

    Logger log;
    log.setAsMessageHandlerForApp();
    appLogger_ptr = &log;

    qInfo() << "App version: " << app.applicationVersion();

    Core* core = new Core;
    NetCore* netCore = new NetCore;
    InterfaceCore* interfaceManager = new InterfaceCore;

    ThreadController threadController(QThread::currentThread());
    core->moveToThread(threadController.backendThread());
    netCore->moveToThread(threadController.backendThread());
#if !defined(Q_OS_MACOS)
    // Нам не нужен доп поток для обслуживания соединения нигде?
    // и UIInterfaceManager тоже не нужен в итоге?
    // interfaceManager->moveToThread(threadController.connectionsThread()); // On MAC BLE can work only on the main thread
#endif

    QObject::connect(threadController.backendThread(), &QThread::finished, core, &QObject::deleteLater);
    QObject::connect(threadController.backendThread(), &QThread::finished, netCore, &QObject::deleteLater);
    // QObject::connect(threadController.backendThread(), &QThread::finished, interfaceManager, &QObject::deleteLater);

    //-----------------------------------------------------------------
    // UI creation
    //----------------------------------------------------------------
    UiCore uiCore;
    UiSettings uiSettings;
    UiInterfaceManager uiInterfaceManager;

    QQmlApplicationEngine engine;
    engine.addImportPath(":/qml");
    engine.addImportPath(":/firmwares");
    engine.addImportPath(":/translations");
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
#ifdef Q_OS_MACOS
    QQuickStyle::setStyle("Basic");
#endif

    qmlRegisterSingletonInstance("CppObjects", 1, 0, "UiCore", &uiCore);
    qmlRegisterSingletonInstance("CppObjects", 1, 0, "UiSettings", &uiSettings);
    qmlRegisterSingletonInstance("CppObjects", 1, 0, "InterfaceManager", &uiInterfaceManager);

    // qmlRegisterType<DeviceDescription>("CppObjects", 1, 0, "DeviceDescription");

    // qmlRegisterUncreatableType<FilterTypeEnum>("CppEnums", 1, 0, "FilterType", "Not creatable as it is an enum type");
    // qmlRegisterUncreatableType<DeviceTypeEnum>("CppEnums", 1, 0, "DeviceType", "Not creatable as it is an enum type");
    // qmlRegisterUncreatableType<DeviceConnectionTypeEnum>("CppEnums", 1, 0, "DeviceConnectionType", "Not creatable as it is an enum type");
    // qmlRegisterUncreatableType<PresetStateEnum>("CppEnums", 1, 0, "PresetState", "Not creatable as it is an enum type");
    // qmlRegisterUncreatableType<DeviceErrorEnum>("CppEnums", 1, 0, "DeviceErrorType", "Not creatable as it is an enum type");
    // qmlRegisterUncreatableType<DeviceMessageEnum>("CppEnums", 1, 0, "DeviceMessageType", "Not creatable as it is an enum type");
    // qmlRegisterUncreatableType<UiMessageTypeEnum>("CppEnums", 1, 0, "UiMessageType", "Not creatable as it is an enum type");
    // qmlRegisterUncreatableType<ModuleTypeEnum>("CppEnums", 1, 0, "ModuleType", "Not creatable as it is an enum type");

    AbstractModule::registerTypestoQml();

    //-------------------------------------------------------------------------------
    // connections
    //-------------------------------------------------------------------------------
    // QObject::connect(core, &Core::sgFirmwareVersionInsufficient, &uiCore, &UiCore::slProposeOfflineFirmwareUpdate, Qt::QueuedConnection);
    // QObject::connect(netCore, &NetCore::sgFirmwareDownloaded, core, &Core::uploadFirmware);
    UiCore::connect(&uiCore, &UiCore::sgTranslatorChanged, &engine, &QQmlApplicationEngine::retranslate);

    QObject::connect(core, &Core::sgSetUIParameter, &uiCore, &UiCore::sgSetUIParameter, Qt::QueuedConnection);
    QObject::connect(core, &Core::sgCurrentDeviceChanged, &uiCore, &UiCore::slCurrentDeviceChanged, Qt::QueuedConnection);
    QObject::connect(core, &Core::sgSetProgress, &uiCore, &UiCore::sgSetProgress, Qt::QueuedConnection);

    QObject::connect(core, &Core::sgRequestNewestFirmware, netCore, &NetCore::requestNewestFirmware);
    QObject::connect(netCore, &NetCore::sgNewFirmwareAvaliable, &uiCore, &UiCore::slProposeNetFirmwareUpdate, Qt::QueuedConnection);
    QObject::connect(&uiCore, &UiCore::sgDoOnlineFirmwareUpdate, netCore, &NetCore::requestFirmwareFile);
    QObject::connect(netCore, &NetCore::sgDownloadProgress, &uiCore, &UiCore::sgDownloadProgress, Qt::QueuedConnection);

    Core::connect(interfaceManager, &InterfaceCore::sgNewData, core, &Core::parseInputData, Qt::QueuedConnection);
    Core::connect(interfaceManager, &InterfaceCore::sgInterfaceConnected, core, &Core::slInterfaceConnected, Qt::QueuedConnection);

    // disconnect
    QObject::connect(&uiCore, &UiCore::sgDisconnectFromDevice, core, &Core::disconnectFromDevice, Qt::QueuedConnection);
    Core::connect(core, &Core::sgReadyToDisconnect, interfaceManager, &InterfaceCore::disconnectFromDevice, Qt::QueuedConnection);

    QObject::connect(interfaceManager, &InterfaceCore::sgErrorDisconnect, &uiCore, &UiCore::disconnectFromDevice);
    Core::connect(core, &Core::sgExchangeError, &uiInterfaceManager, &UiInterfaceManager::sgExchangeError, Qt::QueuedConnection);

    Core::connect(core, &Core::sgWriteToInterface, interfaceManager, &InterfaceCore::writeToDevice, Qt::QueuedConnection);

    UiInterfaceManager::connect(&uiInterfaceManager, &UiInterfaceManager::startScanning, interfaceManager, &InterfaceCore::startScanning);
    UiInterfaceManager::connect(&uiInterfaceManager, &UiInterfaceManager::connectToDevice, interfaceManager, &InterfaceCore::connectToDevice);
    UiInterfaceManager::connect(&uiInterfaceManager, &UiInterfaceManager::sgRssiMeasuring, interfaceManager, &InterfaceCore::rssiMeasuring);

    QObject::connect(&uiCore, &UiCore::sgModuleNameChanged, interfaceManager, &InterfaceCore::setModuleName);
    QObject::connect(interfaceManager, &InterfaceCore::sgModuleNameUpdated, &uiCore, &UiCore::setModuleName);

    // TODO InterfaceCore теперь в UI thread. Нужен ли отдельный объект?
    QObject::connect(interfaceManager, &InterfaceCore::sgDeviceListUpdated, &uiInterfaceManager, &UiInterfaceManager::updateDevicesList);
    QObject::connect(interfaceManager, &InterfaceCore::sgConnectionStarted, &uiInterfaceManager, &UiInterfaceManager::sgConnectionStarted);
    QObject::connect(interfaceManager, &InterfaceCore::sgInterfaceConnected, &uiInterfaceManager, &UiInterfaceManager::sgInterfaceConnected);
    QObject::connect(interfaceManager, &InterfaceCore::sgInterfaceError, &uiInterfaceManager, &UiInterfaceManager::sgInterfaceError);
    QObject::connect(interfaceManager, &InterfaceCore::sgInterfaceUnavaliable, &uiInterfaceManager, &UiInterfaceManager::slInterfaceUnavaliable);
    QObject::connect(interfaceManager, &InterfaceCore::sgDeviceUnavaliable, &uiInterfaceManager, &UiInterfaceManager::sgDeviceUnavaliable);
    QObject::connect(interfaceManager, &InterfaceCore::sgInterfaceDisconnected, &uiInterfaceManager, &UiInterfaceManager::sgInterfaceDisconnected);
    QObject::connect(interfaceManager, &InterfaceCore::sgRssiReaded, &uiInterfaceManager, &UiInterfaceManager::setRssi);
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
