#ifndef USBINTERFACE_H
#define USBINTERFACE_H

#include <QTimer>
#include <QSerialPort>
#include <QSerialPortInfo>

#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

#include "abstractinterface.h"

class UsbInterface : public AbstractInterface
{
    Q_OBJECT
public:
    explicit UsbInterface(QObject *parent = nullptr);

    void startScan();
    void stopScan();
    QList<DeviceDescription> discoveredDevicesList();

    bool connect(DeviceDescription device);
    void disconnectFromDevice();

    void write(QByteArray data);

private slots:
    void slReadyRead();
    void slError(QSerialPort::SerialPortError error);
    void slDestroyed(QObject* obj);

    void slPortTimer();

private:
    QSerialPort* m_port{nullptr};
    QTimer* m_timer{nullptr};

    QList<DeviceDescription> m_discoveredDevices;

    void discoverDevices();

    // settings
    void loadSettings();
    bool isManualConnectAllowed{false};
    QString manualConnectionPortName{"COM3"};

    void checkConnection();
};

#endif // USBINTERFACE_H
