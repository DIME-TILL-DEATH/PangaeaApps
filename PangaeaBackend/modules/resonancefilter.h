#ifndef RESONANCEFILTER_H
#define RESONANCEFILTER_H

#include <QObject>
#include <QtQml/qqmlregistration.h>

#include "abstractmodule.h"
#include "controlvalue.h"

class ResonanceFilter : public AbstractModule
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(ControlValue* mix READ mix NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* fType READ fType NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* fMod READ fMod NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* lfo READ lfo NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* loFreq READ loFreq NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* hiFreq READ hiFreq NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* res READ res NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* dynTh READ dynTh NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* dynAtt READ dynAtt NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* dynRel READ dynRel NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* volume READ volume NOTIFY dataChanged FINAL)

    Q_PROPERTY(ControlValue* lfoType READ lfoType NOTIFY dataChanged FINAL)
public:
    ResonanceFilter(AbstractDevice *owner, modules_data_fx_t* rfData);

    void setValues(const modules_data_fx_t &rfData) override;

    ControlValue *mix() {return m_mix;};
    ControlValue *fType() {return m_fType;};
    ControlValue *fMod() {return m_fMod;};
    ControlValue *lfo() {return m_lfo;};
    ControlValue *loFreq() {return m_loFreq;};
    ControlValue *hiFreq() {return m_hiFreq;};
    ControlValue *res() {return m_res;};
    ControlValue *dynTh() {return m_dynTh;};
    ControlValue *dynAtt() {return m_dynAtt;};
    ControlValue *dynRel() {return m_dynRel;};
    ControlValue *volume() {return m_volume;};
    ControlValue *lfoType() {return m_lfoType;};

private:
    ControlValue *m_mix = nullptr;
    ControlValue *m_fType = nullptr;
    ControlValue *m_fMod = nullptr;
    ControlValue *m_lfo = nullptr;
    ControlValue *m_loFreq = nullptr;
    ControlValue *m_hiFreq = nullptr;
    ControlValue *m_res = nullptr;
    ControlValue *m_dynTh = nullptr;
    ControlValue *m_dynAtt = nullptr;
    ControlValue *m_dynRel = nullptr;
    ControlValue *m_volume = nullptr;
    ControlValue *m_lfoType = nullptr;
};

#endif // RESONANCEFILTER_H
