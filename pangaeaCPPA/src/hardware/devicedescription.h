#ifndef DEVICEDESCRIPTION_H
#define DEVICEDESCRIPTION_H

#include <QString>

enum class DeviceConnectionType
{
    Unknown = 0,
    USBAuto,
    USBManual,
    BLE
};

class DeviceDescription
{
public:
    DeviceDescription(QString name, QString address, DeviceConnectionType connectionType);

    const QString &name() const;
    DeviceConnectionType connectionType() const;
    const QString &address() const;

private:
    QString m_name;
    QString m_address;
    DeviceConnectionType m_connectionType{DeviceConnectionType::Unknown};
};

#endif // DEVICEDESCRIPTION_H
