/****************************************************************************
**
** This file is part of the Bluetooth LE UART example written by Nico Lingg
**
**
** Author: Nico Lingg
** VERSION: 0.1
** Date: 09/12/2016 (DD/MM/YYYY)
**
**
** Permission is hereby granted, free of charge, to any person obtaining a copy of
** this software and associated documentation files (the "Software"), to deal in
** the Software without restriction, including without limitation the rights to
** use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
** the Software, and to permit persons to whom the Software is furnished to do so,
** subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
****************************************************************************/
#ifndef BLUETOOTHLEUART_H
#define BLUETOOTHLEUART_H

#include <QBluetoothLocalDevice>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>
#include <QLowEnergyService>

#include <QHash>
#include <QSettings>
#include <qregularexpression.h>


#define UARTSERVICEUUID "6e400001-b5a3-f393-e0a9-e50e24dcca9e"
#define RXUUID "6e400002-b5a3-f393-e0a9-e50e24dcca9e"
#define TXUUID "6e400003-b5a3-f393-e0a9-e50e24dcca9e"

class BluetoothleUART : public QObject
{
    Q_OBJECT

public:
    enum bluetoothleState {
        Idle = 0,
        Scanning,
        UpdateDeviceList,
        UpdateModuleName,
        Connecting,
        Connected,
        ServiceFound,
        AcquireData
    };
    Q_ENUM(bluetoothleState)

    BluetoothleUART();

    void writeData(QByteArray s);

    void setState(BluetoothleUART::bluetoothleState newState);
    void prevState();
    BluetoothleUART::bluetoothleState getState() const;


    void doDisconnect(void);
    void doConnect(qint8 numDev, QString address);

    void setAutoconnectState(bool newAutoconnect);
    bool AutoconnectState() const;

    void setModuleName(QString name);
    const QString &moduleName() const;

    void getDeviceList(QList<QString> &qlDevices);
    const QBluetoothDeviceInfo* getDeviceByMAC(const QString& macAddress);

public slots:
    void startScan();
    void updateBLEDevicesList();

private slots:
    /* Slots for QBluetothDeviceDiscoveryAgent */
    void addDevice(const QBluetoothDeviceInfo&);
    void deviceScanError(QBluetoothDeviceDiscoveryAgent::Error);
    void scanTimeout();

    /* Slots for QLowEnergyController */
    void serviceDiscovered(const QBluetoothUuid &);
    void serviceScanDone();
    void controllerError(QLowEnergyController::Error);
    void deviceConnected();
    void deviceDisconnected();

    /* Slotes for QLowEnergyService */
    void serviceStateChanged(QLowEnergyService::ServiceState s);
    void updateData(const QLowEnergyCharacteristic &c, const QByteArray &value);
    void confirmedDescriptorWrite(const QLowEnergyDescriptor &d, const QByteArray &value);

    /* Slots for user */
    void slStartConnect(QString address);
    void checkDevicesAvaliabliy();

signals:
    /* Signals for user */
    void newData(QByteArray ba);
    void changedState(BluetoothleUART::bluetoothleState newState);
    void sgConnect(quint8 numDevice);
    void sgErrorDisconnected();
    void sgLocalBluetoothNotReady(QString reason);

private:
    QBluetoothDeviceDiscoveryAgent *m_deviceDiscoveryAgent{nullptr};
    QList<QBluetoothDeviceInfo> m_avaliableDevices;
    QList<QString> m_qlFoundDevices;
    QVector<quint16> m_qvMeasurements;
    QLowEnergyController *m_control;
    QLowEnergyService *m_service;
    QLowEnergyDescriptor m_notificationDescTx;
    QLowEnergyService *m_UARTService;
    bool m_bFoundUARTService;

    QSettings* appSettings;

    QString m_autoconnectDevicetMAC;
    QString m_moduleName;

    QHash<QString, QString> m_moduleUniqueNames;

    BluetoothleUART::bluetoothleState m_state;
    BluetoothleUART::bluetoothleState m_prevState;

};

#endif // BLUETOOTHLEUART_H
