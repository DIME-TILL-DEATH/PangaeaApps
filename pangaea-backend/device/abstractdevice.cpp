#include "abstractdevice.h"

AbstractDevice::AbstractDevice(QObject *parent)
    : QObject{parent},
    m_deviceClass{DeviceClass::ABSTRACT}
{

}

void AbstractDevice::slParseAnswers(QByteArray &baAnswer)
{

}
