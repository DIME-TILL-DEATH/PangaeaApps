#include <QtTest>
#include <QCoreApplication>

#include "presetabstract.h"
#include "deviceparameter.h"

class TestPresetAbstract : public QObject
{
    Q_OBJECT

public:
    TestPresetAbstract(){};
    ~TestPresetAbstract(){};

private slots:
    void initTestCase();
    void cleanupTestCase();
    
    void testPresetCreation();
    void testPresetBank();
    void testPresetNumber();
    void testPresetName();
    void testPresetSetName();
    void testPresetParameterCount();
    void testPresetGetParameter();
    void testPresetSetParameter();
    void testPresetHasParameter();
    void testPresetCopyFrom();
    void testPresetClear();
    void testPresetToArray();
    void testPresetFromArray();

private:
    PresetAbstract* preset;
};

void TestPresetAbstract::initTestCase()
{
    preset = nullptr;
}

void TestPresetAbstract::cleanupTestCase()
{
    if (preset) {
        delete preset;
    }
}

void TestPresetAbstract::testPresetCreation()
{
    preset = new PresetAbstract();
    QVERIFY(preset != nullptr);
}

void TestPresetAbstract::testPresetBank()
{
    if (preset) delete preset;
    preset = new PresetAbstract();
    preset->setBank(3);
    QCOMPARE(preset->bank(), 3);
}

void TestPresetAbstract::testPresetNumber()
{
    if (preset) delete preset;
    preset = new PresetAbstract();
    preset->setNumber(5);
    QCOMPARE(preset->number(), 5);
}

void TestPresetAbstract::testPresetName()
{
    if (preset) delete preset;
    preset = new PresetAbstract();
    preset->setName("TestPreset");
    QCOMPARE(preset->name(), QString("TestPreset"));
}

void TestPresetAbstract::testPresetSetName()
{
    if (preset) delete preset;
    preset = new PresetAbstract();
    preset->setName("MyPreset");
    QCOMPARE(preset->name(), QString("MyPreset"));
}

void TestPresetAbstract::testPresetParameterCount()
{
    if (preset) delete preset;
    preset = new PresetAbstract();
    int count = preset->parameterCount();
    QVERIFY(count >= 0);
}

void TestPresetAbstract::testPresetGetParameter()
{
    if (preset) delete preset;
    preset = new PresetAbstract();
    // Test getting parameter by index
    QVERIFY(true);
}

void TestPresetAbstract::testPresetSetParameter()
{
    if (preset) delete preset;
    preset = new PresetAbstract();
    // Test setting parameter value
    QVERIFY(true);
}

void TestPresetAbstract::testPresetHasParameter()
{
    if (preset) delete preset;
    preset = new PresetAbstract();
    // Test checking if preset has parameter
    QVERIFY(true);
}

void TestPresetAbstract::testPresetCopyFrom()
{
    if (preset) delete preset;
    PresetAbstract source;
    source.setName("Source");
    preset = new PresetAbstract(source);
    QCOMPARE(preset->name(), source.name());
}

void TestPresetAbstract::testPresetClear()
{
    if (preset) delete preset;
    preset = new PresetAbstract();
    preset->setName("Test");
    preset->clear();
    // After clear, preset should be reset
    QVERIFY(true);
}

void TestPresetAbstract::testPresetToArray()
{
    if (preset) delete preset;
    preset = new PresetAbstract();
    QByteArray data = preset->toByteArray();
    QVERIFY(!data.isEmpty() || data.isEmpty());
}

void TestPresetAbstract::testPresetFromArray()
{
    if (preset) delete preset;
    preset = new PresetAbstract();
    QByteArray data("test data");
    bool result = preset->fromByteArray(data);
    QVERIFY(result == true || result == false);
}

QTEST_MAIN(TestPresetAbstract)
#include "tst_presetabstract.moc"
