#include <QFile>
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

    if(resultList.size()>3)
    {
        // Legacy versioning support
        if(resultList.at(0) == "PA")
        {
            result.isPA = true;
            resultList.removeFirst();
        }

        if(resultList.at(0) == "RV")
        {
            result.isPA = false;
            resultList.removeFirst();
        }
    }
    else result.isPA = false;


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

    if(compareFirmware.m_firmwareVersion.global > m_firmwareVersion.global) return true;
    if(compareFirmware.m_firmwareVersion.global < m_firmwareVersion.global) return false;

    if(compareFirmware.m_firmwareVersion.major > m_firmwareVersion.major) return true;
    if(compareFirmware.m_firmwareVersion.major < m_firmwareVersion.major) return false;

    if(compareFirmware.m_firmwareVersion.minor > m_firmwareVersion.minor) return true;
    if(compareFirmware.m_firmwareVersion.minor < m_firmwareVersion.minor) return false;

    return false;
}

bool Firmware::operator<=(const Firmware &compareFirmware)
{
    if(compareFirmware.deviceType() != m_deviceType)
        qWarning() << "Comparing firmwares different device types!";

    if(compareFirmware.m_firmwareVersion.global > m_firmwareVersion.global) return true;
    if(compareFirmware.m_firmwareVersion.global < m_firmwareVersion.global) return false;

    if(compareFirmware.m_firmwareVersion.major > m_firmwareVersion.major) return true;
    if(compareFirmware.m_firmwareVersion.major < m_firmwareVersion.major) return false;

    if(compareFirmware.m_firmwareVersion.minor > m_firmwareVersion.minor) return true;
    if(compareFirmware.m_firmwareVersion.minor < m_firmwareVersion.minor) return false;

    return true;
}

Firmware &Firmware::operator=(const Firmware &other)
{
    if(this != &other)
    {
        m_firmwareVersionString = other.m_firmwareVersionString;
        m_deviceType = other.m_deviceType;
        m_firmwareType = other.m_firmwareType;
        m_path = other.m_path;
        m_firmwareVersion = other.m_firmwareVersion;
        m_rawData = other.m_rawData;
    }
    return *this;
}

bool Firmware::operator>(const Firmware &compareFirmware)
{
    if(compareFirmware.deviceType() != m_deviceType)
        qWarning() << "Comparing firmwares different device types!";

    // return compareVersion(compareFirmware.m_firmwareVersion, m_firmwareVersion);
    if(compareFirmware.m_firmwareVersion.global > m_firmwareVersion.global) return false;
    if(compareFirmware.m_firmwareVersion.global < m_firmwareVersion.global) return true;

    if(compareFirmware.m_firmwareVersion.major > m_firmwareVersion.major) return false;
    if(compareFirmware.m_firmwareVersion.major < m_firmwareVersion.major) return true;

    if(compareFirmware.m_firmwareVersion.minor > m_firmwareVersion.minor) return false;
    if(compareFirmware.m_firmwareVersion.minor < m_firmwareVersion.minor) return true;

    return false;
}

bool Firmware::operator>=(const Firmware &compareFirmware)
{
    if(compareFirmware.deviceType() != m_deviceType)
        qWarning() << "Comparing firmwares different device types!";

    // return compareVersion(compareFirmware.m_firmwareVersion, m_firmwareVersion);
    if(compareFirmware.m_firmwareVersion.global > m_firmwareVersion.global) return false;
    if(compareFirmware.m_firmwareVersion.global < m_firmwareVersion.global) return true;

    if(compareFirmware.m_firmwareVersion.major > m_firmwareVersion.major) return false;
    if(compareFirmware.m_firmwareVersion.major < m_firmwareVersion.major) return true;

    if(compareFirmware.m_firmwareVersion.minor > m_firmwareVersion.minor) return false;
    if(compareFirmware.m_firmwareVersion.minor < m_firmwareVersion.minor) return true;

    return true;
}

QString Firmware::firmwareVersion() const
{
    return QString().setNum(m_firmwareVersion.global) + "." +
           QString().setNum(m_firmwareVersion.major) + "." +
           QString().setNum(m_firmwareVersion.minor);
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

bool Firmware::isFirmwareFile(QString filePath)
{
    QFile file(filePath);

    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray magicNumber = file.read(4);
        file.close();

        return checkData(magicNumber);
    }
    else
    {
        qDebug() << __FUNCTION__ << __LINE__ << "Can not open file " << filePath;
        return false;
    }
}

bool Firmware::checkData(const QByteArray &data)
{
    if(data.size() < 4) return false;

    if(data.at(0) == 0x78 &&
        data.at(1) == 0x56 &&
        data.at(2) == 0x34 &&
        data.at(3) == 0x12) return true;
    else return false;
}

bool Firmware::checkData()
{
    return checkData(m_rawData);
}

DeviceType Firmware::deviceType() const
{
    return m_deviceType;
}
