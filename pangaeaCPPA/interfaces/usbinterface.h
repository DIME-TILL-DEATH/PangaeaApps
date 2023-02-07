#ifndef USBINTERFACE_H
#define USBINTERFACE_H

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

#include "abstractinterface.h"

class UsbInterface : public AbstractInterface
{
public:
    explicit UsbInterface(QObject *parent = nullptr);

    void discoverDevices();
    QList<DeviceDescription> discoveredDevicesList();

    bool connect(DeviceDescription device);
    bool isConnected();
    void checkConnection();

    void write(QByteArray data);
    void disconnect();
    void flushBuffers();

    QString connectionDescription();
private slots:
    void slReadyRead();
    void slError(QSerialPort::SerialPortError error);
    void slDestroyed(QObject* obj);

private:
    QSerialPort* m_port;
    QList<DeviceDescription> m_discoveredDevices;

    void loadSettings();
    // settings
    bool isManualConnectAllowed{false};
    QString manualConnectionPortName{"COM3"};   
};

#endif // USBINTERFACE_H
