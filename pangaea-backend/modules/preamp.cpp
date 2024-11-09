#include "preamp.h"

Preamp::Preamp(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::PR, "PR", "pro"}
{
    m_volume = new ControlValue(this, "prv", "Volume");

    m_low = new ControlValue(this, "prl", "Low", "", -64, 63, 0, 127);
    m_mid = new ControlValue(this, "prm", "Mid", "", -64, 63, 0, 127);
    m_high = new ControlValue(this, "prh", "High", "", -64, 63, 0, 127);
}

void Preamp::setValues(bool enabled, quint8 volume, quint8 low, quint8 mid, quint8 high)
{
    m_moduleEnabled = enabled;
    m_volume->setControlValue(volume);

    m_low->setControlValue(low);
    m_mid->setControlValue(mid);
    m_high->setControlValue(high);

    emit dataChanged();
}

void Preamp::setValues(const preamp_data_t &preData)
{
    m_moduleEnabled = preData.on;
    m_volume->setControlValue(preData.volume);

    m_low->setControlValue(preData.low);
    m_mid->setControlValue(preData.mid);
    m_high->setControlValue(preData.high);

    emit dataChanged();
}
