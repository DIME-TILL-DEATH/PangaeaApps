#include <cmath>
#include <complex>

#include "eqband.h"

EqBand::EqBand(AbstractModule *ownerModule, FilterType bandType, double fStart, double fStop, int bandNum)
    : QObject{ownerModule},
    m_type{bandType},
    m_fStart{fStart},
    m_fStop{fStop},
    m_bandNum{bandNum}
{

    m_Fc = new ControlValue(ownerModule, (DeviceParameter::Type)((quint8)DeviceParameter::Type::EQ_FREQ1+m_bandNum),
                            -100, 100, m_fStart, m_fStop, "Central Frequency", "Hz");
    m_gain = new ControlValue(ownerModule, (DeviceParameter::Type)((quint8)DeviceParameter::Type::EQ_VOLUME1+m_bandNum),
                              0x0000, 0x001E, -15, 15, "Gain", "dB");

    m_Q = new ControlValue(ownerModule, (DeviceParameter::Type)((quint8)DeviceParameter::Type::EQ_Q1+m_bandNum),
                           -100, 99, 0.1, 20, "Q-Factor");

    connect(m_Fc, &ControlValue::displayValueChanged, this, &EqBand::calcFilterCoefs);
    connect(m_gain, &ControlValue::displayValueChanged, this, &EqBand::calcFilterCoefs);
    connect(m_Q, &ControlValue::displayValueChanged, this, &EqBand::calcFilterCoefs);
    calcFilterCoefs();
}

void EqBand::calcFilterCoefs()
{
    double A = pow(10, m_gain->displayValue()/40);
    double w0 = 2 * M_PI * m_Fc->displayValue()/Fs;
    double a = sin(w0)/(2*m_Q->displayValue());

    switch(m_type)
    {
    case FilterType::LOW_CUT:
    {
        m_coefs.b0 = (1 + cos(w0))/2;
        m_coefs.b1 = -(1 + cos(w0));
        m_coefs.b2 = m_coefs.b0;

        m_coefs.a0 = 1 + a;
        m_coefs.a1 = -2*cos(w0);
        m_coefs.a2 = 1 - a;

        break;
    }
    case FilterType::LOW_SHELF:
    {
        m_coefs.b0 = A*((A+1) - (A-1)*cos(w0) + 2*sqrt(A)*a);
        m_coefs.b1 = 2*A*((A-1) - (A+1)*cos(w0));
        m_coefs.b2 = A*((A+1) - (A-1)*cos(w0) - 2*sqrt(A)*a);

        m_coefs.a0 = (A+1) + (A-1)*cos(w0) + 2*sqrt(A)*a;
        m_coefs.a1 = -2*((A-1) + (A+1)*cos(w0));
        m_coefs.a2 = (A+1) + (A-1)*cos(w0) - 2*sqrt(A)*a;
        break;
    }
    case FilterType::PEAKING:
    {
        m_coefs.b0 = 1 + a*A;
        m_coefs.b1 = -2*cos(w0);
        m_coefs.b2 = 1 - a*A;

        m_coefs.a0 = 1 + a/A;
        m_coefs.a1 = -2*cos(w0);
        m_coefs.a2 = 1 - a/A;
        break;
    }
    case FilterType::HIGH_SHELF:
    {
        m_coefs.b0 = A*((A+1) + (A-1)*cos(w0) + 2*sqrt(A)*a);
        m_coefs.b1 = -2*A*((A-1) + (A+1)*cos(w0));
        m_coefs.b2 = A*((A+1) + (A-1)*cos(w0) - 2*sqrt(A)*a);

        m_coefs.a0 = (A+1) - (A-1)*cos(w0) + 2*sqrt(A)*a;
        m_coefs.a1 = 2*((A-1) - (A+1)*cos(w0));
        m_coefs.a2 = (A+1) - (A-1)*cos(w0) - 2*sqrt(A)*a;
        break;
    }
    case FilterType::HIGH_CUT:
    {
        m_coefs.b0 = (1 - cos(w0))/2;
        m_coefs.b1 = 1 - cos(w0);
        m_coefs.b2 = m_coefs.b0;

        m_coefs.a0 = 1 + a;
        m_coefs.a1 = -2*cos(w0);
        m_coefs.a2 = 1 - a;
        break;
    }
    }

    emit filterChanged();
}

