#include "abstractmockdevice.h"

AbstractMockDevice::AbstractMockDevice(QObject *parent)
    : QObject{parent}
{}

void AbstractMockDevice::writeToDevice(const QByteArray &data)
{
    m_parser.parseNewData(data);
}
