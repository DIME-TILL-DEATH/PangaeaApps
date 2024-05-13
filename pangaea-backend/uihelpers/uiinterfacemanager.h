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
public:
    explicit UiInterfaceManager(QObject *parent = nullptr);
    ~UiInterfaceManager();

    Q_INVOKABLE void startScanning();
    Q_INVOKABLE void connectToDevice(DeviceDescription device);
    Q_INVOKABLE void disconnectFromDevice();

    void updateDevicesList(DeviceConnectionType connectionType, QList<DeviceDescription> list);

    DevicesListModel devicesListModel() const;

    bool isBleAvaliable();

    const QString &moduleName() const;
    void setModuleName(const QString &newModuleName);

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

private:
    bool m_isBleAvaliable{true};

    DevicesListModel m_devicesListModel;
    QString m_moduleName;
};

#endif // UIINTERFACEMANAGER_H
