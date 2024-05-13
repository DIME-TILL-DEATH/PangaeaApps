#include <cmath>

#include "eqband.h"
#include "eqresponse.h"


EqResponse::EqResponse(QObject *parent)
    : AbstractModule(DeviceParameter::Type::EQ_ON, parent)
{
    m_EqBands.append(new EqBand(this, EqBand::FilterType::LOW_SHELF, 20, 220, 0));
    m_EqBands.append(new EqBand(this, EqBand::FilterType::PEAKING, 260, 440, 1));
    m_EqBands.append(new EqBand(this, EqBand::FilterType::PEAKING, 600, 1000, 2));
    m_EqBands.append(new EqBand(this, EqBand::FilterType::PEAKING, 1000, 3000, 3));
    m_EqBands.append(new EqBand(this, EqBand::FilterType::HIGH_SHELF, 1000, 11000, 4));
    calcEqResponse();

    for(int i=0; i<m_EqBands.count();i++)
    {
        EqBand* eqBand = qobject_cast<EqBand*>(m_EqBands.at(i));
        connect(eqBand, &EqBand::sgSetDeviceParameter, this, &EqResponse::sgSetDeviceParameter);

        connect(eqBand, &EqBand::filterChanged, this, &EqResponse::calcEqResponse);
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

// void EqResponse::slFilterParamChanged(DeviceParameter::Type deviceParameterType, qint32 value)
// {
//     //slSetDeviceParameter(deviceParameterType, value);
//     calcEqResponse();
// }

void EqResponse::calcEqResponse()
{
    qDebug() << "Calcing response";
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
