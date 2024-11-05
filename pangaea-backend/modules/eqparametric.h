#ifndef EQPARAMETRIC_H
#define EQPARAMETRIC_H

#include <QObject>
#include <QPointF>

#include "abstractmodule.h"
// #include "deviceparameter.h"

#include "preset.h"

class EqParametric : public AbstractModule
{
    Q_OBJECT
    Q_PROPERTY(QList<QPointF> points READ points NOTIFY pointsChanged)
    Q_PROPERTY(QObjectList EqBands READ EqBands NOTIFY eqBandsChanged)

public:
    explicit EqParametric(AbstractDevice *owner);

    double getEqResponse(double f);
    QList<QPointF> points();

    QObjectList EqBands();

    void setBandsData(eq_t eqData);

signals:
    void pointsChanged();
    void eqBandsChanged();


// public slots:
    // void slFilterParamChanged(DeviceParameter::Type deviceParameterType, qint32 value);

private:
    static constexpr quint16 pointsNum = 250;

    QList<QPointF> m_points;
    QObjectList m_EqBands;

    void calcEqResponse();
};
#endif // EQPARAMETRIC_H
