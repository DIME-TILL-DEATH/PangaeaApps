#include "controllerfx.h"

#include "cp100fx.h"
#include "fswfx.h"
#include "controlvalue.h"

ControllerFx::ControllerFx(AbstractDevice* owner, quint8 num)
    : AbstractModule(owner, ModuleType::CONTROLLER, "Controller", ""),
    m_num{num},
    m_destination{nullptr},
    m_source{nullptr},
    m_minValue{nullptr},
    m_maxValue{nullptr}
{
    m_destination = new ControlValue(this, nullptr, QString("cntrl %1 dst").arg(QString::number(num, 16)),
                                     "Destination", "", 0, 31, 0, 31);

    m_source = new ControlValue(this, nullptr, QString("cntrl %1 src").arg(QString::number(num, 16)),
                                "Source", "", 0, 127, 0, 127);
    m_source->setDisplaySetter(std::bind(&ControllerFx::srcDisplaySetter, this, std::placeholders::_1));
    m_source->setControlSetter(std::bind(&ControllerFx::srcControlValueSetter, this, std::placeholders::_1));

    m_minValue = new ControlValue(this, nullptr, QString("cntrl %1 min").arg(QString::number(num, 16)),
                                  "Min Value", "", 0, 127, 0, 127);
    m_maxValue = new ControlValue(this, nullptr, QString("cntrl %1 max").arg(QString::number(num, 16)),
                                  "Max Value", "", 0, 127, 0, 127);

    Cp100fx* device = dynamic_cast<Cp100fx*>(m_owner);
    if(device)
    {
        foreach(QObject* object, device->fswList())
        {
            FswFx* fsw = dynamic_cast<FswFx*>(object);
            connect(fsw, &FswFx::fswTypeChanged, this, &ControllerFx::avaliableSourcesChanged);
        }
    }
}

QStringList ControllerFx::sourcesList()
{
    QStringList srcList;
    srcList.append("Off");
    srcList.append("Expression");
    srcList.append("FSW Down");
    srcList.append("FSW Confirm");
    srcList.append("FSW Up");

    for(quint8 i=0; i<128; i++)
    {
        srcList.append("CC# " + QString::number(i));
    }

    const char* note_list[12] =
        { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

    int8_t noteNum;

    for(quint8 i=0; i<120; i++)
    {
        if(i < 12)
            noteNum = -2;
        else if(i >= 12 && i < 24)
            noteNum = -1;
        else
            noteNum = i / 12 - 2;

        const char* noteChar = note_list[i % 12];
        srcList.append("Note " + QString(noteChar) + QString::number(noteNum));
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
        if(static_cast<FswFx::FswType>(fsw->pressType()->displayValue()) != FswFx::Controller
            && static_cast<FswFx::FswType>(fsw->holdType()->displayValue()) != FswFx::Controller) srcList.removeAll("FSW Down");

        fsw = dynamic_cast<FswFx*>(device->fswList().at(1));
        if(static_cast<FswFx::FswType>(fsw->pressType()->displayValue()) != FswFx::Controller
            && static_cast<FswFx::FswType>(fsw->holdType()->displayValue()) != FswFx::Controller) srcList.removeAll("FSW Confirm");

        fsw = dynamic_cast<FswFx*>(device->fswList().at(2));
        if(static_cast<FswFx::FswType>(fsw->pressType()->displayValue()) != FswFx::Controller
            && static_cast<FswFx::FswType>(fsw->holdType()->displayValue()) != FswFx::Controller) srcList.removeAll("FSW Up");
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

void ControllerFx::setData(const controller_fx_t &m_controllerData)
{
    m_destination->setControlValue(m_controllerData.dst);
    m_source->setControlValue(m_controllerData.src);
    m_minValue->setControlValue(m_controllerData.minVal);
    m_maxValue->setControlValue(m_controllerData.maxVal);
}

void ControllerFx::srcDisplaySetter(double value)
{
    QStringList avaliableSrcList = avaliableSourcesList();
    if(value > avaliableSrcList.count()) return;

    QString source = avaliableSrcList.at(value);
    qDebug() << "Source: " << source;

    QStringList srcList = sourcesList();

    for(quint8 i=0; i < srcList.count(); i++)
    {
        if(srcList.at(i) == source)
        {
            m_source->modifyDisplayValue(value);
            emit m_source->displayValueChanged();

            sendDataToDevice(QByteArray(m_source->commandString().toUtf8() + " " + QString::number(i, 16).toUtf8() + "\r\n"));
            return;
        }
    }
}

void ControllerFx::srcControlValueSetter(qint32 value)
{
    QStringList srcList = sourcesList();
    QString source = srcList.at(value);
    // qDebug() << "Source: " << source;

    QStringList avaliableSrcList = avaliableSourcesList();
    for(quint8 i=0; i < avaliableSrcList.count(); i++)
    {
        if(avaliableSrcList.at(i) == source)
        {
            m_source->modifyDisplayValue(i);
            emit m_source->displayValueChanged();
            return;
        }
    }
}
