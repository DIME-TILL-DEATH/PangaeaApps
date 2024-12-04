#ifndef UICORE_H
#define UICORE_H

#include <QQmlApplicationEngine>
#include <QtQml>

#include <QObject>
#include <QTimer>
#include <QSettings>

#include <QTranslator>

#include "activityresultmanager.h"

#include "deviceparameter.h"
#include "firmware.h"

#include "abstractdevice.h"


class UiCore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString moduleName READ moduleName WRITE setModuleName NOTIFY sgModuleNameChanged FINAL)

    Q_PROPERTY(AbstractDevice* currentDevice READ currentDevice NOTIFY currentDeviceChanged FINAL)
public:
    explicit UiCore(QObject *parent = nullptr);

    Q_INVOKABLE void setupApplication();

    Q_INVOKABLE void disconnectFromDevice();

    Q_INVOKABLE void uploadIr(QString fullFilePath, QString dstFilePath = "");
    Q_INVOKABLE void convertAndUploadIr(QString srcFilePath, QString dstFilePath = "");

    Q_INVOKABLE void exportPreset(QString fileName);
    Q_INVOKABLE void importPreset(QString filePath);

    Q_INVOKABLE void pickFirmwareFile();
    Q_INVOKABLE void setFirmware (QString fullFilePath);
    Q_INVOKABLE void doOnlineFirmwareUpdate();

    Q_INVOKABLE void setLanguage(QString languageCode);
    Q_INVOKABLE void saveSetting(QString settingName, QVariant settingValue);

    Q_INVOKABLE void openManualExternally(QString fileName);

    const QString &moduleName() const {return m_moduleName;};
    void setModuleName(const QString &newModuleName);

    AbstractDevice *currentDevice() const;

signals:

    void sgQmlRequestChangePreset(quint8 bank, quint8 preset);

    void sgSetUIParameter(QString nameParam, qint32 inValue);
    void sgSetUIText(QString nameParam, QString value);
    void sgUpdateAppSetting(QString settingName, QVariant settingValue);

    void sgSetProgress(float val, QString extText);
    void sgDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    //-----------------------------------------
    void sgLocalBluetoothNotReady(QString reason);
    //-----------------------------------------------
    void sgReadAllParameters();

    void sgSetFirmware (QString fullFilePath);

    void sgExportPreset(QString filePath, QString fileName);
    void sgImportPreset(QString filePath, QString fileName);

    void sgDisconnectFromDevice();
    void sgModuleNameChanged(QString name);

    void sgDoOnlineFirmwareUpdate();

    void sgTranslatorChanged(QString langauageCode);
    void currentDeviceChanged();

public slots:
    void slFirmwareFilePicked(QString filePath, QString fileName);
    void slProposeNetFirmwareUpdate(Firmware* updateFirmware, Firmware* oldFirmware);
    void slProposeOfflineFirmwareUpdate(Firmware *minimalFirmware, Firmware *actualFirmware);

    void slCurrentDeviceChanged(AbstractDevice* newDevice);

    void slExportPreset(QString fullFilePath, QString fileName);
    void slImportPreset(QString fullFilePath, QString fileName);

    // void slDeviceDisconnected();

private slots:
    void slImpulseFilePicked(QString filePath, QString fileName);

private:
    QQmlApplicationEngine* m_qmlEngine;

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

    QString m_pickedIrPath;
    QString m_dstIrPath;

    AbstractDevice dummyDevice{nullptr};

    void loadTranslator(QString languageCode);
    void loadDefaultTranslator();

    void pickFile(ActivityType fileType, QString filter);

    AbstractDevice *m_currentDevice = nullptr;
};

#endif // UICORE_H
