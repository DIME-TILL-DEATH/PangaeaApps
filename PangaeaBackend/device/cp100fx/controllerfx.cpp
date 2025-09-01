#include "controllerfx.h"

#include "cp100fx.h"
#include "fswfx.h"

ControllerFx::ControllerFx(controller_fx_t *controllerData, quint8 num, AbstractDevice* owner)
    : QObject{owner},
    m_controllerData{controllerData},
    m_num{num},
    m_owner{owner}
{
    Cp100fx* device = dynamic_cast<Cp100fx*>(m_owner);
    if(device)
    {
        foreach(QObject* object, device->fswList())
        {
            FswFx* fsw = dynamic_cast<FswFx*>(object);
            connect(fsw, &FswFx::fswTypeChanged, this, &ControllerFx::avaliableSourcesChanged);
        }

        connect(this, &ControllerFx::controllerChanged, owner, &AbstractDevice::userModifiedModules);
    }
}

quint8 ControllerFx::destination() const
{
    if(m_controllerData == nullptr) return 0;
    return m_controllerData->dst;
}

void ControllerFx::setDestination(quint8 newDestination)
{
    if(m_controllerData == nullptr) return;

    if (m_controllerData->dst == newDestination)
        return;
    m_controllerData->dst = newDestination;
    emit controllerChanged();

    sendData((QString("cntrl %1 dst %2").arg(m_num, 2, 16).arg(m_controllerData->dst, 2, 16)).toUtf8());
}

quint8 ControllerFx::source()
{
    if(m_controllerData == nullptr) return 0;

    QString sourceName = sourcesList().at(m_controllerData->src);
    QStringList avaliableSources = avaliableSourcesList();

    for(int i=0; i<avaliableSources.count(); i++)
    {
        if(avaliableSources.at(i) == sourceName) return i;
    }

    return 0;
}

void ControllerFx::setSource(const QString &srcName)
{
    if(m_controllerData == nullptr) return;

    quint8 source = dataFromSourceName(srcName);

    if (m_controllerData->src == source)
        return;

    m_controllerData->src = source;
    emit controllerChanged();

    sendData((QString("cntrl %1 src %2").arg(m_num, 2, 16).arg(m_controllerData->src, 2, 16)).toUtf8());
}

quint8 ControllerFx::minValue() const
{
    if(m_controllerData == nullptr) return 0;
    return m_controllerData->minVal;
}

void ControllerFx::setMinValue(quint8 newMinValue)
{
    if(m_controllerData == nullptr) return;

    if (m_controllerData->minVal == newMinValue)
        return;
    m_controllerData->minVal = newMinValue;
    emit controllerChanged();

    sendData((QString("cntrl %1 min %2").arg(m_num, 2, 16).arg(m_controllerData->minVal, 2, 16)).toUtf8());
}

quint8 ControllerFx::maxValue() const
{
    if(m_controllerData == nullptr) return 0;
    return m_controllerData->maxVal;
}

void ControllerFx::setMaxValue(quint8 newMaxValue)
{
    if(m_controllerData == nullptr) return;

    if (m_controllerData->maxVal == newMaxValue)
        return;
    m_controllerData->maxVal = newMaxValue;
    emit controllerChanged();

    sendData((QString("cntrl %1 max %2").arg(m_num, 2, 16).arg(m_controllerData->maxVal, 2, 16)).toUtf8());
}

QStringList ControllerFx::sourcesList()
{
    QStringList srcList;
    srcList.append("Off");
    srcList.append("Expression");
    srcList.append("FSW Up");
    srcList.append("FSW Confirm");
    srcList.append("FSW Down");

    for(quint8 i=0; i<127; i++)
    {
        srcList.append("CC# " + QString::number(i));
    }

    return srcList;
}

QStringList ControllerFx::avaliableSourcesList()
{
    QStringList srcList = sourcesList();

    Cp100fx* device = dynamic_cast<Cp100fx*>(m_owner);
    if(device)
    {
        FswFx* fsw;
        fsw = dynamic_cast<FswFx*>(device->fswList().at(0));
        if(fsw->pressType() != FswFx::Controller && fsw->holdType() != FswFx::Controller) srcList.removeAll("FSW Down");

        fsw = dynamic_cast<FswFx*>(device->fswList().at(1));
        if(fsw->pressType() != FswFx::Controller && fsw->holdType() != FswFx::Controller) srcList.removeAll("FSW Confirm");

        fsw = dynamic_cast<FswFx*>(device->fswList().at(2));
        if(fsw->pressType() != FswFx::Controller && fsw->holdType() != FswFx::Controller) srcList.removeAll("FSW Up");
    }
    return srcList;
}

quint8 ControllerFx::dataFromSourceName(const QString &source)
{
    QStringList srcList = sourcesList();

    QMap<quint8, QString> sourcesMap;
    quint8 num = 0;
    for(auto it = srcList.begin(); it != srcList.end(); ++it)
    {
        sourcesMap.insert(num, *it);
        num++;
    }

    return sourcesMap.key(source);
}

void ControllerFx::sendData(const QByteArray &data)
{
    if(m_owner) emit m_owner->sgWriteToInterface(data + "\r\n");
}

