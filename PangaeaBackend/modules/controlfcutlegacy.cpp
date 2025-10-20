#include "controlfcutlegacy.h"


ControlFCutLegacy::ControlFCutLegacy(AbstractModule *parent, void *pointer, QString commandStr)
    : ControlValue{parent, pointer, "", "", "", 0, 127, 20, 2000}
{
    m_minControlValue = -60;
    m_maxControlValue = 60;
    m_minDisplayValue = 0.1;
    m_maxDisplayValue = 10;
    m_commandString;
    m_name;
}

void ControlFCutLegacy::setDisplayValue(double newDisplayValue)
{

}

void ControlFCutLegacy::setControlValue(qint32 value)
{

}
