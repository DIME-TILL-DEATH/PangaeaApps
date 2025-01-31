#ifndef USBINTERFACE_H
#define USBINTERFACE_H

#include <QTimer>
#include <QSerialPort>
#include <QSerialPortInfo>

#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

#include "abstractinterface.h"

// #if defined(PANGAEA_LIBRARY)
// #  define PANGAEA_LIBRARY_EXPORT Q_DECL_EXPORT
// #else
// #  define PANGAEA_LIBRARY_EXPORT Q_DECL_IMPORT
// #endif

// class PANGAEA_LIBRARY_EXPORT UsbInterface : public AbstractInterface
// {

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

    void write(const QByteArray &data);

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
