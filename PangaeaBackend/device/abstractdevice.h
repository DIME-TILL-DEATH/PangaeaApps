#ifndef ABSTRACTDEVICE_H
#define ABSTRACTDEVICE_H

#include <QObject>
#include <QtQmlIntegration/qqmlintegration.h>

#include "abstractmodule.h"
#include "devicetypeenum.h"
#include "deviceerrorenum.h"
#include "devicemessageenum.h"
#include "parser.h"
#include "maskedparser.h"

#include "firmware.h"

#include "moduleslistmodel.h"
#include "presetlistmodel.h"

#include "presetmanager.h"

#include "presetvolume.h"

class Core;

class AbstractDevice : public QObject
{
    Q_OBJECT

    Q_PROPERTY(DeviceType deviceType READ deviceType CONSTANT)
    Q_PROPERTY(QString firmwareName READ firmwareName NOTIFY firmwareNameChanged FINAL)

    Q_PROPERTY(quint16 processingBudget READ processingBudget CONSTANT)
    Q_PROPERTY(quint16 processingUsed READ processingUsed NOTIFY processingUsedChanged)

    Q_PROPERTY(quint8 bank READ bank NOTIFY bankPresetChanged FINAL)
    Q_PROPERTY(quint8 preset READ preset NOTIFY bankPresetChanged FINAL)
    Q_PROPERTY(quint8 maxBankCount READ maxBankCount CONSTANT)
    Q_PROPERTY(quint8 maxPresetCount READ maxPresetCount CONSTANT)

    Q_PROPERTY(QStringList avaliableOutputModes READ avaliableOutputModes NOTIFY avaliableOutputModesChanged FINAL)
    Q_PROPERTY(quint8 outputMode READ outputMode WRITE setOutputMode NOTIFY outputModeChanged FINAL)

    Q_PROPERTY(PresetManager* presetManager READ presetManager CONSTANT)

    Q_PROPERTY(bool deviceParamsModified READ deviceParamsModified NOTIFY deviceParamsModifiedChanged FINAL)

    Q_PROPERTY(QList<QObject*> avaliableModulesList READ avaliableModulesList CONSTANT)
    Q_PROPERTY(ModulesListModel* modulesListModel READ modulesListModel NOTIFY modulesListModelChanged FINAL)
    Q_PROPERTY(PresetListModel* presetListModel READ presetListModel NOTIFY presetListModelChanged FINAL)

    Q_PROPERTY(PresetVolume* MV READ getMV CONSTANT)
public:
    explicit AbstractDevice(Core *owner);
    ~AbstractDevice();

    QString firmwareName() {return m_firmwareName;};

    quint16 processingBudget() {return m_processingBudget;};
    virtual quint16 processingUsed() {return 0;};

    QStringList avaliableOutputModes() {return m_avaliableOutputModes;};

    virtual void initDevice(DeviceType deviceType) {};
    virtual void readFullState() {};

    DeviceType deviceType() const {return m_deviceType;};

    quint8 outputMode() const {return m_outputMode;};
    void setOutputMode(quint8 newOutputMode);

    // TODO: пока умеет только Preset и bank!!!! Дописать
    Q_INVOKABLE virtual void restoreValue(QString name) {};  // restore parameter

    Q_INVOKABLE virtual void saveChanges() {};
    Q_INVOKABLE virtual void changePreset(quint8 newBank, quint8 newPreset, bool ignoreChanges = false) {};

    Q_INVOKABLE virtual void comparePreset() {};
    Q_INVOKABLE virtual void copyPreset() {};
    Q_INVOKABLE virtual void pastePreset() {};

    Q_INVOKABLE virtual void importPreset(QString filePath, QString fileName) {};
    Q_INVOKABLE virtual void exportPreset(QString filePath, QString fileName) {};

