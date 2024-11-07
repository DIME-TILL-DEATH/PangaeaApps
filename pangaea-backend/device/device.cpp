#include <qdebug.h>

#include "device.h"
#include "firmware.h"

Device::Device()
{

}

Device::Device(DeviceType deviceType)
{
    setDeviceType(deviceType);
}

void Device::setDeviceType(DeviceType newDeviceType)
{
    m_deviceType = newDeviceType;

    if(m_minimalFirmware != nullptr)
    {
        delete(m_minimalFirmware);
        m_minimalFirmware = nullptr;
    }

    switch(m_deviceType)
    {
        case DeviceType::legacyCP16:
            m_minimalFirmware = new Firmware(CP16_FIRMWARE_VERSION, newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareCP16.ble");
            m_maxBankCount = 4;
            m_maxPresetCount = 4;
            m_firmwareName = "CP-16M Blue";
        break;
        case DeviceType::legacyCP16PA:
            m_minimalFirmware = new Firmware(CP16PA_FIRMWARE_VERSION, newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareCP16PA.ble");
            m_maxBankCount = 4;
            m_maxPresetCount = 4;
            m_firmwareName = "CP-16M-PA Green";
        break;
        case DeviceType::legacyCP100:
            m_minimalFirmware = new Firmware(CP100_FIRMWARE_VERSION, newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareCP100.ble");
            m_maxBankCount = 10;
            m_maxPresetCount = 10;
            m_firmwareName = "CP-100";
        break;
        case DeviceType::legacyCP100PA:
            m_minimalFirmware = new Firmware(CP100PA_FIRMWARE_VERSION, newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareCP100PA.ble");
            m_maxBankCount = 10;
            m_maxPresetCount = 10;
            m_firmwareName = "CP-100PA";
        break;

        case DeviceType::LA3:
            m_minimalFirmware = new Firmware(LA3_FIRMWARE_VERSION, newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareLA3RV.ble");
            m_maxBankCount = 0;
            m_maxPresetCount = 16;
            m_firmwareName = "LA3";
        break;
    default:
        qDebug() << __FUNCTION__ << "Unknown device type";
        break;
    }
}

bool Device::isFimwareSufficient()
{
    if(m_actualFirmware==nullptr || m_minimalFirmware==nullptr)
    {
        qWarning() << __FUNCTION__ << "Firmwares does not set!";
        return false;
    }

    if(*m_actualFirmware < *m_minimalFirmware) return false;

    return true;
}

Firmware *Device::actualFirmware() const
{
    return m_actualFirmware;
}

void Device::setActualFirmware(Firmware *newActualFirmware)
{
    m_actualFirmware = newActualFirmware;
}

Firmware *Device::minimalFirmware() const
{
    return m_minimalFirmware;
}

QString Device::firmwareName() const
{
    return m_firmwareName;
}

DeviceType Device::deviceType() const
{
    return m_deviceType;
}
