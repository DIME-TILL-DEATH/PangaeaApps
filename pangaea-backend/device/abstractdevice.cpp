#include "abstractdevice.h"

#include "core.h"

AbstractDevice::AbstractDevice(Core *owner)
   : m_deviceClass{DeviceClass::ABSTRACT}
{
    connect(this, &AbstractDevice::sgWriteToInterface, owner, &Core::sgWriteToInterface);
}

AbstractDevice::~AbstractDevice()
{
    disconnect(this);
}

void AbstractDevice::initDevice()
{

}

void AbstractDevice::parseAnswers(QByteArray &baAnswer)
{
    m_parser.parseNewData(baAnswer);
}

void AbstractDevice::userModifiedModules()
{
    m_deviceParamsModified = true;
    emit deviceParamsModifiedChanged();
}

bool AbstractDevice::deviceParamsModified() const
{
    return m_deviceParamsModified;
}
