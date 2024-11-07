#ifndef CABSIM_H
#define CABSIM_H

#include <QObject>
#include "abstractmodule.h"

class CabSim : public AbstractModule
{
    Q_OBJECT
    Q_PROPERTY(QString impulseName READ impulseName NOTIFY dataChanged FINAL)
public:
    CabSim(AbstractDevice *owner);
    QString impulseName() const;
    void setImpulseName(const QString& impulseName);

    void setEnabled(bool isEnabled);
private:
    QString m_impulseName{"FILENAME.WAV"};
};

#endif // CABSIM_H
