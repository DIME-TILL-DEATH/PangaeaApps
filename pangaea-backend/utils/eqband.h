#ifndef EQBAND_H
#define EQBAND_H

#include <QObject>

#include "deviceparameter.h"

class EqBand : public QObject
{
    Q_OBJECT

    Q_PROPERTY(FilterType type READ type MEMBER m_type NOTIFY typeChanged FINAL)
    Q_PROPERTY(double Fc READ getFc WRITE setFc NOTIFY FcChanged FINAL)
    Q_PROPERTY(double Q READ getQ WRITE setQ NOTIFY QChanged FINAL)
    Q_PROPERTY(double gain READ getGain WRITE setGain NOTIFY gainChanged FINAL)

    Q_PROPERTY(double fStart READ fStart CONSTANT)
    Q_PROPERTY(double fStop READ fStop CONSTANT)
public:    
    enum FilterType
    {
        HIGH_CUT,
        LOW_CUT,
        LOW_SHELF,
        HIGH_SHELF,
        PEAKING
    };

    explicit EqBand(FilterType bandType, double fStart, double fStop, int bandNum, QObject *parent = nullptr);

    static constexpr double Fs = 48000;

    FilterType type() const;

    double getFc() const;
    void setFcValue(qint8 value);
    void setFc(double newFc);

    double getQ() const;
    void setQValue(qint8 value);
    void setQ(double newQ);

    double getGain() const;
    void setGainValue(quint8 value);
    void setGain(double newGain);

    double fStart() {return m_fStart;};
    double fStop() {return m_fStop;};

    double getFilterResponse(double f);
signals:
    void typeChanged();
    void FcChanged();
    void QChanged();
    void gainChanged();

    void setDeviceParameter(DeviceParameter::Type deviceParameterType, quint8 value);

private:
    FilterType m_type;

    double m_Fc{1000};
    double m_Q{1};
    double m_gain{0};

    double m_fStart{1000};
    double m_fStop{10000};
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
};

#endif // EQBAND_H
