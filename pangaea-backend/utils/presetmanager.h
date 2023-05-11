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
        Preview, //нереализован(нужен ли?)
        Modified, //нереализован
        Pasting,
        Imported,
        Compare, // вызван оригинальный пресет
        Copying,
        Exporting,
        Saving,
        UploadingIr
    };
    Q_ENUM(PresetState)

    PresetState currentState() const;
    void setCurrentState(PresetState newCurrentState);

    void returnToPreviousState();
signals:

private:
    PresetState m_currentState{PresetState::Idle};
    PresetState m_previousState{PresetState::Idle};

};

typedef PresetManager::PresetState PresetState;

#endif // PRESETMANAGER_H
