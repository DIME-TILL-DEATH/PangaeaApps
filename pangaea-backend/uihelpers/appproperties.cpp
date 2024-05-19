#include "appproperties.h"

AppProperties::AppProperties(QObject *parent)
    : QObject{parent}
{}

void AppProperties::copyPreset()
{
    emit sendAppAction(Core::AppAction::COPY_PRESET, {});
}

void AppProperties::pastePreset()
{
    emit sendAppAction(Core::AppAction::PASTE_PRESET, {});
}

void AppProperties::comparePreset()
{
    emit sendAppAction(Core::AppAction::COMPARE_PRESET, {});
}

void AppProperties::formatFlash()
{
    emit sendAppAction(Core::AppAction::FORMAT_FLASH, {});
}

void AppProperties::slSetAppParameter(Core::AppParameter appParameterType, QVariant content)
{
    switch (appParameterType)
    {
    case Core::AppParameter::COMPARE_STATE:
    {
        m_compareState = content.toBool();
        emit compareStateChanged();
        break;
    }
    default:
        break;
    }
}
