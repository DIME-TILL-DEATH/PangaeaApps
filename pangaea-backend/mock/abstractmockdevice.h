#ifndef ABSTRACTMOCKDEVICE_H
#define ABSTRACTMOCKDEVICE_H

#include <QObject>
#include <QMutex>

#include "parser.h"

enum MockDeviceType
{
    Mock_Abstract = 0,
    Mock_CPLegacy,
    Mock_CPModern
};

#define FIRMWARE_STRING_SIZE 20
typedef struct system_parameters_t
{
    uint8_t empty[2];
    uint8_t output_mode;
    uint8_t reserved[8];
    uint8_t eol_symb = '\n';
    char firmware_version[FIRMWARE_STRING_SIZE];
#ifdef __LA3_MOD__
    uint8_t la3_cln_preset;
    uint8_t la3_drv_preset;
#endif
}system_parameters_t;

class AbstractMockDevice : public QObject
{
    Q_OBJECT
public:
    explicit AbstractMockDevice(QMutex* mutex, QByteArray* uartBuffer, QObject *parent = nullptr);

    MockDeviceType mockDeviceType() {return m_mockDeviceType;};
    QString basePath() {return m_basePath;};
signals:
    void answerReady(const QByteArray& answerData);

    // for testing
    void impulseUploaded();
public slots:
    virtual void writeToDevice(const QByteArray& data);
    virtual void newDataRecieved();

protected:
    Parser m_parser{"Mock device recieve"};

    QMutex* m_mutex;
    QByteArray* m_uartBuffer;

    QString m_basePath;

    quint8 m_outputMode{0};
    quint8 m_bank{0};
    quint8 m_preset{0};

    MockDeviceType m_mockDeviceType{MockDeviceType::Mock_Abstract};
};

#endif // ABSTRACTMOCKDEVICE_H
