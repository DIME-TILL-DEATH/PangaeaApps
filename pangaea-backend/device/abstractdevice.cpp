#include "abstractdevice.h"

#include "core.h"

AbstractDevice::AbstractDevice(Core *owner)
   : m_deviceClass{DeviceClass::ABSTRACT}
{
    if(owner)
    {
        connect(this, &AbstractDevice::sgWriteToInterface, owner, &Core::sgWriteToInterface);

        connect(this, &AbstractDevice::sgEnableTimeoutTimer, owner->timeoutTimer, qOverload<>(&QTimer::start));
        connect(this, &AbstractDevice::sgDisableTimeoutTimer, owner->timeoutTimer, &QTimer::stop);
    }

    using namespace std::placeholders;
    // undefined идёт после неверное команды, за табуляцией
    // m_parser.addCommandHandler("undefind command", std::bind(&AbstractDevice::undefinedCommandCommHandler, this, _1, _2));

    m_parser.addCommandHandler("go", std::bind(&AbstractDevice::modulesParamsSetCommHandler, this, _1, _2));
    m_parser.addCommandHandler("gt", std::bind(&AbstractDevice::modulesParamsSetCommHandler, this, _1, _2));
    m_parser.addCommandHandler("gv", std::bind(&AbstractDevice::modulesParamsSetCommHandler, this, _1, _2));

    m_parser.addCommandHandler("ao", std::bind(&AbstractDevice::modulesParamsSetCommHandler, this, _1, _2));
    m_parser.addCommandHandler("av", std::bind(&AbstractDevice::modulesParamsSetCommHandler, this, _1, _2));
    m_parser.addCommandHandler("at", std::bind(&AbstractDevice::modulesParamsSetCommHandler, this, _1, _2));
    m_parser.addCommandHandler("as", std::bind(&AbstractDevice::modulesParamsSetCommHandler, this, _1, _2));
}

AbstractDevice::~AbstractDevice()
{
    disconnect(this);
}

void AbstractDevice::initDevice(DeviceType deviceType)
{

}

QList<QByteArray> AbstractDevice::parseAnswers(QByteArray &baAnswer)
{
    return m_parser.parseNewData(baAnswer);
}

void AbstractDevice::userModifiedModules()
{
    m_deviceParamsModified = true;
    emit deviceParamsModifiedChanged();
}

void AbstractDevice::undefinedCommandCommHandler(const QString &command, const QByteArray &arguments)
{
    qDebug() << "callback for undefined command";
    emit sgDeviceError(DeviceErrorType::UndefinedCommand, "", {command, arguments});
}

bool AbstractDevice::deviceParamsModified() const
{
    return m_deviceParamsModified;
}

void AbstractDevice::modulesParamsSetCommHandler(const QString &command, const QByteArray &arguments)
{
    qDebug() << "callback for setDeviceparam" << command << arguments;
}
