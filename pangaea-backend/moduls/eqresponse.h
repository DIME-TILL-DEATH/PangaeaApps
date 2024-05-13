#ifndef EQRESPONSE_H
#define EQRESPONSE_H

#include <QObject>
#include <QPointF>

#include "core.h"
#include "deviceparameter.h"

class EqResponse : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QPointF> points READ points NOTIFY pointsChanged)
    Q_PROPERTY(QObjectList EqBands READ EqBands NOTIFY eqBandsChanged)
public:
    explicit EqResponse(QObject *parent = nullptr);

    double getEqResponse(double f);
    QList<QPointF> points();

    QObjectList EqBands();

signals:
    void pointsChanged();
    void eqBandsChanged();

    void setDeviceParameter(DeviceParameter::Type deviceParameterType, quint8 value);

    void sgSetAppParameter(Core::AppParameter appParameterType, QVariant content);
public slots:
    void slFilterParamChanged(DeviceParameter::Type deviceParameterType, qint32 value);

private:
    static constexpr quint16 pointsNum = 250;

    QList<QPointF> m_points;
    QObjectList m_EqBands;

    void calcEqResponse();
};
#endif // EQRESPONSE_H
