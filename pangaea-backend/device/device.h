#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>

#include "devicetypeenum.h"

class Firmware;

class Device
{
public:
    Device();
    Device(DeviceType deviceType);

    DeviceType deviceType() const;
    void setDeviceType(DeviceType newDeviceType);

    bool isFimwareSufficient();
    bool isFirmwareCanIndicate();

    Firmware *actualFirmware() const;   
    void setActualFirmware(Firmware *newActualFirmware);

    Firmware *minimalFirmware() const;

    quint8 maxBankCount() const {return m_maxBankCount;};
    quint8 maxPresetCount() const {return m_maxPresetCount;};

    bool havePaSection() const;

    QString firmwareName() const;

private:
    DeviceType m_deviceType{DeviceType::UnknownDev};

    Firmware* m_actualFirmware{nullptr};
    Firmware* m_minimalFirmware{nullptr};
    Firmware* m_indicationFirmware{nullptr};

    QString m_firmwareName;

    bool m_havePaSection;

    quint8 m_maxBankCount{0};
    quint8 m_maxPresetCount{0};
};

#endif // DEVICE_H
