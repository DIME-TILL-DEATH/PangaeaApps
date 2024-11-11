#include <cmath>

#include "eqband.h"
#include "eqparametric.h"


EqParametric::EqParametric(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::EQ, "EQ", "eqo"}
{
    m_EqBands.append(new EqBand(this, EqBand::FilterType::PEAKING, 20, 220, 0));
    m_EqBands.append(new EqBand(this, EqBand::FilterType::PEAKING, 260, 460, 1));
    m_EqBands.append(new EqBand(this, EqBand::FilterType::PEAKING, 600, 1000, 2));
    m_EqBands.append(new EqBand(this, EqBand::FilterType::PEAKING, 1000, 3000, 3));
    m_EqBands.append(new EqBand(this, EqBand::FilterType::PEAKING, 1000, 11000, 4));
    calcEqResponse();

    for(int i=0; i<m_EqBands.count();i++)
    {
        EqBand* eqBand = qobject_cast<EqBand*>(m_EqBands.at(i));
        // connect(eqBand, &EqBand::sgSetDeviceParameter, this, &EqResponse::sgSetDeviceParameter);

        connect(eqBand, &EqBand::filterChanged, this, &EqParametric::calcEqResponse);
    }
}

double EqParametric::getEqResponse(double f)
{
    double response=0;
    for(int i=0; i<m_EqBands.count(); i++)
    {
        EqBand* eqBand = qobject_cast<EqBand*>(m_EqBands.at(i));
        response += eqBand->getFilterResponse(f);
    }
    return response;
}

void EqParametric::calcEqResponse()
{
    m_points.clear();

    for(int i=0; i<m_EqBands.count(); i++)
    {
        EqBand* eqBand = qobject_cast<EqBand*>(m_EqBands.at(i));
        eqBand->calcBandResponse(pointsNum);
    }

    // TODO: recalc from calculated bands response
    // double currFreq = 10;
    // double power = log10(currFreq);
    // double powerStep = (log10(EqBand::Fs/2)-power)/pointsNum;
    for(int i=0; i<pointsNum+1; i++)
    {
        double eqPointResponse = 0;
        double currFreq = 0;
        for(int j=0; j<m_EqBands.count(); j++)
        {
            EqBand* eqBand = qobject_cast<EqBand*>(m_EqBands.at(j));

            currFreq = eqBand->bandPoints().at(i).x();
            eqPointResponse += eqBand->bandPoints().at(i).y();
        }
        // m_points.append(QPointF(currFreq, getEqResponse(currFreq)));
        m_points.append(QPointF(currFreq, eqPointResponse));
        // power += powerStep;
        // currFreq = pow(10, power);
    }
   emit pointsChanged();
}

QList<QPointF> EqParametric::points()
{
    return m_points;
}

QObjectList EqParametric::EqBands()
{
    return m_EqBands;
}

void EqParametric::setBandsData(eq_t eqData)
{
    for(int i=0; i<m_EqBands.count(); i++)
    {
        EqBand* eqBand = qobject_cast<EqBand*>(m_EqBands.at(i));

        eqBand->setRawBandParams(static_cast<EqBand::FilterType>(eqData.band_type[i]),
                                 eqData.band_vol[i],
                                 eqData.freq[i],
                                 eqData.Q[i]);
    }

    emit dataChanged();
}
