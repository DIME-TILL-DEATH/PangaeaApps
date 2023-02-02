#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>

class Firmware;

enum DeviceType
{
    UnknownDev=0,
    CP100,
    CP16,
    CP16PA,
    CP100PA
};

class Device
{
public:
    Device();

    quint8 maxBankPresetCount() const;

    DeviceType deviceType() const;
    void setDeviceType(DeviceType newDeviceType);

    bool isFimwareSUfficient();

    Firmware *actualFirmware() const;

    void setActualFirmware(Firmware *newActualFirmware);

    Firmware *minimalFirmware() const;

private:
    DeviceType m_deviceType{DeviceType::UnknownDev};

    Firmware* m_actualFirmware{nullptr};
    Firmware* m_minimalFirmware{nullptr};

    quint8 m_maxBankPresetCount{0};
};

#endif // DEVICE_H
