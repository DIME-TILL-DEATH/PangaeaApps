#ifndef CONTROLFCUTLEGACY_H
#define CONTROLFCUTLEGACY_H

#include <QObject>
#include <QQmlEngine>

#include "controlvalue.h"
#include "filtertypeenum.h"

class ControlFCutLegacy : public ControlValue
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")
public:
    ControlFCutLegacy(AbstractModule *parent, void* pointer, FilterType m_filterType);

    void setDisplayValue(double newDisplayValue) override;
    void setControlValue(qint32 value) override;

private:
    FilterType m_filterType;
};

#endif // CONTROLFCUTLEGACY_H
