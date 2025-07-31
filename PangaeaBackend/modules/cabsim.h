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
    CabSim(AbstractDevice *owner, preset_data_cplegacy_t* csData);
    CabSim(AbstractDevice *owner, preset_data_cpmodern_t* csData);

    QString impulseName() const;
    void setImpulseName(const QString& impulseName);

    void setEnabled(bool isEnabled) {*m_moduleEnabled = isEnabled;};

    void setValues(const preset_data_cplegacy_t& csData) override;
    void setValues(const preset_data_cpmodern_t& csData) override;

    ControlValue *send() const {return m_send;};


private:
    QString m_impulseName{"FILENAME.WAV"};

    ControlValue *m_send = nullptr;
};

#endif // CABSIM_H
