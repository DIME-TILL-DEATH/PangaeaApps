#ifndef DEVICEDESCRIPTION_H
#define DEVICEDESCRIPTION_H

#include <QString>
#include <QObject>

#include <QtCore/QtGlobal>

class DeviceDescription
{
    Q_GADGET
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString address READ address)
    Q_PROPERTY(DeviceConnectionType connectionType READ connectionType)
public:
    enum class DeviceConnectionType
    {
        Unknown = 0,
        USBAuto,
        USBManual,
        BLE
    };
    Q_ENUM(DeviceConnectionType)

    DeviceDescription() = default;
    DeviceDescription(QString name, QString address, DeviceConnectionType connectionType);

    QString name();
    DeviceConnectionType connectionType() const;
    QString address();

    friend bool operator==(const DeviceDescription& lh, const DeviceDescription& rh);
private:
    QString m_name;
    QString m_address;
    DeviceConnectionType m_connectionType{DeviceConnectionType::Unknown};
};
Q_DECLARE_METATYPE(DeviceDescription)


typedef  DeviceDescription::DeviceConnectionType DeviceConnectionType;

#endif // DEVICEDESCRIPTION_H
