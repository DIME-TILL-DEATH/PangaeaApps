#ifndef DEVICEPROPERTIES_H
#define DEVICEPROPERTIES_H

#include <QObject>


#include "core.h"

#include "deviceparameter.h"

class DeviceProperties : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isLa3Mode READ isLa3Mode NOTIFY isLa3ModeChanged FINAL)

    Q_PROPERTY(bool presetModified READ presetModified NOTIFY presetModifiedChanged FINAL)
    Q_PROPERTY(DeviceType deviceType READ deviceType NOTIFY deviceTypeChanged FINAL)
    Q_PROPERTY(QString firmwareName READ firmwareName NOTIFY firmwareNameChanged FINAL)

    Q_PROPERTY(QVariantList banksList READ banksList NOTIFY banksPresetsListChanged FINAL)
    Q_PROPERTY(QVariantList presetsList READ presetsList NOTIFY banksPresetsListChanged FINAL)

    Q_PROPERTY(quint8 bank READ bank WRITE setBank NOTIFY bankChanged FINAL)
    Q_PROPERTY(quint8 preset READ preset WRITE setPreset NOTIFY presetChanged FINAL)
public:
    explicit DeviceProperties(QObject *parent = nullptr);

    Q_INVOKABLE void changePreset(quint8 newBank, quint8 newPreset, bool ignoreChanges = false);
    Q_INVOKABLE void saveChanges();

    bool presetModified() const {return m_presetModified;};

    DeviceType deviceType() const {return m_deviceType;};
    QString firmwareName() const {return m_firmwareName;};
    QVariantList banksList() const {return m_banksList;};
    QVariantList presetsList() const {return m_presetsList;};

    quint8 bank() const {return m_bank;};
    void setBank(quint8 newBank);

    quint8 preset() const {return m_preset;};
    void setPreset(quint8 newPreset);

    bool isLa3Mode() const;

signals:

    void presetNotSaved(quint8 newBank, quint8 newPreset);
    void sgSetUiDeviceParameter(DeviceParameter::Type deviceParameterType, qint32 value);
    void sendAppAction(Core::AppAction appParameterType, QVariantList parameters);

    void isLa3ModeChanged();
    void presetModifiedChanged();
    void deviceTypeChanged();
    void firmwareNameChanged();

    void banksPresetsListChanged();
    void bankChanged();
    void presetChanged();

public slots:
    void slSetUiDeviceParameter(DeviceParameter::Type deviceParameterType, qint32 value);
    void slSetAppParameter(Core::AppParameter appParameterType, QVariant content);

private:
    bool m_presetModified;
    DeviceType m_deviceType;
    QString m_firmwareName;

    QVariantList m_banksList;
    QVariantList m_presetsList;
    quint8 m_bank;
    quint8 m_preset;
};

#endif // DEVICEPROPERTIES_H
