#ifndef UIINTERFACEMANAGER_H
#define UIINTERFACEMANAGER_H

#include <QObject>

#include "deviceslistmodel.h"
#include "devicedescription.h"

class UiInterfaceManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isBleAvaliable READ isBleAvaliable NOTIFY isBleAvaliableChanged)
    Q_PROPERTY(QString moduleName READ moduleName WRITE setModuleName NOTIFY sgModuleNameChanged)
    Q_PROPERTY(qint16 bleRssi READ bleRssi WRITE setRssi NOTIFY bleRssiChanged FINAL)
public:
    explicit UiInterfaceManager(QObject *parent = nullptr);
    ~UiInterfaceManager();

    Q_INVOKABLE void startScanning();
    Q_INVOKABLE void connectToDevice(DeviceDescription device);
    Q_INVOKABLE void connectToVirtualDevice();
    Q_INVOKABLE void disconnectFromDevice();
    Q_INVOKABLE void rssiMeasuring(bool enabled) {emit sgRssiMeasuring(enabled);}

    void updateDevicesList(DeviceConnectionType connectionType, QList<DeviceDescription> list);

    DevicesListModel devicesListModel() const;

    bool isBleAvaliable();

    const QString &moduleName() const;
    void setModuleName(const QString &newModuleName);

    qint16 bleRssi() const;

    void setRssi(qint16 newBleRssi);

public slots:
    void slInterfaceUnavaliable(DeviceConnectionType senderType, QString reason);

signals:
    void sgStartScanning();

    void sgInterfaceUnavaliable(DeviceConnectionType senderType, QString reason);
    void sgConnectToDevice(DeviceDescription device);
    void sgDisconnectFromDevice();
    void sgDeviceUnavaliable(DeviceConnectionType senderType, QString reason);
    void sgExchangeError();

    void sgConnectionStarted();
    void sgInterfaceConnected(DeviceDescription device);
    void sgInterfaceDisconnected(DeviceDescription device);
    void sgInterfaceError(QString errorDescription);

    void isBleAvaliableChanged();

    void sgModuleNameChanged(QString name);

    void bleRssiChanged();
    void sgRssiMeasuring(bool isEnabled);

private:
    bool m_isBleAvaliable{true};

    DevicesListModel m_devicesListModel;
    QString m_moduleName;
    qint16 m_bleRssi;
};

#endif // UIINTERFACEMANAGER_H
