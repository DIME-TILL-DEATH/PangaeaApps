#ifndef DEVICECONTROLS_H
#define DEVICECONTROLS_H

#include <QObject>
#include <QMap>
#include <QDebug>

#include "deviceparameter.h"

class DeviceControls : public QObject
{
    Q_OBJECT
public:
    DeviceControls();

    bool containsParameter(QString paramName);

    QString getParameterSendString(QString parameterName, quint8 value);
    QString getParameterSendString(DeviceParameter::Type parameterType, quint8 value);

    quint8 getParameterValue(QString parameterName);

    // void setParameterValue(QString parameterName, quint8 value);
    void setParameterValue(quint8 numberInArray, quint8 value);

    void setParametersFromRaw(QByteArray ba);

    void setAllUIValues();

signals:
   // void sgSetInterfaceValue(QString nameParam, qint32 inValue);
    void sgSetUiDeviceParameter(DeviceParameter::Type deviceParameterType, quint32 value);

private:
    QList<DeviceParameter> m_parametersList; // TODO QMap удобнее

    DeviceParameter* findParameter(QString parameterName);
    DeviceParameter* findParameter(quint8 numberInArray);
};

#endif // DEVICECONTROLS_H
