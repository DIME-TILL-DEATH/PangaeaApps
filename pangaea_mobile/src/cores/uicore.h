#ifndef UICORE_H
#define UICORE_H

#include <QQmlApplicationEngine>
#include <QtQml>

#include <QObject>
#include <QTimer>
#include <QSettings>

#include <QTranslator>


#include "activityresultmanager.h"
#include "firmware.h"
#include "bluetoothleuart.h"


//TODO: class enum UIValueItem, UITextItem, UIErrorItem
class UICore : public QObject
{
    Q_OBJECT
public:
    explicit UICore(BluetoothleUART *bleConnection, QQmlApplicationEngine* engine, QObject *parent = nullptr);

    Q_PROPERTY(QString moduleName READ moduleName WRITE setModuleName NOTIFY sgModuleNameChanged)

    Q_INVOKABLE void setupApplication();

    Q_INVOKABLE void connectToDevice(quint8 devNum);
    Q_INVOKABLE void disconnectFromDevice(void);

    Q_INVOKABLE void doConnect(quint8 numDev, QString address);
    Q_INVOKABLE void rescanDevices();

    Q_INVOKABLE void setParameter(QString name, quint8 val);
    Q_INVOKABLE void restoreParameter(QString name);

    Q_INVOKABLE void readAll(void);

    Q_INVOKABLE void setImpuls(QString fullFilePath);
    Q_INVOKABLE void escImpuls();

    Q_INVOKABLE void exportPreset(QString fileName);
    Q_INVOKABLE void importPreset(QString filePath);

    Q_INVOKABLE void pickFirmwareFile();
    Q_INVOKABLE void setFirmware (QString fullFilePath);
    Q_INVOKABLE void doOnlineFirmwareUpdate();

    Q_INVOKABLE void setLanguage(QString languageCode);
    Q_INVOKABLE void saveSetting(QString settingName, QVariant settingValue);

    Q_INVOKABLE void sw4Enable();

    Q_INVOKABLE void openManualExternally(QString fileName);

    const QString &moduleName() const;
    void setModuleName(const QString &newModuleName);

private:
    QQmlApplicationEngine* m_qmlEngine;

    BluetoothleUART* m_bleConnection;
    QTranslator m_translator;

    QString m_moduleName;

    QSettings* appSettings;

    QMap<QString, QString> pathFromCode
    {
        {"en", ":/translations/pangaea-mobile_en.qm"},
        {"ru", ":/translations/pangaea-mobile_ru.qm"},
        {"it", ":/translations/pangaea-mobile_it.qm"},
        {"de", ":/translations/pangaea-mobile_de.qm"}
    };


    void loadTranslator(QString languageCode);
    void loadDefaultTranslator();

    void pickFile(ActivityType fileType, QString filter);

signals:
    void sgStartScan();
    void sgUpdateBLEDevicesList(QStringList str);
    void sgConnectReady(void);
    void sgConnectToDevice(int numDevice);
    void sgDoConnect(quint8 numDev, QString address);
    void sgDoDisconnect();

    void sgTranslatorChanged(QString langauageCode);

    void sgSetUIParameter(QString nameParam, qint32 inValue);
    void sgSetUIText(QString nameParam, QString value);
    void sgUpdateAppSetting(QString settingName, QVariant settingValue);

    void sgPresetChangeStage (quint8 inChangePreset);
    void sgSetProgress(float val, QString extText);
    void sgDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    //-----------------------------------------
    void sgLocalBluetoothNotReady(QString reason);
    //-----------------------------------------------
    void sgReadAll();
    void sgSetParameter(QString name, quint8 value);
    void sgRestoreValue(QString name);
    void sgSetImpuls (QString filePath, QString fileName);
    void sgSetFirmware (QString fullFilePath);

    void sgExportPreset(QString filePath, QString fileName);
    void sgImportPreset(QString filePath, QString fileName);

    void sgEscImpuls();
    void sgSw4Enable();
    void sgModuleNameChanged(QString name);

    void sgDoOnlineFirmwareUpdate();

public slots:
    void slFirmwareFilePicked(QString filePath, QString fileName);
    void slProposeNetFirmwareUpdate(Firmware* updateFirmware, Firmware* oldFirmware);
    void slProposeOfflineFirmwareUpdate(Firmware *minimalFirmware, Firmware *actualFirmware);
};

#endif // UICORE_H
