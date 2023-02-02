#ifndef FIRMWARE_H
#define FIRMWARE_H

#include <qdebug.h>
#include <qstring.h>
#include <qbytearray.h>

#include "device.h"

struct strVersion{
    quint8 global;
    quint8 major;
    quint8 minor;
    bool isPA;
};

enum FirmwareType
{
    Unknown = 0,
    DeviceInternal,
    UserSelected,
    ApplicationPackage,
    NetworkUpdate
};

enum FirmwareUpdateMode
{
    Offline = 0,
    Online
};

class Firmware
{
public:
    Firmware(QString version, DeviceType deviceType, FirmwareType type, QString path);

    DeviceType deviceType() const;

    QString firmwareVersion() const;

    bool operator>(const Firmware& compareFirmware);
    bool operator<(const Firmware& compareFirmware);

    FirmwareType firmwareType() const;

    const QString &path() const;

    const QByteArray &rawData() const;
    void setRawData(const QByteArray &newRawData);

private:
    QString m_firmwareVersionString;
    DeviceType m_deviceType;
    FirmwareType m_firmwareType{FirmwareType::Unknown};
    QString m_path;

    strVersion m_firmwareVersion;

    QByteArray m_rawData;

    bool compareVersion(strVersion requestedVesrion, strVersion devVersion);
    strVersion extractVerFromString(QString versionString);
};

#endif // FIRMWARE_H
