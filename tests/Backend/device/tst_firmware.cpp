#include <QtTest>
#include <QCoreApplication>

#include "firmware.h"

class TestFirmware : public QObject
{
    Q_OBJECT

public:
    TestFirmware(){};
    ~TestFirmware(){};

private slots:
    void initTestCase();
    void cleanupTestCase();
    
    void testConstructorDefault();
    void testConstructorWithParams();
    void testMajorVersion();
    void testMinorVersion();
    void testPatchVersion();
    void testSetVersions();
    void testEqualityOperator();
    void testInequalityOperator();
    void testLessThanOperator();
    void testGreaterThanOperator();
    void testVersionToString();
    void testParseVersionFromString();
    void testCopy();

private:
    Firmware* firmware;
};

void TestFirmware::initTestCase()
{
    firmware = nullptr;
}

void TestFirmware::cleanupTestCase()
{
    if (firmware) {
        delete firmware;
    }
}

void TestFirmware::testConstructorDefault()
{
    firmware = new Firmware();
    QVERIFY(firmware != nullptr);
}

void TestFirmware::testConstructorWithParams()
{
    if (firmware) delete firmware;
    firmware = new Firmware(1, 2, 3);
    QCOMPARE(firmware->major(), 1);
    QCOMPARE(firmware->minor(), 2);
    QCOMPARE(firmware->patch(), 3);
}

void TestFirmware::testMajorVersion()
{
    if (firmware) delete firmware;
    firmware = new Firmware(5, 0, 0);
    QCOMPARE(firmware->major(), 5);
}

void TestFirmware::testMinorVersion()
{
    if (firmware) delete firmware;
    firmware = new Firmware(1, 3, 0);
    QCOMPARE(firmware->minor(), 3);
}

void TestFirmware::testPatchVersion()
{
    if (firmware) delete firmware;
    firmware = new Firmware(1, 2, 7);
    QCOMPARE(firmware->patch(), 7);
}

void TestFirmware::testSetVersions()
{
    if (firmware) delete firmware;
    firmware = new Firmware();
    firmware->setVersion(2, 5, 1);
    QCOMPARE(firmware->major(), 2);
    QCOMPARE(firmware->minor(), 5);
    QCOMPARE(firmware->patch(), 1);
}

void TestFirmware::testEqualityOperator()
{
    Firmware fw1(1, 2, 3);
    Firmware fw2(1, 2, 3);
    QVERIFY(fw1 == fw2);
}

void TestFirmware::testInequalityOperator()
{
    Firmware fw1(1, 2, 3);
    Firmware fw2(1, 2, 4);
    QVERIFY(fw1 != fw2);
}

void TestFirmware::testLessThanOperator()
{
    Firmware fw1(1, 2, 3);
    Firmware fw2(1, 2, 4);
    QVERIFY(fw1 < fw2);
}

void TestFirmware::testGreaterThanOperator()
{
    Firmware fw1(1, 3, 0);
    Firmware fw2(1, 2, 0);
    QVERIFY(fw1 > fw2);
}

void TestFirmware::testVersionToString()
{
    Firmware fw(2, 3, 4);
    QString versionStr = fw.toString();
    QVERIFY(!versionStr.isEmpty());
    QVERIFY(versionStr.contains("2"));
}

void TestFirmware::testParseVersionFromString()
{
    Firmware fw1;
    fw1.fromString("1.2.3");
    QCOMPARE(fw1.major(), 1);
    QCOMPARE(fw1.minor(), 2);
    QCOMPARE(fw1.patch(), 3);
}

void TestFirmware::testCopy()
{
    if (firmware) delete firmware;
    Firmware original(3, 4, 5);
    firmware = new Firmware(original);
    QCOMPARE(firmware->major(), 3);
    QCOMPARE(firmware->minor(), 4);
    QCOMPARE(firmware->patch(), 5);
}

QTEST_MAIN(TestFirmware)
#include "tst_firmware.moc"
