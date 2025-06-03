#include <QtConcurrent/QtConcurrentMap>
#include <QtConcurrent/QtConcurrentRun>

#include "abstractdevice.h"

#include "eqband.h"
#include "eqparametric.h"


EqParametric::EqParametric(AbstractDevice *owner, EqMode eqMode, quint8 eqNumber)
    : AbstractModule{owner, ModuleType::EQ1, "EQ", "eqo"},
    m_eqMode{eqMode},
    m_eqNumber{eqNumber}
{
    m_processingTime = 25;
    m_fullModuleName = AbstractModule::tr("Parametric EQ");

    switch(m_eqMode)
    {
    case EqMode::Modern:
    {
        m_EqBands.append(new EqBand(this, FilterType::PEAKING, 20, 12000, 0, 20, 12000));
        m_EqBands.append(new EqBand(this, FilterType::PEAKING, 20, 12000, 1, 20, 12000));
        m_EqBands.append(new EqBand(this, FilterType::PEAKING, 20, 12000, 2, 20, 12000));
        m_EqBands.append(new EqBand(this, FilterType::PEAKING, 20, 12000, 3, 20, 12000));
        m_EqBands.append(new EqBand(this, FilterType::PEAKING, 20, 12000, 4, 20, 12000));

        m_hpf = new EqBand(this, FilterType::LOW_CUT, 20, 1000, 5, 20, 1000);
        m_lpf = new EqBand(this, FilterType::HIGH_CUT, 1000, 20000, 6, 1000, 20000);

        m_commandOnOff = "eq" + QString().setNum(eqNumber) + " par o";

        m_moduleType = (m_eqNumber == 0) ? ModuleType::EQ1 : ModuleType::EQ2;
        m_moduleName += QString::number(eqNumber+1);
        m_fullModuleName += QString::number(eqNumber+1);
        break;
    }
    case EqMode::Fx:
    {
        m_EqBands.append(new EqBand(this, FilterType::PEAKING, 20, 220, 0));
        m_EqBands.append(new EqBand(this, FilterType::PEAKING, 260, 460, 1));
        m_EqBands.append(new EqBand(this, FilterType::PEAKING, 600, 1000, 2));
        m_EqBands.append(new EqBand(this, FilterType::PEAKING, 1000, 3000, 3));
        m_EqBands.append(new EqBand(this, FilterType::PEAKING, 1000, 11000, 4));

        m_hpf = new EqBand(this, FilterType::LOW_CUT, 20, 1000, 5, 0, 127);
        m_lpf = new EqBand(this, FilterType::HIGH_CUT, 1000, 20000, 6, 127, 0);
        break;
    }
    default:
    {
        m_EqBands.append(new EqBand(this, FilterType::PEAKING, 20, 220, 0));
        m_EqBands.append(new EqBand(this, FilterType::PEAKING, 260, 460, 1));
        m_EqBands.append(new EqBand(this, FilterType::PEAKING, 600, 1000, 2));
        m_EqBands.append(new EqBand(this, FilterType::PEAKING, 1000, 3000, 3));
        m_EqBands.append(new EqBand(this, FilterType::PEAKING, 1000, 11000, 4));

        m_hpf = new EqBand(this, FilterType::LOW_CUT, 20, 1000, 5, 0, 255);
        m_lpf = new EqBand(this, FilterType::HIGH_CUT, 1000, 20000, 6, 195, 0);
    }
    }

    if(m_eqMode != EqMode::Legacy)
    {
        m_EqBands.append(m_hpf);
        m_EqBands.append(m_lpf);

        connect(m_hpf, &EqBand::enabledChanged, this, &EqParametric::hpfEnabledChanged);
        connect(m_lpf, &EqBand::enabledChanged, this, &EqParametric::lpfEnabledChanged);
    }
    calcEqResponse();

    EqBand* eqBand = qobject_cast<EqBand*>(m_EqBands.at(0));
    m_minFreq = eqBand->bandPoints().at(0).x();
    m_maxFreq = eqBand->bandPoints().at(pointsNum).x();

    for(int i=0; i<m_EqBands.count();i++)
    {
        EqBand* eqBand = qobject_cast<EqBand*>(m_EqBands.at(i));
        connect(eqBand, &EqBand::bandParametersChanged, this, &EqParametric::calcEqResponse);
        connect(eqBand, &EqBand::filterTypeChanged, this, &EqParametric::bandTypeChanged);
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

void EqParametric::reset()
{
    double defaultFrequencies[] = {120, 360, 800, 2000, 6000};

    for(int i=0; i<5; i++)
    {
        EqBand* eqBand = qobject_cast<EqBand*>(m_EqBands.at(i));

        eqBand->setFilterType(FilterType::PEAKING);
        eqBand->getFc()->setDisplayValue(defaultFrequencies[i]);
        eqBand->getGain()->setDisplayValue(0);
        eqBand->getQ()->setDisplayValue(0.8);
    }
}

void EqParametric::hpfEnabledChanged()
{
    if(m_eqMode == EqParametric::EqMode::Legacy)
    {
        emit m_owner->sgWriteToInterface(QString("ho " + QString("%1").arg(m_hpf->enabled()) + "\r\n").toUtf8());
    }
    else
    {
        emit m_owner->sgWriteToInterface(
            QString("eq" + QString().setNum(m_eqNumber) + " hp o " + QString("%1").arg(m_hpf->enabled()) + "\r\n").toUtf8());
    }
}

void EqParametric::lpfEnabledChanged()
{
    if(m_eqMode == EqParametric::EqMode::Legacy)
    {
        emit m_owner->sgWriteToInterface(QString("lo " + QString("%1").arg(m_lpf->enabled()) + "\r\n").toUtf8());
    }
    else
    {
        emit m_owner->sgWriteToInterface(
            QString("eq" + QString().setNum(m_eqNumber) + " lp o " + QString("%1").arg(m_lpf->enabled()) + "\r\n").toUtf8());
    }
}

void EqParametric::bandTypeChanged(int bandNum)
{
    if(m_eqMode == EqParametric::EqMode::Modern)
    {
        EqBand* eqBand = qobject_cast<EqBand*>(m_EqBands.at(bandNum));
        FilterType bandType = eqBand->filterType();

        emit m_owner->sgWriteToInterface(
            QString("eq" + QString().setNum(m_eqNumber) + " b" + QString().setNum(bandNum) + " t " + QString("%1").arg(bandType) + "\r\n").toUtf8());
    }
}

void EqParametric::calcEqResponse()
{
    m_points.clear();

    QVector<EqBand*> vectorBands;
    for(int i=0; i<m_EqBands.count(); i++)
    {
        EqBand* eqBand = qobject_cast<EqBand*>(m_EqBands.at(i));
        vectorBands << eqBand;
    }

    QFuture<void> future = QtConcurrent::map(vectorBands, [](EqBand*& eqBand) {
        EqBand::FilterCoefs coefs{0};
        if(eqBand->enabled()) coefs = eqBand->filterCoefs();
        else{
            coefs.a0 = 1;
            coefs.b0 = 1;
        }

        QList<QPointF> calcResult = EqBand::calcBandResponse(250, coefs);
        eqBand->setBandPoints(calcResult);
    });
    future.waitForFinished();

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

void EqParametric::setEqData(eq_cpmodern_t eqData)
{
    m_moduleEnabled = eqData.parametric_on;

    for(int i=0; i < 5; i++)
    {
        EqBand* eqBand = qobject_cast<EqBand*>(m_EqBands.at(i));

        eqBand->setRawBandParams(static_cast<FilterType>(eqData.band_type[i]),
                                 eqData.gain[i],
                                 eqData.freq[i],
                                 eqData.Q[i]);
    }

    if(m_eqMode != EqMode::Legacy)
    {
        m_hpf->setRawBandParams(FilterType::LOW_CUT, 0, eqData.hp_freq, 1, eqData.hp_on);
        m_lpf->setRawBandParams(FilterType::HIGH_CUT, 0, eqData.lp_freq, 1, eqData.lp_on);
    }

    emit dataChanged();
}
