#ifndef ABSTRACTDEVICE_H
#define ABSTRACTDEVICE_H

#include <QObject>
#include <QtQmlIntegration/qqmlintegration.h>

#include "abstractmodule.h"
#include "devicetypeenum.h"
#include "parser.h"

#include "moduleslistmodel.h"

class Core;

class AbstractDevice : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool deviceParamsModified READ deviceParamsModified NOTIFY deviceParamsModifiedChanged FINAL)

public:
    enum DeviceClass
    {
        ABSTRACT = 0,
        CP_LEGACY
    };
    Q_ENUM(DeviceClass)

    explicit AbstractDevice(Core *owner);
    ~AbstractDevice();

    DeviceClass deviceClass() {return m_deviceClass;}

    virtual void initDevice();


    bool deviceParamsModified() const;

public slots:
    virtual void parseAnswers(QByteArray& baAnswer);

    void userModifiedModules();

signals:
    void sgDeviceInstanciated(DeviceType);

    void sgWriteToInterface(QByteArray data, bool logCommand = true);
    void sgPushCommandToQueue(QByteArray command, bool finalize = true);
    void sgProcessCommands();

    void sgModuleListUpdated(QList<AbstractModule*> modulesList);
    void sgModuleParametersUpdated(AbstractModule* module, quint16 moduleUid);

    void deviceUpdatingValues();
    void deviceParamsModifiedChanged();

protected:
    DeviceClass m_deviceClass;

    Parser m_parser;
    ModulesListModel m_modulesListModel;

    bool m_deviceParamsModified;
};


#endif // ABSTRACTDEVICE_H
