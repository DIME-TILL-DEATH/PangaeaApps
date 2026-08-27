#include <QMetaEnum>
#include <QDebug>

#include "presetmanager.h"

PresetManager::PresetManager(QObject *parent)
    : QObject{parent}
{

}

PresetState PresetManager::currentState() const
{
    if(m_stateStack.isEmpty()) return PresetState::Idle;
    else return m_stateStack.top();
}

void PresetManager::setCurrentState(PresetState newCurrentState)
{
    if(newCurrentState != m_stateStack.top())
    {
        m_stateStack.push(newCurrentState);

        QMetaEnum enumDescription = QMetaEnum::fromType<PresetState>();
        qDebug() << "Set new preset manager state:" << enumDescription.valueToKey(newCurrentState);
        emit currentStateChanged();
    }
}

void PresetManager::returnToPreviousState()
{
    PresetState state = m_stateStack.pop();

    QMetaEnum enumDescription = QMetaEnum::fromType<PresetState>();
    qDebug() << "Return to previous preset manager state:" << enumDescription.valueToKey(state);
    emit currentStateChanged();
}
