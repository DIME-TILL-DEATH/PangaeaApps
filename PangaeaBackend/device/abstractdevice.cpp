#include "abstractdevice.h"

#include "core.h"

AbstractDevice::AbstractDevice(Core *owner)
{
    if(owner)
    {
        connect(this, &AbstractDevice::sgEnableTimeoutTimer, owner->timeoutTimer, qOverload<>(&QTimer::start));
        connect(this, &AbstractDevice::sgDisableTimeoutTimer, owner->timeoutTimer, &QTimer::stop);
    }
}

AbstractDevice::~AbstractDevice()
{
    disconnect(this);

    if(m_actualFirmware) delete(m_actualFirmware);
    if(m_minimalFirmware) delete(m_minimalFirmware);

    qDeleteAll(m_moduleList.begin(), m_moduleList.end());
    m_moduleList.clear();
}

void AbstractDevice::setOutputMode(quint8 newOutputMode)
{
    if (m_outputMode == newOutputMode)
        return;
    m_outputMode = newOutputMode;
    emit outputModeChanged();

    // Второй gm потому что CPLegacy не ставит \n в конце
    emit sgWriteToInterface(QString("gm %1\r\n").arg(m_outputMode, 0, 16).toUtf8());
}

QStringList AbstractDevice::strPresetNumbers()
{
    QStringList numbers;

    for(quint8 i=0; i < m_maxPresetCount; i++)
    {
        numbers.append(QString::number(i));
    }
    return numbers;
}

QList<QByteArray> AbstractDevice::parseAnswers(QByteArray baAnswer)
{
    QList<QByteArray> args;
    if(undefCommParser.getParse(baAnswer, &args))
    {
        QByteArray data;
        if(!args.isEmpty()) data = args.first();
        undefinedCommandCommHandler("", data);
    }

    QList<QByteArray> recievedCommAnswers = m_parser.parseNewData(baAnswer);
    emit sgCommandsRecieved(recievedCommAnswers);
    return recievedCommAnswers;
}

void AbstractDevice::userModifiedModules()
{
    m_deviceParamsModified = true;
    emit deviceParamsModifiedChanged();

    if(actualPreset)
    {
        m_presetListModel.updatePreset(actualPreset);
    }
}

void AbstractDevice::undefinedCommandCommHandler(const QString &command, const QByteArray &arguments)
{
    emit sgDeviceError(DeviceErrorType::UndefinedCommand, "", {command, arguments});
}

bool AbstractDevice::deviceParamsModified() const
{
    return m_deviceParamsModified;
}

void AbstractDevice::modulesParamsSetCommHandler(const QString &command, const QByteArray &arguments)
{

}
