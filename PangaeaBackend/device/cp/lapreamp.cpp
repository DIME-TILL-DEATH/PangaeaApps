#include "lapreamp.h"

LAPreamp::LAPreamp(Core *parent)
    : CPModern{parent}
{
    using namespace std::placeholders;
    m_parser.addCommandHandler("gb", std::bind(&LAPreamp::getBankPresetLa3CommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("la3map", std::bind(&LAPreamp::la3MapCommHandler, this, _1, _2, _3));
}

void LAPreamp::setDeviceType(DeviceType newDeviceType)
{
    m_deviceType = newDeviceType;

    if(m_minimalFirmware != nullptr)
    {
        delete(m_minimalFirmware);
        m_minimalFirmware = nullptr;
    }

    switch(m_deviceType)
    {
    case DeviceType::LA3:
        m_minimalFirmware = new Firmware("2.00.13", newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareLA3.ble");
        m_maxBankCount = 0;
        m_maxPresetCount = 16;
        m_firmwareName = "LA3";
        break;
    default:
        qWarning() << Q_FUNC_INFO << "Unknown device type";

        m_minimalFirmware = new Firmware("2.00.01", newDeviceType, FirmwareType::DeviceInternal, "");
        m_maxBankCount = 4;
        m_maxPresetCount = 4;
        m_firmwareName = "Unknown CP/LA device";
        break;
    }
}

void LAPreamp::readFullState()
{
    m_presetManager.setCurrentState(PresetState::Changing);

    emit sgPushCommandToQueue("amtver");
    emit sgPushCommandToQueue("plist");
    emit sgPushCommandToQueue("ls ir_library");
    emit sgPushCommandToQueue("gm");
    emit sgPushCommandToQueue("la3map get");
    emit sgPushCommandToQueue("la3map use");
    pushReadPresetCommands();

    emit sgProcessCommands();
}

void LAPreamp::changePreset(quint8 newBank, quint8 newPreset, bool ignoreChanges)
{
    if(newPreset == m_preset) return;

    quint8 realBank = newPreset / 4;
    quint8 realPreset = newPreset % 4;

    quint8 val = PresetLegacy::calcPresetFlatIndex(m_deviceType, realBank, realPreset);


    if(m_presetManager.currentState() == PresetState::Compare)
    {
        comparePreset();
    }

    m_presetListModel.updatePreset(&savedPreset); // Обновить актуальный пресет перед переключением

    emit sgPushCommandToQueue(QString("pc %2").arg(val, 2, 16, QChar('0')).toUtf8());
    emit sgProcessCommands();
}

void LAPreamp::setLa3Mappings(quint8 newClnPresetMap, quint8 newDrvPresetmap)
{
    quint8 realBpCln = ((newClnPresetMap/4) << 4) + newClnPresetMap%4;
    quint8 realBpDrv = ((newDrvPresetmap/4) << 4) + newDrvPresetmap%4;

    QByteArray baCln = QByteArray::number(realBpCln, 16) ;
    QByteArray baDrv = QByteArray::number(realBpDrv, 16);

    if(baCln.size() < 2) baCln.prepend("0");
    if(baDrv.size() < 2) baCln.prepend("0");

    emit sgPushCommandToQueue("la3map set\r" + baCln + baDrv + "\n");
    emit sgProcessCommands();
}

void LAPreamp::getBankPresetLa3CommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    m_bank = data.left(2).toUInt();
    m_preset  = data.right(2).toUInt();
    QString presetPath = "bank_" + QString().setNum(m_bank) + "/preset_" + QString().setNum(m_preset);

    m_preset = m_bank * 4 + m_preset;
    m_bank = 0;

    actualPreset.setBankPreset(m_bank, m_preset);

    qInfo() << "LA3 preset:" << m_preset;
    emit bankPresetChanged();

    emit sgPushCommandToQueue("ls " + presetPath.toUtf8() + "\r\n", false);
    emit sgProcessCommands();
}

void LAPreamp::la3MapCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QList<QByteArray> argList = arguments.split(' ');
    if(argList.size() < 1)
    {
        qWarning() << Q_FUNC_INFO << "Arg size incorrect!";
        return;
    }

    if(argList.at(0) == "get")
    {
        if(data.size() == 4)
        {
            quint8 hex = data.left(2).toShort(nullptr, 16);
            m_clnPresetMap = ((hex & 0xF0) >> 4) * 4 + hex & 0x0F;
            hex = data.right(2).toShort(nullptr, 16);
            m_drvPresetMap  = ((hex & 0xF0) >> 4) * 4 + hex & 0x0F;
            emit presetMapChanged();
        }
        else
        {
            qWarning() << Q_FUNC_INFO << "Data size incorrect!";
        }
    }

    if(argList.at(0) == "use")
    {
        if(argList.size() > 1)
        {
            m_presetManager.returnToPreviousState(); // for correct hardware changing
            m_presetManager.setCurrentState(PresetState::Changing);
            emit presetSwitched();

            setLa3Channel(argList.at(1).toInt());
            pushReadPresetCommands();
            emit sgProcessCommands();
        }
        else
        {
            qWarning() << Q_FUNC_INFO << "Data size incorrect!";
        }
    }
}

void LAPreamp::setClnPresetMap(quint8 newClnPresetMap)
{
    if (m_clnPresetMap == newClnPresetMap)
        return;
    m_clnPresetMap = newClnPresetMap;
    emit presetMapChanged();
}

void LAPreamp::setDrvPresetMap(quint8 newDrvPresetMap)
{
    if (m_drvPresetMap == newDrvPresetMap)
        return;
    m_drvPresetMap = newDrvPresetMap;
    emit presetMapChanged();
}

void LAPreamp::setLa3Channel(quint8 newLa3Channel)
{
    if (m_la3Channel == newLa3Channel)
        return;
    m_la3Channel = newLa3Channel;
    emit la3ChannelChanged();
}
