#include "mockla3.h"

MockLa3::MockLa3(QMutex *mutex, QByteArray *uartBuffer, QObject *parent)
    : MockCP16Modern{mutex, uartBuffer, parent}
{
    m_mockDeviceType = MockDeviceType::Mock_LA3;



    using namespace std::placeholders;
    m_parser.addCommandHandler("amtdev", std::bind(&MockLa3::amtDevLa3CommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("la3map", std::bind(&MockLa3::amtDevLa3CommHandler, this, _1, _2, _3));
}


//===========================================================================================
//  comm handlers
//===========================================================================================

void MockLa3::amtDevLa3CommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    emit answerReady(QString("amtdev\r17\nEND\n").toUtf8());
}

void MockLa3::la3MapCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    if(arguments == "get")
    {
        // QByteArray numbers = QString().setNum(m_systemParameters.)
        return;
    }
    if(arguments == "set")
    {
        saveSysParameters();
        return;
    }
    if(arguments == "use")
    {
        emit answerReady("la3map use\r" + data + "\n");
        return;
    }
    emit answerReady("la3map\rINCORRECT_ARGUMENT\n");
}
