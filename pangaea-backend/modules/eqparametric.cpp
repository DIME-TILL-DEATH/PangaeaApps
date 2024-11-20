#include "abstractdevice.h"

#include "eqband.h"
#include "eqparametric.h"


EqParametric::EqParametric(AbstractDevice *owner, EqMode eqMode)
    : AbstractModule{owner, ModuleType::EQ, "EQ", "eqo"},
    m_eqMode{eqMode}
{
    m_EqBands.append(new EqBand(this, FilterType::PEAKING, 20, 220, 0));
    m_EqBands.append(new EqBand(this, FilterType::PEAKING, 260, 460, 1));
    m_EqBands.append(new EqBand(this, FilterType::PEAKING, 600, 1000, 2));
    m_EqBands.append(new EqBand(this, FilterType::PEAKING, 1000, 3000, 3));
    m_EqBands.append(new EqBand(this, FilterType::PEAKING, 1000, 11000, 4));

    m_hpf = new EqBand(this, FilterType::LOW_CUT, 20, 1000, 5, 0, 255);
    m_lpf = new EqBand(this, FilterType::HIGH_CUT, 1000, 20000, 6, 195, 0);

    if(m_eqMode == EqMode::Modern)
    {
        m_EqBands.append(m_hpf);
        m_EqBands.append(m_lpf);

        connect(m_hpf, &EqBand::enabledChanged, this, &EqParametric::hpfEnabledChanged);
        connect(m_lpf, &EqBand::enabledChanged, this, &EqParametric::lpfEnabledChanged);
    }
    calcEqResponse();

    for(int i=0; i<m_EqBands.count();i++)
    {
        EqBand* eqBand = qobject_cast<EqBand*>(m_EqBands.at(i));
        connect(eqBand, &EqBand::bandParametersChanged, this, &EqParametric::calcEqResponse);
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

void EqParametric::hpfEnabledChanged()
{
    emit m_owner->sgWriteToInterface(QString("ho " + QString("%1").arg(m_hpf->enabled()) + "\r\n").toUtf8());
}

void EqParametric::lpfEnabledChanged()
{
    emit m_owner->sgWriteToInterface(QString("lo " + QString("%1").arg(m_lpf->enabled()) + "\r\n").toUtf8());
}

void EqParametric::calcEqResponse()
{
    m_points.clear();

    for(int i=0; i<m_EqBands.count(); i++)
    {
        EqBand* eqBand = qobject_cast<EqBand*>(m_EqBands.at(i));
        eqBand->calcBandResponse(pointsNum);
    }

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

        m_points.append(QPointF(currFreq, eqPointResponse));
    }
   emit pointsChanged();
}

void EqParametric::setBandsData(eq_t eqData)
{
    for(int i=0; i < 5; i++)
    {
        EqBand* eqBand = qobject_cast<EqBand*>(m_EqBands.at(i));

        eqBand->setRawBandParams(static_cast<FilterType>(eqData.band_type[i]),
                                 eqData.band_vol[i],
                                 eqData.freq[i],
                                 eqData.Q[i]);
    }

    if(m_eqMode == EqMode::Modern)
    {
        m_hpf->setRawBandParams(FilterType::LOW_CUT, 0, eqData.hp_freq, 1, eqData.hp_on);
        m_lpf->setRawBandParams(FilterType::HIGH_CUT, 0, eqData.lp_freq, 1, eqData.lp_on);
    }

    emit dataChanged();
}
