#include "cp100fx.h"

#include "core.h"
#include "eqband.h"

Cp100fx::Cp100fx(Core *parent)
    : AbstractDevice{parent}
{

}

Cp100fx::~Cp100fx()
{

}

void Cp100fx::updateOutputModeNames()
{

}

quint16 Cp100fx::processingUsed()
{
    return 0;
}

void Cp100fx::initDevice(DeviceType deviceType)
{
    m_deviceType = deviceType;


    PR = new Preamp(this);
    PA = new PowerAmp(this);

    // typeToModuleMap.insert(ModuleType::PR, PR);
    // typeToModuleMap.insert(ModuleType::PA, PA);

    m_moduleList.append(PR);
    m_moduleList.append(PA);

    m_modulesListModel.refreshModel(&m_moduleList);

    emit modulesListModelChanged();
    emit presetListModelChanged();
    emit sgDeviceInstanciated();
}

void Cp100fx::readFullState()
{

}

void Cp100fx::saveChanges()
{

}

void Cp100fx::changePreset(quint8 newBank, quint8 newPreset, bool ignoreChanges)
{

}

void Cp100fx::comparePreset()
{

}

void Cp100fx::copyPreset()
{

}

void Cp100fx::pastePreset()
{

}

void Cp100fx::importPreset(QString filePath, QString fileName)
{

}

void Cp100fx::exportPreset(QString filePath, QString fileName)
{

}

void Cp100fx::restoreValue(QString name)
{

}

void Cp100fx::previewIr(QString srcFilePath)
{

}

void Cp100fx::startIrUpload(QString srcFilePath, QString dstFilePath, bool trimFile)
{

}

void Cp100fx::setFirmware(QString fullFilePath)
{

}

void Cp100fx::formatMemory()
{

}

QList<QByteArray> Cp100fx::parseAnswers(QByteArray &baAnswer)
{
    QList<QByteArray> recievedCommAnswers;
    return recievedCommAnswers;
}
