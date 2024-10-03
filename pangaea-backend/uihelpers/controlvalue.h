#ifndef CONTROLVALUE_H
#define CONTROLVALUE_H

#include <QObject>

#include "abstractmodule.h"
#include "deviceparameter.h"

class ControlValue : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool enabled READ enabled NOTIFY enabledChanged FINAL)

    Q_PROPERTY(double value READ displayValue WRITE setDisplayValue NOTIFY displayValueChanged FINAL)
    Q_PROPERTY(double minValue READ minDisplayValue CONSTANT)
    Q_PROPERTY(double maxValue READ maxDisplayValue CONSTANT)

    Q_PROPERTY(bool isModified READ isModified NOTIFY isModifiedChanged FINAL)

    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString units READ units CONSTANT)
public:
    explicit ControlValue(AbstractModule *parent, DeviceParameter::Type deviceParameterType,
                          QString name = "", QString units ="",
                          qint16 minControlValue = 0, qint16 maxControlValue = 31,
                          double minDisplayValue = 0, double maxDisplayValue = 31);

    void setDisplayValue(double newDisplayValue);
    double displayValue() const {return m_displayValue;};

    double minDisplayValue() const {return m_minDisplayValue;};
    double maxDisplayValue() const {return m_maxDisplayValue;};

    bool isModified() const {return m_isModified;};


    QString name() const {return m_name;};
    QString units() const {return m_units;};

    bool enabled() const;

signals:
    void displayValueChanged();
    void isModifiedChanged();

    void sgSetDeviceParameter(DeviceParameter::Type deviceParameterType, quint8 value);

    void enabledChanged();

public slots:
    void slSetAppParameter(Core::AppParameter appParameterType, QVariant content);
    void slSetControlValue(DeviceParameter::Type deviceParameterType, qint32 value);

private:
    DeviceParameter::Type m_deviceParameterType;

    qint16 m_minControlValue;
    qint16 m_maxControlValue;

    double m_displayValue;
    double m_minDisplayValue;
    double m_maxDisplayValue;

    bool m_isModified;
    QString m_name;
    QString m_units;
};

#endif // CONTROLVALUE_H
