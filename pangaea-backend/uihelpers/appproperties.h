#ifndef APPPROPERTIES_H
#define APPPROPERTIES_H

#include <QObject>

#include "core.h"

class AppProperties : public QObject
{
    Q_OBJECT
public:
    explicit AppProperties(QObject *parent = nullptr);

    Q_INVOKABLE void copyPreset();
    Q_INVOKABLE void pastePreset();
    Q_INVOKABLE void comparePreset();

    Q_INVOKABLE void formatFlash();

    // Q_INVOKABLE void openPresetList();

signals:
    // void sgSetUiDeviceParameter(DeviceParameter::Type deviceParameterType, quint8 value);
    void sendAppAction(Core::AppAction appParameterType, QVariantList parameters);

public slots:
    // void slSetUiDeviceParameter(DeviceParameter::Type deviceParameterType, qint32 value);
    void slSetAppParameter(Core::AppParameter appParameterType, QVariant content);

};

#endif // APPPROPERTIES_H
