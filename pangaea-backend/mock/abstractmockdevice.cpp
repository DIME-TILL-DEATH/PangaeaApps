#include "abstractmockdevice.h"

#include <QDebug>

AbstractMockDevice::AbstractMockDevice(QObject *parent)
    : QObject{parent}
{
    m_parser.disableFullEndMode();
}

void AbstractMockDevice::writeToDevice(const QByteArray &data)
{
    m_parser.parseNewData(data);
}
