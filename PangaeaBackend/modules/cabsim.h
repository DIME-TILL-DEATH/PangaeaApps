#ifndef CABSIM_H
#define CABSIM_H

#include "abstractmodule.h"

#include "controlvalue.h"

#include <QObject>
#include <QtQml/qqmlregistration.h>
class CabSim : public AbstractModule
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")
    Q_PROPERTY(QString impulseName READ impulseName NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* send READ send NOTIFY dataChanged FINAL)
public:
    CabSim(AbstractDevice *owner);
    QString impulseName() const;
    void setImpulseName(const QString& impulseName);

    void setEnabled(bool isEnabled);

    ControlValue *send() const {return m_send;};
    void setSendLevel(quint8 sendLevel);

private:
    QString m_impulseName{"FILENAME.WAV"};

    ControlValue *m_send = nullptr;
};

#endif // CABSIM_H
