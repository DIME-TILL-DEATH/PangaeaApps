#ifndef DEVICEPROPERTIES_H
#define DEVICEPROPERTIES_H

#include <QObject>


#include "core.h"

#include "deviceparameter.h"

class DeviceProperties : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isLa3Mode READ isLa3Mode NOTIFY deviceTypeChanged FINAL)
    Q_PROPERTY(bool isPaFirmware READ isPaFirmware NOTIFY deviceTypeChanged FINAL)
    Q_PROPERTY(QVariantList banksList READ banksList NOTIFY deviceTypeChanged FINAL)
    Q_PROPERTY(QVariantList presetsList READ presetsList NOTIFY deviceTypeChanged FINAL)
    Q_PROPERTY(QString firmwareName READ firmwareName NOTIFY deviceTypeChanged FINAL)

    Q_PROPERTY(bool presetModified READ presetModified NOTIFY presetModifiedChanged FINAL)
    Q_PROPERTY(DeviceType deviceType READ deviceType NOTIFY deviceTypeChanged FINAL)

    Q_PROPERTY(quint8 bank READ bank WRITE setBank NOTIFY bankPresetChanged FINAL)
    Q_PROPERTY(quint8 preset READ preset WRITE setPreset NOTIFY bankPresetChanged FINAL)

    Q_PROPERTY(quint8 outputMode READ outputMode NOTIFY outputModeChanged FINAL)

    Q_PROPERTY(quint8 la3CleanPreset READ la3CleanPreset NOTIFY la3CleanPresetChanged FINAL)
    Q_PROPERTY(quint8 la3DrivePreset READ la3DrivePreset NOTIFY la3DrivePresetChanged FINAL)

    Q_PROPERTY(quint8 la3Channel READ la3Channel NOTIFY la3ChannelChanged FINAL)
public:
    explicit DeviceProperties(Core* core, QObject *parent = nullptr);

    Q_INVOKABLE void changePreset(quint8 newBank, quint8 newPreset, bool ignoreChanges = false);
    Q_INVOKABLE void saveChanges();
    Q_INVOKABLE void setLa3Mappings(quint8 cleanPreset, quint8 drivePreset);

    bool presetModified() const {return m_presetModified;};

    DeviceType deviceType() const {return currentDevice.deviceType();};
    QString firmwareName() const {return currentDevice.firmwareName();};
    QVariantList banksList() const {return m_banksList;};
    QVariantList presetsList() const {return m_presetsList;};

    quint8 bank() const {return m_bank;};
    void setBank(quint8 newBank);

    quint8 preset() const {return m_preset;};
    void setPreset(quint8 newPreset);

    bool isLa3Mode() const;

    quint8 la3CleanPreset() const;
    quint8 la3DrivePreset() const;

    quint8 outputMode() const {return m_outputMode;};

    quint8 la3Channel() const {return m_la3Channel;};

    bool isPaFirmware() const {return currentDevice.havePaSection();};

signals:

    void presetNotSaved(quint8 newBank, quint8 newPreset);
    void sgSetUiDeviceParameter(DeviceParameter::Type deviceParameterType, qint32 value);
    void sendAppAction(Core::AppAction appParameterType, QVariantList parameters);

    void presetModifiedChanged();
    void deviceTypeChanged();

    void bankPresetChanged();

    void la3CleanPresetChanged();
    void la3DrivePresetChanged();
    void la3ChannelChanged();

    void outputModeChanged();

public slots:
    void slSetUiDeviceParameter(DeviceParameter::Type deviceParameterType, qint32 value);
    void slSetAppParameter(Core::AppParameter appParameterType, QVariant content);

private:
    Device currentDevice;

    bool m_presetModified{false};
    // QString m_firmwareName;

    QVariantList m_banksList;
    QVariantList m_presetsList;
    quint8 m_bank;
    quint8 m_preset;
    quint8 m_la3CleanPreset;
    quint8 m_la3DrivePreset;
    quint8 m_outputMode;
    quint8 m_la3Channel{0};
};

#endif // DEVICEPROPERTIES_H
