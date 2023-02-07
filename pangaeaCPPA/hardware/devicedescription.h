#ifndef DEVICEDESCRIPTION_H
#define DEVICEDESCRIPTION_H

#include <QString>
#include <QObject>

class DeviceDescription
{
    Q_GADGET
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString address READ address)
public:
    enum class DeviceConnectionType
    {
        Unknown = 0,
        USBAuto,
        USBManual,
        BLE
    };

    DeviceDescription() = default;
    DeviceDescription(QString name, QString address, DeviceConnectionType connectionType);

    QString name();
    DeviceConnectionType connectionType() const;
    QString address();

private:
    QString m_name;
    QString m_address;
    DeviceConnectionType m_connectionType{DeviceConnectionType::Unknown};
};
Q_DECLARE_METATYPE(DeviceDescription)

typedef  DeviceDescription::DeviceConnectionType DeviceConnectionType;

#endif // DEVICEDESCRIPTION_H
