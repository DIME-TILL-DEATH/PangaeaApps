#include "stereoinputfx.h"

StereoInputFx::StereoInputFx(AbstractDevice *owner)
    : AbstractModule(owner, ModuleType::STEREO_INPUT, "", "")
{
    m_inlEnable = new ControlValue(this, nullptr, "inl_on", "IN L ON");
    m_inrEnable = new ControlValue(this, nullptr, "inr_on", "IN R ON");
    m_inlPan = new ControlValue(this, nullptr, "inl_pan", "IN L PAN", "", 0, 127, -63, 63);
    m_inrPan = new ControlValue(this, nullptr, "inr_pan", "IN R PAN", "", 127, 0, -63, 63);
}

void StereoInputFx::setValues(const modules_data_fx_t &inData)
{
    m_inlEnable->setControlValue(inData.in_left_en);
    m_inrEnable->setControlValue(inData.in_right_en);
    m_inlPan->setControlValue(inData.in_left_pan);
    m_inrPan->setControlValue(inData.in_right_pan);
}
