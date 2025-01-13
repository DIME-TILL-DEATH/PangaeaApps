#include "abstractmodule.h"
#include "abstractdevice.h"

#include <QtQuick/QQuickView>

// for register:
#include "controlvalue.h"

#include "presetvolume.h"

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
#include "earlyreflections.h"

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
    qmlRegisterUncreatableType<Compressor>("CppObjects", 1, 0, "Compressor", "Cannot create Compressor in QML");
    qmlRegisterUncreatableType<Preamp>("CppObjects", 1, 0, "Preamp", "Cannot create Preamp in QML");
    qmlRegisterUncreatableType<PowerAmp>("CppObjects", 1, 0, "PowerAmp", "Cannot create PowerAmp in QML");
    qmlRegisterUncreatableType<Presence>("CppObjects", 1, 0, "Presence", "Cannot create Presence in QML");
    qmlRegisterUncreatableType<CabSim>("CppObjects", 1, 0, "CabSim", "Cannot create CabSim in QML");
    qmlRegisterUncreatableType<HiPassFilter>("CppObjects", 1, 0, "HiPassFilter", "Cannot create HP in QML");
    qmlRegisterUncreatableType<EqParametric>("CppObjects", 1, 0, "EqParametric", "Cannot create EQ in QML");
    qmlRegisterUncreatableType<EqBand>("CppObjects", 1, 0, "EqBand", "Cannot create EqBand in QML");
    qmlRegisterUncreatableType<LowPassFilter>("CppObjects", 1, 0, "LowPassFilter", "Cannot create LP in QML");
    qmlRegisterUncreatableType<Tremolo>("CppObjects", 1, 0, "Tremolo", "Cannot create TR in QML");
    qmlRegisterUncreatableType<EarlyReflections>("CppObjects", 1, 0, "EarlyReflections", "Cannot create LP in QML");
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

void AbstractModule::sendDataToDevice(const QByteArray &data)
{
    if(m_owner) emit m_owner->sgWriteToInterface(data, true);
}
