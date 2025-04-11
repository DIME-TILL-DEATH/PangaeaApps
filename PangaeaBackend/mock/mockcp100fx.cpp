#include "mockcp100fx.h"

MockCP100fx::MockCP100fx(QMutex *mutex, QByteArray *uartBuffer, QObject *parent)
    : AbstractMockDevice{mutex, uartBuffer, parent}
{
    m_mockDeviceType = MockDeviceType::Mock_CP100FX;

    initFolders();
    using namespace std::placeholders;
    m_parser.addCommandHandler("amtdev", std::bind(&MockCP100fx::amtDevCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("amtver", std::bind(&MockCP100fx::amtVerCommHandler, this, _1, _2, _3));

    //--------------------------params handler----------------------

}

void MockCP100fx::writeToDevice(const QByteArray &data)
{
    m_parser.parseNewData(data);
}

void MockCP100fx::newDataRecieved()
{
    m_parser.parseNewData(*m_uartBuffer);

    QMutexLocker locker(m_mutex);
    m_uartBuffer->clear();
}

void MockCP100fx::initFolders()
{

}
//===========================================================================================
//  comm handlers
//===========================================================================================

void MockCP100fx::amtDevCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    emit answerReady(QString("amtdev\r5\nEND\n").toUtf8());

}

void MockCP100fx::amtVerCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    emit answerReady(QString("amtver\r1.10.00\nEND\n").toUtf8());

}
