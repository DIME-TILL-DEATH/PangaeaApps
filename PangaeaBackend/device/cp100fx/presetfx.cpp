#include "presetfx.h"

PresetFx::PresetFx(AbstractDevice *ownerDevice)
    : m_ownerDevice{ownerDevice}
{
    presetVersion = 3;
}

PresetAbstract& PresetFx::operator=(const PresetAbstract& preset)
{
    // *this = preset;
    if(&preset != this)
    {
        const PresetFx* fxPreset = dynamic_cast<const PresetFx*>(&preset);
        if(fxPreset)
        {
            m_bankNumber = fxPreset->m_bankNumber;
            m_presetNumber = fxPreset->m_presetNumber;
            presetData = fxPreset->presetData;

            m_ownerDevice = fxPreset->m_ownerDevice;
            m_presetName = fxPreset->m_presetName;
            m_presetComment = fxPreset->m_presetComment;

            m_ir1Name = fxPreset->ir1Name();
            m_ir2Name = fxPreset->ir2Name();

            memcpy(controller, fxPreset->controller, sizeof(TController) * ControllerFx::controllersCount);

            m_cntrlPcOut = fxPreset->cntrlPcOut();
            m_cntrlSet = fxPreset->cntrlSet();

            m_activeModules = fxPreset->activeModules();
        }
    }
    return *this;
}

preset_data_fx_t PresetFx::charsToPresetData(const QByteArray &ba)
{
    preset_data_fx_t presetData;
    quint8 rawArr[sizeof(preset_data_fx_t)];
    for(int i = 0; i < ba.size(); i += 2)
    {
        QString chByte = QString(ba.at(i)) + QString(ba.at(i+1));
        rawArr[i/2] = chByte.toInt(nullptr, 16);
    }
    memcpy(&presetData, rawArr, sizeof(preset_data_fx_t));
    return presetData;
}

QByteArray PresetFx::presetDataToChars(const preset_data_fx_t &presetData)
{
    quint8 buffer[sizeof(preset_data_fx_t)];
    memcpy(buffer, &presetData, sizeof(preset_data_fx_t));
    QByteArray baData;

    for(int i=0; i < sizeof(preset_data_fx_t);  i++)
    {
        QByteArray tempBa = QString().setNum(buffer[i], 16).toUtf8();

        if(tempBa.size() == 1) tempBa.push_front("0");
        baData.append(tempBa);
    }
    return baData;
}

 void PresetFx::setActiveModules(const QByteArray& ba)
{
     m_activeModules.clear();

    if(ba.at(13) ==  QString("1").toUtf8()) m_activeModules.append(ModuleType::RF);
    if(ba.at(11) ==  QString("1").toUtf8()) m_activeModules.append(ModuleType::NG);
    if(ba.at(12) ==  QString("1").toUtf8()) m_activeModules.append(ModuleType::CM);
    if(ba.at(10) ==  QString("1").toUtf8()) m_activeModules.append(ModuleType::PR);
    if(ba.at(0) ==  QString("1").toUtf8()) m_activeModules.append(ModuleType::PA);
    if(ba.at(1) ==  QString("1").toUtf8()) m_activeModules.append(ModuleType::IR);
    if(ba.at(2) ==  QString("1").toUtf8()) m_activeModules.append(ModuleType::EQ1);
    if(ba.at(4) ==  QString("1").toUtf8()) m_activeModules.append(ModuleType::PH);
    if(ba.at(5) ==  QString("1").toUtf8()) m_activeModules.append(ModuleType::FL);
    if(ba.at(6) ==  QString("1").toUtf8()) m_activeModules.append(ModuleType::CH);
    if(ba.at(3) ==  QString("1").toUtf8()) m_activeModules.append(ModuleType::DELAY);
    if(ba.at(7) ==  QString("1").toUtf8()) m_activeModules.append(ModuleType::ER_STEREO);
    if(ba.at(8) ==  QString("1").toUtf8()) m_activeModules.append(ModuleType::RV);
    if(ba.at(9) ==  QString("1").toUtf8()) m_activeModules.append(ModuleType::TR);
}

QStringList PresetFx::strActiveModules() const
{
    QStringList strList;

    foreach(ModuleType module, m_activeModules)
    {
        switch(module)
        {

        case ModuleTypeEnum::BYPASS: break;
        case ModuleTypeEnum::CM: strList.append("CM"); break;
        case ModuleTypeEnum::PR: strList.append("PR"); break;
        case ModuleTypeEnum::PA: strList.append("PA"); break;
        case ModuleTypeEnum::IR: strList.append("IR"); break;
        case ModuleTypeEnum::HP: break;
        case ModuleTypeEnum::EQ1: strList.append("EQ"); break;
        case ModuleTypeEnum::LP: break;
        case ModuleTypeEnum::NG: strList.append("GT"); break;
        case ModuleTypeEnum::PS: strList.append("PS"); break;
        case ModuleTypeEnum::TR: strList.append("TR"); break;
        case ModuleTypeEnum::CH: strList.append("CH"); break;
        case ModuleTypeEnum::PH: strList.append("PH"); break;
        case ModuleTypeEnum::EQ2: strList.append("EQ"); break;
        case ModuleTypeEnum::NUM_MONO_MODULE_TYPES: break;
        case ModuleTypeEnum::RF: strList.append("RF"); break;
        case ModuleTypeEnum::RV: strList.append("RV"); break;
        case ModuleTypeEnum::FL: strList.append("FL"); break;
        case ModuleTypeEnum::MEQ: break;
        case ModuleTypeEnum::ER_MONO: strList.append("ER"); break;
        case ModuleTypeEnum::ER_STEREO: strList.append("ER"); break;
        case ModuleTypeEnum::DELAY: strList.append("DL"); break;
        case ModuleTypeEnum::IR_STEREO: strList.append("IR"); break;
        case ModuleTypeEnum::MASTER: break;
            break;
        }
    }

    return strList;
}

void PresetFx::setPresetData(const preset_data_fx_t &newPresetData)
{
    presetData = newPresetData;

    m_activeModules.clear();
    if(newPresetData.switches.resonance_filter) m_activeModules.append(ModuleType::RF);
    if(newPresetData.switches.gate) m_activeModules.append(ModuleType::NG);
    if(newPresetData.switches.compressor) m_activeModules.append(ModuleType::CM);
    if(newPresetData.switches.preamp) m_activeModules.append(ModuleType::PR);
    if(newPresetData.switches.amp) m_activeModules.append(ModuleType::PA);
    if(newPresetData.switches.cab) m_activeModules.append(ModuleType::IR_STEREO);
    if(newPresetData.switches.eq) m_activeModules.append(ModuleType::EQ1);
    if(newPresetData.switches.phaser) m_activeModules.append(ModuleType::PH);
    if(newPresetData.switches.flanger) m_activeModules.append(ModuleType::FL);
    if(newPresetData.switches.chorus) m_activeModules.append(ModuleType::CH);
    if(newPresetData.switches.delay) m_activeModules.append(ModuleType::DELAY);
    if(newPresetData.switches.early_reflections) m_activeModules.append(ModuleType::ER_STEREO);
    if(newPresetData.switches.reverb) m_activeModules.append(ModuleType::RV);
    if(newPresetData.switches.tremolo) m_activeModules.append(ModuleType::TR);
}

void PresetFx::setCntrlPcOut(quint8 newCntrlPcOut)
{
    m_cntrlPcOut = newCntrlPcOut;
}

void PresetFx::setCntrlSet(quint8 newCntrlSet)
{
    m_cntrlSet = newCntrlSet;
}
