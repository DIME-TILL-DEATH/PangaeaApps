#include "abstractmodule.h"
#include "abstractdevice.h"

#include <QtQuick/QQuickView>

// for register:
#include "controlvalue.h"

#include "presetvolume.h"

#include "noisegate.h"
#include "poweramp.h"
#include "eqband.h"
#include "cabsim.h"
#include "eqparametric.h"


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
    qmlRegisterType<ControlValue>("CppObjects", 1, 0, "ControlValue");
    qmlRegisterUncreatableType<AbstractModule>("CppObjects", 1, 0, "Module", "Cannot create Module in QML");
    qmlRegisterUncreatableType<PresetVolume>("CppObjects", 1, 0, "PresetVolume", "Cannot create MasterVolume in QML");
    qmlRegisterUncreatableType<NoiseGate>("CppObjects", 1, 0, "NoiseGate", "Cannot create NoiseGate in QML");
    qmlRegisterUncreatableType<PowerAmp>("CppObjects", 1, 0, "PowerAmp", "Cannot create PowerAmp in QML");
    qmlRegisterUncreatableType<CabSim>("CppObjects", 1, 0, "CabSim", "Cannot create CabSim in QML");
    qmlRegisterUncreatableType<EqParametric>("CppObjects", 1, 0, "EqParametric", "Cannot create EQ in QML");
    qmlRegisterUncreatableType<EqBand>("CppObjects", 1, 0, "EqBand", "Cannot create EqBand in QML");

}

void AbstractModule::setModuleEnabled(bool newEnabled)
{
    if (m_moduleEnabled == newEnabled)
        return;
    m_moduleEnabled = newEnabled;

    QString fullCommand = m_commandOnOff + QString(" %1\r\n").arg(m_moduleEnabled, 0, 16);
    if(m_owner) emit m_owner->sgWriteToInterface(fullCommand.toUtf8(), true);
    emit dataChanged();
    emit userModifiedModuleParameters();
}

void AbstractModule::sendDataToDevice(const QByteArray &data)
{
    if(m_owner) emit m_owner->sgWriteToInterface(data, true);
}
