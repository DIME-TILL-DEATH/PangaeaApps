#ifndef PRESETMANAGER_H
#define PRESETMANAGER_H

#include <QObject>
#include <QStack>

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
    QStack<PresetState> m_stateStack;

};

#endif // PRESETMANAGER_H
