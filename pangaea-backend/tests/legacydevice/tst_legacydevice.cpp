#include <QtTest>
#include <QCoreApplication>

#include "threadcontroller.h"
#include "core.h"

#include "cplegacy.h"
#include "mockcp16legacy.h"

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

    void copyDirectoryNested(QString from, QString to);
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

    copyDirectoryNested(":/resources/device_internal", mockDevice->basePath());

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
    QSignalSpy spyPresetChange(cpLegacy, &CPLegacy::presetSwitched);

    cpLegacy->changePreset(1, 1);
    thread()->yieldCurrentThread();
    QVERIFY(spyPresetChange.wait(std::chrono::milliseconds{1000}));

    QSignalSpy spyPresetCopied(cpLegacy, &CPLegacy::presetCopied);
    cpLegacy->copyPreset();
    thread()->yieldCurrentThread();
    QVERIFY(spyPresetCopied.wait(std::chrono::milliseconds{1000}));

    cpLegacy->changePreset(2, 2);
    thread()->yieldCurrentThread();
    QVERIFY(spyPresetChange.wait(std::chrono::milliseconds{500}));


    QSignalSpy spyImpulseUploaded(cpLegacy, &CPLegacy::impulseUploaded);
    cpLegacy->pastePreset();
    thread()->yieldCurrentThread();
    QVERIFY(spyImpulseUploaded.wait(std::chrono::milliseconds{500}));

    cpLegacy->saveChanges();
    thread()->yieldCurrentThread();
    QVERIFY(spyImpulseUploaded.wait(std::chrono::milliseconds{500}));

    // Сравнить данные в файле до сохранения и после сохранения
    // в Mock создавать временный файл-предпросмотр(в корне, например или пресете .tmp)
    // сравнить структуры пресета
}

void LegacyDevice::copyDirectoryNested(QString from, QString to)
{

    QDirIterator it(from, QDirIterator::Subdirectories);

    while (it.hasNext()) {

        QString file_in = it.next();

        QFileInfo file_info = QFileInfo(file_in);

        QString file_out = file_in;
        file_out.replace(from,to);

        if(file_info.isFile())
        {
            //is file copy
            QFile::copy(file_in, file_out);
            QFile copiedFile(file_out);
            copiedFile.setPermissions(QFile::ReadOther | QFile::WriteOther);
        }

        if(file_info.isDir())
        {
            //dir mkdir
            QDir dir(file_out);
            if (!dir.exists())
                dir.mkpath(".");
        }
    }
}

QTEST_GUILESS_MAIN(LegacyDevice)

#include "tst_legacydevice.moc"
