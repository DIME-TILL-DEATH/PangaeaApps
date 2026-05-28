#ifndef STEREOINPUTFX_H
#define STEREOINPUTFX_H

#include <QQmlEngine>

#include "abstractmodule.h"
#include "controlvalue.h"

class StereoInputFx : public AbstractModule
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(ControlValue* inlEnable READ inlEnable NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* inrEnable READ inrEnable NOTIFY dataChanged FINAL)

    Q_PROPERTY(ControlValue* inlPan READ inlPan NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* inrPan READ inrPan NOTIFY dataChanged FINAL)
public:
    explicit StereoInputFx(AbstractDevice *owner);

    void setValues(const modules_data_fx_t &inData) override;

    ControlValue* inlEnable() const { return m_inlEnable;};
    ControlValue* inrEnable() const { return m_inrEnable;};
    ControlValue* inlPan() const { return m_inlPan;};
    ControlValue* inrPan() const { return m_inrPan;};
private:
    ControlValue* m_inlEnable{nullptr};
    ControlValue* m_inrEnable{nullptr};
    ControlValue* m_inlPan{nullptr};
    ControlValue* m_inrPan{nullptr};
};

#endif // STEREOINPUTFX_H
