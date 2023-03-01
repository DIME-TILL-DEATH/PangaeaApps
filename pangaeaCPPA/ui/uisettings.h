#ifndef UISETTINGS_H
#define UISETTINGS_H

#include <QObject>
#include <QSettings>

class UiSettings : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool autoConnectEnabled READ autoConnectEnabled NOTIFY sgUiSettingsChanged)
    Q_PROPERTY(bool checkUpdatesEnabled READ checkUpdatesEnabled NOTIFY sgUiSettingsChanged)
    Q_PROPERTY(bool isModulesRightAligned READ isModulesRightAligned NOTIFY sgUiSettingsChanged)
public:
    explicit UiSettings(QObject *parent = nullptr);

    Q_INVOKABLE void saveSetting(QString settingName, QVariant settingValue);

    bool autoConnectEnabled();
    bool checkUpdatesEnabled();
    bool isModulesRightAligned();
signals:
    void sgUiSettingsChanged();

private:
    QSettings* appSettings;
};

#endif // UISETTINGS_H
