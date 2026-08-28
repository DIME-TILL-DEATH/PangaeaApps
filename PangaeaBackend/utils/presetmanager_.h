#ifndef PRESETMANAGER_H
#define PRESETMANAGER_H

#include <QObject>

#include "presetstateenum.h"

class PresetManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(PresetState currentState READ currentState NOTIFY currentStateChanged FINAL)
public:
    explicit PresetManager(QObject *parent = nullptr);

    PresetState currentState() const;
    void setCurrentState(PresetState newCurrentState);

    void returnToPreviousState();
signals:

    void currentStateChanged();
private:
    PresetState m_currentState{PresetState::Idle};
    PresetState m_previousState{PresetState::Idle};

};

#endif // PRESETMANAGER_H
