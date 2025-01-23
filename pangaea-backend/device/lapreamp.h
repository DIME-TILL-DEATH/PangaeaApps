#ifndef LAPREAMP_H
#define LAPREAMP_H

#include "cpmodern.h"

class LAPreamp : public CPModern
{
    Q_OBJECT
    Q_PROPERTY(quint8 clnPresetMap READ clnPresetMap WRITE setClnPresetMap NOTIFY clnPresetMapChanged FINAL)
    Q_PROPERTY(quint8 drvPresetMap READ drvPresetMap WRITE setDrvPresetMap NOTIFY drvPresetMapChanged FINAL)
    Q_PROPERTY(quint8 la3Channel READ la3Channel WRITE setLa3Channel NOTIFY la3ChannelChanged FINAL)
public:
    LAPreamp(Core *parent);

    void setDeviceType(DeviceType newDeviceType) override;
    void readFullState() override;

    Q_INVOKABLE void changePreset(quint8 newBank, quint8 newPreset, bool ignoreChanges = false) override;
    quint8 clnPresetMap() const {return m_clnPresetMap;};
    void setClnPresetMap(quint8 newClnPresetMap);

    quint8 drvPresetMap() const {return m_drvPresetMap;};
    void setDrvPresetMap(quint8 newDrvPresetMap);

    quint8 la3Channel() const {return m_la3Channel;};
    void setLa3Channel(quint8 newLa3Channel);

signals:
    void clnPresetMapChanged();
    void drvPresetMapChanged();
    void la3ChannelChanged();

private:
    void getBankPresetLa3CommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    quint8 m_clnPresetMap{0};
    quint8 m_drvPresetMap{8};
    quint8 m_la3Channel{0};
};

#endif // LAPREAMP_H
