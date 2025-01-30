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

DeviceConnectionType DeviceDescription::connectionType() const
{
    return m_connectionType;
}

QString DeviceDescription::address()
{
    return m_address;
}

bool operator==(const DeviceDescription& lh, const DeviceDescription& rh)
{
    if( (lh.m_name == rh.m_name) &&
        (lh.m_address == rh.m_address) &&
        (lh.m_connectionType == rh.m_connectionType))
        return true;

    return false;
}
