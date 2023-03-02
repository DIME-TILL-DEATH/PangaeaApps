#ifndef UIDESKTOPCORE_H
#define UIDESKTOPCORE_H

#include <QObject>
#include <QTimer>

#include "firmware.h"

//TODO: class enum UIValueItem, UITextItem, UIErrorItem
class UiDesktopCore : public QObject
{
    Q_OBJECT

public:
    explicit UiDesktopCore(QObject *parent = nullptr);

    Q_INVOKABLE void setParameter(QString name, quint8 val);
    Q_INVOKABLE void restoreParameter(QString name);

    Q_INVOKABLE void setImpuls(QString fullFilePath);
    Q_INVOKABLE void escImpuls();
    Q_INVOKABLE void convertAndUploadImpulse(QString fullFilePath);

    Q_INVOKABLE void readAllParameters();

    Q_INVOKABLE void exportPreset(QString filePath);
    Q_INVOKABLE void importPreset(QString filePath);

    Q_INVOKABLE void openManualExternally(QString fileName);
    Q_INVOKABLE void runIrConvertor();

signals:
    void sgSetUIParameter(QString nameParam, qint32 inValue);
    void sgSetUIText(QString nameParam, QString value);

    void sgPresetChangeStage (quint8 inChangePreset);
    void sgSetProgress(float val, QString extText);
    void sgDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

    void sgReadAllParameters();
    void sgSetParameter(QString name, quint8 value);
    void sgRestoreValue(QString name);
    void sgSetImpuls (QString filePath, QString fileName);
    void sgSetFirmware (QString fullFilePath);

    void sgExportPreset(QString filePath, QString fileName);
    void sgImportPreset(QString filePath, QString fileName);

    void sgEscImpuls();
    void sgSw4Enable();

    void sgDoOnlineFirmwareUpdate();

public slots:
    void slProposeNetFirmwareUpdate(Firmware* updateFirmware, Firmware* oldFirmware);
    void slProposeOfflineFirmwareUpdate(Firmware *minimalFirmware, Firmware *actualFirmware);
    void slNewAppVersionAvaliable(QString appVersion);
};

#endif // UIDESKTOPCORE_H
