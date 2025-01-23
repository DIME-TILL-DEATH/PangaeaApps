#ifndef LAPREAMP_H
#define LAPREAMP_H

#include "cpmodern.h"

class LAPreamp : public CPModern
{
    Q_OBJECT
    Q_PROPERTY(quint8 clnPresetMap READ clnPresetMap WRITE setClnPresetMap NOTIFY presetMapChanged FINAL)
    Q_PROPERTY(quint8 drvPresetMap READ drvPresetMap WRITE setDrvPresetMap NOTIFY presetMapChanged FINAL)
    Q_PROPERTY(quint8 la3Channel READ la3Channel WRITE setLa3Channel NOTIFY la3ChannelChanged FINAL)
public:
    LAPreamp(Core *parent);

    void setDeviceType(DeviceType newDeviceType) override;
    void readFullState() override;

    Q_INVOKABLE void changePreset(quint8 newBank, quint8 newPreset, bool ignoreChanges = false) override;
    Q_INVOKABLE void setLa3Mappings(quint8 newClnPresetMap, quint8 newDrvPresetmap);

    quint8 clnPresetMap() const {return m_clnPresetMap;};
    void setClnPresetMap(quint8 newClnPresetMap);

    quint8 drvPresetMap() const {return m_drvPresetMap;};
    void setDrvPresetMap(quint8 newDrvPresetMap);

    quint8 la3Channel() const {return m_la3Channel;};
    void setLa3Channel(quint8 newLa3Channel);

signals:
    void presetMapChanged();
    void la3ChannelChanged();

private:
    quint8 m_clnPresetMap;
    quint8 m_drvPresetMap;
    quint8 m_la3Channel{0};

    void getBankPresetLa3CommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void la3MapCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
};

#endif // LAPREAMP_H
