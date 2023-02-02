#ifndef DEVICEPARAMETER_H
#define DEVICEPARAMETER_H

#include <QString>


class DeviceParameter
{
public:
    DeviceParameter(QString paramName, QString commHandler, quint8 position, bool isSigned=false);

   // QString sendString();
    QString sendString(quint8 value);
    const QString &paramName() const;
    quint8 value() const;
    void setValue(quint8 newValue);

    quint16 positionInArray() const;

    bool isSigned() const;

private:
    QString m_paramName;
    QString m_commHandler;


    quint8 m_value;

    quint16 m_positionInArray;

    bool m_isSigned;
};

#endif // DEVPARAM_H
