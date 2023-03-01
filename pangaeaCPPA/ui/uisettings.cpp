#include "uisettings.h"

UiSettings::UiSettings(QObject *parent)
    : QObject{parent}
{
    appSettings = new QSettings(QSettings::UserScope);
}

void UiSettings::saveSetting(QString settingName, QVariant settingValue)
{
    appSettings->setValue(settingName, settingValue);
    appSettings->sync();

    emit sgUiSettingsChanged();
    qDebug() << __FUNCTION__ << "Setting name: " << settingName << "Setting value:" << settingValue;
}

bool UiSettings::autoConnectEnabled()
{
    return appSettings->value("autoconnect_enable", false).toBool();
}

bool UiSettings::checkUpdatesEnabled()
{
    return appSettings->value("check_updates_enable", false).toBool();
}

bool UiSettings::isModulesRightAligned()
{
    return appSettings->value("modules_right_aligned", true).toBool();
}
