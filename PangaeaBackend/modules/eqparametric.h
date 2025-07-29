#ifndef EQPARAMETRIC_H
#define EQPARAMETRIC_H

#include <QObject>
#include <QPointF>
#include <QtQml/qqmlregistration.h>

#include "abstractmodule.h"
// #include "deviceparameter.h"

#include "eqband.h"
#include "presetlegacy.h"

class EqParametric : public AbstractModule
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(QList<QPointF> points READ points NOTIFY pointsChanged)
    Q_PROPERTY(QObjectList EqBands READ EqBands NOTIFY eqBandsChanged)

     // binding in QML take time. Bind max and min only one time
    Q_PROPERTY(double minFreq READ minFreq CONSTANT)
    Q_PROPERTY(double maxFreq READ maxFreq CONSTANT)

    Q_PROPERTY(EqBand* lpf READ lpf NOTIFY dataChanged FINAL)
    Q_PROPERTY(EqBand* hpf READ hpf NOTIFY dataChanged FINAL)

    Q_PROPERTY(ControlValue* position READ position NOTIFY dataChanged FINAL) // Для FX, в Legacy внешнее значение(переделать?)
public:
    enum EqMode{
        Legacy = 0,
        Modern,
        Fx
    };

    explicit EqParametric(AbstractDevice *owner, EqMode eqMode = EqMode::Legacy, quint8 eqNumber = 0);

    void setValues(const preset_data_cplegacy_t& lpData) override;
    void setValues(const preset_data_cpmodern_t& presetData) override;
    void setValues(const modules_data_fx_t &eqData) override;

    void setValuesPointers(modules_data_fx_t* eqData);

    double getEqResponse(double f);

    double minFreq() {return m_minFreq;};
    double maxFreq() {return m_maxFreq;};

    QObjectList EqBands() {return m_EqBands;};
    QList<QPointF> points() {return m_points;};
    EqBand* lpf() {return m_lpf;};
    EqBand* hpf() {return m_hpf;};

    Q_INVOKABLE void reset();

    void setPrePost(bool isPre) {m_position->setControlValue(isPre);};
    ControlValue* position() {return m_position;};

    EqMode eqMode() const {return m_eqMode;};
    quint8 eqNumber() const {return m_eqNumber;};
signals:
    void pointsChanged();
    void eqBandsChanged();

private slots:
    void hpfEnabledChanged();
    void lpfEnabledChanged();
    void bandTypeChanged(int bandNum);

private:
    static constexpr quint16 pointsNum = 250;

    double m_minFreq;
    double m_maxFreq;

    EqMode m_eqMode;
    quint8 m_parametricBandCount;
    quint8 m_eqNumber{0};

    QList<QPointF> m_points;
    QObjectList m_EqBands;
    // QList<EqBand*> m_EqBands;
    EqBand* m_lpf;
    EqBand* m_hpf;

    ControlValue* m_position;

    void calcEqResponse();
};
#endif // EQPARAMETRIC_H
