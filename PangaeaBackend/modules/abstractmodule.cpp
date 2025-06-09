#include "abstractmodule.h"
#include "abstractdevice.h"

#include <QtQuick/QQuickView>

// for register:
#include "controlvalue.h"

#include "volume.h"

#include "compressor.h"
#include "noisegate.h"
#include "preamp.h"
#include "poweramp.h"
#include "eqband.h"
#include "cabsim.h"
#include "hipassfilter.h"
#include "eqparametric.h"
#include "lowpassfilter.h"
#include "presence.h"
#include "tremolo.h"
#include "chorus.h"
#include "phaser.h"
#include "earlyreflections.h"
#include "delay.h"

AbstractModule::AbstractModule(AbstractDevice* owner, ModuleType moduleType, QString name, QString commandOnOff)
    : QObject{owner},
    m_owner{owner},
    m_moduleType{moduleType},
    m_moduleName{name},
    m_commandOnOff{commandOnOff}
{
    if(owner) connect(this, &AbstractModule::userModifiedModuleParameters, owner, &AbstractDevice::userModifiedModules);
}

void AbstractModule::registerTypestoQml()
{

}

void AbstractModule::setUsed(bool newValue)
{
    if(m_used == newValue) return;

    m_used=newValue;
    emit usedChanged();
}

void AbstractModule::setModuleEnabled(bool newEnabled)
{
    if (m_moduleEnabled == newEnabled)
        return;
    m_moduleEnabled = newEnabled;

    QString fullCommand = m_commandOnOff + QString(" %1\r\n").arg(m_moduleEnabled, 0, 16);
    if(m_owner) emit m_owner->sgWriteToInterface(fullCommand.toUtf8(), true);

    // if(m_owner)
    // {
    //     emit m_owner->sgPushCommandToQueue(fullCommand.toUtf8(), true);
    //     emit m_owner->sgProcessCommands();
    // }
    emit dataChanged();
    emit userModifiedModuleParameters();
}

void AbstractModule::sendDataToDevice(QByteArray data)
{
    if(m_owner) emit m_owner->sgWriteToInterface(data, true);
}
