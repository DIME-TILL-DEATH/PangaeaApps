#ifndef PRESETMANAGER_H
#define PRESETMANAGER_H

#include <QObject>


class PresetManager : public QObject
{
    Q_OBJECT
public:
    explicit PresetManager(QObject *parent = nullptr);

    enum PresetState
    {
        Unknown=0,
        Idle,
        Preview,
        Modified,
        Pasted,
        Imported,
        Compare, // вызван оригинальный пресет
        Copying,
        Exporting
    };

    PresetState currentState() const;
    void setCurrentState(PresetState newCurrentState);

    void returnToPreviousState();
signals:

private:
    PresetState m_currentState;
    PresetState m_previousState;

};

typedef PresetManager::PresetState PresetState;

#endif // PRESETMANAGER_H
