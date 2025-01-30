#include <QMetaEnum>
#include <QDebug>

#include "presetmanager.h"

PresetManager::PresetManager(QObject *parent)
    : QObject{parent}
{

}

PresetState PresetManager::currentState() const
{
    return m_currentState;
}

void PresetManager::setCurrentState(PresetState newCurrentState)
{
    if(newCurrentState != m_currentState)
    {
        m_previousState = m_currentState;
        m_currentState = newCurrentState;

        QMetaEnum enumDescription = QMetaEnum::fromType<PresetState>();
        qDebug() << "Set new preset manager state:" << enumDescription.valueToKey(m_currentState);
        emit currentStateChanged();
    }
}

void PresetManager::returnToPreviousState()
{
    m_currentState = m_previousState;

    QMetaEnum enumDescription = QMetaEnum::fromType<PresetState>();
    qDebug() << "Return to previous preset manager state:" << enumDescription.valueToKey(m_currentState);
    emit currentStateChanged();
}
