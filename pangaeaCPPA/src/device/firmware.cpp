#include "firmware.h"

Firmware::Firmware(QString version, DeviceType deviceType, FirmwareType type, QString path)
     :m_firmwareVersionString{version},
      m_deviceType{deviceType},
      m_firmwareType{type},
      m_path{path}
{
    m_firmwareVersion = extractVerFromString(m_firmwareVersionString);
}

strVersion Firmware::extractVerFromString(QString versionString)
{
    QStringList resultList = versionString.split('.');

    strVersion result;

    if(resultList.at(0) == "PA")
    {
        result.isPA = true;
        resultList.removeFirst();
    }

    if(resultList.count()==3)
    {
        result.global = resultList.at(0).toInt();
        result.major = resultList.at(1).toInt();
        result.minor = resultList.at(2).toInt();
    }
    return result;
}

bool Firmware::operator<(const Firmware &compareFirmware)
{
    if(compareFirmware.deviceType() != m_deviceType)
        qWarning() << "Comparing firmwares different device types!";

    return compareVersion(m_firmwareVersion, compareFirmware.m_firmwareVersion);
}

bool Firmware::operator>(const Firmware &compareFirmware)
{
    if(compareFirmware.deviceType() != m_deviceType)
        qWarning() << "Comparing firmwares different device types!";

    return compareVersion(compareFirmware.m_firmwareVersion, m_firmwareVersion);
}

bool Firmware::compareVersion(strVersion requestedVesrion, strVersion devVersion)
{
    if(requestedVesrion.global > devVersion.global) return false;
    if(requestedVesrion.global < devVersion.global) return true;

    if(requestedVesrion.major > devVersion.major) return false;
    if(requestedVesrion.major < devVersion.major) return true;

    if(requestedVesrion.minor > devVersion.minor) return false;
    if(requestedVesrion.minor < devVersion.minor) return true;

    return false;
}

QString Firmware::firmwareVersion() const
{
    return m_firmwareVersionString;
}

FirmwareType Firmware::firmwareType() const
{
    return m_firmwareType;
}

const QString &Firmware::path() const
{
    return m_path;
}

const QByteArray &Firmware::rawData() const
{
    return m_rawData;
}

void Firmware::setRawData(const QByteArray &newRawData)
{
    m_rawData = newRawData;
}

DeviceType Firmware::deviceType() const
{
    return m_deviceType;
}
