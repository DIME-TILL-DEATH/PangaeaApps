#ifndef UIDESKTOPCORE_H
#define UIDESKTOPCORE_H

#include <QObject>
#include <QTimer>

#include "core.h"

#include "deviceparameter.h"
#include "firmware.h"

//TODO: class enum UIValueItem, UITextItem, UIErrorItem
//TODO: все uiItems без событий передавать через UiProperties(Q_PROPERTY) а не sgSetUiText и тп

class UiDesktopCore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool presetModified READ presetModified NOTIFY presetModifiedChanged FINAL)
    Q_PROPERTY(DeviceType deviceType READ deviceType NOTIFY deviceTypeChanged FINAL)
    Q_PROPERTY(QString firmwareName READ firmwareName NOTIFY firmwareNameChanged FINAL)
public:
    explicit UiDesktopCore(QObject *parent = nullptr);

    Q_INVOKABLE void setParameter(QString name, quint8 val);
    Q_INVOKABLE void setDeviceParameter(DeviceParameter::Type deviceParameterType, quint8 value)
    {
        emit sgSetDeviceParameter(deviceParameterType, value);
    };

    Q_INVOKABLE void restoreParameter(QString name);

    Q_INVOKABLE void setImpuls(QString fullFilePath);
    Q_INVOKABLE void escImpuls();
    Q_INVOKABLE void convertAndUploadImpulse(QString fullFilePath);

    Q_INVOKABLE void readAllParameters();

    Q_INVOKABLE void exportPreset(QString filePath);
    Q_INVOKABLE void importPreset(QString filePath);

    Q_INVOKABLE void openManualExternally(QString fileName);
    Q_INVOKABLE void runIrConvertor();

    Q_INVOKABLE void updateFirmware(QString filePath);

    Q_INVOKABLE void sw4Enable();

    bool presetModified() const {return m_presetModified;};
    DeviceType deviceType() const {return m_deviceType;};
    QString firmwareName() const {return m_firmwareName;};

signals:
    void sgSetUiDeviceParameter(DeviceParameter::Type deviceParameterType, qint32 value);
    void sgSetUIParameter(QString nameParam, qint32 inValue);
    void sgSetUIText(QString nameParam, QString value);

    void sgPresetChangeStage (quint8 inChangePreset);
    void sgSetProgress(float val, QString extText);
    void sgDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

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

    void sgDoOnlineFirmwareUpdate();

    void presetModifiedChanged();
    void deviceTypeChanged();
    void firmwareNameChanged();

public slots:
    void slProposeNetFirmwareUpdate(Firmware* updateFirmware, Firmware* oldFirmware);
    void slProposeOfflineFirmwareUpdate(Firmware *minimalFirmware, Firmware *actualFirmware);
    void slNewAppVersionAvaliable(QString appVersion);

    void slSetUiDeviceParameter(DeviceParameter::Type deviceParameterType, qint32 value);
    void slSetAppParameter(Core::AppParameter appParameterType, QVariant content);
private:
    bool m_presetModified;
    DeviceType m_deviceType;
    QString m_firmwareName;
};

#endif // UIDESKTOPCORE_H
