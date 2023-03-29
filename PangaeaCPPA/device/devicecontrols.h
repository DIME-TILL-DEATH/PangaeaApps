#ifndef DEVICECONTROLS_H
#define DEVICECONTROLS_H

#include <QObject>
#include <QMap>
#include <QDebug>
#include <memory>

#include "deviceparameter.h"

class DeviceControls : public QObject
{
    Q_OBJECT
public:
    DeviceControls();

    bool containsParameter(QString paramName);

    QString getParameterSendString(QString parameterName, quint8 value);

    quint8 getParameterValue(QString parameterName);

    void setParameterValue(QString parameterName, quint8 value);
    void setParameterValue(quint8 numberInArray, quint8 value);

    void setParametersFromRaw(QByteArray ba);

    void setAllUIValues();

signals:
    void sgSetInterfaceValue(QString nameParam, qint32 inValue);

private:
    QList<DeviceParameter> m_parametersList;

    DeviceParameter* findParameter(QString parameterName);
    DeviceParameter* findParameter(quint8 numberInArray);
};

#endif // DEVICECONTROLS_H
