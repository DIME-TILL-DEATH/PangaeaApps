#include "abstractmodule.h"
#include "abstractdevice.h"

AbstractModule::AbstractModule(AbstractDevice* owner, ModuleType moduleType, QString name, QString commandOnOff)
    : QObject{owner},
    m_owner{owner},
    m_moduleType{moduleType},
    m_moduleName{name},
    m_commandOnOff{commandOnOff}
{
    if(owner) connect(this, &AbstractModule::userModifiedModuleParameters, owner, &AbstractDevice::userModifiedModules);
}

void AbstractModule::setModuleEnabled(bool newEnabled)
{
    if (m_moduleEnabled == newEnabled)
        return;
    m_moduleEnabled = newEnabled;

    QString fullCommand = m_commandOnOff + QString(" %1\r\n").arg(m_moduleEnabled, 0, 16);
    if(m_owner) emit m_owner->sgWriteToInterface(fullCommand.toUtf8(), true);
    emit dataChanged();
}

void AbstractModule::sendDataToDevice(const QByteArray &data)
{
    if(m_owner) emit m_owner->sgWriteToInterface(data, true);
}
