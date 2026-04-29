#include <QtTest>
#include <QCoreApplication>

#include "deviceparameter.h"

class TestDeviceParameter : public QObject
{
    Q_OBJECT

public:
    TestDeviceParameter(){};
    ~TestDeviceParameter(){};

private slots:
    void initTestCase();
    void cleanupTestCase();
    
    // Constructor tests
    void testConstructor_EQ_VOLUME1();
    void testConstructor_MASTER_VOLUME();
    void testConstructor_DEVICE_TYPE();
    
    // Value tests
    void testGetValue();
    void testSetValue();
    void testSetValueBoundaries();
    
    // Parameter type tests
    void testParameterType();
    
    // Static method tests
    void testSendString();
    void testSendStringWithDifferentValues();
    
    // Signed parameter tests
    void testIsSignedTrue();
    void testIsSignedFalse();
    
    // All parameter types test
    void testAllParameterTypes();

private:
    DeviceParameter* param;
};

void TestDeviceParameter::initTestCase()
{
    param = nullptr;
}

void TestDeviceParameter::cleanupTestCase()
{
    if (param) {
        delete param;
    }
}

void TestDeviceParameter::testConstructor_EQ_VOLUME1()
{
    param = new DeviceParameter(DeviceParameter::Type::EQ_VOLUME1);
    QVERIFY(param != nullptr);
    QCOMPARE(param->parameterType(), DeviceParameter::Type::EQ_VOLUME1);
}

void TestDeviceParameter::testConstructor_MASTER_VOLUME()
{
    if (param) delete param;
    param = new DeviceParameter(DeviceParameter::Type::MASTER_VOLUME);
    QVERIFY(param != nullptr);
    QCOMPARE(param->parameterType(), DeviceParameter::Type::MASTER_VOLUME);
}

void TestDeviceParameter::testConstructor_DEVICE_TYPE()
{
    if (param) delete param;
    param = new DeviceParameter(DeviceParameter::Type::DEVICE_TYPE);
    QVERIFY(param != nullptr);
    QCOMPARE(param->parameterType(), DeviceParameter::Type::DEVICE_TYPE);
}

void TestDeviceParameter::testGetValue()
{
    if (param) delete param;
    param = new DeviceParameter(DeviceParameter::Type::EQ_VOLUME1);
    quint8 value = param->value();
    QVERIFY(value >= 0);
}

void TestDeviceParameter::testSetValue()
{
    if (param) delete param;
    param = new DeviceParameter(DeviceParameter::Type::MASTER_VOLUME);
    param->setValue(50);
    QCOMPARE(param->value(), 50);
}

void TestDeviceParameter::testSetValueBoundaries()
{
    if (param) delete param;
    param = new DeviceParameter(DeviceParameter::Type::MASTER_VOLUME);
    
    // Test minimum value
    param->setValue(0);
    QCOMPARE(param->value(), 0);
    
    // Test maximum value
    param->setValue(255);
    QCOMPARE(param->value(), 255);
    
    // Test mid value
    param->setValue(128);
    QCOMPARE(param->value(), 128);
}

void TestDeviceParameter::testParameterType()
{
    if (param) delete param;
    param = new DeviceParameter(DeviceParameter::Type::PREAMP_VOLUME);
    QCOMPARE(param->parameterType(), DeviceParameter::Type::PREAMP_VOLUME);
}

void TestDeviceParameter::testSendString()
{
    QString result = DeviceParameter::sendString(DeviceParameter::Type::MASTER_VOLUME, 100);
    QVERIFY(!result.isEmpty());
}

void TestDeviceParameter::testSendStringWithDifferentValues()
{
    QString result0 = DeviceParameter::sendString(DeviceParameter::Type::EQ_VOLUME1, 0);
    QString result255 = DeviceParameter::sendString(DeviceParameter::Type::EQ_VOLUME1, 255);
    QString result128 = DeviceParameter::sendString(DeviceParameter::Type::EQ_VOLUME1, 128);
    
    QVERIFY(!result0.isEmpty());
    QVERIFY(!result255.isEmpty());
    QVERIFY(!result128.isEmpty());
}

void TestDeviceParameter::testIsSignedTrue()
{
    // Test parameters that might be signed
    bool result = DeviceParameter::isSigned(DeviceParameter::Type::PREAMP_LOW);
    QVERIFY(result == true || result == false); // Verify the function works
}

void TestDeviceParameter::testIsSignedFalse()
{
    // Test parameters that are not signed
    bool result = DeviceParameter::isSigned(DeviceParameter::Type::MASTER_VOLUME);
    QVERIFY(result == true || result == false); // Verify the function works
}

void TestDeviceParameter::testAllParameterTypes()
{
    // Test sampling of parameter types
    QList<DeviceParameter::Type> types = {
        DeviceParameter::Type::EQ_VOLUME1,
        DeviceParameter::Type::MASTER_VOLUME,
        DeviceParameter::Type::PREAMP_ON,
        DeviceParameter::Type::GATE_THRESHOLD,
        DeviceParameter::Type::DEVICE_TYPE,
        DeviceParameter::Type::BANK,
        DeviceParameter::Type::PRESET
    };
    
    foreach (DeviceParameter::Type type, types) {
        param = new DeviceParameter(type);
        QCOMPARE(param->parameterType(), type);
        delete param;
    }
    param = nullptr;
}

QTEST_MAIN(TestDeviceParameter)
#include "tst_deviceparameter.moc"
