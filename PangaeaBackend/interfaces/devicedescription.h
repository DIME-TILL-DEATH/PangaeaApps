#ifndef DEVICEDESCRIPTION_H
#define DEVICEDESCRIPTION_H

#include <QString>
#include <QObject>

#include <QtCore/QtGlobal>
#include <QtQml/qqmlregistration.h>

class DeviceConnectionTypeEnum: public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(DeviceConnectionType)
    QML_UNCREATABLE("")
public:
    explicit DeviceConnectionTypeEnum() {};

    enum Value
    {
        Unknown = 0,
        USB,
        BLE,
        Offline
    };
    Q_ENUM(Value)
};
typedef  DeviceConnectionTypeEnum::Value DeviceConnectionType;
Q_DECLARE_METATYPE(DeviceConnectionType)

class DeviceDescription
{
    Q_GADGET
    QML_ELEMENT

    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString address READ address)
    Q_PROPERTY(DeviceConnectionType connectionType READ connectionType)
public:

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



#endif // DEVICEDESCRIPTION_H
