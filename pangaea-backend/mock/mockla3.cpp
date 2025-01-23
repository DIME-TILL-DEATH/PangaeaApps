#include "mockla3.h"

#include <QDebug>
#include <QDir>

MockLa3::MockLa3(QMutex *mutex, QByteArray *uartBuffer, QObject *parent)
    : MockCP16Modern{mutex, uartBuffer, parent}
{
    m_mockDeviceType = MockDeviceType::Mock_LA3;
    m_mockName = "virtual_La3";
    initFolders();

    quint8 startBank = (m_systemParameters.la3_cln_preset&0xF0)>>4;
    quint8 startPreset = m_systemParameters.la3_cln_preset&0x0F;
    changePreset(startBank, 0);

    using namespace std::placeholders;
    m_parser.addCommandHandler("amtdev", std::bind(&MockLa3::amtDevLa3CommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("la3map", std::bind(&MockLa3::la3MapCommHandler, this, _1, _2, _3));
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
        QByteArray baCln = QByteArray::number(m_systemParameters.la3_cln_preset, 16) ;
        QByteArray baDrv = QByteArray::number(m_systemParameters.la3_drv_preset, 16);

        if(baCln.size() < 2) baCln.prepend("0");
        if(baDrv.size() < 2) baCln.prepend("0");

        emit answerReady("la3map get\r" + baCln + baDrv + "\n");
        return;
    }
    if(arguments == "set")
    {
        m_systemParameters.la3_cln_preset = data.left(2).toShort(nullptr, 16);
        m_systemParameters.la3_drv_preset = data.right(2).toShort(nullptr, 16);
        saveSysParameters();
        emit answerReady("la3map set\r" + data + "\n");
        return;
    }
    if(arguments == "use")
    {
        emit answerReady("la3map use " + data + "\r\n");
        return;
    }
    emit answerReady("la3map\rINCORRECT_ARGUMENT\n");
}
