#include <cmath>
#include <complex>

#include "eqband.h"

EqBand::EqBand(AbstractModule *ownerModule, FilterType bandType, double fStart, double fStop, int bandNum)
    : m_type{bandType},
    m_fStart{fStart},
    m_fStop{fStop},
    m_bandNum{bandNum}
{

    m_Fc = new ControlValue(ownerModule, "eqf " + QString().setNum(m_bandNum),
                            "Central Frequency", "Hz",
                            -100, 100, m_fStart, m_fStop);
    m_gain = new ControlValue(ownerModule, "eqv " + QString().setNum(m_bandNum),
                            "Gain", "dB",
                              0x0000, 0x001E, -15, 15);

    m_Q = new ControlQLegacy(ownerModule, "eqq " + QString().setNum(m_bandNum));

    connect(m_Fc, &ControlValue::displayValueChanged, this, &EqBand::calcFilterCoefs);
    connect(m_gain, &ControlValue::displayValueChanged, this, &EqBand::calcFilterCoefs);
    connect(m_Q, &ControlValue::displayValueChanged, this, &EqBand::calcFilterCoefs);

    connect(m_Fc, &ControlValue::userModifiedValue, this, &EqBand::calcFilterCoefs);
    connect(m_gain, &ControlValue::userModifiedValue, this, &EqBand::calcFilterCoefs);
    connect(m_Q, &ControlValue::userModifiedValue, this, &EqBand::calcFilterCoefs);
    calcFilterCoefs();
}

EqBand::~EqBand()
{
    delete(m_Fc);
    delete(m_gain);
    delete(m_Q);
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

void EqBand::calcBandResponse(quint16 pointsNum)
{
    m_bandPoints.clear();

    double currFreq = 10;
    double power = log10(currFreq);
    double powerStep = (log10(EqBand::Fs/2)-power)/pointsNum;
    for(int i=0; i<pointsNum+1; i++)
    {
        m_bandPoints.append(QPointF(currFreq, getFilterResponse(currFreq)));
        power += powerStep;
        currFreq = pow(10, power);
    }
    emit bandPointsChanged();
}

EqBand::FilterType EqBand::type() const
{
    return m_type;
}

ControlValue* EqBand::getFc() const
{
    return m_Fc;
}

ControlValue* EqBand::getQ() const
{
    return m_Q;
}

ControlValue* EqBand::getGain() const
{
    return m_gain;
}

void EqBand::setRawBandParams(FilterType bandType, quint16 gain, qint16 Fc, qint16 Q)
{
    m_gain->setControlValue(gain);
    m_Fc->setControlValue(Fc);
    m_Q->setControlValue(Q);
}

QList<QPointF> EqBand::bandPoints()
{
    return m_bandPoints;
}
