#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>

#include "devicetypeenum.h"

class Firmware;

class Device
{
public:
    Device();


    DeviceType deviceType() const;
    void setDeviceType(DeviceType newDeviceType);

    bool isFimwareSufficient();
    bool isFirmwareCanIndicate();

    Firmware *actualFirmware() const;   
    void setActualFirmware(Firmware *newActualFirmware);

    Firmware *minimalFirmware() const;

    quint8 maxBankCount() const {return m_maxBankCount;};
    quint8 maxPresetCount() const {return m_maxPresetCount;};

private:
    DeviceType m_deviceType{DeviceType::UnknownDev};

    Firmware* m_actualFirmware{nullptr};
    Firmware* m_minimalFirmware{nullptr};
    Firmware* m_indicationFirmware{nullptr};

    quint8 m_maxBankCount{0};
    quint8 m_maxPresetCount{0};
};

#endif // DEVICE_H
