#include "devicedescription.h"

DeviceDescription::DeviceDescription(QString name, QString address, DeviceConnectionType connectionType)
    :m_name{name},
      m_address{address},
      m_connectionType{connectionType}
{

}

const QString &DeviceDescription::name() const
{
    return m_name;
}

DeviceConnectionType DeviceDescription::connectionType() const
{
    return m_connectionType;
}

const QString &DeviceDescription::address() const
{
    return m_address;
}
