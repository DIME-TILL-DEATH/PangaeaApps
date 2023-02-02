#include <qdebug.h>

#include "device.h"
#include "firmware.h"

Device::Device()
{

}

void Device::setDeviceType(DeviceType newDeviceType)
{
    m_deviceType = newDeviceType;

    // TODO: не хардкодить пути и число банков
    switch(m_deviceType)
    {
        case CP16:
            m_minimalFirmware = new Firmware(CP16_FIRMWARE_VERSION, newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareCP16.ble");
            m_maxBankPresetCount = 4;
        break;
        case CP16PA:
            m_minimalFirmware = new Firmware(CP16PA_FIRMWARE_VERSION, newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareCP16PA.ble");
            m_maxBankPresetCount = 4;
        break;
        case CP100:
            m_minimalFirmware = new Firmware(CP100_FIRMWARE_VERSION, newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareCP100.ble");
            m_maxBankPresetCount = 10;
        break;
        case CP100PA:
            m_minimalFirmware = new Firmware(CP100PA_FIRMWARE_VERSION, newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareCP100PA.ble");
            m_maxBankPresetCount = 10;
        break;
    default:
        qDebug() << __FUNCTION__ << "Unknown device type";
        break;
    }
}

bool Device::isFimwareSUfficient()
{
    if(m_actualFirmware==nullptr | m_minimalFirmware==nullptr)
    {
        qWarning() << __FUNCTION__ << "Firmwares does not set!";
        return false;
    }

    if(m_actualFirmware >= m_minimalFirmware)
        return true;
    else return false;
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


DeviceType Device::deviceType() const
{
    return m_deviceType;
}

quint8 Device::maxBankPresetCount() const
{
    return m_maxBankPresetCount;
}
