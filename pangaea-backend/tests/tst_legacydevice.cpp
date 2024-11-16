#include <QtTest>

#include "threadcontroller.h"
#include "core.h"

class LegacyDevice : public QObject
{
    Q_OBJECT

public:
    LegacyDevice();
    ~LegacyDevice();

private slots:
    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();
    void testConnect();

private:
    Core* core;
};

LegacyDevice::LegacyDevice()
{
    Core* core = new Core;

    ThreadController threadController(QThread::currentThread());
    core->moveToThread(threadController.backendThread());
}

LegacyDevice::~LegacyDevice()
{
}

// вызывается перед первой тестовой функцией
void LegacyDevice::initTestCase()
{

    /* 1.создать mock device
     * 2.скопировать в его рабочую директорию содержиоме resources/device_internal
     * */
}

// вызывается перед каждой тестовой функцией
void LegacyDevice::init() {}

// вызывается после каждой тестовой функции
void LegacyDevice::cleanup() {}

void LegacyDevice::cleanupTestCase()
{
    // удалить всё что насоздавал mock device
}


void LegacyDevice::testConnect() {}

QTEST_APPLESS_MAIN(LegacyDevice)

#include "tst_legacydevice.moc"
