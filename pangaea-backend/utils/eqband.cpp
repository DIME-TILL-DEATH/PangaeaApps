#include <cmath>
#include <complex>

#include "eqband.h"

EqBand::EqBand(FilterType bandType, double fStart, double fStop, int bandNum, QObject *parent)
    : QObject{parent},
    m_type{bandType},
    m_fStart{fStart},
    m_fStop{fStop},
    m_bandNum{bandNum}
{
    calcFilterCoefs();
}

void EqBand::calcFilterCoefs()
{
    double A = pow(10, m_gain/40);
    double w0 = 2 * M_PI * m_Fc/Fs;
    double a = sin(w0)/(2*m_Q);

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

double EqBand::getFc() const
{
    return m_Fc;
}

void EqBand::setFcValue(qint8 value)
{
    double valueMin = -100;
    double valueMax = 100;
    double freqMin = m_fStart;
    double freqMax = m_fStop;

    double k2 = (freqMin-freqMax)/(valueMin-valueMax);
    double k1 = freqMin-(valueMin*k2);
    int freqVal = k1 + value*k2;

    m_Fc = freqVal;
    calcFilterCoefs();
    emit FcChanged();
}

void EqBand::setFc(double newFc)
{
    if (qFuzzyCompare(m_Fc, newFc))
        return;

    m_Fc = newFc;
    calcFilterCoefs();
    emit FcChanged();

    double valueMin = -100;
    double valueMax = 100;
    double freqMin = m_fStart;
    double freqMax = m_fStop;

    double k2 = (freqMin-freqMax)/(valueMin-valueMax);
    double k1 = freqMin-(valueMin*k2);

    quint8 value = (m_Fc - k1)/k2;

    emit setDeviceParameter((DeviceParameter::Type)((quint8)DeviceParameter::Type::EQ_FREQ1 + m_bandNum), value);
}

double EqBand::getQ() const
{
    return m_Q;
}

void EqBand::setQValue(qint8 value)
{
    double valueMin = -100;
    double valueMax = 99;
    double qMin = 0.1;
    double qMax = 20;

    double k2 = (qMin-qMax)/(valueMin-valueMax);
    double k1 = qMin-(valueMin*k2);
    double qVal = k1 + value*k2;

    m_Q = qVal;
    calcFilterCoefs();
    emit QChanged();
}

void EqBand::setQ(double newQ)
{
    if (qFuzzyCompare(m_Q, newQ))
        return;

    m_Q = newQ;
    calcFilterCoefs();
    emit QChanged();

    double valueMin = -100;
    double valueMax = 99;
    double qMin = 0.1;
    double qMax = 20;

    double k2 = (qMin-qMax)/(valueMin-valueMax);
    double k1 = qMin-(valueMin*k2);

    quint8 value = (m_Q - k1)/k2;
    emit setDeviceParameter((DeviceParameter::Type)((quint8)DeviceParameter::Type::EQ_Q1 + m_bandNum), value);
}

double EqBand::getGain() const
{
    return m_gain;
}

void EqBand::setGainValue(quint8 value)
{
    double valueMin = 0;
    double valueMax = 31;
    double gainMin = -15;
    double gainMax = 15;

    double k2 = (gainMin-gainMax)/(valueMin-valueMax);
    double k1 = gainMin-(valueMin*k2);
    int gainVal = k1 + value*k2;

    m_gain = gainVal;
    calcFilterCoefs();
    emit gainChanged();
}

void EqBand::setGain(double newGain)
{
    if (qFuzzyCompare(m_gain, newGain))
        return;

    m_gain = newGain;
    calcFilterCoefs();
    emit gainChanged();

    double valueMin = 0;
    double valueMax = 31;
    double gainMin = -15;
    double gainMax = 15;

    double k2 = (gainMin-gainMax)/(valueMin-valueMax);
    double k1 = gainMin-(valueMin*k2);

    quint8 value = (m_gain - k1)/k2;

    emit setDeviceParameter((DeviceParameter::Type)((quint8)DeviceParameter::Type::EQ_VOLUME1 + m_bandNum), value);
}
