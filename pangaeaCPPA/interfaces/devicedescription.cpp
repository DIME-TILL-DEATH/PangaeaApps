#include "devicedescription.h"

DeviceDescription::DeviceDescription(QString name, QString address, DeviceConnectionType connectionType)
    :m_name{name},
      m_address{address},
      m_connectionType{connectionType}
{

}

QString DeviceDescription::name()
{
    return m_name;
}

DeviceDescription::DeviceConnectionType DeviceDescription::connectionType() const
{
    return m_connectionType;
}

QString DeviceDescription::address()
{
    return m_address;
}
