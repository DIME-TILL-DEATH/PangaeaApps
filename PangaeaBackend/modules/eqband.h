#ifndef EQBAND_H
#define EQBAND_H

#include <QObject>
#include <QPointF>

#include "controlvalue.h"
#include "controlqlegacy.h"

#include "filtertypeenum.h"

class EqParametric;
class EqBand : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY bandParametersChanged FINAL)
    Q_PROPERTY(FilterType filterType READ filterType WRITE setFilterType NOTIFY bandParametersChanged FINAL)
    Q_PROPERTY(ControlValue* Fc READ getFc CONSTANT)
    Q_PROPERTY(ControlValue* Q READ getQ CONSTANT)
    Q_PROPERTY(ControlValue* gain READ getGain CONSTANT)

    Q_PROPERTY(QList<QPointF> bandPoints READ bandPoints NOTIFY bandPointsChanged)
public:    

    explicit EqBand(EqParametric* ownerModule, FilterType bandType,
                    double fStart, double fStop, int bandNum,
                    qint32 fControlStart = -100, qint32 fControlStop = 100);

    ~EqBand();

    static constexpr double Fs = 48000;

    ControlValue* getFc() const {return m_Fc;};
    ControlValue* getGain() const {return m_gain;};
    ControlValue* getQ() const {return m_Q;};

    typedef struct
    {
        double a0;
        double a1;
        double a2;
        double b0;
        double b1;
        double b2;
    }FilterCoefs;

    FilterCoefs filterCoefs() const {return m_coefs;};

    void setRawBandParams(FilterType bandType, qint16 gain, qint16 Fc, qint16 Q, bool enabled=true);

    double fStart() {return m_fStart;};
    double fStop() {return m_fStop;};

    double getFilterResponse(double f);
    static double getFilterResponse(double f, FilterCoefs coefs); // for threaded calculations

    void calcBandResponse(quint16 pointsNum);
    static QList<QPointF> calcBandResponse(quint16 pointsNum, FilterCoefs coefs);

    QList<QPointF> bandPoints() {return m_bandPoints;};
    void setBandPoints(const QList<QPointF>& newBandPoints);

    bool enabled() const {return m_enabled;};
    void setEnabled(bool newEnabled);

    void setFilterType(FilterType newFilterType);
    FilterType filterType() const {return m_filterType;};

signals:
    void filterTypeChanged(int bandNum);
    void bandParametersChanged();


    void bandPointsChanged();
    void enabledChanged(int bandNum);

    void userModifiedBandParameters();

private:
    FilterType m_filterType;

    ControlValue* m_Fc;
    ControlQLegacy* m_Q;
    ControlValue* m_gain;

    double m_fStart;
    double m_fStop;

    int m_bandNum{0};

    FilterCoefs m_coefs{0};

    void calcFilterCoefs();
    QList<QPointF> m_bandPoints;
    bool m_enabled{true};
};

#endif // EQBAND_H
