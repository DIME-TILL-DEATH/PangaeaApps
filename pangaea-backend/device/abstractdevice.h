#ifndef ABSTRACTDEVICE_H
#define ABSTRACTDEVICE_H

#include <QObject>

#include "abstractmodule.h"

enum class DeviceClass
{
    ABSTRACT = 0,
    CP_LEGACY
};

class AbstractDevice : public QObject
{
    Q_OBJECT
public:
    explicit AbstractDevice(QObject *parent = nullptr);

    DeviceClass deviceClass() {return m_deviceClass;}

public slots:
    virtual void slParseAnswers(QByteArray& baAnswer);

signals:
    void sgWriteToInterface(QByteArray data, bool logCommand = true);
    void sgRecieveDeviceParameter(DeviceParameter::Type deviceParameterType, qint32 value);
    void sgModuleParametersUpdated(AbstractModule* module, quint16 moduleUid);

protected:
     DeviceClass m_deviceClass;
};

#endif // ABSTRACTDEVICE_H