    Q_INVOKABLE virtual void previewIr(QString srcFilePath) {};
    Q_INVOKABLE virtual void startIrUpload(QString srcFilePath, QString dstFilePath = "", bool trimFile = false) {};
    Q_INVOKABLE virtual void setFirmware(QString fullFilePath) {};
    Q_INVOKABLE virtual void formatMemory() {};

    virtual void updateOutputModeNames() {};

    virtual quint64 maxIrSize() {return 0;};

    quint8 bank() const {return m_bank;};
    quint8 preset() const {return m_preset;};
    quint8 maxBankCount() const {return m_maxBankCount;};
    quint8 maxPresetCount() const {return m_maxPresetCount;};

    PresetVolume* MV;
    PresetVolume* getMV() {return MV;};

    bool deviceParamsModified() const;

    QList<QObject*> avaliableModulesList() {return m_avaliableModulesList;};
    ModulesListModel* modulesListModel() {return &m_modulesListModel;};
    PresetListModel* presetListModel() {return &m_presetListModel;};
    PresetManager* presetManager() {return &m_presetManager;};

    qint64 symbolsToRecieve() {return m_symbolsToRecieve;};

    bool isUpdatingFirmware() {return fwUpdate;};
    bool isMemoryFormatting() {return isFormatting;};

public slots:
    virtual QList<QByteArray> parseAnswers(QByteArray& baAnswer);

    void userModifiedModules();

signals:
    void sgDeviceInstanciated();

    void processingUsedChanged();

    void modulesListModelChanged();
    void presetListModelChanged();

    void avaliableOutputModesChanged();

    void sgDeviceError(DeviceErrorType errorType, QString description = "", QVariantList params = {});
    void sgDeviceMessage(DeviceMessageType msgType, QString description = "", QVariantList params = {});

    void sgWriteToInterface(QByteArray data, bool logCommand = true);
    void sgPushCommandToQueue(QByteArray command, bool finalize = true);
    void sgSendWithoutConfirmation(QByteArray data, qint32 symbolsToSend = -1, qint32 symbolsTorecieve = -1); // -1 don't update
    void sgProcessCommands();

    void sgModuleListUpdated(QList<AbstractModule*> modulesList);
    void sgModuleParametersUpdated(AbstractModule* module, quint16 moduleUid);

    void sgEnableTimeoutTimer();
    void sgDisableTimeoutTimer();

    void deviceUpdatingValues();
    void deviceRestoreValues();

    void firmwareNameChanged();
    void bankPresetChanged();
    void outputModeChanged();
    void deviceParamsModifiedChanged();

    void sgDisconnect();

// пока чтобы контролировать тестирование, но в будущем, возможно для работы(в modern)

    void presetSwitched();
    void presetCopied();
    void presetPasted();
    void presetSaved();
    void impulseUploaded();

protected:
    Parser m_parser;

    DeviceType m_deviceType{DeviceType::UNKNOWN_DEVICE};

    quint16 m_processingBudget{0};

    Firmware* m_actualFirmware{nullptr};
    Firmware* m_minimalFirmware{nullptr}; 
    QString m_firmwareName;

    QStringList m_avaliableOutputModes;
    quint8 m_outputMode;

    quint8 m_bank;
    quint8 m_preset;
    quint8 m_maxBankCount{4};
    quint8 m_maxPresetCount{4};

    PresetManager m_presetManager{this};

    PresetListModel m_presetListModel{this};

    QList<AbstractModule*> m_moduleList;
    QList<QObject*> m_avaliableModulesList;
    ModulesListModel m_modulesListModel{this};

    bool fwUpdate{false};
    bool isFormatting{false};

    bool m_deviceParamsModified{false};
    qint64 m_symbolsToRecieve{0};

    MaskedParser undefCommParser{"undefind command x\n", "11111111111111111X1"};

    void undefinedCommandCommHandler(const QString &command, const QByteArray& arguments);
    void modulesParamsSetCommHandler(const QString &command, const QByteArray& arguments);

};


#endif // ABSTRACTDEVICE_H
