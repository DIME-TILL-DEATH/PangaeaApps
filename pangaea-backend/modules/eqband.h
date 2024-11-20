#ifndef EQBAND_H
#define EQBAND_H

#include <QObject>
#include <QPointF>

#include "controlvalue.h"
#include "controlqlegacy.h"

#include "filtertypeenum.h"

class EqBand : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged FINAL)
    Q_PROPERTY(FilterType filterType READ filterType WRITE setFilterType NOTIFY filterTypeChanged FINAL)
    Q_PROPERTY(ControlValue* Fc READ getFc CONSTANT)
    Q_PROPERTY(ControlValue* Q READ getQ CONSTANT)
    Q_PROPERTY(ControlValue* gain READ getGain CONSTANT)

    Q_PROPERTY(QList<QPointF> bandPoints READ bandPoints NOTIFY bandPointsChanged)
public:    

    explicit EqBand(AbstractModule* ownerModule, FilterType bandType,
                    double fStart, double fStop, int bandNum,
                    qint32 fControlStart = -100, qint32 fControlStop = 100);

    ~EqBand();

    static constexpr double Fs = 48000;

    ControlValue* getFc() const {return m_Fc;};
    ControlValue* getGain() const {return m_gain;};
    ControlValue* getQ() const {return m_Q;};

    void setRawBandParams(FilterType bandType, quint16 gain, qint16 Fc, qint16 Q, bool enabled=true);

    double fStart() {return m_fStart;};
    double fStop() {return m_fStop;};

    double getFilterResponse(double f);
    void calcBandResponse(quint16 pointsNum);

    QList<QPointF> bandPoints() {return m_bandPoints;};

    bool enabled() const {return m_enabled;};
    void setEnabled(bool newEnabled);

    void setFilterType(FilterType newFilterType);
    FilterType filterType() const {return m_filterType;};

signals:
    void filterTypeChanged();
    void bandParametersChanged();


    void bandPointsChanged();
    void enabledChanged();

    void userModifiedBandParameters();

private:
    FilterType m_filterType;

    ControlValue* m_Fc;
    ControlQLegacy* m_Q;
    ControlValue* m_gain;

    double m_fStart;
    double m_fStop;

    int m_bandNum{0};

    struct FilterCoefs
    {
        double a0;
        double a1;
        double a2;
        double b0;
        double b1;
        double b2;
    }m_coefs;

    void calcFilterCoefs();
    QList<QPointF> m_bandPoints;
    bool m_enabled{true};
};

#endif // EQBAND_H
