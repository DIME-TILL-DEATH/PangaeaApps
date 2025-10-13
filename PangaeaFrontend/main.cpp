#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include <qicon.h>
#include <signal.h>

#include <QDir>

#include <QDebug>

#include "core.h"
#include "uiinterfacemanager.h"
#include "netcore.h"

#include "threadcontroller.h"
#include "logger.h"

#include "interfacemanager.h"

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

    QGuiApplication app(argc, argv);

    app.setOrganizationName("AMT");
    app.setOrganizationDomain("amtelectronics.com");
    app.setApplicationName("pangaea mobile");

#if defined(Q_OS_LINUX) & !defined(Q_OS_ANDROID)
    QGuiApplication::setWindowIcon(QIcon("qrc:/Images/AMT.svg"));
    app.setApplicationVersion(VERSION_STRING);
#endif

    Logger log;
    log.setAsMessageHandlerForApp();
    appLogger_ptr = &log;

    qInfo() << "App version: " << app.applicationVersion();

    Core* core = new Core;
    NetCore* netCore = new NetCore;
    InterfaceManager* interfaceManager = new InterfaceManager;

    ThreadController threadController(QThread::currentThread());
    core->moveToThread(threadController.backendThread());
    netCore->moveToThread(threadController.backendThread());
    QObject::connect(threadController.backendThread(), &QThread::finished, core, &QObject::deleteLater);
    QObject::connect(threadController.backendThread(), &QThread::finished, netCore, &QObject::deleteLater);

    //-----------------------------------------------------------------
    // UI creation
    //----------------------------------------------------------------
    UiCore uiCore;
    UiSettings uiSettings;
    UiInterfaceManager uiInterfaceManager;

    QQmlApplicationEngine engine;

    engine.addImportPath("qml/");
    engine.addImportPath(":/firmwares");
    engine.addImportPath(":/translations");

    qmlRegisterSingletonInstance("CppObjects", 1, 0, "UiCore", &uiCore);
    qmlRegisterSingletonInstance("CppObjects", 1, 0, "UiSettings", &uiSettings);
    qmlRegisterSingletonInstance("CppObjects", 1, 0, "InterfaceManager", &uiInterfaceManager);

    //-------------------------------------------------------------------------------
    // connections
    //-------------------------------------------------------------------------------
    // QObject::connect(core, &Core::sgFirmwareVersionInsufficient, &uiCore, &UiCore::slProposeOfflineFirmwareUpdate, Qt::QueuedConnection);
    // QObject::connect(netCore, &NetCore::sgFirmwareDownloaded, core, &Core::uploadFirmware);

    UiCore::connect(&uiCore, &UiCore::sgTranslatorChanged, &engine, &QQmlApplicationEngine::retranslate);
    UiSettings::connect(&uiSettings, &UiSettings::sgTranslatorChanged, &engine, &QQmlApplicationEngine::retranslate);

    QObject::connect(core, &Core::sgSetUIParameter, &uiCore, &UiCore::sgSetUIParameter, Qt::QueuedConnection);
    QObject::connect(core, &Core::sgCurrentDeviceChanged, &uiCore, &UiCore::slCurrentDeviceChanged, Qt::QueuedConnection);
    QObject::connect(core, &Core::sgSetProgress, &uiCore, &UiCore::sgSetProgress, Qt::QueuedConnection);

    QObject::connect(core, &Core::sgRequestNewestFirmware, netCore, &NetCore::requestNewestFirmware);
    QObject::connect(netCore, &NetCore::sgNewFirmwareAvaliable, &uiCore, &UiCore::slProposeNetFirmwareUpdate, Qt::QueuedConnection);
    QObject::connect(&uiCore, &UiCore::sgDoOnlineFirmwareUpdate, netCore, &NetCore::requestFirmwareFile);
    QObject::connect(&uiCore, &UiCore::sgCheckAppUpdates, netCore, &NetCore::requestAppUpdates);
    QObject::connect(netCore, &NetCore::sgDownloadProgress, &uiCore, &UiCore::sgDownloadProgress, Qt::QueuedConnection);

    Core::connect(interfaceManager, &InterfaceManager::sgNewData, core, &Core::parseInputData, Qt::QueuedConnection);
    Core::connect(interfaceManager, &InterfaceManager::sgInterfaceConnected, core, &Core::slInterfaceConnected, Qt::QueuedConnection);

    // disconnect
    QObject::connect(&uiCore, &UiCore::sgDisconnectFromDevice, core, &Core::disconnectFromDevice, Qt::QueuedConnection);
    Core::connect(core, &Core::sgReadyToDisconnect, interfaceManager, &InterfaceManager::disconnectFromDevice, Qt::QueuedConnection);

    QObject::connect(interfaceManager, &InterfaceManager::sgErrorDisconnect, &uiCore, &UiCore::disconnectFromDevice);
    Core::connect(core, &Core::sgExchangeError, &uiInterfaceManager, &UiInterfaceManager::sgExchangeError, Qt::QueuedConnection);

    Core::connect(core, &Core::sgWriteToInterface, interfaceManager, &InterfaceManager::writeToDevice, Qt::QueuedConnection);

    UiInterfaceManager::connect(&uiInterfaceManager, &UiInterfaceManager::startScanning, interfaceManager, &InterfaceManager::startScanning);
    UiInterfaceManager::connect(&uiInterfaceManager, &UiInterfaceManager::sgConnectToDevice, interfaceManager, &InterfaceManager::connectToDevice);
    UiInterfaceManager::connect(&uiInterfaceManager, &UiInterfaceManager::sgRssiMeasuring, interfaceManager, &InterfaceManager::rssiMeasuring);
    UiInterfaceManager::connect(&uiInterfaceManager, &UiInterfaceManager::logDataChanged, interfaceManager, &InterfaceManager::setLogEnadled);

    QObject::connect(&uiCore, &UiCore::sgModuleNameChanged, interfaceManager, &InterfaceManager::setModuleName);
    QObject::connect(interfaceManager, &InterfaceManager::sgModuleNameUpdated, &uiCore, &UiCore::setModuleName);

    // TODO InterfaceCore теперь в UI thread. Нужен ли отдельный объект?
    QObject::connect(interfaceManager, &InterfaceManager::sgDeviceListUpdated, &uiInterfaceManager, &UiInterfaceManager::updateDevicesList);
    QObject::connect(interfaceManager, &InterfaceManager::sgConnectionStarted, &uiInterfaceManager, &UiInterfaceManager::sgConnectionStarted);
    QObject::connect(interfaceManager, &InterfaceManager::sgInterfaceConnected, &uiInterfaceManager, &UiInterfaceManager::slInterfaceConnected);
    QObject::connect(interfaceManager, &InterfaceManager::sgInterfaceError, &uiInterfaceManager, &UiInterfaceManager::sgInterfaceError);
    QObject::connect(interfaceManager, &InterfaceManager::sgInterfaceUnavaliable, &uiInterfaceManager, &UiInterfaceManager::slInterfaceUnavaliable);
    QObject::connect(interfaceManager, &InterfaceManager::sgDeviceUnavaliable, &uiInterfaceManager, &UiInterfaceManager::sgDeviceUnavaliable);
    QObject::connect(interfaceManager, &InterfaceManager::sgInterfaceDisconnected, &uiInterfaceManager, &UiInterfaceManager::sgInterfaceDisconnected);
    QObject::connect(interfaceManager, &InterfaceManager::sgRssiReaded, &uiInterfaceManager, &UiInterfaceManager::setRssi);
    //----------------------------------------------------------------

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

#ifdef Q_OS_IOS
    engine.addImportPath(":/");
    engine.loadFromModule("Pages", "Main");
#elif defined(Q_OS_ANDROID)
    engine.loadFromModule("Pages", "Main");
#else
    engine.addImportPath(":/");
    engine.loadFromModule("Layouts", "Main");
#endif

#ifdef Q_OS_ANDROID
    //-----------------------------------------------------------------------
    // keep screen always on
    // also WAKE_LOCK permision in manifest
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
