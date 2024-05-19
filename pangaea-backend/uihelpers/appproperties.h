#ifndef APPPROPERTIES_H
#define APPPROPERTIES_H

#include <QObject>

#include "core.h"

class AppProperties : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool compareState READ compareState NOTIFY compareStateChanged FINAL)
public:
    explicit AppProperties(QObject *parent = nullptr);

    Q_INVOKABLE void copyPreset();
    Q_INVOKABLE void pastePreset();
    Q_INVOKABLE void comparePreset();

    Q_INVOKABLE void formatFlash();

    bool compareState() const {return m_compareState;};

signals:
    // void sgSetUiDeviceParameter(DeviceParameter::Type deviceParameterType, quint8 value);
    void sendAppAction(Core::AppAction appParameterType, QVariantList parameters);

    void compareStateChanged();

public slots:
    // void slSetUiDeviceParameter(DeviceParameter::Type deviceParameterType, qint32 value);
    void slSetAppParameter(Core::AppParameter appParameterType, QVariant content);

private:
    bool m_compareState{false};
};

#endif // APPPROPERTIES_H
