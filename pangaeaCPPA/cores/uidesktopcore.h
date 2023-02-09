#ifndef UIDESKTOPCORE_H
#define UIDESKTOPCORE_H

#include <QObject>
#include <QTimer>
#include <QSettings>

#include <QTranslator>

#include "firmware.h"

//TODO: class enum UIValueItem, UITextItem, UIErrorItem
class UiDesktopCore : public QObject
{
    Q_OBJECT
public:
    explicit UiDesktopCore(QObject *parent = nullptr);

    Q_INVOKABLE void setupApplication();

    Q_INVOKABLE void setParameter(QString name, quint8 val);
    Q_INVOKABLE void restoreParameter(QString name);

    Q_INVOKABLE void setImpuls(QString fullFilePath);
    Q_INVOKABLE void escImpuls();
    Q_INVOKABLE void convertAndUploadImpulse(QString fullFilePath);

    Q_INVOKABLE void readAllParameters();

    Q_INVOKABLE void exportPreset(QString filePath);
    Q_INVOKABLE void importPreset(QString filePath);

    //Q_INVOKABLE void pickFirmwareFile();
    //Q_INVOKABLE void setFirmware (QString fullFilePath);
    //Q_INVOKABLE void doOnlineFirmwareUpdate();

    Q_INVOKABLE void setLanguage(QString languageCode);
    Q_INVOKABLE void saveSetting(QString settingName, QVariant settingValue);

    Q_INVOKABLE void openManualExternally(QString fileName);
    Q_INVOKABLE void runIrConvertor();

    const QString &moduleName() const;
    void setModuleName(const QString &newModuleName);

private:

    QTranslator m_translator;

    QString m_moduleName;

    QSettings* appSettings;

    QMap<QString, QString> pathFromCode
    {
        {"en", ":/translations/pangaea_desktop_en.qm"},
        {"ru", ":/translations/pangaea_desktop_ru.qm"},
        {"it", ":/translations/pangaea_desktop_it.qm"},
        {"de", ":/translations/pangaea_desktop_de.qm"}
    };
    void loadTranslator(QString languageCode);
    void loadDefaultTranslator();

signals:
   // void sgStartScan();
   // void sgUpdateBLEDevicesList(QStringList str);
    void sgConnectReady(void);
   // void sgConnectToDevice(DeviceDescription device);
//    void sgDoConnect(quint8 numDev, QString address);
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
    void sgReadAllParameters();
    void sgSetParameter(QString name, quint8 value);
    void sgRestoreValue(QString name);
    void sgSetImpuls (QString filePath, QString fileName);
    void sgSetFirmware (QString fullFilePath);

    void sgExportPreset(QString filePath, QString fileName);
    void sgImportPreset(QString filePath, QString fileName);

    void sgEscImpuls();
    void sgSw4Enable();
    void sgModuleNameChanged(QString name);

    void sgUiClosing();

    void sgDoOnlineFirmwareUpdate();

public slots:
    void slProposeNetFirmwareUpdate(Firmware* updateFirmware, Firmware* oldFirmware);
    void slProposeOfflineFirmwareUpdate(Firmware *minimalFirmware, Firmware *actualFirmware);
};

#endif // UIDESKTOPCORE_H
