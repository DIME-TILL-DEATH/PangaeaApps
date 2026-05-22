#ifndef CONTROLSPRESETFX_H
#define CONTROLSPRESETFX_H

#include <QQmlEngine>

#include "abstractmodule.h"
#include "controlvalue.h"


class ControlsPresetFx : public AbstractModule
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(ControlValue* presetVolume READ presetVolume NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* masterVolume READ masterVolume NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* phonesVolume READ phonesVolume NOTIFY dataChanged FINAL)

    Q_PROPERTY(ControlValue* presetCtrlVolume READ presetCtrlVolume NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* cntrlPcOut READ cntrlPcOut NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* cntrlSet READ cntrlSet NOTIFY dataChanged FINAL)
public:
    ControlsPresetFx(AbstractDevice *owner);

    ControlValue* presetVolume() { return m_presetVolume; }
    ControlValue* masterVolume() { return m_masterVolume; }
    ControlValue* phonesVolume() { return m_phonesVolume; }

    ControlValue* presetCtrlVolume() { return m_presetCtrlVolume; }
    ControlValue* cntrlPcOut() { return m_cntrlPcOut; }
    ControlValue* cntrlSet() { return m_cntrlSet; }

    void setMasterValues(quint8 masterVolume, quint8 phonesVolume);
    void setPresetValues(const preset_data_fx_t& presetData);
private:
    ControlValue* m_presetVolume = nullptr;
    ControlValue* m_masterVolume = nullptr;
    ControlValue* m_phonesVolume = nullptr;

    ControlValue* m_presetCtrlVolume = nullptr;
    ControlValue* m_cntrlPcOut = nullptr;
    ControlValue* m_cntrlSet = nullptr;
};

#endif // CONTROLSPRESETFX_H
