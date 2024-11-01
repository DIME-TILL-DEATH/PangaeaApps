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

    if(m_indicationFirmware != nullptr)
    {
        delete(m_indicationFirmware);
        m_indicationFirmware = nullptr;
    }

    switch(m_deviceType)
    {
        case DeviceType::CP16:
            m_minimalFirmware = new Firmware(CP16_FIRMWARE_VERSION, newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareCP16.ble");
            m_indicationFirmware = new Firmware("PA.01.05.00", newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareCP16.ble");
            m_maxBankCount = 4;
            m_maxPresetCount = 4;
            m_havePaSection = false;
            m_firmwareName = "CP-16M Blue";
        break;
        case DeviceType::CP16PA:
            m_minimalFirmware = new Firmware(CP16PA_FIRMWARE_VERSION, newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareCP16PA.ble");
            m_indicationFirmware = new Firmware("PA.01.05.00", newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareCP16PA.ble");
            m_maxBankCount = 4;
            m_maxPresetCount = 4;
            m_havePaSection = true;
            m_firmwareName = "CP-16M-PA Green";
        break;
        case DeviceType::CP100:
            m_minimalFirmware = new Firmware(CP100_FIRMWARE_VERSION, newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareCP100.ble");
            m_indicationFirmware = new Firmware("PA.02.08.12", newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareCP100.ble");
            m_maxBankCount = 10;
            m_maxPresetCount = 10;
            m_havePaSection = false;
            m_firmwareName = "CP-100";
        break;
        case DeviceType::CP100PA:
            m_minimalFirmware = new Firmware(CP100PA_FIRMWARE_VERSION, newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareCP100PA.ble");
            m_indicationFirmware = new Firmware("PA.06.09.08", newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareCP100PA.ble");
            m_maxBankCount = 10;
            m_maxPresetCount = 10;
            m_havePaSection = true;
            m_firmwareName = "CP-100PA";
        break;

        case DeviceType::LA3RV:
            m_minimalFirmware = new Firmware(LA3RV_FIRMWARE_VERSION, newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareLA3RV.ble");
            m_indicationFirmware = new Firmware("PA.01.05.00", newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareLA3RV.ble");
            m_maxBankCount = 0;
            m_maxPresetCount = 16;
            m_havePaSection = false;
            m_firmwareName = "LA3RV";
        break;
        case DeviceType::LA3PA:
            m_minimalFirmware = new Firmware(LA3PA_FIRMWARE_VERSION, newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareLA3PA.ble");
            m_indicationFirmware = new Firmware("PA.01.05.00", newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareLA3PA.ble");
            m_maxBankCount = 0;
            m_maxPresetCount = 16;
            m_havePaSection = true;
            m_firmwareName = "LA3PA";
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

bool Device::isFirmwareCanIndicate()
{
    if(m_actualFirmware==nullptr)
    {
        qWarning() << __FUNCTION__ << "Firmwares does not set!";
        return false;
    }
    if(m_indicationFirmware == nullptr) return false;

    if(*m_actualFirmware < *m_indicationFirmware) return false;

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

bool Device::havePaSection() const
{
    return m_havePaSection;
}

QString Device::firmwareName() const
{
    return m_firmwareName;
}


DeviceType Device::deviceType() const
{
    return m_deviceType;
}

