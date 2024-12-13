#include "abstractmockdevice.h"

#include <QDebug>
#include <QMutexLocker>

AbstractMockDevice::AbstractMockDevice(QMutex *mutex, QByteArray *uartBuffer, QObject *parent)
    : m_mutex{mutex},
    m_uartBuffer{uartBuffer},
    QObject{parent}
{
    m_parser.disableFullEndMode();
}

void AbstractMockDevice::writeToDevice(const QByteArray &data)
{
    m_parser.parseNewData(data);
}

void AbstractMockDevice::newDataRecieved()
{
    QMutexLocker locker(m_mutex);
    m_uartBuffer->clear();
}
