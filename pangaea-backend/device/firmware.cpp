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
    else result.isPA = true;


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

    return true;
}

QString Firmware::firmwareVersion() const
{
    QString isPaStr;
    if(m_firmwareVersion.isPA) isPaStr = "PA";
    else isPaStr = "RV";
    return isPaStr + "." +
           QString().setNum(m_firmwareVersion.global) + "." +
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

        if(magicNumber.at(0) == 0x78 &&
           magicNumber.at(1) == 0x56 &&
           magicNumber.at(2) == 0x34 &&
           magicNumber.at(3) == 0x12) return true;
        else return false;

    }
    else
    {
        qDebug() << __FUNCTION__ << __LINE__ << "Can not open file " << filePath;
        return false;
    }
}

DeviceType Firmware::deviceType() const
{
    return m_deviceType;
}