double EqBand::getFilterResponse(double f)
{
    std::complex<double> z = std::polar(1.0, 2*M_PI*f/Fs);
    std::complex<double> z2 = z*z;
    std::complex<double> h = (m_coefs.b0*z2 + m_coefs.b1*z + m_coefs.b2) / (m_coefs.a0*z2 + m_coefs.a1*z + m_coefs.a2);
    return 20*log10(abs(h));
}

EqBand::FilterType EqBand::type() const
{
    return m_type;
}

ControlValue* EqBand::getFc() const
{
    return m_Fc;
}

// void EqBand::setFcValue(qint8 value)
// {
//     double valueMin = -100;
//     double valueMax = 100;
//     double freqMin = m_fStart;
//     double freqMax = m_fStop;

//     double k2 = (freqMin-freqMax)/(valueMin-valueMax);
//     double k1 = freqMin-(valueMin*k2);
//     int freqVal = k1 + value*k2;

//     if (qFuzzyCompare(m_Fc, freqVal))
//         return;

//     m_Fc = freqVal;
//     emit FcChanged();



//     calcFilterCoefs();
// }

// void EqBand::setFc(double newFc)
// {
//     //------------------------------
//     if (qFuzzyCompare(m_Fc, newFc))
//         return;

//     m_Fc = newFc;
//     m_isFcModified = true;
//     emit FcChanged();
//     emit isFcModifiedChanged();

//     double valueMin = -100;
//     double valueMax = 100;
//     double freqMin = m_fStart;
//     double freqMax = m_fStop;

//     double k2 = (freqMin-freqMax)/(valueMin-valueMax);
//     double k1 = freqMin-(valueMin*k2);

//     qint8 value = (m_Fc - k1)/k2;
//     emit sgSetDeviceParameter((DeviceParameter::Type)((quint8)DeviceParameter::Type::EQ_FREQ1 + m_bandNum), value);
//     //----------------------------
//     calcFilterCoefs();
// }

ControlValue* EqBand::getQ() const
{
    return m_Q;
}

// void EqBand::setQValue(qint8 value)
// {
//     double valueMin = -100;
//     double valueMax = 99;
//     double qMin = 0.1;
//     double qMax = 20;

//     double k2 = (qMin-qMax)/(valueMin-valueMax);
//     double k1 = qMin-(valueMin*k2);
//     double qVal = k1 + value*k2;

//     if(m_Q == qVal) return;

//     m_Q = qVal;
//     emit QChanged();



//     calcFilterCoefs();
// }

// void EqBand::setQ(double newQ)
// {
//     if (qFuzzyCompare(m_Q, newQ))
//         return;

//     m_Q = newQ;
//     m_isQModified = true;
//     emit QChanged();
//     emit isQModifiedChanged();

//     double valueMin = -100;
//     double valueMax = 99;
//     double qMin = 0.1;
//     double qMax = 20;

//     double k2 = (qMin-qMax)/(valueMin-valueMax);
//     double k1 = qMin-(valueMin*k2);


//     qint8 value = (m_Q - k1)/k2;
//     emit sgSetDeviceParameter((DeviceParameter::Type)((quint8)DeviceParameter::Type::EQ_Q1 + m_bandNum), value);



//     calcFilterCoefs();
// }

ControlValue* EqBand::getGain() const
{
    return m_gain;
}

// void EqBand::setGainValue(quint8 value)
// {
//     double valueMin = 0;
//     double valueMax = 31;
//     double gainMin = -15;
//     double gainMax = 15;

//     double k2 = (gainMin-gainMax)/(valueMin-valueMax);
//     double k1 = gainMin-(valueMin*k2);
//     int gainVal = k1 + value*k2;

//     if(m_gain == gainVal) return;

//     m_gain = gainVal;
//     emit gainChanged();



//     calcFilterCoefs();
// }

// void EqBand::setGain(double newGain)
// {
//     if (qFuzzyCompare(m_gain, newGain))
//         return;

//     m_gain = newGain;
//     // TODO функция для установки double параметра и модицикации объекта
//     m_isGainModified=true;
//     emit gainChanged();
//     emit isGainModifiedChanged();

//     double valueMin = 0;
//     double valueMax = 31;
//     double gainMin = -15;
//     double gainMax = 15;

//     double k2 = (gainMin-gainMax)/(valueMin-valueMax);
//     double k1 = gainMin-(valueMin*k2);

//     qint8 value = (m_gain - k1)/k2;

//     emit sgSetDeviceParameter((DeviceParameter::Type)((quint8)DeviceParameter::Type::EQ_VOLUME1 + m_bandNum), value);




//     calcFilterCoefs();
// }

