#ifndef ABSTRACTDEVICE_H
#define ABSTRACTDEVICE_H

#include <QObject>
#include <QtQmlIntegration/qqmlintegration.h>

#include "abstractmodule.h"
#include "devicetypeenum.h"
#include "deviceerrorenum.h"
#include "parser.h"

#include "firmware.h"

#include "moduleslistmodel.h"
#include "presetlistmodel.h"

#include "presetmanager.h"

class Core;

class AbstractDevice : public QObject
{
    Q_OBJECT

    Q_PROPERTY(quint8 bank READ bank NOTIFY bankPresetChanged FINAL)
    Q_PROPERTY(quint8 preset READ preset NOTIFY bankPresetChanged FINAL)
    Q_PROPERTY(quint8 maxBankCount READ maxBankCount CONSTANT)
    Q_PROPERTY(quint8 maxPresetCount READ maxPresetCount CONSTANT)

    Q_PROPERTY(PresetManager* presetManager READ presetManager CONSTANT)

    Q_PROPERTY(bool deviceParamsModified READ deviceParamsModified NOTIFY deviceParamsModifiedChanged FINAL)
    Q_PROPERTY(ModulesListModel* modulesListModel READ modulesListModel NOTIFY modulesListModelChanged FINAL)
    Q_PROPERTY(PresetListModel* presetListModel READ presetListModel NOTIFY presetListModelChanged FINAL)
public:
    enum DeviceClass
    {
        ABSTRACT = 0,
        CP_LEGACY,
        CP_MODERN
    };
    Q_ENUM(DeviceClass)

    explicit AbstractDevice(Core *owner);
    ~AbstractDevice();

    DeviceClass deviceClass() {return m_deviceClass;}

    virtual void initDevice(DeviceType deviceType);

    // TODO: пока умеет только Preset и bank!!!! Дописать
    Q_INVOKABLE virtual void restoreValue(QString name) {};  // restore parameter

    Q_INVOKABLE virtual void saveChanges() {};
    Q_INVOKABLE virtual void changePreset(quint8 newBank, quint8 newPreset, bool ignoreChanges = false) {};

    Q_INVOKABLE virtual void comparePreset() {};
    Q_INVOKABLE virtual void copyPreset() {};
    Q_INVOKABLE virtual void pastePreset() {};

    Q_INVOKABLE virtual void importPreset(QString filePath, QString fileName) {};
    Q_INVOKABLE virtual void exportPreset(QString filePath, QString fileName) {};

    virtual void setImpulse(QString filePath) {};

    quint8 bank() const {return m_bank;};
    quint8 preset() const {return m_preset;};
    quint8 maxBankCount() const {return m_maxBankCount;};
    quint8 maxPresetCount() const {return m_maxPresetCount;};

    bool deviceParamsModified() const;

    ModulesListModel* modulesListModel() {return &m_modulesListModel;};
    PresetListModel* presetListModel() {return &m_presetListModel;};
    PresetManager* presetManager() {return &m_presetManager;};

public slots:
    virtual QList<QByteArray> parseAnswers(QByteArray& baAnswer);

    void userModifiedModules();

signals:
    void sgDeviceInstanciated(DeviceType);
    void modulesListModelChanged();
    void presetListModelChanged();

    void sgDeviceError(DeviceError errorType, QString description = "", QVariantList params = {});

    void sgWriteToInterface(QByteArray data, bool logCommand = true);
    void sgPushCommandToQueue(QByteArray command, bool finalize = true);
    void sgProcessCommands();

    void sgModuleListUpdated(QList<AbstractModule*> modulesList);
    void sgModuleParametersUpdated(AbstractModule* module, quint16 moduleUid);

    void sgEnableTimeoutTimer();
    void sgDisableTimeoutTimer();

    void deviceUpdatingValues();
    void deviceParamsModifiedChanged();

    void bankPresetChanged();


protected:
    Parser m_parser;

    DeviceClass m_deviceClass;    
    DeviceType m_deviceType{DeviceType::UnknownDev};

    Firmware* m_actualFirmware{nullptr};
    Firmware* m_minimalFirmware{nullptr}; 
    QString m_firmwareName;

    quint8 m_bank;
    quint8 m_preset;
    quint8 m_maxBankCount{4};
    quint8 m_maxPresetCount{4};

    Preset currentPreset{this};
    Preset currentSavedPreset{this}; // TODO живёт в модели
    Preset copiedPreset{this};
    PresetManager m_presetManager{this};

    ModulesListModel m_modulesListModel;
    PresetListModel m_presetListModel;

    // TODO: m_presetsList живёт внутри модели или modulesList тут
    QList<Preset> m_presetsList;

    bool m_deviceParamsModified;

    void undefinedCommandCommHandler(const QString &command, const QByteArray& arguments);
    void modulesParamsSetCommHandler(const QString &command, const QByteArray& arguments);
};


#endif // ABSTRACTDEVICE_H