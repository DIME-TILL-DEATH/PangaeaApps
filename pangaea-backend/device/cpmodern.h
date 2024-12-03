#ifndef CPMODERN_H
#define CPMODERN_H

#include <QObject>
#include "abstractdevice.h"

#include <QSettings>

#include "irworker.h"

#include "irfile.h"
#include "presetmodern.h"

#include "presetvolume.h"
#include "compressor.h"
#include "noisegate.h"
#include "preamp.h"
#include "poweramp.h"
#include "cabsim.h"
#include "eqparametric.h"
#include "earlyreflections.h"


class CPModern : public AbstractDevice
{
    Q_OBJECT

    Q_PROPERTY(PresetVolume* MV READ getMV CONSTANT)
    Q_PROPERTY(QString currentPresetName READ currentPresetName WRITE setCurrentPresetName NOTIFY currentPresetNameChanged FINAL)

    Q_PROPERTY(IrFile currentIrFile READ currentIrFile WRITE setCurrentIrFile NOTIFY currentIrFileChanged FINAL)
    Q_PROPERTY(QList<IrFile> irsInLibrary READ irsInLibrary NOTIFY irsListChanged FINAL)
    Q_PROPERTY(QList<IrFile> irsInFolder READ irsInFolder NOTIFY irsListChanged FINAL)
public:
    CPModern(Core *parent);
    ~CPModern();

    void updateOutputModeNames() override;

    void initDevice(DeviceType deviceType) override;
    void readFullState() override;

    Q_INVOKABLE void saveChanges() override;
    Q_INVOKABLE void changePreset(quint8 newBank, quint8 newPreset, bool ignoreChanges = false) override;

    Q_INVOKABLE void comparePreset() override;
    Q_INVOKABLE void copyPreset() override;
    Q_INVOKABLE void pastePreset() override;
    Q_INVOKABLE void importPreset(QString filePath, QString fileName) override;
    Q_INVOKABLE void exportPreset(QString filePath, QString fileName) override;

    Q_INVOKABLE void restoreValue(QString name) override;

    void startIrUpload(QString srcFilePath, QString dstFilePath = "") override;
    Q_INVOKABLE void escImpulse(); // TODO где используется? В мобильном не нашёл вызовов из QML

    Q_INVOKABLE bool isFileInLibrary(const QString& fileName);
    Q_INVOKABLE bool isFileInFolder(const QString& fileName);

    PresetVolume* getMV() {return MV;};

    QString currentPresetName() const;;
    void setCurrentPresetName(const QString &newCurrentPresetName);

    Q_INVOKABLE void setFirmware(QString fullFilePath) override;
    Q_INVOKABLE void formatMemory() override;

    // avaliable modules
    // вынести создание наружу? DependencyContainer
    // public для тестов
    PresetVolume* MV;
    Compressor* CM;
    NoiseGate* NG;
    Preamp* PR;
    PowerAmp* PA;
    CabSim* IR;
    EqParametric* EQ;
    EarlyReflections* ER;

    void slIrEnabledChanged();


    QList<IrFile> irsInLibrary() const {return m_irsInLibrary;};
    QList<IrFile> irsInFolder() const {return m_irsInFolder;};

    IrFile currentIrFile() const {return actualPreset.irFile;};
    void setCurrentIrFile(const IrFile &newCurrentIrFile);

public slots:
    QList<QByteArray> parseAnswers(QByteArray& baAnswer) override;

signals:

    void currentPresetNameChanged();

    void irsListChanged();

    void currentIrFileChanged();

private:
    IRWorker irWorker;
    QSettings* appSettings;

    QList<PresetAbstract*> m_presetsList;
    PresetModern actualPreset{this};
    PresetModern savedPreset{this}; // TODO используется из листа
    PresetModern copiedPreset{this};

    QList<IrFile> m_irsInLibrary;
    QList<IrFile> m_irsInFolder;

    void setDeviceType(DeviceType newDeviceType);

    void pushReadPresetCommands();

    void recieveIrInfo(const QByteArray &data);
    void irDownloaded(const QString& irPath, const QByteArray &data);

    void setPresetData(const PresetModern &preset);
    void uploadIrData(const QString &irName, const QString &dstPath, const QByteArray& irData);
    void previewIr(const QByteArray& irData);

    void uploadFirmware(const QByteArray& fwData);

    void amtVerCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void irCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void getPresetListCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void listCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);

    void getBankPresetCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void getOutputModeCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void pnameCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void stateCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);

    void ackPresetChangeCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void ackPresetSavedCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void copyCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);

    QByteArray m_rawFirmwareData;
    const uint32_t fwUploadBlockSize = 100;
    void requestNextChunkCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void fwuFinishedCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void formatFinishedCommHandler(const QString &command, const QByteArray &argument, const QByteArray &data);
};

#endif // CPMODERN_H
