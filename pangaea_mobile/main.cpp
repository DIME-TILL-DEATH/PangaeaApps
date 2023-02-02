#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include <QBluetoothLocalDevice>

#include <signal.h>

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

#include <QDir>

#include <QDebug>

#include "core.h"
#include "uicore.h"
#include "netcore.h"
#include "threadcontroller.h"
#include "bluetoothleuart.h"
#include "logger.h"

void manageSegFailure(int signalCode);
Logger* Logger::currentHandler = nullptr;
Logger* appLogger_ptr;

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);


    app.setOrganizationName("AMT");
    app.setOrganizationDomain("amtelectronics.com");
    app.setApplicationName("pangaea mobile");

    Logger log;
    log.setAsMessageHandlerForApp();

    QQmlApplicationEngine engine;

    engine.addImportPath(":/qml");
    engine.addImportPath(":/firmwares");
    engine.addImportPath(":/translations");
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));

    qRegisterMetaType<BluetoothleUART::bluetoothleState>("BluetoothState");
    qRegisterMetaType<QBluetoothDeviceInfo>("BluetoothDeviceInfo");

    BluetoothleUART bleConnection;
    Core core(&bleConnection);
    UICore uiCore(&bleConnection, &engine);
    NetCore netCore;

    QQmlContext *ctxt;
    ctxt = engine.rootContext();
    ctxt->setContextProperty("_uiCore", &uiCore);
    core.registerQmlObjects(ctxt);

    ThreadController threadController(QThread::currentThread());
 //   log.moveToThread(threadController.backendThread());
    core.moveToThread(threadController.backendThread());
    netCore.moveToThread(threadController.backendThread());
    bleConnection.moveToThread(threadController.backendThread());

    signal(SIGSEGV, manageSegFailure);
    signal(SIGFPE, manageSegFailure);
    signal(SIGILL, manageSegFailure);

    netCore.initialize();
    //-------------------------------------------------------------------------------
    // connections
    //-------------------------------------------------------------------------------
    QObject::connect(threadController.backendThread(), &QThread::finished, &core, &Core::stopTimer, Qt::QueuedConnection);

    QObject::connect(&uiCore, &UICore::sgReadAll, &core, &Core::readAllParameters);
    QObject::connect(&uiCore, &UICore::sgSetParameter, &core, &Core::setParameter);
    QObject::connect(&uiCore, &UICore::sgRestoreValue, &core, &Core::restoreValue);
    QObject::connect(&uiCore, &UICore::sgSetImpuls, &core, &Core::setImpulse);
    QObject::connect(&uiCore, &UICore::sgSetFirmware, &core, &Core::setFirmware, Qt::QueuedConnection);
    QObject::connect(&uiCore, &UICore::sgEscImpuls, &core, &Core::escImpulse);
    QObject::connect(&uiCore, &UICore::sgSw4Enable, &core, &Core::sw4Enable);
    QObject::connect(&uiCore, &UICore::sgTranslatorChanged, &core, &Core::readPresetState);
    QObject::connect(&uiCore, &UICore::sgExportPreset, &core, &Core::exportPreset);
    QObject::connect(&uiCore, &UICore::sgImportPreset, &core, &Core::importPreset);

//    QObject::connect(&uiCore, &UICore::sgModuleNameChanged, &core, &Core::setModuleName);
    QObject::connect(&uiCore, &UICore::sgModuleNameChanged, &bleConnection, &BluetoothleUART::setModuleName);

    QObject::connect(&uiCore, &UICore::sgDoOnlineFirmwareUpdate, &netCore, &NetCore::requestFirmwareFile);

    QObject::connect(&core, &Core::sgSetUIParameter, &uiCore, &UICore::sgSetUIParameter);
    QObject::connect(&core, &Core::sgSetUIText, &uiCore, &UICore::sgSetUIText);
    QObject::connect(&core, &Core::sgUpdateBLEDevicesList, &uiCore, &UICore::sgUpdateBLEDevicesList);
    QObject::connect(&core, &Core::sgConnectReady, &uiCore, &UICore::sgConnectReady);
    QObject::connect(&core, &Core::sgPresetChangeStage, &uiCore, &UICore::sgPresetChangeStage);
    QObject::connect(&core, &Core::sgSetProgress, &uiCore, &UICore::sgSetProgress);
    QObject::connect(&core, &Core::sgModuleNameUpdated, &uiCore, &UICore::setModuleName);
    QObject::connect(&core, &Core::sgFirmwareVersionInsufficient, &uiCore, &UICore::slProposeOfflineFirmwareUpdate, Qt::QueuedConnection);

    QObject::connect(&core, &Core::sgRequestNewestFirmware, &netCore, &NetCore::requestNewestFirmware);

    QObject::connect(&netCore, &NetCore::sgFirmwareDownloaded, &core, &Core::uploadFirmware);
    QObject::connect(&netCore, &NetCore::sgNewFirmwareAvaliable, &uiCore, &UICore::slProposeNetFirmwareUpdate, Qt::QueuedConnection);
    QObject::connect(&netCore, &NetCore::sgDownloadProgress, &uiCore, &UICore::sgDownloadProgress, Qt::QueuedConnection);
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

                // TODO Получить размеры экрана через JAVA и изменить размер
                // TODO get flags from JNI, or call java function. Class View
//                int SYSTEM_UI_FLAG_FULLSCREEN = 0x00000004;
//                int SYSTEM_UI_FLAG_HIDE_NAVIGATION = 0x00000002;
//                int SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION = 0x00000200;
//                int SYSTEM_UI_FLAG_LAYOUT_STABLE = 0x00000100;
//                int SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN = 0x00000400;
//                int SYSTEM_UI_FLAG_IMMERSIVE_STICKY = 0x00001000;

//                QJniObject decorView = window.callObjectMethod("getDecorView", "()Landroid/view/View;");
//               // resize, fullscreen
//                int flags = SYSTEM_UI_FLAG_HIDE_NAVIGATION|
//                            SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION;
//                decorView.callMethod<void>("setSystemUiVisibility", "(I)V", flags);

//                jint screenHeight = decorView.callMethod<jint>("getPaddingBottom");
//                qDebug() << "Padding bottom" << screenHeight;
//                window.callObjectMethod("setLayout", "(I;I;)V", 1080 ,2340);

//                decorView.callMethod<void>("setMinimumHeight", "(I)V", 2340);
//                jint screenHeight = decorView.callMethod<jint>("getHeight");
//                qDebug() << "Screen height" << screenHeight;

            }
        });
#endif
    //----------------------------------------------------------------------
    return app.exec();
}

// на windows обрабатывает только основной поток. На UNIX системах ловит все потоки.
void manageSegFailure(int signalCode)
{
    qWarning() << "Critical fault! Code:" << signalCode;
    appLogger_ptr->~Logger();

    signal(signalCode, SIG_DFL);
    QGuiApplication::exit(3);
}
