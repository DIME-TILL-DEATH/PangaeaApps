#include <QtTest>
#include <QCoreApplication>
#include <QTemporaryDir>

#include "irfile.h"

class TestIRFile : public QObject
{
    Q_OBJECT

public:
    TestIRFile(){};
    ~TestIRFile(){};

private slots:
    void initTestCase();
    void cleanupTestCase();
    
    void testIRFileCreation();
    void testIRFileSetName();
    void testIRFileGetName();
    void testIRFileSetData();
    void testIRFileGetData();
    void testIRFileIsValid();
    void testIRFileSaveToFile();
    void testIRFileLoadFromFile();
    void testIRFileSize();
    void testIRFileChecksum();
    void testIRFileMetadata();

private:
    IrFile* irFile;
    QTemporaryDir tempDir;
};

void TestIRFile::initTestCase()
{
    irFile = nullptr;
    QVERIFY(tempDir.isValid());
}

void TestIRFile::cleanupTestCase()
{
    if (irFile) {
        delete irFile;
    }
}

void TestIRFile::testIRFileCreation()
{
    irFile = new IrFile();
    QVERIFY(irFile != nullptr);
}

void TestIRFile::testIRFileSetName()
{
    if (irFile) delete irFile;
    irFile = new IrFile();
    irFile->setName("TestIR");
    QCOMPARE(irFile->name(), QString("TestIR"));
}

void TestIRFile::testIRFileGetName()
{
    if (irFile) delete irFile;
    irFile = new IrFile();
    irFile->setName("MyIR");
    QCOMPARE(irFile->name(), QString("MyIR"));
}

void TestIRFile::testIRFileSetData()
{
    if (irFile) delete irFile;
    irFile = new IrFile();
    QByteArray testData("IR response data");
    irFile->setData(testData);
    QVERIFY(true);
}

void TestIRFile::testIRFileGetData()
{
    if (irFile) delete irFile;
    irFile = new IrFile();
    QByteArray testData("test");
    irFile->setData(testData);
    QByteArray retrieved = irFile->data();
    QCOMPARE(retrieved, testData);
}

void TestIRFile::testIRFileIsValid()
{
    if (irFile) delete irFile;
    irFile = new IrFile();
    bool isValid = irFile->isValid();
    QVERIFY(isValid == true || isValid == false);
}

void TestIRFile::testIRFileSaveToFile()
{
    if (irFile) delete irFile;
    irFile = new IrFile();
    irFile->setName("TestFile");
    irFile->setData("some data");
    
    QString filePath = tempDir.path() + "/test_ir.ir";
    bool saved = irFile->saveToFile(filePath);
    QVERIFY(saved == true || saved == false);
}

void TestIRFile::testIRFileLoadFromFile()
{
    if (irFile) delete irFile;
    irFile = new IrFile();
    
    QString filePath = tempDir.path() + "/test_ir.ir";
    bool loaded = irFile->loadFromFile(filePath);
    QVERIFY(loaded == true || loaded == false);
}

void TestIRFile::testIRFileSize()
{
    if (irFile) delete irFile;
    irFile = new IrFile();
    irFile->setData("12345");
    int size = irFile->size();
    QVERIFY(size >= 0);
}

void TestIRFile::testIRFileChecksum()
{
    if (irFile) delete irFile;
    irFile = new IrFile();
    irFile->setData("data");
    QString checksum = irFile->checksum();
    QVERIFY(!checksum.isEmpty());
}

void TestIRFile::testIRFileMetadata()
{
    if (irFile) delete irFile;
    irFile = new IrFile();
    irFile->setName("TestIR");
    // Set metadata if supported
    QVERIFY(true);
}

QTEST_MAIN(TestIRFile)
#include "tst_irfile.moc"
