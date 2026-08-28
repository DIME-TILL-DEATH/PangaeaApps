#ifndef UICORE_H
#define UICORE_H

#include <QQmlApplicationEngine>

#include <QTimer>
#include <QSettings>
#include <QTranslator>

#ifdef __ANDROID__
#include "activityresultmanager.h"
#endif

#include "firmware.h"
#include "abstractdevice.h"
#include "uimessagetype.h"

class UiCore : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QString moduleName READ moduleName WRITE setModuleName NOTIFY sgModuleNameChanged FINAL)
    Q_PROPERTY(AbstractDevice* currentDevice READ currentDevice NOTIFY currentDeviceChanged FINAL)

    Q_PROPERTY(quint16 apiVersion READ apiVersion CONSTANT)
public:
    explicit UiCore(QObject *parent = nullptr);
    ~UiCore();

    Q_INVOKABLE void disconnectFromDevice();

    Q_INVOKABLE void uploadIr(QUrl srcFilePath, QUrl dstFilePath = QUrl());
    Q_INVOKABLE void uploadIr(QList<QUrl> fileList, QUrl dstFilePath = QUrl());

    Q_INVOKABLE void convertAndUploadIr();

    Q_INVOKABLE void exportPreset(QUrl dstPath = QUrl());
    Q_INVOKABLE void importPreset(QString filePath);

#ifdef Q_OS_ANDROID
    Q_INVOKABLE void pickFirmwareFile();
#endif
    Q_INVOKABLE void doOnlineFirmwareUpdate();

    Q_INVOKABLE void openManualExternally(QString fileName);
    Q_INVOKABLE void runIrConvertor();

    const QString &moduleName() const {return m_moduleName;};
    void setModuleName(const QString &newModuleName);

    AbstractDevice *currentDevice() const {return m_currentDevice;};

    quint16 apiVersion();

    static UiCore* instance;
    static QObject* singletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine) {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return instance;
    }

signals:

    void sgQmlRequestChangePreset(quint8 bank, quint8 preset);

    void sgUiMessage(UiMessageTypeEnum::Value messageType, QString message = "", QVariantList params = {});

    void sgUpdateAppSetting(QString settingName, QVariant settingValue);

    void sgSetProgress(float val, QString extText);
    void sgDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    //-----------------------------------------
    void sgLocalBluetoothNotReady(QString reason);
    //-----------------------------------------------

    void sgSetFirmware (QString fullFilePath);

    void sgExportPreset(QString filePath, QString fileName);
    void sgImportPreset(QString filePath, QString fileName);

    void sgDisconnectFromDevice();
    void sgModuleNameChanged(QString name);

    void sgDoOnlineFirmwareUpdate();
    void sgNewAppVersionAvaliable(QString appVersion);
    void sgNewFirmwareAvaliable(QString firmwareVersion);

    void currentDeviceChanged();

    void sgCheckAppUpdates();

    void sgFirmwareFilePicked(QString filePath, QString fileName);

public slots:
    void slFirmwareFilePicked(QString filePath, QString fileName);

    void slProposeNetFirmwareUpdate(Firmware* updateFirmware, Firmware* oldFirmware);

    void slCurrentDeviceChanged(AbstractDevice* newDevice);

    void slExportPreset(QString fullFilePath, QString fileName);
    void slImportPreset(QString fullFilePath, QString fileName);

    Q_INVOKABLE void impulseUploaded();

private:

    QQmlApplicationEngine* m_qmlEngine;

    QString m_moduleName;

    QSettings* appSettings;

    QList<QUrl> m_uploadFileList;


    QString m_pickedIrPath;
    QString m_dstIrPath;

    AbstractDevice dummyDevice{nullptr};

    void uploadIr(QString srcFilePath, QString dstFilePath = "");
#ifdef Q_OS_ANDROID
    void pickFile(ActivityType fileType, QString filter, bool allowMultiple = true);
#endif

    AbstractDevice *m_currentDevice = nullptr;
};

#endif // UICORE_H
