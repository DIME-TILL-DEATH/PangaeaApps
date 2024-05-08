#include <cmath>

#include "eqband.h"
#include "eqresponse.h"


EqResponse::EqResponse(QObject *parent)
    : QObject(parent)
{
    m_EqBands.append(new EqBand(EqBand::FilterType::LOW_SHELF, 20, 220, 0));
    m_EqBands.append(new EqBand(EqBand::FilterType::PEAKING, 260, 440, 1));
    m_EqBands.append(new EqBand(EqBand::FilterType::PEAKING, 600, 1000, 2));
    m_EqBands.append(new EqBand(EqBand::FilterType::PEAKING, 1000, 3000, 3));
    m_EqBands.append(new EqBand(EqBand::FilterType::HIGH_SHELF, 1000, 11000, 4));
    calcEqResponse();

    for(int i=0; i<m_EqBands.count();i++)
    {
        EqBand* eqBand = qobject_cast<EqBand*>(m_EqBands.at(i));
        connect(eqBand, &EqBand::setDeviceParameter, this, &EqResponse::setDeviceParameter);

        connect(eqBand, &EqBand::FcChanged, this, &EqResponse::calcEqResponse);
        connect(eqBand, &EqBand::gainChanged, this, &EqResponse::calcEqResponse);
        connect(eqBand, &EqBand::QChanged, this, &EqResponse::calcEqResponse);
    }

}

double EqResponse::getEqResponse(double f)
{
    double response=0;
    for(int i=0; i<m_EqBands.count(); i++)
    {
        EqBand* eqBand = qobject_cast<EqBand*>(m_EqBands.at(i));
        response += eqBand->getFilterResponse(f);
    }
    return response;
}

void EqResponse::slFilterParamChanged(DeviceParameter::Type deviceParameterType, qint32 value)
{
    switch(deviceParameterType)
    {
    case DeviceParameter::Type::EQ_VOLUME1: qobject_cast<EqBand*>(m_EqBands.at(0))->setGainValue(value); break;
    case DeviceParameter::Type::EQ_VOLUME2: qobject_cast<EqBand*>(m_EqBands.at(1))->setGainValue(value); break;
    case DeviceParameter::Type::EQ_VOLUME3: qobject_cast<EqBand*>(m_EqBands.at(2))->setGainValue(value); break;
    case DeviceParameter::Type::EQ_VOLUME4: qobject_cast<EqBand*>(m_EqBands.at(3))->setGainValue(value); break;
    case DeviceParameter::Type::EQ_VOLUME5: qobject_cast<EqBand*>(m_EqBands.at(4))->setGainValue(value); break;

    case DeviceParameter::Type::EQ_FREQ1: qobject_cast<EqBand*>(m_EqBands.at(0))->setFcValue(value); break;
    case DeviceParameter::Type::EQ_FREQ2: qobject_cast<EqBand*>(m_EqBands.at(1))->setFcValue(value); break;
    case DeviceParameter::Type::EQ_FREQ3: qobject_cast<EqBand*>(m_EqBands.at(2))->setFcValue(value); break;
    case DeviceParameter::Type::EQ_FREQ4: qobject_cast<EqBand*>(m_EqBands.at(3))->setFcValue(value); break;
    case DeviceParameter::Type::EQ_FREQ5: qobject_cast<EqBand*>(m_EqBands.at(4))->setFcValue(value); break;

    case DeviceParameter::Type::EQ_Q1: qobject_cast<EqBand*>(m_EqBands.at(0))->setQValue(value); break;
    case DeviceParameter::Type::EQ_Q2: qobject_cast<EqBand*>(m_EqBands.at(1))->setQValue(value); break;
    case DeviceParameter::Type::EQ_Q3: qobject_cast<EqBand*>(m_EqBands.at(2))->setQValue(value); break;
    case DeviceParameter::Type::EQ_Q4: qobject_cast<EqBand*>(m_EqBands.at(3))->setQValue(value); break;
    case DeviceParameter::Type::EQ_Q5: qobject_cast<EqBand*>(m_EqBands.at(4))->setQValue(value); break;

    default: return;
    }

    calcEqResponse();
}

void EqResponse::calcEqResponse()
{
    m_points.clear();

    double currFreq = 10;
    double power = log10(currFreq);
    double powerStep = (log10(EqBand::Fs/2)-power)/pointsNum;
    for(int i=0; i<pointsNum+1; i++)
    {
        m_points.append(QPointF(currFreq, getEqResponse(currFreq)));
        power += powerStep;
        currFreq = pow(10, power);
    }
    emit pointsChanged();
}

QList<QPointF> EqResponse::points()
{
    return m_points;
}

QObjectList EqResponse::EqBands()
{
    return m_EqBands;
}
