#include "mockcp16modern.h"

MockCP16Modern::MockCP16Modern(QObject *parent)
    : AbstractMockDevice{parent}
{
    m_mockDeviceType = MockDeviceType::Mock_Abstract;

}

void MockCP16Modern::writeToDevice(const QByteArray &data)
{

}

void MockCP16Modern::initFolders()
{

}

bool MockCP16Modern::saveSysParameters()
{

}

bool MockCP16Modern::loadPresetData(quint8 prBank, quint8 prPreset, preset_data_t *presetData)
{

}

bool MockCP16Modern::savePresetData(quint8 prBank, quint8 prPreset, const preset_data_t *presetData)
{

}

void MockCP16Modern::amtDevCommHandler(const QString &command, const QByteArray &arguments)
{

}

void MockCP16Modern::amtVerCommHandler(const QString &command, const QByteArray &arguments)
{

}

void MockCP16Modern::bankPresetCommHandler(const QString &command, const QByteArray &arguments)
{

}

void MockCP16Modern::outputModeCommHandler(const QString &command, const QByteArray &arguments)
{

}

void MockCP16Modern::getStateCommHandler(const QString &command, const QByteArray &arguments)
{

}

void MockCP16Modern::getImpulseNameCommHandler(const QString &command, const QByteArray &arguments)
{

}

void MockCP16Modern::getRnsCommHandler(const QString &command, const QByteArray &arguments)
{

}

void MockCP16Modern::savePresetCommHandler(const QString &command, const QByteArray &arguments)
{

}

void MockCP16Modern::presetChangeCommHandler(const QString &command, const QByteArray &arguments)
{

}

void MockCP16Modern::formatMemoryCommHandler(const QString &command, const QByteArray &arguments)
{

}

void MockCP16Modern::ccCommHandler(const QString &command, const QByteArray &arguments)
{

}

void MockCP16Modern::escAckCommHandler(const QString &command, const QByteArray &arguments)
{

}

void MockCP16Modern::setParamsHandler(QString commStr, quint8 *commPtr)
{

}

void MockCP16Modern::setEqHandler(QString commStr, quint8 *commPtr)
{

}

void MockCP16Modern::parametersCommHandler(const QString &command, const QByteArray &arguments)
{

}

void MockCP16Modern::eqParametersCommHandler(const QString &command, const QByteArray &arguments)
{

}

void MockCP16Modern::startFwUpdateCommHandler(const QString &command, const QByteArray &arguments)
{

}
