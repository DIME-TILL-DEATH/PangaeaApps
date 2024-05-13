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


#include "core.h"


//TODO: class enum UIValueItem, UITextItem, UIErrorItem
class UiCore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString moduleName READ moduleName WRITE setModuleName NOTIFY sgModuleNameChanged)
    Q_PROPERTY(bool presetModified READ presetModified NOTIFY presetModifiedChanged FINAL)
public:
    explicit UiCore(QObject *parent = nullptr);


    Q_INVOKABLE void setupApplication();

    Q_INVOKABLE void setParameter(QString name, quint8 val);

    Q_INVOKABLE void setDeviceParameter(DeviceParameter::Type deviceParameterType, quint8 value)
    {
        emit sgSetDeviceParameter(deviceParameterType, value);
    };
    Q_INVOKABLE void restoreParameter(QString name);

    Q_INVOKABLE void readAll(void);

    Q_INVOKABLE void setImpuls(QString fullFilePath);
    Q_INVOKABLE void convertAndUploadImpulse(QString filePath);
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

    bool presetModified() const {return m_presetModified;};

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

    QString m_pickedImpulsePath;

    void loadTranslator(QString languageCode);
    void loadDefaultTranslator();

    void pickFile(ActivityType fileType, QString filter);

    bool m_presetModified;

signals:
    void sgTranslatorChanged(QString langauageCode);

    void sgSetUiDeviceParameter(DeviceParameter::Type deviceParameterType, qint32 value);
    void sgSetUIParameter(QString nameParam, qint32 inValue);
    void sgSetUIText(QString nameParam, QString value);
    void sgUpdateAppSetting(QString settingName, QVariant settingValue);

    void sgPresetChangeStage (quint8 inChangePreset);
    void sgSetProgress(float val, QString extText);
    void sgDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    //-----------------------------------------
    void sgLocalBluetoothNotReady(QString reason);
    //-----------------------------------------------
    void sgReadAllParameters();
    void sgSetParameter(QString name, quint8 value);
    void sgSetDeviceParameter(DeviceParameter::Type deviceParameterType, quint8 value);

    void sgRestoreValue(QString name);
    void sgSetImpuls (QString filePath, QString fileName);
    void sgSetFirmware (QString fullFilePath);

    void sgExportPreset(QString filePath, QString fileName);
    void sgImportPreset(QString filePath, QString fileName);

    void sgEscImpuls();
    void sgSw4Enable();
    void sgModuleNameChanged(QString name);

    void sgDoOnlineFirmwareUpdate();

    void presetModifiedChanged();

public slots:
    void slFirmwareFilePicked(QString filePath, QString fileName);
    void slProposeNetFirmwareUpdate(Firmware* updateFirmware, Firmware* oldFirmware);
    void slProposeOfflineFirmwareUpdate(Firmware *minimalFirmware, Firmware *actualFirmware);

    void slSetAppParameter(Core::AppParameter appParameterType, QVariant content);

private slots:
    void slImpulseFilePicked(QString filePath, QString fileName);
};

#endif // UICORE_H
