#ifndef EQPARAMETRIC_H
#define EQPARAMETRIC_H

#include <QObject>
#include <QPointF>

#include "abstractmodule.h"
// #include "deviceparameter.h"

#include "eqband.h"
#include "preset.h"

class EqParametric : public AbstractModule
{
    Q_OBJECT
    Q_PROPERTY(QList<QPointF> points READ points NOTIFY pointsChanged)
    Q_PROPERTY(QObjectList EqBands READ EqBands NOTIFY eqBandsChanged)

    Q_PROPERTY(EqBand* lpf READ lpf NOTIFY dataChanged FINAL)
    Q_PROPERTY(EqBand* hpf READ hpf NOTIFY dataChanged FINAL)
public:
    enum EqMode{
        Legacy = 0,
        Modern
    };

    explicit EqParametric(AbstractDevice *owner, EqMode eqMode = EqMode::Legacy);

    void setBandsData(eq_t eqData);

    double getEqResponse(double f);

    QObjectList EqBands() {return m_EqBands;};
    QList<QPointF> points() {return m_points;};
    EqBand* lpf() {return m_lpf;};
    EqBand* hpf() {return m_hpf;};

    Q_INVOKABLE void reset();


signals:
    void pointsChanged();
    void eqBandsChanged();

private slots:
    void hpfEnabledChanged();
    void lpfEnabledChanged();

private:
    static constexpr quint16 pointsNum = 250;

    EqMode m_eqMode;
    quint8 m_parametricBandCount;

    QList<QPointF> m_points;
    QObjectList m_EqBands;
    EqBand* m_lpf;
    EqBand* m_hpf;


    void calcEqResponse();
};
#endif // EQPARAMETRIC_H
