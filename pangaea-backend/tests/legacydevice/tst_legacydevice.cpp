#include <QtTest>
#include <QCoreApplication>

#include "threadcontroller.h"
#include "core.h"

#include "cplegacy.h"
#include "mockcp16legacy.h"

#include "testutils.h"

class LegacyDevice : public QObject
{
    Q_OBJECT

public:
    LegacyDevice(){};
    ~LegacyDevice(){};

signals:
    void deviceConnected(DeviceDescription);

private slots:
    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();
    void testCopyPasteSavedIR();

    //17 подгруженный ir К: параметры и импульс В: только параметры, нет импульса, сохранение
    //18 подгруженный ir К: параметры и импульс В: только параметры, нет импульса, отмена
    //19 подгруженный ir К: параметры и импульс В: параметры и импульс, сохранение
    //20 подгруженный ir К: параметры и импульс В: параметры и импульс, отмена
    //21 подгруженный ir К: параметры и импульс;  подгруженный ir В: параметры и импульс, сохранение
    //22 подгруженный ir К: параметры и импульс;  подгруженный ir В: параметры и импульс, отмена
    //23 К: импортированный, но не сохранённый, параметры и импульс; В: только параметры
    //24 К: импортированный, но не сохранённый, параметры и импульс; В: параметры и импульс
    //25 К: импортированный, но не сохранённый, только параметры В: параметры и импульс

private:
    Core* core;
    CPLegacy* cpLegacy;
    MockCP16Legacy* mockDevice;

    ThreadController* threadController;

    void copyPaste(quint8 bankFrom, quint8 presetFrom, quint8 bankTo, quint8 presetTo);
};

void LegacyDevice::initTestCase()
{
    threadController = new ThreadController(QThread::currentThread());

    core = new Core;
    core->moveToThread(threadController->backendThread());

    mockDevice = new MockCP16Legacy;

    connect(core, &Core::sgWriteToInterface, mockDevice, &AbstractMockDevice::writeToDevice);
    connect(mockDevice, &AbstractMockDevice::answerReady, core, &Core::parseInputData);

    connect(this, &LegacyDevice::deviceConnected, core, &Core::slInterfaceConnected);

    QSignalSpy spyCore(core, &Core::sgCurrentDeviceChanged);
    QSignalSpy spyMock(mockDevice, &AbstractMockDevice::answerReady);

    emit deviceConnected({MockCP16Legacy::mockName(), "virtual", DeviceConnectionType::Offline});

    TestUtils::copyDirectoryNested(":/resources/device_internal", mockDevice->basePath());

    // different threads. All signals catch with timeout
    QTRY_VERIFY_WITH_TIMEOUT(spyCore.count() > 0, 1000);
    QTRY_VERIFY_WITH_TIMEOUT(core->commAwaitsAnswer().size() == 0, 1000);
    QTRY_VERIFY_WITH_TIMEOUT(core->commAwaitsToSend().size() == 0, 1000);

    cpLegacy = dynamic_cast<CPLegacy*>(core->currentDevice);

    qInfo() << "TEST: Device instanciated and connected";
}

// вызывается перед каждой тестовой функцией
void LegacyDevice::init()
{

}

// вызывается после каждой тестовой функции
void LegacyDevice::cleanup()
{

}

void LegacyDevice::cleanupTestCase()
{
    QDir dir(mockDevice->basePath());
    // dir.removeRecursively();
}


//1 сохранённый К: только параметры, нет импульса В: только параметры, нет импульса, сохранение
//2	сохранённый К: только параметры, нет импульса В: только параметры, нет импульса, отмена
//3	сохранённый К: только параметры, нет импульса В: параметры и импульс, сохранение
//4	сохранённый К: только параметры, нет импульса В: параметры и импульс, отмена
//5	сохранённый К: параметры и импульс В: только параметры, нет импульса, сохранение
//6	сохранённый К: параметры и импульс В: только параметры, нет импульса, отмена
//8	сохранённый К: параметры и импульс В: параметры и импульс, отмена
//9	сохранённый К: только параметры, нет импульса В: только параметры, нет импульса, сохранение, переключение, Вставка, сохранение
//10 сохранённый К: только параметры, нет импульса В: только параметры, нет импульса, отмена, переключение, Вставка, сохранение
//11 сохранённый К: только параметры, нет импульса В: параметры и импульс, сохранение, переключение, Вставка, сохранение
//12 сохранённый К: только параметры, нет импульса В: параметры и импульс, отмена, переключение, Вставка, сохранение
//13 сохранённый К: параметры и импульс В: только параметры, нет импульса, сохранение, переключение, Вставка, сохранение
//14 сохранённый К: параметры и импульс В: только параметры, нет импульса, отмена, переключение, Вставка, сохранение
//15 сохранённый К: параметры и импульс В: параметры и импульс, сохранение, переключение, Вставка, сохранение
//16 сохранённый К: параметры и импульс В: параметры и импульс, отмена, переключение, Вставка, сохранение
void LegacyDevice::testCopyPasteSavedIR()
{
    //7	сохранённый К: параметры и импульс В: параметры и импульс, сохранение
    copyPaste(1, 1, 2, 2);
    // Сравнить данные в файле до сохранения(preview в MocK) и после сохранения(файл в Mock)
    //    имя файла надо бы сравнивать по rns до и после сохранения(вычитывать из IR модуля)
    QSignalSpy spyImpulseUploaded(cpLegacy, &CPLegacy::impulseUploaded);
    cpLegacy->saveChanges();
    thread()->yieldCurrentThread();
    QVERIFY(spyImpulseUploaded.wait(std::chrono::milliseconds{500}));

    // в Mock создавать временный файл-предпросмотр(в корне, например или пресете .tmp)
    // сравнить структуры пресета
}

void LegacyDevice::copyPaste(quint8 bankFrom, quint8 presetFrom, quint8 bankTo, quint8 presetTo)
{
    QSignalSpy spyPresetChange(cpLegacy, &CPLegacy::presetSwitched);

    cpLegacy->changePreset(bankFrom, presetFrom);
    thread()->yieldCurrentThread();
    QVERIFY(spyPresetChange.wait(std::chrono::milliseconds{1000}));

    QSignalSpy spyPresetCopied(cpLegacy, &CPLegacy::presetCopied);
    cpLegacy->copyPreset();
    thread()->yieldCurrentThread();
    QVERIFY(spyPresetCopied.wait(std::chrono::milliseconds{1000}));

    cpLegacy->changePreset(bankTo, presetTo);
    thread()->yieldCurrentThread();
    QVERIFY(spyPresetChange.wait(std::chrono::milliseconds{500}));

    QSignalSpy spyImpulseUploaded(cpLegacy, &CPLegacy::impulseUploaded);
    cpLegacy->pastePreset();
    thread()->yieldCurrentThread();
    QVERIFY(spyImpulseUploaded.wait(std::chrono::milliseconds{500}));
}



QTEST_GUILESS_MAIN(LegacyDevice)

#include "tst_legacydevice.moc"
