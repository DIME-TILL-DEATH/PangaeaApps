#ifndef EQBAND_H
#define EQBAND_H

#include <QObject>

#include "controlvalue.h"
#include "deviceparameter.h"

class EqBand : public QObject
{
    Q_OBJECT

    Q_PROPERTY(FilterType type READ type MEMBER m_type NOTIFY typeChanged FINAL)
    Q_PROPERTY(ControlValue* Fc READ getFc CONSTANT)
    Q_PROPERTY(ControlValue* Q READ getQ CONSTANT)
    Q_PROPERTY(ControlValue* gain READ getGain CONSTANT)

public:    
    enum FilterType
    {
        HIGH_CUT,
        LOW_CUT,
        LOW_SHELF,
        HIGH_SHELF,
        PEAKING
    };

    explicit EqBand(AbstractModule* ownerModule, FilterType bandType, double fStart, double fStop, int bandNum);

    static constexpr double Fs = 48000;

    FilterType type() const;

    ControlValue* getFc() const;
    ControlValue* getQ() const;
    ControlValue* getGain() const;

    double fStart() {return m_fStart;};
    double fStop() {return m_fStop;};

    double getFilterResponse(double f);


signals:
    void typeChanged();
    void filterChanged();

    void sgSetDeviceParameter(DeviceParameter::Type deviceParameterType, quint8 value);

private:
    FilterType m_type;

    ControlValue* m_Fc;
    ControlValue* m_Q;
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
};

#endif // EQBAND_H
