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
    }
}

void PresetManager::returnToPreviousState()
{
    m_currentState = m_previousState;
}
